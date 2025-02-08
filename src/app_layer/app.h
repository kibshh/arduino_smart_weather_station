#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "../data_router/data_router.h"
#include "../project_settings.h"
#include "../error_manager/error_manager.h"


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

typedef enum
{
    FINISHED,
    NOT_FINISHED
} task_status_te;

typedef enum
{
    ALL_OUTPUTS,                    // Used to indicate all outputs (works only for sequential readings)
    LCD_DISPLAY,                    // Output option for displays(cannot show all at once)
    SERIAL_CONSOLE,                 // Output option for serial console(can show all at once or sequentially)
    ALL_TIME_INDEPENDENT_OUTPUTS,   // Outputs that can be sent independently of time constraints(all at once)
    ALL_TIME_DEPENDENT_OUTPUTS      // Outputs that require time constraints (e.g., display)
} output_destination_te;

typedef struct
{
    size_t number_of_sensors;
    uint8_t sensor_index;
} sensor_reading_context_ts;

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
task_status_te app_readSpecificSensor(uint8_t sensor_id, output_destination_te output);

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
task_status_te app_readAllSensorsPeriodic(output_destination_te output, sensor_reading_context_ts *context);

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
task_status_te app_readAllSensorsAtOnce(output_destination_te output);

/***
 * Fetches RTC data, and routes the data to defined output(s).
 * @param output The output parameter.
 */
void app_displayCurrentRtcTime(output_destination_te output);

void app_displayAllI2CAddresses(output_destination_te output, bool repeat);

#endif