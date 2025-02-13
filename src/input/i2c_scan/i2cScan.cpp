#include "i2cScan.h"

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief Scans the I2C bus for connected devices.
 * 
 * This function checks all 7-bit I2C addresses to detect connected devices 
 * and marks their presence in a bit field array. The result includes an error 
 * code indicating the success or failure of the scan.
 * 
 * @return i2cScan_return_ts
 *         - `i2cScan_reading`: Contains a bit field array where each bit represents 
 *           an I2C address. Bits set to `1` indicate detected devices.
 *         - `error_code`: Status of the scan operation. Possible values:
 *             - `ERROR_CODE_NO_ERROR`: Scan completed successfully.
 *             - `ERROR_CODE_I2C_SCAN_SCANNING_NOT_FINISHED`: Scan did not complete.
 * 
 * @note Ensure the I2C bus is initialized before calling this function.
 */
static i2cScan_return_ts i2cScan_scanForAddresses();

/**
 * @brief Checks the status of a specific I2C device.
 * 
 * Sends a transmission to the specified I2C address and returns the result.
 * 
 * @param address The 7-bit I2C address to check (1–127).
 * 
 * @return i2cScan_return_ts
 *         - `i2cScan_reading.single_device_status`: Indicates the transmission result:
 *             - `I2CSCAN_TRANSMISSION_RESULT_SUCCESS`: Device detected.
 *             - Other values indicate specific transmission errors.
 *         - `error_code`: 
 *             - `ERROR_CODE_NO_ERROR`: Operation successful.
 *             - `ERROR_CODE_I2C_SCAN_ERROR_READING_DEVICE_STATUS`: Invalid result or bus issue.
 * 
 * @note Ensure the I2C bus is initialized before calling this function.
 */
static i2cScan_return_ts i2cScan_checkDeviceStatus(uint8_t address);

/**
 * @brief Updates the next available I2C address from the scan data.
 *
 * This function searches for the next available I2C address starting from the current 
 * address. It checks a bitfield array (`addresses[]`) to determine if an address has 
 * been previously marked as found. If a valid address is found, it updates 
 * `current_i2c_addr` and returns `I2CSCAN_ADDRESS_FOUND`. If no address is found, 
 * it resets `current_i2c_addr` to `I2CSCAN_STARTING_ADDRESS` and returns `I2CSCAN_ADDRESS_NOT_FOUND`.
 *
 * @param[in,out] i2cScan_data Pointer to the I2C scan data structure.
 * @return `I2CSCAN_ADDRESS_FOUND` if a valid address is found, otherwise `I2CSCAN_ADDRESS_NOT_FOUND`.
 */
static bool i2cScan_updateNextAddress(i2cScan_reading_ts *i2cScan_data);
/* *************************************** */

/* EXPORTED FUNCTIONS */
i2cScan_return_ts i2cScan_getReading(uint8_t device_address)
{
  i2cScan_return_ts return_data;

  if(I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES == device_address)
  {
    // Find all I2C addresses on the bus
    return_data = i2cScan_scanForAddresses();
  }
  else if(device_address >= I2CSCAN_I2C_ADDRESS_MIN && device_address <= I2CSCAN_I2C_ADDRESS_MAX)
  {
    // Find status of the I2C device with specific address
    return_data = i2cScan_checkDeviceStatus(device_address);
  }
  else
  {
    return_data.error_code = ERROR_CODE_I2C_SCAN_INVALID_ADDRESS_PARAMETER;
  }

  return_data.i2cScan_reading.current_i2c_addr = I2CSCAN_STARTING_ADDRESS; // Because we start the loop from current address + 1
  return_data.i2cScan_reading.update_i2c_address = i2cScan_updateNextAddress;
  return_data.i2cScan_reading.device_address = device_address;

  return return_data;
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static i2cScan_return_ts i2cScan_scanForAddresses()
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

static i2cScan_return_ts i2cScan_checkDeviceStatus(uint8_t address)
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

static bool i2cScan_updateNextAddress(i2cScan_reading_ts *i2cScan_data)
{
  uint8_t current_address = i2cScan_data->current_i2c_addr;
  bool next_address_is_found = I2CSCAN_ADDRESS_NOT_FOUND;

  // Iterate through all the possible I2C addresses for starting from current address
  for (uint8_t address = current_address + I2CSCAN_OFFSET_FOR_NEXT_ADDR; address <= I2CSCAN_I2C_ADDRESS_MAX; address++) 
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
    i2cScan_data->current_i2c_addr = I2CSCAN_STARTING_ADDRESS;
  }

  return next_address_is_found;
}
/* *************************************** */