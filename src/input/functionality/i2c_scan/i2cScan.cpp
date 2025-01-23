#include "i2cScan.h"

i2cScan_return_ts i2cScan_getReading(uint8_t device_address)
{
  i2cScan_return_ts return_data;
  return_data.i2cScan_reading.update_i2c_address = i2cScan_updateNextAddress;
  return_data.i2cScan_reading.current_i2c_addr = I2CSCAN_CURRENT_ADDRESS_SEARCH_BEGIN;

  if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES == device_address)
  {
    // Find all I2C addresses on the bus
    return_data = i2cScan_scanForAddresses();
    return_data.i2cScan_reading.device_address = device_address;
  }
  else if(device_address >= I2CSCAN_I2C_ADDRESS_MIN && device_address <= I2CSCAN_I2C_ADDRESS_MAX)
  {
    // Find status of the I2C device with specific address
    return_data = i2cScan_checkDeviceStatus(device_address);
    return_data.i2cScan_reading.device_address = device_address;
  }
  else
  {
    return_data.error_code = ERROR_CODE_I2C_SCAN_INVALID_ADDRESS_PARAMETER;
  }
  return return_data;
}

i2cScan_return_ts i2cScan_scanForAddresses()
{
  i2cScan_return_ts return_data;
  return_data.error_code = ERROR_CODE_I2C_SCAN_SCANNING_NOT_FINISHED;
  // Set all the bits to 0
  memset(return_data.i2cScan_reading.addresses, 0, sizeof(return_data.i2cScan_reading.addresses));

  uint8_t transmission_result = I2CSCAN_TRANSMISSION_RESULT_SUCCESS;
  uint8_t address;

  // Iterate through all the possible I2C addresses for 7-bit addressing
  for (address = I2CSCAN_I2C_ADDRESS_MIN; address <= I2CSCAN_I2C_ADDRESS_MAX; address++) 
  {
    // Try to contact the address and capture the result
    Wire.beginTransmission(address);
    transmission_result = Wire.endTransmission();

    if(I2CSCAN_TRANSMISSION_RESULT_SUCCESS == transmission_result)
    {
      // Set the bit corresponding to this address in the addresses array
      return_data.i2cScan_reading.addresses[address / BITS_IN_BYTE] |= (1 << (address % BITS_IN_BYTE));
    }
  }
  // The loop completed and every I2C address is tried out
  if(I2CSCAN_I2C_ADDRESS_MAX < address)
  {
    return_data.error_code = ERROR_CODE_NO_ERROR;
  }
  return return_data;
}

i2cScan_return_ts i2cScan_checkDeviceStatus(uint8_t address)
{
  i2cScan_return_ts return_data;
  return_data.error_code = ERROR_CODE_NO_ERROR;
  uint8_t transmission_result = I2CSCAN_TRANSMISSION_RESULT_SUCCESS;

  // Try to contact the address and capture the result
  Wire.beginTransmission(address);
  transmission_result = Wire.endTransmission();

  // Check if the transmission result is valid
  if(I2CSCAN_TRANSMISSION_RESULT_SUCCESS == transmission_result || 
     I2CSCAN_TRANSMISSION_RESULT_TOOLONG == transmission_result ||
     I2CSCAN_TRANSMISSION_RESULT_NACKADR == transmission_result ||
     I2CSCAN_TRANSMISSION_RESULT_NACKDAT == transmission_result ||
     I2CSCAN_TRANSMISSION_RESULT_UNKNOWN == transmission_result)
  {
    return_data.i2cScan_reading.single_device_status = transmission_result;
  }
  else
  {
    return_data.error_code = ERROR_CODE_I2C_SCAN_ERROR_READING_DEVICE_STATUS;
  }

  return return_data;
}

bool i2cScan_updateNextAddress(i2cScan_reading_ts *i2cScan_data)
{
  uint8_t current_address = i2cScan_data->current_i2c_addr;
  bool next_address_is_found = I2CSCAN_ADDRESS_NOT_FOUND;

  // Iterate through all the possible I2C addresses for starting from current address
  for (uint8_t address = current_address; address <= I2CSCAN_I2C_ADDRESS_MAX; address++) 
  {
    if(BIT_SET == ((i2cScan_data->addresses[address / BITS_IN_BYTE] >> (address % BITS_IN_BYTE)) & BIT_SET))
    {
      i2cScan_data->current_i2c_addr = address;
      next_address_is_found = I2CSCAN_ADDRESS_FOUND;
      break;
    }
  }

  if(I2CSCAN_ADDRESS_NOT_FOUND == next_address_is_found)
  {
    i2cScan_data->current_i2c_addr = I2CSCAN_CURRENT_ADDRESS_SEARCH_BEGIN;
  }

  return next_address_is_found;
}