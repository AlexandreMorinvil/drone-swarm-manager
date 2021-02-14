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
   m_pcPropellers(NULL),
   m_pcPos(NULL),
   m_uiCurrentStep(0) {}

/****************************************/
/****************************************/

void CDemoPdr::Init(TConfigurationNode& t_node) {
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
   if (m_uiCurrentStep < 10)
   {
        cPos.SetZ(cPos.GetZ()+0.5f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else if (m_uiCurrentStep < 20)
   {
           cPos.SetX(cPos.GetX()+0.5f);
        m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else if (m_uiCurrentStep < 30)
   {
            cPos.SetY(cPos.GetY()-0.5f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else if (m_uiCurrentStep < 40)
   {
           cPos.SetX(cPos.GetX()-0.5f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else if (m_uiCurrentStep < 50)
   {
           cPos.SetY(cPos.GetY()+0.5f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   else if (m_uiCurrentStep < 60)
   {
           cPos.SetZ(cPos.GetZ()-0.25f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }
   //TakeOff();
   m_uiCurrentStep++;
}

/****************************************/
/****************************************/

void CDemoPdr::Reset() {
   m_uiCurrentStep = 0;
 }

bool CDemoPdr::Land() {
   CVector3 cPos = m_pcPos->GetReading().Position;
   if (Abs(cPos.GetZ()) < 0.01f) return false;
   cPos.SetZ(0.0f);
   m_pcPropellers->SetAbsolutePosition(cPos);
   return true;
}

bool CDemoPdr::TakeOff() {
CVector3 cPos = m_pcPos->GetReading().Position;
   if (Abs(cPos.GetZ() - 2.0f) < 0.01f) return false;
   cPos.SetZ(2.0f);
   m_pcPropellers->SetAbsolutePosition(cPos);
   return true;
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