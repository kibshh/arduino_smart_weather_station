#include "src/project_settings.h"
#include "src/sensors/sensors.h"
#include "src/functionality/rtc.h"
#include "src/functionality/i2cScan.h"
#include "src/display/display.h"
#include "src/task/task.h"

void setup() 
{
#if defined(DEBUG_USED) || defined(UART_USED) || defined(USB_USED)
  Serial.begin(BAUDRATE);
  while(!Serial); // Wait for the serial port to connect
#endif

#ifdef I2C_USED
  Wire.begin();
#endif

#ifdef DISPLAY_USED
  display_init();
#endif

#ifdef SENSORS_USED
  sensors_init();
#endif

#ifdef RTC_USED
  rtc_init();
#endif

#ifdef DEBUG_USED
  #ifdef MODE_GET_I2C_ADDR
    Serial.println("\nI2C Scanner\n");
  #else
    Serial.println("\nWeather Station\n");
  #endif
#endif
}

void loop() 
{
  task_cyclicTask();
}
