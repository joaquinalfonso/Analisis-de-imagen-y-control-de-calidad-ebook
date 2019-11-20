-c                                         /* LINK USING C CONVENTIONS      */
-stack 0x200                               /* 512 WORD STACK                */
-i c:\dsp                                  /* DIRECTORIO DE LA LIBRERIA     */
-heap  0x200                               /* 512 WORD HEAP                 */
-l rts40.lib                               /* GET RUN-TIME SUPPORT          */
-o dsp.out                                 /* NAME OF OUTPUT FILE           */
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

