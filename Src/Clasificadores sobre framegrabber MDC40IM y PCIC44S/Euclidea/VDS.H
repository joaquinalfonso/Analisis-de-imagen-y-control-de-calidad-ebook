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
 *   Date     : 14/03/96                                                  *
 *   Filename : VDS.H                                                     *
 *                                                                        *
 *   Version History:                                                     *
 *      Version      Date     Author  Reason for Change                   *
 *      1.00         10/7/95  RP      Release version                     *
 *                                                                        *
 *   Description:                                                         *
 *      This file contains the VDS interface module header.               *
 *                                                                        *
 **************************************************************************/

#ifndef VDS_H
#define VDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 * Data types.
 **************************************************************************/

#ifdef DOS

#ifndef C44DEV_H

#ifndef BOOL
#define BOOL  int
#define FALSE 0
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

#endif /* C44DEV_H */

#else  /* DOS */

#ifdef WINDOWS
#include <windows.h>
#endif /* WINDOWS */

#endif /* DOS */

/**************************************************************************
 * Definitions.
 **************************************************************************/

/*
 * VDS flags bits.
 */
#define VDS_AUTO_COPY_IN_OUT_BUF_BIT            1
#define VDS_DISABLE_AUTO_BUF_ALLOC_BIT          2
#define VDS_DISABLE_AUTO_REMAP_BIT              3
#define VDS_NOT_64K_PHYS_BOUND_BIT              4
#define VDS_NOT_128K_PHYS_BOUND_BIT             5
#define VDS_CPY_PG_TBL_SCAT_GAT_REMAP_BIT       6
#define VDS_ALLOW_NO_PRES_PG_SCAT_GAT_REMAP_BIT 7

#define VDS_NULL_MASK                            0x00
#define VDS_AUTO_COPY_IN_OUT_BUF_MASK            0x02
#define VDS_DISABLE_AUTO_BUF_ALLOC_MASK          0x04
#define VDS_DISABLE_AUTO_REMAP_MASK              0x08
#define VDS_NOT_64K_PHYS_BOUND_MASK              0x10
#define VDS_NOT_128K_PHYS_BOUND_MASK             0x20
#define VDS_CPY_PG_TBL_SCAT_GAT_REMAP_MASK       0x40
#define VDS_ALLOW_NO_PRES_PG_SCAT_GAT_REMAP_MASK 0x80

#define VDS_MAX_NUM_IN_EDDS                      0x64

/**************************************************************************
 * Typedefs.
 **************************************************************************/

typedef struct
{
  DWORD regionSize;	/* DDS_size */
  DWORD offset;		/* DDS_linear */
  WORD segOrSelector;	/* DDS_seg */
  WORD bufferID;	/* DDS_bufferID */
  DWORD physAddress;	/* DDS_physical */
} VDS_DDS;

typedef struct
{
  DWORD regionSize;
  DWORD offset;
  WORD segOrSelector;
  WORD reserved;      /* reserved field */
  WORD numAvailable;
  WORD numUsed;
  union
  {
    struct
    {
      DWORD physAddress;
      DWORD size;
    } region[VDS_MAX_NUM_IN_EDDS];
    struct
    {
      DWORD entry;
    } page[VDS_MAX_NUM_IN_EDDS * 2];
  } v;
} VDS_EDDS;

/**************************************************************************
 * Error Codes.
 **************************************************************************/

typedef enum
{
  VDS_REGION_NOT_IN_CONT_MEM            = 0x01,
  VDS_REGION_CROSSED_PHYS_ALIGN_BOUND   = 0x02,
  VDS_UNABLE_TO_LOCK_PAGES              = 0x03,
  VDS_NO_BUFFER_AVAILABLE               = 0x04,
  VDS_REGION_TOO_LARGE_FOR_BUFFER       = 0x05,
  VDS_BUFFER_IN_USE                     = 0x06,
  VDS_INVALID_MEMORY_REGION             = 0x07,
  VDS_REGION_NOT_LOCKED                 = 0x08,
  VDS_NUM_PHYS_PAGES_GREATER_THAN_TABLE = 0x09,
  VDS_INVALID_BUFFER_ID                 = 0x0A,
  VDS_BUFFER_BOUND_VIOLATED             = 0x0B,
  VDS_INVALID_DMA_CHAN_NUMBER           = 0x0C,
  VDS_DISABLE_COUNT_OVERFLOW            = 0x0D,
  VDS_DISABLE_COUNT_UNDERFLOW           = 0x0E,
  VDS_FUNCTION_NOT_SUPPORTED            = 0x0F,
  VDS_RESERVED_BITS_SET_IN_DX           = 0x10,

  VDS_SUCCESS                           = 0xFF
} VDSErrorCode;

/**************************************************************************
 * Function Prototypes
 **************************************************************************/

VDSErrorCode VDSGetVersion(WORD *, WORD *, WORD *, DWORD *, WORD *);
VDSErrorCode VDSLockDMARegion(VDS_DDS *, WORD);
VDSErrorCode VDSUnlockDMARegion(VDS_DDS *, WORD);
VDSErrorCode VDSScatterGatherLockDMARegion(VDS_EDDS *, WORD);
VDSErrorCode VDSScatterGatherUnlockDMARegion(VDS_EDDS *, WORD);
VDSErrorCode VDSRequestDMABuffer(VDS_DDS *, WORD);
VDSErrorCode VDSReleaseDMABuffer(VDS_DDS *, WORD);
VDSErrorCode VDSCopyIntoDMABuffer(VDS_DDS *, DWORD);
VDSErrorCode VDSCopyOutOfDMABuffer(VDS_DDS *, DWORD);
VDSErrorCode VDSDisableTranslation(WORD);
VDSErrorCode VDSEnableTranslation(WORD);

#ifdef __cplusplus
}
#endif

#endif /* VDS_H */

/**************************************************************************
 *  End of File: vds.h
 **************************************************************************/

