#include "sensors_metadata.h"

/* SENSORS METADATA CATALOG */
/* MUST BE IN THE SAME ORDER AS THE FUNCTIONAL CONFIG ARRAY IN sensors.cpp */
const sensors_metadata_catalog_ts sensors_metadata_catalog[] PROGMEM =
{
#ifdef DHT11_TEMPERATURE
  {
    "Temperature",     
    "C",     
    DHT11_TEMPERATURE,   
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_1_DECIMAL,
    SENSORS_DISPLAY_4_LETTERS
  },
#endif
#ifdef DHT11_HUMIDITY
  {
    "Humidity",        
    "%",   
    DHT11_HUMIDITY,      
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_1_DECIMAL,
    SENSORS_DISPLAY_8_LETTERS
  },
#endif  
#ifdef BMP280_PRESSURE
  {
    "Pressure",        
    "hPa",  
    BMP280_PRESSURE,     
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_1_DECIMAL,
    SENSORS_DISPLAY_5_LETTERS
  },
#endif  
#ifdef BMP280_TEMPERATURE
  {
    "Temperature",     
    "C",   
    BMP280_TEMPERATURE,  
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_1_DECIMAL,
    SENSORS_DISPLAY_4_LETTERS
  },
#endif  
#ifdef BMP280_ALTITUDE
  {
    "Altitude",        
    "m",   
    BMP280_ALTITUDE,     
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_8_LETTERS
  },
#endif  
#ifdef BH1750_LUMINANCE
  {
    "Luminance",       
    "lx",  
    BH1750_LUMINANCE,    
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_9_LETTERS
  },
#endif  
#ifdef MQ135_PPM
  {
    "Gases PPM",       
    "",    
    MQ135_PPM,           
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_9_LETTERS
  },
#endif  
#ifdef MQ7_COPPM
  {
    "CO PPM",          
    "",    
    MQ7_COPPM,           
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_6_LETTERS
  },
#endif  
#ifdef GYML8511_UV
  {
    "UV intensity",    
    "",     
    GYML8511_UV,         
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_1_DECIMAL,
    SENSORS_DISPLAY_2_LETTERS
  },
#endif  
#ifdef ARDUINORAIN_RAINING
  {
    "Raining status",  
    "",    
    ARDUINORAIN_RAINING, 
    SENSORS_MEASUREMENT_TYPE_INDICATION,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_7_LETTERS
  },
#endif
};

bool sensors_metadata_getSensorFromCatalog(uint8_t id, sensors_metadata_catalog_ts * current_sensor)
{
    bool success_status = SENSORS_METADATA_RETRIEVE_FAILED;
    size_t num_of_sensors = sensors_metadata_getSensorsLen();
    uint8_t current_id = INVALID_SENSOR_ID;

    // Loop through all sensors in the catalog
    for (uint8_t index = SENSORS_METADATA_FIRST_SENSOR_INDEX; index < num_of_sensors; index++)
    {
        current_id = pgm_read_byte(&sensors_metadata_catalog[index].sensor_id); // Read from program memory
        if(id == current_id)
        {
             // Copy the sensor configuration from program memory to the provided structure
            memcpy_P(current_sensor, &sensors_metadata_catalog[index], sizeof(sensors_metadata_catalog_ts));
            success_status = SENSORS_METADATA_RETRIEVE_SUCCESS; // Mark as successful
            break;
        }
    }
    return success_status;
}

size_t sensors_metadata_getSensorsLen()
{
  size_t sensors_len = SENSORS_METADATA_NO_SENSORS_CONFIGURED; // Default value in case of no sensors configured
  if (SENSORS_METADATA_NO_SENSORS_CONFIGURED < sizeof(sensors_metadata_catalog)) // Ensure catalog is not empty
  {
    sensors_len = sizeof(sensors_metadata_catalog) / sizeof(sensors_metadata_catalog[SENSORS_METADATA_FIRST_SENSOR_INDEX]);
  }
  return sensors_len;
}

uint8_t sensors_metadata_sensorIndexToId(uint8_t index) 
{
  uint8_t sensor_id = INVALID_SENSOR_ID; // Default sensor ID in case index is out of bounds or there are no sensors configured
  size_t num_of_sensors = sensors_metadata_getSensorsLen();
  if(index < num_of_sensors && SENSORS_METADATA_FIRST_SENSOR_INDEX <= index)
  {
    sensor_id = pgm_read_byte(&sensors_metadata_catalog[index].sensor_id); // Convert to sensor ID
  }
  return sensor_id;
}

