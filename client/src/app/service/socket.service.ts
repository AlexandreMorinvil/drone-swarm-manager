import { Injectable } from "@angular/core";
import { Observable, Subscriber } from "rxjs";
import { HttpClient } from '@angular/common/http';
import {io, Socket} from 'socket.io-client/build';
@Injectable({
  providedIn: 'root'
  })

  export class SocketService {
    droneId: number = 0;
    private socket: Socket;
    vbat: Number[] = [0.0, 0.0]
    online: Boolean[] = [false, false]

    constructor() {}
  
    public initSocket(){
      this.socket = io("127.0.0.1:5000");
      this.socket.on('drone_data', data => {
        this.vbat[data['id']] = data['vbat'];
        this.online[data['id']] = data['isConnected'];
      });
    }

    public toggle_led() {
      console.log(this.droneId);
      this.socket.emit('TOGGLE_LED', {id : this.droneId})
    }

    public refresh() {
      this.socket.emit('REFRESH', {id : this.droneId})
    }
  }