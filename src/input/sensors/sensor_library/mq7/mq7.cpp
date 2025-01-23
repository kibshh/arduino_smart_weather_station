#include "mq7.h"

static float convertToResistance(int raw_adc);

void mq7_init()
{
  pinMode(SENSORS_MQ7_PIN_ANALOG, INPUT);
  pinMode(SENSORS_MQ7_PIN_PWM_HEATER, OUTPUT);
  analogWrite(SENSORS_MQ7_PIN_PWM_HEATER, MQ7_5V_ANALOG_OUTPUT_HEATER); //Start heating
}

float mq7_readPPM()
{
  float coPPM = NAN;
#if defined(SENSORS_MQ7_R_ZERO)
  int raw_analog_read = analogRead(SENSORS_MQ7_PIN_ANALOG);
  float resistance_under_CO = convertToResistance(raw_analog_read);
  float ratio = resistance_under_CO / SENSORS_MQ7_R_ZERO;
  coPPM = pow(MQ7_CALCULATION_POW_BASE_CONSTANT, ((log10(ratio) - SENSORS_MQ7_CALCULATION_CONSTANT_1) / (SENSORS_MQ7_CALCULATION_CONSTANT_2)));
#endif
  return coPPM;
}

//Needs to be called in loop
bool mq7_heatingCycle(bool is_heated_high) 
{
  static unsigned long previous_millis = 0;
  unsigned long current_millis = millis();

  if (current_millis - previous_millis >= MQ7_HEATER_HIGH_TIMEOUT_MS && true == is_heated_high) 
  {
    analogWrite(SENSORS_MQ7_PIN_PWM_HEATER, MQ7_1_4V_ANALOG_OUTPUT_HEATER); //Set heater to 1.4V (approx)
    previous_millis = current_millis;
    is_heated_high = false;
  } 
  else if (current_millis - previous_millis >= MQ7_HEATER_LOW_TIMEOUT_MS && false == is_heated_high) 
  {
    analogWrite(SENSORS_MQ7_PIN_PWM_HEATER, MQ7_5V_ANALOG_OUTPUT_HEATER);   //Set heater back to 5V
    previous_millis = current_millis;
    is_heated_high = true;
  }
}

#ifdef SENSORS_MQ7_CALIBRATION_ENABLED
float mq7_calculateResistanceForCalibration()
{
  float Rs = 0;
  for (uint8_t i = 0; i < MQ7_NUM_OF_MEASUREMENTS_CALIB; i++) 
  {
    int raw_adc = analogRead(SENSORS_MQ7_PIN_ANALOG);
    Rs += (((float)MQ7_ANALOG_INPUT_MAX / raw_adc) - 1) * MQ7_LOAD_RESISTANCE_VAL;
    delay(MQ7_CALIBRATION_DELAY);
  }
  Rs = Rs / MQ7_NUM_OF_MEASUREMENTS_CALIB;  //Average of all readings
  float calculated_resistance = Rs / MQ7_CLEAR_AIR_FACTOR;  //Calculate and return Ro based on clean air factor

  return calculated_resistance;
}
#endif

static float convertToResistance(int raw_adc) 
{
  float v_out = raw_adc * ((float)MQ7_VCC_VOLTAGE / MQ7_ANALOG_INPUT_MAX);
  float Rs = ((MQ7_VCC_VOLTAGE - v_out) / v_out) * MQ7_LOAD_RESISTANCE_VAL; // return in ohms
  return Rs;
}