import { Component, OnInit } from "@angular/core";

import {SocketService} from "@app/service/socket.service";
import { DroneListComponent } from "../drone-list/drone-list.component";

@Component({
  selector: "app-drone-selected-board",
  templateUrl: "./drone-selected-board.component.html",
  styleUrls: ["./drone-selected-board.component.scss"],
})
export class DroneSelectedBoardComponent implements OnInit{
  constructor(private socketService: SocketService){}

  ngOnInit():void{
    this.socketService.initSocket();
   
  }
  getBatteryLevel():Number {
    return this.socketService.getBatteryLevel();
  }
  toggleLed():void {
    this.socketService.toggle_led();
  }
  refresh():void {
    this.socketService.refresh();
  }
  takeOff():void {
    this.socketService.takeOff();
    
  }
}
