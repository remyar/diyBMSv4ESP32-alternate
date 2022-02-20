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

#include "./globalSettings.h"
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
void GLOBALSETTINGS_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mySettings = SETTINGS_Get();
    if (request->hasParam("BypassOverTempShutdown", true) && request->hasParam("BypassThresholdmV", true) && request->hasParam("monduleId", true))
    {
        AsyncWebParameter *p0 = request->getParam("monduleId", true);
        uint8_t ctrlId = p0->value().toInt();

        AsyncWebParameter *p1 = request->getParam("BypassOverTempShutdown", true);
        _mySettings->BypassOverTempShutdown[ctrlId] = p1->value().toInt();

        AsyncWebParameter *p2 = request->getParam("BypassThresholdmV", true);
        _mySettings->BypassThresholdmV[ctrlId] = p2->value().toInt();

        SETTINGS_Save();

        for (uint8_t i = 0; i < (_mySettings->totalNumberOfSeriesModules[ctrlId] * _mySettings->totalNumberOfBanks[ctrlId]); i++)
        {
            if (CMI_Get(ctrlId, i)->valid)
            {
                CMI_Get(ctrlId, i)->BypassThresholdmV = _mySettings->BypassThresholdmV[ctrlId];
                CMI_Get(ctrlId, i)->BypassOverTempShutdown = _mySettings->BypassOverTempShutdown[ctrlId];
            }
        }

        MODBUS_SendGlobalSettings(ctrlId);

        AsyncResponseStream *response = request->beginResponseStream("application/json");
        StaticJsonDocument<100> doc;
        doc["success"] = true;
        serializeJson(doc, *response);
        request->send(response);
    }
    else
    {
        request->send(500, "text/plain", "Missing parameters");
    }
}