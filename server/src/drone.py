import json
import logging
import time
from sensor import Sensor
from vec3 import Vec3
from threading import Thread
from enum import Enum
from sensor import Sensor
from vec3 import Vec3
import struct
import cflib
from cflib.crazyflie import Crazyflie

import sys
import os

# Add paths toward dependecies in different subdirectories
sys.path.append(os.path.abspath('./map'))
from data_accumulator import MapObservationAccumulator

logging.basicConfig(level=logging.ERROR)

class PacketType(Enum):
    TX = 0
    POSITION = 1
    ATTITUDE = 2
    VELOCITY = 3
    DISTANCE = 4
    ORIENTATION = 5

class StateMode(Enum):
    STANDBY = 0
    TAKE_OFF = 1
    FLYING = 2
    RETURN_TO_BASE = 3
    LANDING = 4
    EMERGENCY = 5
    FAIL = 6
    UPDATE = 7

class Drone :

    # Static variable
    id_counter = 0


    def __init__(self, link_uri="", initialPos=Vec3()):
        # Constructor
        self._id = Drone.id_counter
        Drone.id_counter += 1

        self._cf = Crazyflie()
        self.__startPos = initialPos
        self._cf.connected.add_callback(self._connected)
        self._cf.disconnected.add_callback(self._disconnected)
        self._cf.connection_failed.add_callback(self._connection_failed)
        self._cf.connection_lost.add_callback(self._connection_lost)

        self._cf.appchannel.packet_received.add_callback(self._app_packet_received)

        self._cf.open_link(link_uri)
        self._isConnected = False
        self._state = StateMode.STANDBY.value
        self._vbat = 10
        self._speed = Vec3(0, 0, 0)

        self.__startPos = Vec3(0,0,0)
        self.currentPos = Vec3(0,0,0)
        self.led = True

        self.sensors = Sensor()

        # Initialize the live map handler
        self.map_observation_accumulator = MapObservationAccumulator()

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

    def _app_packet_received(self, data_received):
        #data_received += bytearray(16 - len(data_received)) 
        if data_received[0] == PacketType.TX.value:
            (packet_type, state, vbattery, is_led_activated) = struct.unpack("<bbfb", data_received)
            self._state = state
            self._vbat = vbattery
            self.led = is_led_activated
        
        elif data_received[0] == PacketType.POSITION.value:
            (packet_type, x, y, z) = struct.unpack("<bfff", data_received)
            self.currentPos.x = x
            self.currentPos.y = y
            self.currentPos.z = z
            print(f"x : {x}, y : {y}, z : {z}")
            self.map_observation_accumulator.receive_position(x, y, z)
         
        elif data_received[0] == PacketType.DISTANCE.value:
            (packet_type, front, back, up, left, right, zrange) = struct.unpack("<bhhhhhh", data_received)
            print(f"front : {front}, down : {zrange}")
            self.map_observation_accumulator.receive_sensor_distances(front, back, up, left, right, zrange)
        
        elif data_received[0] == PacketType.ORIENTATION.value:
            (packet_type, pitch, roll, yaw) = struct.unpack("<bfff", data_received)
            print(f"yaw : {yaw}")
            self.map_observation_accumulator.receive_sensor_orientations(yaw*(3.14/180), pitch, roll)
        
        elif data_received[0] == PacketType.VELOCITY.value:
            (packet_type, x_speed, y_speed, z_speed) = struct.unpack("<bfff", data_received)
            self._speed.x = x_speed
            self._speed.y = y_speed
            self._speed.z = z_speed

        # if data[0] == PacketType.TX.value:
        #     (packet_type, is_led_activated, vbattery, rssi) = struct.unpack("<bbfb", data)
        #     self._vbat = vbattery
        #     print(f"ID : {self._cf.link_uri} | Battery Voltage : {vbattery} | RSSI : {rssi}")
        # elif data[0] == PacketType.POSITION.value:
        #     (packet_type, x, y, z, yaw) = struct.unpack("<bffff", data)
        #     print(f"ID : {self._cf.link_uri} | Position : {x}, {y}, {z} | Yaw : {yaw}")
        # elif data[0] == PacketType.VELOCITY.value:
        #     (packet_type, px, py, pz) = struct.unpack("<bfff", data)
        #     print(f"ID : {self._cf.link_uri} | Velocity : {px}, {py}, {pz}")
        # elif data[0] == PacketType.DISTANCE.value:
        #     (packet_type, front, back, up, left, right, zrange) = struct.unpack("<bhhhhhh", data)
        #     print(f"ID : {self._cf.link_uri} | Front : {front} | Back : {back} | Up : {up} | Left : {left} | Right : {right} | Zrange : {zrange}")


    def send_data(self, data):
        self._cf.appchannel.send_packet(data)

    def getIsConnected(self):
        return self._isConnected

    def getVBat(self):
        return self._vbat

    def getSpeed(self):
        return self._speed

    def dump(self):
        posAbs = self.__startPos + self.currentPos
        return {'id': self._id,
                'state': self._state,
                'vbat': self._vbat,
                'isConnected': self._isConnected,
                'currentPos': posAbs.toJson(),
                'currentSpeed': self._speed.toJson(),
                }
