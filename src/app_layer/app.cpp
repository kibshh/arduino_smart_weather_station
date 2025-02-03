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
void app_displayAllSensors(uint8_t output, bool repeat)
{
    // Static variables to maintain state across function calls
    static size_t number_of_sensors = sensors_interface_getSensorsLen(); // Calculate the number of sensors only once
    static uint8_t sensor_index = STARTING_SENSOR_INDEX;
    static bool continue_with_displaying = CONTINUE_DISPLAYING_YES;

    if(CONTINUE_DISPLAYING_YES == continue_with_displaying)
    {
        // If sensors are available, process them one by one in a cyclic manner
        if(SENSORS_INTERFACE_NO_SENSORS_CONFIGURED != number_of_sensors)
        {
            // Get sensor ID from the current index
            uint8_t current_sensor_id = sensors_interface_sensorIndexToId(sensor_index);
            // Process only valid sensor IDs
            if(INVALID_SENSOR_ID != current_sensor_id)
            {
                error_manager_error_ts error_return; // Shared return struct used to collect return error data

                data_router_input_data_ts sensor_result = data_router_fetchDataFromInput(INPUT_SENSORS, current_sensor_id); // Fetch data from the sensor
                error_return = sensor_result.error_msg;
                checkForErrors(error_return);

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
                if(STARTING_SENSOR_INDEX == sensor_index && REPEAT_ONCE == repeat)
                {
                    continue_with_displaying = CONTINUE_DISPLAYING_NO;
                }
            }
        }
    }
}

void app_displayCurrentRtcTime(uint8_t output)
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

void app_displayAllI2CAddresses(uint8_t output, bool repeat)
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
        /* Uncomment when error handler is finished */
        // error_manager_handleError(error_status);
    }
}
/* *************************************** */