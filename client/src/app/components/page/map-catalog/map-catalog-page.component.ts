import { AfterViewInit, Component, OnInit, ViewChild } from "@angular/core";
import { MatSidenav } from "@angular/material/sidenav";
import { MapComponent } from "@app/components/map/map/map.component";
import { MapCatalogService } from "@app/service/map-catalog/map-catalog.service";
import { SocketService } from "@app/service/socket.service";

@Component({
  selector: "app-map-catalog-page",
  templateUrl: "./map-catalog-page.component.html",
  styleUrls: ["./map-catalog-page.component.scss", "../page.component.scss"],
})
export class MapCatalogPageComponent implements OnInit, AfterViewInit {
  @ViewChild("selectedMap") map: MapComponent;

  constructor(public socketService: SocketService) {}

  ngOnInit() {
    this.socketService.getMapList();
  }

  ngAfterViewInit() {
    // Active probing to initilize the map when the child component is ready
    const interval: ReturnType<typeof setInterval> = setInterval(() => {
      if (this.map) {
        this.map.deleteMap();
        this.map.setPlot(true);
        clearInterval(interval);
      }
    }, 100);
  }
}
