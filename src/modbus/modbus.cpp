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

#include "modbus.h"

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
// instantiate ModbusMaster object
static ModbusMaster node[NB_MAX_SLAVES];

static s_RTU_SLAVE sRtuSlave[NB_MAX_SLAVES];

static uint8_t idxRtu = 0;
//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
static e_RTU_STATE eRtuState = RTU_START_SCAN;
static unsigned long _ms = millis();
//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
void preTransmission()
{
    digitalWrite(RS485_ENABLE, 1);
}

void postTransmission()
{
    digitalWrite(RS485_ENABLE, 0);
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
bool MODBUS_TaskInit(void)
{
    _ms = millis();
    pinMode(RS485_ENABLE, OUTPUT);
    digitalWrite(RS485_ENABLE, 0);

    Serial1.begin(115200,SERIAL_8N1,RS485_RX ,RS485_TX);

    for (int i = 0; i < NB_MAX_SLAVES; i++)
    {
        node[i].begin(i + 1, Serial1);
        // Callbacks allow us to configure the RS485 transceiver correctly
        node[i].preTransmission(preTransmission);
        node[i].postTransmission(postTransmission);

        sRtuSlave[i].slave = &node[i];
        sRtuSlave[i].availlable = false;
    }

    return true;
}

void MODBUS_TaskRun(void)
{
    switch (eRtuState)
    {
    case (RTU_START_SCAN):
    {
        _ms = millis();
        eRtuState = RTU_SEND_SCAN_REQUEST;
        idxRtu = 0;
        break;
    }
    case (RTU_SEND_SCAN_REQUEST):
    {
        if (idxRtu >= NB_MAX_SLAVES)
        {
            //-- scan end
            eRtuState = RTU_SCAN_FINISH;
        }
        else
        {
            Serial.println("Send Request to : " + String(idxRtu));
            uint8_t result = sRtuSlave[idxRtu].slave->readHoldingRegisters(0, 1);
            if (result == sRtuSlave[idxRtu].slave->ku8MBSuccess)
            {
                sRtuSlave[idxRtu].availlable = true;
            }
            idxRtu++;
        }
        break;
    }
    case (RTU_SCAN_FINISH):
    {
        uint8_t nbControllers = 0;
        for (int i = 0; i < NB_MAX_SLAVES; i++)
        {
            if (sRtuSlave[i].availlable == true)
            {
                nbControllers++;
            }
        }
        eRtuState = RTU_READ_CONTROLLER_VOLTAGE;
        idxRtu = 0;
        break;
    }
    case RTU_READ_CONTROLLER_VOLTAGE:
    {
        while (sRtuSlave[idxRtu].availlable == false)
        {
            idxRtu++;
            if (idxRtu >= NB_MAX_SLAVES)
            {
                idxRtu = 0;
                break;
            }
        }

        for (int i = 0; i < maximum_controller_cell_modules; i++)
        {
            uint16_t cmiAddress = i << 8;
            uint8_t result;
             result = sRtuSlave[idxRtu].slave->readInputRegisters(cmiAddress | 0xFF, 23);
            if (result == sRtuSlave[idxRtu].slave->ku8MBSuccess)
            {
                sRtuSlave[idxRtu].cmi[i].valid = sRtuSlave[idxRtu].slave->getResponseBuffer(4);
                sRtuSlave[idxRtu].cmi[i].BoardVersionNumber = sRtuSlave[idxRtu].slave->getResponseBuffer(2);
            }
            else
            {
                Serial.println("Request timeout");
            }
         /*   if ( sRtuSlave[idxRtu].cmi[i].valid == true ){
                result = sRtuSlave[idxRtu].slave->readInputRegisters(cmiAddress | 17, 1);
                if (result == sRtuSlave[idxRtu].slave->ku8MBSuccess)
                {
                    sRtuSlave[idxRtu].cmi[i].voltagemV = sRtuSlave[idxRtu].slave->getResponseBuffer(0);
                }
                else
                {
                    Serial.println("Request timeout");
                }
            }*/
        }

        Serial.println("*************");
        for ( int i = 0 ; i < maximum_controller_cell_modules ; i++ ){
            if ( sRtuSlave[idxRtu].cmi[i].valid == true ){
                Serial.println(sRtuSlave[idxRtu].cmi[i].BoardVersionNumber);
            }
        }
        Serial.println("*************");
        Serial.println();
        idxRtu++;

        break;
    }
    }
}