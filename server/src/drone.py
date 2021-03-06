import json
import logging
import time
from sensor import Sensor
from vec3 import Vec3
from threading import Thread
from enum import Enum

import struct
import cflib
from cflib.crazyflie import Crazyflie

logging.basicConfig(level=logging.ERROR)

class PacketType(Enum):
    TX = 0
    POSITION = 1
    VELOCITY = 2
    DISTANCE = 3

class Drone :
    sensors = Sensor(0,0,0,0,0,0,0,0,0)
    __startPos = Vec3(0,0,0)
    currentPos = Vec3(0,0,0)
    led = False
    def __init__(self, link_uri, initialPos: Vec3):

        self._cf = Crazyflie()
        self.__startPos = initialPos
        self._cf.connected.add_callback(self._connected)
        self._cf.disconnected.add_callback(self._disconnected)
        self._cf.connection_failed.add_callback(self._connection_failed)
        self._cf.connection_lost.add_callback(self._connection_lost)

        self._cf.appchannel.packet_received.add_callback(self._app_packet_received)

        self._cf.open_link(link_uri)
        self._isConnected = False
        self._vbat = 3.757
        

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
        if data[0] == PacketType.TX.value:
            (packet_type, is_led_activated, vbattery, rssi) = struct.unpack("<bbfb", data)
            self._vbat = vbattery
            print(f"ID : {self._cf.link_uri} | Battery Voltage : {vbattery} | RSSI : {rssi}")
        elif data[0] == PacketType.POSITION.value:
            (packet_type, x, y, z) = struct.unpack("<bfff", data)
            print(f"ID : {self._cf.link_uri} | Position : {x}, {y}, {z}")
        elif data[0] == PacketType.VELOCITY.value:
            (packet_type, px, py, pz) = struct.unpack("<bfff", data)
            print(f"ID : {self._cf.link_uri} | Velocity : {px}, {py}, {pz}")
        elif data[0] == PacketType.DISTANCE.value:
            (packet_type, front, back, up, left, right, zrange) = struct.unpack("<bhhhhhh", data)
            print(f"ID : {self._cf.link_uri} | Front : {front} | Back : {back} | Up : {up} | Left : {left} | Right : {right} | Zrange : {zrange}")

    def toggleLED(self):
        self.led = not self.led
        data = struct.pack("<b", self.led)
        self._cf.appchannel.send_packet(data)

    def getIsConnected(self):
        return self._isConnected

    def getVBat(self):
        if(self._vbat <= 4.3 & self._vbat >= 4.2):
            return 100
        if(self._vbat <4.2 & self._vbat >= 4.15):
            return 95
        
        if(self._vbat < 4.15 & self._vbat >= 4.1):
            return 90
        
        if(self._vbat < 4.1 & self._vbat >= 4.05):
            return 85
        
        if(self._vbat < 4.05 & self._vbat >= 4.025):
            return 80
        
        if(self._vbat < 4.025 & self._vbat >= 4.0):
            return 75
        
        if(self._vbat < 4.0 & self._vbat >= 3.95):
            return 70
        
        if(self._vbat < 3.95 & self._vbat >= 3.90):
            return 65
        
        if(self._vbat < 3.9 & self._vbat >= 3.8875):
            return 60
        
        if(self._vbat < 3.875 & self._vbat >= 3.8625):
            return 55
        
        if(self._vbat < 3.8625 & self._vbat >= 3.85):
            return 50
        
        if(self._vbat < 3.85 & self._vbat >= 3.8375):
            return 45
        
        if(self._vbat < 3.875 & self._vbat >= 3.825):
            return 40
        
        if(self._vbat < 3.825 & self._vbat >= 3.8):
            return 35
        
        if(self._vbat < 3.8 & self._vbat >= 3.775):
            return 30
        
        if(self._vbat < 3.775 & self._vbat >= 3.75):
            return 25
        
        if(self._vbat < 3.75 & self._vbat >= 3.725):
            return 20
        
        if(self._vbat < 3.725 & self._vbat >= 3.7):
            return 10
        
        if(self._vbat < 3.7 & self._vbat >= 3.6):
            return 5
        
        if(self._vbat < 3.6 & self._vbat >= 3.5):
            return 3
        
        if(self._vbat < 3.5):
            return 0
        

    def toJson(self) :
        return {
            'sensors'   : self.sensors.toJson(),
            'currentPos': (self.__startPos + self.currentPos ).toJson(),
            'batteryLvl': self._vbat,
            'status'    : self._isConnected
        }