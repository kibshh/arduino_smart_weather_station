#ifndef ARDUINO_RAIN_SENSOR_H
#define ARDUINO_RAIN_SENSOR_H

#include <Arduino.h>
#include "../sensorsconfig.h"

/* Define the digital output value indicating rain detected by the sensor, used in digital read mode */
/* NOTE: The sensor uses reverse logic — 0 means rain is detected, and 1 means no rain */
#define ARDUINO_RAIN_SENSOR_RAIN_DETECTED    (0)

/* Define the threshold value for analog readings; values below this indicate rain */
#define ARDUINO_RAIN_SENSOR_ANALOG_THRESHOLD (500u)

/**
 * @brief Initializes the rain sensor.
 * Depending on the configuration, sets the pin mode for analog or digital input.
 */
void arduino_rain_sensor_init();

/**
 * @brief Reads the rain sensor status.
 * Delegates the reading to either the analog or digital sensor reading function.
 * @return true if rain is detected, false otherwise.
 */
bool arduino_rain_sensor_readRaining();

#endif