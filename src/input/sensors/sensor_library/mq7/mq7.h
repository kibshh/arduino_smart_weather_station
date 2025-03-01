#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>
#include "../sensors_config.h"

/* Maximum value of the analog input reading (10-bit ADC resolution). */
#define MQ7_ANALOG_INPUT_MAX              (int)(1023)

/* Minimum value of the analog input reading. */
#define MQ7_ANALOG_INPUT_MIN              (int)(0)

/* Minimum valid analog value to avoid division by zero */
#define MQ7_ANALOG_INPUT_MIN_VALID        (int)(1)

/* Analog output value for the heater at 5V. */
#define MQ7_5V_ANALOG_OUTPUT_HEATER       (int)(1023u)  

/* Analog output value for the heater at 1.4V. */
#define MQ7_1_4V_ANALOG_OUTPUT_HEATER     (int)(71u)   

/* The supply voltage of the sensor (5V). */
#define MQ7_VCC_VOLTAGE                   (float)(5)

/* Heater flags. */
#define MQ7_HEATER_IS_ON                  (bool)(true)
#define MQ7_HEATER_IS_OFF                 (bool)(false)

/* Load resistance value in ohms, connected between the analog output of the sensor and ground (based on datasheet). */
#define MQ7_LOAD_RESISTANCE_VAL           (float)(10000) // Load resistance in ohms

/* Base constant for the calculation of CO PPM, used in logarithmic equation. */
#define MQ7_CALCULATION_POW_BASE_CONSTANT (float)(10)

/* Defines the invalid value for the MQ7 sensor readings */
#define MQ7_INVALID_VALUE                 (NAN)

/**
 * @brief Initialize the MQ7 sensor by setting up the necessary pins and starting the heating cycle.
 *
 * This function configures the analog input pin for reading sensor data and the PWM output pin for controlling the heater.
 * It then turns on the heater to start the sensor's heating process, preparing it for measurements.
 */
void mq7_init();

/**
 * @brief Reads the carbon monoxide concentration in parts per million (PPM) using the MQ7 sensor.
 *
 * This function reads the analog value from the MQ7 sensor, converts it to resistance, and calculates the carbon monoxide
 * concentration in PPM based on predefined calibration constants. If the necessary constants or analog input range are 
 * not defined or invalid, the function returns NaN.
 *
 * @return float The carbon monoxide concentration in PPM or NaN if calibration parameters are missing or invalid.
 */
float mq7_readPPM();

/**
 * @brief Reads the resistance of the MQ-7 sensor for calibration.
 *
 * This function performs a single ADC reading from the MQ-7 sensor, validates the reading, 
 * and calculates the sensor resistance (Rs) using the voltage divider formula.
 *
 * @return The calculated sensor resistance in ohms, or MQ7_INVALID_VALUE if the reading is out of range.
 */
float mq7_readResistanceForCalibration();

/**
 * @brief Handles the heating cycle for the MQ-7 sensor.
 * 
 * Alternates between heating (high state) and cooling (low state) phases based on time intervals.
 * NEEDS TO BE CALLED IN A LOOP.
 * 
 * @param current_millis The current time in milliseconds (e.g., from millis()).
 */
void mq7_heatingCycle(const uint32_t *current_millis);

#endif