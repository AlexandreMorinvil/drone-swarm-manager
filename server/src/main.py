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



from drone_list import DroneList
from environment import Environment
from utility import set_interval


Payload.max_decode_packets = 0xFFFFF


logsConfig = LogsConfig()
logger = logsConfig.logger('server')

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins='*')


@socketio.on('SET_MODE')
def setMode(data):
    mode = data['mode_chosen']
    number_drones = data['number_of_drone']

    DroneList.delete_drones()

    # Set the environment's mode
    Environment.set_mode(mode)

    if (Environment.is_in_simulation()):
        DroneList.createDrones(int(number_drones), mode)

        # dronesAreCreated = False
        # while not dronesAreCreated:
        #     try:
        #         DroneList.createDrones(int(number_drones))
        #         dronesAreCreated = True
        #     except:
        #         DroneList.delete_drones()
        #         dronesAreCreated = False
        Environment.launch_simulation(number_drones)
        # call(['scripts/start-simulation.sh', '{}'.format(number_drones)])
    else:
        DroneList.createDrones(int(number_drones))


@socketio.on('INITIAL_POSITION')
def set_real_position(data):
    DroneList.initial_posisitions.clear()
    DroneList.initial_posisitions.extend(data)

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
    number_drones = len(drones)
    DroneList.delete_drones()
    for address in initial_posisitions:
        if (call(['scripts/update-robot.sh', '{}'.format(address['address'])]) == 1):
            socketio.emit("FAILED_UPDATE")
    DroneList.createDrones(number_drones)


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
    data_to_send = DroneList.dumps() #json.dumps([drone.dump() for drone in drones])
    socketio.emit('DRONE_LIST', data_to_send, broadcast=True)

def main():

    # Disable the werkzeug log generated in the server
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)

    # Initialize the database
    database_initializer = DatabaseConnector()
    database_initializer.create_table()

    # Initialze the map handler
    map_handler = MapHandler()
    thread_map_handler = threading.Thread(
        target=map_handler.send_point, args=(socketio,), name='send_new_points')
    thread_map_handler.start()

    # Initialze the environement
    environement = Environment()

    # Initialize the drone list
    drone_list = DroneList()

    # Initialize the drone state transmission
    set_interval(send_data, 0.5)

    # Initialize the serveur's API process
    app.run(host='0.0.0.0', port=5000)

if __name__ == '__main__':
    main()
