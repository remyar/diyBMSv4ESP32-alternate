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

#include "./wifi.h"
#include "../sdCard/sdcard.h"
#include "../keyboard/keyboard.h"
#include "../http_serveur/SoftAP.h"
#include "../http_serveur/diyBms_server.h"
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
static e_WIFI_STATE eWifiState = WIFI_CONNECT;
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

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
bool WIFI_TaskInit(void)
{
    WIFI_Init();
    _ms = millis();
    return true;
}

e_WIFI_STATE WIFI_GetState(void)
{
    return eWifiState;
}

void WIFI_TaskRun(void)
{
    diybms_eeprom_settings *settings = SETTINGS_Get();

    if (KEYBOARD_IsPushed() && KEYBOARD_IsLongPush())
    {
        eWifiState = WIFI_WPS_START;
    }

    switch (eWifiState)
    {
    case (WIFI_CONNECT):
    {
        
        if (settings->wifi_ssid.length() > 0)
        {
            WIFI_Connect(settings->wifi_ssid, settings->wifi_psk);
            _ms = millis();
            eWifiState = WIFI_CONNECT_IN_PROGRESS;
            if (DEBUG_WIFI)
            {
                Serial.println("WIFI_CONNECT_IN_PROGRESS");
            }
        }
        else
        {
            eWifiState = WIFI_AP_MODE_START;
        }
        _ms = millis();
        break;
    }
    case (WIFI_CONNECT_IN_PROGRESS):
    {
        if (WIFI_IsConnected())
        {
            WIFI_PrintMyIP();
            WIFI_PrintServerIP();
            eWifiState = WIFI_CONNECT_SUCCESS;
            configTime(0/*mysettings.timeZone * 3600 + mysettings.minutesTimeZone * 60*/, 0/*mysettings.daylight * 3600*/, "time.google.com" /*mysettings.ntpServer*/);
            if (DEBUG_WIFI)
            {
                Serial.println("WIFI_CONNECT_SUCCESS");
            }
            DIYBMSServer_Begin();
        }
        else if (WIFI_IsConnectedFailed() || ((millis() - _ms) >= 60000))
        {
            eWifiState = WIFI_CONNECT;
            if (DEBUG_WIFI)
            {
                Serial.println("WIFI_CONNECT_FAILED : Retry");
            }
        }
        break;
    }
    case (WIFI_CONNECT_SUCCESS):
    {
        if (WIFI_IsDisconnected())
        {
            eWifiState = WIFI_CONNECT;
            if (DEBUG_WIFI)
            {
                Serial.println("WIFI_DISCONNECTED");
            }
        }
        break;
    }
    case (WIFI_WPS_START):
    {
        GPIO_BUILTIN_LED(LOW);
        if (WIFI_WPSProcess() == false)
        {
            eWifiState = WIFI_AP_MODE_START;
        }
        else
        {
            Serial.println("save setting");
            settings->wifi_ssid = WIFI_GetActualSSID();
            settings->wifi_psk = WIFI_GetActualPSK();
            SETTINGS_Save();
            eWifiState = WIFI_CONNECT;
        }
        break;
    }
    case (WIFI_AP_MODE_START):
    {
        WIFI_APSetup();
        SOFTAP_Begin();
        eWifiState = WIFI_AP_MODE_PROCESS;
        break;
    }
    case (WIFI_AP_MODE_PROCESS):
    {
        break;
    }
    }
}
