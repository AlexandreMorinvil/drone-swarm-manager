import { Component, ElementRef, OnInit, ViewChild } from "@angular/core";
import { MatSelectionList } from "@angular/material/list";
import * as EventEmitter from "events";

@Component({
  selector: "app-drone-list",
  templateUrl: "./drone-list.component.html",
  styleUrls: ["./drone-list.component.scss"],
})
export class DroneListComponent {
  @ViewChild('droneId', { static: false }) droneId: MatSelectionList;
  onDroneChange(): void {
    console.log("hi");
  }



}
