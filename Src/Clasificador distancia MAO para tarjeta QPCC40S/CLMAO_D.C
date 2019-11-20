#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BUFFER;    
unsigned long   *turnoHOST= (unsigned long *) SEMAFORO;

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
            /*Producto escalar de cada elem. del vect. de entrada, con el de entrenamiento*/
            dist += (patrones[j][i] * vector[i]);

        if(dist > dMax) {
            dMax = dist;
            pMax = j;
        }
    }
    *(distancia) = dMax;
    return(pMax);
}

void main()
{
        int i, j;
        unsigned dim, nPE, nPT, vec;
        float **patrones, **test, dist;
        short NOMEM;

        /* Epera a que finalize el HOST */
        while(*turnoHOST);

        /* Obtener el Numero de Patrones y la Dimension de los mismos */
        nPE = (unsigned) bufflotantes[0];
        dim = (unsigned) bufflotantes[1];

        NOMEM = 0;
        /* Reserva de Memoria para almacenar los Patrones de Entrenamiento */
        patrones = (float **)malloc(nPE * sizeof(float *));
        if(patrones == NULL)
            NOMEM = 1;
        for(i=0;i<nPE;i++) {
            patrones[i] = (float *)malloc((dim+1) * sizeof(float));
            if(patrones[i] == NULL)
                NOMEM = 1;
        }

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
        if(test == NULL)
            NOMEM = 1;
        for(i=0;i<nPT;i++) {
            test[i] = (float *)malloc(dim * sizeof(float));
            if(test[i] == NULL)
                NOMEM = 1;
        }

        /* Lectura de los patrones de la Mem. Compartida */
        for(j=0;j<nPT;j++)
            for(i=0;i<dim;i++)
                test[j][i] = bufflotantes[j*dim+i+1];

        /* Calculo de los Vecinos Mas Cercanos a cada patron de Test */
        for(i=0;i<nPT;i++) {
            vec = VecinoMasCercanoMAO(dim,nPE,patrones,test[i],&dist);
            bufflotantes[2*i] = (float)vec;
            bufflotantes[2*i+1] = dist;
            if(NOMEM)
                bufflotantes[2*i+1] = -10000;
        }

        *(turnoHOST) = 1;
        exit(0);
}

