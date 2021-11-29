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

#include "./web_server.h"
#include "./diyBms_server.h"
#include "./sdCard/sdcard.h"
#include "./monitor2.json.h"

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
String TemplateProcessor(const String &var)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();

    if (var == "LANGUAGE")
        return String(_mysettings->language.length() > 0 ? _mysettings->language : "en");

    if (var == "graph_voltagehigh")
        return String(_mysettings->graph_voltagehigh);

    if (var == "graph_voltagelow")
        return String(_mysettings->graph_voltagelow);

    if (var == "integrity_file_jquery_js")
        return "sha256-zp0HUArZHsK1JMJwdk7EyaM+eDINjTdOxADt5Ij2JRs=";

    if (var == "noofseriesmodules")
        return String(maximum_controller_cell_modules);

    if (var == "maxnumberofbanks")
        return String(maximum_number_of_banks);

    return String();
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void DIYBMSServer_Begin(void)
{
    if (SDCARD_GetSD()->cardType() != CARD_NONE)
    {
        WEBSERVER_End();

        AsyncWebServer *server = WEBSERVER_Get();

        server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->redirect("/default.htm"); });

        server->on("/default.htm", HTTP_GET,
                   [](AsyncWebServerRequest *request)
                   {
                       AsyncWebServerResponse *response = request->beginResponse(*SDCARD_GetSD(), "/default.htm", "text/html", false, TemplateProcessor);
                       response->addHeader("Cache-Control", "no-store");
                       request->send(response);
                   });

        server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/style.css"); });

        server->on("/jquery.js", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/jquery.js"); });

        server->on("/wait.png", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/wait.png"); });

        server->on("/logo.png", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/logo.png"); });

        server->on("/patron.png", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/patron.png"); });

        server->on("/warning.png", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/warning.png"); });

        server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/favicon.ico"); });

        server->on("/lang_en.js", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/lang_en.js"); });

        server->on("/echarts.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/echarts.min.js"); });

        server->on("/notify.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/notify.min.js"); });

        server->on("/pagecode.js", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(*SDCARD_GetSD(), "/pagecode.js"); });

        server->on("/monitor2.json", HTTP_GET, MONITOR2_JSON);

        WEBSERVER_Begin();
    }
}