#ifndef PACKET_DATA_H

#define PACKET_DATA_H



#include <Arduino.h>



struct PacketData {

float lx, ly, rx, ry; // Joysticks (-1.0 to 1.0)

float pL, pM, pR; // Potentiometers

float roll, pitch, yaw; // IMU Data

int16_t encL, encR; // Encoders

uint32_t buttons; // Button bitmask

};



#endif