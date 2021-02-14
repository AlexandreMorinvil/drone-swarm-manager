import { TestBed, waitForAsync } from "@angular/core/testing";
import { DroneSelectedBoardComponent } from "./drone-selected-board.component";

describe("DroneSelectedBoardComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [DroneSelectedBoardComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(DroneSelectedBoardComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
