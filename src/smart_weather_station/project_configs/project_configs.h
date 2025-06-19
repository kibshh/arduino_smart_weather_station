#ifndef PROJECT_CONFIGS_H
#define PROJECT_CONFIGS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define CONFIGS_ARDUINO_RAIN_SENSOR_INCLUDED
#define CONFIGS_BH1750_SENSOR_INCLUDED
// #define CONFIGS_BMP280_SENSOR_INCLUDED
// #define CONFIGS_DHT11_SENSOR_INCLUDED
#define CONFIGS_GY_ML8511_SENSOR_INCLUDED
#define CONFIGS_MQ7_SENSOR_INCLUDED
#define CONFIGS_MQ135_SENSOR_INCLUDED
#define CONFIGS_I2C_SCANNER_INCLUDED
#define CONFIGS_RTC_INCLUDED

#define CONFIGS_LCD_HEIGHT            (uint8_t)(2u)
#define CONFIGS_LCD_WIDTH             (uint8_t)(16u)
#define CONFIGS_LCD_I2C_ADDDR         (uint8_t)(0x27)

#define CONFIGS_DISPLAY_0_DECIMALS    (uint8_t)(0u)
#define CONFIGS_DISPLAY_1_DECIMAL     (uint8_t)(1u)
#define CONFIGS_DISPLAY_2_DECIMALS    (uint8_t)(2u)
#define CONFIGS_DISPLAY_3_DECIMALS    (uint8_t)(3u)
#define CONFIGS_DISPLAY_4_DECIMALS    (uint8_t)(4u)
#define CONFIGS_DISPLAY_5_DECIMALS    (uint8_t)(5u)
#define CONFIGS_DISPLAY_6_DECIMALS    (uint8_t)(6u)

extern LiquidCrystal_I2C lcd;
#endif