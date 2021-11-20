#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "HardwareSerial.h"

#include "esp_websocket_client.h"
#include "esp_event.h"

#include "controller.h"
#include "gpio.h"

#define WEBSOCKET_RECONNECT_TIMEOUT_MS 2000
#define WEBSOCKET_URI ""

static const char *TAG = "WEBSOCKET";

static TimerHandle_t shutdown_signal_timer;
static SemaphoreHandle_t shutdown_sema;

static void shutdown_signaler(TimerHandle_t xTimer) {
    ESP_LOGI(TAG, "No data received for %d seconds, signaling shutdown", NO_DATA_TIMEOUT_SEC);
    xSemaphoreGive(shutdown_sema);
}

char ws_data[16];

void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
        case WEBSOCKET_EVENT_CONNECTED:
            Serial.println("Connection success");
            break;
        case WEBSOCKET_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "WS disconnect");
            break;
        case WEBSOCKET_EVENT_DATA:
            ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
            if (data->op_code == 0x08 && data->data_len == 2) {
                ESP_LOGW(TAG, "Received closed message with code=%d", 256*data->data_ptr[0] + data->data_ptr[1]);
            } else {
                Motion motion = parse_ws_input(data->data_ptr);
                if (motion.velocity > 0) 
                    move_forward();
                else if (motion.velocity < 0) {
                    move_backward();
                    motion.velocity *= -1;
                }
                else
                    move_stop();
                printf("Scanning data is %d and %d\n", motion.velocity, motion.rotation);
            }

            xTimerReset(shutdown_signal_timer, portMAX_DELAY);
            break;
        case WEBSOCKET_EVENT_ERROR:
            ESP_LOGI(TAG, "Error");
            break;
        }
}

void websocket_client_start(void) {
    esp_websocket_client_config_t websocket_cfg = {};

    shutdown_signal_timer = xTimerCreate("Websocket shutdown timer", 4 * 1000 / portTICK_PERIOD_MS,
                                         pdFALSE, NULL, shutdown_signaler);
    shutdown_sema = xSemaphoreCreateBinary();

    websocket_cfg.uri = WEBSOCKET_URI;

    esp_websocket_client_handle_t client;

    client = esp_websocket_client_init(&websocket_cfg);

    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);

    esp_websocket_client_start(client);

    while (1) {
        vTaskDelay(10000 / portTICK_RATE_MS);
    }
}