import { NONE_TYPE } from "@angular/compiler";
import { Component } from "@angular/core";
import { Map } from "@app/class/map"
import { MapCatalogService } from "@app/service/map-catalog/map-catalog.service";



@Component({
  selector: "app-select-map",
  templateUrl: "./select-map.component.html",
  styleUrls: ["./select-map.component.scss"],
})
export class SelectMapComponent{
  constructor(public mapCatalogService: MapCatalogService){}

  public get selectedMap(): Map {
    return this.mapCatalogService.selectedMap;
  }

  deleteMap(): void {
    this.mapCatalogService.deleteSelectedMap(this.selectedMap.id);
  }
  
}