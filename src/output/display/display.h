#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#include "display_config.h"
#include "../../data_router/data_router_types.h"

#define DISPLAY_START_COLUMN  (0u)  // Start column for display cursor
#define DISPLAY_START_ROW     (0u)  // Start row for display cursor

#define DISPLAY_PROCEED_WITH_DISPLAY      (true)  // Flag to proceed with displaying data
#define DISPLAY_DONT_PROCEED_WITH_DISPLAY (false) // Flag to prevent displaying data

#define DISPLAY_FIRST_LETTER_IN_STR   (0u)  // Index of the first letter in a string
#define DISPLAY_MIN_FLOAT_STRING_LEN  (1u)  // Minimum length of the float string for display
#define DISPLAY_DTOSTRF_BUFFER_SIZE   (20u) // Size of the buffer for converting float to string
#define DISPLAY_NULL_TERMINATOR_SIZE  (1u)  // Size for the null terminator in strings

#define DISPLAY_MAX_STRING_LEN        (DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE)

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
 * (such as sensor readings, RTC readings, I2C scan results) on the LCD.
 * 
 * @param data A structure containing the data to be displayed, with the input type 
 *             and the corresponding data based on that type.
 * 
 * @return error_manager_error_code_te Returns an error code:
 *         - ERROR_CODE_INVALID_INPUT_TYPE if the input type is not recognized or valid.
 *         - Any other error codes returned from the respective display functions 
 *           (e.g., for sensor, RTC, I2C scan).
 **/
error_manager_error_code_te display_displayData(data_router_data_ts data);

#endif