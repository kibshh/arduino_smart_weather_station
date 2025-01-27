#include "sensors.h"

/* SENSOR CONFIGURATION CATALOG */
const sensors_catalog_ts sensors_catalog[] PROGMEM =
{
#ifdef DHT11_TEMPERATURE
  {
    "Temperature",     
    "C",   
    SENSORS_DHT11_TEMPERATURE_MIN,     
    SENSORS_DHT11_TEMPERATURE_MAX,    
    dht11_readTemperature,          
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_DHT11_HUMIDITY_MIN,        
    SENSORS_DHT11_HUMIDITY_MAX,       
    dht11_readHumidity,             
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_BMP280_PRESSURE_MIN,       
    SENSORS_BMP280_PRESSURE_MAX,      
    bmp280_readPressure,            
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_BMP280_TEMPERATURE_MIN,    
    SENSORS_BMP280_TEMPERATURE_MAX,   
    bmp280_readTemperature,         
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_BMP280_ALTITUDE_MIN,       
    SENSORS_BMP280_ALTITUDE_MAX,      
    bmp280_readAltitude,            
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_BH1750_LUMINANCE_MIN,      
    SENSORS_BH1750_LUMINANCE_MAX,     
    bh1750_readLightLevel,          
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_MQ135_PPM_MIN,             
    SENSORS_MQ135_PPM_MAX,            
    mq135_readPPM,                  
    SENSORS_NO_INDICATION_FUNCTION,  
    MQ135_PPM,           
    SENSORS_MEASUREMENT_TYPE_VALUE,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_9_LETTERS
  },
#endif  
#ifdef MQ7_COPPM
  {
    "CO PPM",          
    "",,   
    SENSORS_MQ7_PPM_MIN,               
    SENSORS_MQ7_PPM_MAX,              
    mq7_readPPM,                    
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_GYML8511_UV_MIN,           
    SENSORS_GYML8511_UV_MAX,          
    gy_ml8511_readUvIntensity,      
    SENSORS_NO_INDICATION_FUNCTION,  
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
    SENSORS_INDICATION_NO_MIN,         
    SENSORS_INDICATION_NO_MAX,        
    SENSORS_NO_VALUE_FUNCTION,      
    arduino_rain_sensor_readRaining, 
    ARDUINORAIN_RAINING, 
    SENSORS_MEASUREMENT_TYPE_INDICATION,
    SENSORS_DISPLAY_0_DECIMALS,
    SENSORS_DISPLAY_7_LETTERS
  },
#endif
};

error_manager_error_code_te sensors_init()
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

#if defined(DHT11_TEMPERATURE) || defined(DHT11_HUMIDITY)
  dht11_init();
#endif
#if defined(BMP280_PRESSURE) || defined(BMP280_TEMPERATURE) || defined(BMP280_ALTITUDE)
  if(!bmp280_init())
  {
    error_code = ERROR_CODE_SENSORS_INIT_BMP280;
  }
#endif
#if defined(BH1750_LUMINANCE)
  if(!bh1750_init())
  {
    error_code = ERROR_CODE_SENSORS_INIT_BH1750;
  }
#endif
#if defined(MQ135_PPM)
  mq135_init();
#endif
#if defined(MQ7_COPPM)
  mq7_init();
#endif
#if defined(GYML8511_UV)
  gy_ml8511_init();
#endif
#if defined(ARDUINORAIN_RAINING)
  arduino_rain_sensor_init();
#endif

  return error_code;
}

