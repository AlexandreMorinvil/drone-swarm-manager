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
#define PI_VALUE 3.14
#define PI_DIVIDE_TWO 1.52
#define PI_DIVIDE_FOUR 0.8
#define SPEED_SRAIGHT 0.2
#define SPEED_SIDE 0.3
#define MAGIC 1.84

#include <argos3/plugins/robots/crazyflie/control_interface/ci_crazyflie_distance_scanner_sensor.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <regex>

enum SensorSide { kLeft, kBack, kRight, kFront, kDefault};

using argos::CCI_CrazyflieDistanceScannerSensor;
using std::string;
using std::cout;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CSensors {
 public:
      CSensors();

      virtual ~CSensors() {}

      SensorSide FreeSide(float sensorValues[4]);
      SensorSide ReturningSide(float sensorValues[4], float angle);
      SensorSide CriticalProximity(float sensorValues[4]);

 private:
      CCI_CrazyflieDistanceScannerSensor* m_pcDistance;
      float leftDist, backDist, frontDist, rightDist;
};

#endif
