#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "dirs.h"

float           *bufflotantes = (float *) BASE;
unsigned long   *bufenteros =   (unsigned long *) BASE;

unsigned long   *SemPPAL = (unsigned long *) SEMAFORO0;

unsigned long   *SemHOST = (unsigned long *) SEMAFORO2;
unsigned long   *SemDSP1 = (unsigned long *) SEMAFORO3;
unsigned char imagen[MAX_VENTANA][MAX_VENTANA];

void main()
{
    int i, j, k, x, y, numRes;
    int alto, ancho;
	int xVen, yVen;
    int numplanos;
    int LimitePlano[MAX_PLANOS];
    int pieza[MAX_PIEZAS];
    float CM[MAX_PLANOS];
	int numVentanas, ventana[4];
    short pl, pi;
    short numPiezas;
    short cabe;
    int totalPiezas[MAX_PIEZAS];
    int X, Y, XFinal, YFinal;
    short color;
    float vnorm;
    int indice;


/********************** Espera a que finalize el HOST ***********************/
    while( (*SemPPAL) == ROJO );

/********************** Lectura de parametros para el Coef. Morf ************/

    xVen = bufenteros[0];
	yVen = bufenteros[1];
	numplanos = bufenteros[2];

	for(x=0; x<numplanos+1; x++)
		   LimitePlano[x] = bufenteros[x+3];

    for(x=0; x<MAX_PIEZAS; x++)
		   pieza[x]=bufenteros[numplanos+x+4];

    (*SemPPAL) = ROJO;

/********************* Lectura del Tamanyo de la Imagen BMP *******************/
    while( (*SemPPAL) == ROJO );

    ancho = (unsigned) bufenteros[0];
    alto  = (unsigned) bufenteros[1];

    (*SemPPAL) = ROJO;

/************************** Fin del Control del Host ************************/


    numRes = (alto / yVen) * (ancho / xVen);

    for( j=0 ; j<numRes ; j++ ) {

        while( (*SemHOST) == VERDE );

        /* Lectura de la ventana a procesar */
        for( y=0 ; y<yVen ; y++ ) {
            for( x=0 ; x<xVen ; x+=4 ) {
                indice = (y*xVen/4) + x/4;
                imagen[y][x]   = (unsigned char) (bufenteros[indice] >> 24);
                imagen[y][x+1] = (unsigned char) ((bufenteros[indice] & 0x00ff0000) >> 16);
                imagen[y][x+2] = (unsigned char) ((bufenteros[indice] & 0x0000ff00) >> 8);
                imagen[y][x+3] = (unsigned char) (bufenteros[indice]  & 0x000000ff);
            }
        }
/*****************************************************************************/
/*                CoeficienteMorfologico                                     */
/*****************************************************************************/

        vnorm=0.0;
        numPiezas = 0;
        for(pi=0;pi<MAX_PIEZAS;pi++)
            if(pieza[pi])
                numPiezas++;

        for(pl=0;pl<numplanos;pl++) {
            for(pi=0;pi<5;pi++)
                totalPiezas[pi] = 0;

            for(pi=0;pi<MAX_PIEZAS;pi++) {
                if(pieza[pi]) {
                    for(Y=0;Y<yVen;Y+=(pi+1)) {
                        for(X=0;X<xVen;X+=(pi+1)) {
                            for(y=0;y<(pi+1);y++)
                            {
                                for(x=0;x<(pi+1);x++)
                                {
                                    if( (X+x) >= xVen || (Y+y) >= yVen)
                                        color = -1;
                                    else
                                        color = imagen[Y+y][X+x];
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

        vnorm=sqrt(vnorm);
        for(k=0; k<numplanos; k++)
            CM[k] = CM[k] / vnorm;

        for(k=0; k<numplanos; k++)
            bufflotantes[k] = CM[k];

        (*SemDSP1) = ROJO;
        while( (*SemDSP1) == ROJO );

        (*SemHOST) = VERDE;

    }

    exit(0);
}

