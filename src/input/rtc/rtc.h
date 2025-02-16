#include <stdint.h>
#ifndef RTC_H
#define RTC_H

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include "../input_types.h"

/* Macro for RTC compile date */
#define RTC_COMPILE_DATE    __DATE__
/* Macro for RTC compile time */
#define RTC_COMPILE_TIME    __TIME__

/* Macro for RTC I2C address */
#define RTC_I2C_ADDR        (0x68)

/* Default RTC identifier */
#define RTC_DEFAULT_RTC     (uint8_t)(0u)

/* Minimum valid year for RTC */
#define RTC_MIN_YEAR        (uint16_t)(2024u)

/* Maximum anmd minimum valid month for RTC */
#define RTC_MIN_MONTH       (uint8_t)(1u)
#define RTC_MAX_MONTH       (uint8_t)(12u)

/* Maximum and minimum valid day for RTC */
#define RTC_MIN_DAY         (uint8_t)(1u)
#define RTC_MAX_DAY         (uint8_t)(31u)

/* Maximum and minimum valid hour for RTC */
#define RTC_MIN_HOUR        (uint8_t)(0u)
#define RTC_MAX_HOUR        (uint8_t)(23u)

/* Maximum anmd minimum valid minute for RTC */
#define RTC_MIN_MINUTE      (uint8_t)(0u)
#define RTC_MAX_MINUTE      (uint8_t)(59u)

/* Maximum and minimum valid second for RTC */
#define RTC_MIN_SECOND      (uint8_t)(0u)
#define RTC_MAX_SECOND      (uint8_t)(59u)

/**
 * @brief Initializes the Real-Time Clock (RTC) module.
 *
 * This function initializes the RTC module, checks its power status, 
 * and sets the RTC time if the module has lost power or is being used for the first time.
 *
 * @return control_error_code_te
 * - ERROR_CODE_NO_ERROR: RTC initialized successfully.
 * - ERROR_CODE_RTC_INIT_FAILED: RTC initialization failed.
 */
control_error_code_te rtc_init();

/**
 * @brief Retrieves the current date and time from the RTC module.
 *
 * This function reads the current time from the RTC module and performs basic validation 
 * to ensure the values fall within expected ranges. If the RTC is found and the values 
 * are valid, the function returns a structure containing the timestamp. Otherwise, 
 * it returns an error code indicating that the RTC was not found.
 *
 * @param[in] id Identifier for the RTC module. Should be `RTC_DEFAULT_RTC` for the default module.
 * @return `rtc_return_ts` structure containing the current date and time if valid, 
 *         or an error code if the RTC is not found or the values are out of range.
 */
rtc_return_ts rtc_getTime(uint8_t id);

#endif