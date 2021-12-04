#include <Arduino.h>
#include <WebSocketsClient.h>

#include "wifi.h"
#include "gpio.h"
#include "pwm.h"
#include "websocket.h"

void setup() {
  Serial.begin(9600);
  gpio_init();
  pwm_setup();
  wifi_setup();
  ws_setup();
}

void loop() {
  extern WebSocketsClient ws_client;
  ws_client.loop();
}