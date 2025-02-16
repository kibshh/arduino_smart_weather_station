#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <Arduino.h>

/* Height of the LCD display (2 rows) */
#define DISPLAY_LCD_HEIGHT    (2u)
/* Width of the LCD display (16 columns) */
#define DISPLAY_LCD_WIDTH     (16u)
/* I2C address of the LCD display */
#define DISPLAY_LCD_I2C_ADDDR (0x27)

/* Row for displaying sensor data */
#define DISPLAY_SENSORS_ROW   (0u)
/* Row for displaying time */
#define DISPLAY_TIME_ROW      (1u)
/* Row for displaying I2C scan string */
#define DISPLAY_I2C_SCAN_STRING_ROW  (0u)
/* Row for displaying I2C address during scan */
#define DISPLAY_I2C_SCAN_ADDR_ROW    (1u)

#endif