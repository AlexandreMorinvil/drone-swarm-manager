import { Injectable } from '@angular/core';
import { files } from 'jszip';
import { connectableObservableDescriptor } from 'rxjs/internal/observable/ConnectableObservable';
import { SocketService } from './socket.service';

@Injectable({
  providedIn: 'root'
})
export class FileUploadService {
  private clientFiles: Array<File> = [];
  private serverFiles = [];
  
  constructor(private ss: SocketService) {
    ss.initSocket();
    this.ss.onEvent('OLD_SOURCE', (file) => {
      this.serverFiles.push({ 'name': file['name'], 'text': file['data'] });
    });
    this.ss.onEvent('FAILED_UPDATE', () => {
      alert("Couldn't update drones")
    });
  }
  public addFile(file: File) {
    this.clientFiles.push(file);
  }
  public rmFile(file: File): boolean {
    const index = this.clientFiles.indexOf(file, 0);
    if (index > -1) {
      this.clientFiles.splice(index, 1);
      return true;
    }
    return false;
  }

  public getFiles() {
    return this.clientFiles;
  }
  public sendFiles() {
    for (const file of this.clientFiles) {
      file.arrayBuffer().then(
        buffer => this.ss.emitEvent("SEND_FILE", { name: file.name, content: buffer })
      );
    }
    this.ss.emitEvent("UPDATE");
    this.clientFiles = [];
  }
  public getCurrentCode() {
    this.serverFiles.length = 0;
    // emit an event to trigger the response from server
    this.ss.emitEvent('REQUEST_SOURCE');

  }
  public clear() {
    this.clientFiles = [];
    this.serverFiles = [];
  }
  public getServerFiles() {
    return this.serverFiles;
  }
}
