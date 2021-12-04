#include "WebSocketsClient.h"

#include "websocket.h"
#include "controller.h"
#include "pwm.h"

#define WS_SERVER_HOST "192.168.100.19"
#define WS_SERVER_PORT 8888
#define RECONNECT_TIMEOUT 3000

WebSocketsClient ws_client;

void ws_event_handler(WStype_t type, uint8_t *payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Ws disconnect");
            break;
        case WStype_CONNECTED:
            Serial.println("Got ws connection");
            break;
        case WStype_TEXT:
            Serial.println((char *)payload);
            Motion m = {0, 0};
            m = parse_ws_input((char *)payload);
            pwm_movement_control(m.velocity);
            pwm_rotation_control(m.rotation);
            break;
        case WStype_BIN:
            Serial.println("Got bin");
            break;
        default:
            break;
    }
}

void ws_setup() {
    Serial.println("Trying connect to the server");
    ws_client.begin(WS_SERVER_HOST, WS_SERVER_PORT);
    ws_client.onEvent(ws_event_handler);
    ws_client.setReconnectInterval(RECONNECT_TIMEOUT);
}