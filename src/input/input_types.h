#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

#include <Arduino.h>
#include "../error_manager/error_codes.h"
#include "functionality/calibration/calibration_types.h"

/**
 * Number of bits in a byte.
 */
#define BITS_IN_BYTE                     (8u)

/**
 * Flag indicating that but is set.
 */
#define BIT_SET                          (1u)

/**
 * Maximum number of devices that can be addressed in 7-bit I2C addressing.
 */
#define I2C_7_BIT_ADDRESSING_MAX_DEVICES (127u)

/**
 * I2C scan mode to detect all devices on the bus.
 * 
 * This value is used to indicate that the scan should check all possible 
 * I2C addresses (1 to 127) and populate the `addresses` field in the 
 * i2cScan_reading_ts structure with the results.
 */
#define I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES     (0u)

/**
 * Array size required to store the presence of I2C devices, one bit per device.
 * Each byte will store 8 device states (1 bit per device).
 */
#define I2C_SCAN_ARRAY_SIZE             ((I2C_7_BIT_ADDRESSING_MAX_DEVICES + BITS_IN_BYTE - 1) / BITS_IN_BYTE)

/* Transmission result codes for I2C communication: */
/* Successful transmission; no error occurred. */
#define I2CSCAN_TRANSMISSION_RESULT_SUCCESS   (0u)

/* Data too long to fit in transmit buffer */
#define I2CSCAN_TRANSMISSION_RESULT_TOOLONG   (1u)

/* Received NACK on transmit of the address */
#define I2CSCAN_TRANSMISSION_RESULT_NACKADR   (2u)

/* Received NACK on transmit of the data */
#define I2CSCAN_TRANSMISSION_RESULT_NACKDAT   (3u)

/* Unknown error occurred during communication 
 * (e.g., arbitration lost or bus issue). */
#define I2CSCAN_TRANSMISSION_RESULT_UNKNOWN   (4u)

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
}sensor_reading_ts;

/**
 * Structure containing the result of a sensor reading operation.
 * Members:
 *  - sensor_reading: Contains the actual data retrieved from the sensor.
 *  - error_code: Indicates whether the operation succeeded or provides an error code in case of failure.
 */
typedef struct
{
  sensor_reading_ts sensor_reading;
  error_manager_error_code_te error_code;
} sensor_return_ts;
/* ***************************************** */

/* CALIBRATION COMPONENT */
/**
 * Structure representing a single calibration reading.
 * Members:
 *  - value: The measured value from the calibration process.
 *  - calibration_id: ID of the calibration providing the measurement.
 *  - current_calibration_status: Status of the calibration (e.g., idle, active, completed).
 */
typedef struct
{
  float value;
  uint8_t calibration_id;
  calibration_state_te current_calibration_status;
} calibration_reading_ts;

/**
 * Structure containing the result of a calibration reading operation.
 * Members:
 *  - calibration_reading: Contains the actual data retrieved from the calibration process.
 *  - error_code: Indicates whether the operation succeeded or provides an error code in case of failure.
 */
typedef struct
{
  calibration_reading_ts calibration_reading;
  error_manager_error_code_te error_code;
} calibration_return_ts;
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
}rtc_reading_ts;

/**
 * Structure containing the result of an RTC operation.
 * Members:
 *  - rtc_reading: Contains the actual time data retrieved from the RTC.
 *  - error_code: Indicates whether the operation succeeded or provides an error code in case of failure.
 */
typedef struct
{
  rtc_reading_ts rtc_reading;
  error_manager_error_code_te error_code;
} rtc_return_ts;
/* ***************************************** */

/* I2C SCAN COMPONENT */
/**
 * Structure representing the result of an I2C scan operation.
 * 
 * Members:
 *  - addresses: A bit-array representing the detected I2C addresses. Each bit
 *               corresponds to an address, where 1 indicates a device is present.
 *               This field is populated when performing a scan for all devices.
 *  - single_device_status: Indicates the status of a specific device on the bus.
 *                          This field is updated when performing a single-device status check.
 *  - device_address: Specifies the type of scan to be performed:
 *                    - 0: Perform a scan for all devices and populate the `addresses` field.
 *                          This is because 0 is not a valid I2C address.
 *                    - 1–127: Perform a single-device status check for the specified address 
 *                              and update the `single_device_status` field.
 */
// Forward declaration of the structure
struct i2cScan_reading;

typedef struct i2cScan_reading
{
  uint8_t addresses[I2C_SCAN_ARRAY_SIZE];
  uint8_t single_device_status;
  uint8_t device_address;
  bool (*update_i2c_address)(struct i2cScan_reading *i2cScan_data);
  uint8_t current_i2c_addr;
} i2cScan_reading_ts;

/**
 * Structure containing the result of an I2C scan operation.
 * Members:
 *  - i2cScan_reading: Contains the data from the I2C scan.
 *  - error_code: Indicates whether the operation succeeded or provides an error code in case of failure.
 */
typedef struct 
{
  i2cScan_reading_ts i2cScan_reading;
  error_manager_error_code_te error_code;
} i2cScan_return_ts;
/* ***************************************** */

#endif