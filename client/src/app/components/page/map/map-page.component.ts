import { Component, OnInit, ViewChild } from "@angular/core";
import { MatSidenav } from "@angular/material/sidenav";
import { MapCatalogService } from "@app/service/map-catalog/map-catalog.service";
import { SocketService } from "@app/service/socket.service";

@Component({
  selector: "app-map-page",
  templateUrl: "./map-page.component.html",
  styleUrls: ["./map-page.component.scss", "../page.component.scss"],
})
export class MapPageComponent implements OnInit{
  constructor(public socketService : SocketService){

  }
  ngOnInit(){
    this.socketService.getMapList();
  }



}
