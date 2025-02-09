#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#include <Arduino.h>
#include "error_manager_types.h"
#include "../data_router/data_router.h"

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