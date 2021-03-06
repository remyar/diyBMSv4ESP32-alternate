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
#include "./display.h"
#include "tft_splash_image.h"
#include "../WiFi/wifi.h"
#include "../modbus/modbus.h"
#include "../bms/bms.h"

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
int16_t fontHeight_2;
int16_t fontHeight_4;

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void DISPLAY_Init(void)
{
    TFT_Init();
    TFT_FillScreen(TFT_BLACK);
    TFT_ScreenBacklight(true);
}

void DISPLAY_FillScreen(void)
{
    TFT_FillScreen(TFT_BLACK);
}

void DISPLAY_Booting(void)
{
    TFT_eSPI *tft = TFT_Get();

    fontHeight_2 = tft->fontHeight(2);
    fontHeight_4 = tft->fontHeight(4);

    uint16_t x = tft->width() / 2;
    uint16_t y = tft->height() / 2 - fontHeight_4 * 2;

    // Draw box in same colour as background of logo/image
    tft->fillRoundRect(16, 16, tft->width() - 32, tft->height() - 48, 8, SplashLogoPalette[0]);

    // White border
    tft->drawRoundRect(16 + 2, 16 + 2, tft->width() - 36, tft->height() - 52, 8, TFT_WHITE);

    TFT_eSprite spr = TFT_eSprite(tft);
    spr.setColorDepth(4);
    spr.createSprite(SplashLogoGraphic_Width, SplashLogoGraphic_Height);
    spr.pushImage(0, 0,
                  SplashLogoGraphic_Width,
                  SplashLogoGraphic_Height,
                  (uint16_t *)SplashLogoGraphic);

    spr.createPalette(SplashLogoPalette, 16);
    spr.pushSprite(tft->width() / 2 - SplashLogoGraphic_Width / 2, 24);
    spr.deleteSprite();

    y = 85;
    tft->setTextColor(TFT_WHITE, SplashLogoPalette[0]);
    tft->drawCentreString("Version:", x, y, 2);
    y += fontHeight_2;

    tft->setTextColor(TFT_YELLOW, SplashLogoPalette[0]);
    tft->drawCentreString("1.0.0 Alternate", x, y, 2);
    y += fontHeight_2;

    tft->setTextColor(TFT_WHITE, SplashLogoPalette[0]);
    tft->drawCentreString("Build Date:", x, y, 2);
    y += fontHeight_2;

    tft->setTextColor(TFT_YELLOW, SplashLogoPalette[0]);
    tft->drawCentreString("14/11/2021", x, y, 2);
    y += fontHeight_2;
}

