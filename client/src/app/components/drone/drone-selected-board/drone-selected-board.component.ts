import { Component } from "@angular/core";
import { Drone } from "@app/class/drone";
import { SelectedDroneService } from "@app/service/selected-drone/selected-drone.service";
@Component({
  selector: "app-drone-selected-board",
  templateUrl: "./drone-selected-board.component.html",
  styleUrls: ["./drone-selected-board.component.scss"],
})
export class DroneSelectedBoardComponent {
  constructor(public selectedDroneService: SelectedDroneService) {}

  sendToggleLedRequest(): void {
    this.selectedDroneService.sendToogleLedRequest();
  }

  sendTakeOffRequest(): void {
    this.selectedDroneService.sendTakeOffRequest();
  }
  sendReturnToBaseRequest(): void {
    this.selectedDroneService.sendReturnToBaseRequest();
  }

  public get drone(): Drone {
    return this.selectedDroneService.drone;
  }
}
