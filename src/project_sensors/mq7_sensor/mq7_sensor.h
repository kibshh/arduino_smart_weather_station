#ifndef MQ7_SENSOR_H
#define MQ7_SENSOR_H

#include "../../project_configs/project_configs.h"

#define MQ7_SENSOR_PIN_ANALOG                    (A1)
#define MQ7_SENSOR_PIN_PWM_HEATER                (uint8_t)(9u) /* pwm pin */
#define MQ7_SENSOR_5V_ANALOG_OUTPUT_HEATER       (uint16_t)(1023u)     
#define MQ7_SENSOR_1_4V_ANALOG_OUTPUT_HEATER     (uint16_t)(71u)   
#define MQ7_SENSOR_HEATER_LOW_TIMEOUT_MS         (uint64_t)(90000u)           
#define MQ7_SENSOR_HEATER_HIGH_TIMEOUT_MS        (uint64_t)(60000u)   
#define MQ7_SENSOR_R_ZERO                        (uint16_t)(10000u)
#define MQ7_SENSOR_VCC_VOLTAGE                   (float)(5.0f)
#define MQ7_SENSOR_ANALOG_INPUT_MAX              (uint16_t)(1023u)
#define MQ7_SENSOR_LOAD_RESISTANCE_VAL           (uint16_t)(10000u) /* Load resistance in ohms which is connected to from analog output of sensor to ground */
#define MQ7_SENSOR_CALCULATION_POW_BASE_CONSTANT (uint8_t)(10u)
#define MQ7_SENSOR_CALCULATION_INTERCEPT         (float)(0.5f)   /* By datasheet, represents intercept in a graph */
#define MQ7_SENSOR_CALCULATION_SLOPE             (float)(-0.27f) /* By datasheet, represents slope in a graph */
#define MQ7_SENSOR_PPM_MINIMUM                   (float)(0.0f)
#define MQ7_SENSOR_PPM_MAXIMUM                   (float)(1000.0f)  

bool Mq7Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index);
void Mq7Sensor_HeatingCycle();

#endif