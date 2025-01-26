#include "serial_console.h"

error_manager_error_code_te serial_console_init()
{
    // Begin serial communication with the defined baud rate.
    Serial.begin(SERIAL_CONSOLE_BAUDRATE);

    // Check if serial is available but don't block program execution.
    if (!Serial) {
        // Return an error code if serial is not available.
        return ERROR_CODE_SERIAL_CONSOLE_INIT_FAILED;
    }
    // If no error occurred during initialization, return success code.
    return ERROR_CODE_NO_ERROR;
}