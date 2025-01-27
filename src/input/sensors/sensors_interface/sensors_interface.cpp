#include "sensors_interface.h"

sensors_interface_metadata sensors_interface_getSensorMetadata(uint8_t id)
{
    sensors_interface_metadata metadata_return;
    metadata_return.success_status = SENSORS_INTERFACE_STATUS_FAILED; // Default failure status

    // Attempt to retrieve the sensor metadata from the catalog
    if(SENSORS_METADATA_RETRIEVE_SUCCESS == sensors_metadata_getSensorFromCatalog(id, &metadata_return.metadata))
    {
        metadata_return.success_status = SENSORS_INTERFACE_STATUS_SUCCESS;
    }
    return metadata_return;
}

size_t sensors_interface_getSensorsLen()
{
    return sensors_metadata_getSensorsLen(); // Use the function from Metadata layer
}

uint8_t sensors_interface_sensorIndexToId(uint8_t index) 
{
    return sensors_metadata_sensorIndexToId(index);
}