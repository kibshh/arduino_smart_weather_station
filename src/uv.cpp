#include "uv.h"


void uv_init()
{
  pinMode(UV_GY_ML8511_PIN_ANALOG, INPUT);
}

uv_reading_t uv_readUvIntensity()
{
  uv_reading_t reading = {0};
  reading.success = false;

  int analog_value = analogRead(UV_GY_ML8511_PIN_ANALOG);
  float uv_voltage = (analog_value / UV_GY_ML8511_ANALOG_INPUT_MAX) * UV_GY_ML8511_VCC_VOLTAGE;  //Convert to voltage
  
  // Convert voltage to intensity (UV intensity in mW/cm^2)
  float calculated_intensity = map(uv_voltage, UV_GY_ML8511_OUTPUT_VOLTAGE_MIN, UV_GY_ML8511_OUTPUT_VOLTAGE_MAX, UV_GY_ML8511_OUTPUT_INTENSITY_MIN, UV_GY_ML8511_OUTPUT_INTENSITY_MAX);  // range from datasheet
  reading.value = calculated_intensity;

  if(UV_GY_ML8511_OUTPUT_INTENSITY_MIN < calculated_intensity && UV_GY_ML8511_OUTPUT_INTENSITY_MAX > calculated_intensity)
  {
    reading.success = true;
  }

  return reading;
}