#include "bh1750.h"

/* STATIC GLOBAL VARIABLES */
static BH1750 lightMeter;
/* *************************************** */

/* EXPORTED FUNCTIONS */
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
/* *************************************** */