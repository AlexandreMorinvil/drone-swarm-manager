/*
 * AUTHORS: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *          Pierre-Yves Lajoie <lajoie.py@gmail.com>
 *
 * An example crazyflie drones sensing.
 *
 * This controller is meant to be used with the XML file:
 *    experiments/foraging.argos
 */

#ifndef P2P_H
#define P2P_H

#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <algorithm>

#include "controllers/sim-alfred/sensors.h"
#include "controllers/sim-alfred/moving.h"
#include "controllers/sim-alfred/timer.h"

using argos::CCI_RangeAndBearingSensor;
using argos::CCI_RangeAndBearingActuator;
using std::max;
using argos::CByteArray;


class CP2P {
 public:
      CP2P(
         CCI_RangeAndBearingSensor* pcRABSens,
         CCI_RangeAndBearingActuator* pcRABAct,
         CTimer* cTimer,
         CMoving* cMoving);

      virtual ~CP2P() {}

      PacketP2P GetClosestPacket(CVector3 cPos);

      void sendPacketToOtherRobots(int idRobot, CVector3 cPos, float speed[3]);

 private:
      CCI_RangeAndBearingSensor* m_pcRABSens;
      CCI_RangeAndBearingActuator* m_pcRABAct;
      CTimer* cTimer;
      CMoving* cMoving;
};


#endif
