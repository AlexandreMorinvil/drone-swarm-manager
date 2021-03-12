import { TestBed } from "@angular/core/testing";

import { DroneListService } from "./drone-list.service";
import { Drone } from "@app/class/drone";

describe("DroneListService", () => {
  let service: DroneListService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DroneListService);
  });

  it("should be created", () => {
    expect(service).toBeTruthy();
  });
});
