import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';
import { MatDialog, MatDialogModule } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { RouterTestingModule } from '@angular/router/testing';
import { ServerMode } from '@app/constants/serverMode';
import { EnvironmentService } from '@app/service/api/environment/environment.service';
import { HomePageComponent } from './home-page.component';

describe('HomePageComponent', () => {
  let component : HomePageComponent;
  let fixture: ComponentFixture<HomePageComponent>
  let environmentSpy : jasmine.SpyObj<EnvironmentService>;
  let matDialogSpy: jasmine.SpyObj<MatDialog>;
  let routeSpy : jasmine.SpyObj<Router>;
  beforeEach(waitForAsync(() => {
    routeSpy = jasmine.createSpyObj('route', ['navigate']);
    matDialogSpy = jasmine.createSpyObj('dialog',['open']);
    environmentSpy = jasmine.createSpyObj('environmentService', ['sendModeToServer']);
    TestBed.configureTestingModule({
      imports: [
        RouterTestingModule
      ],
      declarations: [
        HomePageComponent
      ],
      providers: [{provide: EnvironmentService, useValue:environmentSpy },
      {provide: MatDialog, useValue : matDialogSpy}, 
      {provide: Router, useValue: routeSpy}],
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
    component.modeSelected = ServerMode.SIMULATION;
    component.sendModeToServer();
    expect(environmentSpy.sendModeToServer).toHaveBeenCalled();
    expect(routeSpy.navigate).toHaveBeenCalled();
  });

  it('should call sendModeToServer', () => {
    component.modeSelected = ServerMode.REAL;
    component.sendModeToServer();
    expect(matDialogSpy.open).toHaveBeenCalled();
  });
});
