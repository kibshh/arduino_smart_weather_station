#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>
#include "../../../input/input_types.h"
#if defined(SENSORS_MQ135_CALIBRATION_ENABLED)
#include "../../sensors/sensor_library/mq135/mq135.h"
#endif
#if defined(SENSORS_MQ7_CALIBRATION_ENABLED)
#include "../../sensors/sensor_library/mq7/mq7.h"
#endif

error_manager_error_code_te calibration_startMultipleMeasurementsCalibration(uint16_t num_of_measurements, unsigned long calibration_delay);
sensor_return_ts calibration_getCalibrationResult(uint8_t id);

#endif