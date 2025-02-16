#ifndef SENSORSCONFIG_H
#define SENSORSCONFIG_H

#include <Arduino.h>

/* DHT11 */
#define SENSORS_DHT11_PIN                             (2u)   /** Pin for DHT11 sensor */
#define SENSORS_DHT11_TEMPERATURE_MIN                 (-20)  /** Minimum temperature for DHT11 sensor */
#define SENSORS_DHT11_TEMPERATURE_MAX                 (50)   /** Maximum temperature for DHT11 sensor */
#define SENSORS_DHT11_HUMIDITY_MIN                    (0)    /** Minimum humidity for DHT11 sensor */
#define SENSORS_DHT11_HUMIDITY_MAX                    (100)  /** Maximum humidity for DHT11 sensor */

/* BMP280 */
#define SENSORS_BMP280_I2C_ADDR                       (0x76)      /** I2C address for BMP280 sensor */
#define SENSORS_BMP280_PRESSURE_MIN                   (300)       /** Minimum pressure for BMP280 sensor */
#define SENSORS_BMP280_PRESSURE_MAX                   (1200)      /** Maximum pressure for BMP280 sensor */
#define SENSORS_BMP280_TEMPERATURE_MIN                (-20)       /** Minimum temperature for BMP280 sensor */
#define SENSORS_BMP280_TEMPERATURE_MAX                (50)        /** Maximum temperature for BMP280 sensor */
#define SENSORS_BMP280_ALTITUDE_MIN                   (-1000)     /** Minimum altitude for BMP280 sensor */
#define SENSORS_BMP280_ALTITUDE_MAX                   (9000)      /** Maximum altitude for BMP280 sensor */
#define SENSORS_BMP280_LOCAL_SEA_LEVEL_PRESSURE       (1013.25f)  /** Local sea-level pressure for BMP280 sensor */

/* BH1750 */
#define SENSORS_BH1750_I2C_ADDDR_VCC                  (0x5C)    /** I2C address for BH1750 sensor when VCC is high */
#define SENSORS_BH1750_I2C_ADDDR_GND                  (0x23)    /** I2C address for BH1750 sensor when GND is high */
#define SENSORS_BH1750_LUMINANCE_MIN                  (0)       /** Minimum luminance for BH1750 sensor */
#define SENSORS_BH1750_LUMINANCE_MAX                  (150000)  /** Maximum luminance for BH1750 sensor */

/* MQ135 */
#define SENSORS_MQ135_PIN_ANALOG                      (A0)      /** Analog pin for MQ135 sensor */
#define SENSORS_MQ135_PPM_MIN                         (0)       /** Minimum PPM for MQ135 sensor */
#define SENSORS_MQ135_PPM_MAX                         (10000)   /** Maximum PPM for MQ135 sensor */
#define SENSORS_MQ135_PARAMETER_A                     (116.60)  /** Parameter A for MQ135 sensor calibration */
#define SENSORS_MQ135_PARAMETER_B                     (2.77)    /** Parameter B for MQ135 sensor calibration */
#define SENSORS_MQ135_R_ZERO                          (10000)   /** R-zero for MQ135 sensor */

/* MQ7 */
#define SENSORS_MQ7_PIN_ANALOG                        (A1)      /** Analog pin for MQ7 sensor */
#define SENSORS_MQ7_PIN_PWM_HEATER                    (9u)      /** PWM pin for MQ7 heater */
#define SENSORS_MQ7_PPM_MIN                           (10)      /** Minimum PPM for MQ7 sensor */
#define SENSORS_MQ7_PPM_MAX                           (1000)    /** Maximum PPM for MQ7 sensor */
#define SENSORS_MQ7_R_ZERO                            (10000)   /** R-zero for MQ7 sensor calibration */
#define SENSORS_MQ7_CALCULATION_CONSTANT_1            (0.5)     /** Constant 1 for MQ7 sensor calculation */
#define SENSORS_MQ7_CALCULATION_CONSTANT_2            (-0.27)   /** Constant 2 for MQ7 sensor calculation */
#define SENSORS_MQ7_CLEAR_AIR_FACTOR                  (9.83)    /** Clear air factor for MQ7 sensor */
#define SENSORS_MQ7_HEATER_LOW_TIMEOUT_MS             (90000u)  /** Low timeout for MQ7 heater */
#define SENSORS_MQ7_HEATER_HIGH_TIMEOUT_MS            (60000u)  /** High timeout for MQ7 heater */

/* GY-ML8511 */
#define SENSORS_GY_ML8511_PIN_ANALOG                  (A2)  /** Analog pin for GY-ML8511 sensor */
#define SENSORS_GYML8511_UV_MIN                       (0)   /** Minimum UV for GY-ML8511 sensor */
#define SENSORS_GYML8511_UV_MAX                       (15)  /** Maximum UV for GY-ML8511 sensor */

/* Arduino rain sensor */
#define SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT      /** Flag for analog rain sensor measurement */
#define SENSORS_ARDUINO_RAIN_PIN_ANALOG               (A4)  /** Analog pin for Arduino rain sensor */
#define SENSORS_ARDUINO_RAIN_PIN_DIGITAL              (4u)  /** Digital pin for Arduino rain sensor (if analog measurement is not defined) */

#endif
