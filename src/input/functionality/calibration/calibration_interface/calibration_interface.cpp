#include "calibration_interface.h"

calibration_interface_metadata_ts calibration_interface_getCalibrationMetadata(uint8_t id)
{
    calibration_interface_metadata_ts metadata_return;
    metadata_return.success_status = CALIBRATION_INTERFACE_STATUS_FAILED; // Default failure status

    // Attempt to retrieve the calib metadata from the catalog
    if(CALIBRATION_METADATA_RETRIEVE_SUCCESS == calibration_metadata_getData(id, &metadata_return.metadata))
    {
        metadata_return.success_status = CALIBRATION_INTERFACE_STATUS_SUCCESS;
    }
    return metadata_return;
}

size_t calibration_interface_getCalibrationsLen()
{
    return calibration_metadata_getDataLen(); // Use the function from Metadata layer
}

uint8_t calibration_interface_calibrationIndexToId(uint8_t index) 
{
    return calibration_metadata_sensorIndexToId(index);
}