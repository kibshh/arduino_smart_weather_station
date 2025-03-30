#ifndef SENSORS_CATALOG_H
#define SENSORS_CATALOG_H

#include <Arduino.h>
#include "../../../../project_settings.h"

/* SENSOR ID'S */
    #define INVALID_SENSOR_ID                     (uint8_t)(0u)

#ifdef DHT11_COMPONENT
    #define DHT11_TEMPERATURE                     (uint8_t)(1u)    
    #define DHT11_HUMIDITY                        (uint8_t)(2u)
#endif

#ifdef BMP280_COMPONENT
    #define BMP280_PRESSURE                       (uint8_t)(3u)
    #define BMP280_TEMPERATURE                    (uint8_t)(4u)
    #define BMP280_ALTITUDE                       (uint8_t)(5u)
#endif

#ifdef BH1750_COMPONENT
    #define BH1750_LUMINANCE                      (uint8_t)(6u)
#endif

#ifdef MQ135_COMPONENT
    #define MQ135_PPM                             (uint8_t)(7u)
    #define MQ135_CALIBRATION_RESISTANCE          (uint8_t)(8u)
#endif

#ifdef MQ7_COMPONENT
    #define MQ7_COPPM                             (uint8_t)(9u)
    #define MQ7_CALIBRATION_RESISTANCE            (uint8_t)(10u)
#endif

#ifdef GYML8511_COMPONENT
    #define GYML8511_UV                           (uint8_t)(11u)
#endif

#ifdef ARDUINORAIN_COMPONENT
    #define ARDUINORAIN_RAINING                   (uint8_t)(12u)
#endif
/* ********************************* */

#endif