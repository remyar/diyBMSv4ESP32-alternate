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
#include "../settings/settings.h"
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
static bool _isMounted = false;
static unsigned long _cpt = 0;
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
bool SDCARD_TaskInit(void)
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
            _isMounted = true;
        }
    }
    else
    {
        if (DEBUG_SD)
        {
            Serial.println("Card Mount Failed");
        }
        _isMounted = false;
    }
    return _isMounted;
}

SDFS *SDCARD_GetSD(void)
{
    return &SD;
}

bool SDCARD_IsMounted(void)
{
    return _isMounted;
}

void SDCARD_TaskRun(void)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();
    if ((_isMounted == true) && SD.exists("/extract.tar"))
    {
        TarUnpacker *TARUnpacker = new TarUnpacker();

        TARUnpacker->haltOnError(true);                                                            // stop on fail (manual restart/reset required)
        TARUnpacker->setTarVerify(true);                                                           // true = enables health checks but slows down the overall process
        TARUnpacker->setTarProgressCallback(BaseUnpacker::defaultProgressCallback);                // prints the untarring progress for each individual file
        TARUnpacker->setTarStatusProgressCallback(BaseUnpacker::defaultTarStatusProgressCallback); // print the filenames as they're expanded
        TARUnpacker->setTarMessageCallback(BaseUnpacker::targzPrintLoggerCallback);                // tar log verbosity

        if (!TARUnpacker->tarExpander(*SDCARD_GetSD(), "/extract.tar", *SDCARD_GetSD(), "/"))
        {
            Serial.printf("tarExpander failed with return code #%d\n", TARUnpacker->tarGzGetError());
        }

        SD.remove("/extract.tar");
    }

    if ((_isMounted == true) && (_mysettings->loggingEnabled == true))
    {
        _cpt += 15;
        if ( (_cpt % _mysettings->loggingFrequencySeconds) != 0 ){
            return;
        }
        
      /*  struct tm timeinfo;
        if (getLocalTime(&timeinfo, 1))
        {
            timeinfo.tm_year += 1900;
            // Month is 0 to 11 based!
            timeinfo.tm_mon++;

            // ESP_LOGD(TAG, "%04u-%02u-%02u %02u:%02u:%02u", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

            char filename[32];
            sprintf(filename, "/data_%04u%02u%02u.csv", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday);

            File file;

            if (SD.exists(filename))
            {
                file = SD.open(filename, FILE_APPEND);
            }
            else
            {
                // Create a new file
                uint64_t freeSpace = SD.totalBytes() - SD.usedBytes();
                // Ensure there is more than 25MB of free space on SD card before creating a file
                if (freeSpace > (uint64_t)(25 * 1024 * 1024))
                {
                    // Create the file
                    File file = SD.open(filename, FILE_WRITE);
                    if (file)
                    {
                        Serial.println("Create log " + String(filename));

                        file.print("DateTime,");

                        for (uint8_t i = 0; i < TotalNumberOfCells(); i++)
                        {
                            file.print("VoltagemV_");
                            file.print(i);
                            file.print(",InternalTemp_");
                            file.print(i);
                            file.print(",ExternalTemp_");
                            file.print(i);
                            file.print(",Bypass_");
                            file.print(i);
                            file.print(",PWM_");
                            file.print(i);
                            file.print(",BypassOverTemp_");
                            file.print(i);
                            file.print(",BadPackets_");
                            file.print(i);
                            file.print(",BalancemAh_");
                            file.print(i);

                            if (i < TotalNumberOfCells() - 1)
                            {
                                file.print(',');
                            }
                        }
                        file.println();
                    }
                }
                else
                {
                    Serial.println("SD card has less than 25MiB remaining, logging stopped");
                    // We had an error, so switch off logging (this is only in memory so not written perm.)
                    _isLogging = false;
                }
            }

            if (file && _isLogging)
            {
                char dataMessage[255];

                sprintf(dataMessage, "%04u-%02u-%02u %02u:%02u:%02u,", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
                file.print(dataMessage);

                for (uint8_t i = 0; i < TotalNumberOfCells(); i++)
                {
                    // This may output invalid data when controller is first powered up
                    sprintf(dataMessage, "%u,%i,%i,%c,%u,%c,%u,%u",
                            cmi[i].voltagemV, cmi[i].internalTemp,
                            cmi[i].externalTemp, cmi[i].inBypass ? 'Y' : 'N',
                            (int)((float)cmi[i].PWMValue / (float)255.0 * 100), cmi[i].bypassOverTemp ? 'Y' : 'N',
                            cmi[i].badPacketCount, cmi[i].BalanceCurrentCount);
                    file.print(dataMessage);
                    if (i < TotalNumberOfCells() - 1)
                    {
                        file.print(',');
                    }
                }
                file.println();
                file.close();

                Serial.println("Wrote to SD log");
            }
            else
            {
                Serial.println("Failed to create/append SD logging file");
                // We had an error opening the file, so switch off logging
                // mysettings.loggingEnabled = false;
            }
        }
        else {
            Serial.println("GetlocalTime Failed");
        }*/
    }
}