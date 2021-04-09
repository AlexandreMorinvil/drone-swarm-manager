import { ComponentFixture, TestBed } from '@angular/core/testing';

import { InitRealPosComponent } from './init-real-pos.component';

describe('InitRealPosComponent', () => {
  let component: InitRealPosComponent;
  let fixture: ComponentFixture<InitRealPosComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ InitRealPosComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(InitRealPosComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
