/*
 * AUTHORS: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *          Pierre-Yves Lajoie <lajoie.py@gmail.com>
 *
 * An example crazyflie drones sensing.
 *
 * This controller is meant to be used with the XML file:
 *    experiments/foraging.argos
 */

#ifndef RADIO_H
#define RADIO_H
#define DEFAULT_PORT 5015

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <regex.h>
#include <arpa/inet.h>
#include <argos3/core/utility/math/vector3.h>
#include "controllers/demo_pdr/sensors.h"


using std::regex;
using std::smatch;
using argos::CVector3;

typedef enum {
    kStandby,
    kTakeOff,
    kReturnToBase,
    kLanding
  } StateMode;

typedef enum {
    tx = 0,
    position = 1,
    attitude = 2,
    velocity = 3,
    distance = 4,
    orientation = 5
} PacketType;

struct PacketPosition {
    PacketType packetType;
    float x;
    float y;
    float z;
} __attribute__((packed));

struct PacketTX {
    PacketType packetType;
    StateMode stateMode;
    float vbat;
    bool isLedActivated;
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

struct PacketOrientation {
    PacketType packetType;
    float pitch;
    float roll;
    float yaw;
} __attribute__((packed));


class CRadio {
 public:
      CRadio();

      virtual ~CRadio() {}

      bool connectToServer(int idRobot);

      void sendTelemetry(CVector3 pos, StateMode stateMode, float vBat, float rangeValues[], float orientationValues[], float speed[]);

      StateMode* ReceiveData();

 private:
      int sock, valRead, n;
      bool isConnected;
      struct sockaddr_in serv_addr;
};


#endif
