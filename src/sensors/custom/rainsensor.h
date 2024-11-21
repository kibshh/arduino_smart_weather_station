#ifndef RAINSENSOR_H
#define RAINSENSOR_H

#include <Arduino.h>

#define RAINSENSOR_ANALOG_MEASUREMENT

#define RAINSENSOR_PIN_ANALOG    (A4)
#define RAINSENSOR_PIN_DIGITAL   (4u)

#define RAINSENSOR_ANALOG_THRESHOLD (500u)

typedef struct
{
  boolean success;
  boolean is_raining;
}rainsensor_reading_t;


void rainsensor_init();
rainsensor_reading_t rainsensor_readRaining();
#ifdef RAINSENSOR_ANALOG_MEASUREMENT
  boolean rainsensor_isRainingAnalog();
#else
  boolean rainsensor_isRainingDigital();
#endif

#endif