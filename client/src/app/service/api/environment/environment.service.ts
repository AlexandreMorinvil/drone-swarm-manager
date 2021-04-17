import { Injectable } from "@angular/core";
import { ServerMode } from "@app/constants/serverMode";
import { SocketService } from "@app/service/api/socket.service";


@Injectable({
  providedIn: "root",
})
export class EnvironmentService {

  inMission: boolean = false;

  constructor(public socketService: SocketService) {
  }

  public sendModeToServer(modeSelected: ServerMode, numberOfDrone: Number) {
    this.socketService.emitEvent("SET_MODE", {
      mode_chosen: modeSelected,
      number_of_drone: numberOfDrone,
    });
  }
}
