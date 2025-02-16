#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#include "display_config.h"
#include "../../control/control_types.h"

/* Start column for display cursor */
#define DISPLAY_START_COLUMN  (0u)
/* Start row for display cursor */
#define DISPLAY_START_ROW     (0u)

/* Flag to proceed with displaying data */
#define DISPLAY_PROCEED_WITH_DISPLAY      (true)
/* Flag to prevent displaying data */
#define DISPLAY_DONT_PROCEED_WITH_DISPLAY (false)

/* Minimum length of the float string for display */
#define DISPLAY_MIN_FLOAT_STRING_LEN  (1u)
/* Size of the buffer for converting float to string */
#define DISPLAY_DTOSTRF_BUFFER_SIZE   (20u)
/* Size for the null terminator in strings */
#define DISPLAY_NULL_TERMINATOR_SIZE  (1u)
/** Defines the maximum string length for the display, including the null terminator. */
#define DISPLAY_MAX_STRING_LEN        (DISPLAY_LCD_WIDTH + DISPLAY_NULL_TERMINATOR_SIZE)

/**
 * @brief Initializes the LCD display module.
 *
 * This function sets up the LCD display with the specified dimensions,
 * configures its cursor and backlight settings, and ensures it is ready for use.
 *
 * @return control_error_code_te
 * - ERROR_CODE_NO_ERROR: Display initialized successfully.
 */
control_error_code_te display_init();

/**
 * Displays the data based on the input type received from the data router.
 * This function checks the `input_type` of the provided data structure and 
 * calls the appropriate display function to show the corresponding data 
 * (such as sensor readings, RTC readings, I2C scan results) on the LCD.
 * 
 * @param data A structure containing the data to be displayed, with the input type 
 *             and the corresponding data based on that type.
 * 
 * @return control_error_code_te Returns an error code:
 *         - ERROR_CODE_INVALID_INPUT_TYPE if the input type is not recognized or valid.
 *         - Any other error codes returned from the respective display functions 
 *           (e.g., for sensor, RTC, I2C scan).
 **/
control_error_code_te display_displayData(control_data_ts data);

#endif