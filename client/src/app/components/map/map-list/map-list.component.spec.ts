import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { Map } from "@app/class/map";
import { MapCatalogService } from "@app/service/api/map-catalog/map-catalog.service";
import { MapListComponent } from "./map-list.component";

describe('MapSelectedComponent', () => {
    let component : MapListComponent;
    let fixture: ComponentFixture<MapListComponent>
    let mapCatalogServiceSpy : jasmine.SpyObj<MapCatalogService>;
    beforeEach(waitForAsync(() => {
      mapCatalogServiceSpy = jasmine.createSpyObj('mapCatalogService', ['getSelectedMap']);
      TestBed.configureTestingModule({
        declarations: [
            MapListComponent
        ],
        providers: [{provide: MapCatalogService, useValue: mapCatalogServiceSpy}],
      }).compileComponents();
    }));
  
  
    beforeEach(() => {
      fixture = TestBed.createComponent(MapListComponent);
      component = fixture.componentInstance;
      fixture.detectChanges();
      component.mapCatalogService.mapList = [new Map(2,"Mission","02/03/2313")];
    });
  
    it("should create the component", () => {
      expect(component).toBeTruthy();
    });
  
    it("Should get the selected map", () => {
      component.getMapSelected(2);
      expect(mapCatalogServiceSpy.getSelectedMap).toHaveBeenCalled();
    })
  });
