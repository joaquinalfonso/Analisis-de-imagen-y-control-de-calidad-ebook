//---------------------------------------------------------------------------
#include <vcl\condefs.h>
#include <vcl\extctrls.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma hdrstop
//---------------------------------------------------------------------------
USERES("pgm2bmp.res");
//---------------------------------------------------------------------------
int main(int argc, char **argv)
{
	Graphics::TBitmap *Imagen;
	FILE *ficPGM;
    char linea[100];
    int fil, col, valor;
    HPALETTE paleta;
    TColor color;


	if(argc != 3) {
    	fprintf(stderr,"Error: USO: pgm2bmp fic_pgm fic_bmp \n");
        exit(-1);
    }

	ficPGM = fopen(argv[1],"r");
    if(ficPGM == NULL) {
    	fprintf(stderr,"El fichero %s NO EXISTE.\n", argv[1]);
        exit(-1);
    }

    	// Crear la imagen de tipo BITMAP.
    Imagen = new Graphics::TBitmap;

    fscanf(ficPGM,"%[^\n]",linea);	fgetc(ficPGM);
printf("%s\n",linea);
	fscanf(ficPGM,"%[^\n]",linea);	fgetc(ficPGM);
printf("%s\n",linea);
	fscanf(ficPGM,"%[^\n]",linea);	fgetc(ficPGM);
printf("%s\n",linea);

    fscanf(ficPGM,"%d %d %d",&col, &fil, &valor);
printf("Filas: %d Columnas: %d Colores:%d\n",fil, col,valor);
//getchar();

	Imagen->Width = col;
    Imagen->Height = fil;

	for(int y=0;y<fil;y++) {
    	for(int x=0;x<col;x++) {
        	fscanf(ficPGM,"%d",&valor);
            color = (valor << 16) | (valor << 8) | valor;
            Imagen->Canvas->Pixels[x][y] = color;
        }
    }

	Imagen->SaveToFile(AnsiString(argv[2]));

	return 0;
}
//---------------------------------------------------------------------------
