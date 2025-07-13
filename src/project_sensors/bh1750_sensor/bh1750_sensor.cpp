#include "bh1750_sensor.h"
#include <BH1750.h>

static bool Bh1750Sensor_ReadLuminance();

static BH1750 light_meter;

bool Bh1750Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index)
{
    if(!light_meter.begin())
    {
      Serial.println("Error in initialization of BH1750 sensor\n");
      return false;
    }
    else
    {
      init_func[*current_index] = Bh1750Sensor_ReadLuminance;
      (*current_index)++;
      return true;
    }
}

static bool Bh1750Sensor_ReadLuminance()
{
  float luminance = light_meter.readLightLevel();
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);

  if(!isnan(luminance))
  {
    if(luminance > BH1750_SENSOR_LUMINANCE_MIN && luminance < BH1750_SENSOR_LUMINANCE_MAX)
    {
        String luminance_formatted = String(luminance, CONFIGS_DISPLAY_0_DECIMALS);

        Serial.print("Luminance: ");
        Serial.print(luminance_formatted);
        Serial.println(" lux\n");

        lcd.print("Lum: ");
        lcd.print(luminance_formatted);
        lcd.print(" lux");   

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