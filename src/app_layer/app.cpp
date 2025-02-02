#include "app.h"

/* STATIC FUNCTION PROTOTYPES */
/***
 * Updates the sensor index cyclically.
 * @param current_index The current sensor index.
 * @param number_of_sensors The total number of sensors.
 * @return The updated sensor index.
 */
static uint8_t cyclicDisplaySensorsUpdateSensorIndex(uint8_t current_index, size_t number_of_sensors);

/***
 * Checks for errors and handles them if any occur.
 * @param error_status The error status structure.
 */
static void checkForErrors(error_manager_error_ts error_status);
/* *************************************** */

/* EXPORTED FUNCTIONS */
void app_cyclicDisplayAllSensors(uint8_t output)
{
    // Static variables to maintain state across function calls
    static bool is_sensors_len_calculated = NUM_OF_SENSORS_NOT_CALCULATED;
    static size_t number_of_sensors = SENSORS_INTERFACE_NO_SENSORS_CONFIGURED;
    static uint8_t sensor_index = STARTING_SENSOR_INDEX;

    // Calculate the number of sensors only once
    if(NUM_OF_SENSORS_NOT_CALCULATED == is_sensors_len_calculated)
    {
        number_of_sensors = sensors_interface_getSensorsLen();
        is_sensors_len_calculated = NUM_OF_SENSORS_CALCULATED;
    }
    // If sensors are available, process them one by one in a cyclic manner
    if(SENSORS_INTERFACE_NO_SENSORS_CONFIGURED != number_of_sensors)
    {
        // Get sensor ID from the current index
        uint8_t current_sensor_id = sensors_interface_sensorIndexToId(sensor_index);
        // Process only valid sensor IDs
        if(INVALID_SENSOR_ID != current_sensor_id)
        {
            data_router_input_data_ts sensor_result = data_router_fetchDataFromInput(INPUT_SENSORS, current_sensor_id); // Fetch data from the sensor
            error_manager_error_ts error_return; // Shared return struct used to collect return error data from outputs

            if(ALL_OUTPUTS == output || LCD_DISPLAY == output)
            {
                error_return = data_router_routeDataToOutput(OUTPUT_DISPLAY, sensor_result); // Send sensor data to display output
                checkForErrors(error_return);
            }
            if(ALL_OUTPUTS == output || SERIAL_CONSOLE == output)
            {
                error_return = data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, sensor_result); // Send sensor data to serial console output
                checkForErrors(error_return);
            }
            // Update sensor index for the next iteration
            sensor_index = cyclicDisplaySensorsUpdateSensorIndex(sensor_index, number_of_sensors);
        }
    }
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static uint8_t cyclicDisplaySensorsUpdateSensorIndex(uint8_t current_index, size_t number_of_sensors)
{
    current_index++;
    // Reset index if it exceeds the total number of sensors
    if(current_index >= number_of_sensors) 
    {
      current_index = STARTING_SENSOR_INDEX;
    }

    return current_index;
}

static void checkForErrors(error_manager_error_ts error_status)
{
    // If an error occurred, handle it
    if(ERROR_CODE_NO_ERROR != error_status.error_code)
    {
        error_manager_handleError(error_status);
    }
}
/* *************************************** */