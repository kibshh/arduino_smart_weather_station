#ifndef TASK_H
#define TASK_H

#include <Arduino.h>
#include "../project_settings.h"
#include "../sensors/sensors.h"
#include "../functionality/i2cScan.h"
#include "../display/display.h"
#include "../data_router/data_router_common.h"

void task_cyclicTask();

#endif