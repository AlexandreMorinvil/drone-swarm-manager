/*
 * AUTHORS: Carlo Pinciroli <cpinciro@ulb.ac.be>
 *          Pierre-Yves Lajoie <lajoie.py@gmail.com>
 *
 * An example crazyflie drones sensing.
 *
 * This controller is meant to be used with the XML file:
 *    experiments/foraging.argos
 */

#ifndef CRAZYFLIE_SENSING_H
#define CRAZYFLIE_SENSING_H

/*
 * Include some necessary headers.
 */
/* Definition of the CCI_Controller class. */
#include <argos3/core/control_interface/ci_controller.h>
/* Definition of the crazyflie distance sensor */
#include <argos3/plugins/robots/crazyflie/control_interface/ci_crazyflie_distance_scanner_sensor.h>
/* Definition of the LEDs actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
/* Definition of the crazyflie position actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
/* Definition of the crazyflie position sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
/* Definition of the crazyflie range and bearing actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
/* Definition of the crazyflie range and bearing sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
/* Definition of the crazyflie battery sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.h>
/* Definitions for random number generation */
#include <argos3/core/utility/math/rng.h>

#include "controllers/sim-alfred/moving.h"
#include "controllers/sim-alfred/sensors.h"
#include "controllers/sim-alfred/p2p.h"
#include "controllers/sim-alfred/radio.h"
#include "controllers/sim-alfred/timer.h"

/*
 * All the ARGoS stuff in the 'argos' namespace.
 * With this statement, you save typing argos:: every time.
 */
using namespace argos;

/*
 * A controller is simply an implementation of the CCI_Controller class.
 */
class CSimAlfred : public CCI_Controller {
 public:
    /* Class constructor. */
    CSimAlfred();
    /* Class destructor. */
    virtual ~CSimAlfred() {}

    /*
    * This function initializes the controller.
    * The 't_node' variable points to the <parameters> section in the XML
    * file in the <controllers><footbot_foraging_controller> section.
    */
    virtual void Init(TConfigurationNode& t_node);

    /*
    * This function is called once every time step.
    * The length of the time step is set in the XML file.
    */
    virtual void ControlStep();

    /*
    * This function resets the controller to its state right after the
    * Init().
    * It is called when you press the reset button in the GUI.
    */
    virtual void Reset();

    /*
    * Called to cleanup what done by Init() when the experiment finishes.
    * In this example controller there is no need for clean anything up,
    * so the function could have been omitted. It's here just for
    * completeness.
    */
    virtual void Destroy() {}

    /*
    * This function lifts the drone from the ground
    */
    bool TakeOff();

    /*
    * This function returns the drone to the ground
    */
    bool Land();

    void setPosVelocity();

 private:
    CCI_CrazyflieDistanceScannerSensor* m_pcDistance;
    CCI_RangeAndBearingSensor* m_pcRABSens;
    CCI_RangeAndBearingActuator* m_pcRABAct;
    CCI_QuadRotorPositionActuator* m_pcPropellers;
    CCI_PositioningSensor* m_pcPos;
    CCI_BatterySensor* m_pcBattery;

    CMoving* cMoving;
    CSensors* cSensors;
    CP2P* cP2P;
    CRadio* cRadio;
    CTimer* cTimer;

    /* The random number generator */
    CRandom::CRNG* m_pcRNG;

    CVector3 posInitial;
    CVector3 posFinal;

    /* Current step */
    uint m_uiCurrentStep;

    StateMode stateMode;
    CVector3 objective;

    CVector3 previousPos;
    CVector3 cPos;
    PacketP2P packetOverP2PSaved;

    int idRobot;
};

#endif
