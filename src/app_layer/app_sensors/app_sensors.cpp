#include "app_sensors.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Updates the sensor index for periodic readings.
 *
 * Increments the current sensor index and resets it if it exceeds 
 * the total number of sensors, ensuring continuous cyclic updates.
 *
 * @param current_index The current sensor index.
 * @param number_of_sensors Total number of sensors in the system.
 * @return uint8_t The updated sensor index.
 */
static uint8_t readAllSensorsPeriodicUpdateSensorIndex(uint8_t current_index, size_t number_of_sensors);
/* *************************************** */

/* EXPORTED FUNCTIONS */
task_status_te app_readSpecificSensor(uint8_t sensor_id, output_destination_t output)
{
    // Define input component and fetch sensor data
    control_device_ts sensor_to_read = {INPUT_SENSORS, sensor_id};
    control_input_data_ts sensor_reading_result = control_fetchDataFromInput(&sensor_to_read);
    // Handle input errors
    control_error_ts error = {sensor_reading_result.error_code, sensor_to_read};
    checkForErrors(&error);

    if (IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
    {
        sendToOutputAndCheckForErrors(OUTPUT_DISPLAY, &(sensor_reading_result.data));
    }

    if (IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
    {
        sendToOutputAndCheckForErrors(OUTPUT_SERIAL_CONSOLE, &(sensor_reading_result.data));
    }

    return FINISHED;  // Notify that task is finished
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

sensor_reading_context_ts app_createNewSensorsReadingContext()
{
    sensor_reading_context_ts new_sensor_reading_context = {sensors_interface_getSensorsLen(), STARTING_SENSOR_INDEX};
    return new_sensor_reading_context;
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
/* *************************************** */