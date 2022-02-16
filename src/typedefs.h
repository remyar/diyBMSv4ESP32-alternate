#ifndef DIYBMS_DEFINES_H_
#define DIYBMS_DEFINES_H_

#define maximum_controller_cell_modules 16

struct CellModuleInfo
{
    // Used as part of the enquiry functions
    bool settingsCached : 1;
    // Set to true once the module has replied with data
    bool valid : 1;
    // Bypass is active
    bool inBypass : 1;
    // Bypass active and temperature over set point
    bool bypassOverTemp : 1;

    uint16_t voltagemV;
    uint16_t voltagemVMin;
    uint16_t voltagemVMax;
    // Signed integer byte (negative temperatures)
    int8_t internalTemp;
    int8_t externalTemp;

    uint8_t BypassOverTempShutdown;
    uint16_t BypassThresholdmV;
    uint16_t badPacketCount;

    // Resistance of bypass load
    float LoadResistance;
    // Voltage Calibration
    float Calibration;
    // Reference voltage (millivolt) normally 2.00mV
    float mVPerADC;
    // Internal Thermistor settings
    uint16_t Internal_BCoefficient;
    // External Thermistor settings
    uint16_t External_BCoefficient;
    // Version number returned by code of module
    uint16_t BoardVersionNumber;
    // Last 4 bytes of GITHUB version
    uint32_t CodeVersionNumber;
    // Value of PWM timer for load shedding
    uint16_t PWMValue;

    uint16_t BalanceCurrentCount;
    uint16_t PacketReceivedCount;
};

#endif