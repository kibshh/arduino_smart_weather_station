#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "../data_router/data_router.h"
#include "../project_settings.h"
#include "../error_manager/error_manager.h"

#define ALL_OUTPUTS                   (0u)  // Used to indicate all possible outputs
#define LCD_DISPLAY                   (1u)  // Output option for LCD display
#define SERIAL_CONSOLE                (2u)  // Output option for serial console

#define NUM_OF_SENSORS_NOT_CALCULATED (false) // Flag indicating sensors length has not been calculated
#define NUM_OF_SENSORS_CALCULATED     (true)  // Flag indicating sensors length has been calculated

#define STARTING_SENSOR_INDEX         (0u)  // Initial sensor index for cyclic display

#define REPEAT_CYCLIC                        (true)
#define REPEAT_ONCE                          (false)

#define CONTINUE_DISPLAYING_YES                  (true)
#define CONTINUE_DISPLAYING_NO                   (false)

/***
 * Cycles through all sensors, fetches their data, and routes the data to defined output(s).
 * @param output The output parameter.
 */
void app_displayAllSensors(uint8_t output, bool repeat);

/***
 * Fetches RTC data, and routes the data to defined output(s).
 * @param output The output parameter.
 */
void app_displayCurrentRtcTime(uint8_t output);

void app_displayAllI2CAddresses(uint8_t output, bool repeat);

#endif