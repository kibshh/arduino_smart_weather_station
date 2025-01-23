#ifndef I2CSCAN_H
#define I2CSCAN_H

#include <Arduino.h>
#include <Wire.h>
#include "../../input_types.h"

#define I2CSCAN_CURRENT_ADDRESS_SEARCH_BEGIN  (0u)

#define I2CSCAN_ADDRESS_FOUND                 (true)

#define I2CSCAN_ADDRESS_NOT_FOUND             (false)

/**
 * This macro is used to indicate that no address update function is assigned.
 * This is default if scan for all addresses is NOT called.
 */
#define I2CSCAN_NO_ADDRESS_UPDATE_FUNCTION    (nullptr)

/* The minimum 7-bit I2C address to scan. */
#define I2CSCAN_I2C_ADDRESS_MIN               (1u)

/* The maximum 7-bit I2C address to scan. */
#define I2CSCAN_I2C_ADDRESS_MAX               (127u)

/**
 * @brief Scans the I2C bus or checks the status of a specific device.
 * 
 * Depending on the input, this function either:
 * 1. Scans all I2C addresses (`I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES`) and marks detected devices.
 * 2. Checks the status of a device at a specific 7-bit address (1–127).
 * 
 * @param device_address Address to check or `I2CSCAN_MODE_SCAN_FOR_ALL_DEVICES` for a full scan.
 * 
 * @return i2cScan_return_ts
 *         - `i2cScan_reading`: Contains detected devices or single device status.
 *         - `error_code`: Indicates success or specific errors.
 * 
 * @note Ensure the I2C bus is initialized before calling.
 */
i2cScan_return_ts i2cScan_getReading(uint8_t device_address);

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
i2cScan_return_ts i2cScan_scanForAddresses();

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
i2cScan_return_ts i2cScan_checkDeviceStatus(uint8_t address);

bool i2cScan_updateNextAddress(i2cScan_reading_ts *i2cScan_data);

#endif