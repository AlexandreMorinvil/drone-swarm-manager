import { Injectable } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";
@Injectable({
  providedIn: "root",
})
export class SocketService {
  private socket: Socket;

  constructor() {
    this.initSocket();
  }

  public initSocket() {
    this.socket = io(window.location.hostname + ":5000");
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

  /**
     * Use this function  from the service attached to your component to add event you want to listen to
     * @param name name of the event
     * @param func the function to be executed
     */
   public onEvent(name: string, func : Function): void {
    this.socket.on(name, func);
  }

}
