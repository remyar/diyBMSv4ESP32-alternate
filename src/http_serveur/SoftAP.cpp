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

#include "./SoftAP.h"
#include "./web_server.h"
#include "../sdCard/sdcard.h"
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

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
String TemplateProcessor(String var)
{
    String str = "";
    if (var == "SSID")
    {
        str = "";
        for (int i = 0; i < 20; ++i)
        {
            if (WIFI_GetNetworkSSID(i).length() > 0)
            {
                str += "<option>" + WIFI_GetNetworkSSID(i) + "</option>";
                if (DEBUG_WIFI)
                {
                    Serial.println(WIFI_GetNetworkSSID(i).c_str());
                }
            }
        }
    }

    return str;
}

void handleSave(AsyncWebServerRequest *request)
{
    String s;
    String ssid = request->arg("ssid");
    String password = request->arg("pass");
    diybms_eeprom_settings *settings = SETTINGS_Get();

    if ((ssid.length() <= 32) && (password.length() <= 32))
    {
        settings->wifi_ssid = ssid;
        settings->wifi_psk = password;

        SETTINGS_Save();

        s = F("<p>WIFI settings saved, will reboot in 5 seconds.</p>");

        request->send(200, "text/html", s);

        //Delay 6 seconds
        for (size_t i = 0; i < 60; i++)
        {
            delay(100);
        }

        ESP.restart();
    }
    else
    {
        s = F("<p>WIFI settings too long.</p>");
        request->send(200, "text/html", s);
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
void SOFTAP_Begin(void)
{
    if (SDCARD_GetSD()->cardType() != CARD_NONE)
    {
        AsyncWebServer* server = WEBSERVER_Get();

        server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                    { request->redirect("/softap.htm"); });

        server->on("/softap.htm", HTTP_GET,
                    [](AsyncWebServerRequest *request)
                    {
                        AsyncWebServerResponse *response = request->beginResponse(*SDCARD_GetSD(), "/softap.htm", "text/html", false, TemplateProcessor);
                        request->send(response);
                    });

        server->on("/save", HTTP_POST, handleSave);

        WEBSERVER_Begin();
    }
}
