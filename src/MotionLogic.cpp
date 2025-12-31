#include "MotionLogic.h"

MotionLogic::MotionLogic() {}

// Note the return type uses MotionLogic:: to tell the compiler where the struct is
MotionLogic::CalculatedSpeeds MotionLogic::process(const PacketData& joyData) {
    CalculatedSpeeds s = {0, 0, 0, 0};

    // Mapping normalized float (-1.0 to 1.0) to your legacy 500-scale
    float rawX = joyData.rx * 500.0f; 
    float rawY = joyData.ry * 500.0f;

    // Apply Deadzone
    if (abs(rawX) <= DEAD_ZONE) rawX = 0;
    if (abs(rawY) <= DEAD_ZONE) rawY = 0;

    float x = rawX * SCALE;
    float y = rawY * SCALE;

    // INNER ZONE: Forward/Backward or Point Turns
    if (abs(rawX) <= INNER_ZONE_LIMIT && abs(rawY) <= INNER_ZONE_LIMIT) {
        if (abs(rawY) > DEAD_ZONE) {
            s.fl = s.fr = s.rl = s.rr = (int)y;
        } 
        else if (abs(rawX) > DEAD_ZONE) {
            // Point Turn Logic
            s.fl = s.rl = (int)x;
            s.fr = s.rr = (int)-x;
        }
    } 
    // OUTER ZONE: Full Mecanum movement
    else {
        float rot = (abs(rawY) > DEAD_ZONE) ? 0 : x * 0.2f;
        s.fl = (int)(y + x + rot);
        s.fr = (int)(y - x - rot);
        s.rl = (int)(y - x + rot);
        s.rr = (int)(y + x - rot);
    }

    // Constraints
    s.fl = constrain(s.fl, -255, 255);
    s.fr = constrain(s.fr, -255, 255);
    s.rl = constrain(s.rl, -255, 255);
    s.rr = constrain(s.rr, -255, 255);

    return s;
}