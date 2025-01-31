#include "mq135.h"


void mq135_init()
{
  pinMode(SENSORS_MQ135_PIN_ANALOG, INPUT);
}

float mq135_readPPM()
{
  float ppm = MQ135_INVALID_VALUE;
#if defined(SENSORS_MQ135_PARAMETER_A) && defined(SENSORS_MQ135_PARAMETER_B) && defined(SENSORS_MQ135_R_ZERO)
  int sensor_analog_reading = analogRead(SENSORS_MQ135_PIN_ANALOG); // Read the analog value from the MQ135 sensor pin
  
  if(MQ135_ANALOG_INPUT_MIN <= sensor_analog_reading && 
     MQ135_ANALOG_INPUT_MAX >= sensor_analog_reading && // Check for valid analog read
     SENSORS_MQ135_R_ZERO >= MQ135_R_ZERO_MINIMUM)      // Check for valid R0 resistance and avoid division by 0
  {
    if(MQ135_ANALOG_INPUT_MIN == sensor_analog_reading)
    {
      sensor_analog_reading = MQ135_ANALOG_INPUT_MIN_VALID; // To avoid division by 0
    }
    float ratio = (float)sensor_analog_reading / SENSORS_MQ135_R_ZERO;
    ppm = SENSORS_MQ135_PARAMETER_A * pow(ratio, -SENSORS_MQ135_PARAMETER_B); // Calculate PPM with formula
  }
#endif
  return ppm; // Return calculated PPM or invalid value
}

#ifdef SENSORS_MQ135_CALIBRATION_ENABLED
float mq135_calculateResistanceForCalibration()
{
  float calculated_resistance = CALIBRATION_INVALID_VALUE; // If analog read is not valid

  int sensor_analog_reading = analogRead(SENSORS_MQ135_PIN_ANALOG);
  if(MQ135_ANALOG_INPUT_MIN <= sensor_analog_reading && MQ135_ANALOG_INPUT_MAX >= sensor_analog_reading) // Check for valid analog read
  {
    if(MQ135_ANALOG_INPUT_MIN == sensor_analog_reading)
    {
      sensor_analog_reading = MQ135_ANALOG_INPUT_MIN_VALID; // To avoid division by 0
    }
    calculated_resistance = (((float)MQ135_ANALOG_INPUT_MAX / sensor_analog_reading) - 1) * MQ135_LOAD_RESISTANCE_VAL; // Calculate resistance
  }
  return calculated_resistance;
}
#endif

