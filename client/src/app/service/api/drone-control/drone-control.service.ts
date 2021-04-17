import { Injectable } from "@angular/core";
import { SocketService } from "@app/service/api/socket.service";

@Injectable({
  providedIn: "root",
})
export class DroneControlService {
  constructor(public socketService: SocketService) {
  }

  public sendToogleLedRequest(droneId: number): void {
    this.socketService.emitEvent("TOGGLE_LED", { id: droneId });
  }

  public sendTakeOffRequest(droneId: number): void {
    this.socketService.emitEvent("TAKEOFF", { id: droneId });
  }

  public sendEndOfMission(): void{
    this.socketService.emitEvent("END_MISSION")
  }

  public sendReturnToBaseRequest(droneId: number): void {
    this.socketService.emitEvent("RETURN_BASE", { id: droneId });
  }

  public sendLandRequest(droneId: number): void {
    this.socketService.emitEvent("LAND", { id: droneId });
  }

  public sendEmergencyLandingRequest(droneId: number): void {
    this.socketService.emitEvent("EMERGENCY_LANDING", { id: droneId });
  }  
}
