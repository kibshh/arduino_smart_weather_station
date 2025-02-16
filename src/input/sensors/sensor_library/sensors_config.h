#ifndef SENSORS_CONFIG_H
#define SENSORS_CONFIG_H

#include <Arduino.h>

/* DHT11 */
#define SENSORS_DHT11_PIN                             (uint8_t)(2u) /** Pin for DHT11 sensor */
#define SENSORS_DHT11_TEMPERATURE_MIN                 (float)(-20)  /** Minimum temperature for DHT11 sensor */
#define SENSORS_DHT11_TEMPERATURE_MAX                 (float)(50)   /** Maximum temperature for DHT11 sensor */
#define SENSORS_DHT11_HUMIDITY_MIN                    (float)(0)    /** Minimum humidity for DHT11 sensor */
#define SENSORS_DHT11_HUMIDITY_MAX                    (float)(100)  /** Maximum humidity for DHT11 sensor */

/* BMP280 */
#define SENSORS_BMP280_I2C_ADDR                       (uint8_t)(0x76)    /** I2C address for BMP280 sensor */
#define SENSORS_BMP280_PRESSURE_MIN                   (float)(300)       /** Minimum pressure for BMP280 sensor */
#define SENSORS_BMP280_PRESSURE_MAX                   (float)(1200)      /** Maximum pressure for BMP280 sensor */
#define SENSORS_BMP280_TEMPERATURE_MIN                (float)(-20)       /** Minimum temperature for BMP280 sensor */
#define SENSORS_BMP280_TEMPERATURE_MAX                (float)(50)        /** Maximum temperature for BMP280 sensor */
#define SENSORS_BMP280_ALTITUDE_MIN                   (float)(-1000)     /** Minimum altitude for BMP280 sensor */
#define SENSORS_BMP280_ALTITUDE_MAX                   (float)(9000)      /** Maximum altitude for BMP280 sensor */
#define SENSORS_BMP280_LOCAL_SEA_LEVEL_PRESSURE       (float)(1013.25f)  /** Local sea-level pressure for BMP280 sensor */

/* BH1750 */
#define SENSORS_BH1750_I2C_ADDDR_VCC                  (uint8_t)(0x5C)  /** I2C address for BH1750 sensor when VCC is high */
#define SENSORS_BH1750_I2C_ADDDR_GND                  (uint8_t)(0x23)  /** I2C address for BH1750 sensor when GND is high */
#define SENSORS_BH1750_LUMINANCE_MIN                  (float)(0)       /** Minimum luminance for BH1750 sensor */
#define SENSORS_BH1750_LUMINANCE_MAX                  (float)(150000)  /** Maximum luminance for BH1750 sensor */

/* MQ135 */
#define SENSORS_MQ135_PIN_ANALOG                      (A0)      /** Analog pin for MQ135 sensor */
#define SENSORS_MQ135_PPM_MIN                         (float)(0)       /** Minimum PPM for MQ135 sensor */
#define SENSORS_MQ135_PPM_MAX                         (float)(10000)   /** Maximum PPM for MQ135 sensor */
#define SENSORS_MQ135_PARAMETER_A                     (float)(116.60)  /** Parameter A for MQ135 sensor calibration */
#define SENSORS_MQ135_PARAMETER_B                     (float)(2.77)    /** Parameter B for MQ135 sensor calibration */
#define SENSORS_MQ135_R_ZERO                          (float)(10000)   /** R-zero for MQ135 sensor */

/* MQ7 */
#define SENSORS_MQ7_PIN_ANALOG                        (A1)                     /** Analog pin for MQ7 sensor */
#define SENSORS_MQ7_PIN_PWM_HEATER                    (uint8_t)(9u)            /** PWM pin for MQ7 heater */
#define SENSORS_MQ7_PPM_MIN                           (float)(10)              /** Minimum PPM for MQ7 sensor */
#define SENSORS_MQ7_PPM_MAX                           (float)(1000)            /** Maximum PPM for MQ7 sensor */
#define SENSORS_MQ7_R_ZERO                            (float)(10000)           /** R-zero for MQ7 sensor calibration */
#define SENSORS_MQ7_CALCULATION_CONSTANT_1            (float)(0.5)             /** Constant 1 for MQ7 sensor calculation */
#define SENSORS_MQ7_CALCULATION_CONSTANT_2            (float)(-0.27)           /** Constant 2 for MQ7 sensor calculation */
#define SENSORS_MQ7_CLEAR_AIR_FACTOR                  (float)(9.83)            /** Clear air factor for MQ7 sensor */
#define SENSORS_MQ7_HEATER_LOW_TIMEOUT_MS             (unsigned long)(90000u)  /** Low timeout for MQ7 heater */
#define SENSORS_MQ7_HEATER_HIGH_TIMEOUT_MS            (unsigned long)(60000u)  /** High timeout for MQ7 heater */

/* GY-ML8511 */
#define SENSORS_GY_ML8511_PIN_ANALOG                  (A2)  /** Analog pin for GY-ML8511 sensor */
#define SENSORS_GYML8511_UV_MIN                       (float)(0)   /** Minimum UV for GY-ML8511 sensor */
#define SENSORS_GYML8511_UV_MAX                       (float)(15)  /** Maximum UV for GY-ML8511 sensor */

/* Arduino rain sensor */
#define SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT               /** Flag for analog rain sensor measurement */
#define SENSORS_ARDUINO_RAIN_PIN_ANALOG               (A4)           /** Analog pin for Arduino rain sensor */
#define SENSORS_ARDUINO_RAIN_PIN_DIGITAL              (uint8_t)(4u)  /** Digital pin for Arduino rain sensor (if analog measurement is not defined) */

#endif
