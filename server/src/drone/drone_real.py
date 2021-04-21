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
from drone_interface import DroneInterface, PacketType
from vec3 import Vec3
from data_accumulator import MapObservationAccumulator
from setup_logging import LogsConfig
import struct


class DroneReal(DroneInterface) :
    def __init__(self, address, initialPos=Vec3()):
        super().__init__(address, initialPos)
        self.logsConfig = LogsConfig()
        self.logger = self.logsConfig.logger('DroneReal')
        self._cf = Crazyflie()
        self._cf.connected.add_callback(self._connected)
        self._cf.disconnected.add_callback(self._disconnected)
        self._cf.connection_failed.add_callback(self._connection_failed)
        self._cf.connection_lost.add_callback(self._connection_lost)

        self._cf.appchannel.packet_received.add_callback(self._process_data_received)

        self._cf.open_link("radio://0/72/250K/" + address)

        self.map_observation_accumulator = MapObservationAccumulator(False)
        self.logger.info('Create drone real in server')

        print('Connecting to %s' % "radio://0/72/250K/" + address)

        packet = struct.pack("<iff", PacketType.SET_INIT_POS.value, self._startPos.x, self._startPos.y)
        self._send_data(packet)

    def delete(self):
        self.close_connection()

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

    def _send_data(self, packet):
        self._cf.appchannel.send_packet(packet)

    def get_vBat(self):
        if(self._vbat <= 4.3 and self._vbat >= 4.2):
            return 1
        if(self._vbat <4.2 and self._vbat >= 4.15):
            return 0.95
        
        if(self._vbat < 4.15 and self._vbat >= 4.1):
            return 0.90
        
        if(self._vbat < 4.1 and self._vbat >= 4.05):
            return 0.85
        
        if(self._vbat < 4.05 and self._vbat >= 4.025):
            return 0.80
        
        if(self._vbat < 4.025 and self._vbat >= 4.0):
            return 0.75
        
        if(self._vbat < 4.0 and self._vbat >= 3.95):
            return 0.70
        
        if(self._vbat < 3.95 and self._vbat >= 3.90):
            return 0.65
        
        if(self._vbat < 3.9 and self._vbat >= 3.8875):
            return 0.60
        
        if(self._vbat < 3.875 and self._vbat >= 3.8625):
            return 0.55
        
        if(self._vbat < 3.8625 and self._vbat >= 3.85):
            return 0.50
        
        if(self._vbat < 3.85 and self._vbat >= 3.8375):
            return 0.45
        
        if(self._vbat < 3.875 and self._vbat >= 3.825):
            return 0.40
        
        if(self._vbat < 3.825 and self._vbat >= 3.8):
            return 0.35
        
        if(self._vbat < 3.8 and self._vbat >= 3.775):
            return 0.30
        
        if(self._vbat < 3.775 and self._vbat >= 3.75):
            return 0.25
        
        if(self._vbat < 3.75 and self._vbat >= 3.725):
            return 0.20
        
        if(self._vbat < 3.725 and self._vbat >= 3.7):
            return 0.10
        
        if(self._vbat < 3.7 and self._vbat >= 3.6):
            return 0.05
        
        if(self._vbat < 3.6 and self._vbat >= 3.5):
            return 0.03
        
        if(self._vbat < 3.5):
            return 0

    def close_connection(self):
        self._cf.close_link()
