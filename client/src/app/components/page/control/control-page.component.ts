import { Component, ViewChild } from "@angular/core";
import { MatSidenav } from "@angular/material/sidenav";

@Component({
  selector: "app-control-page",
  templateUrl: "./control-page.component.html",
  styleUrls: ["./control-page.component.scss", "../page.component.scss"],
})
export class ControlPageComponent {
  @ViewChild("droneSelectedBoard") board: MatSidenav;

  constructor() {}

  toogleSelectedDroneBoard(isNewSelection: boolean = true): void {
    if (!this.board) return;
    if (isNewSelection) this.board.open();
    else this.board.toggle();
  }
}
