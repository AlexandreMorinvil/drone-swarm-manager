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

#define NON_BLOCKING 0
#define DEBUG_MODULE "HELLOWORLD"
#define PI_OVER_8 0.4



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
static xTimerHandle avoidTimer;
StateMode stateMode = kStandby;

P2PPacket initializeP2PPacket() {
  static P2PPacket p_reply;
  p_reply.port=0x00;
  uint64_t address = configblockGetRadioAddress();
  uint8_t my_id =(uint8_t)((address) & 0x00000000ff);
  p_reply.data[0]=my_id;
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

  packetTX.isLedActivated = isLedActivated;
  packetTX.vbat = logGetFloat(logGetVarId("pm", "vbat"));
  packetTX.rssiToBase = logGetInt(logGetVarId("radio", "rssi"));
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

  if (crtpIsConnected()) {
    appchannelSendPacket(&packetTX, sizeof(packetTX));
    appchannelSendPacket(&packetDistance, sizeof(packetDistance));
    appchannelSendPacket(&packetPosition, sizeof(packetPosition));
    appchannelSendPacket(&packetVelocity, sizeof(packetVelocity));
  }

  // Send info to other robots
  P2PPacket p_reply = initializeP2PPacket();
  memcpy(&p_reply.data[1], &packetTX, sizeof(packetTX));
  p_reply.size = sizeof(packetTX)+1;
  radiolinkSendP2PPacketBroadcast(&p_reply);
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

float computeAngleToFollow() {
  float posX = logGetFloat(logGetVarId("stateEstimate", "x");
  float posY = logGetFloat(logGetVarId("stateEstimate", "y");
  float xdiff = objective->x - posX);
  float ydiff = objective->y - posY);

  if (abs(xdiff) < 0.5 && abs(ydiff) < 0.5) {
    stateMode = kLanding;
  }
  float length = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
  if (ydiff < 0) {
      return (- atan(xdiff/ydiff) + PI_VALUE);
  }
  return (- atan(xdiff/ydiff));
}

void appMain()
{
  vTaskDelay(M2T(3000));
  struct packetRX rxPacket;
  
  ledseqRegisterSequence(&seq_lock);

  p2pRegisterCB(p2pcallbackHandler);
  timer = xTimerCreate("SendInfos", M2T(500), pdTRUE, NULL, sendInfos);
  xTimerStart(timer, 500);
  sendInfos();

  setObjective(0.0f, 0.0f, 0.0f);

  float yaw = 0.0;
  uint16_t leftDistance = logGetUint(logGetVarId("range", "left"));
  uint16_t backDistance = logGetUint(logGetVarId("range", "back"));
  uint16_t rightDistance = logGetUint(logGetVarId("range", "right"));
  uint16_t frontDistance = logGetUint(logGetVarId("range", "front"));
  float sensorValues[4] =
    {leftDistance, backDistance, rightDistance, frontDistance};


  while (1) {
    if (appchannelReceivePacket(&rxPacket, sizeof(rxPacket), NON_BLOCKING)) {
      processRXPacketReceived(rxPacket);
    }
    switch (stateMode) {
      case kStandby:
        break;
      case kTakeOff:
        crtpCommanderHighLevelTakeoff(5.0, 1.0);
        stateMode = kFlying;
        break;
      case kFlying:
        if (frontDistance < 130) {
          yaw = PI_OVER_8;
          crtpCommanderHighLevelLandYaw(0.5f, 1.0, yaw); // valeurs arbitraires
        }
        Vector3* vec3 = GoInSpecifiedDirection(FreeSide(sensorValues));
        crtpCommanderHighLevelGoTo(vec3->x, vec3->y, vec3->z, yaw, 1.0, true);
        break;
      case kReturnToBase:
        Vector3* vec3 = GoInSpecifiedDirection(ReturningSide(sensorValues, computeAngleToFollow());
        crtpCommanderHighLevelGoTo(vec3->x, vec3->y, vec3->z, yaw, 1.0, true);
        if (CriticalProximity(sensorValues) == kDefault) {
          crtpCommanderHighLevelLandYaw(abs(vec3->x - vec3->y), 1.0, computeAngleToFollow())
        } 
        break;
      case kLanding:
        if (logGetFloat(logGetVarId("stateEstimate", "z")) > 0.2) {
          crtpCommanderHighLevelGoTo(0.0f, 0.0f, -0.1f)); // temporaire
        }
        break;
      default:
        break;
    }
  }
}