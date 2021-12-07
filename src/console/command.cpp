//=============================================================================
//
// PROJET       : POSTE FIXE 2013
// MODULE       : main.c
// DESCRIPTION  : initialisation et lancement de l'application
//
//-----------------------------------------------------------------------------
//
// HISTORIQUE   :
//
//     11.04.2012 - V0.01 : - Creation
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "console.h"
#include "command.h"
#include "config.h"

//-----------------------------------------------------------------------------
// Fonctions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constantes
//-----------------------------------------------------------------------------
const cmd_tbl_t __u_boot_cmd[] =
    {
#ifdef CONFIG_CMD_MEMORY
#if defined(CONFIG_SUB_CMD_MD) && defined(CONFIG_CMD_MEMORY)
        {"md", 3, 1, CMD_mem_md,
#ifdef CONFIG_CMD_HELP
         "memory display\r\n"
         "[.b, .w, .l] address [# of objects]"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_MM) && defined(CONFIG_CMD_MEMORY)
        {"mm", 2, 1, CMD_mem_mm,
#ifdef CONFIG_CMD_HELP
         "memory modify (auto-incrementing address)\r\n"
         "[.b, .w, .l] address"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_NM) && defined(CONFIG_CMD_MEMORY)
        {"nm", 2, 1, CMD_mem_nm,
#ifdef CONFIG_CMD_HELP
         "memory modify (constant address)\r\n"
         "[.b, .w, .l] address"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_MW) && defined(CONFIG_CMD_MEMORY)
        {"mw", 4, 1, CMD_mem_mw,
#ifdef CONFIG_CMD_HELP
         "memory write (fill)\r\n"
         "[.b, .w, .l] address value [count]"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_CP) && defined(CONFIG_CMD_MEMORY)
        {"cp", 4, 1, CMD_mem_cp,
#ifdef CONFIG_CMD_HELP
         "memory copy\r\n"
         "[.b, .w, .l] source target count"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_CMP) && defined(CONFIG_CMD_MEMORY)
        {"cmp", 4, 1, CMD_mem_cmp,
#ifdef CONFIG_CMD_HELP
         "memory compare\r\n"
         "[.b, .w, .l] addr1 addr2 count"
#endif
        },
#endif
#ifdef CONFIG_CMD_CRC32
#ifndef CONFIG_CRC32_VERIFY
        {"crc32", 4, 1, do_mem_crc,
#ifdef CONFIG_CMD_HELP
         "checksum calculation\r\n"
         "address count [addr]\r\n"
         "    - compute CRC32 checksum [save at addr]"
#endif
        },
#else /* CONFIG_CRC32_VERIFY */
        {"crc32", 5, 1, do_mem_crc,
#ifdef CONFIG_CMD_HELP
         "checksum calculation\r\n"
         "address count [addr]\r\n"
         "    - compute CRC32 checksum [save at addr]\r\n"
         "    -v address count crc\r\n"
         "    - verify crc of memory area"
#endif
        },
#endif /* CONFIG_CRC32_VERIFY */
#endif
#if defined(CONFIG_SUB_CMD_BASE) && defined(CONFIG_CMD_MEMORY)
        {"base", 2, 1, CMD_mem_base,
#ifdef CONFIG_CMD_HELP
         "print or set address offset\r\n"
         "   - print address offset for memory commands\r\n"
         "   base off\r\n"
         "   - set address offset for memory commands to 'off'"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_LOOP) && defined(CONFIG_CMD_MEMORY)
        {"loop", 3, 1, CMD_mem_loop,
#ifdef CONFIG_CMD_HELP
         "infinite loop on address range\r\n"
         "[.b, .w, .l] address number_of_objects"
#endif
        },
#endif
#ifdef CONFIG_CMD_LOOPW
        {"loopw", 4, 1, CMD_mem_loopw,
#ifdef CONFIG_CMD_HELP
         "infinite write loop on address range\r\n"
         "[.b, .w, .l] address number_of_objects data_to_write"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_MTEST) && defined(CONFIG_CMD_MEMORY)
        {"mtest", 5, 1, CMD_mem_mtest,
#ifdef CONFIG_CMD_HELP
         "simple RAM read/write test\r\n"
         "[start [end [pattern [iterations]]]]"
#endif
        },
#endif
#ifdef CONFIG_MX_CYCLIC
        {"mdc", 4, 1, do_mem_mdc,
#ifdef CONFIG_CMD_HELP
         "memory display cyclic\r\n"
         "[.b, .w, .l] address count delay(ms)"
#endif
        },
        {"mwc", 4, 1, do_mem_mwc,
#ifdef CONFIG_CMD_HELP
         "memory write cyclic\r\n"
         "[.b, .w, .l] address value delay(ms)"
#endif
        },
#endif /* CONFIG_MX_CYCLIC */
#endif /* CONFIG_CMD_MEMORY */

#if defined(PROTOCOLE_KIMO) && (PROTOCOLE_KIMO == 0)
        {"env", CONFIG_SYS_MAXARGS, 1, CMD_env,
#ifdef CONFIG_CMD_HELP
         "environment handling commands\r\n"
#if defined(CONFIG_CMD_ASKENV)
         "ask name [message] [size] - ask for environment variable\r\nenv "
#endif
         "default -f - reset default environment\r\n"
#if defined(CONFIG_CMD_EDITENV)
         "env edit name - edit environment variable\r\n"
#endif
         "env export [-t | -b | -c] [-s size] addr [var ...] - export environment\r\n"
#if defined(CONFIG_CMD_GREPENV)
         "env grep string [...] - search environment\r\n"
#endif
         "env import [-d] [-t | -b | -c] addr [size] - import environment\r\n"
         "env print [name ...] - print environment\r\n"
#if defined(CONFIG_CMD_RUN)
         "env run var [...] - run commands in an environment variable\r\n"
#endif
#if defined(CONFIG_CMD_SAVEENV) && !defined(CONFIG_ENV_IS_NOWHERE)
         "env save - save environment\r\n"
#endif
         "env set [-f] name [arg ...]"
#endif
        },
#endif
#ifdef CONFIG_CMD_FLASH

#if defined(CONFIG_SUB_CMD_FLINFO) && defined(CONFIG_CMD_FLASH)
        {"flinfo", 2, 1, CMD_flinfo,
#ifdef CONFIG_CMD_HELP
         "print FLASH memory information\r\n"
         "    - print information for all FLASH memory banks\r\n"
         "flinfo N\r\n"
         "    - print information for FLASH memory bank # N"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_ERASE) && defined(CONFIG_CMD_FLASH)
        {"erase", 3, 0, CMD_flerase,
#ifdef CONFIG_CMD_HELP
         "erase FLASH memory\r\n"
         "start end\r\n"
         "    - erase FLASH from addr 'start' to addr 'end'\r\n"
         "erase all\r\n"
         "    - erase all FLASH banks"
#endif
        },
#endif
#if defined(CONFIG_SUB_CMD_PROTECT) && defined(CONFIG_CMD_FLASH)
        {"protect", 4, 0, CMD_protect,
#ifdef CONFIG_CMD_HELP
         "enable or disable FLASH write protection\r\n"
         "on/off  start end"
#endif
        },
#endif
#endif /* CONFIG_CMD_FLASH */
#ifdef CONFIG_CMD_HWFLOW
        {"hwflow", 2, 0, do_hwflow,
#ifdef CONFIG_CMD_HELP
         "turn RTS/CTS hardware flow control in serial line on/off\r\n"
         "[on|off]"
#endif
        },
#endif
#ifdef CONFIG_CMD_LOADB
        {"loadb", 3, 0, do_load_serial_bin,
#ifdef CONFIG_CMD_HELP
         "load binary file over serial line (kermit mode)\r\n"
         "[ off ] [ baud ]\r\n"
         "    - load binary file over serial line with offset 'off' and baudrate 'baud'"
#endif
        },
        {"loady", 3, 0, do_load_serial_bin,
#ifdef CONFIG_CMD_HELP
         "load binary file over serial line (ymodem mode)\r\n"
         "[ off ] [ baud ]\r\n"
         "    - load binary file over serial line with offset 'off' and baudrate 'baud'"
#endif
        },
#endif
#ifdef CONFIG_CMD_LOADS
#ifdef CONFIG_SYS_LOADS_BAUD_CHANGE
        {"loads", 3, 0, do_load_serial,
#ifdef CONFIG_CMD_HELP
         "load S-Record file over serial line\r\n"
         "[ off ] [ baud ]\r\n"
         "    - load S-Record file over serial line with offset 'off' and baudrate 'baud'"
#endif
        },
#else
        {"loads", 2, 0, do_load_serial,
#ifdef CONFIG_CMD_HELP
         "load S-Record file over serial line\r\n"
         "[ off ]\r\n"
         "    - load S-Record file over serial line with offset 'off'"
#endif
        },
#endif
#endif
#ifdef CONFIG_CMD_SAVES
#ifdef CONFIG_SYS_LOADS_BAUD_CHANGE
        {"saves", 4, 0, CMD_do_save_serial,
#ifdef CONFIG_CMD_HELP
         "save S-Record file over serial line\r\n"
         "[ off ] [size] [ baud ]\r\n"
         "    - save S-Record file over serial line with offset 'off', size 'size' and baudrate 'baud'"
#endif
        },
#else
        {"saves", 3, 0, CMD_do_save_serial,
#ifdef CONFIG_CMD_HELP
         "save S-Record file over serial line\r\n"
         "[ off ] [size]\r\n"
         "    - save S-Record file over serial line with offset 'off' and size 'size'"
#endif
        },
#endif
#endif
#ifdef CONFIG_CMD_RESET
        {"reset", 1, 0, CPU_reset,
#ifdef CONFIG_CMD_HELP
         "Perform RESET of the CPU"
#endif
        },
#endif
#ifdef CONFIG_CMD_GO
        {"go", CONFIG_SYS_MAXARGS, 0, CMD_go,
#ifdef CONFIG_CMD_HELP
         "start application at address 'addr'\r\n"
         "addr [arg ...]\r\n"
         "    - start application at address 'addr'\r\n"
         "      passing 'arg' as arguments"
#endif
        },
#endif
};

