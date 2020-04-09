#define ENABLE_IOT_DEBUG 1

#include <stdio.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "switch_task.h"
#include "aws_iot_client.h"


#define ENABLE_IOT_DEBUG 1

//#define true 1
//#define false 0


#ifndef BIT0
#define BIT0 (0x1 << 0)
#endif

#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

const int CONNECTED_BIT = BIT0;
static const char *TAG = "Tankey";

EventGroupHandle_t wifi_event_group;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
    	ESP_LOGI(TAG, "Wifi Connected!!!!");
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP platform WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}


void init_wifi(){
	tcpip_adapter_init();
	wifi_event_group =xEventGroupCreate();

    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );

}

void aws_task(void *pvParameter){
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP success!");

    aws_iot_task();

    vTaskDelete(NULL);
}

void switch_task(void *pvParameter){

	ESP_LOGI(TAG, "Switch Task fired!");
	switch_task_run();
	vTaskDelete(NULL);
}


void app_main(void)
{

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    //Initialize WiFi
    init_wifi();
    queue  = xQueueCreate(2, sizeof(int));
    esp_log_level_set(TAG, ESP_LOG_VERBOSE);
    //xTaskCreatePinnedToCore(&switch_task, "switch_task", 1024 * 10, NULL, 3, NULL,1);
    //xTaskCreatePinnedToCore(&aws_task, "aws_task", 1024 * 20, NULL, 3, NULL,1);

    xTaskCreate(&switch_task, "switch_task", 1024 * 10, NULL, 3, NULL);
    xTaskCreate(&aws_task, "aws_task", 1024 * 20, NULL, 3, NULL);

}

