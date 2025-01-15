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

sensor_reading_t sensors_getReading(sensor_id_te id, sensor_measurement_type_te measurement_type)
{
  sensor_reading_t reading;
  reading.success = false;

  if(SENSORS_NO_SENSORS_CONFIGURED != sizeof(sensor_sensors_config))
  {
    size_t sensor_config_len = sizeof(sensor_sensors_config) / sizeof(sensor_sensors_config[0]);
    if(sensor_config_len > id)
    {
      sensor_sensors_config_t current_sensor;
      memcpy_P(&current_sensor, &sensor_sensors_config[id], sizeof(sensor_sensors_config_t));

      if(id == sensor_sensors_config[id].sensor_id)
      {
        if(SENSORS_VALUE == measurement_type)
        {
          if(SENSORS_NO_VALUE_FUNCTION != current_sensor.sensor_value_function)
          {
            reading.measurement_type_switch = SENSORS_VALUE;
            reading.value = current_sensor.sensor_value_function();
            if(!isnan(reading.value))
            {
              if(reading.value > current_sensor.min_value && reading.value < current_sensor.max_value)
              {
                reading.success = true;
              }
            }
          }
        }
        else
        {
          if(SENSORS_NO_INDICATION_FUNCTION != current_sensor.sensor_indication_function)
          {
            reading.measurement_type_switch = SENSORS_INDICATION;
            reading.indication = current_sensor.sensor_indication_function();
            reading.success = true;
          }
        }
      }
    }
  }
  return reading;
}
