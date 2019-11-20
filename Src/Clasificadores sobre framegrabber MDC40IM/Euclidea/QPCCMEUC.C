#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
 
#include "dirs.h"
#include "qpcc40s.c"
#include "bmp.c"


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

/*--------------------- FUNCIONES AUXILIARES --------------------------*/

int
CargarParametrosCM()
{
        printf("\nPaso de Parametros del C.M.\n");

        Escribir_palabras_32(BUFFER, 2, Ventana);
        Escribir_palabras_32(BUFFER+2,1,&NumPlanos);
        Escribir_palabras_32(BUFFER+3, (UINT)NumPlanos+1,LimitePlanos);
        Escribir_palabras_32(BUFFER+NumPlanos+4,5,Piezas);

        SetSemaforo(VERDE);

        EsperarSemaforo(ROJO);

		return(0);
}



void LecturaParametros(int argc, char **argv)
{
    int i;

    if(argc != 4){
        fprintf(stderr,"Error: USO: qpccmeuc imagen.bmp parametros fich_entren \n");
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
    for(i=0 ; i<(int)MAX_PIEZAS ; i++ )
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
    return;
}



void ObtenerPatEntrenamiento(char *nomfich)
{
        float dato;
        int i, j;
        int dim, nPEnt;
        float patrones[ 50*MAX_PLANOS ];

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
        dato = (float)nPEnt;
        Escribir_flotantes_32(BUFFER,1,&dato);
        dato = (float)dim;
        Escribir_flotantes_32(BUFFER+1,1,&dato);
        SetSemaforo(VERDE);
        EsperarSemaforo(ROJO);


        for(j=0;j<(int)nPEnt;j++) {
            for(i=0;i< (int) (dim+1);i++) {
                fscanf(fentr,"%f",&dato);
                patrones[ (j%50) * (int)( dim + 1 ) + i ] = dato;
            }
            if( (j%50) == 49 )
            {
                Escribir_flotantes_32( BUFFER, 50 * (dim+1), patrones );
                SetSemaforo(VERDE);
                EsperarSemaforo(ROJO);
            }
        }

        Escribir_flotantes_32( BUFFER, (nPEnt%50) * (dim+1), patrones );
        SetSemaforo(VERDE);
        EsperarSemaforo(ROJO);

        fclose(fentr);

        return;
}




void ObtenerVentana(int Limites[])
{
    /*unsigned char V[MAX_VENTANA][MAX_VENTANA];*/
    /*unsigned long buffer[1024];*/
    unsigned long dato;
    int indice, x, y;

    LeerVentana(cabeceraBMP, fbmp, Limites, V);
    for(y=0; y<(int)Ventana[1]; y++)
    {
        for(x=0; x<(int)Ventana[0]; x+=4)
        {
            dato=(unsigned long) V[y][x] << 24;
            dato |= (unsigned long) V[y][x+1] << 16;
            dato |= (unsigned long) V[y][x+2] <<  8;
            dato |= (unsigned long) V[y][x+3];
            indice=(int)(Ventana[0]*y/4 + x/4);
            buffer[indice]=dato;
        }
    }
    Escribir_palabras_32(BUFFER, (UINT) (Ventana[1]*Ventana[0]/4), buffer);

}


/******************************* PRINCIPAL ***********************************/

int main(int argc, char **argv)
{

    int i,j, k, Lim[4], cont;
    float cm[MAX_PLANOS];
    float resul[4];

    clock_t tIni, tFin;
    float tiempo;

    LecturaParametros(argc, argv);
    InicializarDSP("cmeuc.out");
    CargarParametrosCM();

    ObtenerTamanyoImagenBMP();


    ObtenerPatEntrenamiento(argv[3]);


    /* INICIO DEL FUNCIONAMIENTO DEL DSP FG */
    /* CALCULO COEFICIENTE MORFOLOGICO Y CLASIFICACION */

    cont=0;
    tIni = clock();
    Lim[2]=(int)Ventana[1]; /*Alto*/
    Lim[3]=(int)Ventana[0]; /*Ancho*/
    for(i=0; i<(int)altoBMP;)
    {
        Lim[0]=i;
        for(j=0; j<(int)anchoBMP; j+=(int)Ventana[0])
        {
            Lim[1]=j;

            /* A partir de la imagen se obtiene la ventana a procesar ahora */
            ObtenerVentana(Lim);
            SetSemaforo(VERDE); /*Comienzo CM*/
            EsperarSemaforo(ROJO);  /*Calculado CM*/
            Leer_flotantes_32( BUFFER, (UINT)NumPlanos, cm );

/*
            for(k=0; k<NumPlanos; k++)
                    printf("%f ", cm[k]);
            printf("\n");
*/
            /*Dar permiso para que clasifique*/
            SetSemaforo(VERDE); /*Comienzo clasificacion*/
            EsperarSemaforo(ROJO);  /*Calculado clasificacion*/

            Leer_flotantes_32(BUFFER,3,resul);
            printf("%03d Vec: %2d  Dist: %f  Etiq: %d\n", cont, (int)resul[0], resul[1], (int)resul[2]);
            cont++;


        }
        i+=(int)Ventana[1];
    }
    tFin = clock();
    tiempo = (float) ((tFin-tIni)/CLOCKS_PER_SEC);
    printf("Tiempo de Proceso: %f\n",tiempo);

    /*Terminar la ejecucion del DSP*/
    SetSemaforo(VERDE);

    return 0;
}
