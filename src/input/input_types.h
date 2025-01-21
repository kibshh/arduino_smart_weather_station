#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

#include <Arduino.h>
#include "../error_manager/error_codes.h"

/* SENSORS COMPONENT */
/**
 * Structure representing a single sensor reading.
 * Members:
 *  - value: The measured value from the sensor.
 *  - sensor_id: ID of the sensor providing the measurement.
 *  - measurement_type_switch: Identifier for the type of measurement (float value / indication).
 *  - indication: A flag for indication (for example raining / not raining).
 */
typedef struct
{
  float value;
  uint8_t sensor_id;
  uint8_t measurement_type_switch;
  bool indication;
}sensor_reading_t;

/**
 * Structure containing the result of a sensor reading operation.
 * Members:
 *  - sensor_reading: Contains the actual data retrieved from the sensor.
 *  - error_code: Indicates whether the operation succeeded or provides an error code in case of failure.
 */
typedef struct
{
  sensor_reading_t sensor_reading;
  error_manager_error_code_te error_code;
} sensor_return_t;
/* ***************************************** */

/* RTC COMPONENT */
/**
 * Structure representing a Real-Time Clock (RTC) reading.
 * Members:
 *  - year: The current year.
 *  - month: The current month (1–12).
 *  - day: The current day of the month (1–31).
 *  - hour: The current hour (0–23).
 *  - mins: The current minutes (0–59).
 *  - secs: The current seconds (0–59).
 */
typedef struct
{
  uint16_t year; 
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t mins;
  uint8_t secs;
}rtc_reading_t;

/**
 * Structure containing the result of an RTC operation.
 * Members:
 *  - rtc_reading: Contains the actual time data retrieved from the RTC.
 *  - error_code: Indicates whether the operation succeeded or provides an error code in case of failure.
 */
typedef struct
{
  rtc_reading_t rtc_reading;
  error_manager_error_code_te error_code;
} rtc_return_t;
/* ***************************************** */

#endif