#include "app.h"
#include "app_common.h"

/* EXPORTED FUNCTIONS */
void checkForErrors(control_error_code_te error_code, control_component_te component, uint8_t component_id)
{
    // If an error occurred, handle it
    if(ERROR_CODE_NO_ERROR != error_code)
    {
        control_handleError(error_code, component, component_id);
    }
}

output_destination_t filterOutTimeDependentOutputs(output_destination_t output)
{
    return (output & ALL_TIME_INDEPENDENT_OUTPUTS);
}
/* *************************************** */