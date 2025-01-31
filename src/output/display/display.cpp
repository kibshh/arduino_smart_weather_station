#include "display.h"


static LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Formats sensor data for display on an LCD screen.
 * 
 * This function takes sensor metadata and a measurement value, 
 * formats it as a string, and ensures it fits within the display width.
 * 
 * @param sensor_metadata Metadata containing sensor type, unit, and display settings.
 * @param val The sensor measurement value as a string.
 * @return A formatted string ready for LCD display.
 */
static String formatDisplaySensorData(sensors_metadata_catalog_ts sensor_metadata, char* val);

/** 
 * Clears a specific row on the LCD by printing blank spaces.
 * @param row The row index to clear.
 */
static void displayEmptyLine(uint8_t row);
/* *************************************** */

/* EXPORTED FUNCTIONS */
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
  error_manager_error_code_te error_code = ERROR_CODE_INVALID_INPUT_TYPE; // Error code if input type is invalid

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
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR; // Default Error code
  bool proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY; // Flag to determine if the display should be updated
  char val[DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE]; // Holds the formatted sensor value or indication

  // Retrieve metadata for the given sensor ID
  sensors_interface_metadata_ts sensor_metadata = sensors_interface_getSensorMetadata(sensor_data.sensor_id);

  if(SENSORS_INTERFACE_STATUS_SUCCESS == sensor_metadata.success_status)
  {
    // Extract what's needed for now
    uint8_t measurement_type = sensor_metadata.metadata.measurement_type; // Expected measurement type
    uint8_t num_of_decimals = sensor_metadata.metadata.num_of_decimals; // Number of decimal places for display

    if(SENSORS_MEASUREMENT_TYPE_VALUE == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_VALUE == measurement_type)
    {
      // Case: Sensor provides a numerical value
      char val_buffer[DISPLAY_DTOSTRF_BUFFER_SIZE]; // Buffer for formatted float value
      dtostrf(sensor_data.value, DISPLAY_MIN_STRING_LEN, num_of_decimals, val_buffer); // Convert float to string
      strncpy(val, val_buffer, sizeof(val) - DISPLAY_NULL_TERMINATOR_SIZE); // Copy into val
      val[sizeof(val) - DISPLAY_NULL_TERMINATOR_SIZE] = '\0'; // Null terminate
      proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;
    }
    else if(SENSORS_MEASUREMENT_TYPE_INDICATION == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_INDICATION == measurement_type)
    {
      // Case: Sensor provides an indication (boolean)
      snprintf(val, sizeof(val), "%s", sensor_data.indication ? "yes" : "no");
      proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;
    }
    else
    {
      error_code = ERROR_CODE_DISPLAY_INVALID_MEASUREMENT_TYPE; // Return an error if types mismatch
    }
  }
  else
  {
    error_code = ERROR_CODE_DISPLAY_SENSOR_NOT_CONFIGURED; // Error code in case sensor metadata is not found
  }

  // Display the formatted value if valid, otherwise clear the display row
  if(DISPLAY_PROCEED_WITH_DISPLAY == proceed_with_display)
  {
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_SENSORS_ROW);
    String display_string = formatDisplaySensorData(sensor_metadata.metadata, val); // Format display string
    lcd.print(display_string); // Print the formatted sensor data to the LCD
  }
  else
  {
    displayEmptyLine(DISPLAY_SENSORS_ROW); // Clear the display row if no valid data
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

  // Build the formatted time string to fit the 16-character display
  char time_string[DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE]; // One extra for null terminator
  snprintf(time_string, sizeof(time_string), "%02d:%02d %02d/%02d/%04d", hour, mins, day, month, year); // To avoid dynamic allocation

  lcd.print(time_string);

  return ERROR_CODE_NO_ERROR; // Return success error code
}

error_manager_error_code_te display_displayI2cScan(i2cScan_reading_ts i2c_scan_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  // Create buffer for display strings
  char display_string[DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE];  // +1 for null terminator

  if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES == i2c_scan_data.device_address)
  {
    // Print user friendly scanning message
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_STRING_ROW);
    snprintf(display_string, sizeof(display_string), "Scanning I2C....");
    lcd.print(display_string);

    // Print I2C address
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_ADDR_ROW);
    snprintf(display_string, sizeof(display_string), "I2C Addr: 0x%02X", i2c_scan_data.current_i2c_addr);
    lcd.print(display_string);
  }
  else
  {
    bool proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;

    // Create buffer for status
    char status_string[DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE];  // +1 for null terminator

    switch(i2c_scan_data.single_device_status)
    {
      case I2CSCAN_TRANSMISSION_RESULT_SUCCESS:
        snprintf(status_string, sizeof(status_string), "Successful");
        break;

      case I2CSCAN_TRANSMISSION_RESULT_TOOLONG:
        snprintf(status_string, sizeof(status_string), "Result too long");
        break;

      case I2CSCAN_TRANSMISSION_RESULT_NACKADR:
        snprintf(status_string, sizeof(status_string), "Result NACK");
        break;

      case I2CSCAN_TRANSMISSION_RESULT_NACKDAT:
        snprintf(status_string, sizeof(status_string), "Result NACKDAT");
        break;

      case I2CSCAN_TRANSMISSION_RESULT_UNKNOWN:
        snprintf(status_string, sizeof(status_string), "Unknown error");
        break;

      default:
        error_code = ERROR_CODE_DISPLAY_UNKNOWN_I2C_DEVICE_STATUS;
        proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY;
        break;
    }

    if(DISPLAY_PROCEED_WITH_DISPLAY == proceed_with_display)
    {
      // Print headline with device address
      lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_STRING_ROW);
      snprintf(display_string, sizeof(display_string), "I2C 0x%02X status:", i2c_scan_data.device_address);
      lcd.print(display_string);

      // Print device status based on scan result
      lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_ADDR_ROW);
      // Print the status on LCD with padding
      while(strlen(status_string) < DISPLAY_LCD_WIDTH) 
      {
        strcat(status_string, " "); // Pad the string with spaces
      }
      lcd.print(status_string);
    }
  }
  /* IMPORTANT: Check of invalid I2C address is done on I2C scanner side and it should not arrive on the Display */
  return error_code;
}

