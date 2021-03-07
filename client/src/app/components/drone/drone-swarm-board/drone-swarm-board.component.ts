import { Component } from "@angular/core";
import { DroneControlService } from "@app/service/api/drone-control/drone-control.service";
import { ALL_DRONE_INDEX } from "@app/class/drone";

@Component({
  selector: "app-drone-swarm-board",
  templateUrl: "./drone-swarm-board.component.html",
  styleUrls: ["./drone-swarm-board.component.scss"],
})
export class DroneSwarmBoardComponent {

  constructor(public droneControlService: DroneControlService) {
  }

  sendSwarmToggleLedRequest(): void {
    this.droneControlService.sendToogleLedRequest(ALL_DRONE_INDEX);
  }

  sendSwarmTakeOffRequest(): void {
    this.droneControlService.sendTakeOffRequest(ALL_DRONE_INDEX);
  }
  sendSwarmReturnToBaseRequest(): void {
    this.droneControlService.sendReturnToBaseRequest(ALL_DRONE_INDEX);
  }
}
