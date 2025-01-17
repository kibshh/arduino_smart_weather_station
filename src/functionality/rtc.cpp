#include "rtc.h"

//GLOBAL VARIABLES
RTC_DS3231 rtc;

void rtc_init()
{
    if (!rtc.begin()) 
    {
      Serial.println("Error in initialization of RTC sensor");
    }

  if (rtc.lostPower()) // When time needs to be set on a new device, or after a power los
  {
    Serial.println("RTC lost power, setting the time...");
    rtc.adjust(DateTime(F(RTC_COMPILE_DATE), F(RTC_COMPILE_TIME))); // Set to the compile time
  }
}

rtc_reading_t rtc_getTime()
{
  DateTime now = rtc.now();

  rtc_reading_t new_reading = {.error_code=ERROR_CODE_INTERNAL_ERROR};

  if(now.hour() >= RTC_MIN_HOUR && now.hour() <= RTC_MAX_HOUR && 
     now.minute() >= RTC_MIN_MINUTE && now.minute() <= RTC_MAX_MINUTE && 
     now.second() >= RTC_MIN_SECOND && now.second() <= RTC_MAX_SECOND &&
     now.year() >= RTC_MIN_YEAR && now.month() >= RTC_MIN_MONTH &&
     now.month() >= RTC_MIN_MONTH && now.month() <= RTC_MAX_MONTH &&
     now.day() >= RTC_MIN_DAY && now.day() <= RTC_MAX_DAY)
  {
    new_reading.year = now.year();
    new_reading.month = now.month();
    new_reading.day = now.day();
    new_reading.hour = now.hour();
    new_reading.mins = now.minute();
    new_reading.secs = now.second();

    new_reading.error_code = ERROR_CODE_NO_ERROR;
  }
  return new_reading;
}