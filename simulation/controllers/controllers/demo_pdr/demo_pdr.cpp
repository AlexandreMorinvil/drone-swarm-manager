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



#define DEFAULT_PORT 5015
#define CRITICAL_VALUE 70.0f

typedef enum {
   tx,
   position,
   attitude,
   velocity,
   distance
} PacketType;

struct packetRX {
  bool led_activation;
} __attribute__((packed));

struct PacketPosition {
  PacketType packetType;
  float x;
  float y;
  float z;
} __attribute__((packed));

struct PacketTX {
  PacketType packetType;
  bool isLedActivated;
  float vbat;
  uint8_t rssiToBase;
} __attribute__((packed));

struct PacketVelocity {
  PacketType packetType;
  float px;
  float py;
  float pz;
} __attribute__((packed));

struct PacketDistance {
  PacketType packetType;
  uint16_t front;
  uint16_t back;
  uint16_t up;
  uint16_t left;
  uint16_t right;
  uint16_t zrange;
} __attribute__((packed));



/****************************************/
/****************************************/


int CDemoPdr::getIntId()
{
   std::regex regular_exp("[0-9].*");
   std::smatch sm;
   regex_search(GetId(), sm, regular_exp);
   return stoi(sm[0]);
}

void CDemoPdr::connectToServer()
{
   sock = 0;
   isConnected = true;

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
   { 
      printf("\n Socket creation error \n"); 
      return; 
   }
   serv_addr.sin_family = AF_INET;

   std::regex regular_exp("[0-9].*");
   std::smatch sm;
   regex_search(GetId(), sm, regular_exp);
   serv_addr.sin_port = htons(DEFAULT_PORT + stoi(sm[0]));
   

   // Convert IPv4 and IPv6 addresses from text to binary form 
   if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
   { 
      printf("\nInvalid address/ Address not supported \n"); 
      return; 
   }
   isConnected = true;

   
   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
   { 
      printf("\nConnection Failed \n");
   }
}

void CDemoPdr::setPosVelocity() {
   if (m_uiCurrentStep % 10 == 0 && m_uiCurrentStep != 0) {
      posInitial = cPos;
   }
   if (m_uiCurrentStep % 10 == 9) {
      posFinal = cPos;
   }
   
}


void CDemoPdr::sendTelemetry()
{
   // Unblock socket
   int flags = fcntl(sock, F_GETFL);
   fcntl(sock, F_SETFL, flags | O_NONBLOCK);

   
   struct PacketPosition packetPosition;
   packetPosition.x = cPos.GetX();
   packetPosition.y = cPos.GetY();
   packetPosition.z = cPos.GetZ();
   packetPosition.packetType = position;
   send(sock, &packetPosition, sizeof(packetPosition), 0 );

   struct PacketVelocity packetVelocity;
   packetVelocity.packetType = velocity;
   packetVelocity.px = (posFinal.GetX() - posInitial.GetX());
   packetVelocity.py = (posFinal.GetY() - posInitial.GetY());
   packetVelocity.pz = (posFinal.GetZ() - posInitial.GetZ());
   send(sock, &packetVelocity, sizeof(packetVelocity), 0 );

   struct PacketDistance packetDistance;
   packetDistance.packetType = distance;
   packetDistance.front = 0;
   packetDistance.left = 0;
   packetDistance.right = 0;
   packetDistance.up = 0;
   packetDistance.back = 0;
   packetDistance.zrange = 0;
   send(sock, &packetDistance, sizeof(packetDistance), 0 );

   const CCI_BatterySensor::SReading& sBatRead = m_pcBattery->GetReading();
   struct PacketTX packetTx;
   packetTx.packetType = tx;
   packetTx.isLedActivated = true;
   packetTx.vbat = sBatRead.AvailableCharge;
   packetTx.stateMode = stateMode;
   packetTx.rssiToBase = 0;
   send(sock, &packetTx, sizeof(packetTx), 0 );
}

float CDemoPdr::computeAngleToFollow()
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

}


CDemoPdr::CDemoPdr() : m_pcDistance(NULL),
                       m_pcPropellers(NULL),
                       m_pcPos(NULL),
                       m_uiCurrentStep(0) {}


