#include "display.h"


const display_sensors_config_t display_sensors_config[] PROGMEM =
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

const int num_of_display_functions = sizeof(display_sensors_config) / sizeof(display_sensors_config[0]);

LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

void display_init()
{
  lcd.begin(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT); // Initialize a 16x2 LCD
  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_START_ROW);
  lcd.backlight();
  lcd.noCursor();
}

error_manager_error_code_te display_displayData(data_router_output_input_type input_type, uint8_t *payload, size_t payload_len)
{
  error_manager_error_code_te error_code = ERROR_CODE_INVALID_OUTPUT_PAYLOAD_LEN;

  switch(input_type)
  {
    case SENSOR_MEASUREMENT:
      if(DATA_ROUTER_INPUT_SENSORS_RETURN_PAYLOAD_MIN_LEN <= payload_len)
      {
        error_code = display_displaySensorMeasurement(payload, payload_len);
      }
      break;

    case TIME_MEASUREMENT:
      if(DATA_ROUTER_INPUT_RTC_RETURN_PAYLOAD_LEN_MIN <= payload_len)
      {
        error_code = display_displayTime(payload, payload_len);
      }
      break;

    case I2C_SCAN_OUTPUT:
      if(DATA_ROUTER_INPUT_I2C_SCAN_RETURN_PAYLOAD_LEN_MIN <= payload_len)
      {
        error_code = display_displayI2CScan(payload, payload_len);
      }
      break;

    default:
      error_code = ERROR_CODE_INVALID_OUTPUT_PARAMETERS;
      break;
  }

  return error_code;
}

error_manager_error_code_te display_displaySensorMeasurement(uint8_t *payload, size_t payload_len)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_SENSORS_ROW);

  uint8_t sensor_id = payload[DATA_ROUTER_INPUT_SENSORS_RETURN_ID_POS];
  bool is_sensor_configured = false;
  uint8_t sensor_index = 0;
  for (uint8_t index = 0; index < num_of_display_functions; index++)
  {
    if(display_sensors_config[index].id == sensor_id)
    {
      is_sensor_configured = true;
      sensor_index = index;
      break;
    }
  }

  if(true == is_sensor_configured)
  {
    uint8_t measurement_type = payload[DATA_ROUTER_INPUT_SENSORS_RETURN_MEASUREMENT_TYPE_POS];

    display_sensors_config_t current_sensor;
    memcpy_P(&current_sensor, &display_sensors_config[sensor_index], sizeof(display_sensors_config_t));
    const char* sensor_type = (const char*)pgm_read_word(&(current_sensor.sensor_type));
    const char* measurement_unit = (const char*)pgm_read_word(&(current_sensor.measurement_unit));
    String display_string = "";
    String val = "";

    if(DISPLAY_READING_VALUE == measurement_type)
    {
      float value;
      memcpy(&value, &(payload[DATA_ROUTER_INPUT_SENSORS_RETURN_VALUE_POS]), DATA_ROUTER_INPUT_SENSORS_RETURN_VALUE_LEN);
      val = String(value, current_sensor.accuracy);
    }
    else if(DISPLAY_INDICATION == measurement_type)
    {
      bool indication = (bool)payload[DATA_ROUTER_INPUT_SENSORS_RETURN_INDICATION_POS];
      if(true == indication)
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

  return error_code;
}

error_manager_error_code_te display_displayTime(uint8_t *payload, size_t payload_len)
{
  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_TIME_ROW);

  uint16_t year;
  memcpy(&year, &(payload[DATA_ROUTER_INPUT_RTC_RETURN_YEAR_POS]), DATA_ROUTER_INPUT_RTC_RETURN_YEAR_LEN);
  uint8_t month = payload[DATA_ROUTER_INPUT_RTC_RETURN_MONTH_POS];
  uint8_t day = payload[DATA_ROUTER_INPUT_RTC_RETURN_DAY_POS];
  uint8_t hour = payload[DATA_ROUTER_INPUT_RTC_RETURN_HOUR_POS];
  uint8_t mins = payload[DATA_ROUTER_INPUT_RTC_RETURN_MINS_POS];
  uint8_t secs = payload[DATA_ROUTER_INPUT_RTC_RETURN_SECS_POS];

  String time_string = (hour < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(hour) + ":" +
                        (mins < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(mins) + " " +
                        (day < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(day) + "/" +
                        (month < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(month) + "/" +
                        String(year);
  lcd.print(time_string);

  return ERROR_CODE_NO_ERROR;
}

error_manager_error_code_te display_displayI2CScan(uint8_t *payload, size_t payload_len)
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  lcd.setCursor(DISPLAY_START_COLUMN, DISPLAY_I2C_SCAN_ROW);

  uint8_t number_of_addresses = payload[DATA_ROUTER_INPUT_I2C_SCAN_RETURN_NUM_OF_ADDR_POS];
  uint8_t selected_i2c_address = payload[DATA_ROUTER_INPUT_I2C_SCAN_RETURN_CURRENT_ADDR_POS];

  if(selected_i2c_address < number_of_addresses)
  {
    uint8_t i2c_address = payload[DATA_ROUTER_INPUT_I2C_SCAN_RETURN_STARTING_ADDR_POS + selected_i2c_address];
    String display_string = "I2C Addr: 0x" + String(i2c_address < DISPLAY_TWO_CIPHER_NUMBER ? "0" : "") + String(i2c_address, HEX);
    while (display_string.length() < DISPLAY_LCD_WIDTH) 
    {
      display_string += ' '; //Add spaces to fill to the end
    }
    lcd.print(display_string);
  }
  else
  {
    error_code = ERROR_CODE_DISPLAY_CURRENT_I2C_ADDR_OUT_OF_RANGE;
  }

  return error_code;
}