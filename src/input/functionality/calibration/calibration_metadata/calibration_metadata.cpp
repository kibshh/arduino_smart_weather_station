#include "calibration_metadata.h"

/* It's very important to keep the strings (calibration_type and calibration_unit) short enough to fit into the allocated buffer size.
   Ensure that:
   - `calibration_type` does not exceed 40 characters.
   - `calibration_unit` does not exceed 10 characters.
   This is to avoid potential overflow issues when reading from flash memory and formatting the final output in the buffer. */
const calibration_metadata_ts calibration_metadata[] PROGMEM =
    {
#ifdef SENSORS_MQ135_CALIBRATION_ENABLED
        {
         "MQ135 R0 - clean air resistance",
         "ohms",
         MQ135_PPM,
         CALIBRATION_SINGLE_MEASUREMENT,
         CALIBRATION_DISPLAY_0_DECIMALS,
         CALIBRATION_DISPLAY_5_LETTERS
        },
#endif
#ifdef SENSORS_MQ7_CALIBRATION_ENABLED
        {
         "MQ7 R0 - clean air resistance",
         "ohms",
         MQ7_COPPM,
         CALIBRATION_MULTIPLE_MEASUREMENTS,
         CALIBRATION_DISPLAY_0_DECIMALS,
         CALIBRATION_DISPLAY_3_LETTERS
        },
#endif
};

bool calibration_metadata_getData(uint8_t id, calibration_metadata_ts * current_calibration)
{
    bool success_status = CALIBRATION_METADATA_RETRIEVE_FAILED;
    size_t num_of_calibrations = calibration_metadata_getDataLen();
    uint8_t current_id = INVALID_SENSOR_ID;

    // Loop through all calibration data
    for (uint8_t index = CALIBRATION_METADATA_FIRST_CALIB_INDEX; index < num_of_calibrations; index++)
    {
        current_id = pgm_read_byte(&calibration_metadata[index].sensor_id); // Read from program memory
        if(id == current_id)
        {
            // Copy the calib configuration from program memory to the provided structure
            memcpy_P(current_calibration, &calibration_metadata[index], sizeof(calibration_metadata_ts));
            success_status = CALIBRATION_METADATA_RETRIEVE_SUCCESS; // Mark as successful
            break;
        }
    }
    return success_status;
}

size_t calibration_metadata_getDataLen()
{
  size_t data_len = CALIBRATION_METADATA_NO_CALIBS_CONFIGURED; // Default value in case of no calibs configured
  if (CALIBRATION_METADATA_NO_CALIBS_CONFIGURED < sizeof(calibration_metadata)) // Ensure catalog is not empty
  {
    data_len = sizeof(calibration_metadata) / sizeof(calibration_metadata[CALIBRATION_METADATA_FIRST_CALIB_INDEX]);
  }
  return data_len;
}

uint8_t calibration_metadata_sensorIndexToId(uint8_t index) 
{
  uint8_t sensor_id = INVALID_SENSOR_ID; // Default sensor ID in case index is out of bounds or there are no calibs configured
  size_t num_of_calibs = calibration_metadata_getDataLen();
  if(index < num_of_calibs && CALIBRATION_METADATA_FIRST_CALIB_INDEX <= index)
  {
    sensor_id = pgm_read_byte(&calibration_metadata[index].sensor_id); // Convert to sensor ID
  }
  return sensor_id;
}