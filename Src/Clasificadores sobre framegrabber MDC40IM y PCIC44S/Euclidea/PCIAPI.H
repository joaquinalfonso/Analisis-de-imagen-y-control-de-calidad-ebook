/**************************************************************************
 *                                                                        *
 *   Loughborough Sound Images plc                                        *
 *   Loughborough Park                                                    *
 *   Ashby Road                                                           *
 *   Loughborough                                                         *
 *   Leicestershire           Tel : +44 1509 634444                       *
 *   LE11 3NE                 FAX : +44 1509 634450                       *
 *                                                                        *
 *   Software Copyright (c) 1995, 1996                                    *
 *                                                                        *
 *   Program  : Windows 3.1x VxD for PCI/C44                              *
 *   Author   : BJ                                                        *
 *   Date     : 24/4/95                                                   *
 *   Filename : PCIAPI.H                                                  *
 *                                                                        *
 *   Version History:                                                     *
 *      Version      Date      Author         Reason for Change           *
 *      1.00         24/4/95   BJ             Creation                    *
 *      1.10         26/1/96   RP             Added structures for user   *
 *                                            callback & interrupthandler *
 *      1.20         2/7/96    SW             Sync with Win32 version     *
 *      1.30         11/9/96   SW             Added #define for card C44A *
 *                                                                        *
 *   Description:                                                         *
 *                                                                        *
 *   Public Core functions header.                                        *
 *                                                                        *
 **************************************************************************/

#ifndef __PCIAPI_H
#define __PCIAPI_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 * Definitions.
 **************************************************************************/

#define PCIC44_MAJOR_REVISION 0x02
#define PCIC44_MINOR_REVISION 0x30
#define PCIC44_DEVICE_ID      0x360D /* VxD ID as assigned by Microsoft */

#define PCIC44_MAX_NUM_CARDS   4

/*
 * The PCI9060 PCI Configuration space registers.
 */
typedef enum
{
  VENDOR_ID   = 0x0,  /* WORD */
  DEVICE_ID   = 0x2,  /* WORD */

  COMMAND     = 0x4,  /* WORD */
  STATUS      = 0x6,  /* WORD */

  REVISION_ID = 0x8,  /* BYTE */
  CLASS_CODE  = 0x9,  /* 3 BYTES */

   CLASS_CODE_BASE_CLASS   = 0xB,  /* Base class code */
   CLASS_CODE_SUB_CLASS    = 0xA,  /* Sub class code  */
   CLASS_CODE_SPECIFIC_REG = 0x9,  /* Specific register level interface */

  CACHE_LINE_SIZE = 0xC,  /* BYTE */
  LATENCY_TIMER   = 0xD,  /* BYTE */
  HEADER_TYPE     = 0xE,  /* BYTE */
  BIST            = 0xF,   /* BYTE */

  MM_RUNTIME_REG_ADDRESS     = 0x10,  /* DWORD */

  IO_RUNTIME_REG_ADDRESS     = 0x14,  /* DWORD */

  LOCAL_MEM_ADDRESS          = 0x18,  /* DWORD */

  EXPANSION_ROM_BASE_ADDRESS = 0x30,  /* DWORD */

  INTERRUPT_LINE = 0x3C,  /* BYTE */
  INTERRUPT_PIN  = 0x3D,  /* BYTE */
  MIN_GNT        = 0x3E,  /* BYTE */
  MIN_LAT        = 0x3F   /* BYTE */

} PCIC44_PCI_CFG_REGISTER;

/*
 * Bit masks for COMMAND register of PCI configuration space
 */
#define COMMAND_IO_SPACE                0x1    /* 1 Bit */
#define COMMAND_MEMORY_SPACE            0x2    /* 1 Bit */
#define COMMAND_MASTER_ENABLE           0x4    /* 1 Bit */
#define COMMAND_SPECIAL_CYCLE           0x8    /* 1 Bit */
#define COMMAND_MEMORY_WRITE_INVALIDATE 0x10   /* 1 Bit */
#define COMMAND_VGA_PALETTE_SNOOP       0x20   /* 1 Bit */
#define COMMAND_PARITY_ERROR_RESPONSE   0x40   /* 1 Bit */
#define COMMAND_WAIT_CYCLE_CONTROL      0x80   /* 1 Bit */
#define COMMAND_SERR_ENABLE             0x100  /* 1 Bit */
#define COMMAND_FAST_BACK_TO_BACK       0x200  /* 1 Bit */

