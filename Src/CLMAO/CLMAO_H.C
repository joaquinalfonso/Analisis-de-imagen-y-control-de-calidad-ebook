#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <malloc.h>

#include "c4xapp.h"
#include "dirs.h"
#include "dsperror.c"

#define NOMBRE_DSP      "CPU_A"                 /* Nombre del procesador */
#define CODIGO_DSP      "clmao_d.out"          /* Codigo COFF para el DSP */

UINT	returnCode;				/* Codigo de error de la lib. NETAPI */
unsigned long   turnoHOST;                      /* Valor del cerrojo HAND */


void main(int argc, char *argv[])
{
        clock_t tIni, tFin;
        float tiempo;
        PROC_ID *Id_DSP;
        FILE *fentr, *ftest;

        unsigned nP, dim;
        float *patrones, dato;
        int i, j;

        if(argc != 3) {
                fprintf(stderr,"Numero de parametros incorrecto.\n");
		exit(-1);
        }

        if((fentr=fopen(argv[1], "r"))==NULL) {
                fprintf(stderr, "No puedo abrir el fichero de entrenamiento.\n");
                exit(-1);
        }

        if((ftest=fopen(argv[2], "r"))==NULL) {
                fprintf(stderr, "No puedo abrir el fichero de test.\n");
                exit(-1);
        }

        /** Inicializar el DSP **/
        printf("Inicializando DSP...\n");
        returnCode = Global_Network_Reboot();
        CheckReturnCode(returnCode);

        printf("Abriendo procesador...\n");
        returnCode = Open_Processor_ID(&Id_DSP,NOMBRE_DSP,NULL);
        CheckReturnCode(returnCode);


        /* Extraer datos Fichero entrenamiento */
        fscanf(fentr,"%d",&nP);
        fscanf(fentr,"%d",&dim);

        fprintf(stdout,"Patrones de ENTRENAMIENTO.\n");
        fprintf(stdout,"Num. Patrones: %d\n",nP);
        fprintf(stdout,"Dimension: %d\n",dim);

        patrones = (float *)malloc(nP*(dim+1)*sizeof(float));
        if(patrones == NULL) {
            fprintf(stderr,"Error en la reserva de Memoria.\n");
            exit(-1);
        }

        for(j=0;j<nP;j++) {
            for(i=0;i<dim+1;i++) {
                fscanf(fentr,"%f",&dato);
                patrones[j*(dim+1)+i] = dato;
            }
        }

        fclose(fentr);

        /* Escribir los patrones de Entrenamiento en la DPRAM */
        fprintf(stdout,"Escribiendo patrones Entrenamiento en la DPRAM.\n");
        dato = (float)nP;
        returnCode=Write_DPRAM_Floats_32(Id_DSP,BUFFER,1,&dato);
        CheckReturnCode(returnCode);
        dato = (float)dim;
        returnCode=Write_DPRAM_Floats_32(Id_DSP,BUFFER+1,1,&dato);
        CheckReturnCode(returnCode);

        returnCode=Write_DPRAM_Floats_32(Id_DSP,BUFFER+2,nP*(dim+1),patrones);
        CheckReturnCode(returnCode);
  
        printf("Darle paso.\n");
        turnoHOST = 0;                           /* Dar paso al DSP */
        returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
        CheckReturnCode(returnCode);

        printf("Cargando y ejecutando programa COFF...\n"); /* Carga del codigo para DSP) */
        returnCode=Load_And_Run_File_LIA(Id_DSP,CODIGO_DSP);
        CheckReturnCode(returnCode);

        printf("EJECUCION EN EL DSP\n");              /* Tarea a realizar en el DSP */

        do {
            returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
            CheckReturnCode(returnCode);
        } while(!turnoHOST);
          
        /* Extraer datos Fichero de test */
        fscanf(ftest,"%d",&nP);
        fscanf(ftest,"%d",&dim);

        fprintf(stdout,"Patrones de TEST.\n");
        fprintf(stdout,"Num. Patrones: %d\n",nP);
        fprintf(stdout,"Dimension: %d\n",dim);

        free(patrones);
        patrones = (float *)malloc(nP*dim*sizeof(float));
        if(patrones == NULL) {
            fprintf(stderr,"Error en la reserva de Memoria.\n");
            exit(-1);
        }

        for(j=0;j<nP;j++) {
            for(i=0;i<dim;i++) {
                fscanf(ftest,"%f",&dato);
                patrones[j*dim+i] = dato;
            }
        }

        fclose(ftest);

        /* Escribir los patrones de Test en la DPRAM */
        fprintf(stdout,"Escribiendo patrones Test en la DPRAM.\n");
        dato = (float)nP;
        returnCode=Write_DPRAM_Floats_32(Id_DSP,BUFFER,1,&dato);
        CheckReturnCode(returnCode);

        returnCode=Write_DPRAM_Floats_32(Id_DSP,BUFFER+1,nP*dim,patrones);
        CheckReturnCode(returnCode);
  
        printf("Darle paso.\n");
        turnoHOST = 0;                           /* Dar paso al DSP */
        returnCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
        CheckReturnCode(returnCode);

        tIni = clock();

        do {
            returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&turnoHOST);
            CheckReturnCode(returnCode);
        } while(!turnoHOST);

        tFin = clock();
        tiempo = (float) ((tFin - tIni)/CLOCKS_PER_SEC);

        fprintf(stdout,"Recogiendo resultados del DSP.\n");
        free(patrones);
        patrones = (float *)malloc(nP*2*sizeof(float));
        if(patrones == NULL) {
            fprintf(stderr,"Error en la reserva de Memoria.\n");
            exit(-1);
        }

        returnCode = Read_DPRAM_Floats_32(Id_DSP,BUFFER,2*nP,patrones);
        CheckReturnCode(returnCode);

        for(i=0;i<nP;i++) 
            fprintf(stdout,"Patron(%d): Vecino: %d - Distancia Mao: %f\n",i,(unsigned)patrones[2*i],patrones[2*i+1]);

        fprintf(stdout,"Tiempo Ejecucion: %.10f\n",tiempo);

        free(patrones);
        exit(0);
}
