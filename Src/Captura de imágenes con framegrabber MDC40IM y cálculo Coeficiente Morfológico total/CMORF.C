#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef unsigned char unchar;

unsigned ancho, alto;
unchar **imagen;

void CoeficienteMorfologico(unchar numPlanos,unsigned limPlanos[], unchar piezas[], float CM[])
{
	unchar pl, pi;
	unchar numPiezas;
	unchar cabe;
	unsigned totalPiezas[5];
	unsigned X, Y, x, y, ultX;
	short color;
	
	
	numPiezas = 0;
	for(pi=0;pi<5;pi++)	if(piezas[pi])	numPiezas++;

	/* Para cada uno de los planos de la imagen */	
	for(pl=0;pl<numPlanos;pl++) {
		
		/* Inicializamos el numero de piezas que caben de cada tamanyo */
		for(pi=0;pi<5;pi++)	totalPiezas[pi] = 0;

		/* Para cada tamanyo de pieza elegido */
		for(pi=0;pi<5;pi++) {
			if(piezas[pi]) {
				
				/* Recorremos toda la imagen */
                /*for(Y=0;Y<alto;Y++) {*/
                for(Y=0;Y<alto;Y+=(l+1)) {
                    /*if((Y%(pi+1)) == 0)
                        ultX = 0;*/
                    /*for(X=ultX;X<ancho;) {*/
                    for(X=0;X<ancho;X+=(l+1)) {
						/* Comprobar si cabe la pieza */
						for(y=0;y<pi+1;y++) {
							for(x=0;x<pi+1;x++) {
								if( (X+x) >= ancho || (Y+y) >= alto)
									color = -1;
								else
									color = imagen[X+x][Y+y];
                                cabe = (color > limPlanos[pl]) && (color <= limPlanos[pl+1]);
								if(!cabe)
									break;
							}
							if(!cabe)
								break;
						}
						if(cabe) {
							totalPiezas[pi]++;
                            /*ultX += (pi+1);
                            X += (pi+1);*/
						}
                        /*else
                            X++;*/
					}
				}
			}
		}


		for(pi=1;pi<5;pi++)
			totalPiezas[0] += totalPiezas[pi];
		
		CM[pl] = (float)totalPiezas[0] / numPiezas;
	}
}

void CoeficienteMorfologico1()
{
  int endy, inity, initx, endx, numplanos;
/*  int alto, altoventana, color, numpiezas;*/
  int altoventana, color, numpiezas;
  int total[5];  
  int cm[5];              /* Numero planos CM */
  short int cabe;         /* Flag que indicara si cabe la pieza de CM */
  int  auxInitY;          /* Columna comienzo imagen */
  int  auxEndY;           /* Columna final imagen */
  int k,l,x,y,j,i;
  int pieza[]={0,1,0,0,0};
  int LimitePlano[]={32,64,96,128,256};
  
/*  endy = 512;*/
  endy = 7;
  inity=0;
  initx=0;
/*  endx=512;*/
  endx = 7;
  numplanos=5;
 

/*  video_get_size(NULL, &alto);*/
  altoventana=endy-inity+1;
  auxInitY=alto-inity-1;
  auxEndY=alto-endy-1;
  numpiezas=0;
 
  for (k=0; k < numplanos; k++) cm[k]=0;
 
  for (l=0; l < 5;l++)
    if (pieza[l]) numpiezas++;
   
  
  /* Para cada plano que se quiere examinar */
  for (k=0; k < numplanos; k++)
  {
    for (l=0; l < 5;l++)    total[l]=0;

    /* Para cada uno de los tamaños de pieza seleccionados */
    for (l=0; l < 5;l++)
    {
      if (pieza[l])
      {

        /* Centrado en un pixel del cuadrado de pieza a examinar */
        for (y=auxInitY; y >= auxEndY; y-=(l+1))
        {
          for (x=initx; x <= endx; x+=(l+1))
          {
            /* Se examina el area circundante al tamaño de la pieza */
            for (j=y; j >= y-l; j--)       /* Loop horizontal ? */
            {
              for (i=x; i <= x+l; i++)       /* Loop vertical ? */
              {
                if (j < auxEndY || i > endx)  /* Fuera del area */
                  color=-1;
                else
                  /*color=video_get_pixel(i,j);*/
                  color=imagen[i][j];

                if (k == 0)  /* Solo tenemos un intervalo */
                  cabe=(color>=0 && color<=LimitePlano[k]);
                else
                  cabe=(color>=(LimitePlano[k-1]+1) && color<=LimitePlano[k]);

                if (!cabe) break;
              }
              if (!cabe) break;
            }
            if (cabe)   /* Si cabe tras examinar todos los pixeles dentro de la pieza... */
             total[l]++;
          }
        } 
      }
    }

    /* Una vez se han pasado todas las piezas seleccionadas segun ese intervalo,
       y se han almacenado cuantas caben de cada tipo en ese intervalo... */
    for (l=1; l < 5;l++) 
      total[0]+=total[l];

    cm[k]=total[0]/numpiezas;
  }

  /*Escribo en la DPRAM el resultado*/
  for (k=0; k < numplanos+1; k++)
  {
  	fprintf(stdout,"CM1(%d): %d\n",k,cm[k]);
/*     buffer[k]=cm[k];*/
  }


  /*Le digo al HOST que lo lea*/
/*  *(turnoHOST)=1;  */
}


void main(int argc, char *argv[])
{
    clock_t tIni, tFin;
    float tiempo;
    int color, x, y;
	char linea[100];
	FILE *fic;
	unchar numPlanos = 5;
    int limitesPlanos[5+1] = { -1,32,64,96,128,255 };
    unchar piezas[5] = { 0,1,1,1,0 };
	float cm[5];

	if(argc !=2) {
		fprintf(stderr,"Numero de Argumentos Incorrecto.\n");
		exit(-1);
	}

	if((fic=fopen(argv[1],"r"))==NULL) {
		fprintf(stderr,"No se puede Abrir el Fichero: %s\n",argv[1]);
		exit(-1);
	}

	fscanf(fic,"%[^\n]",linea);		fgetc(fic);
	fscanf(fic,"%[^\n]",linea);		fgetc(fic);
	fscanf(fic,"%d",&ancho);
	fscanf(fic,"%d",&alto);
	fscanf(fic,"%d",&color);

	imagen = (unchar **)malloc(alto*sizeof(unchar *));
	if(imagen == NULL) {
		fprintf(stderr,"Error en Asignacion de Memoria.\n");
		exit(-1);
	}
	for(y=0;y<alto;y++) {
		imagen[y] = (unchar *)malloc(ancho*sizeof(unchar));
		if(imagen[y] == NULL) {
			fprintf(stderr,"Error en Asignacion de Memoria (%d).\n",y);
			exit(-1);
		}
	}

	for(y=0;y<alto;y++) {
		for(x=0;x<ancho;x++) {
			fscanf(fic,"%d",&color);
			imagen[x][y] = (unchar)color;
		}
	}

    tIni = clock();

	CoeficienteMorfologico(numPlanos,limitesPlanos,piezas,cm);

    tFin = clock();

    tiempo = (float) (tFin - tIni)/CLOCKS_PER_SEC;
	
	for(x=0;x<numPlanos;x++)
		fprintf(stdout,"CM(%d): %f\n",x,cm[x]);

	fprintf(stdout,"CM Galiana.\n");
	CoeficienteMorfologico1();
 
	exit(0);
}



