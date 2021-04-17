import { TestBed } from "@angular/core/testing";

import { SelectedDroneService } from "./selected-drone.service";

describe("SelectedDroneService", () => {
  let service: SelectedDroneService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(SelectedDroneService);
  });

  it("should be created", () => {
    expect(service).toBeTruthy();
  });
});
