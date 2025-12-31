#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "hal/MotorDrive.h"
#include "hal/IMUReader.h"
#include "hal/OLEDHandler.h"
#include "hal/ESPNowHandler.h"
#include "MotionLogic.h"

MotorDrive motors;
IMUReader imu;
OLEDHandler oled;
ESPNowHandler radio;
MotionLogic logic;

void setup() {
    Serial.begin(115200);
    
    // --- POWER UP WIFI & LOCK CHANNEL 1 ---
    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
    
    motors.init();
    oled.begin();
    radio.begin(); 
    
    if (imu.begin()) {
        oled.clearScreen();
        oled.drawCenteredText("CALIBRATING...", 28, 1);
        oled.updateDisplay();
        imu.calibrate();
    }
}

void loop() {
    imu.update();
    
    bool connected = radio.isConnected();
    
    if (connected) {
        const PacketData& remote = radio.getLatestPacket();
        MotionLogic::CalculatedSpeeds speeds = logic.process(remote);
        motors.drive(speeds.fl, speeds.fr, speeds.rl, speeds.rr);
    } else {
        motors.stop();
    }
    
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 66) { // 15 FPS
        oled.clearScreen();
        oled.drawWifiIcon(connected);
        oled.drawRobotData(
            imu.getPitch(), imu.getRoll(),
            motors.getFLSpeed(), motors.getFRSpeed(),
            motors.getRLSpeed(), motors.getRRSpeed()
        );
        oled.drawBalanceBar(imu.getRoll());
        oled.updateDisplay();
        lastUpdate = millis();
    }
}