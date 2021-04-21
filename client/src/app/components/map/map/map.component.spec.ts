import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { Vec3 } from "@app/class/vec3";
import { MapComponent } from "./map.component";

describe("MapComponent", () => {
  let component : MapComponent;
  let fixture: ComponentFixture<MapComponent>
  beforeEach(
    waitForAsync(() => {
      TestBed.configureTestingModule({
        declarations: [MapComponent],
      }).compileComponents();
    })
  );

  beforeEach(() => {
    fixture = TestBed.createComponent(MapComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should call updateAxisRange if x is higher that x_max", () => {
    const updateAxisRangeSpy =  spyOn<any>(component, 'updateAxisRange');
    component.max_x = 0;
    const points = [new Vec3(1,0,0), new Vec3(2,0,0)];
    component.addWallPoint(points);
    expect(updateAxisRangeSpy).toHaveBeenCalled();
  });

  it("should call updateAxisRange if y is higher that y_max", () => {
    const updateAxisRangeSpy =  spyOn<any>(component, 'updateAxisRange');
    component.max_y = 0;
    const points = [new Vec3(1,2,0), new Vec3(2,2,0)];
    component.addWallPoint(points);
    expect(updateAxisRangeSpy).toHaveBeenCalled();
  });

  it("should call updateAxisRange if y or x is lower that x_min or y_min", () => {
    const updateAxisRangeSpy =  spyOn<any>(component, 'updateAxisRange');
    component.min_x = 5;
    const points = [new Vec3(1,2,0), new Vec3(2,2,0)];
    component.addWallPoint(points);
    expect(updateAxisRangeSpy).toHaveBeenCalled();
  });

  it("Should reset isFirstTime to true when deleteMap", () => {
    component.isFirstTime = false;
    component.deleteMap();
    expect(component.isFirstTime).toBeTrue();
  });

  it("erasePlot should eraseDrone", () => {
    const eraseDroneSpy = spyOn<any>(component, 'eraseDrones');
    component.erasePlot();
    expect(eraseDroneSpy).toHaveBeenCalled();
  });

  it('Should compute global range', () =>{
    const computeGlobalDataRangeSpy = spyOn<any>(component, 'computeGlobalDataRange').and.callThrough();
    component.wallPoints = [new Vec3(5,8,3), new Vec3(3,9,2)];
    computeGlobalDataRangeSpy.call(component);
    expect(component.min_x).toEqual(3);
    expect(component.min_y).toEqual(8);
    expect(component.max_x).toEqual(5);
    expect(component.max_y).toEqual(9);
  });

  it('should compute global display range', () => {
    const computeGlobalDisplayRangeSpy = spyOn<any>(component, 'computeGlobalDisplayRange').and.callThrough();
    component.min_x = 2;
    component.min_y = 2;
    component.max_x = 10;
    component.max_y = 10;
    computeGlobalDisplayRangeSpy.call(component);
    expect(component.display_min_x).toEqual(1.6);
    expect(component.display_min_y).toEqual(1.6);
    expect(component.display_max_x).toEqual(10.4);
    expect(component.display_max_y).toEqual(10.4);

  });

  it('should compute global display range when width is higher then heigth', () => {
    const computeGlobalDisplayRangeSpy = spyOn<any>(component, 'computeGlobalDisplayRange').and.callThrough();
    component.min_x = 2;
    component.min_y = 2;
    component.max_x = 20;
    component.max_y = 10;
    computeGlobalDisplayRangeSpy.call(component);
    expect(component.display_min_x).toEqual(1.1);
    expect(component.display_min_y).toEqual(-3.4);
    expect(component.display_max_x).toEqual(20.9);
    expect(component.display_max_y).toEqual(15.4);
  });
  
  it("updateAxisRange should redraw map", () => {
    const drawAxisSpy = spyOn<any>(component, "drawAxis");
    const erasePlotSpy = spyOn(component, "erasePlot");
    const drawWallsSpy = spyOn<any>(component, "drawWalls");
    const drawDronesSpy = spyOn<any>(component, "drawDrones");
    const updateAxisRangeSpy = spyOn<any>(component, 'updateAxisRange').and.callThrough();
    updateAxisRangeSpy.call(component);
    expect(drawAxisSpy).toHaveBeenCalled();
    expect(erasePlotSpy).toHaveBeenCalled();
    expect(drawWallsSpy).toHaveBeenCalled();
    expect(drawDronesSpy).toHaveBeenCalled();
  });

  it('drawDrones should call eraseDrones', () => {
    const drawDronesSpy = spyOn<any>(component, "drawDrones").and.callThrough();
    const eraseDronesSpy = spyOn<any>(component, 'eraseDrones');
    drawDronesSpy.call(component);
    expect(eraseDronesSpy).toHaveBeenCalled();
  })
  
});
