import { Injectable } from "@angular/core";
import { SocketService } from "@app/service/api/socket.service";
import { io, Socket } from "socket.io-client/build/index";
import { Drone, MinimalDrone } from "@app/class/drone";
import { ServerMode } from "@app/constants/serverMode";
import { Vec2, Vec3 } from "@app/class/vec3";

@Injectable({
  providedIn: "root",
})
export class DroneListService {
  droneId: number = 0;
  droneList: Drone[] = [];
  firstIndex: number = 0;
  mode: ServerMode;
  numberOfDrones: Number = 0;

  constructor(public socketService: SocketService) {
    this.socketService.addEventHandler("DRONE_LIST", (data) => { this.receiveData(data) });
  }

  public receiveData(data) {
    const droneData = JSON.parse(data);
    this.updateList(droneData);
  }

  public updateList(droneData: any): void {

    this.firstIndex = (droneData[0]) ? droneData[0].id : this.firstIndex;
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

  public getPositions(): Vec3[] {
    return this.droneList.map( (v:Drone) => v.getPosition()) 
  }
}
