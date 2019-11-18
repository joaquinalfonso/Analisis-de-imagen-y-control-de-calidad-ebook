/**************************************************************************
 *                                                                        *
 *   Loughborough Sound Images plc                                        *
 *   Loughborough Park                                                    *
 *   Ashby Road                                                           *
 *   Loughborough                                                         *
 *   Leicestershire           Tel : +44 1509 634444                       *
 *   LE11 3NE                 FAX : +44 1509 634450                       *
 *                                                                        *
 *   Software Copyright (c) 1995                                          *
 *                                                                        *
 *   Program  : Interface libraries for PCI/C44                           *
 *   Date     : 14/03/96                                                  *
 *   Filename : C44LIB.H                                                  *
 *                                                                        *
 *   Version History:                                                     *
 *      Version      Date      Author  Reason for Change                  *
 *      1.00         14/03/96  RP      Release version                    *
 *      2.00         24/06/96  SW      Sync with Win32 Library            *
 *      2.10         29/10/96  SL/SW   Incorporate LSI-France's floating  *
 *                                     point conversion routines.         *
 *                                                                        *
 *   Description:                                                         *
 *      This file contains macros, type definitions & function prototypes *
 *   for the PCI/C44S interface library.                                  *
 *                                                                        *
 **************************************************************************/

#ifndef C44LIB_H
#define C44LIB_H

