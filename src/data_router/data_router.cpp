#include "data_router.h"


error_manager_error_code_te data_router_routeDataToOutput(data_router_output_component output_component, data_router_input_data_ts data)
{
    error_manager_error_code_te error_code = ERROR_CODE_OUTPUT_ROUTER_DATA_CONTAINS_ERRORS;

    if(ERROR_CODE_NO_ERROR != data.error_code)
    {
        switch(output_component)
        {
            case OUTPUT_DISPLAY:
                error_code = display_displayData(data);
                break;

            case OUTPUT_SERIAL_CONSOLE:
                break;

            default:
                error_code = ERROR_CODE_INVALID_OUTPUT;
                break;
        }
    }

    return error_code;
}

data_router_input_data_ts data_router_fetchDataFromInput(data_router_input_component input_component, uint8_t component_id)
{
    data_router_input_data_ts return_data;
    return_data.error_code = ERROR_CODE_INVALID_INPUT;

    switch(input_component)
    {
        case INPUT_SENSORS:
            return_data.input_type = INPUT_SENSORS;
            uint8_t sensor_id = component_id;
            sensor_return_t sensor_return = sensors_getReading(sensor_id);

            return_data.error_code = sensor_return.error_code;
            return_data.input_return.sensor_reading = sensor_return.sensor_reading;
            break;

        case INPUT_RTC:
            return_data.input_type = INPUT_RTC;
            rtc_return_t rtc_return = rtc_getTime();
            return_data.error_code = rtc_return.error_code;
            return_data.input_return.rtc_reading = rtc_return.rtc_reading;
            break;

        case INPUT_I2C_SCAN:
            break;

        default:
            break;
    }

    return return_data;
}

