#ifndef BMP280_H
#define BMP280_H

#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include "../../sensorsconfig.h"


#define BMP280_MODE_NORMAL    Adafruit_BMP280::MODE_NORMAL //The sensor continuously takes measurements based on the configured sampling and standby time.
#define BMP280_MODE_SLEEP     Adafruit_BMP280::MODE_SLEEP  //The sensor is in sleep mode, consuming minimal power and not taking any measurements.
#define BMP280_MODE_FORCED    Adafruit_BMP280::MODE_FORCED //The sensor takes a single measurement and then returns to sleep mode.

// Higher sampling means higher resolution at the cost of slower measurements and higher power consumption.
#define BMP280_SAMPLING_NONE  Adafruit_BMP280::SAMPLING_NONE
#define BMP280_SAMPLING_X1    Adafruit_BMP280::SAMPLING_X1
#define BMP280_SAMPLING_X2    Adafruit_BMP280::SAMPLING_X2
#define BMP280_SAMPLING_X4    Adafruit_BMP280::SAMPLING_X4
#define BMP280_SAMPLING_X8    Adafruit_BMP280::SAMPLING_X8
#define BMP280_SAMPLING_X16   Adafruit_BMP280::SAMPLING_X16

//Filtering is used to reduce noise but it introduces a slight delay in readings.
#define BMP280_FILTER_OFF     Adafruit_BMP280::FILTER_OFF
#define BMP280_FILTER_X2      Adafruit_BMP280::FILTER_X2
#define BMP280_FILTER_X4      Adafruit_BMP280::FILTER_X4
#define BMP280_FILTER_X8      Adafruit_BMP280::FILTER_X8
#define BMP280_FILTER_X16     Adafruit_BMP280::FILTER_X16

//The amount of time the sensor waits between measurements
#define BMP280_WAIT_MS_0_5    Adafruit_BMP280::STANDBY_MS_0_5
#define BMP280_WAIT_MS_62_5   Adafruit_BMP280::STANDBY_MS_62_5
#define BMP280_WAIT_MS_125    Adafruit_BMP280::STANDBY_MS_125
#define BMP280_WAIT_MS_250    Adafruit_BMP280::STANDBY_MS_250
#define BMP280_WAIT_MS_500    Adafruit_BMP280::STANDBY_MS_500
#define BMP280_WAIT_MS_1000   Adafruit_BMP280::STANDBY_MS_1000
#define BMP280_WAIT_MS_2000   Adafruit_BMP280::STANDBY_MS_2000
#define BMP280_WAIT_MS_4000   Adafruit_BMP280::STANDBY_MS_4000

/**
 * @brief Initializes the BMP280 sensor.
 *
 * This function initializes the BMP280 sensor by attempting to start the sensor
 * with the specified I2C address. If the initialization is successful, it
 * configures the sensor's sampling settings, including operating mode, oversampling,
 * filtering, and standby time. If any initialization step fails, the function
 * returns false.
 *
 * @return true if the sensor is successfully initialized, false otherwise.
 */
boolean bmp280_init();

/**
 * @brief Reads the current temperature from the BMP280 sensor.
 *
 * This function retrieves the current temperature from the BMP280 sensor.
 * The temperature is measured in degrees Celsius.
 *
 * @return The current temperature in degrees Celsius.
 */
float bmp280_readTemperature();

/**
 * @brief Reads the current atmospheric pressure from the BMP280 sensor.
 *
 * This function retrieves the current atmospheric pressure from the BMP280 sensor.
 * The pressure is returned in Pascals (Pa).
 *
 * @return The current atmospheric pressure in Pascals.
 */
float bmp280_readPressure();

/**
 * @brief Reads the current altitude from the BMP280 sensor.
 *
 * This function calculates and returns the current altitude based on the pressure
 * measurement and a given sea-level pressure value. The altitude is calculated
 * using the barometric formula.
 *
 * @return The calculated altitude in meters.
 */
float bmp280_readAltitude();

#endif