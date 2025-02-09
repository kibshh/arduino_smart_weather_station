#ifndef APP_COMMON_H
#define APP_COMMON_H

#include <Arduino.h>
#include "../data_router/data_router.h"
#include "../error_manager/error_manager.h"

// Macro to check if a specific output bit is set in the output variable
// It returns true if the specified bit (bit_mask) is set in the output, otherwise false
#define IS_OUTPUT_INCLUDED(output, bit_mask) ((output & bit_mask) != 0)

// Type for representing and managing output destinations (supports up to 16 different output options)
typedef uint16_t output_destination_t;
// Bitmask macros for output destinations:
// - The first 8 bits (0x00FF) are for time-independent outputs (supports up to 8 outputs at once).
// - The second 8 bits (0xFF00) are for time-dependent outputs (supports up to 8 outputs at once, e.g., display).
// - ALL_OUTPUTS combines both time-independent and time-dependent outputs (16 bits in total).
#define SERIAL_CONSOLE                 (output_destination_t)(1 << 0) // Output option for serial console(can show all at once or sequentially)
// 7 bytes reserved for the future
#define LCD_DISPLAY                    (output_destination_t)(1 << 8) // Output option for displays(cannot show all at once)
// 7 bytes reserved for the future
#define ALL_TIME_INDEPENDENT_OUTPUTS   (output_destination_t)(0x00FF) // Outputs that can be sent independently of time constraints(all at once)
#define ALL_TIME_DEPENDENT_OUTPUTS     (output_destination_t)(0xFF00) // Outputs that require time constraints (e.g., display)
#define ALL_OUTPUTS                    (output_destination_t)(0xFFFF) // Used to indicate all outputs
#define NO_OUTPUTS                     (output_destination_t)(0x0000) // Used to indicate no outputs are set

// Enum representing the status of a task(function called)
typedef enum
{
    FINISHED,
    NOT_FINISHED
} task_status_te;

/***
 * Checks for errors and handles them if any occur.
 * @param error_status The error status structure.
 */
void checkForErrors(error_manager_error_ts error_status);

/***
 * Filters out time-dependent outputs from the provided output mask.
 * Only time-independent outputs are retained.
 * @param output The original output destination mask to be filtered.
 * @return A mask containing only time-independent outputs.
 ***/
output_destination_t filterOutTimeDependentOutputs(output_destination_t output);

#endif