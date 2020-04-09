#include "common.h"
#include "switch_task.h"

#include "aws_iot_client.h"
static char* SWITCH_TAG = "Tankey";


void switch_task_run(){
	esp_log_level_set(SWITCH_TAG, ESP_LOG_VERBOSE);
	ESP_LOGI(SWITCH_TAG, "Switch Button Task triggered");

	bool btnState = false;
	ledState = false;

	setupControls();
    gpio_set_level(SWITCH_PIN, false);
    gpio_set_level(MOTOR_PIN, false);

	while(1)
	{
		if(debounce(btnState)==true && btnState==false){
			//ESP_LOGI(TANKEY_TAG, "Switch Button Triggered");
			btnState = true;
			ledState = !ledState;
			controlMotor(ledState);
			ESP_LOGI(SWITCH_TAG, "Motor State : %d", ledState);
			//In case button pressed, add it to the Queue
			int state = ledState;
			//xQueueSend(queue, &state, portMAX_DELAY);

		}
		else if(debounce(btnState) == false && btnState == true)
		    btnState = false;


		if(gpio_get_level(ROOF_TANK_LEVEL_PIN)){
			controlMotor(false);
			ESP_LOGI(SWITCH_TAG, "Roof tank is filled, back motor is switched off");

		}

		if(gpio_get_level(MAIN_TANK_LEVEL_PIN)){
//			https_with_url();
		}
	}
}
