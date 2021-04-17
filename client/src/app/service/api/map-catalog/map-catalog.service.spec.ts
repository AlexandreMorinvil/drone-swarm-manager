import { TestBed } from '@angular/core/testing';

import { MapCatalogService } from './map-catalog.service';

describe('MapCatalogService', () => {
  let service: MapCatalogService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(MapCatalogService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});