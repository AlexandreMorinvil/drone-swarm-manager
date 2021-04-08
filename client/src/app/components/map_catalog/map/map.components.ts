import { Component } from "@angular/core";
import { LiveMapService } from "@app/service/map/live-map.service";

@Component({
    selector: "app-map",
    templateUrl: "./map.component.html",
    styleUrls: ["./map.component.scss"],
})
  
export class MapComponent {
    constructor(public liveMapService: LiveMapService){
        this.liveMapService.mapId = "#mapCatalog";
    }

    ngOnInit() {
        this.liveMapService.setPlot(true);
    }
}  
