#include "arduino_rain_sensor.h"


void rainsensor_init()
{
#ifdef RAINSENSOR_ANALOG_MEASUREMENT
  pinMode(RAINSENSOR_PIN_ANALOG, INPUT);
#else
  pinMode(RAINSENSOR_PIN_DIGITAL, INPUT);
#endif
}

rainsensor_reading_t rainsensor_readRaining()
{
  rainsensor_reading_t reading = {0};
  reading.success = false;

#ifdef RAINSENSOR_ANALOG_MEASUREMENT
  reading.is_raining = rainsensor_isRainingAnalog();
#else
  reading.is_raining = rainsensor_isRainingDigital();
#endif
  if(!isnan(reading.is_raining))
  {
    reading.success = true;
  }
  return reading;
}

#ifndef RAINSENSOR_ANALOG_MEASUREMENT
  boolean rainsensor_isRainingDigital()
  {
    int rain_detected = digitalRead(RAINSENSOR_PIN_DIGITAL);

    if(LOW == rain_detected)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
#endif

#ifdef RAINSENSOR_ANALOG_MEASUREMENT
  boolean rainsensor_isRainingAnalog()
  {
    int analog_reading = analogRead(RAINSENSOR_PIN_ANALOG);
    if(RAINSENSOR_ANALOG_THRESHOLD >= analog_reading)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
#endif