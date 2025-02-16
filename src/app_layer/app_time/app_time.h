#ifndef APP_TIME_H
#define APP_TIME_H

#include <Arduino.h>
#include "../app_common.h"

/**
 * @brief Reads the current RTC time and routes it to the specified output.
 *
 * Fetches the current time from the RTC module and sends it to the selected 
 * output(s) (LCD, serial console, or both). Handles any errors encountered 
 * during data retrieval or routing.
 *
 * @param output The output destination (LCD, serial console, or both).
 * @return task_status_te Returns FINISHED to notify the task component.
 */
task_status_te app_readCurrentRtcTime(output_destination_t output);

#endif