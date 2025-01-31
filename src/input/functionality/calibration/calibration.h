#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>
#include "../../../input/input_types.h"
#include "calibration_interface/calibration_interface.h"
#if defined(SENSORS_MQ135_CALIBRATION_ENABLED)
#include "../../sensors/sensor_library/mq135/mq135.h"
#endif
#if defined(SENSORS_MQ7_CALIBRATION_ENABLED)
#include "../../sensors/sensor_library/mq7/mq7.h"
#endif

#define CALIBRATION_FIRST_SENSOR_INDEX            (0u)      // The index of the first sensor in the configuration

/**
 * @brief Retrieves a calibration reading for the specified sensor ID.
 * 
 * This function checks if any calibrations are configured. If so, it retrieves 
 * the metadata for the given sensor ID and determines whether it requires a 
 * single or multiple measurement calibration. It then calls the appropriate 
 * calibration function.
 * 
 * @param id Sensor ID for which calibration data is requested.
 * @return `calibration_return_ts` structure containing the calibration result 
 *         or an error code if the calibration is not found or invalid.
 */
calibration_return_ts calibration_getReading(uint8_t id);

/**
 * @brief Performs a multiple measurement calibration for the specified sensor ID.
 * 
 * This function handles the calibration process for sensors requiring multiple 
 * measurements. It checks if the calibration process has started or if results 
 * are available. If the calibration is still in progress, it updates the status accordingly.
 * 
 * @param id Sensor ID for which calibration is performed.
 * @return `calibration_return_ts` structure containing the calibration result 
 *         or an error code if the sensor is incompatible.
 */
calibration_return_ts calibration_getMultipleMeasurementsCalibration(uint8_t id);

/**
 * @brief Performs a single measurement calibration for the specified sensor ID.
 * 
 * This function handles the calibration process for sensors that require only 
 * a single measurement. It retrieves the calibration value and updates the result 
 * structure accordingly.
 * 
 * @param id Sensor ID for which calibration is performed.
 * @return `calibration_return_ts` structure containing the calibration result 
 *         or an error code if the sensor is incompatible.
 */
calibration_return_ts calibration_getSingleMeasurementCalibration(uint8_t id);

#endif