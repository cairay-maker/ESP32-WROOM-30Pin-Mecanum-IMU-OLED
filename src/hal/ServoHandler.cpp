#include "ServoHandler.h"

#define SERVOMIN 100
#define SERVOMAX 500

ServoHandler::ServoHandler() : _pwm(Adafruit_PWMServoDriver(0x40)) {}

uint16_t ServoHandler::degreesToPWM(float degrees) {
    degrees = constrain(degrees, 0.0f, 180.0f);
    return map((long)(degrees * 10), 0, 1800, SERVOMIN, SERVOMAX);
}

void ServoHandler::begin() {
    _pwm.begin();
    _pwm.setOscillatorFrequency(27000000);
    _pwm.setPWMFreq(50);
    _lastUpdateMicros = micros();
}

void ServoHandler::setEngaged(bool engage) {
    _isEngaged = engage;
    if (_isEngaged) {
        for (int i = 0; i < JOINT_COUNT; i++) {
            _targetAngles[i] = _currentAngles[i];
            _pwm.setPWM(i, 0, degreesToPWM(_currentAngles[i]));
        }
        Serial.println(">>> SERVOS: ENGAGED");
    } else {
        for (int i = 0; i < JOINT_COUNT; i++) {
            _pwm.setPWM(i, 0, 0); // De-energize PWM outputs
        }
        Serial.println(">>> SERVOS: RELAXED");
    }
}

void ServoHandler::setNeutralPose() {
    for (int i = 0; i < JOINT_COUNT; i++) {
        _targetAngles[i] = NEUTRAL_ANGLES[i];
    }
    Serial.println(">>> POSE: NEUTRAL RESTORED");
}

void ServoHandler::adjustHeight(float offsetDeg) {
    // offsetDeg > 0 raises body (UP), offsetDeg < 0 lowers body (DN)
    
    // FL & RR: Subtract offset to raise leg
    _targetAngles[JOINT_FL] = constrain(NEUTRAL_ANGLES[JOINT_FL] - offsetDeg, 0.0f, 180.0f);
    _targetAngles[JOINT_RR] = constrain(NEUTRAL_ANGLES[JOINT_RR] - offsetDeg, 0.0f, 180.0f);

    // FR & RL: Add offset due to mirrored mounting geometry
    _targetAngles[JOINT_FR] = constrain(NEUTRAL_ANGLES[JOINT_FR] + offsetDeg, 0.0f, 180.0f);
    _targetAngles[JOINT_RL] = constrain(NEUTRAL_ANGLES[JOINT_RL] + offsetDeg, 0.0f, 180.0f);

    Serial.printf(">>> HEIGHT TARGET: Offset %.1f deg relative to Neutral\n", offsetDeg);
}

void ServoHandler::applySplit(float offsetDeg) {
    // FL & RR: Lower body by adding offsetDeg (- relative shift)
    _targetAngles[JOINT_FL] = constrain(NEUTRAL_ANGLES[JOINT_FL] + offsetDeg, 0.0f, 180.0f);
    _targetAngles[JOINT_RR] = constrain(NEUTRAL_ANGLES[JOINT_RR] + offsetDeg, 0.0f, 180.0f);

    // FR & RL: Raise body by adding offsetDeg (+ relative shift due to opposite side)
    _targetAngles[JOINT_FR] = constrain(NEUTRAL_ANGLES[JOINT_FR] + offsetDeg, 0.0f, 180.0f);
    _targetAngles[JOINT_RL] = constrain(NEUTRAL_ANGLES[JOINT_RL] + offsetDeg, 0.0f, 180.0f);

    Serial.printf(">>> SPLIT TARGET: Offset %.1f deg (FR/RL raised, FL/RR lowered)\n", offsetDeg);
}

