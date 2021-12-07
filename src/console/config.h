//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// HEADER       : config.h
//
//=============================================================================
#ifndef CONFIG_H_
#define CONFIG_H_
 
/*-----------------------------------------------------------------------*
 * K-BOOT command config                                                 *
 *-----------------------------------------------------------------------*/
#include "config_cmd_all.h"
 
/*-----------------------------------------------------------------------*
 * K-BOOT debug Messages                                                 *
 *-----------------------------------------------------------------------*/
#define _DEBUG    0 /* use for print messages     0 => No Messages
                                                1 => Messages*/
#if _DEBUG > 0
   //#define _DEBUG_FLASH
   //#define _DEBUG_BOOT_CMD
   //#define _DEBUG_FLASH_CMD
   //#define _DEBUG_LOAD_CMD
   //#define _DEBUG_MEM_CMD
   //#define _DEBUG_NVEDIT_CMD
   //#define _DEBUG_COMMAND
   //#define _DEBUG_ENV
#endif
 
#define  PROTOCOLE_KIMO 1  /* use for com with KIMO Software
                                                1 => Yes but no console
                                                0 => No */
 
/*-----------------------------------------------------------------------*
 * INFO CPU                                                              *
 *-----------------------------------------------------------------------*/
//#define CONFIG_DISPLAY_CPUINFO    /* Print CPUINFO  */
 
#define  CPU_NAME          "RENESAS 78F1505A"      /* CPU Name */
#define  CPU_SPEED         freqClock   /* CPU Frequency */
#define  BASE_FLASH        0x00000000    /* Internal FLASH */
#define  BASE_SRAM         0x000FE300    /* Internal SRAM */
 
/*-----------------------------------------------------------------------*
 * FLASH organization                                                    *
 *-----------------------------------------------------------------------*/
 
#define  CONFIG_SYS_MAX_FLASH_BANKS       1    /* max number of memory banks           */
#define  CONFIG_SYS_MAX_FLASH_SECT        128    /* max number of sectors on one chip   */
#define  CONFIG_SYS_FLASH_SECT_SIZE       1024    /* size of one sectors ( 1Ko )         */
#define  CONFIG_SYS_FLASH_BANKS           { CONFIG_SYS_MAX_FLASH_BANKS }
#define  CONFIG_LOADADDR                  0x0003800   /* Start Programming addr */
 
#define  CONFIG_SYS_MEMTEST_START      BASE_SRAM + 0x8000 /* start addr for mtest */
#define  CONFIG_SYS_MEMTEST_END          CONFIG_SYS_MEMTEST_START + 0x1000  /* end addr for mtest */
#define  CONFIG_SYS_LOAD_ADDR          CONFIG_LOADADDR
 
/*-----------------------------------------------------------------------*
 * HELP organization                                                     *
 *-----------------------------------------------------------------------*/
//#define    CONFIG_SYS_LONGHELP        1
 
/*-----------------------------------------------------------------------*
 * K-BOOT bootdelay                                                      *
 *-----------------------------------------------------------------------*/
#define     CONFIG_BOOTDELAY    5         /* nb secs avant lancement programme */
 
/*-----------------------------------------------------------------------*
 * K-BOOT environment config                                             *
 *-----------------------------------------------------------------------*/
#define  CONFIG_NAME_ENV_SIZE    16
#define     CONFIG_PARAM_ENV_SIZE    16
 
 
/*-----------------------------------------------------------------------*
 * UART speed value                                                      *
 *-----------------------------------------------------------------------*/
#define  CONFIG_BAUDRATE_19200   19200
 
#endif /* CONFIG_H_ */