import { Injectable } from "@angular/core";
import { Drone, UNSET_DRONE_INDEX } from "@app/class/drone";
import { DroneControlService } from "../api/drone-control/drone-control.service";
import { DroneListService } from "../api/drone-list/drone-list.service";

@Injectable({
  providedIn: "root",
})
export class SelectedDroneService {
  private previousDroneId: number = UNSET_DRONE_INDEX;
  private droneId: number = UNSET_DRONE_INDEX;

  constructor(private droneListService: DroneListService, private droneControlService: DroneControlService) {}

  sendTakeOffRequest(): void {
    this.validateDroneAvailability(this.droneControlService.sendTakeOffRequest);
  }

  sendReturnToBaseRequest(): void {
    this.validateDroneAvailability(this.droneControlService.sendReturnToBaseRequest);
  }

  sendLandRequest(): void {
    this.validateDroneAvailability(this.droneControlService.sendLandRequest);
  }

  validateDroneAvailability(callback: (droneId: number) => void): boolean {
    this.droneId = this.drone.droneId;
    if (this.droneId !== UNSET_DRONE_INDEX) {
      callback.call(this.droneControlService, this.droneId);
      return true;
    } else {
      this.unsetSelectedDrone();
      return false;
    }
  }

  setSelectedDrone(droneId: number): boolean {
    this.previousDroneId = this.droneId;
    this.droneId = droneId;
    return this.validateDroneAvailability(() => {});
  }

  get drone(): Drone {
    return this.droneListService.getDrone(this.droneId) || new Drone(UNSET_DRONE_INDEX);
  }

  get isNewSelection(): boolean {
    return this.previousDroneId !== this.droneId;
  }

  private unsetSelectedDrone(): void {
    this.droneId = UNSET_DRONE_INDEX;
  }
}
