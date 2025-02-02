#include "serial_console.h"

error_manager_error_code_te serial_console_init()
{
    // Begin serial communication with the defined baud rate.
    Serial.begin(SERIAL_CONSOLE_BAUDRATE);

    // Check if serial is available but don't block program execution.
    if (!Serial) {
        // Return an error code if serial is not available.
        return ERROR_CODE_SERIAL_CONSOLE_INIT_FAILED;
    }
    // If no error occurred during initialization, return success code.
    return ERROR_CODE_NO_ERROR;
}

error_manager_error_code_te serial_console_displayData(data_router_data_ts data)
{
  // Default error code for invalid input type
  error_manager_error_code_te error_code = ERROR_CODE_INVALID_INPUT_TYPE;

  switch(data.input_type)
  {
    case INPUT_SENSORS:
      error_code = serial_console_displaySensorMeasurement(data.input_return.sensor_reading); // Display sensor data
      break;

    case INPUT_RTC:
      error_code = serial_console_displayTime(data.input_return.rtc_reading); // Display RTC time data 
      break;

    case INPUT_I2C_SCAN:
      error_code = serial_console_displayI2cScan(data.input_return.i2cScan_reading); // Display I2C scan results
      break;

    case INPUT_CALIBRATION:
      error_code = serial_console_displayCalibrationResults(data.input_return.calib_reading); // Display calibration results

    default:
      // No action, error code is already set
      break;
  }

  return error_code;
}

