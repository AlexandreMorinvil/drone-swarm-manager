import { Component, OnInit } from "@angular/core";

import {SocketService} from "@app/service/socket.service";
import { DroneListComponent } from "../drone-list/drone-list.component";

@Component({
  selector: "app-drone-selected-board",
  templateUrl: "./drone-selected-board.component.html",
  styleUrls: ["./drone-selected-board.component.scss"],
})
export class DroneSelectedBoardComponent implements OnInit{
  droneID: number;
  constructor(public socketService: SocketService){}

  ngOnInit():void{
    this.socketService.initSocket();
   
  }

  changeDrone(droneId: number):void{
    this.droneID = droneId;
  }

}
