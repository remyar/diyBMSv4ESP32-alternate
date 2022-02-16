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

#include "./settings.json.h"
#include "../sdcard/sdcard.h"
#include "./printStream.h"

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
// FONCTION    : MONITOR2_JSON
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void SETTINGS_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();

    AsyncResponseStream *response =
        request->beginResponseStream("application/json");

    DynamicJsonDocument doc(2048);
    JsonObject root = doc.to<JsonObject>();

    JsonObject settings = root.createNestedObject("settings");

    settings["totalControllers"] = _mysettings->totalControllers;

    for (int i = 0; i < _mysettings->totalControllers; i++)
    {
        settings["totalNumberOfSeriesModules_" + String(i)] = _mysettings->totalNumberOfSeriesModules[i];
        settings["totalNumberOfBanks_" + String(i)] = _mysettings->totalNumberOfBanks[i];
        settings["baudRate_" + String(i)] = _mysettings->baudRate[i];
    }

    settings["NTPServerName"] = _mysettings->ntpServer;
    settings["TimeZone"] = _mysettings->timeZone;
    settings["MinutesTimeZone"] = _mysettings->minutesTimeZone;
    settings["DST"] = _mysettings->daylight;

    settings["FreeHeap"] = ESP.getFreeHeap();
    settings["MinFreeHeap"] = ESP.getMinFreeHeap();
    settings["HeapSize"] = ESP.getHeapSize();
    settings["SdkVersion"] = ESP.getSdkVersion();

    settings["HostName"] = WiFi.getHostname();

    time_t now;
    if (time(&now))
    {
        settings["now"] = now;
    }

    response->addHeader("Cache-Control", "no-store");

    serializeJson(doc, *response);
    request->send(response);
}