#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "../project_settings.h"
#include "../data_router/data_router.h"

#define TASK_STARTING_SENSOR_INDEX         (0u)

void task_initTask();
void task_cyclicTask();

#endif