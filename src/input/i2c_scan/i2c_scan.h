#ifndef I2C_SCAN_H
#define I2C_SCAN_H

#include <Arduino.h>
#include <Wire.h>
#include "../input_types.h"

#define I2C_SCAN_ADDRESS_FOUND                 (bool)(true)

#define I2C_SCAN_ADDRESS_NOT_FOUND             (bool)(false)

/**
 * This macro is used to indicate that no address update function is assigned.
 * This is default if scan for all addresses is NOT called.
 */
#define I2C_SCAN_NO_ADDRESS_UPDATE_FUNCTION    (nullptr)

/* The minimum 7-bit I2C address to scan. */
#define I2C_SCAN_I2C_ADDRESS_MIN               (uint8_t)(1u)

/* The maximum 7-bit I2C address to scan. */
#define I2C_SCAN_I2C_ADDRESS_MAX               (uint8_t)(127u)

/* Fixes the problem of iterating over the same I2C address */
#define I2C_SCAN_OFFSET_FOR_NEXT_ADDR          (uint8_t)(1u)
#define I2C_SCAN_STARTING_ADDRESS              (uint8_t)(I2C_SCAN_I2C_ADDRESS_MIN - I2C_SCAN_OFFSET_FOR_NEXT_ADDR)

/**
 * @brief Scans the I2C bus or checks the status of a specific device.
 * 
 * Depending on the input, this function either:
 * 1. Scans all I2C addresses (`I2C_SCAN_MODE_SCAN_FOR_ALL_DEVICES`) and marks detected devices.
 * 2. Checks the status of a device at a specific 7-bit address (1–127).
 * 
 * @param device_address Address to check or `I2C_SCAN_MODE_SCAN_FOR_ALL_DEVICES` for a full scan.
 * 
 * @return i2c_scan_return_ts
 *         - `i2c_scan_reading`: Contains detected devices or single device status.
 *         - `error_code`: Indicates success or specific errors.
 * 
 * @note Ensure the I2C bus is initialized before calling.
 */
i2c_scan_return_ts i2c_scan_getReading(uint8_t device_address);

#endif