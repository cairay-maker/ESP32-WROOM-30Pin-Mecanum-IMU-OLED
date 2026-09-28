#ifndef ESPNOW_HANDLER_H
#define ESPNOW_HANDLER_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "PacketData.h"

class ESPNowHandler {
public:
    ESPNowHandler();
    
    void begin();
    
    // Data access
    bool isConnected();
    const PacketData& getLatestPacket() const { return latestPacket; }
    uint32_t getRecvCount() const { return recvCount; }

private:
    static PacketData latestPacket; 
    static uint32_t recvCount;
    static uint32_t lastRecvTime;
    
    // Callback must be static for ESP-NOW
    static void onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
};

#endif