//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _RULES_JSON_H_
#define _RULES_JSON_H_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//
#include "../typedefs.h"
#include "../settings/settings.h"
#include "./web_server.h"
#include <ArduinoJson.h>

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
void RULES_JSON(AsyncWebServerRequest *request);
void RULES_Save(AsyncWebServerRequest *request);
#endif //--- _BOARD_
