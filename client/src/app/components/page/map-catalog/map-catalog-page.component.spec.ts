import { TestBed, waitForAsync } from '@angular/core/testing';
import { RouterTestingModule } from '@angular/router/testing';
import { MapCatalogPageComponent } from './map-catalog-page.component';

describe('MapCatalogPageComponent', () => {
  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      imports: [
        RouterTestingModule
      ],
      declarations: [
        MapCatalogPageComponent
      ],
    }).compileComponents();
  }));

  it('should create the app', () => {
    const fixture = TestBed.createComponent(MapCatalogPageComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
