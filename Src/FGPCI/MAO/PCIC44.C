#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

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

CARD_ID CardID;


void
CerrarYSalir(int Valor)
{
    PCIC44ErrorCode ret;

    ret = CloseCard(CardID);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Cerrar la taarjeta (%d)\n",ret);
        exit(ERROR);
    }

    exit( Valor );
}

void
PonerValorSemaforo(int NumSemaforo, ULONG Valor)
{
    ULONG semaforo;
    PCIC44ErrorCode ret;

    semaforo = Valor;

    ret = WriteMailbox(CardID, NumSemaforo, semaforo);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Escribir en Mailbox %d (%d)\n",NumSemaforo,ret);
        exit(ERROR);
    }
}

ULONG
ObtenerValorSemaforo(int NumSemaforo)
{
    ULONG semaforo;
    PCIC44ErrorCode ret;

    ret = ReadMailbox(CardID, NumSemaforo, &semaforo);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Escribir en Mailbox %d (%d)\n",NumSemaforo,ret);
        exit(ERROR);
    }

    return ( semaforo );
}


void
InicializarPCIC44(char ProgDSP1[], char ProgDSP2[])
{
    PCIC44ErrorCode ret;

    /** Inicializar el DSP **/
    ret = InitPCIC44Lib();
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede inicializar la Runtime Support Library (%d)\n", ret);
        exit(ERROR);
    }

    ret = GetCardIdentity("BOARD_A", &CardID);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se encuentra el ID de tarjeta para BOARD_A (%d)\n", ret);
        exit(ERROR);
    }

    ret = OpenCard(CardID);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Abrir la tarjeta (%d)\n", ret);
        exit(ERROR);
    }

    if( strcmp( ProgDSP1, "" ) ) {
        PonerValorSemaforo( 0, ROJO );
        printf("Cargando y Ejecutando programa DSP1\n");
        ret = LoadAndRunObjectFile(CardID, (WORD) 1, ProgDSP1);
        if ( ret != PCIC44_SUCCESS ) {
            fprintf(stderr,"ERROR: No se puede Descargar/Ejecutar programa sobre el DSP1 (%d)\n", ret);
            CerrarYSalir(ERROR);
        }
    }

    if( strcmp( ProgDSP2, "" ) ) {
        PonerValorSemaforo( 1, ROJO );
        printf("Cargando y Ejecutando programa DSP2\n");
        ret = LoadAndRunObjectFile(CardID, (WORD) 2, ProgDSP2);
        if ( ret != PCIC44_SUCCESS ) {
            fprintf(stderr,"ERROR: No se puede Descargar/Ejecutar programa sobre el DSP2 (%d)\n", ret);
            CerrarYSalir(ERROR);
        }
    }

    return;
}

void
EscribirBloqueFlotantes(ULONG Direccion, ULONG NumDatos, float *Buffer)
{
    PCIC44ErrorCode ret;

    ret = Write_Shared_Mem_Block_Float(CardID, Direccion, NumDatos, Buffer, FALSE);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Escribir en memoria compartida (%d)\n", ret);
        CerrarYSalir(ERROR);
    }

}

void
EscribirBloqueEnteros(ULONG Direccion, ULONG NumDatos, ULONG *Buffer)
{
    PCIC44ErrorCode ret;

    ret = Write_Shared_Mem_Block(CardID, Direccion, NumDatos, Buffer, FALSE);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Escribir en memoria compartida (%d)\n", ret);
        CerrarYSalir(ERROR);
    }

}

void
LeerBloqueFlotantes(ULONG Direccion, ULONG NumDatos, float *Buffer)
{
    PCIC44ErrorCode ret;

    ret = Read_Shared_Mem_Block_Float(CardID, Direccion, NumDatos, Buffer, FALSE);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Leer en memoria compartida (%d)\n", ret);
        CerrarYSalir(ERROR);
    }
}

void
LeerBloqueEnteros(ULONG Direccion, ULONG NumDatos, ULONG *Buffer)
{
    PCIC44ErrorCode ret;

    ret = Read_Shared_Mem_Block(CardID, Direccion, NumDatos, Buffer, FALSE);
    if ( ret != PCIC44_SUCCESS ) {
        fprintf(stderr,"ERROR: No se puede Leer en memoria compartida (%d)\n", ret);
        CerrarYSalir(ERROR);
    }

}

