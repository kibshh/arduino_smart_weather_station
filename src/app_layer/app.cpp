#include "app.h"

/* STATIC FUNCTION PROTOTYPES */
/***
 * Updates the sensor index cyclically.
 * @param current_index The current sensor index.
 * @param number_of_sensors The total number of sensors.
 * @return The updated sensor index.
 */
static uint8_t readAllSensorsPeriodicUpdateSensorIndex(uint8_t current_index, size_t number_of_sensors);

/***
 * Checks for errors and handles them if any occur.
 * @param error_status The error status structure.
 */
static void checkForErrors(error_manager_error_ts error_status);

/***
 * Filters out time-dependent outputs from the provided output mask.
 * Only time-independent outputs are retained.
 * @param output The original output destination mask to be filtered.
 * @return A mask containing only time-independent outputs.
 ***/
static output_destination_t filterOutTimeDependentOutputs(output_destination_t output);
/* *************************************** */

/* EXPORTED FUNCTIONS */
task_status_te app_readSpecificSensor(uint8_t sensor_id, output_destination_t output)
{
    data_router_input_data_ts sensor_reading_result = data_router_fetchDataFromInput(INPUT_SENSORS, sensor_id); // Fetch data from the sensor
    checkForErrors(sensor_reading_result.error_msg);
    if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
    {
        checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, sensor_reading_result)); // Send sensor data to display output
    }
    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
    {
        checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, sensor_reading_result)); // Send sensor data to serial console output
    }
    return FINISHED; // Return value is used to notify task component
}

task_status_te app_readAllSensorsPeriodic(output_destination_t output, sensor_reading_context_ts *context)
{
    // If sensors are available, process them one by one in a cyclic manner
    if(SENSORS_INTERFACE_NO_SENSORS_CONFIGURED != context->number_of_sensors)
    {
        // Get sensor ID from the current index
        uint8_t current_sensor_id = sensors_interface_sensorIndexToId(context->sensor_index);
        // Process only valid sensor IDs
        if(INVALID_SENSOR_ID != current_sensor_id)
        {
            (void)app_readSpecificSensor(current_sensor_id, output);
        }
        // Update sensor index for the next iteration
        context->sensor_index = readAllSensorsPeriodicUpdateSensorIndex(context->sensor_index, context->number_of_sensors);
    }

    // If we have cycled through all sensors, indicate that processing is complete
    return (STARTING_SENSOR_INDEX == context->sensor_index) ? FINISHED : NOT_FINISHED;
}

sensor_reading_context_ts app_createNewSensorsReadingContext()
{
    sensor_reading_context_ts new_sensor_reading_context = {sensors_interface_getSensorsLen(), STARTING_SENSOR_INDEX};
    return new_sensor_reading_context;
}

task_status_te app_readAllSensorsAtOnce(output_destination_t output)
{
    output = filterOutTimeDependentOutputs(output);
    if(NO_OUTPUTS != output) // Check if all outputs are filtered out
    {
        size_t number_of_sensors = sensors_interface_getSensorsLen();

        // If there are sensors configured, process each one
        if(SENSORS_INTERFACE_NO_SENSORS_CONFIGURED != number_of_sensors)
        {
            // Loop through all sensor indices and process each one
            for (uint8_t sensor_index = STARTING_SENSOR_INDEX; sensor_index < number_of_sensors; sensor_index++)
            {
                uint8_t current_sensor_id = sensors_interface_sensorIndexToId(sensor_index);
                // Only process valid sensor IDs
                if(INVALID_SENSOR_ID != current_sensor_id)
                {
                    (void)app_readSpecificSensor(current_sensor_id, output);
                }
            }
        }   
    }

    return FINISHED; // Return FINISHED since all sensors are processed
}

void app_displayCurrentRtcTime(output_destination_t output)
{
    error_manager_error_ts error_return; // Shared return struct used to collect return error data

    data_router_input_data_ts rtc_result = data_router_fetchDataFromInput(INPUT_RTC, RTC_DEFAULT_RTC); // Fetch data from the RTC
    error_return = rtc_result.error_msg;
    checkForErrors(error_return);

    if(ALL_OUTPUTS == output || LCD_DISPLAY == output)
    {
        error_return = data_router_routeDataToOutput(OUTPUT_DISPLAY, rtc_result); // Send RTC data to display output
        checkForErrors(error_return);
    }
    if(ALL_OUTPUTS == output || SERIAL_CONSOLE == output)
    {
        error_return = data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, rtc_result); // Send RTC data to serial console output
        checkForErrors(error_return);
    }    
}

void app_displayAllI2CAddresses(output_destination_t output, bool repeat)
{
    static bool continue_with_displaying = CONTINUE_DISPLAYING_YES;

    if(CONTINUE_DISPLAYING_YES == continue_with_displaying)
    {
        error_manager_error_ts error_return; // Shared return struct used to collect return error data

        data_router_input_data_ts i2c_scan_result = data_router_fetchDataFromInput(INPUT_I2C_SCAN, I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES);
        error_return = i2c_scan_result.error_msg;
        checkForErrors(error_return);

        if(ALL_OUTPUTS == output || LCD_DISPLAY == output)
        {
            error_return = data_router_routeDataToOutput(OUTPUT_DISPLAY, i2c_scan_result); // Send I2C scan address to display output
            checkForErrors(error_return);
        }
        if(ALL_OUTPUTS == output || SERIAL_CONSOLE == output)
        {
            error_return = data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, i2c_scan_result); // Send I2C scan address to serial console output
            checkForErrors(error_return);
        }

        i2cScan_reading_ts current_reading = i2c_scan_result.data.input_return.i2cScan_reading;
        current_reading.update_i2c_address(&current_reading);
        if(I2CSCAN_I2C_ADDRESS_MIN == current_reading.current_i2c_addr && REPEAT_ONCE == repeat)
        {
            continue_with_displaying = CONTINUE_DISPLAYING_NO;
        }
    }
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static uint8_t readAllSensorsPeriodicUpdateSensorIndex(uint8_t current_index, size_t number_of_sensors)
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
        /* Uncomment when error handler is finished */
        // error_manager_handleError(error_status);
    }
}

static output_destination_t filterOutTimeDependentOutputs(output_destination_t output)
{
    return (output & ALL_TIME_INDEPENDENT_OUTPUTS);
}
/* *************************************** */