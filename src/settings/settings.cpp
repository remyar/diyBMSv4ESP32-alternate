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
#include "./settings.h"
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 VARIABLES PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
diybms_eeprom_settings _mysettings;
//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
void _initialise(void)
{
    _mysettings.wifi_ssid = "Livebox-1326";
    _mysettings.wifi_psk = "";
    _mysettings.language = "en";
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
bool SETTINGS_Load(void)
{
    bool initialiseSettings = false;
    StaticJsonDocument<4096> doc;

    if (SDCARD_IsMounted() && SDCARD_GetSD()->exists("/config.json"))
    {
        File f = SDCARD_GetSD()->open("/config.json", "r");

        deserializeJson(doc, f);

        f.close();

        _mysettings.wifi_ssid = doc["wifi_ssid"].as<String>();
        _mysettings.wifi_psk = doc["wifi_psk"].as<String>();

        _mysettings.language = doc["language"].as<String>();

        _mysettings.totalControllers = doc["totalControllers"].as<uint8_t>();

        for (int i = 0; i < _mysettings.totalControllers; i++)
        {
            _mysettings.totalNumberOfSeriesModules[i] = doc["totalNumberOfSeriesModules_" + String(i)].as<uint8_t>();
            _mysettings.totalNumberOfBanks[i] = doc["totalNumberOfBanks_" + String(i)].as<uint8_t>();
            _mysettings.baudRate[i] = doc["baudRate_" + String(i)].as<uint8_t>();

            _mysettings.BypassOverTempShutdown[i] = doc["BypassOverTempShutdown_" + String(i)].as<uint8_t>();
            _mysettings.BypassThresholdmV[i] = doc["BypassThresholdmV_" + String(i)].as<uint16_t>();
        }

        _mysettings.loggingEnabled = doc["loggingEnabled"];
        _mysettings.loggingFrequencySeconds = doc["loggingFrequencySeconds"];
    }
    else
    {
        initialiseSettings = true;
    }

    if (initialiseSettings == true)
    {

        Serial.println("Config.json ERROR , set default settings");

        _initialise();

        SETTINGS_Save();
    }

    return false;
}

diybms_eeprom_settings *SETTINGS_Get(void)
{
    return &_mysettings;
}

void SETTINGS_Save(void)
{
    StaticJsonDocument<4096> doc;

    Serial.println("on sauvegarde");

    doc["wifi_ssid"] = _mysettings.wifi_ssid;
    doc["wifi_psk"] = _mysettings.wifi_psk;
    doc["language"] = _mysettings.language;

    doc["totalControllers"] = _mysettings.totalControllers;

    for (int i = 0; i < _mysettings.totalControllers; i++)
    {
        doc["totalNumberOfSeriesModules_" + String(i)] = _mysettings.totalNumberOfSeriesModules[i];
        doc["totalNumberOfBanks_" + String(i)] = _mysettings.totalNumberOfBanks[i];
        doc["baudRate_" + String(i)] = _mysettings.baudRate[i];

        doc["BypassOverTempShutdown_" + String(i)] = _mysettings.BypassOverTempShutdown[i];
        doc["BypassThresholdmV_" + String(i)] = _mysettings.BypassThresholdmV[i];
    }

    doc["loggingEnabled"] = _mysettings.loggingEnabled;
    doc["loggingFrequencySeconds"] = _mysettings.loggingFrequencySeconds;

    if (SDCARD_IsMounted())
    {
        File f = SDCARD_GetSD()->open("/config.json", "w");

        serializeJson(doc, f);

        f.close();
    }
}
