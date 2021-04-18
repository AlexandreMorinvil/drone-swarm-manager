import { TestBed } from "@angular/core/testing";
import { DroneListService } from "./drone-list.service";
import { Drone } from "@app/class/drone";
import { SocketService } from "../socket.service";
import { Vec3 } from "@app/class/vec3";

describe("DroneListService", () => {
  let service: DroneListService;
  let socketServiceSpy : jasmine.SpyObj<SocketService>;
  beforeEach(() => {
    socketServiceSpy = jasmine.createSpyObj('socketService', ['addEventHandler']);
    TestBed.configureTestingModule({
      providers : [{provide: SocketService, useValue: socketServiceSpy}],
    });
    service = TestBed.inject(DroneListService);
  });

  it("should be created", () => {
    expect(service).toBeTruthy();
  });

  it("should update the list even if a drone was delete", () => {
    service.droneList = [new Drone(2,0,30,false,new Vec3(0,0,0), new Vec3(1,1,1)), new Drone(3,0,90,true,new Vec3(6,0,0), new Vec3(1,1,1)), new Drone(4,0,54,false,new Vec3(0,6,0), new Vec3(1,1,1))];
    const newDroneListJson = [{id:3,state:0,vbat:90, isConnected:true, currentPos:new Vec3(6,0,0), currentSpeed :new Vec3(1,1,1) },{id:4,state:0,vbat:54, isConnected:false, currentPos:new Vec3(0,6,0), currentSpeed :new Vec3(1,1,1) }];
    const newDroneList = [ new Drone(3,0,90,true,new Vec3(6,0,0), new Vec3(1,1,1)), new Drone(4,0,54,false,new Vec3(0,6,0), new Vec3(1,1,1))];
    service.updateList(newDroneListJson);
    expect(service.droneList).toEqual(newDroneList);
  });

  it("should update the list when there is a new Drone", () => {
    service.droneList = [new Drone(2,0,30,false,new Vec3(0,0,0), new Vec3(1,1,1)), new Drone(3,0,90,true,new Vec3(6,0,0), new Vec3(1,1,1))];
    const newDroneListJson = [{id:2,state:0,vbat:30, isConnected:false, currentPos:new Vec3(0,0,0), currentSpeed :new Vec3(1,1,1) }, {id:3,state:0,vbat:90, isConnected:true, currentPos:new Vec3(6,0,0), currentSpeed :new Vec3(1,1,1) },{id:4,state:0,vbat:54, isConnected:false, currentPos:new Vec3(0,6,0), currentSpeed :new Vec3(1,1,1) }];
    const newDroneList = [ new Drone(2,0,30,false,new Vec3(0,0,0), new Vec3(1,1,1)), new Drone(3,0,90,true,new Vec3(6,0,0), new Vec3(1,1,1)), new Drone(4,0,54,false,new Vec3(0,6,0), new Vec3(1,1,1))];
    service.updateList(newDroneListJson);
    expect(service.droneList).toEqual(newDroneList);
  });

  it("should update the list", () => {
    service.droneList = [ new Drone(2,0,30,false,new Vec3(0,0,0), new Vec3(1,1,1)), new Drone(3,0,90,true,new Vec3(6,0,0), new Vec3(1,1,1)), new Drone(4,0,54,false,new Vec3(0,6,0), new Vec3(1,1,1))];
    const newDroneListJson = [{id:2,state:1,vbat:45, isConnected:true, currentPos:new Vec3(0,2,0), currentSpeed :new Vec3(1,1,1) }, {id:3,state:0,vbat:80, isConnected:true, currentPos:new Vec3(6,0,0), currentSpeed :new Vec3(1,1,4) },{id:4,state:0,vbat:54, isConnected:false, currentPos:new Vec3(0,6,0), currentSpeed :new Vec3(1,1,1) }];
    const newDroneList = [ new Drone(2,1,45,true,new Vec3(0,2,0), new Vec3(1,1,1)), new Drone(3,0,80,true,new Vec3(6,0,0), new Vec3(1,1,4)), new Drone(4,0,54,false,new Vec3(0,6,0), new Vec3(1,1,1))];
    service.updateList(newDroneListJson);
    expect(service.droneList).toEqual(newDroneList);
  });
});
