import { Component, ViewChild } from "@angular/core";
import { MatSelectionList } from "@angular/material/list";
import { SocketService } from "../../../service/socket/socket.service";
import { ControlPageComponent } from "../../page/control/control-page.component";
import {Drone} from "../../../service/drone/drone.service";


@Component({
  selector: "app-drone-list",
  templateUrl: "./drone-list.component.html",
  styleUrls: ["./drone-list.component.scss"],
})
export class DroneListComponent {
  id: number = 0;
  drones: Drone[];
  @ViewChild('droneId') droneId: MatSelectionList;
  constructor(public socket: SocketService, public sidebar: ControlPageComponent){
    this.drones = socket.droneList;
  }
  onDroneChange(): void {
    this.socket.droneId = this.droneId.selectedOptions.selected[0].value;
  }



}
