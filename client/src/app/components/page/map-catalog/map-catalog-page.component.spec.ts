import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';
import { RouterTestingModule } from '@angular/router/testing';
import { MapComponent } from '@app/components/map/map/map.component';
import { MapCatalogPageComponent } from './map-catalog-page.component';

describe('MapCatalogPageComponent', () => {
  let component : MapCatalogPageComponent;
  let fixture: ComponentFixture<MapCatalogPageComponent>;
  beforeEach(waitForAsync(() => {
    
    TestBed.configureTestingModule({
      imports: [
        RouterTestingModule
      ],
      declarations: [
        MapCatalogPageComponent, MapComponent
      ],
    }).compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(MapCatalogPageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

});
