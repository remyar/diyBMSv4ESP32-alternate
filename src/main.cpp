#include "./Low-Level/board.h"
#include <scheduler.h>
#include "./screen/screen.h"
#include "./Wifi/wifi.h"
#include "./sdCard/sdcard.h"
#include "./settings/settings.h"
#include "./ota/ota.h"
#include "./leds/leds.h"
#include "./rules/Rules.h"
#include "./TCA9534APWR/TCA9534APWR.h"
#include "./bms/bms.h"

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

    SCHEDULER_AddTask("TCA9534APWR" , TCA9534APWR_TaskInit, NULL, TCA9534APWR_TaskRun, 1);
    SCHEDULER_AddTask("LEDS" , LEDS_TaskInit, NULL, LEDS_TaskRun, 1);

    SCHEDULER_AddTask("SDCARD" , SDCARD_TaskInit, NULL, SDCARD_TaskRun, 15000);
    SCHEDULER_AddTask("SETTINGS" , SETTINGS_Load, NULL, NULL, 0);

    SCHEDULER_AddTask("SCREEN" , SCREEN_TaskInit, SCREEN_TaskUpdate, SCREEN_TaskRun, 1000);
    SCHEDULER_AddTask("WIFI" , WIFI_TaskInit, NULL, WIFI_TaskRun, 1);
    SCHEDULER_AddTask("OTA" , OTA_TaskInit, NULL, OTA_TaskRun, 1);
    SCHEDULER_AddTask("BMS" , BMS_TaskInit, NULL, BMS_TaskRun, 1000);
    SCHEDULER_AddTask("RULES" , RULES_TaskInit, NULL, RULES_TaskRun, 1000);

   // SCHEDULER_AddTask("CONSOLE" , CONSOLE_TaskInit , NULL ,CONSOLE_TaskRun , 1000);

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