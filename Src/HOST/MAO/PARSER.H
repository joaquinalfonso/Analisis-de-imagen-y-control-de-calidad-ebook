/***************************************************************************
 *
 * Loughborouh Sound Images Ltd.
 * The Technology Centre
 * Epinal Way
 * Loughborough
 * LE11 0QE
 *
 * Phone: (+44) 509 231843
 * Fax: (+44) 509 262433
 *
 * Project: Global config file reader.
 *
 * Author: Bijal R. Shah
 *
 * Filename: parser.h
 *
 * Revision & Date:	V1.00 13/5/93
 *					V2.00 8/9/93
 *					V2.10 27/7/94
 *					V2.20 31/10/94
 *                  V2.30 16/3/95
 *                  V2.31 19/4/95
 *      
 * Description: Header file for parser.
 *
 ***************************************************************************/

#ifndef PARSER_H
#define PARSER_H

/****************************************************************************
 *
 * Common Include files.
 *
 ****************************************************************************/
#ifdef DLL
#include <windows.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************
 *
 * Preprocessor directives.
 *
 ****************************************************************************/

/*
 * For things that have no values
 */ 
#define CFR_NULL_ID		    -1
#define CFR_NULL_STR	    "\0"
#define CFR_NULL_CHAR	    '\0'
#define CFR_STRING_LENGTH   16

/****************************************************************************
 *
 * Structure and Type Definitions
 *
 ****************************************************************************/

/*
 * Useful Data Types
 */
typedef enum {
	CFR_FALSE = 0,
	CFR_TRUE
} CFR_BOOL;

/*
 * This may cause a warning when ULONG is redefined
 * elsewhere in the build.
 */
#ifndef ULONG
typedef unsigned long ULONG;
#endif

/*
 * Error codes
 */
typedef enum {
	CFR_NO_ERROR = 0,
	CFR_ERROR_INTERNAL_FATAL,
	CFR_ERROR_UNABLE_TO_OPEN_FILE,
	CFR_ERROR_SYNTAX_ERROR,
	CFR_ERROR_INVALID_NAME,
	CFR_ERROR_INVALID_ID,
	CFR_ERROR_UNKNOWN_HANDLE,
	CFR_ERROR_DUPLICATE_PROC_NAME,
	CFR_ERROR_ONLY_ONE_SHARE_INSTANCE,
	CFR_ERROR_BAD_LINK_PROC_NAME,
	CFR_ERROR_MODULES_ON_SAME_SITE,
	CFR_ERROR_INVALID_BOARD_TYPE,
	CFR_ERROR_INVALID_MODULE_TYPE,
	CFR_ERROR_INVALID_PROCESSOR_TYPE,
	CFR_ERROR_MORE_MODULES_THAN_SITES,
	CFR_ERROR_BAD_SITE_NUMBER,
	CFR_ERROR_OVERLAPPING_MODULES,
	CFR_ERROR_TOO_MANY_BOARD_PROCESSORS,
	CFR_ERROR_TOO_MANY_MODULE_PROCESSORS
} CFR_ERROR_CODE;

/*
 * Structures used to hold config file info
 * and type definitions
 */

typedef char STRING[CFR_STRING_LENGTH];

/*
 * Handle to access config file...
 */
typedef short CFR_ID;

typedef short CFR_HOST_ID;
typedef short CFR_HOST_C_ID;
typedef short CFR_BOARD_ID;
typedef short CFR_MODULE_ID;
typedef short CFR_PROC_ID;
typedef short CFR_PAGE_ID;
typedef short CFR_LINK_ID;
typedef short CFR_SHARE_ID;

struct module {
	CFR_MODULE_ID module_id;
	CFR_BOARD_ID board_id;
	CFR_HOST_ID host_id;
	STRING module_type;
	short site_num;
	short num_mod_procs;
	CFR_PROC_ID *mod_proc_ids;
};

typedef struct module CFR_MODULE;

struct host_connect {
	CFR_HOST_C_ID connect_id;
#ifdef IO_MAPPED
	ULONG io_address;
#endif
#ifdef DEVICE_DRIVER
	STRING io_device;
#endif
};

typedef struct host_connect CFR_HOST_CONNECT;

struct dpram_location {
	ULONG start_address;
	ULONG end_address;
};

typedef struct dpram_location CFR_DPRAM;

struct host {
	CFR_HOST_ID host_id;
	STRING hostname;
	short num_boards;
	CFR_BOARD_ID *board_ids;
};

typedef struct host CFR_HOST;

struct reg_preset {
	CFR_HOST_C_ID connect_id;
	ULONG offset;
	ULONG value;
};

typedef struct reg_preset CFR_REG_PRESET;

struct board {
	CFR_BOARD_ID board_id;
	CFR_HOST_ID host_id;
	short bus_width;
	STRING board_type;
	short num_host_connections;
	CFR_HOST_CONNECT *host_connections;
	short num_register_presets;
    CFR_REG_PRESET *preset_registers;
	short num_dpram_locations;
	CFR_DPRAM *dpram;
	short num_board_modules;
	CFR_MODULE_ID *module_ids;
	short num_board_procs;
	CFR_PROC_ID *board_proc_ids;
};

