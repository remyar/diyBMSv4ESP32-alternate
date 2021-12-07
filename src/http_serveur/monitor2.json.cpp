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

#include "./monitor2.json.h"
#include "../bms/bms.h"
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
void MONITOR2_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();
    PacketRequestGenerator * _prg = BMS_GetPrg();
    PacketReceiveProcessor * _receiveProc = BMS_GetReceiveProc();
    Rules* _rules = RULES_Get();

    uint8_t totalModules = _mysettings->totalNumberOfBanks * _mysettings->totalNumberOfSeriesModules;

    const char comma = ',';
    const char *null = "null";

    AsyncResponseStream *response = request->beginResponseStream("application/json");

    response->print("{");

    PrintStreamComma(response, "\"banks\":", _mysettings->totalNumberOfBanks);
    PrintStreamComma(response, "\"seriesmodules\":", _mysettings->totalNumberOfSeriesModules);

    PrintStreamComma(response, "\"sent\":", _prg->packetsGenerated);
    PrintStreamComma(response, "\"received\":", _receiveProc->packetsReceived);
    PrintStreamComma(response, "\"modulesfnd\":", _receiveProc->totalModulesFound);
    PrintStreamComma(response, "\"badcrc\":", _receiveProc->totalCRCErrors);
    PrintStreamComma(response, "\"ignored\":", _receiveProc->totalNotProcessedErrors);
    PrintStreamComma(response, "\"roundtrip\":", _receiveProc->packetTimerMillisecond);
    PrintStreamComma(response, "\"oos\":", _receiveProc->totalOutofSequenceErrors);

    PrintStreamComma(response, "\"activerules\":", _rules->active_rule_count);
    PrintStreamComma(response, "\"uptime\":", (uint32_t)(esp_timer_get_time() / (uint64_t)1e+6));

