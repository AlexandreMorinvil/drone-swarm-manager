import { TestBed, waitForAsync } from "@angular/core/testing";
import { MapComponent } from "./map.component";

describe("MapComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [MapComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(MapComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });

  it("should get points of a selected map from server", () => {
  
  });
});
