/* Include the controller definition */
#include "demo_pdr.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector2.h>
/* Logging */
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CDemoPdr::CDemoPdr() :
   m_pcDistance(NULL),
   m_pcPropellers(NULL),
   m_pcPos(NULL),
   m_uiCurrentStep(0) {}

/****************************************/
/****************************************/

void CDemoPdr::Init(TConfigurationNode& t_node) {
   m_pcDistance   = GetSensor  <CCI_CrazyflieDistanceScannerSensor>("crazyflie_distance_scanner");
   m_pcPropellers = GetActuator  <CCI_QuadRotorPositionActuator>("quadrotor_position");
   try {
         m_pcPos = GetSensor  <CCI_PositioningSensor>("positioning");
      }
      catch(CARGoSException& ex) {}
	/*
    * Initialize other stuff
    */
   /* Create a random number generator. We use the 'argos' category so
      that creation, reset, seeding and cleanup are managed by ARGoS. */
   m_pcRNG = CRandom::CreateRNG("argos");

   m_uiCurrentStep = 0;
   Reset();
}

/****************************************/
/****************************************/

void CDemoPdr::ControlStep() {
   CVector3 cPos = m_pcPos->GetReading().Position;
   //Real angle = m_pcPos->GetReading().Orientation.GetZ(); 
   CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead = 
      m_pcDistance->GetReadingsMap();
   auto iterDistRead = sDistRead.begin();
   float frontDist = (iterDistRead++)->second;
   float leftDist = (iterDistRead++)->second;
   float backDist = (iterDistRead++)->second;
   float rightDist = (iterDistRead++)->second;
   /*if (sDistRead.size() == 4) {
      LOG << "Front dist: " << frontDist  << std::endl;
      LOG << "Left dist: "  << leftDist  << std::endl;
      LOG << "Back dist: "  << backDist  << std::endl;
      LOG << "Right dist: " << rightDist  << std::endl;
   }*/
//LOG << m_pcPos->GetReading().Orientation << std::endl;

   if (m_uiCurrentStep < 10)
   {
        cPos.SetZ(cPos.GetZ()+0.5f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else
   {
      LOG << "leftDist : " << leftDist << std::endl;
      CRadians* currentAngle = new CRadians(0.1f);
      CRadians* useless = new CRadians(0.1f);
      m_pcPos->GetReading().Orientation.ToEulerAngles(*currentAngle, *useless, *useless);
      //LOG << "rightDist : " << rightDist << std::endl;
      /*if (rightDist < 20.0f || frontDist < 20.0f || backDist < 20.0f)
      {
          newCVector = new CVector3(
			 (cos(currentAngle->GetValue() + )*0.4 + cPos.GetX())*1,
			 (sin(currentAngle->GetValue())*0.4 + cPos.GetY())*1,
			 cPos.GetZ());
	 m_pcPropellers->SetAbsolutePosition(*newCVector);
      }*/
      if ((leftDist > 80.0f || leftDist == -2) && !isLocked)
      {
         //cPos.SetX(cPos.GetX() + 0.2f);
	 newCVector = new CVector3(
			 (cos(currentAngle->GetValue())*0.4 + cPos.GetX())*1,
			 (sin(currentAngle->GetValue())*0.4 + cPos.GetY())*1,
			 cPos.GetZ());
	 m_pcPropellers->SetAbsolutePosition(*newCVector);
      }
      else
      {
	 *currentAngle = *currentAngle + CRadians::PI_OVER_TWO;
	 firstAngle = currentAngle->GetValue();
	 m_pcPropellers->SetAbsoluteYaw(*currentAngle);
      }
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
