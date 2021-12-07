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

#include "./monitor3.json.h"
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
void MONITOR3_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();
    PacketRequestGenerator *_prg = BMS_GetPrg();
    PacketReceiveProcessor *_receiveProc = BMS_GetReceiveProc();
    Rules *_rules = RULES_Get();

    // DynamicJsonDocument doc(maximum_controller_cell_modules * 50);
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    uint8_t totalModules = _mysettings->totalNumberOfBanks * _mysettings->totalNumberOfSeriesModules;
    uint8_t comma = totalModules - 1;

    response->print("{\"badpacket\":[");

    for (uint8_t i = 0; i < totalModules; i++)
    {
        if (cmi[i].valid)
        {
            response->print(cmi[i].badPacketCount);
        }
        else
        {
            // Return NULL
            response->print("null");
        }
        if (i < comma)
        {
            response->print(',');
        }
    }

    response->print("],\"balcurrent\":[");

    for (uint8_t i = 0; i < totalModules; i++)
    {
        if (cmi[i].valid)
        {
            response->print(cmi[i].BalanceCurrentCount);
        }
        else
        {
            // Return NULL
            response->print("null");
        }
        if (i < comma)
        {
            response->print(',');
        }
    }

    response->print("],\"pktrecvd\":[");

    for (uint8_t i = 0; i < totalModules; i++)
    {
        if (cmi[i].valid)
        {
            response->print(cmi[i].PacketReceivedCount);
        }
        else
        {
            // Return NULL
            response->print("null");
        }
        if (i < comma)
        {
            response->print(',');
        }
    }
    response->print("]}");

    response->addHeader("Cache-Control", "no-store");
    request->send(response);
}