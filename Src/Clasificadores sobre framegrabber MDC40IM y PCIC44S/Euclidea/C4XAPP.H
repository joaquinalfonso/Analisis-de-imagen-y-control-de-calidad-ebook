/****************************************************************************
*                                                                           *
*   Loughborough Sound Images Plc                                           *
*   Loughborough Park                                                       *
*   Ashby Road                                                              *
*   Loughborough                                                            *
*   Leics                         Tel : (01509)  634444                     *
*   LE11  3NE                     FAX : (01509)  634450                     *
*                                                                           *
*   Software Copyright 1993-1996                                            *
*                                                                           *
*   Date     : 7th September 1993                                           *    
*   Project  : 320C40 library header file for JTAG interface                *
*              for mixed libraries.                                         *
*                                                                           *
*   History  :                                                              *
*     Date         Ver        Change                                        *
*   03/03/94        1.00            Created                                 *
*   12/05/94        1.01            EDRAM support and minor bug fixes.      *
*   03/08/94        1.02            Further bug fixes and a new config file *
*                                    format.                                *
*   15/11/94        1.10            Global update and SRead/SWrite bug      *
*                                    fixed. IL module added.                *
*   05/07/95        1.11            Added MDC44S/T module support and QPCS  *
*                                    carrier board.                         *
*   01/08/95        1.12            Warm_Restart and GNR_Without_Kernels    *
*                                    functions added.                       *
*                                                                           *
*****************************************************************************/





/****************************************************************************
 *                                                                          *
 *                                                                          *
 *   #include statements start here                                         *
 *                                                                          *
 *                                                                          *
 ****************************************************************************/
 
#include        <stdio.h>
#include        <conio.h>
#include        <string.h>
#include        <stdlib.h>
#include        <io.h>
#include        <ctype.h>
#include        <dos.h>
#include        <math.h>
#ifdef __WATCOMC__

#else
#include        <memory.h>
#endif      
#include        <float.h>
#include        <sys/types.h>
#include        <sys/stat.h>
#include        <process.h>

#include "parser.h"   

#ifdef WIN32
#include "windows.h"
#endif


/*********************************************
 *                                           *
 *                                           *
 *   #define statements start here           *
 *                                           *
 *                                           *
 *********************************************/

/******************************************************************************
 *       Type definitions for use with BOOL data type.                        *
 ******************************************************************************/
#ifndef True
#define              True                 1
#endif
#ifndef False
#define              False                0
#endif

#ifndef TRUE
#define              TRUE                 1
#endif
#ifndef FALSE
#define              FALSE                0
#endif

#ifndef DSP_NAME
#define              DSP_NAME             "320C40"
#endif

/******************************************************************************
 *       The library and the Modular monitor have to interface in their       *
 *       use of registers. Below is a set of indirections used to link the    *
 *       Modular Monitor register handler to the Standard Library register    *
 *       value block.                                                         *
 ******************************************************************************/
 /* All register indirection values */

typedef enum {                   
              _REG_R0      = 1,
              _REG_R1,      
              _REG_R2,      
              _REG_R3,      
              _REG_R4,      
              _REG_R5,      
              _REG_R6,      
              _REG_R7,      
              _REG_R8,      
              _REG_R9,      
              _REG_R10,     
              _REG_R11,
     
              _REG_F0,
              _REG_F1,      
              _REG_F2,      
              _REG_F3,     
              _REG_F4,      
              _REG_F5,      
              _REG_F6,      
              _REG_F7,      
              _REG_F8,      
              _REG_F9,      
              _REG_F10,     
              _REG_F11,

              _REG_E0,
              _REG_E1,      
              _REG_E2,      
              _REG_E3,     
              _REG_E4,      
              _REG_E5,      
              _REG_E6,      
              _REG_E7,      
              _REG_E8,      
              _REG_E9,      
              _REG_E10,     
              _REG_E11,

              _REG_AR0,
              _REG_AR1,      
              _REG_AR2,      
              _REG_AR3,      
              _REG_AR4,      
              _REG_AR5,      
              _REG_AR6,      
              _REG_AR7,      

              _REG_DP,    
              _REG_IR0,     
              _REG_IR1,    
              _REG_BK,    
              _REG_SP,    

              _REG_DIE,    
              _REG_IIE,    
              _REG_IIF,    

              _REG_RS,     
              _REG_RE,     
              _REG_RC,     

              _REG_ST, 
              _REG_PC, 

              _REG_ST_C,
              _REG_ST_V,
              _REG_ST_Z,
              _REG_ST_N,
              _REG_ST_UF,
              _REG_ST_LV,
              _REG_ST_LUF,
              _REG_ST_OVM,
              _REG_ST_RM,
              _REG_ST_PCF,
              _REG_ST_CF,
              _REG_ST_CE,
              _REG_ST_CC,
              _REG_ST_GIE,
              _REG_ST_PGIE,
              _REG_ST_SETC,
              _REG_ST_ANAL
              
        } REGISTER_INDIRECTION_VALUES;


/******************************************************************************
 *       Definitions to define the what key press is used to stop a failed    *
 *       singlestep over routine which doesnt come back.                      *
 ******************************************************************************/
#define  ESC                      27

/******************************************************************************
 *       Definitions to define the current processor state. ie its running    *
 *       a program, or in hold mode                                           *
 ******************************************************************************/
#define  PROC_RUNNING               0
#define  PROC_DEBUG                 2
#define  PROC_RESET                 2
#define  ENDED_WITH_SB              0x80

/******************************************************************************
 *       When we set the processor running we take a copy of all the regs     *
 *       in BEFORE_RUN and get back the regs into AFTER_RUN of                *
 *       Current_Register[..]                                                 *
 ******************************************************************************/
#define  BEFORE_RUN                 1
#define  AFTER_RUN                  0


