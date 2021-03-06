import { ComponentFixture, TestBed } from "@angular/core/testing";
import { DroneListComponent } from "./drone-list.component";
import { SocketService} from "../../../service/socket/socket.service"
import { ControlPageComponent } from "../../page/control/control-page.component";
import { MatSelectionList } from "@angular/material/list";

describe("DroneListComponent", () => {
  let component: DroneListComponent;
  let fixture: ComponentFixture<DroneListComponent>;
  let socketServiceStud: SocketService;
  let controlPageStub: ControlPageComponent;

  beforeEach(async () => {
    socketServiceStud = new SocketService();
    controlPageStub = new ControlPageComponent();
    await TestBed.configureTestingModule({
      declarations: [ DroneListComponent, MatSelectionList ],
      providers: [
        { provide: SocketService, useValue: socketServiceStud },
        { provide: ControlPageComponent, useValue: controlPageStub }
      ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
