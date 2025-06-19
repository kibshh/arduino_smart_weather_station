#ifndef DHT11_SENSOR_H
#define DHT11_SENSOR_H

#include "../../project_configs/project_configs.h"
#include <DHT.h>

/* Pin where the DHT11 sensor is wired */
#define DHT11_SENSOR_PIN          (uint8_t)(2u)

/* Type of used DHT sensor (11 or 22) */
#define DHT11_SENSOR_DHT_TYPE     DHT11

/* Min and Max values */
#define DHT11_SENSOR_TEMPERATURE_MIN  (int32_t)(-20)
#define DHT11_SENSOR_TEMPERATURE_MAX  (int32_t)(50)
#define DHT11_SENSOR_HUMIDITY_MIN     (int32_t)(0)
#define DHT11_SENSOR_HUMIDITY_MAX     (int32_t)(100)

bool Dht11Sensor_Init();
bool Dht11Sensor_ReadTemperature();
bool Dht11Sensor_ReadHumidity();

#endif