error_manager_error_code_te serial_console_displaySensorMeasurement(sensor_reading_ts sensor_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  // Retrieve sensor metadata
  sensors_interface_metadata_ts sensor_metadata = sensors_interface_getSensorMetadata(sensor_data.sensor_id);

  // Check if metadata retrieval was successful
  if(SENSORS_INTERFACE_STATUS_SUCCESS == sensor_metadata.success_status)
  {
    // Extract metadata fields (display_num_of_letters is not needed in this case since everything is displayed)
    const char* sensor_type = sensor_metadata.metadata.sensor_type;
    const char* measurement_unit = sensor_metadata.metadata.measurement_unit;
    uint8_t measurement_type = sensor_metadata.metadata.measurement_type;
    uint8_t num_of_decimals = sensor_metadata.metadata.num_of_decimals;

    char display_string[SERIAL_CONSOLE_STRING_RESERVED_LARGE]; // Buffer for output string
    char val[SERIAL_CONSOLE_DTOSTRF_BUFFER_SIZE]; // Buffer for value string

    bool proceed_with_display = SERIAL_CONSOLE_PROCEED_WITH_DISPLAY;

    // Handle value-based measurements
    if(SENSORS_MEASUREMENT_TYPE_VALUE == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_VALUE == measurement_type)
    {
      dtostrf(sensor_data.value, SERIAL_CONSOLE_MIN_FLOAT_STRING_LEN, num_of_decimals, val); // Convert float to char array
    }
    // Handle indication-based measurements
    else if(SENSORS_MEASUREMENT_TYPE_INDICATION == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_INDICATION == measurement_type)
    {
      strncpy(val, (sensor_data.indication ? "yes" : "no"), sizeof(val) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
      val[sizeof(val) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE] = '\0'; // Ensure null termination
    }
    else
    {
      // Set error code for invalid measurement type
      error_code = ERROR_CODE_SERIAL_CONSOLE_INVALID_MEASUREMENT_TYPE;
      proceed_with_display = SERIAL_CONSOLE_DONT_PROCEED_WITH_DISPLAY;
    }
    // Format and display the sensor data if everything is okay
    if(SERIAL_CONSOLE_PROCEED_WITH_DISPLAY == proceed_with_display)
    {
      snprintf(display_string, sizeof(display_string), "%s: %s%s", sensor_type, val, measurement_unit);
      Serial.println(display_string);
    }
  }
  else
  {
    error_code = ERROR_CODE_SERIAL_CONSOLE_SENSOR_NOT_CONFIGURED;
  }
  return error_code;
}

error_manager_error_code_te serial_console_displayTime(rtc_reading_ts time_data)
{
  // Extract time components
  uint16_t year = time_data.year;
  uint8_t month = time_data.month;
  uint8_t day = time_data.day;
  uint8_t hour = time_data.hour;
  uint8_t mins = time_data.mins;
  uint8_t secs = time_data.secs;

  // Buffer for formatted time string
  char time_string[SERIAL_CONSOLE_STRING_RESERVED_MEDIUM]; // Ensures enough space

  // Format the time string with zero-padding
  snprintf(time_string, sizeof(time_string), 
           "Current time: %02u:%02u %02u/%02u/%u", 
           hour, mins, day, month, year);

  // Display the formatted time
  Serial.println(time_string);

  return ERROR_CODE_NO_ERROR;
}

error_manager_error_code_te serial_console_displayI2cScan(i2cScan_reading_ts i2c_scan_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  bool proceed_with_display = SERIAL_CONSOLE_PROCEED_WITH_DISPLAY;

  char display_string[SERIAL_CONSOLE_STRING_RESERVED_GIANT]; // Allocate a reasonable buffer
  char addr_string[SERIAL_CONSOLE_HEX_ADDR_STRING_LEN]; // Buffer for hexadecimal address representation

  // Handle scan for all devices mode
  if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES == i2c_scan_data.device_address)
  {
    snprintf(addr_string, sizeof(addr_string), "%02X", i2c_scan_data.current_i2c_addr);
    snprintf(display_string, sizeof(display_string), "I2C scan - I2C device found at address: 0x%s", addr_string);
  }
  else
  {
    snprintf(addr_string, sizeof(addr_string), "%02X", i2c_scan_data.device_address);
    snprintf(display_string, sizeof(display_string), "I2C device on address 0x%s status: ", addr_string);

    char status_msg[SERIAL_CONSOLE_STRING_RESERVED_LARGE]; // Buffer for the status message
    // Interpret and append the device status
    switch (i2c_scan_data.single_device_status)
    {
      case I2CSCAN_TRANSMISSION_RESULT_SUCCESS:
        strncpy(status_msg, "Successful transmission", sizeof(status_msg) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
        break;
      case I2CSCAN_TRANSMISSION_RESULT_TOOLONG:
        strncpy(status_msg, "Data too long to fit in transmit buffer", sizeof(status_msg) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
        break;
      case I2CSCAN_TRANSMISSION_RESULT_NACKADR:
        strncpy(status_msg, "Received NACK on transmit of the address", sizeof(status_msg) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
        break;
      case I2CSCAN_TRANSMISSION_RESULT_NACKDAT:
        strncpy(status_msg, "Received NACK on transmit of the data", sizeof(status_msg) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
        break;
      case I2CSCAN_TRANSMISSION_RESULT_UNKNOWN:
        strncpy(status_msg, "Unknown error occurred during communication", sizeof(status_msg) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
        break;
      default:
        error_code = ERROR_CODE_SERIAL_CONSOLE_UNKNOWN_I2C_DEVICE_STATUS;
        proceed_with_display = SERIAL_CONSOLE_DONT_PROCEED_WITH_DISPLAY;
        break;
    }

    if (SERIAL_CONSOLE_PROCEED_WITH_DISPLAY == proceed_with_display)
    {
      strncat(display_string, status_msg, sizeof(display_string) - strlen(display_string) - SERIAL_CONSOLE_NULL_TERMINATOR_SIZE);
    }
  }
  /* IMPORTANT: Check of invalid I2C address is done on I2C scanner side and it should not arrive on the Serial Console */
  // Display the formatted string if no error occurred
  if(SERIAL_CONSOLE_PROCEED_WITH_DISPLAY == proceed_with_display)
  {
    Serial.println(display_string);
  }

  return error_code;
}

error_manager_error_code_te serial_console_displayCalibrationResults(calibration_reading_ts calibration_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  // Fetch metadata for the given calibration ID
  calibration_interface_metadata_ts calib_metadata = calibration_interface_getCalibrationMetadata(calibration_data.calibration_id);

  // Proceed only if calibration metadata is successfully retrieved
  if(CALIBRATION_INTERFACE_STATUS_SUCCESS == calib_metadata.success_status)
  {
    bool proceed_with_display = SERIAL_CONSOLE_PROCEED_WITH_DISPLAY;

    // Extract metadata fields
    const char* calibration_type = calib_metadata.metadata.calibration_type;
    const char* calibration_unit = calib_metadata.metadata.calibration_unit;
    uint8_t num_of_measurements_type = calib_metadata.metadata.num_of_measurements_type;
    uint8_t num_of_decimals = calib_metadata.metadata.num_of_decimals;

    // Buffer for formatted display string
    char display_string[SERIAL_CONSOLE_STRING_RESERVED_GIANT]; // Enough space for formatted output

    if(CALIBRATION_MULTIPLE_MEASUREMENTS == num_of_measurements_type)
    {
      switch (calibration_data.current_calibration_status)
      {
        case CALIBRATION_STATE_IDLE:
          // Should never occur since calibration at least goes to 'in progress'
          snprintf(display_string, sizeof(display_string), "%s: No calibration active", calibration_type);
          break;

        case CALIBRATION_STATE_IN_PROGRESS:
          snprintf(display_string, sizeof(display_string), "%s: Calibration in progress...", calibration_type);
          break;

        case CALIBRATION_STATE_FINISHED:
        {
          // Convert float value to string with proper decimal places
          char val[SERIAL_CONSOLE_DTOSTRF_BUFFER_SIZE]; // Temporary buffer for float conversion
          dtostrf(calibration_data.value, SERIAL_CONSOLE_MIN_FLOAT_STRING_LEN, num_of_decimals, val); // Convert float to string

          snprintf(display_string, sizeof(display_string), "Calibration for %s: %s%s", calibration_type, val, calibration_unit);
          break;
        }

        default:
          error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS;
          proceed_with_display = SERIAL_CONSOLE_DONT_PROCEED_WITH_DISPLAY;
          break;
      }
    }
    else if(CALIBRATION_SINGLE_MEASUREMENT == num_of_measurements_type)
    {
      if (CALIBRATION_STATE_FINISHED == calibration_data.current_calibration_status)
      {
        // Convert float value to string with proper decimal places
        char val[SERIAL_CONSOLE_DTOSTRF_BUFFER_SIZE]; // Temporary buffer for float conversion
        dtostrf(calibration_data.value, SERIAL_CONSOLE_MIN_FLOAT_STRING_LEN, num_of_decimals, val);

        snprintf(display_string, sizeof(display_string), "Calibration for %s: %s%s", calibration_type, val, calibration_unit);
      }
      else
      {
        error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS;
        proceed_with_display = SERIAL_CONSOLE_DONT_PROCEED_WITH_DISPLAY;
      }
    }
    else
    {
      error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_NUM_OF_MEASUREMENTS_TYPE_CONFIGURED;
      proceed_with_display = SERIAL_CONSOLE_DONT_PROCEED_WITH_DISPLAY;
    }
    // Display the message only if there is no error
    if(SERIAL_CONSOLE_PROCEED_WITH_DISPLAY == proceed_with_display)
    {
      Serial.println(display_string);
    }
  }
  else
  {
    // Calibration metadata retrieval failed
    error_code = ERROR_CODE_CALIBRATION_CALIB_NOT_CONFIGURED;
  }

  return error_code;
}