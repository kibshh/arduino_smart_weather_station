#include "task.h"


static bool intervalPassed(uint8_t task_id);
static uint8_t findTaskIndex(uint8_t task_id);
static size_t getNumOfTasks();
static uint32_t currentTime();

static tasks_config_ts tasks_config[] = 
{
  {PERIOD_REINIT,        currentTime(), TASK_REINIT},
  {PERIOD_CALIBRATION,   currentTime(), TASK_CALIBRATION},
  {PERIOD_I2C_SCANNING,  currentTime(), TASK_I2C_SCANNING},
  {PERIOD_SHOW_TIME,     currentTime(), TASK_SHOW_TIME},
  {PERIOD_SENSOR_READ,   currentTime(), TASK_SENSOR_READ},
};

void task_cyclicTask()
{
  uint32_t current_time = currentTime();
  control_loop(&current_time);
}

void task_mainTask()
{
  static uint8_t current_state = STATE_IDLE;

  static bool all_components_initialized = control_init();
  if(CONTROL_INITIALIZATION_FAILED == all_components_initialized && INTERVAL_PASSED == intervalPassed(TASK_REINIT))
  {
    all_components_initialized = control_reinit();
  }
  
  switch(current_state)
  {
    case STATE_IDLE:
      // Should send some introductory message here
      current_state = STATE_I2C_SCANNING;
      break;

    case STATE_I2C_SCANNING:
      static i2c_scan_reading_context_ts i2c_scan_reading_context = app_createI2CScanReadingContext();
      if(INTERVAL_PASSED == intervalPassed(TASK_I2C_SCANNING))
      {
        if(FINISHED == app_readAllI2CAddressesPeriodic(ALL_OUTPUTS, &i2c_scan_reading_context))
        {
          current_state = STATE_CALIBRATING_MQ135;
        }
      }
      break;

    case STATE_CALIBRATING_MQ135:
      static uint8_t mq135_calibration_counter = 0;
      if(INTERVAL_PASSED == intervalPassed(TASK_CALIBRATION))
      {
        if(FINISHED == app_readSpecificSensor(MQ135_CALIBRATION_RESISTANCE, ALL_OUTPUTS) && mq135_calibration_counter >= SENSORS_MQ135_NUM_OF_CALIBRATIONS)
        {
          current_state = STATE_CALIBRATING_MQ7;
        }
      }
      break;

    case STATE_CALIBRATING_MQ7:
      static uint8_t mq7_calibration_counter = 0;
      if(INTERVAL_PASSED == intervalPassed(TASK_CALIBRATION))
      {
        if(FINISHED == app_readSpecificSensor(MQ7_CALIBRATION_RESISTANCE, ALL_OUTPUTS) && mq7_calibration_counter >= SENSORS_MQ7_NUM_OF_CALIBRATIONS)
        {
          current_state = STATE_MAIN_FUNCTION;
        }
      }
      break;

    case STATE_MAIN_FUNCTION:
      if(INTERVAL_PASSED == intervalPassed(TASK_SHOW_TIME))
      {
        (void)app_readCurrentRtcTime(LCD_DISPLAY);
      }
      if(INTERVAL_PASSED == intervalPassed(TASK_SENSOR_READ))
      {
        static sensor_reading_context_ts sensor_reading_context = app_createNewSensorsReadingContext();
        (void)app_readAllSensorsPeriodic(ALL_OUTPUTS, &sensor_reading_context);
      }
      break;
  }
}

static bool intervalPassed(uint8_t task_id)
{
  uint8_t task_index = findTaskIndex(task_id);
  if(TASK_INVALID_INDEX != task_index)
  {
    uint32_t current_time = currentTime();
    
    if(current_time - tasks_config[task_index].time_elapsed >= tasks_config[task_index].task_period)
    {
      tasks_config[task_index].time_elapsed = current_time;
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

static uint32_t currentTime()
{
  return millis();
}