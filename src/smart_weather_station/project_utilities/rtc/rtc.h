#ifndef RTC_H
#define RTC_H

#include "../../project_configs/project_configs.h"

#define RTC_COMPILE_DATE    __DATE__
#define RTC_COMPILE_TIME    __TIME__

#define RTC_MIN_YEAR        (uint16_t)(2024u)
#define RTC_MIN_MONTH       (uint8_t)(1u)
#define RTC_MAX_MONTH       (uint8_t)(12u)
#define RTC_MIN_DAY         (uint8_t)(1u)
#define RTC_MAX_DAY         (uint8_t)(31u)
#define RTC_MIN_HOUR        (uint8_t)(0u)
#define RTC_MAX_HOUR        (uint8_t)(23u)
#define RTC_MIN_MINUTE      (uint8_t)(0u)
#define RTC_MAX_MINUTE      (uint8_t)(59u)
#define RTC_MIN_SECOND      (uint8_t)(0u)
#define RTC_MAX_SECOND      (uint8_t)(59u)

bool Rtc_Init();
bool Rtc_ReadDateTime();

#endif