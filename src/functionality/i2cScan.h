#ifndef I2CSCAN_H
#define I2CSCAN_H

#include <Arduino.h>
#include <Wire.h>
#include "../error_manager/error_manager.h"


#define I2CSCAN_I2C_ADDRESS_MIN     (1u)
#define I2CSCAN_I2C_ADDRESS_MAX     (127u)

#define I2CSCAN_ERRORCODE_SUCCESS   (0u) //The transmission to the I2C device was successful, and no error occurred.
#define I2CSCAN_ERRORCODE_TOOLONG   (1u) //Data too long to fit in transmit buffer
#define I2CSCAN_ERRORCODE_NACKADR   (2u) //Received NACK on transmit of address
#define I2CSCAN_ERRORCODE_NACKDAT   (3u) //Received NACK on transmit of data
#define I2CSCAN_ERRORCODE_UNKNOWN   (4u) //There was some other error during communication, such as a loss of arbitration or an issue with the bus itself

#define I2CSCAN_NO_DEVICES_FOUND    (0u)

#define I2CSCAN_I2CSCAN_INTERVAL_MS (2000)


void i2cScan_scanForAdress();


#endif