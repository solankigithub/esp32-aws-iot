#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"


#define MOTOR_PIN 				5   //D5 on ESP32
#define SWITCH_PIN 				4   //D4 on ESP32
#define ROOF_TANK_LEVEL_PIN 	18	//D18 on ESP32
#define MAIN_TANK_LEVEL_PIN		19	//D19 on ESP32

bool ledState;
QueueHandle_t queue;

void setupControls(void);
void controlMotor(bool);
void delay(int);
bool debounce(bool);
