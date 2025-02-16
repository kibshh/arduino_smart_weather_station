#ifndef APP_I2C_SCAN_H
#define APP_I2C_SCAN_H

#include <Arduino.h>
#include "../app_common.h"

/* Enable the I2C scanner operation */
#define I2C_SCANER_RUN       (bool)(true)
/* Disable the I2C scanner operation, used while data is still being displayed or processed */
#define I2C_SCANER_DONT_RUN  (bool)(false)

/* Context structure to manage the state of the I2C scanner operation */
typedef struct
{
    control_input_data_ts i2c_scan_return; // Stores the result of the I2C scan operation
    bool run_i2c_scanner;                  // Flag indicating whether the I2C scanner should be run
} i2c_scan_reading_context_ts;

/**
 * @brief Periodically scans and reads I2C addresses.
 *
 * This function runs the I2C scanner periodically and updates the display or console with 
 * detected I2C addresses. If no more addresses are found, it marks the scanning process as completed.
 *
 * @param output The output destination (LCD display, serial console, etc.).
 * @param context The I2C scan reading context containing the scan data.
 * @return task_status_te - `NOT_FINISHED` if there are more addresses to process, `FINISHED` otherwise.
 */
task_status_te app_readAllI2CAddressesPeriodic(output_destination_t output, i2c_scan_reading_context_ts *context);

/**
 * @brief Reads all I2C addresses at once by performing a scan and routing the results to specified outputs.
 * 
 * This function scans all I2C addresses and attempts to update the addresses where applicable. 
 * It routes the scan results to the specified outputs (LCD display, serial console) if included. 
 * The scan is limited by the range of I2C addresses and the number of attempts to avoid infinite loops.
 * 
 * @param output The destination(s) for the I2C scan results (e.g., LCD display, serial console).
 *               Time-dependent outputs are filtered out before processing.
 * 
 * @return task_status_te Returns `FINISHED` once the I2C scan process is completed.
 * 
 * @note This function includes a safeguard to prevent infinite loops by limiting the number of address attempts.
 *       If no addresses are found or the maximum attempts are reached, the loop will exit.
 *       Error messages are generated in case of issues with fetching data or routing outputs.
 */
task_status_te app_readAllI2CAddressesAtOnce(output_destination_t output);

/**
 * @brief Reads the status of a specific I2C device based on the provided address and routes the data to specified outputs.
 * 
 * This function fetches the status of an I2C device by its address, routes the result to the desired outputs (LCD display, 
 * serial console), and performs error checks. If the provided device address is invalid, the function will not attempt to fetch 
 * data or route it to outputs.
 * 
 * @param device_address The address of the I2C device to read the status from.
 * @param output The destination(s) for the I2C device status data (LCD display, serial console).
 * 
 * @return task_status_te Returns `FINISHED` once the process is completed.
 */
task_status_te app_readI2CDeviceStatus(uint8_t device_address, output_destination_t output);

/**
 * @brief Initializes and returns a new I2C scan reading context.
 *
 * This function initializes the I2C scan reading context with default values.
 * The function pointer `update_i2c_address` is explicitly set to `I2C_SCAN_NO_ADDRESS_UPDATE_FUNCTION`,
 * which serves as the key check for determining if address updates should occur.
 *
 * @return i2c_scan_reading_context_ts - A half initialized I2C scan reading context.
 */
i2c_scan_reading_context_ts app_createI2CScanReadingContext();

#endif