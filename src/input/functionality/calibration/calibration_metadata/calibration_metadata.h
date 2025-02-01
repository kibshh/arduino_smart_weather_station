#ifndef CALIBRATION_METADATA_H
#define CALIBRATION_METADATA_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include "../../../sensors/sensorsconfig.h"

#define CALIBRATION_METADATA_NO_CALIBS_CONFIGURED          (0u)      // Indicates that no calibration metadata is configured
#define CALIBRATION_METADATA_FIRST_CALIB_INDEX             (0u)      // The index of the first calibration in the metadata configuration

// Metadata retrieve success status codes
#define CALIBRATION_METADATA_RETRIEVE_FAILED               (false)
#define CALIBRATION_METADATA_RETRIEVE_SUCCESS              (true)

#define CALIBRATION_MULTIPLE_MEASUREMENTS                  (0u)      // Calibration type for taking multiple measurements at defined time intervals
#define CALIBRATION_SINGLE_MEASUREMENT                     (1u)      // Calibration type for taking single calibration measurement

// Macros for defining number of decimals based on calibration config
#define CALIBRATION_DISPLAY_0_DECIMALS    (0u)
#define CALIBRATION_DISPLAY_1_DECIMAL     (1u)
#define CALIBRATION_DISPLAY_2_DECIMALS    (2u)
#define CALIBRATION_DISPLAY_3_DECIMALS    (3u)
#define CALIBRATION_DISPLAY_4_DECIMALS    (4u)
#define CALIBRATION_DISPLAY_5_DECIMALS    (5u)
#define CALIBRATION_DISPLAY_6_DECIMALS    (6u)

// Macros for defining the number of letters displayed for calibration names.
// These macros help save space when displaying calibration names on limited character outputs based on the sensor type.
#define CALIBRATION_DISPLAY_0_LETTERS    (0u)
#define CALIBRATION_DISPLAY_1_LETTER     (1u)
#define CALIBRATION_DISPLAY_2_LETTERS    (2u)
#define CALIBRATION_DISPLAY_3_LETTERS    (3u)
#define CALIBRATION_DISPLAY_4_LETTERS    (4u)
#define CALIBRATION_DISPLAY_5_LETTERS    (5u)
#define CALIBRATION_DISPLAY_6_LETTERS    (6u)
#define CALIBRATION_DISPLAY_7_LETTERS    (7u)
#define CALIBRATION_DISPLAY_8_LETTERS    (8u)
#define CALIBRATION_DISPLAY_9_LETTERS    (9u)
#define CALIBRATION_DISPLAY_10_LETTERS   (10u)
#define CALIBRATION_DISPLAY_11_LETTERS   (11u)
#define CALIBRATION_DISPLAY_12_LETTERS   (12u)

/**
 * @brief Structure representing metadata for a calibration process.
 *
 * This structure defines the static properties of a calibration, such as its type, 
 * measurement unit, unique ID, and display-related attributes. It is used to 
 * store descriptive information about calibration processes that do not change during runtime.
 * Used in other components like Display, Serial Console, etc.
 */
typedef struct
{
    const char* calibration_type;       // Type of calibration (e.g. MQ7 R0 resistance).
    const char* calibration_unit;       // Unit of measurement for the calibration process (e.g., C, Pa, etc.).
    uint8_t sensor_id;                  // Unique identifier of the sensor.
    uint8_t num_of_measurements_type;   // Specifies whether the calibration involves a single or multiple measurements.
    uint8_t num_of_decimals;            // Number of decimal places for the calibration values.
    uint8_t display_num_of_letters;     // Number of letters to display for the calibration name in compact formats.
} calibration_metadata_ts;

/**
 * @brief Retrieves a calibs's metadata from the catalog based on sensor ID.
 * 
 * This function looks up a calibration by its sensor ID in the catalog.
 * If found, it copies the calibration's metadata into the provided structure.
 * 
 * @param id The ID of the sensor to retrieve.
 * @param current_calibration Pointer to the structure where the calib metadata will be stored.
 * @return bool `true` if the calib was found and metadata was copied, `false` otherwise.
 */
bool calibration_metadata_getData(uint8_t id, calibration_metadata_ts *current_calibration);

/**
 * @brief Retrieves the number of calibration configurations in the metadata catalog.
 * 
 * This function calculates the total number of calibration configurations in the catalog.
 * 
 * @return size_t The total number of calibration configurations.
 */
size_t calibration_metadata_getDataLen();

/**
 * @brief Converts a sensor index to its corresponding sensor ID.
 *
 * This function takes an index within the calib metadata configuration array and 
 * retrieves the corresponding sensor ID. Returns an invalid ID if the 
 * index is out of bounds or no calibs are configured.
 *
 * @param index The index of the sensor in the configuration array.
 * @return uint8_t The sensor ID corresponding to the index, or an invalid ID if the index is invalid.
 */
uint8_t calibration_metadata_sensorIndexToId(uint8_t index);

#endif