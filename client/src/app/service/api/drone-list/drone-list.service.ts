import { Injectable } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";
import { Drone, DroneState } from "@app/class/drone";
import {DroneControlService} from "@app/service/api/drone-control/drone-control.service";

@Injectable({
  providedIn: "root",
})
export class DroneListService {
  private socket: Socket;
  droneId: number = 0;
  droneList: Drone[] = [];

  constructor(public droneControlService: DroneControlService) {
    this.initSocket();
  }

  public initSocket() {
    this.socket = io("127.0.0.1:5000");
    this.socket.on("drone_data", (data: any) => this.receiveData(data));
  }

  public receiveData(data: any) {
    const droneData = JSON.parse(data);
    if(this.droneControlService.inMission){
      this.isEndOfMission(droneData);
    }
    this.updateList(droneData);
  }

  public updateList(droneData: any): void {
    for (let i = 0; i < droneData.length; i++) {
      // Parse the drone
      const currentId = droneData[i].id;
      console.log("currentId" + currentId);
      console.log("state : " + droneData[i].state);
      const updatedDrone = new Drone(
        droneData[i].id,
        droneData[i].state,
        droneData[i].vbat,
        droneData[i].isConnected,
        droneData[i].currentPos,
        droneData[i].currentSpeed
      );
      // If the drone does not exist, we add it to the drone list
      if (!this.droneList[i]) this.droneList.push(updatedDrone);
      // If the drone order does not correspond, we add the drone in the right order
      else if (this.droneList[i].getDroneId() !== currentId) this.droneList.splice(i, 1, updatedDrone);
      // Otherwise, we update the drone
      else this.droneList[currentId].updateDrone(updatedDrone);
    }
    if (this.droneList.length !== droneData.length) {
      this.droneList = this.droneList.slice(0, droneData.length);
    }
  }

  isEndOfMission(droneData: any): boolean {
    let droneEndMission = 0;
    for (let i = 0; i < droneData.length; i++) {
      if(droneData[i].state == DroneState.STANDBY || droneData[i].state == DroneState.LANDING){
        droneEndMission += 1;
      }
    }
    if(droneEndMission == droneData.length){
      this.droneControlService.sendEndOfMission();
      
    }
    else{
      return false;
    }

  }

  public getDroneNumber() {
    return this.droneList.length;
  }

  public getDrone(droneId: number) {
    return this.droneList[droneId];
  }
  
  public get isConnected() : boolean {
    return this.socket.connected;
  }
  
}
