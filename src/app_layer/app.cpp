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

/**
 * @brief Updates the I2C scan result to the next available address.
 * 
 * This function scans the bit-array of detected I2C addresses and updates 
 * `current_i2c_addr` to the next detected address. If no other addresses 
 * are found, it resets to the minimum I2C address.
 * 
 * @param i2cScan_data Pointer to the I2C scan structure.
 * @return bool Returns true if a valid address was found, false if no addresses remain.
 */
static bool updateI2CScanForAllAddressesUpdateNextAddress(i2cScan_reading_ts *reading);
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

task_status_te app_readCurrentRtcTime(output_destination_t output)
{
    data_router_input_data_ts rtc_result = data_router_fetchDataFromInput(INPUT_RTC, RTC_DEFAULT_RTC); // Fetch data from the RTC
    checkForErrors(rtc_result.error_msg);

    if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
    {
        checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, rtc_result)); // Send RTC data to display output and check for errors
    }
    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
    {
        checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, rtc_result)); // Send RTC data to serial console output and check for errors
    }   
    return FINISHED;
}

task_status_te app_readAllI2CAddressesPeriodic(output_destination_t output, i2cScan_reading_context_ts *context)
{
    // Run the I2C scanner if it's not already completed
    if(I2C_SCANER_RUN == context->run_i2c_scanner)
    {
        // Fetch the I2C scan result and check for errors
        context->i2c_scan_return = data_router_fetchDataFromInput(INPUT_I2C_SCAN, I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES);
        checkForErrors(context->i2c_scan_return.error_msg);
        // Mark scanner as stopped after fetching the data
        context->run_i2c_scanner = I2C_SCANER_DONT_RUN;
    }

    // Check if an address update function is assigned
    if(I2CSCAN_NO_ADDRESS_UPDATE_FUNCTION != context->i2c_scan_return.data.input_return.i2cScan_reading.update_i2c_address)
    {
        // Try updating the I2C address (returns true if a valid address is found)
        if(I2CSCAN_ADDRESS_NOT_FOUND != updateI2CScanForAllAddressesUpdateNextAddress(&(context->i2c_scan_return.data.input_return.i2cScan_reading)))
        {
            if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
            {
                checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, context->i2c_scan_return)); // Send I2C scan address to display output and check for errors
            }
            if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
            {
                checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, context->i2c_scan_return)); // Send I2C scan address to serial console output and check for errors
            }

            return NOT_FINISHED;
        }
    }
    // If we have cycled through all I2C addresses and there is no more, indicate that processing is complete
    context->run_i2c_scanner = I2C_SCANER_RUN; // Also, in case if start fails, make it possible to retry again
    return FINISHED; // No update function assigned, just return finished
}

i2cScan_reading_context_ts app_createI2CScanReadingContext()
{
    // Initialize the I2C scan reading context with zeroed-out default values
    i2cScan_reading_context_ts new_i2c_scan_reading_context = {0};

    // Set the function pointer to indicate no address update functionality is available
    new_i2c_scan_reading_context.i2c_scan_return.data.input_return.i2cScan_reading.update_i2c_address = I2CSCAN_NO_ADDRESS_UPDATE_FUNCTION;

    // Mark the scanner as active, meaning it should start scanning when triggered
    new_i2c_scan_reading_context.run_i2c_scanner = I2C_SCANER_RUN;

    // Return the half initialized scan reading context structure
    return new_i2c_scan_reading_context;
}

task_status_te app_readAllI2CAddressesAtOnce(output_destination_t output)
{
    output = filterOutTimeDependentOutputs(output);

    if(NO_OUTPUTS != output) // Check if all outputs are filtered out
    {
        // Fetch the I2C scan result and check for errors
        data_router_input_data_ts i2c_scan_reading_result = data_router_fetchDataFromInput(INPUT_I2C_SCAN, I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES);
        checkForErrors(i2c_scan_reading_result.error_msg);

        // Check if an address update function is assigned
        if(I2CSCAN_NO_ADDRESS_UPDATE_FUNCTION != i2c_scan_reading_result.data.input_return.i2cScan_reading.update_i2c_address)
        {
            // Initialize a loop counter or timeout check to prevent infinite loop
            uint8_t attempt_counter = I2CSCAN_I2C_ADDRESS_MIN;
            // Timeout based on attempts
            while(attempt_counter <= I2CSCAN_I2C_ADDRESS_MAX)
            {
                // Try updating the I2C address (returns true if a valid address is found)
                if(I2CSCAN_ADDRESS_NOT_FOUND != updateI2CScanForAllAddressesUpdateNextAddress(&(i2c_scan_reading_result.data.input_return.i2cScan_reading)))
                {
                    if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
                    {
                        checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, i2c_scan_reading_result)); // Send I2C scan address to display output and check for errors
                    }
                    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
                    {
                        checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, i2c_scan_reading_result)); // Send I2C scan address to serial console output and check for errors
                    }

                    attempt_counter++; // Increment the attempt counter after a successful address update
                }
                else
                {
                    break;
                }
            }
        }
    }

    return FINISHED; // No update function assigned, just return finished
}

task_status_te app_readI2CDeviceStatus(uint8_t device_address, output_destination_t output)
{
    if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES < device_address)
    {
        data_router_input_data_ts i2c_device_status_result = data_router_fetchDataFromInput(INPUT_I2C_SCAN, device_address); // Fetch data from the I2C Scan
        checkForErrors(i2c_device_status_result.error_msg);

        if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
        {
            checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, i2c_device_status_result)); // Send I2C device status data to display output and check for errors
        }
        if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
        {
            checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, i2c_device_status_result)); // Send I2C device status data to serial console output and check for errors
        }   
    }
    return FINISHED; // Return value is used to notify task component
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

static bool updateI2CScanForAllAddressesUpdateNextAddress(i2cScan_reading_ts *reading)
{
    return reading->update_i2c_address(reading);
}
/* *************************************** */