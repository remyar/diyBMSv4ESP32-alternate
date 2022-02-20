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
#include "./monitor3.json.h"
#include "./modules.json.h"
#include "./identify.module h"
#include "./globalSettings.h"
#include "./storage.json.h"
#include "./settings.json.h"
#include "./rules.json.h"

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
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
  <p><h1>File Upload</h1></p>
  <p>Free Storage: %FREESPIFFS% | Used Storage: %USEDSPIFFS% | Total Storage: %TOTALSPIFFS%</p>
  <form method="POST" action="/upload" enctype="multipart/form-data"><input type="file" name="data"/><input type="submit" name="upload" value="Upload" title="Upload File"></form>
  <p>After clicking upload it will take some time for the file to firstly upload and then be written to SPIFFS, there is no indicator that the upload began.  Please be patient.</p>
  <p>Once uploaded the page will refresh and the newly uploaded file will appear in the file list.</p>
  <p>If a file does not appear, it will be because the file was too big, or had unusual characters in the file name (like spaces).</p>
  <p>You can see the progress of the upload by watching the serial output.</p>
  <p>%FILELIST%</p>
</body>
</html>
)rawliteral";

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String humanReadableSize(const size_t bytes)
{
    if (bytes < 1024)
        return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
        return String(bytes / 1024.0 / 1024.0) + " MB";
    else
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

// list all of the files, if ishtml=true, return html rather than simple text
String listFiles(bool ishtml)
{
    String returnText = "";
    Serial.println("Listing files stored on SPIFFS");
    File root = SDCARD_GetSD()->open("/");
    File foundfile = root.openNextFile();
    if (ishtml)
    {
        returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th></tr>";
    }
    while (foundfile)
    {
        if (ishtml)
        {
            returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td></tr>";
        }
        else
        {
            returnText += "File: " + String(foundfile.name()) + "\n";
        }
        foundfile = root.openNextFile();
    }
    if (ishtml)
    {
        returnText += "</table>";
    }
    root.close();
    foundfile.close();
    return returnText;
}

// handles uploads
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
    Serial.println(logmessage);

    if (!index)
    {
        logmessage = "Upload Start: " + String(filename);
        // open the file on first call and store the file handle in the request object
        request->_tempFile = SDCARD_GetSD()->open("/" + filename, "w");
        Serial.println(logmessage);
    }

    if (len)
    {
        // stream the incoming chunk to the opened file
        request->_tempFile.write(data, len);
        logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
        Serial.println(logmessage);
    }

    if (final)
    {
        logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
        // close the file handle as the upload is now done
        request->_tempFile.close();
        Serial.println(logmessage);

        SDCARD_GetSD()->rename("/" + String(filename), String("/extract.tar"));

        // mount spiffs (or any other filesystem)
        request->redirect("/");
    }
}

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

    if (var == "FILELIST")
    {
        return listFiles(true);
    }
    if (var == "FREESPIFFS")
    {
        return humanReadableSize((SDCARD_GetSD()->totalBytes() - SDCARD_GetSD()->usedBytes()));
    }

    if (var == "USEDSPIFFS")
    {
        return humanReadableSize(SDCARD_GetSD()->usedBytes());
    }

    if (var == "TOTALSPIFFS")
    {
        return humanReadableSize(SDCARD_GetSD()->totalBytes());
    }

    return String();
}

void SendSuccess(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    StaticJsonDocument<100> doc;
    doc["success"] = true;
    serializeJson(doc, *response);
    request->send(response);
}

void SendFailure(AsyncWebServerRequest *request)
{
    request->send(500, "text/plain", "Failed");
}

void saveBankConfiguration(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();

    if (request->hasParam("totalControllers", true))
    {
        AsyncWebParameter *p1 = request->getParam("totalControllers", true);
        _mysettings->totalControllers = p1->value().toInt();
    }

    for (int i = 0; i < _mysettings->totalControllers; i++)
    {
        if (request->hasParam("totalSeriesModules_" + String(i), true))
        {
            AsyncWebParameter *p1 = request->getParam("totalSeriesModules_" + String(i), true);
            _mysettings->totalNumberOfSeriesModules[i] = p1->value().toInt();
        }
        if (request->hasParam("totalBanks_" + String(i), true))
        {
            AsyncWebParameter *p1 = request->getParam("totalBanks_" + String(i), true);
            _mysettings->totalNumberOfBanks[i] = p1->value().toInt();
        }
        if (request->hasParam("baudrate_" + String(i), true))
        {
            AsyncWebParameter *p1 = request->getParam("baudrate_" + String(i), true);
            _mysettings->baudRate[i] = p1->value().toInt();
        }

        //-- send to controller
        MODBUS_SendConfiguration(i);
    }

    SETTINGS_Save();

    SendSuccess(request);
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
                   { request->redirect("/index.html"); });

        server->on("/index.htm", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->redirect("/index.html"); });

        server->serveStatic("/", *SDCARD_GetSD(), "/");

        server->on("/upload", HTTP_GET,
                   [](AsyncWebServerRequest *request)
                   {
                       String logmessage = "Client:" + request->client()->remoteIP().toString() + +" " + request->url();
                       Serial.println(logmessage);
                       request->send_P(200, "text/html", index_html, TemplateProcessor);
                   });

        server->on(
            "/upload", HTTP_POST,
            [](AsyncWebServerRequest *request)
            {
                request->send(200);
            },
            handleUpload);

        server->on("/monitor2.json", HTTP_GET, MONITOR2_JSON);
        server->on("/monitor3.json", HTTP_GET, MONITOR3_JSON);
        server->on("/modules.json", HTTP_GET, MODULES_JSON);
        server->on("/storage.json", HTTP_GET, STORAGE_JSON);
        server->on("/settings.json", HTTP_GET, SETTINGS_JSON);
        server->on("/rules.json", HTTP_GET, RULES_JSON);
        server->on("/identifyModule.json", HTTP_GET, IDENTIFY_MODULE);

        server->on("/savebankconfig.json", HTTP_POST, saveBankConfiguration);
        server->on("/saveglobalsetting.json", HTTP_POST, GLOBALSETTINGS_JSON);
        server->on("/savestorage.json", HTTP_POST, STORAGE_Save);

        WEBSERVER_Begin();
    }
}