#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WString.h>
#include "../sensors/sensors.h"
#include "../functionality/rtc.h"


#define DISPLAY_LCD_HEIGHT    (2u)
#define DISPLAY_LCD_WIDTH     (16u)
#define DISPLAY_LCD_I2C_ADDDR (0x27)

#define DISPLAY_0_DECIMALS    (0u)
#define DISPLAY_1_DECIMAL     (1u)
#define DISPLAY_2_DECIMALS    (2u)
#define DISPLAY_3_DECIMALS    (3u)
#define DISPLAY_4_DECIMALS    (4u)
#define DISPLAY_5_DECIMALS    (5u)
#define DISPLAY_6_DECIMALS    (6u)

#define DISPLAY_DATETIME_FORMATER_LEN (3u)

#define DISPLAY_DISPLAY_INTERVAL_MS   (2000)

typedef void (*display_displayFunction_t)();
extern LiquidCrystal_I2C lcd;
extern display_displayFunction_t display_functions[];
extern const int display_functions_size;

void display_init();
void display_displayTemperature();
void display_displayHumidity();
void display_displayPressure();
void display_displayTemperatureBPM();
void display_displayAltitude();
void display_displayLuminance();
void display_displayVariousGasesPPM();
void display_displayCOPPM();
void display_displayUV();
void display_displayRainingStatus();
void display_displayDate();
void display_displayTime();

#endif