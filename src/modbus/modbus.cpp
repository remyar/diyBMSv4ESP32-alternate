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

static uint8_t idxCtrl = 0;
static uint8_t idxCmi = 0;
//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
static e_RTU_STATE eRtuState = RTU_READ_CONTROLLER;
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

CellModuleInfo* CMI_Get(uint8_t controllerId , uint8_t cmiId){
    return &sRtuSlave[controllerId].cmi[cmiId];
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

    eRtuState = RTU_READ_CONTROLLER;
    idxCtrl = 0;
    idxCmi = 0;
    return true;
}

void MODBUS_TaskRun(void)
{
    diybms_eeprom_settings * _mySettings = SETTINGS_Get();
    switch(eRtuState){
        case RTU_READ_CONTROLLER:{
            uint16_t cmiAddress = idxCmi << 8;
            uint8_t result = sRtuSlave[idxCtrl].slave->readInputRegisters(cmiAddress | 0xFF, 23);
            if (result == sRtuSlave[idxCtrl].slave->ku8MBSuccess){
                sRtuSlave[idxCtrl].cmi[idxCmi].valid = sRtuSlave[idxCtrl].slave->getResponseBuffer(19);
                sRtuSlave[idxCtrl].cmi[idxCmi].voltagemV = sRtuSlave[idxCtrl].slave->getResponseBuffer(20);
            } else {
                Serial.print("Request Timeout");
            }
            if ( idxCmi >= _mySettings->totalNumberOfSeriesModules[idxCtrl] ){
                idxCmi = 0;
                idxCtrl++;
                if ( idxCtrl >= _mySettings->totalControllers ){
                    idxCtrl = 0;
                }
            }
            break;
        }
        default:{
            break;
        }
    }
}