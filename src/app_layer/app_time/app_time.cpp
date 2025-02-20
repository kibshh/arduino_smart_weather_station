#include "app_time.h"

/* EXPORTED FUNCTIONS */
task_status_te app_readCurrentRtcTime(output_destination_t output)
{
    control_input_data_ts rtc_result = control_fetchDataFromInput(INPUT_RTC, RTC_DEFAULT_RTC); // Fetch data from the RTC
    checkForErrors(rtc_result.error_code, INPUT_RTC, RTC_DEFAULT_RTC);

    if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
    {
        // Send RTC data to display output and check for errors
        checkForErrors(control_routeDataToOutput(OUTPUT_DISPLAY, rtc_result.data).error_code, OUTPUT_DISPLAY, CONTROL_ID_UNUSED); 
    }
    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
    {
        // Send RTC data to serial console output and check for errors
        checkForErrors(control_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, rtc_result.data).error_code, OUTPUT_SERIAL_CONSOLE, CONTROL_ID_UNUSED); 
    }   
    return FINISHED;
}
/* *************************************** */