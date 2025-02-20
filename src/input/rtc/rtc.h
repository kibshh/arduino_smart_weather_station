#include <stdint.h>
#ifndef RTC_H
#define RTC_H

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "../input_types.h"


#define RTC_COMPILE_DATE    __DATE__
#define RTC_COMPILE_TIME    __TIME__

#define RTC_I2C_ADDR        (0x68)

#define RTC_DEFAULT_RTC     (0u)

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

/**
 * @brief Initializes the Real-Time Clock (RTC) module.
 *
 * This function initializes the RTC module, checks its power status, 
 * and sets the RTC time if the module has lost power or is being used for the first time.
 *
 * @return error_manager_error_code_te
 * - ERROR_CODE_NO_ERROR: RTC initialized successfully.
 * - ERROR_CODE_RTC_INIT_FAILED: RTC initialization failed.
 */
error_manager_error_code_te rtc_init();
rtc_return_ts rtc_getTime(uint8_t id);

#endif