/******************************************************************************
 *       These definitions are used to denote the offsets from the port       *
 *       addresses of the various registers that we will write to.            *
 ******************************************************************************/
#define    BANK1_BASE_ADDR     Global_Bank1_Base_Address
#define    BANK2_BASE_ADDR     Global_JTAG_IO_Address
#define    JTAG_BASE_ADDR      Global_JTAG_IO_Address
#define    LIA_BASE_ADDR       Global_LIA_Address

#define    CONTROL1_OFFSET          0x10  
#define    BLOCK_ENABLE_OFFSET      0x12
#define    CONTROL2_OFFSET          0x14  
#define    STATUS_OFFSET            0x14 
#define    CONFIG_OFFSET            0x18
#define    DP_RAM_BASE_OFFSET       0x1c
#define    IO_BANK2_BASE_OFFSET     0x1e 
#define    BASE_ADDRESS_REG_OFFSET  0x18

#define    DEFAULT_BASE_ADDRESS_OFFSET   0x20
/* 
       BLOCK 1
*/

#define    JTAG_STAT_0         0x0 + BANK1_BASE_ADDR
#define    JTAG_STAT_1         0x2 + BANK1_BASE_ADDR
#define    JTAG_STAT_2         0x4 + BANK1_BASE_ADDR
#define    JTAG_STAT_3         0x6 + BANK1_BASE_ADDR
#define    JTAG_CAPT_0         0x8 + BANK1_BASE_ADDR
#define    JTAG_CAPT_1         0xa + BANK1_BASE_ADDR
#define    JTAG_READ_BUFF      0xc + BANK1_BASE_ADDR
#define    JTAG_WRITE_BUFF     0xe + BANK1_BASE_ADDR
#define    CONTROL1            CONTROL1_OFFSET + BANK1_BASE_ADDR    /*  Read/Write   QPC write only */
#define    STATUS              STATUS_OFFSET + BANK1_BASE_ADDR      /*  Read     */
#define    CONFIGURE           CONFIG_OFFSET + BANK1_BASE_ADDR
#define    DP_RAM_BASE         DP_RAM_BASE_OFFSET + BANK1_BASE_ADDR
#define    IO_BANK2_BASE       IO_BANK2_BASE_OFFSET + BANK1_BASE_ADDR

/* QPCC40 and QPCC40S only */
#define    BASE_ADDRESS_REG    BASE_ADDRESS_REG_OFFSET + BANK1_BASE_ADDR

/* QPCC40S only */
#define    QPCS_BLOCK_ENABLE   0x12 + BANK1_BASE_ADDR
#define    QPCS_CONTROL2       0x14 + BANK1_BASE_ADDR
#define    QPCS_LIA_STATUS     0x1c + BANK1_BASE_ADDR
#define    QPCS_LIA_INTERRUPT  0x1c + BANK1_BASE_ADDR

/* 
       BLOCK 2
*/
#define    JTAG_CONT_0         0x0 + Global_JTAG_IO_Address
#define    JTAG_CONT_1         0x2 + Global_JTAG_IO_Address
#define    JTAG_CONT_2         0x4 + Global_JTAG_IO_Address
#define    JTAG_CONT_3         0x6 + Global_JTAG_IO_Address
#define    JTAG_CONT_4         0x8 + Global_JTAG_IO_Address
#define    JTAG_CONT_5         0xa + Global_JTAG_IO_Address
#define    JTAG_CONT_6         0xc + Global_JTAG_IO_Address
#define    JTAG_CONT_7         0xe + Global_JTAG_IO_Address
#define    JTAG_CONT_8         0x10 + Global_JTAG_IO_Address
#define    JTAG_CONT_9         0x12 + Global_JTAG_IO_Address
#define    JTAG_MINOR_CMD      0x14 + Global_JTAG_IO_Address
#define    JTAG_MAJOR_CMD      0x16 + Global_JTAG_IO_Address
#define    JTAG_CNT1_UPD_0     0x18 + Global_JTAG_IO_Address
#define    JTAG_CNT1_UPD_1     0x1a + Global_JTAG_IO_Address
#define    JTAG_CNT2_UPD_0     0x1c + Global_JTAG_IO_Address
#define    JTAG_CNT2_UPD_1     0x1e + Global_JTAG_IO_Address


/*
       LIA BLOCK
*/
#define     LIA_A_DATA_LO       0x04 + LIA_BASE_ADDR
#define     LIA_A_DATA_HI       0x06 + LIA_BASE_ADDR
#define     LIA_B_DATA_LO       0x0c + LIA_BASE_ADDR
#define     LIA_B_DATA_HI       0x0e + LIA_BASE_ADDR
#define     LIA_C_DATA_LO       0x00 + LIA_BASE_ADDR
#define     LIA_C_DATA_HI       0x02 + LIA_BASE_ADDR
#define     LIA_D_DATA_LO       0x08 + LIA_BASE_ADDR
#define     LIA_D_DATA_HI       0x0a + LIA_BASE_ADDR
#define     LIA_STATUS          0x10 + LIA_BASE_ADDR
#define     INTERRUPT_SELECT    0x12 + LIA_BASE_ADDR
#define     INTERRUPT_STATUS    0x12 + LIA_BASE_ADDR
#define     INT_STATUS_ENABLE   0x12 + LIA_BASE_ADDR
#define     INT_STATUS_FLAG     0x12 + LIA_BASE_ADDR     
#define     STATUS_ID           0x14 + LIA_BASE_ADDR
#define     INTERRUPT_ENABLE    0x16 + LIA_BASE_ADDR
#define     INTERRUPT_MODE      0x16 + LIA_BASE_ADDR

/******************************************************************************
 *       Theses definitions are used to assert or not various bits in the     *
 *       primary control register. The register can also be read so these     *                                            
 *       definitions are used to apply a mask when reading the register       *                                            
 *       as well.                                                             *                                            
 ******************************************************************************/

