#include "project_configs.h"

LiquidCrystal_I2C lcd(CONFIGS_LCD_I2C_ADDDR, CONFIGS_LCD_WIDTH, CONFIGS_LCD_HEIGHT);

bool ProjectConfigs_Init()
{
  Wire.begin();
  Serial.begin(CONFIGS_BAUDRATE);
  lcd.begin(CONFIGS_LCD_WIDTH, CONFIGS_LCD_HEIGHT); /* Initialize a 16x2 LCD */
  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.noCursor();

  uint64_t serial_try_start_time = millis();

  while (!Serial) /* Wait for the serial port to connect */
  {
    if(CONFIGS_SERIAL_CONNECT_TIMEOUT <= (uint64_t)millis() - serial_try_start_time)
    {
        return false;
    }
  }
  return true;
}