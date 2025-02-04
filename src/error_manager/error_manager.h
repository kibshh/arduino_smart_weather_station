#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#include <Arduino.h>
#include "error_codes.h"
#include "../data_router/data_router.h"

#define ERROR_INPUT_ID_UNUSED (0xFF)
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
    data_router_output_component_te output_component; /**< The output component where the error occurred. */
    data_router_input_te input_type;        /**< The associated input component type. */
    uint8_t input_id;                                 /**< The ID of the input component. */
} error_manager_output_error_ts;

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
    data_router_input_te input_component; /**< The input component where the error occurred. */
    uint8_t input_id;                               /**< The ID of the input component. */
} error_manager_input_error_ts;

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
    error_manager_output_error_ts output_error; /**< Details of the output error. */
    error_manager_input_error_ts input_error;   /**< Details of the input error. */
} error_type_tu;

/**
 * Structure representing a generic error in the system.
 * 
 * This structure encapsulates all relevant details of an error, including
 * the error code, whether it occurred in an input or output operation, 
 * and specific details about the affected component.
 * 
 * Members:
 *  - error_code: The error code identifying the specific type of error.
 *  - io_flag: Specifies whether the error occurred in an input or output operation.
 *  - component: Contains detailed information about the affected input/output component.
 */
typedef struct
{
    error_manager_error_code_te error_code; /**< The specific error code. */
    data_router_io_type_te io_flag;        /**< Input/output flag indicating the type of operation. */
    error_type_tu component;               /**< Detailed information about the error source. */
} error_manager_error_ts;

/**
 * Handles errors by routing them to output components
 * 
 * This function retrieves the specific error structure and tries to display it
 * using output components. It tries each one of the output components sequentially if some
 * of them produce an error. If all of the attempts are unsuccessful, it aborts the operation.
 * 
 * @param error_msg A structure containing the information about the error.
 */
void error_manager_handleError(error_manager_error_ts error_msg);

#endif