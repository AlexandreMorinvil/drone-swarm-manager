import { Injectable } from "@angular/core";

@Injectable({
    providedIn: 'root'
    })

export class Drone {
    vbat: Number = 0.0
    online: Boolean = false
    droneId: Number = 0;
    constructor(batterie: Number, online:Boolean, droneId: Number ){
        this.vbat = batterie;
        this.online = online;
        this.droneId = droneId;
    }
    
}