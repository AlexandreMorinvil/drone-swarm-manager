
import socketio
import json
from drone import Drone
import threading

from flask import Flask, jsonify, render_template
from flask_socketio import *
import cflib
from cflib.crazyflie import Crazyflie


app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins='*')

# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
print('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
print('Crazyflies found:')
drones = [Drone("radio://0/80/250K", 0), Drone("radio://0/71/250K", 1)]

@socketio.on('TOGGLE_LED')
def ledToggler(data):
    print(data['id'])
    drones[data['id']].toggleLED()
    print("LED TOGGLER")

def send_data():
    data_to_send = json.dumps([drone.dump() for drone in drones])
    socketio.emit('drone_data', data_to_send)

@socketio.on('TAKEOFF')
def takeOff(data):
    print("TAKE OFF")

@socketio.on('RETURN_BASE')
def returnToBase(data):
    print("Returning to base")  


def set_interval(func, sec):
    def func_wrapper():
        set_interval(func, sec) 
        func()  
    t = threading.Timer(sec, func_wrapper)
    t.start()
    return t

set_interval(send_data, 2)

if __name__ == '__main__':
    app.run()