error_manager_error_code_te display_displayCalibrationResults(calibration_reading_ts calibration_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  // Retrieve calibration metadata for the given calibration ID
  calibration_interface_metadata_ts calib_metadata = calibration_interface_getCalibrationMetadata(calibration_data.calibration_id);

  // Check if the calibration metadata retrieval was successful
  if(CALIBRATION_INTERFACE_STATUS_SUCCESS == calib_metadata.success_status)
  {
    const char* calibration_type = calib_metadata.metadata.calibration_type;
    const char* calibration_unit = calib_metadata.metadata.calibration_unit;
    uint8_t num_of_measurements_type = calib_metadata.metadata.num_of_measurements_type;
    uint8_t num_of_decimals = calib_metadata.metadata.num_of_decimals;
    uint8_t display_num_of_letters = calib_metadata.metadata.display_num_of_letters;

    // Flag to control whether display should proceeded
    bool proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;

    // Prepare display strings with appropriate buffer size (+1 for null terminator)
    char display_string[DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE];  // +1 for null terminator
    char val[DISPLAY_LCD_WIDTH];  // Buffer for value representation

    // Determine the display string length based on the maximum allowed letters
    int display_length = min(display_num_of_letters, strlen(calibration_type));
    strncpy(display_string, calibration_type, display_length); // Copy the calibration type to display_string
    display_string[display_length] = '\0';  // Null-terminate the string

    // Handle calibration based on the number of measurements
    if(CALIBRATION_MULTIPLE_MEASUREMENTS == num_of_measurements_type)
    {
      // Process different calibration states when multiple measurements are required
      switch(calibration_data.current_calibration_status)
      {
        case CALIBRATION_STATE_IDLE:
          snprintf(display_string, sizeof(display_string), "%s No calib", calibration_type);
          break;

        case CALIBRATION_STATE_IN_PROGRESS:
          snprintf(display_string, sizeof(display_string), "%s Progress", calibration_type);
          break;

        case CALIBRATION_STATE_FINISHED:
          snprintf(val, sizeof(val), "%.*f", num_of_decimals, calibration_data.value);
          snprintf(display_string, sizeof(display_string), "%s: %s%s", calibration_type, val, calibration_unit);
          break;

        default:
          // Invalid calibration state
          error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS;
          proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY;
          break;
      }
    }
    else if(CALIBRATION_SINGLE_MEASUREMENT == num_of_measurements_type)
    {
      // Process calibration for a single measurement type
      if(CALIBRATION_STATE_FINISHED == calibration_data.current_calibration_status)
      {
        // Calibration finished; display the result value
        snprintf(val, sizeof(val), "%.*f", num_of_decimals, calibration_data.value);
        snprintf(display_string, sizeof(display_string), "%s: %s%s", calibration_type, val, calibration_unit);
      }
      else
      {
        // Invalid calibration state for single measurement
        error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS;
        proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY;
      }
    }
    else
    {
      // Invalid calibration measurement type configuration
      error_code = ERROR_CODE_DISPLAY_INVALID_CALIBRATION_NUM_OF_MEASUREMENTS_TYPE_CONFIGURED;
      proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY;
    }

    // Only proceed to display if no error occurred
    if(DISPLAY_PROCEED_WITH_DISPLAY == proceed_with_display)
    {
      lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_CALIB_ROW);
      // Calculate the length of the display string and pad with spaces if necessary
      int len = strlen(display_string);
      for(int i = len; i < DISPLAY_LCD_WIDTH; ++i)
      {
        display_string[i] = ' ';
      }
      display_string[DISPLAY_LCD_WIDTH] = '\0';  // Null-terminate the string
      // Print the final string to the display
      lcd.print(display_string);
    }
  }
  else
  {
    // Calibration metadata retrieval failed
    error_code = ERROR_CODE_CALIBRATION_CALIB_NOT_CONFIGURED;
  }

  return error_code;
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static String formatDisplaySensorData(sensors_metadata_catalog_ts sensor_metadata, char* val)
{
  char display_string[DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE]; // Buffer to hold formatted string
  int display_sensor_type_length = min(sensor_metadata.display_num_of_letters, strlen(sensor_metadata.sensor_type));
  
  snprintf(display_string, sizeof(display_string), "%s: %s%s", sensor_metadata.sensor_type, val, sensor_metadata.measurement_unit);

  // Ensure the string fits the display by padding with spaces
  int len = strlen(display_string);
  for (int i = len; i < DISPLAY_LCD_WIDTH; ++i)
  {
    display_string[i] = ' ';
  }
  display_string[DISPLAY_LCD_WIDTH] = '\0';  // Null-terminate the string
  
  return String(display_string); // Return the formatted string as a String object
}

static void displayEmptyLine(uint8_t row)
{
  lcd.setCursor(DISPLAY_START_COLUMN, row);
  for (uint8_t i = 0; i < DISPLAY_LCD_WIDTH; i++) 
  {
    lcd.print(" ");
  }
}
/* *************************************** */