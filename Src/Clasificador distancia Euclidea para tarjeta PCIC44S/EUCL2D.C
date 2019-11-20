#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BASE;
unsigned long   *semaforo2 = (unsigned long *) SEMAFORO2;


void main()
{
        int i, j, k;
        unsigned pMin;
        float dMin, dist, aux;
        unsigned dimension;
        unsigned numPEntrenamiento;
        unsigned numPTest;
        unsigned vecino;
        float **patrones, **test;
        float distancia;
        int desplazamiento;

        /****** Espera a que finalize el HOST ******/
        while( (*semaforo2) == ROJO );


        /***** Obtener el Numero de Patrones y la Dimension de los mismos ****/
        k = (int) bufflotantes[0];
        numPEntrenamiento = k - ((unsigned) (k/2));
        dimension = (unsigned) bufflotantes[1];

        /* Reserva de Memoria para almacenar los Patrones de Entrenamiento */
        patrones = (float **) malloc( numPEntrenamiento * sizeof(float *) );
        for( i=0 ; i<numPEntrenamiento ; i++ ) {
            patrones[i] = (float *) malloc( (dimension + 1) * sizeof(float) );
        } 

        /***** Lectura de los patrones de ENTRENAMIENTO      ******/
        /***** Tenemos que leer solo los que nos corresponden *****/

        desplazamiento = (k/2) * (dimension+1) + 2;

        for( j=0 ; j<numPEntrenamiento ; j++ ) {
            for( i=0 ; i<(dimension+1) ; i++ ) {
                patrones[j][i] = bufflotantes[ (j * (dimension+1)) + i + desplazamiento ];
            }
        }

        /* Fin de lectura de patrones de entrenamiento */
        (*semaforo2) = ROJO;

        /****** Espera a que finalize el HOST ******/
        while( (*semaforo2) == ROJO );

        /* Obtener el Numero de Patrones de test */
        numPTest = (unsigned) bufflotantes[0];

        /* Reserva de Memoria para almacenar los Patrones de Test */
        test = (float **) malloc( numPTest * sizeof(float *) );
        for(i=0;i<numPTest;i++) {
            test[i] = (float *) malloc( dimension * sizeof(float) );
        } 

        /******* Lectura de los patrones de TEST de la Mem. Compartida *******/
        for( j=0 ; j<numPTest ; j++ )
            for( i=0 ; i<dimension ; i++ )
                test[j][i] = bufflotantes[ (j * dimension) + i + 1 ];


        desplazamiento = 2 * numPTest;

        /********* Calculo del Vecino Mas Cercano al patron de Test *********/
        for ( i=0 ; i<numPTest ; i++ )
        {
            dMin = 10000000.0;        /* INFINITO inicialmente */
            pMin = numPEntrenamiento + 1;

            for(j=0;j<numPEntrenamiento;j++)
            {
                dist = 0.0;
                for(k=0;k<dimension;k++)
                {
                   aux = patrones[j][k] - test[i][k] ; 
                   dist += aux*aux;
                }
                dist = sqrt(dist);
                if( dist < dMin )
                {
                    dMin = dist;
                    pMin = j;
                }
            }
            vecino = pMin;
            distancia = dMin;

            bufflotantes[ 2 * i + desplazamiento ]   = (float) vecino;
            bufflotantes[ 2 * i + desplazamiento + 1 ] = distancia;

        }

        /* Fin del calculo de los vecimos mas cercanos */
        (*semaforo2) = ROJO;

        exit(0);
}

