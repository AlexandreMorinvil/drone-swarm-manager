import { BrowserModule } from "@angular/platform-browser";
import { NgModule } from "@angular/core";
import { RouterModule } from "@angular/router";

import { AppRoutingModule } from "./app-routing.module";

// Page components
import { AppComponent } from "./components/app/app.component";
import { ControlPageComponent } from "./components/page/control/control-page.component";
import { HomePageComponent } from "./components/page/home/home-page.component";
import { MapPageComponent } from "./components/page/map/map-page.component";

// Components
import { DroneListComponent } from "./components/drone/drone-list/drone-list.component";
import { DroneSelectedBoardComponent } from "./components/drone/drone-selected-board/drone-selected-board.component";
import { DroneSwarmBoardComponent } from "./components/drone/drone-swarm-board/drone-swarm-board.component";
import { MapGeneratedBoardComponent } from "./components/map/map-generated-board/map-generated-board.component";
@NgModule({
  declarations: [
    AppComponent,
    ControlPageComponent,
    HomePageComponent,
    MapPageComponent,
    DroneListComponent,
    DroneSelectedBoardComponent,
    DroneSwarmBoardComponent,
    MapGeneratedBoardComponent,
  ],
  imports: [BrowserModule, AppRoutingModule, RouterModule],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
