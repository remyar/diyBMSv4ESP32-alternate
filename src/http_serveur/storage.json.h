//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _STORAGE_JSON_H_
#define _STORAGE_JSON_H_

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
void STORAGE_JSON(AsyncWebServerRequest *request);
void STORAGE_Save(AsyncWebServerRequest *request);
#endif //--- _BOARD_