typedef struct board CFR_BOARD;

typedef enum {
	NO_RT_ACCESS = 0,
	RT_ACCESS
} CFR_ACCESS;

/*
 * The CFR_MEM_MAP structure may cause a member alignment sensitivity
 * warning because the members of the structure have not been aligned
 * to word boundaries.
 */
struct mem_map {
	STRING mem_type;
	ULONG start_address;
	ULONG end_address;
	CFR_ACCESS rt_access;
	CFR_SHARE_ID share_id;
};

typedef struct mem_map CFR_MEM_MAP;

/*
 * The CFR_PROCESSOR structure may cause a member alignment sensitivity
 * warning because the members of the structure have not been aligned
 * to word boundaries.
 */
struct processor {
	CFR_PROC_ID proc_id;
	CFR_MODULE_ID module_id;
	CFR_BOARD_ID board_id;
	CFR_HOST_ID host_id;
	STRING proc_type;
	double clk_speed;
	short num_mem_pages;
	CFR_MEM_MAP *memory_map;
};

typedef struct processor CFR_PROCESSOR;

struct link_map {
	CFR_LINK_ID link_id;
	CFR_PROC_ID from_proc;
	short from_proc_port;
	CFR_PROC_ID to_proc;
	short to_proc_port;
	CFR_BOOL two_way;
};

typedef struct link_map CFR_LINK;

struct shared_mem_with {
	CFR_PROC_ID proc_id;
	CFR_PAGE_ID page_id;
};

typedef struct shared_mem_with CFR_SHARE_INFO;

struct shared_mem {
	CFR_SHARE_ID shared_id;
	short num_procs_in_share;
	CFR_SHARE_INFO *share_info;
};

typedef struct shared_mem CFR_SHARE;

struct config_file {
	short number_of_hosts;
	CFR_HOST *hosts;
	short number_of_boards;
	CFR_BOARD *boards;
	short number_of_modules;
	CFR_MODULE *modules;
	short number_of_processors;
	CFR_PROCESSOR *processors;
	short number_of_links;
	CFR_LINK *links;
	short number_of_shares;
	CFR_SHARE *mem_shares;
};

typedef struct config_file CFR_CONFIG;

/*
 * internal usage.
 */
struct handle {

	CFR_ID handle_id;

	char config_filename[255];

	/*
     * These must be known permanently
     */
    short number_of_hosts;
	short number_of_boards;
	short number_of_modules;
	short number_of_processors;
	short number_of_links;
	short number_of_memory_shares;

	STRING *processor_names;
	STRING *mem_share_names;

	struct handle *next;

	/*
	 * (Internal Only) counters
	 */
	short num_hosts;
	short *num_boards_h;

	short num_boards;
	short *num_host_connects_b;
	short *num_reg_b;
	short *num_dpram_b;
	short *num_mods_b;
	short *num_procs_b;

	short num_modules;
	short *num_procs_m;

	short num_processors;
	short *num_mem_p;

	short num_links;

	short num_shares;
	short *num_procs_s;

};

typedef struct handle CFR_HANDLE;

/*
 * Fill idents for third pass of parser (internal);
 */
enum filler {
	NO_FILL = -1,
	FILL_CONFIG = 1,
	FILL_HOST,
	FILL_BOARD,
	FILL_MODULE,
	FILL_PROCESSOR,
	FILL_LINKS,
	FILL_SHARE
};

typedef enum filler CFR_FILL;

/****************************************************************************
 *
 * Accessible global data.
 *
 ****************************************************************************/

extern int CFR_Error_Line_Number;

/****************************************************************************
 *
 * Parsing function prototypes (public functions).
 *
 ****************************************************************************/

#ifdef DLL
CFR_ERROR_CODE __export FAR PASCAL CFR_init(CFR_ID far *, char far *);

