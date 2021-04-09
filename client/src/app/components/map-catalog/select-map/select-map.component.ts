import { NONE_TYPE } from "@angular/compiler";
import { Component } from "@angular/core";
import { Map } from "@app/class/map"
import { MapCatalogService } from "@app/service/map-catalog/map-catalog.service";
import { SocketService } from "@app/service/socket.service";



@Component({
  selector: "app-select-map",
  templateUrl: "./select-map.component.html",
  styleUrls: ["./select-map.component.scss"],
})
export class SelectMapComponent{
  constructor(public mapCatalogService: MapCatalogService, public socketService: SocketService){}

  public get selectedMap(): Map {
    // return this.mapCatalogService.selectedMap;
    return new Map(1, "2", "3");
  }

  public get isMapSelected(): Boolean {
    // return this.selectedMap.id != -1;
    return true;
  }

  deleteMap(): void {
    // this.socketService.deleteSelectedMap(this.selectedMap.id);
  }
  
}