/*
    PrintStreamComma(response, "\"can_fail\":", canbus_messages_failed_sent);
    PrintStreamComma(response, "\"can_sent\":", canbus_messages_sent);
    PrintStreamComma(response, "\"can_rec\":", canbus_messages_received);
*/
    //Output last 2 charaters from security cookie, to allow browser to detect when its
    //no longer in sync with the back end and report warning.
    //Technically this downgrades the complexity of the XSS key, as it reduces key length.
 /*   response->print("\"sec\":\"");
    response->print(UUIDStringLast2Chars);
    response->print("\",");
*/
    response->print(F("\"errors\":["));
    uint8_t count = 0;

    for (size_t i = 0; i < sizeof(_rules->ErrorCodes); i++)
    {
        if (_rules->ErrorCodes[i] != InternalErrorCode::NoError)
        {
            //Comma if not zero
            if (count)
            {
                response->print(comma);
            }

            response->print(_rules->ErrorCodes[i]);
            count++;
        }
    }
    response->print("],");

    response->print(F("\"warnings\":["));
    count = 0;
    for (size_t i = 0; i < sizeof(_rules->WarningCodes); i++)
    {
        if (_rules->WarningCodes[i] != InternalWarningCode::NoWarning)
        {
            //Comma if not zero
            if (count)
            {
                response->print(comma);
            }

            response->print(_rules->WarningCodes[i]);
            count++;
        }
    }
    response->print("],");

    //voltages
    response->print(F("\"voltages\":["));

    for (uint8_t i = 0; i < totalModules; i++)
    {
        //Comma if not zero
        if (i)
            response->print(comma);

        if (BMS_GetCMI(i)->valid)
        {
            response->print(BMS_GetCMI(i)->voltagemV);
        }
        else
        {
            //Module is not yet valid so return null values...
            response->print(null);
        }
    }
    response->print("],");

    response->print(F("\"minvoltages\":["));

    for (uint8_t i = 0; i < totalModules; i++)
    {
        //Comma if not zero
        if (i)
            response->print(comma);

        if (BMS_GetCMI(i)->valid)
        {
            response->print(BMS_GetCMI(i)->voltagemVMin);
        }
        else
        {
            //Module is not yet valid so return null values...
            response->print(null);
        }
    }
    response->print("],");

    //maxvoltages

    response->print(F("\"maxvoltages\":["));

    for (uint8_t i = 0; i < totalModules; i++)
    {
        //Comma if not zero
        if (i)
            response->print(comma);

        if (BMS_GetCMI(i)->valid)
        {
            response->print(BMS_GetCMI(i)->voltagemVMax);
        }
        else
        {
            //Module is not yet valid so return null values...
            response->print(null);
        }
    }
    response->print("]");

    response->print(comma);

    //inttemp
    response->print(F("\"inttemp\":["));

    for (uint8_t i = 0; i < totalModules; i++)
    {
        //Comma if not zero
        if (i)
            response->print(comma);

        if (BMS_GetCMI(i)->valid && BMS_GetCMI(i)->internalTemp != -40)
        {
            response->print(BMS_GetCMI(i)->internalTemp);
        }
        else
        {
            //Module is not yet valid so return null values...
            response->print(null);
        }
    }
    response->print("]");

    response->print(comma);

    //exttemp
    response->print(F("\"exttemp\":["));

    for (uint8_t i = 0; i < totalModules; i++)
    {
        //Comma if not zero
        if (i)
            response->print(comma);

        if (BMS_GetCMI(i)->valid && BMS_GetCMI(i)->externalTemp != -40)
        {
            response->print(BMS_GetCMI(i)->externalTemp);
        }
        else
        {
            //Module is not yet valid so return null values...
            response->print(null);
        }
    }
    response->print(']');

    response->print(comma);

    //bypass
    response->print(F("\"bypass\":["));

    for (uint8_t i = 0; i < totalModules; i++)
    {
        //Comma if not zero
        if (i)
            response->print(comma);

        if (BMS_GetCMI(i)->valid && BMS_GetCMI(i)->inBypass)
        {
            response->print('1');
        }
        else
        {
            response->print('0');
        }
    }
    response->print("]");
    response->print(comma);

    //bypasshot
  response->print(F("\"bypasshot\":["));

  for (uint8_t i = 0; i < totalModules; i++)
  {
    //Comma if not zero
    if (i)
      response->print(comma);

    if (BMS_GetCMI(i)->valid && BMS_GetCMI(i)->bypassOverTemp)
    {
      response->print('1');
    }
    else
    {
      response->print('0');
    }
  }
  response->print(']');

  response->print(comma);

  //bypasspwm
  response->print(F("\"bypasspwm\":["));

  for (uint8_t i = 0; i < totalModules; i++)
  {
    //Comma if not zero
    if (i)
      response->print(comma);

    if (BMS_GetCMI(i)->valid && BMS_GetCMI(i)->inBypass)
    {
      response->print(BMS_GetCMI(i)->PWMValue);
    }
    else
    {
      response->print('0');
    }
  }
  response->print(']');

/*  response->print(comma);


  //bypasspwm
  response->print(F("\"bankv\":["));

  for (uint8_t i = 0; i < _mysettings->totalNumberOfBanks; i++)
  {
    //Comma if not zero
    if (i)
      response->print(comma);

    response->print(_rules->packvoltage[i]);
  }
  response->print("]");

  response->print(comma);

  //bypasspwm
  response->print(F("\"voltrange\":["));

  for (uint8_t i = 0; i < _mysettings->totalNumberOfBanks; i++)
  {
    //Comma if not zero
    if (i)
      response->print(comma);

    response->print(_rules->VoltageRangeInBank(i));
  }
  response->print("]");

  response->print(comma);
  response->print(F("\"current\":["));
  if (_mysettings->currentMonitoringEnabled && currentMonitor.validReadings)
  {
    //Output current monitor values, this is inside an array, so could be more than 1
    response->print(F("{\"c\":"));
    response->print(currentMonitor.modbus.current, 4);
    response->print(F(",\"v\":"));
    response->print(currentMonitor.modbus.voltage, 4);
    response->print(F(",\"mahout\":"));
    response->print(currentMonitor.modbus.milliamphour_out);
    response->print(F(",\"mahin\":"));
    response->print(currentMonitor.modbus.milliamphour_in);
    response->print(F(",\"p\":"));
    response->print(currentMonitor.modbus.power, 2);
    response->print(F(",\"soc\":"));
    response->print(currentMonitor.stateofcharge, 2);

    response->print("}");
  }
  else
  {
    response->print(null);
  }

  response->print("]");
*/
    response->print("}");

    request->send(response);
}