import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { DroneControlService } from "@app/service/api/drone-control/drone-control.service";
import { DroneSwarmBoardComponent } from "./drone-swarm-board.component";
import { ALL_DRONE_INDEX } from "@app/class/drone";
import { MatDialog } from "@angular/material/dialog";

describe("DroneSwarmBoardComponent", () => {
  let component: DroneSwarmBoardComponent;
  let fixture: ComponentFixture<DroneSwarmBoardComponent>;
  let droneControlServiceSpy : jasmine.SpyObj<DroneControlService>;
  let dialogSpy : jasmine.SpyObj<MatDialog>;
  beforeEach(
    waitForAsync(() => {
      dialogSpy = jasmine.createSpyObj('dialog', ['open'])
      droneControlServiceSpy= jasmine.createSpyObj('droneControlService', ['sendTakeOffRequest','sendReturnToBaseRequest','sendEmergencyLandingRequest','sendLandRequest']);
      TestBed.configureTestingModule({
        declarations: [DroneSwarmBoardComponent],
        providers: [
          {provide: DroneControlService, useValue: droneControlServiceSpy},
          {provide: MatDialog, useValue: dialogSpy},
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