/*
 * Bit masks for STATUS register of PCI configuration space
 */
#define EXPANSION_ROM_ENABLE            0x00000001L

#define STATUS_FAST_BACK_TO_BACK        0x80    /* 1 Bit */
#define STATUS_MASTER_DATA_PARITY_ERROR 0x100   /* 1 Bit */
#define STATUS_DEVSEL_TIMING            0x600   /* 2 Bits */
#define STATUS_TARGET_ABORT             0x800   /* 1 Bit */
#define STATUS_RECV_TARGET_ABORT        0x1000  /* 1 Bit */
#define STATUS_RECV_MASTER_ABORT        0x2000  /* 1 Bit */
#define STATUS_SIGNALED_SYSTEM_ERROR    0x4000  /* 1 Bit */
#define STATUS_DETECTED_PARITY_ERROR    0x8000  /* 1 Bit */

/*
 * Bit masks for HEADER_TYPE register of PCI configuration space
 */
#define HEADER_CFG_LAYOUT_TYPE 0x7F  /* 7 Bits */
#define HEADER_TYPE_FUNCTION   0x80  /* 1 Bit */

/*
 * Bit masks for BIST register of PCI configuration space
 */
#define BIST_RETURN_CODE 0xF   /* 4 Bits */
#define BIST_INVOKE_BIT  0x40  /* 1 Bit */
#define BIST_CAPABLE     0x80  /* 1 Bit */

/*
 * Possible values for INTERRUPT_PIN register of PCI configuration space
 */
#define NO_PIN   0x0
#define PIN_INTA 0x1
#define PIN_INTB 0x2
#define PIN_INTC 0x3
#define PIN_INTD 0x4

/*
 * The offsets for the PCI9060 IO and memory mapped IO registers.
 */
typedef enum
{
/*
 * The following two registers
 */
  RANGE_PCI_TO_LOCAL              = 0x0,  /* DWORD */
  REMAP_BASE_ADDRESS_PCI_TO_LOCAL = 0x4,  /* DWORD */

  RANGE_PCI_TO_EXPANSION_ROM              = 0x10,  /* DWORD */
  REMAP_BASE_ADDRESS_PCI_TO_EXAPNSION_ROM = 0x14,  /* DWORD */

  BUS_REGION_DESCRIPTOR_PCI_TO_LOCAL = 0x18,  /* DWORD */

/*
 * The following 5 registers configure how and where the bus on
 * the card can access the PCI bus with memory, IO or configuration space
 * accesses.
 *
 * The first 2 registers specify the location and range of the card
 * memory map that maps to the PCI bus.  The 3rd register specifies the region
 * of the PCI memory space that is mapped.
 *
 * The final 2 registers control and specify how the local bus on the card
 * can access the IO or configuration spaces of the PCI bus.
 */
  LOCAL_RANGE_LOCAL_TO_PCI                 = 0x1C,  /* DWORD */
  LOCAL_BASE_ADDRESS_LOCAL_TO_PCI          = 0x20,  /* DWORD */

  PCI_BASE_ADDRESS_FOR_LOCAL_TO_PCI_ACCESS = 0x28,  /* DWORD */

  LOCAL_BASE_ADDRESS_LOCAL_TO_PCI_IO_CFG   = 0x24,  /* DWORD */

  LOCAL_PCI_CFG_ACCESS_LOCAL_TO_PCI_IO_CFG = 0x2C,  /* DWORD */

  MAILBOX_0 = 0x40,  /* DWORD */
  MAILBOX_1 = 0x44,  /* DWORD */
  MAILBOX_2 = 0x48,  /* DWORD */
  MAILBOX_3 = 0x4C,  /* DWORD */
  MAILBOX_4 = 0x50,  /* DWORD */
  MAILBOX_5 = 0x54,  /* DWORD */
  MAILBOX_6 = 0x58,  /* DWORD */
  MAILBOX_7 = 0x5C,  /* DWORD */

  PCI_TO_LOCAL_DOORBELL    = 0x60,  /* DWORD */
  LOCAL_TO_PCI_DOORBELL    = 0x64,  /* DWORD */

  INTERRUPT_CONTROL_STATUS = 0x68,  /* DWORD */

  GENERAL_CONTROL_CFG_REGISTER = 0x6C /* DWORD */

} PCIC44_IO_REGISTER_OFFSET;


