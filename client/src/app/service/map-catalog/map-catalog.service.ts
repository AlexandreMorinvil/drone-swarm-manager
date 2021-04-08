import { Injectable } from "@angular/core";
import {Map} from "@app/class/map";
import { LiveMapService } from "../map/live-map.service";
import { Vec3 } from "@app/class/vec3";




@Injectable({
    providedIn: "root",
  })
  export class MapCatalogService {
    map_list: Map[] = [];
    isNewSelection: boolean = false;
    selectedMap: Map = {id:-1, name:"", date:""};

    constructor(public liveMapService: LiveMapService) {
    }

    receiveMap(data: any): void {
        this.map_list = [];
        const mapData = JSON.parse(data);
        for(let i = 0; i < mapData.length; i++){
            this.map_list.push(new Map(mapData[i].id, mapData[i].name,mapData[i].date));
        }
    }

    selectMap(mapId:Number): void{
        this.map_list.forEach((data)=>{
            if(data.id == mapId) this.selectedMap = data;
        });
    }


    receiveSelectedMapPoints(pointsData:any): void{
        let points = [];
        for(let i = 0; i < pointsData.length; i++){
            points.push(new Vec3(pointsData[i].x, pointsData[i].y, pointsData[i].z));
        }
        this.liveMapService.setBaseMap(points);
    }
  }