import { ComponentFixture, TestBed } from "@angular/core/testing";
import { DroneSwarmBoardComponent } from "./drone-swarm-board.component";

describe("DroneSwarmBoardComponent", () => {
  let component: DroneSwarmBoardComponent;
  let fixture: ComponentFixture<DroneSwarmBoardComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DroneSwarmBoardComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneSwarmBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
