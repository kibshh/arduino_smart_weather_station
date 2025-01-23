#include "gy_ml8511.h"


void gy_ml8511_init()
{
  pinMode(SENSORS_GY_ML8511_PIN_ANALOG, INPUT);
}

float gy_ml8511_readUvIntensity()
{
  int analog_value = analogRead(SENSORS_GY_ML8511_PIN_ANALOG);
  float uv_voltage = ((float)analog_value / GY_ML8511_ANALOG_INPUT_MAX) * GY_ML8511_VCC_VOLTAGE;  //Convert to voltage
  
  // Convert voltage to intensity (UV intensity in mW/cm^2)
  float calculated_intensity = (float)map(uv_voltage, GY_ML8511_OUTPUT_VOLTAGE_MIN, GY_ML8511_OUTPUT_VOLTAGE_MAX, 
                                   SENSORS_GYML8511_UV_MIN, SENSORS_GYML8511_UV_MAX);  // range from datasheet

  return calculated_intensity;
}