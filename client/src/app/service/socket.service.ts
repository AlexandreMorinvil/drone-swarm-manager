import { Injectable } from "@angular/core";
import { Observable, Subscriber } from "rxjs";
import { HttpClient } from '@angular/common/http';
import {io, Socket} from 'socket.io-client/build/index';
@Injectable({
  providedIn: 'root'
  })

  export class SocketService {
    private socket: Socket;
    vbat: Number[] = [0.0, 0.0]
    online: Boolean[] = [false, false]
    droneId: number = 0;
    constructor() {}
  
    public initSocket(){
      this.socket = io("127.0.0.1:5000");
      this.socket.on('drone_data', data => {
        this.vbat[data['id']] = data['vbat'];
        this.online[data['id']] = data['isConnected'];
      });
    }
    public getBatteryLevel(): Number {
      return this.vbat[this.droneId];
    }
    public toggle_led() {

      this.socket.emit('TOGGLE_LED', {id : this.droneId})
    }
    public refresh() {
      this.socket.emit('REFRESH', {id : this.droneId})
    }

    public takeOff() {
      this.socket.emit('TAKEOFF', {id : this.droneId})
    }

    public returnToBase() {
      this.socket.emit('RETURN_BASE', {id: this.droneId});
    }
  }