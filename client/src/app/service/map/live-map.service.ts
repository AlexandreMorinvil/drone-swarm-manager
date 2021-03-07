import { Injectable } from "@angular/core";
import { Observable, Subscriber } from "rxjs";
import { HttpClient } from "@angular/common/http";
import { io, Socket } from "socket.io-client/build/index";
@Injectable({
  providedIn: "root",
})
export class LiveMapService {
  private socket: Socket;
  vbat: number[] = [0.0, 0.0];
  online: boolean[] = [false, false];
  droneId: number = 0;
  constructor() {}

  public initSocket() {
    this.socket = io("127.0.0.1:5000");
    this.socket.on("MAP_BASE", (data) => {
      this.vbat[data["id"]] = data["vbat"];
    });
  }

  public toggle_led() {
    this.socket.emit("LIVE_MAP_BASE");
  }
}
