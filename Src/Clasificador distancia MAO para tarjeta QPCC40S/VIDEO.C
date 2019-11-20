/*************************************************************************
*                                                                        *
* File               : video.c                                           *
* Date first created : 25-7-94                                           *
* Author             : Rob Whitton                                       *
* Version            : 1.00                                              *
*                                                                        *
* Description        :                                                   *
*                                                                        *
* This file contains all of the C source for the support functions that  *
* accompany the imaging module. It is designed to be simple and easily   *
* modified to the users own requirements. To make use of the SW in this  *
* file it should be compiled and linked with the users own applicaion    *
* which should include the file video.h. To swap between mono and colour *
* versions of this file see regs.h.                                      *
*                                                                        *
* All user functions in this library are prefixed with video_            *
*                                                                        *
*                                                                        *
* Updates            :                                                   *
*                                                                        *
**************************************************************************/


#include <stddef.h>
#include <stdlib.h>

#include "video.h"


#define STACK_SIZE 32  /* This specifies the heap space used by */
                       /* video_fill. If it is all used up then */
                       /* additional blocks of heap space of    */
                       /* this size are allocated on the heap.  */


typedef struct stack_struct  /* This typedef is used for each stack */
  {                          /* entry used by video_fill.           */
  int x;
  int y;
  } stack_type;


#ifdef COLOUR

/*************************************************************************
*                                                                        *
* Define the PAL video mode with external sync. See the module manual    *
* for more details. (COLOUR)                                             *
*                                                                        *
**************************************************************************/

video_setup_type ccir_esync = {
  0x60,                               /* ADC command register   */
  0x94, 0x00,                         /* IOUT registers         */
  0x94, 0x00,
  0x94, 0x00,
  0x76,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,                      
    0x17, 0x00,
    0x5a, 0x00,
    0xae, 0x00,
    0x94, 0x03,
    0x07, 0x00,
    0x0c, 0x00,
    0x33, 0x00,
    0x71, 0x02,
    0x39, 0x00,
    0x9e, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x76,
    0x00, 0x00, 0x00, 0x46, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x46,
    0x00, 0x16, 0x01, 0x16, 0x03,
    0xdf, 0x00, 0x9c, 0x02, 0x68,
    0x00, 0x95, 0x03, 0xaf, 0x03
  },
  0x11,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xffffff,                           /* The mask register      */
  0x000000,                           /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  512                                 /* Number of rows         */
  };


/*************************************************************************
*                                                                        *
* Define the NTSC video mode with external sync. See the module manual   *
* for more details. (COLOUR)                                             *
*                                                                        *
**************************************************************************/

video_setup_type rs170_esync = {
  0x60,                               /* ADC command register   */
  0x98, 0x00,                         /* IOUT registers         */
  0x98, 0x00,
  0x98, 0x00,
  0x66,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,                      
    0x17, 0x00,
    0x5b, 0x00,
    0x9d, 0x00,
    0x8e, 0x03,
    0x07, 0x00,
    0x0d, 0x00,
    0x29, 0x00,
    0x0d, 0x02,
    0x38, 0x00,
    0x9a, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x62,
    0x00, 0x00, 0x00, 0x3b, 0x00,
    0x00, 0x00, 0x5f, 0x00, 0x65,
    0x00, 0xb9, 0x00, 0xb9, 0x02,
    0x36, 0x02, 0xbd, 0x00, 0x58,
    0x00, 0xf6, 0x02, 0x0b, 0x03
  },
  0x31,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xffffff,                           /* The mask register      */
  0x000000,                           /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  480                                 /* Number of rows         */
  };

  
/*************************************************************************
*                                                                        *
* Define the PAL video mode with internal sync. See the module manual    *
* for more details. (COLOUR)                                             *
*                                                                        *
**************************************************************************/

video_setup_type ccir_isync = {
  0x60,                               /* ADC command register   */
  0x94, 0x00,                         /* IOUT registers         */
  0x94, 0x00,
  0x94, 0x00,
  0x77,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,                      
    0x17, 0x00,
    0x5a, 0x00,
    0xae, 0x00,
    0x94, 0x03,
    0x07, 0x00,
    0x0c, 0x00,
    0x33, 0x00,
    0x71, 0x02,
    0x39, 0x00,
    0x9e, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x76,
    0x00, 0x00, 0x00, 0x46, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x46,
    0x00, 0x16, 0x01, 0x16, 0x03,
    0xdf, 0x00, 0x9c, 0x02, 0x68,
    0x00, 0x95, 0x03, 0xaf, 0x03
  },
  0x11,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xffffff,                           /* The mask register      */
  0x000000,                           /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  512                                 /* Number of rows         */
  };


/*************************************************************************
*                                                                        *
* Define the NTSC video mode with internal sync. See the module manual   *
* for more details. (COLOUR)                                             *
*                                                                        *
**************************************************************************/

video_setup_type rs170_isync = {
  0x60,                               /* ADC command register   */
  0x98, 0x00,                         /* IOUT registers         */
  0x98, 0x00,
  0x98, 0x00,
  0x67,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,                      
    0x17, 0x00,
    0x5b, 0x00,
    0x9d, 0x00,
    0x8e, 0x03,
    0x07, 0x00,
    0x0d, 0x00,
    0x29, 0x00,
    0x0d, 0x02,
    0x38, 0x00,
    0x9a, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x62,
    0x00, 0x00, 0x00, 0x3b, 0x00,
    0x00, 0x00, 0x5f, 0x00, 0x65,
    0x00, 0xb9, 0x00, 0xb9, 0x02,
    0x36, 0x02, 0xbd, 0x00, 0x58,
    0x00, 0xf6, 0x02, 0x0b, 0x03
  },
  0x31,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xffffff,                           /* The mask register      */
  0x000000,                           /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  480                                 /* Number of rows         */
  };

#else

/*************************************************************************
*                                                                        *
* Define the PAL video mode with external sync. See the module manual    *
* for more details. (MONO)                                               *
*                                                                        *
**************************************************************************/

video_setup_type ccir_esync = {
  0x0C,                               /* ADC command register   */
  0x94, 0x00,                         /* IOUT registers         */
  0x76,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,
    0x17, 0x00,
    0x5a, 0x00,
    0xae, 0x00,
    0x94, 0x03,
    0x07, 0x00,
    0x0c, 0x00,
    0x33, 0x00,
    0x71, 0x02,
    0x39, 0x00,
    0x9e, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x76,
    0x00, 0x00, 0x00, 0x46, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x46,
    0x00, 0x16, 0x01, 0x16, 0x03,
    0xdf, 0x00, 0x9c, 0x02, 0x68,
    0x00, 0x95, 0x03, 0xaf, 0x03
  },
  0x4a,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xff,                               /* The mask register      */
  0x00,                               /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  512                                 /* Number of rows         */
  };


/*************************************************************************
*                                                                        *
* Define the NTSC video mode with external sync. See the module manual   *
* for more details. (MONO)                                               *
*                                                                        *
**************************************************************************/

video_setup_type rs170_esync = {
  0x0c,                               /* ADC command register   */
  0x98, 0x00,                         /* IOUT registers         */
  0x66,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,                      
    0x17, 0x00,
    0x5b, 0x00,
    0x9d, 0x00,
    0x8e, 0x03,
    0x07, 0x00,
    0x0d, 0x00,
    0x29, 0x00,
    0x0d, 0x02,
    0x38, 0x00,
    0x9a, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x62,
    0x00, 0x00, 0x00, 0x3b, 0x00,
    0x00, 0x00, 0x5f, 0x00, 0x65,
    0x00, 0xb9, 0x00, 0xb9, 0x02,
    0x36, 0x02, 0xbd, 0x00, 0x58,
    0x00, 0xf6, 0x02, 0x0b, 0x03
  },
  0x6a,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xff,                               /* The mask register      */
  0x00,                               /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  480                                 /* Number of rows         */
  };


/*************************************************************************
*                                                                        *
* Define the PAL video mode with internal sync. See the module manual    *
* for more details. (MONO)                                               *
*                                                                        *
**************************************************************************/

