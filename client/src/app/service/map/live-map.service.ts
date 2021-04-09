import { Injectable } from "@angular/core";
import { Vec3 } from "@app/class/vec3";

import { SocketService } from "../socket.service";

@Injectable({
  providedIn: "root",
})
export class LiveMapService {

  mustResetMap: boolean = true;

  wallPointsToAdd: Vec3[] = [];
  wallPoints: Vec3[] = [];
  dronePoints: Vec3[] = [];

  constructor(public socketService: SocketService) {
    this.socketService.addEventHandler("LIVE_MAP_NEW_POINT", this.receivePoint);
    this.socketService.addEventHandler("LIVE_MAP_BASE_MAP", this.receiveBaseMap);
  }

  givePointsToRender(): Vec3[] {
    const pointsToGive = this.wallPointsToAdd;
    this.wallPointsToAdd = [];
    this.wallPoints.concat(pointsToGive);
    return pointsToGive
  }

  private receivePoint(data): void {
    const pointData: any = JSON.parse(data);
    this.wallPoints.push(new Vec3(pointData.x, pointData.y, pointData.z))
  }

  private receiveBaseMap(data): void {
    const pointsData: any = JSON.parse(data);
    const baseMap = [];
    for (let point of pointsData)
      baseMap.push(new Vec3(point.x, point.y, point.z))
    this.wallPointsToAdd = baseMap;
    this.mustResetMap = true;
  }
}
