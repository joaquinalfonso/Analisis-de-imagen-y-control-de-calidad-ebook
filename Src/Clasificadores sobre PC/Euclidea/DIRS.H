/* =======================================================================	*/
/* Direcciones de memoria DPRAM usadas para comunicacion HOST-DSP           */
/* =======================================================================	*/
#ifndef _DIRS_
#define _DIRS_

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


#define BUFFER		0x0A0100000L	/* Array de datos que se transmiten */
#define MAX_BUFFER	0x400			/* Tamanyo del array BUFFER (1K) */
#define MAX_PLANOS  10

#define SEMAFORO	0x0A0100500L	/* Variable cerrojo para comunicacion */


#endif
/* =======================================================================	*/
