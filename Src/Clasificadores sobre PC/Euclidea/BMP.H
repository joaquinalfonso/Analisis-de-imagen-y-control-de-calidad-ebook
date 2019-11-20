/****************************************************************************\
**  Title:   BMP.H                                                          **
**  Purpose: BMP Header file                                                **
**  Version: 1.0                                                            **
**  Date:    October 1991                                                   **
**  Author:  James D. Murray, Anaheim, CA, USA                              **
**                                                                          **
**  This header file contains the structures for the three flavors of the   **
**  BMP image file format (OS/2 1.x, WIndows 3.0, and OS/2 2.0).  Each BMP  **
**  file will contain a BMPINFO header followed by either a PMINFPHEAD,     **
**  WININFOHEAD, or PM2INFOHEAD header.  To simplify reading and writing    **
**  BMP files the BMP file format structure defined in BMP.H contains       **
**  structures for all three flavors of the BMP image file format.          **
**                                                                          **
**  Copyright (C) 1991 Graphics Software Labs.  All rights reserved.        **
\****************************************************************************/
#ifndef BMP_H
#define BMP_H   1

#define TByte   unsigned char
#define TWord   unsigned int
#define TDWord  unsigned long
#define TShort  short
#define TVoid    void


#define COMPRESS_RGB        0L      /* No compression               */
#define COMPRESS_RLE8       1L      /* 8 bits per pixel compression */
#define COMPRESS_RLE4       2L      /* 4 bits per pixel compression */
#define BMP_ID              0x4d42  /* BMP "magic" number           */

#define LSN(value)	((value) & 0x0f)	    /* Least-significant nibble */
#define MSN(value)	(((value) & 0xf0) >> 4)	/* Most-significant nibble  */

/*
**  BMP File Format Bitmap Header.
*/
typedef struct _BmpInfo     /* Offset   Description                      */
{
    TWord   Type;            /*  00h     File Type Identifier             */
    TDWord  FileSize;        /*  02h     Size of File                     */
    TWord   Reserved1;       /*  06h     Reserved (should be 0)           */
    TWord   Reserved2;       /*  08h     Reserved (should be 0)           */
    TDWord  Offset;          /*  0Ah     Offset to bitmap data            */
} BMPINFO;

/*
**  Presentation Manager (OS/2 1.x) Information Header Format.
*/
typedef struct _PmInfoHeader   /* Offset   Description                     */
{
    TDWord   Size;               /*  0Eh     Size of Remianing Header        */
    TWord    Width;              /*  12h     Width of Bitmap in Pixels       */
    TWord    Height;             /*  14h     Height of Bitmap in Pixels      */
    TWord    Planes;             /*  16h     Number of Planes                */
    TWord    BitCount;           /*  18h     Color Bits Per Pixel            */
} PMINFOHEAD;

/*
**  Windows 3.x Information Header Format.
*/
typedef struct _WinInfoHeader   /* Offset  Description                      */
{
    TDWord  Size;                /*  0Eh    Size of Remianing Header         */
    TDWord  Width;               /*  12h    Width of Bitmap in Pixels        */
    TDWord  Height;              /*  16h    Height of Bitmap in Pixels       */
    TWord   Planes;              /*  1Ah    Number of Planes                 */
    TWord   BitCount;            /*  1Ch    Bits Per Pixel                   */
    TDWord  Compression;         /*  1Eh    Compression Scheme (0=none)      */
    TDWord  SizeImage;           /*  22h    Size of bitmap in bytes          */
    TDWord  XPelsPerMeter;       /*  26h    Horz. Resolution in Pixels/Meter */
    TDWord  YPelsPerMeter;       /*  2Ah    Vert. Resolution in Pixels/Meter */
    TDWord  ClrUsed;             /*  2Eh    Number of Colors in Color Table  */
    TDWord  ClrImportant;        /*  32h    Number of Important Colors       */
} WININFOHEAD;

