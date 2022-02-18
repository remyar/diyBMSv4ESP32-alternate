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

#include "./rules.json.h"
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
void RULES_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(4096);
    JsonObject root = doc.to<JsonObject>();

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 100))
    {
        root["timenow"] = 0;
    }
    else
    {
        root["timenow"] = (timeinfo.tm_hour * 60) + timeinfo.tm_min;
    }

    JsonArray defaultArray = root.createNestedArray("relaydefault");
    for (uint8_t relay = 0; relay < RELAY_TOTAL; relay++)
    {
        switch (_mysettings->rulerelaydefault[relay])
        {
        case RELAY_OFF:
            defaultArray.add(false);
            break;
        case RELAY_ON:
            defaultArray.add(true);
            break;
        default:
            defaultArray.add((char *)0);
            break;
        }
    }

    JsonArray typeArray = root.createNestedArray("relaytype");
    for (uint8_t relay = 0; relay < RELAY_TOTAL; relay++)
    {
        switch (_mysettings->relaytype[relay])
        {
        case RELAY_STANDARD:
            typeArray.add("Std");
            break;
        case RELAY_PULSE:
            typeArray.add("Pulse");
            break;
        default:
            typeArray.add((char *)0);
            break;
        }
    }

    JsonArray bankArray = root.createNestedArray("rules");

    for (uint8_t r = 0; r < RELAY_RULES; r++)
    {
        JsonObject rule = bankArray.createNestedObject();
        rule["value"] = _mysettings->rulevalue[r];
        rule["hysteresis"] = _mysettings->rulehysteresis[r];
        rule["triggered"] = false;//_rules->rule_outcome[r];
        JsonArray data = rule.createNestedArray("relays");

        for (uint8_t relay = 0; relay < RELAY_TOTAL; relay++)
        {
            switch (_mysettings->rulerelaystate[r][relay])
            {
            case RELAY_OFF:
                data.add(false);
                break;
            case RELAY_ON:
                data.add(true);
                break;
            default:
                data.add((char *)0);
                break;
            }
        }
    }

    serializeJson(doc, *response);
    request->send(response);
}