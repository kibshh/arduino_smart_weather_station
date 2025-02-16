#ifndef SENSORS_INTERFACE_H
#define SENSORS_INTERFACE_H

#include <Arduino.h>
#include "sensors_metadata/sensors_metadata.h"

/**
 * @file sensors_interface.h
 * @brief Interface component for sensor metadata encapsulation.
 *
 * This file provides an abstraction layer for accessing sensor metadata 
 * and information, ensuring encapsulation of underlying implementation details. 
 * It exposes functions for retrieving sensor metadata, mapping sensor indices 
 * to IDs, and obtaining the number of configured sensors. Other components 
 * interact with the sensor metadata exclusively through this interface, 
 * promoting modularity and reducing dependencies on internal structures.
 */

/* Status codes indicating the sensor interface operation success or failure */
#define SENSORS_INTERFACE_STATUS_FAILED     (false)
#define SENSORS_INTERFACE_STATUS_SUCCESS    (true)

/* Indicates that no sensors are configured */
#define SENSORS_INTERFACE_NO_SENSORS_CONFIGURED (SENSORS_METADATA_NO_SENSORS_CONFIGURED)

/**
 * @brief Structure representing the sensor metadata interface.
 * 
 * Contains metadata information for a sensor and the status of the operation
 * (whether metadata retrieval was successful or not).
 */
typedef struct
{
    sensors_metadata_catalog_ts metadata; /* Metadata details for a sensor */
    bool success_status;                  /* Status of the metadata retrieval operation */
} sensors_interface_metadata_ts;

/**
 * @brief Retrieves sensor metadata for a given sensor ID from the metadata catalog.
 * 
 * This function attempts to retrieve the metadata for a sensor by its ID.
 * If the sensor is found in the catalog, the metadata is returned along 
 * with a success status. If the sensor is not found, the success status 
 * indicates failure.
 * 
 * @param id The unique identifier of the sensor to retrieve.
 * @return sensors_interface_metadata_ts A structure containing the sensor metadata 
 *                                    and success status.
 */
sensors_interface_metadata_ts sensors_interface_getSensorMetadata(uint8_t id);

/**
 * @brief Returns the number of configured sensors.
 *
 * This function fetches the total count of configured sensors 
 * from the metadata catalog.
 *
 * @return size_t The number of sensors or a default value if none.
 */
size_t sensors_interface_getSensorsLen();

/**
 * @brief Gets the sensor ID for a given index.
 *
 * Retrieves the sensor ID for the specified index. Returns an invalid ID if the index is out of bounds.
 *
 * @param index Index of the sensor.
 * @return uint8_t Sensor ID or invalid ID if the index is invalid.
 */
uint8_t sensors_interface_sensorIndexToId(uint8_t index);

#endif