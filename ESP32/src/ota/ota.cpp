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
#include "./ota.h"
#include "../WiFi/wifi.h"
#include "../screen/screen.h"

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
static e_OTA_STATE eOtaState = OTA_INIT;
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
bool OTA_TaskInit(void)
{
  eOtaState = OTA_INIT;
  return true;
}

void OTA_TaskRun(void)
{
  switch (eOtaState)
  {
  case (OTA_INIT):
  {
    if (WIFI_IsConnected())
    {
      ArduinoOTA.setPort(3232);
      ArduinoOTA.setPassword("1jiOOx12AQgEco4e");
      ArduinoOTA
          .onStart([]()
                   {
                  String type;
                  if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                  else // U_SPIFFS
                    type = "filesystem";

                  SCREEN_Change(SCREEN_UPDATE);
                  // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                  ESP_LOGI(TAG, "Start updating %s", type); });
      ArduinoOTA.onEnd([]()
                       { 
                         ESP_LOGD(TAG, "\nEnd"); 
                         
                        });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                            { 
                             // ESP_LOGD(TAG, "Progress: %u%%\r", (progress / (total / 100))); 
                              SCREEN_SetUpdateProgress((progress / (total / 100)));
                            });
      ArduinoOTA.onError([](ota_error_t error)
                         {
                        ESP_LOGD(TAG, "Error [%u]: ", error);
                        if (error == OTA_AUTH_ERROR)
                          ESP_LOGE(TAG, "Auth Failed");
                        else if (error == OTA_BEGIN_ERROR)
                          ESP_LOGE(TAG, "Begin Failed");
                        else if (error == OTA_CONNECT_ERROR)
                          ESP_LOGE(TAG, "Connect Failed");
                        else if (error == OTA_RECEIVE_ERROR)
                          ESP_LOGE(TAG, "Receive Failed");
                        else if (error == OTA_END_ERROR)
                          ESP_LOGE(TAG, "End Failed"); });

      ArduinoOTA.setHostname(WiFi.getHostname());
      ArduinoOTA.setMdnsEnabled(true);
      ArduinoOTA.begin();
      eOtaState = OTA_RUN;
    }
    break;
  }
  case (OTA_RUN):
  {
    if (WIFI_IsConnected())
    {
      ArduinoOTA.handle();
    }
    else
    {
      eOtaState = OTA_INIT;
    }
    break;
  }
  }
}