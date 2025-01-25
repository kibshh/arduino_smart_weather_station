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

/** Calibration start success flags */
#define MQ7_CALIBRATION_START_SUCCESS     (true)
#define MQ7_CALIBRATION_START_FAILED      (false)

/** Load resistance value in ohms, connected between the analog output of the sensor and ground (based on datasheet). */
#define MQ7_LOAD_RESISTANCE_VAL           (10000) // Load resistance in ohms

/** Base constant for the calculation of CO PPM, used in logarithmic equation. */
#define MQ7_CALCULATION_POW_BASE_CONSTANT (10)

/**
 * @brief Defines the invalid value for the MQ7 sensor readings.
 * 
 * This macro represents a "Not a Number" (NAN) value, which is used to indicate 
 * invalid readings for the MQ7 sensor.
 */
#define MQ7_INVALID_VALUE                 (NAN)

#ifdef SENSORS_MQ7_CALIBRATION_ENABLED
/**
 * @brief Enumeration representing the states of the MQ7 sensor calibration process.
 * 
 * This enum is used to define and track the state of the calibration state machine. 
 * It helps to manage the flow of the calibration process and ensures a clear 
 * transition between different stages.
 * 
 * - `CALIBRATION_STATE_IDLE`: Indicates that the calibration process is not active. 
 *   The system is in its default state, waiting for the calibration to be triggered.
 * 
 * - `CALIBRATION_STATE_IN_PROGRESS`: Indicates that the calibration is currently
 *   ongoing. During this state, the system will periodically take measurements and
 *   accumulate results until the specified number of measurements is completed.
 * 
 * - `CALIBRATION_STATE_FINISHED`: Indicates that the calibration process has 
 *   successfully completed. At this stage, the calculated calibration result is 
 *   available for retrieval, and the state machine can be reset.
 * 
 * This enumeration is essential for implementing the non-blocking state machine logic 
 * in the MQ7 calibration workflow.
 */
typedef enum
{
  CALIBRATION_STATE_IDLE,
  CALIBRATION_STATE_IN_PROGRESS,
  CALIBRATION_STATE_FINISHED
} mq7_calibration_state_te;

/**
 * @brief Structure to hold calibration process data for the MQ7 sensor.
 *
 * This structure stores the accumulated resistance, timing, and measurement information 
 * required to perform and track the progress of the calibration process.
 */
typedef struct
{
  float accumulated_resistance;                       /**< Sum of all resistance values measured during calibration. Used to calculate the average resistance. */
  unsigned long last_measurement_time;                /**< The timestamp (in milliseconds) when the last measurement was taken. Helps ensure proper delays between measurements. */
  mq7_calibration_state_te calibration_state;         /**< The current state of the calibration process (idle, in progress, or finished). */
  uint16_t current_measurement;                       /**< Counter for the number of measurements already taken. Tracks progress during calibration. */
  uint16_t num_of_measurements;                       /**< The total number of measurements to be performed for a complete calibration cycle. */
  unsigned long calibration_delay;                    /**< Delay (in milliseconds) between consecutive measurements to allow sensor stabilization. */
  float calculated_resistance;                        /**< The final calculated resistance (R0) value after the calibration process completes. */
} mq7_calibration_helper_struct_ts;

/**
 * @brief Initializes the calibration process for the MQ-7 sensor.
 * 
 * This function sets up the calibration parameters and prepares the system 
 * to start measuring the sensor's resistance over a specified number of measurements. 
 * The calibration process calculates the sensor's baseline resistance (\( R0 \)), 
 * which represents the sensor resistance in clean air. This \( R0 \) value is essential 
 * for determining gas concentrations during normal operation.
 * 
 * @param current_millis The current time in milliseconds (e.g., from millis()), 
 *                       used to track the delay between consecutive measurements.
 * @param num_of_measurements The total number of resistance measurements to perform 
 *                            during the calibration process.
 * @param calibration_delay The delay (in milliseconds) between each measurement to ensure 
 *                          stable readings from the sensor.
 * 
 * @note This function must be followed by periodic calls to 
 *       `mq7_calibratingLoopFunction()` in the main loop to complete the calibration process.
 */
void mq7_startCalculatingResistanceForCalibration(unsigned long current_millis, uint16_t num_of_measurements);

/**
 * @brief Handles the MQ7 sensor calibration by accumulating resistance measurements over time.
 *
 * This function manages the calibration process by taking periodic ADC readings from the MQ7 sensor,
 * converting them to resistance, and accumulating the results. Once the required number of measurements
 * is reached, it calculates the average resistance and updates the calibration state to indicate that
 * calibration is complete.
 *
 * @param current_millis The current time in milliseconds, used to manage timing of measurements 
 *                       and ensure proper delays between consecutive readings.
 * 
 * @note This function does not return any value. The calibration progress and results are updated 
 *       in the `mq7_calibration_helper_struct_ts` structure. Ensure to periodically call this function 
 *       in your loop to complete the calibration process.
 */
void mq7_calibratingLoopFunction(unsigned long current_millis);

/**
 * @brief Retrieve the calculated resistance after MQ7 sensor calibration is complete.
 *
 * This function provides the final calculated resistance value for the MQ7 sensor 
 * once the calibration process has finished. If the calibration process has not 
 * started or is still ongoing, it returns an invalid value (`MQ7_INVALID_VALUE`). 
 * Once the result is retrieved, the calibration state is reset to `IDLE`, allowing 
 * for future calibrations to be initiated.
 *
 * @return The calculated resistance value if calibration is complete; 
 *         otherwise, returns `MQ7_INVALID_VALUE`.
 *
 * Usage Notes:
 * - Ensure that this function is called only after starting and completing the 
 *   calibration process. 
 * - The calibration state will automatically reset to `IDLE` after calling this function, 
 *   so it must be started again for subsequent calibrations.
 */
float mq7_getResultsCalculateResistanceForCalibration();
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