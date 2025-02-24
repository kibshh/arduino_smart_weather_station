#ifndef SENSORS_CATALOG_H
#define SENSORS_CATALOG_H

#include <Arduino.h>
#include "../../../../project_settings.h"

/* SENSOR ID'S */
    #define INVALID_SENSOR_ID                     (uint8_t)(0u)

#ifdef DHT11
    #define DHT11_TEMPERATURE                     (uint8_t)(1u)    
    #define DHT11_HUMIDITY                        (uint8_t)(2u)
#endif

#ifdef BMP280
    #define BMP280_PRESSURE                       (uint8_t)(3u)
    #define BMP280_TEMPERATURE                    (uint8_t)(4u)
    #define BMP280_ALTITUDE                       (uint8_t)(5u)
#endif

#ifdef BH1750
    #define BH1750_LUMINANCE                      (uint8_t)(6u)
#endif

#ifdef MQ135
    #define MQ135_PPM                             (uint8_t)(7u)
#endif

#ifdef MQ7
    #define MQ7_COPPM                             (uint8_t)(8u)
#endif

#ifdef GYML8511
    #define GYML8511_UV                           (uint8_t)(9u)
#endif

#ifdef ARDUINORAIN
    #define ARDUINORAIN_RAINING                   (uint8_t)(10u)
#endif
/* ********************************* */

#endif