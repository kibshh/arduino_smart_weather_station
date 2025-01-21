#include "bh1750.h"

BH1750 lightMeter;

boolean bh1750_init()
{
  if(!lightMeter.begin())
  {
    return false;
  }
  return true;
}

float bh1750_readLightLevel()
{
  return lightMeter.readLightLevel();
}