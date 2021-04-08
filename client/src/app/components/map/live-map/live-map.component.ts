import { AfterViewInit, Component, ViewChild } from "@angular/core";
import { Vec3 } from "@app/class/vec3";
import { LiveMapService } from "@app/service/map/live-map.service";
import { io, Socket } from "socket.io-client/build/index";

@Component({
  selector: "app-live-map",
  templateUrl: "./live-map.component.html",
  styleUrls: ["./live-map.component.scss"],
})
export class LiveMapComponent {

  private socket: Socket;

  constructor(public liveMapService: LiveMapService) {
    this.liveMapService.mapId = "#liveMap";

  }

  ngOnInit() {
    this.liveMapService.setPlot(true);
  }

  setBaseMap(): void {
    return;
  }

  addCoord(): void {}
}
