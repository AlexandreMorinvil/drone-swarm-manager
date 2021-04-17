import { AfterViewInit, Component, OnInit, ViewChild } from "@angular/core";
import { MatSidenav } from "@angular/material/sidenav";
import { MapComponent } from "@app/components/map/map/map.component";
import { MapCatalogService } from "@app/service/api/map-catalog/map-catalog.service";
import { SocketService } from "@app/service/api/socket.service";

@Component({
  selector: "app-map-catalog-page",
  templateUrl: "./map-catalog-page.component.html",
  styleUrls: ["./map-catalog-page.component.scss", "../page.component.scss"],
})
export class MapCatalogPageComponent implements OnInit, AfterViewInit {
  @ViewChild("selectedMap") map: MapComponent;

  constructor(public mapCatalogService: MapCatalogService) {}

  ngOnInit(): void {
    this.mapCatalogService.getMapList();
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

    this.mapCatalogService.selectedMapSource.subscribe((selectedMap) => {
      this.map.setBaseMap(selectedMap.points);
    });
  }
}
