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
#include "FreeRTOS.h"
#include "timers.h"

#include "debug.h"

#define NON_BLOCKING 0
#define DEBUG_MODULE "HELLOWORLD"

struct packetRX {
  bool led_activation;
} __attribute__((packed));

struct packetTX {
  bool is_led_activated;
  float vbat;
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


P2PPacket initializeP2PPacket()
{
  static P2PPacket p_reply;
  p_reply.port=0x00;
  uint64_t address = configblockGetRadioAddress();
  uint8_t my_id =(uint8_t)((address) & 0x00000000ff);
  p_reply.data[0]=my_id;
  return p_reply;
}


void processRXPacketReceived(struct packetRX rxPacket)
{
  if ((bool)rxPacket.led_activation) {
    ledseqRun(&seq_lock);
    isLedActivated = true;
  }
  else {
    isLedActivated = false;
    ledseqStop(&seq_lock);
  }
}

void processTXPacketReceived(struct packetTX txPacket)
{
  if (crtpIsConnected())
  {
    appchannelSendPacket(&txPacket, sizeof(txPacket));
  }
}

void sendInfos()
{
  logVarId_t logIdPm = logGetVarId("pm", "vbat");
  struct packetTX txPacket;
  txPacket.is_led_activated = isLedActivated;
  txPacket.vbat = logGetFloat(logIdPm);

  if (crtpIsConnected())
  {
    appchannelSendPacket(&txPacket, sizeof(txPacket));
  }

  // Send info to other robots
  /*P2PPacket p_reply = initializeP2PPacket();
  memcpy(&p_reply.data[1], &txPacket, sizeof(txPacket));
  p_reply.size = sizeof(txPacket)+1;
  radiolinkSendP2PPacketBroadcast(&p_reply);*/
}


void p2pcallbackHandler(P2PPacket *p)
{
  /*if (p->size == sizeof(struct packetTX))
  {
    struct packetTX packet;
    memcpy(&packet, &p->data[1], sizeof(packet));
    processTXPacketReceived(packet);
  }
  if (p->size == sizeof(struct packetRX))
  {
    struct packetRX packet;
    memcpy(&packet, &p->data[1], sizeof(packet));
    processRXPacketReceived(packet);
  }*/
}

void appMain()
{
  struct packetRX rxPacket;
  ledseqRegisterSequence(&seq_lock);

  p2pRegisterCB(p2pcallbackHandler);
  timer = xTimerCreate("SendInfos", M2T(500), pdTRUE, NULL, sendInfos);
  xTimerStart(timer, 500);
  sendInfos();

  while(1) {    
    if (appchannelReceivePacket(&rxPacket, sizeof(rxPacket), NON_BLOCKING)) {

      processRXPacketReceived(rxPacket);
      //P2PPacket p_reply = initializeP2PPacket();
      //memcpy(&p_reply.data[1], &rxPacket, sizeof(rxPacket));
      //p_reply.size = sizeof(rxPacket)+1;
      //radiolinkSendP2PPacketBroadcast(&p_reply);
    }
  }
}
