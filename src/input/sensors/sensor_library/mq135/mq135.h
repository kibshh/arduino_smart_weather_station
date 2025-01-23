#ifndef MQ135_H
#define MQ135_H

#include <Arduino.h>
#include "../../sensorsconfig.h"


#define MQ135_LOAD_RESISTANCE_VAL           (10000) //Load resistance in ohms which is connected to from analog output of sensor to ground
#define MQ135_ANALOG_INPUT_MAX              (1023)
#define MQ135_ANALOG_INPUT_MIN              (0)      

void mq135_init();
float mq135_readPPM();
#ifdef SENSORS_MQ135_CALIBRATION_ENABLED
float mq135_calculateResistanceForCalibration();
#endif

#endif