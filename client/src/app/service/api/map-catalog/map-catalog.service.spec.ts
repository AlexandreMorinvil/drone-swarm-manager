import { TestBed } from '@angular/core/testing';
import { SocketService } from '../socket.service';
import {Map} from "@app/class/map";
import { MapCatalogService } from './map-catalog.service';
import { Vec3 } from '@app/class/vec3';

describe('MapCatalogService', () => {
  let service: MapCatalogService;
  let socketServiceSpy : jasmine.SpyObj<SocketService>;
  beforeEach(() => {
    socketServiceSpy = jasmine.createSpyObj('socketService', ['addEventHandler', 'emitEvent']);
    TestBed.configureTestingModule({
      providers : [{provide: SocketService, useValue: socketServiceSpy}],
    });
    service = TestBed.inject(MapCatalogService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it("should received the list of Map from socket", () => {
    const mapsJson = [{id:0, name:"Terre",date:"02/04/1922"}, {id:1, name:"Venus",date:"09/12/2009"}, {id:2, name:"Saturne",date:"31/03/1999"}];
    const maps = [new Map(0,"Terre", "02/04/1922"), new Map(1,"Venus","09/12/2009"), new Map(2,"Saturne", "31/03/1999")];
    service.receiveMaps(mapsJson);
    expect(service.mapList).toEqual(maps);
  });

  it("should get points of a selected map from server", () => {
    const nextSpy = spyOn(service.selectedMapSource, 'next');
    const data = [{x:1,y:2,z:4},{x:2,y:3,z:4},{x:5,y:7,z:4},{x:1,y:8,z:4}];
    const points = [new Vec3(1,2,4),new Vec3(2,3,4),new Vec3(5,7,4),new Vec3(1,8,4)];
    service.receiveSelectedMapPoints(data);
    expect(service.selectedMap.points).toEqual(points);
    expect(nextSpy).toHaveBeenCalledWith(service.selectedMap);
  });
});