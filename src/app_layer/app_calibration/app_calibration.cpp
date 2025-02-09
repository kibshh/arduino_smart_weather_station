#include "app_calibration.h"

/* EXPORTED FUNCTIONS */
task_status_te app_readCalibrationValue(uint8_t sensor_id, output_destination_t output)
{
    data_router_input_data_ts calibration_result = data_router_fetchDataFromInput(INPUT_CALIBRATION, sensor_id); // Fetch data from Calibration
    checkForErrors(calibration_result.error_msg);

    // Check if calibration is complete
    if(CALIBRATION_STATE_FINISHED == calibration_result.data.input_return.calib_reading.current_calibration_status)
    {
        if(IS_OUTPUT_INCLUDED(output, LCD_DISPLAY))
        {
            checkForErrors(data_router_routeDataToOutput(OUTPUT_DISPLAY, calibration_result)); // Send calibration data to display output and check for errors
        }
        if(IS_OUTPUT_INCLUDED(output, SERIAL_CONSOLE))
        {
            checkForErrors(data_router_routeDataToOutput(OUTPUT_SERIAL_CONSOLE, calibration_result)); // Send calibration data to serial console output and check for errors
        }   
        return FINISHED;
    }
    return NOT_FINISHED;
}
/* *************************************** */