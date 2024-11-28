#ifndef MQ135_H
#define MQ135_H

#include <Arduino.h>

#define MQ135_CALIBRATION_ENABLED

#define MQ135_PIN_ANALOG                    (A0)
#define MQ135_LOAD_RESISTANCE_VAL           (10000) //Load resistance in ohms which is connected to from analog output of sensor to ground
#define MQ135_ANALOG_INPUT_MAX              (1023)
#define MQ135_ANALOG_INPUT_MIN              (0)
#define MQ135_PPM_MINIMUM                   (0)
#define MQ135_PPM_MAXIMUM                   (10000)

//#define MQ135_PARAMETER_A     
//#define MQ135_PARAMETER_B     
//#define MQ135_R_ZERO        


typedef struct
{
  boolean success;
  float value;
}mq135_ppm_reading_t;


void mq135_init();
mq135_ppm_reading_t mq135_readPPM();
#ifdef MQ135_CALIBRATION_ENABLED
  void mq135_calculateResistanceForCalibration();
#endif

#endif