#include "display.h"


display_sensors_config_t display_sensors_config[] =
{
  {"Temp",        "C",      DHT11_TEMPERATURE,       SENSORS_VALUE,      DISPLAY_1_DECIMAL},
  {"Humidity",    "%",      DHT11_HUMIDITY,          SENSORS_VALUE,      DISPLAY_1_DECIMAL},
  {"Press",       "hPa",    BMP280_PRESSURE,         SENSORS_VALUE,      DISPLAY_1_DECIMAL},
  {"BPM Temp",    "C",      BMP280_TEMPERATURE,      SENSORS_VALUE,      DISPLAY_1_DECIMAL},
  {"Altitude",    "m",      BMP280_ALTITUDE,         SENSORS_VALUE,      DISPLAY_0_DECIMALS},
  {"Luminance",   "lx",     BH1750_LUMINANCE,        SENSORS_VALUE,      DISPLAY_0_DECIMALS},
  {"Gases PPM",   "",       MQ135_PPM,               SENSORS_VALUE,      DISPLAY_0_DECIMALS},
  {"CO PPM",      "",       MQ7_COPPM,               SENSORS_VALUE,      DISPLAY_0_DECIMALS},
  {"UV int",      "",       GYML8511_UV,             SENSORS_VALUE,      DISPLAY_1_DECIMAL},
  {"Raining",     "",       ARDUINORAIN_RAINING,     SENSORS_INDICATION, DISPLAY_NO_DECIMALS},
};

const int num_of_display_functions = sizeof(display_sensors_config) / sizeof(display_sensors_config[0]);


LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

void display_init()
{
  lcd.begin(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT); // Initialize a 16x2 LCD
  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.noCursor();
}

void display_displayData(uint8_t current_sensor_index)
{
  lcd.setCursor(0, 0);

  display_sensors_config_t current_sensor = display_sensors_config[current_sensor_index];

  sensor_reading_t reading = sensors_getReading(current_sensor.id, current_sensor.measurement_type);

  String display_string = "";

  if(true == reading.success)
  {
    String val = "";

    if(DISPLAY_READING_VALUE == reading.measurement_type_switch)
    {
      val = String(reading.value, current_sensor.accuracy);
    }
    else
    {
      if(true == reading.indication)
      {
        val = "yes";
      }
      else
      {
        val = "no";
      }
    }
    display_string = current_sensor.sensor_type + ": " + val + current_sensor.measurement_unit;
  }
  else
  {
    display_string = "Error " + current_sensor.sensor_type;
  }
  while (display_string.length() < DISPLAY_LCD_WIDTH) 
  {
    display_string += ' '; //Add spaces to fill to the end
  }
  lcd.print(display_string);
}

void display_updateTime()
{
  lcd.setCursor(0, 1);

  rtc_time_reading_t time_reading = rtc_getTime();
  rtc_date_reading_t date_reading = rtc_getDate();

  if(true == time_reading.success && true == date_reading.success)
  {
    String time_string = (time_reading.currentTime.hour < 10 ? "0" : "") + String(time_reading.currentTime.hour) + ":" +
                         (time_reading.currentTime.mins < 10 ? "0" : "") + String(time_reading.currentTime.mins) + " " +
                         (date_reading.currentDate.day < 10 ? "0" : "") + String(date_reading.currentDate.day) + "/" +
                         (date_reading.currentDate.month < 10 ? "0" : "") + String(date_reading.currentDate.month) + "/" +
                         String(date_reading.currentDate.year);

    lcd.print(time_string);
  }
  else
  {
    lcd.print("Error time");
  }  
}