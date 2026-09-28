#ifndef OLED_HANDLER_H
#define OLED_HANDLER_H

#include <Adafruit_SSD1327.h>
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
    Adafruit_SSD1327 display = Adafruit_SSD1327(128, 128, &Wire);
    const int SCREEN_WIDTH = 128;
    const int SCREEN_HEIGHT = 128;
};

#endif