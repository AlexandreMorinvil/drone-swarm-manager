import { AfterViewInit, Component, ViewChild } from "@angular/core";
import { Vec3 } from "@app/class/vec3";
import { io, Socket } from "socket.io-client/build/index";

@Component({
  selector: "app-live-map",
  templateUrl: "./live-map.component.html",
  styleUrls: ["./live-map.component.scss"],
})
export class LiveMapComponent implements AfterViewInit {
  @ViewChild("live-map") map;

  private socket: Socket;

  constructor() {
    this.socket = io("127.0.0.1:5000");
    this.socket.on("LIVE_MAP_NEW_POINT", (data) => {
      console.log("The data received : " + data);

      // const pointData: any = JSON.parse(data);

      // this.map.addWallPoint(new Vec3(data.x, data.y, data.z))
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