/*
 * The offsets for the PCI9060 local DMA registers.
 */
typedef enum
{
    CHANNEL_0_MODE_REGISTER = 0x100,
    CHANNEL_0_PCI_START_ADDRESS_REGISTER = 0x104,
    CHANNEL_0_LOCAL_START_ADDRESSS_REGISTER = 0x108,
    CHANNEL_0_TRANSFER_SIZE_REGISTER = 0x10C,
    CHANNEL_0_DESCRIPTOR_POINTER_REGISTER = 0x110,
    CHANNEL_1_MODE_REGISTER = 0x114,
    CHANNEL_1_PCI_START_ADDRESS_REGISTER = 0x118,
    CHANNEL_1_LOCAL_START_ADDRESSS_REGISTER = 0x11C,
    CHANNEL_1_TRANSFER_SIZE_REGISTER = 0x120,
    CHANNEL_1_DESCRIPTOR_POINTER_REGISTER = 0x124,
    COMMAND_STATUS_REGISTER = 0x128,
    ARBITRATION_REGISTER_0 = 0x12C,
    ARBITRATION_REGISTER_1 = 0x130,
} PCIC44_LOCAL_DMA_REGISTER;


/*
 * Bit masks for BUS_REGION_DESCRIPTOR_PCI_TO_LOCAL register of PCI9060 local
 * configuration space
 */
#define BUS_REGION_MEM_SPACE_0_BUS_WIDTH          0x00000003L  /* 2 Bits */
#define BUS_REGION_MEM_SPACE_0_WAIT_STATES        0x0000003CL  /* 4 Bits */
#define BUS_REGION_MEM_SPACE_0_READY_INPUT_ENABLE 0x00000040L  /* 1 Bit */
#define BUS_REGION_MEM_SPACE_0_BTERM_INPUT_ENABLE 0x00000080L  /* 1 Bit */
#define BUS_REGION_MEM_SPACE_0_PREFETCH_DISABLE   0x00000100l  /* 1 Bit */
#define BUS_REGION_EXPAN_ROM_BUS_WIDTH            0x00030000L  /* 2 Bits */
#define BUS_REGION_EXPAN_ROM_WAIT_STATES          0x0003C000L  /* 4 Bits */
#define BUS_REGION_EXPAN_ROM_READY_INPUT_ENABLE   0x00040000L  /* 1 Bit */
#define BUS_REGION_EXPAN_ROM_BTERM_INPUT_ENABLE   0x00080000L  /* 1 Bit */
#define BUS_REGION_MEM_SPACE_0_BURST_ENABLE       0x00100000L  /* 1 Bit */
#define BUS_REGION_EXPAN_ROM_BURST_ENABLE         0x00400000L  /* 1 Bit */
#define BUS_REGION_DIRECT_SLAVE_WRITE_MODE        0x00800000L  /* 1 Bit */
#define BUS_REGION_PCI_TARGET_RETRY_DELAY_CLOCKS  0xFF000000L  /* 4 Bits */

/*
 * Bit masks for PCI_BASE_ADDRESS_FOR_LOCAL_TO_PCI_ACCESS register of PCI9060 local
 * configuration space
 */
#define PCI_BASE_DIRECT_MASTER_MEMORY_ACCESS_ENABLE  0x00000001L  /* 1 Bit */
#define PCI_BASE_DIRECT_MASTER_IO_ACCESS_ENABLE      0x00000002L  /* 1 Bit */
#define PCI_BASE_LOCK_INPUT_ENABLE                   0x00000004L  /* 1 Bit */
#define PCI_BASE_DIRECT_MASTER_READ_PREFRETCH_SIZE   0x00000008L  /* 1 Bit */
#define PCI_BASE_DIRECT_MASTER_PCI_READ_MODE         0x00000010L  /* 1 Bit */
#define PCI_BASE_PROGRAMMABLE_ALMOST_FULL_FLAG       0x000000E0L  /* 3 Bits */
#define PCI_BASE_REMAP_LOCAL_TO_PCI_ADDRESS_BITS     0xFFFF0000L  /* 16 Bits */

/*
 * Bit masks for LOCAL_PCI_CFG_ACCESS_LOCAL_TO_PCI_IO_CFG register of PCI9060 local
 * configuration space
 */
