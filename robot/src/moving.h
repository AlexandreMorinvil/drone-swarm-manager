#ifndef MOVING_H
#define MOVING_H
#define CLOSE_DISTANCE 500.0f
#define NO_COLLISION -999.0f

#include "sensor.h"
#include <math.h>


typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct PacketOverP2P {
    uint8_t id;
    short x;
    short y;
    short vx;
    short vy;
} __attribute__((packed)) PacketOverP2P;

Vector3 GoInSpecifiedDirection(SensorSide freeSide);

float ComputeAngleToFollow(Vector3 objective, Vector3 cPos);

float ComputeDistanceBetweenPoints(PacketOverP2P packetP2P, Vector3 cPos);

float GetAngleToAvoidCollision(PacketOverP2P packetP2P, Vector3 cPos, float speed[3]);

#endif