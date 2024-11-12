#include "mq135.h"


static float calculatePpm(float a, float b, float resistance_under_zero)
{
  float resistance_under_gas = (float)analogRead(MQ135_PIN_ANALOG);
  float ratio = resistance_under_gas / resistance_under_zero;
  float ppm = a * pow(ratio, -b);
  return ppm;
}

void mq135_init()
{
  pinMode(MQ135_PIN_ANALOG, INPUT);
}

mq135_ppm_reading_t mq135_readPPM()
{
  mq135_ppm_reading_t reading = {0};
  reading.success = false;
#if defined(MQ135_PARAMETER_A) && defined(MQ135_PARAMETER_B) && defined(MQ135_R_ZERO)
  float ppm = calculatePpm(MQ135_PARAMETER_A, MQ135_PARAMETER_B, MQ135_R_ZERO);
  reading.value = ppm;
  if(ppm >= MQ135_PPM_MINIMUM &&  ppm <= MQ135_PPM_MAXIMUM)
  {
    reading.success = true;
  }
#endif
  return reading;
}

#ifdef MQ135_CALIBRATION_ENABLED
  void mq135_calculateResistanceForCalibration()
  {
    Serial.println("Calibrating MQ135 sensor, please ensure clean air...");

    float sensor_analog_reading = (float)analogRead(MQ135_PIN_ANALOG);
    float maximum_analog_reading = MQ135_ANALOG_INPUT_MAX;

    float calculated_resistance = ((maximum_analog_reading / sensor_analog_reading) - 1) * MQ135_LOAD_RESISTANCE_VAL;

    Serial.print("Calculated Resistance in OHMS: ");
    Serial.println(calculated_resistance);
  }
#endif

