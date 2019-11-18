#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BUFFER;    
unsigned long   *turnoHOST= (unsigned long *) SEMAFORO;

unsigned VecinoMasCercano(unsigned dim, unsigned numPat, float **patrones, float *vector, float *distancia)
{
    unsigned i, j;
    unsigned pMin;
    float dMin, dist;

    dMin = (float) LONG_MAX;        /* INFINITO inicialmente */
    pMin = numPat + 1;
    for(j=0;j<numPat;j++) {
        dist = 0.0;
        for(i=0;i<dim;i++)
            dist += (patrones[j][i] - vector[i]) * (patrones[j][i] - vector[i]);

        dist = sqrt(dist);
        if(dist < dMin) {
            dMin = dist;
            pMin = j;
        }
    }
    *(distancia) = dMin;
    return(pMin);
}

void main()
{
        int i, j;
        unsigned dim, nPE, nPT, vec;
        float **patrones, **test, dist;

        /* Epera a que finalize el HOST */
        while(*turnoHOST);

        /* Obtener el Numero de Patrones y la Dimension de los mismos */
        nPE = (unsigned) bufflotantes[0];
        dim = (unsigned) bufflotantes[1];

        /* Reserva de Memoria para almacenar los Patrones de Entrenamiento */
        patrones = (float **)malloc(nPE * sizeof(float *));
        for(i=0;i<nPE;i++)
            patrones[i] = (float *)malloc((dim+1) * sizeof(float));

        /* Lectura de los patrones de la Mem. Compartida */
        for(j=0;j<nPE;j++)
            for(i=0;i<dim+1;i++)
                patrones[j][i] = bufflotantes[j*(dim+1)+i+2];

        *(turnoHOST) = 1;
        while(*(turnoHOST));

        /* Obtener el Numero de Patrones de test */
        nPT = (unsigned) bufflotantes[0];

        /* Reserva de Memoria para almacenar los Patrones de Test */
        test = (float **)malloc(nPT * sizeof(float *));
        for(i=0;i<nPT;i++)
            test[i] = (float *)malloc(dim * sizeof(float));

        /* Lectura de los patrones de la Mem. Compartida */
        for(j=0;j<nPT;j++)
            for(i=0;i<dim;i++)
                test[j][i] = bufflotantes[j*dim+i+1];

        /* Calculo de los Vecinos Mas Cercanos a cada patron de Test */
        for(i=0;i<nPT;i++) {
            vec = VecinoMasCercano(dim,nPE,patrones,test[i],&dist);
            bufflotantes[2*i] = (float)vec;
            bufflotantes[2*i+1] = dist;
        }

        *(turnoHOST) = 1;
        exit(0);
}

