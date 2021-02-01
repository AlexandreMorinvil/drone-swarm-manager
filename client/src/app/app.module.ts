import { BrowserModule } from "@angular/platform-browser";
import { NgModule } from "@angular/core";
import { RouterModule } from "@angular/router";

import { AppRoutingModule } from "./app-routing.module";

import { AppComponent } from "./components/app/app.component";
import { ControlPageComponent } from "./components/page/control/control-page.component";
import { HomePageComponent } from "./components/page/home/home-page.component";
import { MapPageComponent } from "./components/page/map/map-page.component";

@NgModule({
  declarations: [
    AppComponent,
    ControlPageComponent,
    HomePageComponent,
    MapPageComponent,
  ],
  imports: [BrowserModule, AppRoutingModule, RouterModule],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
