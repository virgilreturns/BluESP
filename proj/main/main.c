#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN 2  // Define the onboard LED pin

void blink_task(void *pvParameter) {
    //gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_PIN, 1);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(500));  // Delay 500ms
        gpio_set_level(LED_PIN, 0);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(500));  // Delay 500ms
    }
}

void app_main() {
    char *ourTaskname = pcTaskGetName(NULL);
    ESP_LOGI(ourTaskname, "Hello, starting up!\n");

    xTaskCreate(blink_task, "blink_task", 2048, NULL, 5, NULL);

}
