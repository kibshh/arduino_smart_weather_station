#ifndef I2C_SCANNER_H
#define I2C_SCANNER_H

#include "../../project_configs/project_configs.h"

#define I2C_SCANNER_I2C_ADDRESS_MIN     (uint8_t)(1u)
#define I2C_SCANNER_I2C_ADDRESS_MAX     (uint8_t)(127u)

#define I2C_SCANNER_ERRORCODE_SUCCESS   (uint8_t)(0u) /* The transmission to the I2C device was successful, and no error occurred. */
#define I2C_SCANNER_ERRORCODE_TOOLONG   (uint8_t)(1u) /* Data too long to fit in transmit buffer */
#define I2C_SCANNER_ERRORCODE_NACKADR   (uint8_t)(2u) /* Received NACK on transmit of address */
#define I2C_SCANNER_ERRORCODE_NACKDAT   (uint8_t)(3u) /* Received NACK on transmit of data */
#define I2C_SCANNER_ERRORCODE_UNKNOWN   (uint8_t)(4u) /* There was some other error during communication, such as a loss of arbitration or an issue with the bus itself */

#define I2C_SCANNER_NO_DEVICES_FOUND    (uint8_t)(0u)

bool I2cScanner_ReadConnectedI2cDevices();


#endif