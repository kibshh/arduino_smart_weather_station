#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "../data_router/data_router.h"
#include "../project_settings.h"
#include "../error_manager/error_manager.h"

// Macro to check if a specific output bit is set in the output variable
// It returns true if the specified bit (bit_mask) is set in the output, otherwise false
#define IS_OUTPUT_INCLUDED(output, bit_mask) ((output & bit_mask) != 0)

#define NUM_OF_SENSORS_NOT_CALCULATED (false) // Flag indicating sensors length has not been calculated
#define NUM_OF_SENSORS_CALCULATED     (true)  // Flag indicating sensors length has been calculated

#define STARTING_SENSOR_INDEX         (0u)  // Initial sensor index for cyclic display

#define REPEAT_CYCLIC                        (true)
#define REPEAT_ONCE                          (false)

#define EXECUTE                       (true)
#define DONT_EXECUTE                  (false)

#define CONTINUE_DISPLAYING_YES                  (true)
#define CONTINUE_DISPLAYING_NO                   (false)

#define TIME_INDEPENDENT_OUTPUT       (true)
#define TIME_DEPENDENT_OUTPUT         (false)

#define I2C_SCANER_RUN                (true)
#define I2C_SCANER_DONT_RUN           (false)

typedef enum
{
    FINISHED,
    NOT_FINISHED
} task_status_te;

// Type for representing and managing output destinations (supports up to 16 different output options)
typedef uint16_t output_destination_t;
// Bitmask macros for output destinations:
// - The first 8 bits (0x00FF) are for time-independent outputs (supports up to 8 outputs at once).
// - The second 8 bits (0xFF00) are for time-dependent outputs (supports up to 8 outputs at once, e.g., display).
// - ALL_OUTPUTS combines both time-independent and time-dependent outputs (16 bits in total).
#define SERIAL_CONSOLE                 (output_destination_t)(1 << 0) // Output option for serial console(can show all at once or sequentially)
// 7 bytes reserved for the future
#define LCD_DISPLAY                    (output_destination_t)(1 << 8) // Output option for displays(cannot show all at once)
// 7 bytes reserved for the future
#define ALL_TIME_INDEPENDENT_OUTPUTS   (output_destination_t)(0x00FF) // Outputs that can be sent independently of time constraints(all at once)
#define ALL_TIME_DEPENDENT_OUTPUTS     (output_destination_t)(0xFF00) // Outputs that require time constraints (e.g., display)
#define ALL_OUTPUTS                    (output_destination_t)(0xFFFF) // Used to indicate all outputs
#define NO_OUTPUTS                     (output_destination_t)(0x0000) // Used to indicate no outputs are set

typedef struct
{
    size_t number_of_sensors;
    uint8_t sensor_index;
} sensor_reading_context_ts;

typedef struct
{
    data_router_input_data_ts i2c_scan_return;
    bool run_i2c_scanner;
} i2cScan_reading_context_ts;

/***
 * Reads data from a specific sensor and routes it to the defined output(s).
 * 
 * This function fetches data from the specified sensor and routes the 
 * retrieved data to one or more output destinations (LCD display, serial console, or both).
 * Any errors encountered during data retrieval or routing are handled internally.
 * 
 * @param sensor_id The ID of the sensor to be read.
 * @param output The output destination (LCD, serial console, or both).
 * @return task_status_te Returns FINISHED to notify the task component.
 */
task_status_te app_readSpecificSensor(uint8_t sensor_id, output_destination_t output);

/**
 * @brief Reads all configured sensors in a cyclic manner and processes their data.
 *
 * This function iterates through all available sensors, reads their data, and 
 * routes the data to the specified output destination. The reading process is 
 * performed cyclically, meaning that each call to this function processes the 
 * next sensor in sequence until all sensors have been read.
 *
 * @param output The destination where sensor data should be routed (e.g., LCD, Serial Console).
 * @param context Pointer to the sensor reading context, which maintains the sensor index 
 *                and total sensor count to enable cyclic processing.
 *
 * @return task_status_te Returns:
 *         - `FINISHED` when all sensors have been read in the current cycle.
 *         - `NOT_FINISHED` if there are more sensors left to process.
 */
task_status_te app_readAllSensorsPeriodic(output_destination_t output, sensor_reading_context_ts *context);

/**
 * @brief Creates and initializes a new sensor reading context.
 *
 * This function creates a new `sensor_reading_context_ts` structure and initializes it with:
 * - The total number of sensors available, fetched using `sensors_interface_getSensorsLen()`.
 * - The starting sensor index, which is set to `STARTING_SENSOR_INDEX` to begin reading from the first sensor.
 *
 * @return sensor_reading_context_ts The initialized sensor reading context containing:
 *         - `number_of_sensors`: Total number of configured sensors.
 *         - `sensor_index`: Starting index for the cyclic sensor reading process.
 */
sensor_reading_context_ts app_createNewSensorsReadingContext();

/**
 * @brief Reads all sensors at once and sends their data to the specified output.
 *
 * This function iterates over all configured sensors and fetches their data using
 * `app_readSpecificSensor`. It sends the data to the provided output destination (e.g., LCD, serial console).
 * 
 * The function performs the following:
 * - Checks if any sensors are configured.
 * - Iterates over all sensor indices, fetching data and routing it to the output.
 * 
 * @param output The destination output where sensor data will be sent (e.g., LCD_DISPLAY, SERIAL_CONSOLE, etc.).
 * 
 * @return task_status_te Always returns FINISHED, indicating that all sensors were processed.
 */
task_status_te app_readAllSensorsAtOnce(output_destination_t output);

/***
 * Retrieves the current RTC time and sends it to the specified output destinations.
 * @param output The output destinations where the RTC time should be displayed (e.g., LCD display, serial console).
 * @return FINISHED after processing.
 *
 * This function fetches the current time from the RTC module and routes the data 
 * to the selected outputs. It checks for errors in data retrieval and output routing.
 */
task_status_te app_displayCurrentRtcTime(output_destination_t output);

/**
 * @brief Periodically scans and displays I2C addresses.
 *
 * This function runs the I2C scanner periodically and updates the display or console with 
 * detected I2C addresses. If no more addresses are found, it marks the scanning process as completed.
 *
 * @param output The output destination (LCD display, serial console, etc.).
 * @param context The I2C scan reading context containing the scan data.
 * @return task_status_te - `NOT_FINISHED` if there are more addresses to process, `FINISHED` otherwise.
 */
task_status_te app_displayAllI2CAddressesPeriodic(output_destination_t output, i2cScan_reading_context_ts *context);

/**
 * @brief Initializes and returns a new I2C scan reading context.
 *
 * This function initializes the I2C scan reading context with default values.
 * The function pointer `update_i2c_address` is explicitly set to `I2CSCAN_NO_ADDRESS_UPDATE_FUNCTION`,
 * which serves as the key check for determining if address updates should occur.
 *
 * @return i2cScan_reading_context_ts - A half initialized I2C scan reading context.
 */
i2cScan_reading_context_ts app_createI2CScanReadingContext();

#endif