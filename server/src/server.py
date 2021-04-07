import socketio
import json
from vec3 import Vec3
from drone import *
import threading

from flask import Flask, jsonify, render_template
from flask_socketio import *
import cflib
from cflib.crazyflie import Crazyflie
from argos_server import ArgosServer
import threading
from enum import Enum
from threading import *

class Mode(Enum):
    REAL_TIME = 0
    SIMULATION = 1


app = Flask(__name__)
socketio = SocketIO(app ,cors_allowed_origins='*')

default_port = 5015

# Select mode
mode = Mode.REAL_TIME

# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
print('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
print('Crazyflies found:')

if (mode == Mode.REAL_TIME):
    drones = [Drone("radio://0/72/250K",Vec3(0,0,0),0), Drone("radio://0/80/250K",Vec3(0,0,0),1), Drone("radio://0/80/250K",Vec3(0,0,0),2), Drone("radio://0/80/250K",Vec3(0,0,0),4)]
else:
    drones = []

socks = []
for i in range(4):
    socks.append(ArgosServer(i, default_port + i))
    t = threading.Thread(target=socks[i].waiting_connection, name='waiting_connection')
    t.start()

    if mode == Mode.SIMULATION:
        drones.append(socks[i].drone_argos)


def setMode(mode_choosen):
    mode = mode_choosen

@socketio.on('TAKEOFF')
def takeOff(data):
    led = False
    packet = struct.pack("<bi", led, StateMode.TAKE_OFF.value)
    if (data['id'] == -2):
        for i in drones:
            i.send_data(packet)
    else:
        drones[data['id']].send_data(packet)

@socketio.on('EMERGENCY_LANDING')
def takeOff(data):
    led = False
    packet = struct.pack("<bi", led, StateMode.EMERGENCY.value)
    if (data['id'] == -2):
        for i in drones:
            i.send_data(packet)
    else:
        drones[data['id']].send_data(packet)
    
@socketio.on('RETURN_BASE')
def returnToBase(data):
    led = False
    packet = struct.pack("<bi", led, StateMode.RETURN_TO_BASE.value)
    if (data['id'] == -2):
        for i in drones:
            i.send_data(packet)
    else:
        drones[data['id']].send_data(packet)

def sendPosition():
    position_json = json.dumps({"x": socks[0].drone_argos.currentPos.x, "y": socks[0].drone_argos.currentPos.y, "z": socks[0].drone_argos.currentPos.z})
    socketio.emit('POSITION', position_json)

def send_data():
    data_to_send = json.dumps([drone.dump() for drone in drones])
    socketio.emit('drone_data', data_to_send, broadcast=True)

def set_interval(func, sec):
        def func_wrapper():
            set_interval(func, sec)
            func()  
        t = threading.Timer(sec, func_wrapper)
        t.start()
        return t


def set_interval(func, sec):
    def func_wrapper():
        set_interval(func, sec) 
        func()  
    t = threading.Timer(sec, func_wrapper)
    t.start()
    return t

if __name__ == '__main__':
    #t2 = threading.Thread(target=socks[1].receive_data, name='receive_data')
    #t2.start()
    set_interval(sendPosition, 1)
    set_interval(send_data, 1)
    app.run()

    while True:
        for i in range(4):
            if (socks[i].data_received != None):
                socks[i].start_receive_data()
