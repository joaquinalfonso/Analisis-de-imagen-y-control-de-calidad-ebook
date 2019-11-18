#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BASE;

unsigned long   *semaforo1 = (unsigned long *) SEMAFORO0;
unsigned long   *semaforo2 = (unsigned long *) SEMAFORO1;
unsigned long   *semaforo3 = (unsigned long *) SEMAFORO2;
unsigned long   *semaforo4 = (unsigned long *) SEMAFORO3;



/************************* PRINCIPAL *************************/

void main()
{
        int i, j, despla;
        unsigned dim, nPE, nPT, vec, k;
        float **patrones, *test, dist;
        short NOMEM;
        int tamVX, tamVY, ancho, alto, res, numRes;
        long iniRes;
        float CM[MAX_PLANOS], etiq;
        unsigned pMax;
        float dMax;
        float memglobal[500];

        
        /* Espera a que finalize el HOST */
        while( (*semaforo2) == ROJO );


        /* Obtener el Numero de Patrones y la Dimension de los mismos */
        k=bufflotantes[0];
        nPE = (unsigned) (k-(k/2));
        dim = (unsigned) bufflotantes[1];

        /***** Dimensiones de la Imagen y las Ventanas */
        ancho = (int) bufflotantes[2];
        alto  = (int) bufflotantes[3];
        tamVX = (int) bufflotantes[4];
        tamVY = (int) bufflotantes[5];

        (*semaforo2) = ROJO;
        while( (*semaforo2) == ROJO );
        


        /***** Lectura de los patrones de ENTRENAMIENTO      ******/
        for( j=k/2 ; j<k; j++ ) {
            while( (*semaforo2) == ROJO );
               for( i=0 ; i<(dim+1) ; i++ )
                   memglobal[ ( (j-k/2)  * (int)(dim+1) ) + i ] = (float)bufflotantes[ (((j-k/2)%50)*(int)(dim+1))+i ];


            if( ((j-k/2)%50) == 49)
                (*semaforo2) = ROJO;
        }

        /* Fin de lectura de patrones de entrenamiento */
        (*semaforo2) = ROJO;


/***************** COMIENZO CLASIFICACION ****************/
        numRes = (alto / tamVY) * (ancho / tamVX);
        iniRes = nPE * (dim+1);

        for( res=0 ; res<=numRes ; res++ )
        {

            /****** Espera a que se envie un CM ******/
            while( (*semaforo2) == ROJO );


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


            bufflotantes[3] = (float) vec;
            bufflotantes[4] = dist;
            bufflotantes[5] = etiq;

            (*semaforo2) = ROJO;

        }

        while((*semaforo2) == ROJO);
        exit(0);
}
