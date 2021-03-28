import { Component } from "@angular/core";
import { Router } from "@angular/router";
import { io, Socket } from "socket.io-client/build/index";


enum ServerMode {
  REAL = 0,
  SIMULATION = 1,
}

@Component({
  selector: "app-home-page",
  templateUrl: "./home-page.component.html",
  styleUrls: ["./home-page.component.scss", "../page.component.scss"],
})
export class HomePageComponent {

  private socket: Socket;
  modeSelected: ServerMode = ServerMode.REAL;
  numberOfDrone: Number = 0;

  constructor() {
    this.socket = io("127.0.0.1:5000");
  }
  
  sendModeToServer() {
    this.socket.emit("SET_MODE", { mode_chosen: this.modeSelected, number_of_drone: this.numberOfDrone });
  }

  
}
