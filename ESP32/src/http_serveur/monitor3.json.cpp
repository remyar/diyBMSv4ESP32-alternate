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
    /*PacketRequestGenerator *_prg = BMS_GetPrg();
    PacketReceiveProcessor *_receiveProc = BMS_GetReceiveProc();
    Rules *_rules = RULES_Get();*/

    // DynamicJsonDocument doc(maximum_controller_cell_modules * 50);
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    const char comma = ',';
    const char *null = "null";

    response->print("{");
    response->print(F("\"controllers\":["));
    for (int i = 0; i < _mysettings->totalControllers; i++)
    {
        uint8_t totalModules = _mysettings->totalNumberOfBanks[i] * _mysettings->totalNumberOfSeriesModules[i];

        // Comma if not zero
        if (i)
            response->print(comma);

        response->print("{");
        response->print("\"badpacket\":[");
        for (uint8_t j = 0; j < totalModules; j++)
        {
            if (j)
                response->print(comma);

            if (CMI_Get(i, j)->valid)
            {
                response->print(CMI_Get(i, j)->badPacketCount);
            }
            else
            {
                // Module is not yet valid so return null values...
                response->print(null);
            }
        }
        response->print("]");
        response->print(comma);

        response->print("\"pktrecvd\":[");

        for (uint8_t j = 0; j < totalModules; j++)
        {
            if (j)
                response->print(comma);

            if (CMI_Get(i, j)->valid)
            {
                response->print(CMI_Get(i, j)->PacketReceivedCount);
            }
            else
            {
                // Return NULL
                response->print("null");
            }
        }
        response->print("]");
        response->print(comma);

        response->print("\"balcurrent\":[");

        for (uint8_t j = 0; j < totalModules; j++)
        {
            if (j)
                response->print(comma);
                
            if (CMI_Get(i, j)->valid)
            {
                response->print(CMI_Get(i, j)->BalanceCurrentCount);
            }
            else
            {
                // Return NULL
                response->print("null");
            }
        }

        response->print("]");

        response->print("}");
    }
    response->print("]");
    response->print("}");
    response->addHeader("Cache-Control", "no-store");
    request->send(response);
    /*
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
        request->send(response);*/
}