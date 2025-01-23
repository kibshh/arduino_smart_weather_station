#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include "../../sensorsconfig.h"


/**
 * @brief Initializes the DHT11 sensor.
 * 
 * Prepares the DHT11 sensor for reading temperature and humidity.
 */
void dht11_init();

/**
 * @brief Reads the temperature from the DHT11 sensor.
 * 
 * @return float Temperature in Celsius.
 */
float dht11_readTemperature();

/**
 * @brief Reads the humidity from the DHT11 sensor.
 * 
 * @return float Humidity as a percentage.
 */
float dht11_readHumidity();

#endif