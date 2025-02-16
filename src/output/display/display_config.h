#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <Arduino.h>

/* Height of the LCD display (2 rows) */
#define DISPLAY_LCD_HEIGHT    (uint8_t)(2u)
/* Width of the LCD display (16 columns) */
#define DISPLAY_LCD_WIDTH     (uint8_t)(16u)
/* I2C address of the LCD display */
#define DISPLAY_LCD_I2C_ADDDR (uint8_t)(0x27)

/* Row for displaying sensor data */
#define DISPLAY_SENSORS_ROW          (uint8_t)(0u)
/* Row for displaying time */
#define DISPLAY_TIME_ROW             (uint8_t)(1u)
/* Row for displaying I2C scan string */
#define DISPLAY_I2C_SCAN_STRING_ROW  (uint8_t)(0u)
/* Row for displaying I2C address during scan */
#define DISPLAY_I2C_SCAN_ADDR_ROW    (uint8_t)(1u)

#endif