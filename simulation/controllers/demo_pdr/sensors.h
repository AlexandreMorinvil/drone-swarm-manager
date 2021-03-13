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

enum SensorSide { kLeft, kBack, kRight, kFront, kDefault};

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CSensors {
   public:
      CSensors();
      
      virtual ~CSensors() {}

   private:

};


#endif