CFR_ERROR_CODE __export FAR PASCAL CFR_get_processor_name(CFR_ID, CFR_PROC_ID , STRING far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_get_share_name(CFR_ID, CFR_SHARE_ID , STRING far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_get_processor_id(CFR_ID, CFR_PROC_ID far *, char far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_get_share_id(CFR_ID, CFR_SHARE_ID far *, char far *);

CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_info(CFR_ID, CFR_CONFIG far **);
CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_host_info(CFR_ID, CFR_HOST far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_board_info(CFR_ID, CFR_BOARD far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_module_info(CFR_ID, CFR_MODULE far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_processor_info(CFR_ID, CFR_PROCESSOR far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_link_info(CFR_ID, CFR_LINK far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_all_share_info(CFR_ID, CFR_SHARE far **);
CFR_ERROR_CODE __export FAR PASCAL CFR_get_host_info(CFR_ID, CFR_HOST_ID, CFR_HOST far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_board_info(CFR_ID, CFR_BOARD_ID, CFR_BOARD far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_module_info(CFR_ID, CFR_MODULE_ID, CFR_MODULE far **);
CFR_ERROR_CODE __export FAR PASCAL CFR_get_processor_info(CFR_ID, CFR_PROC_ID, CFR_PROCESSOR far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_link_info(CFR_ID, CFR_LINK_ID, CFR_LINK far **); 
CFR_ERROR_CODE __export FAR PASCAL CFR_get_share_info(CFR_ID, CFR_SHARE_ID, CFR_SHARE far **);

CFR_ERROR_CODE __export FAR PASCAL CFR_free_handle(CFR_ID);

CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_info(CFR_ID, CFR_CONFIG far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_hosts(CFR_ID, CFR_HOST far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_boards(CFR_ID, CFR_BOARD far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_modules(CFR_ID, CFR_MODULE far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_processors(CFR_ID, CFR_PROCESSOR far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_links(CFR_ID, CFR_LINK far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_all_shared_mems(CFR_ID, CFR_SHARE far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_host(CFR_ID, CFR_HOST far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_board(CFR_ID, CFR_BOARD far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_module(CFR_ID, CFR_MODULE far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_processor(CFR_ID, CFR_PROCESSOR far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_link(CFR_ID, CFR_LINK far *);
CFR_ERROR_CODE __export FAR PASCAL CFR_free_shared_mem(CFR_ID, CFR_SHARE far *);
#else
CFR_ERROR_CODE CFR_init(CFR_ID *, char *);

CFR_ERROR_CODE CFR_get_processor_name(CFR_ID, CFR_PROC_ID , STRING *);
CFR_ERROR_CODE CFR_get_share_name(CFR_ID, CFR_SHARE_ID , STRING *);
CFR_ERROR_CODE CFR_get_processor_id(CFR_ID, CFR_PROC_ID *, char *);
CFR_ERROR_CODE CFR_get_share_id(CFR_ID, CFR_SHARE_ID *, char *);

CFR_ERROR_CODE CFR_get_all_info(CFR_ID, CFR_CONFIG **);
CFR_ERROR_CODE CFR_get_all_host_info(CFR_ID, CFR_HOST **); 
CFR_ERROR_CODE CFR_get_all_board_info(CFR_ID, CFR_BOARD **); 
CFR_ERROR_CODE CFR_get_all_module_info(CFR_ID, CFR_MODULE **); 
CFR_ERROR_CODE CFR_get_all_processor_info(CFR_ID, CFR_PROCESSOR **); 
CFR_ERROR_CODE CFR_get_all_link_info(CFR_ID, CFR_LINK **); 
CFR_ERROR_CODE CFR_get_all_share_info(CFR_ID, CFR_SHARE **);
CFR_ERROR_CODE CFR_get_host_info(CFR_ID, CFR_HOST_ID, CFR_HOST **); 
CFR_ERROR_CODE CFR_get_board_info(CFR_ID, CFR_BOARD_ID, CFR_BOARD **);
CFR_ERROR_CODE CFR_get_module_info(CFR_ID, CFR_MODULE_ID, CFR_MODULE **); 
CFR_ERROR_CODE CFR_get_processor_info(CFR_ID, CFR_PROC_ID, CFR_PROCESSOR **); 
CFR_ERROR_CODE CFR_get_link_info(CFR_ID, CFR_LINK_ID, CFR_LINK **); 
CFR_ERROR_CODE CFR_get_share_info(CFR_ID, CFR_SHARE_ID, CFR_SHARE **);

CFR_ERROR_CODE CFR_free_handle(CFR_ID);

CFR_ERROR_CODE CFR_free_all_info(CFR_ID, CFR_CONFIG *);
CFR_ERROR_CODE CFR_free_all_hosts(CFR_ID, CFR_HOST *);
CFR_ERROR_CODE CFR_free_all_boards(CFR_ID, CFR_BOARD *);
CFR_ERROR_CODE CFR_free_all_modules(CFR_ID, CFR_MODULE *);
CFR_ERROR_CODE CFR_free_all_processors(CFR_ID, CFR_PROCESSOR *);
CFR_ERROR_CODE CFR_free_all_links(CFR_ID, CFR_LINK *);
CFR_ERROR_CODE CFR_free_all_shared_mems(CFR_ID, CFR_SHARE *);
CFR_ERROR_CODE CFR_free_host(CFR_ID, CFR_HOST *);
CFR_ERROR_CODE CFR_free_board(CFR_ID, CFR_BOARD *);
CFR_ERROR_CODE CFR_free_module(CFR_ID, CFR_MODULE *);
CFR_ERROR_CODE CFR_free_processor(CFR_ID, CFR_PROCESSOR *);
CFR_ERROR_CODE CFR_free_link(CFR_ID, CFR_LINK *);
CFR_ERROR_CODE CFR_free_shared_mem(CFR_ID, CFR_SHARE *);
#endif

#endif /* PARSER_H */
