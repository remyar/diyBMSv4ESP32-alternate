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

#include "./modules.json.h"
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
void MODULES_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();
    /*PacketRequestGenerator *_prg = BMS_GetPrg();
    PacketReceiveProcessor *_receiveProc = BMS_GetReceiveProc();
    Rules *_rules = RULES_Get();*/
    if (request->hasParam("c", false))
    {
        AsyncWebParameter *cellid = request->getParam("c", false);
        uint8_t c = cellid->value().toInt();

//        if (c > _mysettings->totalNumberOfBanks * _mysettings->totalNumberOfSeriesModules)
        {
            request->send(500, "text/plain", "Wrong parameter bank");
            return;
        }

    /*    if (cmi[c].settingsCached == false)
        {
            _prg->sendGetSettingsRequest(c);
        }*/

        AsyncResponseStream *response = request->beginResponseStream("application/json");

        DynamicJsonDocument doc(2048);
        JsonObject root = doc.to<JsonObject>();
        JsonObject settings = root.createNestedObject("settings");
/*
        uint8_t b = c / _mysettings->totalNumberOfSeriesModules;
        uint8_t m = c - (b * _mysettings->totalNumberOfSeriesModules);
        settings["bank"] = b;
        settings["module"] = m;*/
        settings["id"] = c;
        settings["ver"] = cmi[c].BoardVersionNumber;
        settings["code"] = cmi[c].CodeVersionNumber;
        settings["Cached"] = cmi[c].settingsCached;

        if (cmi[c].settingsCached)
        {
            settings["BypassOverTempShutdown"] = cmi[c].BypassOverTempShutdown;
            settings["BypassThresholdmV"] = cmi[c].BypassThresholdmV;
            settings["LoadRes"] = cmi[c].LoadResistance;
            settings["Calib"] = cmi[c].Calibration;
            settings["mVPerADC"] = cmi[c].mVPerADC;
            settings["IntBCoef"] = cmi[c].Internal_BCoefficient;
            settings["ExtBCoef"] = cmi[c].External_BCoefficient;
        }

        serializeJson(doc, *response);
        request->send(response);
    }
}