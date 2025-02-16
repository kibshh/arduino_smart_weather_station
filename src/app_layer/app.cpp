#include "app.h"
#include "app_common.h"

/* EXPORTED FUNCTIONS */
void checkForErrors(control_error_ts error_status)
{
    // If an error occurred, handle it
    if(ERROR_CODE_NO_ERROR != error_status.error_code)
    {
        /* Uncomment when error handler is finished */
        // control_handleError(error_status);
    }
}

output_destination_t filterOutTimeDependentOutputs(output_destination_t output)
{
    return (output & ALL_TIME_INDEPENDENT_OUTPUTS);
}
/* *************************************** */