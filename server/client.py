import socket
import struct
import sys

# Create a TCP/IP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 8080)
s.connect(server_address)
print('Client connected to server')

dataToSend = str.encode('Hello from client')


data = s.recv(1024)
print('Data from server ', data)

s.sendall(dataToSend)
print('Package sent', dataToSend)
print('size ', len(dataToSend))

if (len(data) == 0):
    print('Not receive data')
    s.close

s.close()



