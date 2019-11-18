/****************************************************************************        
 *                                                                          *
 *  Loughborough Sound Images plc                                           *
 *  Loughborough Park                                                       *
 *  Ashby Road                                                              *
 *  Loughborough                                                            *
 *  Leics                         Tel : (01509)  634444                     *
 *  LE11  3NE                     FAX : (01509)  634450                     *
 *                                                                          *
 *  Copyright 1993, 1994, 1995, 1996 Loughborough Sound Images plc          *
 *                                                                          *
 *  Date     : Wednesday 8th September 1993                                 *
 *  Project  : NetAPI                                                       *
 *  Author   : G.Shooter                                                    *
 *  Modules  :                                                              *
 *                                                                          *
 *  History  :                                                              *
 *    Date            Ver           Change                                  *
 *  03/03/94        1.00            Created                                 *
 *  12/05/94        1.01            EDRAM support and minor bug fixes.      *
 *  03/08/94        1.02            Further bug fixes and a new config file *
 *                                      format.                             *
 *  15/11/94        1.10            Global update and SRead/SWrite bug      *
 *                                      IL module added.                    *
 *  05/07/95        1.11            Added MDC44S/T module support and QPCS  *
 *                                      carrier board.                      *
 *  01/08/95        1.12            Warm_Restart and GNR_Without_Kernels    *
 *                                      functions added.                    *
 *  01/07/96        1.13            Added support for paging in QPCSs to    *
 *                                      the same memory location.           *
 *                                                                          *
 ****************************************************************************/

#pragma message ("All warnings produced when this file is compiled are commented with ")
#pragma message ("reasons for their existance. ")

#ifdef __WATCOMC__
/*WQPCSLIA.ASM.*/
#pragma aux LIA_FRead_Block_EM parm [];
#pragma aux LIA_FWrite_Block_EM parm [];
#pragma aux LIA_Read_Block_EM parm [];
#pragma aux LIA_Write_Block_EM parm [];
/*WFASTLIA.ASM.*/
#pragma aux LIA_Read parm [];
#pragma aux LIA_Write parm [];
#pragma aux LIA_Read_Block parm [];
#pragma aux LIA_Write_Block parm [];
#pragma aux LIA_FRead_Block parm [];
#pragma aux LIA_FWrite_Block parm [];
/*DPRAM.ASM.*/
#pragma aux Send_Word_Fast parm [];
#else

#endif

/*
  Some unreachable code has been deliberately left in. e.g.
  return (error_code);
  break;

  The break has been left in.
*/


#define IO_MAPPED

#ifndef _DOS
    #define _DOS
#endif

#ifdef WIN32
    int Read_IO_Word(int Port_Address);
    void Write_IO_Word(int Port_Address, int Value);
#endif

#include "c4xapp.h"     

#define    A_RESET             0x1
#define    B_RESET             0x2
#define    GLOB_RESET          0x4
#define    NOT_INITIALISED     -1
#define    RESET_MASK_VAL      0xfff8
				
#define    ENABLED             1
#define    DISABLED            0


#ifdef WIN32
    #define    FAST_MODE           DISABLED     /* Windows 95 / NT  */
#else
    #define    FAST_MODE           ENABLED      /* DOS / Windows    */
#endif

/*  
   !! THIS ONLY APPLIES TO THE Load_And_Run_DPRAM FUNCTIONS AND
      LIA SEND WORD SUBFUNCTIONS                                !!
   
   The FAST_MODE definition is used to choose whether to use the C version
   of the code to send a word to the board and complete the handshake
   requirements, or use a "quick-and-dirty" assembler version. The assembler 
   version gives a speed up of about 30x over the C version, but is harder 
   to understand.

   !! FAST_MODE IS NOT USED IN WIN32 IMPLEMENTATIONS !!


   Windows 95 / Windows NT Support
   -------------------------------

   Implemented : December 1995, January 1996

   Support added for Windows 95, and Windows NT.

   To build either implementation please ensure that WIN32 is defined,
   and the appropriate driver library is incorporated.

   !! Windows 95/NT users must call Clear_All_Lib_Memory() when finished !!
*/

#ifdef WIN32
 #pragma message("---->  NetAPI Windows 95/NT build")