/* DPC */      
#define    DPC_A_RESET               0x00010001L
#define    DPC_B_RESET               0x00020002L
#define    DPC_GLOB_RESET            0x00040004L
#define    DPC_CONFIG                0x00080008L
#define    DPC_JTAG_MASTER           0x00100010L
#define    DPC_JTAG_OFF_CARD         0x00200020L      
/*         RESERVED                  0x0040  */
#define    DPC_PC_INTERRUPT_BITS     0x03800380L
#define    DPC_SEMAPHORE_ENABLE      0x04000400L
#define    DPC_EISA_CONFIG           0x08000800L    
#define    DPC_BYTE_WORD             0x10001000L    
#define    DPC_SRAM_WAIT_STATES      0x20002000L   
/*         RESERVED                  0x4000  */
/*         RESERVED                  0x8000  */
#define    DPC_INT_DISABLED          0x00000380L

/* QPC */

#define    QPC_GRESETC               0x00010001L  
#define    QPC_A_RESET               0x00020002L  
#define    QPC_B_RESET               0x00040004L 
#define    QPC_C_RESET               0x00080008L
#define    QPC_D_RESET               0x00100010L  
#define    QPC_PC_INTERRUPT_BITS     0x00E000E0L  
#define    QPC_A_PC_TO_DSP_FLAG      0x01000100L
#define    QPC_B_PC_TO_DSP_FLAG      0x02000200L
#define    QPC_C_PC_TO_DSP_FLAG      0x04000400L
#define    QPC_D_PC_TO_DSP_FLAG      0x08000800L    
#define    QPC_LINK_ADPT_ENABLE      0x10001000L    
#define    QPC_JTAG_BLK_ENABLE       0x20002000L   
#define    QPC_LINK_ADPT_RESET       0x40004000L  
#define    QPC_JTAG_NOT_RESET        0x80008000L
#define    QPC_INT_DISABLED          0x000000E0L 

/* EVM */

#define    EVM_GRESETC               0x00010001L  
#define    EVM_RESET                 0x00020002L  
#define    EVM_PC_INTERRUPT_BITS     0x00E000E0L  
#define    EVM_PC_TO_DSP_FLAG        0x01000100L
#define    EVM_LINK_ADPT_ENABLE      0x10001000L    
#define    EVM_JTAG_BLK_ENABLE       0x20002000L   
#define    EVM_LINK_ADPT_RESET       0x40004000L  
#define    EVM_JTAG_NOT_RESET        0x80008000L
#define    EVM_INT_DISABLED          0x000000E0L 

/* QPCC40S */

#define    QPCS_GRESETC              0x00010001L  
#define    QPCS_A_RESET              0x00020002L  
#define    QPCS_B_RESET              0x00040004L 
#define    QPCS_C_RESET              0x00080008L
#define    QPCS_D_RESET              0x00100010L  
#define    QPCS_PC_INTERRUPT1_BITS   0x00E000E0L  
#define    QPCS_INT_DISABLED         0x000000E0L 

/******************************************************************************
 *       Theses definitions are used to assert or not various bits in the     *
 *       secondary control register.                                          *                                            
 ******************************************************************************/

/* QPCC40S */

#define    QPCS_ISA_8_BIT            0x00010001L
#define    QPCS_NOT_ZERO_WAITS       0x00020002L  
#define    QPCS_PC_INTERRUPT2_BITS   0x000D000DL  
#define    QPCS_PCINT2_CLEAR         0x00200020L
#define    QPCS_LINK_ADPT_RESET      0x00400040L  
#define    QPCS_JTAG_NOT_RESET       0x00800080L
#define    QPCS_A_PC_TO_DSP_FLAG     0x01000100L
#define    QPCS_B_PC_TO_DSP_FLAG     0x02000200L
#define    QPCS_C_PC_TO_DSP_FLAG     0x04000400L
#define    QPCS_D_PC_TO_DSP_FLAG     0x08000800L    
#define    QPCS_DSPLINK2_ENABLE      0x10001000L  
#define    QPCS_WAIT_ENABLE          0x20002000L  
#define    QPCS_A_IIOF2              0x40004000L 
#define    QPCS_SHARED_LOCK_DISABLE  0x80008000L
  
/******************************************************************************
 *       Theses definitions are used to assert or not various bits in the     *
 *       block enable register.                                               *                                            
 ******************************************************************************/
  
/* QPCC40S */

#define    QPCS_MEM_BLK_ENABLE       0x00010001L    
#define    QPCS_BLOCK2_ENABLE        0x00020002L   
#define    QPCS_DPRAM_BLK_ENABLE     0x00040004L

/******************************************************************************
 *       Theses definitions are used to mask out other bits in the status     *
 *       register and produce a result.                                       *
 ******************************************************************************/

/* DPC */      
#define    DPC_A_SENSE             0x01
#define    DPC_B_SENSE             0x02
#define    DPC_GLOBAL_RESET        0x04
#define    DPC_PC_INT_FROM_DSP     0x08
#define    DPC_PC_INT_TO_DSP       0x10
/*         RESERVED                0x20  */
#define    DPC_PRI_GLOB_BUS_LOCK   0x40
#define    DPC_GLOBAL_CONFIG       0x80

/* QPC */      

#define    QPC_GLOBAL_RESET    0x0001
#define    QPC_GLOBAL_CONFIG   0x0002
#define    QPC_A_SENSE         0x0004
#define    QPC_A_FLAG_TO_PC    0x0008
#define    QPC_B_SENSE         0x0010
/*         RESERVED            0x00E0  */
#define    QPC_B_FLAG_TO_PC    0x0100
#define    QPC_C_SENSE         0x0200
#define    QPC_C_FLAG_TO_PC    0x0400
#define    QPC_D_SENSE         0x0800
#define    QPC_D_FLAG_TO_PC    0x1000
/*         RESERVED            0xE000  */

