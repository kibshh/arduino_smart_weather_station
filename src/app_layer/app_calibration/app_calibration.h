#ifndef APP_CALIBRATION_H
#define APP_CALIBRATION_H

#include <Arduino.h>
#include "../app_common.h"

/**
 * @brief Reads the calibration value for a given sensor and routes it to the specified output(s).
 *
 * This function fetches calibration data from the data router, checks for errors,
 * and if the calibration is completed, it sends the data to the requested output destinations.
 *
 * @param sensor_id The ID of the sensor whose calibration value is to be read.
 * @param output The output destination(s) (e.g., LCD display, serial console).
 * @return task_status_te Returns FINISHED if calibration is complete and data is sent, otherwise NOT_FINISHED.
 */
task_status_te app_readCalibrationValue(uint8_t sensor_id, output_destination_t output);

#endif