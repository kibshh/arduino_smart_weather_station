#ifndef SERIAL_CONSOLE_H
#define SERIAL_CONSOLE_H

#include <Arduino.h>
#include "../../error_manager/error_codes.h"
#include "serial_console_config.h"


/**
 * @brief Initializes the serial console communication.
 *
 * This function begins serial communication at the specified baud rate
 * and does not block the program execution if the serial port is not available.
 *
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Serial communication initialized successfully.
 * - ERROR_CODE_SERIAL_INIT_FAILED: Failed to initialize the serial communication.
 */
error_manager_error_code_te serial_console_init();

#endif