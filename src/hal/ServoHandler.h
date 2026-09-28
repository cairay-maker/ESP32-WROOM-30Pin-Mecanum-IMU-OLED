#ifndef SERVO_HANDLER_H
#define SERVO_HANDLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define JOINT_COUNT 4

enum ServoJoint {
    JOINT_FL = 0, // Calibrated Neutral: 130° (90° lift reference)
    JOINT_FR = 1, // Calibrated Neutral: 100° (120° lift reference)
    JOINT_RL = 2, // Calibrated Neutral: 110° (130° lift reference)
    JOINT_RR = 3  // Calibrated Neutral: 122° (90° lift reference)
};

class ServoHandler {
private:
    Adafruit_PWMServoDriver _pwm;
    
    // Calibrated Neutral Angles
    const float NEUTRAL_ANGLES[JOINT_COUNT] = {130.0f, 100.0f, 110.0f, 122.0f};

    float _currentAngles[JOINT_COUNT] = {130.0f, 100.0f, 110.0f, 122.0f};
    float _targetAngles[JOINT_COUNT]  = {130.0f, 100.0f, 110.0f, 122.0f};
    
    float _speedDegPerSec = 30.0f; // Default 30 deg/sec
    bool _isEngaged = false;
    unsigned long _lastUpdateMicros = 0;

    // Round-robin index for interleaved stepping
    int _interleavedLegIndex = 0;

    uint16_t degreesToPWM(float degrees);

public:
    ServoHandler();
    void begin();
    void update();
    void processSerialCommands();
    void setNeutralPose();
    void adjustHeight(float offsetDeg); // Handles UP / DN relative commands
    void setEngaged(bool engage);
    void applySplit(float offsetDeg); // Handles SPLIT:X roll/tilt commands
    void applyPoseOffsets(float heightOffsetDeg, float splitOffsetDeg);
};

#endif