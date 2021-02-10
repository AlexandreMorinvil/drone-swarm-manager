import socketio
from drone import Drone
from flask import Flask, jsonify, render_template
from flask_socketio import *


app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins='*')

# TODO REMOVE THIS
drone = Drone(-1, "ready", 100) #dummy drone
@socketio.on('TOGGLE_LED')
def ledToggler(id):
    drone.toggleLED()
if __name__ == '__main__':
    app.run()
