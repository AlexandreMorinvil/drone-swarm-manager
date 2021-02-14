import { TestBed, waitForAsync } from "@angular/core/testing";
import { DroneSwarmBoardComponent } from "./drone-swarm-board.component";

describe("DroneSwarmBoardComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [DroneSwarmBoardComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(DroneSwarmBoardComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
