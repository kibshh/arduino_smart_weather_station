#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <Arduino.h>

/* OUTPUT HARDWARE COMPONENTS */
/**
 * Uncomment if serial monitor is used for debugging or logging.
 * Make sure the serial monitor is properly initialized and the baud rate is set correctly.
 */
#define SERIAL_CONSOLE_COMPONENT            (uint8_t)(0u)

/**
 * Uncomment if display is present(for example LCD1602).
 * Make sure that the display is properly connected.
 * Make sure to enable communication protocol which the display uses (I2C for example).
 */
#define LCD_DISPLAY_COMPONENT               (uint8_t)(1u)
/* ********************************* */

/* INPUT HARDWARE COMPONENTS */
/* SENSOR COMPONENTS - Comment out if sensor is not used */
#define DHT11_COMPONENT                     (uint8_t)(0u)    
#define BMP280_COMPONENT                    (uint8_t)(1u)
#define BH1750_COMPONENT                    (uint8_t)(2u)
#define MQ135_COMPONENT                     (uint8_t)(3u)
#define MQ7_COMPONENT                       (uint8_t)(4u)
#define GYML8511_COMPONENT                  (uint8_t)(5u)
#define ARDUINORAIN_COMPONENT               (uint8_t)(6u)
/* ********************************* */

/* OTHER INPUT COMPONENTS */
/**
 * Uncomment if RTC is used(for example Arduino RTC).
 * Make sure that the RTC is properly connected and configured.
 */
#define RTC_COMPONENT                       (uint8_t)(0u)
/* ********************************* */
/* ********************************* */

#endif