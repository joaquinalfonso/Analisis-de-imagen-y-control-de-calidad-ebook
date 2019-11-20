#ifndef DSPERROR_C
#define DSPERROR_C

/* ========================================================================== */
/*                                                                            */
/*  Archivo....: dsperror.c (NetAPI para QPC/C40S)                            */
/*                                                                            */
/*  Contenido..: Codigo para HOST. Muestra la descripcion del error           */
/*                                                                            */
/*  Asignatura.: Sistemas Informaticos                                        */
/*                                                                            */
/*  Practica...: Proyecto fin de carrera                                      */
/*                                                                            */
/*  Alumno.....: Francisco Moragues Molines                                   */
/*                                                                            */
/*  Tutor......: Francisco Ibarra Pico                                        */
/*                                                                            */
/* ========================================================================== */
#include <stdio.h>

static char *RC_ErrorMessages[] = {
  "RC_NO_ERROR						\nNo error has occurred",
  "RC_INVALID_MODULE_ID			\nThe module id given is not within the valid range",
  "RC_TIMEOUT			  			\nA function has been unable to complete",
  "RC_INVALID_ADDRESS			\nThe memory address given is out of range",
  "RC_CANNOT_OPEN_FILE			\nThe file name supplied can not be found",
  "RC_INVALID_FORMAT				\nThe boot file opened is not a COFF format file",
  "RC_MEM_ACCESS_ERROR			\nThe memory read or write requested could not be performed",
  "RC_CANNOT_SELECT				\nI don't know",
  "RC_NO_MODULES					\nThe target board has no modules resident upon it",
  "RC_TARGET_ERROR				\nThe JTAG scan chain could not communicate with the target\nprocessor. The processor has probably crashed and must be\nreset and then rebooted",

  "RC_INVALID_REGISTER_ID		\nThe register supplied, to be read from or written to, is not\na valid one",
  "RC_PROC_RUNNING				\nThe target processor is running",
  "RC_PROC_STOPPED				\nThe target processor has been stopped",
  "RC_BREAK_ERROR					\nA JTAG error occured while trying to perform a break point",
  "RC_SINGLE_STEP_ERROR			\nA JTAG error occured while trying to perform a single step",
  "RC_COND_EXEC_ERROR			\nA JTAG error occured while trying to perform a conditional run",
  "RC_INVALID_SEC_PORT_ADDR	\nThe second host IO connection supplied in the NETAPI.CFG\nfile is invalid",
  "RC_SOFTWARE_RESET_ERROR		\nAn unrecoverable error occured during the reset functions",
  "RC_PROC_STILL_RUNNING		\nThe target processor is still running. JTAG functions require\nthat the processor is stopped (using\nSTOP(processor_identity_handle)) before they can be run",
  "RC_NET_FILE_NOT_FOUND		\nThe netapi.cfg file could not be found",

  "RC_UNABLE_TO_ALLOC_MEM		\nThe program was unable to allocate sufficient memory to store\nall of the required information. Try freeing more memory and\nthen running the program again",
  "RC_CONFIG_FILE_ERROR			\nAn error has been detected while reading the NETAPI.CFG file\nA suitable error message will be printed for a netapi.cfg\nsyntax error. This also occurs for a processor name given\nin the boardxxx.cfg files that is not matched in the netapi.cfg file",
  "RC_INVALID_PROCESSOR_ID		\nThe processor identity number found within the library\nfunctions is outside the valid range. This will occur\nif the processor name supplied by the user does not exist\nin the NETAPI.CFG file",
  "RC_DSP_ERROR					\nThe target DSP processor has failed to respond to the\nrequest from the host PC. The processor has most likely\nstopped running",
  "RC_ZERO_BLOCK_SIZE			\nAn error has been found within the COFF file. A code\nblock size of zero has been found",
  "RC_DPRAM_OVERRUN				\nThe memory read or write operation is trying to read/write\nto memory locations that are not within the dual port RAM",
  "RC_INVALID_FUNCTION			\nThe requested functionality is not supported for the\ntarget processor. For example trying to use LIA operations\non a DPC/C40B board will return this error code",
  "RC_UNKNOWN_MODULE_TYPE		\nThe module type given in the NETAPI.CFG file is not\nrecognised as a valid module type",
  "RC_INVALID_PATH_TO_DSP		\nA communication port path can not be found to the target\nprocessor from the host PC. The target processor can not\nbe booted",
  "RC_PATH_ALREADY_LOADED		\nThe path from the host PC to the target processor\nhas already been booted. The boot code can not therefore\nbe routed to the target processor",

  "RC_INVALID_BOARD_TYPE		\nThe board type given in the NETAPI.CFG file is not\na recognised type",

  "Se superó el tiempo de espera mientras el HOST\nestaba en el bucle de transferencia hacia el DSP.\nEl DSP no responde",
  "Se superó el tiempo de espera mientras el HOST\nestaba en el bucle de transición de carga a descarga al DSP.\nEl DSP no responde",
  "Se superó el tiempo de espera mientras el HOST\nestaba en el bucle de transferencia desde el DSP.\nEl DSP no responde",
};

void CheckReturnCode(UINT returnCode)
{
  char sx[99];

//  sprintf(sx,"\nERROR[%i]=%s.\n",returnCode,RC_ErrorMessages[returnCode]);
//  wxMessageBox(sx,"Error DSP",wxICON_EXCLAMATION);

  if(returnCode) {
    printf("\nERROR_MESSAGE[%i]=%s.\n",returnCode,RC_ErrorMessages[returnCode]);
    exit(-1);
    }
}
#endif
/* ========================================================================== */
