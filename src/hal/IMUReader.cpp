#include "IMUReader.h"
#include <Wire.h>
#include <Arduino.h>

bool IMUReader::begin() {

    Wire.setClock(400000);

    if (!lsm.begin_I2C()) return false;

    lsm.setAccelRange(LSM6DS_ACCEL_RANGE_4_G);
    lsm.setGyroRange(LSM6DS_GYRO_RANGE_1000_DPS);

    lastMicros = micros();
    return true;
}

void IMUReader::calibrate() {
    float pSum = 0, rSum = 0, gxb = 0, gyb = 0, gzb = 0;
    const int samples = 100;

    for (int i = 0; i < samples; i++) {
        sensors_event_t a, g, t;
        lsm.getEvent(&a, &g, &t);
        
        // Calculate raw angles for initial offset
        pSum += atan2(a.acceleration.y, a.acceleration.z) * 57.2958f;
        rSum += atan2(a.acceleration.x, a.acceleration.z) * 57.2958f;
        gxb += g.gyro.x;
        gyb += g.gyro.y;
        gzb += g.gyro.z;
        delay(2); 
    }

    pitchOffset = pSum / (float)samples;
    rollOffset = rSum / (float)samples;
    gxBias = gxb / (float)samples;
    gyBias = gyb / (float)samples;
    gzBias = gzb / (float)samples;

    pitch = 0; 
    roll = 0;
    filteredYawRate = 0;
}

void IMUReader::update() {
    sensors_event_t accel, gyro, temp;
    lsm.getEvent(&accel, &gyro, &temp);

    ax = accel.acceleration.x; 
    ay = accel.acceleration.y; 
    az = accel.acceleration.z;
    
    // 1. Gyro Bias Correction
    gx = gyro.gyro.x - gxBias;
    gy = gyro.gyro.y - gyBias;
    gz = gyro.gyro.z - gzBias;

    unsigned long currentMicros = micros();
    float dt = (currentMicros - lastMicros) / 1000000.0f;
    lastMicros = currentMicros;

    // 2. Convert Gyro to Degrees/sec
    // Note: Signs may need flipping depending on sensor orientation relative to chassis
    float gx_deg = gx * 57.2958f; 
    float gy_deg = gy * 57.2958f;
    float gz_deg = gz * 57.2958f;

    // 3. Accelerometer Angles
    float accPitch = atan2(ay, az) * 57.2958f;
    float accRoll  = atan2(ax, az) * 57.2958f;

    // 4. Complementary Filter
    pitch = alpha * (pitch + gx_deg * dt) + (1.0f - alpha) * accPitch;
    roll  = alpha * (roll + gy_deg * dt) + (1.0f - alpha) * accRoll;

    // 5. Yaw Rate Smoothing
    filteredYawRate = (filteredYawRate * (1.0f - yawAlpha)) + (gz_deg * yawAlpha);
}