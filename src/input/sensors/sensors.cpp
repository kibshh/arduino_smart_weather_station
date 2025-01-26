#include "sensors.h"

/* Sensor configuration array */
const sensor_sensors_config_t sensor_sensors_config[] PROGMEM =
{
#ifdef DHT11_TEMPERATURE
  {SENSORS_DHT11_TEMPERATURE_MIN,     SENSORS_DHT11_TEMPERATURE_MAX,    DHT11_TEMPERATURE,      dht11_readTemperature,          SENSORS_NO_INDICATION_FUNCTION},
#endif
#ifdef DHT11_HUMIDITY
  {SENSORS_DHT11_HUMIDITY_MIN,        SENSORS_DHT11_HUMIDITY_MAX,       DHT11_HUMIDITY,         dht11_readHumidity,             SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef BMP280_PRESSURE
  {SENSORS_BMP280_PRESSURE_MIN,       SENSORS_BMP280_PRESSURE_MAX,      BMP280_PRESSURE,        bmp280_readPressure,            SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef BMP280_TEMPERATURE
  {SENSORS_BMP280_TEMPERATURE_MIN,    SENSORS_BMP280_TEMPERATURE_MAX,   BMP280_TEMPERATURE,     bmp280_readTemperature,         SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef BMP280_ALTITUDE
  {SENSORS_BMP280_ALTITUDE_MIN,       SENSORS_BMP280_ALTITUDE_MAX,      BMP280_ALTITUDE,        bmp280_readAltitude,            SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef BH1750_LUMINANCE
  {SENSORS_BH1750_LUMINANCE_MIN,      SENSORS_BH1750_LUMINANCE_MAX,     BH1750_LUMINANCE,       bh1750_readLightLevel,          SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef MQ135_PPM
  {SENSORS_MQ135_PPM_MIN,             SENSORS_MQ135_PPM_MAX,            MQ135_PPM,              mq135_readPPM,                  SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef MQ7_COPPM
  {SENSORS_MQ7_PPM_MIN,               SENSORS_MQ7_PPM_MAX,              MQ7_COPPM,              mq7_readPPM,                    SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef GYML8511_UV
  {SENSORS_GYML8511_UV_MIN,           SENSORS_GYML8511_UV_MAX,          GYML8511_UV,            gy_ml8511_readUvIntensity,      SENSORS_NO_INDICATION_FUNCTION},
#endif  
#ifdef ARDUINORAIN_RAINING
  {SENSORS_INDICATION_NO_MIN,         SENSORS_INDICATION_NO_MAX,        ARDUINORAIN_RAINING,    SENSORS_NO_VALUE_FUNCTION,      arduino_rain_sensor_readRaining},
#endif
};


error_manager_error_code_te sensors_init()
{
  error_manager_error_code_te error_code = ERROR_CODE_NO_ERROR;

#if defined(DHT11_TEMPERATURE) || defined(DHT11_HUMIDITY)
  dht11_init();
#endif
#if defined(BMP280_PRESSURE) || defined(BMP280_TEMPERATURE) || defined(BMP280_ALTITUDE)
  (void)bmp280_init();
#endif
#if defined(BH1750_LUMINANCE)
  (void)bh1750_init();
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

size_t sensors_getSensorsLen()
{
  size_t sensors_len = SENSORS_NO_SENSORS_CONFIGURED; // Default value in case of no sensors configured
  if (SENSORS_NO_SENSORS_CONFIGURED != sizeof(sensor_sensors_config))
  {
    sensors_len = sizeof(sensor_sensors_config) / sizeof(sensor_sensors_config[SENSORS_FIRST_SENSOR_INDEX]);
  }
  return sensors_len;
}

uint8_t sensors_sensorIndexToId(uint8_t index) 
{
  uint8_t sensor_id = INVALID_SENSOR_ID; // Default sensor ID in case index is out of bounds or there are no sensors configured
  size_t num_of_sensors = sensors_getSensorsLen();
  if(index < num_of_sensors && SENSORS_MINIMUM_INDEX <= index)
  {
    sensor_id = pgm_read_byte(&sensor_sensors_config[index].sensor_id); // Convert to sensor ID
  }
  return sensor_id;
}
