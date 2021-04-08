import { Component } from "@angular/core";
import { FormControl, Validators } from "@angular/forms";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { ServerMode } from "@app/constants/serverMode";

@Component({
  selector: "app-home-page",
  templateUrl: "./home-page.component.html",
  styleUrls: ["./home-page.component.scss", "../page.component.scss"],
})
export class HomePageComponent {

  modeSelected: ServerMode = ServerMode.REAL;
  numberOfDrone = new FormControl(1, Validators.min(1));

  constructor(public droneListService: DroneListService) { }
  
  sendModeToServer() {
    this.droneListService.sendModeToServer(this.modeSelected, this.numberOfDrone.value);
  }

}
