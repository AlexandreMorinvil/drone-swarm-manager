import { formatNumber } from "@angular/common";
import { Injectable } from "@angular/core";
import {io, Socket} from 'socket.io-client/build/index';
import { Drone } from "./Drone/drone.service";

@Injectable({
  providedIn: 'root'
  })

  export class SocketService {
    private socket: Socket;
    vbat: Number[] = [0.0, 0.0]
    online: Boolean[] = [false, false]
    droneId: number = 0;
    droneList: Drone[] = []
    constructor() {}
  
    public initSocket(){
      this.socket = io("127.0.0.1:5000");
      this.socket.on('drone_data', data => {
        const droneData = JSON.parse(data);
        this.receiveData(droneData);
        console.log(this.droneList);
      });
    }

    public receiveData(droneData: any): void {
      for(let i = 0; i < droneData.length; i++){
        if(this.droneList[i] == null){
          this.droneList.push(new Drone(droneData[i].vbat, droneData[i].isConnected, droneData[i].id));
        }
        else if(this.droneList[i].droneId != droneData[i].id){
          this.droneList.splice(i, 1, new Drone(droneData[i].vbat, droneData[i].isConnected, droneData[i].id));
        }
        else{
          this.droneList[droneData[i].id].vbat = droneData[i].vbat;
          this.droneList[droneData[i].id].online = droneData[i].isConnected;
        }
        this.vbat[droneData[i].id] = droneData[i].vbat;
        this.online[droneData[i].id] = droneData[i].isConnected;
      }
      if(this.droneList.length != droneData.length){
        this.droneList = this.droneList.slice(0, droneData.length);
      }
    }

    public getBatteryLevel(): Number {
      return this.vbat[this.droneId];
    }
    public toggle_led() {

      this.socket.emit('TOGGLE_LED', {id : this.droneId})
    }
    public refresh() {
      this.socket.emit('REFRESH', {id : this.droneId})
      console.log(this.droneList);
    }

    public takeOff() {
      this.socket.emit('TAKEOFF', {id : this.droneId})
    }

    public returnToBase() {
      this.socket.emit('RETURN_BASE', {id: this.droneId});
    }


  }