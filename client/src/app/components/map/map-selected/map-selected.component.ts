import { Component } from "@angular/core";
import { Map } from "@app/class/map"
import { MapCatalogService } from "@app/service/api/map-catalog/map-catalog.service";

@Component({
  selector: "app-map-selected",
  templateUrl: "./map-selected.component.html",
  styleUrls: ["./map-selected.component.scss"],
})
export class MapSelectedComponent{
  constructor(public mapCatalogService: MapCatalogService){}

  deleteMap(): void {
    this.mapCatalogService.deleteSelectedMap(this.selectedMap.id);
  }

  public get selectedMap(): Map {
    return this.mapCatalogService.selectedMap;
  }

  public get isMapSelected(): Boolean {
    return this.mapCatalogService.isMapSelected;
  }
}