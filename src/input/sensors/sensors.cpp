#include "sensors.h"

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
  {SENSORS_INDICATION_NO_MIN,         SENSORS_INDICATION_NO_MAX,        ARDUINORAIN_RAINING,    SENSORS_NO_VALUE_FUNCTION,      rainsensor_readRaining        }
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
  return_data.error_code = ERROR_CODE_SENSORS_NO_SENSORS_CONFIGURED;

  if(SENSORS_NO_SENSORS_CONFIGURED != sizeof(sensor_sensors_config))
  {
    size_t sensor_config_len = sizeof(sensor_sensors_config) / sizeof(sensor_sensors_config[0]);
    bool is_sensor_configured = false;
    uint8_t sensor_index = 0;

    for (uint8_t index = 0; index < sensor_config_len; index++)
    {
      if(pgm_read_byte(&sensor_sensors_config[index].sensor_id) == id)
      {
        is_sensor_configured = true;
        sensor_index = index;
        break;
      }
    }
    
    if(true == is_sensor_configured)
    {
      sensor_sensors_config_t current_sensor;
      memcpy_P(&current_sensor, &sensor_sensors_config[sensor_index], sizeof(sensor_sensors_config_t));
      return_data.sensor_reading.sensor_id = id;

      if(SENSORS_NO_VALUE_FUNCTION != current_sensor.sensor_value_function)
      {
        return_data.sensor_reading.measurement_type_switch = SENSORS_MEASUREMENT_TYPE_VALUE;
        return_data.sensor_reading.value = current_sensor.sensor_value_function();
        if(!isnan(return_data.sensor_reading.value))
        {
          if(return_data.sensor_reading.value > current_sensor.min_value && return_data.sensor_reading.value < current_sensor.max_value)
          {
            return_data.error_code = ERROR_CODE_NO_ERROR;
          }
          else
          {
            return_data.error_code = ERROR_CODE_SENSORS_ABNORMAL_VALUE;
          }
        }
        else
        {
          return_data.error_code = ERROR_CODE_SENSORS_INVALID_VALUE_FROM_SENSOR;
        }
      }
      else if(SENSORS_NO_INDICATION_FUNCTION != current_sensor.sensor_indication_function)
      {
        return_data.sensor_reading.measurement_type_switch = SENSORS_MEASUREMENT_TYPE_INDICATION;
        return_data.sensor_reading.indication = current_sensor.sensor_indication_function();
        return_data.error_code = ERROR_CODE_NO_ERROR;
      }
      else
      {
        return_data.error_code = ERROR_CODE_SENSORS_MEASUREMENT_TYPE_MISSING_FUNCTION;
      }
    }
    else
    {
      return_data.error_code = ERROR_CODE_SENSORS_SENSOR_NOT_FOUND;
    }
  }
  return return_data;
}

size_t sensors_getSensorsLen()
{
  size_t sensors_len = SENSORS_NO_SENSORS_CONFIGURED;
  if (SENSORS_NO_SENSORS_CONFIGURED != sizeof(sensor_sensors_config))
  {
    sensors_len = sizeof(sensor_sensors_config) / sizeof(sensor_sensors_config[0]);
  }
  return sensors_len;
}

uint8_t sensors_sensorIndexToId(uint8_t index) 
{
  uint8_t sensor_id = sensor_sensors_config[index].sensor_id;
  return sensor_id;
}
