#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "sensorsconfig.h"
#include "../input_types.h"
#if defined(DHT11_TEMPERATURE) || defined(DHT11_HUMIDITY)
#include "sensor_library/dht11/dht11.h"
#endif
#if defined(BMP280_PRESSURE) || defined(BMP280_TEMPERATURE) || defined(BMP280_ALTITUDE)
#include "sensor_library/bmp280/bmp280.h"
#endif
#if defined(BH1750_LUMINANCE)
#include "sensor_library/bh1750/bh1750.h"
#endif
#if defined(GYML8511_UV)
#include "sensor_library/gy_ml8511/gy_ml8511.h"
#endif
#if defined(MQ135_PPM)
#include "sensor_library/mq135/mq135.h"
#endif
#if defined(MQ7_COPPM)
#include "sensor_library/mq7/mq7.h"
#endif
#if defined(ARDUINORAIN_RAINING)
#include "sensor_library/arduino_rain_sensor/arduino_rain_sensor.h"
#endif

#define SENSORS_NO_SENSORS_CONFIGURED         (0u)
#define SENSORS_NO_INDICATION_FUNCTION        (nullptr)
#define SENSORS_NO_VALUE_FUNCTION             (nullptr)

#define SENSORS_MEASUREMENT_TYPE_VALUE        (0u)
#define SENSORS_MEASUREMENT_TYPE_INDICATION   (1u)

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


error_manager_error_code_te sensors_init();
sensor_return_ts sensors_getReading(uint8_t id);
size_t sensors_getSensorsLen();
uint8_t sensors_sensorIndexToId(uint8_t index);

/**
 * @brief Handles periodic tasks for sensors in the main loop.
 *
 * This function manages sensor-related operations, including handling 
 * time-based processes and calibration routines. It should be called 
 * periodically in the main loop with the current time in milliseconds.
 *
 * @param current_millis The current time in milliseconds (e.g., from millis()).
 */
void sensors_loop(unsigned long current_millis);

#endif