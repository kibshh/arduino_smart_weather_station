#ifndef DATA_ROUTER_H
#define DATA_ROUTER_H

#include <Arduino.h>
#include "../display/display.h"
#include "../error_manager/error_manager.h"
#include "data_router_common.h"


error_manager_error_code_te data_router_routeDataToOutput(data_router_output_component output_component,
                                                          data_router_output_input_type input_type,
                                                          uint8_t *payload,
                                                          size_t payload_len);

data_router_receive_ts data_router_fetchDataFromInput(data_router_input_component input_component, 
                                                      uint8_t *params, 
                                                      size_t params_len,
                                                      uint8_t *return_buffer, 
                                                      size_t return_buffer_size);

#endif