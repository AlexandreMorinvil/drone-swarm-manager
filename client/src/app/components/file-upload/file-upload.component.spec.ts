import { ChangeDetectorRef } from '@angular/core';
import { ComponentFixture, TestBed } from '@angular/core/testing';
import { FileUploadService } from '@app/service/file-upload-service.service';
import { FileUploadComponent } from './file-upload.component';

describe('FileUploadComponent', () => {
  let component: FileUploadComponent;
  let fixture: ComponentFixture<FileUploadComponent>;
  let fus: FileUploadService;
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ FileUploadComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(FileUploadComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
  it('on drag color changes', () => {
    const upLogo = document.querySelector('.box__icon') as HTMLElement;
    component.onDragOver(new DragEvent('drag'));
    expect(upLogo.style.fill).toEqual('#5d00ff');
    component.onDragLeave(new DragEvent('dragLeave'));
    expect(upLogo.style.fill).toEqual('#673ab7');
  })
  it('change style on drop'), ()=>{
    const upLogo = document.querySelector('.box__icon') as HTMLElement;
    component.onDragOver(new DragEvent('drag'));
    component.onDrop(new DragEvent('hi'));
    expect(upLogo.style.fill).toEqual('#673ab7');
  }
  it('submit Update', () => {
    component.submitUpdate();
    expect(fus.sendFiles).toHaveBeenCalled();
  })
  it('on text edit change values', () => {
    component.fileEditArray = [];
    component.fileEditArray.push({name: 'emak', text: ' '});
    const ele = document.getElementById('emak') as HTMLTextAreaElement;
    ele.value = 'emak';
    component.onTextEdit();
    expect(component.fileEditArray[0].text = 'emak')
  })
});
