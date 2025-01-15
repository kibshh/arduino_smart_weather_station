#ifndef BH1750_H
#define BH1750_H

#include <Arduino.h>
#include <BH1750.h>
#include "../../sensorsconfig.h"

boolean bh1750_init();
float bh1750_readLightLevel();

#endif