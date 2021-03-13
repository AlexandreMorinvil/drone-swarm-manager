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

#include "sensors.h"
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/angles.h>


#define PI_OVER_FOUR 0.8
#define SPEED_SRAIGHT 0.4
#define SPEED_SIDE 0.3

/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CMoving {
   public:
      CMoving(CCI_QuadRotorPositionActuator* pcPropellers, CCI_PositioningSensor* pcPos);
      
      virtual ~CMoving() {}

      void GoInSpecifiedDirection(SensorSide sensorSide);

   private:

      void updateAngle();
      CCI_QuadRotorPositionActuator* m_pcPropellers;
      CCI_PositioningSensor* m_pcPos;
      CRadians* currentAngle;

};
#endif
