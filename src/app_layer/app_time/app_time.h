#ifndef APP_TIME_H
#define APP_TIME_H

#include <Arduino.h>
#include "../app_common.h"

/***
 * Retrieves the current RTC time and sends it to the specified output destinations.
 * @param output The output destinations where the RTC time should be displayed (e.g., LCD display, serial console).
 * @return FINISHED after processing.
 *
 * This function fetches the current time from the RTC module and routes the data 
 * to the selected outputs. It checks for errors in data retrieval and output routing.
 */
task_status_te app_readCurrentRtcTime(output_destination_t output);

#endif