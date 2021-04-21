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
import { MapCatalogPageComponent } from "./components/page/map-catalog/map-catalog-page.component";

// Components
import { DroneListComponent } from "./components/drone/drone-list/drone-list.component";
import { DroneSelectedBoardComponent } from "./components/drone/drone-selected-board/drone-selected-board.component";
import { DroneSwarmBoardComponent } from "./components/drone/drone-swarm-board/drone-swarm-board.component";
import { MapGeneratedBoardComponent } from "./components/map/map-generated-board/map-generated-board.component";
import { NavigationBarComponent } from "./components/navigation-bar/navigation-bar.component";
import { MapComponent } from "./components/map/map/map.component";
import { MapListComponent } from "./components/map/map-list/map-list.component";
import { MapSelectedComponent } from "./components/map/map-selected/map-selected.component";

import { DronePositionComponent } from "./components/drone/drone-position/drone-position.component";

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
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatDialogModule } from '@angular/material/dialog';
import { MatStepperModule } from '@angular/material/stepper';
import { FileUploadComponent } from "./components/file-upload/file-upload.component";
import {MatSlideToggleModule} from '@angular/material/slide-toggle';
import {MatTabsModule} from '@angular/material/tabs';

@NgModule({
  declarations: [
    AppComponent,
    ControlPageComponent,
    HomePageComponent,
    MapCatalogPageComponent,
    MapGeneratedBoardComponent,
    DroneListComponent,
    DroneSelectedBoardComponent,
    DroneSwarmBoardComponent,
    MapComponent,
    MapGeneratedBoardComponent,
    NavigationBarComponent,
    MapListComponent,
    MapSelectedComponent,
    DronePositionComponent,
    FileUploadComponent,
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
    MatDialogModule,
    MatIconModule,
    MatSidenavModule,
    MatToolbarModule,
    MatGridListModule,
    MatCardModule,
    MatFormFieldModule,
    MatInputModule,
    MatRadioModule,
    FormsModule,
    ReactiveFormsModule,
    MatStepperModule,
    MatDialogModule,
    MatSlideToggleModule,
    MatTabsModule,
    FormsModule,
  ],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
