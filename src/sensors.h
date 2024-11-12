#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include "mq135.h"
#include "mq7.h"
#include "uv.h"
#include "rainsensor.h"

#define SENSORS_SEA_LEVEL_PRESSURE       (1013.25f)

#define SENSORS_TEMPERATURE_MIN  (-20)
#define SENSORS_TEMPERATURE_MAX  (50)
#define SENSORS_HUMIDITY_MIN     (0)
#define SENSORS_HUMIDITY_MAX     (100)
#define SENSORS_PRESSURE_MAX     (1200)
#define SENSORS_PRESSURE_MIN     (300)
#define SENSORS_ALTITUDE_MAX     (9000)
#define SENSORS_ALTITUDE_MIN     (-1000)
#define SENSORS_LUMINANCE_MAX    (150000)
#define SENSORS_LUMINANCE_MIN    (0)

#define SENSORS_PIN_DHT          (2u)

#define SENSORS_DHTTYPE          DHT11

#define SENSORS_BMP280_I2C_ADDDR (0x76)

#define SENSORS_BMP280_MODE_NORMAL    Adafruit_BMP280::MODE_NORMAL //The sensor continuously takes measurements based on the configured sampling and standby time.
#define SENSORS_BMP280_MODE_SLEEP     Adafruit_BMP280::MODE_SLEEP  //The sensor is in sleep mode, consuming minimal power and not taking any measurements.
#define SENSORS_BMP280_MODE_FORCED    Adafruit_BMP280::MODE_FORCED //The sensor takes a single measurement and then returns to sleep mode.

// Higher sampling means higher resolution at the cost of slower measurements and higher power consumption.
#define SENSORS_BMP280_SAMPLING_NONE  Adafruit_BMP280::SAMPLING_NONE
#define SENSORS_BMP280_SAMPLING_X1    Adafruit_BMP280::SAMPLING_X1
#define SENSORS_BMP280_SAMPLING_X2    Adafruit_BMP280::SAMPLING_X2
#define SENSORS_BMP280_SAMPLING_X4    Adafruit_BMP280::SAMPLING_X4
#define SENSORS_BMP280_SAMPLING_X8    Adafruit_BMP280::SAMPLING_X8
#define SENSORS_BMP280_SAMPLING_X16   Adafruit_BMP280::SAMPLING_X16

//Filtering is used to reduce noise but it introduces a slight delay in readings.
#define SENSORS_BMP280_FILTER_OFF     Adafruit_BMP280::FILTER_OFF
#define SENSORS_BMP280_FILTER_X2      Adafruit_BMP280::FILTER_X2
#define SENSORS_BMP280_FILTER_X4      Adafruit_BMP280::FILTER_X4
#define SENSORS_BMP280_FILTER_X8      Adafruit_BMP280::FILTER_X8
#define SENSORS_BMP280_FILTER_X16     Adafruit_BMP280::FILTER_X16

//The amount of time the sensor waits between measurements
#define SENSORS_BMP280_WAIT_MS_0_5    Adafruit_BMP280::STANDBY_MS_0_5
#define SENSORS_BMP280_WAIT_MS_62_5   Adafruit_BMP280::STANDBY_MS_62_5
#define SENSORS_BMP280_WAIT_MS_125    Adafruit_BMP280::STANDBY_MS_125
#define SENSORS_BMP280_WAIT_MS_250    Adafruit_BMP280::STANDBY_MS_250
#define SENSORS_BMP280_WAIT_MS_500    Adafruit_BMP280::STANDBY_MS_500
#define SENSORS_BMP280_WAIT_MS_1000   Adafruit_BMP280::STANDBY_MS_1000
#define SENSORS_BMP280_WAIT_MS_2000   Adafruit_BMP280::STANDBY_MS_2000
#define SENSORS_BMP280_WAIT_MS_4000   Adafruit_BMP280::STANDBY_MS_4000

#define SENSORS_BH1750_I2C_ADDDR_VCC (0x5C)
#define SENSORS_BH1750_I2C_ADDDR_GND (0x23)


typedef struct
{
  boolean success;
  float value;
}sensor_reading_t;

typedef struct
{
  boolean success;
  boolean value;
}sensor_reading_indicator_type_t;

extern DHT dht;
extern Adafruit_BMP280 bmp;
extern BH1750 lightMeter;;

void sensors_init();
sensor_reading_t sensors_getTemperature();
sensor_reading_t sensors_getHumidity();
sensor_reading_t sensors_getPressure();
sensor_reading_t sensors_getTemperatureBMP();
sensor_reading_t sensors_getAltitude();
sensor_reading_t sensors_getLuminance();
sensor_reading_t sensors_getVariousGasesPPM();
sensor_reading_t sensors_getCOPPM();
sensor_reading_t sensors_getUvIntensity();
sensor_reading_indicator_type_t sensors_getRainingStatus();

#endif