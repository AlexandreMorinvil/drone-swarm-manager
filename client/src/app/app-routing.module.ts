import { NgModule } from "@angular/core";
import { Routes, RouterModule } from "@angular/router";
import { ControlPageComponent } from "./components/page/control/control-page.component";
import { HomePageComponent } from "./components/page/home/home-page.component";
import { MapPageComponent } from "./components/page/map/map-page.component";

const routes: Routes = [
  { path: "", redirectTo: "/home", pathMatch: "full" },
  { path: "control", component: ControlPageComponent },
  { path: "home", component: HomePageComponent },
  { path: "map", component: MapPageComponent },
  { path: "**", redirectTo: "/home" },
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { relativeLinkResolution: "legacy" })],
  exports: [RouterModule],
})
export class AppRoutingModule {}
