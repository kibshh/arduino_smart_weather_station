#include "calibration.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Initializes a calibration return structure with default values.
 * 
 * Sets `error_code` to `NO_ERROR`, `value` to `INVALID_VALUE`, 
 * assigns `id`, and marks calibration as `FINISHED`.
 * 
 * @param id Calibration ID.
 * @return Initialized `calibration_return_ts` structure.
 */
static calibration_return_ts initializeCalibrationReturnStruct(uint8_t id);

/* EXPORTED FUNCTIONS */
calibration_return_ts calibration_getReading(uint8_t id)
{
    calibration_return_ts return_data;
    return_data.error_code = ERROR_CODE_CALIBRATION_NO_CALIBRATIONS_CONFIGURED; // Set default error code to indicate no calibs are configured
    size_t catalog_len = calibration_interface_getCalibrationsLen(); // Get the number of configured calibrations
    if (CALIBRATION_INTERFACE_NO_CALIBRATIONS_CONFIGURED != catalog_len) // Check if any calibs are configured
    {
        calibration_interface_metadata_ts current_calibration = calibration_interface_getCalibrationMetadata(id);

        if(CALIBRATION_INTERFACE_STATUS_SUCCESS == current_calibration.success_status) // Check if calibration metadata retrieval succeeded
        {
            uint8_t num_of_measurements_type = current_calibration.metadata.num_of_measurements_type;
            // Determine if calibration requires single or multiple measurements
            if(CALIBRATION_MULTIPLE_MEASUREMENTS == num_of_measurements_type)
            {
                return_data = calibration_getMultipleMeasurementsCalibration(id);
            }
            else if(CALIBRATION_SINGLE_MEASUREMENT == num_of_measurements_type)
            {
                return_data = calibration_getSingleMeasurementCalibration(id);
            }
            else
            {
                return_data.error_code = ERROR_CODE_CALIBRATION_INVALID_NUM_OF_MEASUREMENTS_TYPE; // Invalid measurement type
            }
        }
        else
        {
            return_data.error_code = ERROR_CODE_CALIBRATION_CALIBRATION_NOT_FOUND; // Calibration metadata not found
        }
    }
    return return_data;
}

calibration_return_ts calibration_getMultipleMeasurementsCalibration(uint8_t id)
{
    unsigned long current_millis = millis(); // Get current time

    calibration_return_ts return_struct = initializeCalibrationReturnStruct(id);
    calibration_state_te current_calibration_status = CALIBRATION_STATE_IDLE;
    float calib_value = CALIBRATION_INVALID_VALUE;

    switch(id)
    {
        case MQ7_COPPM:
            bool calibration_starting_now = mq7_startCalculatingResistanceForCalibration(current_millis);
            if(MQ7_CALIBRATION_START_SUCCESS == calibration_starting_now)
            {
                // Calibration was not started, has started just yet
                current_calibration_status = CALIBRATION_STATE_IN_PROGRESS;
            }
            else
            {
                // Check if calibration results are available
                calib_value = mq7_getResultsCalculateResistanceForCalibration();
                if(CALIBRATION_INVALID_VALUE != calib_value)
                {
                    // Calibration has finished successfully
                    current_calibration_status = CALIBRATION_STATE_FINISHED;
                }
                else
                {
                    // Calibration is still in progress
                    current_calibration_status = CALIBRATION_STATE_IN_PROGRESS;
                }
            }
            break;
        default:
            return_struct.error_code = ERROR_CODE_CALIBRATION_SENSOR_AND_NUM_OF_MEASUREMENTS_INCOMPATIBLE;
    }
    if(ERROR_CODE_NO_ERROR == return_struct.error_code)
    {
        // Fill the struct with new values if no error occured
        return_struct.calibration_reading.value = calib_value;
        return_struct.calibration_reading.current_calibration_status = current_calibration_status;
    }
    return return_struct;
}

calibration_return_ts calibration_getSingleMeasurementCalibration(uint8_t id)
{
    calibration_return_ts return_struct = initializeCalibrationReturnStruct(id);

    switch(id)
    {
        case MQ135_PPM:
            float value = mq135_calculateResistanceForCalibration();
            if(CALIBRATION_INVALID_VALUE == value)
            {
                return_struct.error_code = ERROR_CODE_CALIBRATION_INVALID_VALUE_FROM_SENSOR;
            }
            else
            {
                return_struct.calibration_reading.value = value;
            }
            break;
        default:
            return_struct.error_code = ERROR_CODE_CALIBRATION_SENSOR_AND_NUM_OF_MEASUREMENTS_INCOMPATIBLE;
    }
    return return_struct;
}

static calibration_return_ts initializeCalibrationReturnStruct(uint8_t id)
{
    calibration_return_ts return_struct;

    return_struct.error_code = ERROR_CODE_NO_ERROR;
    return_struct.calibration_reading.value = CALIBRATION_INVALID_VALUE;
    return_struct.calibration_reading.calibration_id = id;
    return_struct.calibration_reading.current_calibration_status = CALIBRATION_STATE_FINISHED;

    return return_struct;
}
