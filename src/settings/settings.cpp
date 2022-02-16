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

        //_mysettings.language = doc["language"].as<String>();
        _mysettings.language = "en";
        _mysettings.totalNumberOfBanks = 1;
        _mysettings.totalNumberOfSeriesModules = 1;
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

    doc["wifi_ssid"] = _mysettings.wifi_ssid;
    doc["wifi_psk"] = _mysettings.wifi_psk;
    doc["language"] = _mysettings.language;

    if ( SDCARD_IsMounted() ){
        File f = SDCARD_GetSD()->open("/config.json", "w");

        serializeJson(doc, f);

        f.close();
    }
}


uint32_t TotalNumberOfCells(void)
{
    return _mysettings.totalNumberOfBanks * _mysettings.totalNumberOfSeriesModules;
}