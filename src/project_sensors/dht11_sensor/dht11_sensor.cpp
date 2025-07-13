#include "dht11_sensor.h"

static bool Dht11Sensor_ReadTemperature();
static bool Dht11Sensor_ReadHumidity();

static DHT dht(DHT11_SENSOR_PIN, DHT11_SENSOR_DHT_TYPE);

bool Dht11Sensor_Init(CurrentReading_t *init_func, uint8_t *current_index)
{
  dht.begin();
  init_func[*current_index] = Dht11Sensor_ReadTemperature;
  (*current_index)++;
  init_func[*current_index] = Dht11Sensor_ReadHumidity;
  (*current_index)++;
  return true;
}

static bool Dht11Sensor_ReadTemperature()
{
  float temperature = dht.readTemperature(); /* Celsius */
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);

  if(temperature > DHT11_SENSOR_TEMPERATURE_MIN && temperature < DHT11_SENSOR_TEMPERATURE_MAX)
  {
    String temperature_formatted = String(temperature, CONFIGS_DISPLAY_1_DECIMAL);

    Serial.print("DHT Temperature: ");
    Serial.print(temperature_formatted);
    Serial.println("C\n");

    lcd.print("DHT Temp: ");
    lcd.print(temperature_formatted);
    lcd.print("C");
    return true;   
  }
  Serial.println("Error DHT temp\n");
  lcd.print("Error DHT temp");
  return false;
}

static bool Dht11Sensor_ReadHumidity()
{
  float humidity = dht.readHumidity(); /* Percent */
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);

  if(humidity > DHT11_SENSOR_HUMIDITY_MIN && humidity < DHT11_SENSOR_HUMIDITY_MAX)
  {
    String humidity_formatted = String(humidity, CONFIGS_DISPLAY_1_DECIMAL);

    Serial.print("Humidity: ");
    Serial.print(humidity_formatted);
    Serial.println("%\n");

    lcd.print("Humidity: ");
    lcd.print(humidity_formatted);
    lcd.print("%");
    return true; 
  }
  Serial.println("Error humidity\n");
  lcd.print("Error humidity");
  return false;
}