//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//
#include "../Low-Level/Serial.h"
#include "command.h"
//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//
#define CONFIG_SYS_MAXARGS        8        /* Max number of command args    */

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//
typedef enum {
    CONSOLE_DISPLAY_PROMPT = 0,
    CONSOLE_READ_LINE,
    CONSOLE_EXECUTE_CMD,
    CONSOLE_IDLE,
}e_CONSOLE;

//================================================================================================//
//                                      STRUCTURES ET UNIONS                                      //
//================================================================================================//

//================================================================================================//
//                                VARIABLES ET FONCTION PARTAGEES                                 //
//================================================================================================//

//------------------------------------------------------------------------------------------------//
//---                                        Variables                                         ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Fonctions                                         ---//
//------------------------------------------------------------------------------------------------//
bool CONSOLE_TaskInit(void);
void CONSOLE_TaskRun(void);

#endif //--- _BOARD_