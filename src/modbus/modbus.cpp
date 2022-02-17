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
enum
{
    CONTOLLER_READ_VOLTAGE_AND_STATUS = 0x01,
    CONTROLLER_IDENTIFY = 0x02,
    CONTROLLER_READ_TEMPERATURE = 0x03,
    CONTROLLER_REPORT_NUMBER_OF_BAD_PACKET = 0x04,
    CONTROLLER_REPORT_CONFIGURATION = 0x05
};

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
static e_RTU_STATE eRtuState = RTU_READ_CONTROLLER_VOLTAGE_AND_STATUS;
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

CellModuleInfo *CMI_Get(uint8_t controllerId, uint8_t cmiId)
{
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

    Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);

    for (int i = 0; i < NB_MAX_SLAVES; i++)
    {
        node[i].begin(i + 1, Serial1);
        // Callbacks allow us to configure the RS485 transceiver correctly
        node[i].preTransmission(preTransmission);
        node[i].postTransmission(postTransmission);

        sRtuSlave[i].slave = &node[i];
        sRtuSlave[i].availlable = false;
    }

    eRtuState = RTU_READ_CONTROLLER_VOLTAGE_AND_STATUS;
    idxCtrl = 0;
    idxCmi = 0;
    return true;
}

void MODBUS_TaskRun(void)
{
    diybms_eeprom_settings *_mySettings = SETTINGS_Get();
    switch (eRtuState)
    {
    case RTU_READ_CONTROLLER_VOLTAGE_AND_STATUS:
    {
        uint8_t result = sRtuSlave[idxCtrl].slave->readHoldingRegisters(CONTOLLER_READ_VOLTAGE_AND_STATUS, (_mySettings->totalNumberOfBanks[idxCtrl] * 2) + 1);
        if (result == sRtuSlave[idxCtrl].slave->ku8MBSuccess)
        {
            s_GLOBAL_BMS *bms = BMS_GetBmsPtr();
            bms->isStarted = sRtuSlave[idxCtrl].slave->getResponseBuffer(0);
            uint32_t controllerVoltage = 0;
            for (int i = 0; i < _mySettings->totalNumberOfBanks[idxCtrl]; i++)
            {
                UINT32UNION_t val;
                val.number = 0;
                val.word[0] = sRtuSlave[idxCtrl].slave->getResponseBuffer((i * 2) + 1);
                val.word[1] = sRtuSlave[idxCtrl].slave->getResponseBuffer(((i * 2) + 1) + 1);

                controllerVoltage += val.number;
            }

            bms->controllerVoltage[idxCtrl] = controllerVoltage;
        }
        idxCtrl++;
        if (idxCtrl >= _mySettings->totalControllers)
        {
            idxCtrl = 0;
        }
        break;
    }
    /*case RTU_READ_CONTROLLER_VOLTAGE_AND_STATUS:{
        uint16_t cmiAddress = idxCmi << 8;
        uint8_t result = sRtuSlave[idxCtrl].slave->readInputRegisters(cmiAddress | 0xEF, 23);
        if (result == sRtuSlave[idxCtrl].slave->ku8MBSuccess){

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
    }*/
    default:
    {
        break;
    }
    }
}

void MODBUS_SendConfiguration(uint8_t idxController)
{
    ModbusMaster * node = sRtuSlave[idxController].slave;
    diybms_eeprom_settings *_mySettings = SETTINGS_Get();

    node->setTransmitBuffer(0, _mySettings->totalNumberOfBanks[idxCtrl]);

    uint8_t result = sRtuSlave[idxController].slave->writeMultipleRegisters(CONTROLLER_REPORT_CONFIGURATION, 1);
    if (result == sRtuSlave[idxCtrl].slave->ku8MBSuccess)
    {
        Serial.println("write ok");
    } else {
        Serial.println("Write false");
    }
}