void ServoHandler::applyPoseOffsets(float heightOffsetDeg, float splitOffsetDeg) {
    // 1. Automatically engage servos if receiving remote command
    if (!_isEngaged) {
        setEngaged(true);
    }

    // 2. Calculate combined height & split target angles using mirrored signs
    // FL & RR: Subtract height, Add split
    float flTarget = NEUTRAL_ANGLES[JOINT_FL] - heightOffsetDeg + splitOffsetDeg;
    float rrTarget = NEUTRAL_ANGLES[JOINT_RR] - heightOffsetDeg + splitOffsetDeg;

    // FR & RL: Add height, Add split (mirrored geometry)
    float frTarget = NEUTRAL_ANGLES[JOINT_FR] + heightOffsetDeg + splitOffsetDeg;
    float rlTarget = NEUTRAL_ANGLES[JOINT_RL] + heightOffsetDeg + splitOffsetDeg;

    // 3. Set constrained target angles
    _targetAngles[JOINT_FL] = constrain(flTarget, 0.0f, 180.0f);
    _targetAngles[JOINT_RR] = constrain(rrTarget, 0.0f, 180.0f);
    _targetAngles[JOINT_FR] = constrain(frTarget, 0.0f, 180.0f);
    _targetAngles[JOINT_RL] = constrain(rlTarget, 0.0f, 180.0f);
}

void ServoHandler::processSerialCommands() {
    if (!Serial.available()) return;

    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toUpperCase();

    if (input.length() == 0) return;

    if (input == "OFF") {
        setEngaged(false);
    } else if (input == "ON") {
        setEngaged(true);
    } else if (input == "NEUTRAL") {
        setNeutralPose();
    } else if (input.indexOf(':') > 0) {
        String key = input.substring(0, input.indexOf(':'));
        float val = input.substring(input.indexOf(':') + 1).toFloat();

        if (!_isEngaged) setEngaged(true);

        // --- FIXED CHECK FOR UP / DN / SPEED ---
        if (key == "UP") {
            adjustHeight(val);    // e.g., UP:10 -> Raise chassis 10 deg
        } else if (key == "DN") {
            adjustHeight(-val);   // e.g., DN:10 -> Lower chassis 10 deg
        } else if (key == "SPLIT") {
            applySplit(val);      // e.g., SPLIT:10 -> Raise FR/RL 10 deg, Lower FL/RR 10 deg
        } else if (key == "SPEED") {
            _speedDegPerSec = constrain(val, 5.0f, 300.0f);
            Serial.printf(">>> Speed set to %.1f deg/sec\n", _speedDegPerSec);
        } else {
            // Check individual joint channels
            int ch = -1;
            if (key == "FL") ch = JOINT_FL;
            else if (key == "FR") ch = JOINT_FR;
            else if (key == "RL") ch = JOINT_RL;
            else if (key == "RR") ch = JOINT_RR;

            if (ch >= 0) {
                _targetAngles[ch] = constrain(val, 0.0f, 180.0f);
                Serial.printf(">>> Joint %s target set to %.1f deg\n", key.c_str(), val);
            } else {
                Serial.println("!!! Unknown command. Type HELP for options.");
            }
        }
    } else {
        Serial.println("!!! Unknown command. Type HELP for options.");
    }
}

void ServoHandler::update() {
    unsigned long currentMicros = micros();
    float dt = (currentMicros - _lastUpdateMicros) / 1000000.0f;
    _lastUpdateMicros = currentMicros;

    if (!_isEngaged) return;

    float maxStep = _speedDegPerSec * dt;

    // Round-robin Interleaved Stepping
    for (int count = 0; count < JOINT_COUNT; count++) {
        int leg = (_interleavedLegIndex + count) % JOINT_COUNT;
        float diff = _targetAngles[leg] - _currentAngles[leg];

        if (abs(diff) > 0.05f) {
            // Nudge by up to 1.0 degree or maxStep, whichever is smaller
            float step = (diff > 0) ? min(1.0f, maxStep) : max(-1.0f, -maxStep);
            _currentAngles[leg] += step;

            // Clamp precisely to target
            if ((diff > 0 && _currentAngles[leg] > _targetAngles[leg]) ||
                (diff < 0 && _currentAngles[leg] < _targetAngles[leg])) {
                _currentAngles[leg] = _targetAngles[leg];
            }

            _pwm.setPWM(leg, 0, degreesToPWM(_currentAngles[leg]));

            // Move index to the next leg for the following loop iteration
            _interleavedLegIndex = (leg + 1) % JOINT_COUNT;
            break; // Exit after updating 1 joint step
        }
    }
}