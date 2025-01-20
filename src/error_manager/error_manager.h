#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#include <Arduino.h>
#include "error_codes.h"
#include "../data_router/data_router_types.h"

typedef struct
{
  data_router_output_component_te output_component;
  data_router_input_component_te input_type;
  uint8_t input_id;
} error_manager_output_error_ts;

typedef struct
{
  data_router_input_component_te input_component;
  uint8_t input_id;
} error_manager_input_error_ts;

typedef union
{
  error_manager_output_error_ts output_error;
  error_manager_input_error_ts input_error;
} error_type_tu;

typedef struct
{
  error_manager_error_code_te error_code;
  data_router_io_type io_flag;
  error_type_tu component;
} error_manager_error_ts;

void error_manager_handleError(error_manager_error_ts error_msg);

#endif