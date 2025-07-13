#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include "../../project_configs/project_configs.h"

/* I2C addresses based on wiring */
#define BH1750_SENSOR_I2C_ADDR_VCC      (uint8_t)(0x5C)
#define BH1750_SENSOR_I2C_ADDR_GND      (uint8_t)(0x23)

/* Min and Max values */
#define BH1750_SENSOR_LUMINANCE_MAX     (float)(150000.0f)
#define BH1750_SENSOR_LUMINANCE_MIN     (float)(0.0f)

bool Bh1750Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index);

#endif