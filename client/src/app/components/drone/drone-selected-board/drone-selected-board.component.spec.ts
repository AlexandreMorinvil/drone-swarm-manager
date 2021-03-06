import { ComponentFixture, TestBed } from "@angular/core/testing";
import { DroneSelectedBoardComponent } from "./drone-selected-board.component";

describe("DroneSelectedBoardComponent", () => {
  let component: DroneSelectedBoardComponent;
  let fixture: ComponentFixture<DroneSelectedBoardComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DroneSelectedBoardComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(DroneSelectedBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
