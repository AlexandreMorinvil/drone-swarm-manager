
import socketio
import json
from vec3 import Vec3
from drone import Drone
from flask import Flask, jsonify, render_template
from flask_socketio import *
import cflib
from cflib.crazyflie import Crazyflie


app = Flask(__name__)
socketio = SocketIO(app, host= '192.168.0.173' ,cors_allowed_origins='*')

# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
print('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
print('Crazyflies found:')
drones = [Drone("radio://0/80/250K",Vec3(0,0,0)), Drone("radio://0/71/250K", Vec3(0,0,0))]

@socketio.on('TOGGLE_LED')
def ledToggler(data):
    print(data['id'])
    drones[data['id']].toggleLED()
    print("LED TOGGLER")
@socketio.on('REFRESH')
def ledToggler(data):
    data_to_send = {
        'id' : data['id'],
        'vbat' : drones[data['id']].getVBat(),
        'isConnected' : drones[data['id']].getIsConnected()
    }
    socketio.emit('drone_data', data_to_send)
    

if __name__ == '__main__':
    app.run()
