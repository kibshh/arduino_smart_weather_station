#include "rtc.h"
#include <RTClib.h>

static RTC_DS3231 rtc;

static bool Rtc_ReadDateTime();
static DateTime Rtc_FixTimeOffset(DateTime current_time);

bool Rtc_Init(CurrentReading_t *init_func, uint8_t *current_index)
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
  
  init_func[*current_index] = Rtc_ReadDateTime;
  (*current_index)++;
  return true;
}

static bool Rtc_ReadDateTime()
{
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);

  DateTime now = rtc.now();
  if(now.year() >= RTC_MIN_YEAR && 
     now.month() >= RTC_MIN_MONTH && now.month() <= RTC_MAX_MONTH && 
     now.day() >= RTC_MIN_DAY && now.day() <= RTC_MAX_DAY &&
     now.hour() >= RTC_MIN_HOUR && now.hour() < RTC_MAX_HOUR && 
     now.minute() >= RTC_MIN_MINUTE && now.minute() < RTC_MAX_MINUTE && 
     now.second() >= RTC_MIN_SECOND && now.second() < RTC_MAX_SECOND)
     {
      /* Fix offset */
      now = Rtc_FixTimeOffset(now);

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

static DateTime Rtc_FixTimeOffset(DateTime current_time)
{
  uint16_t year = current_time.year();
  uint8_t month = current_time.month();
  uint8_t day = current_time.day();
  uint8_t hour = current_time.hour();
  uint8_t minute = current_time.minute();
  uint8_t second = current_time.second();

  minute += RTC_OFFSET_FIX_MINUTES;

  if (minute >= RTC_MAX_MINUTE)
  {
    minute -= RTC_MAX_MINUTE;
    hour += 1;
  }

  if (hour >= RTC_MAX_HOUR)
  {
    hour -= RTC_MAX_HOUR;
    day += 1;
  }

  /* Handle day overflow depending on month and year */
  uint8_t days_in_month;

  if (month == RTC_MONTH_FEB)
  {
    bool is_leap = ((year % RTC_LEAP_YEAR_DIVIDER1 == 0) && (year % RTC_LEAP_YEAR_DIVIDER2 != 0)) || (year % RTC_LEAP_YEAR_DIVIDER3 == 0);
    days_in_month = is_leap ? RTC_LEAP_YEAR_DAYS : RTC_NOT_LEAP_YEAR_DAYS;
  }
  else if (month == RTC_MONTH_APR || month == RTC_MONTH_JUN || month == RTC_MONTH_SEP || month == RTC_MONTH_NOV)
  {
    days_in_month = RTC_DAYS_IN_MONTH_LOWER;
  }
  else
  {
    days_in_month = RTC_DAYS_IN_MONTH_HIGHER;
  }

  if (day > days_in_month)
  {
    day = 1;
    month += 1;

    if (month > RTC_MONTH_DEC)
    {
      month = 1;
      year += 1;
    }
  }

  return DateTime(year, month, day, hour, minute, second);
}