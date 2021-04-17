import logging
import socketio
import json
from vec3 import Vec3
import threading
import sys
import argparse
from subprocess import call
from flask import Flask, jsonify, render_template
from flask_socketio import *
import cflib
from cflib.crazyflie import Crazyflie
from drone_real import DroneReal
from drone_simulation import DroneSimulation
from drone_interface import StateMode
import threading
from enum import Enum
from threading import *
from DBconnect import DatabaseConnector
from map_catalog import MapCatalog
from map_handler import MapHandler

import logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

# Add paths toward dependecies in different subdirectories
sys.path.append(os.path.abspath('./src/map'))
from map_handler import MapHandler
from setup_logging import LogsConfig

class Mode(Enum):
    REAL = 0
    SIMULATION = 1

logsConfig = LogsConfig()
logger = logsConfig.logger('server')

app = Flask(__name__)
socketio = SocketIO(app ,cors_allowed_origins='*')

default_port = 5015

# Select mode in commande line
parser =  argparse.ArgumentParser()
parser.add_argument('mode', default='real', choices=['simulation', 'real'], help='Type simulation to launch simulation mode and type real to run server with real drones')
args = parser.parse_args()

if (args.mode == 'simulation'):
    mode = Mode.SIMULATION
    print('mode simulation')
elif (args.mode == 'real'):
    mode = Mode.REAL
    print('mode real')
    
# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
print('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
print('Crazyflies found:')

if (mode == Mode.REAL):
    logger.info('Mode time')
    drones = []
    
else:
    logger.info('Mode simulation')
    drones = []
    
def createDrones(numberOfDrone):
    if mode == Mode.REAL:
        drones.append(DroneReal("radio://0/72/250K/E7E7E7E7E7"))
        drones.append(DroneReal("radio://0/72/250K/E7E7E7E7E5"))
    else:
        for i in range(numberOfDrone):
            drones.append(DroneSimulation(default_port + i))
            t = threading.Thread(target=drones[i].waiting_connection, name='waiting_connection')
            t.start()
            logger.info('Connection to port {}'.format(default_port + i))
            

def deleteDrones():
    for i in drones:
        if hasattr(i, "connection"):
            i.connection.close()
        i.sock.shutdown(2)
        i.sock.close()
        del i
    drones.clear()

@socketio.on('SET_MODE')
def setMode(data):
    deleteDrones()
    mode = data['mode_chosen']
    numberOfDrone = data['number_of_drone']
    dronesAreCreated = False
    while not dronesAreCreated:
        try:
            createDrones(int(numberOfDrone))
            dronesAreCreated = True
        except:
            deleteDrones()
            dronesAreCreated = False
    call(['./start-simulation.sh', '{}'.format(numberOfDrone)])

@socketio.on('TOGGLE_LED')
def ledToggler(data):
    print(data['id'])
    drones[data['id']].toggleLED()
    print("LED TOGGLER")
    logger.info('ledTogger function executed with data {}'.format(data['id']))

@socketio.on('TAKEOFF')
def takeOff(data):
    print("takeoff for", data)
    if (data['id'] == -2):
        for i in drones:
            i.send_data(StateMode.TAKE_OFF.value, "<i")
            logger.info('Take off of {}'.format(i))

    else:
        socks[data['id'] - drones[0]._id].send_data(StateMode.TAKE_OFF.value, "<i")
        logger.info('Take off of {}'.format(socks[data['id'] - drones[0]._id]))
     
@socketio.on('RETURN_BASE')
def returnToBase(data):
    if (data['id'] == -2):
        for i in drones:
            i.send_data(StateMode.RETURN_TO_BASE.value, "<i")
            logger.info('Return to base of {}'.format(i))
    else:
        socks[data['id'] - drones[0]._id].send_data(StateMode.RETURN_TO_BASE.value, "<i")
        logger.info('Return to base of {}'.format(socks[data['id'] - drones[0]._id]))

@socketio.on('LAND')
def land(data):
    if (data['id'] == -2):
        for i in drones:
            i.send_data(StateMode.LANDING.value, "<i")
            logger.info('Landing of {}'.format(i))
    else:
        drones[data['id']].send_data(StateMode.LANDING.value, "<i")
        logger.info('Landing of {}'.format(socks[data['id']]))

@socketio.on('MAP_CATALOG')
def getMapList():
    map_catalog = MapCatalog()
    maps = map_catalog.get_map_list()
    map_list = json.dumps([map_catalog.map_list_to_Json(map) for map in maps])
    socketio.emit('MAP_LIST', map_list)

@socketio.on('END_MISSION')
def endOfMission(data):
    mapHandler = MapHandler()
    mapHandler.current_map.end_mission()

@socketio.on('SELECT_MAP')
def getMapPoints(data):
    map_catalog = MapCatalog()
    map_points_json = map_catalog.get_select_map(data['id'])
    socketio.emit('MAP_POINTS', map_points_json)

@socketio.on("DELETE_MAP")
def deleteMap(data):
    map_catalog = MapCatalog()
    map_catalog.delete_map(data['id'])
    maps = map_catalog.get_map_list()
    map_list = json.dumps([map_catalog.map_list_to_Json(map) for map in maps])
    socketio.emit('MAP_LIST', map_list)

def send_data():
    data_to_send = json.dumps([drone.dump() for drone in drones])
    socketio.emit('DRONE_LIST', data_to_send, broadcast=True)
    logger.info('send data to client')

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

    database_initializer = DatabaseConnector()
    database_initializer.create_table()
    
    map_handler = MapHandler()
    thread_map_handler = threading.Thread(target=map_handler.send_point, args=(socketio,), name='send_new_points')
    thread_map_handler.start()
        
    createDrones(2)
    set_interval(send_data, 1)
    app.run()
    while True:
        if mode == Mode.SIMULATION:
            for i in range(numberOfDrone):
                if (drones[i].data_received != None):
                    drones[i].start_receive_data()
