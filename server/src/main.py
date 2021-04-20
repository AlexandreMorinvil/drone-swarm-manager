# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./drone'))
sys.path.append(os.path.abspath('./log'))
sys.path.append(os.path.abspath('./map'))

# Add dependecies
from engineio.payload import Payload
from setup_logging import LogsConfig
from map_handler import MapHandler
from map_catalog import MapCatalog
from DBconnect import DatabaseConnector
from enum import Enum
from drone_interface import StateMode
from drone_simulation import DroneSimulation
from drone_real import DroneReal
from cflib.crazyflie import Crazyflie
import cflib
from flask_socketio import *
from flask import Flask, jsonify, render_template
from subprocess import call
import threading
from vec3 import Vec3
import json
import socketio
import logging

from utility import set_interval


log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)


Payload.max_decode_packets = 0xFFFFF


class Mode(Enum):
    REAL = 0
    SIMULATION = 1


logsConfig = LogsConfig()
logger = logsConfig.logger('server')

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins='*')

default_port = 5015

mode = Mode.SIMULATION

# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
logger.info('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
logger.info('Crazyflies found:')

drones = []
initPos = []


def createDrones(numberOfDrone):
    for i in range(numberOfDrone):
        if mode == Mode.REAL:
            initPosVec3 = Vec3(initPos[i]['x'], initPos[i]['y'])
            drones.append(DroneReal(initPos[i]['address'], initPosVec3))
        else:
            drones.append(DroneSimulation(default_port + i))
            t = threading.Thread(
                target=drones[i].waiting_connection, name='waiting_connection')
            t.start()
            logger.info('Connection to port {}'.format(default_port + i))


def deleteDrones():
    if (mode == Mode.SIMULATION):
        for i in drones:
            if hasattr(i, "connection"):
                i.connection.close()
            i.sock.shutdown(2)
            i.sock.close()
            del i
    else:
        for i in drones:
            i.close_connection()
    drones.clear()


@socketio.on('SET_MODE')
def setMode(data):
    deleteDrones()
    mode = data['mode_chosen']
    numberOfDrone = data['number_of_drone']
    if (mode == Mode.SIMULATION.value):
        dronesAreCreated = False
        while not dronesAreCreated:
            try:
                createDrones(int(numberOfDrone))
                dronesAreCreated = True
            except:
                deleteDrones()
                dronesAreCreated = False
        call(['scripts/start-simulation.sh', '{}'.format(numberOfDrone)])
    else:
        createDrones(int(numberOfDrone))

    if (mode == Mode.REAL):
        logger.info('Mode time')
    else:
        logger.info('Mode simulation')


@socketio.on('INITIAL_POSITION')
def set_real_position(data):
    initPos.clear()
    initPos.extend(data)


@socketio.on('TOGGLE_LED')
def ledToggler(data):
    print(data['id'])
    drones[data['id']].toggleLED()
    print("LED TOGGLER")
    logger.info('ledTogger function executed with data {}'.format(data['id']))


@socketio.on('SWITCH_STATE')
def land(data):
    if (data['id'] == -2):
        for i in drones:
            i.switch_state(data['state'])
            logger.info('Landing of {}'.format(i))
    else:
        drones[data['id']].switch_state(data['state'])
        logger.info('Switch state of {} to {} mode'.format(
            socks[data['id'], StateMode(data['state'])]))


@socketio.on('MAP_CATALOG')
def getMapList():
    map_catalog = MapCatalog()
    maps = map_catalog.get_map_list()
    map_list = json.dumps([map_catalog.map_list_to_Json(map) for map in maps])
    socketio.emit('MAP_LIST', map_list)


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


@socketio.on('SEND_FILE')
def receiveFile(data):
    file = open('../../robot/src/' + data['name'], 'wb')
    file.write(data['content'])


@socketio.on('UPDATE')
def startUpdate():
    for drone in drones:
        if (drone.get_state() != StateMode.STANDBY.value):
            socketio.emit("FAILED_UPDATE")
            return
    numberOfDrone = len(drones)
    deleteDrones()
    for address in initPos:
        if (call(['scripts/update-robot.sh', '{}'.format(address['address'])]) == 1):
            socketio.emit("FAILED_UPDATE")
    createDrones(numberOfDrone)


@socketio.on('ReqSource')
def sendSources():
    for root, dirs, files in os.walk('../../robot/src/'):
        for filename in files:
            f = open('../../robot/src/'+filename)
            socketio.emit('old_src', {
                'name': filename,
                'data': f.read()
            })


def send_data():
    data_to_send = json.dumps([drone.dump() for drone in drones])
    socketio.emit('DRONE_LIST', data_to_send, broadcast=True)
    logger.info('send data to client')

def main():

    # Initialize the database
    database_initializer = DatabaseConnector()
    database_initializer.create_table()

    # Initialze the map handler
    map_handler = MapHandler()
    thread_map_handler = threading.Thread(
        target=map_handler.send_point, args=(socketio,), name='send_new_points')
    thread_map_handler.start()

    set_interval(send_data, 0.5)
    app.run(host='0.0.0.0', port=5000)
    while True:
        if mode == Mode.SIMULATION:
            for i in range(numberOfDrone):
                if (drones[i].data_received != None):
                    drones[i].start_receive_data()

if __name__ == '__main__':
    main()
