#include "dht11.h"

/* STATIC GLOBAL VARIABLES */
static DHT dht(SENSORS_DHT11_PIN, DHT11);
/* *************************************** */

/* EXPORTED FUNCTIONS */
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
/* *************************************** */