/* EVM*/
#define    EVM_GLOBAL_RESET    0x0001
#define    EVM_GLOBAL_CONFIG   0x0002
/*         RESERVED            0x0004 */
#define    EVM_FLAG_TO_PC       0x0008

/* QPCC40S */      

#define    QPCS_RECEIVE_DATA_FULL_MASK     0x0001
#define    QPCS_TRANSMIT_DATA_EMPTY_MASK   0x0002
#define    QPCS_A_FLAG_TO_PC_MASK          0x0004
#define    QPCS_B_FLAG_TO_PC_MASK          0x0008
#define    QPCS_C_FLAG_TO_PC_MASK          0x0010
#define    QPCS_D_FLAG_TO_PC_MASK          0x0020
#define    QPCS_GLOBAL_RESET               0x0040
#define    QPCS_GLOBAL_CONFIG              0x0080
#define    QPCS_A_SENSE                    0x0100
#define    QPCS_B_SENSE                    0x0200
#define    QPCS_C_SENSE                    0x0400
#define    QPCS_D_SENSE                    0x0800
#define    QPCS_ERROR                      0x1000
/*         RESERVED                        0x2000  */



/******************************************************************************
 *       Theses definitions are used to assert or not various bits in the     *
 *       configuration register. The register can also be read so these       *
 *       definitions are used to apply a mask when reading the register       *
 *       as well.                                                             *
 ******************************************************************************/
#define    DPC_MOTHER_SIZE         0x00030003L
#define    DPC_IO_BANK2_ENABLE     0x00040004L
#define    DPC_DP_RAM_ENABLE       0x00080008L
#define    DPC_DB_TIM_0_SEL        0x00100010L
#define    DPC_DB_TIM_1_SEL        0x00200020L
#define    DPC_DB_MCLK_0_SEL       0x00400040L
#define    DPC_DB_MCLK_1_SEL       0x00800080L

/******************************************************************************
 *       These definitions are used to alter individual fields in either the  *
 *       control or config registers.                                         *
 ******************************************************************************/
#define     CONTROL_REG         0x00
#define     CONFIG_REG          0x01
#define     CONTROL2_REG        0x02
#define     INT_SELECT_REG      0x03
#define     INT_ENABLE_REG      0x04
#define     QPCS_STATUS_REG     0x05
#define     QPCS_INTERRUPT_REG  0x06

#define     BIT_CLEAR           0x00
#define     BIT_SET             0xffff
#define     PC_INT_DISABLED     0x00

/* DPC */

#define    DPC_PC_INT_IRQ3         0x00800380L
#define    DPC_PC_INT_IRQ4         0x01000380L
#define    DPC_PC_INT_IRQ5         0x01800380L
#define    DPC_PC_INT_IRQ6         0x02000380L
#define    DPC_PC_INT_IRQ7         0x02800380L
#define    DPC_PC_INT_IRQ9         0x03000380L
#define    DPC_BANK0_32K           0x00000003L
#define    DPC_BANK0_128K          0x00010003L
#define    DPC_BANK0_512K          0x00020003L

/* QPC */

#define    QPC_PC_INT_IRQ3         0x002000E0L
#define    QPC_PC_INT_IRQ4         0x004000E0L
#define    QPC_PC_INT_IRQ5         0x006000E0L
#define    QPC_PC_INT_IRQ6         0x008000E0L
#define    QPC_PC_INT_IRQ7         0x00a000E0L
#define    QPC_PC_INT_IRQ9         0x00c000E0L

/* QPCS control1 and 2 */

#define    QPCS_PC_INT_IRQ3        0x002000E0L
#define    QPCS_PC_INT_IRQ4        0x004000E0L
#define    QPCS_PC_INT_IRQ5        0x006000E0L
#define    QPCS_PC_INT_IRQ6        0x008000E0L
#define    QPCS_PC_INT_IRQ7        0x00a000E0L
#define    QPCS_PC_INT_IRQ9        0x00c000E0L

/* QPCS status */

#define    QPCS_RECEIVE_DATA_FULL     0x00010001
#define    QPCS_TRANSMIT_DATA_EMPTY   0x00020002
#define    QPCS_A_FLAG_TO_PC          0x00040004
#define    QPCS_B_FLAG_TO_PC          0x00080008
#define    QPCS_C_FLAG_TO_PC          0x00100010
#define    QPCS_D_FLAG_TO_PC          0x00200020


/******************************************************************************
 *  If we really want to check the status of the LIA XMIT directly then we    *
 *  selectively mask out the appropriate bit.                                 *
 ******************************************************************************/
/* QPCB */
#define    A_XMIT_DATA_EMPTY   0x0008
#define    B_XMIT_DATA_EMPTY   0x0800
#define    C_XMIT_DATA_EMPTY   0x0002
#define    D_XMIT_DATA_EMPTY   0x0200

/* QPCS */
#define    QPCS_TRANSMIT_DATA_MASK   0x0002

/******************************************************************************
 *   If we really want to check the status of the LIA REC directly then we    *
 *   selectively mask out the appropriate bit.                                *
 ******************************************************************************/
#define    A_REC_DATA_FULL     0x0004
#define    B_REC_DATA_FULL     0x0400
#define    C_REC_DATA_FULL     0x0001
#define    D_REC_DATA_FULL     0x0100

/******************************************************************************
 *       This definition defines the number of times the status register is   *
 *       polled before concluding that the board is not responding.           *
 ******************************************************************************/
#define    DSP_TIMEOUT         0x1000

/******************************************************************************
 *       These definitions are use where we want to distinguish between the   *
 *       modules in Load and Run DPRAM                                        *
 ******************************************************************************/
