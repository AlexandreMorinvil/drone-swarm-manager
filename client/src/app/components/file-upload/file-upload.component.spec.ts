import { ComponentFixture, TestBed } from '@angular/core/testing';
import { FileUploadService } from '@app/service/api/file-upload/file-upload.service';
import { FileUploadComponent } from './file-upload.component';

describe('FileUploadComponent', () => {
  let component: FileUploadComponent;
  let fixture: ComponentFixture<FileUploadComponent>;
  let fileUploadServiceSpy: jasmine.SpyObj<FileUploadService>;
  beforeEach(async () => {
    fileUploadServiceSpy = jasmine.createSpyObj('fus', ['getCurrentCode','getServerFiles', 'addFile','getFiles','sendFiles', 'clear']);
    await TestBed.configureTestingModule({
      declarations: [ FileUploadComponent ],
      providers : [{provide: FileUploadService, useValue: fileUploadServiceSpy}],
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
  /*it('on drag color changes', () => {
    const upLogo = document.querySelector('.box__icon') as HTMLElement;
    component.onDragOver(new DragEvent('drag'));
    expect(upLogo.style.fill).toEqual('#5d00ff');
    component.onDragLeave(new DragEvent('dragLeave'));
    expect(upLogo.style.fill).toEqual('#673ab7');
  })
  it('change style on drop'), ()=>{
    const upLogoDummy = document.createElement(".box__icon") as HTMLElement;
    component.onDragOver(new DragEvent('drag'));
    component.onDrop(new DragEvent('hi'));
    expect(upLogoDummy.style.fill).toEqual('#673ab7');
  }
  it('submit Update', () => {
    component.submitUpdate();
    expect(fileUploadServiceSpy.sendFiles).toHaveBeenCalled();
  })
  it('on text edit change values', () => {
    component.fileEditArray = [];
    component.fileEditArray.push({name: 'emak', text: ' '});
    //const ele = document.getElementById('emak') as HTMLTextAreaElement;
    const eleDummy = document.createElement('.TxtEdit') as HTMLTextAreaElement;
    eleDummy.id = "emak";
    eleDummy.value = "dummyElement";
    component.onTextEdit();
    expect(component.fileEditArray[0].text).toEqual("dummyElement");
  })*/
});
