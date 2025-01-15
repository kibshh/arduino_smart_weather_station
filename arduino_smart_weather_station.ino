#include "src/project_settings.h"
#include "src/sensors/sensors.h"
#include "src/functionality/i2cScan.h"
#include "src/display/display.h"

static uint8_t current_display_function = 0;
static unsigned long previous_millis = 0;

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
  unsigned long current_millis = millis();

#ifdef MODE_GET_I2C_ADDR
    if(current_millis - previous_millis >= (unsigned long)I2CSCAN_I2CSCAN_INTERVAL_MS)
    {
      previous_millis = current_millis;
      i2cScan_scanForAdress();
    }
#else
    if(current_millis - previous_millis >= (unsigned long)DISPLAY_TIME_DISPLAY_INTERVAL_MS)
    {
      display_updateTime();
    }
    if(current_millis - previous_millis >= (unsigned long)DISPLAY_DISPLAY_INTERVAL_MS)
    {
      previous_millis = current_millis;
      display_displayData(current_display_function);
      current_display_function++;
      current_display_function %= num_of_display_functions;
      display_updateTime();
    }
#endif
}
