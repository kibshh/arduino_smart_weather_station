#include "src/project_configs/project_configs.h"
#ifdef CONFIGS_ARDUINO_RAIN_SENSOR_INCLUDED
#include "src/project_sensors/arduino_rain_sensor/arduino_rain_sensor.h"
#endif
#ifdef CONFIGS_BH1750_SENSOR_INCLUDED
#include "src/project_sensors/bh1750_sensor/bh1750_sensor.h"
#endif
#ifdef CONFIGS_BMP280_SENSOR_INCLUDED
#include "src/project_sensors/bmp280_sensor/bmp280_sensor.h"
#endif
#ifdef CONFIGS_DHT11_SENSOR_INCLUDED
#include "src/project_sensors/dht11_sensor/dht11_sensor.h"
#endif
#ifdef CONFIGS_GY_ML8511_SENSOR_INCLUDED
#include "src/project_sensors/gy_ml8511_sensor/gy_ml8511_sensor.h"
#endif
#ifdef CONFIGS_MQ7_SENSOR_INCLUDED
#include "src/project_sensors/mq7_sensor/mq7_sensor.h"
#endif
#ifdef CONFIGS_MQ135_SENSOR_INCLUDED
#include "src/project_sensors/mq135_sensor/mq135_sensor.h"
#endif
#ifdef CONFIGS_I2C_SCANNER_INCLUDED
#include "src/project_utilities/i2c_scanner/i2c_scanner.h"
#endif
#ifdef CONFIGS_RTC_INCLUDED
#include "src/project_utilities/rtc/rtc.h"
#endif

//#define MODE_GET_I2C_ADDR
// #define BAUDRATE 9600

// int current_display_function = 0;
// unsigned long previous_millis = 0;

// void setup() {
//   Wire.begin();
//   Serial.begin(BAUDRATE);
//   display_init();
//   sensors_init();
//   rtc_init();
//   while (!Serial); // Wait for the serial port to connect

//   #ifdef MODE_GET_I2C_ADDR
//     Serial.println("\nI2C Scanner");
//   #else
//     Serial.println("\nWeather Station");
//   #endif
// }

// void loop() {
//   unsigned long current_millis = millis();

//   #ifdef MODE_GET_I2C_ADDR
//     if(current_millis - previous_millis >= (unsigned long)I2CSCAN_I2CSCAN_INTERVAL_MS)
//     {
//       previous_millis = current_millis;
//       i2cScan_scanForAdress();
//     }
//   #else
//     if(current_millis - previous_millis >= (unsigned long)DISPLAY_DISPLAY_INTERVAL_MS)
//     {
//       previous_millis = current_millis;
//       display_functions[current_display_function]();
//       current_display_function++;
//       current_display_function %= display_functions_size;
//     }
//   #endif
// }
