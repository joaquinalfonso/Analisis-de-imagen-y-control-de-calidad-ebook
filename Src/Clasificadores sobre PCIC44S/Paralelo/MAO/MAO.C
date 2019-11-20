#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BASE;
/*float           *memglobal =    (float *) MEM_GLOBAL;*/

unsigned long   *SemPPAL = (unsigned long *) SEMAFORO1;

unsigned long   *SemDSP1 = (unsigned long *) SEMAFORO3;
unsigned long   *SemRSLT = (unsigned long *) SEMAFORO7;


void main()
{
        int i, j, k, p;
        unsigned pMax;
        float dist, dMax;
        unsigned dimension;
        unsigned numPEntrenamiento;
        unsigned vecino;
        float CM[MAX_PLANOS];
        float distancia;
        float etiq;
        int tamVX, tamVY, ancho, alto, res, numRes;
        long iniRes;
        float memglobal[500], result[300];

        /****** Espera a que finalice el HOST ******/
        while( (*SemPPAL) == ROJO );


        /***** Obtener el Numero de Patrones y la Dimension de los mismos ****/
        numPEntrenamiento = (int) bufflotantes[0];
        dimension = (unsigned) bufflotantes[1];

        /***** Dimensiones de la Imagen y las Ventanas */
        ancho = (int) bufflotantes[2];
        alto  = (int) bufflotantes[3];
        tamVX = (int) bufflotantes[4];
        tamVY = (int) bufflotantes[5];

        (*SemPPAL) = ROJO;
        while ((*SemPPAL) == ROJO);

        /***** Lectura de los patrones de ENTRENAMIENTO      ******/
        for( j=0 ; j<numPEntrenamiento ; j++ ) {
            while( (*SemPPAL) == ROJO );
            for( i=0 ; i<(dimension+1) ; i++ )
               memglobal[ (j * (dimension+1)) + i ] = bufflotantes[ ((j%50) * (dimension+1)) + i ];

            if( (j%50) == 49)
                (*SemPPAL) = ROJO;
        }

        /* Fin de lectura de patrones de entrenamiento */
        (*SemPPAL) = ROJO;

/************************** Fin del Control del Host ************************/

        numRes = (alto / tamVY) * (ancho / tamVX);
        iniRes = numPEntrenamiento * (dimension+1);

        for( res=0 ; res<numRes ; res++ )
        {
            while( (*SemDSP1) == VERDE );

            /* Lectura de los CM de la Mem. Compartida */
            for( i=0; i<dimension ; i++ )
                CM[i] = bufflotantes[i];
            
            (*SemDSP1) = VERDE;

            /*************** Clasificador ****************/
            dMax = -10000000.0;
            pMax = numPEntrenamiento + 1;

            for(j=0; j<numPEntrenamiento; j++)
            {
                dist = 0.0;
                for(k=0;k<dimension;k++)
                    dist += memglobal[ ( j * (dimension+1) + k) ] * CM[k];

                if( dist > dMax )
                {
                   dMax = dist;
                   pMax = j;
                }

            }
            vecino = pMax;
            distancia = dMax;

            etiq = memglobal[ vecino * (dimension + 1) + dimension ];

            /* Escribir datos */
            /*memglobal[ iniRes + (3 * res) ] = (float) vecino;
            memglobal[ iniRes + (3 * res) + 1 ] = distancia;
            memglobal[ iniRes + (3 * res) + 2 ] = etiq;*/

            result[3*res]=(float)vecino;
            result[3*res+1]=distancia;
            result[3*res+2]=etiq;

        }

        (*SemPPAL) = ROJO;

        for( res=0 ; res<numRes ; res++ ) {
            for( k=0 ; k<3 ; k++ )
                /*bufflotantes[ (3 * (res % 50)) + k] = memglobal[ iniRes + (3 * res) + k];*/
                bufflotantes[ (3 * (res % 50)) + k] = result[ (3 * res) + k];
            if( (res % 50) == 49 ) {
                (*SemRSLT) = VERDE;
                while( (*SemRSLT) == VERDE );
            }
        }
        (*SemRSLT) = VERDE;

        exit(0);
}

