import { Component, OnInit, ViewChild } from "@angular/core";
import { MatSidenav } from "@angular/material/sidenav";
import { MatDialog } from '@angular/material/dialog';
import { DroneListService } from "@app/service/api/drone-list/drone-list.service"

@Component({
  selector: "app-control-page",
  templateUrl: "./control-page.component.html",
  styleUrls: ["./control-page.component.scss", "../page.component.scss"],
})
export class ControlPageComponent {
  @ViewChild("droneSelectedBoard") board: MatSidenav;
  isNewSimulation: boolean = true;

  constructor(public dialog: MatDialog, public droneListService: DroneListService) {}

  /*ngOnInit(): void {
    if(this.droneListService.mode == ServerMode.REAL ){
      console.log("open dialog");
      this.dialog.open(InitRealPosComponent);
    }
  }*/

  toogleSelectedDroneBoard(isNewSelection: boolean = true): void {
    if (!this.board) return;
    if (isNewSelection) this.board.open();
    else this.board.toggle();
  }
}
