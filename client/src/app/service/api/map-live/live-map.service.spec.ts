import { TestBed } from '@angular/core/testing';

import { LiveMapService } from './live-map.service';

describe('LiveMapService', () => {
  let service: LiveMapService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(LiveMapService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});