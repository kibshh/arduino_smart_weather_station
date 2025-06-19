#include "rtc.h"
#include <RTClib.h>

static RTC_DS3231 rtc;

bool Rtc_Init()
{
    if (!rtc.begin()) 
    {
      Serial.println("Error in initialization of RTC module");
      return false;
    }

  if (rtc.lostPower()) /* When time needs to be set on a new device, or after a power loss */
  {
    Serial.println("RTC lost power, setting the time...");
    rtc.adjust(DateTime(F(RTC_COMPILE_DATE), F(RTC_COMPILE_TIME))); /* Set to the compile time */
  }
  
  return true;
}

bool Rtc_ReadDateTime()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  DateTime now = rtc.now();
  if(now.year() >= RTC_MIN_YEAR && 
     now.month() >= RTC_MIN_MONTH && now.month() <= RTC_MAX_MONTH && 
     now.day() >= RTC_MIN_DAY && now.day() <= RTC_MAX_DAY &&
     now.hour() >= RTC_MIN_HOUR && now.hour() <= RTC_MAX_HOUR && 
     now.minute() >= RTC_MIN_MINUTE && now.minute() <= RTC_MAX_MINUTE && 
     now.second() >= RTC_MIN_SECOND && now.second() <= RTC_MAX_SECOND)
     {
       char datetime_string[CONFIGS_LCD_WIDTH + 1]; /* One extra for null terminator */
       snprintf(datetime_string, sizeof(datetime_string), "%02d:%02d %02d/%02d/%04d", now.hour(), now.minute(), now.day(), now.month(), now.year());
       Serial.println(datetime_string);
       Serial.println("\n");
       lcd.print(datetime_string);

       return true;
     }

     Serial.println("Error date and time\n");
     lcd.print("Error datetime");
     return false;
}