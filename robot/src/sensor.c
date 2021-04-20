#include "sensor.h"


SensorSide* prioritise(float angle) {
    SensorSide sensor[4];
    SensorSide* retval = malloc(sizeof(sensor));
    retval[2] = kBack;
    if (angle < - (float)PI_DIVIDE_TWO ||angle > (float)PI_DIVIDE_TWO) {
            retval[0] = kLeft;
            retval[1] = kRight;
    } else {
            retval[0] = kRight;
            retval[1] = kLeft;
    }
    return retval;
}



SensorSide FreeSide(float sensorValues[4]) {
    SensorSide closeSens = CriticalProximity(sensorValues);
    if (closeSens == kDefault) return closeSens;
    SensorSide oppSens = (SensorSide) ((closeSens +2) % 4);
    if (sensorValues[oppSens] == -2
      || sensorValues[oppSens] > 2 * CRITICAL_VALUE)
        return oppSens;
    float max   = sensorValues[closeSens];
    SensorSide maxSensor = kDefault;
    for (unsigned i = 4; i > -1; i--) {
        if (sensorValues[i] == -2 ) return (SensorSide) i;
        if (max < sensorValues[i]) {
            max = sensorValues[i];
            maxSensor = (SensorSide) i;
        }
    }
    return maxSensor;
}

SensorSide CriticalProximity(float sensorValues[4]) {
    float min   = CRITICAL_VALUE;
    SensorSide minSensor = kDefault;

    for (unsigned i = 0; i < 4; i++) {
        if (min > sensorValues[i] && sensorValues[i] > 0.0f) {
            min = sensorValues[i];
            minSensor = (SensorSide) i;
        }
    }

    return minSensor;
}


SensorSide ReturningSide(float sensorValues[4], float angle) {
    SensorSide closeSens = CriticalProximity(sensorValues);
    if (closeSens == kDefault) return closeSens;
    SensorSide* prioList = prioritise(angle);
    for (unsigned i =0 ; i < 2; ++i) {
        int index = (int)(prioList[i]);
        if (sensorValues[index] > 2*CRITICAL_VALUE ||
            sensorValues[index] == -2)
            return prioList[i];
    }
    return FreeSide(sensorValues);
}
