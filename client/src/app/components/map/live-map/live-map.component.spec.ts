import { TestBed, waitForAsync } from "@angular/core/testing";
import { LiveMapComponent } from "./live-map.component";

describe("LiveMapComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [LiveMapComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(LiveMapComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
