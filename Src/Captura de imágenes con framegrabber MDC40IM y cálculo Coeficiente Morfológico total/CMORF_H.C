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
#define CODIGO_DSP      "cmorf_d.out"          /* Codigo COFF para el DSP */

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
        int i;
        FILE *fimag, *fparam;

        unsigned long numplanos;
        unsigned long *limitePlanos = NULL;
        unsigned long piezas[5];
        float *CM = NULL;


        if(argc != 3) {
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

        fscanf(fparam,"%ld",&numplanos);
        limitePlanos = (unsigned long *) malloc(sizeof(unsigned long)*(numplanos+1));
        CM = (float *) malloc(sizeof(float)*numplanos);
        if(limitePlanos == NULL || CM == NULL) {
                fprintf(stderr,"Error en la reserva de memoria.\n");
                exit(-1);
        }


        printf("Planos: %ld\n",numplanos);
        printf("Limites:\n");
        for (i=0;i<numplanos+1;i++) {
            fscanf(fparam,"%ld",&limitePlanos[i]);
            printf("%d ",limitePlanos[i]);
        }

        printf("\nPiezas\n");
        for(i=0;i<5;i++) {
            fscanf(fparam,"%ld",&piezas[i]);
            printf("%d ",piezas[i]);
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
        printf("Escribir en DPRAM el num Planos.\n");
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER,1,&numplanos);
        CheckReturnCode(returnCode);
        printf("Escribir en DPRAM los limites Planos.\n");
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER+1,numplanos+1,limitePlanos);
        CheckReturnCode(returnCode);
        printf("Escribir en DPRAM las piezas.\n");
        returnCode=Write_DPRAM_Words_32(Id_DSP,BUFFER+numplanos+2,5,piezas);
        CheckReturnCode(returnCode);
  
        printf("Darle paso.\n");
        turnoHOST = 0;                           /* Dar paso al DSP */
        returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
        CheckReturnCode(returnCode);

        printf("Cargando y ejecutando programa COFF...\n\n"); /* Carga del codigo para DSP) */
        returnCode=Load_And_Run_File_LIA(Id_DSP,CODIGO_DSP);
        CheckReturnCode(returnCode);

        printf("EJECUCION EN EL DSP\n");              /* Tarea a realizar en el DSP */

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

        printf("Esperando Ejecuci¢n del C.M.\n");

        tIni = clock();

        do {
             returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
             CheckReturnCode(returnCode);
        } while(!turnoHOST);

        tFin = clock();

        tiempo = (float) ((tFin - tIni)/CLOCKS_PER_SEC);

        /* Leer de la DPRAM el resultado del CM */
        returnCode=Read_DPRAM_Floats_32(Id_DSP, BUFFER, numplanos, CM);
        CheckReturnCode(returnCode);

        printf("\nRESULTADOS EN EL DSP (CM):\n");
        for(i=0;i<numplanos;i++) {
                printf("Coef. Morf. Plano %d = %f\n",i,CM[i]);
        }
        printf("Tiempo Ejecucion: %.10f\n",tiempo);

        exit(0);
}
