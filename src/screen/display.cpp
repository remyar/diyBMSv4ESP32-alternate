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
#include "../rules/Rules.h"
#include "../WiFi/wifi.h"

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
    Rules *rules = RULES_Get();
    TFT_eSPI *tft = TFT_Get();

    // We have a single bank/pack
    tft->setTextColor(TFT_LIGHTGREY, TFT_BLACK);

    int16_t w = tft->width();
    // Take off the wifi banner height
    int16_t h = tft->height() - fontHeight_2 - 100;

    const int16_t xoffset = 32;

    tft->setTextFont(2);
    // Need to think about multilingual strings in the longer term
    tft->drawString("Bank voltage", xoffset + 0, 0);
    tft->drawString("External temp", xoffset + 0, h);
    tft->drawString("Module temp", xoffset + w / 2, h);
    tft->drawString("Cell voltage", xoffset + 0, 44 + h);
    tft->drawString("Modules balancing", xoffset + w / 2, 44 + h);

    // Single bank, large font
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    // Large FONT 8 - 75 pixel high (only numbers available)
    // Top centre
    tft->setTextDatum(TC_DATUM);
    tft->setTextFont(8);

    int16_t y = fontHeight_2;
    int16_t x = tft->width() / 2;
    float value = rules->packvoltage[0] / 1000.0;
    x += tft->drawFloat(value, 2, x, y);
    // Clear right hand side of display
    tft->fillRect(x, y, tft->width() - x, tft->fontHeight(), TFT_BLACK);

    // Top left
    tft->setTextDatum(TL_DATUM);
    tft->setTextFont(4);

    // Cell temperatures and stuff

    y = h + fontHeight_2;
    x = xoffset + 0;
    if (rules->moduleHasExternalTempSensor)
    {
        x += tft->drawNumber(rules->lowestExternalTemp, x, y);
        x += tft->drawString(" / ", x, y);
        x += tft->drawNumber(rules->highestExternalTemp, x, y);
    }
    else
    {
        x += tft->drawString("Not fitted", x, y);
    }
    // blank out gap between numbers
    tft->fillRect(x, y, (tft->width() / 2) - x, fontHeight_4, TFT_BLACK);

    x = xoffset + tft->width() / 2;
    y = h + fontHeight_2;
    x += tft->drawNumber(rules->lowestInternalTemp, x, y);
    x += tft->drawString(" / ", x, y);
    x += tft->drawNumber(rules->highestInternalTemp, x, y);
    // blank out gap between numbers
    tft->fillRect(x, y, tft->width() - x, fontHeight_4, TFT_BLACK);

    // Cell voltage ranges
    y = h + fontHeight_4 + fontHeight_2 + fontHeight_2 + 2;
    x = xoffset + 0;
    value = rules->lowestCellVoltage / 1000.0;
    x += tft->drawFloat(value, 3, x, y);
    x += tft->drawString(" / ", x, y);
    value = rules->highestCellVoltage / 1000.0;
    x += tft->drawFloat(value, 3, x, y);
    // blank out gap between numbers
    tft->fillRect(x, y, tft->width() / 2 - x, fontHeight_4, TFT_BLACK);

    y = h + fontHeight_4 + fontHeight_2 + fontHeight_2 + 2;
    x = xoffset + tft->width() / 2;
    x += tft->drawNumber(rules->numberOfBalancingModules, x, y);
    // blank out gap between numbers
    tft->fillRect(x, y, tft->width() - x, fontHeight_4, TFT_BLACK);
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