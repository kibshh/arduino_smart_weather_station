#ifndef PROJECT_CONFIGS_H
#define PROJECT_CONFIGS_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h>

#define CONFIGS_ARDUINO_RAIN_SENSOR_INCLUDED
#define CONFIGS_BH1750_SENSOR_INCLUDED
#define CONFIGS_BMP280_SENSOR_INCLUDED
#define CONFIGS_DHT11_SENSOR_INCLUDED
#define CONFIGS_GY_ML8511_SENSOR_INCLUDED
#define CONFIGS_MQ7_SENSOR_INCLUDED
#define CONFIGS_MQ135_SENSOR_INCLUDED
#define CONFIGS_I2C_SCANNER_INCLUDED
#define CONFIGS_RTC_INCLUDED

#define CONFIGS_MAX_NUM_OF_MEASUREMENTS  (uint8_t)(11u)
#define CONFIGS_READ_INTERVAL_MS         (uint64_t)(2000u)
#define CONFIGS_LOOP_FREQUENCY_MS        (uint64_t)(5u)

#define CONFIGS_BAUDRATE                 (uint16_t)(9600u)
#define CONFIGS_SERIAL_CONNECT_TIMEOUT   (uint64_t)(5000u)

#define CONFIGS_LCD_HEIGHT               (uint8_t)(2u)
#define CONFIGS_LCD_WIDTH                (uint8_t)(16u)
#define CONFIGS_LCD_I2C_ADDDR            (uint8_t)(0x27)

#define CONFIGS_DISPLAY_0_DECIMALS       (uint8_t)(0u)
#define CONFIGS_DISPLAY_1_DECIMAL        (uint8_t)(1u)
#define CONFIGS_DISPLAY_2_DECIMALS       (uint8_t)(2u)
#define CONFIGS_DISPLAY_3_DECIMALS       (uint8_t)(3u)
#define CONFIGS_DISPLAY_4_DECIMALS       (uint8_t)(4u)
#define CONFIGS_DISPLAY_5_DECIMALS       (uint8_t)(5u)
#define CONFIGS_DISPLAY_6_DECIMALS       (uint8_t)(6u)

#define CONFIGS_WATCHDOG_TIMEOUT         (WDTO_2S)

typedef bool (*CurrentReading_t)(void);

extern LiquidCrystal_I2C lcd;

bool ProjectConfigs_Init();

#endif