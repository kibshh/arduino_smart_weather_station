#include "dht11.h"

static DHT dht(SENSORS_DHT11_PIN, DHT11);

void dht11_init()
{
    dht.begin();
}

float dht11_readTemperature()
{
    return dht.readTemperature();
}

float dht11_readHumidity()
{
    return dht.readHumidity();
}