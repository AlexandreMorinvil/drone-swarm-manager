import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";

import { MapListComponent } from "./map-list.component";


describe("MapListComponent", () => {
  let component: MapListComponent;
  let fixture: ComponentFixture<MapListComponent>;
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [MapListComponent],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(MapListComponent);
    component = fixture.componentInstance;

});


  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

});