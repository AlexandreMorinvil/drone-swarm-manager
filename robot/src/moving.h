#ifndef MOVING_H
#define MOVING_H

#include "sensor.h"
#include <math.h>


typedef struct  Vector3
{
    float x;
    float y;
    float z;
} Vector3;

Vector3 GoInSpecifiedDirection(SensorSide freeSide);


#endif