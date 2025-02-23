#ifndef CONTROL_ERROR_CODES_H
#define CONTROL_ERROR_CODES_H

#include <Arduino.h>

/**
 * @brief Enumerates error codes for system components.
 *
 * Defines various error codes related to sensor operations, 
 * input/output validation, RTC, I2C communication, and display handling. 
 * Used for error detection and debugging throughout the system.
 */
typedef enum
{
  /* Generic */
  ERROR_CODE_NO_ERROR,
  ERROR_CODE_INVALID_INPUT,
  ERROR_CODE_INVALID_OUTPUT,
  ERROR_CODE_INVALID_INPUT_TYPE,
  /* ********************************* */

  /* Sensors related */
  ERROR_CODE_SENSOR_NOT_CONFIGURED,
  ERROR_CODE_INVALID_SENSOR_MEASUREMENT_TYPE,
  ERROR_CODE_NO_SENSORS_CONFIGURED,
  ERROR_CODE_SENSOR_NOT_FOUND,
  ERROR_CODE_SENSORS_MEASUREMENT_TYPE_MISSING_FUNCTION,
  ERROR_CODE_INVALID_VALUE_FROM_SENSOR,
  ERROR_CODE_ABNORMAL_VALUE_FROM_SENSOR,
  /* ********************************* */

  /* RTC related */
  ERROR_CODE_RTC_NOT_FOUND,
  /* ********************************* */

  /* I2C scan related */
  ERROR_CODE_I2C_SCAN_SCANNING_NOT_FINISHED,
  ERROR_CODE_I2C_SCAN_INVALID_ADDRESS_PARAMETER,
  ERROR_CODE_I2C_SCAN_ERROR_READING_DEVICE_STATUS,
  ERROR_CODE_UNKNOWN_I2C_DEVICE_STATUS,
  /* ********************************* */

  /* Init related */
  ERROR_CODE_INIT_FAILED,
  /* ********************************* */
} control_error_code_te;

#endif