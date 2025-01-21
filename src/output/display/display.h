#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#include "../../error_manager/error_codes.h"
#include "../../input/sensors/sensorsconfig.h"
#include "../../data_router/data_router_types.h"
#include "../../input/input_types.h"

#define DIPSLAY_NO_SENSORS_CONFIGURED (0u)

#define DISPLAY_LCD_HEIGHT    (2u)
#define DISPLAY_LCD_WIDTH     (16u)
#define DISPLAY_LCD_I2C_ADDDR (0x27)

#define DISPLAY_START_COLUMN  (0u)
#define DISPLAY_START_ROW     (0u)
#define DISPLAY_SENSORS_ROW   (0u)
#define DISPLAY_TIME_ROW      (1u)
#define DISPLAY_I2C_SCAN_ROW  (0u)

#define DISPLAY_READING_VALUE (0u)
#define DISPLAY_INDICATION    (1u)

#define DISPLAY_NO_DECIMALS   (-1)
#define DISPLAY_0_DECIMALS    (0u)
#define DISPLAY_1_DECIMAL     (1u)
#define DISPLAY_2_DECIMALS    (2u)
#define DISPLAY_3_DECIMALS    (3u)
#define DISPLAY_4_DECIMALS    (4u)
#define DISPLAY_5_DECIMALS    (5u)
#define DISPLAY_6_DECIMALS    (6u)

#define DISPLAY_DATETIME_FORMATER_LEN (3u)

#define DISPLAY_TWO_CIPHER_NUMBER     (10u)

#define DISPLAY_DISPLAY_INTERVAL_MS        (2000)  //2 seconds
#define DISPLAY_TIME_DISPLAY_INTERVAL_MS   (30000) //30 seconds

typedef struct
{
    const char* sensor_type;
    const char* measurement_unit;
    uint8_t id;
    uint8_t measurement_type;
    uint8_t accuracy;
}display_sensors_config_t;

extern const int num_of_display_functions;

void display_init();
error_manager_error_code_te display_displayData(data_router_data_ts data);
error_manager_error_code_te display_displaySensorMeasurement(sensor_reading_t sensor_data);
error_manager_error_code_te display_displayTime(rtc_reading_t time_data);
error_manager_error_code_te display_displayI2CScan(uint8_t *payload, size_t payload_len);

#endif