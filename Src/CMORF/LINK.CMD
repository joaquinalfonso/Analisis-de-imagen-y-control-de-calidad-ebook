-c
-x
-l rts40.lib

/* Specify memory configuration */

MEMORY
  {
	VECS:   	origin = 00000000h	length = 0040h
	ROM:	        origin = 00000040h	length = 0FC0h
	IRAM0:	        origin = 002FF800h	length = 0400h
	IRAM1:  	origin = 002FFC00h	length = 0400h
        VRAM:           origin = 10000000h      length = 40000h
        OVERLAY:        origin = 20000000h      length = 40000h
        DRAM:           origin = 30000000h      length = 100000h
	PEROM:	        origin = 40000000h	length = 8000h
	SRAM:	        origin = 80000000h	length = 20000h
  }


/* Specify output sections */

SECTIONS
  {
        .cinit                  : { }   > SRAM
        .text			: { }	> SRAM
	.data			: { }	> SRAM
        .bss                    : { }   > SRAM
        .sysmem                 : { }   > SRAM
        .stack                  : { }   > IRAM0
        .const                  : { }   > SRAM
        .switch                 : { }   > SRAM
  }
