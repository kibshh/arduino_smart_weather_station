#include "display.h"

/* STATIC GLOBAL VARIABLES */
static LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);
/* *************************************** */

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Displays the sensor measurement data on the LCD based on the provided sensor reading.
 * It retrieves the metadata for the sensor, formats the sensor value, and updates the display.
 * If the measurement type is invalid or metadata is not found, the function returns an error code.
 *
 * @param control_data_ts Pointer to data containing sensor reading with value, and measurement type switch and sensor ID.
 *
 * @return control_error_code_te Returns an error code based on the display process:
 *         - ERROR_CODE_NO_ERROR if the sensor data is successfully displayed.
 *         - ERROR_CODE_INVALID_SENSOR_MEASUREMENT_TYPE if the measurement type is invalid.
 *         - ERROR_CODE_SENSOR_NOT_CONFIGURED if the sensor metadata is not found.
 **/
static control_error_code_te display_displaySensorMeasurement(const control_data_ts *data);

/**
 * @brief Displays the current time on the LCD, formatted to fit a 16-character wide display.
 * This function formats the time and date values from the RTC reading and displays it.
 *
 * @param control_data_ts Pointer to data containing RTC reading with the current time (year, month, day, hour, minutes, seconds).
 *
 * @return control_error_code_te Returns an error code:
 *         - ERROR_CODE_NO_ERROR indicating successful execution.
 **/
static control_error_code_te display_displayTime(const control_data_ts *data);

/**
 * @brief Displays the results of an I2C bus scan on the LCD.
 * If scanning all devices, it will show a scanning message, otherwise it displays the status
 * of the specific device address being scanned.
 *
 * @param control_data_ts Pointer to data containing I2C scanning data that contains the device address and scan status.
 *
 * @return control_error_code_te Returns an error code:
 *         - ERROR_CODE_NO_ERROR for a successful display update.
 *         - ERROR_CODE_UNKNOWN_I2C_DEVICE_STATUS for unknown device statuses.
 **/
static control_error_code_te display_displayI2cScan(const control_data_ts *data);

/**
 * @brief Displays the error message on the LCD.
 * This function formats the error data, including its error code, and displays it.
 *
 * @param control_data_ts Pointer to data containing the error information, including the component and error code.
 *
 * @return control_error_code_te Returns an error code:
 *         - ERROR_CODE_NO_ERROR indicating successful execution.
 **/
static control_error_code_te display_displayError(const control_data_ts *data);

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
static String formatDisplaySensorData(sensors_metadata_catalog_ts sensor_metadata, char *val);

/**
 * @brief Clears a specific row on the LCD by printing blank spaces.
 *
 * @param row The row index to clear.
 */
static void displayEmptyLine(uint8_t row);

/**
 * @brief Pads a string with ' ' characters, to be displayed on the LCD.
 * NOTE: Caller must initialize the string with large enough buffer size
 * (DISPLAY_MAX_STRING_LEN).
 *
 * @param string The string to pad.
 */
static void padString(char *string);
/* *************************************** */

/* EXPORTED FUNCTIONS */
control_error_code_te display_init()
{
  lcd.begin(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT); // Initialize a 16x2 LCD
  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_START_ROW);
  lcd.backlight();
  lcd.noCursor();
  return ERROR_CODE_NO_ERROR;
}

