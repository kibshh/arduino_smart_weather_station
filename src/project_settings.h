#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <Arduino.h>


/**
 * Uncomment if serial monitor is used for debugging or logging.
 * Make sure the serial monitor is properly initialized and the baud rate is set correctly.
 */
#define OUTPUT_SERIAL_MONITOR_USED

/**
 * Uncomment if display is present(for example LCD1602).
 * Make sure that the display is properly connected.
 * Make sure to enable communication protocol which the display uses (I2C for example).
 */
#define OUTPUT_LCD_DISPLAY_USED

/**
 * Uncomment if at least one sensor from available sensors libraary is present and used.
 * Make sure that the sensor(s) is/are properly connected.
 */
#define INPUT_SENSORS_USED

/**
 * Uncomment if RTC is used(for example Arduino RTC).
 * Make sure that the RTC is properly connected and configured.
 */
#define INPUT_RTC_USED

#endif