#define LOCAL_PCI_CFG_CONFIGURATION_TYPE   0x00000003L  /* 2 Bits */
#define LOCAL_PCI_CFG_REGISTER_NUMBER      0x000000FCL  /* 6 Bits */
#define LOCAL_PCI_CFG_FUNCTION_NUMBER      0x00000700L  /* 3 Bits */
#define LOCAL_PCI_CFG_DEVICE_NUMBER        0x0000F800L  /* 5 Bits */
#define LOCAL_PCI_CFG_BUS_NUMBER           0x00FF0000L  /* 8 Bits */
#define LOCAL_PCI_CFG_CONFIGURATION_ENABLE 0x80000000L  /* 1 Bit */

/*
 * Bit masks for INTERRUPT_CONTROL_STATUS register of PCI9060 local
 * configuration space
 */
#define INT_CON_STS_ENABLE_LOCAL_BUS_LSERR        0x00000001L  /* 1 Bit */
#define INT_CON_STS_ENABLE_LOCAL_BUS_LSERR_PARITY 0x00000002L  /* 1 Bit */
#define INT_CON_STS_GENERATE_PCI_BUS_SERR         0x00000004L  /* 1 Bit */
#define INT_CON_STS_PCI_INT_ENABLE                0x00000100L  /* 1 Bit */
#define INT_CON_STS_PCI_DOORBELL_INT_ENABLE       0x00000200L  /* 1 Bit */
#define INT_CON_STS_PCI_ABORT_INT_ENABLE          0x00000400L  /* 1 Bit */
#define INT_CON_STS_PCI_LOCAL_INT_ENABLE          0x00000800L  /* 1 Bit */
#define INT_CON_STS_RETRY_ABORT_ENABLE            0x00001000L  /* 1 Bit */
#define INT_CON_STS_PCI_DOORBELL_INT_ACTIVE       0x00002000L  /* 1 Bit */
#define INT_CON_STS_PCI_ABORT_INT_ACTIVE          0x00004000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_INT_ACTIVE              0x00008000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_INT_ENABLE              0x00010000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_DOORBELL_INT_ENABLE     0x00020000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_DMA_0_INT_ENABLE        0x00040000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_DMA_1_INT_ENABLE        0x00080000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_DOORBELL_INT_ACTIVE     0x00100000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_DMA_0_INT_ACTIVE        0x00200000L  /* 1 Bit */
#define INT_CON_STS_LOCAL_DMA_1_INT_ACTIVE        0x00400000L  /* 1 Bit */
#define INT_CON_STS_BIST_INTERRUPT_ACTIVE         0x00800000L  /* 1 Bit */
#define INT_CON_STS_DIRECT_MASTER_IN_ABORT        0x01000000L  /* 1 Bit */
#define INT_CON_STS_DMA_0_MASTER_IN_ABORT         0x02000000L  /* 1 Bit */
#define INT_CON_STS_DMA_1_MASTER_IN_ABORT         0x04000000L  /* 1 Bit */
#define INT_CON_STS_256_RETRY_TARGET_ABORT        0x08000000L  /* 1 Bit */

/*
 * Bit masks for GENERAL_CONTROL_CFG_REGISTER register of PCI9060 local
 * configuration space
 */
#define GENERAL_PCI_READ_COMMAND_FOR_DMA            0x0000000FL  /* 4 Bits */
#define GENERAL_PCI_WRITE_COMMAND_FOR_DMA           0x000000F0L  /* 4 Bits */
#define GENERAL_PCI_READ_COMMAND_FOR_DIRECT_MASTER  0x00000F00L  /* 4 Bits */
#define GENERAL_PCI_WRITE_COMMAND_FOR_DIRECT_MASTER 0x0000F000L  /* 4 Bits */
#define GENERAL_GENERAL_PURPOSE_OUTPUT              0x00010000L  /* 1 Bit */
#define GENERAL_GENERAL_PURPOSE_INPUT               0x00020000L  /* 1 Bit */
#define GENERAL_EEPROM_CLOCK_TOGGLE                 0x01000000L  /* 1 Bit */
#define GENERAL_EEPROM_CHIP_SELECT                  0x02000000L  /* 1 Bit */
#define GENERAL_WRITE_BIT_TO_EEPROM                 0x04000000L  /* 1 Bit */
#define GENERAL_READ_BIT_FROM_EEPROM                0x08000000L  /* 1 Bit */
#define GENERAL_EEPROM_PRESENT                      0x10000000L  /* 1 Bit */
#define GENERAL_RELOAD_CONFIGURATION_REGISTERS      0x20000000L  /* 1 Bit */
#define GENERAL_PCI_ADAPTOR_SOFT_RESET              0x40000000L  /* 1 Bit */
#define GENERAL_LOCAL_INIT_DONE                     0x80000000L  /* 1 Bit */

