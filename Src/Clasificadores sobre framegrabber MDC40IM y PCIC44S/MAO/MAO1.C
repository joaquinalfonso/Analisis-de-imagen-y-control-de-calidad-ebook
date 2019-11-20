#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BASE;

unsigned long   *semaforo1 = (unsigned long *) SEMAFORO0;
unsigned long   *semaforo2 = (unsigned long *) SEMAFORO1;
unsigned long   *semaforo3 = (unsigned long *) SEMAFORO2;
unsigned long   *semaforo4 = (unsigned long *) SEMAFORO3;



/******** FUNCIONES AUXILIARES **********/

unsigned VecinoMasCercanoMAO(unsigned dim, unsigned numPat, float **patrones, float *vector, float *distancia)
{
    unsigned i, j;
    unsigned pMax;
    float dMax, dist;

    dMax = (float) LONG_MIN;        /* -INFINITO inicialmente */
    pMax = numPat + 1;

    for(j=0;j<numPat;j++) {
        dist = 0.0;
        for(i=0;i<dim;i++)
            /*Producto escalar de cada elem. del vect. de entrada, con el de entrenamiento */
            dist += (patrones[j][i] * vector[i]);

        if(dist > dMax) {
            dMax = dist;
            pMax = j;
        }
    }
    *(distancia) = dMax;
    return(pMax);
}





/************************** PRINCIPAL *************************/
void main()
{
        int i, j;
        unsigned dim, nPE, nPT, vec, vec2, k;
        float **patrones, *test, dist, dist2, etiq, etiq2;
        short NOMEM;
        int tamVX, tamVY, ancho, alto, res, numRes;
        long iniRes;
        float CM[MAX_PLANOS];
        unsigned pMax;
        float dMax;
        float memglobal[500];


        /****** Espera a que finalice el HOST ******/
        while( (*semaforo1) == ROJO );


        /***** Obtener el Numero de Patrones y la Dimension de los mismos ****/
        k   = (unsigned)bufflotantes[0];
        nPE = (unsigned) (k/2);
        dim = (unsigned) bufflotantes[1];

        /***** Dimensiones de la Imagen y las Ventanas */
        ancho = (int) bufflotantes[2];
        alto  = (int) bufflotantes[3];
        tamVX = (int) bufflotantes[4];
        tamVY = (int) bufflotantes[5];


        (*semaforo1) = ROJO;
        while((*semaforo1)==ROJO);

        /***** Lectura de los patrones de ENTRENAMIENTO      ******/
        for( j=0 ; j< nPE ; j++ )
        {
            while((*semaforo1)==ROJO);

            for( i=0 ; i<(int)(dim+1) ; i++ )
                    memglobal[ j * (dim+1) + i ] = (float)bufflotantes[ ((j%50)*(int)(dim+1)) +i ];

            if( (j%50) == 49)
                (*semaforo1) = ROJO;
        }

        /* Fin de lectura de patrones de entrenamiento */
        (*semaforo1) = ROJO;

/***************** COMIENZO CLASIFICACION ****************/
        numRes = (alto / tamVY) * (ancho / tamVX);
        iniRes = nPE * (dim+1);

        for( res=0 ; res<=numRes ; res++ )
        {

            /****** Espera a que se envie un CM ******/
            while( (*semaforo1) == ROJO );


            /* Lectura de los CM de la Mem. Compartida */
            for( i=0; i<dim ; i++ )
                CM[i] = bufflotantes[i];


            /*************** Clasificador ****************/
            dMax = -10000000.0;
            pMax = nPE + 1;

            for(j=0; j<nPE; j++)
            {
                dist = 0.0;
                for(k=0;k<dim;k++)
                    dist += memglobal[ ( j * (dim+1) + k) ] * CM[k];

                if( dist > dMax )
                {
                   dMax = dist;
                   pMax = j;
                }

            }
            vec = pMax;
            dist = dMax;
            etiq = memglobal[ vec * (dim + 1) + dim ];

            /*Esperar resultados del Procesador 2*/
            while( (*semaforo2) == VERDE );


            /* Obtengo el resultado del procesador 2 */
            vec2=(unsigned)bufflotantes[3]+nPE;
            dist2= bufflotantes[4];
            etiq2=bufflotantes[5];

            if ( dist > dist2 ) /*Si gana proc1.*/
            {
                 bufflotantes[0] = vec;
                 bufflotantes[1] = dist;
                 bufflotantes[2] = etiq;
            }
            else
            {
                bufflotantes[0]=(float)vec2;
                bufflotantes[1]=dist2;
                bufflotantes[2]=etiq2;
            }

            /*El resultado final lo envio al HOST*/

            (*semaforo1) = ROJO;    /*Fin de MAO1*/
        }

        while ((*semaforo1)==ROJO);
        exit(0);
}

