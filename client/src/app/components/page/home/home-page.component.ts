import { Component, ViewChild } from "@angular/core";
import { FormControl, Validators } from "@angular/forms";
import { DroneListService, ServerMode } from "@app/service/api/drone-list/drone-list.service"
import { MatSidenav } from "@angular/material/sidenav";
import { Drone } from "@app/class/drone";

@Component({
  selector: "app-home-page",
  templateUrl: "./home-page.component.html",
  styleUrls: ["./home-page.component.scss", "../page.component.scss"],
})
export class HomePageComponent {
  @ViewChild("initRealPos") sidenav: MatSidenav;

  drones: Drone[] = [];
  modeSelected: ServerMode = ServerMode.REAL;
  numberOfDrone = new FormControl(1, Validators.min(1));

  constructor(public droneListService: DroneListService) { }
  
  sendModeToServer() {
    this.droneListService.sendModeToServer(this.modeSelected, this.numberOfDrone.value);
  }

  openSidenav(): void {
    if(this.numberOfDrone.value != 0 || this.numberOfDrone.value != null){
        this.sidenav.open();
    }
  }

  closeSidenav(): void {
    this.sidenav.close();
  }

  receiveDrones(dronesList: Drone[]): Drone[] {
    return dronesList;
  }
//openchanged
}
