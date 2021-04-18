import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { DroneControlService } from "@app/service/api/drone-control/drone-control.service";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { SelectedDroneService } from "@app/service/selected-drone/selected-drone.service";
import { DroneSelectedBoardComponent } from "./drone-selected-board.component";

describe("DroneSelectedBoardComponent", () => {
  let component: DroneSelectedBoardComponent  ;
  let fixture: ComponentFixture<DroneSelectedBoardComponent>;
  let selectedDroneServiceSpy: SelectedDroneService;
  let droneListSpy : jasmine.SpyObj<DroneListService>;
  let droneControlServiceStub : DroneControlService = {} as DroneControlService;
  beforeEach(
    waitForAsync(() => {
      droneListSpy = jasmine.createSpyObj('droneListService', ['getDrone']);
      selectedDroneServiceSpy = new SelectedDroneService(droneListSpy, droneControlServiceStub);
      TestBed.configureTestingModule({
        declarations: [DroneSelectedBoardComponent],
        providers: [
          {provide: SelectedDroneService, useValue: selectedDroneServiceSpy},
        ],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneSelectedBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    component.selectedDroneService = selectedDroneServiceSpy;
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should call sendToogledRequest of selectedDroneService", () => {
    const sendToggleLedRequestSpy = spyOn(selectedDroneServiceSpy, 'sendToogleLedRequest');
    component.sendToggleLedRequest();
    expect(sendToggleLedRequestSpy).toHaveBeenCalled();
  });

  it("should call sendTakeOffRequest of selectedDroneService", () => {
    const sendTakeOffRequestSpy = spyOn(selectedDroneServiceSpy, 'sendTakeOffRequest');
    component.sendTakeOffRequest();
    expect(sendTakeOffRequestSpy).toHaveBeenCalled();
  });

  it("should call sendReturnToBaseRequest of selectedDroneService", () => {
    const sendReturnToBaseRequestSpy = spyOn(selectedDroneServiceSpy, 'sendReturnToBaseRequest');
    component.sendReturnToBaseRequest();
    expect(sendReturnToBaseRequestSpy).toHaveBeenCalled();
  });
});
