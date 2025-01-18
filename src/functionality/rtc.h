#include <stdint.h>
#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "../error_manager/error_manager.h"


#define RTC_COMPILE_DATE    __DATE__
#define RTC_COMPILE_TIME    __TIME__

#define RTC_I2C_ADDR        (0x68)

#define RTC_MIN_YEAR        (2024u)
#define RTC_MIN_MONTH       (1u)
#define RTC_MAX_MONTH       (12u)
#define RTC_MIN_DAY         (1u)
#define RTC_MAX_DAY         (31u)
#define RTC_MIN_HOUR        (0u)
#define RTC_MAX_HOUR        (23u)
#define RTC_MIN_MINUTE      (0u)
#define RTC_MAX_MINUTE      (59u)
#define RTC_MIN_SECOND      (0u)
#define RTC_MAX_SECOND      (59u)

typedef struct
{
  uint16_t year; 
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t mins;
  uint8_t secs;
}rtc_reading_t;

typedef struct
{
  rtc_reading_t rtc_reading;
  error_manager_error_code_te error_code;
} rtc_return_t;

void rtc_init();
rtc_return_t rtc_getTime();

#endif