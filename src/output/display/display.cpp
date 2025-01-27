#include "display.h"


const display_sensors_config_ts display_sensors_config[] PROGMEM =
{
#ifdef DHT11_TEMPERATURE
  {"Temp",        "C",      DHT11_TEMPERATURE,       DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef DHT11_HUMIDITY
  {"Humidity",    "%",      DHT11_HUMIDITY,          DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef BMP280_PRESSURE
  {"Press",       "hPa",    BMP280_PRESSURE,         DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef BMP280_TEMPERATURE
  {"BPM Temp",    "C",      BMP280_TEMPERATURE,      DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef BMP280_ALTITUDE
  {"Altitude",    "m",      BMP280_ALTITUDE,         DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef BH1750_LUMINANCE
  {"Luminance",   "lx",     BH1750_LUMINANCE,        DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef MQ135_PPM
  {"Gases PPM",   "",       MQ135_PPM,               DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef MQ7_COPPM
  {"CO PPM",      "",       MQ7_COPPM,               DISPLAY_READING_VALUE,      DISPLAY_0_DECIMALS},
#endif
#ifdef GYML8511_UV
  {"UV int",      "",       GYML8511_UV,             DISPLAY_READING_VALUE,      DISPLAY_1_DECIMAL},
#endif
#ifdef ARDUINORAIN_RAINING
  {"Raining",     "",       ARDUINORAIN_RAINING,     DISPLAY_INDICATION,         DISPLAY_NO_DECIMALS},
#endif
};

LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

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

    default:
      break;
  }

  return error_code;
}

error_manager_error_code_te display_displaySensorMeasurement(sensor_reading_ts sensor_data)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_SENSORS_ROW);

  bool is_sensor_configured = false;
  uint8_t sensor_index = 0;
  if(DIPSLAY_NO_SENSORS_CONFIGURED != sizeof(display_sensors_config))
  {
    size_t num_of_display_functions = sizeof(display_sensors_config) / sizeof(display_sensors_config[0]);
    for (uint8_t index = 0; index < num_of_display_functions; index++)
    {
      if(pgm_read_byte(&display_sensors_config[index].id) == sensor_data.sensor_id)
      {
        is_sensor_configured = true;
        sensor_index = index;
        break;
      }
    }

    if(true == is_sensor_configured)
    {
      display_sensors_config_ts current_sensor;
      memcpy_P(&current_sensor, &display_sensors_config[sensor_index], sizeof(display_sensors_config_ts));
      const char* sensor_type = (const char*)pgm_read_word(&(current_sensor.sensor_type));
      const char* measurement_unit = (const char*)pgm_read_word(&(current_sensor.measurement_unit));
      String display_string = "";
      String val = "";

      if(DISPLAY_READING_VALUE == sensor_data.measurement_type_switch)
      {
        float value = sensor_data.value;
        val = String(value, current_sensor.accuracy);
      }
      else if(DISPLAY_INDICATION == sensor_data.measurement_type_switch)
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

      display_string = String(sensor_type) + ": " + val + String(measurement_unit);
      while (display_string.length() < DISPLAY_LCD_WIDTH) 
      {
        display_string += ' '; //Add spaces to fill to the end
      }
      lcd.print(display_string);
    }
    else
    {
      error_code = ERROR_CODE_DISPLAY_SENSOR_NOT_CONFIGURED;
    }
  }
  else
  {
    error_code = ERROR_CODE_DISPLAY_NO_SENSORS_CONFIGURED;
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
    String display_string = "I2C 0x" + String(i2c_scan_data.device_address < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(i2c_scan_data.device_address, HEX) + " status:";
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

  return error_code;
}