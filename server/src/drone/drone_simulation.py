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

        # Initialize the logger
        self.logsConfig = LogsConfig()
        self.logger = self.logsConfig.logger('DroneSimulation {}'.format(address))

        # listen for incoming connections (server mode) with one connection at a time
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('localhost', address))
        self.sock.listen()
        self.logger.info('Create drone simulation in server')

        # Initialize the live map handler
        self.map_observation_accumulator = MapObservationAccumulator(True)

        # Observation thread initialization
        self.t2 = None
    
    def delete(self):
        try:
            if hasattr(self, "connection"):
                self.connection.close()
            self.sock.shutdown(2)
            self.sock.close()
        except:
            pass

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
            try:
                self.connection.settimeout(5.0)
                self.data_received = self.connection.recv(16)
                self.data_received += bytearray(16 - len(self.data_received)) 
                self._process_data_received(self.data_received)
            except:
                pass
    
    def get_vBat(self):
        return self._vbat