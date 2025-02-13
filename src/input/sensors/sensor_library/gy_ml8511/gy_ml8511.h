#ifndef GY_ML8511_H
#define GY_ML8511_H

#include <Arduino.h>
#include "../sensorsconfig.h"

// Minimum output voltage of the GY-ML8511 UV sensor (in volts)
#define GY_ML8511_OUTPUT_VOLTAGE_MIN    (0.99)

// Maximum output voltage of the GY-ML8511 UV sensor (in volts)
#define GY_ML8511_OUTPUT_VOLTAGE_MAX    (2.9)

// Maximum analog input value for the Arduino ADC (10-bit resolution, range 0-1023)
#define GY_ML8511_ANALOG_INPUT_MAX      (1023)

// Supply voltage for the GY-ML8511 UV sensor, typically 3.3V
#define GY_ML8511_VCC_VOLTAGE           (3.3)

/**
 * @brief Initializes the GY-ML8511 UV sensor.
 * 
 * Sets the analog pin for reading UV intensity as input.
 */
void gy_ml8511_init();

/**
 * @brief Reads the UV intensity from the GY-ML8511 sensor.
 * 
 * Converts the analog reading to a voltage and then maps it to UV intensity.
 * 
 * @return float UV intensity in mW/cm^2.
 */
float gy_ml8511_readUvIntensity();

#endif