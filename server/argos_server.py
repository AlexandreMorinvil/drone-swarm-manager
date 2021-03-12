import socket
import threading
import struct
import vec3


class ArgosServer : 

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('localhost', 8001)

    data_received = None
    sent_data = None
    point = vec3.Vec3(0,0,0)
    
    def __init__(self):
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(self.server_address)
         # listen for incoming connections (server mode) with one connection at a time
        self.sock.listen()

        # wait for a connection
        print ('waiting for a connection')
        self.connection, self.client_address = self.sock.accept()

        # show who connected to us
        print ('connection from', self.client_address)



    def send_data(self):
        self.sent_data = "From Server"
        self.connection.send(self.sent_data.encode())
        print("Data sent ")
        
    def receive_data(self):
        print('entree receive_data')
        while True:
            self.data_received = self.connection.recv(16)
            if self.data_received:
                #rint(data)
                (a, b, c, packet_type, x, y, z) = struct.unpack("<bbbbfff", self.data_received)
                #print(f"ID : 1 | Position : {x}, {y}, {z}")
                self.point = vec3.Vec3(x, y, z)
                #print(str(self.point.x))
            #else:
                #print ("no data.")

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
    
    