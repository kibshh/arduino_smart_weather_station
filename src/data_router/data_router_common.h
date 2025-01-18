#ifndef DATA_ROUTER_COMMON_H
#define DATA_ROUTER_COMMON_H

#include <Arduino.h>
#include "../error_manager/error_manager.h"

/* SENSORS INPUT PARAMETERS */
#define DATA_ROUTER_INPUT_SENSORS_PARAMS_SENSOR_ID_POS           (0u)
#define DATA_ROUTER_INPUT_SENSORS_PARAMS_SENSOR_ID_LEN           (1u)
#define DATA_ROUTER_INPUT_SENSORS_PARAMS_MEASUREMENT_TYPE_POS    (1u)
#define DATA_ROUTER_INPUT_SENSORS_PARAMS_MEASUREMENT_TYPE_LEN    (1u)
#define DATA_ROUTER_INPUT_SENSORS_PARAMS_LEN                     (DATA_ROUTER_INPUT_SENSORS_PARAMS_SENSOR_ID_LEN + DATA_ROUTER_INPUT_SENSORS_PARAMS_MEASUREMENT_TYPE_LEN)
/* ********************************* */

typedef enum
{
    INPUT_SENSORS,
    INPUT_RTC,
    INPUT_I2C_SCAN
} data_router_input_component;

typedef enum
{
    OUTPUT_DISPLAY,
    OUTPUT_SERIAL_CONSOLE
} data_router_output_component;

typedef struct
{
    union 
    {
        sensor_reading_t sensor_reading;
        rtc_reading_t rtc_reading;
    } input_return;
    data_router_input_component input_type;
    error_manager_error_code_te error_code;
} data_router_input_data_ts;

#endif