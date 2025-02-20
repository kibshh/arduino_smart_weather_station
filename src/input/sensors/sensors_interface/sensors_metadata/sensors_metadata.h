#ifndef SENSORS_METADATA_H
#define SENSORS_METADATA_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "sensors_catalog.h"

/* Indicates that no sensor metadata is configured */
#define SENSORS_METADATA_NO_SENSORS_CONFIGURED         (size_t)(0u)
/* The index of the first sensor in the metadata configuration */
#define SENSORS_METADATA_FIRST_SENSOR_INDEX            (uint8_t)(0u)

/* Metadata retrieve success status codes */
#define SENSORS_METADATA_RETRIEVE_FAILED               (bool)(false)
#define SENSORS_METADATA_RETRIEVE_SUCCESS              (bool)(true)

/* Measurement type for sensors providing float values */
#define SENSORS_MEASUREMENT_TYPE_VALUE                 (uint8_t)(0u)
/* Measurement type for sensors providing indications */
#define SENSORS_MEASUREMENT_TYPE_INDICATION            (uint8_t)(1u)

/* Macros for defining number of decimals based on sensor type */
#define SENSORS_DISPLAY_0_DECIMALS    (uint8_t)(0u)
#define SENSORS_DISPLAY_1_DECIMAL     (uint8_t)(1u)
#define SENSORS_DISPLAY_2_DECIMALS    (uint8_t)(2u)
#define SENSORS_DISPLAY_3_DECIMALS    (uint8_t)(3u)
#define SENSORS_DISPLAY_4_DECIMALS    (uint8_t)(4u)
#define SENSORS_DISPLAY_5_DECIMALS    (uint8_t)(5u)
#define SENSORS_DISPLAY_6_DECIMALS    (uint8_t)(6u)

/* Macros for defining the number of letters displayed for sensor names */
/* These macros help save space when displaying sensor names on limited character outputs based on the sensor type */
#define SENSORS_DISPLAY_0_LETTERS    (uint8_t)(0u)
#define SENSORS_DISPLAY_1_LETTER     (uint8_t)(1u)
#define SENSORS_DISPLAY_2_LETTERS    (uint8_t)(2u)
#define SENSORS_DISPLAY_3_LETTERS    (uint8_t)(3u)
#define SENSORS_DISPLAY_4_LETTERS    (uint8_t)(4u)
#define SENSORS_DISPLAY_5_LETTERS    (uint8_t)(5u)
#define SENSORS_DISPLAY_6_LETTERS    (uint8_t)(6u)
#define SENSORS_DISPLAY_7_LETTERS    (uint8_t)(7u)
#define SENSORS_DISPLAY_8_LETTERS    (uint8_t)(8u)
#define SENSORS_DISPLAY_9_LETTERS    (uint8_t)(9u)
#define SENSORS_DISPLAY_10_LETTERS   (uint8_t)(10u)
#define SENSORS_DISPLAY_11_LETTERS   (uint8_t)(11u)
#define SENSORS_DISPLAY_12_LETTERS   (uint8_t)(12u)

/**
 * @brief Structure representing metadata for a sensor.
 *
 * This structure defines the static properties of a sensor, such as its type, 
 * measurement unit, unique ID, and display-related attributes. It is used to 
 * store descriptive information about sensors that does not change during runtime.
 * Used in other components like Display, Serial Console etc.
 */
typedef struct
{
  const char* sensor_type;            // Type of the sensor (e.g., Temperature, Pressure, etc.).
  const char* measurement_unit;       // Unit of measurement for the sensor (e.g., C, Pa, etc.).
  uint8_t sensor_id;                  // Unique identifier for the sensor. Used to reference the sensor. From config file.
  uint8_t measurement_type;           // Type of measurement the sensor provides (e.g., value, indication).
  uint8_t num_of_decimals;            // Number of decimal places for the sensor's measurement values.
  uint8_t display_num_of_letters;     // Number of letters to display for the sensor name in compact formats.
} sensors_metadata_catalog_ts;
/* ***************************************** */

/**
 * @brief Retrieves a sensor's metadata from the catalog based on its ID.
 * 
 * This function looks up a sensor by its unique identifier in the catalog.
 * If found, it copies the sensor's metadata into the provided structure.
 * 
 * @param id The ID of the sensor to retrieve.
 * @param current_sensor Pointer to the structure where the sensor metadata will be stored.
 * @return bool `true` if the sensor was found and metadata was copied, `false` otherwise.
 */
bool sensors_metadata_getSensorFromCatalog(uint8_t id, sensors_metadata_catalog_ts *current_sensor);

/**
 * @brief Retrieves the number of sensors in the metadata catalog.
 * 
 * This function calculates the total number of sensors in the catalog.
 * 
 * @return size_t The total number of sensors.
 */
size_t sensors_metadata_getSensorsLen();

/**
 * @brief Converts a sensor index to its corresponding sensor ID.
 *
 * This function takes an index within the sensor metadata configuration array and 
 * retrieves the corresponding sensor ID. Returns an invalid ID if the 
 * index is out of bounds or no sensors are configured.
 *
 * @param index The index of the sensor in the configuration array.
 * @return uint8_t The sensor ID corresponding to the index, or an invalid ID if the index is invalid.
 */
uint8_t sensors_metadata_sensorIndexToId(uint8_t index);

#endif