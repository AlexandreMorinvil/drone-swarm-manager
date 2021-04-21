# Add paths toward dependecies in different subdirectories
import struct
from abc import ABC, abstractmethod
from vec3 import Vec3
from enum import Enum
from setup_logging import LogsConfig
import logging
import sys
import os
sys.path.append(os.path.abspath('./map'))
sys.path.append(os.path.abspath('./log'))

# Add dependecies


class PacketType(Enum):
    TX = 0
    POSITION = 1
    ATTITUDE = 2
    VELOCITY = 3
    DISTANCE = 4
    ORIENTATION = 5
    SWITCH_STATE = 6
    SET_INIT_POS = 7


class StateMode(Enum):
    STANDBY = 0
    TAKE_OFF = 1
    FLYING = 2
    RETURN_TO_BASE = 3
    LANDING = 4
    EMERGENCY = 5
    FAIL = 6
    UPDATE = 7


class DroneInterface(ABC):
    # Static variable
    id_counter = 0

    def __init__(self, address, initialPos=Vec3()):
        self._id = DroneInterface.id_counter
        DroneInterface.id_counter += 1
        self._startPos = initialPos
        self._isConnected = False
        self._state = StateMode.STANDBY.value
        self._vbat = 10
        self._speed = Vec3(0, 0, 0)
        self.currentPos = Vec3(0, 0, 0)
        self.led = True

    def dump(self):
        #posAbs = self._startPos + self.currentPos
        return {
            'id': self._id,
            'state': self._state,
            'vbat': self.get_vBat(),
            'isConnected': self._isConnected,
            'currentPos': self.currentPos.toJson(),
            'currentSpeed': self._speed.toJson(),
        }

    def _process_data_received(self, data):
        data += bytearray(16 - len(data))
        if PacketType(data[0]) == PacketType.TX:
            (packet_type, state, vbattery, is_led_activated,
             a, b, c) = struct.unpack("<iif?bbb", data)
            self._state = state
            self._vbat = vbattery
            self.led = is_led_activated

        elif PacketType(data[0]) == PacketType.POSITION:
            (packet_type, x, y, z) = struct.unpack("<ifff", data)
            self.currentPos.x = x
            self.currentPos.y = y
            self.currentPos.z = z
            self.map_observation_accumulator.receive_position(x, y, z)

        elif PacketType(data[0]) == PacketType.DISTANCE:
            (packet_type, front, back, up, left, right,
             zrange) = struct.unpack("<ihhhhhh", data)
            self.map_observation_accumulator.receive_sensor_distances(
                front, back, up, left, right, zrange)

        elif PacketType(data[0]) == PacketType.ORIENTATION:
            (packet_type, pitch, roll, yaw) = struct.unpack("<ifff", data)
            self.map_observation_accumulator.receive_sensor_orientations(
                yaw, pitch, roll)

        elif PacketType(data[0]) == PacketType.VELOCITY:
            (packet_type, x_speed, y_speed, z_speed) = struct.unpack("<ifff", data)
            self._speed.x = x_speed
            self._speed.y = y_speed
            self._speed.z = z_speed

    def switch_state(self, stateMode):
        packet = struct.pack("<if", PacketType.SWITCH_STATE.value, stateMode)
        self._send_data(packet)

    def get_state(self):
        return self._state

    @abstractmethod
    def _send_data(self, packet):
        pass

    @abstractmethod
    def get_vBat(self):
        pass

    @abstractmethod
    def delete():
        pass
