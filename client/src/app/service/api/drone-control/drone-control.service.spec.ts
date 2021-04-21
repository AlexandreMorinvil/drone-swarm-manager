import { TestBed } from '@angular/core/testing';
import { DroneState } from '@app/class/drone';
import { SocketService } from '../socket.service';

import { DroneControlService } from './drone-control.service';

describe('DroneControlService', () => {
  let service: DroneControlService;
  let socketServiceSpy : jasmine.SpyObj<SocketService>;
  beforeEach(() => {
    socketServiceSpy = jasmine.createSpyObj('socketService', ['emitEvent']);
    TestBed.configureTestingModule({
      providers : [{provide: SocketService, useValue: socketServiceSpy}],
    });
    service = TestBed.inject(DroneControlService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });

  it('should call the emitEvent when a function is call', () =>{
    service.sendTakeOffRequest(2);
    expect(socketServiceSpy.emitEvent).toHaveBeenCalledWith("SWITCH_STATE", {id : 2, state: DroneState.TAKE_OFF});
  });
});