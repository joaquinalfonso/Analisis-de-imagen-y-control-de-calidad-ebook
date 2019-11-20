#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
 
#include "dirs.h"
#include "qpcc40s.c"
#include "bmp.c"


extern void CerrarYSalir(int);
extern void PonerValorSemaforo(int, ULONG );
extern ULONG ObtenerValorSemaforo(int );
extern void InicializarPCIC44(char [], char []);
extern void EscribirBloqueFlotantes(ULONG , ULONG , float *);
extern void EscribirBloqueEnteros(ULONG , ULONG , ULONG *);
extern void LeerBloqueEnteros(ULONG , ULONG , ULONG *);
extern void LeerBloqueFlotantes(ULONG , ULONG , float *);


void LecturaParametros(int, char **);
int CargarParametrosCM(void);
void ObtenerTamanyoImagenBMP(void);
void ObtenerPatEntrenamiento(char *);
void ObtenerVentana(int []);


FILE *fbmp;
FILE *fparam;
FILE *fentr;

BMPHEADER cabeceraBMP;
unsigned long Ventana[2];
unsigned long NumPlanos;
unsigned long *LimitePlanos;
unsigned long Piezas[5];
unsigned long anchoBMP, altoBMP;
unsigned char V[MAX_VENTANA][MAX_VENTANA];
unsigned long buffer[1024];
float patrones[ 50*MAX_PLANOS ];




/*--------------------- FUNCIONES AUXILIARES --------------------------*/

int
CargarParametrosCM()
{
        printf("\nPaso de Parametros del C.M.\n");

        Escribir_palabras_32(BUFFER, 2, Ventana);
        Escribir_palabras_32(BUFFER+2,1,&NumPlanos);
        Escribir_palabras_32(BUFFER+3,(UINT) NumPlanos+1,LimitePlanos);
        Escribir_palabras_32(BUFFER+NumPlanos+4,5,Piezas);

        SetSemaforo(VERDE);

        EsperarSemaforo(ROJO);

		return(0);
}



void LecturaParametros(int argc, char **argv)
{
    int i;

    if(argc != 4){
        fprintf(stderr,"Error: USO: fgpci imagen.bmp parametros fich_entren\n");
        exit(-1);
    }

    if( ( fbmp = fopen( argv[1], "rb" ) ) == NULL )
	{
	    fprintf(stderr,"No se puede Abrir el Fichero BMP: %s\n",argv[1]);
	    exit(-1);
	}

    if((fparam=fopen(argv[2], "r"))==NULL) {
        fprintf(stderr, "No puedo abrir el fichero de parametros.\n");
        exit(-1);
    }

                        /*VENTANA Y NUMPLANOS*/
    fscanf(fparam,"%ld %ld %ld", &Ventana[0], &Ventana[1], &NumPlanos);
    LimitePlanos = (unsigned long *) malloc(sizeof(unsigned long)*((int) NumPlanos+1));
    if( LimitePlanos == NULL ) {
		fprintf(stderr,"Error en la reserva de memoria.\n");
		exit(-1);
    }

    printf("Tama¤o de Ventana %ld,%ld\n",Ventana[0], Ventana[1]);
    printf("Planos: %ld\n",NumPlanos);


                        /*LIMITE PLANOS*/
    printf("Limites:\n");
    for (i=0 ; i<(int)(NumPlanos+1) ; i++ )
    {
           fscanf(fparam,"%ld",&LimitePlanos[i]);
           printf("%ld ",LimitePlanos[i]);
    }


                        /*PIEZAS*/
    printf("\nPiezas\n");
    for(i=0 ; i<MAX_PIEZAS ; i++ )
    {
    	fscanf(fparam,"%ld",&Piezas[i]);
        printf("%ld ",Piezas[i]);
    }
    printf("\n");


    fclose(fparam);
    return;
}



void ObtenerTamanyoImagenBMP()
{

    ReadBmpHeader(&cabeceraBMP, fbmp);

	/* Ir al comienzo de los datos */
	fseek(fbmp, cabeceraBMP.Header.Offset, SEEK_SET);

	/* Obtener los tama¤os de la imagen */
    anchoBMP = (unsigned long) cabeceraBMP.WinInfo.Width;
    altoBMP  = (unsigned long) cabeceraBMP.WinInfo.Height;

    printf("Ancho: %ld - Alto: %ld\n", anchoBMP, altoBMP);
}




