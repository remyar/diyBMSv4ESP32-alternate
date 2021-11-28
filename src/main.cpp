#include "./Low-Level/board.h"
#include "./Wifi/wifi.h"
#include "./keyboard/keyboard.h"
#include "./sdCard/sdcard.h"
#include "./memory/memory.h"
#include "./settings/settings.h"
#include "./screen/screen.h"
#include "./bms/bms.h"
#include "./rules/Rules.h"

void setup()
{
    BOARD_Init();

    MEM_TaskInit();

    SDCARD_TaskInit();
    SETTINGS_Load();
    SCREEN_TaskInit();

    WIFI_TaskInit();
    KEYBOARD_TaskInit();

    BMS_TaskInit();
    RULES_TaskInit();

}

void loop()
{
    // put your main code here, to run repeatedly:
    WIFI_TaskRun();
    KEYBOARD_TaskRun();
    SCREEN_TaskRun();
    BMS_TaskRun();
    RULES_TaskRun();
}