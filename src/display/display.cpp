#include "display.h"


display_sensors_config_t display_sensors_config[] =
{
  {"Temp",        "C",      sensors_getTemperature,       DISPLAY_1_DECIMAL},
  {"Humidity",    "%",      sensors_getHumidity,          DISPLAY_1_DECIMAL},
  {"Press",       "hPa",    sensors_getPressure,          DISPLAY_1_DECIMAL},
  {"BPM Temp",    "C",      sensors_getTemperatureBMP,    DISPLAY_1_DECIMAL},
  {"Altitude",    "m",      sensors_getAltitude,          DISPLAY_0_DECIMALS},
  {"Luminance",   "lx",     sensors_getLuminance,         DISPLAY_0_DECIMALS},
  {"Gases PPM",   "",       sensors_getVariousGasesPPM,   DISPLAY_0_DECIMALS},
  {"CO PPM",      "",       sensors_getCOPPM,             DISPLAY_0_DECIMALS},
  {"UV int",      "",       sensors_getUvIntensity,       DISPLAY_1_DECIMAL},
  {"Raining",     "",       sensors_getRainingStatus,     DISPLAY_NO_DECIMALS},
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

  sensor_reading_t reading = current_sensor.function();

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