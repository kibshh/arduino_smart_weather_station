#ifndef SENSORS_CATALOG_H
#define SENSORS_CATALOG_H

#include <Arduino.h>

/* SENSOR ID'S - Comment out if sensor is not used */
#define INVALID_SENSOR_ID                     (0u)
#define DHT11_TEMPERATURE                     (1u)    
#define DHT11_HUMIDITY                        (2u)
#define BMP280_PRESSURE                       (3u)
#define BMP280_TEMPERATURE                    (4u)
#define BMP280_ALTITUDE                       (5u)
#define BH1750_LUMINANCE                      (6u)
#define MQ135_PPM                             (7u)
#define MQ7_COPPM                             (8u)
#define GYML8511_UV                           (9u)
#define ARDUINORAIN_RAINING                   (10u)
/* ********************************* */

#endif