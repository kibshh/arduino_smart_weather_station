#ifndef UV_H
#define UV_H

#include <Arduino.h>

#define UV_GY_ML8511_PIN_ANALOG            (A2)
#define UV_GY_ML8511_OUTPUT_VOLTAGE_MIN    (0.99)
#define UV_GY_ML8511_OUTPUT_VOLTAGE_MAX    (2.9)
#define UV_GY_ML8511_ANALOG_INPUT_MAX      (1023u)
#define UV_GY_ML8511_VCC_VOLTAGE           (5)

typedef struct
{
  boolean success;
  float value;
}uv_reading_t;


void uv_init();
uv_reading_t uv_readUvIntensity();

#endif