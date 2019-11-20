/*************************************************************************
*                                                                        *
* File               : regs.h                                            *
* Date first created : 25-7-94                                           *
* Author             : Rob Whitton                                       *
* Version            : 1.00                                              *
*                                                                        *
* Description        :                                                   *
*                                                                        *
* This file contains the register definitions that are used in video.c.  *
* This file is included by video.h. So the user may use these            *
* definitions in their own programs. eg IMAGE is the address of the      *
* image plane and OVERLAY is the address to the overlay plane.           *
*                                                                        *
*                                                                        *
* Updates            :                                                   *
*                                                                        *
**************************************************************************/


#ifndef REGS_H      /* Make sure this header is only included once */

#define REGS_H


/*************************************************************************
*                                                                        *
* Comment out this line for the MONO version of the library.             *
*                                                                        *
**************************************************************************/

/*#define COLOUR*/      /* Colour or MONO library */


/*************************************************************************
*                                                                        *
* Uncomment this line for an ILIB compatible version of the library.     *
*                                                                        *
**************************************************************************/

/*#define ILIB*/        /* ILIB compatible or not */


#ifdef COLOUR

/* BT254 ADC Internal Register Addresses */

#define ADC_CMND       (*((volatile int *) 0x300000))
#define ADC_IOUT0_DATA (*((volatile int *) 0x300001))
#define ADC_IOUT1_DATA (*((volatile int *) 0x300002))
#define ADC_IOUT2_DATA (*((volatile int *) 0x300003))
#define ADC_IOUT3_DATA (*((volatile int *) 0x300004))
#define ADC_IOUT4_DATA (*((volatile int *) 0x300005))
#define ADC_IOUT5_DATA (*((volatile int *) 0x300006))

#else

/* BT252 ADC Internal Register Addresses */

#define ADC_ADDR     (*((volatile int *) 0x300000))
#define ADC_RAM_DATA (*((volatile int *) 0x300001))
#define ADC_CI_DATA  (*((volatile int *) 0x300002))

#endif


/* BT261 Pixel Clock Generator Internal Addresses */

#define PIX_ADDR (*((volatile int *) 0x300008))
#define PIX_DATA (*((volatile int *) 0x300009))


/* BT473 (COLOUR) or BT477 (MONO) DAC Internal Register Addresses */

#define DAC_RAMW_ADDR (*((volatile int *) 0x300010))
#define DAC_RAM_DATA  (*((volatile int *) 0x300011))
#define DAC_MASK      (*((volatile int *) 0x300012))
#define DAC_OVW_ADDR  (*((volatile int *) 0x300014))
#define DAC_OV_DATA   (*((volatile int *) 0x300015))
#define DAC_CMND      (*((volatile int *) 0x300016))


/* Internal Sync Pulse Generator Addresses */

#define SYNC_MAN_ADDR  (*((volatile int *) 0x300018))
#define SYNC_AUTO_ADDR (*((volatile int *) 0x300019))
#define SYNC_LB_DATA   (*((volatile int *) 0x30001a))
#define SYNC_HB_DATA   (*((volatile int *) 0x30001b))


/* Video Control Register Address */

#define VID_CNTRL_REG (*((volatile int *) 0x30001c))


/* Misc Locations */

#define IM_MASK_ADDR     (*((volatile int *) 0x16000000))
#define IM_COLOUR_ADDR   (*((volatile int *) 0x15000000))
#define IM_MASK_ADDR_O   (*((volatile int *) 0x26000000))
#define IM_COLOUR_ADDR_O (*((volatile int *) 0x25000000))


#define IACKLOC_E     0x30000000
#define IACKLOC_O     0x30000001


/* Memory Block Addresses */

#define OVERLAY         0x20000000 /* Overlay start           */
#define IMAGE           0x10000000 /* Image start             */
#define LENGTH          0x00040000 /* Image length            */
#define FLASH_WR_OFFSET 0x4000000  /* Flash VRAM write offset */


/* Misc */

#ifndef COLOUR

#define RED_SYNC   0x04 /* Bit twiddles to use with the mono module */
#define GREEN_SYNC 0x08 /* when adding and removing sync from the   */
#define BLUE_SYNC  0x10 /* component outputs.                       */

#endif


#endif
