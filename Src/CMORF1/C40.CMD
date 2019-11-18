/****************************************************************************/
/* C40.CMD -  v4.70   COMMAND FILE FOR LINKING C40 C PROGRAMS               */
/*                                                                          */
/*    Usage:       lnk30 <obj files...> c40.out -o <out file> -m <map file> */
/*                 or cl30 <options> <source files...> -z c40.out           */
/*                                                                          */
/*    Description: This file is a sample command file that can be used      */
/*                 for linking programs built with the TMS320C40 C          */
/*                 Compiler.   Use it a guideline; you may want to change   */
/*                 the allocation scheme according to the size of your      */
/*                 program and the memory layout of your target system.     */
/*                                                                          */
/*    Notes: (1)   Be sure to use the correct library!  Use a library that  */
/*                 matches the runtime model you are using.                 */
/*                                                                          */
/*           (2)   You must specify the directory in which rts40.lib is     */
/*                 located.  Either add a "-i<directory>" line to this      */
/*                 file, or use the system environment variable C_DIR to    */
/*                 specify a search path for libraries.                     */
/*                                                                          */
/*           (3)   When using the small (default) memory model, be sure     */
/*                 that the ENTIRE .bss section fits within a single page.  */
/*                 To satisfy this, .bss must be smaller than 64K words and */
/*                 must not cross any 64K boundaries.                       */
/****************************************************************************/
-c                                         /* LINK USING C CONVENTIONS      */
-stack 0x200                               /* 512 WORD STACK                */
-heap  0x200                               /* 512 WORD HEAP                 */
-l rts40.lib                               /* GET RUN-TIME SUPPORT          */
-o c40.out                                 /* NAME OF OUTPUT FILE           */
-m c40.map	                               /* NAME OF MAP FILE              */
-w                                         /* WARN IF NOT IN SECTIONS       */
-x                                         /* ALLOW RE-READING OF LIB       */

/* SPECIFY THE SYSTEM MEMORY MAP FOR TI 'C40 PPDS */

MEMORY
{
   RAM0:  org = 0x002ff800 len = 0x00000400   /* RAM BLOCK 0                */
   RAM1:  org = 0x002ffC00 len = 0x00000400   /* RAM BLOCK 1                */
   LOCAL: org = 0x40000000 len = 0x00010000   /* LOCAL BUS                  */
   GLOBAL:org = 0x80000000 len = 0x00020000   /* GLOBAL BUS                 */
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
   .vector: > LOCAL                  /* INTERRUPT VECTOR TABLE              */
   .text:   > RAM0                   /* CODE                                */
   .cinit:  > RAM0                   /* C INITIALIZATION TABLES             */
   .const:  > RAM0                   /* CONSTANTS                           */
   .stack:  > RAM1                   /* SYSTEM STACK                        */
   .sysmem: > RAM1                   /* DYNAMIC MEMORY (HEAP)               */
   .bss:    > RAM0, block = 0x10000  /* VARIABLES                           */
   .data:   > RAM0	                 /* DATA - NOT USED FOR C CODE          */
}
