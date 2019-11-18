#include <stdlib.h>
#include <math.h>

#include "dirs.h"
#include "video.h"


#define TRUE 1
#define FALSE 0

#define TAMX    512
#define TAMY    512

unsigned long   *bufenteros   = (unsigned long *) BUFFER;
float           *bufflotantes = (float *) BUFFER;
unsigned long   *Semaforo= (unsigned long *) SEMAFORO;
unsigned char imagen[MAX_VENTANA][MAX_VENTANA];


/*int paleta[256];                    /* Storage for IMAGE ADC palette  */                                                                                  /* Localizacion de la variable cerrojo (sincronizacion) */


/*---------------------- FUNCIONES AUXILIARES --------------------*/
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



/************************************************************************/

void main()
{

        int x,y,i,j;
        int ancho, alto, xVen, yVen, indice;
        int *LimitePlano, numplanos, pieza[5];
        float *CM;
        int NumVentanas, ventana[4];
        unsigned nPEnt, dim, vec;
        unsigned pl, pi;
        unsigned numPiezas;
        int cabe;
        unsigned totalPiezas[5];
        unsigned X, Y, XFinal, YFinal;
        int color;
        float vnorm;


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
        xVen = (int)bufenteros[0];       /*Columnas*/
        yVen = (int)bufenteros[1];       /*Filas*/
        numplanos = (int)bufenteros[2];
        LimitePlano =(int *)malloc( sizeof(int)*numplanos+1 );
        for(x=0; x<(int)(numplanos+1); x++)
                   LimitePlano[x] = (int)bufenteros[x+3];

        for(x=0; x<MAX_PIEZAS; x++)
                   pieza[x]=(int)bufenteros[numplanos+x+4];

        SetSemaforo(ROJO);



        /*  OBTENCION DE LA VENTANA A PROCESAR */
        CM = (float *)malloc( sizeof(float)*numplanos );
        NumVentanas = (TAMX / xVen) * (TAMY / yVen);

        for(j=0; j<=NumVentanas; j++)
        {
            EsperarSem(VERDE);

            /*Lectura de la ventana a procesar*/
            for(y=0; y<yVen; y++)
                for(x=0; x<xVen; x+=4)
                {
                    indice=(y*xVen/4) + x/4;
                    imagen[y][x] = (unsigned char) (bufenteros[indice] >> 24);
                    imagen[y][x+1] = (unsigned char) ((bufenteros[indice] & 0x00ff0000) >> 16);
                    imagen[y][x+2] = (unsigned char) ((bufenteros[indice] & 0x0000ff00) >>  8);
                    imagen[y][x+3] = (unsigned char) (bufenteros[indice] & 0x000000ff);
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
                                            color = (int) imagen[Y+y][X+x];
                                        cabe = ((int)color > (int)LimitePlano[pl]) && ((int)color <= (int)LimitePlano[pl+1]);
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
        }

        free(CM);
        EsperarSem(VERDE);


        exit(0);

}
