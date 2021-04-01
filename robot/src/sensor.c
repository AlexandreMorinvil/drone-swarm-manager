#include "sensor.h"


SensorSide* prioritise(float angle) {
    printf("angle : %f", angle);
    SensorSide sensor[4];
    SensorSide* retval = sensor;
    if (angle < 0) {
        retval[1] = kBack;
        retval[3] = kFront;
        if (angle < - PI_DIVIDE_TWO) {
            retval[0] = kLeft;
            retval[2] = kRight;
        } else {
            retval[0] = kRight;
            retval[2] = kLeft;
        }
    } else {
        retval[3] = kBack;
        retval[1] = kFront;
        if (angle < PI_DIVIDE_TWO) {
            retval[0] = kRight;
            retval[2] = kLeft;
        } else {
            retval[0] = kLeft;
            retval[2] = kRight;
        }
    }
    return retval;
}


// CSensors::CSensors() { }


SensorSide FreeSide(Sensor *sensor, float sensorValues[4]) {
    SensorSide closeSens = CriticalProximity(sensor, sensorValues);
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

SensorSide CriticalProximity(Sensor *sensor, float sensorValues[4]) {
    float min   = CRITICAL_VALUE;
    SensorSide minSensor = kDefault;

    for (unsigned i = 0; i < 4; i++) {
        if (min > sensorValues[i] && sensorValues[i] > 0.0) {
            min = sensorValues[i];
            minSensor = (SensorSide) i;
        }
    }

    return minSensor;
}


SensorSide ReturningSide(Sensor *sensor, float sensorValues[4], float angle) {
    SensorSide closeSens = CriticalProximity(sensor, sensorValues);
    if (closeSens == kDefault) return closeSens;
    SensorSide* prioList = prioritise(angle);
    for (unsigned i =0 ; i < 4; ++i) {
        int index = (int)(prioList[i]);
        if (sensorValues[index] > 2* CRITICAL_VALUE ||
            sensorValues[index] == -2)
            return prioList[i];
    }
    return FreeSide(sensor, sensorValues);
}
