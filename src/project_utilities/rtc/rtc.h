#ifndef RTC_H
#define RTC_H

#include "../../project_configs/project_configs.h"

#define RTC_COMPILE_DATE    __DATE__
#define RTC_COMPILE_TIME    __TIME__

#define RTC_MIN_YEAR              (uint16_t)(2024u)
#define RTC_MIN_MONTH             (uint8_t)(1u)
#define RTC_MAX_MONTH             (uint8_t)(12u)
#define RTC_MIN_DAY               (uint8_t)(1u)
#define RTC_MAX_DAY               (uint8_t)(31u)
#define RTC_MIN_HOUR              (uint8_t)(0u)
#define RTC_MAX_HOUR              (uint8_t)(24u)
#define RTC_MIN_MINUTE            (uint8_t)(0u)
#define RTC_MAX_MINUTE            (uint8_t)(60u)
#define RTC_MIN_SECOND            (uint8_t)(0u)
#define RTC_MAX_SECOND            (uint8_t)(60u)
#define RTC_OFFSET_FIX_MINUTES    (uint8_t)(7u)

#define RTC_MONTH_JAN             (uint8_t)(1u)
#define RTC_MONTH_FEB             (uint8_t)(2u)
#define RTC_MONTH_MAR             (uint8_t)(3u)
#define RTC_MONTH_APR             (uint8_t)(4u)
#define RTC_MONTH_MAY             (uint8_t)(5u)
#define RTC_MONTH_JUN             (uint8_t)(6u)
#define RTC_MONTH_JUL             (uint8_t)(7u)
#define RTC_MONTH_AUG             (uint8_t)(8u)
#define RTC_MONTH_SEP             (uint8_t)(9u)
#define RTC_MONTH_OCT             (uint8_t)(10u)
#define RTC_MONTH_NOV             (uint8_t)(11u)
#define RTC_MONTH_DEC             (uint8_t)(12u)

#define RTC_DAYS_IN_MONTH_HIGHER  (uint8_t)(31u)
#define RTC_DAYS_IN_MONTH_LOWER   (uint8_t)(30u)
#define RTC_NOT_LEAP_YEAR_DAYS    (uint8_t)(28u)
#define RTC_LEAP_YEAR_DAYS        (uint8_t)(29u)
#define RTC_LEAP_YEAR_DIVIDER1    (uint16_t)(4u)
#define RTC_LEAP_YEAR_DIVIDER2    (uint16_t)(100u)
#define RTC_LEAP_YEAR_DIVIDER3    (uint16_t)(400u)

bool Rtc_Init(CurrentReading_t *init_func, uint8_t *current_index);

#endif