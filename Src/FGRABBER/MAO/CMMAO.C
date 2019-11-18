#include <stdlib.h>
#include <math.h>

#include "dirs.h"
#include "video.h"

#define TRUE 1
#define FALSE 0

#define TAMX    512
#define TAMY    512

float           *bufflotantes = (float *) BUFFER;
unsigned long   *bufenteros   = (unsigned long *) BUFFER;
unsigned long   *Semaforo     = (unsigned long *) SEMAFORO;

unsigned char ventana[MAX_VENTANA][MAX_VENTANA];

/*int paleta[256];                    /* Storage for IMAGE ADC palette  */                                                                                  /* Localizacion de la variable cerrojo (sincronizacion) */





/*------------------------ FUNCIONES AUXILIARES ------------------------*/
void SetSemaforo (unsigned long temp)
{
    (*Semaforo)=temp;
    return;
}


void EsperarSem(unsigned long temp)
{

    switch(temp)
    {
        case ROJO  :
            while( (*Semaforo) == VERDE );
            break;

        case VERDE :
            while( (*Semaforo) == ROJO );
            break;
    }
    return;
}

unsigned VecinoMasCercanoMAO(unsigned dim, unsigned numPat, float **patrones, float *vector, float *distancia)
{
    unsigned i, j;
    unsigned pMax;
    float dMax, dist;

    dMax = -100000000.0;        /* -INFINITO inicialmente */
    pMax = numPat + 1;

    for(j=0;j<numPat;j++)
    {
        dist = 0.0;
        for(i=0;i<dim;i++)
            dist += (patrones[j][i] * vector[i]);

        if(dist > dMax) {
            dMax = dist;
            pMax = j;
        }
    }
    *(distancia) = dMax;
    return(pMax);
}




/************************************************************************/

void main()
{

        int i,j;
        int ancho, alto, xVen, yVen;
        int color;
        int *LimitePlano, numplanos, pieza[5];
        float **pEntr, dist, *CM;
        int NumVentanas;
        unsigned numPiezas, pi, pl;
        unsigned nPEnt, dim, vec;
        float vnorm;
        int cabe;
        unsigned totalPiezas[5];
        unsigned X, Y, x, y;


        /* Preparar paleta de Tonos de Gris */
        /*for(i = 0; i < 256; i++)        paleta[i] = i;*/

        /* Configurar el modo de trabajo de la framegraber */
        /*video_setup(&ccir_esync);*/

        /* Set up the screen origin to the IMAGE page */
        /*video_set_screen_base(IMAGE);*/

        /* Configurar la paleta de Entrada */
        /*video_set_adc_lut(paleta);*/

        /* Clear the image to transparent. */
        /*video_clear_image();*/

        /* Espera a que finalice el HOST */
        /*El paro se produce en la inicializacion del DSP*/
        EsperarSem(VERDE);




        /* Lectura de parametros para el Coef. Morf */
        xVen = (int)bufenteros[0];
        yVen = (int)bufenteros[1];
        numplanos = (int)bufenteros[2];
        LimitePlano = (int *)malloc( sizeof(int)*(numplanos+1));
        for(x=0; x<(numplanos+1); x++)
                   LimitePlano[x] = (int)bufenteros[x+3];

        for(x=0; x<(int)MAX_PIEZAS; x++)
                   pieza[x]=(int)bufenteros[numplanos+x+4];

        SetSemaforo(ROJO);
        EsperarSem(VERDE);


        /* Obtener el Numero de Patrones Entr. y la Dimension de los mismos */
        nPEnt =(unsigned)  bufflotantes[0];
        dim =  (unsigned) bufflotantes[1];



        /* Reserva de Memoria para almacenar los Patrones de Entrenamiento */
        pEntr = (float **)malloc((int)nPEnt * sizeof(float *));
        for(i=0;i<(int)nPEnt;i++)
            pEntr[i] = (float *)malloc((int)(dim+1) * sizeof(float));


        SetSemaforo(ROJO);
       
        /***** Lectura de los patrones de ENTRENAMIENTO      ******/
        for( j=0 ; j<(int)nPEnt; j++ )
        {
            EsperarSem(VERDE);
            for( i=0 ; i<(int)(dim+1) ; i++ )
               pEntr[j][i] = bufflotantes[ ((j%50) * (int)(dim+1)) + i ];

            if( (j%50) == 49)
                SetSemaforo(ROJO);
        }

        /* Fin de lectura de patrones de entrenamiento */
        SetSemaforo(ROJO);


        /*OBTENCION VENTANA A PROCESAR*/

        CM = (float *)malloc( sizeof(float)*(int)numplanos );
        NumVentanas = (TAMX / xVen) * (TAMY / yVen);

        for(j=0; j<=NumVentanas; j++)
        {
            EsperarSem(VERDE);

            /*Lectura de la ventana a procesar*/
            for(y=0; y<yVen;)
            {
                for(x=0; x<xVen;)
                {
                    i=(y*xVen/4) + x/4;
                    ventana[y][x] = (unsigned char) (bufenteros[i] >> 24);
                    ventana[y][x+1] = (unsigned char) ((bufenteros[i] & 0x00ff0000) >> 16);
                    ventana[y][x+2] = (unsigned char) ((bufenteros[i] & 0x0000ff00) >>  8);
                    ventana[y][x+3] = (unsigned char) ( bufenteros[i]  &  0x000000ff);
                    x=4+x;
                }
                y++;
            }
/************************* COEFICIENTE MORFOLOGICO *************************/
            vnorm=0.0;
            numPiezas = 0;
            for(pi=0;pi<MAX_PIEZAS;pi++) if(pieza[pi])  numPiezas++;

            /* Para cada uno de los planos de la imagen */
            for(pl=0;pl<numplanos;pl++) {

                /* Inicializamos el numero de piezas que caben de cada tamanyo */
                for(pi=0;pi<MAX_PIEZAS;pi++) totalPiezas[pi] = 0;

                /* Para cada tamanyo de pieza elegido */
                for(pi=0;pi<MAX_PIEZAS;pi++) {
                    if(pieza[pi]) {

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
                                            color = ventana[Y+y][X+x];
                                        cabe = (color > LimitePlano[pl]) && (color <= LimitePlano[pl+1]);
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
            for(x=0; x<numplanos; x++)
                CM[x] = CM[x] / vnorm;


            /*ENVIO DEL CM AL HOST*/
                
            for(i=0; i<numplanos; i++)
                     bufflotantes[i]=CM[i];

            SetSemaforo(ROJO);

            /*CLASIFICACION DEL CM OBTENIDO*/
            EsperarSem(VERDE);
            vec = VecinoMasCercanoMAO(dim,nPEnt,pEntr,CM,&dist);

            bufflotantes[0] =   (float)vec;
            bufflotantes[1] =   dist;
            bufflotantes[2] =   pEntr[vec][dim];

            SetSemaforo(ROJO);


        }

        EsperarSem(VERDE);
        exit(0);

}
