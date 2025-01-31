#include "sensors.h"

/* SENSOR FUNCTIONAL CONFIGURATION CATALOG */
/* MUST BE IN THE SAME ORDER AS THE METADATA CONFIG ARRAY IN sensors_metadata.cpp */
const sensors_functional_catalog_ts sensors_functional_catalog[] PROGMEM =
{
#ifdef DHT11_TEMPERATURE
  { 
    SENSORS_DHT11_TEMPERATURE_MIN,     
    SENSORS_DHT11_TEMPERATURE_MAX,    
    dht11_readTemperature,          
    SENSORS_NO_INDICATION_FUNCTION,  
    DHT11_TEMPERATURE   
  },
#endif
#ifdef DHT11_HUMIDITY
  {
    SENSORS_DHT11_HUMIDITY_MIN,        
    SENSORS_DHT11_HUMIDITY_MAX,       
    dht11_readHumidity,             
    SENSORS_NO_INDICATION_FUNCTION,  
    DHT11_HUMIDITY
  },
#endif  
#ifdef BMP280_PRESSURE
  {
    SENSORS_BMP280_PRESSURE_MIN,       
    SENSORS_BMP280_PRESSURE_MAX,      
    bmp280_readPressure,            
    SENSORS_NO_INDICATION_FUNCTION,  
    BMP280_PRESSURE
  },
#endif  
#ifdef BMP280_TEMPERATURE
  {
    SENSORS_BMP280_TEMPERATURE_MIN,    
    SENSORS_BMP280_TEMPERATURE_MAX,   
    bmp280_readTemperature,         
    SENSORS_NO_INDICATION_FUNCTION,  
    BMP280_TEMPERATURE
  },
#endif  
#ifdef BMP280_ALTITUDE
  { 
    SENSORS_BMP280_ALTITUDE_MIN,       
    SENSORS_BMP280_ALTITUDE_MAX,      
    bmp280_readAltitude,            
    SENSORS_NO_INDICATION_FUNCTION,  
    BMP280_ALTITUDE
  },
#endif  
#ifdef BH1750_LUMINANCE
  {
    SENSORS_BH1750_LUMINANCE_MIN,      
    SENSORS_BH1750_LUMINANCE_MAX,     
    bh1750_readLightLevel,          
    SENSORS_NO_INDICATION_FUNCTION,  
    BH1750_LUMINANCE
  },
#endif  
#ifdef MQ135_PPM
  { 
    SENSORS_MQ135_PPM_MIN,             
    SENSORS_MQ135_PPM_MAX,            
    mq135_readPPM,                  
    SENSORS_NO_INDICATION_FUNCTION,  
    MQ135_PPM
  },
#endif  
#ifdef MQ7_COPPM
  { 
    SENSORS_MQ7_PPM_MIN,               
    SENSORS_MQ7_PPM_MAX,              
    mq7_readPPM,                    
    SENSORS_NO_INDICATION_FUNCTION,  
    MQ7_COPPM
  },
#endif  
#ifdef GYML8511_UV
  { 
    SENSORS_GYML8511_UV_MIN,           
    SENSORS_GYML8511_UV_MAX,          
    gy_ml8511_readUvIntensity,      
    SENSORS_NO_INDICATION_FUNCTION,  
    GYML8511_UV
  },
#endif  
#ifdef ARDUINORAIN_RAINING
  { 
    SENSORS_INDICATION_NO_MIN,         
    SENSORS_INDICATION_NO_MAX,        
    SENSORS_NO_VALUE_FUNCTION,      
    arduino_rain_sensor_readRaining, 
    ARDUINORAIN_RAINING
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

  size_t catalog_len = sensors_interface_getSensorsLen(); // Get the length of the sensor configuration array
  if(SENSORS_INTERFACE_NO_SENSORS_CONFIGURED != catalog_len) // Check if any sensors are configured
  {
    bool is_sensor_configured = false; // Flag to check if the sensor is found
    uint8_t sensor_index = SENSORS_FIRST_SENSOR_INDEX;

    for (uint8_t index = SENSORS_FIRST_SENSOR_INDEX; index < catalog_len; index++) // Iterate through sensor configurations to find the matching sensor ID
    {
      if(pgm_read_byte(&sensors_functional_catalog[index].sensor_id) == id)
      {
        // Sensor ID is found, store it and exit the loop
        is_sensor_configured = true;
        sensor_index = index;
        break;
      }
    }
    if(SENSORS_SENSOR_CONFIGURED == is_sensor_configured) // If the sensor is configured, proceed to read its values
    {
      sensors_functional_catalog_ts current_sensor;
      memcpy_P(&current_sensor, &sensors_functional_catalog[sensor_index], sizeof(sensors_functional_catalog_ts)); // Copy the sensor configuration from program memory to a local structure
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