#define    PRI_MODULE          0x00

/******************************************************************************
 *       These definitions are use where we want to specify DSP specific      *
 *       values like start address of DP RAM, etc                             *
 ******************************************************************************/
#define    DSP_DP_START        0xA0000000L     /* Address of 1st DP RAM loc  */
#define    DSP_DP_SEMA_LOC     0xA0000000L     /* Address of first semaphore */
#define    DSP_DP_LENGTH       0x2000L
#define    MONITOR_LENGTH      0x80            /* Length of Onboard mon */
#define    DSP_QPCS_DP_START   0xA0100000L     /* Address of 1st DP RAM loc on
                                                  QPCS carrier board. */

/******************************************************************************
 *       These definitions are use where we want to specify DSP specific      *
 *       values                                                               *
 ******************************************************************************/
#define    IVTP_ADDR           0x2ff800L
#define    TVTP_ADDR           0x2ff800L
#define    IACK_ADDR           0x300000L

/******************************************************************************
 *       This definition is used by the Get_Register_Value routine to specify *
 *       whether the register has changed during the last execution.          *
 ******************************************************************************/
#define    REGISTER_UNCHANGED      0x0
#define    REGISTER_CHANGED        0x1

/******************************************************************************
 *       This definition is used by the Run_Target function to request an     *
 *       indefinite length of run                                             *
 ******************************************************************************/
#define    RUN_FOREVER             0x0

/******************************************************************************
 *       This definition is used by the IEEE to C4x conversion routines       *
 *       to decide the block size for each convert/transfer operation         *
 ******************************************************************************/
#define    FLOAT_BLOCK_SIZE        0x20

/******************************************************************************
 *       This definition defines the return codes generated by the library    *
 *       routines. These definitions should be used to ensure future          *
 *       compatibility.                                                       *
 ******************************************************************************/
#define    RC_NO_ERROR                     0x0
#define    RC_INVALID_MODULE_ID            0x1
#define    RC_TIMEOUT                      0x2
#define    RC_INVALID_ADDRESS              0x3
#define    RC_CANNOT_OPEN_FILE             0x4
#define    RC_INVALID_FORMAT               0x5
#define    RC_MEM_ACCESS_ERROR             0x6
#define    RC_CANNOT_SELECT                0x7
#define    RC_NO_MODULES                   0x8
#define    RC_TARGET_ERROR                 0x9
#define    RC_INVALID_REGISTER_ID          0xa
#define    RC_PROC_RUNNING                 0xb
#define    RC_PROC_STOPPED                 0xc
#define    RC_BREAK_ERROR                  0xd
#define    RC_SINGLE_STEP_ERROR            0xe
#define    RC_COND_EXEC_ERROR              0xf
#define    RC_INVALID_SEC_PORT_ADDR        0x10
#define    RC_SOFTWARE_RESET_ERROR         0x11
#define    RC_PROC_STILL_RUNNING           0x12                
#define    RC_NET_FILE_NOT_FOUND           0x13    
#define    RC_UNABLE_TO_ALLOC_MEM          0X14
#define    RC_CONFIG_FILE_ERROR            0x15
#define    RC_INVALID_PROCESSOR_ID         0x16 
#define    RC_DSP_ERROR                    0x17
#define    RC_ZERO_BLOCK_SIZE              0x18 
#define    RC_DPRAM_OVERRUN                0x19
#define    RC_INVALID_FUNCTION             0x1a  
#define    RC_UNKNOWN_MODULE_TYPE          0x1b
#define    RC_INVALID_PATH_TO_DSP          0x1c
#define    RC_PATH_ALREADY_LOADED          0x1d
#define    RC_INVALID_BOARD_TYPE           0x1e

/******************************************************************************
 *      Definitions for board.cfg file constituents.                          * 
 *                                                                            *
 ******************************************************************************/
#define MAX_PROC_NAME_SIZE 16                                                  

/******************************************************************************
 *  Definitions for which LIAs to use on a QPCB for downloading monitors      *
 *                                                                            *
 ******************************************************************************/
#ifdef _QPCB_USE_ALL_LIA
    #undef _QPCB_USE_ALL_LIA                /* Don't use all LIAs           */
#endif

#define _QPCB_LOADER_LIA    1               /* Use site A to download the   */
                                            /*  monitors down. This         */
                                            /*  definition should not be    */
                                            /*  changed.                    */

/******************************************************************************
 *       Macro definition section used for increasing speed and readability   *
 *       of the code when reading or writing to the DSP PC ports.             *
 *       Also we can now selectively set/ reset bits in any 'register'        *
 ******************************************************************************/
#ifdef WIN32
#define  Write_to_Port(port, value)          Write_IO_Word( port, (int) value)
#define  Read_from_Port(port)                Read_IO_Word( port )
#else
#define  Write_to_Port(port, value)          outpw( port, (int) value)
#define  Read_from_Port(port)                inpw( port )
#endif
#define  set_reg_field(register,field,value) register = ( register & ~(field) ) | ( field & value )


/*********************************************
 *                                           *
 *                                           *
 *   global type definitions                 *
 *                                           *
 *                                           *
 *********************************************/
#ifndef UINT
   typedef unsigned int         UINT;
#endif

#ifndef ULONG
   typedef unsigned long        ULONG;
#endif

#ifndef WIN32
    #ifndef BOOLEAN
        typedef int             BOOLEAN;
    #endif
#endif

/*
   The Register_Block_Type structure is used to hold the contents of
   the processors register set.
*/

typedef struct {
       ULONG       AR[8];

       ULONG       R[12];
       UINT        E[12];
       double      F[12];

       ULONG       DP;
       ULONG       IR0;
       ULONG       IR1;
       ULONG       BK;
       ULONG       SP;

       ULONG       DIE;
       ULONG       IIE;
       ULONG       IIF;

       ULONG       RS;
       ULONG       RE;   
       ULONG       RC;   

       ULONG       ST;    
       ULONG       PC;    

} Register_Block_Type;

