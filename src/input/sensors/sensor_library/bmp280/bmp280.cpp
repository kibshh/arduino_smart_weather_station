#include "bmp280.h"

static Adafruit_BMP280 bmp;

boolean bmp280_init()
{
  if(!bmp.begin(SENSORS_BMP280_I2C_ADDR))
  {
    return false;
  }
  bmp.setSampling(BMP280_MODE_NORMAL,     // Operating Mode
                  BMP280_SAMPLING_X2,     // Temperature oversampling(takes 2 samples)
                  BMP280_SAMPLING_X16,    // Pressure oversampling(takes 16 samples)->more accurrate
                  BMP280_FILTER_X16,      // Filtering
                  BMP280_WAIT_MS_500);    // Standby time between readings
  return true;
}

float bmp280_readTemperature()
{
    return bmp.readTemperature();
}

float bmp280_readPressure()
{
    return bmp.readPressure();
}

float bmp280_readAltitude()
{
    return bmp.readAltitude(BMP280_SEA_LEVEL_PRESSURE);
}