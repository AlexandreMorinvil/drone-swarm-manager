/*
 * AUTHORS: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *          Pierre-Yves Lajoie <lajoie.py@gmail.com>
 *
 * An example crazyflie drones sensing.
 *
 * This controller is meant to be used with the XML file:
 *    experiments/foraging.argos
 */

#ifndef TIMER_H
#define TIMER_H

enum TimerType {kLandingTimer, kAvoidTimer};

class CTimer {
 public:
      CTimer();

      void SetTimer(TimerType timerType, int value);

      int GetTimer(TimerType timerType);

      void CountOneCycle();

 private:
      int landingTimer;
      int avoidTimer;
};


#endif
