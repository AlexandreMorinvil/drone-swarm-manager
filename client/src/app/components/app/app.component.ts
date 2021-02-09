import { Component } from '@angular/core';
import { SocketService } from '@app/services/socket.service';
import {io} from 'socket.io-client/build/index';
@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'client';
  constructor(public socket: SocketService) {}
  ngOnInit() {
    this.socket.initSocket();
  }
}
