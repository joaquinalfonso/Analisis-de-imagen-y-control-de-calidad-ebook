#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

#include "bmp.h"


TByte GetByte(FILE *fic)
{
        TByte dato;
        fread(&dato, sizeof(TByte),1,fic);
		return(dato);
}

TWord GetWord(FILE *fic)
{
        TWord dato;
        fread(&dato, sizeof(TWord),1,fic);
		return(dato);
}

TDWord GetDword(FILE *fic)
{
        TDWord dato;
        fread(&dato, sizeof(TDWord),1,fic);
		return(dato);
}


TShort ReadBmpHeader(BMPHEADER *BmpHead,FILE *FpBmp)
{
    register TShort i;       /* Loop Counter */
    TDWord InfoHeaderSize;   /* Size of the BMP information header in bytes */
    TWord  NumColors;        /* Number of colors in color table */

    BmpHead->Header.Type      = GetWord(FpBmp);
    BmpHead->Header.FileSize  = GetDword(FpBmp);
    BmpHead->Header.Reserved1 = GetWord(FpBmp);
    BmpHead->Header.Reserved2 = GetWord(FpBmp);
    BmpHead->Header.Offset    = GetDword(FpBmp);

	InfoHeaderSize            = GetDword(FpBmp);

    /*
    ** The type of information found in a BMP structure is indicated by
    ** the Size (Information Headere Size) field with a non-zero value.
    */
    BmpHead->PmInfo.Size   = 0;
    BmpHead->WinInfo.Size  = 0;
    BmpHead->Pm2Info.Size  = 0;

    /*
	** The size if the information header indicates if the BMP file
	** originated on an MS Windows or OS/2 Presentation Manager system.
    */
    if (InfoHeaderSize == 12L)         /* OS/2 1.x */
    {
        BmpHead->PmInfo.Size     = InfoHeaderSize;
        BmpHead->PmInfo.Width    = GetWord(FpBmp);
        BmpHead->PmInfo.Height   = GetWord(FpBmp);
        BmpHead->PmInfo.Planes   = GetWord(FpBmp);
        BmpHead->PmInfo.BitCount = GetWord(FpBmp);

        if (BmpHead->PmInfo.BitCount != 24)
        {
			/* Determine number of entries in color table */
            NumColors = (TWord) (1U << (BmpHead->PmInfo.Planes *
              BmpHead->PmInfo.BitCount));
 
            /* Allocate memory for the color table entries */
			if ((BmpHead->PmColorTable = (PMRGBTRIPLE *)
                 calloc((size_t) NumColors, sizeof(PMRGBTRIPLE))) ==
				 (PMRGBTRIPLE *) NULL)
				return(-1);

            /* Read in the color table one color triple at a time */
            for (i = 0; i < NumColors; i++)
			{
                BmpHead->PmColorTable[i].rgbBlue  = GetByte(FpBmp);
                BmpHead->PmColorTable[i].rgbGreen = GetByte(FpBmp);
                BmpHead->PmColorTable[i].rgbRed   = GetByte(FpBmp);
            }
		}
    }
    else                               /* Windows 3 */
    if (InfoHeaderSize == 40L)
    {                              
        BmpHead->WinInfo.Size          = InfoHeaderSize;
        BmpHead->WinInfo.Width         = GetDword(FpBmp);
		BmpHead->WinInfo.Height        = GetDword(FpBmp);
		BmpHead->WinInfo.Planes        = GetWord(FpBmp);
        BmpHead->WinInfo.BitCount      = GetWord(FpBmp);
		BmpHead->WinInfo.Compression   = GetDword(FpBmp);
		BmpHead->WinInfo.SizeImage     = GetDword(FpBmp);
        BmpHead->WinInfo.XPelsPerMeter = GetDword(FpBmp);
        BmpHead->WinInfo.YPelsPerMeter = GetDword(FpBmp);
        BmpHead->WinInfo.ClrUsed       = GetDword(FpBmp);
        BmpHead->WinInfo.ClrImportant  = GetDword(FpBmp);

        /* Read in the color table (if any) */
        if (BmpHead->WinInfo.BitCount != 24 || BmpHead->WinInfo.ClrUsed != 0)
		{
			/* Determine number of entries in color table */
            if (BmpHead->WinInfo.ClrUsed)
                NumColors = (TWord) BmpHead->WinInfo.ClrUsed;
            else
                NumColors = (TWord) (1U << (BmpHead->WinInfo.Planes *
                  BmpHead->WinInfo.BitCount));

            /* Allocate memory for the color table entries */
			if ((BmpHead->WinColorTable = (WINRGBQUAD *)
                 calloc((size_t) NumColors, sizeof(WINRGBQUAD))) ==
				 (WINRGBQUAD *) NULL)
				return(-1);
      
            /* Read in the color table one color quad at a time */
            for (i = 0; i < NumColors; i++)
			{
                BmpHead->WinColorTable[i].rgbBlue     = GetByte(FpBmp);
                BmpHead->WinColorTable[i].rgbGreen    = GetByte(FpBmp);
                BmpHead->WinColorTable[i].rgbRed      = GetByte(FpBmp);
                BmpHead->WinColorTable[i].rgbReserved = GetByte(FpBmp);
			}
        }
    }
	else                               /* OS/2 2.0 */
    if (InfoHeaderSize == 64L)
    {                              
        BmpHead->Pm2Info.Size          = InfoHeaderSize;
        BmpHead->Pm2Info.Width         = (TWord) GetDword(FpBmp);
        BmpHead->Pm2Info.Height        = (TWord) GetDword(FpBmp);
        BmpHead->Pm2Info.Planes        = GetWord(FpBmp);
		BmpHead->Pm2Info.BitCount      = GetWord(FpBmp);
		BmpHead->Pm2Info.Compression   = GetDword(FpBmp);
        BmpHead->Pm2Info.SizeImage     = GetDword(FpBmp);
        BmpHead->Pm2Info.XPelsPerMeter = GetDword(FpBmp);
        BmpHead->Pm2Info.YPelsPerMeter = GetDword(FpBmp);
		BmpHead->Pm2Info.ClrUsed       = GetDword(FpBmp);
        BmpHead->Pm2Info.ClrImportant  = GetDword(FpBmp);
        BmpHead->Pm2Info.Units         = GetWord(FpBmp);
        BmpHead->Pm2Info.Reserved      = GetWord(FpBmp);
        BmpHead->Pm2Info.Recording     = GetWord(FpBmp);
		BmpHead->Pm2Info.Rendering     = GetWord(FpBmp);
        BmpHead->Pm2Info.Size1         = GetDword(FpBmp);
        BmpHead->Pm2Info.Size2         = GetDword(FpBmp);
        BmpHead->Pm2Info.ColorEncoding = GetDword(FpBmp);
        BmpHead->Pm2Info.Identifier    = GetDword(FpBmp);
      
        /* Read in the color table (if any) */
		if (BmpHead->Pm2Info.BitCount != 24 || BmpHead->Pm2Info.ClrUsed != 0)
		{
            /* Determine number of entries in color table */
			if (BmpHead->Pm2Info.ClrUsed)
                NumColors = (TWord) BmpHead->Pm2Info.ClrUsed;
            else
                NumColors = (TWord) (1U << (BmpHead->Pm2Info.Planes *
                  BmpHead->Pm2Info.BitCount));

			/* Allocate memory for the color table entries */
			if ((BmpHead->Pm2ColorTable = (PM2RGBQUAD *)
				 calloc((size_t) NumColors, sizeof(PM2RGBQUAD))) ==
				 (PM2RGBQUAD *) NULL)
				return(-1);

			/* Read in the color table one color quad at a time */
			for (i = 0; i < NumColors; i++)
			{
				BmpHead->Pm2ColorTable[i].rgbBlue     = GetByte(FpBmp);
				BmpHead->Pm2ColorTable[i].rgbGreen    = GetByte(FpBmp);
				BmpHead->Pm2ColorTable[i].rgbRed      = GetByte(FpBmp);
				BmpHead->Pm2ColorTable[i].rgbReserved = GetByte(FpBmp);
			}
		}
    }
    return(0);
}


TShort LeerVentana(BMPHEADER BmpHead, FILE *FpBmp, int Limites[], TByte Ventana[][MAX_VENTANA])
{
    int x, y;
    int AltoV, AnchoV, IniX, IniY, Ancho;
    unsigned long DesplIni, DesplLin;

    IniY = Limites[0];
    IniX = Limites[1];
    AltoV = Limites[2];
    AnchoV = Limites[3];

	/* Ir al comienzo de los datos */

    Ancho = (int) BmpHead.WinInfo.Width;

    fseek( FpBmp, BmpHead.Header.Offset, SEEK_SET );

    DesplIni = (unsigned long)IniY * Ancho + IniX;
    fseek( FpBmp, DesplIni, SEEK_CUR );

    DesplLin = Ancho - AnchoV;

    for( y=0 ; y < AltoV; y++) {
        for( x=0 ; x < AnchoV ; x++ ) {
            Ventana[y][x] = GetByte(FpBmp);
        }
        fseek( FpBmp, DesplLin, SEEK_CUR );
    }

    return(0);

}

