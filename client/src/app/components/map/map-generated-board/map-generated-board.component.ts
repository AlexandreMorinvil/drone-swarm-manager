import { AfterViewInit, Component, ViewChild } from "@angular/core";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { LiveMapService } from "@app/service/api/map-live/live-map.service";
import { MapComponent } from "../map/map.component";

@Component({
  selector: "app-map-generated-board",
  templateUrl: "./map-generated-board.component.html",
  styleUrls: ["./map-generated-board.component.scss"],
})
export class MapGeneratedBoardComponent implements AfterViewInit {
  @ViewChild("liveMap") map: MapComponent;

  updateIntervalID: ReturnType<typeof setTimeout>;
  REFRESH_INTERVAL: number = 100;

  constructor(private liveMapService: LiveMapService, private droneListService: DroneListService) {}

  setBaseMap(): void {
    return;
  }

  updateMap(): void {
    // if (this.liveMapService.getMustResetMap()) this.map.erasePlot();
    if (this.liveMapService.hasNewPoint()) this.map.addWallPoint(this.liveMapService.givePointsToRender());
    this.map.updateDronePositions(this.droneListService.getPositions());
  }

  ngAfterViewInit() {
    // Active probing to initilize the map when the child component is ready
    const interval: ReturnType<typeof setInterval> = setInterval(() => {
      if (!this.map) return;
      this.map.deleteMap();
      this.map.setPlot(true);
      clearInterval(interval);

      // Activate the map update
      this.updateIntervalID = setInterval(() => {
        this.updateMap();
      }, this.REFRESH_INTERVAL);
    }, 200);
  }

  ngOnDestroy() {
    clearInterval(this.updateIntervalID);
  }
}
