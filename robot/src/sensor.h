#ifndef SENSOR_H
#define SENSOR_H

#define CRITICAL_VALUE 400.0f
#define PI_VALUE 3.14
#define PI_DIVIDE_TWO 1.52
#define PI_DIVIDE_FOUR 0.8f
#define SPEED_SRAIGHT 0.4
#define SPEED_SIDE 0.4
#define MAGIC 0.0

#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include <fcntl.h>

typedef enum { 
    kLeft, kBack, kRight, kFront, kDefault
} SensorSide;

SensorSide* prioritise(float angle);
SensorSide FreeSide(float sensorValues[4]);
SensorSide CriticalProximity(float sensorValues[4]);
SensorSide ReturningSide(float sensorValues[4], float angle);

#endif