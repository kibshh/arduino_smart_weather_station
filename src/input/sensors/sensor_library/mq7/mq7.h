#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>
#include "../../sensorsconfig.h"

/** Maximum value of the analog input reading (10-bit ADC resolution). */
#define MQ7_ANALOG_INPUT_MAX              (1023)

/** Minimum value of the analog input reading. */
#define MQ7_ANALOG_INPUT_MIN              (0)

/** Analog output value for the heater at 5V. */
#define MQ7_5V_ANALOG_OUTPUT_HEATER       (1023u)  

/** Analog output value for the heater at 1.4V. */
#define MQ7_1_4V_ANALOG_OUTPUT_HEATER     (71u)   

/** The supply voltage of the sensor (5V). */
#define MQ7_VCC_VOLTAGE                   (5)

/** Heater flags. */
#define MQ7_HEATER_IS_ON                  (true)
#define MQ7_HEATER_IS_OFF                 (false)

/** Calibration progress flags */
#define MQ7_CALIBRATION_IN_PROGRESS       (true)
#define MQ7_CALIBRATION_NOT_IN_PROGRESS   (false)

/** Calibration success flags */
#define MQ7_CALIBRATION_FINISHED          (true)
#define MQ7_CALIBRATION_NOT_FINISHED      (false)

/** Clear air factor used in calibration (based on datasheet). */
#define MQ7_CLEAR_AIR_FACTOR              (9.83)  // By datasheet

/** Load resistance value in ohms, connected between the analog output of the sensor and ground (based on datasheet). */
#define MQ7_LOAD_RESISTANCE_VAL           (10000) // Load resistance in ohms

/** Base constant for the calculation of CO PPM, used in logarithmic equation. */
#define MQ7_CALCULATION_POW_BASE_CONSTANT (10)

#ifdef MQ7_CALIBRATION_ENABLED
/**
 * @brief Structure to hold calibration process data for the MQ7 sensor.
 *
 * This structure stores the accumulated resistance, timing, and measurement information 
 * required to perform and track the progress of the calibration process.
 */
typedef struct
{
  float accumulated_resistance;         /**< The accumulated resistance value during calibration. */
  unsigned long last_measurement_time;  /**< The timestamp of the last measurement taken. */
  bool calibration_in_progress;         /**< Flag indicating if the calibration process is currently ongoing. */
  uint16_t current_measurement;         /**< The index of the current measurement being taken in the calibration process. */
  uint16_t num_of_measurements;         /**< The total number of measurements to be taken for calibration. */
  unsigned long calibration_delay;      /**< Delay time between consecutive measurements during calibration. */
} mq7_calibration_helper_struct_ts;

/**
 * @brief Structure to hold the results of the MQ7 sensor calibration process.
 *
 * This structure stores the final calculated resistance and the calibration status 
 * after completing the calibration process.
 */
typedef struct 
{
  float calculated_resistance;  /**< The final resistance value calculated during calibration. */
  bool calibration_status;      /**< The status of the calibration process (finished or not). */
} mq7_calibration_return_struct_ts;

/**
 * Initializes the calibration process for the MQ-7 sensor.
 * This function sets up the calibration parameters and prepares the system 
 * to start measuring the sensor's resistance over a specified number of measurements.
 * 
 * @param current_millis The current time in milliseconds (e.g., from millis()), 
 *                       used to track the delay between consecutive measurements.
 * @param num_of_measurements The total number of resistance measurements to perform 
 *                            during the calibration process.
 * @param calibration_delay The delay (in milliseconds) between each measurement to ensure 
 *                          stable readings from the sensor.
 * 
 * @note This function must be followed by periodic calls to 
 *       `mq7_calculateResistanceForCalibration()` in the main loop to complete the calibration process.
 */
void mq7_startCalculatingResistanceForCalibration(unsigned long current_millis, uint16_t num_of_measurements);

/**
 * @brief Perform the MQ7 sensor calibration by accumulating resistance measurements over time.
 *
 * This function takes periodic ADC readings from the MQ7 sensor, converts them to resistance, 
 * and calculates the average resistance once the required number of measurements is reached. 
 * It then returns the calculated resistance adjusted by the clean air factor and updates the 
 * calibration status.
 *
 * @param current_millis The current time in milliseconds, used to manage timing of measurements.
 * @return A struct of type `mq7_calibration_return_struct_ts` containing:
 *         - The calculated resistance if calibration is complete.
 *         - The calibration status (finished or not finished).
 */
mq7_calibration_return_struct_ts mq7_calculateResistanceForCalibration(unsigned long current_millis);
#endif

/**
 * @brief Initialize the MQ7 sensor by setting up the necessary pins and starting the heating cycle.
 *
 * This function configures the analog input pin for reading sensor data and the PWM output pin for controlling the heater.
 * It then turns on the heater to start the sensor's heating process, preparing it for measurements.
 */
void mq7_init();

/**
 * Handles the heating cycle for the MQ-7 sensor.
 * Alternates between heating (high state) and cooling (low state) phases based on time intervals.
 * NEEDS TO BE CALLED IN A LOOP.
 * 
 * @param current_millis The current time in milliseconds (e.g., from millis()).
 */
void mq7_heatingCycle(unsigned long current_millis);

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

#endif