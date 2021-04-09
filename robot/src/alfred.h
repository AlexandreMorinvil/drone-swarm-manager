#ifndef ALFRED_H
#define ALFRED_H

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
#include "moving.h"
#include "sensor.h"
#include "crtp_commander_high_level.h"

#include "debug.h"

#define NON_BLOCKING 0
#define DEBUG_MODULE "HELLOWORLD"

typedef enum {
    tx,
    position,
    attitude,
    velocity,
    distance,
    orientation
} PacketType;

typedef enum {
    kStandby,
    kTakeOff,
    kFlying,
    kReturnToBase,
    kLanding,
    kEmergency
  } StateMode;

typedef struct packetRX {
  StateMode stateMode;
} __attribute__((packed)) packetRX;

typedef struct PacketTX {
  int packetType;
  int stateMode;
  float vbat;
  bool isLedActivated;
} __attribute__((packed)) PacketTX;

typedef struct PacketPosition {
  int packetType;
  float x;
  float y;
  float z;
} __attribute__((packed)) PacketPosition;

typedef struct PacketVelocity {
  int packetType;
  float px;
  float py;
  float pz;
} __attribute__((packed)) PacketVelocity;

typedef struct PacketDistance {
  int packetType;
  uint16_t front;
  uint16_t back;
  uint16_t up;
  uint16_t left;
  uint16_t right;
  uint16_t zrange;
} __attribute__((packed)) PacketDistance;

typedef struct PacketOrientation {
    int packetType;
    float pitch;
    float roll;
    float yaw;
} __attribute__((packed)) PacketOrientation;

Vector3* objective;

#endif
