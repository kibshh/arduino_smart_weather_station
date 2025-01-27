#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

#include <Arduino.h>
#include "../sensor_metadata/sensor_metadata.h"


typedef struct
{
    const char* sensor_type;
    const char* measurement_unit;
    uint8_t sensor_id; // Unique identifier for the sensor. Used to map the sensor in configurations or operations.
    uint8_t measurement_type;
    uint8_t accuracy;
    uint8_t display_num_of_letters;
    bool success_status;
} sensor_interface_ts;

sensor_interface_ts sensor_interface_getSensorFromCatalog(uint8_t id);

/**
 * @brief Retrieves the number of configured sensors.
 *
 * This function calculates and returns the total number of configured 
 * sensors by evaluating the size of the configuration array. Returns 
 * a default value if no sensors are configured.
 *
 * @return size_t The number of configured sensors or a default value 
 * if none are configured.
 */
size_t sensors_getSensorsLen();

/**
 * @brief Converts a sensor index to its corresponding sensor ID.
 *
 * This function takes an index within the sensor configuration array and 
 * retrieves the corresponding sensor ID. Returns an invalid ID if the 
 * index is out of bounds or no sensors are configured.
 *
 * @param index The index of the sensor in the configuration array.
 * @return uint8_t The sensor ID corresponding to the index, or an invalid ID if the index is invalid.
 */
uint8_t sensors_sensorIndexToId(uint8_t index);

#endif