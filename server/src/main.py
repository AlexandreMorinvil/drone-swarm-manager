# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./api'))
sys.path.append(os.path.abspath('./drone'))
sys.path.append(os.path.abspath('./log'))
sys.path.append(os.path.abspath('./map'))

# Add dependencies
from api_drone import api_control_switch_state
from api_map import api_map_get_map_list, api_map_get_map_points, api_map_delete_map

from engineio.payload import Payload
from setup_logging import LogsConfig
from map_handler import MapHandler
from map_catalog import MapCatalog
from DBconnect import DatabaseConnector
from drone_interface import StateMode
from cflib.crazyflie import Crazyflie
import cflib
from flask_socketio import *
from flask import Flask, jsonify, render_template
from subprocess import call
import threading
import json
import socketio
import logging



from drone_list import DroneList
from environment import Environment
from utility import set_interval


Payload.max_decode_packets = 0xFFFFF


logsConfig = LogsConfig()
logger = logsConfig.logger('main')

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
        Environment.launch_simulation(number_drones)
    else:
        DroneList.createDrones(int(number_drones))


@socketio.on('INITIAL_POSITION')
def set_real_position(data):
    DroneList.initial_posisitions.clear()
    DroneList.initial_posisitions.extend(data)

@socketio.on('SWITCH_STATE')
def switch_state(data):
    api_control_switch_state(data)

@socketio.on('MAP_CATALOG')
def get_map_list():
    data_to_return = api_map_get_map_list()
    socketio.emit('MAP_LIST', data_to_return)

@socketio.on('SELECT_MAP')
def get_map_points(data):
    data_to_return = api_map_get_map_points(data)
    socketio.emit('MAP_POINTS', data_to_return)

@socketio.on("DELETE_MAP")
def delete_map(data):
    data_to_return = api_map_delete_map(data)
    socketio.emit('MAP_LIST', data_to_return)


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


def api_drone_list_send_fleet():
    data_to_send = DroneList.dumps()
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

    # Initialize automatically transmitted data for the API
    set_interval(api_drone_list_send_fleet, 0.5)

    # Initialize the serveur's API process
    app.run(host='0.0.0.0', port=5000)

if __name__ == '__main__':
    main()
