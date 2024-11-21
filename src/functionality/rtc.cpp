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

rtc_date_reading_t rtc_getDate()
{
  DateTime now = rtc.now();

  rtc_date_reading_t new_reading = {.success=false, .currentDate={0}};

  if(now.year() >= RTC_MIN_YEAR && now.month() >= RTC_MIN_MONTH && now.month() <= RTC_MAX_MONTH && 
     now.day() >= RTC_MIN_DAY && now.day() <= RTC_MAX_DAY)
  {
    new_reading.currentDate.year = now.year();
    new_reading.currentDate.month = now.month();
    new_reading.currentDate.day = now.day();

    new_reading.success = true;
  }
  return new_reading;
}

rtc_time_reading_t rtc_getTime()
{
  DateTime now = rtc.now();

  rtc_time_reading_t new_reading = {.success=false, .currentTime={0}};

  if(now.hour() >= RTC_MIN_HOUR && now.hour() <= RTC_MAX_HOUR && 
     now.minute() >= RTC_MIN_MINUTE && now.minute() <= RTC_MAX_MINUTE && 
     now.second() >= RTC_MIN_SECOND && now.second() <= RTC_MAX_SECOND)
  {
    new_reading.currentTime.hour = now.hour();
    new_reading.currentTime.mins = now.minute();
    new_reading.currentTime.secs = now.second();

    new_reading.success = true;
  }
  return new_reading;
}