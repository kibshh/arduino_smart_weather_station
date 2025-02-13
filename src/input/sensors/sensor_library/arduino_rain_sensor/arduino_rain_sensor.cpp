#include "arduino_rain_sensor.h"

/* STATIC FUNCTION PROTOTYPES */
#ifdef SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT
/**
 * @brief Checks rain status using an analog rain sensor pin.
 * @return true if the analog reading is below the threshold, false otherwise.
 */
static bool arduino_rain_sensor_isRainingAnalog();
#else
/**
 * @brief Checks rain status using a digital rain sensor pin.
 * @return true if rain is detected (LOW signal), false otherwise (HIGH signal).
 */
static bool arduino_rain_sensor_isRainingDigital();
#endif
/* *************************************** */

/* EXPORTED FUNCTIONS */
void arduino_rain_sensor_init()
{
#ifdef SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT
  // Configure the pin for analog input if analog measurement is enabled
  pinMode(SENSORS_ARDUINO_RAIN_PIN_ANALOG, INPUT);
#else
  // Configure the pin for digital input if digital measurement is enabled
  pinMode(SENSORS_ARDUINO_RAIN_PIN_DIGITAL, INPUT);
#endif
}

bool arduino_rain_sensor_readRaining()
{
#ifdef SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT
  // Call analog-specific rain detection function
  return arduino_rain_sensor_isRainingAnalog();
#else
  // Call digital-specific rain detection function
  return arduino_rain_sensor_isRainingDigital();
#endif
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
#ifdef SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT
static bool arduino_rain_sensor_isRainingAnalog()
{
  int analog_reading = analogRead(SENSORS_ARDUINO_RAIN_PIN_ANALOG);
  // Return true if the reading is below or equal to the defined threshold
  if(ARDUINO_RAIN_SENSOR_ANALOG_THRESHOLD >= analog_reading)
  {
    return true;
  }
  else
  {
    return false;
  }
}
#endif

#ifndef SENSORS_ARDUINO_RAIN_SENSOR_ANALOG_MEASUREMENT
static bool arduino_rain_sensor_isRainingDigital()
{
  int rain_detected = digitalRead(RAINSENSOR_PIN_DIGITAL);

  // Return true if LOW (rain detected), false otherwise
  if(ARDUINO_RAIN_SENSOR_RAIN_DETECTED == rain_detected)
  {
    return true;
  }
  else
  {
    return false;
  }
}
#endif
/* *************************************** */