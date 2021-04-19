import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';
import { RouterTestingModule } from '@angular/router/testing';
import { Map } from '@app/class/map';
import { MapCatalogService } from '@app/service/api/map-catalog/map-catalog.service';
import { MapSelectedComponent } from './map-selected.component';

describe('MapSelectedComponent', () => {
  let component : MapSelectedComponent;
  let fixture: ComponentFixture<MapSelectedComponent>
  let mapCatalogServiceSpy : jasmine.SpyObj<MapCatalogService>;
  beforeEach(waitForAsync(() => {
    mapCatalogServiceSpy = jasmine.createSpyObj('mapCatalogService', ['deleteSelectedMap']);
    TestBed.configureTestingModule({
      imports: [
        RouterTestingModule
      ],
      declarations: [
        MapSelectedComponent
      ],
      providers: [{provide: MapCatalogService, useValue: mapCatalogServiceSpy}],
    }).compileComponents();
  }));


  beforeEach(() => {
    fixture = TestBed.createComponent(MapSelectedComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    component.mapCatalogService.selectedMap = new Map(2,"mission","02/09/2134");
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("Should delete the selected map", () => {
    component.deleteMap();
    expect(mapCatalogServiceSpy.deleteSelectedMap).toHaveBeenCalled();
  })
});
