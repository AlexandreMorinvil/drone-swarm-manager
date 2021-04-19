import { Injectable } from "@angular/core";
import { DroneState } from "@app/class/drone";
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
    this.socketService.emitEvent("SWITCH_STATE",
      { id: droneId,  state: DroneState.TAKE_OFF});
  }

  public sendEndOfMission(): void{
    this.socketService.emitEvent("END_MISSION")
  }

  public sendReturnToBaseRequest(droneId: number): void {
    this.socketService.emitEvent("SWITCH_STATE",
      { id: droneId,  state: DroneState.RETURN_TO_BASE});
  }

  public sendLandRequest(droneId: number): void {
    this.socketService.emitEvent("SWITCH_STATE",
      { id: droneId,  state: DroneState.LANDING});
  }

  public sendEmergencyLandingRequest(droneId: number): void {
    this.socketService.emitEvent("SWITCH_STATE",
      { id: droneId,  state: DroneState.EMERGENCY});
  }  
}
