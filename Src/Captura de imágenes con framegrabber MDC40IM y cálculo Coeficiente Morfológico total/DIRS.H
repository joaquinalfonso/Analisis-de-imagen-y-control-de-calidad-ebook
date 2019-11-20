/* =======================================================================	*/
/* Direcciones de memoria DPRAM usadas para comunicacion HOST-DSP             	*/
/* =======================================================================	*/
#ifndef _DIRS_
#define _DIRS_

#define BUFFER		0x0A0100000L	/* Array de datos que se transmiten */
#define MAX_BUFFER      0x400           /* Tamanyo del array BUFFER (1K) */
#define SQRT_MAX_BUFFER	32		/* Raiz cuadrada de MAX_BUFFER */

#define SEMAFORO        0x0A0100500L    /* Variable cerrojo para comunicacion */


//typedef unsigned long   DATA_TYPE;              /* Tipo de datos (ULONG o float) */
typedef float DATA_TYPE;              /* Tipo de datos (ULONG o float) */
//#define DATA_FLOATS

#endif
/* =======================================================================	*/
