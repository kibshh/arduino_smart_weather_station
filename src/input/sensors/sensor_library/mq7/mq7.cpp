#include "mq7.h"

/* STATIC GLOBAL VARIABLES */
// Global variable for heater status
static bool mq7_is_heater_hot = MQ7_HEATER_IS_OFF;
/* *************************************** */

/* STATIC FUNCTION PROTOTYPES */
/** 
 * Converts the raw ADC reading to sensor resistance in ohms. 
 */
static float convertToResistance(int raw_adc);

/** 
 * Turns the heater on by setting it to the required high voltage level. 
 */
static bool heaterOn();

/** 
 * Turns the heater off by setting it to the required low voltage level. 
 */
static bool heaterOff();
/* *************************************** */

/* EXPORTED FUNCTIONS */
void mq7_init()
{
  pinMode(SENSORS_MQ7_PIN_ANALOG, INPUT);
  pinMode(SENSORS_MQ7_PIN_PWM_HEATER, OUTPUT);
  mq7_is_heater_hot = heaterOn(); //Start heating
}

float mq7_readPPM()
{
  float coPPM = MQ7_INVALID_VALUE; // Return value in case of not defined macros(handled by sensors module)
#if defined(SENSORS_MQ7_R_ZERO) && defined(SENSORS_MQ7_CALCULATION_CONSTANT_1) && defined(SENSORS_MQ7_CALCULATION_CONSTANT_2) // All parameters must be defined
  int raw_analog_read = analogRead(SENSORS_MQ7_PIN_ANALOG);
  if(raw_analog_read >= MQ7_ANALOG_INPUT_MIN && raw_analog_read <= MQ7_ANALOG_INPUT_MAX) // Check for valid analog read
  {
    float resistance_under_CO = convertToResistance(raw_analog_read); // Convert analog read to resistance in ohms
    float ratio = resistance_under_CO / SENSORS_MQ7_R_ZERO; // Calculate ratio based on calibrated resistance in clear air
    /* Function for calculating PPM */
    coPPM = pow(MQ7_CALCULATION_POW_BASE_CONSTANT, ((log10(ratio) - SENSORS_MQ7_CALCULATION_CONSTANT_1) / (SENSORS_MQ7_CALCULATION_CONSTANT_2)));
  }
#endif
  return coPPM;
}

float mq7_readResistanceForCalibration()
{
  float calculated_resistance = MQ7_INVALID_VALUE; // Default invalid value

  int sensor_analog_reading = analogRead(SENSORS_MQ7_PIN_ANALOG);
  
  if (MQ7_ANALOG_INPUT_MIN <= sensor_analog_reading && sensor_analog_reading <= MQ7_ANALOG_INPUT_MAX) // Ensure valid reading
  {
    if (sensor_analog_reading == MQ7_ANALOG_INPUT_MIN)
    {
      sensor_analog_reading = MQ7_ANALOG_INPUT_MIN_VALID; // Avoid division by zero
    }
    
    calculated_resistance = (((float)MQ7_ANALOG_INPUT_MAX / sensor_analog_reading) - 1) * MQ7_LOAD_RESISTANCE_VAL; // Calculate resistance
  }

  return calculated_resistance;
}

// Needs to be called in loop
void mq7_heatingCycle(unsigned long current_millis) 
{
  static unsigned long previous_millis = 0;
  unsigned long elapsed_time = current_millis - previous_millis; // Elapsed time between last transition from high to low or low to high till now

  // If the heater has been in the high state for the specified duration and is currently hot
  if (elapsed_time >= SENSORS_MQ7_HEATER_HIGH_TIMEOUT_MS && MQ7_HEATER_IS_ON == mq7_is_heater_hot) // Case heater 
  {
    mq7_is_heater_hot = heaterOff(); // Turn the heater off
    previous_millis = current_millis; // Update the last transition time
  } 
  // If the heater has been in the low state for the specified duration and is currently off
  else if (elapsed_time >= SENSORS_MQ7_HEATER_LOW_TIMEOUT_MS && MQ7_HEATER_IS_OFF == mq7_is_heater_hot) 
  {
    mq7_is_heater_hot = heaterOn(); // Turn the heater on
    previous_millis = current_millis; // Update the last transition time
  }
}
/* *************************************** */

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static float convertToResistance(int raw_adc) 
{
  if(MQ7_ANALOG_INPUT_MIN == raw_adc)
  {
    raw_adc += 1; // To avoid division by 0
  }
  float v_out = raw_adc * ((float)MQ7_VCC_VOLTAGE / MQ7_ANALOG_INPUT_MAX);
  float Rs = ((MQ7_VCC_VOLTAGE - v_out) / v_out) * MQ7_LOAD_RESISTANCE_VAL; // return in ohms
  return Rs;
}

static bool heaterOn()
{
  analogWrite(SENSORS_MQ7_PIN_PWM_HEATER, MQ7_5V_ANALOG_OUTPUT_HEATER);   // Set heater to 5V
  return MQ7_HEATER_IS_ON;
}

static bool heaterOff()
{
  analogWrite(SENSORS_MQ7_PIN_PWM_HEATER, MQ7_1_4V_ANALOG_OUTPUT_HEATER); // Set heater to 1.4V (approx)
  return MQ7_HEATER_IS_OFF;
}
/* *************************************** */