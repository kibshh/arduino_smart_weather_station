#ifndef MQ135_H
#define MQ135_H

#include <Arduino.h>
#include "../sensors_config.h"

/* Load resistance in ohms which is connected to from analog output of sensor to ground (default value for the module) */
#define MQ135_LOAD_RESISTANCE_VAL           (10000)

/* Maximum and minimum analog values (10-bit ADC resolution) */
#define MQ135_ANALOG_INPUT_MAX              (1023)
#define MQ135_ANALOG_INPUT_MIN              (0)

/* Minimum valid analog value to avoid division by zero */
#define MQ135_ANALOG_INPUT_MIN_VALID        (1)

/* Minimum valid sensor resistance (R0) in ohms */
#define MQ135_R_ZERO_MINIMUM                (1u)

/* Defines the invalid value for the MQ135 sensor readings */
#define MQ135_INVALID_VALUE                 (NAN)

/**
 * @brief Initializes the MQ135 sensor.
 * 
 * Sets the MQ135 sensor pin as an input to prepare for reading analog values.
 */
void mq135_init();

/**
 * @brief Reads the PPM value from the MQ135 sensor.
 * 
 * This function calculates the Parts Per Million (PPM) concentration of gases
 * based on the analog reading and the defined sensor parameters. It ensures
 * that the analog reading and sensor configuration are valid before performing
 * the calculation.
 * 
 * @return The calculated PPM value or `MQ135_INVALID_VALUE` if parameters are
 * not defined or invalid.
 */
float mq135_readPPM();

/**
 * @brief Calculates the sensor's \( R0 \) value during calibration.
 * 
 * This function computes the baseline resistance (\( R0 \)) of the MQ135 sensor 
 * based on the analog input value during the calibration phase. The \( R0 \) value 
 * is crucial for determining the gas concentration later during normal operation. 
 * 
 * The function ensures the analog reading is within valid ranges and avoids division by zero. 
 * Unlike other gas sensors (e.g., MQ7), this function uses only one sample because the MQ135 
 * is designed for air quality monitoring in stable environments. Gas concentration does not 
 * fluctuate rapidly in such use cases, making multiple samples unnecessary.
 * 
 * @return float - The calculated \( R0 \) resistance (Ohms) or NAN if the input reading is invalid.
 */
float mq135_readResistanceForCalibration();

#endif