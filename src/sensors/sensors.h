#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "sensor_library/dht11/dht11.h"
#include "sensor_library/bmp280/bmp280.h"
#include "sensor_library/bh1750/bh1750.h"
#include "sensor_library/gy_ml8511/gy_ml8511.h"
#include "sensor_library/mq135/mq135.h"
#include "sensor_library/mq7/mq7.h"
#include "sensor_library/arduino_rain_sensor/arduino_rain_sensor.h"
#include "sensorsconfig.h"
#include "../error_manager/error_manager.h"

#define SENSORS_NO_SENSORS_CONFIGURED         (0u)
#define SENSORS_NO_INDICATION_FUNCTION        (nullptr)
#define SENSORS_NO_VALUE_FUNCTION             (nullptr)

#define SENSORS_MEASUREMENT_TYPE_VALUE        (0u)
#define SENSORS_MEASUREMENT_TYPE_INDICATION   (1u)


typedef struct
{
  float value;
  uint8_t sensor_id;
  uint8_t measurement_type_switch;
  bool indication;
}sensor_reading_t;

typedef struct
{
  sensor_reading_t sensor_reading;
  error_manager_error_code_te error_code;
} sensor_return_t;

typedef float (*sensor_sensor_value_function_t)();
typedef bool (*sensor_sensor_indication_function_t)();

typedef struct
{
  float min_value;
  float max_value;
  uint8_t sensor_id;
  sensor_sensor_value_function_t sensor_value_function;
  sensor_sensor_indication_function_t sensor_indication_function;
}sensor_sensors_config_t;


void sensors_init();
sensor_return_t sensors_getReading(uint8_t id, uint8_t measurement_type);

#endif