/*
   The Soft_Break_Type structure is used to hold details of each
   of the software breakpoints available to the user.
*/

typedef struct {
   BOOLEAN        Enable;
   ULONG          Break_Address;
} Soft_Break_Type;



/****************************************************************************
 *    Collection of header definitions for use in the COFF loader           *
 ****************************************************************************/
typedef struct filehdr
{
   unsigned short f_magic;  /* magic number */
   unsigned short f_nscns;  /* number of sections */
   long           f_timdat; /* time and date stamp */
   long           f_symptr; /* file ptr to symbol table */
   long           f_nsyms;  /* number of entries in symbol table */
   unsigned short f_opthdr; /* size of optional header */
   unsigned short f_flags;  /* flags */
};


#define FILHDR struct filehdr   
#define FILHSZ sizeof(FILHDR)   

typedef struct opthdr
{
   short          magic;       
   short          vstamp;      
   long           tsize;       
   long           dsize;       
   long           bsize;       
   long           entry;       
   long           text_start;  
   long           data_start;  
};

#define OPTHDR struct opthdr   
#define OPTHSZ sizeof(OPTHDR)   


typedef struct scnhdr
{
   char  s_name[8];           /* section name */
   long  s_paddr;             /* physical address */
   long  s_vaddr;             /* virtual address */
   long  s_size;              /* section size */
   long  s_scnptr;            /* file ptr to section raw data */
   long  s_relptr;            /* file ptr to relocation */
   long  s_lnnoptr;           /* file pointer to line number */
   unsigned short s_nreloc;   /* numder of relocation entries */
   unsigned short s_nlnno;    /* number of line number entries */
   unsigned short s_flags;    /* flags and things           */
   char  s_reserved;          /* Not available */
   unsigned char  s_page;     /* Page number for this data */
};   


#define SCNHDR struct scnhdr   
#define SCNHSZ sizeof(SCNHDR)   

/****************************************************************************
 *      The magic number to shoe the COFF target processor                    *
 ****************************************************************************/
#define     COFF_C40_MAGIC_NO          0x0093L


#define     SECTION_CODE        0x0
#define     SECTION_DATA        0x1

#define    PRI_LOAD_FUNCTION          0x1L
#define    SEC_LOAD_FUNCTION          0x2L


/*********************************************
 *                                           *
 *                                           *
 *   global variable definitions             *
 *                                           *
 *                                           *
 *********************************************/

extern  UINT       Global_Bank1_Base_Address;      /* in C40DEV.LIB    */
extern  UINT       Global_JTAG_IO_Address;         /* in C40DEV.LIB    */
extern  UINT       Global_LIA_Address;             /* in C40DEV.LIB    */
extern  UINT       Global_DB40_Run;                /* in C40DEV.LIB    */





/*********************************************
 *                                           *
 *                                           *
 *   New structures and definitions added    *
 *   to old header file.                     *
 *                                           *
 *********************************************/




#define EVM_MODULE_SITE_NUM    1      /* This is used to allow the LIA routines
                                         to access the EVM as if it were site
                                         1 on a QPC. This is done for
                                         efficiency*/
#define LINK_TABLE_ENDFLAG     0x6
#define LINK_TABLE_BOOT_NEXT   0x7
#define TWIN_MOD_LINK_A_TO_B   2
#define TWIN_MOD_LINK_B_TO_A   0
#define MAX_DEPTH_OF_BOOT      8                                              
#define FILE_NAME_LENGTH         13                                              
#define MAX_BOARD_NAME_SIZE      15         
#define MAX_MODULE_NAME_SIZE     15         
#define JMASTER                  0x10
#define JOFFBOARD                0x20                  
#define MAX_NUM_SOFT_BREAKS    16
#define PROCESSOR_UNOBTAINABLE  -1 /* This means that the link connection to 
                                      the carrier board needed to reach the processor
                                      does not exist*/

#define MONITOR_FILE_NAME   "boot.out"
#define DPRAM_MONITOR       "c4xload.rom"
#define EDRAM_MONITOR       "edboot.out"
#define EDRAM_DPRAM_MONITOR "edload.rom"

#define EDRAM_NAME         "MDC40ED"


#define MAX_NUM_PROCS_PER_QPCS 8
#define MAX_SITES_PER_QPCS 4
#define QPCS_INTER_PROC_LINK 2
#define MAX_NUM_LINKS_PER_DEVICE 6


/* MDC40D */
#define   MDC40D_LMCR                                   0x3DEA2540L
#define   MDC40D_GMCR                                   0x1EEA0000L

#define   MDC40D_PEROM_LMCR                             0x3EEA2540L
#define   MDC40D_PEROM_GMCR                             0x1EEA0000L

/* MDC40S */
#define   MDC40S1_32K_LMCR                              0x3DEBA050L              
#define   MDC40S1_32K_GMCR                              0x2F778010L

#define   MDC40S2_128K_LMCR                             0x3DEC2050L
#define   MDC40S2_128K_GMCR                             0x303BC010L

#define   MDC40S1_32K_PEROM_LMCR                        0x3EEB8550L
#define   MDC40S1_32K_PEROM_GMCR                        0x2F778010L

#define   MDC40S2_128K_PEROM_LMCR                       0x3EE90550L
#define   MDC40S2_128K_PEROM_GMCR                       0x303BC010L

/* MDC40T */
#define   MDC40T1_32K_LMCR                              0x1E738510L
#define   MDC40T1_32K_GMCR                              0x0F738000L

#define   MDC40T2_128K_LMCR                             0x1E840510L
#define   MDC40T2_128K_GMCR                             0x10840000L