/* 
 * Doorbell Interrupt States
*/
#define INT_DMA_COMPLETE 0x10000000

/*
 * Transfer Types for Reads and Writes.
 */
#define TRANSFER_DEFAULT    0x0000
#define TRANSFER_BYTES_8    0x0100
#define TRANSFER_WORDS_16   0x0200
#define TRANSFER_DWORDS_32  0x0400
#define TRANSFER_DDWORDS_64 0x0800
#define TRANSFER_TYPES      (                       \
			      TRANSFER_BYTES_8      \
			      | TRANSFER_WORDS_16   \
			      | TRANSFER_DWORDS_32  \
			      | TRANSFER_DDWORDS_64 \
			    )

#define PCI_IO_RUNTIME_RANGE 0x134 /* Range to cover all PLX runtime registers */

#define V0_SHARED_SRAM_REMAP 0xc0000000L
#define MEM_512K_BY_32_BIT 0x200000L  /* 512K x 32 bit SRAM range */
#define V0_TBC_REMAP 0x200000L
#define TBC_REG_RANGE 0x80   /* TBC SIZE 18 regs x 32 access although regs */
			     /* are 16 bits wide                           */
#define NOT_USED_ON_V0 0xFFFFFFFFL
#define RANGE_NOT_USED_ON_V0 0x00

#define V1_SHARED_SRAM_REMAP 0x00000000L
#define V1_LOCKED_SHARED_SRAM_REMAP 0x10000000L
#define V1_TBC_REMAP 0xC0000000L
#define DMA_PLX_REMAP 0x80000000L
#define DMA_LOCKED_PLX_REMAP 0x90000000L
#define DMA_PLX_RANGE (0x134 >> 2) /* Multiplied by 4 when 32 bit access */
				 /* is made in Read & WriteBlockDirect */
#define DMA_PLX_TRUE_RANGE (0x134)
			    
#define DMA_REG_START (0x100 >> 2) /* Offset multiplied by 4 using 32 bit access */


/**************************************************************************
 * Type Definitions.
 **************************************************************************/

typedef WORD           CARD_ID;
#define NULL_CARD_ID   ((CARD_ID)0xffff)

/*
 * Must be DWORD aligned since it passed to VxD and the caller may
 * be either byte, word or dword alignment compiled.
 */
typedef struct
{
  CARD_ID CardIdent;
  WORD padding_0;

  WORD wNumProcs;
  WORD padding_1;

  WORD wProc1State;
  WORD padding_2;

  WORD wProc2State;
  WORD padding_3;

  WORD wProc3State;
  WORD padding_4;

  WORD wProc4State;
  WORD padding_5;
/* Possible states for a processor */
#define PROC_NOT_FITTED 0x00
#define PROC_UNBOOTED   0x01
#define PROC_BOOTED     0x02

  WORD wType;
  WORD padding_6;
/* Different card Types supported by this version of the software. */
#define PCIC44_NUM_CARD_TYPES 16
#define PCIC44_CARD_ID0       0xC440U /* C44 Device ID V0.0 Card */
#define PCIC44_CARD_ID1       0xC441U /* C44 Device ID V1.0 Card */
#define PCIC44_CARD_IDa       0xC44aU /* C44 Device ID, special version */

  WORD wSerial;
  WORD padding_7;               

  WORD wIOBase;
  WORD padding_8;       

  BYTE bIRQ;
  BYTE padding_9;
  WORD padding_10;              

} PCIC44_CARD_INFO;

/**************************************************************************
 * PCIC44 PM & V86 API Functions and  Error Codes.
 **************************************************************************/