video_setup_type ccir_isync = {
  0x0C,                               /* ADC command register   */
  0x94, 0x00,                         /* IOUT registers         */
  0x77,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,
    0x17, 0x00,
    0x5a, 0x00,
    0xae, 0x00,
    0x94, 0x03,
    0x07, 0x00,
    0x0c, 0x00,
    0x33, 0x00,
    0x71, 0x02,
    0x39, 0x00,
    0x9e, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x76,
    0x00, 0x00, 0x00, 0x46, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x46,
    0x00, 0x16, 0x01, 0x16, 0x03,
    0xdf, 0x00, 0x9c, 0x02, 0x68,
    0x00, 0x95, 0x03, 0xaf, 0x03
  },
  0x4a,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xff,                               /* The mask register      */
  0x00,                               /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  512                                 /* Number of rows         */
  };


/*************************************************************************
*                                                                        *
* Define the NTSC video mode with internal sync. See the module manual   *
* for more details. (MONO)                                               *
*                                                                        *
**************************************************************************/

video_setup_type rs170_isync = {
  0x0c,                               /* ADC command register   */
  0x98, 0x00,                         /* IOUT registers         */
  0x67,                               /* Video control register */
  {                                   /* Sync setup             */
    0x00, 0x04,                      
    0x17, 0x00,
    0x5b, 0x00,
    0x9d, 0x00,
    0x8e, 0x03,
    0x07, 0x00,
    0x0d, 0x00,
    0x29, 0x00,
    0x0d, 0x02,
    0x38, 0x00,
    0x9a, 0x01
  },
  {                                   /* Pixel clock registers  */
    0x98, 0x87, 0x32, 0x80, 0x62,
    0x00, 0x00, 0x00, 0x3b, 0x00,
    0x00, 0x00, 0x5f, 0x00, 0x65,
    0x00, 0xb9, 0x00, 0xb9, 0x02,
    0x36, 0x02, 0xbd, 0x00, 0x58,
    0x00, 0xf6, 0x02, 0x0b, 0x03
  },
  0x6a,                               /* DAC command register   */
  0xff,                               /* DAC mask register      */
  0xff,                               /* The mask register      */
  0x00,                               /* The colour register    */
  0x0f,                               /* The mask register      */
  0x00,                               /* The colour register    */
  512,                                /* Number of columns      */
  480                                 /* Number of rows         */
  };

#endif


/*************************************************************************
*                                                                        *
* Include the charset as a C file. A bit unconventional but it avoids    *
* messing around with externs and requiring the user to compile and link *
* with another file. The data could have been included here but it makes *
* a bit of a mess of this file.                                          *
*                                                                        *
**************************************************************************/

  
#include "charset.c"


/*************************************************************************
*                                                                        *
* Define all of the globals. In general the user probably won't want to  *
* access these from his own application but by making them externs in    *
* video.h access could be achieved. For example if the user wanted to    *
* directly modify the video control register then the soft copy kept in  *
* video_control_reg would need to be used otherwise the functions in     *
* this file will get out of step with the users own application.         *
* A better approach to maintain an object orientated type approach would *
* be to add additional functions to this file that will allow the user   *
* access to the local resources.                                         *
*                                                                        *
**************************************************************************/

#ifndef ILIB

int video_control_reg;              /* Soft copy of the video control  */
                                    /* reg which is mostly write only. */

#endif

int *g_scrn_addr = (int *) OVERLAY; /* Set the default screen base to  */
                                    /* to the OVERLAY page.            */
int g_col = 0;                      /* Set the plot color to 0.        */
int g_tcol = 0;                     /* Set the text color to 0.        */
int g_tbcol = -1;                   /* Set the text back color to 0.   */
int g_xc = 0;                       /* Set the text cursor X to 0.     */
int g_yc = 0;                       /* Set the text cursor Y to 0.     */
int g_x1 = 0;                       /* Set the window left margin.     */
int g_y1 = 0;                       /* Set the window top margin.      */
int g_x2 = 511;                     /* Set the window right margin.    */
int g_y2 = 511;                     /* Set the window_bottom_margin.   */
int g_xm = 511;                     /* Set the max x coordinate.       */
int g_ym = 511;                     /* Set the max y coordinate.       */  
int g_xs = 1;                       /* Set the font X scale factor.    */
int g_ys = 1;                       /* Set the font Y scale factor.    */
int g_pmask = 0xff;                 /* Set the pixel mask value.       */
int g_line_inc = LINE_LEN;

int iackloc_e = IACKLOC_E;          /* Store the IACK even loc in mem  */
int iackloc_o = IACKLOC_O;          /* Store the IACK odd loc in mem   */


/*************************************************************************
*                                                                        *
* Function: video_setup()                                                *
*                                                                        *
* This function sets up a video mode. It reads information from the video*
* setup type and configures the registers on the module appropriately.   *
* For details please refer to the module manual.                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

#ifdef COLOUR

void video_setup(video_setup_type *v)
  {
  int i;

  ADC_CMND = v->adc_cmnd;

  ADC_IOUT0_DATA = v->adc_iout0_data;
  ADC_IOUT1_DATA = v->adc_iout1_data;
  ADC_IOUT2_DATA = v->adc_iout2_data;
  ADC_IOUT3_DATA = v->adc_iout3_data;
  ADC_IOUT4_DATA = v->adc_iout4_data;
  ADC_IOUT5_DATA = v->adc_iout5_data;

  VID_CNTRL_REG = v->vid_cntrl_reg;
  video_control_reg = v->vid_cntrl_reg; /* Set the soft copy of this reg */

  SYNC_AUTO_ADDR = 0;
  for(i = 0; i < 22; i++)
    SYNC_HB_DATA = v->sync[i];
  
  PIX_ADDR = 0;
  for(i = 0; i < 30; i++)
    PIX_DATA = v->pcdata[i];

  DAC_CMND = v->dac_cmnd;

  DAC_MASK = v->dac_mask;

  IM_MASK_ADDR = v->mask_reg;
  IM_COLOUR_ADDR = v->colour_reg;

  IM_MASK_ADDR_O = v->mask_reg_overlay;
  IM_COLOUR_ADDR_O = v->colour_reg_overlay;

  g_x1 = 0;
  g_y1 = 0;
  g_xm = g_x2 = v->cols - 1;
  g_ym = g_y2 = v->rows - 1;
  }

#else

/*************************************************************************
*                                                                        *
* Function: video_setup()                                                *
*                                                                        *
* This function sets up a video mode. It reads information from the video*
* setup type and configures the registers on the module appropriately.   *
* For details please refer to the module manual.                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_setup(video_setup_type *v)
  {
  int i;

  ADC_ADDR    = 0;
  ADC_CI_DATA = v->adc_cmnd;

  ADC_ADDR    = 1;
  ADC_CI_DATA = v->adc_iout0_data;

  ADC_ADDR    = 2;
  ADC_CI_DATA = v->adc_iout1_data;

  VID_CNTRL_REG = v->vid_cntrl_reg;
  video_control_reg = v->vid_cntrl_reg; /* Set the soft copy of this reg */

  SYNC_AUTO_ADDR = 0;
  for(i = 0; i < 22; i++)
    SYNC_HB_DATA = v->sync[i];
  
  PIX_ADDR = 0;
  for(i = 0; i < 30; i++)
    PIX_DATA = v->pcdata[i];

  DAC_CMND = v->dac_cmnd;

  DAC_MASK = v->dac_mask;

  IM_MASK_ADDR = v->mask_reg;
  IM_COLOUR_ADDR = v->colour_reg;

  IM_MASK_ADDR_O = v->mask_reg_overlay;
  IM_COLOUR_ADDR_O = v->colour_reg_overlay;

  g_x1 = 0;
  g_y1 = 0;
  g_xm = g_x2 = v->cols - 1;
  g_ym = g_y2 = v->rows - 1;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_adc_lut_entry()                                    *
*                                                                        *
* This function initialise the entry 'e' in the ADC LUT to the specified *
* value.                                                                 *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_adc_lut_entry(int e, int x)
  {
  ADC_ADDR     = e;
  ADC_RAM_DATA = x;
  }
  

