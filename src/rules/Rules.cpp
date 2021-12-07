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
#include "./Rules.h"
#include "../bms/bms.h"
#include "../settings/settings.h"

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
static Rules rules;
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
void ClearValues(void){

    for (uint8_t r = 0; r < maximum_number_of_banks; r++)
    {
        rules.packvoltage[r] = 0;
        rules.lowestvoltageinpack[r] = 0xFFFF;
        rules.highestvoltageinpack[r] = 0;
    }

    rules.highestPackVoltage = 0;
    rules.lowestPackVoltage = 0xFFFFFFFF;
    rules.highestCellVoltage = 0;
    rules.lowestCellVoltage = 0xFFFF;
    rules.highestExternalTemp = -127;
    rules.lowestExternalTemp = 127;
    rules.highestInternalTemp = -127;
    rules.lowestInternalTemp = 127;
    rules.zeroVoltageModuleCount = 0;
    rules.invalidModuleCount = 0;
    rules.moduleHasExternalTempSensor = false;

    rules.address_LowestCellVoltage = maximum_controller_cell_modules + 1;
    rules.address_lowestExternalTemp = maximum_controller_cell_modules + 1;
    rules.address_highestExternalTemp = maximum_controller_cell_modules + 1;
    rules.address_HighestCellVoltage = maximum_controller_cell_modules + 1;
}

//Looking at individual voltages and temperatures and sum up pack voltages.
void ProcessCell(uint8_t bank, uint8_t cellNumber, CellModuleInfo *c)
{
    if (c->valid == false)
    {
        rules.invalidModuleCount++;
        return;
    }

    rules.packvoltage[bank] += c->voltagemV;

    //If the voltage of the module is zero, we probably haven't requested it yet (which happens during power up)
    //so keep count so we don't accidentally trigger rules.
    if (c->voltagemV == 0)
    {
        rules.zeroVoltageModuleCount++;
    }

    if (c->voltagemV > rules.highestvoltageinpack[bank])
    {
        rules.highestvoltageinpack[bank] = c->voltagemV;
    }
    if (c->voltagemV < rules.lowestvoltageinpack[bank])
    {
        rules.lowestvoltageinpack[bank] = c->voltagemV;
    }

    if (c->voltagemV > rules.highestCellVoltage)
    {
        rules.highestCellVoltage = c->voltagemV;
        rules.address_HighestCellVoltage = cellNumber;
    }

    if (c->voltagemV < rules.lowestCellVoltage)
    {
        rules.lowestCellVoltage = c->voltagemV;
        rules.address_LowestCellVoltage = cellNumber;
    }

    if (c->externalTemp != -40)
    {
        //Record that we do have at least one external temperature sensor on a module
        rules.moduleHasExternalTempSensor = true;

        if (c->externalTemp > rules.highestExternalTemp)
        {
            rules.highestExternalTemp = c->externalTemp;
            rules.address_highestExternalTemp = cellNumber;
        }

        if (c->externalTemp < rules.lowestExternalTemp)
        {
            rules.lowestExternalTemp = c->externalTemp;
            rules.address_lowestExternalTemp = cellNumber;
        }
    }

    if (c->internalTemp > rules.highestInternalTemp)
    {
        rules.highestInternalTemp = c->internalTemp;
    }

    if (c->externalTemp < rules.lowestInternalTemp)
    {
        rules.lowestInternalTemp = c->internalTemp;
    }
}

void ProcessBank(uint8_t bank)
{
    //Combine the voltages - work out the highest and lowest pack voltages
    if (rules.packvoltage[bank] > rules.highestPackVoltage)
    {
        rules.highestPackVoltage = rules.packvoltage[bank];
    }
    if (rules.packvoltage[bank] < rules.lowestPackVoltage)
    {
        rules.lowestPackVoltage = rules.packvoltage[bank];
    }
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void RULES_TaskInit(void)
{
    _ms = millis();
}

void RULES_TaskRun(void)
{
    if ((millis() - _ms) < 1000)
    {
        return;
    }

    ClearValues();

    rules.rule_outcome[BMSError] = false;

    diybms_eeprom_settings *mysettings = SETTINGS_Get();
    uint16_t totalConfiguredModules = TotalNumberOfCells();

      if (totalConfiguredModules > maximum_controller_cell_modules)
    {
        //System is configured with more than maximum modules - abort!
    }

 /*   if (receiveProc.totalModulesFound > 0 && receiveProc.totalModulesFound != totalConfiguredModules)
    {
        //Found more or less modules than configured for
    }*/

    rules.numberOfBalancingModules = 0;
    uint8_t cellid = 0;
    for (int8_t bank = 0; bank < mysettings->totalNumberOfBanks; bank++)
    {
        for (int8_t i = 0; i < mysettings->totalNumberOfSeriesModules; i++)
        {
            CellModuleInfo *cmi = BMS_GetCMI(cellid);
            ProcessCell(bank, cellid, cmi);

            if (cmi->valid && cmi->settingsCached)
            {
                if (cmi->BypassThresholdmV != mysettings->BypassThresholdmV)
                {
                }
                if (cmi->BypassOverTempShutdown != mysettings->BypassOverTempShutdown)
                {
                }
                if (cmi->inBypass)
                {
                    rules.numberOfBalancingModules++;
                }
                if (BMS_GetCMI(0)->settingsCached && cmi->CodeVersionNumber != BMS_GetCMI(0)->CodeVersionNumber)
                {
                    //Do all the modules have the same version of code as module zero?
                }
                if (BMS_GetCMI(0)->settingsCached && cmi->BoardVersionNumber != BMS_GetCMI(0)->BoardVersionNumber)
                {
                    //Do all the modules have the same hardware revision?
                }
            }

            cellid++;
        }

        ProcessBank(bank);
    }
    _ms = millis();
}

Rules *RULES_Get(void)
{
    return &rules;
}