void DISPLAY_Voltage(void)
{
    // CellModuleInfo *cmi = CMI_Get(0, 0);
    s_GLOBAL_BMS *bms = BMS_GetBmsPtr();

    //   Rules *rules = RULES_Get();
    TFT_eSPI *tft = TFT_Get();

    tft->fillScreen(TFT_BLACK);

    int16_t w = tft->width();
    // Take off the wifi banner height
    int16_t h = tft->height() - fontHeight_2 - 68;
    int16_t yhalfway = h / 2;

    // Grid lines
    tft->drawLine(w / 2, 0, w / 2, h, TFT_DARKGREY);

    tft->drawLine(0, yhalfway, w, yhalfway, TFT_DARKGREY);

    tft->drawLine(0, h, w, h, TFT_DARKGREY);

    tft->setTextFont(2);
    // Need to think about multilingual strings in the longer term
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft->drawString("Bank voltage", 0, 0);
    tft->drawString("External temp", 0, h + 2);
    tft->drawString("Module temp", 2 + w / 2, h + 2);
    tft->drawString("Cell voltages", 0, fontHeight_2 + fontHeight_2 + h + 2);
    tft->drawString("Modules balancing", 2 + w / 2, fontHeight_2 + fontHeight_2 + h + 2);

    uint8_t ctrl = maximum_of_total_controllers > 4 ? 4 : maximum_of_total_controllers;

    for (uint8_t i = 0; i < ctrl; i++)
    {
        int16_t y = 0;
        int16_t x = 0;

        if (i == 1 || i == 3)
        {
            x += 4 + w / 2;
        }

        if (i > 1)
        {
            y += 2 + yhalfway;
        }

        // Need to think about multilingual strings in the longer term
        x += tft->drawString("Controller voltage ", x, y);
        x += tft->drawNumber(i, x, y);

        //-- VALUE
        tft->setTextDatum(TL_DATUM);
        int16_t halfway = h / 2;
        y = 1 + fontHeight_2;
        x = 0;

        // Half way across screen, minus vertical line
        int16_t limitx = (w / 2) - 1;

        if (i == 1 || i == 3)
        {
            x += 4 + w / 2;
            limitx = w;
        }

        if (i > 1)
        {
            y += halfway;
        }

        tft->setTextColor(TFT_GREEN, TFT_BLACK);
        tft->setTextFont(7);
        float value = bms->controllerVoltage[i] / 1000.0;
        x += tft->drawFloat(value, 2, x, y);

        // Clear right hand side of display
        tft->fillRect(x, y, limitx - x, tft->fontHeight(), TFT_BLACK);
    }

    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    //Tiny font
    tft->setTextFont(2);

    int16_t x = 0;
    int16_t y = fontHeight_2 + h + 2;
  /*  if (rules.moduleHasExternalTempSensor)
    {
        x += tft.drawNumber(rules.lowestExternalTemp, x, y);
        x += tft.drawString(" / ", x, y);
        x += tft.drawNumber(rules.highestExternalTemp, x, y);
    }
    else*/
    {
        x += tft->drawString("Not fitted", x, y);
    }
    //blank out gap between numbers
    tft->fillRect(x, y, (w / 2) - 1 - x, fontHeight_2, TFT_BLACK);

    x = 2 + w / 2;
    y = fontHeight_2 + h + 2;
    x += tft->drawNumber(0.0, x, y);
    x += tft->drawString(" / ", x, y);
    x += tft->drawNumber(0.0, x, y);
    tft->fillRect(x, y, w - x, fontHeight_2, TFT_BLACK);

    x = 0;
    y = fontHeight_2 + fontHeight_2 + fontHeight_2 + h + 2;
    float value = 0 / 1000.0;
    x += tft->drawFloat(value, 3, x, y);
    x += tft->drawString(" / ", x, y);
    value = 0 / 1000.0;
    x += tft->drawFloat(value, 3, x, y);
    tft->fillRect(x, y, (w / 2) - 1 - x, fontHeight_2, TFT_BLACK);

    x = 2 + w / 2;
    y = fontHeight_2 + fontHeight_2 + fontHeight_2 + h + 2;
    x += tft->drawNumber(0, x, y);
    tft->fillRect(x, y, w - x, fontHeight_2, TFT_BLACK);
}

void DISPLAY_WifiDetails(void)
{
    TFT_eSPI *tft = TFT_Get();

    tft->setTextDatum(TL_DATUM);
    int16_t y = tft->height() - fontHeight_2;
    tft->fillRect(0, y, tft->width(), tft->height() - y, TFT_DARKGREY);
    tft->setTextFont(2);
    tft->setTextColor(TFT_BLACK, TFT_DARKGREY);
    int16_t x = 2;

    if (WiFi.isConnected())
    {
        x += tft->drawString(WiFi.getHostname(), x, y);
        x += 10;
        x += tft->drawString(WiFi.localIP().toString(), x, y);

        // Draw RSSI on bottom right corner
        // Received Signal Strength in dBm
        x += 10;
        x += tft->drawNumber(WiFi.RSSI(), x, y);
        x += tft->drawString("dBm", x, y);
    }
    else
    {
        x += tft->drawString("WIFI not connected", x, y);
    }
}

void DISPLAY_Progress(float value)
{
    TFT_eSPI *tft = TFT_Get();

    tft->drawRoundRect(16 + 2, 16 + 2, tft->width() - 36, tft->height() - 52, 8, TFT_RED);
}