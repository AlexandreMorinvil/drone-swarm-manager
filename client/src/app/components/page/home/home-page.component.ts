import { Component } from "@angular/core";
import { FormControl, Validators } from "@angular/forms";
import { EnvironmentService } from "@app/service/api/environment/environment.service";
import { ServerMode } from "@app/constants/serverMode";
import {DronePositionComponent} from "@app/components/drone/drone-position/drone-position.component";
import { Router } from "@angular/router";
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';

@Component({
  selector: "app-home-page",
  templateUrl: "./home-page.component.html",
  styleUrls: ["./home-page.component.scss", "../page.component.scss"],
})
export class HomePageComponent {

  modeSelected: ServerMode = ServerMode.REAL;
  numberOfDrone = new FormControl(1, Validators.min(1));

  constructor(public environmentService: EnvironmentService, public route: Router, public dialog: MatDialog) { }
  
  sendModeToServer() {
    if(this.modeSelected == ServerMode.SIMULATION){
    this.environmentService.sendModeToServer(ServerMode.SIMULATION, this.numberOfDrone.value);
      this.route.navigate(['/control']);
    }
    else if(this.modeSelected == ServerMode.REAL){
      const dialogConfig = new MatDialogConfig();
      dialogConfig.data = {
        numberOfDrone: this.numberOfDrone
      };
      dialogConfig.height = '800px';
      this.dialog.open(DronePositionComponent, dialogConfig);
    }
  }

}
