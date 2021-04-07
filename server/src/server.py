import logging
import socketio
import json
from vec3 import Vec3
from drone import *
import threading
import sys
import argparse

from flask import Flask, jsonify
from flask_socketio import *
import cflib
from cflib.crazyflie import Crazyflie
from argos_server import ArgosServer
import threading
from enum import Enum
from threading import *
from subprocess import call
from setup_logging import LogsConfig

import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

# Add paths toward dependecies in different subdirectories
sys.path.append(os.path.abspath('./src/map'))
from map_handler import MapHandler

class Mode(Enum):
    REAL = 0
    SIMULATION = 1

logsConfig = LogsConfig()
logger = logsConfig.logger('server')

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
    logger.info('Mode simulation')
    drones = []
    
socks = []

def createDrones(numberOfDrone):
    for i in range(numberOfDrone):
        socks.append(ArgosServer(i, default_port + i))
        t = threading.Thread(target=socks[i].waiting_connection, name='waiting_connection')
        t.start()

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
    logger.info('send drones position')

def send_data():
    data_to_send = json.dumps([drone.dump() for drone in drones])
    socketio.emit('drone_data', data_to_send, broadcast=True)
    logger.info('send data to client')

def set_interval(func, sec):
        def func_wrapper():
            set_interval(func, sec)
            func()  
        t = threading.Timer(sec, func_wrapper)
        t.start()
        return t

if __name__ == '__main__':
    createDrones(4)

    # Map generation thread
    map_handler = MapHandler()
    thread_map_handler = threading.Thread(target=map_handler.send_point, args=(socketio,), name='send_new_points')
    thread_map_handler.start()

    set_interval(send_data, 1)
    app.run()
    while True:
        for i in range(numberOfDrone):
            if (socks[i].data_received != None):
                socks[i].start_receive_data()
