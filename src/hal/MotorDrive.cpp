#include "hal/MotorDrive.h"

MotorDrive::MotorDrive() : 
    motorFR(FR_F, FR_R), motorFL(FL_F, FL_R), 
    motorRR(RR_F, RR_R), motorRL(RL_F, RL_R) {}

void MotorDrive::init() {
    stop();
}

void MotorDrive::setMotorSpeed(MX1508& m, int speed) {
    if (speed == 0) {
        m.motorStop();
    } else {
        m.motorGo(constrain(speed, -255, 255));
    }
}

void MotorDrive::drive(int fl, int fr, int rl, int rr) {
    _fl = fl; 
    _fr = fr; 
    _rl = rl; 
    _rr = rr;

    // Left motors use positive sign
    setMotorSpeed(motorFL, _fl);
    setMotorSpeed(motorRL, _rl);

    // Right motors are physically mirrored, so negate input sign
    setMotorSpeed(motorFR, -_fr);
    setMotorSpeed(motorRR, -_rr);
}

void MotorDrive::stop() {
    _fl = _fr = _rl = _rr = 0;
    motorFR.motorStop();
    motorFL.motorStop();
    motorRR.motorStop();
    motorRL.motorStop();
}