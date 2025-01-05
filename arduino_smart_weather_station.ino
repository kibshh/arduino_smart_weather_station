#include "src/sensors/sensors.h"
#include "src/functionality/i2cScan.h"
#include "src/display/display.h"

//#define MODE_GET_I2C_ADDR
#define BAUDRATE 9600

int current_display_function = 0;
unsigned long previous_millis = 0;

void setup() {
  Wire.begin();
  Serial.begin(BAUDRATE);
  display_init();
  sensors_init();
  rtc_init();
  while (!Serial); // Wait for the serial port to connect

  #ifdef MODE_GET_I2C_ADDR
    Serial.println("\nI2C Scanner");
  #else
    Serial.println("\nWeather Station\n");
  #endif
}

void loop() {
  unsigned long current_millis = millis();

  #ifdef MODE_GET_I2C_ADDR
    if(current_millis - previous_millis >= (unsigned long)I2CSCAN_I2CSCAN_INTERVAL_MS)
    {
      previous_millis = current_millis;
      i2cScan_scanForAdress();
    }
  #else
    if(current_millis - previous_millis >= (unsigned long)DISPLAY_DISPLAY_INTERVAL_MS)
    {
      previous_millis = current_millis;
      display_displayData(current_display_function);
      current_display_function++;
      current_display_function %= num_of_display_functions;
    }
  #endif
}
