/**
 * ,---------,       ____  _ __
 * |  ,-^-,  |      / __ )(_) /_______________ _____  ___
 * | (  O  ) |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * | / ,--´  |    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *    +------`   /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2019 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include "alfred.h"

#include "commander.h"

#define NON_BLOCKING 0
#define DEBUG_MODULE "HELLOWORLD"
#define PI_OVER_8 0.4
#define PI_OVER_4 0.8
#define DISTANCE_AVOID_COLLISION 200 


ledseqStep_t seq_lock_def[] = {
  { true, LEDSEQ_WAITMS(1000)},
  {    0, LEDSEQ_LOOP},
};

ledseqContext_t seq_lock = {
  .sequence = seq_lock_def,
  .led = LED_BLUE_L,
};

bool isLedActivated = false;
static xTimerHandle timerSendBase;
static xTimerHandle timerSendRobots;
static xTimerHandle timerSwitchState;
static xTimerHandle timerTimeoutPacket;
StateMode stateMode = kStandby;
float newAltitudeZ = 0.0f;
float initPosX = 0.0;
float initPosY = 0.0;

uint8_t getId() {
  uint64_t address = configblockGetRadioAddress();
  return (uint8_t)((address) & 0x00000000ff);
}

float elevation = 0.2;

P2PPacket initializeP2PPacket() {
  static P2PPacket p_reply;
  p_reply.port=0x00;
  return p_reply;
}


void processRXPacketReceived(struct packetRX rxPacket){
  if (rxPacket.packetType == (int)kSwitchState) {
    stateMode = (int)rxPacket.firstPayload;
  } else if (rxPacket.packetType == (int)kSetInitPos) {
    initPosX = rxPacket.firstPayload;
    initPosY = rxPacket.secondPayload;
  }
}

void setObjective(float x, float y, float z) {
  objective->x = x;
  objective->y = y;
  objective->z = z;
}


void sendInfosToBase() {
  struct PacketTX packetTX;
  struct PacketDistance packetDistance;
  struct PacketPosition packetPosition;
  struct PacketVelocity packetVelocity;
  struct PacketOrientation packetOrientation;

  packetTX.isLedActivated = isLedActivated;
  packetTX.vbat = logGetFloat(logGetVarId("pm", "vbat"));
  packetTX.stateMode = (int) stateMode;
  // packetTX.rssiToBase = logGetInt(logGetVarId("radio", "rssi"));
  packetTX.packetType = (int) kTx;

  packetPosition.x = logGetFloat(logGetVarId("stateEstimate", "x")) + initPosX;
  packetPosition.y = logGetFloat(logGetVarId("stateEstimate", "y")) + initPosY;
  packetPosition.z = logGetFloat(logGetVarId("stateEstimate", "z"));
  packetPosition.packetType = (int) kPosition;

  packetVelocity.px = logGetFloat(logGetVarId("stateEstimate", "vx"));
  packetVelocity.py = logGetFloat(logGetVarId("stateEstimate", "vy"));
  packetVelocity.pz = logGetFloat(logGetVarId("stateEstimate", "vz"));
  packetVelocity.packetType = (int) kVelocity;

  packetDistance.front = logGetUint(logGetVarId("range", "front"));
  packetDistance.back = logGetUint(logGetVarId("range", "back"));
  packetDistance.up = logGetUint(logGetVarId("range", "up"));
  packetDistance.left = logGetUint(logGetVarId("range", "left"));
  packetDistance.right = logGetUint(logGetVarId("range", "right"));
  packetDistance.zrange = logGetUint(logGetVarId("range", "zrange"));
  packetDistance.packetType = (int) kDistance;

  packetOrientation.packetType = (int) kOrientation;
  packetOrientation.roll  = 0.0f;
  packetOrientation.pitch = 0.0f;
  packetOrientation.yaw   = logGetFloat(logGetVarId("stabilizer", "yaw"))
    * (3.142f/180.0f);

  if (crtpIsConnected()) {
    appchannelSendPacket(&packetTX, sizeof(packetTX));
    appchannelSendPacket(&packetDistance, sizeof(packetDistance));
    appchannelSendPacket(&packetPosition, sizeof(packetPosition));
    appchannelSendPacket(&packetVelocity, sizeof(packetVelocity));
    appchannelSendPacket(&packetOrientation, sizeof(packetOrientation));
  }
}

void sendInfoToOtherRobots() {
  P2PPacket p_reply = initializeP2PPacket();
  PacketOverP2P packetOverP2P;
  packetOverP2P.id = getId();
  packetOverP2P.vx = logGetFloat(logGetVarId("stateEstimate", "vx"))*100;
  packetOverP2P.vy = logGetFloat(logGetVarId("stateEstimate", "vy"))*100;
  packetOverP2P.x = (logGetFloat(logGetVarId("stateEstimate", "x"))+initPosX)*100;
  packetOverP2P.y = (logGetFloat(logGetVarId("stateEstimate", "y"))+initPosY)*100;
  memcpy(&p_reply.data, &packetOverP2P, sizeof(packetOverP2P));
  p_reply.size = sizeof(packetOverP2P);
  radiolinkSendP2PPacketBroadcast(&p_reply);
}

static void setHoverSetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate, enum mode_e modeYaw)
{
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;

  setpoint->mode.yaw = modeYaw;
  if (modeYaw == modeAbs) {
    setpoint->attitude.yaw = yawrate;
  } else if (modeYaw == modeVelocity) {
    setpoint->attitudeRate.yaw = yawrate;
  }

  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = vx;
  setpoint->velocity.y = vy;

  setpoint->velocity_body = true;
}

static setpoint_t setpoint;
float yaw = 0.0;
PacketOverP2P packetOverP2PSaved;

void p2pcallbackHandler(P2PPacket *p) {
  xTimerReset(timerTimeoutPacket, M2T(2000));
  PacketOverP2P packetOverP2P;
  memcpy(&packetOverP2P, p->data, sizeof(packetOverP2P));
  float x = logGetFloat(logGetVarId("stateEstimate", "x")) + initPosX;
  float y = logGetFloat(logGetVarId("stateEstimate", "y")) + initPosY;
  float z = logGetFloat(logGetVarId("stateEstimate", "z"));
  Vector3 cPos;
  cPos.x = x*100;
  cPos.y = y*100;
  cPos.z = z*100;
  float distance = ComputeDistanceBetweenPoints(packetOverP2P, cPos);
  float speedValues[3];
  speedValues[0] = logGetFloat(logGetVarId("stateEstimate", "vx"));
  speedValues[1] = logGetFloat(logGetVarId("stateEstimate", "vy"));
  speedValues[2] = logGetFloat(logGetVarId("stateEstimate", "vz"));
  yaw =
      GetAngleToAvoidCollision(packetOverP2P, cPos, speedValues)
      * (180.0f/3.14f);
  if (yaw == NO_COLLISION && stateMode == kCollisionResolver) {
    ledseqStop(&seq_lock);
    stateMode = kFlying;
  } else if (distance <= 110 && stateMode == kFlying && yaw != NO_COLLISION) {
    ledseqRun(&seq_lock);
    stateMode = kCollisionResolver;
    packetOverP2PSaved = packetOverP2P;
  } else if (distance > 130 && stateMode == kCollisionResolver) {
    ledseqStop(&seq_lock);
    stateMode = kFlying;
  }
}

void timeoutPacket() {
  if (stateMode == kCollisionResolver) {
    stateMode = kFlying;
  }
}

void switchState() {
  uint16_t leftDistance = logGetUint(logGetVarId("range", "left"));
  uint16_t backDistance = logGetUint(logGetVarId("range", "back"));
  uint16_t rightDistance = logGetUint(logGetVarId("range", "right"));
  uint16_t frontDistance = logGetUint(logGetVarId("range", "front"));
  Vector3 cPos;
  cPos.x = logGetFloat(logGetVarId("stateEstimate", "x"));
  cPos.y = logGetFloat(logGetVarId("stateEstimate", "y"));
  cPos.z = logGetFloat(logGetVarId("stateEstimate", "z"));
  float sensorValues[4] =
    {leftDistance, backDistance, rightDistance, frontDistance};
  float angleToFollow = ComputeAngleToFollow(*objective, cPos) * (180.0f/3.14f);
  float speedValues[3];
  speedValues[0] = logGetFloat(logGetVarId("kalman", "varPX"));
  speedValues[1] = logGetFloat(logGetVarId("kalman", "varPY"));
  speedValues[2] = logGetFloat(logGetVarId("kalman", "varPZ"));

  switch (stateMode) {
      case kStandby:
        break;
      case kTakeOff:
        crtpCommanderHighLevelTakeoff(elevation, 1.5);
        if (logGetFloat(logGetVarId("stateEstimate", "z")) > elevation) {
          stateMode = kFlying;
          xTimerStart(timerSendRobots, 100);

          // Apply a correction on value read
          //initPosX-=cPos.x;
          //initPosY-=cPos.y;
        }
        break;
      case kFlying:
        if (sensorValues[3] < 700) {
          yaw = 170.0f;
        } else {
          yaw = 0.0f;
        }
        Vector3 vec3 = GoInSpecifiedDirection(FreeSide(sensorValues));
        setHoverSetpoint(&setpoint, vec3.x, vec3.y, elevation, yaw, modeVelocity);
        commanderSetSetpoint(&setpoint, 1);
        yaw = 0.0f;
        break;
      case kCollisionResolver:
        /*cPos.x = cPos.x*100;
        cPos.y = cPos.y*100;
        cPos.z = cPos.z*100;*/
        yaw =
          GetAngleToAvoidCollision(packetOverP2PSaved, cPos, speedValues)
          * (180.0f/3.14f);
        vec3 = GoInSpecifiedDirection(
          ReturningSide(sensorValues, yaw));
        setHoverSetpoint(&setpoint, vec3.x, vec3.y, elevation, yaw, modeAbs);
        commanderSetSetpoint(&setpoint, 1);
        break;
      case kReturnToBase:
        yaw = angleToFollow;
        vec3 = GoInSpecifiedDirection(
        ReturningSide(sensorValues, angleToFollow));
        setHoverSetpoint(&setpoint, vec3.x, vec3.y, elevation, yaw, modeAbs);
        commanderSetSetpoint(&setpoint, 1);
        int rssi = logGetInt(logGetVarId("radio", "rssi"));
        if (rssi < 45
          && fabs(cPos.x) - 0.2 < 0
          && fabs(cPos.y) - 0.2 < 0) {
          stateMode = kEmergency;
        }
        break;
      case kEmergency:
        memset(&setpoint, 0, sizeof(setpoint_t));
        commanderSetSetpoint(&setpoint, 1);
        xTimerStop(timerSendRobots, 0);
        break;
      case kLanding:
        crtpCommanderHighLevelLand(0.0f, 1.0f);
        xTimerStop(timerSendRobots, 0);
        stateMode = kStandby;
        break;
      default:
        break;
    }
}

