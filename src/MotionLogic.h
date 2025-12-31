#ifndef MOTION_LOGIC_H
#define MOTION_LOGIC_H

#include <Arduino.h>
#include "hal/ESPNowHandler.h"

class MotionLogic {
public:
    // Struct defined inside the class to prevent global scope naming conflicts
    struct CalculatedSpeeds {
        int fl, fr, rl, rr;
    };

    MotionLogic();
    
    // Returns the nested struct
    CalculatedSpeeds process(const PacketData& joyData);

private:
    const int DEAD_ZONE = 10;
    const int INNER_ZONE_LIMIT = 495;
    const float SCALE = 255.0 / 500.0;
};

#endif