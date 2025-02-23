#include "app.h"
#include "app_common.h"

/* EXPORTED FUNCTIONS */
void checkForErrors(const control_error_ts *error)
{
    // If an error occurred, handle it
    if(ERROR_CODE_NO_ERROR != error->error_code)
    {
        control_handleError(error);
    }
}

output_destination_t filterOutTimeDependentOutputs(output_destination_t output)
{
    return (output & ALL_TIME_INDEPENDENT_OUTPUTS);
}

void sendToOutputAndCheckForErrors(control_io_t output, const control_data_ts *data)
{
    control_error_ts error;
    // Define output component
    control_device_ts output_component = {output, CONTROL_ID_UNUSED};

    error.component = output_component;
    error.error_code = control_routeDataToOutput(output, data);
    checkForErrors(&error);
}
/* *************************************** */