#ifndef SENSORSCONFIG_H
#define SENSORSCONFIG_H

/* SENSOR ID'S - Comment out if sensor is not used */
#define DHT11_TEMPERATURE                     (0u)    
#define DHT11_HUMIDITY                        (1u)
#define BMP280_PRESSURE                       (2u)
#define BMP280_TEMPERATURE                    (3u)
#define BMP280_ALTITUDE                       (4u)
#define BH1750_LUMINANCE                      (5u)
#define MQ135_PPM                             (6u)
#define MQ7_COPPM                             (7u)
#define GYML8511_UV                           (8u)
#define ARDUINORAIN_RAINING                   (9u)
/* ********************************* */


#define SENSORS_INDICATION_NO_MIN        (0)
#define SENSORS_INDICATION_NO_MAX        (0)

/* DHT11 */
#define SENSORS_DHT11_PIN                (2u)
#define SENSORS_DHT11_TEMPERATURE_MIN    (-20)
#define SENSORS_DHT11_TEMPERATURE_MAX    (50)
#define SENSORS_DHT11_HUMIDITY_MIN       (0)
#define SENSORS_DHT11_HUMIDITY_MAX       (100)

/* BMP280 */
#define SENSORS_BMP280_I2C_ADDR          (0x76)
#define SENSORS_BMP280_PRESSURE_MIN      (300)
#define SENSORS_BMP280_PRESSURE_MAX      (1200)
#define SENSORS_BMP280_TEMPERATURE_MIN   (-20)
#define SENSORS_BMP280_TEMPERATURE_MAX   (50)
#define SENSORS_BMP280_ALTITUDE_MIN      (-1000)
#define SENSORS_BMP280_ALTITUDE_MAX      (9000)

/* BH1750 */
#define SENSORS_BH1750_I2C_ADDDR_VCC     (0x5C)
#define SENSORS_BH1750_I2C_ADDDR_GND     (0x23)
#define SENSORS_BH1750_LUMINANCE_MIN     (0)
#define SENSORS_BH1750_LUMINANCE_MAX     (150000)

/* MQ135 */
#define SENSORS_MQ135_PIN_ANALOG         (A0)
#define SENSORS_MQ135_PPM_MIN            (0)
#define SENSORS_MQ135_PPM_MAX            (10000)

/* MQ7 */
#define SENSORS_MQ7_PIN_ANALOG           (A1)
#define SENSORS_MQ7_PPM_MIN              (10)
#define SENSORS_MQ7_PPM_MAX              (1000)  

/* GY-ML8511 */
#define SENSORS_GY_ML8511_PIN_ANALOG     (A2)
#define SENSORS_GYML8511_UV_MIN          (0)
#define SENSORS_GYML8511_UV_MAX          (15)

#endif