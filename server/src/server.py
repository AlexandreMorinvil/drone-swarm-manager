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
from subprocess import call

class Mode(Enum):
    REAL = 0
    SIMULATION = 1


app = Flask(__name__)
socketio = SocketIO(app ,cors_allowed_origins='*')

default_port = 5015

# Select mode
mode = Mode.SIMULATION

# Initialize the low-level drivers (don't list the debug drivers)
cflib.crtp.init_drivers(enable_debug_driver=False)
# Scan for Crazyflies and use the first one found
print('Scanning interfaces for Crazyflies...')
available = cflib.crtp.scan_interfaces()
print('Crazyflies found:')

if (mode == Mode.REAL):
    drones = [Drone("radio://0/80/250K",Vec3(0,0,0),0), Drone("radio://0/72/250K",Vec3(0,0,0),1)]
else:
    drones = []

socks = []

def createDrones(numberOfDrone):
    for i in range(numberOfDrone):
        socks.append(ArgosServer(i, default_port + i))
        t = threading.Thread(target=socks[i].waiting_connection, name='waiting_connection')
        t.start()

        if mode == Mode.SIMULATION:
            drones.append(socks[i].drone_argos)

def deleteDrones():
    for i in socks:
        if hasattr(i, "connection"):
            i.connection.close()
        i.sock.shutdown(2)
        i.sock.close()
        del i
    drones.clear()
    socks.clear()



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
            
    call(['./start-simulation.sh'])


@socketio.on('TOGGLE_LED')
def ledToggler(data):
    print(data['id'])
    drones[data['id']].toggleLED()
    print("LED TOGGLER")

@socketio.on('TAKEOFF')
def takeOff(data):
    if (data['id'] == -2):
        for i in socks:
            i.send_data(StateMode.TAKE_OFF.value, "<i")
    else:
        socks[data['id']].send_data(StateMode.TAKE_OFF.value, "<i")
    
@socketio.on('RETURN_BASE')
def returnToBase(data):
    if (data['id'] == -2):
        for i in socks:
            i.send_data(StateMode.RETURN_TO_BASE.value, "<i")
    else:
        socks[data['id']].send_data(StateMode.RETURN_TO_BASE.value, "<i")

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
    createDrones(4)
    #set_interval(sendPosition, 1)
    set_interval(send_data, 1)
    app.run()

    while True:
        for i in range(numberOfDrone):
            if (socks[i].data_received != None):
                socks[i].start_receive_data()
