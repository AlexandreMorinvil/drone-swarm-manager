import { Component } from "@angular/core";
import { LiveMapService } from "@app/service/map/live-map.service";

@Component({
  selector: "app-map-generated-board",
  templateUrl: "./map-generated-board.component.html",
  styleUrls: ["./map-generated-board.component.scss"],
})
export class MapGeneratedBoardComponent {
  constructor(public liveMapService: LiveMapService) {
  }

  setBaseMap(): void {
    return;
  }

  addCoord(): void {}
}
