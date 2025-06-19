#include "bmp280_sensor.h"

static Adafruit_BMP280 bmp;

bool Bmp280Sensor_Init()
{
  if(!bmp.begin(BMP280_SENSOR_I2C_ADDDR))
  {
    Serial.println("Error in initialization of BMP280 sensor");
  }
  bmp.setSampling(BMP280_SENSOR_MODE_NORMAL,     /* Operating Mode */
                  BMP280_SENSOR_SAMPLING_X2,     /* Temperature oversampling(takes 2 samples) */
                  BMP280_SENSOR_SAMPLING_X16,    /* Pressure oversampling(takes 16 samples)->more accurrate */
                  BMP280_SENSOR_FILTER_X16,      /* Filtering */
                  BMP280_SENSOR_WAIT_MS_500);    /* Standby time between readings */
}

bool Bmp280Sensor_ReadPressure()
{
  float pressure = bmp.readPressure() / BMP280_SENSOR_HPA_DIVIDER; /* Converting to hPa */
  lcd.clear();
  lcd.setCursor(0, 0);

  if(!isnan(pressure))
  {
    if(pressure > BMP280_SENSOR_PRESSURE_MIN && pressure < BMP280_SENSOR_PRESSURE_MAX)
    {
      String pressure_formatted = String(pressure, CONFIGS_DISPLAY_1_DECIMAL);

      Serial.print("Pressure: ");
      Serial.print(pressure_formatted);
      Serial.println("hPa\n");

      lcd.print("Press: ");
      lcd.print(pressure_formatted);
      lcd.print("hPa");
      return true;    
    }
  }
  Serial.println("Error pressure\n");
  lcd.print("Error pressure");
  return false; 
}

bool Bmp280Sensor_ReadTemperature()
{
  float temperature = bmp.readTemperature(); /* Celsius */
  lcd.clear();
  lcd.setCursor(0, 0);

  if(!isnan(temperature))
  {
    if(temperature > BMP280_SENSOR_TEMPERATURE_MIN && temperature < BMP280_SENSOR_TEMPERATURE_MAX)
    {
      String temperature_formatted = String(temperature, CONFIGS_DISPLAY_1_DECIMAL);

      Serial.print("BMP Temperature: ");
      Serial.print(temperature_formatted);
      Serial.println("C\n");

      lcd.print("BMP Temp: ");
      lcd.print(temperature_formatted);
      lcd.print("C");
      return true;   
    }
  }
  Serial.println("Error BMP temp\n");
  lcd.print("Error BMP temp");
  return false;  
}

bool Bmp280Sensor_ReadAltitude()
{
  float altitude = bmp.readAltitude(BMP280_SENSOR_SEA_LEVEL_PRESSURE);
  lcd.clear();
  lcd.setCursor(0, 0);

  if(!isnan(altitude))
  {
    if(altitude > BMP280_SENSOR_ALTITUDE_MIN && altitude < BMP280_SENSOR_ALTITUDE_MAX)
    {
      String altitude_formatted = String(altitude, CONFIGS_DISPLAY_0_DECIMALS);

      Serial.print("Altitude: ");
      Serial.print(altitude_formatted);
      Serial.println("m\n");

      lcd.print("Altitude: ");
      lcd.print(altitude_formatted);
      lcd.print("m");
      return true;     
    }
  }
  Serial.println("Error altitude\n");
  lcd.print("Error altitude");
  return false;  
}