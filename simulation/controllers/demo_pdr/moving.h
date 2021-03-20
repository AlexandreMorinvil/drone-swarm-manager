/*
 * AUTHORS: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *          Pierre-Yves Lajoie <lajoie.py@gmail.com>
 *
 * An example crazyflie drones sensing.
 *
 * This controller is meant to be used with the XML file:
 *    experiments/foraging.argos
 */
#ifndef MOVING_H
#define MOVING_H

#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include "./sensors.h"




using argos::CCI_QuadRotorPositionActuator;
using argos::CCI_PositioningSensor;
using argos::CRadians;
using argos::CVector3;
using argos::Real;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CMoving {
 public:
      CMoving();

      virtual ~CMoving() {}

      CVector3* GoInSpecifiedDirection(
          SensorSide freeSide);

 private:
};

#endif
