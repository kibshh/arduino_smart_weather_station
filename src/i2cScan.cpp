#include "i2cScan.h"

void i2cScan_scanForAdress()
{
  byte num_of_devices = I2CSCAN_NO_DEVICES_FOUND;
  byte error, address;

  Serial.println("Scanning...");
  for (address = I2CSCAN_I2C_ADDRESS_MIN; address < I2CSCAN_I2C_ADDRESS_MAX; address++) 
  {
    // The I2C scanner will try to contact every address between 1 and 127
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if(I2CSCAN_ERRORCODE_SUCCESS == error)
    {
      Serial.print("I2C device found at address ");
      Serial.println(address, HEX);
      num_of_devices++;
    }
  }

  if (num_of_devices == I2CSCAN_NO_DEVICES_FOUND) 
  {
    Serial.println("No I2C devices found\n");
  } 
  else 
  {
    Serial.println("Done\n");
  }
}