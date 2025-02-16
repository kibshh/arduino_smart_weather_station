#include "dht11.h"

/* STATIC GLOBAL VARIABLES */
static Adafruit_BMP280 bmp;
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