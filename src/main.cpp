#include "./Low-Level/board.h"
#include <scheduler.h>
#include "./screen/screen.h"
#include "./console/console.h"

/*#include "./Wifi/wifi.h"
#include "./keyboard/keyboard.h"
#include "./sdCard/sdcard.h"
#include "./memory/memory.h"
#include "./settings/settings.h"
#include "./screen/screen.h"
#include "./bms/bms.h"
#include "./rules/Rules.h"
#include "./console/console.h"
*/
void setup()
{
    BOARD_Init();

    SCHEDULER_AddTask(SCREEN_TaskInit , NULL ,SCREEN_TaskRun , 1000);
    SCHEDULER_AddTask(CONSOLE_TaskInit , NULL ,CONSOLE_TaskRun , 1);

    SCHEDULER_Init();
    /*
        MEM_TaskInit();

        SDCARD_TaskInit();
        SETTINGS_Load();
        SCREEN_TaskInit();

        WIFI_TaskInit();
        KEYBOARD_TaskInit();

        BMS_TaskInit();
        RULES_TaskInit();

        CONSOLE_TaskInit();*/
}

void loop()
{
    SCHEDULER_Run();

    // put your main code here, to run repeatedly:
    /*  WIFI_TaskRun();
      KEYBOARD_TaskRun();
      SCREEN_TaskRun();
      BMS_TaskRun();
      RULES_TaskRun();
      CONSOLE_TaskRun();*/
}