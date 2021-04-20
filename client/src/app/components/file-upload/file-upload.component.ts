import { Component, HostListener, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { io, Socket } from 'socket.io-client/build/index';
import { SocketService } from '@app/service/api/socket.service';
import { FileUploadService } from '@app/service/api/file-upload-service.service';
import { file } from 'jszip';
@Component({
  selector: 'app-file-upload',
  templateUrl: './file-upload.component.html',
  styleUrls: ['./file-upload.component.scss']
})
export class FileUploadComponent implements OnDestroy{
  public disableSend= true;
  public isEdit = true;
  public fileEditArray = [];
  public isDark = true;
  constructor(private fus: FileUploadService) {
    this.fus.getCurrentCode();
    this.fileEditArray = this.fus.getServerFiles();
  }
  ngOnDestroy(){
    this.fileEditArray = [];
  }
  @HostListener('dragover', ['$event'])
  public onDragOver(evt: DragEvent) {
    evt.preventDefault();
    evt.stopPropagation();
    const upLogo = document.querySelector('.box__icon') as HTMLElement;
    upLogo.style.fill = '#5d00ff';
  }
  @HostListener('dragleave', ['$event'])
  public onDragLeave(evt: DragEvent) {
    evt.preventDefault();
    evt.stopPropagation();
    const upLogo = document.querySelector('.box__icon') as HTMLElement;
    upLogo.style.fill = '#673ab7';
  }

  @HostListener('drop', ['$event']) public onDrop(evt: DragEvent) {
    //default behavior stop
    evt.preventDefault();
    // esthetics fix
    const upLogo = document.querySelector('.box__icon') as HTMLElement;
    upLogo.style.fill = '#673ab7';
    
    // Use DataTransferItemList interface to access the file(s)
    if (evt.dataTransfer.items) {
      this.disableSend = false;
      for (let i = 0; i < evt.dataTransfer.items.length; i++) {
        // If dropped items aren't files, reject them
        if (evt.dataTransfer.items[i].kind === 'file') {
          const file: File = evt.dataTransfer.items[i].getAsFile();
          this.fus.addFile(file);
        }

      }
    }
  }

  public inputFiles(ev){
      const fileList = ev.target.files;
      for(const file of fileList) {
        this.fus.addFile(file);
      }
  }
  public getServerFiles(){
    return this.fileEditArray;
  }
  public getFiles(): File[] {
    return this.fus.getFiles();
  }
  public submitUpdate() {
    if (this.isEdit) {
      this.fileEditArray.forEach(file => {
        const f = new File([file.text], file.name);
        this.fus.addFile(f);
      });
    }
    this.fus.sendFiles();
  }
  public clear(){
    this.fus.clear();
  }
  public onTextEdit() {
    this.disableSend = false;
    const txtArea = document.querySelector(".TxtEdit") as HTMLTextAreaElement;

    for (const file of this.fileEditArray) {
      if (file.name == txtArea.id) {
        file.text = txtArea.value;
        break;
      }
    }
  }
  public toggleInput(evt){
    const form = document.querySelector(".box") as HTMLElement;
    const tabGroup = document.querySelector(".fileTabs") as HTMLElement;
    if(this.isEdit){
      form.style.display = 'none';
      tabGroup.style.display = 'block';
    }
    else{
      tabGroup.style.display = 'none';
      form.style.display = 'block';
    }
  }
  public toggleMode(ev){
    const textEdit = document.querySelector('.TxtEdit') as HTMLElement;
    if(this.isDark) {
      textEdit.style.backgroundColor = 'darkblue';
      textEdit.style.color = 'white';
    } else {
      textEdit.style.backgroundColor = 'white';
      textEdit.style.color = 'black';
    }
  }
}
