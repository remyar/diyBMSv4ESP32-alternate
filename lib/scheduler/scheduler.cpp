//=============================================================================
//
// PROJET       :
// MODULE       : Scheduler.c
// DESCRIPTION  : Gestion des taches
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Fichiers Inclus
//-----------------------------------------------------------------------------
#include "Scheduler.h"


//-----------------------------------------------------------------------------
// Constantes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables globales
//-----------------------------------------------------------------------------
uint8_t nbTask = 0;
s_TASK tasks[MAX_TASKS];

//-----------------------------------------------------------------------------
// FONCTION    : SCHEDULER_Init
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void SCHEDULER_Init(void)
{
    uint8_t i;

    for (i = 0; i < nbTask; i++)
        EVENT_None(i);

    for (i = 0; i < nbTask; i++)
    {
        if (tasks[i].init != NULL)
        {
            if (TASK_Init(&tasks[i]) == true)
                tasks[i].status = READY_STATUS;
            else
                tasks[i].status = WAITING_STATUS;
        }
        else
            tasks[i].status = WAITING_STATUS;
    }
}

//-----------------------------------------------------------------------------
// FONCTION    : SCHEDULER_Update
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void SCHEDULER_Update(void)
{
    uint8_t i;

    for (i = 0; i < nbTask; i++)
    {
        //--- Mise a jour des compteurs si la tache est active
        if (tasks[i].status == READY_STATUS)
        {
            if (tasks[i].counter > 0)
                tasks[i].counter--;
        }
    }
}

//-----------------------------------------------------------------------------
// FONCTION    : SCHEDULER_Run
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
void SCHEDULER_Run(void)
{
    uint8_t idxActiveTasks[MAX_TASKS];
    uint8_t nbActive;
    uint8_t i;
    uint32_t tickStart;
    uint32_t taskLen;

    SCHEDULER_Update();

    //--- Mise a jour des evenements
    for (i = 0; i < nbTask; i++)
    {
        if (tasks[i].init != NULL)
        {
            //--- Les taches ayant un evenement en cours
            if (tasks[i].event.type != NO_EVENT){
                if ( tasks[i].update != NULL ){
                    TASK_Update(i);
                }
            }
        }
    }

    //--- Mise a jour des tasks actives
    nbActive = 0;
    for (i = 0; i < nbTask; i++)
    {
        if (tasks[i].init != NULL)
        {
            //--- Les taches ayant fini de s'executer redeviennent pretes
            if (tasks[i].status == END_RUN_STATUS && tasks[i].counter == 0)
            {
                TASK_SetActive(i, false);
                tasks[i].status = READY_STATUS;
            }

            //--- Les taches pretes avec un timer a 0 deviennent actives
            if (tasks[i].status == READY_STATUS && tasks[i].counter == 0)
                idxActiveTasks[nbActive++] = i;
        }
    }

    //--- Execution des taches
    for (i = 0; i < nbActive; i++)
    {
        tasks[idxActiveTasks[i]].status = RUNNING_STATUS;

        tickStart = millis();

        if ( tasks[idxActiveTasks[i]].run != NULL){
            tasks[idxActiveTasks[i]].run();
        }

        taskLen = millis() - tickStart;

        //--- LOG_NB_TASK_TIMES derniers temps
        tasks[idxActiveTasks[i]].taskLength = taskLen;

        //--- temps min
        if (taskLen < tasks[idxActiveTasks[i]].minTaskLength)
            tasks[idxActiveTasks[i]].minTaskLength = taskLen;

        //--- temps max
        if (taskLen > tasks[idxActiveTasks[i]].maxTaskLength)
            tasks[idxActiveTasks[i]].maxTaskLength = taskLen;

        tasks[idxActiveTasks[i]].status = END_RUN_STATUS;
    }
}

void SCHEDULER_AddTask(PTR_TASK_INIT_FUNC initFunc , PTR_TASK_UPDATE_FUNC updFunc , PTR_TASK_RUN_FUNC runFunc , uint16_t period)
{
    s_TASK *t = &tasks[nbTask];
    t->idx = nbTask;
    t->init = initFunc;
    t->update = updFunc;
    t->run = runFunc;
    t->period = period;

    nbTask++;
    if ( nbTask >= MAX_TASKS ){
        while (1){ Serial.print("olalalalalalalaalal");}
    }
}