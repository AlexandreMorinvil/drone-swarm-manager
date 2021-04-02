#include "moving.h"


Vector3* GoInSpecifiedDirection(SensorSide freeSide) {
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
    Vector3 *pos =  malloc(sizeof(Vector3));
    pos->x = xVector;
    pos->y = yVector;
    pos->z = 0;
    return pos;
}
