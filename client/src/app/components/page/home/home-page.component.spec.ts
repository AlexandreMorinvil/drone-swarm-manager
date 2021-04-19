import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';
import { RouterTestingModule } from '@angular/router/testing';
import { EnvironmentService } from '@app/service/api/environment/environment.service';
import { env } from 'process';
import { HomePageComponent } from './home-page.component';

describe('HomePageComponent', () => {
  let component : HomePageComponent;
  let fixture: ComponentFixture<HomePageComponent>
  let environmentSpy : jasmine.SpyObj<EnvironmentService>;
  beforeEach(waitForAsync(() => {
    environmentSpy = jasmine.createSpyObj('environmentService', ['sendModeToServer']);
    TestBed.configureTestingModule({
      imports: [
        RouterTestingModule
      ],
      declarations: [
        HomePageComponent
      ],
      providers: [{provide: EnvironmentService, useValue:environmentSpy }],
    }).compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(HomePageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it("should create the component", () => {
    expect(component).toBeTruthy();
  });

  it('should call sendModeToServer', () => {
    component.sendModeToServer();
    expect(environmentSpy.sendModeToServer).toHaveBeenCalled();
  });
});