//-----------------------------------------------------------------------------
// Variables globales
//-----------------------------------------------------------------------------
//=============================================================================
//--- DEFINITIONS
//=============================================================================
//-----------------------------------------------------------------------------
// FONCTION    : _cmd_usage
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static int16_t _cmd_usage(const cmd_tbl_t *cmdtp)
{
#ifdef CONFIG_CMD_HELP
#if defined(_DEBUG) && (_DEBUG > 0) && defined(_DEBUG_COMMAND)
    debug("%s - %s\r\n", cmdtp->name, cmdtp->usage);
#endif
#endif
    return 1;
}

//-----------------------------------------------------------------------------
// FONCTION    : _cmd_call
//
// DESCRIPTION : appel de la commande
//-----------------------------------------------------------------------------
static int16_t _cmd_call(cmd_tbl_t *cmdtp, int16_t flag, int16_t argc, char *const argv[])
{
    int16_t result;

    result = (cmdtp->cmd)(cmdtp, flag, argc, argv);

#if defined(_DEBUG) && (_DEBUG > 0) && defined(_DEBUG_COMMAND)
    if (result)
        debug("Command failed, result=%d\r\n", result);
#endif
    return result;
}

//-----------------------------------------------------------------------------
// FONCTION    : CMD_find_cmd_tbl
//
// DESCRIPTION : Cherche la commande correspondante dans le tableau
//-----------------------------------------------------------------------------
cmd_tbl_t *CMD_find_cmd_tbl(const char *cmd, cmd_tbl_t *table, int16_t table_len)
{
    cmd_tbl_t *cmdtp;
    cmd_tbl_t *cmdtp_temp = table; /*Init value */
    const char *p;
    int16_t len;
    int16_t n_found = 0;

    if (!cmd)
        return NULL;
    /*
     * Some commands allow length modifiers (like "cp.b");
     * compare command name only until first dot.
     */
    len = ((p = strchr(cmd, '.')) == NULL) ? strlen(cmd) : (p - cmd);

    for (cmdtp = table; cmdtp != table + (table_len / sizeof(cmd_tbl_t)); cmdtp++)
    {
        if (strncmp(cmd, cmdtp->name, len) == 0)
        {
            if (len == strlen(cmdtp->name))
                return cmdtp; /* full match */

            cmdtp_temp = cmdtp; /* abbreviated command ? */
            n_found++;
        }
    }
    if (n_found == 1)
    { /* exactly one match */
        return cmdtp_temp;
    }

    return NULL; /* not found or ambiguous command */
}