#else
 #pragma message("---->  NetAPI Standard build")
#endif
				

/****************************************************************************
 *                                                                          *
 *       Global variables                                                   *
 *                                                                          *
 ****************************************************************************/
 
UINT                Global_Bank1_Base_Address;
UINT                Global_JTAG_IO_Address;
UINT                Global_LIA_Address;
UINT                Global_DB40_Run;

BOOLEAN             Config_File_Read        = FALSE;
int                 Current_Processor_ID    = NOT_INITIALISED;
int                 Current_Board_ID        = NOT_INITIALISED;
int                 Current_JTAG_Chain      = NOT_INITIALISED;
UINT                Current_Board_Port_Address;                   
CFR_CONFIG         *Ptr_Config_File;  
CFR_ID              CFR_Identity;
BOARD_DESCRIPTION  *Ptr_Board_Descript_Table;
PROC_ID            *Ptr_Proc_Descript_Table;        
JTAG_FILE_TABLE    *Ptr_JTAG_File_Table;               
UINT                Num_JTAG_Scan_Chains;
char                Global_Board_Dat_File[FILE_NAME_LENGTH];

/* global within this file only */                                         

ULONG               Global_Function_Address; 
ULONG               Global_Data_Address;
ULONG               Global_Link_Address;

/*
   The TIMER1 is used for memory refresh on the MDC40D module. This timer
   is setup by writing to the TIMER1 registers with the values defined here.
   The top 2 bits of the control register are set to ensure that refresh
   continues while the C40 is in Emulator mode. This feature of the C40 is
   not documented in the C40 User Guide.
*/

#define  C40_TIMER1_CONTROL_ADDR    0x100030
#define  C40_TIMER1_CONTROL_VALUE   0xc00002c1
#define  C40_TIMER1_PERIOD_ADDR     0x100038
#define  C40_TIMER1_PERIOD_VALUE    0x80
#define  DRAM_REFRESH_PERIOD        0x9c
#define  CLOCK_SPEED_SCALING_FACTOR 0x28

/****************************************************************************
 *                                                                          *
 *                                                                          *
 *   Local function prototype definition                                    *
 *                                                                          *
 *                                                                          *
 ****************************************************************************/

/*
Private function prototypes
*/
/* GLOBAL NETWORK REBOOT */
UINT Reset_All_Proc_Statuses(void);
void Create_JTAG_Chain_Table (void);
UINT Create_Board_Descriptions (void);    
UINT Create_Proc_Descriptions (void);
void Reset_All_System_Boards (void);
UINT Warm_Init_All_Boards (void);    
UINT Initialise_All_Boards (void);                  
void Assign_Board_Type (BOARD_DESCRIPTION *);      
void Clear_All_Software_Breaks (PROC_ID *ptr_proc_handle);
UINT Target_Changeover (int proc_id);
UINT Read_In_JTAG_Chains (void); 
void Increment_File_Name (char *ptr_filename);
UINT Read_In_Scan_Chain (FILE *file_stream, UINT file_count, 
                                                    UINT *u_procs_in_chain);
void Read_New_Line( FILE * );
void Skip_Initial_Comments( FILE * );                                                                                     
UINT Get_Host_Board_ID (char *proc_name, int *board_id); 
UINT Create_Board_Dat_File (char *filename);   
UINT Add_DPC_JTAG_Defaults (void);     
void Board_Changeover (int proc_id, char *ptr_processor_name);
void Unmake_Board_Ready(int board_id);
void Make_Board_Ready(int board_id);
void JTAG_Chain_Changeover (int proc_id, int board_id);
UINT Set_Memory_Map (PROC_ID *ptr_proc_handle);
void Clear_Errors (void);
void Get_JTAG_Chain_ID (char *ptr_proc_name, int *ptr_JTAG_id);                                                                            
UINT Add_Memory_Map(PROC_ID* , ULONG , ULONG );
UINT Clear_Memory_Map(PROC_ID* );


/* LIA */
UINT Send_BOOT_Header_LIA (PROC_ID *ptr_proc_handle, ULONG ul_entry,
                PROC_ID *ptr_dest_handle);
