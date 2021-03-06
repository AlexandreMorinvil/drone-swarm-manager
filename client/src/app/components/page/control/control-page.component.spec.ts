import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';
import { RouterTestingModule } from '@angular/router/testing';
import { ControlPageComponent } from './control-page.component';

describe('ControlPageComponent', () => {
  let component: ControlPageComponent;
  let fixture: ComponentFixture<ControlPageComponent>;
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ControlPageComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ControlPageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });


  it('should create the app', () => {
    expect(component).toBeTruthy();
  });
});
