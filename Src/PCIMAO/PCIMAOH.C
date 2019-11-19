#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <malloc.h>

#ifdef  __BORLANDC__
    #include <dir.h>
#else
    #include <direct.h>
#endif

#ifdef DOS
    #include <dos.h>
#else
    #ifdef WINDOWS
        #include <windows.h>
    #endif
#endif

#include "c44lib.h"
#include "dirs.h"



char    *mpy_fnames[NUMBER_OF_PROCS] = { "mao1d.out", "mao2d.out" };

BOOL    NeedToClose = FALSE;
CARD_ID card_identity;
UINT	returnCode;				/* Codigo de error de la lib. NETAPI */
unsigned long semaforo1;
unsigned long semaforo2;



void clean_exit(int code)
{
    if ( NeedToClose == TRUE )
    {
        if ( CloseCard(card_identity) != PCIC44_SUCCESS )
            printf("Error closing card!\n");
    }

    exit(code);
}



void main(int argc, char *argv[])
{
        PCIC44ErrorCode ret;
        WORD    wNumProcs;
        int    i;
        FILE *fentr, *ftest;
        unsigned nP, dim;
        float *patrones, dato;
        unsigned int j;

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
        ret = InitPCIC44Lib();
        if ( ret != PCIC44_SUCCESS )
        {
           printf("ERROR: Failed to initialise Runtime Support Library (%d)\n", ret);
           clean_exit(1);
        }

        ret = GetCardIdentity("BOARD_A", &card_identity);
        if ( ret != PCIC44_SUCCESS )
        {
             printf("Failed to find card identity for BOARD_A (%d)\n", ret);
             clean_exit(1);
        }    

        ret = GetCardNumProcs(card_identity, &wNumProcs);
        if ( ret != PCIC44_SUCCESS )
        {
             printf("ERROR: Failed to get number of processors (%d)\n", ret);
             clean_exit(1);
        }

        ret = OpenCard(card_identity);
        if ( ret != PCIC44_SUCCESS )
        {
             printf("ERROR: Failed to open card (%d)\n", ret);
             clean_exit(1);
        }

        NeedToClose = TRUE;

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
        ret = Write_Shared_Mem_Block_Float(card_identity, BASE , 1,&dato, FALSE);
        if ( ret != PCIC44_SUCCESS )
        {
          printf("ERROR: Failed to write shared memory (%d)\n", ret);
          clean_exit(1);
        }


        dato = (float)dim;
        ret = Write_Shared_Mem_Block_Float(card_identity, BASE+1 , 1, &dato, FALSE);

        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }


        ret = Write_Shared_Mem_Block_Float(card_identity, BASE+2 , nP*(dim+1),patrones, FALSE);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }


        printf("Darle paso.\n");
        semaforo1 = VERDE;
        semaforo2 = VERDE;
        ret = WriteMailbox(card_identity, 0, semaforo1);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write Mailbox (0)\n");
            clean_exit(1);
        }
        ret = WriteMailbox(card_identity, 1, semaforo2);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write Mailbox (1)\n");
            clean_exit(1);
        }

        /* Pongo en marcha cada uno de los procesadores */

       for (i = 0; i < wNumProcs; i++)
       {
        printf("Load and Running processor %d\n", i + 1);

        ret = LoadAndRunObjectFile(card_identity, (WORD)(i + 1), mpy_fnames[i]);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to download and run processor %d (%d)\n", i + 1, ret);
            clean_exit(1);
        }
     }


    /*Paso de los patrones de TEST, a cada uno de los procesadores, cuando estos hayan
    terminado*/

        
        printf("EJECUCION EN EL DSP\n");              /* Tarea a realizar en el DSP */
        printf("\nEsperando a que los procesadores terminen de leer ENTRE\n");

       do {
           ret = ReadMailbox(card_identity, 0, &semaforo1);
           if ( ret != PCIC44_SUCCESS )
           {
               printf("ERROR: Failed to read Mailbox (0)\n");
               clean_exit(1);
           }
           ret = ReadMailbox(card_identity, 1, &semaforo2);
           if ( ret != PCIC44_SUCCESS )
           {
               printf("ERROR: Failed to read Mailbox (0)\n");
               clean_exit(1);
           }

       } while( (semaforo1 == VERDE) && (semaforo2 == VERDE) );

          
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
        ret = Write_Shared_Mem_Block_Float(card_identity, BASE , 1, &dato, FALSE);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }
       /* dato = (float)dim;

        ret = Write_Shared_Mem_Block_Float(card_identity, BASE+1 , 1, &dato, FALSE);

        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }*/

        ret = Write_Shared_Mem_Block_Float(card_identity, BASE+1 , nP*dim, patrones, FALSE);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }


    
        printf("Darle paso.\n");
        printf("Darles paso a los procesadores para leer TEST.\n");

        semaforo1 = VERDE;
        semaforo2 = VERDE;
 
        ret = WriteMailbox(card_identity, 0, semaforo1);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }
        ret = WriteMailbox(card_identity, 1, semaforo2);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to write shared memory (%d)\n", ret);
            clean_exit(1);
        }


        do {
           ret = ReadMailbox(card_identity, 0, &semaforo1);
           if ( ret != PCIC44_SUCCESS )
           {
               printf("ERROR: Failed to read Mailbox(0)\n");
               clean_exit(1);
           }
           ret = ReadMailbox(card_identity, 1, &semaforo2);
           if ( ret != PCIC44_SUCCESS )
           {
               printf("ERROR: Failed to read Mailbox(1)\n");
               clean_exit(1);
           }
           
       } while( (semaforo1 == VERDE) && (semaforo2 == VERDE) );

        fprintf(stdout,"Recogiendo resultados del DSP.\n");
        free(patrones);
        patrones = (float *)malloc(nP*2*sizeof(float));
        if(patrones == NULL) {
            fprintf(stderr,"Error en la reserva de Memoria.\n");
            exit(-1);
        }

      /* Lectura del resultado desde la memoria compartida */

        ret = Read_Shared_Mem_Block_Float(card_identity, BASE, 2*nP, patrones, FALSE);
        if ( ret != PCIC44_SUCCESS )
        {
            printf("ERROR: Failed to read shared memory (%d)\n", ret);
            clean_exit(1);
        }

        for(i=0;i<nP;i++) 
            fprintf(stdout,"Patron(%d): Vecino: %f - Distancia: %f\n",i, patrones[2*i],patrones[2*i+1]);

        free(patrones);
	      CloseCard(card_identity);

        flushall();     // flush all buffers before exiting

        NeedToClose = FALSE;

        clean_exit(0);


        exit(0);
}
