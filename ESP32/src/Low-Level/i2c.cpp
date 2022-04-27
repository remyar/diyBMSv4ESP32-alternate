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

#include "i2c.h"

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

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : WIFI_Init
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void I2C_Init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = gpio_num_t::GPIO_NUM_27;
    conf.scl_io_num = gpio_num_t::GPIO_NUM_26;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
    conf.master.clk_speed = 400000;

    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
}

esp_err_t I2C_WriteByte(i2c_port_t i2c_num, uint8_t deviceAddress, uint8_t i2cregister, uint8_t data)
{
    // We use the native i2c commands for ESP32 as the Arduino library
    // seems to have issues with corrupting i2c data if used from multiple threads
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);

    uint8_t buffer[2];
    buffer[0] = i2cregister;
    buffer[1] = data;
    i2c_master_write(cmd, buffer, 2, true);

    // i2c_master_write_byte(cmd, i2cregister, true);
    // i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    return ret;
}

uint8_t I2C_ReadByte(i2c_port_t i2c_num, uint8_t dev, uint8_t reg)
{
    // We use the native i2c commands for ESP32 as the Arduino library
    // seems to have issues with corrupting i2c data if used from multiple threads
    uint8_t data;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // Select the correct register on the i2c device
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    // Send repeated start, and read the register
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (dev << 1) | I2C_MASTER_READ, true);
    // Read single byte and expect NACK in reply
    i2c_master_read_byte(cmd, &data, i2c_ack_type_t::I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    // esp_err_t ret =
    i2c_master_cmd_begin(i2c_num, cmd, pdMS_TO_TICKS(100));

    i2c_cmd_link_delete(cmd);

    return data;
}