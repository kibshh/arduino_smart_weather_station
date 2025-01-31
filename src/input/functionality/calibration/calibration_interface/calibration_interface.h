#ifndef CALIBRATION_INTERFACE_H
#define CALIBRATION_INTERFACE_H

#include <Arduino.h>
#include "../calibration_metadata/calibration_metadata.h"

/**
 * @file calibration_interface.h
 * @brief Interface component for calibration metadata encapsulation.
 *
 * This file provides an abstraction layer for accessing calibration metadata 
 * and information, ensuring encapsulation of underlying implementation details. 
 * It exposes functions for retrieving calibration metadata, mapping calibration indexes 
 * to sensor IDs, and obtaining the number of configured calibrations. Other components 
 * interact with the calibration metadata exclusively through this interface, 
 * promoting modularity and reducing dependencies on internal structures.
 */

// Status codes indicating the calibration interface operation success or failure
#define CALIBRATION_INTERFACE_STATUS_FAILED     (false)
#define CALIBRATION_INTERFACE_STATUS_SUCCESS    (true)

// Indicates that no calibrations are configured.
#define CALIBRATION_INTERFACE_NO_CALIBRATIONS_CONFIGURED (CALIBRATION_METADATA_NO_CALIBS_CONFIGURED)

/**
 * @brief Structure representing the calibration metadata interface.
 * 
 * Contains metadata information for a calibration and the status of the operation
 * (whether metadata retrieval was successful or not).
 */
typedef struct
{
    calibration_metadata_ts metadata;         // Metadata details for a calibration
    bool success_status;                      // Status of the metadata retrieval operation
} calibration_interface_metadata_ts;

/**
 * @brief Retrieves calibration metadata for a given sensor ID from the metadata catalog.
 * 
 * This function attempts to retrieve the metadata for a calibration by sensor ID.
 * If the calibration is found in the catalog, the metadata is returned along 
 * with a success status. If the calibration is not found, the success status 
 * indicates failure.
 * 
 * @param id The unique identifier of the calibration to retrieve.
 * @return calibration_interface_metadata_ts A structure containing the calibration metadata 
 *                                    and success status.
 */
calibration_interface_metadata_ts calibration_interface_getCalibrationMetadata(uint8_t id);

/**
 * @brief Returns the number of configured calibrations.
 *
 * This function fetches the total count of configured calibrations 
 * from the metadata catalog.
 *
 * @return size_t The number of calibrations or a default value if none.
 */
size_t calibration_interface_getCalibrationsLen();

/**
 * @brief Gets the sensor ID for a given calibration array index.
 *
 * Retrieves the sensor ID for the specified index. Returns an invalid ID if the index is out of bounds.
 *
 * @param index Index of the calibration.
 * @return uint8_t Sensor ID or invalid ID if the index is invalid.
 */
uint8_t calibration_interface_calibrationIndexToId(uint8_t index);


#endif