/*
**  Presentation Manager (OS/2 2.0) Information Header Format.
*/
typedef struct _Pm2InfoHeader   /* Offset  Description                      */
{  
    TDWord   Size;               /*  0Eh    Size of Info Header (always 64)  */
    TWord    Width;              /*  12h    Width of Bitmap in Pixels        */
    TWord    Height;             /*  14h    Height of Bitmap in Pixels       */
    TWord    Planes;             /*  16h    Number of Planes                 */
    TWord    BitCount;           /*  18h    Color Bits Per Pixel             */
    TDWord   Compression;        /*  1Ah    Compression Scheme (0=none)      */
    TDWord   SizeImage;          /*  1Eh    Size of bitmap in bytes          */
    TDWord   XPelsPerMeter;      /*  22h    Horz. Resolution in Pixels/Meter */
    TDWord   YPelsPerMeter;      /*  26h    Vert. Resolution in Pixels/Meter */
    TDWord   ClrUsed;            /*  2Ah    Number of Colors in Color Table  */
    TDWord   ClrImportant;       /*  2Eh    Number of Important Colors       */
    TWord    Units;              /*  32h    Resolution Mesaurement Used      */
    TWord    Reserved;           /*  34h    Reserved FIelds (always 0)       */
    TWord    Recording;          /*  36h    Orientation of Bitmap            */
    TWord    Rendering;          /*  38h    Halftone Algorithm Used on Image */
    TDWord   Size1;              /*  3Ah    Halftone Algorithm Data          */
    TDWord   Size2;              /*  3Eh    Halftone Algorithm Data          */
    TDWord   ColorEncoding;      /*  42h    Color Table Format (always 0)    */
    TDWord   Identifier;         /*  46h    Misc. Field for Application Use  */
} PM2INFOHEAD;

/*
**  Presentation Manager (OS/2) RGB Color Triple.
*/
typedef struct _PmRgbTriple
{
    TByte   rgbBlue;             /* Blue Intensity Value  */
    TByte   rgbGreen;            /* Green Intensity Value */
    TByte   rgbRed;              /* Red Intensity Value   */
} PMRGBTRIPLE;

/*
**  Windows 3.x RGB Color Quadruple.
*/
typedef struct _WinRgbQuad
{
    TByte   rgbBlue;             /* Blue Intensity Value   */
    TByte   rgbGreen;            /* Green Intensity Value  */
    TByte   rgbRed;              /* Red Intensity Value    */
    TByte   rgbReserved;         /* Reserved (should be 0) */
} WINRGBQUAD;

/*
**  OS/2 2.0 RGB Color Quadruple.
*/
typedef struct _Pm2RgbQuad
{
    TByte   rgbBlue;             /* Blue Intensity Value   */
    TByte   rgbGreen;            /* Green Intensity Value  */
    TByte   rgbRed;              /* Red Intensity Value    */
    TByte   rgbReserved;         /* Reserved (should be 0) */
} PM2RGBQUAD;


/*
** Composite structure of the BMP image file format.
**
** This structure holds information for all three flavors of the BMP format.
*/
typedef struct _BmpHeader
{
    BMPINFO      Header;        /* Bitmap Header                */
    PMINFOHEAD   PmInfo;        /* OS/2 1.x Information Header  */
    PMRGBTRIPLE *PmColorTable;  /* OS/2 1.x Color Table         */
    WININFOHEAD  WinInfo;       /* Windows 3 Information Header */
    WINRGBQUAD  *WinColorTable; /* Windows 3 Color Table        */
    PM2INFOHEAD  Pm2Info;       /* OS/2 2.0 Information Header  */
    PM2RGBQUAD  *Pm2ColorTable; /* OS/2 2.0 Color Table         */
} BMPHEADER;

/*
**  Function prototypes
*/
TShort ReadBmpHeader(BMPHEADER *, FILE *);
TVoid  WriteBmpHeader(BMPHEADER *, FILE *);
TShort BmpEncodeScanLine(TByte *, TWord, TWord, TDWord, FILE *);
TShort BmpDecodeScanLine(TByte *, TWord, TWord, TDWord, FILE *);

#endif /* BMP_H */
