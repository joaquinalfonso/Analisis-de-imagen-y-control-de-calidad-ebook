#include <stdlib.h>
#include <math.h>

#include "dirs.h"
#include "video.h"

#define TAMX    512
#define TAMY    512


/*int r[256], g[256], b[256];     /* Storage for IMAGE palette      */
int y1[256];                    /* Storage for IMAGE ADC palette  */
/*int ro[16], go[16], bo[16];     /* Storage for OVERLAY palette    */

unsigned long *buffer = (unsigned long *) BUFFER;

unsigned long *Semaforo = (unsigned long *) SEMAFORO;
										  /* Localizacion de la variable cerrojo (sincronizacion) */

void main()
{

	int x,y, i;
        int ancho, alto;
	unsigned long color;

/*        asm(" or 800h, ST"); */

        for(i = 0; i < 256; i++) {
                y1[i] = i;
        }


       while( (*Semaforo) == ROJO );               /* Espera a que finalice el HOST */

       video_setup(&ccir_esync);

    ADC_ADDR = 0x00;
    ADC_CI_DATA = 0xA0;


/* Wait for sync to occur before continuing */
        video_wait_lock();

/* Set up the screen origin to the IMAGE page */
        video_set_screen_base(IMAGE);

	video_set_adc_lut(y1);


/* Clear the image to transparent. */
        video_clear_image();

/*        video_set_videomask(0xf0);*/

/* Grab the current video frame and goto display mode */
        video_grab(); 


/*        video_display();

        video_set_colour(250);
        for(y=0;y<20;y++)
                for(x=0;x<100;x++)
			video_set_pixel(x,y);

        video_set_text_colour(150);
        video_set_text_scale(1,1);
        video_gotoxy(5,5);
        video_outchar('J');

        video_get_size(&ancho,&alto);
        video_gotoxy(1,10);
        video_printf("%d - %d",ancho, alto);

        video_gotoxy(1,1);
        video_printf("%d",video_get_pixel(50,50));
*/

/* Rellenamos el buffer y pasamos a la memoria compartida */
        for(y=0; y<TAMY ; y++) {
                for(x=0; x<TAMX; x++) {
                /*        color = (y % 256);*/
                        color=(unsigned long)video_get_pixel(x,y);
                        buffer[x] = color;
                }
                (*Semaforo) = ROJO;     /* Indica al HOST que tome el relevo */
                while( (*Semaforo) == ROJO );
        }

}