/****************************************/
/****************************************/
SensorSide CDemoPdr::CriticalProximity() {
   float sensor[4]  = {leftDist, backDist, rightDist, frontDist};
   float min   = CRITICAL_VALUE;
   SensorSide minSensor = SensorSide::kDefault;

   for (unsigned i = 0; i < 4; i++) {
      if (min > sensor[i] && sensor[i] > 0.0) 
      {
         min = sensor[i];
         minSensor = (SensorSide) i;
      }
   }

   if (((frontDist < 130.0 && frontDist > 0) && minSensor == SensorSide::kDefault)
        || (frontDist < min && frontDist > 0)){
      minSensor = SensorSide::kFront;
   } 

   return minSensor;
}



SensorSide CDemoPdr::FreeSide() {
   float sensor[4]  = {leftDist, backDist, rightDist, frontDist};
   SensorSide closeSens = CriticalProximity();
   if (closeSens == SensorSide::kDefault) return closeSens;
   SensorSide oppSens = (SensorSide) ((closeSens +2) % 4);
   if (sensor[oppSens] == -2 || sensor[oppSens] > 2 * CRITICAL_VALUE ) return oppSens;
   float max   = sensor[closeSens];
   SensorSide maxSensor = SensorSide::kDefault;
   for (unsigned i = 0; i < 4; i++) {
      if (sensor[i] == -2 ) return (SensorSide) i;
      if (max < sensor[i]) {
         max = sensor[i];
         maxSensor = (SensorSide) i;
      }
   }
   return maxSensor;
}



