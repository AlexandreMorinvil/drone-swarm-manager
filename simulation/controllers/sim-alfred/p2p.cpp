#include "controllers/sim-alfred/p2p.h"
#include <argos3/core/utility/logging/argos_log.h>

CP2P::CP2P(
    CCI_RangeAndBearingSensor* pcRABSens,
    CCI_RangeAndBearingActuator* pcRABAct,
    CTimer* cTimer) {
    m_pcRABSens = pcRABSens;
    m_pcRABAct = pcRABAct;
    _cTimer = cTimer;
}

float CP2P::GetAltitudeToAvoidCollision(CVector3 position, int idRobot) {
    const CCI_RangeAndBearingSensor::TReadings& tMsgs =
        m_pcRABSens->GetReadings();
    float targetAltitude = 0.0;
    bool droneInRange = false;
    if (!tMsgs.empty()) {
        int idMax = -1;
        for (int i = 0; i < tMsgs.size(); i++) {
            if (tMsgs[i].Range < 200.0) {
                droneInRange = true;
                PacketP2P packetReceived = *reinterpret_cast<const PacketP2P*>
                    (tMsgs[i].Data.ToCArray());
                if (packetReceived.id > idMax
                    && packetReceived.id < idRobot) {
                    Real z = position.GetZ();
                    float altitude = packetReceived.currentAltitude;
                    if ((altitude - 0.1 < z) && (altitude + 0.1) > z) {
                        idMax = packetReceived.id;
                        float target = max(0.5, altitude - 0.25);
                        if (target < position.GetZ()) {
                            targetAltitude = - 0.2;
                        }
                    }
                }
            }
        }
    }
    if (!droneInRange) {
        // Go up because there no robot in range
        targetAltitude = 0.01;
    }
    return targetAltitude;
}

void CP2P::sendPacketToOtherRobots(float altitude, int idRobot) {
    struct PacketP2P packet;
    packet.id = idRobot;
    packet.currentAltitude = altitude;
    CByteArray cBuf(10);
    memcpy(&cBuf[0], &packet, sizeof(packet));
    m_pcRABAct->SetData(cBuf);
}