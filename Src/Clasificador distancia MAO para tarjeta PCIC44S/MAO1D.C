#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BASE;
unsigned long   *semaforo1 = (unsigned long *) SEMAFORO1;
unsigned long   *semaforo2 = (unsigned long *) SEMAFORO2;


void main()
{
        int i, j, k;
        unsigned pMax;
        float dMax, dist,aux;
        unsigned dimension;
        unsigned numPEntrenamiento;
        unsigned numPTest;
        unsigned vecino1, vecino2;
        float **patrones, **test;
        float distancia1, distancia2;

        /****** Espera a que finalize el HOST ******/
        while( (*semaforo1) == ROJO );


        /***** Obtener el Numero de Patrones y la Dimension de los mismos ****/
        k = (int) bufflotantes[0];
        numPEntrenamiento = (unsigned) (k/2);
        dimension = (unsigned) bufflotantes[1];

        /* Reserva de Memoria para almacenar los Patrones de Entrenamiento */
        patrones = (float **) malloc( numPEntrenamiento * sizeof(float *) );
        for( i=0 ; i<numPEntrenamiento ; i++ ) {
            patrones[i] = (float *) malloc( (dimension + 1) * sizeof(float) );
        } 

        /***** Lectura de los patrones de ENTRENAMIENTO      ******/
        /***** Tenemos que leer solo los que nos corresponden *****/

        for( j=0 ; j<numPEntrenamiento ; j++ ) {
            for( i=0 ; i<(dimension+1) ; i++ ) {
                patrones[j][i] = bufflotantes[ (j * (dimension+1)) + i + 2 ];
            }
        }

        /* Fin de lectura de patrones de entrenamiento */
        (*semaforo1) = ROJO;

        /****** Espera a que finalize el HOST ******/
        while( (*semaforo1) == ROJO );

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


        /********* Calculo del Vecino Mas Cercano al patron de Test *********/
        for ( i=0 ; i<numPTest ; i++ )
        {
            dMax = -10000000.0;        /* INFINITO inicialmente */
            pMax = numPEntrenamiento + 1;

            for(j=0;j<numPEntrenamiento;j++)
            {
                dist = 0.0;
                for(k=0;k<dimension;k++)
                {
                   dist += patrones[j][k] * test[i][k] ; 
                }
                if( dist > dMax )
                {
                    dMax = dist;
                    pMax = j;
                }
            }
            vecino1 = pMax;
            distancia1 = dMax;
                             
            bufflotantes[ 2 * i ]   = (float) vecino1;
            bufflotantes[ 2 * i + 1 ] = distancia1;

        }

        while( (*semaforo2) == VERDE );

        for( i=0 ; i<numPTest ; i++ ) {
            distancia1 = bufflotantes[ 2 * i + 1 ];
            vecino2 = bufflotantes[ 2 * (numPTest + i) ];
            distancia2 = bufflotantes[ 2 * (numPTest + i) + 1 ];
            if ( distancia2 > distancia1 ) {
                bufflotantes[ 2 * i ]   = (float) vecino2 + numPEntrenamiento;
                bufflotantes[ 2 * i + 1 ] = distancia2;
            }
        }

        /* Fin del calculo de los vecimos mas cercanos */
        (*semaforo1) = ROJO;

        exit(0);
}

