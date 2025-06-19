#include "bh1750_sensor.h"
#include <BH1750.h>

static BH1750 light_meter;

bool Bh1750Sensor_Init()
{
    if(!light_meter.begin())
    {
      Serial.println("Error in initialization of BH1750 sensor\n");
      return false;
    }
    else
    {
        return true;
    }
}

bool Bh1750Sensor_ReadLuminance()
{
  float luminance = light_meter.readLightLevel();
  lcd.clear();
  lcd.setCursor(0, 0);

  if(!isnan(luminance))
  {
    if(luminance > BH1750_SENSOR_LUMINANCE_MIN && luminance < BH1750_SENSOR_LUMINANCE_MAX)
    {
        String luminance_formatted = String(luminance, CONFIGS_DISPLAY_0_DECIMALS);

        Serial.print("Luminance: ");
        Serial.print(luminance_formatted);
        Serial.println("lx\n");

        lcd.print("Luminance: ");
        lcd.print(luminance_formatted);
        lcd.print("lx");   

        return true;
    }
    else
    {
        return false;
    }
  }
  else
  {
    Serial.println("Error luminance\n");
    lcd.print("Error luminance");
    return false;
  }
}