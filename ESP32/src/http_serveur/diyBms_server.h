//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _DIYBMS_SERVER_H_
#define _DIYBMS_SERVER_H_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//
#define DEST_FS_USES_SD
#include "../Low-Level/WiFi.h"
#include "../settings/settings.h"
#include "../typedefs.h"

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//

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
void DIYBMSServer_Begin(void);

#endif //--- _BOARD_
