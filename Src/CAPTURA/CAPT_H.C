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
#define CODIGO_DSP      "capt_d.out"          /* Codigo COFF para el DSP */

UINT   returnCode;             /* Codigo de error de la lib. NETAPI */
unsigned long   Semaforo;                      /* Valor del cerrojo HAND */
FILE *fimag;

void main(int argc, char *argv[])
{
        
        PROC_ID *Id_DSP;
        long x,y;
        unsigned long linea[512];
        long cont;

        if(argc != 2) {
                fprintf(stderr,"Numero de parametros incorrecto.\n");
		exit(-1);
        }

        if((fimag=fopen(argv[1], "w"))==NULL) {
                fprintf(stderr, "No puedo abrir el fichero.\n");
                exit(-1);
        }

	/** Inicializar el DSP **/
          
        printf("Inicializando DSP...\n");
        returnCode = Global_Network_Reboot();
        CheckReturnCode(returnCode);

        printf("Abriendo procesador...\n");
        returnCode = Open_Processor_ID(&Id_DSP,NOMBRE_DSP,NULL);
        CheckReturnCode(returnCode);

        Semaforo = VERDE;                           /* Dar paso al DSP */
        returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&Semaforo);
        CheckReturnCode(returnCode);
        

        printf("Cargando y ejecutando programa COFF...\n\n"); /* Carga del codigo para DSP) */
        returnCode=Load_And_Run_File_LIA(Id_DSP,CODIGO_DSP);
        CheckReturnCode(returnCode);

        printf("EJECUCION EN EL DSP\n");              /* Tarea a realizar en el DSP */

        fprintf(fimag,"P2\n");
        fprintf(fimag,"# CREATOR: Capturador imagenes DSP Rev: 30/04/99\n");
        fprintf(fimag,"%d %d\n",TAMX,TAMY);
        fprintf(fimag,"255\n");


        printf("Esperando la imagen.\n");
        cont = 0;
        for(y=0; y<TAMY; y++) {

                do {
                        returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&Semaforo);
                        CheckReturnCode(returnCode);
                } while( Semaforo == VERDE );

                returnCode=Read_DPRAM_Words_32(Id_DSP, BUFFER, TAMX, linea);
                CheckReturnCode(returnCode);
                
                for(x=0; x<TAMX; x++) {
                        fprintf(fimag, "%3d ",linea[x]);
                        cont++;
                        if((cont % 100) == 99)
                                fprintf(fimag, "\n");
                }

                Semaforo = VERDE;
                returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&Semaforo);
                CheckReturnCode(returnCode);

                if( (y % 16) == 0 )
                        printf(".");

        }

        fclose(fimag);

        printf("\nFin Captura!!!\n");

	exit(0);
}
