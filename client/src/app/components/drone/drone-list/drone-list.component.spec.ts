import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { ControlPageComponent } from "@app/components/page/control/control-page.component";
import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { SelectedDroneService } from "@app/service/selected-drone/selected-drone.service";
import { DroneListComponent } from "./drone-list.component";

describe("DroneListComponent", () => {
  let component : DroneListComponent;
  let fixture: ComponentFixture<DroneListComponent>
  let droneListServiceSpy : jasmine.SpyObj<DroneListService>;
  let selectedDroneServiceSpy : jasmine.SpyObj<SelectedDroneService>;
  let controlPageComponentSpy : jasmine.SpyObj<ControlPageComponent>;
  beforeEach(
    waitForAsync(() => {
      droneListServiceSpy = jasmine.createSpyObj('droneListService', ['getDroneNumber']);
      selectedDroneServiceSpy = jasmine.createSpyObj('selectedDroneService', ['setSelectedDrone']);
      controlPageComponentSpy = jasmine.createSpyObj('controlPage', ['toogleSelectedDroneBoard']);
      TestBed.configureTestingModule({
        declarations: [DroneListComponent],
        providers: [
          {provide: DroneListService, useValue: droneListServiceSpy},
          {provide: SelectedDroneService, useValue: selectedDroneServiceSpy},
          {provide: ControlPageComponent, useValue : controlPageComponentSpy}
        ],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should do nothing if it's not connected", () => {
    component.socketService['socket'].connected = false;
    component.toogleSelectedDroneBoard(1);
    expect(controlPageComponentSpy.toogleSelectedDroneBoard).not.toHaveBeenCalled();
  });

  it("should call setSlectedDrone if isConnected", () => {
    component.socketService['socket'].connected = true;
    component.toogleSelectedDroneBoard(1);
    expect(selectedDroneServiceSpy.setSelectedDrone).toHaveBeenCalled();
  });


});
