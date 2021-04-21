import { Injectable } from "@angular/core";
import { Vec3 } from "@app/class/vec3";
import { SocketService } from "../socket.service";

@Injectable({
  providedIn: "root",
})
export class LiveMapService {

  mustResetRender: boolean = true;

  wallPointsToAdd: Vec3[] = [];
  wallPoints: Vec3[] = [];
  dronePoints: Vec3[] = [];

  constructor(public socketService: SocketService) {
    this.socketService.addEventHandler("LIVE_MAP_NEW_POINT", (data) => { this.receivePoint(data) } );
    this.socketService.addEventHandler("LIVE_MAP_BASE_MAP", (data) => {
      const pointsData: any = JSON.parse(data);
      this.receiveBaseMap(pointsData) });
  }

  getMustResetMap(): boolean {
    if(this.mustResetRender) {
      this.mustResetRender = false;
      return true;  
    }
    else return false;
  }

  hasNewPoint(): boolean {
    return this.wallPointsToAdd.length > 0;
  }

  givePointsToRender(): Vec3[] {
    const pointsToGive = this.wallPointsToAdd;
    this.wallPointsToAdd = [];
    this.wallPoints.push(...pointsToGive);
    return pointsToGive
  }

  private receivePoint(data): void {
    const pointData: any = JSON.parse(data);
    this.wallPointsToAdd.push(new Vec3(pointData.x, pointData.y, pointData.z))
  }

  private receiveBaseMap(data): void {
    const baseMap = [];
    for (let point of data)
      baseMap.push(new Vec3(point.x, point.y, point.z))
    this.wallPointsToAdd = baseMap;
    this.mustResetRender = true;
  }
}
