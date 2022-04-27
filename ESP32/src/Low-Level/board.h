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
#include "./WiFi.h"
#include "./Serial.h"
#include "./i2c.h"

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

#define GPIO_PIN_BUILTIN_LED    GPIO_NUM_13
#define GPIO_BUILTIN_LED_ON()   digitalWrite(GPIO_PIN_BUILTIN_LED, LOW)
#define GPIO_BUILTIN_LED_OFF()  digitalWrite(GPIO_PIN_BUILTIN_LED, HIGH)
#define GPIO_BUILTIN_LED(X)     digitalWrite(GPIO_PIN_BUILTIN_LED, X)

#define GPIO_WRITE(X,Y)         digitalWrite(X, Y)

#define GPIO_PIN_BUTTON         GPIO_NUM_0
#define GPIO_BUTTON_VAL()       !digitalRead(GPIO_PIN_BUTTON)

#define GPIO_PIN_SDCARD_CHIPSELECT  GPIO_NUM_5
#define TCA9534A_INTERRUPT_PIN      GPIO_NUM_34

#define DEBUG_WIFI 0
#define DEBUG_MQTT 0
#define DEBUG_SD   0

#define RS485_RX GPIO_NUM_21
#define RS485_TX GPIO_NUM_22
#define RS485_ENABLE GPIO_NUM_25

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