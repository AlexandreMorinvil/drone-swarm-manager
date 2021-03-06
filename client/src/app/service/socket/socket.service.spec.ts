import { TestBed } from '@angular/core/testing';

import { SocketService } from './socket.service';
import { Drone } from '../drone/drone.service';

describe('SocketService', () => {
  let service: SocketService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(SocketService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it('Should push into DroneList the items of droneData when Dronelist is empty', () => {
    service.droneList = [];
    console.log('hi');
    const droneData = [new Drone(10,true,4), new Drone(80,false,3)];
    service.receiveData(droneData);
    expect(service.droneList).toEqual(droneData);
  });
});