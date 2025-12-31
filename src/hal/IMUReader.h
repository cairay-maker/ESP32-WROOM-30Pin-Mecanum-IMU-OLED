#ifndef IMUREADER_H
#define IMUREADER_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class IMUReader {
public:
    bool begin();
    void update();
    void calibrate();

    // Raw Accel and Gyro Data
    float ax, ay, az, gx, gy, gz;

    // Filtered Output (Calculated)
    float getPitch() const { return pitch - pitchOffset; }
    float getRoll() const  { return roll - rollOffset; }
    float getYawRate() const { return filteredYawRate; } 

    // Raw comparison helpers (Restored for consistency)
    float getRawPitch() const { return (atan2(ay, az) * 57.2958f) - pitchOffset; }
    float getRawRoll() const  { return (atan2(ax, az) * 57.2958f) - rollOffset; }

private:
    Adafruit_MPU6050 mpu;
    float pitch = 0, roll = 0;
    float filteredYawRate = 0; 
    unsigned long lastMicros;
    
    const float alpha = 0.96f;      
    const float yawAlpha = 0.15f;   

    // Calibration Offsets
    float pitchOffset = 0;
    float rollOffset = 0;
    float gxBias = 0, gyBias = 0, gzBias = 0;
};

#endif