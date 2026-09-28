Script to Concatenate all the code for Gemini
    find src -type f ! -name "AllCode.txt" -exec sh -c 'echo "=== $1 ==="; cat "$1"; echo' _ {} \; > AllCode.txt	

Project: ESP32-WROOM-30Pin-Mecanum-IMU-OLED


Hardware Overview & System ArchitectureController: 
    ESP32 NodeMCU (38-Pin)
    Wireless Protocol: ESP-NOW (for real-time remote controller telemetry)
    Display: 1.5" SSD1327 OLED ($128 \times 128$, I2C)
    IMU: Adafruit LSM6DS (6-DOF, I2C)
    Servo Driver: PCA9685 16-Channel PWM Driver (I2C)
    Motor Drivers: 2x MX1508 / Mini L298N (Dual H-Bridge)
    AI Camera / Host Link: Hardware UART2 (HuskyLens / Raspberry Pi interface)
    Wheels: 80mm large mecanum wheels
    Servo Motor: MG995 x 4 for leg joints

ESP-NOW & ADC Conflicts: 
    Utilizing Wi-Fi / ESP-NOW disables ESP32 ADC2 (GPIOs 0, 2, 4, 12–15, 25–27) for analog inputs. All analog sensors (e.g., battery voltage dividers) are mapped exclusively to ADC1 pins (GPIOs 32–39).	

    GPIO 6 to 11: Internal Flash Memory. If you connect anything here, the ESP32 won't boot.
    GPIO 0: Strapping pin (used to enter "Download Mode"). If held LOW at startup, the code won't run.
    GPIO 2: Often connected to the onboard LED. Best to leave it as a status light.
    GPIO 34, 35, 36, 39: Input-Only. These cannot output a PWM signal to a motor driver.

GPIO Pin	Function / Target Component	Direction / Notes								
    GPIO 21	I2C SDA (OLED, IMU, PCA9685, Color Sensor)	Shared I2C Data Bus								
    GPIO 22	I2C SCL (OLED, IMU, PCA9685, Color Sensor)	Shared I2C Clock Bus								
    GPIO 16	UART2 RX2 (HuskyLens TX / Raspberry Pi TX)	Hardware Serial Receive								
    GPIO 17	UART2 TX2 (HuskyLens RX / Raspberry Pi RX)	Hardware Serial Transmit								
    
Motor Drive:

    
    
    GPIO 18	Motor FR - IN4 Output / PWM	
    GPIO 19	Motor FR - IN3 Output / PWM

    GPIO 14	Motor RR - IN2 Output / PWM
    GPIO 27	Motor RR - IN1 Output / PWM

    GPIO 26	Motor FL - IN4 Output / PWM
    GPIO 25	Motor FL - IN3 Output / PWM

    GPIO 04	Motor RL - IN2 Output / PWM								
    GPIO 23	Motor RL - IN1 Output / PWM							
    								
PCA9685 Servo Channel Mapping										
										
All leg and upper-body servos are offloaded to the PCA9685 via I2C 
										
Channel	Assignment	Function								
    Ch 0	Front-Left Leg	Joint Servo 1								
    Ch 1	Front-Right Leg	Joint Servo 2								
    Ch 2	Rear-Left Leg	Joint Servo 3								
    Ch 3	Rear-Right Leg	Joint Servo 4								
								
									



