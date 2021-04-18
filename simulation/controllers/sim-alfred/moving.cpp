#include "controllers/sim-alfred/moving.h"


CMoving::CMoving() {}

CVector3* CMoving::GoInSpecifiedDirection(SensorSide freeSide) {
    Real xVector = 0.0;
    Real yVector = 0.0;
    switch (freeSide) {
        case SensorSide::kDefault:
        case SensorSide::kFront:
            xVector = cos(MAGIC) * SPEED_SRAIGHT;
            yVector = sin(MAGIC) * SPEED_SRAIGHT;
            break;
        case SensorSide::kLeft:
            xVector = cos(MAGIC + PI_DIVIDE_TWO) * SPEED_SIDE;
            yVector = sin(MAGIC + PI_DIVIDE_TWO) * SPEED_SIDE;
            break;
        case SensorSide::kRight:
            xVector = cos(MAGIC - PI_DIVIDE_TWO) * SPEED_SIDE;
            yVector = sin(MAGIC - PI_DIVIDE_TWO) * SPEED_SIDE;
            break;
        case SensorSide::kBack:
            xVector = cos(MAGIC + PI_VALUE) * SPEED_SIDE;
            yVector = sin(MAGIC + PI_VALUE) * SPEED_SIDE;
            break;
        default:
            break;
    }
    return new CVector3(xVector, yVector, 0);
}

float CMoving::computeAngleToFollow(CVector3 objective, CVector3 cPos) {
      float xdiff = objective.GetX() - cPos.GetX();
      float ydiff = objective.GetY() - cPos.GetY();

      if (ydiff < 0) {
         return (- atan(xdiff/ydiff) + PI_VALUE);
      }
      return (- atan(xdiff/ydiff));
}

float CMoving::computeDistanceBetweenPoints(PacketP2P packetP2P, CVector3 cPos) {
    short x = cPos.GetX();
    short y = cPos.GetY();
    return sqrt(pow(packetP2P.x - x, 2.0) + pow(packetP2P.y - y, 2.0));
}

float CMoving::GetAngleToAvoidCollision(
    PacketP2P closestPacket,
    CVector3 cPos,
    float speed[3]) {

    float projectedClosestPacket[2];
    projectedClosestPacket[0] = closestPacket.x + closestPacket.vx;
    projectedClosestPacket[1] = closestPacket.y + closestPacket.vy;
    float projectedCurrentDrone[2];
    projectedCurrentDrone[0] = cPos.GetX()*100 + speed[0]*100;
    projectedCurrentDrone[1] = cPos.GetY()*100 + speed[1]*100;

    float projectedDistanceBetweenDrones =
        sqrt(pow(projectedClosestPacket[0] - projectedCurrentDrone[0], 2.0)
            + pow(projectedClosestPacket[1] - projectedCurrentDrone[1], 2.0));

    float distanceBetweenDrones =
        computeDistanceBetweenPoints(closestPacket, cPos*100);

    if (projectedDistanceBetweenDrones - distanceBetweenDrones > 0) {
        return NO_COLLISION;
    }

    CVector3* projectedPositionOfOtherRobot
        = new CVector3(projectedClosestPacket[0], projectedClosestPacket[1], 0.0);
    CVector3* projectedPositionCurrentRobot
        = new CVector3(projectedCurrentDrone[0], projectedCurrentDrone[1], 0.0);
    return computeAngleToFollow(*projectedPositionCurrentRobot, *projectedPositionOfOtherRobot);
}
