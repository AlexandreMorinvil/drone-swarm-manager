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

#include "app.h"
#include "app_channel.h"
#include "ledseq.h"
#include "log.h"
#include "crtp.h"
#include "radiolink.h"
#include "configblock.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "timers.h"
#include "estimator_kalman.h"
#include "param.h"

#include "debug.h"

#define NON_BLOCKING 0
#define DEBUG_MODULE "HELLOWORLD"


typedef enum {
    tx,
    position,
    velocity,
    distance
} PacketType;

struct packetRX {
  bool ledActivation;
} __attribute__((packed));

struct PacketTX {
  PacketType packetType;
  bool isLedActivated;
  float vbat;
  uint8_t rssiToBase;
} __attribute__((packed));

struct PacketPosition {
  PacketType packetType;
  float x;
  float y;
  float z;
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

bool isLedActivated = false;
ledseqStep_t seq_lock_def[] = {
  { true, LEDSEQ_WAITMS(1000)},
  {    0, LEDSEQ_LOOP},
};

ledseqContext_t seq_lock = {
  .sequence = seq_lock_def,
  .led = LED_BLUE_L,
};


static xTimerHandle timer;

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

void appMain()
{
  vTaskDelay(M2T(3000));
  struct packetRX rxPacket;
  
  ledseqRegisterSequence(&seq_lock);

  p2pRegisterCB(p2pcallbackHandler);
  timer = xTimerCreate("SendInfos", M2T(500), pdTRUE, NULL, sendInfos);
  xTimerStart(timer, 500);
  sendInfos();

  while(1) {
    if (appchannelReceivePacket(&rxPacket, sizeof(rxPacket), NON_BLOCKING)) {
      processRXPacketReceived(rxPacket);
    }
  }
}
