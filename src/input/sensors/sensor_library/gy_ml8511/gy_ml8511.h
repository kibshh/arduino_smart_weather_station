#ifndef GY_ML8511_H
#define GY_ML8511_H

#include <Arduino.h>
#include "../../sensorsconfig.h"

#define GY_ML8511_OUTPUT_VOLTAGE_MIN    (0.99)
#define GY_ML8511_OUTPUT_VOLTAGE_MAX    (2.9)
#define GY_ML8511_ANALOG_INPUT_MAX      (1023u)
#define GY_ML8511_VCC_VOLTAGE           (5)

void gy_ml8511_init();
float gy_ml8511_readUvIntensity();

#endif