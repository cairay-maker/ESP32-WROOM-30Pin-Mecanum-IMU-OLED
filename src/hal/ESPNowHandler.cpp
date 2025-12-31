#include "hal/ESPNowHandler.h"

// Initialize static members
PacketData ESPNowHandler::latestPacket;
uint32_t ESPNowHandler::recvCount = 0;
uint32_t ESPNowHandler::lastRecvTime = 0;

ESPNowHandler::ESPNowHandler() {
    memset(&latestPacket, 0, sizeof(PacketData));
}

void ESPNowHandler::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("!!! ESP-NOW: Initialization Failed");
        return;
    }

    // Register receive callback
    esp_now_register_recv_cb(onDataRecv);
    Serial.println(">>> ESP-NOW: Receiver Ready");
}

void ESPNowHandler::onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    if (len == sizeof(PacketData)) {
        memcpy(&latestPacket, incomingData, sizeof(PacketData));
        recvCount++;
        lastRecvTime = millis();
    }
}

bool ESPNowHandler::isConnected() {
    // If we haven't received a packet in 500ms, consider it disconnected
    return (millis() - lastRecvTime < 500);
}