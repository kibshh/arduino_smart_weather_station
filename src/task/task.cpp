#include "task.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Retrieves the next state based on the current state.
 *
 * This function increments the current state if it's not the last state in the sequence.
 * If the current state is the last one, it remains unchanged. This ensures that state transitions
 * happen sequentially and no circular transitions occur.
 *
 * @param current_state The current state.
 * @return The next state, or the same state if already at the last state.
 */
static uint8_t getNextState(uint8_t current_state);

/**
 * @brief Checks if the interval for a given task has passed.
 *
 * This function calculates whether the time interval for executing a task
 * has elapsed based on the current system time.
 *
 * @param task_id The unique identifier of the task.
 * @return true if the interval has passed, false otherwise.
 */
static bool intervalPassed(uint8_t task_id);

/**
 * @brief Increments the execution count of a task.
 *
 * This function searches for the task in the task configuration array using
 * its unique ID and increments its execution count if found.
 *
 * @param task_id The unique identifier of the task.
 */
static void incrementTaskExecutionNumber(uint8_t task_id);

/**
 * @brief Retrieves the execution count of a task.
 *
 * This function searches for the task in the task configuration array using
 * its unique ID and returns the number of times it has been executed.
 * If the task is not found, it returns TASK_INVALID_EXECUTION_NUM.
 *
 * @param task_id The unique identifier of the task.
 * @return The execution count of the task, or TASK_INVALID_EXECUTION_NUM if not found.
 */
static uint16_t getTaskExecutionNumber(uint8_t task_id);

/**
 * @brief Finds the index of a task in the task configuration array.
 *
 * This function loops through the task configuration array to find the index
 * of the task that matches the given task ID.
 *
 * @param task_id The unique identifier of the task.
 * @return The index of the task in the array or TASK_INVALID_INDEX if not found.
 */
static uint8_t findTaskIndex(uint8_t task_id);

/**
 * @brief Retrieves the total number of configured tasks.
 *
 * This function calculates the number of tasks configured in the system.
 *
 * @return The number of tasks in the task configuration array.
 */
static size_t getNumOfTasks();

/**
 * @brief Retrieves the current system time.
 *
 * This function returns the current system time in milliseconds.
 *
 * @return The current time in milliseconds.
 */
static uint32_t currentTime();
/* *************************************** */

/* STATIC GLOBAL VARIABLES */
static tasks_config_ts tasks_config[] = 
{
  {PERIOD_REINIT,            currentTime(), 0, TASK_REINIT},
  {PERIOD_CALIBRATION_MQ135, currentTime(), 0, TASK_CALIBRATION_MQ135},
  {PERIOD_CALIBRATION_MQ7,   currentTime(), 0, TASK_CALIBRATION_MQ7},
  {PERIOD_I2C_SCANNING,      currentTime(), 0, TASK_I2C_SCANNING},
  {PERIOD_SHOW_TIME,         currentTime(), 0, TASK_SHOW_TIME},
  {PERIOD_SENSOR_READ,       currentTime(), 0, TASK_SENSOR_READ},
};
/* *************************************** */

/* EXPORTED FUNCTIONS */
void task_cyclicTask()
{
  uint32_t current_time = currentTime();
  control_loop(&current_time);
}

