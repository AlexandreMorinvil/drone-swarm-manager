import { TestBed } from "@angular/core/testing";
import { Drone } from "@app/class/drone";
import { Vec3 } from "@app/class/vec3";
import { DroneControlService } from "../api/drone-control/drone-control.service";
import { DroneListService } from "../api/drone-list/drone-list.service";

import { SelectedDroneService } from "./selected-drone.service";

describe("SelectedDroneService", () => {
  let service: SelectedDroneService;
  let droneListServiceSpy : jasmine.SpyObj<DroneListService>;
  let droneConrtrolServiceSpy: jasmine.SpyObj<DroneControlService>;
  beforeEach(() => {
    droneListServiceSpy = jasmine.createSpyObj('droneListService', ['getDrone']);
    droneConrtrolServiceSpy = jasmine.createSpyObj('droneConrtrolService', ['sendToogleLedRequest', 'sendTakeOffRequest','sendReturnToBaseRequest'])
    TestBed.configureTestingModule({
      providers: [
        {provide: DroneControlService, useValue: droneConrtrolServiceSpy},
        {provide: DroneListService, useValue: droneListServiceSpy},
      ],
    });
    service = TestBed.inject(SelectedDroneService);
  });

  it("should be created", () => {
    expect(service).toBeTruthy();
  });

  it('setSelectedDrone should change the id of the drones and validate', () => {
    service['droneId'] = 2;
    const validateSpy = spyOn(service, 'validateDroneAvailability');
    service.setSelectedDrone(3);
    expect(service['previousDroneId']).toEqual(2);
    expect(service['droneId']).toEqual(3);
    expect(validateSpy).toHaveBeenCalled();
  });

  it('should call unsetSelectedDrone if the drone is unset', () => {
    const unsetSelectedDroneSpy = spyOn<any>(service, 'unsetSelectedDrone');
    droneListServiceSpy.droneList = [new Drone(1,0,0,false,new Vec3(0,0,0), new Vec3(1,1,1))];
    service['droneId'] = 2;
    service.validateDroneAvailability(() => {});
    expect(unsetSelectedDroneSpy).toHaveBeenCalled();
  });

  it('should call the validateDroneAvailavility with the controlService function', () => {
    const validateSpy = spyOn(service, 'validateDroneAvailability');
    service.sendTakeOffRequest();
    expect(validateSpy).toHaveBeenCalledWith(droneConrtrolServiceSpy.sendTakeOffRequest);
  });
});
