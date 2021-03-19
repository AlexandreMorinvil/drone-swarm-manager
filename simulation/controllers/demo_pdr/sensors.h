/*
 * AUTHORS: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *          Pierre-Yves Lajoie <lajoie.py@gmail.com>
 *
 * An example crazyflie drones sensing.
 *
 * This controller is meant to be used with the XML file:
 *    experiments/foraging.argos
 */

#ifndef SENSORS_H
#define SENSORS_H
#define CRITICAL_VALUE 70.0f

#include <argos3/plugins/robots/crazyflie/control_interface/ci_crazyflie_distance_scanner_sensor.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <string>
#include <fcntl.h>
#include <regex>


enum SensorSide { kLeft, kBack, kRight, kFront, kDefault};

using argos::CCI_CrazyflieDistanceScannerSensor;
using std::string;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CSensors {
 public:
      CSensors();

      virtual ~CSensors() {}

      SensorSide FreeSide(float sensorValues[4]);

      SensorSide CriticalProximity(float sensorValues[4]);

 private:
      CCI_CrazyflieDistanceScannerSensor* m_pcDistance;
      float leftDist, backDist, frontDist, rightDist;
};

#endif
