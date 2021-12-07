//================================================================================================//
//                                                                                                //
// PROJET       : DongleWifi GoodRace                                                             //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 27/01/2020                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//
#define DEST_FS_USES_SD
#include "./sdcard.h"
#include <ESP32-targz.h>
//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//

//================================================================================================//
//                                      STRUCTURES ET UNIONS                                      //
//================================================================================================//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 VARIABLES PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void SDCARD_TaskInit(void)
{
    // Initialize SD card
    if (SD.begin(GPIO_PIN_SDCARD_CHIPSELECT))
    {
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE)
        {
            if (DEBUG_SD)
            {
                Serial.println("No SD card attached");
            }
        }
        else
        {
            if (DEBUG_SD)
            {
                Serial.println("SD card available");
            }
        }
    }
    else
    {
        if (DEBUG_SD)
        {
            Serial.println("Card Mount Failed");
        }
    }
}

SDFS *SDCARD_GetSD(void)
{
    return &SD;
}

void SDCARD_TaskRun(void)
{
    if ( SD.exists("/extract.tar")){
        TarUnpacker *TARUnpacker = new TarUnpacker();

        TARUnpacker->haltOnError(true);                                                            // stop on fail (manual restart/reset required)
        TARUnpacker->setTarVerify(true);                                                           // true = enables health checks but slows down the overall process
        TARUnpacker->setTarProgressCallback(BaseUnpacker::defaultProgressCallback);                             // prints the untarring progress for each individual file
        TARUnpacker->setTarStatusProgressCallback(BaseUnpacker::defaultTarStatusProgressCallback); // print the filenames as they're expanded
        TARUnpacker->setTarMessageCallback(BaseUnpacker::targzPrintLoggerCallback);                // tar log verbosity

        if (!TARUnpacker->tarExpander(*SDCARD_GetSD(), "/extract.tar", *SDCARD_GetSD(), "/"))
        {
            Serial.printf("tarExpander failed with return code #%d\n", TARUnpacker->tarGzGetError());
        }

        SD.remove("/extract.tar");
    }
}