#ifndef DATA_ROUTER_H
#define DATA_ROUTER_H

#include <Arduino.h>
#include "../input/functionality/i2c_scan/i2cScan.h"
#include "../input/rtc/rtc.h"
#include "../input/sensors/sensors.h"
#include "../input/functionality/calibration/calibration.h"
#include "../output/display/display.h"
#include "../output/serial_console/serial_console.h"
#include "data_router_types.h"
#include "../error_manager/error_manager.h"

/**
 * Structure for managing the dual output of data fetching operations.
 * 
 * This structure is designed to encapsulate two outputs resulting from 
 * a data fetch operation. One part (`data`) contains the fetched data 
 * that can be forwarded to an output component, while the other part 
 * (`error_msg`) contains an error message to be handled by the Error Manager.
 * 
 * Members:
 *  - data:      Contains the fetched data, encapsulated in the `data_router_data_ts` 
 *               structure, to be forwarded to an output component (e.g., display, console).
 *  - error_msg: Represents the error message of type `error_manager_error_ts` 
 *               that can be passed to the Error Manager for further processing.
 */
typedef struct
{
    data_router_data_ts data;                /**< The fetched data for output forwarding. */
    error_manager_error_ts error_msg;  /**< The error message for the Error Manager. */
} data_router_input_data_ts;

/**
 * Routes data to the specified output component.
 * 
 * This function forwards data fetched from an input component to one of the 
 * defined output components. It returns an error code that can be passed 
 * to the Error Manager for handling.
 * 
 * @param output_component The ID of the output component to which the data 
 *                         is forwarded (e.g., display, serial console).
 * @param data             The actual data to be forwarded, which must match 
 *                         the format/type returned by the data fetch function.
 * 
 * @return An error message of type `error_manager_error_ts` indicating the 
 *         status of the routing operation.
 */
error_manager_error_ts data_router_routeDataToOutput(data_router_output_component_te output_component,
                                                          data_router_input_data_ts data);

/**
 * Fetches data from the specified input component.
 * 
 * This function retrieves data from a specified input component (e.g., sensors, RTC) 
 * and returns it as a structured result. One part of the fetched data can then be used by  
 * output components and the other can be forwarded to an Error manager.
 * 
 * @param input_component The ID of the input component from which data is fetched 
 *                        (e.g., sensors, RTC).
 * @param component_id    The specific ID within the input component (e.g., sensor ID).
 * 
 * @return A structure of type `data_router_input_data_ts` has two parts.
 *         `data` - containing the fetched data, input type, and component ID. 
 *         `error_msg` - containing error code, flag for input/output and the details of the 
 *         component( ID, etc.)
 */
data_router_input_data_ts data_router_fetchDataFromInput(data_router_input_component_te input_component, 
                                                         uint8_t component_id);

#endif