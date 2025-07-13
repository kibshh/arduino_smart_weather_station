#include "src/project_configs/project_configs.h"
#ifdef CONFIGS_ARDUINO_RAIN_SENSOR_INCLUDED
#include "src/project_sensors/arduino_rain_sensor/arduino_rain_sensor.h"
#endif
#ifdef CONFIGS_BH1750_SENSOR_INCLUDED
#include "src/project_sensors/bh1750_sensor/bh1750_sensor.h"
#endif
#ifdef CONFIGS_BMP280_SENSOR_INCLUDED
#include "src/project_sensors/bmp280_sensor/bmp280_sensor.h"
#endif
#ifdef CONFIGS_DHT11_SENSOR_INCLUDED
#include "src/project_sensors/dht11_sensor/dht11_sensor.h"
#endif
#ifdef CONFIGS_GY_ML8511_SENSOR_INCLUDED
#include "src/project_sensors/gy_ml8511_sensor/gy_ml8511_sensor.h"
#endif
#ifdef CONFIGS_MQ7_SENSOR_INCLUDED
#include "src/project_sensors/mq7_sensor/mq7_sensor.h"
#endif
#ifdef CONFIGS_MQ135_SENSOR_INCLUDED
#include "src/project_sensors/mq135_sensor/mq135_sensor.h"
#endif
#ifdef CONFIGS_I2C_SCANNER_INCLUDED
#include "src/project_utilities/i2c_scanner/i2c_scanner.h"
#endif
#ifdef CONFIGS_RTC_INCLUDED
#include "src/project_utilities/rtc/rtc.h"
#endif

static uint8_t current_readfunction_idx = 0;
static uint8_t num_of_reading_functions = 0;
static uint64_t previous_millis = 0;
static CurrentReading_t current_reading[CONFIGS_MAX_NUM_OF_MEASUREMENTS] = {0};

void setup() 
{
  wdt_enable(CONFIGS_WATCHDOG_TIMEOUT); /* Watchdog: 2 seconds timeout */

  (void)ProjectConfigs_Init();
#ifdef CONFIGS_ARDUINO_RAIN_SENSOR_INCLUDED
  (void)ArduinoRainSensor_Init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_BH1750_SENSOR_INCLUDED
  (void)Bh1750Sensor_Init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_BMP280_SENSOR_INCLUDED
  (void)Bmp280Sensor_Init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_DHT11_SENSOR_INCLUDED
  (void)Dht11Sensor_Init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_GY_ML8511_SENSOR_INCLUDED
  (void)GyMl8511Sensor_Init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_MQ7_SENSOR_INCLUDED
  (void)Mq7Sensor_Init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_MQ135_SENSOR_INCLUDED
  (void)Mq135Sensor_init(current_reading, &num_of_reading_functions);
#endif
#ifdef CONFIGS_RTC_INCLUDED
  (void)Rtc_Init(current_reading, &num_of_reading_functions);
#endif

  Serial.println("\nWeather Station");

#ifdef CONFIGS_I2C_SCANNER_INCLUDED
  (void)I2cScanner_ReadConnectedI2cDevices();
#endif
}

void loop() 
{
#ifdef CONFIGS_MQ7_SENSOR_INCLUDED
  Mq7Sensor_HeatingCycle();
#endif

  uint64_t current_millis = (uint64_t)millis();
  if(current_millis - previous_millis >= CONFIGS_READ_INTERVAL_MS)
  {
    previous_millis = current_millis;
    current_reading[current_readfunction_idx]();
    current_readfunction_idx++;
    current_readfunction_idx %= num_of_reading_functions;
  }
  delay(CONFIGS_LOOP_FREQUENCY_MS);
  wdt_reset();  /* Feed the watchdog */
}
