#ifndef OLED_HANDLER_H
#define OLED_HANDLER_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/TomThumb.h> // The "50% scale" look font
#include <Wire.h>

class OLEDHandler {
public:
    void begin();
    void clearScreen();
    void updateDisplay();
    
    void drawBalanceBar(float roll);
    void drawWifiIcon(bool connected);
    void drawRobotData(float p, float r, int fl, int fr, int rl, int rr);
    void drawCenteredText(const char* text, int y, int size);

private:
    Adafruit_SSD1306 display;
    const int SCREEN_WIDTH = 128;
    const int SCREEN_HEIGHT = 64;
};

#endif