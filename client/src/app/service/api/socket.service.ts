import { Injectable } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";
import { DroneListService } from "./drone-list/drone-list.service";
import { ServerMode } from "@app/constants/serverMode";
@Injectable({
  providedIn: "root",
})
export class SocketService {
  private socket: Socket;

  constructor() {
    this.initSocket();
  }

  public initSocket() {
    this.socket = io("127.0.0.1:5000");
  }

  public addEventHandler(eventName: string, callbackFunction: (data: any) => void): void {
    this.socket.on(eventName, callbackFunction);
  }

  public emitEvent(eventName: string, payload: any = undefined): void {
    if (payload) this.socket.emit(eventName, payload);
    else this.socket.emit(eventName);
  }

  public get isConnected(): boolean {
    return this.socket.connected;
  }
}
