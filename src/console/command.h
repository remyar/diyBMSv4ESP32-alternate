//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// HEADER       : command.h
//
//=============================================================================
#ifndef __COMMAND_H
#define __COMMAND_H

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "config.h"

//-----------------------------------------------------------------------------
// K-Boot  Typedefs
//-----------------------------------------------------------------------------
struct cmd_tbl_s
{
    char * name;                                                          /* Command Name            */
    uint16_t maxargs;                                                       /* maximum number of arguments    */
    uint16_t repeatable;                                                    /* autorepeat allowed?        */
    int16_t (*cmd)(struct cmd_tbl_s *, int16_t, int16_t, char *const[]); /* Implementation function    */
#ifdef CONFIG_CMD_HELP
    char * usage; /* Usage message    (short)    */
#endif
};

typedef struct cmd_tbl_s cmd_tbl_t;

/*
 * Error codes that commands return to cmd_process(). We use the standard 0
 * and 1 for success and failure, but add one more case - failure with a
 * request to call cmd_usage(). But the cmd_process() function handles
 * CMD_RET_USAGE itself and after calling cmd_usage() it will return 1.
 * This is just a convenience for commands to avoid them having to call
 * cmd_usage() all over the place.
 */
enum command_ret_t
{
    CMD_RET_SUCCESS,    /* 0 = Success */
    CMD_RET_FAILURE,    /* 1 = Failure */
    CMD_RET_USAGE = -1, /* Failure, please report 'usage' error */
};

//-----------------------------------------------------------------------------
// Constantes : defines et enums
//-----------------------------------------------------------------------------
#ifndef NULL
#define NULL 0
#endif

/* Default to a width of 8 characters for help message command width */
#ifndef CONFIG_SYS_HELP_CMD_WIDTH
#define CONFIG_SYS_HELP_CMD_WIDTH 8
#endif

#define CMD_FLAG_REPEAT 0x0001 /* repeat last command        */
#define CMD_FLAG_BOOTD 0x0002  /* command is from bootd    */

#define SREC3_START "S0030000FC\n"
#define SREC3_FORMAT "S3%02X%08lX%s%02X\n"
#define SREC3_END "S70500000000FA\n"
#define SREC_BYTES_PER_RECORD 16

#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)

//-----------------------------------------------------------------------------
// Variables et Fonctions partagees
//-----------------------------------------------------------------------------
cmd_tbl_t *CMD_find_cmd(const uint8_t *cmd);
cmd_tbl_t *CMD_find_cmd_tbl(const uint8_t *cmd, cmd_tbl_t *table, uint16_t table_len);

#if defined(CONFIG_CMD_MEMORY) || defined(CONFIG_CMD_I2C) || defined(CONFIG_CMD_ITEST) || defined(CONFIG_CMD_PCI) || defined(CONFIG_CMD_PORTIO)
/* utilisation de la commande cmd_get_data_size*/
#define CMD_DATA_SIZE
int16_t CMD_get_data_size(uint8_t *arg, int16_t default_size);
#endif

//-----------------------------------------------------------------------------
// Exécution des commandes
//-----------------------------------------------------------------------------
int16_t CMD_process(int16_t flag, int16_t argc, char *argv[], int16_t *repeatable);

//-----------------------------------------------------------------------------
// Listes des commandes
//-----------------------------------------------------------------------------
#ifdef CONFIG_CMD_MEMORY
int16_t CMD_mem_md(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_mem_mm(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_mem_nm(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_mem_mw(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_mem_cp(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_mem_cmp(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#ifdef CONFIG_CMD_CRC32
int16_t do_mem_crc(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif
int16_t CMD_mem_base(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_mem_loop(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#ifdef CONFIG_CMD_LOOPW
int16_t CMD_mem_loopw(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif
int16_t CMD_mem_mtest(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif /* CONFIG_CMD_MEMORY */
int16_t CMD_env(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#ifdef CONFIG_CMD_FLASH
int16_t CMD_flinfo(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_flerase(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_protect(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif /* CONFIG_CMD_FLASH */
#ifdef CONFIG_CMD_LOADB
int16_t do_load_serial_bin(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif
#ifdef CONFIG_CMD_LOADS
int16_t CMD_do_load_serial(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif
#ifdef CONFIG_CMD_SAVES
int16_t CMD_do_save_serial(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif
#ifdef CONFIG_CMD_HWFLOW
int16_t do_hwflow(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif
int16_t CPU_reset(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
int16_t CMD_flash_sect_erase(uint32_t addr_first, uint32_t addr_last);
#ifdef CONFIG_CMD_GO
int16_t CMD_go(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, uint8_t *const argv[]);
#endif

#endif /* __COMMAND_H */