#ifdef  __cplusplus
extern "C" {
#endif

/**************************************************************************
 * Data types.
 **************************************************************************/

#ifdef DOS

#ifndef BOOL
#define BOOL  int
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  !FALSE
#endif

typedef unsigned long  DWORD,  *PDWORD;
typedef unsigned short WORD,   *PWORD;
typedef unsigned char  BYTE,   *PBYTE;

typedef unsigned long  ULONG,  *PULONG;
typedef unsigned int   UINT,   *PUINT;
typedef unsigned short USHORT, *PUSHORT;
typedef unsigned char  UCHAR,  *PUCHAR;

typedef long           LONG,   *PLONG;
typedef int            INT,    *PINT;
typedef char           CHAR,   *PCHAR,   **PPCHAR;
typedef void           VOID,   *PVOID,   **PPVOID;
#endif
 
#ifdef WINDOWS
typedef VOID   *PVOID, **PPVOID;
typedef unsigned long  ULONG,  *PULONG;
#endif

/*
 * Include this now because all the above typedefs are required
 * for correct processing.
 */
#include "pciapi.h"
#include "vds.h"

/*
 * For calling VxD.
 */
typedef void (far *FUNCPTR)(void);

/******************************************************************************
 *       This definition defines the number of times the status register is   *
 *       polled before concluding that the board is not responding.           *
 ******************************************************************************/
#define    DSP_TIMEOUT         0xFFFF
/******************************************************************************
 *       These definitions are use where we want to specify DSP specific      *
 *       values like start address of Shared Memory, etc                             *
 ******************************************************************************/
#define    DSP_SHMEM_START            0xC0000000L  /* Address of 1st Shared Memory loc  */
#define    DSP_SHMEM_SEMA_LOC         0xC0000080L  /* Address of first semaphore */
#define    DSP_SHMEM_LENGTH           0x00080000L  /* Length of Shared Memory */
#define    DSP_LENGTH_BEFORE_SEMA_LOC 0x00000081L  /* Number of words before semaphore */
#define    DSP_LENGTH_AFTER_SEMA_LOC  0x0000007EL  /* Number of words after semaphore */

#define    NUMBER_OF_PROCS 4 
#define    ONBOARD_MONITOR_A   "c4xloada.rom"
#define    ONBOARD_MONITOR_B   "c4xloadb.rom"
#define    ONBOARD_MONITOR_C   "c4xloadc.rom"
#define    ONBOARD_MONITOR_D   "c4xloadd.rom"
#define    MONITOR_LENGTH      0x100L            /* Length of Onboard mon */
#define    C44_MONITOR_OFFSET MONITOR_LENGTH

#define    V0_TBC_ADDRESS 0x200000L
#define    V1_TBC_ADDRESS 0xC0000000L

#define    DMA_BASE        0x80000000L
#define    LOCKED_DMA_BASE 0x90000000L

/*
 * Constants which determine data transfer direction of IntercardDMA().
 */
#define ICDMA_C44_TO_PCI   1   /* From PCI/C44S shared RAM to other PCI card */
#define ICDMA_PCI_TO_C44   2   /* From other PCI card to PCI/C44S shared RAM */


/******************************************************************************
 *       These definitions are use where we want to specify DSP specific      *
 *       values                                                               *
 ******************************************************************************/
#define    IVTP_ADDR           0x2ff800L
#define    TVTP_ADDR           0x2ff800L
#define    IACK_ADDR           0x300000L
#define    C44_GMCR_VAL        0x3DBC8010L
#define    C44_LMCR_VAL        0x3D74A050L /* Should be 0x3D748010 for node A */
 

/* Do not include following for DB40 */
#ifndef PROD_C40

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

#endif

/****************************************************************************
 *      The magic number to shoe the COFF target processor                    *
 ****************************************************************************/
#define     COFF_C40_MAGIC_NO          0x0093L


#define     SECTION_CODE        0x0
#define     SECTION_DATA        0x1

#define    PRI_LOAD_FUNCTION          0x1L
#define    SEC_LOAD_FUNCTION          0x2L

/**************************************************************************
 * Macros
 **************************************************************************/

/*
 * For extra speed, MS compilers support inlining of C code.
 * And if using C++, C++ supports inlined code.
 */
#ifdef __BORLANDC__
  #define INLINE
#else
  #if (_MSC_VER == 800)
    #define INLINE __inline
  #endif /* _MSC_VER */
#endif /* __BORLANDC__ */

/**************************************************************************
 * Error Codes.
 **************************************************************************/

typedef enum
{
  PCIC44_SUCCESS = 0,
  PCIC44_FAILURE,
  PCIC44_NOT_INITIALISED,
  PCIC44_NOT_IN_VMM,
  PCIC44_VxD_NOT_LOADED,
  PCIC44_VxD_INCORRECT_VERSION,
  PCIC44_VXD_API_FUNCTION_NOT_FOUND,
  PCIC44_NOT_CARD_OWNER,
  PCIC44_CARD_ALREADY_OPENED,
  PCIC44_CARD_NOT_OPEN,
  PCIC44_CARD_NOT_BOOTED,
  PCIC44_UNABLE_TO_GET_CARD_INFO,
  PCIC44_MEMORY_ALLOCATION_FAULT,
  PCIC44_UNABLE_TO_OPEN_COFF_FILE,
  PCIC44_INVALID_COFF_FILE,
  PCIC44_INVALID_CARD_IDENT,
  PCIC44_INVALID_MEMMAP_IO_REG,
  PCIC44_INVALID_IO_REG,
  PCIC44_INVALID_DMA_REG,
  PCIC44_INVALID_OFFSET,
  PCIC44_UNKNOWN_MAILBOX_NUMBER,
  PCIC44_BAD_RW_ADDRESS_OR_RANGE,
  PCIC44_INVALID_TRANSFER_TYPE,
  PCIC44_UNKNOWN_CARD_COMMAND,
  PCIC44_UNKNOWN_VXD_API_ERROR,
  PCIC44_CANNOT_FIND_PCI_CFG,
  PCIC44_CANNOT_FIND_BOARD,
  PCIC44_INVALID_PROCESSOR_NUMBER,
/*
 * PCI BIOS access functions.
 */
  PCIC44_PCI_SUCCESSFUL,
  PCIC44_PCI_FUNC_NOT_SUPPORTED,
  PCIC44_PCI_BAD_VENDOR_ID,
  PCIC44_PCI_DEVICE_NOT_FOUND,
  PCIC44_PCI_BAD_REGISTER_NUMBER,
  PCIC44_PCI_SET_FAILED,
  PCIC44_PCI_BUFFER_TOO_SMALL,
  PCIC44_PCI_UNKNOWN_ERROR
} PCIC44ErrorCode;

/**************************************************************************
 * Typedefs.
 **************************************************************************/

#ifdef WINDOWS

typedef HANDLE CARD_HANDLE;

#else /* WINDOWS */

#ifdef DOS

typedef struct
{
  CARD_ID CardIdent;
#ifdef WINDOWS
  HINSTANCE hInstance;
#endif
} CARD_HANDLE;

#endif /* DOS */

#endif /* WINDOWS */

/**************************************************************************
 * Data available
 **************************************************************************/

extern PCIC44_CARD_INFO sCardInfo[];

/**************************************************************************
 * Function Prototypes
 **************************************************************************/

#ifdef DOS
  #define CALLING_CONV
#else
  #ifdef WINDOWS
	 #define CALLING_CONV __export FAR PASCAL
  #endif /* WINDOWS */
#endif /* DOS */

/*****************************************************
 * THE FOLLOWING FUNCTIONS ARE FOR USER APPLICATIONS *
 *****************************************************/
PCIC44ErrorCode CALLING_CONV InitPCIC44Lib(void);
PCIC44ErrorCode CALLING_CONV GetCardIdentity(char *, CARD_ID *);
PCIC44ErrorCode CALLING_CONV GetPCICardInfo(WORD, PCIC44_PCI_CARD_INFO *);
PCIC44ErrorCode CALLING_CONV GetCardNumProcs(CARD_ID, WORD *);
PCIC44ErrorCode CALLING_CONV OpenSystem(void);
PCIC44ErrorCode CALLING_CONV OpenCard(CARD_ID);
PCIC44ErrorCode CALLING_CONV CloseSystem(void);
PCIC44ErrorCode CALLING_CONV CloseCard(CARD_ID);
PCIC44ErrorCode CALLING_CONV Read_Shared_Mem_Block(CARD_ID, ULONG, ULONG, ULONG *, BOOL);
PCIC44ErrorCode CALLING_CONV Read_Shared_Mem_Block_Float(CARD_ID, ULONG, ULONG, float *, BOOL);
PCIC44ErrorCode CALLING_CONV Write_Shared_Mem_Block(CARD_ID, ULONG, ULONG, ULONG *, BOOL);
PCIC44ErrorCode CALLING_CONV Write_Shared_Mem_Block_Float(CARD_ID, ULONG, ULONG, float *, BOOL);
PCIC44ErrorCode CALLING_CONV RequestHostBuffer(DWORD *, DWORD *, DWORD);
PCIC44ErrorCode CALLING_CONV ReleaseHostBuffer(DWORD, DWORD);
PCIC44ErrorCode CALLING_CONV DMARead(CARD_ID, WORD, DWORD, DWORD, DWORD, BOOL);
PCIC44ErrorCode CALLING_CONV DMAWrite(CARD_ID, WORD, DWORD, DWORD, DWORD, BOOL);
PCIC44ErrorCode CALLING_CONV IntercardDMA(CARD_ID, WORD, DWORD, DWORD, DWORD, WORD, BOOL);
PCIC44ErrorCode CALLING_CONV CheckDMAStatus(CARD_ID, WORD, BOOL *, BOOL);
PCIC44ErrorCode CALLING_CONV InterruptProcessor(CARD_ID, WORD);
PCIC44ErrorCode CALLING_CONV InstallInterruptHandler(CARD_ID, FUNCPTR, INTHANDLERPTR);
PCIC44ErrorCode CALLING_CONV LoadAndRunObjectFile(CARD_ID, WORD, char *);
PCIC44ErrorCode CALLING_CONV ReadMailbox(CARD_ID, WORD, DWORD *);
PCIC44ErrorCode CALLING_CONV WriteMailbox(CARD_ID, WORD, DWORD);


/*****************************************************
 * THE FOLLOWING FUNCTIONS ARE FOR INTERNAL USE ONLY *
 *****************************************************/
PCIC44ErrorCode CALLING_CONV GetVxDVersion(WORD *);
PCIC44ErrorCode CALLING_CONV GetCardInfo(PCIC44_CARD_INFO *); 
PCIC44ErrorCode CALLING_CONV ReadBlockDirect(CARD_ID, DWORD, VOID *, DWORD, WORD);
PCIC44ErrorCode CALLING_CONV WriteBlockDirect(CARD_ID, DWORD, VOID *, DWORD, WORD);
PCIC44ErrorCode CALLING_CONV Read_TBC_Value(CARD_ID, WORD, VOID *);
PCIC44ErrorCode CALLING_CONV Write_TBC_Value(CARD_ID, WORD, VOID *);
PCIC44ErrorCode CALLING_CONV ReadROMDirect(CARD_ID, DWORD, VOID *);
PCIC44ErrorCode CALLING_CONV WriteROMDirect(CARD_ID, DWORD, VOID *);
PCIC44ErrorCode CALLING_CONV ReadLocalDMARegister(CARD_ID, WORD, DWORD *, BOOL);
PCIC44ErrorCode CALLING_CONV WriteLocalDMARegister(CARD_ID, WORD, DWORD, BOOL);
PCIC44ErrorCode CALLING_CONV ResetSystem(void);
PCIC44ErrorCode CALLING_CONV ResetCard(CARD_ID);
PCIC44ErrorCode CALLING_CONV AssertReset(CARD_ID);
PCIC44ErrorCode CALLING_CONV DeassertReset(CARD_ID);
PCIC44ErrorCode CALLING_CONV LoadProcessor (CARD_ID, WORD, char *);
PCIC44ErrorCode CALLING_CONV RunProcessor(CARD_ID, WORD);
PCIC44ErrorCode CALLING_CONV ReadPCIConfigDWord(CARD_ID, WORD, DWORD *);
PCIC44ErrorCode CALLING_CONV WritePCIConfigDWord(CARD_ID, WORD, DWORD);
PCIC44ErrorCode CALLING_CONV ReadMemMapIOByte(CARD_ID, WORD, BYTE *);
PCIC44ErrorCode CALLING_CONV WriteMemMapIOByte(CARD_ID, WORD, BYTE);
PCIC44ErrorCode CALLING_CONV ReadMemMapIOWord(CARD_ID, WORD, WORD *);
PCIC44ErrorCode CALLING_CONV WriteMemMapIOWord(CARD_ID, WORD, WORD);
PCIC44ErrorCode CALLING_CONV ReadMemMapIODWord(CARD_ID, WORD, DWORD *);
PCIC44ErrorCode CALLING_CONV WriteMemMapIODWord(CARD_ID, WORD, DWORD);
PCIC44ErrorCode CALLING_CONV ReadIOByte(CARD_ID, WORD, BYTE *);
PCIC44ErrorCode CALLING_CONV WriteIOByte(CARD_ID, WORD, BYTE);
PCIC44ErrorCode CALLING_CONV ReadIOWord(CARD_ID, WORD, WORD *);
PCIC44ErrorCode CALLING_CONV WriteIOWord(CARD_ID, WORD, WORD);
PCIC44ErrorCode CALLING_CONV ReadIODWord(CARD_ID, WORD, DWORD *);
PCIC44ErrorCode CALLING_CONV WriteIODWord(CARD_ID, WORD, DWORD);

/*
 * Utility Functions.
 */
DWORD CALLING_CONV GetBits(DWORD, int, int);
void CALLING_CONV SetBits(DWORD*, int, int, DWORD);
void ReadSerialNumber(int, int *);

/*
 * The standard IO functions (inp(), outp(), inpw() and outpw() ) are ok,
 * but additional inpdw() and outpdw() are required on 386 and above
 * machines, because most 16 bit compilers do not supply them as standard.
 */
DWORD CALLING_CONV inpdw(unsigned);
void CALLING_CONV outpdw(unsigned, DWORD);


extern void private_MsgHelperCallback(void);


#ifdef  __cplusplus
}
#endif

#endif /* C44LIB_H */

/**************************************************************************
 *  End of File: c44lib.h
 **************************************************************************/

