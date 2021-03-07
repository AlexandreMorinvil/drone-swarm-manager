import { AfterViewInit, Component, ViewChild } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";

@Component({
  selector: "app-live-map",
  templateUrl: "./live-map.component.html",
  styleUrls: ["./live-map.component.scss"],
})
export class LiveMapComponent implements AfterViewInit {
  @ViewChild("live-map") map;

  private socket: Socket;
  points: any[] = [];

  constructor() {
    this.socket = io("127.0.0.1:5000");
    this.socket.on("drone_data", (data) => {
      console.log("The data received : " + data);
      this.points.push(data.left);
      this.points.push(data.right);
      this.points.push(data.front);
      this.points.push(data.back);
      // this.drawBars();
    });
  }

  ngAfterViewInit() {
    console.log(this.map);
  }

  setBaseMap(): void {
    return;
  }

  addCoord(): void {}
}
