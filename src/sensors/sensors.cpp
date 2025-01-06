#include "sensors.h"


//GLOBAL VARIABLES
DHT dht(SENSORS_PIN_DHT, SENSORS_DHTTYPE);
Adafruit_BMP280 bmp;
BH1750 lightMeter;


void sensors_init()
{
  dht.begin();
  if(!lightMeter.begin())
  {
    Serial.println("Error in initialization of BH1750 sensor");
  }
  if(!bmp.begin(SENSORS_BMP280_I2C_ADDDR))
  {
    Serial.println("Error in initialization of BMP280 sensor");
  }
  bmp.setSampling(SENSORS_BMP280_MODE_NORMAL,     // Operating Mode
                  SENSORS_BMP280_SAMPLING_X2,     // Temperature oversampling(takes 2 samples)
                  SENSORS_BMP280_SAMPLING_X16,    // Pressure oversampling(takes 16 samples)->more accurrate
                  SENSORS_BMP280_FILTER_X16,      // Filtering
                  SENSORS_BMP280_WAIT_MS_500);    // Standby time between readings
  mq135_init();
  mq7_init();
  uv_init();
}


sensor_reading_t sensors_getTemperature()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};
  reading.value = dht.readTemperature(); // Celsius

  float temperature = reading.value;

  if(!isnan(temperature))
  {
    if((int)temperature > SENSORS_TEMPERATURE_MIN && (int)temperature < SENSORS_TEMPERATURE_MAX)
    {
      reading.success = true;    
    }
  }
  return reading;
}


sensor_reading_t sensors_getHumidity()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};
  reading.value = dht.readHumidity(); // Percent

  float humidity = reading.value;

  if(!isnan(humidity))
  {
    if((int)humidity > SENSORS_HUMIDITY_MIN && (int)humidity < SENSORS_HUMIDITY_MAX)
    {
      reading.success = true;    
    }
  }
  return reading;
}


sensor_reading_t sensors_getPressure()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};
  reading.value = bmp.readPressure() / 100.0F; // Converting to hPa

  float pressure = reading.value;

  if(!isnan(pressure))
  {
    if((int)pressure > SENSORS_PRESSURE_MIN && (int)pressure < SENSORS_PRESSURE_MAX)
    {
      reading.success = true;    
    }
  }
  return reading;
}

sensor_reading_t sensors_getTemperatureBMP()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};
  reading.value = bmp.readTemperature(); // Celsius

  float temperature = reading.value;

  if(!isnan(temperature))
  {
    if((int)temperature > SENSORS_TEMPERATURE_MIN && (int)temperature < SENSORS_TEMPERATURE_MAX)
    {
      reading.success = true;    
    }
  }
  return reading;  
}

sensor_reading_t sensors_getAltitude()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};
  reading.value = bmp.readAltitude(SENSORS_SEA_LEVEL_PRESSURE); // Celsius

  float altitude = reading.value;

  if(!isnan(altitude))
  {
    if((int)altitude > SENSORS_ALTITUDE_MIN && (int)altitude < SENSORS_ALTITUDE_MAX)
    {
      reading.success = true;    
    }
  }
  return reading;  
}

sensor_reading_t sensors_getLuminance()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};
  reading.value = lightMeter.readLightLevel(); // lux

  float luminance = reading.value;

  if(!isnan(luminance))
  {
    if((int)luminance > SENSORS_LUMINANCE_MIN && (int)luminance < SENSORS_LUMINANCE_MAX)
    {
      reading.success = true;    
    }
  }
  return reading;  
}

sensor_reading_t sensors_getVariousGasesPPM()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};

  mq135_ppm_reading_t ppm_reading = mq135_readPPM(); // PPM

  if(true == ppm_reading.success)
  {
    reading.value = ppm_reading.value;
    reading.success = true;
  }
  return reading;
}

sensor_reading_t sensors_getCOPPM()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};

  mq7_ppm_reading_t ppm_reading = mq7_readPPM(); // CO PPM

  if(true == ppm_reading.success)
  {
    reading.value = ppm_reading.value;
    reading.success = true;
  }
  return reading;  
}

sensor_reading_t sensors_getUvIntensity()
{
  sensor_reading_t reading = {0.0, DISPLAY_READING_VALUE, false, false};

  uv_reading_t uv_reading = uv_readUvIntensity(); // mW/cm^2 

  if(true == uv_reading.success)
  {
    reading.value = uv_reading.value;
    reading.success = true;
  }
  return reading;  
}

sensor_reading_t sensors_getRainingStatus()
{
  sensor_reading_t reading = {0.0, DISPLAY_INDICATION, false, false};

  rainsensor_reading_t raining_status = rainsensor_readRaining(); // true/false 

  if(true == raining_status.success)
  {
    reading.indication = raining_status.is_raining;
    reading.success = true;
  }
  return reading;  
}

