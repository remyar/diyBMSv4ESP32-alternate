//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _BOARD_
#define _BOARD_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include <Arduino.h>

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

#define GPIO_PIN_BUILTIN_LED 13
#define GPIO_BUILTIN_LED_ON() digitalWrite(GPIO_PIN_BUILTIN_LED, LOW)
#define GPIO_BUILTIN_LED_OFF() digitalWrite(GPIO_PIN_BUILTIN_LED, HIGH)
#define GPIO_BUILTIN_LED(X) digitalWrite(GPIO_PIN_BUILTIN_LED, X)

#define GPIO_WRITE(X,Y) digitalWrite(X, Y)

#define GPIO_PIN_BUTTON 0
#define GPIO_BUTTON_VAL() !digitalRead(GPIO_PIN_BUTTON)

#define GPIO_PIN_SDCARD_CHIPSELECT  5

#define DEBUG_WIFI 1
#define DEBUG_MQTT 1
#define DEBUG_SD    1

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
void BOARD_Init(void);

#endif //--- _BOARD_