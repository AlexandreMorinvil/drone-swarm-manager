#include "sim-alfred.h"


/****************************************/
/****************************************/


void CSimAlfred::setPosVelocity() {
      if (m_uiCurrentStep % 10 == 0 && m_uiCurrentStep != 0) {
         posInitial = cPos;
      }
      if (m_uiCurrentStep % 10 == 9) {
         posFinal = cPos;
      }
}


float CSimAlfred::computeAngleToFollow() {
      float xdiff = objective.GetX() - cPos.GetX();
      float ydiff = objective.GetY() - cPos.GetY();

      if (std::abs(xdiff) < 0.5 && std::abs(ydiff) < 0.5) {
         stateMode = kLanding;
         cTimer->SetTimer(TimerType::kLandingTimer, 100);
      }
      if (ydiff < 0) {
         return (- atan(xdiff/ydiff) + PI_VALUE);
      }
      return (- atan(xdiff/ydiff));
}


CSimAlfred::CSimAlfred() : m_pcDistance(NULL),
                       m_pcPropellers(NULL),
                       m_pcPos(NULL),
                       m_uiCurrentStep(0) {}


/****************************************/
/****************************************/


void CSimAlfred::Init(TConfigurationNode &t_node) {
      m_pcRABSens =
         GetSensor<CCI_RangeAndBearingSensor>("range_and_bearing");
      m_pcRABAct =
         GetActuator<CCI_RangeAndBearingActuator>("range_and_bearing");
      m_pcDistance =
         GetSensor<CCI_CrazyflieDistanceScannerSensor>
         ("crazyflie_distance_scanner");
      m_pcPropellers =
         GetActuator<CCI_QuadRotorPositionActuator>("quadrotor_position");

      try {
         m_pcBattery = GetSensor<CCI_BatterySensor>("battery");
         m_pcPos = GetSensor<CCI_PositioningSensor>("positioning");
      } catch (CARGoSException &ex) {
      }

      cMoving = new CMoving();
      cSensors = new CSensors();
      cTimer = new CTimer();
      cP2P = new CP2P(m_pcRABSens, m_pcRABAct, cTimer);
      cRadio = new CRadio();

      m_pcRNG = CRandom::CreateRNG("argos");

      m_uiCurrentStep = 0;
      Reset();

      objective = m_pcPos->GetReading().Position;
      posInitial = *(new CVector3(0, 0, 0));
      posFinal   = *(new CVector3(0, 0, 0));
      stateMode = kStandby;
      std::regex regular_exp("[0-9].*");
      std::smatch sm;
      regex_search(GetId(), sm, regular_exp);
      idRobot = stoi(sm[0]);
}

/****************************************/
/****************************************/

void CSimAlfred::ControlStep() {
      if (!cRadio->connectToServer(idRobot)) {
         return;
      }

      // Update metrics
      CRadians* currentAngle = new CRadians(0.1f);
      CRadians *useless = new CRadians(0.1f);
      m_pcPos->GetReading()
        .Orientation.ToEulerAngles(*currentAngle, *useless, *useless);
      const CCI_BatterySensor::SReading& sBatRead = m_pcBattery->GetReading();
      cPos = m_pcPos->GetReading().Position;
      cP2P->sendPacketToOtherRobots(cPos.GetZ(), idRobot);
      setPosVelocity();
      cRadio->sendTelemetry(cPos, stateMode, sBatRead.AvailableCharge);

      CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead =
        m_pcDistance->GetReadingsMap();
      auto iterDistRead = sDistRead.begin();
      float sensorValues[4];
      // {leftDist, backDist, rightDist, frontDist};
      sensorValues[1] = (iterDistRead++)->second;
      sensorValues[2] = (iterDistRead++)->second;
      sensorValues[3] = (iterDistRead++)->second;
      sensorValues[0] = (iterDistRead++)->second;

      // Update StateMode received from ground station
      StateMode* stateModeReceived = cRadio->ReceiveData();
      if (stateModeReceived) {
         stateMode = *stateModeReceived;
      }

      cTimer->CountOneCycle();

      switch (stateMode) {
         case kStandby:
            return;
            break;
         case kTakeOff:
         {
            if (sBatRead.AvailableCharge < 0.3) {
               stateMode = kReturnToBase;
            }
            if (m_uiCurrentStep < 20) {  // decolage
               cPos.SetZ(cPos.GetZ() + 0.25f);
               m_pcPropellers->SetAbsolutePosition(cPos);
            } else {
               if (sensorValues[3] < 130 && sensorValues[3] != -2.0) {
                  m_pcPropellers->SetRelativeYaw(CRadians::PI_OVER_FOUR/2);
               }
               CVector3* vector = cMoving->GoInSpecifiedDirection(
                     cSensors->FreeSide(sensorValues));
               vector->SetZ(cP2P->GetAltitudeToAvoidCollision(cPos, idRobot));
               // Prevent robot from touching ground
               if (cPos.GetZ() < 0.2) {
                  vector->SetZ(0.1);
               }
               m_pcPropellers->SetRelativePosition(*vector);
            }
            break;
         }
         case kReturnToBase:
         {
            if (sensorValues[3] > 130 || sensorValues[3] == -2.0) {
               m_pcPropellers->SetAbsoluteYaw(
                  *new CRadians(computeAngleToFollow()));
            }
            CVector3* vector = cMoving->GoInSpecifiedDirection(
               cSensors->ReturningSide(sensorValues, computeAngleToFollow()));

            vector->SetZ(cP2P->GetAltitudeToAvoidCollision(cPos, idRobot));
            // Prevent robot from touching ground
            if (cPos.GetZ() < 0.2) {
               vector->SetZ(0.1);
            }
            m_pcPropellers->SetRelativePosition(*vector);
            break;
         }
         case kLanding:
            if (cPos.GetZ() > 0.2
             && cTimer->GetTimer(TimerType::kLandingTimer) < 0) {
               CVector3* test = new CVector3(0, 0, -0.1);
               m_pcPropellers->SetRelativePosition(*test);
            }
            break;
      }
      m_uiCurrentStep++;
}




/****************************************/
/****************************************/

void CSimAlfred::Reset() {
      m_uiCurrentStep = 0;
}

/****************************************/
/****************************************/

/*
 * This statement notifies ARGoS of the existence of the controller.
 * It binds the class passed as first argument to the string passed as
 * second argument.
 * The string is then usable in the XML configuration file to refer to
 * this controller.
 * When ARGoS reads that string in the XML file, it knows which controller
 * class to instantiate.
 * See also the XML configuration files for an example of how this is used.
 */
REGISTER_CONTROLLER(CSimAlfred, "demo_pdr_controller")

