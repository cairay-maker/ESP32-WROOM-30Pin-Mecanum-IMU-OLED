#ifndef MOTOR_DRIVE_H
#define MOTOR_DRIVE_H

#include <Arduino.h>
#include <FS_MX1508.h>

// Your Active GPIO Pin Assignments
#define FR_F 19  // IN3
#define FR_R 18  // IN4

#define RR_F 27  // IN1
#define RR_R 14  // IN2

#define FL_F 25  // IN3
#define FL_R 26  // IN4

#define RL_F 23  // IN1
#define RL_R 4   // IN2 (GPIO 04)

class MotorDrive {
public:
    MotorDrive();
    void init();
    void drive(int fl, int fr, int rl, int rr);
    void stop();

    // Getters for OLED / telemetry
    int getFLSpeed() { return _fl; }
    int getFRSpeed() { return _fr; }
    int getRLSpeed() { return _rl; }
    int getRRSpeed() { return _rr; }

private:
    MX1508 motorFR, motorFL, motorRR, motorRL;
    int _fl = 0, _fr = 0, _rl = 0, _rr = 0;
    void setMotorSpeed(MX1508& m, int speed);
};

#endif