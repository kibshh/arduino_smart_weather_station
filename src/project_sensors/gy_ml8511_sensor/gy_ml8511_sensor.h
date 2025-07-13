#ifndef GY_ML8511_SENSOR_H
#define GY_ML8511_SENSOR_H

#include "../../project_configs/project_configs.h"

#define GY_ML8511_SENSOR_PIN                   (A2)
#define GY_ML8511_SENSOR_ANALOG_INPUT_MAX      (uint16_t)(1023u)
#define GY_ML8511_SENSOR_VCC_VOLTAGE           (float)(5.0f)
#define GY_ML8511_SENSOR_OUTPUT_VOLTAGE_MIN    (float)(1.0f)
#define GY_ML8511_SENSOR_OUTPUT_VOLTAGE_MAX    (float)(2.8f)
#define GY_ML8511_SENSOR_OUTPUT_INTENSITY_MAX  (float)(15.0f)
#define GY_ML8511_SENSOR_OUTPUT_INTENSITY_MIN  (float)(0.0f)

bool GyMl8511Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index);

#endif