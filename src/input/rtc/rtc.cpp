#include "rtc.h"

//GLOBAL VARIABLES
RTC_DS3231 rtc;

error_manager_error_code_te rtc_init()
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

  if (!rtc.begin()) 
  {
    error_code = ERROR_CODE_RTC_INIT_FAILED;
  }

  if (rtc.lostPower()) // When time needs to be set on a new device, or after a power loss
  {
    // Serial.println("RTC lost power, setting the time...");
    rtc.adjust(DateTime(F(RTC_COMPILE_DATE), F(RTC_COMPILE_TIME))); // Set to the compile time
  }
}

rtc_return_ts rtc_getTime(uint8_t id)
{
  rtc_return_ts new_reading;
  new_reading.error_code = ERROR_CODE_RTC_RTC_NOT_FOUND;

  if(id == RTC_DEFAULT_RTC)
  {
    DateTime now = rtc.now();

    if(now.hour() >= RTC_MIN_HOUR && now.hour() <= RTC_MAX_HOUR && 
        now.minute() >= RTC_MIN_MINUTE && now.minute() <= RTC_MAX_MINUTE && 
        now.second() >= RTC_MIN_SECOND && now.second() <= RTC_MAX_SECOND &&
        now.year() >= RTC_MIN_YEAR && now.month() >= RTC_MIN_MONTH &&
        now.month() >= RTC_MIN_MONTH && now.month() <= RTC_MAX_MONTH &&
        now.day() >= RTC_MIN_DAY && now.day() <= RTC_MAX_DAY)
    {
      new_reading.rtc_reading.year = now.year();
      new_reading.rtc_reading.month = now.month();
      new_reading.rtc_reading.day = now.day();
      new_reading.rtc_reading.hour = now.hour();
      new_reading.rtc_reading.mins = now.minute();
      new_reading.rtc_reading.secs = now.second();

      new_reading.error_code = ERROR_CODE_NO_ERROR;
    }
  }
  return new_reading;
}