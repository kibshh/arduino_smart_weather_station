#include "gy_ml8511_sensor.h"

static bool GyMl8511Sensor_ReadUv();

bool GyMl8511Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index)
{
  pinMode(GY_ML8511_SENSOR_PIN, INPUT);
  init_func[*current_index] = GyMl8511Sensor_ReadUv;
  (*current_index)++;
  return true;
}

static bool GyMl8511Sensor_ReadUv()
{
  float analog_reading = (float)analogRead(GY_ML8511_SENSOR_PIN);
  float uv_voltage = (analog_reading / GY_ML8511_SENSOR_ANALOG_INPUT_MAX) * GY_ML8511_SENSOR_VCC_VOLTAGE;  /* Convert to voltage */
  float calculated_uv;

  if(uv_voltage <= GY_ML8511_SENSOR_OUTPUT_VOLTAGE_MIN)
  {
    /* If the voltage is less than or equal to the minimum output voltage (~1.0 V), that means there is no measurable UV light — 0 mW/cm². */
    calculated_uv = GY_ML8511_SENSOR_OUTPUT_INTENSITY_MIN;
  }
  else
  {
    /* Subtract the baseline (zero point) of 1.0 V, since the sensor starts responding to UV only above this. This gives the "active" UV voltage range. */
    uv_voltage -= GY_ML8511_SENSOR_OUTPUT_VOLTAGE_MIN;
    /* 
     * Calculate the UV intensity in mW/cm² by scaling the active UV voltage linearly.
     * (15.0 / (2.9 - 1.0)) gives the slope of the conversion based on the datasheet graph.
     * i.e., every volt above 1.0 V corresponds to ~7.89 mW/cm².
     */
    calculated_uv = uv_voltage * (GY_ML8511_SENSOR_OUTPUT_INTENSITY_MAX / (GY_ML8511_SENSOR_OUTPUT_VOLTAGE_MAX - GY_ML8511_SENSOR_OUTPUT_VOLTAGE_MIN));
  }

  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  String uv_formatted = String(calculated_uv, CONFIGS_DISPLAY_1_DECIMAL);

  Serial.print("UV intensity: ");
  Serial.print(uv_formatted);
  Serial.println("mW/cm2\n");

  lcd.print("UV int: ");
  lcd.print(uv_formatted);

  return true;
}