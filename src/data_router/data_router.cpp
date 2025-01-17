#include "data_router.h"


error_manager_error_code_te data_router_routeDataToOutput(data_router_output_component output_component, data_router_output_input_type input_type, uint8_t *payload, size_t payload_len)
{
    error_manager_error_code_te error_code = ERROR_CODE_INVALID_OUTPUT_PAYLOAD_LEN;

    if(payload_len != 0 && payload != NULL)
    {
        switch(output_component)
        {
            case OUTPUT_DISPLAY:
                error_code = display_displayData(input_type, payload, payload_len);
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

error_manager_error_code_te data_router_fetchDataFromInput(data_router_input_component input_component, uint8_t *params, size_t params_len, uint8_t *return_buffer, size_t *return_buffer_size)
{
    error_manager_error_code_te error_code = ERROR_CODE_INVALID_RETURN_BUFFER;

    if (return_buffer != NULL && return_buffer_size != NULL && *return_buffer_size != 0)
    {
        switch(input_component)
        {
            case INPUT_SENSORS:
                if(NULL != params && DATA_ROUTER_INPUT_SENSORS_PARAMS_LEN <= params_len)
                {
                    if(DATA_ROUTER_INPUT_SENSORS_RETURN_LEN <= *return_buffer_size)
                    {
                        uint8_t sensor_id = params[DATA_ROUTER_INPUT_SENSORS_PARAMS_SENSOR_ID_POS];
                        uint8_t sensor_measurement_type = params[DATA_ROUTER_INPUT_SENSORS_PARAMS_MEASUREMENT_TYPE_POS];
                        sensor_reading_t sensor_reading = sensors_getReading(sensor_id, sensor_measurement_type);

                        error_code = sensor_reading.error_code;
                        return_buffer[DATA_ROUTER_INPUT_SENSORS_RETURN_MEASUREMENT_TYPE_POS] = (uint8_t)sensor_reading.measurement_type_switch;
                        memcpy(&(return_buffer[DATA_ROUTER_INPUT_SENSORS_RETURN_VALUE_POS]), &(sensor_reading.value), DATA_ROUTER_INPUT_SENSORS_RETURN_VALUE_LEN);
                        return_buffer[DATA_ROUTER_INPUT_SENSORS_RETURN_INDICATION_POS] = (uint8_t)sensor_reading.indication;
                        return_buffer[DATA_ROUTER_INPUT_SENSORS_RETURN_ID_POS] = (uint8_t)sensor_reading.sensor_id;
                        *return_buffer_size = DATA_ROUTER_INPUT_SENSORS_RETURN_VALUE_LEN;
                    }
                    else
                    {
                        error_code = ERROR_CODE_INVALID_RETURN_BUFFER;
                    }
                }
                else
                {
                    error_code = ERROR_CODE_INVALID_INPUT_PARAMETERS;
                }
                break;

            case INPUT_RTC:
                    if(DATA_ROUTER_INPUT_RTC_RETURN_LEN <= *return_buffer_size)
                    {
                        rtc_reading_t rtc_reading = rtc_getTime();

                        error_code = rtc_reading.error_code;
                        memcpy(&(return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_YEAR_POS]), &(rtc_reading.year), DATA_ROUTER_INPUT_RTC_RETURN_YEAR_LEN);
                        return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_MONTH_POS] = (uint8_t)rtc_reading.month;
                        return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_DAY_POS] = (uint8_t)rtc_reading.day;
                        return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_HOUR_POS] = (uint8_t)rtc_reading.hour;
                        return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_MINS_POS] = (uint8_t)rtc_reading.mins;
                        return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_SECS_POS] = (uint8_t)rtc_reading.secs;
                        *return_buffer_size = DATA_ROUTER_INPUT_RTC_RETURN_LEN;
                    }
                    else
                    {
                        error_code = ERROR_CODE_INVALID_RETURN_BUFFER;
                    }
                break;

            case INPUT_I2C_SCAN:
                break;

            default:
                error_code = ERROR_CODE_INVALID_INPUT;
                break;
        }
    }

    return error_code;
}

