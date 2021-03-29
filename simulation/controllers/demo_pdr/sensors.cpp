#include "controllers/demo_pdr/sensors.h"


SensorSide* prioritise(float angle) {
    SensorSide* retval = new SensorSide[4];
    if (angle < 0) {
        retval[1] = SensorSide::kBack;
        retval[3] = SensorSide::kFront;
        if (angle < - PI_DIVIDE_TWO) {
            retval[0] = SensorSide::kRight;
            retval[2] = SensorSide::kLeft;
        } else {
            retval[0] = SensorSide::kLeft;
            retval[2] = SensorSide::kRight;
        }
    } else {
        retval[3] = SensorSide::kBack;
        retval[1] = SensorSide::kFront;
        if (angle < PI_DIVIDE_TWO) {
            retval[0] = SensorSide::kLeft;
            retval[2] = SensorSide::kRight;
        } else {
            retval[0] = SensorSide::kRight;
            retval[2] = SensorSide::kLeft;
        }
    }
    return retval;
}



CSensors::CSensors() { }


SensorSide CSensors::FreeSide(float sensorValues[4]) {
    SensorSide closeSens = CriticalProximity(sensorValues);
    if (closeSens == SensorSide::kDefault) return closeSens;
    SensorSide oppSens = (SensorSide) ((closeSens +2) % 4);
    if (sensorValues[oppSens] == -2
      || sensorValues[oppSens] > 2 * CRITICAL_VALUE)
        return oppSens;
    float max   = sensorValues[closeSens];
    SensorSide maxSensor = SensorSide::kDefault;
    for (unsigned i = 4; i > -1; i--) {
        if (sensorValues[i] == -2 ) return (SensorSide) i;
        if (max < sensorValues[i]) {
            max = sensorValues[i];
            maxSensor = (SensorSide) i;
        }
    }
    return maxSensor;
}

SensorSide CSensors::CriticalProximity(float sensorValues[4]) {
    float min   = CRITICAL_VALUE;
    SensorSide minSensor = SensorSide::kDefault;

    for (unsigned i = 0; i < 4; i++) {
        if (min > sensorValues[i] && sensorValues[i] > 0.0) {
            min = sensorValues[i];
            minSensor = (SensorSide) i;
        }
    }

    return minSensor;
}


SensorSide CSensors::ReturningSide(float sensorValues[4], float angle) {
    SensorSide closeSens = CriticalProximity(sensorValues);
    if (closeSens == SensorSide::kDefault) return closeSens;
    SensorSide* prioList = prioritise(angle);
    for (unsigned i =0 ; i < 4; ++i) {
        int index = static_cast<int>(prioList[i]);
        if (sensorValues[index] > 2* CRITICAL_VALUE ||
            sensorValues[index] == -2)
            return prioList[i];
    }
    return FreeSide(sensorValues);

}

