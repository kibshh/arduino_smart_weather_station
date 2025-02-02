#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "../input_types.h"
#include "sensors_interface/sensors_interface.h"
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

#define SENSORS_FIRST_SENSOR_INDEX            (0u)      // The index of the first sensor in the configuration

#define SENSORS_NO_INDICATION_FUNCTION        (nullptr) // Placeholder for sensors without an indication function
#define SENSORS_NO_VALUE_FUNCTION             (nullptr) // Placeholder for sensors without a value function

// Placeholders for min_value and max_value in indication sensors
#define SENSORS_INDICATION_NO_MIN             (0)       
#define SENSORS_INDICATION_NO_MAX             (0)

#define SENSORS_SENSOR_CONFIGURED             (true)    // Flag indicating the sensor is configured in functional catalog

typedef float (*sensors_sensor_value_function_t)();     // Function pointer type for sensors returning a float value
typedef bool (*sensors_sensor_indication_function_t)(); // Function pointer type for sensors returning a bool indication

/**
 * @brief Defines the functional properties of a sensor.
 * 
 * Includes valid measurement ranges, optional function pointers for sensor readings, 
 * and a unique identifier for referencing the sensor.
 */
typedef struct
{
  float min_value; // The minimum valid value for the sensor's reading. Values below this are considered invalid.
  float max_value; // The maximum valid value for the sensor's reading. Values above this are considered invalid.
  sensors_sensor_value_function_t sensor_value_function; // Function pointer for obtaining a numerical reading from the sensor. Optional.
  sensors_sensor_indication_function_t sensor_indication_function; // Function pointer for obtaining a boolean status/indication from the sensor. Optional.
  uint8_t sensor_id; // Unique identifier for the sensor. Used to reference the sensor. From config file.
} sensors_functional_catalog_ts;

/**
 * @brief Initializes all configured sensors based on their compile-time definitions.
 *
 * This function checks which sensors are defined at compile time and initializes them.
 * If an initialization function fails, an appropriate error code is returned.
 *
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: All sensors initialized successfully.
 *
 * @note
 * - Each sensor has its own initialization function.
 * - Sensors are included based on preprocessor definitions.
 */
error_manager_error_code_te sensors_init();

/**
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
 **/
sensor_return_ts sensors_getReading(uint8_t id);

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