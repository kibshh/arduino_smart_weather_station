#ifndef SENSORSCONFIG_H
#define SENSORSCONFIG_H

#include <Arduino.h>

/* DHT11 */
#define SENSORS_DHT11_PIN                             (2u)
#define SENSORS_DHT11_TEMPERATURE_MIN                 (-20)
#define SENSORS_DHT11_TEMPERATURE_MAX                 (50)
#define SENSORS_DHT11_HUMIDITY_MIN                    (0)
#define SENSORS_DHT11_HUMIDITY_MAX                    (100)

/* BMP280 */
#define SENSORS_BMP280_I2C_ADDR                       (0x76)
#define SENSORS_BMP280_PRESSURE_MIN                   (300)
#define SENSORS_BMP280_PRESSURE_MAX                   (1200)
#define SENSORS_BMP280_TEMPERATURE_MIN                (-20)
#define SENSORS_BMP280_TEMPERATURE_MAX                (50)
#define SENSORS_BMP280_ALTITUDE_MIN                   (-1000)
#define SENSORS_BMP280_ALTITUDE_MAX                   (9000)
#define SENSORS_BMP280_LOCAL_SEA_LEVEL_PRESSURE       (1013.25f)

/* BH1750 */
#define SENSORS_BH1750_I2C_ADDDR_VCC                  (0x5C)
#define SENSORS_BH1750_I2C_ADDDR_GND                  (0x23)
#define SENSORS_BH1750_LUMINANCE_MIN                  (0)
#define SENSORS_BH1750_LUMINANCE_MAX                  (150000)

/* MQ135 */
#define SENSORS_MQ135_PIN_ANALOG                      (A0)
#define SENSORS_MQ135_PPM_MIN                         (0)
#define SENSORS_MQ135_PPM_MAX                         (10000)
#define SENSORS_MQ135_PARAMETER_A                     (116.60) 
#define SENSORS_MQ135_PARAMETER_B                     (2.77)
#define SENSORS_MQ135_R_ZERO                          (10000)

/* MQ7 */
#define SENSORS_MQ7_PIN_ANALOG                        (A1)
#define SENSORS_MQ7_PIN_PWM_HEATER                    (9u)
#define SENSORS_MQ7_PPM_MIN                           (10)
#define SENSORS_MQ7_PPM_MAX                           (1000)  
#define SENSORS_MQ7_R_ZERO                            (10000)  // Set to the vaule you got after calibration, in ohms
#define SENSORS_MQ7_CALCULATION_CONSTANT_1            (0.5)    //By datasheet, change after calibration
#define SENSORS_MQ7_CALCULATION_CONSTANT_2            (-0.27)  //By datasheet, change after calibration
#define SENSORS_MQ7_CLEAR_AIR_FACTOR                  (9.83)   // By datasheet
#define SENSORS_MQ7_HEATER_LOW_TIMEOUT_MS             (90000u)           
#define SENSORS_MQ7_HEATER_HIGH_TIMEOUT_MS            (60000u)  

/* GY-ML8511 */
#define SENSORS_GY_ML8511_PIN_ANALOG                  (A2)
#define SENSORS_GYML8511_UV_MIN                       (0)
#define SENSORS_GYML8511_UV_MAX                       (15)

/* Arduino rain sensor */
#define SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT  
#define SENSORS_ARDUINO_RAIN_PIN_ANALOG               (A4)
#define SENSORS_ARDUINO_RAIN_PIN_DIGITAL              (4u) // In case SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT is not defined

#endif