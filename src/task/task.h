#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "../app_layer/app.h"

#define MS_PER_SECOND              ((uint32_t)(1000u))
#define MS_PER_MINUTE              ((uint32_t)(60u * MS_PER_SECOND))
#define MS_PER_HOUR                ((uint32_t)(60u * MS_PER_MINUTE))

#define TIME_HOURS(h)              ((uint32_t)((h) * MS_PER_HOUR))
#define TIME_MINS(m)               ((uint32_t)((m) * MS_PER_MINUTE))
#define TIME_SECS(s)               ((uint32_t)((s) * MS_PER_SECOND))
#define TIME_MSECS(m)              ((uint32_t)(m))

#define TASK_NO_TASKS              (size_t)(0u)
#define TASK_FIRST_TASK_INDEX      (uint8_t)(0u)
#define TASK_INVALID_INDEX         (uint8_t)(127u)

#define INTERVAL_PASSED            (true)
#define INTERVAL_NOT_PASSED        (false)

#define CYCLIC_TASK_DELAY_MS       (TIME_MSECS(50))

#define PERIOD_REINIT              (TIME_SECS(5))
#define PERIOD_CALIBRATION         (TIME_SECS(3))
#define PERIOD_I2C_SCANNING        (TIME_SECS(2))
#define PERIOD_SHOW_TIME           (TIME_SECS(1))
#define PERIOD_SENSOR_READ         (TIME_SECS(3))

typedef enum
{
    TASK_REINIT,
    TASK_CALIBRATION,
    TASK_I2C_SCANNING,
    TASK_SHOW_TIME,
    TASK_SENSOR_READ
} task_id_te;

typedef struct
{
  uint32_t task_period;
  uint32_t time_elapsed;
  uint8_t task_id;
} tasks_config_ts;

typedef enum
{
    STATE_IDLE,
    STATE_I2C_SCANNING,
    STATE_CALIBRATING_MQ135,
    STATE_CALIBRATING_MQ7,
    STATE_MAIN_FUNCTION
} task_state_machine_states_te;

void task_cyclicTask();
void task_mainTask();

#endif