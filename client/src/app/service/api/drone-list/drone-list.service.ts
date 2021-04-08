import { Injectable } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";
import { Drone } from "@app/class/drone";
import { ServerMode } from "@app/constants/serverMode";

@Injectable({
  providedIn: "root",
})
export class DroneListService {
  private socket: Socket;
  droneId: number = 0;
  droneList: Drone[] = [];
  firstIndex: number = 0;

  constructor() {
    this.socket = io("127.0.0.1:5000");
  }

  public receiveData(data) {
    const droneData = JSON.parse(data);
    this.updateList(droneData);
  }

  public updateList(droneData: any): void {

    this.firstIndex = droneData[0].id;
    for (let i = 0; i < droneData.length; i++) {
      // Parse the drone
      const currentId = droneData[i].id;
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
      else this.droneList[currentId - this.firstIndex].updateDrone(updatedDrone);
    }


    if (this.droneList.length !== droneData.length) {
      this.droneList = this.droneList.slice(0, droneData.length);
    }
  }

  public getDroneNumber() {
    return this.droneList.length;
  }

  public getDrone(droneId: number) {
    return this.droneList[droneId - this.firstIndex];
  }

  public get isConnected(): boolean {
    return this.socket.connected;
  }

  public sendModeToServer(modeSelected: ServerMode, numberOfDrone: Number) {
    this.socket.emit("SET_MODE", {
      mode_chosen: modeSelected,
      number_of_drone: numberOfDrone,
    });
  }
}
