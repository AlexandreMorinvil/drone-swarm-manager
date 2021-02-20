import { Component, ViewChild } from "@angular/core";
import { MatSelectionList } from "@angular/material/list";
import { SocketService } from "@app/service/socket.service";
import { ControlPageComponent } from "@app/components/page/control/control-page.component";


@Component({
  selector: "app-drone-list",
  templateUrl: "./drone-list.component.html",
  styleUrls: ["./drone-list.component.scss"],
})
export class DroneListComponent {
  id: number = 0;
  @ViewChild('droneId') droneId: MatSelectionList;
  constructor(public socket: SocketService, public sidebar: ControlPageComponent){}
  onDroneChange(): void {
    this.socket.droneId = this.droneId.selectedOptions.selected[0].value;
  }



}
