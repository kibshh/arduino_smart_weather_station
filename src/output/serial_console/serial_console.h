#ifndef SERIAL_CONSOLE_H
#define SERIAL_CONSOLE_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <WString.h>
#include "../../data_router/data_router_types.h"
#include "serial_console_config.h"
#include "../../input/input_types.h"
#include "../../input/sensors/sensors_interface/sensors_interface.h"
#include "../../input/functionality/calibration/calibration_interface/calibration_interface.h"

#define SERIAL_CONSOLE_PROCEED_WITH_DISPLAY      (true)  // Flag to proceed with displaying data
#define SERIAL_CONSOLE_DONT_PROCEED_WITH_DISPLAY (false) // Flag to prevent displaying data

#define SERIAL_CONSOLE_DTOSTRF_BUFFER_SIZE   (20u) // Size of the buffer for converting float to string
#define SERIAL_CONSOLE_NULL_TERMINATOR_SIZE  (1u)  // Size for the null terminator in strings
#define SERIAL_CONSOLE_MIN_FLOAT_STRING_LEN  (1u)  // Minimal size of float string for serial console

#define SERIAL_CONSOLE_HEX_ADDR_STRING_LEN   (3u)  // Len of formated hex address

// Predefined buffer sizes for string storage in serial console output
#define SERIAL_CONSOLE_STRING_RESERVED_MINI          (10u)  // Suitable for short strings like status codes or flags
#define SERIAL_CONSOLE_STRING_RESERVED_SMALL         (20u)  // Suitable for short messages or small numerical values
#define SERIAL_CONSOLE_STRING_RESERVED_MEDIUM        (40u)  // Suitable for longer messages or formatted data
#define SERIAL_CONSOLE_STRING_RESERVED_LARGE         (60u)  // Suitable for longer messages or formatted data
#define SERIAL_CONSOLE_STRING_RESERVED_GIANT         (100u) // Suitable for multi-field messages or debugging output
#define SERIAL_CONSOLE_STRING_RESERVED_ENORMOUS      (200u) // Very large messages or extensive debugging output, not recommended for RAM saving

#define SERIAL_CONSOLE_TWO_CIPHER_NUMBER     (10u) // Defines the threshold value below which a leading zero is added for formatting.

/**
 * @brief Initializes the serial console communication.
 *
 * This function begins serial communication at the specified baud rate
 * and does not block the program execution if the serial port is not available.
 *
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Serial communication initialized successfully.
 * - ERROR_CODE_SERIAL_INIT_FAILED: Failed to initialize the serial communication.
 */
error_manager_error_code_te serial_console_init();

/**
 * @brief Displays data on the serial console based on input type.
 *
 * This function handles data routing and invokes specific display functions
 * based on the type of input provided (e.g., sensor data, RTC time, or I2C scan results).
 *
 * @param data The data structure containing the input type and associated readings.
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Data displayed successfully.
 * - ERROR_CODE_INVALID_INPUT_TYPE: Invalid input type specified.
 */
error_manager_error_code_te serial_console_displayData(data_router_data_ts data);

/**
 * @brief Displays sensor measurements on the serial console.
 *
 * This function retrieves sensor metadata and formats the sensor readings
 * for display on the serial console, including the sensor type, value, and unit.
 *
 * @param sensor_data The sensor reading structure containing the data to be displayed.
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Sensor data displayed successfully.
 * - ERROR_CODE_SERIAL_CONSOLE_SENSOR_NOT_CONFIGURED: Sensor metadata retrieval failed.
 * - ERROR_CODE_SERIAL_CONSOLE_INVALID_MEASUREMENT_TYPE: Invalid measurement type.
 */
error_manager_error_code_te serial_console_displaySensorMeasurement(sensor_reading_ts sensor_data);

/**
 * @brief Displays the current RTC time on the serial console.
 *
 * This function formats and displays the current time data, including
 * hours, minutes, seconds, day, month, and year.
 *
 * @param time_data The RTC reading structure containing the time information.
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Time data displayed successfully.
 */
error_manager_error_code_te serial_console_displayTime(rtc_reading_ts time_data);

/**
 * @brief Displays I2C scan results on the serial console.
 *
 * This function formats and displays I2C scan results, including the
 * detected device address and its communication status.
 *
 * @param i2c_scan_data The I2C scan reading structure containing the scan results.
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: I2C data displayed successfully.
 * - ERROR_CODE_SERIAL_CONSOLE_UNKNOWN_I2C_DEVICE_STATUS: Unknown device status during communication.
 */
error_manager_error_code_te serial_console_displayI2cScan(i2cScan_reading_ts i2c_scan_data);

/**
 * @brief Displays calibration results on the serial console.
 *
 * This function retrieves calibration metadata, processes calibration data, 
 * and formats a display message. It supports both single and multiple measurement 
 * calibrations, handling various calibration states accordingly.
 *
 * @param calibration_data The structure containing calibration measurement results.
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Calibration data displayed successfully.
 * - ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS: Invalid calibration status encountered.
 * - ERROR_CODE_DISPLAY_INVALID_CALIBRATION_NUM_OF_MEASUREMENTS_TYPE_CONFIGURED: Invalid measurement type configuration.
 * - ERROR_CODE_CALIBRATION_CALIB_NOT_CONFIGURED: Calibration metadata retrieval failed.
 */
error_manager_error_code_te serial_console_displayCalibrationResults(calibration_reading_ts calibration_data);

#endif