#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#include "../../error_manager/error_codes.h"
#include "../../input/sensors/sensors_interface/sensors_interface.h"
#include "../../data_router/data_router_types.h"
#include "../../input/input_types.h"

#define DISPLAY_LCD_HEIGHT    (2u)
#define DISPLAY_LCD_WIDTH     (16u)
#define DISPLAY_LCD_I2C_ADDDR (0x27)

#define DISPLAY_START_COLUMN  (0u)
#define DISPLAY_START_ROW     (0u)
#define DISPLAY_SENSORS_ROW   (0u)
#define DISPLAY_TIME_ROW      (1u)
#define DISPLAY_I2C_SCAN_STRING_ROW  (0u)
#define DISPLAY_I2C_SCAN_ADDR_ROW    (1u)

#define DISPLAY_DATETIME_FORMATER_LEN (3u)

#define DISPLAY_TWO_CIPHER_NUMBER     (10u)

#define DISPLAY_FIRST_LETTER_IN_STR   (0u)

#define DISPLAY_DISPLAY_INTERVAL_MS        (2000)  //2 seconds
#define DISPLAY_TIME_DISPLAY_INTERVAL_MS   (30000) //30 seconds

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
error_manager_error_code_te display_displayData(data_router_data_ts data);
error_manager_error_code_te display_displaySensorMeasurement(sensor_reading_ts sensor_data);
error_manager_error_code_te display_displayTime(rtc_reading_ts time_data);
error_manager_error_code_te display_displayI2cScan(i2cScan_reading_ts i2c_scan_data);

#endif