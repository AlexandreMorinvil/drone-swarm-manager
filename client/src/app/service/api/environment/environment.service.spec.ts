import { TestBed } from '@angular/core/testing';
import { SocketService } from '../socket.service';

import { EnvironmentService } from './environment.service';

describe('EnvironmentService', () => {
  let service: EnvironmentService;
  let socketServiceSpy : jasmine.SpyObj<SocketService>;
  beforeEach(() => {
    socketServiceSpy = jasmine.createSpyObj('socketService', ['emitEvent']);
    TestBed.configureTestingModule({
      providers : [{provide: SocketService, useValue: socketServiceSpy}],
    });
    service = TestBed.inject(EnvironmentService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it('should send mode to the socket service with emitEvent', () => {
    service.sendModeToServer(0,4);
    expect(socketServiceSpy.emitEvent).toHaveBeenCalled();
  });
});