/*************************************************************************
*                                                                        *
* Function: video_set_adc_lut()                                          *
*                                                                        *
* This function sets the entire ADC LUT from the contents of the array x *
* which must be 256 elements long.                                       *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_adc_lut(int *x)
  {
  int i;

  ADC_ADDR = 0;

  for(i = 0; i < 256; i++)
    ADC_RAM_DATA = *x++;
  }


#endif


/*************************************************************************
*                                                                        *
* Function: video_set_videomask()                                        *
*                                                                        *
* Sets the image VRAM video mask register to x.                          *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/
  
void video_set_videomask(int x)
  {
  IM_MASK_ADDR = x;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_colourreg()                                        *
*                                                                        *
* Sets the image VRAM colour register to x.                              *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/
  
void video_set_colourreg(int x)
  {
  IM_COLOUR_ADDR = x;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_videomask_overlay()                                *
*                                                                        *
* Sets the overlay VRAM video mask register to x.                        *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/
  
void video_set_videomask_overlay(int x)
  {
  IM_MASK_ADDR_O = x;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_colourreg_overlay()                                *
*                                                                        *
* Sets the overlay VRAM colour register to x.                            *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/
  
void video_set_colourreg_overlay(int x)
  {
  IM_COLOUR_ADDR_O = x;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_dac_lut_entry()                                    *
*                                                                        *
* This function initialise the entry 'e' in the IMAGE plane output LUT   *
* to the specified red, green and blue values.                           *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_dac_lut_entry(int e, int r, int g, int b)
  {
  DAC_RAMW_ADDR = e;
  DAC_RAM_DATA = r;
  DAC_RAM_DATA = g;
  DAC_RAM_DATA = b;
  }
  

/*************************************************************************
*                                                                        *
* Function: video_set_dac_lut()                                          *
*                                                                        *
* This function sets the entire IMAGE plane LUT from the contents of the *
* arrays r, g and b which must be 256 elements long.                     *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_dac_lut(int *r, int *g, int *b)
  {
  int i;

  DAC_RAMW_ADDR = 0;

  for(i = 0; i < 256; i++)
    {
    DAC_RAM_DATA = *r++;
    DAC_RAM_DATA = *g++;
    DAC_RAM_DATA = *b++;
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_set_dac_lut_overlay_entry()                            *
*                                                                        *
* This function initialise the entry 'e' in the OVERLAY plane output LUT *
* to the specified red, green and blue values.                           *
*                                                                        *
* e may be in the range 1-15. A value of o is reserved. r, g and b must  *
* lie between 0 and 255 inclusive.                                       *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_dac_lut_overlay_entry(int e, int r, int g, int b)
  {
  DAC_OVW_ADDR = e;
  DAC_OV_DATA = r;
  DAC_OV_DATA = g;
  DAC_OV_DATA = b;
  }
  

/*************************************************************************
*                                                                        *
* Function: video_set_dac_lut_overlay()                                  *
*                                                                        *
* This function sets the entire OVERLAY plane LUT from the contents of   *
* the arrays r, g and b which must be 16 elements long.                  *
*                                                                        *
* Only the top 15 elements of the array are used the 0th element is      *
* reserved for future expansion.                                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_dac_lut_overlay(int *r, int *g, int *b)
  {
  int i;

  DAC_OVW_ADDR = 1;

  for(i = 1; i < 16; i++)
    {
    DAC_OV_DATA = *++r;
    DAC_OV_DATA = *++g;
    DAC_OV_DATA = *++b;
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_check_lock()                                           *
*                                                                        *
* This function checks to see if the PLL has locked correctly. On        *
* success it returns a non zero value. On failure it returns zero.       *
*                                                                        *
* Returns: integer                                                       *
*                                                                        *
**************************************************************************/

int video_check_lock(void)
  {
  PIX_ADDR = 7;
  return ((PIX_DATA & 0x20) == 0);
  }


/*************************************************************************
*                                                                        *
* Function: video_wait_lock()                                            *
*                                                                        *
* This function waits for the PLL to lock. If lock isn't achieved then   *
* the function toggles the int/ext sync bit to try to jolt it into lock. *
* This process continues until lock is achieved.                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_wait_lock(void)
  {
  volatile int i;

  while(!video_check_lock())
    {
    video_control_reg ^= 0x01;         /* Always update the soft video_ */
    VID_CNTRL_REG = video_control_reg; /* control_reg before writing it */
    for(i = 0; i < 0x10000; i++);      /* to the hard one.              */

    video_control_reg ^= 0x01;
    VID_CNTRL_REG = video_control_reg;

    for(i = 0; i < 0x10000; i++);

    for(i = 0; i < 0x100000; i++)
      if (video_check_lock())
        break;
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_int_line()                                       *
*                                                                        *
* This function is used to clear the line events. It is used by some     *
* other functions in this library.                                       *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_int_line(void)
  {
  asm(" PUSH   DP");              /* Save DP for large model compatab'   */
  asm(" LDP    _iackloc_e");      /* Set DP to the page for the even IACK*/
  asm(" LDI    @_iackloc_e,AR2"); /* Set AR2 to the address of the IACK  */
  asm(" IACK   *AR2");            /* Do the IACK to clear the interrupt  */
  asm(" POP    DP");              /* Restore the DP register             */
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_int_field()                                      *
*                                                                        *
* This function is used to clear the field events. It is used by some    *
* other functions in this library.                                       *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_int_field(void)
  {
  asm(" PUSH   DP");              /* Save DP for large model compatab'   */
  asm(" LDP    _iackloc_o");      /* Set DP to the page for the odd IACK */
  asm(" LDI    @_iackloc_o,AR2"); /* Set AR2 to the address of the IACK  */
  asm(" IACK   *AR2");            /* Do the IACK to clear the interrupt  */
  asm(" POP    DP");              /* Restore the DP register             */
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_ints()                                           *
*                                                                        *
* This function is used to clear the field and line events.              *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_ints(void)
  {
  asm(" PUSH   DP");              /* Save DP for large model compatab'   */
  asm(" LDP    _iackloc_e");      /* Set DP to the page for the even IACK*/
  asm(" LDI    @_iackloc_e,AR2"); /* Set AR2 to the address of the IACK  */
  asm(" IACK   *AR2");            /* Do the IACK to clear the line int   */
  asm(" LDP    _iackloc_o");      /* Set DP to the page for the odd IACK */
  asm(" LDI    @_iackloc_o,AR2"); /* Aet AR2 to the address of the IACK  */
  asm(" IACK   *AR2");            /* Do the IACK to clear the field int  */
  asm(" POP    DP");              /* Restore the DP register             */
  }


/*************************************************************************
*                                                                        *
* Function: video_wait_line_event()                                      *
*                                                                        *
* This function waits for the the end of a line.                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_wait_line_event(void)
  {
#ifdef ILIB
  video_control_reg |= 0x40;         /* Enable line events          */
  VID_CNTRL_REG = video_control_reg;
#endif
  while(VID_CNTRL_REG & 0x100);
  }

  
/*************************************************************************
*                                                                        *
* Function: video_wait_field_event()                                     *
*                                                                        *
* This function waits for the the end of a field.                        *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_wait_field_event(void)
  {
#ifdef ILIB
  video_control_reg |= 0x20;         /* Enable field events         */
  VID_CNTRL_REG = video_control_reg;
#endif
  while(VID_CNTRL_REG & 0x80);
  }


/*************************************************************************
*                                                                        *
* Function: video_get_field()                                            *
*                                                                        *
* This function returns the current field that is being applied to the   *
* module. It returns either 0 or 1. The meaning of these values is       *
* dependant on the set-up of the module and reference should be made to  *
* the module manual for more information.                                *
*                                                                        *
* Returns: int                                                           *
*                                                                        *
**************************************************************************/

int video_get_field(void)
  {
  return (video_control_reg >> 9) & 0x0001;
  }


#ifndef COLOUR

