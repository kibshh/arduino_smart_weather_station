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
}

const int num_of_display_functions = sizeof(display_sensors_config) / sizeof(display_sensors_config[0]);


LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

void display_init()
{
  lcd.begin(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT); // Initialize a 16x2 LCD
  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.noCursor();
}

void display_displayData(uint_8 current_sensor_index)
{
  lcd.clear();
  lcd.setCursor(0, 0);

  current_sensor = display_sensors_config[current_sensor_index];

  sensor_reading_t reading = current_sensor.function();

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

    lcd.print(current_sensor.sensor_type);
    lcd.print(": ");
    lcd.print(val);
    lcd.print(current_sensor.measurement_unit);
  }
  else
  {
    lcd.print("Error ");
    lcd.print(current_sensor.sensor_type);
  }
}

void display_displayDate()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  rtc_date_reading_t reading = rtc_getDate();

  if(true == reading.success)
  {
    char formatter_month[DISPLAY_DATETIME_FORMATER_LEN];
    char formatter_day[DISPLAY_DATETIME_FORMATER_LEN];

    sprintf(formatter_month, "%02d", reading.currentDate.month);
    sprintf(formatter_day, "%02d", reading.currentDate.day);

    lcd.print(reading.currentDate.year);
    lcd.print("/");
    lcd.print(formatter_month);
    lcd.print("/");
    lcd.print(formatter_day); 
  }
  else
  {
    lcd.print("Error date");
  }
}

void display_displayTime()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  rtc_time_reading_t reading = rtc_getTime();

  if(true == reading.success)
  {
    char formatter_hour[DISPLAY_DATETIME_FORMATER_LEN];
    char formatter_mins[DISPLAY_DATETIME_FORMATER_LEN];
    char formatter_secs[DISPLAY_DATETIME_FORMATER_LEN];

    sprintf(formatter_hour, "%02d", reading.currentTime.hour);
    sprintf(formatter_mins, "%02d", reading.currentTime.mins);
    sprintf(formatter_secs, "%02d", reading.currentTime.secs);

    lcd.print(formatter_hour);
    lcd.print(":");
    lcd.print(formatter_mins);
    lcd.print(":");
    lcd.print(formatter_secs); 
  }
  else
  {
    lcd.print("Error time");
  }
}