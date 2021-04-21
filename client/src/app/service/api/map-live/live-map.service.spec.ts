import { TestBed } from '@angular/core/testing';
import { Vec3 } from '@app/class/vec3';
import { SocketService } from '../socket.service';

import { LiveMapService } from './live-map.service';

describe('LiveMapService', () => {
  let service: LiveMapService;
  let socketServiceSpy : jasmine.SpyObj<SocketService>;
  beforeEach(() => {
    socketServiceSpy = jasmine.createSpyObj('socketService', ['addEventHandler']);
    TestBed.configureTestingModule({
      providers : [{provide: SocketService, useValue: socketServiceSpy}],
    });
    service = TestBed.inject(LiveMapService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it('should push the wallPointsToAdd to wallPoints', () => {
    service.wallPoints = [new Vec3(0,2,0), new Vec3(2,0,0), new Vec3(0,0,2)];
    service.wallPointsToAdd =  [new Vec3(0,3,0), new Vec3(3,0,0), new Vec3(0,0,3)];
    const finalPoints = [new Vec3(0,2,0), new Vec3(2,0,0), new Vec3(0,0,2),new Vec3(0,3,0), new Vec3(3,0,0), new Vec3(0,0,3)];
    service.givePointsToRender();
    expect(service.wallPoints).toEqual(finalPoints);
    expect(service.wallPointsToAdd).toEqual([]);
  });

  it('should add the new points to wallPointsToAdd', () => {
    const receiveBaseMapSpy = spyOn<any>(service, 'receiveBaseMap').and.callThrough();
    const data = [{x:1,y:2,z:4},{x:2,y:3,z:4},{x:5,y:7,z:4},{x:1,y:8,z:4}];
    const points = [new Vec3(1,2,4),new Vec3(2,3,4),new Vec3(5,7,4),new Vec3(1,8,4)];
    receiveBaseMapSpy.call(service, data);
    expect(service.wallPointsToAdd).toEqual(points);
    expect(service.mustResetRender).toBeTrue();
    
  });
});