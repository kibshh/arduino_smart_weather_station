#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include "../../project_configs/project_configs.h"
#include <Adafruit_BMP280.h>

#define BMP280_SENSOR_I2C_ADDDR         (uint8_t)(0x76)

/* Working modes of BMP280 sensor */
#define BMP280_SENSOR_MODE_NORMAL    Adafruit_BMP280::MODE_NORMAL /* The sensor continuously takes measurements based on the configured sampling and standby time. */
#define BMP280_SENSOR_MODE_SLEEP     Adafruit_BMP280::MODE_SLEEP  /* The sensor is in sleep mode, consuming minimal power and not taking any measurements. */
#define BMP280_SENSOR_MODE_FORCED    Adafruit_BMP280::MODE_FORCED /* The sensor takes a single measurement and then returns to sleep mode. */

/* Higher sampling means higher resolution at the cost of slower measurements and higher power consumption. */
#define BMP280_SENSOR_SAMPLING_NONE  Adafruit_BMP280::SAMPLING_NONE
#define BMP280_SENSOR_SAMPLING_X1    Adafruit_BMP280::SAMPLING_X1
#define BMP280_SENSOR_SAMPLING_X2    Adafruit_BMP280::SAMPLING_X2
#define BMP280_SENSOR_SAMPLING_X4    Adafruit_BMP280::SAMPLING_X4
#define BMP280_SENSOR_SAMPLING_X8    Adafruit_BMP280::SAMPLING_X8
#define BMP280_SENSOR_SAMPLING_X16   Adafruit_BMP280::SAMPLING_X16

/* Filtering is used to reduce noise but it introduces a slight delay in readings. */
#define BMP280_SENSOR_FILTER_OFF     Adafruit_BMP280::FILTER_OFF
#define BMP280_SENSOR_FILTER_X2      Adafruit_BMP280::FILTER_X2
#define BMP280_SENSOR_FILTER_X4      Adafruit_BMP280::FILTER_X4
#define BMP280_SENSOR_FILTER_X8      Adafruit_BMP280::FILTER_X8
#define BMP280_SENSOR_FILTER_X16     Adafruit_BMP280::FILTER_X16

/* The amount of time the sensor waits between measurements */
#define BMP280_SENSOR_WAIT_MS_0_5    Adafruit_BMP280::STANDBY_MS_0_5
#define BMP280_SENSOR_WAIT_MS_62_5   Adafruit_BMP280::STANDBY_MS_62_5
#define BMP280_SENSOR_WAIT_MS_125    Adafruit_BMP280::STANDBY_MS_125
#define BMP280_SENSOR_WAIT_MS_250    Adafruit_BMP280::STANDBY_MS_250
#define BMP280_SENSOR_WAIT_MS_500    Adafruit_BMP280::STANDBY_MS_500
#define BMP280_SENSOR_WAIT_MS_1000   Adafruit_BMP280::STANDBY_MS_1000
#define BMP280_SENSOR_WAIT_MS_2000   Adafruit_BMP280::STANDBY_MS_2000
#define BMP280_SENSOR_WAIT_MS_4000   Adafruit_BMP280::STANDBY_MS_4000

/* Min and Max values */
#define BMP280_SENSOR_PRESSURE_MIN     (float)(300.0f)
#define BMP280_SENSOR_PRESSURE_MAX     (float)(1200.0f)
#define BMP280_SENSOR_TEMPERATURE_MIN  (float)(-20.0f)
#define BMP280_SENSOR_TEMPERATURE_MAX  (float)(50.0f)
#define BMP280_SENSOR_ALTITUDE_MAX     (float)(9000.0f)
#define BMP280_SENSOR_ALTITUDE_MIN     (float)(-1000.0f)

/* Divide constant to convert pressure from pascals to hectopascals */
#define BMP280_SENSOR_HPA_DIVIDER              (float)(100.0f)

/* Pressure at the sea level height */
#define BMP280_SENSOR_SEA_LEVEL_PRESSURE       (float)(1015.5f) /* Local sea level pressure for Novi Sad, Serbia */

bool Bmp280Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index);

#endif