UINT Send_DRAM_Header_LIA (PROC_ID *ptr_proc_handle, PROC_ID *ptr_dest_handle);
UINT Send_BOOT_Trailer_LIA(PROC_ID *ptr_proc_handle); 
UINT Send_Word_LIA(PROC_ID *ptr_proc_handle, ULONG ul_data_word );
UINT Download_Section_Data_LIA (PROC_ID *ptr_proc_handle, 
                ULONG ul_block_size, ULONG   ul_address, FILE *fp_file_ptr);
UINT Send_File_LIA (PROC_ID *ptr_proc_handle, char *filename,
                PROC_ID *ptr_dest_handle);

			      
/* DPRAM */        
UINT Send_BOOT_Trailer_DPRAM (PROC_ID *ptr_proc_handle,
                PATH_TABLE *ptr_path_table);
UINT Send_BOOT_Header_DPRAM (PROC_ID *ptr_proc_handle, ULONG ul_entry,
                PATH_TABLE *ptr_path_table, PROC_ID *ptr_dest_handle);
UINT Send_DRAM_Header_DPRAM (PROC_ID *ptr_proc_handle, PROC_ID *ptr_dest_handle);
UINT Send_File_DPRAM (PROC_ID *ptr_proc_handle, PATH_TABLE *ptr_path_table,
                char *filename, PROC_ID *ptr_dest_handle);
UINT Download_Section_Data_DPRAM (PROC_ID *ptr_proc_handle, ULONG   ul_block_size, 
				ULONG   ul_address, FILE    *fp_file_ptr);        
UINT Send_Word_DPRAM (PROC_ID *, ULONG);
UINT Send_Final_Primary_Word_DPRAM(PROC_ID *ptr_proc_handle, ULONG   ul_data_word );
UINT Send_Final_Secondary_Word_DPRAM(PROC_ID *ptr_proc_handle, ULONG   ul_data_word );
UINT Load_Monitor(PROC_ID *ptr_proc_handle, char *file_name);
ULONG  Read_Word( FILE *fp);
UINT Send_Word_Fast(ULONG ul_data_word, UINT Memory_Segment,
                UINT Port_Address, UINT DPRAM_Offset);


/* UTILITIES */                  
UINT Print_CFR_Error_Message (CFR_ERROR_CODE cfr_error_code);
UINT Set_Processor_Loaded (PROC_ID *ptr_proc_handle);
UINT Is_LIA_Supported (PROC_ID *ptr_proc_handle, BOARD_TYPE_ENUM *board_type);
UINT Is_DPRAM_Supported (PROC_ID *ptr_proc_handle, BOARD_TYPE_ENUM *board_type);
UINT Is_Within_Valid_Memory (PROC_ID *ptr_proc_handle, ULONG start_address, UINT len,
			     BOOLEAN dpram);
UINT Put_Control_Register( UINT Current_Control_Register );           
UINT Put_Control2_Register( UINT Current_Control_Register );           
UINT Get_Control_Register( UINT *Current_Control_Register );      
UINT Put_Config_Register( UINT Current_Config_Register );                                       
UINT Get_Config_Register (UINT *Current_Config_Register);                                                                                        


/* C4X CONVERSIONS */
UINT C4x_To_IEEE_32(void *, UINT );
UINT IEEE_To_C4x_32(void *, UINT );

/* JTAG REGISTER ACCESSES */
UINT Get_U_Register_Value( PROC_ID *, UINT , UINT *, UINT * );
UINT Get_UL_Register_Value(  PROC_ID *, UINT , ULONG *, UINT * );
UINT Get_Dbl_Register_Value(  PROC_ID *, UINT , double *, UINT * );
UINT Get_Bitfield_Register_Value( PROC_ID *, UINT , ULONG *, UINT * );
UINT Put_U_Register_Value(  PROC_ID *, UINT , UINT *);
UINT Put_UL_Register_Value(  PROC_ID *, UINT , ULONG *);
UINT Put_Dbl_Register_Value( PROC_ID *, UINT , double *);
UINT Put_Bitfield_Register_Value( PROC_ID *, UINT , ULONG *);

/* JTAG RUN FUNCTIONS */
void Install_Software_Breaks( PROC_ID * );
void Uninstall_Software_Breaks( PROC_ID * );
UINT Run_Target_Free( PROC_ID *);
UINT Run_Target_Step( PROC_ID * , UINT );
UINT Run_Target_Cond( PROC_ID * );
UINT Get_All_Registers( PROC_ID *);


