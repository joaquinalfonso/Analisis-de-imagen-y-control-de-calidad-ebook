#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "c4xapp.h"
#include "dirs.h"
#include "dsperror.c"

#define NOMBRE_DSP      "CPU_A"        /* Nombre del procesador */

PROC_ID *Id_DSP;

/*------------------------ FUNCIONES AUXILIARES --------------------------*/
void Escribir_palabras_32(ULONG direc, UINT longitud, ULONG *datos)
{
    UINT returnCode;
    returnCode=Write_DPRAM_Words_32(Id_DSP,direc,longitud,datos);
    CheckReturnCode(returnCode);
    return;
}


void Leer_palabras_32(ULONG direc, UINT longitud, ULONG *datos)
{
    UINT returnCode;
    returnCode=Read_DPRAM_Words_32(Id_DSP,direc,longitud,datos);
    CheckReturnCode(returnCode);
    return;
}


void Leer_flotantes_32(ULONG direc, UINT longitud, float *datos)
{
    UINT returnCode;
    returnCode=Read_DPRAM_Floats_32(Id_DSP, direc, longitud, datos);
    CheckReturnCode(returnCode);
    return;
}

void Escribir_flotantes_32(ULONG direc, UINT longitud, float *datos)
{
    UINT returnCode;
    returnCode=Write_DPRAM_Floats_32(Id_DSP, direc, longitud, datos);
    CheckReturnCode(returnCode);
    return;
}



void SetSemaforo (unsigned long Semaforo)
{
    unsigned long aux;
    UINT RetCode;

    aux = Semaforo;

    RetCode=Write_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&aux);
	CheckReturnCode(RetCode);
    return;
}

void EsperarSemaforo(unsigned long Semaforo)
{
    UINT returnCode;
    switch(Semaforo)
    {
        case ROJO :
                     do {
                            returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&Semaforo);
                            CheckReturnCode(returnCode);
                     } while( Semaforo == VERDE );
                     break;

        case VERDE :
                     do {
                            returnCode = Read_DPRAM_Words_32(Id_DSP,SEMAFORO,1,&Semaforo);
                            CheckReturnCode(returnCode);
                     } while( Semaforo == ROJO );
                     break;
    }
    return;
}


int
InicializarDSP(char Prog[])
{
    UINT    RetCode;                   /* Codigo de error de la lib. NETAPI */

	printf("Inicializando DSP...\n");
	RetCode = Global_Network_Reboot();
	CheckReturnCode(RetCode);

	printf("Abriendo procesador...\n");
	RetCode = Open_Processor_ID(&Id_DSP,NOMBRE_DSP,NULL);
	CheckReturnCode(RetCode);

    SetSemaforo(ROJO);  /* Parar al DSP */

	printf("Cargando y ejecutando programa COFF...\n"); /* Carga del codigo para DSP) */
	RetCode=Load_And_Run_File_LIA(Id_DSP,Prog);
	CheckReturnCode(RetCode);


    return(0);
}


