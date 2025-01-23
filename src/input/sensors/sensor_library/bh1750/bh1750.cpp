#include "bh1750.h"

static BH1750 lightMeter;

bool bh1750_init()
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