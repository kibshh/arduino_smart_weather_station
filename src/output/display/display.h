#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#include "display_config.h"
#include "../../error_manager/error_codes.h"
#include "../../input/sensors/sensors_interface/sensors_interface.h"
#include "../../input/functionality/calibration/calibration_interface/calibration_interface.h"
#include "../../data_router/data_router_types.h"
#include "../../input/input_types.h"

#define DISPLAY_START_COLUMN  (0u)  // Start column for display cursor
#define DISPLAY_START_ROW     (0u)  // Start row for display cursor

#define DISPLAY_PROCEED_WITH_DISPLAY      (true)  // Flag to proceed with displaying data
#define DISPLAY_DONT_PROCEED_WITH_DISPLAY (false) // Flag to prevent displaying data

#define DISPLAY_FIRST_LETTER_IN_STR   (0u)  // Index of the first letter in a string
#define DISPLAY_MIN_STRING_LEN        (0u)  // Minimum length of the string for display
#define DISPLAY_DTOSTRF_BUFFER_SIZE   (10u) // Size of the buffer for converting float to string
#define DISPLAY_NULL_TERMINATOR_SIZE  (1u)  // Size for the null terminator in strings

/**
 * @brief Initializes the LCD display module.
 *
 * This function sets up the LCD display with the specified dimensions,
 * configures its cursor and backlight settings, and ensures it is ready for use.
 *
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: Display initialized successfully.
 */
error_manager_error_code_te display_init();

/**
 * Displays the data based on the input type received from the data router.
 * This function checks the `input_type` of the provided data structure and 
 * calls the appropriate display function to show the corresponding data 
 * (such as sensor readings, RTC readings, I2C scan results, or calibration results) on the LCD.
 * 
 * @param data A structure containing the data to be displayed, with the input type 
 *             and the corresponding data based on that type.
 * 
 * @return error_manager_error_code_te Returns an error code:
 *         - ERROR_CODE_INVALID_INPUT_TYPE if the input type is not recognized or valid.
 *         - Any other error codes returned from the respective display functions 
 *           (e.g., for sensor, RTC, I2C scan, or calibration display).
 **/
error_manager_error_code_te display_displayData(data_router_data_ts data);

/**
 * Displays the sensor measurement data on the LCD based on the provided sensor reading.
 * It retrieves the metadata for the sensor, formats the sensor value, and updates the display. 
 * If the measurement type is invalid or metadata is not found, the function returns an error code.
 *
 * @param sensor_data The sensor reading containing the sensor ID, value, and measurement type switch.
 * 
 * @return error_manager_error_code_te Returns an error code based on the display process:
 *         - ERROR_CODE_NO_ERROR if the sensor data is successfully displayed.
 *         - ERROR_CODE_DISPLAY_INVALID_MEASUREMENT_TYPE if the measurement type is invalid.
 *         - ERROR_CODE_DISPLAY_SENSOR_NOT_CONFIGURED if the sensor metadata is not found.
 **/
error_manager_error_code_te display_displaySensorMeasurement(sensor_reading_ts sensor_data);

/** 
 * Displays the current time on the LCD, formatted to fit a 16-character wide display.
 * This function formats the time and date values from the RTC reading and displays it.
 *
 * @param time_data The RTC reading containing the current time (year, month, day, hour, minutes, seconds).
 * 
 * @return error_manager_error_code_te Returns an error code:
 *         - ERROR_CODE_NO_ERROR indicating successful execution.
 **/
error_manager_error_code_te display_displayTime(rtc_reading_ts time_data);

/** 
 * Displays the results of an I2C bus scan on the LCD. 
 * If scanning all devices, it will show a scanning message, otherwise it displays the status 
 * of the specific device address being scanned.
 *
 * @param i2c_scan_data The I2C scanning data that contains the device address and scan status.
 * 
 * @return error_manager_error_code_te Returns an error code:
 *         - ERROR_CODE_NO_ERROR for a successful display update.
 *         - ERROR_CODE_DISPLAY_UNKNOWN_I2C_DEVICE_STATUS for unknown device statuses.
 **/
error_manager_error_code_te display_displayI2cScan(i2cScan_reading_ts i2c_scan_data);

/**
 * Displays the calibration results on the LCD display based on the given calibration data.
 * The function retrieves the calibration metadata and displays the status of the calibration
 * process, which can include idle, in progress, or finished states. It handles both single 
 * and multiple measurement types, formatting the displayed value according to the calibration 
 * metadata (e.g., the number of decimals). The result is displayed on a specific row of the LCD.
 *
 * @param calibration_data The calibration data that includes calibration ID, status, and value.
 *
 * @return error_manager_error_code_te Returns an error code:
 *         - ERROR_CODE_NO_ERROR for a successful display update.
 *         - ERROR_CODE_DISPLAY_INVALID_CALIBRATION_STATUS for invalid calibration status.
 *         - ERROR_CODE_DISPLAY_INVALID_CALIBRATION_NUM_OF_MEASUREMENTS_TYPE_CONFIGURED 
 *           for invalid number of measurement types.
 *         - ERROR_CODE_CALIBRATION_CALIB_NOT_CONFIGURED if calibration metadata is not configured.
 **/
error_manager_error_code_te display_displayCalibrationResults(calibration_reading_ts calibration_data);

#endif