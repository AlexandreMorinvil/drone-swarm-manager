# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./api'))
sys.path.append(os.path.abspath('./drone'))
sys.path.append(os.path.abspath('./log'))
sys.path.append(os.path.abspath('./map'))

# Add dependencies
from api_drone import api_control_switch_state
from api_environment import api_environment_set_mode, api_environment_set_real_position
from api_map import api_map_get_map_list, api_map_get_map_points, api_map_delete_map
from api_update import api_update_receive_file, api_update_start_update, api_update_send_sources

import cflib
import logging
import socketio
import threading

from engineio.payload import Payload
from setup_logging import LogsConfig
from map_handler import MapHandler
from DBconnect import DatabaseConnector
from cflib.crazyflie import Crazyflie
from flask_socketio import *
from flask import Flask, jsonify, render_template
from drone_list import DroneList
from environment import Environment
from utility import set_interval

# Initializing the socketio socket
app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins='*')

@socketio.on('SET_MODE')
def set_mode(data):
    api_environment_set_mode(data)

@socketio.on('INITIAL_POSITION')
def set_real_position(data):
    api_environment_set_real_position(data)

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
def receive_file(data):
    api_update_receive_file(data)

@socketio.on('UPDATE')
def start_update():
    is_successful = api_update_start_update()
    if not is_successful:
        socketio.emit("FAILED_UPDATE")

@socketio.on('REQUEST_SOURCE')
def sendSources():
    data_to_send = api_update_send_sources()
    socketio.emit('OLD_SOURCE', data_to_send)

def api_drone_list_send_fleet():
    data_to_send = DroneList.dumps()
    socketio.emit('DRONE_LIST', data_to_send, broadcast=True)

def main():

    # Initialize the logs
    logsConfig = LogsConfig()
    logger = logsConfig.logger('main')
    log = logging.getLogger('werkzeug')
    log.setLevel(logging.ERROR)
    logger.info('Starting the application')

    # Initialize the database
    database_initializer = DatabaseConnector()
    database_initializer.create_table()

    # Initialze the map handler
    map_handler = MapHandler()
    thread_map_handler = threading.Thread(
        target=map_handler.send_point, args=(socketio,), name='send_new_points')
    thread_map_handler.start()

    # Initialize the maximum payload for the file update
    Payload.max_decode_packets = 0xFFFFF

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
