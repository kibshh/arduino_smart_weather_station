#ifndef DATA_ROUTER_H
#define DATA_ROUTER_H

#include <Arduino.h>
#include "../display/display.h"
#include "../sensors/sensors.h"
#include "../functionality/rtc.h"
#include "../error_manager/error_manager.h"
#include "data_router_common.h"


error_manager_error_code_te data_router_routeDataToOutput(data_router_output_component output_component,
                                                          data_router_input_data_ts data);

data_router_input_data_ts data_router_fetchDataFromInput(data_router_input_component input_component, 
                                                         uint8_t component_id);

#endif