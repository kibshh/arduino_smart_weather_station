#include "task.h"

static tasks_config_ts tasks_config[] =
{
  {millis(), TASK_I2C_ADDR_READ_TIMER, TASK_I2C_ADDR_READ},
  {millis(), TASK_SENSOR_READ_TIMER, TASK_SENSOR_READ},
  {millis(), TASK_TIME_READ_TIMER, TASK_TIME_READ}
};

static bool intervalPassed(uint8_t task_id);
static uint8_t findTaskIndex(uint8_t task_id);
static size_t getNumOfTasks();

void task_initTask()
{

}

void task_cyclicTask()
{
  static i2c_scan_reading_context_ts context_i2c_scan = app_createI2CScanReadingContext();
  static sensor_reading_context_ts context_sensor_reading = app_createNewSensorsReadingContext();
  static task_state_machine_te current_state = STATE_SCANNING_FOR_I2C_ADDRESSES;

  if(STATE_SCANNING_FOR_I2C_ADDRESSES == current_state)
  {
    if(INTERVAL_PASSED == intervalPassed(TASK_I2C_ADDR_READ))
    {
      if(FINISHED == app_readAllI2CAddressesPeriodic(ALL_OUTPUTS, &context_i2c_scan))
      {
        current_state = STATE_CYCLIC_SENSOR_AND_TIME_READING;
      }
    }
  }
  else if(STATE_CYCLIC_SENSOR_AND_TIME_READING == current_state)
  {
    if(INTERVAL_PASSED == intervalPassed(TASK_SENSOR_READ))
    {
      (void)app_readAllSensorsPeriodic(ALL_OUTPUTS, &context_sensor_reading);
    }
    if(INTERVAL_PASSED == intervalPassed(TASK_TIME_READ))
    {
      (void)app_readCurrentRtcTime(LCD_DISPLAY);
    }
  }

  delay(CYCLIC_TASK_DELAY_MS);
}

static bool intervalPassed(uint8_t task_id)
{
  uint32_t current_milis = millis();
  tasks_config_ts current_task;
  uint8_t task_index = findTaskIndex(task_id);
  if(TASK_INVALID_INDEX != task_index)
  {
    if(current_milis - tasks_config[task_index].previous_millis >= tasks_config[task_index].task_period)
    {
      tasks_config[task_index].previous_millis = current_milis;
      return INTERVAL_PASSED;
    }
  }
  return INTERVAL_NOT_PASSED;
}

static uint8_t findTaskIndex(uint8_t task_id)
{
  uint8_t index_returned = TASK_INVALID_INDEX;
  size_t num_of_tasks = getNumOfTasks();

  // Loop through all tasks till the right one is found
  for (uint8_t index = TASK_FIRST_TASK_INDEX; index < num_of_tasks; index++)
  {
    if(task_id == tasks_config[index].task_id)
    {
      index_returned = index;
      break;
    }
  }

  return index_returned;
}

static size_t getNumOfTasks()
{
  size_t num_of_tasks = TASK_NO_TASKS; // Default value in case of no tasks configured
  if (TASK_NO_TASKS < sizeof(tasks_config)) // Ensure there is at least 1 task
  {
    num_of_tasks = sizeof(tasks_config) / sizeof(tasks_config[TASK_FIRST_TASK_INDEX]);
  }
  return num_of_tasks;
}