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
static xTimerHandle timer;
static xTimerHandle timerSwitchState;
static xTimerHandle timerAltitude;
StateMode stateMode = kStandby;
float newAltitudeZ = 0.0f;

uint8_t getId() {
  uint64_t address = configblockGetRadioAddress();
  uint8_t my_id =(uint8_t)((address) & 0x00000000ff);
  return my_id;
}
P2PPacket initializeP2PPacket() {
  static P2PPacket p_reply;
  p_reply.port=0x00;
  //uint64_t address = configblockGetRadioAddress();
  //uint8_t my_id =(uint8_t)((address) & 0x00000000ff);
  p_reply.data[0]=getId();
  return p_reply;
}


void processRXPacketReceived(struct packetRX rxPacket){
  if ((bool)rxPacket.ledActivation) {
    ledseqRun(&seq_lock);
    isLedActivated = true;
  }
  else {
    isLedActivated = false;
    ledseqStop(&seq_lock);
  }
  stateMode = rxPacket.stateMode;
}

void setObjective(float x, float y, float z) {
  objective->x = x;
  objective->y = y;
  objective->z = z;
}

void sendInfos() {
  struct PacketTX packetTX;
  struct PacketDistance packetDistance;
  struct PacketPosition packetPosition;
  struct PacketVelocity packetVelocity;
  struct PacketOrientation packetOrientation;

  packetTX.isLedActivated = isLedActivated;
  packetTX.vbat = logGetFloat(logGetVarId("pm", "vbat"));
  packetTX.stateMode = stateMode;
  packetTX.packetType = tx;

  packetPosition.x = logGetFloat(logGetVarId("stateEstimate", "x"));
  packetPosition.y = logGetFloat(logGetVarId("stateEstimate", "y"));
  packetPosition.z = logGetFloat(logGetVarId("stateEstimate", "z"));
  packetPosition.packetType = position;

  packetVelocity.px = logGetFloat(logGetVarId("kalman", "varPX"));
  packetVelocity.py = logGetFloat(logGetVarId("kalman", "varPY"));
  packetVelocity.pz = logGetFloat(logGetVarId("kalman", "varPZ"));
  packetVelocity.packetType = velocity;

  packetDistance.front = logGetUint(logGetVarId("range", "front"));
  packetDistance.back = logGetUint(logGetVarId("range", "back"));
  packetDistance.up = logGetUint(logGetVarId("range", "up"));
  packetDistance.left = logGetUint(logGetVarId("range", "left"));
  packetDistance.right = logGetUint(logGetVarId("range", "right"));
  packetDistance.zrange = logGetUint(logGetVarId("range", "zrange"));
  packetDistance.packetType = distance;

  packetOrientation.yaw = logGetFloat(logGetVarId("stabilizer", "yaw"));
  packetOrientation.pitch = 0.0f;
  packetOrientation.roll = 0.0f;
  packetOrientation.packetType = orientation;

  if (crtpIsConnected()) {
    appchannelSendPacket(&packetTX, sizeof(packetTX));
    appchannelSendPacket(&packetDistance, sizeof(packetDistance));
    appchannelSendPacket(&packetPosition, sizeof(packetPosition));
    appchannelSendPacket(&packetVelocity, sizeof(packetVelocity));
    appchannelSendPacket(&packetOrientation, sizeof(packetOrientation));
  }

  // Send info to other robots
  P2PPacket p_reply = initializeP2PPacket();
  memcpy(&p_reply.data[1], &packetTX, sizeof(packetTX));
  p_reply.size = sizeof(packetTX)+1;
  radiolinkSendP2PPacketBroadcast(&p_reply);
}

void sendAltitude() {
  P2PPacket p_altitude = initializeP2PPacket();
  float altitudeZ = logGetFloat(logGetVarId("stateEstimate", "z"));
  memcpy(&p_altitude.data[1], &altitudeZ, sizeof(altitudeZ));
  p_altitude.size = sizeof(altitudeZ) + 1;
  radiolinkSendP2PPacketBroadcast(&p_altitude);
}


void p2pcallbackHandler(P2PPacket *p) {
  if (p->rssi < 48) {
    ledseqRun(&seq_lock);
    isLedActivated = true;
  }
  else {
    isLedActivated = false;
    ledseqStop(&seq_lock);
  }
}

