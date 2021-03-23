import { Component } from "@angular/core";
import { Map } from "@app/class/map";
import { MapCatalogService } from "@app/service/map-catalog/map-catalog.service";


@Component({
  selector: "app-map-list",
  templateUrl: "./map-list.component.html",
  styleUrls: ["./map-list.component.scss"],
})
export class MapListComponent {
  constructor(public mapCatalogService : MapCatalogService){}
  public get maps(): Map[] {
    return this.mapCatalogService.map_list;
  }
}