/* MDC40B */
#define   MDC40B1_LMCR                                  0x1E4A4000L
#define   MDC40B1_GMCR                                  0x344A4000L

#define   MDC40B2_LMCR                                  0x1E4A4000L
#define   MDC40B2_GMCR                                  0x354A4000L

#define   MDC40B3_LMCR                                  0x1E4A4000L
#define   MDC40B3_GMCR                                  0x364A4000L

/* MDC40HB */
#define   MDC40HB1_LMCR                                 0x1E4A4000L
#define   MDC40HB1_GMCR                                 0x344A4000L

#define   MDC40HB2_LMCR                                 0x1E4A4000L
#define   MDC40HB2_GMCR                                 0x354A4000L

#define   MDC40HB3_LMCR                                 0x1E4A4000L
#define   MDC40HB3_GMCR                                 0x364A4000L

/* MDC40X */
#define  MDC40X_FS2_LMCR                                0x3D740000L
#define  MDC40X_FS2_GMCR                                0x304C0010L

#define  MDC40X_CS2_LMCR                                0x3D740000L
#define  MDC40X_CS2_GMCR                                0x304C0010L


/* MDC40ED */
#define  MDC40ED1_LMCR                                  0x36721870L
#define  MDC40ED1_GMCR                                  0x33720000L

#define  MDC40ED2_LMCR                                  0x36721870L
#define  MDC40ED2_GMCR                                  0x33720000L

/* MDC40IC */
#define  MDC40IC1_LMCR                                  0x3df22040L
#define  MDC40IC1_GMCR                                  0x32740010L

#define  MDC40IC2_LMCR                                  0x3df22040L
#define  MDC40IC2_GMCR                                  0x32740010L

/* MDC40IM */
#define  MDC40IM1_LMCR                                  0x3df22040L
#define  MDC40IM1_GMCR                                  0x30740010L

#define  MDC40IM2_LMCR                                  0x3df22040L
#define  MDC40IM2_GMCR                                  0x30740010L

/* MDC40IL */
#define  MDC40ILA1_LMCR                                 0x3df3a040
#define  MDC40ILA1_GMCR                                 0x32840010

#define  MDC40ILA2_LMCR                                 0x3df42040
#define  MDC40ILA2_GMCR                                 0x3381c000

#define  MDC40ILA3_LMCR                                 0x3df42040
#define  MDC40ILA3_GMCR                                 0x3581c000


#define  MDC40ILB1_LMCR                                 0x3df3a040
#define  MDC40ILB1_GMCR                                 0x32840010

#define  MDC40ILB2_LMCR                                 0x3df42040
#define  MDC40ILB2_GMCR                                 0x3381c000

#define  MDC40ILB3_LMCR                                 0x3df42040
#define  MDC40ILB3_GMCR                                 0x3581c000


#define  MDC40ILC1_LMCR                                 0x3df42040
#define  MDC40ILC1_GMCR                                 0x3381c000

#define  MDC40ILC2_LMCR                                 0x3df42040
#define  MDC40ILC2_GMCR                                 0x3581c000


/* MDC44S */
#define  MDC44S3_LMCR                                   0x1D741050
#define  MDC44S3_GMCR                                   0x34740010

#define  MDC44S4_LMCR                                   0x1D741050
#define  MDC44S4_GMCR                                   0x34740010

#define  MDC44S5_LMCR                                   0x1D749050
#define  MDC44S5_GMCR                                   0x34748010

#define  MDC44S6_LMCR                                   0x1D749050
#define  MDC44S6_GMCR                                   0x34748010

/* MDC44T */
#define  MDC44T3_LMCR                                   0x1D741050
#define  MDC44T3_GMCR                                   0x34740010

#define  MDC44T4_LMCR                                   0x1D749050
#define  MDC44T4_GMCR                                   0x34748010


typedef ULONG BIT_FIELD;                                              

typedef enum
{
   UNTARGETED = 0,
   TARGETED
}   TARGET_PROCESSOR;

typedef enum
{
   BT_DPCC40 = 0,
   BT_QPCC40,
   BT_QPCC40S,
   BT_DBV42,
   BT_DBV44, 
   BT_DBV46,
   BT_EVMC40,
   BT_UNKNOWN
}  BOARD_TYPE_ENUM;

typedef enum
{
   CLOSED = 0,
   OPEN
}   PROCESSOR_STATUS;

typedef struct
{
   ULONG  GMCR_VAL;
   ULONG  LMCR_VAL;
}MEM_CNTRL_REGS;

typedef struct
{
   int      size;
   int      link[MAX_DEPTH_OF_BOOT];
}  PATH_TABLE;

typedef struct
{
   UINT        JTAG_Chain_ID;
   char        JTAG_File_Name[FILE_NAME_LENGTH];
   int         Master_Board_ID;     
   UINT            Num_Procs_In_Chain;
   char        *Ptr_JTAG_Scan_Chain;                           
}  JTAG_FILE_TABLE;           

#define JTAG_FILE_TABLE_SIZE sizeof(JTAG_FILE_TABLE)

typedef struct
{
   int                  proc_id;                /* Unique processor number  */
   int                  Host_Site_Number;       /* Carrier board site num   */
                                                /*  to access the proc      */
   CFR_PROCESSOR       *Ptr_CFR_Processor;
   UINT                 Processor_Mode;         /* PROC_DEBUG or            */
                                                /*  PROC_RUNNING            */
   UINT                 JTAG_Number;
   UINT                 JTAG_Chain_ID;          /* JTAG chain number        */
   TARGET_PROCESSOR     Targeted;               /* JTAG Target processor    */
   PROCESSOR_STATUS     Processor_Status;       /* CFR data opened or       */
                                                /*  closed                  */
   BOOLEAN              Soft_Breaks_Installed;
   MEM_CNTRL_REGS       Memory_Control_Regs;
   char                *DP_Mem;                 /* DPRAM location pointer   */
                                                /*  for Load_And_Run_DPRAM  */
   BOOLEAN              Kernel_Loaded;          /* Set to TRUE when the     */
                                                /*  kernel has been booted. */
   BOOLEAN              Code_Loaded;            /* Set to TRUE when the     */
                                                /*  proc has been booted.   */
} PROC_ID;

