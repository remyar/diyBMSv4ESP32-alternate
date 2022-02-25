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
    s_GLOBAL_BMS * bms = BMS_GetBmsPtr();
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

    for (int i = 0; i < _mysettings->totalControllers; i++)
    {
        JsonArray defaultArray = root.createNestedArray("relaydefault_" + String(i));
        for (uint8_t relay = 0; relay < RELAY_TOTAL; relay++)
        {
            switch (_mysettings->rulerelaydefault[i][relay])
            {
            case RELAY_ON:
                defaultArray.add(true);
                break;
            default:
                defaultArray.add(false);
                break;
            }
        }

        /*    JsonArray typeArray = root.createNestedArray("relaytype_" + String(i));
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
            }*/

        JsonArray bankArray = root.createNestedArray("rules_" + String(i));

        for (uint8_t r = 0; r < RELAY_RULES; r++)
        {
            JsonObject rule = bankArray.createNestedObject();
            rule["value"] = _mysettings->rulevalue[i][r];
            rule["hysteresis"] = _mysettings->rulehysteresis[i][r];
            rule["triggered"] = bms->rule_outcome[i][r];
            JsonArray data = rule.createNestedArray("relays");

            for (uint8_t relay = 0; relay < RELAY_TOTAL; relay++)
            {
                switch (_mysettings->rulerelaystate[i][r][relay])
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
    }
    serializeJson(doc, *response);
    request->send(response);
}

void RULES_Save(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();

    for (int c = 0; c < _mysettings->totalControllers; c++)
    {
        for (int rule = 0; rule < RELAY_RULES; rule++)
        {
            // TODO: This STRING doesnt work properly if its on a single line!
            String name = "rule_";
            name += String(c);
            name += "_";
            name += String(rule);
            name += "_value";

            if (request->hasParam(name, true))
            {
                AsyncWebParameter *p1 = request->getParam(name, true);
                _mysettings->rulevalue[c][rule] = p1->value().toInt();
            }

            // TODO: This STRING doesnt work properly if its on a single line!
            String hname = "rule_";
            hname += String(c);
            hname += "_";
            hname += String(rule);
            hname = hname + "_hysteresis";

            if (request->hasParam(hname, true))
            {
                AsyncWebParameter *p1 = request->getParam(hname, true);
                _mysettings->rulehysteresis[c][rule] = p1->value().toInt();
            }

            // Rule/relay processing
            for (int i = 0; i < RELAY_TOTAL; i++)
            {
                // TODO: This STRING doesnt work properly if its on a single line!
                String _name = "rule_";
                _name += String(c);
                _name += "_";
                _name += String(rule);
                _name += "_relay_";
                _name += String(i + 1);

                if (request->hasParam(_name, true))
                {
                    AsyncWebParameter *p1 = request->getParam(_name, true);
                    _mysettings->rulerelaystate[c][rule][i] = p1->value().equals("X") ? RELAY_X : p1->value().equals("On") ? RelayState::RELAY_ON
                                                                                                                           : RelayState::RELAY_OFF;
                }
            }

            // Reset state of rules after updating the new values
            for (int8_t r = 0; r < RELAY_RULES; r++)
            {
                // _rules->rule_outcome[r] = false;
            }
        }

        for (int relay = 0; relay < RELAY_TOTAL; relay++)
        {
            String _name = "relaydefault_";
            _name += String(c);
            _name += "_";
            _name += String(relay);

            if (request->hasParam(_name, true))
            {
                AsyncWebParameter *p1 = request->getParam(_name, true);
                _mysettings->rulerelaydefault[c][relay] = p1->value().equals("On") ? RelayState::RELAY_ON : RelayState::RELAY_OFF;
            }
        }
    }

    MODBUS_SendRules();

    SETTINGS_Save();

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    StaticJsonDocument<100> doc;
    doc["success"] = true;
    serializeJson(doc, *response);
    request->send(response);
}