/*Paso de Patrones de Entrenamiento a la PCI */
void ObtenerPatEntrenamiento(char *nomfich)
{
        float dato, V[4];
        int i, j;
        unsigned dim, nPEnt;
        /*float patrones[ 50*MAX_PLANOS ];*/

        if((fentr=fopen(nomfich, "r"))==NULL) {
            fprintf(stderr, "No puedo abrir el fichero de entrenamiento.\n");
        	exit(-1);
	    }
    
        /*Recoger datos del fichero de entrenamiento y pasarselos al DSP*/
        fscanf(fentr,"%d",&nPEnt);
        fscanf(fentr,"%d",&dim);

        fprintf(stdout,"Patrones de ENTRENAMIENTO.\n");
        fprintf(stdout,"Num. Patrones: %d\n",nPEnt);
        fprintf(stdout,"Dimension: %d\n",dim);

        if(dim!=NumPlanos)
        {
            fprintf(stderr, "Dimension de planos de fich. entren. distinta del de parametros.\n");
        	exit(-1);
	    }


        /* ESCRITURA DE PATRONES DE ENTRENAMIENTO EN DPRAM */

        /* Para PCI */
        dato = (float) nPEnt;
        EscribirBloqueFlotantes( BASE, 1, &dato);

        dato = (float) dim;
        EscribirBloqueFlotantes( (BASE + 1), 1, &dato );

        V[0] = (float)anchoBMP;
        V[1] = (float)altoBMP;
        V[2] = (float)Ventana[0];
        V[3] = (float)Ventana[1];
        EscribirBloqueFlotantes( (BASE + 2), 4, V );



        /*Activacion de Semaforos*/
        PonerValorSemaforo( 0, VERDE );
        PonerValorSemaforo( 1, VERDE );
        while( ObtenerValorSemaforo( 0 ) == VERDE );
        while( ObtenerValorSemaforo( 1 ) == VERDE );

        /*Pasar los patrones de Entrenamiento primero a un proc. y luego al
        otro.*/


        /*************** PASO AL PROC.1 ****************/

        for(j=0; j<(int)nPEnt/2; j++)
        {
            for(i=0;i<(int)(dim+1);i++)
            {
                fscanf(fentr,"%f",&dato);
                patrones[ (j%50)*(dim+1)+i ] = dato;
            }

            if(j%50==49)
            {
                EscribirBloqueFlotantes( BASE, 50*(dim+1), patrones );
                PonerValorSemaforo( 0, VERDE );
                while( ObtenerValorSemaforo( 0 ) == VERDE );
            }
        }
        EscribirBloqueFlotantes( BASE, 50 * (dim+1), patrones );
        PonerValorSemaforo( 0, VERDE );
        while( ObtenerValorSemaforo( 0 ) == VERDE);



        /********PASO AL PROC.2**********/
        for(j=(int)nPEnt/2; j<(int)nPEnt; j++)
        {
            for(i=0;i<(int)(dim+1);i++)
            {
                fscanf(fentr,"%f",&dato);
                patrones[ ((j-(nPEnt/2))%50) * ( dim + 1 ) + i ] = dato;
            }
            if( ((j-(nPEnt/2))%50) == 49 )
            {
                EscribirBloqueFlotantes( BASE, 50 * (dim+1), patrones );
                PonerValorSemaforo( 1, VERDE );
                while( ObtenerValorSemaforo( 1 ) == VERDE );
            }
        }

        EscribirBloqueFlotantes( BASE, 50 * (dim+1), patrones );
        PonerValorSemaforo( 1, VERDE );
        while( ObtenerValorSemaforo( 1 ) == VERDE);

        fclose(fentr);


        return;
}



void ObtenerVentana(int Limites[])
{
    unsigned long dato;
    int indice, x, y;

    LeerVentana(cabeceraBMP, fbmp, Limites, V);
    for(y=0; y<Ventana[1]; y++)
        for(x=0; x<Ventana[0]; x+=4)
        {
            dato=(unsigned long) V[y][x] << 24;
            dato |= (unsigned long) V[y][x+1] << 16;
            dato |= (unsigned long) V[y][x+2] <<  8;
            dato |= (unsigned long) V[y][x+3];
            indice=(int)Ventana[0]*y/4 +x/4;
            buffer[indice]=dato;
        }

    Escribir_palabras_32(BUFFER, (UINT) (Ventana[0]*Ventana[1]/4), buffer);
    return;
}


/******************************* PRINCIPAL ***********************************/

int main(int argc, char **argv)
{

    int i,j,k, Lim[4], cont;
    float resul[4], cm[MAX_PLANOS];

    clock_t tIni, tFin;
    float tiempo;

    LecturaParametros(argc, argv);
    InicializarDSP("fgcmeuc.out");
    InicializarPCIC44("euc1.out", "euc2.out" );
    CargarParametrosCM();


    ObtenerTamanyoImagenBMP();
    ObtenerPatEntrenamiento(argv[3]);

    /* INICIO DEL FUNCIONAMIENTO DE LOS DSPS */
    /* CALCULO COEFICIENTE MORFOLOGICO Y CLASIFICACION */

    cont=0;
    tIni = clock();
    Lim[2]=(int)Ventana[1]; /*Alto*/
    Lim[3]=(int)Ventana[0]; /*Ancho*/
    for(i=0; i< (int) altoBMP;)
    {
        Lim[0]=i;
        for(j=0; j< (int) anchoBMP; j+=(int)Ventana[0])
        {
            Lim[1]=j;

            /* A partir de la imagen se obtiene la ventana a procesar ahora */
            ObtenerVentana(Lim);
            SetSemaforo(VERDE); /*Comienzo algoritmo*/
            EsperarSemaforo(ROJO);  /*Calculado CM*/

            Leer_flotantes_32(BUFFER,(UINT) NumPlanos,cm);
/*
            for(k=0; k<NumPlanos; k++)
                    printf("%f ", cm[k]);
            printf("\n");
*/
            /*Escribir CM a la PCI*/
            EscribirBloqueFlotantes(BASE, NumPlanos, cm);
            PonerValorSemaforo( 0, VERDE );
            PonerValorSemaforo( 1, VERDE );
            while( ObtenerValorSemaforo( 0 ) == VERDE ||
                   ObtenerValorSemaforo( 1 ) == VERDE );

            LeerBloqueFlotantes(BASE, 3, resul);
            printf("%03d Vec: %02d  Dist: %f  Etiq: %d\n", cont, (int) resul[0], resul[1], (int) resul[2]);
            cont++;
        }
        i+=(int)Ventana[1];
    }
    tFin = clock();
    tiempo = (float)((tFin-tIni)/CLOCKS_PER_SEC);
    printf("Tiempo de Proceso: %f\n", tiempo);

    /*Terminar la ejecucion de los DSPS*/
    SetSemaforo(VERDE);
    PonerValorSemaforo( 0, VERDE );
    PonerValorSemaforo( 1, VERDE );

    return 0;
}
