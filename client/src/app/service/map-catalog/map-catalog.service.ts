import { Injectable } from "@angular/core";
import { io, Socket } from "socket.io-client/build/index";
import {Map} from "@app/class/map";



@Injectable({
    providedIn: "root",
  })
  export class MapCatalogService {
    map_list: Map[] = [];
    private socket: Socket;
    isNewSelection: boolean = false;
    mapSelected: Map = {id:-1, name:"", date:""};

    constructor() {
        this.initSocket();
    }

    public initSocket() {
        this.socket = io("127.0.0.1:5000");
        this.socket.on("MAP_LIST", (data) => {
            this.map_list = [];
            const mapData = JSON.parse(data);  
            this.receiveMap(mapData);         
        });
    }

    reloadMap(): void {
        this.socket.emit("MAP_CATALOG");
    }

    receiveMap(mapData: any): void {
        for(let i = 0; i < mapData.length; i++){
            this.map_list.push(new Map(mapData[i].id, mapData[i].name,mapData[i].date));
        }
    }

    selectedMap(mapId:Number): void{
        this.socket.emit("SELECT_MAP", { id: mapId });
        this.map_list.forEach((data)=>{
            if(data.id == mapId) this.mapSelected = data;
        });
    }

    deleteSelectedMap(mapId: Number): void {
        this.socket.emit("DELETE_MAP", {id: mapId});
    }

  }