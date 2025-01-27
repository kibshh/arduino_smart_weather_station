#include "serial_console.h"

const serial_console_config_ts display_sensors_config[] PROGMEM =
{
#ifdef DHT11_TEMPERATURE
  {"Temperature",        "C",      DHT11_TEMPERATURE,       DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef DHT11_HUMIDITY
  {"Humidity",           "%",      DHT11_HUMIDITY,          DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef BMP280_PRESSURE
  {"Pressure",           "hPa",    BMP280_PRESSURE,         DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef BMP280_TEMPERATURE
  {"Temperature",        "C",      BMP280_TEMPERATURE,      DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef BMP280_ALTITUDE
  {"Altitude",           "m",      BMP280_ALTITUDE,         DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef BH1750_LUMINANCE
  {"Luminance",          "lx",     BH1750_LUMINANCE,        DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef MQ135_PPM
  {"Gases PPM",          "",       MQ135_PPM,               DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef MQ7_COPPM
  {"CO PPM",             "",       MQ7_COPPM,               DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef GYML8511_UV
  {"UV intensity",       "",       GYML8511_UV,             DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef ARDUINORAIN_RAINING
  {"Raining status",     "",       ARDUINORAIN_RAINING,     DISPLAY_INDICATION,         DISPLAY_NO_DECIMALS},
#endif
};

error_manager_error_code_te serial_console_init()
{
    // Begin serial communication with the defined baud rate.
    Serial.begin(SERIAL_CONSOLE_BAUDRATE);

    // Check if serial is available but don't block program execution.
    if (!Serial) {
        // Return an error code if serial is not available.
        return ERROR_CODE_SERIAL_CONSOLE_INIT_FAILED;
    }
    // If no error occurred during initialization, return success code.
    return ERROR_CODE_NO_ERROR;
}