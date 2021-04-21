import { Component } from "@angular/core";
import { Map } from "@app/class/map";
import { Vec3 } from "@app/class/vec3";
import { MapCatalogService } from "@app/service/api/map-catalog/map-catalog.service";

@Component({
  selector: "app-map-list",
  templateUrl: "./map-list.component.html",
  styleUrls: ["./map-list.component.scss"],
})
export class MapListComponent { 
  points : Vec3[] = []
  constructor(public mapCatalogService : MapCatalogService){
  }
  public get maps(): Map[] {
    return this.mapCatalogService.mapList;
  }

  getMapSelected(id:Number):void {
    this.mapCatalogService.getSelectedMap(id);
  }
}