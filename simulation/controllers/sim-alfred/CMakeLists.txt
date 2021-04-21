add_library(sim-alfred SHARED timer.h timer.cpp p2p.h p2p.cpp radio.h radio.cpp sensors.h sensors.cpp moving.h moving.cpp sim-alfred.h sim-alfred.cpp)
target_link_libraries(sim-alfred
  argos3core_simulator
  argos3plugin_simulator_crazyflie
  argos3plugin_simulator_genericrobot)
