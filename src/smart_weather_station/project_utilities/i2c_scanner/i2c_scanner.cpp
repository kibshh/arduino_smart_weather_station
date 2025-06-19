#include "i2c_scanner.h"

bool I2cScanner_ReadConnectedI2cDevices()
{
  uint8_t num_of_devices = I2C_SCANNER_NO_DEVICES_FOUND;
  uint8_t error;

  Serial.println("Scanning for I2C devices...");
  for (uint8_t address = I2C_SCANNER_I2C_ADDRESS_MIN; address < I2C_SCANNER_I2C_ADDRESS_MAX; address++) 
  {
    /* The I2C scanner will try to contact every address between 1 and 127 */
    Wire.beginTransmission(address);
    error = (uint8_t)Wire.endTransmission();

    if(I2C_SCANNER_ERRORCODE_SUCCESS == error)
    {
      Serial.print("I2C device found at address ");
      Serial.println(address, HEX);
      Serial.println("\n");
      num_of_devices++;
    }
  }

  if (num_of_devices == I2C_SCANNER_NO_DEVICES_FOUND) 
  {
    Serial.println("No I2C devices found\n");
    return false;
  } 
  else 
  {
    Serial.println("I2C scanning done\n");
    return true;
  }
}