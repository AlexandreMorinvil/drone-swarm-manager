import { Injectable } from "@angular/core";
import { MinimalDrone } from "@app/class/drone";
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

  public sendInitialPosition(positions: MinimalDrone[]) {
    let positionJson = [];
    for(let position of positions){
      positionJson.push({
        "id": position.id,
        "address": position.address,
        "x": position.initRealPos.x,
        "y": position.initRealPos.y
      })
    }

    this.socketService.emitEvent("INITIAL_POSITION", positionJson);
  }
}
