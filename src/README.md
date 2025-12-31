# Project: ESP32-WROOM-30Pin-Mecanum-IMU-OLED

## Overview
A dual-phase robotics project utilizing an ESP32 to drive a 4-wheel Mecanum platform via ESP-NOW remote control, evolving into a self-balancing inverted pendulum robot.

## Hardware Stack
- **MCU:** ESP32-WROOM-30 Pin (Dual Core) https://lastminuteengineers.com/esp32-pinout-reference/
- **Display:** 0.96" OLED (SSD1306 I2C)
- **IMU:** MPU-6050 (6-Axis)
- **Drivers:** 2x MX1508 (Mini L298N)
- **Motors:** 4x N20 (60 RPM) with 48mm Mecanum Wheels

## Pin Mapping

GPIO 6 to 11: Internal Flash Memory. If you connect anything here, the ESP32 won't boot.
GPIO 0: Strapping pin (used to enter "Download Mode"). If held LOW at startup, the code won't run.
GPIO 2: Often connected to the onboard LED. Best to leave it as a status light.
GPIO 34, 35, 36, 39: Input-Only. These cannot output a PWM signal to a motor driver.

- I2C (OLED/IMU): SDA:21, SCL:22
- Motors FR: 13, 12 (+,-)
- Motors FL: 14, 27 (+,-)
- Motors RR: 26, 25 (+,-)
- Motors RL: 33, 32 (+,-)

Project Plan: Phase 1 (Mecanum Control)
To keep the momentum, we will follow a "Software First" approach using the same packet structure from your Controller project.

Week 1: Foundations & Communication
Step 1: Setup the platformio.ini with FS_MX1508, Adafruit MPU6050, and Adafruit SSD1306.

Step 2: Port your PacketData struct to the Receiver code.

Step 3: Establish the ESP-NOW "Handshake." Use the OLED to display the X/Y Joystick values being received from your handheld controller.

Week 2: Kinematics (The "Mecanum" Math)
Step 4: Implement the Mecanum drive algorithm. This converts Joystick X (Slide), Y (Forward), and Z (Spin) into 4 different motor speeds.

Step 5: Software "Bench Test." Watch the OLED/Serial monitor to ensure the motors think they are spinning the right way before putting it on the floor.

Week 3: Hardware Integration
Step 6: Wiring and Power. (We will discuss how to power the ESP32 and Motors separately to prevent crashes).

Step 7: First Drive. Tuning the "deadzone" of your handheld joysticks so the robot doesn't crawl away on its own.

Transitioning to Phase 2 (Self-Balance)
Self-balancing requires very high-speed PID loops. Because we used a Mecanum setup, we will likely "lock" the strafing and treat it as a 2-wheel balancer later, or use the 4 wheels to create a wider, more stable balancing base.