import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { SelectMapComponent } from "./select-map.component";
import {MapCatalogService} from "@app/service/map-catalog/map-catalog.service";

import SpyObj = jasmine.SpyObj;
import { Map } from "@app/class/map";

describe("SelectMapComponent", () => {
  let component: SelectMapComponent;
  let fixture: ComponentFixture<SelectMapComponent>;
  let mapCatalogServiceSpy : SpyObj<MapCatalogService>;
  beforeEach(
    waitForAsync(() => {
        mapCatalogServiceSpy = jasmine.createSpyObj('mapCatalogService',['deleteSelectedMap']);
      TestBed.configureTestingModule({
        declarations: [SelectMapComponent],
        providers: [
            { provide: MapCatalogService, useValue: mapCatalogServiceSpy }
        ],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(SelectMapComponent);
    component = fixture.componentInstance;
    

});

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it('isMapSelected should be false if no map is selected', () => {
    mapCatalogServiceSpy.mapSelected = new Map(-1, "","");
    expect(component.isMapSelected).toBeFalse();
  });

  it('isMapSelected should be true if a map is selected', () => {
    mapCatalogServiceSpy.mapSelected = new Map(4, "map4","01/04/02");
    expect(component.isMapSelected).toBeTrue();
  });

});