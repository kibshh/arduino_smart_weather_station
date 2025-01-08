#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include "custom/mq135.h"
#include "custom/mq7.h"
#include "custom/uv.h"
#include "custom/rainsensor.h"
#include "sensorsconfig.h"

#define SENSORS_NO_SENSORS_CONFIGURED    (0u)
#define SENSORS_NO_INDICATION_FUNCTION   (nullptr)
#define SENSORS_NO_VALUE_FUNCTION        (nullptr)

#define SENSORS_SEA_LEVEL_PRESSURE       (1013.25f)

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

typedef enum
{
  SENSORS_VALUE,
  SENSORS_INDICATION
}sensor_measurement_type_te;

typedef enum
{
  DHT11_TEMPERATURE = 0,
  DHT11_HUMIDITY,
  BMP280_PRESSURE,
  BMP280_TEMPERATURE,
  BMP280_ALTITUDE,
  BH1750_LUMINANCE,
  MQ135_PPM,
  MQ7_COPPM,
  GYML8511_UV,
  ARDUINORAIN_RAINING
}sensor_id_te;

typedef struct
{
  float value;
  sensor_measurement_type_te measurement_type_switch;
  boolean success;
  boolean indication;
}sensor_reading_t;

typedef float (*sensor_sensor_value_function_t)();
typedef bool (*sensor_sensor_indication_function_t)();

typedef struct
{
  float min_value;
  float max_value;
  sensor_id_te sensor_id;
  sensor_sensor_value_function_t sensor_value_function;
  sensor_sensor_indication_function_t sensor_indication_function;
}sensor_sensors_config_t;

extern DHT dht;
extern Adafruit_BMP280 bmp;
extern BH1750 lightMeter;;

void sensors_init();
sensor_reading_t sensors_getReading(sensor_id_te id, sensor_measurement_type_te measurement_type);

#endif