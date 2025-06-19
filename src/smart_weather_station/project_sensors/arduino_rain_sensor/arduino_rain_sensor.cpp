#include "arduino_rain_sensor.h"

bool ArduinoRainSensor_Init()
{
#if ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE_ANALOG == ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE
  pinMode(ARDUINO_RAIN_SENSOR_PIN_ANALOG, INPUT);
#else
  pinMode(ARDUINO_RAIN_SENSOR_PIN_DIGITAL, INPUT);
#endif
  return true;
}

bool ArduinoRainSensor_ReadRainingStatus()
{
#if ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE_ANALOG == ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE
    uint16_t analog_reading = (uint16_t)analogRead(ARDUINO_RAIN_SENSOR_PIN_ANALOG);
    lcd.clear();
    lcd.setCursor(0, 0);
    if(ARDUINO_RAIN_SENSOR_ANALOG_THRESHOLD >= analog_reading)
    {
        Serial.println("Raining Status: Raining\n");
        lcd.print("Raining");
    }
    else
    {
        Serial.println("Raining Status: Not Raining\n");
        lcd.print("Not Raining");
    }
#else
    uint8_t digital_reading = (uint8_t)digitalRead(ARDUINO_RAIN_SENSOR_PIN_DIGITAL);
    lcd.clear();
    lcd.setCursor(0, 0);
    if(HIGH == digital_reading)
    {
        Serial.println("Raining Status: Raining\n");
        lcd.print("Raining");
    }
    else
    {
        Serial.println("Raining Status: Not Raining\n");
        lcd.print("Not Raining");
    }
#endif
    return true
}