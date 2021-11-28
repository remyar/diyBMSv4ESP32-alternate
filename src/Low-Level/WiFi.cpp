//================================================================================================//
//                                                                                                //
// PROJET       : DongleWifi GoodRace                                                             //
// MODULE       : WiFi                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 27/01/2020                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include "./WiFi.h"
#include <EEPROM.h>

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

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
static esp_wps_config_t config;
static bool wpsStatus = 0;
static bool wpsSuccess = 0;
static String newSSIDTemp = "";
#define ESP_MANUFACTURER "ESPRESSIF"
#define ESP_MODEL_NUMBER "ESP32"
#define ESP_MODEL_NAME "ESPRESSIF IOT"
#define ESP_DEVICE_NAME "ESP STATION"

void wpsInitConfig()
{
    config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
    config.wps_type = WPS_TYPE_PBC;
    strcpy(config.factory_info.manufacturer, ESP_MANUFACTURER);
    strcpy(config.factory_info.model_number, ESP_MODEL_NUMBER);
    strcpy(config.factory_info.model_name, ESP_MODEL_NAME);
    strcpy(config.factory_info.device_name, ESP_DEVICE_NAME);
}

String wpspin2string(uint8_t a[])
{
    char wps_pin[9];
    for (int i = 0; i < 8; i++)
    {
        wps_pin[i] = a[i];
    }
    wps_pin[8] = '\0';
    return (String)wps_pin;
}
//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : WIFI_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void WIFI_Init()
{
    //WiFi.mode(WIFI_STA);
}

void WiFiEvent(WiFiEvent_t event, system_event_info_t info)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_START:
        Serial.println("Station Mode Started");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("Connected to :" + String(WiFi.SSID()));
        Serial.print("Got IP: ");
        Serial.println(WiFi.localIP());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("Disconnected from station, attempting reconnection");
        // WiFi.reconnect();
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        Serial.println("WPS Successfull, stopping WPS and connecting to: " + String(WiFi.SSID()));
        esp_wifi_wps_disable();
        WiFi.reconnect();
        wpsSuccess = 1;
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        Serial.println("WPS Failed");
        esp_wifi_wps_disable();
        wpsStatus = 1;
        wpsSuccess = 0;
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        Serial.println("WPS Timedout");
        esp_wifi_wps_disable();
        wpsStatus = 1;
        wpsSuccess = 0;
        break;
        /*  case SYSTEM_EVENT_STA_WPS_ER_PIN:
        Serial.println("WPS_PIN = " + wpspin2string(info.sta_er_pin.pin_code));
        break;*/
    default:
        break;
    }
}

void WIFI_APSetup(void)
{
    if (DEBUG_WIFI)
    {
        Serial.println("Create AP for configure it");
    }

    String ssid = "DIY_BMS_CONTROLLER";

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    delay(100);

    WiFi.scanNetworks();
    /*  int n = WiFi.scanNetworks();
    if (n == 0)
        String networks = "no networks found";
    else
    {
        String networks = "";
        for (int i = 0; i < n; ++i)
        {
            networks += "<option>" + WiFi.SSID(i) + "</option>";
            if (DEBUG_WIFI)
            {
                Serial.println(WiFi.SSID(i).c_str());
            }
        }
    }*/

    WiFi.mode(WIFI_OFF);
    delay(100);
    WiFi.mode(WIFI_AP);
    delay(100);
    WiFi.softAP(ssid.c_str());

    IPAddress IP = WiFi.softAPIP();
    if (DEBUG_WIFI)
    {
        Serial.print("Access point IP address : ");
        Serial.println(IP.toString().c_str());
    }
}

bool WIFI_WPSProcess(void)
{
    newSSIDTemp = "";
    if (DEBUG_WIFI)
    {
        Serial.println("WIFI_WPSProcess");
    }

    WiFi.disconnect();
    delay(100);
    WiFi.onEvent(WiFiEvent);
    WiFi.mode(WIFI_MODE_STA);

    wpsSuccess = 0;
    wpsInitConfig();
    esp_wifi_wps_enable(&config);
    esp_wifi_wps_start(0);

    while (WIFI_IsDisconnected() && (wpsStatus == 0))
    {
        GPIO_BUILTIN_LED_ON();
        delay(100);
        GPIO_BUILTIN_LED_OFF();
        delay(100);
    }

    if (wpsSuccess)
    {
        String newSSID = WiFi.SSID();
        newSSIDTemp = newSSID;
        if (DEBUG_WIFI)
        {
            Serial.println(WiFi.psk());
        }

        if (newSSID.length() > 0)
        {
            // WPSConfig has already connected in STA mode successfully to the new station.
            if (DEBUG_WIFI)
            {
                Serial.printf("WPS finished. Connected successfull to SSID '%s'\r\n", newSSID.c_str());
            }

            //MEM_SetSSID(newSSID);
            //MEM_SetPSK(WiFi.psk());
            //EEPROM.writeString(0, newSSID.c_str());
            //EEPROM.writeString(32, String(WiFi.psk()).c_str());
            //EEPROM.commit();
        }
        else
        {
            wpsSuccess = false;
        }
    }

    if (wpsSuccess == false)
    {
        if (DEBUG_WIFI)
        {
            Serial.println("Failed to connect with WPS :-(");
        }
    }

    WiFi.disconnect();
    return wpsSuccess;
}

void WIFI_Connect(String ssid , String psk)
{
    //String ssid = EEPROM.readString(0);
    //String psk = EEPROM.readString(32);
    if (DEBUG_WIFI)
    {
        Serial.println(ssid);
        Serial.println(psk);
    }

    WiFi.begin(ssid.c_str(), psk.c_str());
    WiFi.setSleep(false);
}

bool WIFI_IsConnected(void)
{
    return (WiFi.status() == WL_CONNECTED);
}

bool WIFI_IsConnectedFailed(void)
{
    return (WiFi.status() == WL_CONNECT_FAILED);
}

bool WIFI_IsDisconnected(void)
{
    return (WiFi.status() != WL_CONNECTED);
}

void WIFI_PrintMyIP(void)
{
    if (DEBUG_WIFI)
    {
        Serial.println(WiFi.localIP());
    }
}

void WIFI_PrintServerIP(void)
{
    if (DEBUG_WIFI)
    {
        Serial.println(WiFi.gatewayIP().toString());
    }
}

IPAddress WIFI_GetServerIp(void)
{
    return WiFi.gatewayIP();
}

String WIFI_GetMacAddress(void)
{
    return WiFi.macAddress();
}

int WIFI_ScanNetwork(void)
{
    return WiFi.scanNetworks();
}

String WIFI_GetNetworkSSID(int i)
{
    return WiFi.SSID(i);
}

String WIFI_GetActualSSID(void){
    if ( WiFi.SSID().length() > 0 ){
        return WiFi.SSID();
    } else {
        return newSSIDTemp;
    }
}

String WIFI_GetActualPSK(void){
    return WiFi.psk();
}