/*************************************************************************
*                                                                        *
* Function: video_sync_on_component()                                    *
*                                                                        *
* This function enables sync onformation to be added onto either the     *
* red, green or blue video output and should be called with the          *
* predefines RED_SYNC, GREEN_SYNC or BLUE_SYNC. These may be logically   *
* combined with the | operator to effect multiple outputs or the         *
* function may be called multiple times.                                 *
*                                                                        *
* Adding sync to a component that already has it has no effect.          *
*                                                                        *
* This function is only available on the mono module MDC40IM.            *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_sync_on_component(int comp)
  {
  DAC_CMND = DAC_CMND | comp;
  }


/*************************************************************************
*                                                                        *
* Function: video_sync_off_component()                                   *
*                                                                        *
* This function enables sync information to be removed from either the   *
* red, green or blue video output and should be called with the          *
* predefines RED_SYNC, GREEN_SYNC or BLUE_SYNC. These may be logically   *
* combined with the | operator to effect multiple outputs or the         *
* function may be called multiple times.                                 *
*                                                                        *
* Removing sync from a component that doesn't have it has no effect.     *
*                                                                        *
* This function is only available on the mono module MDC40IM.            *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_sync_off_component(int comp)
  {
  DAC_CMND = DAC_CMND & (~comp);
  }

#endif
  
   
/*************************************************************************
*                                                                        *
* Function: video_pass_through()                                         *
*                                                                        *
* Select video pass through mode. ie the video output echos the camera   *
* input but the data isn't stored in the VRAM.                           *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_pass_through(void)
  {
#ifdef ILIB
  video_control_reg |= 0x40;         /* Enable line events          */
  VID_CNTRL_REG = video_control_reg;
#endif
  video_clear_int_line();            /* Clear old event             */
  while(VID_CNTRL_REG & 0x100);      /* Wait for a line event       */
  video_control_reg |= 0x0a;         /* Update the soft reg         */
  VID_CNTRL_REG = video_control_reg; /* Change mode to pass through */
                                     /* by updating the hard reg    */
  }

  
/*************************************************************************
*                                                                        *
* Function: video_capture_cont()                                         *
*                                                                        *
* Select for continuous capture. The video output echos the camera input *
* and the data is written to the VRAM.                                   *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_capture_cont(void)
  {
#ifdef ILIB
  video_control_reg |= 0x40;         /* Enable line events          */
  VID_CNTRL_REG = video_control_reg;
#endif
  video_clear_int_line();            /* Clear old event             */
  while(VID_CNTRL_REG & 0x100);      /* Wait for a line event       */
  video_control_reg &= 0xfffffff5;   /* Update the soft reg         */
  video_control_reg |= 0x02;
  VID_CNTRL_REG = video_control_reg; /* Change mode to capture cont */
                                     /* by updating the hard reg    */
  }


/*************************************************************************
*                                                                        *
* Function: video_display()                                              *
*                                                                        *
* Select to display the contents of VRAM with the capture circuitry      *
* disabled.                                                              *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_display(void)
  {
#ifdef ILIB
  video_control_reg |= 0x40;         /* Enable line events           */
  VID_CNTRL_REG = video_control_reg;
#endif
  video_clear_int_line();            /* Clear old event              */
  while(VID_CNTRL_REG & 0x100);      /* Wait for a line event        */
  video_control_reg &= 0xfffffff5;   /* Update the soft reg          */
  VID_CNTRL_REG = video_control_reg; /* Change mode to display by    */
                                     /* updating the hard reg        */
  }


/*************************************************************************
*                                                                        *
* Function: video_disable()                                              *
*                                                                        *
* Select to disable display from and capture to the VRAM. This is        *
* essential before the PEROM is accessed.                                *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_disable(void)
  {
#ifdef ILIB
  video_control_reg |= 0x40;         /* Enable line events           */
  VID_CNTRL_REG = video_control_reg;
#endif
  video_clear_int_line();            /* Clear old event              */
  while(VID_CNTRL_REG & 0x100);      /* Wait for a line event        */
  video_control_reg &= 0xfffffff5;   /* Update the soft reg          */
  video_control_reg |= 0x8;
  VID_CNTRL_REG = video_control_reg; /* Change mode to disable by    */
                                     /* updating the hard reg        */
  }


/*************************************************************************
*                                                                        *
* Function: video_grab()                                                 *
*                                                                        *
* Start the module capturing continuously. Once a frame has been         *
* obtained then the module is switched to display mode.                  *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_grab(void)
  {
  volatile int i;

  video_capture_cont();

#ifdef ILIB
  video_control_reg |= 0x20;         /* Enable field events          */
  VID_CNTRL_REG = video_control_reg;
#endif

  video_clear_int_field();           /* Clear old event              */
  while(VID_CNTRL_REG & 0x80);       /* Make the next field capture  */
  video_clear_int_field();           /* Clear old event              */
  while(VID_CNTRL_REG & 0x80);       /* Wait for 1st bad field       */
  video_clear_int_field();           /* Clear old event              */
  while(VID_CNTRL_REG & 0x80);       /* Wait for 1st complete field  */
  video_clear_int_field();           /* Clear old event              */
  while(VID_CNTRL_REG & 0x80);       /* Wait for 2nd complete field  */

  video_display();                   /* Stop the capturing           */
  }


/*************************************************************************
*                                                                        *
* Function: video_set_size()                                             *
*                                                                        *
* This function sets the x and y screen margins and the number of words  *
* of memory per video line. This function should be used before any of   *
* the graphic or text functions. The defaults are sensible for the       *
* standard PAL and NTSC setups however.                                  *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_size(int width, int height)
  {
  if ((width <= 0) || (height <= 0))    /* Check the bounds and return   */
    return;                             /* if no good.                   */
  g_xm = width - 1;                     /* Set the screen size           */
  g_ym = height - 1;
  g_x1 = 0;                             /* Set the window size to full   */
  g_y1 = 0;                             /* screen.                       */
  g_x2 = g_xm;
  g_y2 = g_ym;
  }
  

/*************************************************************************
*                                                                        *
* Function: video_get_size()                                             *
*                                                                        *
* This function gets the current screen width and height.                *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_get_size(int *width, int *height)
  {
  *width = g_xm + 1;;
  *height = g_ym + 1;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_window()                                           *
*                                                                        *
* This function sets the window for graphics and text operations. Most   *
* of these functions will then clip to the screen bounds.                *
* The function takes the top left hand corner of the window (x, y) and   *
* the width and height of the window (w, h)                              *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_window(int x, int y, int w, int h)
  {
  if (x < 0)
    x = g_x1;
  if (x > g_xm)
    return;
  if (y < 0)
    y = g_y1;
  if (y > g_ym)
    return;

  if ((w <= 0) || (h <= 0))
    return;

  g_x1 = x;
  g_y1 = y;
  g_x2 = (x + w) - 1;
  g_y2 = (y + h) - 1;

  if (g_x2 > g_xm)
    g_x2 = g_xm;
  if (g_y2 > g_ym)
    g_y2 = g_ym;

  g_xc = g_x1;
  g_yc = g_y1;
  }


/*************************************************************************
*                                                                        *
* Function: video_get_window()                                           *
*                                                                        *
* This function returns the current window position and size via the     *
* pointers x, y, w, h.                                                   *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_get_window(int *x, int *y, int *w, int *h)
  {
  *x = g_x1;
  *y = g_y1;
  *w = (g_x2 - g_x1) + 1;
  *h = (g_y2 - g_y1) + 1;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_screen_base()                                      *
*                                                                        *
* This function sets the screen base address. It should always be used   *
* before calling any of the text or graphics functions (the default is   *
* sensible and points to the OVERLAY). Usually it will just be set to    *
* the #defines (see reg.h) OVERLAY or IMAGE depending which page the     *
* users wants text and graphics to appear on.                            *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/
  
void video_set_screen_base(int addr)
  {
  if ((addr >= IMAGE) && (addr < OVERLAY))
    {
#ifdef COLOUR
    g_pmask = 0xffffff;
#else
    g_pmask = 0xff;
#endif
    }
  else
    {
    g_pmask = 0xff;
    }

  g_scrn_addr = (int *) addr;
  }


/*************************************************************************
*                                                                        *
* Function: video_get_screen_base()                                      *
*                                                                        *
* This function returns the current screen base address.                 *
*                                                                        *
* Returns: int                                                           *
*                                                                        *
**************************************************************************/
  
