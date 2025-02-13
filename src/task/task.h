#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "../app_layer/app.h"

#define MS_PER_SECOND   ((uint32_t)1000u)
#define MS_PER_MINUTE   (60u * MS_PER_SECOND)
#define MS_PER_HOUR     (60u * MS_PER_MINUTE)

#define TIME_HOURS(h)   ((h) * MS_PER_HOUR)
#define TIME_MINS(m)    ((m) * MS_PER_MINUTE)
#define TIME_SECS(s)    ((s) * MS_PER_SECOND)

#define TASK_CALIBRATING_TIMER     (TIME_SECS(1))
#define TASK_TIME_READ_TIMER       (TIME_SECS(1))
#define TASK_SENSOR_READ_TIMER     (TIME_SECS(2))
#define TASK_I2C_ADDR_READ_TIMER   (TIME_SECS(2))

#define TASK_CALIBRATING           (0u)
#define TASK_TIME_READ             (1u)
#define TASK_SENSOR_READ           (2u)
#define TASK_I2C_ADDR_READ         (3u)

#define CYCLIC_TASK_DELAY_MS       ((uint32_t)50u)

#define TASK_NO_TASKS              (0u)
#define TASK_FIRST_TASK_INDEX      (0u)
#define TASK_INVALID_INDEX         (127u)

#define INTERVAL_PASSED            (true)
#define INTERVAL_NOT_PASSED        (false)

typedef enum
{
    STATE_SCANNING_FOR_I2C_ADDRESSES,
    STATE_CYCLIC_SENSOR_AND_TIME_READING
} task_state_machine_te;

typedef struct
{
    uint32_t previous_millis;
    uint32_t task_period;
    uint8_t task_id;
} tasks_config_ts;

void task_initTask();
void task_cyclicTask();

#endif