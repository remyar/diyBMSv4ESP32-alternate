#include "./Low-Level/board.h"
#include <scheduler.h>
#include "./screen/screen.h"
#include "./keyboard/keyboard.h"
#include "./Wifi/wifi.h"
#include "./sdCard/sdcard.h"
#include "./settings/settings.h"
#include "./ota/ota.h"
#include "./bms/bms.h"
#include "./leds/leds.h"
#include "./TCA9534APWR/TCA9534APWR.h"
#include "./modbus/modbus.h"

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

    SCHEDULER_AddTask("MODBUS" , MODBUS_TaskInit, NULL, MODBUS_TaskRun, 100);
    SCHEDULER_AddTask("KEYBOARD" , KEYBOARD_TaskInit, NULL, KEYBOARD_TaskRun, 100);
    SCHEDULER_AddTask("BMS" , BMS_TaskInit, NULL, BMS_TaskRun, 100);
    
    SCHEDULER_Init();
}

void loop()
{
    SCHEDULER_Run();
}