int video_get_screen_base(void)
  {
  return (int) g_scrn_addr;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_colour()                                           *
*                                                                        *
* This function sets the default colour to be used for graphic           *
* functions. The default is 0. For OVERLAY only the least significant 4  *
* bits are of any relevance. For IMAGE then upto 24 bits are significant *
* depending on the module setup. Note it is 24 bits for the PAL and NTSC *
* setups. 8 bits for MONO.                                               * 
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_colour(int c)
  {
  g_col = c;
  }

  
/*************************************************************************
*                                                                        *
* Function: video_get_colour()                                           *
*                                                                        *
* This function returns the current graphics colour.                     *
*                                                                        *
* Returns: int                                                           *
*                                                                        *
**************************************************************************/

int video_get_colour(void)
  {
  return g_col;
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_screen()                                         *
*                                                                        *
* This function clears the current screen plane (IMAGE or OVERLAY) to    *
* the current colour.                                                    *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_screen(void)
  {
  int i;
  int *p;

  p = g_scrn_addr;
  for(i = (LINE_LEN * (g_ym + 1)); i > 0; i--)
    *p++ = g_col;
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_window()                                         *
*                                                                        *
* This function clears the current windowed plane (IMAGE or OVERLAY) to  *
* the current colour.                                                    *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_window(void)
  {
  int i, j;
  int *p, *np;

  p = g_scrn_addr + g_x1 + (g_y1 * g_line_inc);
  for(j = g_y1; j <= g_y2; j++)
    {
    np = p;
    p += g_line_inc;
    for(i = g_x1; i <= g_x2; i++)
      *np++ = g_col;
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_overlay()                                        *
*                                                                        *
* This function sets the OVERLAY plane according to the VRAM mask and    *
* colour registers. This uses the flash write mode and is very fast.     *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_overlay(void)
  {
  int *p;
  int i;

  p = (int *) OVERLAY + FLASH_WR_OFFSET;
  for(i = 0; i <= g_ym; i++)
    {
    *p = 0;
    p += LINE_LEN;
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_clear_image()                                          *
*                                                                        *
* This function sets the IMAGE plane according to the VRAM mask and      *
* colour registers. This uses the flash write mode and is very fast.     *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_clear_image(void)
  {
  int *p;
  int i;

  p = (int *) IMAGE + FLASH_WR_OFFSET;
  for(i = 0; i <= g_ym; i++)
    {
    *p = 0;
    p += LINE_LEN;
    }
  }


/*************************************************************************
*                                                                        *
* Define: set_pixel()                                                    *
*                                                                        *
* Internal #def for plotting a pixel.                                    *
*                                                                        *
**************************************************************************/

#define set_pixel(x, y) {\
  if (((x) >= g_x1) && ((x) <= g_x2) &&\
      ((y) >= g_y1) && ((y) <= g_y2))\
    *(g_scrn_addr + (x) + ((y) * g_line_inc)) = g_col;\
  }\


/*************************************************************************
*                                                                        *
* Function: video_set_pixel()                                            *
*                                                                        *
* This function plots a point in the current colour at the specified X,Y *
* coordinate. Note that X goes from left to right and Y goes from top to *
* bottom of the display.                                                 *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_pixel(int x, int y)
  {
  set_pixel(x, y);
  }


/*************************************************************************
*                                                                        *
* Function: video_get_pixel()                                            *
*                                                                        *
* This function returns the colour of a pixel located at x, y on the     *
* screen. Points outside of the screen bounds return -1.                 *
*                                                                        *
* Returns: int                                                           *
*                                                                        *
**************************************************************************/

int video_get_pixel(int x, int y)
  {
  int t;

  if ((x >= 0) && (x <= g_xm) &&
      (y >= 0) && (y <= g_ym))
    {
    t = *(g_scrn_addr + x + (y * g_line_inc));
    t &= g_pmask;
    return t;
    }
  else
    return -1;
  }


/*************************************************************************
*                                                                        *
* Function: code_point()                                                 *
*                                                                        *
* This static function is used in the line cliping algorithm.            *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

static int code_point(double x, double y)
  {
  int c = 0;

  if (x < g_x1)
    c |= 0x1;
  else if (x > g_x2)
    c |= 0x2;
  if (y < g_y1)
    c |= 0x4;
  else if (y > g_y2)
    c |= 0x8;
  return c;
  }


/*************************************************************************
*                                                                        *
* Function: clip_line()                                                  *
*                                                                        *
* This static function clips a line to the window bounds.                *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

static int clip_line(int *x1, int *y1, int *x2, int *y2)
  {
  int c, c1, c2;
  double X1, X2, Y1, Y2, x, y;

  X1 = *x1;
  Y1 = *y1;
  X2 = *x2;
  Y2 = *y2;
  c1 = code_point(X1, Y1);
  c2 = code_point(X2, Y2);
  while((c1 != 0) || (c2 != 0))
    {
    if (c1 & c2)
      return 1;
    c = (c1 != 0) ? c1 : c2;
    if (c & 0x1)
      {
      y = Y1 + (((Y2 - Y1) * (g_x1 - X1)) / (X2 - X1));
      if (y >= 0.0)
        y = (int) (y + 0.5);
      else
        y = (int) (y - 0.5);
      x = g_x1;
      }
    else if (c & 0x2)
      {
      y = Y1 + (((Y2 - Y1) * (g_x2 - X1)) / (X2 - X1));
      if (y >= 0.0)
        y = (int) (y + 0.5);
      else
        y = (int) (y - 0.5);
      x = g_x2;
      }
    else if (c & 0x4)
      {
      x = X1 + (((X2 - X1) * (g_y1 - Y1)) / (Y2 - Y1));
      if (x >= 0.0)
        x = (int) (x + 0.5);
      else
        x = (int) (x - 0.5);
      y = g_y1;
      }
    else if (c & 0x8)
      {
      x = X1 + (((X2 - X1) * (g_y2 - Y1)) / (Y2 - Y1));
      if (x >= 0.0)
        x = (int) (x + 0.5);
      else
        x = (int) (x - 0.5);
      y = g_y2;
      }
    if (c == c1)
      {
      X1 = x;
      Y1 = y;
      c1 = code_point(x, y);
      }
    else
      {
      X2 = x;
      Y2 = y;
      c2 = code_point(x, y);
      }
    }

  *x1 = X1;
  *y1 = Y1;
  *x2 = X2;
  *y2 = Y2;

  return 0;
  }


/*************************************************************************
*                                                                        *
* Function: video_plot_line()                                            *
*                                                                        *
* This function plots a line in the current colour from x1,y1 to x2,y2.  *
* It makes use of Bressenhams line drawing algorithm and the line is     *
* clipped to the window bounds.                                          *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_plot_line(int x1, int y1, int x2, int y2)
  {
  int i, dxabs, dyabs, cnt;
  int *scrn_ptr;

  if (clip_line(&x1, &y1, &x2, &y2))
    return;

  dxabs = abs(x2 - x1);

  scrn_ptr = g_scrn_addr + x1 + (y1 * g_line_inc);

  if (y2 > y1)
    {
    dyabs = y2 - y1;
    if (dxabs >= dyabs)
      {
      cnt = dxabs / 2;
      if (x2 > x1)
        {
        for(; x1 <= x2; x1++)
          {
          *scrn_ptr++ = g_col;
          cnt += dyabs;
          if (cnt > dxabs)
            {
            cnt -= dxabs;
            scrn_ptr += g_line_inc;
            }
          }
        }
      else
        {
        for(; x2 <= x1; x2++)
          {
          *scrn_ptr-- = g_col;
          cnt += dyabs;
          if (cnt > dxabs)
            {
            cnt -= dxabs;
            scrn_ptr += g_line_inc;
            }
          }
        }
      }
    else
      {
      cnt = dyabs / 2;
      if (x2 > x1)
        {
        for(; y1 <= y2; y1++)
          {
          *scrn_ptr = g_col;
          scrn_ptr += g_line_inc;
          cnt += dxabs;
          if (cnt > dyabs)
            {
            cnt -= dyabs;
            scrn_ptr++;
            }
          }
        }
      else
        {
        for(; y1 <= y2; y1++)
          {
          *scrn_ptr = g_col;
          scrn_ptr += g_line_inc;
          cnt += dxabs;
          if (cnt > dyabs)
            {
            cnt -= dyabs;
            scrn_ptr--;
            }
          }
        }
      }
    }
  else
    {
    dyabs = y1 - y2;
    if (dxabs >= dyabs)
      {
      cnt = dxabs / 2;
      if (x2 > x1)
        {
        for(; x1 <= x2; x1++)
          {
          *scrn_ptr++ = g_col;
          cnt += dyabs;
          if (cnt > dxabs)
            {
            cnt -= dxabs;
            scrn_ptr -= g_line_inc;
            }
          }
        }
      else
        {
        for(; x2 <= x1; x2++)
          {
          *scrn_ptr-- = g_col;
          cnt += dyabs;
          if (cnt > dxabs)
            {
            cnt -= dxabs;
            scrn_ptr -= g_line_inc;
            }
          }
        }
      }
    else
      {
      cnt = dyabs / 2;
      if (x2 > x1)
        {
        for(; y2 <= y1; y1--)
          {
          *scrn_ptr = g_col;
          scrn_ptr -= g_line_inc;
          cnt += dxabs;
          if (cnt > dyabs)
            {
            cnt -= dyabs;
            scrn_ptr++;
            }
          }
        }
      else
        {
        for(; y2 <= y1; y1--)
          {
          *scrn_ptr = g_col;
          scrn_ptr -= g_line_inc;
          cnt += dxabs;
          if (cnt > dyabs)
            {
            cnt -= dyabs;
            scrn_ptr--;
            }
          }
        }
      }
    }
  }


/*************************************************************************
*                                                                        *
* Function: hline()                                                      *
*                                                                        *
* Internal function for drawing fast horizontal lines. It clips to the   *
* window bounds.                                                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void hline(int x1, int x2, int y)
  {
  int *sptr;

  if (x1 < g_x1)
    x1 = g_x1;
  if (x1 > g_x2)
    return;
  if (x2 > g_x2)
    x2 = g_x2;
  if (x2 < g_x1)
    return;
  if ((y < g_y1) || (y > g_y2))
    return;

  sptr = g_scrn_addr + x1 + (y * g_line_inc);
  for(; x1 <= x2; x1++)
    *sptr++ = g_col;
  }

  
/*************************************************************************
*                                                                        *
* Function: video_circle_solid()                                         *
*                                                                        *
* This function draws a solid circle centered at x, y with a radius of r.*
* It clips to the window bound.                                          *
*                                                                        *
* I call this algorithm Whitton's algorithm as I worked it out from      *
* first principles and have never seen it published anywhere. It is very *
* very fast especially on processors with a hardware multiply to         *
* calculate r*r. This is only done once however so is not a big issue.   *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_circle_solid(int x, int y, int r)
  {
  int r2 = r * r;
  int x2 = r2;
  int y2 = 0;
  int ln;

  for(ln = 0; ln <= r; ln++)
    {
    hline(x - r, x + r, y + ln);
    hline(x - r, x + r, y - ln);
    y2 += (ln << 1) + 1;
    if (x2 > (r2 - y2))
      {
      hline(x - ln, x + ln, y + r);
      hline(x - ln, x + ln, y - r);
      x2 -= (r << 1) + 1;
      r--;
      }
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_circle()                                               *
*                                                                        *
* This function draws a circle centered at x, y with a radius of r.      *
* It clips to the window bound.                                          *
*                                                                        *
* I call this algorithm Whitton's algorithm as I worked it out from      *
* first principles and have never seen it published anywhere. It is very *
* very fast especially on processors with a hardware multiply to         *
* calculate r*r. This is only done once however so is not a big issue.   *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_circle(int x, int y, int r)
  {
  int r2 = r * r;
  int x2 = r2;
  int y2 = 0;
  int ln;

  for(ln = 0; ln <= r; ln++)
    {
    set_pixel(x - r, y + ln);
    set_pixel(x + r, y + ln);
    set_pixel(x - r, y - ln);
    set_pixel(x + r, y - ln);
    set_pixel(x - ln, y + r);
    set_pixel(x + ln, y + r);
    set_pixel(x - ln, y - r);
    set_pixel(x + ln, y - r);
    y2 += (ln << 1) + 1;
    if (x2 > (r2 - y2))
      {
      x2 -= (r << 1) + 1;
      r--;
      }
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_rect_solid()                                           *
*                                                                        *
* Plots a rectangle on the screen x1, y1 and x2, y2 are diagonally       *
* opposite corners.                                                      *
* This function clips to the window bounds.                              *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_rect_solid(int x1, int y1, int x2, int y2)
  {
  int t;

  if (y1 > y2)
    {
    t = y1;
    y1 = y2;
    y2 = t;
    }
  if (x1 > x2)
    {
    t = x1;
    x1 = x2;
    x2 = t;
    }
  if (y1 < g_y1)
    y1 = g_y1;
  if (y2 > g_y2)
    y2 = g_y2;
  if (x1 < g_x1)
    x1 = g_x1;
  if (x2 > g_x2)
    x2 = g_x2;
  for(; y1 <= y2; y1++)
    hline(x1, x2, y1);
  }


/*************************************************************************
*                                                                        *
* Function: video_rect()                                                 *
*                                                                        *
* This function draws an outline rectangle to the screen.                *
* It clips to the window bounds.                                         *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_rect(int x1, int y1, int x2, int y2)
  {
  video_plot_line(x1, y1, x2, y1);
  video_plot_line(x2, y1, x2, y2);
  video_plot_line(x2, y2, x1, y2);
  video_plot_line(x1, y2, x1, y1);
  }

   
/*************************************************************************
*                                                                        *
* Function: video_fill()                                                 *
*                                                                        *
* This function flood fills from x, y in the currently selected colour.  *
*                                                                        *
* Returns: An integer indicating the number of pixels that have been     *
* filled. -1 if the seed coordinates are outside the window or the       *
* function runs out of heap space.                                       *
*                                                                        *
**************************************************************************/

int video_fill(int x, int y)
  {
  stack_type *stack;
  stack_type *stack_tmp;
  int stack_ptr;
  int stack_size;

  int *sptr;
  int *tptr;
  int i, r;
  int tot;
  int fcol;
  int ir;

  if ((x < g_x1) || (x > g_x2) ||
      (y < g_y1) || (y > g_y2))
    return -1;

  stack = (stack_type *) malloc(STACK_SIZE * sizeof(stack_type));
  if (stack == NULL)
    return -1;

  stack_size = STACK_SIZE;
  stack_ptr  = 0;

  sptr = g_scrn_addr + x + (y * g_line_inc);
  fcol = *sptr & g_pmask;
  for(x--; x >= g_x1; x--)
    {
    sptr--;
    if ((*sptr & g_pmask) != fcol)
      break;
    }

  stack[stack_ptr].x = x + 1;
  stack[stack_ptr].y = y;

  tot = 0;

  while(stack_ptr >= 0)
    {
    x = stack[stack_ptr].x;
    y = stack[stack_ptr].y;

    sptr = g_scrn_addr + x + (y * g_line_inc);
    tptr = sptr;

    for(r = x; r <= g_x2; r++)
      {
      if ((*sptr & g_pmask) != fcol)
        break;
      *sptr++ = g_col;
      }
    r--;

    stack_ptr--;

    tot += (r - x) + 1;

    if (y < g_y2)
      {
      sptr = tptr + g_line_inc;
      ir = 0;
      for(i = x; i >= g_x1; i--)
        {
        if ((*sptr & g_pmask) != fcol)
          break;
        sptr--;
        }
      sptr++;
      for(i++; i <= r; i++)
        {
        if (ir)
          ir = ((*sptr & g_pmask) == fcol);
        else
          if ((*sptr & g_pmask) == fcol)
            {
            ir = 1;
            stack_ptr++;
            if (stack_ptr == stack_size)
              {
              stack_tmp = (stack_type *) realloc(stack, (stack_size + STACK_SIZE) * sizeof(stack_type));
              if (stack_tmp == NULL)
                {
                free(stack);
                return -1;
                }
              stack = stack_tmp;
              stack_size += STACK_SIZE;
              }
            stack[stack_ptr].x = i;
            stack[stack_ptr].y = y + 1;
            }
        sptr++;
        }
      }

    if (y > g_y1)
      {
      sptr = tptr - g_line_inc;
      ir = 0;
      for(i = x; i >= g_x1; i--)
        {
        if ((*sptr & g_pmask) != fcol)
          break;
        sptr--;
        }
      sptr++;
      for(i++; i <= r; i++)
        {
        if (ir)
          ir = ((*sptr & g_pmask) == fcol);
        else
          if ((*sptr & g_pmask) == fcol)
            {
            ir = 1;
            stack_ptr++;
            if (stack_ptr == stack_size)
              {
              stack_tmp = (stack_type *) realloc(stack, (stack_size + STACK_SIZE) * sizeof(stack_type));
              if (stack_tmp == NULL)
                {
                free(stack);
                return -1;
                }
              stack = stack_tmp;
              stack_size += STACK_SIZE;
              }
            stack[stack_ptr].x = i;
            stack[stack_ptr].y = y - 1;
            }
        sptr++;
        }
      }
    }
  free(stack);

  return tot;
  }
  

/*************************************************************************
*                                                                        *
* Function: video_get_sprite_size()                                      *
*                                                                        *
* This function calculates the storage requirement for a sprite. It      *
* returns (width x height) + 2.                                          *
*                                                                        *
* Returns: An integer that may be passed to malloc to get an appropriate *
* ammount of storage for a sprite of the specified size.                 *
*                                                                        *
**************************************************************************/

int video_get_sprite_size(int w, int h)
  {
  return ((w * h) + 2) * sizeof(int);
  }


/*************************************************************************
*                                                                        *
* Function: video_get_sprite()                                           *
*                                                                        *
* This function makes a sprite from a region of the IMAGE or OVERLAY     *
* plane. It takes an x,y pair that represent the top left hand corner of *
* the region of interest and a w,h pair that represent the width and     *
* height of the region.                                                  *
*                                                                        *
* The sprite is stored in im. If im is NULL the function returns without *
* effect. The first 2 entries in im are the width and height. This is    *
* followed by the image data (row by row). The top of each integer isn't *
* masked so take care if manually examining the data.                    *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_get_sprite(int x, int y, int w, int h, int *im)
  {
  int *sptr;
  int i, j;

  if ((w <= 0) || (h <= 0))
    return;
  
  if (im == NULL)
    return;

  *im++ = w;
  *im++ = h;

  sptr = g_scrn_addr + x + (y * g_line_inc);

  for(j = 0; j < h; j++)
    {
    for(i = 0; i < w; i++)
      *im++ = *sptr++;
    sptr += g_line_inc - w;
    }
  }


/*************************************************************************
*                                                                        *
* Function: video_put_sprite()                                           *
*                                                                        *
* This function plots the sprite stored in im to the IMAGE or OVERLAY    *
* plane. It takes an x,y pair that represent the top left hand corner of *
* the region of interest. The width and height information are extracted *
* from the sprite data stored in im.                                     *
*                                                                        *
* The sprite is stored in im. If im is NULL the function returns without *
* effect. The first 2 entried in im are the width and height. This is    *
* followed by the image data (row by row).                               *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_put_sprite(int x, int y, int *im)
  {
  int *sptr;
  int i, j;
  int *w, *h;

  if (im == NULL)
    return;

  w = im++;
  h = im++;

  sptr = g_scrn_addr + x + (y * g_line_inc);

  for(j = 0; j < (*h); j++)
    {
    for(i = 0; i < (*w); i++)
      *sptr++ = *im++;
    sptr += g_line_inc - *w;
    }
  }

  
/*************************************************************************
*                                                                        *
* Function: video_home()                                                 *
*                                                                        *
* This function homes the text cursor to the top left hand corner of the *
* current window.                                                        *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_home(void)
  {
  g_xc = g_x1;
  g_yc = g_y1;
  }

  
/*************************************************************************
*                                                                        *
* Function: video_set_text_colour()                                      *
*                                                                        *
* This function sets the default colour to be used for text              *
* functions. The default is 0. For OVERLAY only the least significant 4  *
* bits are of any relevance. For IMAGE then upto 24 bits are significant *
* depending on the module setup. Note it is 24 bits for the PAL and NTSC *
* setups. 8 bits for MONO.                                               * 
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_text_colour(int c)
  {
  g_tcol = c;
  }


/*************************************************************************
*                                                                        *
* Function: video_get_text_colour()                                      *
*                                                                        *
* This function returns the current text colour.                         *
*                                                                        *
* Returns: int                                                           *
*                                                                        *
**************************************************************************/

int video_get_text_colour(void)
  {
  return g_tcol;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_text_back_colour()                                 *
*                                                                        *
* This function sets the default background colour to be used for text   *
* functions. The default is 0. For OVERLAY only the least significant 4  *
* bits are of any relevance. For IMAGE then upto 24 bits are significant *
* depending on the module setup. Note it is 24 bits for the PAL and NTSC *
* setups. 8 bits for MONO.                                               *
*                                                                        *
* Negative values make the background transparent.                       *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_text_back_colour(int c)
  {
  g_tbcol = c;
  }


/*************************************************************************
*                                                                        *
* Function: video_get_text_back_colour()                                 *
*                                                                        *
* This function returns the current text background colour.              *
*                                                                        *
* Returns: int                                                           *
*                                                                        *
**************************************************************************/

int video_get_text_back_colour(void)
  {
  return g_tbcol;
  }


/*************************************************************************
*                                                                        *
* Function: video_gotoxy()                                               *
*                                                                        *
* This function moves the current text cursor to the specified X,Y       *
* position.                                                              *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_gotoxy(int x, int y)
  {
  if (x > ((g_xm + 1) - (8 * g_xs)))
    x = 0;
  else if (x < 0)
    x = 0;
  
  if (y > ((g_ym + 1) - (8 * g_ys)))
    y = 0;
  else if (y < 0)
    y = 0;

  g_xc = x;
  g_yc = y;
  }


/*************************************************************************
*                                                                        *
* Function: video_getxy()                                                *
*                                                                        *
* This function returns the current text cursor position x, y.           *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_getxy(int *x, int *y)
  {
  *x = g_xc;
  *y = g_yc;
  }


/*************************************************************************
*                                                                        *
* Function: video_set_text_scale()                                       *
*                                                                        *
* This function sets the X and Y text scale factors.                     *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_set_text_scale(int xs, int ys)
  {
  if (xs < 1)
    xs = 1;
  if (xs > 16)
    xs = 16;
  if (ys < 1)
    ys = 1;
  if (ys > 16)
    ys = 16;
  g_xs = xs;
  g_ys = ys;
  }


/*************************************************************************
*                                                                        *
* Function: video_get_text_scale()                                       *
*                                                                        *
* This function gets the X and Y text scale factors.                     *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_get_text_scale(int *xs, int *ys)
  {
  *xs = g_xs;
  *ys = g_ys;
  }

   
/*************************************************************************
*                                                                        *
* Function: video_outchar()                                              *
*                                                                        *
* This function writes the specified character at the current text       *
* cursor position. The text cursor is then incremented by 8*g_xs in the  *
* X direction. If X exceeds the screen bound then it is set to g_x1 and  *
* Y position is incremented by 8*g_ys. If the Y position goes off the    *
* screen then it is reset to g_y1. The text colour and plane are in the  *
* current selection.                                                     *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_outchar(char ch)
  {
  int i, j, xs, ys;
  int  *scrn_ptr;
  unsigned char *char_ptr;
  unsigned char c;

  char_ptr = charset + (8 * ch);
  scrn_ptr = g_scrn_addr + g_xc + (g_yc * g_line_inc);
  for(j = 0; j < 8; j++)
    {
    for(ys = 0; ys < g_ys; ys++)
      {
      c = *char_ptr;
      for(i = 0; i < 8; i++)
        {
        if (c & 0x80)
          {
          for(xs = 0; xs < g_xs; xs++)
            {
            *scrn_ptr = g_tcol;
            scrn_ptr++;
            }
          }
        else if (g_tbcol >= 0)
          for(xs = 0; xs < g_xs; xs++)
            {
            *scrn_ptr = g_tbcol;
            scrn_ptr++;
            }
        else
          scrn_ptr += g_xs;

        c <<= 1;
        }
      scrn_ptr += (g_line_inc - (8 * g_xs));
      }
    char_ptr++;
    }
  g_xc += 8 * g_xs;
  if (g_xc > ((g_x2 + 1) - (8 * g_xs)))
    {
    g_xc = g_x1;
    g_yc += 8 * g_ys;
    if (g_yc > ((g_y2 + 1) - (8 * g_ys)))
      g_yc = g_y1;
    }
  }
  

/*************************************************************************
*                                                                        *
* Function: video_outtext()                                              *
*                                                                        *
* This function outputs a text string at the current text cursor positon.*
* The linefeed character causes a line feed and carriage return to occur.*
* The text colour and plane are according to the current selection.      *
*                                                                        *
* Returns: void                                                          *
*                                                                        *
**************************************************************************/

void video_outtext(char *s)
  {
  while(*s != 0)
    {
    if (*s == 0x0a)
      {                                                
      g_xc = g_x1;
      g_yc += 8 * g_ys;
      if (g_yc > ((g_y2 + 1) - (8 * g_ys)))
        g_yc = g_y1;
      }
    else
      video_outchar(*s);
    s++;
    }
  }

 
/*************************************************************************
*                                                                        *
* The majority of the following code was obtained from the NET. It       *
* consists of a format converter that can be used for parsing printf     *
* arguments. This allows the library to include a video_printf function. *
*                                                                        *
**************************************************************************/

#include <ctype.h>
#include <string.h>
#include <stdarg.h>


static double round[] =
  {
  10, 1, 5e-1, 5e-2, 5e-3, 5e-4, 5e-5, 5e-6, 5e-7, 5e-8, 5e-9, 5e-10,
  5e-11, 5e-12, 5e-13, 5e-14, 5e-15, 5e-16
  };


static int ftoa(double number, char *buffer, int maxwidth, int flag)
  {
  int i, exp, digit, decpos, ndig;

  ndig = maxwidth+1;
  exp = 0;
  if (number < 0.0)
    {
    number = -number;
    *buffer++ = '-';
    }
  if (number > 0.0)
    {
    while(number < round[1])
      {
      number *= round[0];
      --exp;
      }
     while (number >= round[0])
        {
        number /= round[0];
        ++exp;
        }
     }

  if (flag == 2)
    {    /* 'g' format */
    ndig = maxwidth;
    if (exp < -4 || exp > maxwidth)
      flag = 0;    /* switch to 'e' format */
    }
  else if (flag == 1)  /* 'f' format */
    ndig += exp;

  if (ndig >= 0)
    {
    if ((number += round[(ndig > 16 ? 16 : ndig) + 1]) >= round[0])
      {
      number = round[1];
      ++exp;
      if (flag)
      ++ndig;
      }
    }

  if (flag)
    {
    if (exp < 0)
      {
      *buffer++ = '0';
      *buffer++ = '.';
      i = -exp - 1;
      if (ndig <= 0)
        i = maxwidth;
      while (i--)
        *buffer++ = '0';
      decpos = 0;
      }
    else
      decpos = exp+1;
    }
  else
    decpos = 1;

  if (ndig > 0)
    {
    for (i = 0 ; ; ++i)
      {
      if (i < 16)
        {
        digit = (int)number;
        *buffer++ = digit+'0';
        number = (number - digit) * round[0];
        }
      else
        *buffer++ = '0';
      if (--ndig == 0)
        break;
      if (decpos && --decpos == 0)
        *buffer++ = '.';
      }
    }

  if (!flag)
    {
    *buffer++ = 'e';
    if (exp < 0)
      {
      exp = -exp;
      *buffer++ = '-';
      }
    else
      *buffer++ = '+';
    if (exp >= 100)
      {
      *buffer++ = exp/100 + '0';
      exp %= 100;
      }
    *buffer++ = exp/10 + '0';
    *buffer++ = exp%10 + '0';
    }
  *buffer = 0;
  }


static char *_fmtcvt(void *ap, int base, char *cp, int len)
  {
  unsigned long val;
  static char digits[]="0123456789abcdef";

  if (len == sizeof(long))
    val = *(long *)ap;
  else if (base > 0)
    val = *(unsigned *)ap;
  else
    val = *(int *)ap;

  len = 0;
  if (base < 0)
    {
    base = -base;
    if ((long)val < 0)
      {
      val = -val;
      len = 1;
      }
    }

  do
    {
    *--cp = digits[(int)(val%base)];
    } while ((val /= base) != 0);
  if (len)
    *--cp = '-';
  return cp;
  }


static int format(int (*putsub)(char), char *fmt, va_list ap)
  {
  int c;
  int charcount;
  int rj, fillc;
  int maxwidth, width;
  int i, k;
  char *cp;
  char s[200];
  int tmp;

  charcount = 0;
  while(c = *fmt++)
    {
    if (c == '%')
      {
      s[14] = 0;
      rj = 1;
      fillc = ' ';
      maxwidth = 10000;
      if ((c = *fmt++) == '-')
        {
        rj = 0;
        c = *fmt++;
        }
      if (c == '0')
        {
        fillc = '0';
        c = *fmt++;
        }
      if (c == '*')
        {
        width = va_arg(ap, int);
        c = *fmt++;
        }
      else
        for (width = 0 ; isdigit(c) ; c = *fmt++)
          width = width*10 + c - '0';
      if ( c == '.' )
        {
        if ((c = *fmt++) == '*')
          {
          maxwidth = va_arg(ap, int);
          c = *fmt++;
          }
        else
          {
          for (maxwidth = 0 ; isdigit(c) ; c = *fmt++)
            maxwidth = maxwidth*10 + c - '0';
          }
        }
      i = sizeof(int);
      if (c == 'l')
        {
        c = *fmt++;
        i = sizeof(long);
        }
      else if (c == 'h')
        c = *fmt++;

      switch(c)
        {
        case 'o':
          k = 8;
          goto conv;
        case 'u':
          k = 10;
          goto conv;
        case 'x':
          k = 16;
          goto conv;

        case 'd':
          k = -10;
conv:
          tmp = va_arg(ap, int);
          cp = _fmtcvt(&tmp, k, s+14, i);
          break;

        case 's':
          cp = va_arg(ap, char *);
          i = strlen(cp);
          goto gotlen;
        case 'e':
        case 'f':
        case 'g':
          ftoa(va_arg(ap, double), s, maxwidth==10000?6:maxwidth, c-'e');
          i = strlen(cp = s);
          maxwidth = 200;
          goto gotlen;

        case 'c':
          c = va_arg(ap, char);
        default:
          *(cp = s+13) = c;
          break;
        }

      i = (s+14) - cp;
gotlen:
      if ( i > maxwidth )
        i = maxwidth;
      
      if (rj)
        {
        if ((*cp == '-' || *cp == '+') && fillc == '0')
          {
          --width;
          if ((*putsub)(*cp++) == -1)
            return -1;
          }
        for(; width-- > i ; ++charcount)
          if ((*putsub)(fillc) == -1)
            return -1;
        }
      for (k = 0 ; *cp && k < maxwidth ; ++k )
        if ((*putsub)(*cp++) == -1)
          return -1;
      charcount += k;
      
      if (!rj)
        {
        for (; width-- > i ; ++charcount)
          if ((*putsub)(' ') == -1)
            return -1;
        }
      }
    else
      {
      if ((*putsub)(c) == -1)
        return -1;
      ++charcount;
      }
    }
  return charcount;
  }


static int local_putchar(char ch)
  {
  static char str[2] = {0, 0};

  str[0] = ch;
  video_outtext(str);
  return ch;
  }


/*************************************************************************
*                                                                        *
* Function: video_printf()                                               *
*                                                                        *
* This function copies the functionality of the standard 'printf' func   *
* on the display of the imaging module. It is identical to video_outtext *
* but will process standard formating characters and will take a         *
* variable number of arguments.                                          *
*                                                                        *
* Returns: The number of characters printed or -1 on error.              *
*                                                                        *
**************************************************************************/

int video_printf(char *fmt, ...)
  {
  va_list ap;

  va_start(ap, fmt);
  return(format(local_putchar, fmt, ap));
  va_end(ap);
  }


