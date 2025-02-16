#include "control.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * Routes error to the specified output component.
 *
 * This function forwards error recieved from an input component to one of the
 * defined output components. It returns an error code that can be passed
 * to the Error Manager for handling.
 *
 * @param output_component The ID of the output component to which the error
 *                         is forwarded (e.g., display, serial console).
 * @param error_msg            A structure containing the information about the error.
 *
 * @return An error message of type `control_error_ts` indicating the
 *         status of the routing operation.
 */
static control_error_ts routeErrorToOutput(control_output_component_te output_component,
                                           control_error_ts error_msg);

/**
 * Initializes the input return data structure before fetching data.
 */
static control_input_data_ts initializeInputReturnData(control_input_te input_component, uint8_t component_id);

/**
 * Initializes the output return error message.
 */
static control_error_ts initializeOutputReturnErrorMsg(control_output_component_te output_component, control_input_data_ts data);
/* *************************************** */

/* EXPORTED FUNCTIONS */
control_error_ts control_routeDataToOutput(control_output_component_te output_component, control_input_data_ts data)
{
    // Initialize error message with default values
    control_error_ts error_msg = initializeOutputReturnErrorMsg(output_component, data);

    if (ERROR_CODE_NO_ERROR != data.error_msg.error_code)
    {
        switch (output_component)
        {
        case OUTPUT_DISPLAY:
            // Route data to display and update error code
            error_msg.error_code = display_displayData(data.data);
            break;

        case OUTPUT_SERIAL_CONSOLE:
            // Route data to serial console and update error code
            error_msg.error_code = serial_console_displayData(data.data);
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

control_input_data_ts control_fetchDataFromInput(control_input_te input_component, uint8_t component_id)
{
    // Initialize input return data with defaults
    control_input_data_ts return_data = initializeInputReturnData(input_component, component_id);

    switch (input_component)
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

void control_handleError(control_error_ts error_msg)
{
    control_error_ts error_msg_serial = routeErrorToOutput(OUTPUT_SERIAL_CONSOLE, error_msg);
    if (error_msg_serial.error_code != ERROR_CODE_NO_ERROR)
    {
        routeErrorToOutput(OUTPUT_DISPLAY, error_msg);
    }
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static control_error_ts routeErrorToOutput(control_output_component_te output_component, control_error_ts error_msg)
{
    control_data_ts data;
    data.input_id = ERROR_INPUT_ID_UNUSED;
    data.input_type = INPUT_ERROR;
    data.input_return.error_msg = error_msg;

    switch (output_component)
    {
    case OUTPUT_DISPLAY:
        // Route data to display and update error code
        error_msg.error_code = display_displayData(data);
        break;

    case OUTPUT_SERIAL_CONSOLE:
        // Serial console output not implemented yet
        break;

    default:
        // Set error code for invalid output
        error_msg.error_code = ERROR_CODE_INVALID_OUTPUT;
        break;
    }

    return error_msg;
}

static control_input_data_ts initializeInputReturnData(control_input_te input_component, uint8_t component_id)
{
    control_input_data_ts return_data;

    // Initialize data part
    return_data.data.input_type = input_component;
    return_data.data.input_id = component_id;

    // Initialize error part
    return_data.error_msg.error_code = ERROR_CODE_INVALID_INPUT;
    return_data.error_msg.component.input_error.input_component = input_component;
    return_data.error_msg.component.input_error.input_id = component_id;

    return return_data;
}

static control_error_ts initializeOutputReturnErrorMsg(control_output_component_te output_component, control_input_data_ts data)
{
    // Initialize the error message with default values
    control_error_ts error_msg;
    error_msg.error_code = ERROR_CODE_OUTPUT_ROUTER_DATA_CONTAINS_ERRORS;
    error_msg.component.output_error.output_component = output_component;
    error_msg.component.output_error.input_type = data.data.input_type;
    error_msg.component.output_error.input_id = data.data.input_id;

    return error_msg;
}
/* *************************************** */