#ifndef ARDUINO_RAIN_SENSOR_H
#define ARDUINO_RAIN_SENSOR_H\

#include "../../project_configs/project_configs.h"

/* Measurement mode - select based on wiring */
#define ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE_ANALOG     (uint8_t)(0u)
#define ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE_DIGITAL    (uint8_t)(1u)
#define ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE            ARDUINO_RAIN_SENSOR_MEASUREMENT_MODE_ANALOG

/* Pin connected to sensor */
#define ARDUINO_RAIN_SENSOR_PIN_ANALOG                  (uint8_t)(A4)
#define ARDUINO_RAIN_SENSOR_PIN_DIGITAL                 (uint8_t)(4u)

/* Threshold value in case of analog measurement */
#define ARDUINO_RAIN_SENSOR_ANALOG_THRESHOLD            (uint16_t)(500u)

bool ArduinoRainSensor_Init();
bool ArduinoRainSensor_ReadRainingStatus();

#endif