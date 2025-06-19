#include "mq7_sensor.h"

static uint64_t previous_millis = 0;
static bool is_heated_high = false;

bool Mq7Sensor_Init()
{
  pinMode(MQ7_SENSOR_PIN_ANALOG, INPUT);
  pinMode(MQ7_SENSOR_PIN_PWM_HEATER, OUTPUT);
  analogWrite(MQ7_SENSOR_PIN_PWM_HEATER, MQ7_SENSOR_5V_ANALOG_OUTPUT_HEATER); /* Start heating */
  is_heated_high = true;
}

/* Needs to be called in loop */
void Mq7Sensor_HeatingCycle() 
{
  uint64_t current_millis = millis();

  if (current_millis - previous_millis >= MQ7_SENSOR_HEATER_HIGH_TIMEOUT_MS && true == is_heated_high) 
  {
    analogWrite(MQ7_SENSOR_PIN_PWM_HEATER, MQ7_SENSOR_1_4V_ANALOG_OUTPUT_HEATER); /* Set heater to 1.4V */
    previous_millis = current_millis;
    is_heated_high = false;
  } 
  else if (current_millis - previous_millis >= MQ7_SENSOR_HEATER_LOW_TIMEOUT_MS && false == is_heated_high) 
  {
    analogWrite(MQ7_SENSOR_PIN_PWM_HEATER, MQ7_SENSOR_5V_ANALOG_OUTPUT_HEATER);   /* Set heater back to 5V */
    previous_millis = current_millis;
    is_heated_high = true;
  }
}

bool Mq7Sensor_ReadPpm() 
{
  lcd.clear();
  lcd.setCursor(0, 0);

  uint16_t analog_read = analogRead(MQ7_SENSOR_PIN_ANALOG);
  float voltage_read = analog_read * (MQ7_SENSOR_VCC_VOLTAGE / MQ7_SENSOR_ANALOG_INPUT_MAX);
  float resistance_read = ((MQ7_SENSOR_VCC_VOLTAGE - voltage_read) / voltage_read) * MQ7_SENSOR_LOAD_RESISTANCE_VAL; /* in ohms */
  float ratio = resistance_read / MQ7_SENSOR_R_ZERO;
  float co_ppm = pow(MQ7_SENSOR_CALCULATION_POW_BASE_CONSTANT, ((log10(ratio) - MQ7_SENSOR_CALCULATION_CONSTANT_1) / (MQ7_SENSOR_CALCULATION_CONSTANT_2)));

  if(co_ppm >= MQ7_SENSOR_PPM_MINIMUM && co_ppm <= MQ7_SENSOR_PPM_MAXIMUM)
  {
    String co_ppm_formatted = String(co_ppm, CONFIGS_DISPLAY_0_DECIMALS);

    Serial.print("CO PPM: ");
    Serial.print(co_ppm_formatted);
    Serial.println("\n");

    lcd.print("CO PPM: ");
    lcd.print(co_ppm_formatted);
    return true;
  }

  Serial.println("Error CO PPM\n");
  lcd.print("Error CO PPM");
  return false;
}