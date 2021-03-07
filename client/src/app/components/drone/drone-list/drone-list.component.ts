import { Component, ViewChild } from "@angular/core";
import { MatSelectionList } from "@angular/material/list";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { ControlPageComponent } from "@app/components/page/control/control-page.component";
import { Drone, UNSET_DRONE_INDEX } from "@app/class/drone";
import { SelectedDroneService } from "@app/service/selecte-drone/selected-drone.service";

@Component({
  selector: "app-drone-list",
  templateUrl: "./drone-list.component.html",
  styleUrls: ["./drone-list.component.scss"],
})
export class DroneListComponent {
  @ViewChild("droneId") droneId: MatSelectionList;

  previousSelectedId: number = UNSET_DRONE_INDEX;

  constructor(
    public droneListService: DroneListService,
    public selectedDroneService: SelectedDroneService,
    public controlPage: ControlPageComponent
  ) {}

  toogleSelectedDroneBoard(droneId: number): void {
    if (!this.isConnected) return;
    const isSelectionValid: boolean = this.selectedDroneService.setSelectedDrone(droneId);
    const isNewSelection = this.selectedDroneService.isNewSelection;
    if (isSelectionValid) this.controlPage.toogleSelectedDroneBoard(isNewSelection);
  }

  public get isConnected(): boolean {
    return this.droneListService.isConnected;
  }

  public get connectionStatus(): string {
    if (this.droneListService.isConnected) return "CONNECTED : " + this.droneNumber;
    else return "DISCONNECTED";
  }

  public get drones(): Drone[] {
    return this.droneListService.droneList;
  }

  public get droneNumber(): number {
    return this.droneListService.getDroneNumber();
  }
}
