#include "WiFi.h"
#include "wifi.h"

#define SSID "HUAWEI-D2N5"
#define PASSWORD "48575443A774889D"

void wifi_connect(WiFiEvent_t event, WiFiEventInfo_t info) {
    
}

void wifi_disconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
    WiFi.begin(SSID, PASSWORD);
}

void wifi_gotip(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println("IP for current session: ");
    Serial.println(WiFi.localIP());
}

void wifi_setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    Serial.println("WiFi got connection, congrats, sir!");
    Serial.println(WiFi.localIP());

    WiFi.onEvent(wifi_connect, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(wifi_disconnect, SYSTEM_EVENT_STA_DISCONNECTED);
    WiFi.onEvent(wifi_gotip, SYSTEM_EVENT_STA_GOT_IP);
}