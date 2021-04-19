
import json
import logging
import time
from sensor import Sensor
from vec3 import Vec3
from threading import Thread
from enum import Enum
from sensor import Sensor
from vec3 import Vec3
import cflib
from cflib.crazyflie import Crazyflie
from drone_interface import DroneInterface
from vec3 import Vec3
from data_accumulator import MapObservationAccumulator
from setup_logging import LogsConfig
import struct


class DroneReal(DroneInterface) :
    def __init__(self, port, initialPos=Vec3()):
        super().__init__(port, initialPos)
        self.logsConfig = LogsConfig()
        self.logger = self.logsConfig.logger('DroneReal')
        self._cf = Crazyflie()
        self._cf.connected.add_callback(self._connected)
        self._cf.disconnected.add_callback(self._disconnected)
        self._cf.connection_failed.add_callback(self._connection_failed)
        self._cf.connection_lost.add_callback(self._connection_lost)

        self._cf.appchannel.packet_received.add_callback(self._process_data_received)

        self._cf.open_link(port)

        self.map_observation_accumulator = MapObservationAccumulator(False)
        self.logger.info('Create drone real in server')

        print('Connecting to %s' % "" + port)

    def _connected(self, link_uri):
        """ This callback is called form the Crazyflie API when a Crazyflie
        has been connected and the TOCs have been downloaded."""
        self._isConnected = True

    def _connection_failed(self, link_uri, msg):
        """Callback when connection initial connection fails (i.e no Crazyflie
        at the specified address)"""
        print('Connection to %s failed: %s' % (link_uri, msg))

    def _connection_lost(self, link_uri, msg):
        """Callback when disconnected after a connection has been made (i.e
        Crazyflie moves out of range)"""
        self._isConnected = False
        print('Connection to %s lost: %s' % (link_uri, msg))

    def _disconnected(self, link_uri):
        """Callback when the Crazyflie is disconnected (called in all cases)"""
        self._isConnected = False
        print('Disconnected from %s' % link_uri)

    def send_data(self, packet, format_packer):
        data = struct.pack(format_packer, packet)
        self._cf.appchannel.send_packet(data)
