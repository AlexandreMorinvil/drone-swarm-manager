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
    const droneData = [{vbat:4, isConnected:true,id:0 }, {vbat:80, isConnected:false, id: 1}];
    const expectDroneList = [new Drone(4,true,0),new Drone(80,false,1)];
    service.receiveData(droneData);
    expect(service.droneList).toEqual(expectDroneList);
  });

  it('droneList should equal droneData',() => {
    service.droneList = [new Drone(75,true,0), new Drone(90,false,1),new Drone(5,false,2), new Drone(55, false,3)];
    const droneData = [{vbat:75, isConnected:true ,id:0 }, {vbat:90, isConnected:false, id: 1}, {vbat:55, isConnected:false, id:3}];
    const expectDroneList =  [new Drone(75,true,0), new Drone(90,false,1), new Drone(55, false,3)];
    service.receiveData(droneData);
    expect(service.droneList).toEqual(expectDroneList);
  });

  it('Should update the dronelist when something change', () => {
    service.droneList = [new Drone(75,true,0), new Drone(90,false,1),new Drone(5,false,2), new Drone(55, false,3)];
    const droneData = [{vbat:75, isConnected:true ,id:0 }, {vbat:90, isConnected:false, id: 1}, {vbat:30, isConnected:true, id: 2}, {vbat:55, isConnected:false, id:3}];
    const expectDroneList =  [new Drone(75,true,0), new Drone(90,false,1),new Drone(30,true,2), new Drone(55, false,3)];
    service.receiveData(droneData);
    expect(service.droneList).toEqual(expectDroneList);
  });
});