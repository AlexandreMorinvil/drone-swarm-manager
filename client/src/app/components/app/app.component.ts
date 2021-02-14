import { Component } from '@angular/core';
import { SocketService } from '@app/service/socket.service';
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
