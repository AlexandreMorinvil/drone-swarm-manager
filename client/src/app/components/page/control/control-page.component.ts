import { Component, ViewChild } from "@angular/core";
import { MatSidenav } from "@angular/material/sidenav";

@Component({
  selector: "app-control-page",
  templateUrl: "./control-page.component.html",
  styleUrls: ["./control-page.component.scss", "../page.component.scss"],
})
export class ControlPageComponent {
  @ViewChild('board') board: MatSidenav;
}
