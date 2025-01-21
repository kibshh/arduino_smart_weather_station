#include "mq135.h"


void mq135_init()
{
  pinMode(SENSORS_MQ135_PIN_ANALOG, INPUT);
}

float mq135_readPPM()
{
  float ppm = NAN;
#if defined(MQ135_PARAMETER_A) && defined(MQ135_PARAMETER_B) && defined(MQ135_R_ZERO)
  float resistance_under_gas = (float)analogRead(SENSORS_MQ135_PIN_ANALOG);
  float ratio = resistance_under_gas / MQ135_R_ZERO;
  float ppm = MQ135_PARAMETER_A * pow(ratio, -MQ135_PARAMETER_B);
#endif
  return ppm;
}

#ifdef MQ135_CALIBRATION_ENABLED
  void mq135_calculateResistanceForCalibration()
  {
    Serial.println("Calibrating MQ135 sensor, please ensure clean air...");

    float sensor_analog_reading = (float)analogRead(SENSORS_MQ135_PIN_ANALOG);
    float maximum_analog_reading = MQ135_ANALOG_INPUT_MAX;

    float calculated_resistance = ((maximum_analog_reading / sensor_analog_reading) - 1) * MQ135_LOAD_RESISTANCE_VAL;

    Serial.print("Calculated Resistance in OHMS: ");
    Serial.println(calculated_resistance);
  }
#endif