//-----------------------------------------------------------------------------
// FONCTION    : CMD_find_cmd
//
// DESCRIPTION : Cherche la commande correspondante
//-----------------------------------------------------------------------------
cmd_tbl_t *CMD_find_cmd(const char *cmd)
{
    uint16_t len = sizeof(__u_boot_cmd);
    return CMD_find_cmd_tbl(cmd, (struct cmd_tbl_s *)&__u_boot_cmd[0], len);
}

//-----------------------------------------------------------------------------
// FONCTION    : CMD_get_data_size
//
// DESCRIPTION : Permet de savoir si un argument précise la taille
//-----------------------------------------------------------------------------
#ifdef CMD_DATA_SIZE
int16_t CMD_get_data_size(uint8_t *arg, int16_t default_size)
{
    /* Check for a size specification .b, .w or .l.
     */
    int16_t len = STRING_strlen(arg);
    if (len > 2 && arg[len - 2] == '.')
    {
        switch (arg[len - 1])
        {
        case 'b':
            return 1;
        case 'w':
            return 2;
        case 'l':
            return 4;
        case 's':
            return -2;
        default:
            return -1;
        }
    }
    return default_size;
}
#endif

//-----------------------------------------------------------------------------
// FONCTION    : CMD_process
//
// DESCRIPTION : gestion de la commande
//-----------------------------------------------------------------------------
int16_t CMD_process(int16_t flag, int16_t argc, char *argv[], int16_t *repeatable)
{
    enum command_ret_t rc = CMD_RET_SUCCESS;
    cmd_tbl_t *cmdtp;

    /* Look up command in command table */
    cmdtp = CMD_find_cmd(argv[0]);
    if (cmdtp == NULL)
    {
#if defined(_DEBUG) && (_DEBUG > 0) && defined(_DEBUG_COMMAND)
        debug("Unknown command '%s' - try 'help'\r\n", argv[0]);
#endif
        return 1;
    }

    /* found - check max args */
    if (argc > cmdtp->maxargs)
        rc = CMD_RET_USAGE;

    /* If OK so far, then do the command */
    if (!rc)
    {
        rc = (enum command_ret_t)_cmd_call(cmdtp, flag, argc, argv);
        *repeatable &= cmdtp->repeatable;
    }
    if (rc == CMD_RET_USAGE)
        rc = (enum command_ret_t)_cmd_usage(cmdtp);
    return rc;
}