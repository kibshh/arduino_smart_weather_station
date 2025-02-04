#include "error_manager.h"

void error_manager_handleError(error_manager_error_ts error_msg) {
    error_manager_error_ts error_msg_serial = data_router_routeErrorToOutput(OUTPUT_SERIAL_CONSOLE, error_msg);
    if (error_msg_serial != ERROR_CODE_NO_ERROR) {
        data_router_routeErrorToOutput(OUTPUT_DISPLAY, error_msg);
    }
}