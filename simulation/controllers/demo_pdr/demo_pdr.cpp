#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <fcntl.h>
#include <regex>

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





void CDemoPdr::setPosVelocity() {
   if (m_uiCurrentStep % 10 == 0 && m_uiCurrentStep != 0) {
      posInitial = cPos;
   }
   if (m_uiCurrentStep % 10 == 9) {
      posFinal = cPos;
   }
   
}


/*float CDemoPdr::computeAngleToFollow()
{
   float xdiff = objective.GetX() - cPos.GetX();
   float ydiff = objective.GetY() - cPos.GetY();
   if (std::abs(xdiff) < 1 && std::abs(ydiff) < 1)
   {
      stateMode = kLanding;
      count = 100;
   }
   float length = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
   if (xdiff > 0)
   {
      return asin(ydiff/length);
   }
   return acos(ydiff/length);

}*/


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

      cMoving = new CMoving(m_pcPropellers, m_pcPos);
      cSensors = new CSensors(m_pcDistance);
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
      if (!isConnected) {
         cRadio->connectToServer(idRobot);
      }

      // Update metrics
      const CCI_BatterySensor::SReading& sBatRead = m_pcBattery->GetReading();
      cPos = m_pcPos->GetReading().Position;
      cP2P->sendPacketToOtherRobots(cPos.GetZ(), idRobot);
      setPosVelocity();
      cRadio->sendTelemetry(cPos, stateMode, sBatRead.AvailableCharge);
      cSensors->UpdateValues();

      // Update StateMode received from ground station
      StateMode* stateModeReceived = cRadio->ReceiveData();
      if (stateModeReceived) {
         stateMode = *stateModeReceived;
      }

      switch (stateMode) {
         case kStandby:
            return;
            break;
         case kTakeOff:
            if (sBatRead.AvailableCharge < 0.3) {
               stateMode = kReturnToBase;
            }
            // Check for collision avoidance
            if (cTimer->GetTimer(TimerType::kAvoidTimer) < 0) {
               m_pcPropellers->SetAbsolutePosition(
                  *cP2P->GetNewVectorToAvoidCollision(cPos, idRobot));
            }
            cTimer->CountOneCycle();
            if (m_uiCurrentStep < 20) {  // decolage
               cPos.SetZ(cPos.GetZ() + 0.25f);
               m_pcPropellers->SetAbsolutePosition(cPos);
            } else {
               cMoving->GoInSpecifiedDirection(cSensors->FreeSide());
            }
            // Prevent robot from touching ground
            if (cPos.GetZ() < 0.2) {
               CVector3* test = new CVector3(cPos.GetX(), cPos.GetY(), 0.2);
               m_pcPropellers->SetRelativePosition(*test);
            }
            break;
         case kReturnToBase:
            break;
         case kLanding:
            if (cPos.GetZ() > 0.2) {
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

