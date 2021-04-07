import { Injectable } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";

@Injectable({
  providedIn: "root",
})
export class DroneControlService {
  private socket: Socket;

  constructor() {
    this.initSocket();
  }

  public initSocket() {
    this.socket = io("127.0.0.1:5000");
  }

  public sendToogleLedRequest(droneId: number): void {
    this.socket.emit("TOGGLE_LED", { id: droneId });
  }

  public sendTakeOffRequest(droneId: number): void {
    this.socket.emit("TAKEOFF", { id: droneId });
  }

  public sendReturnToBaseRequest(droneId: number): void {
    this.socket.emit("RETURN_BASE", { id: droneId });
  }

  public sendEmergencyLandingRequest(droneId: number): void {
    this.socket.emit("EMERGENCY_LANDING", { id: droneId });
  }
}
