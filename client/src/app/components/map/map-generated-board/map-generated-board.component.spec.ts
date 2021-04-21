import { ComponentFixture, TestBed, waitForAsync } from "@angular/core/testing";
import { Vec3 } from "@app/class/vec3";
import { LiveMapService } from "@app/service/api/map-live/live-map.service";
import { SocketService } from "@app/service/api/socket.service";
import { MapComponent } from "../map/map.component";
import { MapGeneratedBoardComponent } from "./map-generated-board.component";

describe("MapGeneratedBoardComponent", () => {
  let component : MapGeneratedBoardComponent;
  let fixture: ComponentFixture<MapGeneratedBoardComponent>
  let socketServiceStub: jasmine.SpyObj<SocketService>;
  let liveMapServiceStub : LiveMapService;
  //let mapSpy : jasmine.SpyObj<MapComponent>;
  beforeEach(
    waitForAsync(() => {
      //mapSpy = jasmine.createSpyObj('map', ['setBaseMap',''])
      socketServiceStub = jasmine.createSpyObj('socketService', ['addEventHandler'])
      liveMapServiceStub = new LiveMapService(socketServiceStub);
      TestBed.configureTestingModule({
        declarations: [MapGeneratedBoardComponent, MapComponent],
        providers : [{provide : LiveMapService, useValue: liveMapServiceStub}],
      }).compileComponents();
    })
  );
  beforeEach(() => {
    fixture = TestBed.createComponent(MapGeneratedBoardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
    component['liveMapService'] = liveMapServiceStub;   
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it("should call add point of map if there's new points", () => {
    component['liveMapService'].wallPointsToAdd = [new Vec3(0,0,0)];
    const addWallPoinSpy = spyOn(component.map, "addWallPoint");
    component.updateMap();
    expect(addWallPoinSpy).toHaveBeenCalled();
  });

});
