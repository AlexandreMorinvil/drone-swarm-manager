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

#include "controllers/demo_pdr/sensors.h"
#include "controllers/demo_pdr/moving.h"
#include "controllers/demo_pdr/timer.h"

using argos::CCI_RangeAndBearingSensor;
using argos::CCI_RangeAndBearingActuator;
using std::max;
using argos::CByteArray;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CP2P {
 public:
      CP2P(
         CCI_RangeAndBearingSensor* pcRABSens,
         CCI_RangeAndBearingActuator* pcRABAct,
         CTimer* cTimer);

      virtual ~CP2P() {}

      CVector3* GetNewVectorToAvoidCollision(CVector3 position, int idRobot);

      void sendPacketToOtherRobots(float altitude, int idRobot);

 private:
      CCI_RangeAndBearingSensor* m_pcRABSens;
      CCI_RangeAndBearingActuator* m_pcRABAct;
      CTimer* _cTimer;
      struct PacketP2P {
         uint8_t id;
         float currentAltitude;
      };
};


#endif
