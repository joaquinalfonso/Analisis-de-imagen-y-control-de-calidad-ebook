/* =======================================================================	*/
/* Direcciones de memoria DPRAM usadas para comunicacion HOST-DSP           */
/* =======================================================================	*/
#ifndef _DIRS_
#define _DIRS_

#define ROJO	0
#define VERDE	1


#define BUFFER		0x0A0100000L	/* Array de datos que se transmiten */
#define MAX_BUFFER	0x400			/* Tamanyo del array BUFFER (1K) */


#define SEMAFORO	0x0A0100500L	/* Variable cerrojo para comunicacion */


#endif
/* =======================================================================	*/
