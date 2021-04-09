import { Injectable } from "@angular/core";
import {io, Socket} from 'socket.io-client/build/index';
import { MapCatalogService } from "./map-catalog/map-catalog.service";
import { DroneListService } from "./api/drone-list/drone-list.service";
import { LiveMapService } from "./map/live-map.service";
import { Vec3 } from "@app/class/vec3";
import { ServerMode } from "@app/constants/serverMode"
@Injectable({
  providedIn: 'root'
  })

  export class SocketService {
    private socket: Socket;
    inMission: boolean = false;
    
    constructor(public mapCatalogService: MapCatalogService, public dronelistService: DroneListService, public liveMapService: LiveMapService) {
      this.initSocket();
    }
  
    public initSocket(){
      this.socket = io("127.0.0.1:5000");
      
      this.socket.on('MAP_POINTS',(data) => {
        const points = JSON.parse(data);
        this.mapCatalogService.receiveSelectedMapPoints(points);
      });
      this.socket.on("MAP_LIST", (data) => {
        this.mapCatalogService.receiveMap(data);           
      });
      this.socket.on("drone_data", (data: any) => this.dronelistService.receiveData(data));
      this.socket.on("LIVE_MAP_NEW_POINT", (data) => {
        const pointData: any = JSON.parse(data);
        this.liveMapService.addWallPoint(new Vec3(pointData.x, pointData.y, pointData.z))
      });
  
    }

    public sendModeToServer(modeSelected: ServerMode, numberOfDrone: Number) {
      this.socket.emit("SET_MODE", {
          mode_chosen: modeSelected,
          number_of_drone: numberOfDrone });
    }

    public getMapList() {
      this.socket.emit("MAP_CATALOG");
    } 
    
    public deleteSelectedMap(mapId: Number){
      this.socket.emit("DELETE_MAP", {id: mapId});
    }

    public getSelectedMap(mapId : Number){
      this.socket.emit("SELECT_MAP", { id: mapId });
    }
    public sendToogleLedRequest(droneId: Number) {
      this.socket.emit('TOGGLE_LED', {id : droneId})
    }
    
    public sendTakeOffRequest(droneId: number): void {
      this.inMission = true;
      this.socket.emit("TAKEOFF", { id: droneId });
    }

    public sendEndOfMission(): void{
      this.socket.emit("END_MISSION")
    }

    public sendReturnToBaseRequest(droneId: number): void {
      this.socket.emit("RETURN_BASE", { id: droneId });
    }

    public receivePosition() {
      this.socket.on('POSITION', (position) => {
        console.log('position' + position);
      });
    }

    public sendEmergencyLandingRequest(droneId: number): void {
      this.socket.emit("EMERGENCY_LANDING", { id: droneId });
    }  
  }