typedef enum {
  API_PCIC44_SUCCESS = 0,
  API_PCIC44_FAILURE,

  API_PCIC44_UNKNOWN_ERROR,

  API_PCIC44_INVALID_VMHANDLE,

  API_PCIC44_UNABLE_TO_MAP_CARD_INFO,

  API_PCIC44_CARD_ALREADY_OPENED,
  API_PCIC44_CARD_NOT_OPEN,
  API_PCIC44_NOT_CARD_OWNER,

  API_PCIC44_FUNCTION_NOT_FOUND,

  API_PCIC44_INVALID_IDENT,
  API_PCIC44_INVALID_MMIO_REG,
  API_PCIC44_INVALID_IO_REG,
  API_PCIC44_INVALID_DMA_REG,
  API_PCIC44_BAD_RW_ADDRESS_OR_RANGE,

  API_PCIC44_CARD_NOT_BOOTED,

  API_PCIC44_UNKNOWN_CMD,

  API_INVALID_TRANSFER_TYPE,

  API_PCIC44_INVALID_PROCESSOR_NUMBER,

/*
 * Error codes for PCI BIOS - matching the spec.
 */
  API_PCI_SUCCESSFUL,
  API_PCI_FUNC_NOT_SUPPORTED,
  API_PCI_BAD_VENDOR_ID,
  API_PCI_DEVICE_NOT_FOUND,
  API_PCI_BAD_REGISTER_NUMBER,
  API_PCI_SET_FAILED,
  API_PCI_BUFFER_TOO_SMALL,
  API_PCI_UNKNOWN_ERROR

} API_ERROR_CODE;


typedef struct
{
	PVOID   HostBuffer;     /* host address for buffer (seg:offset) */
	PVOID   Callback;       /* primary function, setup by API */
	PVOID   Context;        /* secondary function, passed by API */
}
APICALLBACKPARAM;

typedef struct
{
    WORD    ProcNum;
    CARD_ID CardIdent;
} INTHANDLER;

typedef INTHANDLER *INTHANDLERPTR;


/*
 * ============================================================
 * PCI card info. Lists details of all PCI cards in the system.
 * ============================================================
 */
typedef struct {
    CARD_ID CardIdent;      /* Unique Identifier */
    WORD    wPadding;       /* Only needed on Win3.1x version */
    WORD    wVendorID;      /* Unique Vendor ID */
    WORD    wDeviceID;      /* Device ID assigned by Vendor */
    BYTE    bRevision;      /* Revision level of card */
    BYTE    bBaseClass;     /* Card's base class code */
    BYTE    bSubClass;      /* Sub-class code */
    BYTE    Padding;
#define PCI_NUM_ADDRESS_RANGES  6
    DWORD   dwPhysaddr[PCI_NUM_ADDRESS_RANGES]; /* Possible physical addresses */
} PCIC44_PCI_CARD_INFO;


#define API_PCIC44_GetVersion              0x0

#define API_PCIC44_GetCardInfo             0x1

#define API_PCIC44_OpenCard                0x2
#define API_PCIC44_CloseCard               0x3

#define API_PCIC44_ResetCard               0x4
#define API_PCIC44_AssertReset             0x5
#define API_PCIC44_DeassertReset           0x6
#define API_PCIC44_RunProcessor            0x7
#define API_PCIC44_InstallInterruptHandler 0x8 

#define API_PCIC44_ReadBlockDirect         0x9
#define API_PCIC44_WriteBlockDirect        0xa

#define API_PCIC44_ReadROM 0xb
#define API_PCIC44_WriteROM 0xc

#define API_PCIC44_ReadPCIConfigByte   0xd
#define API_PCIC44_WritePCIConfigByte  0xe
#define API_PCIC44_ReadPCIConfigWord   0xf
#define API_PCIC44_WritePCIConfigWord  0x10
#define API_PCIC44_ReadPCIConfigDWord  0x11
#define API_PCIC44_WritePCIConfigDWord 0x12

#define API_PCIC44_ReadMemMapIOByte    0x13
#define API_PCIC44_WriteMemMapIOByte   0x14
#define API_PCIC44_ReadMemMapIOWord    0x15
#define API_PCIC44_WriteMemMapIOWord   0x16
#define API_PCIC44_ReadMemMapIODWord   0x17
#define API_PCIC44_WriteMemMapIODWord  0x18

#define API_PCIC44_ReadIOByte          0x19
#define API_PCIC44_WriteIOByte         0x1a
#define API_PCIC44_ReadIOWord          0x1b
#define API_PCIC44_WriteIOWord         0x1c
#define API_PCIC44_ReadIODWord         0x1d
#define API_PCIC44_WriteIODWord        0x1e

#define API_PCIC44_GetPCICardInfo      0x1f

#define API_PCIC44_Last_API_Function   API_PCIC44_GetPCICardInfo

#ifdef __cplusplus
}
#endif

#endif /* __PCIAPI_H */

/**************************************************************************
 *  End of File: pciapi.h
 **************************************************************************/
