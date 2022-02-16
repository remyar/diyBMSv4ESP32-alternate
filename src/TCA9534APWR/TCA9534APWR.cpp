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
#include "./TCA9534APWR.h"

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
static uint8_t TCA9534APWR_Value = 0;
static uint8_t oldTCA9534APWR_Value = 0;
//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
// Triggered when TCA9534A INT pin goes LOW
void IRAM_ATTR _TCA9534AInterrupt()
{
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//
uint8_t TCA9534APWR_GetValue(void)
{
    return TCA9534APWR_Value;
}

void TCA9534APWR_SetValue(uint8_t value)
{
    TCA9534APWR_Value = value;
}

//--------------------------------------------------------------------------------------------------
// FONCTION    : KEYBOARD_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
bool TCA9534APWR_TaskInit(void)
{
    // https://datasheet.lcsc.com/szlcsc/1809041633_Texas-Instruments-TCA9534APWR_C206010.pdf
    // TCA9534APWR Remote 8-Bit I2C and Low-Power I/O Expander With Interrupt Output and Configuration Registers
    // https://lcsc.com/product-detail/Interface-I-O-Expanders_Texas-Instruments-TCA9534APWR_C206010.html
    // A0/A1/A2 are LOW, so i2c address is 0x38

    // PINS
    // P0= BLUE
    // P1= RED
    // P2= GREEN
    // P3= DISPLAY BACKLIGHT LED
    // P4= SPARE on J13
    // P5= Canbus RS
    // P6= SPARE on J13
    // P7= ESTOP (pull to ground to trigger)
    // INTERRUPT PIN = ESP32 IO34

    // BIT  76543210
    // PORT 76543210
    // MASK=10000000

    // All off

    esp_err_t ret = I2C_WriteByte(I2C_NUM_0, TCA9534APWR_ADDRESS, TCA9534APWR_OUTPUT, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE("TCA9534APWR", "TCA9534APWR Error");
        return false;
    }

    // 0×03 Configuration, P7 (estop) and P4 (remote touch) as input, others outputs (0=OUTPUT)
    ret = I2C_WriteByte(I2C_NUM_0, TCA9534APWR_ADDRESS, TCA9534APWR_CONFIGURATION, TCA9534APWR_INPUTMASK);
    // 0×02 Polarity Inversion, zero = off
    // writeByte(TCA9534APWR_ADDRESS, TCA9534APWR_POLARITY_INVERSION, 0);

    TCA9534APWR_Value = I2C_ReadByte(I2C_NUM_0, TCA9534APWR_ADDRESS, TCA9534APWR_INPUT);
    // SERIAL_DEBUG.println("Found TCA9534APWR");

    attachInterrupt(TCA9534A_INTERRUPT_PIN, _TCA9534AInterrupt, FALLING);
    ESP_LOGI("TCA9534APWR", "Found TCA9534A");
    return true;
}

void TCA9534APWR_TaskRun(void)
{
    if (TCA9534APWR_Value != oldTCA9534APWR_Value)
    {
        I2C_WriteByte(I2C_NUM_0, TCA9534APWR_ADDRESS, TCA9534APWR_OUTPUT, TCA9534APWR_Value);
        oldTCA9534APWR_Value = TCA9534APWR_Value;
    }
}