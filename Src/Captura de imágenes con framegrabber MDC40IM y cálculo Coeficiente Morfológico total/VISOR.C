#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include "svga16.h"
#include "twk16.h"
#include "svga256.h"
#include "twk256.h"
#include "svga32k.h"

int WhitePixel()
{
  if (getmaxcolor() > 256)
	return(32767);
  return(15);
}

int huge DetectVGA256()
{
  int Vid = 3;

  return Vid;
}

/* Setvgapalette256 sets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* setvgapalette256(&dac256);			      */
void setvgapalette256(DacPalette256 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1012;
  reg.r_bx = 0;
  reg.r_cx = 256;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}


main(int argc, char *argv[])
{
	int Gd = DETECT, Gm;
	int Drv, i, j, x, y;
	DacPalette256 dac256;
	FILE *fic;
	char linea[100];
	int tamx, tamy, color, dsplzx, dsplzy;

	if(argc !=2) {
		fprintf(stderr,"Numero de Argumentos Incorrecto.\n");
		exit(-1);
	}

	if((fic=fopen(argv[1],"r"))==NULL) {
		fprintf(stderr,"No se puede Abrir el Fichero: %s\n",argv[1]);
		exit(-1);
	}

	for(i=0;i<64;i++) {
		for(j=0;j<4;j++) {
			dac256[4*i+j][0] = i;
			dac256[4*i+j][1] = i;
			dac256[4*i+j][2] = i;
		}
	}

	fscanf(fic,"%[^\n]",linea);		fgetc(fic);
	fscanf(fic,"%[^\n]",linea);		fgetc(fic);
	fscanf(fic,"%d",&tamx);
	fscanf(fic,"%d",&tamy);
	fscanf(fic,"%d",&color);

	dsplzx = ( 800 - tamx ) / 2;
	dsplzy = ( 600 - tamy ) / 2;

	installuserdriver("Svga256",DetectVGA256);
	/*  If driver is linked with file,remove comments */
	/*          registerfarbgidriver(Svga256_fdriver); */
	initgraph(&Gd,&Gm,"");
	setvgapalette256(&dac256);

	for(y=0;y<tamy;y++) {
		for(x=0;x<tamx;x++) {
			fscanf(fic,"%d",&color);
/*			printf("%d ",color);*/
			putpixel(dsplzx + x,dsplzy + y, color );
		}
	}

	getch();
	closegraph();

}


/*
	for(i=0;i<256;i++) {
		setcolor(i);
		line(100,100+i,300,100+i);
	}
	setcolor(165);
	line(0,0,getmaxx(),getmaxy());
	line(0,getmaxy(),getmaxx(),0);
*/


