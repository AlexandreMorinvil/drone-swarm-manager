import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { MapGeneratedBoardComponent } from "./map-generated-board.component";

describe("MapGeneratedBoardComponent", () => {
  let component: MapGeneratedBoardComponent;
  let fixture: ComponentFixture<MapGeneratedBoardComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ MapGeneratedBoardComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(MapGeneratedBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
