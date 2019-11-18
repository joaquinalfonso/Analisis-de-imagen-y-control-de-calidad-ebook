#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <malloc.h>

#include "c4xapp.h"
#include "dirs.h"
#include "dsperror.c"

#define TAMX    512
#define TAMY    512


#define NOMBRE_DSP      "CPU_A"                 /* Nombre del procesador */
#define CODIGO_DSP      "cmorf1_d.out"          /* Codigo COFF para el DSP */

UINT	returnCode;				/* Codigo de error de la lib. NETAPI */
unsigned long   turnoHOST;                      /* Valor del cerrojo HAND */


void main(int argc, char *argv[])
{
        clock_t tIni, tFin;
        float tiempo;
        PROC_ID *Id_DSP;
        long x,y;
        unsigned long *linea=NULL;
        long cont;
        int i, numVentanas;
        FILE *fimag, *fparam, *fsal;

        unsigned long xVen, yVen;
        unsigned long numplanos;
        unsigned long *limitePlanos = NULL;
        unsigned long piezas[5];
        float *CM = NULL, Xvent, Yvent;


        if(argc != 4) {
                fprintf(stderr,"Numero de parametros incorrecto.\n");
		exit(-1);
        }

        if((fparam=fopen(argv[1], "r"))==NULL) {
                fprintf(stderr, "No puedo abrir el fichero de parametros.\n");
                exit(-1);
        }

        if((fimag=fopen(argv[2], "w"))==NULL) {
                fprintf(stderr, "No puedo abrir el fichero de imagen.\n");
                exit(-1);
        }

        if((fsal=fopen(argv[3], "w"))==NULL) {
                fprintf(stderr, "No puedo abrir el fichero de salida.\n");
                exit(-1);
        }

        fscanf(fparam,"%ld",&xVen);
        fscanf(fparam,"%ld",&yVen);
        fscanf(fparam,"%ld",&numplanos);
        limitePlanos = (unsigned long *) malloc(sizeof(unsigned long)*(numplanos+1));
        CM = (float *) malloc(sizeof(float)*numplanos);
        if(limitePlanos == NULL || CM == NULL) {
                fprintf(stderr,"Error en la reserva de memoria.\n");
                exit(-1);
        }

        printf("Tama¤o de Ventana %ld,%ld\n",xVen,yVen);
        printf("Planos: %ld\n",numplanos);
        printf("Limites:\n");
        for (i=0;i<numplanos+1;i++) {
            fscanf(fparam,"%ld",&limitePlanos[i]);
            printf("%ld ",limitePlanos[i]);
        }

        printf("\nPiezas\n");
        for(i=0;i<5;i++) {
            fscanf(fparam,"%ld",&piezas[i]);
            printf("%ld ",piezas[i]);
        }
        printf("\n");

        linea=(unsigned long *)malloc(sizeof(unsigned long)*TAMX);
        if(linea == NULL) {
                fprintf(stderr,"Error en la reserva de memoria.\n");
                exit(-1);
        }

        fclose(fparam);

        /** Inicializar el DSP **/
          
        printf("Inicializando DSP...\n");
        returnCode = Global_Network_Reboot();
        CheckReturnCode(returnCode);

        printf("Abriendo procesador...\n");
        returnCode = Open_Processor_ID(&Id_DSP,NOMBRE_DSP,NULL);
        CheckReturnCode(returnCode);

        /* Escribir los parametros del CM en la DPRAM */
        /*printf("Escribir en DPRAM el Tam de Ventana.\n");*/
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER,1,&xVen);
        CheckReturnCode(returnCode);
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER+1,1,&yVen);
        CheckReturnCode(returnCode);
        /*printf("Escribir en DPRAM el num Planos.\n");*/
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER+2,1,&numplanos);
        CheckReturnCode(returnCode);
        /*printf("Escribir en DPRAM los limites Planos.\n");*/
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER+3,numplanos+1,limitePlanos);
        CheckReturnCode(returnCode);
        /*printf("Escribir en DPRAM las piezas.\n");*/
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER+numplanos+4,5,piezas);
        CheckReturnCode(returnCode);
  
        printf("Darle paso.\n");
        turnoHOST = 0;                           /* Dar paso al DSP */
        returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
        CheckReturnCode(returnCode);

        printf("Cargando y ejecutando programa COFF...\n\n"); /* Carga del codigo para DSP) */
        returnCode=Load_And_Run_File_LIA(Id_DSP,CODIGO_DSP);
        CheckReturnCode(returnCode);

        printf("EJECUCION EN EL DSP\n");              /* Tarea a realizar en el DSP */
        printf("Esperando Ejecuci¢n del C.M.\n");

        numVentanas = (TAMX / xVen) * (TAMY / yVen);
        printf("Numero de Ventanas: %d\n",numVentanas);

        fprintf(fsal,"%d\t%d\n",numVentanas,numplanos);

        tIni = clock();
        for(y=0;y<TAMY;y+=yVen) {
            for(x=0;x<TAMX;x+=xVen) {

                do {
                     returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
                     CheckReturnCode(returnCode);
                } while(!turnoHOST);

                /* Leer de la DPRAM el resultado del CM */
                returnCode=Read_DPRAM_Floats_32(Id_DSP, BUFFER, numplanos, CM);
                CheckReturnCode(returnCode);

                /* printf("(%2ld,%2ld):\t",(x/xVen),(y/yVen)); */
                for(i=0;i<numplanos;i++) {
                    fprintf(stdout,"%f\t",CM[i]);
                    fprintf(fsal,"%f\t",CM[i]);
                }
                fprintf(stdout,"\n");
                fprintf(fsal,"\n");

                turnoHOST = 0;                           /* Dar paso al DSP */
                returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
                CheckReturnCode(returnCode);
            }
        }
        tFin = clock();

        tiempo = (float) ((tFin - tIni)/CLOCKS_PER_SEC);
        printf("Tiempo Procesamiento Total: %.10f\n",tiempo);

        fclose(fsal);





        /* Lectura de la Imagen Capturada */
        printf("Lectura de la Imagen Capturada\n");

        fprintf(fimag,"P2\n");
        fprintf(fimag,"# CREATOR: Capturador imagenes DSP Rev: 30/04/99\n");
        fprintf(fimag,"%d %d\n",TAMX,TAMY);
        fprintf(fimag,"255\n");


        cont = 0;
        for(y=0; y<TAMY; y++) {

                do {
                        returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
                        CheckReturnCode(returnCode);
                } while(!turnoHOST);

                returnCode=Read_DPRAM_Words_32(Id_DSP, BUFFER, TAMX, linea);
                CheckReturnCode(returnCode);
                
                for(x=0; x<TAMX; x++) {
                        fprintf(fimag, "%3d ",linea[x]);

                        cont++;
                        if((cont % 100) == 99)
                                fprintf(fimag, "\n");
                }


                turnoHOST = 0;                           /* Dar paso al DSP */
                returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
                CheckReturnCode(returnCode);

                if( (y % 16) == 0 )
                        printf(".");
        }

        fclose(fimag);
        free(linea);
        linea=NULL;

        printf("\nFin Captura!!!\n");

        exit(0);
}




