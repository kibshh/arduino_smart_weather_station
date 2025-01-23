#include "mq135.h"


void mq135_init()
{
  pinMode(SENSORS_MQ135_PIN_ANALOG, INPUT);
}

float mq135_readPPM()
{
  float ppm = NAN;
#if defined(SENSORS_MQ135_PARAMETER_A) && defined(SENSORS_MQ135_PARAMETER_B) && defined(SENSORS_MQ135_R_ZERO)
  float resistance_under_gas = (float)analogRead(SENSORS_MQ135_PIN_ANALOG);
  float ratio = resistance_under_gas / SENSORS_MQ135_R_ZERO;
  float ppm = SENSORS_MQ135_PARAMETER_A * pow(ratio, -SENSORS_MQ135_PARAMETER_B);
#endif
  return ppm;
}

#ifdef SENSORS_MQ135_CALIBRATION_ENABLED
float mq135_calculateResistanceForCalibration()
{
  float sensor_analog_reading = (float)analogRead(SENSORS_MQ135_PIN_ANALOG);
  float maximum_analog_reading = MQ135_ANALOG_INPUT_MAX;

  float calculated_resistance = ((maximum_analog_reading / sensor_analog_reading) - 1) * MQ135_LOAD_RESISTANCE_VAL;

  return calculated_resistance
}
#endif

