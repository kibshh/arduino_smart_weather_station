#include "app_i2c_scan.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Updates the I2C scan result to the next available address.
 * 
 * This function scans the bit-array of detected I2C addresses and updates 
 * `current_i2c_addr` to the next detected address. If no other addresses 
 * are found, it resets to the minimum I2C address.
 * 
 * @param i2c_scan_data Pointer to the I2C scan structure.
 * @return bool Returns true if a valid address was found, false if no addresses remain.
 */
static bool updateI2CScanForAllAddressesUpdateNextAddress(i2c_scan_reading_ts *reading);
/* *************************************** */

/* EXPORTED FUNCTIONS */
task_status_te app_readAllI2CAddressesPeriodic(output_destination_t output, i2c_scan_reading_context_ts *context)
{
    // Run the I2C scanner if it's not already completed
    if(I2C_SCANER_RUN == context->run_i2c_scanner)
    {
        control_device_ts i2c_scanner = {INPUT_I2C_SCAN, I2C_SCAN_MODE_SCAN_FOR_ALL_DEVICES};
        // Fetch the I2C scan result
        context->i2c_scan_return = control_fetchDataFromInput(&i2c_scanner);
        // Handle input errors
        control_error_ts error = {context->i2c_scan_return.error_code, i2c_scanner};
        checkForErrors(&error);
        // Mark scanner as stopped after fetching the data
        context->run_i2c_scanner = I2C_SCANER_DONT_RUN;
    }

    i2c_scan_reading_ts current_reading = context->i2c_scan_return.data.input_return.i2c_scan_reading;
    // Check if an address update function is assigned
    if(I2C_SCAN_NO_ADDRESS_UPDATE_FUNCTION != current_reading.update_i2c_address)
    {
        // Try updating the I2C address (returns true if a valid address is found)
        if(I2C_SCAN_ADDRESS_NOT_FOUND != updateI2CScanForAllAddressesUpdateNextAddress(&current_reading))
        {
            if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
            {
                // Send I2C scan address to display output and check for errors
                sendToOutputAndCheckForErrors(OUTPUT_DISPLAY, &(context->i2c_scan_return.data));
            }
            if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
            {
                // Send I2C scan address to serial console output and check for errors
                sendToOutputAndCheckForErrors(OUTPUT_SERIAL_CONSOLE, &(context->i2c_scan_return.data));
            }

            return NOT_FINISHED;
        }
    }
    // If we have cycled through all I2C addresses and there is no more, indicate that processing is complete
    context->run_i2c_scanner = I2C_SCANER_RUN; // Also, in case if start fails, make it possible to retry again
    return FINISHED; // No update function assigned, just return finished
}

task_status_te app_readAllI2CAddressesAtOnce(output_destination_t output)
{
    output = filterOutTimeDependentOutputs(output);

    if(NO_OUTPUTS != output) // Check if all outputs are filtered out
    {
        control_device_ts i2c_scanner = {INPUT_I2C_SCAN, I2C_SCAN_MODE_SCAN_FOR_ALL_DEVICES};
        // Fetch the I2C scan result
        control_input_data_ts i2c_scan_reading_result = control_fetchDataFromInput(&i2c_scanner);
        // Handle input errors
        control_error_ts error = {i2c_scan_reading_result.error_code, i2c_scanner};
        checkForErrors(&error);

        i2c_scan_reading_ts current_reading = i2c_scan_reading_result.data.input_return.i2c_scan_reading;
        // Check if an address update function is assigned
        if(I2C_SCAN_NO_ADDRESS_UPDATE_FUNCTION != current_reading.update_i2c_address)
        {
            // Initialize a loop counter or timeout check to prevent infinite loop
            uint8_t attempt_counter = I2C_SCAN_I2C_ADDRESS_MIN;
            // Timeout based on attempts
            while(attempt_counter <= I2C_SCAN_I2C_ADDRESS_MAX)
            {
                // Try updating the I2C address (returns true if a valid address is found)
                if(I2C_SCAN_ADDRESS_NOT_FOUND != updateI2CScanForAllAddressesUpdateNextAddress(&current_reading))
                {
                    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
                    {
                        // Send I2C scan address to serial console output and check for errors
                        sendToOutputAndCheckForErrors(OUTPUT_SERIAL_CONSOLE, &(i2c_scan_reading_result.data));
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
    if(I2C_SCAN_MODE_SCAN_FOR_ALL_DEVICES < device_address)
    {
        control_device_ts i2c_scanner = {INPUT_I2C_SCAN, device_address};
        // Fetch the I2C scan result
        control_input_data_ts i2c_scan_reading_result = control_fetchDataFromInput(&i2c_scanner);
        // Handle input errors
        control_error_ts error = {i2c_scan_reading_result.error_code, i2c_scanner};
        checkForErrors(&error);

        if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
        {
            // Send I2C scan address to display output and check for errors
            sendToOutputAndCheckForErrors(OUTPUT_DISPLAY, &(i2c_scan_reading_result.data));
        }
        if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
        {
            // Send I2C scan address to serial console output and check for errors
            sendToOutputAndCheckForErrors(OUTPUT_SERIAL_CONSOLE, &(i2c_scan_reading_result.data));
        }
    }
    return FINISHED; // Return value is used to notify task component
}

i2c_scan_reading_context_ts app_createI2CScanReadingContext()
{
    // Initialize the I2C scan reading context with zeroed-out default values
    i2c_scan_reading_context_ts new_i2c_scan_reading_context = {0};

    // Set the function pointer to indicate no address update functionality is available
    new_i2c_scan_reading_context.i2c_scan_return.data.input_return.i2c_scan_reading.update_i2c_address = I2C_SCAN_NO_ADDRESS_UPDATE_FUNCTION;

    // Mark the scanner as active, meaning it should start scanning when triggered
    new_i2c_scan_reading_context.run_i2c_scanner = I2C_SCANER_RUN;

    // Return the half initialized scan reading context structure
    return new_i2c_scan_reading_context;
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static bool updateI2CScanForAllAddressesUpdateNextAddress(i2c_scan_reading_ts *reading)
{
    return reading->update_i2c_address(reading);
}
/* *************************************** */