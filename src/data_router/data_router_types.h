#ifndef DATA_ROUTER_TYPES_H
#define DATA_ROUTER_TYPES_H

#include <Arduino.h>
#include "../input/input_types.h"

/**
 * Enum representing the type of component (input or output).
 * 
 * This enumeration is used to specify whether a component is used as an 
 * input source or as an output destination in the data routing system.
 */
typedef enum
{
    DATA_ROUTER_INPUT,  /**< Represents an input component type. */
    DATA_ROUTER_OUTPUT  /**< Represents an output component type. */
} data_router_io_type_te;

/**
 * Enum listing all available input components.
 * 
 * This enumeration defines the possible input sources for fetching data 
 * within the data routing system.
 */
typedef enum
{
    INPUT_SENSORS,      /**< Input component for sensors. */
    INPUT_RTC,          /**< Input component for the Real-Time Clock (RTC). */
    INPUT_I2C_SCAN,     /**< Input component for I2C address scanning. */
    INPUT_CALIBRATION   /**< Input component for calibration. */
} data_router_input_component_te;

/**
 * Enum listing all available output components.
 * 
 * This enumeration defines the possible output destinations where data 
 * can be forwarded by the data routing system.
 */
typedef enum
{
    OUTPUT_DISPLAY,          /**< Output component for a display device. */
    OUTPUT_SERIAL_CONSOLE    /**< Output component for the serial console. */
} data_router_output_component_te;

/**
 * Union for handling various input types dynamically.
 * 
 * This union is designed to accommodate different types of input data that 
 * may be returned from `data_router_fetchDataFromInput`. The specific input 
 * type (e.g., sensor reading, RTC reading) is determined dynamically at runtime, 
 * allowing the function to flexibly handle multiple input sources.
 * 
 * Fields:
 *  - sensor_reading:     Contains data specific to sensor readings, such as value, 
 *                        measurement type, and sensor ID.
 *  - rtc_reading:        Contains data specific to RTC readings, such as date and time.
 *  - i2cScan_reading:    Contains data specific to I2C scan readings, 
 *                        such as addresses bit fields or I2C device status.
 *  - calib_reading:      Contains data specific to calibration readings, such as calibration 
 *                        value and calibration status.
 */
typedef union 
{
    sensor_reading_ts sensor_reading;      /**< Data structure for sensor readings. */
    rtc_reading_ts rtc_reading;            /**< Data structure for RTC readings. */
    i2cScan_reading_ts i2cScan_reading;    /**< Data structure for I2C scan readings. */
    calibration_reading_ts calib_reading;  /**< Data structure for calibration readings. */
} input_return_tu;

/**
 * Structure representing data returned from an input component.
 * 
 * This structure encapsulates the data fetched from an input component, 
 * including the actual return data, the type of input component, and its unique identifier.
 * It is used to standardize and simplify the handling of data from various inputs 
 * (e.g., sensors, RTC).
 * 
 * Members:
 *  - input_return: A union (`input_return_tu`) holding the actual data returned from 
 *                  the input component. The specific type (e.g., sensor data, RTC data) 
 *                  is determined dynamically based on the `input_type`.
 *  - input_type:   An enumeration (`data_router_input_component_te`) specifying the 
 *                  type of input component (e.g., sensors, RTC).
 *  - input_id:     A unique identifier for the input component (e.g., sensor ID) to 
 *                  differentiate between multiple components of the same type.
 */
typedef struct 
{
    input_return_tu input_return;               /**< Union holding the returned input data. */
    data_router_input_component_te input_type;  /**< Type of the input component. */
    uint8_t input_id;                           /**< ID of the specific input instance. */
} data_router_data_ts;

#endif