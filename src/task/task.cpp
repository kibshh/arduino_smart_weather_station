#include "task.h"

void task_cyclicTask()
{
/*
    static uint8_t current_display_function = 0;
    static unsigned long previous_millis = 0;

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
      display_displayTime();
    }
#endif
*/
}