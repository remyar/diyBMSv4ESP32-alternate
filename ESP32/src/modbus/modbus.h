//================================================================================================//
//                                                                                                //
// PROJET       : DBM620                                                                          //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 18/12/2017                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

#ifndef _MODBUS_H_
#define _MODBUS_H_

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include <ModbusMaster.h>
#include "../Low-Level/board.h"
#include "../typedefs.h"
#include "../settings/settings.h"
#include "../bms/bms.h"

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//
#define NB_MAX_SLAVES   8

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//
typedef enum {
    RTU_READ_CONTROLLER_VOLTAGE_AND_STATUS = 0,
    RTU_READ_CMI_INFORMATION,
    RTU_SEND_RULES_INFORMATION,
    RTU_SEND_RELAY_INFORMATION,
    RTU_SEND_RELAY_DEFAULT_INFORMATION,
    RTU_MAX
}e_RTU_STATE;

//================================================================================================//
//                                      STRUCTURES ET UNIONS                                      //
//================================================================================================//

typedef struct {
    ModbusMaster * slave;
    bool    availlable;
    CellModuleInfo cmi[maximum_controller_cell_modules];
}s_RTU_SLAVE;

//================================================================================================//
//                                VARIABLES ET FONCTION PARTAGEES                                 //
//================================================================================================//

//------------------------------------------------------------------------------------------------//
//---                                        Variables                                         ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Fonctions                                         ---//
//------------------------------------------------------------------------------------------------//
bool MODBUS_TaskInit(void);
void MODBUS_TaskRun(void);
void MODBUS_SendConfiguration(uint8_t idxController);
void MODBUS_SendIdentifyModule(uint8_t idxController , uint8_t idxModule);
void MODBUS_SendGlobalSettings(uint8_t idxController);
void MODBUS_SendRules(void);

CellModuleInfo* CMI_Get(uint8_t controllerId , uint8_t cmiId);

#endif //--- _BOARD_
