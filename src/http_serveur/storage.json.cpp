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

#include "./modules.json.h"
#include "../bms/bms.h"
#include "../sdcard/sdcard.h"
#include "./printStream.h"

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
void fileSystemListDirectory(AsyncResponseStream *response, fs::FS &fs, const char *dirname, uint8_t levels)
{

  File root = fs.open(dirname);
  if (!root)
  {
    ESP_LOGE(TAG, "failed to open dir");
    return;
  }
  if (!root.isDirectory())
  {
    ESP_LOGE(TAG, "not a dir");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      //Hide the diybms folder where the config files are kept
      if (levels && String(file.name()).startsWith("/diybms") == false)
      {
        fileSystemListDirectory(response, fs, file.name(), levels - 1);
        response->print(',');
      }
    }
    else
    {
      response->print('\"');
      response->print(file.name());
      response->print('\"');

      response->print(',');
    }

    file = root.openNextFile();
  }

  //Trailing null to cope with trailing ','
  response->print("null");
}
//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : MONITOR2_JSON
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void STORAGE_JSON(AsyncWebServerRequest *request)
{
    diybms_eeprom_settings *_mysettings = SETTINGS_Get();
    SDFS* SD = SDCARD_GetSD();
    sdcard_info info;

    info.available = SD->cardType() ? true : false;

    // Convert to KiB
    info.totalkilobytes = SD->totalBytes() / 1024;
    info.usedkilobytes = SD->usedBytes() / 1024;

/*
    info.flash_totalkilobytes = LITTLEFS.totalBytes() / 1024;
    info.flash_usedkilobytes = LITTLEFS.usedBytes() / 1024;
*/
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    response->print("{\"storage\":{");
    PrintStreamCommaBoolean(response, "\"logging\":", _mysettings->loggingEnabled);
    PrintStreamComma(response, "\"frequency\":", _mysettings->loggingFrequencySeconds);

    response->print("\"sdcard\":{");
    PrintStreamCommaBoolean(response, "\"available\":", info.available);
    PrintStreamComma(response, "\"total\":", info.totalkilobytes);
    PrintStreamComma(response, "\"used\":", info.usedkilobytes);
    response->print("\"files\":[");
    // File listing goes here
    if (info.available)
    {
        fileSystemListDirectory(response, *SD, "/", 2);
    }
    response->print(']');

    response->print("},\"flash\":{");
    PrintStreamComma(response, "\"total\":", info.flash_totalkilobytes);
    PrintStreamComma(response, "\"used\":", info.flash_usedkilobytes);

    response->print("\"files\":[");
    response->print(']');
    response->print("}");

    // The END...
    response->print('}');
    response->print('}');

    response->addHeader("Cache-Control", "no-store");
    request->send(response);
}