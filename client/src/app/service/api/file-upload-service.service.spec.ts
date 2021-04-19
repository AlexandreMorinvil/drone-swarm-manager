import { TestBed } from '@angular/core/testing';

import { FileUploadService } from './file-upload-service.service';
import { SocketService } from './socket.service'
fdescribe('FileUploadServiceService', () => {
  let service: FileUploadService;
  let ss: SocketService;
  const mockFile = new File(['hi'], 'hi')
  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(FileUploadService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it('should add file into the client array', () => {
    service.addFile(mockFile);
    expect((service.getFiles()).length).toEqual(1)
  })
  it('should remove file from the client array', () => {
    service.rmFile(mockFile);
    expect((service.getFiles()).length).toEqual(0)
  })
  it('should ask for sources'), () => {
    service.getCurrentCode();
    expect(ss.emitEvent).toHaveBeenCalled();
  }
});
