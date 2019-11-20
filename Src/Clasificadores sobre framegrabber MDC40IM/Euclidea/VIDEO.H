/*************************************************************************
*                                                                        *
* File               : video.h                                           *
* Date first created : 25-7-94                                           *
* Author             : Rob Whitton                                       *
* Version            : 1.00                                              *
*                                                                        *
* Description        :                                                   *
*                                                                        *
* This file contains the type definitions, externs and prototypes        *
* required by the library and the users own application. This file       *
* should be included in the users own file. To change between            *
* colour and mono versions of this file see regs.h.                      *
*                                                                        *
*                                                                        *
* Updates            :                                                   *
*                                                                        *
**************************************************************************/


#ifndef VIDEO_H      /* Make sure this file isn't included twice */

#define VIDEO_H

#include "regs.h"    /* Get the register definitions. */


#define LINE_LEN 512 /* The line length in words */


/*************************************************************************
*                                                                        *
* Declare a type that will hold the video setup information for a        *
* particular mode.                                                       *
*                                                                        *
**************************************************************************/

typedef struct video_setup_struct
  {
  int adc_cmnd;

  int adc_iout0_data;
  int adc_iout1_data;

#ifdef COLOUR
  int adc_iout2_data;
  int adc_iout3_data;
  int adc_iout4_data;
  int adc_iout5_data;
#endif

  int vid_cntrl_reg;

  int sync[22];

  int pcdata[30];

  int dac_cmnd;

  int dac_mask;

  int mask_reg;
  int colour_reg;

  int mask_reg_overlay;
  int colour_reg_overlay;

  int cols;
  int rows;
  } video_setup_type;


/*************************************************************************
*                                                                        *
* Declare four externals that give the user access to the two default    *
* setups. PAL (with external sync) and NTSC (with external syncs). The   *
* user can easily extract a copy of the definitions of these modes from  *
* the file video.c. Such a copy may be easily modified by refering to    *
* the module manual to support other modes.                              *
*                                                                        *
**************************************************************************/

extern video_setup_type ccir_esync;
extern video_setup_type rs170_esync;
extern video_setup_type ccir_isync;
extern video_setup_type rs170_isync;


/*************************************************************************
*                                                                        *
* Give the user access to the video control register soft copy in their  *
* software. And some other internal stuff.                               *
*                                                                        *
**************************************************************************/

#ifdef ILIB

extern int VIDCTRL;
#define video_control_reg (VIDCTRL)

#else

extern int video_control_reg;

#endif

extern int g_line_inc;


/*************************************************************************
*                                                                        *
* Declare all of the function prototypes.                                *
*                                                                        *
**************************************************************************/

/* Board setup functions */

void video_setup(video_setup_type *v);

#ifndef COLOUR
void video_set_adc_lut_entry(int e, int x);
void video_set_adc_lut(int *x);
#endif

void video_set_videomask(int x);
void video_set_colourreg(int x);
void video_set_videomask_overlay(int x);
void video_set_colourreg_overlay(int x);
void video_set_dac_lut_entry(int e, int r, int g, int b);
void video_set_dac_lut(int *r, int *g, int *b);
void video_set_dac_lut_overlay_entry(int e, int r, int g, int b);
void video_set_dac_lut_overlay(int *r, int *g, int *b);

/* Board status functions */

int video_check_lock(void);
void video_wait_lock(void);
void video_clear_int_line(void);
void video_clear_int_field(void);
void video_clear_ints(void);
void video_wait_line_event(void);
void video_wait_field_event(void);
int video_get_field(void);               /* This is not documented */
void video_sync_on_component(int comp);  /* This is not documented */
void video_sync_off_component(int comp); /* This is not documented */

/* Board mode functions */

void video_pass_through(void);
void video_capture_cont(void);
void video_display(void);
void video_disable(void);
void video_grab(void);

/* Graphics and text setup functions */

void video_set_size(int width, int height);
void video_get_size(int *width, int *height);
void video_set_window(int x, int y, int w, int h);
void video_get_window(int *x, int *y, int *w, int *h);
void video_set_screen_base(int addr);
int video_get_screen_base(void);

/* Graphics functions */

void video_set_colour(int c);
int video_get_colour(void);
void video_clear_screen(void);
void video_clear_window(void);
void video_clear_overlay(void);
void video_clear_image(void);
void video_set_pixel(int x, int y);
int video_get_pixel(int x, int y);
void video_plot_line(int x1, int y1, int x2, int y2);
void video_circle_solid(int x, int y, int r);
void video_circle(int x, int y, int r);
void video_rect_solid(int x1, int y1, int x2, int y2);
void video_rect(int x1, int y1, int x2, int y2);
int video_fill(int x, int y);
int video_get_sprite_size(int w, int h);
void video_get_sprite(int x, int y, int w, int h, int *im);
void video_put_sprite(int x, int y, int *im);

/* Text functions */

void video_home(void);
void video_set_text_colour(int c);
int video_get_text_colour(void);
void video_set_text_back_colour(int c);
int video_get_text_back_colour(void);
void video_gotoxy(int x, int y);
void video_getxy(int *x, int *y);
void video_set_text_scale(int xs, int ys);
void video_get_text_scale(int *xs, int *ys);
void video_outchar(char ch);
void video_outtext(char *s);
int video_printf(char *fmt, ...);

#endif
