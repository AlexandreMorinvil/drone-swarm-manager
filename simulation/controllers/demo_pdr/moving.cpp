#include "controllers/demo_pdr/moving.h"


void CMoving::updateAngle() {
    currentAngle = new CRadians(0.1f);
    CRadians *useless = new CRadians(0.1f);
    m_pcPos->GetReading()
        .Orientation.ToEulerAngles(*currentAngle, *useless, *useless);
}


CMoving::CMoving(
    CCI_QuadRotorPositionActuator* pcPropellers,
    CCI_PositioningSensor* pcPos) {
    m_pcPropellers = pcPropellers;
    m_pcPos = pcPos;
}

void CMoving::GoInSpecifiedDirection(SensorSide freeSide) {
    updateAngle();
    Real xVector = 0.0;
    Real yVector = 0.0;
    switch (freeSide) {
        case SensorSide::kDefault:
        case SensorSide::kFront:
            xVector = cos(currentAngle->GetValue()) * SPEED_SRAIGHT;
            yVector = sin(currentAngle->GetValue()) * SPEED_SRAIGHT;
            break;
        case SensorSide::kLeft:
            xVector = cos(PI_DIVIDE_FOUR - currentAngle->GetValue())
                * - SPEED_SIDE;
            yVector = sin(PI_DIVIDE_FOUR - currentAngle->GetValue())
                * SPEED_SIDE;
            break;
        case SensorSide::kRight:
            xVector = cos(PI_DIVIDE_FOUR - currentAngle->GetValue())
                * SPEED_SIDE;
            yVector = sin(PI_DIVIDE_FOUR - currentAngle->GetValue())
                * - SPEED_SIDE;
            break;
        case SensorSide::kBack:
            xVector = cos(currentAngle->GetValue() + PI_DIVIDE_FOUR)
                * SPEED_SIDE;
            yVector = sin(currentAngle->GetValue() + PI_DIVIDE_FOUR)
                * SPEED_SIDE;
            break;
        default:
            break;
    }
    if (freeSide != SensorSide::kDefault) {
        m_pcPropellers->SetRelativeYaw(CRadians::PI_OVER_FOUR/3);
    }
    CVector3* newCVector = new CVector3(xVector, yVector, 0);
    m_pcPropellers->SetRelativePosition(*newCVector);
}

CVector3 CMoving::GetPosition() {
    return m_pcPos->GetReading().Position;
}
