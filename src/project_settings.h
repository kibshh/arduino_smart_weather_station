#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <Arduino.h>

/**
 * Uncomment to enable I2C scan mode.
 * When defined, application scans all available I2C addresses and prints them to the console.
 */
// #define MODE_GET_I2C_ADDR

/**
 * Uncomment to enable Serial communication with the computer console.
 * When defined, debug and other messages are printed to the Serial Monitor.
 * Make sure that USB communication with the PC is physically established.
 */
#define DEBUG_USED

/**
 * Uncomment if USB communication for the device is present.
 * Make sure that the necessary USB drivers and connections are properly set up.
 */
// #define USB_USED

/**
 * Uncomment if UART (serial) communication for the device is present.
 * Make sure that the UART pins are properly connected and configured.
 */
// #define UART_USED

/**
 * Uncomment if I2C communication for the device is present.
 * Make sure that the I2C pins are properly connected and configured.
 */
#define I2C_USED

/**
 * Uncomment if display is present(for example LCD1602).
 * Make sure that the display is properly connected.
 * Make sure to enable communication protocol which the display uses (I2C for example).
 */
#define DISPLAY_USED

/**
 * Uncomment if at least one sensor from available sensors libraary is present and used.
 * Make sure that the sensor(s) is/are properly connected.
 */
#define SENSORS_USED

/**
 * Uncomment if RTC is used(for example Arduino RTC).
 * Make sure that the RTC is properly connected and configured.
 */
#define RTC_USED

/**
 * Defines the baud rate for serial communication - UART or USB cable.
 * Set the same baud rate on your device or change it here to accomodate it.
 */
#define BAUDRATE (9600u)

#endif