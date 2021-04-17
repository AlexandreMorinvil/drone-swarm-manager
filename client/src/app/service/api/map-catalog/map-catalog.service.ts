import { Injectable } from "@angular/core";
import { Map, UNSET_MAP_INDEX } from "@app/class/map";
import { Vec3 } from "@app/class/vec3";
import { BehaviorSubject, Observable } from "rxjs";
import { SocketService } from "../socket.service";

@Injectable({
  providedIn: "root",
})
export class MapCatalogService {
  mapList: Map[] = [];
  selectedMapPoints: Vec3[] = [];
  selectedMap: Map = new Map();

  selectedMapSource: BehaviorSubject<Map>;
  selectedMapObservable: Observable<Map>;

  constructor(public socketService: SocketService) {
    this.selectedMapSource = new BehaviorSubject<Map>(this.selectedMap);
    this.selectedMapObservable = this.selectedMapSource.asObservable();

    this.socketService.addEventHandler("MAP_LIST", (data) => {
      this.receiveMaps(data);
    });
    this.socketService.addEventHandler("MAP_POINTS", (data) => {
      this.receiveSelectedMapPoints(data);
    });
  }

  receiveMaps(data: any): void {
    const mapData = JSON.parse(data);
    this.mapList = [];
    for (let i in mapData) {
      this.mapList.push(new Map(mapData[i].id, mapData[i].name, mapData[i].date));
    }
  }

  receiveSelectedMapPoints(data: any): void {
    const pointsData = JSON.parse(data);
    const slectedMapPoints: Vec3[] = [];
    for (const point of pointsData) {
      slectedMapPoints.push(new Vec3(point.x, point.y, point.z));
    }
    this.selectedMap.points = slectedMapPoints;
    this.selectedMapSource.next(this.selectedMap);
  }

  getMapList() {
    this.socketService.emitEvent("MAP_CATALOG");
  }

  deleteSelectedMap(mapId: Number) {
    this.socketService.emitEvent("DELETE_MAP", { id: mapId });
  }

  getSelectedMap(mapId: Number) {
    this.socketService.emitEvent("SELECT_MAP", { id: mapId });
  }

  public get isMapSelected(): boolean {
    return this.selectedMap.id !== UNSET_MAP_INDEX;
  }
}
