import { Injectable } from "@angular/core";
import { Observable } from "rxjs";
import { HttpClient } from '@angular/common/http';
import {io, Socket} from 'socket.io-client/build/index';
@Injectable({
  providedIn: 'root'
  })

  export class SocketService {
    private socket: Socket;
    constructor() {}
  
  
    public initSocket(){
      this.socket = io("127.0.0.1:5000");
    }
    public toggle_led(droneID: number) {
      this.socket.emit('TOGGLE_LED', {id : "droneID"})
    }
  }