#include "sensors.h"
#include "i2cScan.h"
#include "display.h"

//#define MODE_GET_I2C_ADDR

#define BAUDRATE 9600


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
    Serial.println("\nWeather Station");
  #endif
}

void loop() {

  #ifdef MODE_GET_I2C_ADDR
    delay(2000);
    i2cScan_scanForAdress();
  #else
    // display_displayTemperature();
    // delay(2000);
    // display_displayHumidity();
    // delay(2000);
    // display_displayPressure();
    // delay(2000);
    // display_displayTemperatureBPM();
    // delay(2000);
    // display_displayAltitude();
    // delay(2000);
    // display_displayLuminance();
    // delay(2000);
    display_displayDate();
    delay(2000);
    display_displayTime();
    delay(2000);
  #endif
}
