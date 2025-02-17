#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include "../input/i2c_scan/i2c_scan.h"
#include "../input/rtc/rtc.h"
#include "../input/sensors/sensors.h"
#include "../output/display/display.h"
#include "../output/serial_console/serial_console.h"
#include "control_types.h"

/* Index for components that are used in the system. */
#define CONTROL_COMPONENTS_STATUS_USED_INDEX     (uint8_t)(0u)

/* Index for components that are currently functioning. */
#define CONTROL_COMPONENTS_STATUS_WORKING_INDEX  (uint8_t)(1u)

/* Total number of component status entries. */
#define CONTROL_COMPONENTS_STATUS_SIZE           (uint8_t)(2u)

/* Macro defining a value (0) indicating all components are initialized */
#define CONTROL_ALL_INITIALIZED                  (uint8_t)(0u)

/* Macro defining a value (0) indicating component is initialized */
#define CONTROL_COMPONENT_INITIALIZED            (uint8_t)(0u)

/* Macro indicating successful initialization */
#define CONTROL_INITIALIZATION_SUCCESSFUL        (bool)(true)

/* Macro indicating failed initialization */
#define CONTROL_INITIALIZATION_FAILED            (bool)(false)

/* Macro used for first initialization */
#define CONTROL_FIRST_INIT                       (bool)(false)

/* Macro used for reinitialization */
#define CONTROL_REINIT                           (bool)(true)

/**
 * @brief Structure to track the status of system components.
 *
 * This structure contains bit fields where each bit represents the status of a specific component.  
 * A bit set to 1 indicates that the component is in use or functioning, while a bit set to 0 indicates it is not.
 *
 * Fields:
 * - `sensors_status` (uint64_t):  
 *   - Each bit (0-63) represents the status of a sensor.  
 *   - Supports up to 64 different sensors.
 * - `other_inputs_status` (uint8_t):  
 *   - Each bit (0-7) represents the status of other input components (e.g., RTC, buttons, etc.).  
 *   - Supports up to 8 different other input components.
 * - `outputs_status` (uint8_t):  
 *   - Each bit (0-7) represents the status of output components (e.g., display, serial console, etc.).  
 *   - Supports up to 8 different output components.
 */
typedef struct
{
    uint64_t sensors_status;
    uint8_t other_inputs_status;
    uint8_t outputs_status;
} components_status_ts;

/**
 * @brief Performs the first-time initialization of all system components.
 * 
 * Calls the control_initialize function with CONTROL_FIRST_INIT to initialize 
 * all outputs, inputs, and sensors from a fresh state.
 * 
 * @return CONTROL_INITIALIZATION_SUCCESSFUL if all components initialize correctly, 
 *         otherwise CONTROL_INITIALIZATION_FAILED.
 */
bool control_init();

/**
 * @brief Attempts to reinitialize any uninitialized system components.
 * 
 * Calls the control_initialize function with CONTROL_REINIT to check for and 
 * reattempt initialization of components that failed during the initial startup.
 * 
 * @return CONTROL_INITIALIZATION_SUCCESSFUL if all components are successfully 
 *         initialized after reattempt, otherwise CONTROL_INITIALIZATION_FAILED.
 */
bool control_reinit();

/**
 * @brief Routes data to the specified output component.
 *
 * This function forwards data fetched from an input component to one of the
 * defined output components. It returns an error code that can be passed
 * to the Error Manager for handling.
 *
 * @param output_component The ID of the output component to which the data
 *                         is forwarded (e.g., display, serial console).
 * @param data             The actual data to be forwarded, which must match
 *                         the format/type of data part returned by the data fetch function.
 *
 * @return An error message of type `control_output_data_ts` indicating the
 *         status of the routing operation.
 */
control_output_data_ts control_routeDataToOutput(control_component_te output_component,
                                                 control_data_ts data);

/**
 * @brief Fetches data from the specified input component.
 *
 * This function retrieves data from a specified input component (e.g., sensors, RTC)
 * and returns it as a structured result. One part of the fetched data can then be used by
 * output components and the other can be forwarded to an Error manager.
 *
 * @param input_component The ID of the input component from which data is fetched
 *                        (e.g., sensors, RTC).
 * @param component_id    The specific ID within the input component (e.g., sensor ID).
 *
 * @return A structure of type `control_input_data_ts` has two parts.
 *         `data` - containing the fetched data, input type, and component ID.
 *         `error_msg` - containing error code, flag for input/output and the details of the
 *         component( ID, etc.)
 */
control_input_data_ts control_fetchDataFromInput(control_component_te input_component,
                                                 uint8_t component_id);

/**
 * @brief Handles errors by routing them to appropriate output components.
 *
 * This function packages the error details and attempts to route the error message  
 * to the serial console for logging. If routing to the serial console fails,  
 * the error message is forwarded to the display as a fallback.
 *
 * @param error_code   The error code indicating the type of error.
 * @param component    The component where the error occurred.
 * @param component_id The unique identifier of the component.
 */
void control_handleError(control_error_code_te error_code, control_component_te component, uint8_t component_id);


#endif