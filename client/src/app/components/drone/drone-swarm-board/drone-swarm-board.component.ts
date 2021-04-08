import { Component } from "@angular/core";
import { SocketService } from "@app/service/socket.service";
import { ALL_DRONE_INDEX } from "@app/class/drone";

@Component({
  selector: "app-drone-swarm-board",
  templateUrl: "./drone-swarm-board.component.html",
  styleUrls: ["./drone-swarm-board.component.scss"],
})
export class DroneSwarmBoardComponent {

  constructor(public socketService: SocketService) {
  }

  sendSwarmToggleLedRequest(): void {
    this.socketService.sendToogleLedRequest(ALL_DRONE_INDEX);
  }

  sendSwarmTakeOffRequest(): void {
    this.socketService.sendTakeOffRequest(ALL_DRONE_INDEX);
  }
  sendSwarmReturnToBaseRequest(): void {
    this.socketService.sendReturnToBaseRequest(ALL_DRONE_INDEX);
  }
}
