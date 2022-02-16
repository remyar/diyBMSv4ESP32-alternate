#include "./Low-Level/board.h"
#include <scheduler.h>
#include "./modbus/modbus.h"

void setup()
{
    BOARD_Init();

    SCHEDULER_AddTask(MODBUS_TaskInit,NULL,MODBUS_TaskRun,1000);
    SCHEDULER_Init();
}

void loop()
{
    SCHEDULER_Run();
}