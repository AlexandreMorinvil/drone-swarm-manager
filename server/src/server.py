import logging
import socketio
import json
from vec3 import Vec3
from drone import *
import threading
import sys
import argparse

from flask import Flask, jsonify, render_template
from flask_socketio import *
import cflib
from cflib.crazyflie import Crazyflie
from argos_server import ArgosServer
import threading
from enum import Enum
from threading import *
from setup_logging import LogsConfig

class Mode(Enum):
    REAL_TIME = 0
    SIMULATION = 1

logsConfig = LogsConfig()
logger = logsConfig.logger('server')

app = Flask(__name__)
socketio = SocketIO(app ,cors_allowed_origins='*')

default_port = 5015

# Select mode in commande line
parser =  argparse.ArgumentParser()
parser.add_argument('mode', default='realtime', choices=['simulation', 'realtime'], help='Type simulation for simulation mode and type realtime for real time mode')
args = parser.parse_args()

if (args.mode == 'simulation'):
    mode = Mode.SIMULATION
    print('mode simulation')
elif (args.mode == 'realtime'):
    mode = Mode.REAL_TIME
    print('mode real time')
    
# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
print('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
print('Crazyflies found:')

if (mode == Mode.REAL_TIME):
    logger.info('Mode real time')
    drones = [Drone("radio://0/80/250K",Vec3(0,0,0),0), Drone("radio://0/72/250K",Vec3(0,0,0),1), Drone("radio://0/72/250K",Vec3(0,0,0),2), Drone("radio://0/72/250K",Vec3(0,0,0),4)]
    
else:
    logger.info('Mode simulation')
    drones = []
    
socks = []
for i in range(4):
    socks.append(ArgosServer(i, default_port + i))
    t = threading.Thread(target=socks[i].waiting_connection, name='waiting_connection')
    t.start()

    if mode == Mode.SIMULATION:
        drones.append(socks[i].drone_argos)
        logger.info('Connection to port {}'.format(default_port + i))
        
def setMode(mode_choosen):
    mode = mode_choosen

@socketio.on('TOGGLE_LED')
def ledToggler(data):
    print(data['id'])
    drones[data['id']].toggleLED()
    print("LED TOGGLER")
    logger.info('ledTogger function executed with data {}'.format(data['id']))

@socketio.on('TAKEOFF')
def takeOff(data):
    if (data['id'] == -2):
        for i in range(4):
            socks[i].send_data(StateMode.TAKE_OFF.value, "<i")
            logger.info('Take off of {}'.format(socks[i]))
    else:
        socks[data['id']].send_data(StateMode.TAKE_OFF.value, "<i")
        logger.info('Take off of {}'.format(socks[data['id']]))
     
@socketio.on('RETURN_BASE')
def returnToBase(data):
    if (data['id'] == -2):
        for i in range(4):
            socks[i].send_data(StateMode.RETURN_TO_BASE.value, "<i")
            logger.info('Return to base of {}'.format(socks[i]))
    else:
        socks[data['id']].send_data(StateMode.RETURN_TO_BASE.value, "<i")
        logger.info('Return to base of {}'.format(socks[data['id']]))
    
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
    set_interval(sendPosition, 1)
    set_interval(send_data, 1)
    app.run()
    while True:
        for i in range(4):
            if (socks[i].data_received != None):
                socks[i].start_receive_data()
