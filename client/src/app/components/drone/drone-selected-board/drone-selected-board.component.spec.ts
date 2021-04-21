import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { MatDialog } from "@angular/material/dialog";
import { DroneControlService } from "@app/service/api/drone-control/drone-control.service";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { SelectedDroneService } from "@app/service/selected-drone/selected-drone.service";
import { DroneSelectedBoardComponent } from "./drone-selected-board.component";

describe("DroneSelectedBoardComponent", () => {
  let component: DroneSelectedBoardComponent  ;
  let fixture: ComponentFixture<DroneSelectedBoardComponent>;
  let selectedDroneServiceStub: SelectedDroneService;
  let droneListSpy : jasmine.SpyObj<DroneListService>;
  let droneControlServiceStub : DroneControlService = {} as DroneControlService;
  beforeEach(
    waitForAsync(() => {
      droneListSpy = jasmine.createSpyObj('droneListService', ['getDrone']);
      selectedDroneServiceStub = new SelectedDroneService(droneListSpy, droneControlServiceStub);
      TestBed.configureTestingModule({
        declarations: [DroneSelectedBoardComponent],
        providers: [
          {provide: SelectedDroneService, useValue: selectedDroneServiceStub},
        ],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneSelectedBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    component.selectedDroneService = selectedDroneServiceStub;
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should call sendTakeOffRequest of selectedDroneService", () => {
    const sendTakeOffRequestSpy = spyOn(selectedDroneServiceStub, 'sendTakeOffRequest');
    component.sendTakeOffRequest();
    expect(sendTakeOffRequestSpy).toHaveBeenCalled();
  });

  it("should call sendReturnToBaseRequest of selectedDroneService", () => {
    const sendReturnToBaseRequestSpy = spyOn(selectedDroneServiceStub, 'sendReturnToBaseRequest');
    component.sendReturnToBaseRequest();
    expect(sendReturnToBaseRequestSpy).toHaveBeenCalled();
  });
});
