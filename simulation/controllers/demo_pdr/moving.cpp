#include "controllers/demo_pdr/moving.h"


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