/* TWIN MODULE AND DPC THROUGH ROUTING CODE */
UINT Does_Board_Support_LIA (PROC_ID *ptr_proc_handle);
UINT Send_Path_Table_LIA (PROC_ID *ptr_proc_handle, PATH_TABLE *ptr_path_table);
UINT Does_Board_Support_DPRAM (PROC_ID *ptr_proc_handle);
int  Find_C40_Comms_Link_ID (int source_proc, int target_proc);

BOOLEAN Is_Twin_Module(const char *);

UINT Create_Proc_Id_List (PROC_ID *ptr_proc_handle, int *ptr_proc_identities);
UINT Send_Path_Table_DPRAM (PROC_ID *ptr_proc_handle, PATH_TABLE *ptr_path_table);

UINT Load_DPC_Monitors (BOARD_DESCRIPTION *ptr_board);
UINT Load_QPCS_Monitors (BOARD_DESCRIPTION *ptr_board);
UINT Load_QPC_Monitors (BOARD_DESCRIPTION *ptr_host_board);


/* Generic path table creation routines. */
UINT Create_Board_Proc_ID_List (UINT u_board_id, int *ptr_id_list,
                        UINT *ptr_num_processors, UINT *max_num_entry_pts_p);
UINT Create_Entry_Point_List (int u_board_id, int *ptr_id_list,
                             UINT *ptr_num_processors, UINT max_num_entry_pts);
UINT List_All_Procs_On_Board(int u_board_id, int *ptr_id_list,
                             UINT *ptr_num_processors);
UINT Create_Monitor_Path_Table  (PROC_ID *ptr_proc_handle,
PATH_TABLE *ptr_path_table, int *ptr_target_proc_id, UINT max_num_entry_pts);
UINT Create_Path_Table  (PROC_ID *ptr_proc_handle, PATH_TABLE *ptr_path_table,
                     int *ptr_target_proc_id, UINT max_num_entry_pts);
int Find_Next_Proc_In_Chain (int u_proc_id, UINT link_number,
                             UINT procs_to_check, int *ptr_procs_array);


/* EDRAM module functions */
UINT Initialise_EDRAM_Modules(void);

#ifdef WIN32                                /* Windows 95/NT Only           */

    extern BOOLEAN Load_VxD(char *);
    void Unload_VxD(void);

    extern void Install_Handler(UINT irq_number, FUNCPTR irq_handler);

    void Read_Mem_Block(int Address, int Length, ULONG *lpBuffer);
    void Write_Mem_Block(int Address, int Length, ULONG *lpBuffer);

#endif

/****************************************************************************
 *                                                                          *
 * When we check the status of the LIA XMIT directly then we                *
 *  selectively mask out the appropriate bit.                               *
 *                                                                          *           
 ****************************************************************************/
 
unsigned int    u_module_xmit_masks[] = {   A_XMIT_DATA_EMPTY, \
                                            B_XMIT_DATA_EMPTY, \
                                            C_XMIT_DATA_EMPTY, \
                                            D_XMIT_DATA_EMPTY };

/****************************************************************************
 *                                                                          *
 * When we check the status of the LIA REC directly then we                 *
 *  selectively mask out the appropriate bit.                               *
 *                                                                          *
 ****************************************************************************/
 
unsigned int    u_module_rec_masks[]  = { A_REC_DATA_FULL, B_REC_DATA_FULL, \
                                          C_REC_DATA_FULL, D_REC_DATA_FULL };


/****************************************************************************
 *                                                                          *
 *   When we write to the LIA we have to add a bias from the                *
 *   base to the appropriate registers. The offsets are stored              *
 *   in the array below.                                                    *
 *                                                                          *
 ****************************************************************************/
 
unsigned int    u_module_LIA_reg_base[] = { 4, 12, 0, 8 };
                
 
/****************************************************************************
 *                                                                          *
 *              INCLUDE THE FILE INCLUDING THE FUNCTIONS THEMSELVES         *
 *                                                                          *
 ****************************************************************************/
 
#include "c4xinit.c"
#include "c4xappu.c"

/*EOF*/    
