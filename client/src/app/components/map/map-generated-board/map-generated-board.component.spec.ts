import { TestBed, waitForAsync } from "@angular/core/testing";
import { MapGeneratedBoardComponent } from "./map-generated-board.component";

describe("MapGeneratedBoardComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [MapGeneratedBoardComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(MapGeneratedBoardComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
