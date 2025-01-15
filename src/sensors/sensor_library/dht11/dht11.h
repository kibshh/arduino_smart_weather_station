#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>
#include "../../sensorsconfig.h"

void dht11_init();
float dht11_readTemperature();
float dht11_readHumidity();

#endif