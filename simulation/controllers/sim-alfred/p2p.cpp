#include "controllers/sim-alfred/p2p.h"
#include <argos3/core/utility/logging/argos_log.h>

CP2P::CP2P(
    CCI_RangeAndBearingSensor* pcRABSens,
    CCI_RangeAndBearingActuator* pcRABAct,
    CTimer* cTimer,
    CMoving* cMoving) {
    m_pcRABSens = pcRABSens;
    m_pcRABAct = pcRABAct;
    cTimer = cTimer;
    cMoving = cMoving;
}



PacketP2P CP2P::GetClosestPacket(CVector3 cPos) {
    const CCI_RangeAndBearingSensor::TReadings& tMsgs =
        m_pcRABSens->GetReadings();
    float closestDistance = CLOSE_DISTANCE;
    PacketP2P closestPacket;
    if (!tMsgs.empty()) {
        for (int i = 0; i < tMsgs.size(); i++) {
            PacketP2P packetReceived = *reinterpret_cast<const PacketP2P*>
                (tMsgs[i].Data.ToCArray());
            float distance = cMoving
                ->computeDistanceBetweenPoints(packetReceived, cPos*100);
            if (distance < closestDistance) {
                closestPacket = packetReceived;
                closestDistance = distance;
            }
        }
    }
    // If no robot is close
    if (closestDistance == CLOSE_DISTANCE) {
        closestPacket.id = -1;
        return closestPacket;
    }

    return closestPacket;
}

void CP2P::sendPacketToOtherRobots(int idRobot, CVector3 cPos, float speed[3]) {
    struct PacketP2P packet;
    packet.id = idRobot;
    packet.x = cPos.GetX()*100;
    packet.y = cPos.GetY()*100;
    packet.vx = speed[0]*100;
    packet.vy = speed[1]*100;
    CByteArray cBuf(10);
    memcpy(&cBuf[0], &packet, sizeof(packet));
    m_pcRABAct->SetData(cBuf);
}