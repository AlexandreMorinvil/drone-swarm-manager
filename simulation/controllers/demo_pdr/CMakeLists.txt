add_library(demo_pdr SHARED timer.h timer.cpp p2p.h p2p.cpp radio.h radio.cpp sensors.h sensors.cpp moving.h moving.cpp demo_pdr.h demo_pdr.cpp)
target_link_libraries(demo_pdr
  argos3core_simulator
  argos3plugin_simulator_crazyflie
  argos3plugin_simulator_genericrobot)