void appMain() {
  struct packetRX rxPacket;

  if (getId() == 231) {
    elevation = 0.3;
  }

  ledseqRegisterSequence(&seq_lock);
  p2pRegisterCB(p2pcallbackHandler);

  timerSendBase = xTimerCreate("sendInfosToBase", M2T(100), pdTRUE, NULL, sendInfosToBase);
  xTimerStart(timerSendBase, 100);
  sendInfosToBase();

  timerSendRobots = xTimerCreate("sendInfoToOtherRobots", M2T(200), pdTRUE, NULL, sendInfoToOtherRobots);

  timerSwitchState = xTimerCreate("switchState", M2T(100), pdTRUE, NULL, switchState);
  xTimerStart(timerSwitchState, 100);
  switchState();

  timerTimeoutPacket = xTimerCreate("timeoutPacket", M2T(2000), pdFALSE, NULL, timeoutPacket);

  paramSetInt(paramGetVarId("commander", "enHighLevel"), 1);

  // Vector3* posTemp;
  setObjective(0.0f, 0.0f, 0.0f); // Temporaire
 

  while (1) {

    if (appchannelReceivePacket(&rxPacket, sizeof(rxPacket), NON_BLOCKING)) {
      processRXPacketReceived(rxPacket);
    }
    
  }
}
