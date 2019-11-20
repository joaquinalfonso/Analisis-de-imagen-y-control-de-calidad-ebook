/* =======================================================================	*/
/* Direcciones de memoria DPRAM usadas para comunicacion HOST-DSP           */
/* =======================================================================	*/
#ifndef __DIRS_H__
#define __DIRS_H__

#define ROJO            1
#define VERDE           0
#define MAX_VENTANA     64
#define ERROR           -1
#define MAX_PLANOS      10
#define MAX_PIEZAS      5
#define DSP1            1
#define DSP2            2
#define NOP             0
#define VEN             1
#define RES             2

/*FRAME - GRABBER*/
#define BUFFER		0x0A0100000L	/* Array de datos que se transmiten */
#define MAX_BUFFER	0x400			/* Tamanyo del array BUFFER (1K) */
#define MAX_PLANOS  10
#define SEMAFORO    0x0A0100500L    /* Variable cerrojo para comunicacion */



/*PCI*/
#define SEMAFORO0       0xC0800030L
#define SEMAFORO1       0xC0800031L
#define SEMAFORO2       0xC0800032L
#define SEMAFORO3       0xC0800033L
#define SEMAFORO4       0xC0800034L
#define SEMAFORO5       0xC0800035L
#define SEMAFORO6       0xC0800036L
#define SEMAFORO7       0xC0800037L


#define BASE            0xC0000000L
#define MEM_GLOBAL      0x80000000L
#define MEM_LOCAL       0x00300000L


#endif
/* =======================================================================	*/
