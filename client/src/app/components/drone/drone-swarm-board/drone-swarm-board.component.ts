import { Component } from "@angular/core";
import { ALL_DRONE_INDEX } from "@app/class/drone";
import { DroneControlService } from "@app/service/api/drone-control/drone-control.service";
import {MatDialog, MatDialogModule, MatDialogRef} from "@angular/material/dialog"
import { FileUploadComponent } from "@app/components/file-upload/file-upload.component";

@Component({
  selector: "app-drone-swarm-board",
  templateUrl: "./drone-swarm-board.component.html",
  styleUrls: ["./drone-swarm-board.component.scss"],
})
export class DroneSwarmBoardComponent {

  constructor(public droneControlService: DroneControlService, public dialog: MatDialog) {
  }

  sendSwarmTakeOffRequest(): void {
    this.droneControlService.sendTakeOffRequest(ALL_DRONE_INDEX);
  }

  sendSwarmReturnToBaseRequest(): void {
    this.droneControlService.sendReturnToBaseRequest(ALL_DRONE_INDEX);
  }

  sendEmmergencyLandingRequest(): void {
    this.droneControlService.sendEmergencyLandingRequest(ALL_DRONE_INDEX);
  }

  sendLandingRequest(): void {
    this.droneControlService.sendLandRequest(ALL_DRONE_INDEX);
  }
  
  openUpdateDialog(): void {
    const dialogRef = this.dialog.open(FileUploadComponent, {
      backdropClass: 'bdrop'
    });
  }

}
