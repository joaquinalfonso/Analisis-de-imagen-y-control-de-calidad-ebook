#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
 
#include "dirs.h"
#include "bmp.c"

#define TAMX 512
#define TAMY 512

void LecturaParametros(int, char **);
void ObtenerTamanyoImagenBMP(void);
void ObtenerPatEntrenamiento(char *);
void ObtenerVentana(int []);
unsigned VecinoMasCercanoMAO(float *vector, float *distancia);
void CMyClasif(void);


FILE *fbmp;
FILE *fparam;
FILE *fentr;

BMPHEADER cabeceraBMP;
unsigned long Ventana[2];
unsigned long NumPlanos;
unsigned long *LimitePlanos;
unsigned long Piezas[5];
unsigned long anchoBMP, altoBMP;
unsigned dim, nPEnt;
float **pEntr;
unsigned char V[MAX_VENTANA][MAX_VENTANA];
unsigned long contventana;
unsigned long yVen, xVen;


/*--------------------- FUNCIONES AUXILIARES --------------------------*/

void LecturaParametros(int argc, char **argv)
{
    int i;

    if(argc != 4){
        fprintf(stderr,"Error: USO: host imagen.bmp parametros fich_entren \n");
        exit(-1);
    }

    contventana=-1;

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
    yVen=Ventana[1];
    xVen=Ventana[0];

    LimitePlanos = (unsigned long *) malloc(sizeof(unsigned long)*((int) NumPlanos+1));
    if( LimitePlanos == NULL ) {
		fprintf(stderr,"Error en la reserva de memoria.\n");
		exit(-1);
    }

    printf("Tama¤o de Ventana %ld,%ld\n",Ventana[0], Ventana[1]);
    printf("Planos: %ld\n",NumPlanos);


                        /*LIMITE PLANOS*/
    printf("Limites:\n");
    for (i=0 ; i<NumPlanos+1 ; i++ )
    {
           fscanf(fparam,"%ld",&LimitePlanos[i]);
           printf("%ld ",LimitePlanos[i]);
    }


                        /*PIEZAS*/
    printf("\nPiezas\n");
    for(i=0 ; i<5 ; i++ )
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



void ObtenerPatEntrenamiento(char *nomfich)
{
        float dato;
        int i, j;

        if((fentr=fopen(nomfich, "r"))==NULL) {
            fprintf(stderr, "No puedo abrir el fichero de entrenamiento.\n");
        	exit(-1);
	    }


        /*Recoger datos del fichero de entrenamiento*/
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

        /* Reserva de Memoria para almacenar los Patrones de Entrenamiento */
        pEntr = (float **)malloc(nPEnt * sizeof(float *));
        for(i=0;i<nPEnt;i++)
            pEntr[i] = (float *)malloc((dim+1) * sizeof(float));


        for(j=0;j< (int) nPEnt;j++)
            for(i=0;i< (int) dim+1;i++) {
                fscanf(fentr,"%f",&dato);
                pEntr[j][i] = dato;
            }

        fclose(fentr);
        return;
}




void ObtenerVentana(int Limites[])
{
    int i,j;

    LeerVentana(cabeceraBMP, fbmp, Limites, V);
    return;
}



unsigned VecinoMasCercanoMAO(float *vector, float *distancia)
{
    unsigned i, j;
    unsigned pMax;
    float dMax, dist;

    dMax = -10000.0;        /* INFINITO inicialmente */
    pMax = nPEnt + 1;
    for(j=0;j<nPEnt;j++)
    {
        dist = 0.0;
        for(i=0;i<dim;i++)
            dist += pEntr[j][i] * vector[i];

        if(dist > dMax)
        {
            dMax = dist;
            pMax = j;
        }
    }
    *(distancia) = dMax;
    return(pMax);
}



/****************** CMorfologico y Clasificador ***************/
void CMyClasif(void)
{
        int i,j;
        int color;
        int numPiezas;
        float CM[MAX_PLANOS], dist;
        unsigned pi, pl;
        unsigned vec;
        float vnorm;
        int cabe;
        float totalPiezas[5];
        unsigned X, Y, x, y;

        contventana++;

        vnorm=0.0;
        numPiezas = 0;
        for(pi=0;pi<MAX_PIEZAS;pi++) if(Piezas[pi])  numPiezas++;

        /* Para cada uno de los planos de la imagen */
        for(pl=0;pl<NumPlanos;pl++) {

        /* Inicializamos el numero de piezas que caben de cada tamanyo */
                for(pi=0;pi<MAX_PIEZAS;pi++) totalPiezas[pi] = 0.0;

                /* Para cada tamanyo de pieza elegido */
                for(pi=0;pi<MAX_PIEZAS;pi++) {
                    if(Piezas[pi]) {

                        for(Y=0;Y<yVen;Y+=(pi+1)) {
                            for(X=0;X<xVen;X+=(pi+1)) {
                                /* Comprobar si cabe la pieza */
                                for(y=0;y<pi+1;y++)
                                {
                                    for(x=0;x<pi+1;x++)
                                    {
                                        if( (X+x) >= xVen || (Y+y) >= yVen)
                                            color = -1;
                                        else
                                            color = (int)V[Y+y][X+x];

                                        cabe = ((int)color > (int)LimitePlanos[pl]) && ((int)color <= (int)LimitePlanos[pl+1]);

                                        if(!cabe)
                                            break;
                                    }
                                    if(!cabe)
                                        break;
                                }
                                if(cabe)
                                    totalPiezas[pi]++;
                            }
                        }
                    }
                }
                for(pi=1;pi<MAX_PIEZAS;pi++)
                    totalPiezas[0] += totalPiezas[pi];


                CM[pl] = (float)totalPiezas[0] / numPiezas;
                vnorm += CM[pl]*CM[pl];

        }

        vnorm = sqrt(vnorm);
        /*Normalizacion de valores entre 0 y 1*/
        for(x=0; x<NumPlanos; x++)
              CM[x] = CM[x] / vnorm;


        /*for(j=0; j<NumPlanos; j++)
                    printf("%f ", CM[j]);
        printf("\n");*/

        /*CLASIFICACION DEL CM OBTENIDO*/
        vec = VecinoMasCercanoMAO(CM,&dist);
        printf("%03ld Vec: %03d  Dist: %f  Etiq: %d\n", contventana, vec, dist, (int)pEntr[vec][dim]);

        return;
}




/******************************* PRINCIPAL ***********************************/

int main(int argc, char **argv)
{

    int j,i, Lim[4];

    clock_t tIni, tFin;
    float tiempo;

    LecturaParametros(argc, argv);
    ObtenerTamanyoImagenBMP();
    ObtenerPatEntrenamiento(argv[3]);


    /* CALCULO COEFICIENTE MORFOLOGICO Y CLASIFICACION */

    tIni=clock();
    Lim[2]=(int)Ventana[1]; /*Alto*/
    Lim[3]=(int)Ventana[0]; /*Ancho*/
    for(i=0; i < (int) altoBMP;)
    {
        Lim[0]=i;
        for(j=0; j< (int) anchoBMP; j+=(int)Ventana[0])
        {
            Lim[1]=j;
            ObtenerVentana(Lim);

            CMyClasif();
        }
        i+=(int)Ventana[1];
    }
    tFin=clock();
    tiempo=(float)((tFin-tIni)/CLOCKS_PER_SEC);
    printf("Tiempo de proceso : %f\n", tiempo);
    return 0;
}
