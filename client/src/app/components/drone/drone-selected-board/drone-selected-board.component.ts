import { Component } from "@angular/core";
import { Drone } from "@app/class/drone";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { SelectedDroneService } from "@app/service/selecte-drone/selected-drone.service";
@Component({
  selector: "app-drone-selected-board",
  templateUrl: "./drone-selected-board.component.html",
  styleUrls: ["./drone-selected-board.component.scss"],
})
export class DroneSelectedBoardComponent {
  constructor(public selectedDroneService: SelectedDroneService, public droneListService: DroneListService) {}

  sendToggleLedRequest(): void {
    this.selectedDroneService.sendToogleLedRequest();
  }

  sendTakeOffRequest(): void {
    this.selectedDroneService.sendTakeOffRequest();
  }
  sendReturnToBaseRequest(): void {
    console.log(this.selectedDroneService.drone.initRealPosition);
    this.selectedDroneService.sendReturnToBaseRequest();
  }

  public get drone(): Drone {
    return this.selectedDroneService.drone;
  }

  public initRealPosChange() {
    
    this.droneListService.sendInitRealPositionToServer(this.drone.getDroneId());
  }

}
