#include "moving.h"


Vector3 GoInSpecifiedDirection(SensorSide freeSide) {
    float xVector = 0.0;
    float yVector = 0.0;
    switch (freeSide) {
        case kDefault:
        case kFront:
            xVector = cos(MAGIC) * SPEED_SRAIGHT;
            yVector = sin(MAGIC) * SPEED_SRAIGHT;
            break;
        case kLeft:
            xVector = cos(MAGIC + PI_DIVIDE_TWO) * SPEED_SIDE;
            yVector = sin(MAGIC + PI_DIVIDE_TWO) * SPEED_SIDE;
            break;
        case kRight:
            xVector = cos(MAGIC - PI_DIVIDE_TWO) * SPEED_SIDE;
            yVector = sin(MAGIC - PI_DIVIDE_TWO) * SPEED_SIDE;
            break;
        case kBack:
            xVector = cos(MAGIC + PI_VALUE) * SPEED_SIDE;
            yVector = sin(MAGIC + PI_VALUE) * SPEED_SIDE;
            break;
        default:
            break;
    }
    struct Vector3 pos;
    pos.x = xVector;
    pos.y = yVector;
    pos.z = 0;
    return pos;
}

float ComputeAngleToFollow(Vector3 objective, Vector3 cPos) {
  float xdiff = objective.x - cPos.x;
  float ydiff = objective.y - cPos.y;

  float yaw = 0.0;

  if (xdiff < 0) {
    yaw = PI_VALUE - atan(ydiff/xdiff);
  } else {
    yaw = atan(ydiff/xdiff);
  }
  if (ydiff < 0) {
    yaw = - yaw;
  }
  return yaw;
}

float ComputeDistanceBetweenPoints(PacketOverP2P packetP2P, Vector3 cPos) {
    short x = cPos.x;
    short y = cPos.y;
    return sqrt(pow(packetP2P.x - x, 2.0) + pow(packetP2P.y - y, 2.0));
}


float GetAngleToAvoidCollision(
    PacketOverP2P closestPacket,
    Vector3 cPos,
    float speed[3]) {

    float projectedClosestPacket[2];
    projectedClosestPacket[0] = closestPacket.x + closestPacket.vx;
    projectedClosestPacket[1] = closestPacket.y + closestPacket.vy;
    float projectedCurrentDrone[2];
    projectedCurrentDrone[0] = cPos.x + speed[0]*100;
    projectedCurrentDrone[1] = cPos.y + speed[1]*100;

    float projectedDistanceBetweenDrones =
        sqrt(pow(projectedClosestPacket[0] - projectedCurrentDrone[0], 2.0)
            + pow(projectedClosestPacket[1] - projectedCurrentDrone[1], 2.0));

    float distanceBetweenDrones =
        ComputeDistanceBetweenPoints(closestPacket, cPos);

    if (projectedDistanceBetweenDrones - distanceBetweenDrones > 0) {
        return NO_COLLISION;
    }

    Vector3 projectedPositionOfOtherRobot;
    projectedPositionOfOtherRobot.x = projectedClosestPacket[0];
    projectedPositionOfOtherRobot.y = projectedClosestPacket[0];
    projectedPositionOfOtherRobot.z = 0;

    Vector3 projectedPositionCurrentRobot;
    projectedPositionCurrentRobot.x = projectedCurrentDrone[0];
    projectedPositionCurrentRobot.y = projectedCurrentDrone[1];
    projectedPositionCurrentRobot.z = 0;

    return ComputeAngleToFollow(projectedPositionCurrentRobot, projectedPositionOfOtherRobot);
}

