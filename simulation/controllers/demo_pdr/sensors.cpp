#include "controllers/demo_pdr/sensors.h"


CSensors::CSensors(
    argos::CCI_CrazyflieDistanceScannerSensor* pcDistance) {
    m_pcDistance = pcDistance;
}

void CSensors::UpdateValues() {
    CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead =
        m_pcDistance->GetReadingsMap();
    auto iterDistRead = sDistRead.begin();
    rightDist = (iterDistRead++)->second;
    frontDist = (iterDistRead++)->second;
    leftDist = (iterDistRead++)->second;
    backDist = (iterDistRead++)->second;
}

SensorSide CSensors::FreeSide() {
    float sensor[4]  = {leftDist, backDist, rightDist, frontDist};
    SensorSide closeSens = CriticalProximity();
    if (closeSens == SensorSide::kDefault) return closeSens;
    SensorSide oppSens = (SensorSide) ((closeSens +2) % 4);
    if (sensor[oppSens] == -2 || sensor[oppSens] > 2 * CRITICAL_VALUE )
        return oppSens;
    float max   = sensor[closeSens];
    SensorSide maxSensor = SensorSide::kDefault;
    for (unsigned i = 0; i < 4; i++) {
        if (sensor[i] == -2 ) return (SensorSide) i;
        if (max < sensor[i]) {
            max = sensor[i];
            maxSensor = (SensorSide) i;
        }
    }
    return maxSensor;
}

SensorSide CSensors::CriticalProximity() {
    float sensor[4]  = {leftDist, backDist, rightDist, frontDist};
    float min   = CRITICAL_VALUE;
    SensorSide minSensor = SensorSide::kDefault;

    for (unsigned i = 0; i < 4; i++) {
        if (min > sensor[i] && sensor[i] > 0.0) {
            min = sensor[i];
            minSensor = (SensorSide) i;
        }
    }

    if (((frontDist < 130.0 && frontDist > 0)
            && minSensor == SensorSide::kDefault)
        || (frontDist < min && frontDist > 0)) {
        minSensor = SensorSide::kFront;
    }

    return minSensor;
}
