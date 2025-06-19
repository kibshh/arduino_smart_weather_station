#include "mq135_sensor.h"

static float calculatePpm(float a, float b, float resistance_under_zero)
{
  float resistance_under_gas = (float)analogRead(MQ135_PIN_ANALOG);
  float ratio = resistance_under_gas / resistance_under_zero;
  float ppm = a * pow(ratio, -b);
  return ppm;
}

void Mq135Sensor_init()
{
  pinMode(MQ135_SENSOR_PIN_ANALOG, INPUT);
}

bool Mq135Sensor_ReadPpm()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  uint16_t analog_read = analogRead(MQ135_SENSOR_PIN_ANALOG);
  float v_out = (analog_read * MQ135_SENSOR_VCC_VOLTAGE) / MQ135_SENSOR_ANALOG_INPUT_MAX;
  float resistance_under_gas = ((MQ135_SENSOR_VCC_VOLTAGE - v_out) / v_out) * MQ135_SENSOR_LOAD_RESISTANCE;
  float ratio = resistance_under_gas / MQ135_SENSOR_R_ZERO;
  float ppm = MQ135_SENSOR_PARAMETER_A * pow(ratio, -MQ135_SENSOR_PARAMETER_B);
  
  if(ppm >= MQ135_SENSOR_PPM_MINIMUM &&  ppm <= MQ135_SENSOR_PPM_MAXIMUM)
  {
    String ppm_formatted = String(ppm, CONFIGS_DISPLAY_0_DECIMALS);

    Serial.print("Gases PPM: ");
    Serial.print(ppm_formatted);
    Serial.println("\n");

    lcd.print("Gases PPM: ");
    lcd.print(ppm_formatted);

    return true;
  }

  Serial.println("Error gases PPM\n");
  lcd.print("Error gases PPM");
  return false;
}