#include "app_time.h"

/* EXPORTED FUNCTIONS */
task_status_te app_readCurrentRtcTime(output_destination_t output)
{
    data_router_input_data_ts rtc_result = data_router_fetchDataFromInput(INPUT_RTC, RTC_DEFAULT_RTC); // Fetch data from the RTC
    checkForErrors(rtc_result.error_msg);

    if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
    {
        checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, rtc_result)); // Send RTC data to display output and check for errors
    }
    if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
    {
        checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, rtc_result)); // Send RTC data to serial console output and check for errors
    }   
    return FINISHED;
}
/* *************************************** */