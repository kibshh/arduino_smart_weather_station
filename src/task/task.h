#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "../app_layer/app.h"

/**
 * Defines time-related constants which are used to convert between time units (milliseconds, seconds, minutes, and hours)
 * and to define specific time intervals for various tasks.
 */
/* Defines the number of milliseconds in one second */
#define MS_PER_SECOND              ((uint32_t)(1000u))  
/* Defines the number of milliseconds in one minute */
#define MS_PER_MINUTE              ((uint32_t)(60u * MS_PER_SECOND))  
/* Defines the number of milliseconds in one hour */
#define MS_PER_HOUR                ((uint32_t)(60u * MS_PER_MINUTE))  
/* Converts hours to milliseconds */
#define TIME_HOURS(h)              ((uint32_t)((h) * MS_PER_HOUR))  
/* Converts minutes to milliseconds */
#define TIME_MINS(m)               ((uint32_t)((m) * MS_PER_MINUTE))  
/* Converts seconds to milliseconds */
#define TIME_SECS(s)               ((uint32_t)((s) * MS_PER_SECOND))  
/* Directly uses milliseconds */
#define TIME_MSECS(m)              ((uint32_t)(m))  

/* Represents no tasks available */
#define TASK_NO_TASKS              (size_t)(0u)  
/* Index of the first task in a task list */
#define TASK_FIRST_TASK_INDEX      (uint8_t)(0u)  
/* Represents an invalid task index (out of range) */
#define TASK_INVALID_INDEX         (uint8_t)(127u)  
/* Represents an invalid task execution number */
#define TASK_INVALID_EXECUTION_NUM (uint16_t)(0xFFFF)  

/* Boolean values indicating whether the interval has passed or not */
#define INTERVAL_PASSED            (bool)(true)  
#define INTERVAL_NOT_PASSED        (bool)(false)  

/* Delay for while loop, called in .ino file */
#define CYCLIC_DELAY_MS       (TIME_MSECS(50))  

/* Time intervals for each task */
#define PERIOD_REINIT              (TIME_SECS(5))  
#define PERIOD_CALIBRATION_MQ135   (TIME_SECS(3))  
#define PERIOD_CALIBRATION_MQ7     (TIME_SECS(3))  
#define PERIOD_I2C_SCANNING        (TIME_SECS(2))  
#define PERIOD_SHOW_TIME           (TIME_SECS(1))  
#define PERIOD_SENSOR_READ         (TIME_SECS(3))  

/* Enum representing various task identifiers */
typedef enum
{
    TASK_REINIT,               /* Task for reinitialization */
    TASK_I2C_SCANNING,         /* Task for scanning I2C devices */
    TASK_CALIBRATION_MQ135,    /* Task for calibrating MQ135 sensor */
    TASK_CALIBRATION_MQ7,      /* Task for calibrating MQ7 sensor */
    TASK_SHOW_TIME,            /* Task for displaying current time */
    TASK_SENSOR_READ           /* Task for reading sensor data */
} task_id_te;

/* Structure to hold task configuration data. */
typedef struct
{
  uint32_t task_period;              /* Period of the task in milliseconds */
  uint32_t time_elapsed;             /* Elapsed time since the task started */
  uint16_t task_execution_number;    /* Number of times the task has been executed */
  uint8_t task_id;                   /* Task identifier */
} tasks_config_ts;

/* Enum representing different states in the task state machine. */
typedef enum
{
    STATE_IDLE,                     /* Idle state, waiting for actions */
    STATE_I2C_SCANNING,             /* State for scanning I2C devices */
    STATE_CALIBRATING_MQ135,        /* State for calibrating MQ135 sensor */
    STATE_CALIBRATING_MQ7,          /* State for calibrating MQ7 sensor */
    STATE_MAIN_FUNCTION,            /* Main function state where tasks are executed */
    STATES_COUNT                    /* Total number of states in the state machine */
} task_state_machine_states_te;


/**
 * @brief Performs the cyclic task execution.
 *
 * This function is responsible for periodically executing the control loop in the system.
 * It retrieves the current time and passes it to the control loop for processing.
 *
 * @note This function is typically called at a regular interval to maintain consistent operation.
 */
void task_cyclicTask();

/**
 * @brief Main task that manages state transitions and executes the core logic for each state.
 *
 * This function transitions through different states in a finite state machine. Each state
 * is responsible for specific tasks, such as sensor calibration, I2C scanning, and sensor reading.
 * The state transitions are handled by the `getNextState` function, ensuring a clear flow of execution.
 */
void task_mainTask();

#endif