#include "app_time.h"

/* EXPORTED FUNCTIONS */
task_status_te app_readCurrentRtcTime(output_destination_t output)
{
    // Define input component and fetch sensor data
    control_device_ts time_component = {INPUT_RTC, RTC_DEFAULT_RTC};
    control_input_data_ts rtc_result = control_fetchDataFromInput(&time_component);
    // Handle input errors
    control_error_ts error = {rtc_result.error_code, time_component};
    checkForErrors(&error);

    if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
    {
        // Send RTC data to display output and check for errors
        sendToOutputAndCheckForErrors(OUTPUT_DISPLAY, &(rtc_result.data));
    }
    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
    {
        // Send RTC data to serial console output and check for errors
        sendToOutputAndCheckForErrors(OUTPUT_DISPLAY, &(rtc_result.data));
    }   
    return FINISHED;
}
/* *************************************** */