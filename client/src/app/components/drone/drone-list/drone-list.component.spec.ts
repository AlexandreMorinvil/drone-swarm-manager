import { TestBed, waitForAsync } from "@angular/core/testing";
import { DroneListComponent } from "./drone-list.component";

describe("DroneListComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [DroneListComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(DroneListComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
