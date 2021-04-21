import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { DroneControlService } from "@app/service/api/drone-control/drone-control.service";
import { DroneSwarmBoardComponent } from "./drone-swarm-board.component";
import { ALL_DRONE_INDEX } from "@app/class/drone";

describe("DroneSelectedBoardComponent", () => {
  let component: DroneSwarmBoardComponent;
  let fixture: ComponentFixture<DroneSwarmBoardComponent>;
  let droneControlServiceSpy : jasmine.SpyObj<DroneControlService>;
  beforeEach(
    waitForAsync(() => {
      droneControlServiceSpy= jasmine.createSpyObj('droneControlService', ['sendTakeOffRequest','sendReturnToBaseRequest','sendEmergencyLandingRequest']);
      TestBed.configureTestingModule({
        declarations: [DroneSwarmBoardComponent],
        providers: [
          {provide: DroneControlService, useValue: droneControlServiceSpy},
        ],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneSwarmBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should call sendTakeOffRequest of droneControlService", ()=> {
    component.sendSwarmTakeOffRequest();
    expect(droneControlServiceSpy.sendTakeOffRequest).toHaveBeenCalledWith(ALL_DRONE_INDEX);
  });

  it("should call sendReturnToBaseRequest of droneControlService", ()=> {
    component.sendSwarmReturnToBaseRequest();
    expect(droneControlServiceSpy.sendReturnToBaseRequest).toHaveBeenCalledWith(ALL_DRONE_INDEX);
  });

  it("should call sendEmergencyLandingRequest of droneControlService", ()=> {
    component.sendEmmergencyLandingRequest();
    expect(droneControlServiceSpy.sendEmergencyLandingRequest).toHaveBeenCalledWith(ALL_DRONE_INDEX);
  });

  it("should call sendLandingRequest of droneControlService", ()=> {
    component.sendLandingRequest();
    expect(droneControlServiceSpy.sendLandRequest).toHaveBeenCalledWith(ALL_DRONE_INDEX);
  });
});
