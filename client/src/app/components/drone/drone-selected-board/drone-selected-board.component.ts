import { Component, OnInit } from "@angular/core";

import {SocketService} from "@app/service/socket.service";

@Component({
  selector: "app-drone-selected-board",
  templateUrl: "./drone-selected-board.component.html",
  styleUrls: ["./drone-selected-board.component.scss"],
})
export class DroneSelectedBoardComponent implements OnInit{
  id: number;
  constructor(public socketService: SocketService){}

  ngOnInit():void{
    this.socketService.initSocket();
  }
}
