
/******************************************************/
/*                                                    */
/*  Function     : checkReturnCode                    */
/*                                                    */
/*  Purpose      : Prints appropriate error message   */
/*                 and exits if error. Returns if     */
/*                 no error. Adapted from             */
/*                 Print_Error_Code                   */
/*                                                    */
/*                                                    */
/*  Parameters   : 1  Error message returned from     */
/*                    C4XAPP library functions.       */
/*                                                    */
/*  Returns      : None                               */
/*                                                    */
/******************************************************/


void checkReturnCode(UINT error_code)
{  
   switch (error_code)
   {
      case RC_NO_ERROR:
      break; 

      case RC_INVALID_MODULE_ID:
	      printf("\nError message is RC_INVALID_MODULE_ID\n");
	      printf("The module id given is not within the valid range.\n");
	      exit(-1);
      break;

      case RC_TIMEOUT:
	       printf("\nError message is RC_TIMEOUT\n");
	       printf("A function has been unable to complete.\n");
	       exit(-1);         
      break;

      case RC_INVALID_ADDRESS:
	      printf("\nError message is RC_INVALID_ADDRESS\n");
	      printf("The memory address given is out of range.\n");
	      exit(-1);         
      break;

      case RC_CANNOT_OPEN_FILE:
	      printf("\nError message is RC_CANNOT_OPEN_FILE\n");
	      printf("The file name supplied cannot be found.\n");
	      exit(-1);
      break;

      case RC_INVALID_FORMAT:
	      printf("\nError message is RC_INVALID_FORMAT\n");
	      printf("The boot file opened is not a COFF file.\n");
	      exit(-1);
      break;
      

      case RC_MEM_ACCESS_ERROR:
	      printf("\nError message is RC_MEM_ACCESS_ERROR\n");
	      printf("The memory read or write requested could not be performed.\n");
	      exit(-1);
      break;

      case RC_NO_MODULES:
	      printf("\nError message is RC_NO_MODULES\n");
	      printf("The target board has no modules resident upon it.\n");
	      exit(-1);
      break;

      case RC_TARGET_ERROR:
	      printf("\nError message is RC_TARGET_ERROR\n");
	      printf("The JTAG scan chain could not communicate with the target\n");
	      printf("processor. The Processor has probably crashed and must be\n");
	      printf("reset and then rebooted.\n");
	      exit(-1);
      break;

      case RC_INVALID_REGISTER_ID:
	      printf("\nError message is RC_INVALID_REGISTER_ID\n");
	      printf("The register supplied to read to or be written from,is not\n");
	      printf("valid.\n");
	      exit(-1);
      break;

      case RC_PROC_RUNNING:
	      printf("\nError message is RC_PROC_RUNNING\n");
	      printf("The target processor is running.\n");
	      exit(-1);
      break;

      case RC_PROC_STOPPED:
	      printf("\nEror message is RC_PROC_STOPPED\n");
	      printf("The target processor has been stopped\n");
	      exit(-1);
      break;

      case RC_BREAK_ERROR:
	      printf("\nError message is RC_BREAK_ERROR\n");
	      printf("A JTAG error has occured while trying to perform a break point.\n");
	      exit(-1);
      break;

      case RC_SINGLE_STEP_ERROR:
	      printf("\nError message is RC_SINGLE_STEP_ERROR\n");
	      printf("A JTAG error occured while trying to perform a single\n");
	      printf("step.\n");
	      exit(-1);
      break;

      case RC_COND_EXEC_ERROR:
	      printf("\nError message is RC_COND_EXEC_ERROR\n");
	      printf("A JTAG error occured while trying to perform a conditional\n");
	      printf("execution.\n");
	      exit(-1);
      break;

      case RC_INVALID_SEC_PORT_ADDR:
	      printf("\nError message is RC_INVALID_SEC_PORT_ADDR\n");
	      printf("The second host IO connection supplied in the NETAPI.CFG\n");
	      printf("file is invalid.\n");
	      exit(-1);
      break;

      case RC_SOFTWARE_RESET_ERROR:
	      printf("\nError message is RC_SOFTWARE_RESET_ERROR\n");
	      printf("An unrecoverable error occured during the reset\n");
	      printf("functions.\n");
	      exit(-1);
      break;
      
      case RC_PROC_STILL_RUNNING:
	      printf("\nError message is RC_PROC_STILL_RUNNING\n");
	      printf("The target processor is still running. JTAG functions require\n");
	      printf("that the processor is stopped (using\n");
	      printf("STOP(processor_identity_handle))before they can be run.\n");
	      exit(-1);
      break;

      case RC_NET_FILE_NOT_FOUND:
	      printf("\nError message is RC_NET_FILE_NOT_FOUND\n");
	      printf("The netapi.cfg file can not be found.\n");
	      exit(-1);
      break;

      case RC_UNABLE_TO_ALLOC_MEM:
	      printf("\nError message is RC_UNABLE_TO_ALLOC_MEM\n");
	      printf("The program was unable to allocate sufficient memory to store\n");
	      printf("all of the required information. Try freeing more memory and\n");
	      printf("then running the program again.\n");
	      exit(-1);
      break;

      case RC_CONFIG_FILE_ERROR:
	      printf("\nError message is RC_CONFIG_FILE_ERROR\n");
	      printf("An error has been detected while reading the NETAPI.CFG file.\n");
	      printf("A suitable error message will be printed if this is within\n");
	      printf("the netapi.cfg file reader. This can also occur if a\n");
	      printf("processor name given in the boardxxx.cfg files is not in the\n");
	      printf("netapi.cfg file.\n");
	      exit(-1);
      break;

      case RC_INVALID_PROCESSOR_ID:
	      printf("\nError message is RC_INVALID_PROCESSOR_ID\n");
	      printf("The processor identity number found within the library\n");
	      printf("functions is outside the valid range. This will occur\n");
	      printf("if the processor name supplied by the user does not exist\n");
	      printf("in the netapi.cfg file.\n");
	      exit(-1);
      break;

      case RC_DSP_ERROR:
	      printf("\nError message is RC_DSP_ERROR\n");
	      printf("The target DSP processor has failed to respond to the\n");
	      printf("request from the host PC. The processor has most likely\n");
	      printf("stopped running.\n");
	      exit(-1);
      break;
      
      case RC_ZERO_BLOCK_SIZE:
	      printf("\nError message is RC_ZERO_BLOCK_SIZE\n");
	      printf("An error has been found within the COFF file. A code\n");
	      printf("block size of zero has been found.\n");
	      exit(-1);
      break;

      case RC_DPRAM_OVERRUN:
	      printf("\n Error message is RC_DPRAM_OVERRUN\n");
	      printf("The memory read or write operation is trying to read/write\n");
	      printf("to memory locations that are not within the dual port RAM.\n");
	      exit(-1);
      break;

      case RC_INVALID_FUNCTION:
	      printf("\nError message is RC_INVALID_FUNCTION\n");
	      printf("The requested functionality is not supported for the\n");
	      printf("target processor. For example try to use LIA operations\n");
	      printf("on a DPC/C40B board will return this error code.\n");
	      exit(-1);
      break;

      case RC_UNKNOWN_MODULE_TYPE:
	printf("\nError message is RC_UNKNOWN_MODULE_TYPE\n");
	      printf("The module type given in the NETAPI.CFG file is not\n");
	      printf("recognised as a valid module type.\n");
	      exit(-1);
	break;

      case RC_INVALID_PATH_TO_DSP:
	      printf("\nError message is RC_INVALID_PATH_TO_DSP\n");
	      printf("A communication port path can not be found to the target\n");
	      printf("processor from the host PC. The target processor can not\n");
	      printf("be booted.\n");
	      exit(-1);
      break;

      case RC_PATH_ALREADY_LOADED:
	      printf("\nError message is RC_PATH_ALREADY_LOADED\n");
	      printf("The path from the host PC to the target processor \n");
	      printf("has already been booted. The boot can not therefore\n");
	      printf("be routed to the target processor.\n");
	      exit(-1);
      break;

      case RC_INVALID_BOARD_TYPE:
	      printf("\nError message is RC_INVALID_BOARD_TYPE\n");
	      printf("The board type given in the NETAPI.CFG file is not\n");
	      printf("a recognised type.\n");
	      exit(-1);
      break;


		default: 
			printf("An UNKNOWN ERROR has occurred\n");
			exit(-1);

}
}/*End of function checkReturnCode*/