sensor_return_ts sensors_getReading(uint8_t id)
{
  sensor_return_ts return_data;
  return_data.error_code = ERROR_CODE_SENSORS_NO_SENSORS_CONFIGURED; // Set default error code to indicate no sensors are configured

  size_t sensors_config_len = sensors_getSensorsLen(); // Get the length of the sensor configuration array
  if(SENSORS_NO_SENSORS_CONFIGURED != sensors_config_len) // Check if any sensors are configured
  {
    bool is_sensor_configured = false; // Flag to check if the sensor is found
    uint8_t sensor_index = SENSORS_MINIMUM_INDEX;

    for (uint8_t index = SENSORS_MINIMUM_INDEX; index < sensors_config_len; index++) // Iterate through sensor configurations to find the matching sensor ID
    {
      if(pgm_read_byte(&sensor_sensors_config[index].sensor_id) == id)
      {
        // Sensor ID is found, store it and exit the loop
        is_sensor_configured = true;
        sensor_index = index;
        break;
      }
    }
    if(true == is_sensor_configured) // If the sensor is configured, proceed to read its values
    {
      sensor_sensors_config_t current_sensor;
      memcpy_P(&current_sensor, &sensor_sensors_config[sensor_index], sizeof(sensor_sensors_config_t)); // Copy the sensor configuration from program memory to a local structure
      return_data.sensor_reading.sensor_id = id;

      if(SENSORS_NO_VALUE_FUNCTION != current_sensor.sensor_value_function) // Check if the sensor has a value function defined
      {
        return_data.sensor_reading.measurement_type_switch = SENSORS_MEASUREMENT_TYPE_VALUE;
        return_data.sensor_reading.value = current_sensor.sensor_value_function();
        if(!isnan(return_data.sensor_reading.value)) // Check if the value is valid
        {
          // Check if the value is within the acceptable range
          if(return_data.sensor_reading.value >= current_sensor.min_value && return_data.sensor_reading.value <= current_sensor.max_value)
          {
            return_data.error_code = ERROR_CODE_NO_ERROR; // No error, value is valid
          }
          else
          {
            return_data.error_code = ERROR_CODE_SENSORS_ABNORMAL_VALUE; // Value is outside the range
          }
        }
        else
        {
          return_data.error_code = ERROR_CODE_SENSORS_INVALID_VALUE_FROM_SENSOR; // Sensor returned an invalid value
        }
      }
      else if(SENSORS_NO_INDICATION_FUNCTION != current_sensor.sensor_indication_function) // Check if the sensor has an indication function defined
      {
        return_data.sensor_reading.measurement_type_switch = SENSORS_MEASUREMENT_TYPE_INDICATION;
        return_data.sensor_reading.indication = current_sensor.sensor_indication_function();
        return_data.error_code = ERROR_CODE_NO_ERROR;
      }
      else
      {
        return_data.error_code = ERROR_CODE_SENSORS_MEASUREMENT_TYPE_MISSING_FUNCTION; // Error: No function defined for the sensor's measurement type
      }
    }
    else
    {
      return_data.error_code = ERROR_CODE_SENSORS_SENSOR_NOT_FOUND; // Error: Sensor ID not found in the configuration
    }
  }
  return return_data;
}

void sensors_loop(unsigned long current_millis)
{
#ifdef MQ7_COPPM
  mq7_heatingCycle(current_millis);
#ifdef SENSORS_MQ7_CALIBRATION_ENABLED
  mq7_calibratingLoopFunction(current_millis);
#endif
#endif
}

sensor_interface_ts sensor_interface_getSensorFromCatalog(uint8_t id)
{
  sensor_interface_ts return_interface;
  return_interface.success_status = false;

  sensors_catalog_ts current_sensor_from_catalog;
  if(true == sensors_getSensorFromCatalog(id, &current_sensor_from_catalog))
  {
    // Fill the return_interface structure with data from current_sensor_from_catalog
    return_interface.sensor_type = current_sensor_from_catalog.sensor_type;
    return_interface.measurement_unit = current_sensor_from_catalog.measurement_unit;
    return_interface.sensor_id = current_sensor_from_catalog.sensor_id;
    return_interface.measurement_type = current_sensor_from_catalog.measurement_type;
    return_interface.accuracy = current_sensor_from_catalog.accuracy;
    return_interface.display_num_of_letters = current_sensor_from_catalog.display_num_of_letters;
    return_interface.success_status = true; // Indicate success
  }
  return return_interface;
}