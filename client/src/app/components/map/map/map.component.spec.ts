import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { Vec3 } from "@app/class/vec3";
import { MapComponent } from "./map.component";
import * as d3 from "d3";

describe("MapComponent", () => {
  let component: MapComponent;
  let fixture: ComponentFixture<MapComponent>;
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [MapComponent],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(MapComponent);
    component = fixture.componentInstance;

    component.chart = d3;
});


  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should get points of a selected map from server", () => {
    const setBaseMapSpy = spyOn(component, 'setBaseMap');
    const data = [{x:1,y:2,z:4},{x:2,y:3,z:4},{x:5,y:7,z:4},{x:1,y:8,z:4}];
    const points = [new Vec3(1,2,4),new Vec3(2,3,4),new Vec3(5,7,4),new Vec3(1,8,4)];
    component.receiveSelectedMapPoints(data);
    expect(setBaseMapSpy).toHaveBeenCalledOnceWith(points);

  });
});
