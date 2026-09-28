#include "OLEDHandler.h"

void OLEDHandler::begin() {
    if(!display.begin(0x3C)) return;
    display.clearDisplay();
    display.setTextColor(SSD1327_WHITE);
    display.setTextSize(1);
    display.display();
}

void OLEDHandler::clearScreen() {
    display.clearDisplay();
    display.setFont(NULL);      // Use original system font
    display.setTextSize(1);
    display.setCursor(0,0);
}

void OLEDHandler::updateDisplay() {
    display.display();
}

void OLEDHandler::drawWifiIcon(bool connected) {
    if (connected) {
        // Graphic bars in the corner
        display.fillRect(116, 6, 2, 2, SSD1327_WHITE);
        display.fillRect(120, 4, 2, 4, SSD1327_WHITE);
        display.fillRect(124, 2, 2, 6, SSD1327_WHITE);
    } else {
        display.setCursor(110, 0);
        display.print("!!"); // Signal "Not Connected" clearly
    }
}

void OLEDHandler::drawRobotData(float p, float r, int fl, int fr, int rl, int rr) {
    display.setCursor(0, 0);
    display.printf("P: %.1f  R: %.1f", p, r);
    
    display.setCursor(0, 10);
    display.printf("FL:%3d  FR:%3d" , fl, fr);
    display.setCursor(0, 20);
    display.printf("RL:%3d  RR:%3d" , rl, rr);

}

void OLEDHandler::drawBalanceBar(float roll) {
    // Original thin bar at the bottom
    int barY = 116;
    int barWidth = 100;
    int barX = (128 - barWidth) / 2;
    
    display.drawRect(barX, barY, barWidth, 7, SSD1327_WHITE);
    display.drawFastVLine(64, barY - 2, 11, SSD1327_WHITE); // Center notch

    // Sensitivity: 2.0 pixels per degree
    int ballX = 64 + (int)(roll * 2.0f);
    ballX = constrain(ballX, barX + 2, barX + barWidth - 4);
    
    display.fillCircle(ballX, barY + 3, 2, SSD1327_WHITE);
}

void OLEDHandler::drawCenteredText(const char* text, int y, int size) {
    display.setTextSize(size);
    display.setFont(NULL);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((128 - w) / 2, y);
    display.print(text);
}