void CDemoPdr::Init(TConfigurationNode &t_node)
{
   m_pcRABSens     = GetSensor <CCI_RangeAndBearingSensor>("range_and_bearing" );
   m_pcRABAct      = GetActuator <CCI_RangeAndBearingActuator>("range_and_bearing");
   m_pcDistance = GetSensor<CCI_CrazyflieDistanceScannerSensor>("crazyflie_distance_scanner");
   m_pcPropellers = GetActuator<CCI_QuadRotorPositionActuator>("quadrotor_position");


   try
   {
      m_pcBattery = GetSensor<CCI_BatterySensor>("battery");
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
   
   objective = m_pcPos->GetReading().Position;
   posInitial = *(new CVector3(0,0,0));
   posFinal   = *(new CVector3(0,0,0));
   stateMode = kStandby;
}

/****************************************/
/****************************************/

void CDemoPdr::ControlStep()
{
   if (!isConnected)
   {
      connectToServer();
   }

   if (m_pcBattery->GetReading().AvailableCharge < 0.3 && stateMode == kTakeOff)
   {
      stateMode = kReturnToBase;
   }


   currentAngle = *(new CRadians(0.1f));
   CRadians *useless = new CRadians(0.1f);
   m_pcPos->GetReading().Orientation.ToEulerAngles(currentAngle, *useless, *useless);

   cPos = m_pcPos->GetReading().Position;


   checkIfPacketIsComing();
   sendPacketToOtherRobots();


   setPosVelocity();
   sendTelemetry();

   
   valRead = recv(sock , buffer, sizeof(buffer), 0);
   if (valRead != -1){
      stateMode = *reinterpret_cast<const StateMode*>(buffer);
   }

   CCI_CrazyflieDistanceScannerSensor::TReadingsMap sDistRead = m_pcDistance->GetReadingsMap();
   auto iterDistRead = sDistRead.begin();
   rightDist = (iterDistRead++)->second;
   frontDist = (iterDistRead++)->second;
   leftDist = (iterDistRead++)->second;
   backDist = (iterDistRead++)->second;

   if (stateMode == kStandby)
   {
      return;
   }

   count--;
   if (m_uiCurrentStep < 20) // decolage
   {
      cPos.SetZ(cPos.GetZ() + 0.25f);
      m_pcPropellers->SetAbsolutePosition(cPos);
   }

   else if ((stateMode == kTakeOff || stateMode == kReturnToBase) && count <= 0)
   {
      LOG << "angle : " << currentAngle << std::endl;

     switch (FreeSide()) {
        case SensorSide::kDefault:
        case SensorSide::kFront:
            LOG << "kDefault" << std::endl;
            // avance
            newCVector = new CVector3(
               (cos(currentAngle.GetValue()) * 0.4 + cPos.GetX()) * 1,
               (sin(currentAngle.GetValue()) * 0.4 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            if (stateMode == kReturnToBase) {
               m_pcPropellers->SetAbsoluteYaw(*(new CRadians(computeAngleToFollow())));
            }
         break;
            
        case SensorSide::kLeft:
            LOG << "kLeft" << std::endl;
            // go left
            newCVector = new CVector3(
               (cos(0.8 - currentAngle.GetValue()) * -0.3 + cPos.GetX()) * 1,
               (sin(0.8 - currentAngle.GetValue()) * 0.3 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            if (stateMode != kReturnToBase)
               m_pcPropellers->SetRelativeYaw(CRadians::PI_OVER_FOUR/3);
            break;
        case SensorSide::kRight:
            LOG << "kRight" << std::endl;
            //go right
            newCVector = new CVector3(
               (cos(0.8 - currentAngle.GetValue()) * 0.3 + cPos.GetX()) * 1,
               (sin(0.8 - currentAngle.GetValue()) * -0.3 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            if (stateMode != kReturnToBase)
               m_pcPropellers->SetRelativeYaw(-CRadians::PI_OVER_FOUR/3);
            break;
        case SensorSide::kBack:
            LOG << "kBack" << std::endl;
            if (stateMode != kReturnToBase)
               m_pcPropellers->SetRelativeYaw(CRadians::PI_OVER_FOUR);
             newCVector = new CVector3(
               (cos(currentAngle.GetValue() + 0.8) * 0.9 + cPos.GetX()) * 1,
               (sin(currentAngle.GetValue() + 0.8) * 0.9 + cPos.GetY()) * 1,
               cPos.GetZ());
            m_pcPropellers->SetAbsolutePosition(*newCVector);
            break;

      }
   }

   if (stateMode != kLanding && cPos.GetZ() < 0.2)
   {
      CVector3* test = new CVector3(cPos.GetX(), cPos.GetY(), 0.2);
      m_pcPropellers->SetRelativePosition(*test);
   }

   if (stateMode == kLanding && count <= 0)
   {
      if (cPos.GetZ() > 0.2)
      {
         CVector3* test = new CVector3(0,0,-0.1);
         m_pcPropellers->SetRelativePosition(*test);
      }
     
   }


   m_uiCurrentStep++;
}

void CDemoPdr::checkIfPacketIsComing()
{
   const CCI_RangeAndBearingSensor::TReadings& tMsgs = m_pcRABSens->GetReadings();
   if (count <= 0 && stateMode == kTakeOff)
   {
      if (!tMsgs.empty())
      {
         int idMax = -1;
         float targetAltitude = 0.0;
         for (int i = 0; i < tMsgs.size(); i++) {
            if (tMsgs[i].Range < 300.0)
            {
               PacketP2P packetReceived = *reinterpret_cast<const PacketP2P*>(tMsgs[i].Data.ToCArray());
               if (packetReceived.id > idMax
                     && packetReceived.id < getIntId()
                     && (packetReceived.currentAltitude - 0.1) < cPos.GetZ() && (packetReceived.currentAltitude + 0.1) > cPos.GetZ())
               {
                  idMax = packetReceived.id;
                  targetAltitude = std::max(0.5, packetReceived.currentAltitude - 0.25);
               }
            }
         }
         if (idMax != -1 && cPos.GetZ() > 0.5)
         {
            CVector3* newAltitude = new CVector3(cPos.GetX(), cPos.GetY(), targetAltitude);
            m_pcPropellers->SetAbsolutePosition(*newAltitude);
            count = 50;
         }
      }
      else if (count <= 0)
      {
         CVector3* newAltitude = new CVector3(cPos.GetX(), cPos.GetY(), 1.0);
         m_pcPropellers->SetAbsolutePosition(*newAltitude);
      }
   }
   
}

void CDemoPdr::sendPacketToOtherRobots()
{
   /* ---------------------------
      ---- P2P COMMUNICATION ----
      --------------------------- */
   struct PacketP2P packet;
   packet.id = getIntId();
   packet.currentAltitude = cPos.GetZ();
   CByteArray cBuf(10);
   memcpy(&cBuf[0], &packet, sizeof(packet));
   m_pcRABAct->SetData(cBuf);
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

