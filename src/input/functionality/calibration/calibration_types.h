#ifndef CALIBRATION_TYPES_H
#define CALIBRATION_TYPES_H

#include <Arduino.h>

/**
 * @brief Defines the invalid value for the calibration readings.
 * 
 * This macro represents a "Not a Number" (NAN) value, which is used to indicate 
 * invalid calibration readings in sensor.
 */
#define CALIBRATION_INVALID_VALUE    (NAN)

/**
 * @brief Enumeration representing the states of the sensor calibration process.
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
 * in the calibration workflow.
 */
typedef enum
{
  CALIBRATION_STATE_IDLE,
  CALIBRATION_STATE_IN_PROGRESS,
  CALIBRATION_STATE_FINISHED
} calibration_state_te;

#endif