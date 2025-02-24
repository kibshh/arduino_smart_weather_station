#ifndef APP_COMMON_H
#define APP_COMMON_H

#include <Arduino.h>
#include "../control/control.h"

/* Macro to check if a specific output bit is set in the output variable */
/* It returns true if the specified bit (bit_mask) is set in the output, otherwise false */
#define IS_OUTPUT_INCLUDED(output, bit_mask) ((output & bit_mask) != 0)

/* Type for representing and managing output destinations (supports up to 16 different output options) */
typedef uint16_t output_destination_t;

/** Bitmask macros for output destinations:
 * - The first 8 bits (0x00FF) are for time-independent outputs (supports up to 8 outputs at once).
 * - The second 8 bits (0xFF00) are for time-dependent outputs (supports up to 8 outputs at once, e.g., display).
 * - ALL_OUTPUTS combines both time-independent and time-dependent outputs (16 bits in total).
 */
/* Output option for serial console(can show all at once or sequentially) */
#define SERIAL_CONSOLE                 (output_destination_t)(1 << 0) /* 7 bytes reserved for the future outputs */
/* Output option for displays(cannot show all at once) */
#define LCD_DISPLAY                    (output_destination_t)(1 << 8) /* 7 bytes reserved for the future outputs */
/* Outputs that can be sent independently of time constraints(all at once) */
#define ALL_TIME_INDEPENDENT_OUTPUTS   (output_destination_t)(0x00FF)
/* Outputs that require time constraints (e.g., display) */
#define ALL_TIME_DEPENDENT_OUTPUTS     (output_destination_t)(0xFF00)
/* Used to indicate all outputs */
#define ALL_OUTPUTS                    (output_destination_t)(0xFFFF)
/* Used to indicate no outputs are set */
#define NO_OUTPUTS                     (output_destination_t)(0x0000)

/* Enum representing the status of a task(function called) */
typedef enum
{
    FINISHED,
    NOT_FINISHED
} task_status_te;

/**
 * @brief Checks for errors and triggers error handling if needed.
 *
 * This function evaluates the provided error code and, if an error is detected,  
 * calls the error handling mechanism with the corresponding component and ID.  
 * It ensures that errors are properly processed and logged for diagnostics.
 *
 * @param error Pointer to struct with error code and another struct 
 *              representing the component associated with the error.
 */
void checkForErrors(const control_error_ts *error);


/**
 * @brief Filters out time-dependent outputs.
 *
 * Modifies the provided output destination by removing any 
 * time-dependent outputs, ensuring only time-independent 
 * destinations remain.
 *
 * @param output The original output destination.
 * @return output_destination_t The filtered output destination.
 */
output_destination_t filterOutTimeDependentOutputs(output_destination_t output);

/**
 * @brief Routes data to the specified output component and checks for errors.
 *
 * This function attempts to send the provided data to the specified output component (e.g., LCD display, serial console).
 * It then checks for any errors encountered during the operation by calling `checkForErrors`.
 *
 * @param output The output component to send the data to (e.g., OUTPUT_DISPLAY, OUTPUT_SERIAL_CONSOLE).
 * @param data A pointer to the data to be sent to the output component.
 * 
 * @note The `CONTROL_ID_UNUSED` is used to indicate that the device ID is not used in this operation.
 */
void sendToOutputAndCheckForErrors(control_io_t output, const control_data_ts *data);

#endif