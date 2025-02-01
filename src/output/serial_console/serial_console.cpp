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

    String display_string = "";
    String val = "";

    // Handle value-based measurements
    if(SENSORS_MEASUREMENT_TYPE_VALUE == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_VALUE == measurement_type)
    {
      float value = sensor_data.value;
      val = String(value, num_of_decimals);
    }
    // Handle indication-based measurements
    else if(SENSORS_MEASUREMENT_TYPE_INDICATION == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_INDICATION == measurement_type)
    {
      if(true == sensor_data.indication)
      {
        val = "yes";
      }
      else
      {
        val = "no";
      }
    }
    else
    {
      // Set error code for invalid measurement type
      error_code = ERROR_CODE_SERIAL_CONSOLE_INVALID_MEASUREMENT_TYPE;
    }
    // Format and display the sensor data if everything is okay
    if(ERROR_CODE_NO_ERROR == error_code)
    {
      display_string = String(sensor_type) + ": " + val + String(measurement_unit);
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

  // Format the time string with zero-padding for single-digit values
  String time_string = (hour < SERIAL_CONSOLE_TWO_CIPHER_NUMBER ? "0" : "") + String(hour) + ":" +
                        (mins < SERIAL_CONSOLE_TWO_CIPHER_NUMBER ? "0" : "") + String(mins) + " " +
                        (day < SERIAL_CONSOLE_TWO_CIPHER_NUMBER ? "0" : "") + String(day) + "/" +
                        (month < SERIAL_CONSOLE_TWO_CIPHER_NUMBER ? "0" : "") + String(month) + "/" +
                        String(year);

  // Display the formatted time
  String display_string = "Current time: " + time_string;
  Serial.println(display_string);

  return ERROR_CODE_NO_ERROR;
}

error_manager_error_code_te serial_console_displayI2cScan(i2cScan_reading_ts i2c_scan_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  String display_string = "";

  // Handle scan for all devices mode
  if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES == i2c_scan_data.device_address)
  {
    // Format the detected I2C address
    display_string = "I2C scan - I2C device found at address: 0x" + 
                     String(i2c_scan_data.current_i2c_addr < SERIAL_CONSOLE_TWO_CIPHER_NUMBER ? "0" : "") + 
                     String(i2c_scan_data.current_i2c_addr, HEX);
  }
  else
  {
    // Format the specific device address and status
    display_string = "I2C device on address 0x" + 
                            String(i2c_scan_data.device_address < SERIAL_CONSOLE_TWO_CIPHER_NUMBER ? "0" : "") + 
                            String(i2c_scan_data.device_address, HEX) + 
                            " status: ";
    // Interpret and append the device status
    switch(i2c_scan_data.single_device_status)
    {
      case I2CSCAN_TRANSMISSION_RESULT_SUCCESS:
        display_string += "Successful transmission";
        break;
      
      case I2CSCAN_TRANSMISSION_RESULT_TOOLONG:
        display_string += "Data too long to fit in transmit buffer";
        break;

      case I2CSCAN_TRANSMISSION_RESULT_NACKADR:
        display_string += "Received NACK on transmit of the address";
        break;
      
      case I2CSCAN_TRANSMISSION_RESULT_NACKDAT:
        display_string += "Received NACK on transmit of the data";
        break;

      case I2CSCAN_TRANSMISSION_RESULT_UNKNOWN:
        display_string += "Unknown error occurred during communication";
        break;
      // Set error code for unknown I2C device status
      default:
        error_code = ERROR_CODE_SERIAL_CONSOLE_UNKNOWN_I2C_DEVICE_STATUS;
        break;
    }
  }
  /* IMPORTANT: Check of invalid I2C address is done on I2C scanner side and it should not arrive on the Serial Console */
  // Display the formatted string if no error occurred
  if(ERROR_CODE_NO_ERROR == error_code)
  {
    Serial.println(display_string);
  }

  return error_code;
}

error_manager_error_code_te serial_console_displayCalibrationResults(calibration_reading_ts calibration_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  calibration_interface_metadata_ts calib_metadata = calibration_interface_getCalibrationMetadata(calibration_data.calibration_id);

  if(CALIBRATION_INTERFACE_STATUS_SUCCESS == calib_metadata.success_status)
  {
    const char* calibration_type = calib_metadata.metadata.calibration_type;
    const char* calibration_unit = calib_metadata.metadata.calibration_unit;
    uint8_t num_of_measurements_type = calib_metadata.metadata.num_of_measurements_type;
    uint8_t num_of_decimals = calib_metadata.metadata.num_of_decimals;

    String display_string = "";
    String val = "";

    if(CALIBRATION_MULTIPLE_MEASUREMENTS == num_of_measurements_type)
    {
      switch(calibration_data.current_calibration_status)
      {
        case CALIBRATION_STATE_IDLE:
          // Should not enter here because fetching of calibration data at least sets it to in progress
          display_string = String(calibration_type) + ": No calibration active";
          break;

        case CALIBRATION_STATE_IN_PROGRESS:
          display_string = String(calibration_type) + ": Calibration in progress...";
          break;

        case CALIBRATION_STATE_FINISHED:
          float value = calibration_data.value;
          val = String(value, num_of_decimals);
          display_string = "Calibration for " + String(calibration_type) + ": " + val + String(calibration_unit);
          break;

        default:
          error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS;
      }
    }
    else if(CALIBRATION_SINGLE_MEASUREMENT == num_of_measurements_type)
    {
      if(CALIBRATION_STATE_FINISHED == calibration_data.current_calibration_status)
      {
        float value = calibration_data.value;
        val = String(value, num_of_decimals);
        display_string = "Calibration for " + String(calibration_type) + ": " + val + String(calibration_unit);
      }
      else
      {
        error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS;
      }
    }
    else
    {
      error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_NUM_OF_MEASUREMENTS_TYPE_CONFIGURED;
    }

    if(ERROR_CODE_NO_ERROR == error_code)
    {
      Serial.println(display_string);
    }
  }
  else
  {
    error_code = ERROR_CODE_CALIBRATION_CALIB_NOT_CONFIGURED;
  }

  return error_code;
}