import socket
import threading
import struct
from vec3 import Vec3
from enum import Enum
from drone import Drone
from setup_logging import LogsConfig

import sys
import os

# Add paths toward dependecies in different subdirectories
sys.path.append(os.path.abspath('./map'))
from data_accumulator import MapObservationAccumulator

class PacketType(Enum):
    TX = 0
    POSITION = 1
    ATTITUDE = 2
    VELOCITY = 3
    DISTANCE = 4
    ORIENTATION = 5


class ArgosServer() : 
    #server_address =  socket.getaddrinfo('localhost', 8))
    
    
    def __init__(self, id, port):
        self.drone_argos = Drone()

        # listen for incoming connections (server mode) with one connection at a time
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #self.sock.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
        self.sock.bind(('localhost', port))
        self.sock.listen()

        self.logsConfig = LogsConfig()
        self.logger = self.logsConfig.logger('Argos_server')
        self.logger.info('Create drone argos in server')

        # Initialize the live map handler
        # self.map_observation_accumulator = MapObservationAccumulator()

    def waiting_connection(self):
        # wait for a connection
        self.logger.info('waiting for a connection')
        self.connection, self.client_address = self.sock.accept()

        # show who connected to us
        self.logger.info('connection from {}'.format(self.client_address))
        self.receive_data()

    def send_data(self, packet, format_packer):
        data = struct.pack(format_packer, packet)
        self.connection.send(data)
        
    def receive_data(self):
        self.logger.info('Receive data from argos id s{}'.format(id))
        while True:
            self.connection.settimeout(5.0)
            self.data_received = self.connection.recv(16)   
            self.data_received += bytearray(16 - len(self.data_received)) 

            if PacketType(self.data_received[0]) == PacketType.TX:
                (packet_type, state, vbattery, is_led_activated, a, b, c) = struct.unpack("<iif?bbb", self.data_received)
                self.drone_argos._state = state
                self.drone_argos._vbat = vbattery
                self.drone_argos.led = is_led_activated

            elif PacketType(self.data_received[0]) == PacketType.POSITION:
                (packet_type, x, y, z) = struct.unpack("<ifff", self.data_received)
                self.drone_argos.currentPos.x = x
                self.drone_argos.currentPos.y = y
                self.drone_argos.currentPos.z = z
                self.map_observation_accumulator.receive_position(x, y, z)
                
            elif PacketType(self.data_received[0]) == PacketType.DISTANCE:
                (packet_type, front, back, up, left, right, zrange) = struct.unpack("<ihhhhhh", self.data_received)
                self.map_observation_accumulator.receive_sensor_distances(front, back, up, left, right, zrange)

            elif PacketType(self.data_received[0]) == PacketType.ORIENTATION:
                (packet_type, pitch, roll, yaw) = struct.unpack("<ifff", self.data_received)
                self.map_observation_accumulator.receive_sensor_orientations(yaw, pitch, roll)

            elif PacketType(self.data_received[0]) == PacketType.VELOCITY:
                (packet_type, x_speed, y_speed, z_speed) = struct.unpack("<ifff", self.data_received)
                self.drone_argos._speed.x = x_speed
                self.drone_argos._speed.y = y_speed
                self.drone_argos._speed.z = z_speed
    
    def start_receive_data(self):
        t2 = threading.Thread(target=self.receive_data, args=(), name="receive_data_{}".format(id))
        t2.start
    
    