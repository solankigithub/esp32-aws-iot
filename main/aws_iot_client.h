#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"

#define ENABLE_IOT_DEBUG 1
#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"


#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

//#define AWS_IOT_MQTT_HOST CONFIG_AWS_IOT_MQTT_HOST

void iot_subscribe_callback_handler(AWS_IoT_Client *, char *, uint16_t,
                                    IoT_Publish_Message_Params *, void *);
void aws_iot_task(void);
void disconnectCallbackHandler(AWS_IoT_Client *, void *);

