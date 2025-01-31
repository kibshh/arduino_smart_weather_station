#include "display.h"


static LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

error_manager_error_code_te display_init()
{
  lcd.begin(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT); // Initialize a 16x2 LCD
  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_START_ROW);
  lcd.backlight();
  lcd.noCursor();
  return ERROR_CODE_NO_ERROR;
}

error_manager_error_code_te display_displayData(data_router_data_ts data)
{
  error_manager_error_code_te error_code = ERROR_CODE_INVALID_INPUT_TYPE;

  switch(data.input_type)
  {
    case INPUT_SENSORS:
      error_code = display_displaySensorMeasurement(data.input_return.sensor_reading);
      break;

    case INPUT_RTC:
      error_code = display_displayTime(data.input_return.rtc_reading);
      break;

    case INPUT_I2C_SCAN:
      error_code = display_displayI2cScan(data.input_return.i2cScan_reading);
      break;

    case INPUT_CALIBRATION:
      error_code = display_displayCalibrationResults(data.input_return.calib_reading);
      break;

    default:
      break;
  }

  return error_code;
}

error_manager_error_code_te display_displaySensorMeasurement(sensor_reading_ts sensor_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_SENSORS_ROW);

  sensors_interface_metadata_ts sensor_metadata = sensors_interface_getSensorMetadata(sensor_data.sensor_id);

  if(SENSORS_INTERFACE_STATUS_SUCCESS == sensor_metadata.success_status)
  {
    const char* sensor_type = sensor_metadata.metadata.sensor_type;
    const char* measurement_unit = sensor_metadata.metadata.measurement_unit;
    uint8_t measurement_type = sensor_metadata.metadata.measurement_type;
    uint8_t num_of_decimals = sensor_metadata.metadata.num_of_decimals;
    uint8_t display_num_of_letters = sensor_metadata.metadata.display_num_of_letters;

    String display_string = "";
    String val = "";

    if(SENSORS_MEASUREMENT_TYPE_VALUE == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_VALUE == measurement_type)
    {
      float value = sensor_data.value;
      val = String(value, num_of_decimals);
    }
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
      error_code = ERROR_CODE_DISPLAY_INVALID_MEASUREMENT_TYPE;
    }
    if(ERROR_CODE_NO_ERROR == error_code)
    {
      int display_length = min(display_num_of_letters, String(sensor_type).length());
      display_string = String(sensor_type).substring(DISPLAY_FIRST_LETTER_IN_STR, display_length) + ": " + val + String(measurement_unit);
      while (display_string.length() < DISPLAY_LCD_WIDTH) 
      {
        display_string += ' '; //Add spaces to fill to the end
      }
      lcd.print(display_string);
    }
  }
  else
  {
    error_code = ERROR_CODE_DISPLAY_SENSOR_NOT_CONFIGURED;
  }

  return error_code;
}

error_manager_error_code_te display_displayTime(rtc_reading_ts time_data)
{
  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_TIME_ROW);

  uint16_t year = time_data.year;
  uint8_t month = time_data.month;
  uint8_t day = time_data.day;
  uint8_t hour = time_data.hour;
  uint8_t mins = time_data.mins;
  uint8_t secs = time_data.secs;

  String time_string = (hour < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(hour) + ":" +
                        (mins < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(mins) + " " +
                        (day < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(day) + "/" +
                        (month < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(month) + "/" +
                        String(year);
  lcd.print(time_string);

  return ERROR_CODE_NO_ERROR;
}

error_manager_error_code_te display_displayI2cScan(i2cScan_reading_ts i2c_scan_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES == i2c_scan_data.device_address)
  {
    // Print user friendly string
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_STRING_ROW);
    String display_string = "Scanning I2C....";
    lcd.print(display_string);

    // Print I2C address
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_ADDR_ROW);
    display_string = "";
    display_string = "I2C Addr: 0x" + 
                     String(i2c_scan_data.current_i2c_addr < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + 
                     String(i2c_scan_data.current_i2c_addr, HEX);
    lcd.print(display_string);
  }
  else
  {
    // Print headline string
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_STRING_ROW);
    String display_string = "I2C 0x" + 
                            String(i2c_scan_data.device_address < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + 
                            String(i2c_scan_data.device_address, HEX) + 
                            " status:";
    lcd.print(display_string);

    // Print device status
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_ADDR_ROW);
    display_string = "";
    switch(i2c_scan_data.single_device_status)
    {
      case I2CSCAN_TRANSMISSION_RESULT_SUCCESS:
        display_string = "Successful";
        break;
      
      case I2CSCAN_TRANSMISSION_RESULT_TOOLONG:
        display_string = "Result too long";
        break;

      case I2CSCAN_TRANSMISSION_RESULT_NACKADR:
        display_string = "Result NACK";
        break;
      
      case I2CSCAN_TRANSMISSION_RESULT_NACKDAT:
        display_string = "Result NACKDAT";
        break;

      case I2CSCAN_TRANSMISSION_RESULT_UNKNOWN:
        display_string = "Unknown error";
        break;
      
      default:
        error_code = ERROR_CODE_DISPLAY_UNKNOWN_I2C_DEVICE_STATUS;
        break;
    }
    while (display_string.length() < DISPLAY_LCD_WIDTH) 
    {
      display_string += ' '; //Add spaces to fill to the end
    }
    lcd.print(display_string);
  }
  /* IMPORTANT: Check of invalid I2C address is done on I2C scanner side and it should not arrive on the Display */
  return error_code;
}

error_manager_error_code_te display_displayCalibrationResults(calibration_reading_ts calibration_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;
  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_CALIB_ROW);

  calibration_interface_metadata_ts calib_metadata = calibration_interface_getCalibrationMetadata(calibration_data.calibration_id);

  if(CALIBRATION_INTERFACE_STATUS_SUCCESS == calib_metadata.success_status)
  {
    const char* calibration_type = calib_metadata.metadata.calibration_type;
    const char* calibration_unit = calib_metadata.metadata.calibration_unit;
    uint8_t num_of_measurements_type = calib_metadata.metadata.num_of_measurements_type;
    uint8_t num_of_decimals = calib_metadata.metadata.num_of_decimals;
    uint8_t display_num_of_letters = calib_metadata.metadata.display_num_of_letters;

    String display_string = "";
    String val = "";
    int display_length = min(display_num_of_letters, String(calibration_type).length());
    String calib_name = String(calibration_type).substring(DISPLAY_FIRST_LETTER_IN_STR, display_length);

    if(CALIBRATION_MULTIPLE_MEASUREMENTS == num_of_measurements_type)
    {
      switch(calibration_data.current_calibration_status)
      {
        case CALIBRATION_STATE_IDLE:
          // Should not enter here because fetching of calibration data at least sets it to in progress
          display_string = calib_name + " No calib";
          break;

        case CALIBRATION_STATE_IN_PROGRESS:
          display_string = calib_name + " Progress";
          break;

        case CALIBRATION_STATE_FINISHED:
          float value = calibration_data.value;
          val = String(value, num_of_decimals);
          display_string =calib_name + ": " + val + String(calibration_unit);
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
        display_string =calib_name + ": " + val + String(calibration_unit);
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
      while (display_string.length() < DISPLAY_LCD_WIDTH) 
      {
        display_string += ' '; //Add spaces to fill to the end
      }
      lcd.print(display_string);
    }
  }
  else
  {
    error_code = ERROR_CODE_CALIBRATION_CALIB_NOT_CONFIGURED;
  }

  return error_code;
}