control_error_code_te display_displayData(const control_data_ts *data)
{
  control_error_code_te error_code = ERROR_CODE_INVALID_INPUT_TYPE; // Error code if input type is invalid

  switch (data->input.io_component)
  {
  case INPUT_SENSORS:
    error_code = display_displaySensorMeasurement(data);
    break;

  case INPUT_RTC:
    error_code = display_displayTime(data);
    break;

  case INPUT_I2C_SCAN:
    error_code = display_displayI2cScan(data);
    break;

  case INPUT_ERROR:
    error_code = display_displayError(data);
    break;

  default:
    break;
  }

  return error_code;
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static control_error_code_te display_displaySensorMeasurement(const control_data_ts *data)
{
  sensor_reading_ts sensor_data = data->input_return.sensor_reading;
  uint8_t sensor_id = data->input.device_id;

  control_error_code_te error_code = ERROR_CODE_NO_ERROR;        // Default Error code
  bool proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY; // Flag to determine if the display should be updated
  char val[DISPLAY_MAX_STRING_LEN];                              // Holds the formatted sensor value or indication

  // Retrieve metadata for the given sensor ID
  sensors_interface_metadata_ts sensor_metadata = sensors_interface_getSensorMetadata(sensor_id);

  if (SENSORS_INTERFACE_STATUS_SUCCESS == sensor_metadata.success_status)
  {
    // Extract what's needed for now
    uint8_t measurement_type = sensor_metadata.metadata.measurement_type; // Expected measurement type
    uint8_t num_of_decimals = sensor_metadata.metadata.num_of_decimals;   // Number of decimal places for display

    if (SENSORS_MEASUREMENT_TYPE_VALUE == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_VALUE == measurement_type)
    {
      // Case: Sensor provides a numerical value
      char val_buffer[DISPLAY_DTOSTRF_BUFFER_SIZE];                                          // Buffer for formatted float value
      dtostrf(sensor_data.value, DISPLAY_MIN_FLOAT_STRING_LEN, num_of_decimals, val_buffer); // Convert float to string
      strncpy(val, val_buffer, sizeof(val) - DISPLAY_NULL_TERMINATOR_SIZE);                  // Copy into val
      val[sizeof(val) - DISPLAY_NULL_TERMINATOR_SIZE] = '\0';                                // Null terminate
      proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;
    }
    else if (SENSORS_MEASUREMENT_TYPE_INDICATION == sensor_data.measurement_type_switch && SENSORS_MEASUREMENT_TYPE_INDICATION == measurement_type)
    {
      // Case: Sensor provides an indication (boolean)
      snprintf(val, sizeof(val), "%s", sensor_data.indication ? "yes" : "no");
      proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;
    }
    else
    {
      error_code = ERROR_CODE_INVALID_SENSOR_MEASUREMENT_TYPE; // Return an error if types mismatch
    }
  }
  else
  {
    error_code = ERROR_CODE_SENSOR_NOT_CONFIGURED; // Error code in case sensor metadata is not found
  }

  // Display the formatted value if valid, otherwise clear the display row
  if (DISPLAY_PROCEED_WITH_DISPLAY == proceed_with_display)
  {
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_SENSORS_ROW);
    String display_string = formatDisplaySensorData(sensor_metadata.metadata, val); // Format display string
    lcd.print(display_string);                                                      // Print the formatted sensor data to the LCD
  }
  else
  {
    displayEmptyLine(DISPLAY_SENSORS_ROW); // Clear the display row if no valid data
  }
  return error_code;
}

static control_error_code_te display_displayTime(const control_data_ts *data)
{
  rtc_reading_ts time_data = data->input_return.rtc_reading;

  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_TIME_ROW);

  uint16_t year = time_data.year;
  uint8_t month = time_data.month;
  uint8_t day = time_data.day;
  uint8_t hour = time_data.hour;
  uint8_t mins = time_data.mins;
  uint8_t secs = time_data.secs;

  // Build the formatted time string to fit the 16-character display
  char time_string[DISPLAY_MAX_STRING_LEN];                                                             // One extra for null terminator
  snprintf(time_string, sizeof(time_string), "%02d:%02d %02d/%02d/%04d", hour, mins, day, month, year); // To avoid dynamic allocation

  lcd.print(time_string);

  return ERROR_CODE_NO_ERROR; // Return success error code
}