void task_mainTask()
{
  // Initialize components if they haven't been initialized yet or reinitialize if needed
  static bool all_components_initialized = control_init();
  if(CONTROL_INITIALIZATION_FAILED == all_components_initialized && INTERVAL_PASSED == intervalPassed(TASK_REINIT))
  {
    all_components_initialized = control_reinit();
  }
  
  static uint8_t current_state = STATE_IDLE; // Tracks the current state of the task
  switch(current_state)
  {
    case STATE_IDLE:
      // Initial state
      current_state = getNextState(current_state);
      break;

    case STATE_I2C_SCANNING: 
      // In this state, the task scans I2C addresses periodically
      static i2c_scan_reading_context_ts i2c_scan_reading_context = app_createI2CScanReadingContext();
      if(INTERVAL_PASSED == intervalPassed(TASK_I2C_SCANNING))
      {
        if(FINISHED == app_readAllI2CAddressesPeriodic(ALL_OUTPUTS, &i2c_scan_reading_context))
        {
          current_state = getNextState(current_state);
        }
      }
      break;

    case STATE_CALIBRATING_MQ135:
      // This state handles the calibration of the MQ135 sensor
      if(INTERVAL_PASSED == intervalPassed(TASK_CALIBRATION_MQ135))
      {
        incrementTaskExecutionNumber(TASK_CALIBRATION_MQ135);
        if(FINISHED == app_readSpecificSensor(MQ135_CALIBRATION_RESISTANCE, ALL_OUTPUTS) && getTaskExecutionNumber(TASK_CALIBRATION_MQ135) >= SENSORS_MQ135_NUM_OF_CALIBRATIONS)
        {
          current_state = getNextState(current_state);
        }
      }
      break;

    case STATE_CALIBRATING_MQ7:
      // This state handles the calibration of the MQ7 sensor
      if(INTERVAL_PASSED == intervalPassed(TASK_CALIBRATION_MQ7))
      {
        incrementTaskExecutionNumber(TASK_CALIBRATION_MQ7);
        if(FINISHED == app_readSpecificSensor(MQ7_CALIBRATION_RESISTANCE, ALL_OUTPUTS) && getTaskExecutionNumber(TASK_CALIBRATION_MQ7) >= SENSORS_MQ7_NUM_OF_CALIBRATIONS)
        {
          current_state = getNextState(current_state);
        }
      }
      break;

    case STATE_MAIN_FUNCTION:
      // This state performs the core operations, such as reading the RTC and sensor data
      if(INTERVAL_PASSED == intervalPassed(TASK_SHOW_TIME)) // Read the current time from the RTC and display it
      {
        (void)app_readCurrentRtcTime(LCD_DISPLAY);
      }
      if(INTERVAL_PASSED == intervalPassed(TASK_SENSOR_READ))
      {
        static sensor_reading_context_ts sensor_reading_context = app_createNewSensorsReadingContext();
        (void)app_readAllSensorsPeriodic(ALL_OUTPUTS, &sensor_reading_context); // Read data from all connected sensors periodically one by one
      }
      break;
  }
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static uint8_t getNextState(uint8_t current_state)
{
  if(STATES_COUNT - 1 > current_state)
  {
    current_state++;
  }
  return current_state;
}

static bool intervalPassed(uint8_t task_id)
{
  uint8_t task_index = findTaskIndex(task_id);
  if(TASK_INVALID_INDEX != task_index)
  {
    uint32_t current_time = currentTime(); // Get current system time
    
    if((uint32_t)(current_time - tasks_config[task_index].time_elapsed) >= tasks_config[task_index].task_period)
    {
      tasks_config[task_index].time_elapsed = current_time; // Update last execution time
      return INTERVAL_PASSED;
    }
  }
  return INTERVAL_NOT_PASSED;
}

static void incrementTaskExecutionNumber(uint8_t task_id)
{
  uint8_t task_index = findTaskIndex(task_id);
  if(TASK_INVALID_INDEX != task_index)
  {
    tasks_config[task_index].task_execution_number++;
  }
}

static uint16_t getTaskExecutionNumber(uint8_t task_id)
{
  uint8_t task_index = findTaskIndex(task_id);
  if(TASK_INVALID_INDEX != task_index)
  {
    return tasks_config[task_index].task_execution_number;
  }
  return TASK_INVALID_EXECUTION_NUM;
}

static uint8_t findTaskIndex(uint8_t task_id)
{
  uint8_t index_returned = TASK_INVALID_INDEX;

  size_t num_of_tasks = getNumOfTasks(); // Get total number of tasks

   // Loop through all tasks until the matching task is found
  for (uint8_t index = TASK_FIRST_TASK_INDEX; index < num_of_tasks; index++)
  {
    if(task_id == tasks_config[index].task_id)
    {
      index_returned = index; // Store the index if found
      break;
    }
  }

  return index_returned;
}

static size_t getNumOfTasks()
{
  size_t num_of_tasks = TASK_NO_TASKS; // Default value in case of no tasks configured
  // Ensure there is at least one task in the configuration
  if (TASK_NO_TASKS < sizeof(tasks_config))
  {
    num_of_tasks = sizeof(tasks_config) / sizeof(tasks_config[TASK_FIRST_TASK_INDEX]);
  }
  return num_of_tasks;
}

static uint32_t currentTime()
{
  return millis(); // Get system uptime in milliseconds
}
/* *************************************** */