void p2pcallbackHandlerAltitude(P2PPacket *p) {
  uint8_t my_id = getId();
  uint8_t received_id = p->data[0];
  newAltitudeZ = logGetFloat(logGetVarId("stateEstimate", "z"));

  if (p->rssi < DISTANCE_AVOID_COLLISION && received_id < my_id) {
    float x = logGetFloat(logGetVarId("stateEstimate", "x"));
    float y = logGetFloat(logGetVarId("stateEstimate", "y"));
    newAltitudeZ = logGetFloat(logGetVarId("stateEstimate", "z")) - 0.25f;
    float yaw = logGetFloat(logGetVarId("stabilizer", "yaw"));
    crtpCommanderHighLevelGoTo(x, y, newAltitudeZ, yaw, 1.0f, true);  
  }
}

float computeAngleToFollow() {
  float posX = logGetFloat(logGetVarId("stateEstimate", "x")) * 100.0f;
  float posY = logGetFloat(logGetVarId("stateEstimate", "y")) * 100.0f;
  float xdiff = objective->x - posX;
  float ydiff = objective->y - posY;

  //if (abs(xdiff) < 0.5 && abs(ydiff) < 0.5) {
  //  stateMode = kLanding;
  //}

  float yaw = 0.0;

  if (ydiff < 0) {
    yaw = atan(ydiff/xdiff) + PI_VALUE/2;
  } else {
    yaw = atan(xdiff/ydiff);
  }
  if (xdiff < 0) {
    yaw = - yaw;
  }
  return yaw;
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


void switchState() {
  uint16_t leftDistance = logGetUint(logGetVarId("range", "left"));
  uint16_t backDistance = logGetUint(logGetVarId("range", "back"));
  uint16_t rightDistance = logGetUint(logGetVarId("range", "right"));
  uint16_t frontDistance = logGetUint(logGetVarId("range", "front"));
  // float yawRead = logGetFloat(logGetVarId("stabilizer", "yaw"));
  float sensorValues[4] =
    {leftDistance, backDistance, rightDistance, frontDistance};
  float angleToFollow = computeAngleToFollow() * (180.0f/3.14f);

  switch (stateMode) {
      case kStandby:
        break;
      case kTakeOff:
        crtpCommanderHighLevelTakeoff(0.3, 1.5);
        if (logGetFloat(logGetVarId("stateEstimate", "z")) > 0.3f) {
          stateMode = kFlying;
        }
        break;
      case kFlying:
        if (sensorValues[3] < 700) {
          yaw = 100.0f;
        } else {
          yaw = 0.0f;
        }
        Vector3 vec3 = GoInSpecifiedDirection(FreeSide(sensorValues));
        setHoverSetpoint(&setpoint, vec3.x, vec3.y, 0.3, yaw, modeVelocity);
        commanderSetSetpoint(&setpoint, 1);
        yaw = 0.0f;
        break;
      case kReturnToBase:
        yaw = angleToFollow;
        vec3 = GoInSpecifiedDirection(
          ReturningSide(sensorValues, computeAngleToFollow()));
        setHoverSetpoint(&setpoint, vec3.x, vec3.y, 0.3, 0.0, modeAbs);
        commanderSetSetpoint(&setpoint, 1);
        break;
      case kEmergency:
        memset(&setpoint, 0, sizeof(setpoint_t));
        commanderSetSetpoint(&setpoint, 1);
        break;
      case kLanding:
        if (logGetFloat(logGetVarId("stateEstimate", "z")) > 0.3f) {
          crtpCommanderHighLevelLand(0.0f, 2.0f); 
        }
        break;
      default:
        break;
    }
}

void appMain()
{
  
  vTaskDelay(M2T(3000));
  struct packetRX rxPacket;
  
  ledseqRegisterSequence(&seq_lock);

  p2pRegisterCB(p2pcallbackHandler);
  timer = xTimerCreate("SendInfos", M2T(100), pdTRUE, NULL, sendInfos);
  xTimerStart(timer, 100);
  sendInfos();
  timerSwitchState = xTimerCreate("switchState", M2T(100), pdTRUE, NULL, switchState);
  xTimerStart(timerSwitchState, 100);
  switchState();
  paramSetInt(paramGetVarId("commander", "enHighLevel"), 1);

  p2pRegisterCB(p2pcallbackHandlerAltitude);
  timerAltitude = xTimerCreate("sendAltitude", M2T(100), pdTRUE, NULL, sendAltitude);
  xTimerStart(timerAltitude, 100);
  sendAltitude();

  // Vector3* posTemp;
  setObjective(0.0f, 0.0f, 0.0f); // Temporaire
 

  while (1) {

    if (appchannelReceivePacket(&rxPacket, sizeof(rxPacket), NON_BLOCKING)) {
      processRXPacketReceived(rxPacket);
    }
    
  }
}