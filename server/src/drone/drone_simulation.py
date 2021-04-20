import socket
import threading
from vec3 import Vec3
from enum import Enum
from setup_logging import LogsConfig
from drone_interface import DroneInterface
from data_accumulator import MapObservationAccumulator
import struct


class DroneSimulation(DroneInterface) : 

    def __init__(self, address, initialPos=Vec3()):
        super().__init__(address, initialPos)
        self.logsConfig = LogsConfig()
        self.logger = self.logsConfig.logger('DroneSimulation')

        # listen for incoming connections (server mode) with one connection at a time
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(('localhost', address))
        self.sock.listen()
        self.logger.info('Create drone simulation in server')

        # Initialize the live map handler
        self.map_observation_accumulator = MapObservationAccumulator(True)

    def waiting_connection(self):
        # wait for a connection
        self.logger.info('waiting for a connection')
        self.connection, self.client_address = self.sock.accept()

        # show who connected to us
        self.logger.info('connection from {}'.format(self.client_address))
        self.receive_data()

    def _send_data(self, packet):
        self.connection.send(packet)
        
    def receive_data(self):
        self.logger.info('Receive data from argos id s{}'.format(id))
        while True:
            self.connection.settimeout(5.0)
            self.data_received = self.connection.recv(16)
            self.data_received += bytearray(16 - len(self.data_received)) 
            self._process_data_received(self.data_received)
    
    def start_receive_data(self):
        t2 = threading.Thread(target=self.receive_data, args=(), name="receive_data_{}".format(id))
        t2.start
    
    def get_vBat(self):
        return self._vbat