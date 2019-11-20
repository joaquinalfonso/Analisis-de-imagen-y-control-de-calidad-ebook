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
unsigned long   *turnoHOST= (unsigned long *) SEMAFORO;

int paleta[256];                    /* Storage for IMAGE ADC palette  */                                                                                  /* Localizacion de la variable cerrojo (sincronizacion) */

void CoeficienteMorfologico(int vent[], int numPlanos, int limPlanos[], int piezas[], float CM[])
{
    unsigned pl, pi;
    unsigned numPiezas;
    short cabe;
	unsigned totalPiezas[5];
    unsigned X, Y, XFinal, YFinal, x, y;
	short color;
    int alto, ancho;
    float vnorm;

    video_get_size(&ancho,&alto);

    vnorm=0.0;
    numPiezas = 0;
	for(pi=0;pi<5;pi++)	if(piezas[pi])	numPiezas++;

	/* Para cada uno de los planos de la imagen */	
	for(pl=0;pl<numPlanos;pl++) {
		
		/* Inicializamos el numero de piezas que caben de cada tamanyo */
		for(pi=0;pi<5;pi++)	totalPiezas[pi] = 0;

		/* Para cada tamanyo de pieza elegido */
		for(pi=0;pi<5;pi++) {
			if(piezas[pi]) {
				
                /* Recorremos toda la ventana */
                YFinal = vent[1] + vent[3];
                XFinal = vent[0] + vent[2];
                for(Y=vent[1];Y<YFinal;Y+=(pi+1)) {
                    for(X=vent[0];X<XFinal;X+=(pi+1)) {
						/* Comprobar si cabe la pieza */
						for(y=0;y<pi+1;y++) {
							for(x=0;x<pi+1;x++) {
                                if( (X+x) >= XFinal || (Y+y) >= YFinal)
									color = -1;
								else
                                    color = video_get_pixel(X+x,Y+y);
                                cabe = (color > limPlanos[pl]) && (color <= limPlanos[pl+1]);
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


		for(pi=1;pi<5;pi++)
			totalPiezas[0] += totalPiezas[pi];
		
		CM[pl] = (float)totalPiezas[0] / numPiezas;

        vnorm += CM[pl]*CM[pl];

    }
                         
    vnorm=sqrt(vnorm);
    /*Normalizacion de valores entre 0 y 1*/
    for(x=0; x<numPlanos; x++)
        CM[x] = CM[x] / vnorm;
     


}



void main()
{

        int x,y, i;
        int ancho, alto, xVen, yVen;
        unsigned long color;
        int *LimitePlano, numplanos, pieza[5];
        float *cm;
        int numVentanas, ventana[4];

        /* Preparar paleta de Tonos de Gris */
        for(i = 0; i < 256; i++)        paleta[i] = i;

        /* Epera a que finalize el HOST */
        while(*turnoHOST);

        /* Lectura de parametros para el Coef. Morf */
        xVen = bufenteros[0];
        yVen = bufenteros[1];
        numplanos=bufenteros[2];
        LimitePlano=(int *)malloc(sizeof(int)*numplanos+1);
        for(x=0; x<numplanos+1; x++)
                   LimitePlano[x]=bufenteros[x+3];

        for(x=0; x<5; x++)
                   pieza[x]=bufenteros[numplanos+x+4];
        
        cm=(float *)malloc(sizeof(float)*numplanos);


        /* Configurar el modo de trabajo de la framegraber */
        video_setup(&ccir_esync);

        /* Wait for sync to occur before continuing */
        video_wait_lock();

        /* Set up the screen origin to the IMAGE page */
        video_set_screen_base(IMAGE);

        /* Configurar la paleta de Entrada */
        video_set_adc_lut(paleta);

        /* Clear the image to transparent. */
        video_clear_image();

        /* Grab the current video frame and goto display mode */
        video_grab(); 

        ventana[2] = xVen;
        ventana[3] = yVen;
        for(y=0;y<TAMY;y+=yVen) {
            ventana[1] = y;
            for(x=0;x<TAMX;x+=xVen) {
                ventana[0] = x;      
                CoeficienteMorfologico(ventana, numplanos, LimitePlano, pieza, cm);
                /*video_gotoxy(ventana[0],ventana[1]);
                video_printf("%d,%d",ventana[0],ventana[1]);*/

                for(i=0; i<numplanos; i++)
                            bufflotantes[i] = cm[i];

                *(turnoHOST)=1;
                while(*turnoHOST);
            }
        }


        /* Envio de la imagen por filas al HOST */
/*        video_printf("VENT: %dx%d\n",xVen,yVen);
        video_printf("N.P.: %d\n",numplanos);
        for(y=0;y<numplanos+1;y++)
            video_printf("%d ",LimitePlano[y]);
        video_printf("\n");
        for(y=0;y<5;y++)
            video_printf("%d ",pieza[y]);
        video_set_colour(255);
*/
        for(y=yVen;y<TAMY;y+=yVen)
            video_plot_line(0,y,511,y);
        for(x=xVen;x<TAMX;x+=xVen)
            video_plot_line(x,0,x,511);


        /* Rellenamos el buffer y pasamos a la memoria compartida */
        for(y=0; y<TAMY ; y++) {
                for(x=0; x<TAMX; x++) {
                        color=(unsigned long)video_get_pixel(x,y);
                        bufenteros[x] = color;
                }
                *(turnoHOST) = 1;     /* Indica al HOST que tome el relevo */
                while(*(turnoHOST));
        }

}



