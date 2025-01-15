#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>
#include "../../sensorsconfig.h"

#define MQ7_CALIBRATION_ENABLED

#define MQ7_PIN_PWM_HEATER                (9u) //pwm pin
#define MQ7_LOAD_RESISTANCE_VAL           (10000) //Load resistance in ohms which is connected to from analog output of sensor to ground
#define MQ7_ANALOG_INPUT_MAX              (1023u)
#define MQ7_NUM_OF_MEASUREMENTS_CALIB     (50u) //fore more precise resistance value
#define MQ7_CALIBRATION_DELAY             (100u)
#define MQ7_CALIBRATION_TIME              ((MQ7_CALIBRATION_DELAY) * (MQ7_NUM_OF_MEASUREMENTS_CALIB))
#define MQ7_HEATER_LOW_TIMEOUT_MS         (90000u)           
#define MQ7_HEATER_HIGH_TIMEOUT_MS        (60000u)    
#define MQ7_5V_ANALOG_OUTPUT_HEATER       (1023u)      
#define MQ7_1_4V_ANALOG_OUTPUT_HEATER     (71u)   

#define MQ7_CALCULATION_POW_BASE_CONSTANT (10)
#define MQ7_CALCULATION_CONSTANT_1        (0.5)   //By datasheet
#define MQ7_CALCULATION_CONSTANT_2        (-0.27) //By datasheet

#define MQ7_CLEAR_AIR_FACTOR              (9.83)  //By datasheet

#define MQ7_VCC_VOLTAGE                   (5)

//#define MQ7_R_ZERO 


void mq7_init();
bool mq7_heatingCycle(bool is_heated_high); 
float mq7_readPPM();
#ifdef MQ7_CALIBRATION_ENABLED
  void mq7_calculateResistanceForCalibration();
#endif

#endif