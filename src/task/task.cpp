#include "task.h"

void task_cyclicTask()
{
  static unsigned long previous_millis = 0;

  static size_t sensors_len = sensors_getSensorsLen();

  static uint8_t sensor_index = TASK_STARTING_SENSOR_INDEX;

  unsigned long current_millis = millis();

#ifdef MODE_GET_I2C_ADDR
  if (current_millis - previous_millis >= (unsigned long)I2CSCAN_I2CSCAN_INTERVAL_MS)
  {
    previous_millis = current_millis;
    i2cScan_scanForAdress();
  }
#else
  if (current_millis - previous_millis >= (unsigned long)DISPLAY_TIME_DISPLAY_INTERVAL_MS)
  {
    previous_millis = current_millis;

    data_router_input_data_ts return_data = data_router_fetchDataFromInput(INPUT_RTC, RTC_DEFAULT_RTC);

    error_manager_error_ts error = data_router_routeDataToOutput(OUTPUT_DISPLAY, return_data);

    // handle with error handler
  }
  if (current_millis - previous_millis >= (unsigned long)DISPLAY_DISPLAY_INTERVAL_MS)
  {
    previous_millis = current_millis;

    if (sensor_index >= sensors_len) {
      sensor_index = TASK_STARTING_SENSOR_INDEX;
    }

    data_router_input_data_ts return_data = data_router_fetchDataFromInput(INPUT_SENSORS, sensors_sensorIndexToId(sensor_index));

    sensor_index++;

    error_manager_error_ts error = data_router_routeDataToOutput(OUTPUT_DISPLAY, return_data);

    // handle with error handler
  }
#endif
}