#define SIZE_OF_PROC_ID_TABLE (sizeof (PROC_ID))
                                                                           
typedef struct
{
   CFR_BOARD           *Ptr_CFR_Board;
   UINT                 Control_Reg;
   UINT                 Control2_Reg;
   UINT                 Current_Control_Reg;
   UINT                 Current_Control2_Reg;
   UINT                 Config_Reg;
   UINT                 Port_Address1;
   UINT                 Port_Address2;
   UINT                 Port_Address3;
   UINT                 Memory_Segment;
   UINT                 Status_Reg;
   UINT                 Mem_Blk_Disable_Val;    /* RJW 960701 - to support  */
   UINT                 Mem_Blk_Enable_Val;     /*  paging in of QPCSs      */
   BOARD_TYPE_ENUM      Board_Type;
}  BOARD_DESCRIPTION;
#define SIZE_OF_BOARD_TABLE sizeof (BOARD_DESCRIPTION)

/********************************************
 *                                          *
 *       Global variables                   *
 ********************************************/
extern BOOLEAN          Config_File_Read;
extern int              Current_Processor_ID;
extern int              Current_Board_ID;
extern int              Current_JTAG_Chain; 
extern UINT                             Current_Board_Port_Address;
extern CFR_CONFIG       *Ptr_Config_File;
extern CFR_ID           CFR_Identity;
extern BOARD_DESCRIPTION      *Ptr_Board_Descript_Table;
extern PROC_ID            *Ptr_Proc_Descript_Table;        
extern JTAG_FILE_TABLE    *Ptr_JTAG_File_Table;   
extern UINT               Num_JTAG_Scan_Chains;
extern char               Global_Board_Dat_File[FILE_NAME_LENGTH];

/* Netapi function prototypes for user */
                                                            
/* DPRAM */
UINT Load_And_Run_File_DPRAM (PROC_ID *ptr_proc_handle, char *filename);

UINT Read_DPRAM_Words_32 (PROC_ID *ptr_proc_handle, ULONG addr, UINT len, ULONG *ptr_data);
UINT Write_DPRAM_Words_32 (PROC_ID *ptr_proc_handle, ULONG addr, UINT len, ULONG *ptr_data);
UINT Read_DPRAM_Floats_32 (PROC_ID *ptr_proc_handle, ULONG addr, UINT len, float *ptr_data);
UINT Write_DPRAM_Floats_32 (PROC_ID *ptr_proc_handle, ULONG addr, UINT len, float *ptr_data);

/*LIA*/
UINT Load_And_Run_File_LIA (PROC_ID *ptr_proc_handle, char *filename);

UINT Read_LIA_Words_32 (PROC_ID *ptr_proc_handle, UINT len, ULONG *ptr_data);
UINT Write_LIA_Words_32 (PROC_ID *ptr_proc_handle, UINT len, ULONG *ptr_data);
UINT Read_LIA_Floats_32 (PROC_ID *ptr_proc_handle, UINT len, float *ptr_data);
UINT Write_LIA_Floats_32 (PROC_ID *ptr_proc_handle, UINT len, float *ptr_data);
UINT SRead_LIA_Words_32 (PROC_ID *ptr_proc_handle, UINT *len, ULONG *ptr_data, int timeout);
UINT SWrite_LIA_Words_32 (PROC_ID *ptr_proc_handle, UINT *len, ULONG *ptr_data, int timeout); 
UINT SRead_LIA_Floats_32 (PROC_ID *ptr_proc_handle, UINT *len, float *ptr_data, int timeout);
UINT SWrite_LIA_Floats_32 (PROC_ID *ptr_proc_handle, UINT *len, float *ptr_data, int timeout);


/* utility functions */ 
UINT Global_Network_Reboot( void );
UINT GNR_Without_Kernels( void );
UINT Warm_Restart( void );

UINT Get_Status_Register (PROC_ID *ptr_proc_handle, UINT  *u_status_reg );
UINT Open_Processor_ID (PROC_ID **ptr_p_proc_handle, char *ptr_proc_name,
                        MEM_CNTRL_REGS *mem_control_regs);
UINT Close_Processor_ID (PROC_ID *ptr_Proc_handle);
void Clear_All_Lib_Memory (void);
UINT Change_Board_Register_Fields (PROC_ID *ptr_proc_handle, UINT reg, BIT_FIELD Bits_To_Set,
                       BIT_FIELD Bits_To_Clear);

/* internal use only */
void    LIA_Write_Block(UINT , UINT , ULONG  *, UINT);
void    LIA_Read_Block(UINT , UINT , ULONG  *, UINT);
UINT    LIA_FWrite_Block(UINT , UINT , ULONG  *, UINT, UINT);
UINT    LIA_FRead_Block(UINT , UINT , ULONG  *, UINT, UINT);

void    LIA_Write_Block_EM(UINT , ULONG  *, UINT, UINT);
void    LIA_Read_Block_EM(UINT , ULONG  *, UINT, UINT);
UINT    LIA_FWrite_Block_EM(UINT , ULONG  *, UINT, UINT, UINT);
UINT    LIA_FRead_Block_EM(UINT , ULONG  *, UINT, UINT, UINT);


#ifdef WIN32

typedef void (*FUNCPTR)(void);

UINT	 Install_Interrupt_Handler(PROC_ID *, UINT, FUNCPTR);

#endif
