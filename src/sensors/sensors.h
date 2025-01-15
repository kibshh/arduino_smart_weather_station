#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "sensor_library/dht11/dht11.h"
#include "sensor_library/bmp280/bmp280.h"
#include "sensor_library/bh1750/bh1750.h"
#include "sensor_library/gy_ml8511/gy_ml8511.h"
#include "sensor_library/mq135/mq135.h"
#include "sensor_library/mq7/mq7.h"
#include "sensor_library/arduino_rain_sensor/arduino_rain_sensor.h"
#include "sensorsconfig.h"

#define SENSORS_NO_SENSORS_CONFIGURED    (0u)
#define SENSORS_NO_INDICATION_FUNCTION   (nullptr)
#define SENSORS_NO_VALUE_FUNCTION        (nullptr)

typedef enum
{
  SENSORS_VALUE,
  SENSORS_INDICATION
}sensor_measurement_type_te;

typedef enum
{
  DHT11_TEMPERATURE = 0,
  DHT11_HUMIDITY,
  BMP280_PRESSURE,
  BMP280_TEMPERATURE,
  BMP280_ALTITUDE,
  BH1750_LUMINANCE,
  MQ135_PPM,
  MQ7_COPPM,
  GYML8511_UV,
  ARDUINORAIN_RAINING
}sensor_id_te;

typedef struct
{
  float value;
  sensor_measurement_type_te measurement_type_switch;
  boolean success;
  boolean indication;
}sensor_reading_t;

typedef float (*sensor_sensor_value_function_t)();
typedef bool (*sensor_sensor_indication_function_t)();

typedef struct
{
  float min_value;
  float max_value;
  sensor_id_te sensor_id;
  sensor_sensor_value_function_t sensor_value_function;
  sensor_sensor_indication_function_t sensor_indication_function;
}sensor_sensors_config_t;


void sensors_init();
sensor_reading_t sensors_getReading(sensor_id_te id, sensor_measurement_type_te measurement_type);

#endif