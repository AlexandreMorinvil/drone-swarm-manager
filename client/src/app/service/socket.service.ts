import { Injectable } from "@angular/core";
import {io, Socket} from 'socket.io-client/build/index';
import { DroneListService } from "./api/drone-list/drone-list.service";
import { ServerMode } from "@app/constants/serverMode"
@Injectable({
  providedIn: 'root'
  })

  export class SocketService {
    private socket: Socket;
    inMission: boolean = false;
    
    constructor(public dronelistService: DroneListService) {
      this.initSocket();
    }

    addEventHandler(eventName: string, callbackFunction: (data: any) => void) : void {
      this.socket.on(eventName, callbackFunction);
    }

    emitEvent(eventName: string, payload: any=undefined): void {
      if (payload) this.socket.emit(eventName, payload);
      else this.socket.emit(eventName);
    }
  
    public initSocket(){
      this.socket = io("127.0.0.1:5000");
      this.socket.on("drone_data", (data: any) => this.dronelistService.receiveData(data));
    }

    public sendModeToServer(modeSelected: ServerMode, numberOfDrone: Number) {
      this.socket.emit("SET_MODE", {
          mode_chosen: modeSelected,
          number_of_drone: numberOfDrone 
      });
    }


    public sendToogleLedRequest(droneId: Number) {
      this.socket.emit('TOGGLE_LED', {id : droneId})
    }
    
    public sendTakeOffRequest(droneId: number): void {
      this.inMission = true;
      this.socket.emit("TAKEOFF", { id: droneId });
    }

    public sendEndOfMission(): void{
      this.socket.emit("END_MISSION")
    }

    public sendReturnToBaseRequest(droneId: number): void {
      this.socket.emit("RETURN_BASE", { id: droneId });
    }

    public receivePosition() {
      this.socket.on('POSITION', (position) => {
        console.log('position' + position);
      });
    }
  }