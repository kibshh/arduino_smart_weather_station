#include "sensors.h"

const sensor_sensors_config_t sensor_sensors_config[] PROGMEM =
{
  {SENSORS_DHT11_TEMPERATURE_MIN,     SENSORS_DHT11_TEMPERATURE_MAX,    DHT11_TEMPERATURE,      dht11_readTemperature,          SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_DHT11_HUMIDITY_MIN,        SENSORS_DHT11_HUMIDITY_MAX,       DHT11_HUMIDITY,         dht11_readHumidity,             SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_BMP280_PRESSURE_MIN,       SENSORS_BMP280_PRESSURE_MAX,      BMP280_PRESSURE,        bmp280_readPressure,            SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_BMP280_TEMPERATURE_MIN,    SENSORS_BMP280_TEMPERATURE_MAX,   BMP280_TEMPERATURE,     bmp280_readTemperature,         SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_BMP280_ALTITUDE_MIN,       SENSORS_BMP280_ALTITUDE_MAX,      BMP280_ALTITUDE,        bmp280_readAltitude,            SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_BH1750_LUMINANCE_MIN,      SENSORS_BH1750_LUMINANCE_MAX,     BH1750_LUMINANCE,       bh1750_readLightLevel,          SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_MQ135_PPM_MIN,             SENSORS_MQ135_PPM_MAX,            MQ135_PPM,              mq135_readPPM,                  SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_MQ7_PPM_MIN,               SENSORS_MQ7_PPM_MAX,              MQ7_COPPM,              mq7_readPPM,                    SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_GYML8511_UV_MIN,           SENSORS_GYML8511_UV_MAX,          GYML8511_UV,            gy_ml8511_readUvIntensity,      SENSORS_NO_INDICATION_FUNCTION},
  {SENSORS_INDICATION_NO_MIN,         SENSORS_INDICATION_NO_MAX,        ARDUINORAIN_RAINING,    SENSORS_NO_VALUE_FUNCTION,      rainsensor_readRaining        }
};


void sensors_init()
{
  dht11_init();
  (void)bmp280_init();
  (void)bh1750_init();
  mq135_init();
  mq7_init();
  gy_ml8511_init();
}

sensor_reading_t sensors_getReading(uint8_t id, uint8_t measurement_type)
{
  sensor_reading_t reading;
  reading.error_code = ERROR_CODE_SENSORS_NO_SENSORS_CONFIGURED;

  if(SENSORS_NO_SENSORS_CONFIGURED != sizeof(sensor_sensors_config))
  {
    size_t sensor_config_len = sizeof(sensor_sensors_config) / sizeof(sensor_sensors_config[0]);
    bool is_sensor_configured = false;
    uint8_t sensor_index = 0;

    for (uint8_t index = 0; index < sensor_config_len; index++)
    {
      if(sensor_sensors_config[index].sensor_id == id)
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
      reading.sensor_id = id;

      if(SENSORS_MEASUREMENT_TYPE_VALUE == measurement_type)
      {
        if(SENSORS_NO_VALUE_FUNCTION != current_sensor.sensor_value_function)
        {
          reading.measurement_type_switch = SENSORS_MEASUREMENT_TYPE_VALUE;
          reading.value = current_sensor.sensor_value_function();
          if(!isnan(reading.value))
          {
            if(reading.value > current_sensor.min_value && reading.value < current_sensor.max_value)
            {
              reading.error_code = ERROR_CODE_NO_ERROR;
            }
            else
            {
              reading.error_code = ERROR_CODE_SENSORS_ABNORMAL_VALUE;
            }
          }
          else
          {
            reading.error_code = ERROR_CODE_SENSORS_INVALID_VALUE_FROM_SENSOR;
          }
        }
        else
        {
          reading.error_code = ERROR_CODE_SENSORS_MEASUREMENT_TYPE_MISSING_FUNCTION;
        }
      }
      else if(SENSORS_MEASUREMENT_TYPE_INDICATION == measurement_type)
      {
        if(SENSORS_NO_INDICATION_FUNCTION != current_sensor.sensor_indication_function)
        {
          reading.measurement_type_switch = SENSORS_MEASUREMENT_TYPE_INDICATION;
          reading.indication = current_sensor.sensor_indication_function();
          reading.error_code = ERROR_CODE_NO_ERROR;
        }
        else
        {
          reading.error_code = ERROR_CODE_SENSORS_MEASUREMENT_TYPE_MISSING_FUNCTION;
        }
      }
      else
      {
        reading.error_code = ERROR_CODE_SENSORS_INVALID_MEASUREMENT_TYPE;
      }
    }
    else
    {
      reading.error_code = ERROR_CODE_SENSORS_SENSOR_NOT_FOUND;
    }
  }
  return reading;
}
