#include "data_router.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * Initializes the input return data structure before fetching data.
 */
static data_router_input_data_ts initializeInputReturnData(data_router_input_component_te input_component, uint8_t component_id);

/**
 * Initializes the output return error message.
 */
static error_manager_error_ts initializeOutputReturnErrorMsg(data_router_output_component_te output_component, data_router_input_data_ts data);
/* *************************************** */

/* EXPORTED FUNCTIONS */
error_manager_error_ts data_router_routeDataToOutput(data_router_output_component_te output_component, data_router_input_data_ts data)
{
    // Initialize error message with default values
    error_manager_error_ts error_msg = initializeOutputReturnErrorMsg(output_component, data);

    if(ERROR_CODE_NO_ERROR != data.error_msg.error_code)
    {
        switch(output_component)
        {
            case OUTPUT_DISPLAY:
                // Route data to display and update error code
                error_msg.error_code = display_displayData(data.data);
                break;

            case OUTPUT_SERIAL_CONSOLE:
                // Serial console output not implemented yet
                break;

            default:
                // Set error code for invalid output
                error_msg.error_code = ERROR_CODE_INVALID_OUTPUT;
                break;
        }
    }
    // Default error code is set to ERROR_CODE_OUTPUT_ROUTER_DATA_CONTAINS_ERRORS so there is not need for else statement.
    return error_msg;
}

data_router_input_data_ts data_router_fetchDataFromInput(data_router_input_component_te input_component, uint8_t component_id)
{
    // Initialize input return data with defaults
    data_router_input_data_ts return_data = initializeInputReturnData(input_component, component_id);

    switch(input_component)
    {
        case INPUT_SENSORS:
            // Fetch sensor reading and update return data
            sensor_return_ts sensor_return = sensors_getReading(component_id);
            return_data.error_msg.error_code = sensor_return.error_code;
            return_data.data.input_return.sensor_reading = sensor_return.sensor_reading;
            break;

        case INPUT_RTC:
            // Fetch RTC data and update return data
            rtc_return_ts rtc_return = rtc_getTime(component_id);
            return_data.error_msg.error_code = rtc_return.error_code;
            return_data.data.input_return.rtc_reading = rtc_return.rtc_reading;
            break;

        case INPUT_I2C_SCAN:
            // Fetch I2C scan data and update return data
            uint8_t i2c_addr = component_id;
            i2cScan_return_ts i2cScan_return = i2cScan_getReading(i2c_addr);
            return_data.error_msg.error_code = i2cScan_return.error_code;
            return_data.data.input_return.i2cScan_reading = i2cScan_return.i2cScan_reading;
            break;

        default:
            // Default error code is set to ERROR_CODE_INVALID_INPUT so no need to set it again here.
            break;
    }
    return return_data;
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static data_router_input_data_ts initializeInputReturnData(data_router_input_component_te input_component, uint8_t component_id)
{
    data_router_input_data_ts return_data;

    // Initialize data part
    return_data.data.input_type = input_component;
    return_data.data.input_id = component_id;

    // Initialize error part
    return_data.error_msg.error_code = ERROR_CODE_INVALID_INPUT;
    return_data.error_msg.io_flag = DATA_ROUTER_INPUT;
    return_data.error_msg.component.input_error.input_component = input_component;
    return_data.error_msg.component.input_error.input_id = component_id;

    return return_data;
}

static error_manager_error_ts initializeOutputReturnErrorMsg(data_router_output_component_te output_component, data_router_input_data_ts data)
{
    // Initialize the error message with default values
    error_manager_error_ts error_msg;
    error_msg.error_code = ERROR_CODE_OUTPUT_ROUTER_DATA_CONTAINS_ERRORS;
    error_msg.io_flag = DATA_ROUTER_OUTPUT;
    error_msg.component.output_error.output_component = output_component;
    error_msg.component.output_error.input_type = data.data.input_type;
    error_msg.component.output_error.input_id = data.data.input_id;

    return error_msg;
}
/* *************************************** */