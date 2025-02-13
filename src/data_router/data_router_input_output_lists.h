#ifndef DATA_ROUTER_INPUT_OUTPUT_LISTS_H
#define DATA_ROUTER_INPUT_OUTPUT_LISTS_H

#include <Arduino.h>

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
} data_router_input_te;

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
} data_router_output_component_te;

#endif