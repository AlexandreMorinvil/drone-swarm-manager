import { TestBed, waitForAsync } from "@angular/core/testing";
import { NavigationBarComponent } from "./navigation-bar.component";

describe("NavigationBarComponent", () => {
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [NavigationBarComponent],
      }).compileComponents();
    })
  );

  it("should create the component", () => {
    const fixture = TestBed.createComponent(NavigationBarComponent);
    const app = fixture.componentInstance;
    expect(app).toBeTruthy();
  });
});
