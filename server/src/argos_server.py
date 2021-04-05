import socket
import threading
import struct
from vec3 import Vec3
from enum import Enum
from drone import Drone
from setup_logging import LogsConfig


class PacketType(Enum):
    TX = 0
    POSITION = 1
    ATTITUDE = 2
    VELOCITY = 3
    DISTANCE = 4


class ArgosServer() : 
    #server_address =  socket.getaddrinfo('localhost', 8))
    
    
    def __init__(self, id, port):
        self.logsConfig = LogsConfig()
        self.logger = self.logsConfig.logger('Argos_server')
        self.drone_argos = Drone("id", Vec3(0, 0, 0),id)
        self.data_received = None
        self.sent_data = None
        self.point = Vec3(0,0,0)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(('localhost', port))
         # listen for incoming connections (server mode) with one connection at a time
        self.sock.listen()
        self.logger.info('Create drone argos in server')

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
                (a, b, packet_type, is_led_activated, vbattery, rssi) = struct.unpack("<hffbfb", self.data_received)
                self.drone_argos._vbat = vbattery

            elif PacketType(self.data_received[0]) == PacketType.POSITION:
                (a, b, c, packet_type, x, y, z) = struct.unpack("<bbbbfff", self.data_received)
                self.drone_argos.currentPos.x = x
                self.drone_argos.currentPos.y = y
                self.drone_argos.currentPos.z = z


            elif PacketType(self.data_received[0]) == PacketType.VELOCITY:
                (packet_type, px, py, pz) = struct.unpack("<ffff", self.data_received)
                self.drone_argos._speed.x = px
                self.drone_argos._speed.y = py
                self.drone_argos._speed.z = pz
                
            elif PacketType(self.data_received[0]) == PacketType.DISTANCE:
                (packet_type, front, back, up, left, right, zrange, a, b) = struct.unpack("<hbhhhhhhb", self.data_received)
                self.drone_argos.sensors.front = front
                self.drone_argos.sensors.back = back
                self.drone_argos.sensors.up = up
                self.drone_argos.sensors.left = left
                self.drone_argos.sensors.right = right
                self.drone_argos.sensors.down = zrange
                

    def set_interval(self, func, sec):
        def func_wrapper():
            self.set_interval(func, sec)
            func()  
        t = threading.Timer(sec, func_wrapper)
        t.start()
        return t
    
    def start_receive_data(self):
        t2 = threading.Thread(target=self.receive_data, name="receive_data")
        t2.start
    
    