/* Include the controller definition */
#include "demo_pdr.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector2.h>
/* Logging */
#include <argos3/core/utility/logging/argos_log.h>

#define CRITICAL_VALUE 40.0f
/****************************************/
/****************************************/

CVector3 objective = *(new CVector3(0,0,0));
bool returnToBase = false;
bool hasArrived = false;

float CDemoPdr::computeAngleToFollow()
{
   float xdiff = objective.GetX() - cPos.GetX();
   float ydiff = objective.GetY() - cPos.GetY();
   if (std::abs(xdiff) < 1 && std::abs(ydiff) < 1)
   {
      hasArrived = true;
      count = 100;
   }
   float length = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
   if (xdiff > 0)
   {
      return asin(ydiff/length);
   }
   return acos(ydiff/length);
}


CRadians* decideTurn(float left, float right)
{
   return new CRadians(CRadians::PI_OVER_FOUR);
}
CDemoPdr::CDemoPdr() : m_pcDistance(NULL),
                       m_pcPropellers(NULL),
                       m_pcPos(NULL),
                       m_uiCurrentStep(0) {}

/****************************************/
/****************************************/
SensorSide CDemoPdr::CriticalProximity() {
   float sensor[3]  = {leftDist, backDist, rightDist};
   float min   = CRITICAL_VALUE;
   SensorSide minSensor = SensorSide::kDefault;

   for (unsigned i = 0; i < 3; i++) {
      if (min > sensor[i] && sensor[i] > 0.0) 
      {
         min = sensor[i];
         minSensor = (SensorSide) i;
      }
   }

   if (((frontDist < 90.0 && frontDist > 0) && minSensor == SensorSide::kDefault)
        || (frontDist < min && frontDist > 0)){
      minSensor = SensorSide::kFront;
   }

   return minSensor;
}

void CDemoPdr::Init(TConfigurationNode &t_node)
{
   m_pcRABSens     = GetSensor <CCI_RangeAndBearingSensor>("range_and_bearing" );
   m_pcRABAct      = GetActuator <CCI_RangeAndBearingActuator>("range_and_bearing");
   m_pcDistance = GetSensor<CCI_CrazyflieDistanceScannerSensor>("crazyflie_distance_scanner");
   m_pcPropellers = GetActuator<CCI_QuadRotorPositionActuator>("quadrotor_position");
   try
   {
      m_pcPos = GetSensor<CCI_PositioningSensor>("positioning");
   }
   catch (CARGoSException &ex)
   {
   }
   /*
    * Initialize other stuff
    */
   /* Create a random number generator. We use the 'argos' category so
      that creation, reset, seeding and cleanup are managed by ARGoS. */

   m_pcRNG = CRandom::CreateRNG("argos");

   count = 0;
   m_uiCurrentStep = 0;
   Reset();
   lockAngle = *(new CRadians(0.1f));
   CRadians *useless = new CRadians(0.1f);
   m_pcPos->GetReading().Orientation.ToEulerAngles(lockAngle, *useless, *useless);
   LOG << "LOCKANGLE : " << lockAngle << std::endl;
   objective = m_pcPos->GetReading().Position;
}

/****************************************/
/****************************************/

void CDemoPdr::ControlStep()
{
   cPos = m_pcPos->GetReading().Position;
   //Real angle = m_pcPos->GetReading().Orientation.GetZ();
   CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead = m_pcDistance->GetReadingsMap();
   auto iterDistRead = sDistRead.begin();
   rightDist = (iterDistRead++)->second;
   frontDist = (iterDistRead++)->second;
   leftDist = (iterDistRead++)->second;
   backDist = (iterDistRead++)->second;
   if (sDistRead.size() == 4)
   {
      LOG << "Front dist: " << frontDist << std::endl;
      LOG << "Left dist: " << leftDist << std::endl;
      LOG << "Back dist: " << backDist << std::endl;
      LOG << "Right dist: " << rightDist << std::endl;
   }
   //LOG << m_pcPos->GetReading().Orientation << std::endl;

   struct Packet packet;
      packet.test = 1.5;
      CByteArray cBuf(10);
      memcpy(&cBuf[0], &packet, sizeof(packet));
      if (GetId() == "s0")
      {
         LOG << "Send Packet (from: " << GetId() << "): " << packet.test << std::endl;
         m_pcRABAct->SetData(cBuf);
      }

   if (m_uiCurrentStep > 1000)
   {
      returnToBase = true;
   }

   count--;
   if (m_uiCurrentStep < 20) // decolage
   {
      cPos.SetZ(cPos.GetZ() + 0.25f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else if (!hasArrived)
   {
      lockAngle = *(new CRadians(0.1f));
      CRadians *useless = new CRadians(0.1f);
      m_pcPos->GetReading().Orientation.ToEulerAngles(lockAngle, *useless, *useless);
      
            
      switch (CriticalProximity()) {
        case SensorSide::kDefault:
            LOG << "kDefault" << std::endl;
            turnAngle = nullptr;
            newCVector = new CVector3(
               (cos(lockAngle.GetValue()) * 0.4 + cPos.GetX()) * 1,
               (sin(lockAngle.GetValue()) * 0.4 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            if (returnToBase) {
               m_pcPropellers->SetAbsoluteYaw(*(new CRadians(computeAngleToFollow())));
            }
         break;
            
        case SensorSide::kRight:
            LOG << "kRight" << std::endl;
            newCVector = new CVector3(
               (cos(lockAngle.GetValue() - 1.56) * -0.5 + cPos.GetX()) * 1,
               (sin(lockAngle.GetValue() - 1.56) * -0.5 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            break;
        case SensorSide::kLeft:
            LOG << "kLeft" << std::endl;
            newCVector = new CVector3(
               (cos(lockAngle.GetValue() - 1.56) * 0.5 + cPos.GetX()) * 1,
               (sin(lockAngle.GetValue() - 1.56) * 0.5 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            break;
        case SensorSide::kBack:
            LOG << "kBack" << std::endl;
            newCVector = new CVector3(
               (cos(lockAngle.GetValue() + 0.8) * 0.5 + cPos.GetX()) * 1,
               (sin(lockAngle.GetValue() + 0.8) * 0.5 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            break;
        case SensorSide::kFront:
            LOG << "lockAngle : " << lockAngle << std::endl;
            m_pcPropellers->SetRelativeYaw(CRadians::PI_OVER_FOUR/3);
	         //count = 40;
            break;
      }
   }
   else if (hasArrived && count <= 0)
   {
      if (cPos.GetZ() > 0.2)
      {
         CVector3* test = new CVector3(0,0,-0.1);
         m_pcPropellers->SetRelativePosition(*test);
      }
     
   }

   checkIfPacketIsComing();
   m_uiCurrentStep++;
}



/****************************************/
/****************************************/

void CDemoPdr::Reset()
{
   m_uiCurrentStep = 0;
}

/****************************************/
/****************************************/

void CDemoPdr::checkIfPacketIsComing()
{
   const CCI_RangeAndBearingSensor::TReadings& tMsgs = m_pcRABSens->GetReadings();
   if (! tMsgs.empty() && GetId() == "s1") {
     Packet packetReceived = *reinterpret_cast<const Packet*>(tMsgs[0].Data.ToCArray());
     if (packetReceived.test != 0)
     {
        LOG << "Packet Received (from:" << GetId() << " ): " << packetReceived.test << std::endl;
     }
   }
}


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

