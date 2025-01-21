#include "task.h"

void task_cyclicTask()
{

  // static uint8_t current_display_function = 0;
  static unsigned long previous_millis = 0;

  unsigned long current_millis = millis();

#ifdef MODE_GET_I2C_ADDR
  if (current_millis - previous_millis >= (unsigned long)I2CSCAN_I2CSCAN_INTERVAL_MS)
  {
    previous_millis = current_millis;
    i2cScan_scanForAdress();
  }
#else
/*
  if (current_millis - previous_millis >= (unsigned long)DISPLAY_TIME_DISPLAY_INTERVAL_MS)
  {
    previous_millis = current_millis;

    uint8_t return_buffer[DATA_ROUTER_INPUT_RTC_RETURN_LEN] = {0};
    size_t return_buffer_size = sizeof(return_buffer);

    error_manager_error_code_te error_code = data_router_fetchDataFromInput(
        INPUT_RTC, NULL, 0, return_buffer, &return_buffer_size);
    // RTC read success code
    if (error_code == ERROR_CODE_NO_ERROR)
    {
      data_router_routeDataToOutput(OUTPUT_DISPLAY, TIME_MEASUREMENT, return_buffer, return_buffer_size);
    }
    else
    {
      Serial.println("Internal RTC error, failed to write to buffer");
    }
  }
  */
  // if (current_millis - previous_millis >= (unsigned long)DISPLAY_DISPLAY_INTERVAL_MS)
  // {

  //   current_display_function++;
  //   current_display_function %= num_of_display_functions;
  // }
#endif
}