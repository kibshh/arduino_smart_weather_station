#ifndef BH1750_H
#define BH1750_H

#include <Arduino.h>
#include <BH1750.h>
#include "../sensors_config.h"

/**
 * @brief Initializes the BH1750 light sensor.
 *
 * This function initializes the BH1750 sensor by calling its `begin()` method.
 * It ensures that the sensor is ready to be used. If the initialization fails,
 * the function returns false, indicating an error.
 *
 * @return true if the sensor is successfully initialized, false otherwise.
 */
bool bh1750_init();

/**
 * @brief Reads the light level from the BH1750 sensor.
 *
 * This function retrieves the current light level in lux from the BH1750 sensor.
 * The value returned is a float, representing the light intensity measured by the sensor.
 *
 * @return The light level in lux as a float.
 */
float bh1750_readLightLevel();

#endif