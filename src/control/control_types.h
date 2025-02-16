#ifndef CONTROL_TYPES_H
#define CONTROL_TYPES_H

#include <Arduino.h>
#include "../input/input_types.h"
#include "control_error_codes.h"

// Represents an unused or invalid input ID
#define ERROR_INPUT_ID_UNUSED (0xFF)

/**
 * Enum listing all available inputs.
 *
 * This enumeration defines the possible input sources for data
 * within the data routing system.
 */
typedef enum
{
    INPUT_SENSORS,     /**< Input for sensors. */
    INPUT_RTC,         /**< Input for the Real-Time Clock (RTC). */
    INPUT_I2C_SCAN,    /**< Input for I2C address scanning. */
    INPUT_ERROR        /**< Input for error. */
} control_input_te;

/**
 * Enum listing all available output components.
 *
 * This enumeration defines the possible output destinations where data
 * can be forwarded by the data routing system.
 */
typedef enum
{
    OUTPUT_DISPLAY,       /**< Output component for a display device. */
    OUTPUT_SERIAL_CONSOLE /**< Output component for the serial console. */
} control_output_component_te;

/**
 * Structure representing an error related to an output component.
 * 
 * This structure provides details about errors that occur while routing
 * data to an output component.
 * 
 * Members:
 *  - output_component: The specific output component where the error occurred (e.g., display, serial console).
 *  - input_type: The type of input component associated with the error.
 *  - input_id: The ID of the input component that generated the data routed to the output.
 */
typedef struct
{
    control_output_component_te output_component;     /**< The output component where the error occurred. */
    control_input_te input_type;                      /**< The associated input component type. */
    uint8_t input_id;                                 /**< The ID of the input component. */
} control_output_error_ts;

/**
 * Structure representing an error related to an input component.
 * 
 * This structure provides details about errors that occur while fetching
 * data from an input component.
 * 
 * Members:
 *  - input_component: The specific input component where the error occurred (e.g., sensors, RTC).
 *  - input_id: The ID of the input component where the error occurred.
 */
typedef struct
{
    control_input_te input_component;               /**< The input component where the error occurred. */
    uint8_t input_id;                               /**< The ID of the input component. */
} control_input_error_ts;

/**
 * Union for managing input and output error details.
 * 
 * This union allows the error manager to encapsulate error details for both 
 * input and output components dynamically, based on the context.
 * 
 * Members:
 *  - output_error: Details about an output component error.
 *  - input_error: Details about an input component error.
 */
typedef union
{
    control_output_error_ts output_error; /**< Details of the output error. */
    control_input_error_ts input_error;   /**< Details of the input error. */
} control_error_type_tu;

/**
 * Structure representing a generic error in the system.
 * 
 * This structure encapsulates all relevant details of an error, including
 * the error code, whether it occurred in an input or output operation, 
 * and specific details about the affected component.
 * 
 * Members:
 *  - error_code: The error code identifying the specific type of error.
 *  - component: Contains detailed information about the affected input/output component.
 */
typedef struct
{
    control_error_code_te error_code; /**< The specific error code. */
    control_error_type_tu component;  /**< Detailed information about the error source. */
} control_error_ts;

/**
 * Union for handling various input types dynamically.
 *
 * This union is designed to accommodate different types of input data that
 * may be returned from `control_fetchDataFromInput` or an error. The specific input
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
    control_error_ts error_msg;           /**< Data structure for error message. */
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
 *  - input_type:   An enumeration (`control_input_te`) specifying the
 *                  type of input (e.g., sensors, RTC).
 *  - input_id:     A unique identifier for the input (e.g., sensor ID) to
 *                  differentiate between multiple components of the same type.
 */
typedef struct
{
    input_return_tu input_return;    /**< Union holding the returned input data. */
    control_input_te input_type;     /**< Type of the input. */
    uint8_t input_id;                /**< ID of the specific input instance. */
} control_data_ts;

#endif