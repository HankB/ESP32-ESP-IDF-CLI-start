/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include <time.h>

#include "proj_wifi.h"
#include "proj_sntp.h"
#include "proj_mqtt.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

static uint8_t s_led_state = 0;


static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}


void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    wifi_init_sta();
    time_t t = init_sntp();
    ESP_LOGI(TAG,"init_sntp(): %lld", t);
    init_mqtt();
    esp_log_level_set("*", ESP_LOG_WARN); 
    // ref: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/log.html#dynamic-log-level-control
     
#define payload_len 64
    multi_heap_info_t   heap_info;
    wifi_ap_record_t    ap_info;
    char payload_buf[payload_len];

    while (1) {
        ESP_LOGI(TAG, "Turning the LED %s at %lld!", s_led_state == true ? "ON" : "OFF", time(0));
        heap_caps_get_info(&heap_info, MALLOC_CAP_8BIT);
        esp_wifi_sta_get_ap_info(&ap_info);
        time_t timestamp = time(0);
        int len=snprintf(payload_buf, payload_len, "ts %llu: heap total:%u, used:%u, rssi:%d",
            timestamp, heap_info.total_free_bytes, heap_info.total_allocated_bytes,
            ap_info.rssi);

        proj_mqtt_publish("/topic/repeating/C3", payload_buf, len, 0, 0);
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
