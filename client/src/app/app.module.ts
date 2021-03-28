import { BrowserModule } from "@angular/platform-browser";
import { NgModule } from "@angular/core";
import { RouterModule } from "@angular/router";
import { HttpClientModule } from "@angular/common/http";
import { BrowserAnimationsModule } from "@angular/platform-browser/animations";
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
import { MapComponent } from "./components/map/map/map.component";
import { MapGeneratedBoardComponent } from "./components/map/map-generated-board/map-generated-board.component";
import { NavigationBarComponent } from "./components/navigation-bar/navigation-bar.component";

//Service
import { DroneListService } from "./service/api/drone-list/drone-list.service";

//angular/materials
import { MatDividerModule } from "@angular/material/divider";
import { MatListModule } from "@angular/material/list";
import { MatButtonModule } from "@angular/material/button";
import { MatIconModule } from "@angular/material/icon";
import { MatSidenavModule } from "@angular/material/sidenav";
import { MatToolbarModule } from "@angular/material/toolbar";
import { MatGridListModule } from "@angular/material/grid-list";
import { MatCardModule } from "@angular/material/card";
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatInputModule } from '@angular/material/input';
import { MatRadioModule } from '@angular/material/radio';
import { FormsModule } from '@angular/forms';


@NgModule({
  declarations: [
    AppComponent,
    ControlPageComponent,
    HomePageComponent,
    MapPageComponent,
    MapGeneratedBoardComponent,
    DroneListComponent,
    DroneSelectedBoardComponent,
    DroneSwarmBoardComponent,
    MapComponent,
    MapGeneratedBoardComponent,
    NavigationBarComponent,
  ],
  imports: [
    BrowserModule,
    BrowserAnimationsModule,
    AppRoutingModule,
    RouterModule,
    MatDividerModule,
    MatListModule,
    MatButtonModule,
    HttpClientModule,
    MatIconModule,
    MatSidenavModule,
    MatToolbarModule,
    MatGridListModule,
    MatCardModule,
    MatFormFieldModule,
    MatInputModule,
    MatRadioModule,
    FormsModule,
  ],
  providers: [DroneListService],
  bootstrap: [AppComponent],
})
export class AppModule {}
