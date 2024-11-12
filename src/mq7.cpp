#include "mq7.h"


static float convertToResistance(int raw_adc) 
{
  float v_out = raw_adc * (MQ7_VCC_VOLTAGE / MQ7_ANALOG_INPUT_MAX);
  float Rs = ((MQ7_VCC_VOLTAGE - v_out) / v_out) * MQ7_LOAD_RESISTANCE_VAL; // return in ohms
  return Rs;
}

static float calculatePpm(float resistance_under_zero)
{
  int raw_analog_read = analogRead(MQ7_PIN_ANALOG);
  float resistance_under_CO = convertToResistance(raw_analog_read);
  float ratio = resistance_under_CO / resistance_under_zero;
  float coPPM = pow(MQ7_CALCULATION_POW_BASE_CONSTANT, ((log10(ratio) - MQ7_CALCULATION_CONSTANT_1) / (MQ7_CALCULATION_CONSTANT_2)));

  return coPPM;
}

void mq7_init()
{
  pinMode(MQ7_PIN_ANALOG, INPUT);
  pinMode(MQ7_PIN_PWM_HEATER, OUTPUT);

  analogWrite(MQ7_PIN_PWM_HEATER, MQ7_5V_ANALOG_OUTPUT_HEATER); //Start heating
}

//Needs to be called in loop
bool mq7_heatingCycle(bool is_heated_high) 
{
  static unsigned long previous_millis = 0;
  unsigned long current_millis = millis();

  if (current_millis - previous_millis >= MQ7_HEATER_HIGH_TIMEOUT_MS && true == is_heated_high) 
  {
    analogWrite(MQ7_PIN_PWM_HEATER, MQ7_1_4V_ANALOG_OUTPUT_HEATER); //Set heater to 1.4V (approx)
    previous_millis = current_millis;
    is_heated_high = false;
  } 
  else if (current_millis - previous_millis >= MQ7_HEATER_LOW_TIMEOUT_MS && false == is_heated_high) 
  {
    analogWrite(MQ7_PIN_PWM_HEATER, MQ7_5V_ANALOG_OUTPUT_HEATER);   //Set heater back to 5V
    previous_millis = current_millis;
    is_heated_high = true;
  }
}

mq7_ppm_reading_t mq7_readPPM() 
{
  mq7_ppm_reading_t reading = {0};
  reading.success = false;
#if defined(MQ7_R_ZERO)
  float coPPM = calculatePpm(MQ7_R_ZERO);
  reading.value = coPPM;
  if(coPPM >= MQ7_PPM_MINIMUM && coPPM <= MQ7_PPM_MAXIMUM)
  {
    reading.success = true;
  }
#endif
  return reading;
}

#ifdef MQ7_CALIBRATION_ENABLED
  void mq7_calculateResistanceForCalibration()
  {

    Serial.println("Calibrating MQ7 sensor, please ensure clean air...");
    Serial.print("Estimated calibration time : ");
    Serial.print(MQ7_CALIBRATION_TIME);
    Serial.println(" ms");

    float Rs = 0;
    for (int i = 0; i < MQ7_NUM_OF_MEASUREMENTS_CALIB; i++) 
    {
      int raw_adc = analogRead(MQ7_PIN_ANALOG);
      Rs += ((MQ7_ANALOG_INPUT_MAX / raw_adc) - 1) * MQ7_LOAD_RESISTANCE_VAL;
      delay(MQ7_CALIBRATION_DELAY);
    }
    Rs = Rs / MQ7_NUM_OF_MEASUREMENTS_CALIB;  //Average of all readings
    float calculated_resistance = Rs / MQ7_CLEAR_AIR_FACTOR;  //Calculate and return Ro based on clean air factor
    Serial.print("Calculated Resistance in OHMS: ");
    Serial.println(calculated_resistance);
  }
#endif