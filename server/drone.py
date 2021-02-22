import json
import logging
import time
from threading import Thread
from enum import Enum
import struct

import cflib
from cflib.crazyflie import Crazyflie

logging.basicConfig(level=logging.ERROR)
State = Enum('State', 'standBy in_mission crash')
class Drone :
    led = False
    state = State.standBy

    def __init__(self, link_uri, id):

        self._cf = Crazyflie()

        self._cf.connected.add_callback(self._connected)
        self._cf.disconnected.add_callback(self._disconnected)
        self._cf.connection_failed.add_callback(self._connection_failed)
        self._cf.connection_lost.add_callback(self._connection_lost)

        self._cf.appchannel.packet_received.add_callback(self._app_packet_received)

        self._cf.open_link(link_uri)
        self._isConnected = False
        self._vbat = 0.0
        self._id = id
        self._speed = 0.0
        

        print('Connecting to %s' % link_uri)

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

    def _app_packet_received(self, data):
        (is_led_activated, vbattery) = struct.unpack("<bf", data)
        self._vbat = vbattery
        print(f"ID : {self._cf.link_uri} | Led status : {is_led_activated}  ---  Battery Voltage : {vbattery}")

    def toggleLED(self):
        self.led = not self.led
        data = struct.pack("<b", self.led)
        self._cf.appchannel.send_packet(data)

    def getIsConnected(self):
        return self._isConnected

    def getVBat(self):
        return self._vbat

    def getSpeed(self):
        return self._speed

    def dump(self):
        return {'id': self._id,
                'vbat': self._vbat,
                'isConnected': self._isConnected,
                'speed': self._speed}