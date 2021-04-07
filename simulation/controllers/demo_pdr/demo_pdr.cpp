#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <fcntl.h>
#include <regex>
#include <math.h>

/* Include the controller definition */
#include "demo_pdr.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector2.h>
/* Logging */
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include<argos3/plugins/simulator/entities/box_entity.h>




/****************************************/
/****************************************/


void CDemoPdr::checkForCollisionAvoidance() {
      if (cTimer->GetTimer(TimerType::kAvoidTimer) < 0) {
         m_pcPropellers->SetAbsolutePosition(
            *cP2P->GetNewVectorToAvoidCollision(cPos, idRobot));
      }

      // Prevent robot from touching ground
      if (cPos.GetZ() < 0.2) {
         CVector3* vector = new CVector3(cPos.GetX(), cPos.GetY(), 0.2);
         m_pcPropellers->SetRelativePosition(*vector);
      }
}

float CDemoPdr::computeAngleToFollow() {
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

CDemoPdr::CDemoPdr() : m_pcDistance(NULL),
                       m_pcPropellers(NULL),
                       m_pcPos(NULL),
                       m_uiCurrentStep(0) {}


/****************************************/
/****************************************/


void CDemoPdr::Init(TConfigurationNode &t_node) {
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

void CDemoPdr::ControlStep() {
      if (!cRadio->connectToServer(idRobot)) {
         return;
      }

      // Update metrics
      // Orientation
      CRadians yawAngle = CRadians(0.0f);
      CRadians pitchAngle = CRadians(0.0f);
      CRadians rollAngle = CRadians(0.0f);
      m_pcPos->GetReading().Orientation.ToEulerAngles(yawAngle, pitchAngle, rollAngle);
      float orientationValues[3];
      orientationValues[0] = yawAngle.GetValue()  + M_PI/2;
      orientationValues[1] = pitchAngle.GetValue();
      orientationValues[2] = rollAngle.GetValue();

      // Battery
      const CCI_BatterySensor::SReading& sBatRead = m_pcBattery->GetReading();

      // Position
      previousPos = cPos;
      cPos = m_pcPos->GetReading().Position;
      cP2P->sendPacketToOtherRobots(cPos.GetZ(), idRobot);

      // Speed
      float speedValues[3];
      speedValues[0] = (cPos.GetX() - previousPos.GetX()) / SECONDS_PER_STEP;
      speedValues[1] = (cPos.GetY() - previousPos.GetY()) / SECONDS_PER_STEP;
      speedValues[2] = (cPos.GetZ() - previousPos.GetZ()) / SECONDS_PER_STEP;

      // Range distances
      // [ leftDist, backDist, rightDist, frontDist, downDistance, upDistance ]
      CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead = m_pcDistance->GetReadingsMap();
      auto iterDistRead = sDistRead.begin();
      float sensorValues[6];
      sensorValues[1] = (iterDistRead++)->second;  // Back
      sensorValues[2] = (iterDistRead++)->second;  // Right
      sensorValues[3] = (iterDistRead++)->second;  // Front
      sensorValues[0] = (iterDistRead++)->second;  // Left
      sensorValues[4] = cPos.GetZ();               // Height
      sensorValues[5] = ROOF_HEIGHT - cPos.GetZ(); // Roof distance

      if (GetId() == "s0") {
         // LOG << "FRONT_SENSOR " << sensorValues[3] << std::endl;
         // LOG << "POSITION : " << cPos.GetX() << " ; " << cPos.GetY() << " ; " << cPos.GetZ() << std::endl;
         LOG << "YAW" << orientationValues[0] << std::endl;
      }

      cRadio->sendTelemetry(cPos, stateMode, sBatRead.AvailableCharge, sensorValues, orientationValues, speedValues);

      // Update StateMode received from ground station
      StateMode* stateModeReceived = cRadio->ReceiveData();
      if (stateModeReceived) {
         stateMode = *stateModeReceived;
      }

      // LOG << "stateMode : " << stateMode << std::endl;

      cTimer->CountOneCycle();

      switch (stateMode) {
         case kStandby:
            return;
            break;
         case kTakeOff:
            // if (sBatRead.AvailableCharge < 0.3) {
            //   stateMode = kReturnToBase;
            //}
            // Check for collision avoidance
            checkForCollisionAvoidance();
            if (m_uiCurrentStep < 20) {  // decolage
               cPos.SetZ(cPos.GetZ() + 0.25f);
               m_pcPropellers->SetAbsolutePosition(cPos);
            } else {
               if (sensorValues[3] < 130 && sensorValues[3] != -2.0) {
                  m_pcPropellers->SetRelativeYaw(CRadians::PI_OVER_FOUR/2);
               }
               m_pcPropellers->SetRelativePosition(
                  *cMoving->GoInSpecifiedDirection(
                     cSensors->FreeSide(sensorValues)));
            }
            break;
         case kReturnToBase:
            checkForCollisionAvoidance();
            m_pcPropellers->SetRelativePosition(
               *cMoving->GoInSpecifiedDirection(
                  cSensors->ReturningSide(sensorValues,
                     computeAngleToFollow())));

            if (cSensors->CriticalProximity(sensorValues) == kDefault) {
               m_pcPropellers->SetAbsoluteYaw(
                     (*new CRadians(computeAngleToFollow())));
            }
            break;
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

void CDemoPdr::Reset() {
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
REGISTER_CONTROLLER(CDemoPdr, "demo_pdr_controller")

