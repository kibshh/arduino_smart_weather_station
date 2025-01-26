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

#define SENSORS_MINIMUM_INDEX                 (0u)      // The minimum valid index for sensors in the configuration
#define SENSORS_INVALID_INDEX                 (255u)    // An invalid index value used as a default or error indicator
#define SENSORS_FIRST_SENSOR_INDEX            (0u)      // The index of the first sensor in the configuration

#define SENSORS_NO_SENSORS_CONFIGURED         (0u)      // Indicates that no sensors are configured
#define SENSORS_NO_INDICATION_FUNCTION        (nullptr) // Placeholder for sensors without an indication function
#define SENSORS_NO_VALUE_FUNCTION             (nullptr) // Placeholder for sensors without a value function

#define SENSORS_MEASUREMENT_TYPE_VALUE        (0u)      // Measurement type for sensors providing float values
#define SENSORS_MEASUREMENT_TYPE_INDICATION   (1u)      // Measurement type for sensors providing indications


typedef float (*sensor_sensor_value_function_t)();     // Function pointer type for sensors returning a float value
typedef bool (*sensor_sensor_indication_function_t)(); // Function pointer type for sensors returning a bool indication

/**
 * @brief Structure representing the configuration for a single sensor.
 * 
 * This structure is used to define the properties and functions associated with a sensor. 
 * Each sensor is identified by a unique ID and has optional functions to provide its value 
 * or indication. Additionally, valid ranges for the sensor's readings can be specified (in case of value sensor).
 */
typedef struct
{
  float min_value; // The minimum valid value for the sensor's reading. Values below this are considered invalid.
  float max_value; // The maximum valid value for the sensor's reading. Values above this are considered invalid.
  uint8_t sensor_id; // Unique identifier for the sensor. Used to map the sensor in configurations or operations.
  sensor_sensor_value_function_t sensor_value_function; // Function pointer for obtaining a numerical reading from the sensor. Optional.
  sensor_sensor_indication_function_t sensor_indication_function; // Function pointer for obtaining a boolean status/indication from the sensor. Optional.
} sensor_sensors_config_t;


error_manager_error_code_te sensors_init();

/***
 * Retrieves a sensor reading based on the provided sensor ID.
 * Handles both value-based and indication-based sensor measurements.
 * Validates sensor data against configured thresholds.
 *
 * @param id The sensor ID for which the reading is requested.
 * 
 * @return A `sensor_return_ts` structure containing:
 *         - Sensor reading (value or indication).
 *         - Error code indicating success or failure:
 *           - ERROR_CODE_NO_ERROR: Reading successful.
 *           - ERROR_CODE_SENSORS_NO_SENSORS_CONFIGURED: No sensors are configured.
 *           - ERROR_CODE_SENSORS_SENSOR_NOT_FOUND: Sensor ID is not found in the configuration.
 *           - ERROR_CODE_SENSORS_ABNORMAL_VALUE: Sensor value is outside configured thresholds.
 *           - ERROR_CODE_SENSORS_INVALID_VALUE_FROM_SENSOR: Sensor returned an invalid value.
 *           - ERROR_CODE_SENSORS_MEASUREMENT_TYPE_MISSING_FUNCTION: No valid function for the sensor.
 *
 * @note The function verifies whether the requested sensor ID exists in the configuration.
 *       If the sensor ID is valid, it invokes the appropriate function for the sensor 
 *       (either value-based or indication-based).
 ***/
sensor_return_ts sensors_getReading(uint8_t id);

/**
 * @brief Retrieves the number of configured sensors.
 *
 * This function calculates and returns the total number of configured 
 * sensors by evaluating the size of the configuration array. Returns 
 * a default value if no sensors are configured.
 *
 * @return size_t The number of configured sensors or a default value 
 * if none are configured.
 */
size_t sensors_getSensorsLen();

/**
 * @brief Converts a sensor index to its corresponding sensor ID.
 *
 * This function takes an index within the sensor configuration array and 
 * retrieves the corresponding sensor ID. Returns an invalid ID if the 
 * index is out of bounds or no sensors are configured.
 *
 * @param index The index of the sensor in the configuration array.
 * @return uint8_t The sensor ID corresponding to the index, or an invalid ID if the index is invalid.
 */
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