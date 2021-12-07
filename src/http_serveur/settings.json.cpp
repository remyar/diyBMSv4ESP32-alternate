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
#include "../bms/bms.h"
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

    // settings["Version"] = String(GIT_VERSION);
    // settings["CompileDate"] = String(COMPILE_DATE_TIME);

    settings["totalnumberofbanks"] = _mysettings->totalNumberOfBanks;
    settings["totalseriesmodules"] = _mysettings->totalNumberOfSeriesModules;
    settings["baudrate"] = _mysettings->baudRate;

    uint8_t numberOfV440OrNewer = 0;
    uint8_t totalModules = _mysettings->totalNumberOfBanks * _mysettings->totalNumberOfSeriesModules;
    for (uint8_t i = 0; i < totalModules; i++)
    {
        if (cmi[i].valid)
        {
            // Count of v440 or newer modules
            if (cmi[i].BoardVersionNumber >= 440)
            {
                numberOfV440OrNewer++;
            }
        }
    }
    // Only true if all modules have communicated and are all v440 or newer
    settings["supportSpeedChange"] = numberOfV440OrNewer == totalModules;

    settings["bypassthreshold"] = _mysettings->BypassThresholdmV;
    settings["bypassovertemp"] = _mysettings->BypassOverTempShutdown;

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