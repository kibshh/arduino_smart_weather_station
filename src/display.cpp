#include "display.h"


LiquidCrystal_I2C lcd(DISPLAY_LCD_I2C_ADDDR, DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT);

void display_init()
{
  lcd.begin(DISPLAY_LCD_WIDTH, DISPLAY_LCD_HEIGHT); // Initialize a 16x2 LCD

  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.noCursor();
}

void display_displayTemperature()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getTemperature();
  String temperature = String(reading.value, DISPLAY_1_DECIMAL);

  if(true == reading.success)
  {
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
  }
  else
  {
    lcd.print("Error temp");
  }
}

void display_displayHumidity()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getHumidity();
  String humidity = String(reading.value, DISPLAY_1_DECIMAL);

  if(true == reading.success)
  {
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }
  else
  {
    lcd.print("Error humidity");
  }
}

void display_displayPressure()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getPressure();
  String pressure = String(reading.value, DISPLAY_1_DECIMAL);

  if(true == reading.success)
  {
    lcd.print("Press: ");
    lcd.print(pressure);
    lcd.print("hPa");
  }
  else
  {
    lcd.print("Error pressure");
  }
}

void display_displayTemperatureBPM()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getTemperatureBMP();
  String temperature = String(reading.value, DISPLAY_1_DECIMAL);

  if(true == reading.success)
  {
    lcd.print("BPM Temp: ");
    lcd.print(temperature);
    lcd.print("C");
  }
  else
  {
    lcd.print("Error BPM temp");
  }
}

void display_displayAltitude()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getAltitude();
  String altitude = String(reading.value, DISPLAY_0_DECIMALS);

  if(true == reading.success)
  {
    lcd.print("Altitude: ");
    lcd.print(altitude);
    lcd.print("m");
  }
  else
  {
    lcd.print("Error altitude");
  }
}

void display_displayLuminance()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getLuminance();
  String luminance = String(reading.value, DISPLAY_0_DECIMALS);

  if(true == reading.success)
  {
    lcd.print("Luminance: ");
    lcd.print(luminance);
    lcd.print("lx");
  }
  else
  {
    lcd.print("Error luminance");
  }
}

void display_displayVariousGasesPPM()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getVariousGasesPPM();
  String ppm = String(reading.value, DISPLAY_0_DECIMALS);

  if(true == reading.success)
  {
    lcd.print("Gases PPM: ");
    lcd.print(ppm);
  }
  else
  {
    lcd.print("Error gases PPM");
  }
}

void display_displayCOPPM()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getCOPPM();
  String ppm = String(reading.value, DISPLAY_0_DECIMALS);

  if(true == reading.success)
  {
    lcd.print("CO PPM: ");
    lcd.print(ppm);
  }
  else
  {
    lcd.print("Error CO PPM");
  }
}

void display_displayUV()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  sensor_reading_t reading = sensors_getUvIntensity();
  String uv_intensity = String(reading.value, DISPLAY_1_DECIMAL);

  if(true == reading.success)
  {
    lcd.print("UV int: ");
    lcd.print(uv_intensity);
  }
  else
  {
    lcd.print("Error UV");
  }
}

void display_displayRainingStatus()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  String is_it_raining = "Error Raining";
  sensor_reading_indicator_type_t reading = sensors_getRainingStatus();

  if(true == reading.success)
  {
    if(true == reading.value)
    {
      is_it_raining = "Raining";
    }
    else
    {
      is_it_raining = "Not Raining";
    }
  }
  lcd.print(is_it_raining);
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