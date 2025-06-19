#ifndef MQ135_SENSOR_H
#define MQ135_SENSOR_H

#include "../../project_configs/project_configs.h"

#define MQ135_SENSOR_PIN_ANALOG                    (A0)
#define MQ135_SENSOR_PARAMETER_A                   (float)(116.6f)
#define MQ135_SENSOR_PARAMETER_B                   (float)(2.77f)
#define MQ135_SENSOR_R_ZERO                        (uint16_t)(30000f)
#define MQ135_SENSOR_PPM_MINIMUM                   (float)(0.0f)
#define MQ135_SENSOR_PPM_MAXIMUM                   (float)(10000.0f)
#define MQ135_SENSOR_ANALOG_INPUT_MAX              (uint16_t)(1023u)
#define MQ135_SENSOR_ANALOG_INPUT_MIN              (uint16_t)(0u)
#define MQ135_SENSOR_VCC_VOLTAGE                   (float)(5.0f)
#define MQ135_SENSOR_LOAD_RESISTANCE               (uint16_t)(10000u)

#endif