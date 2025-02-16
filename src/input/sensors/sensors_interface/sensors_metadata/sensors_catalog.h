#ifndef SENSORS_CATALOG_H
#define SENSORS_CATALOG_H

#include <Arduino.h>

/* SENSOR ID'S - Comment out if sensor is not used */
#define INVALID_SENSOR_ID                     (uint8_t)(0u)
#define DHT11_TEMPERATURE                     (uint8_t)(1u)    
#define DHT11_HUMIDITY                        (uint8_t)(2u)
#define BMP280_PRESSURE                       (uint8_t)(3u)
#define BMP280_TEMPERATURE                    (uint8_t)(4u)
#define BMP280_ALTITUDE                       (uint8_t)(5u)
#define BH1750_LUMINANCE                      (uint8_t)(6u)
#define MQ135_PPM                             (uint8_t)(7u)
#define MQ7_COPPM                             (uint8_t)(8u)
#define GYML8511_UV                           (uint8_t)(9u)
#define ARDUINORAIN_RAINING                   (uint8_t)(10u)
/* ********************************* */

#endif