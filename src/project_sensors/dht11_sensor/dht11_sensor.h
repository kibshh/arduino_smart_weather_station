#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#include "../../project_configs/project_configs.h"
#include <DHT.h>

/* Pin where the DHT11 sensor is wired */
#define DHT11_SENSOR_PIN          (uint8_t)(2u)

/* Type of used DHT sensor (11 or 22) */
#define DHT11_SENSOR_DHT_TYPE     DHT11

/* Min and Max values */
#define DHT11_SENSOR_TEMPERATURE_MIN  (float)(-20.0f)
#define DHT11_SENSOR_TEMPERATURE_MAX  (float)(50.0f)
#define DHT11_SENSOR_HUMIDITY_MIN     (float)(0.0f)
#define DHT11_SENSOR_HUMIDITY_MAX     (float)(100.0f)

bool Dht11Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index);

#endif