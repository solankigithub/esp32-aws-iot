#include "common.h"

/*****************************************************
Setup GPIO Controls
*****************************************************/
void setupControls()
{
	gpio_pad_select_gpio(MOTOR_PIN);
	gpio_set_direction(MOTOR_PIN, GPIO_MODE_OUTPUT);
	gpio_set_level(MOTOR_PIN, true);

	gpio_pad_select_gpio(ROOF_TANK_LEVEL_PIN);
	gpio_set_direction(ROOF_TANK_LEVEL_PIN, GPIO_MODE_INPUT);
	gpio_set_level(ROOF_TANK_LEVEL_PIN, true);

	gpio_pad_select_gpio(MAIN_TANK_LEVEL_PIN);
	gpio_set_direction(MAIN_TANK_LEVEL_PIN, GPIO_MODE_INPUT);
	gpio_set_level(MAIN_TANK_LEVEL_PIN, true);

    gpio_config_t btn_config;
    btn_config.mode = GPIO_MODE_INPUT;
    btn_config.pull_down_en = GPIO_PULLDOWN_ENABLE;
    btn_config.intr_type = GPIO_INTR_DISABLE;
    btn_config.pull_up_en = GPIO_PULLUP_DISABLE;
    btn_config.pin_bit_mask = SWITCH_PIN;
    gpio_config(&btn_config);
}


/**************************************************
Custom Delay function
 **************************************************/
void delay(int duration)
{
	vTaskDelay(duration / portTICK_PERIOD_MS);
}


/*****************************************************
Debounce Function
*****************************************************/
bool debounce(bool state)
{
  bool stateNow = gpio_get_level(SWITCH_PIN);
  if(state!=stateNow)
  {
    delay(50);
    stateNow = gpio_get_level(SWITCH_PIN);
  }
  return stateNow;
}


/*****************************************************
Function to switch ON/OFF motor
*****************************************************/
void controlMotor(bool state){
	gpio_set_level(MOTOR_PIN, state);
	ledState = state;

}