static control_error_code_te display_displayI2cScan(const control_data_ts *data)
{
  i2c_scan_reading_ts i2c_scan_data = data->input_return.i2c_scan_reading;

  control_error_code_te error_code = ERROR_CODE_NO_ERROR;

  // Create buffer for display strings
  char display_string[DISPLAY_MAX_STRING_LEN]; // +1 for null terminator

  if (I2C_SCAN_MODE_SCAN_FOR_ALL_DEVICES == i2c_scan_data.device_address)
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
    char status_string[DISPLAY_MAX_STRING_LEN]; // +1 for null terminator

    switch (i2c_scan_data.single_device_status)
    {
    case I2C_SCAN_TRANSMISSION_RESULT_SUCCESS:
      snprintf(status_string, sizeof(status_string), "Successful");
      break;

    case I2C_SCAN_TRANSMISSION_RESULT_TOOLONG:
      snprintf(status_string, sizeof(status_string), "Result too long");
      break;

    case I2C_SCAN_TRANSMISSION_RESULT_NACKADR:
      snprintf(status_string, sizeof(status_string), "Result NACK");
      break;

    case I2C_SCAN_TRANSMISSION_RESULT_NACKDAT:
      snprintf(status_string, sizeof(status_string), "Result NACKDAT");
      break;

    case I2C_SCAN_TRANSMISSION_RESULT_UNKNOWN:
      snprintf(status_string, sizeof(status_string), "Unknown error");
      break;

    default:
      error_code = ERROR_CODE_UNKNOWN_I2C_DEVICE_STATUS;
      proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY;
      break;
    }

    if (DISPLAY_PROCEED_WITH_DISPLAY == proceed_with_display)
    {
      // Print headline with device address
      lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_STRING_ROW);
      snprintf(display_string, sizeof(display_string), "I2C 0x%02X status:", i2c_scan_data.device_address);
      lcd.print(display_string);

      // Print device status based on scan result
      lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_ADDR_ROW);
      // Print the status on LCD with padding
      padString(status_string);
      lcd.print(status_string);
    }
  }
  /* IMPORTANT: Check of invalid I2C address is done on I2C scanner side and it should not arrive on the Display */
  return error_code;
}

static control_error_code_te display_displayError(const control_data_ts *data)
{
  control_error_ts error_data = data->input_return.error_msg;

  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_ERROR_CODE_ROW);

  char error_string[DISPLAY_MAX_STRING_LEN];

  snprintf(error_string, sizeof(error_string), "Error Code: %d", error_data.error_code);
  padString(error_string);
  lcd.print(error_string); // In case of print error we do not need an error message,
                           // since this is a last resort operation. If error message write
                           // to LCD fails, we just discard.
  control_error_code_te error_code = ERROR_CODE_NO_ERROR;
  bool proceed_with_display = DISPLAY_PROCEED_WITH_DISPLAY;

  switch (error_data.component.io_component)
  {
  case INPUT_SENSORS:
    snprintf(error_string, sizeof(error_string), "Sensor ID:%u", error_data.component.device_id);
    break;

  case INPUT_RTC:
    snprintf(error_string, sizeof(error_string), "RTC ID:%u", error_data.component.device_id);
    break;

  case INPUT_I2C_SCAN:
    snprintf(error_string, sizeof(error_string), "I2C Scan");
    break;

  case OUTPUT_SERIAL_CONSOLE:
    snprintf(error_string, sizeof(error_string), "Serial Console");
    break;

  case OUTPUT_DISPLAY:
    snprintf(error_string, sizeof(error_string), "LCD Display");
    break;

  default:
    proceed_with_display = DISPLAY_DONT_PROCEED_WITH_DISPLAY;
    break;
  }

  if (DISPLAY_PROCEED_WITH_DISPLAY == proceed_with_display)
  {
    lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_ERROR_COMPONENT_INFO_ROW);
    padString(error_string);
    lcd.print(error_string);
  }

  return error_code; // Return success error code
}

static String formatDisplaySensorData(sensors_metadata_catalog_ts sensor_metadata, char *val)
{
  char display_string[DISPLAY_MAX_STRING_LEN]; // Buffer to hold formatted string
  int display_sensor_type_length = min(sensor_metadata.display_num_of_letters, strlen(sensor_metadata.sensor_type));

  snprintf(display_string, sizeof(display_string), "%s: %s%s", sensor_metadata.sensor_type, val, sensor_metadata.measurement_unit);

  // Ensure the string fits the display by padding with spaces
  padString(display_string);

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

static void padString(char *string)
{
  size_t len = strlen(string);
  for (size_t i = len; i < DISPLAY_LCD_WIDTH; ++i)
  {
    string[i] = ' ';
  }
  string[DISPLAY_MAX_STRING_LEN] = '\0'; // Null-terminate the string
}
/* *************************************** */
