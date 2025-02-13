#ifndef DATA_ROUTER_TYPES_H
#define DATA_ROUTER_TYPES_H

#include <Arduino.h>
#include "../input/input_types.h"
#include "data_router_input_output_lists.h"
#include "../error_manager/error_manager_types.h"

/**
 * Union for handling various input types dynamically.
 *
 * This union is designed to accommodate different types of input data that
 * may be returned from `data_router_fetchDataFromInput` or an error. The specific input
 * type (e.g., sensor reading, RTC reading) is determined dynamically at runtime,
 * allowing the function to flexibly handle multiple input sources.
 *
 * Fields:
 *  - sensor_reading:     Contains data specific to sensor readings, such as value,
 *                        measurement type, and sensor ID.
 *  - rtc_reading:        Contains data specific to RTC readings, such as date and time.
 *  - i2cScan_reading:    Contains data specific to I2C scan readings,
 *                        such as addresses bit fields or I2C device status.
 *  - error_msg           Contains data specific to the error message, such as error source,
 *                        input/output flag and specific error code.
 */
typedef union
{
    sensor_reading_ts sensor_reading;     /**< Data structure for sensor readings. */
    rtc_reading_ts rtc_reading;           /**< Data structure for RTC readings. */
    i2cScan_reading_ts i2cScan_reading;   /**< Data structure for I2C scan readings. */
    error_manager_error_ts error_msg;     /**< Data structure for error message. */
} input_return_tu;

/**
 * Structure representing data returned from an input.
 *
 * This structure encapsulates the data recieved from an input,
 * including the actual return data, the type of input, and its unique identifier.
 * It is used to standardize and simplify the handling of data from various inputs
 * (e.g., sensors, RTC).
 *
 * Members:
 *  - input_return: A union (`input_return_tu`) holding the actual data returned from
 *                  the input. The specific type (e.g., sensor data, RTC data)
 *                  is determined dynamically based on the `input_type`.
 *  - input_type:   An enumeration (`data_router_input_te`) specifying the
 *                  type of input (e.g., sensors, RTC).
 *  - input_id:     A unique identifier for the input (e.g., sensor ID) to
 *                  differentiate between multiple components of the same type.
 */
typedef struct
{
    input_return_tu input_return;    /**< Union holding the returned input data. */
    data_router_input_te input_type; /**< Type of the input. */
    uint8_t input_id;                /**< ID of the specific input instance. */
} data_router_data_ts;

#endif