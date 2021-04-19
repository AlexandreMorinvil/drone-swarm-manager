import { Component, Inject, ViewChild } from "@angular/core";
import { FormArray, FormBuilder, FormControl, FormGroup, Validators } from "@angular/forms";
import { MatDialogRef, MAT_DIALOG_DATA } from "@angular/material/dialog";
import { Router } from "@angular/router";
import { MinimalDrone } from "@app/class/drone";
import { Vec2 } from "@app/class/vec3";
import { ServerMode } from "@app/constants/serverMode";

import { DroneListService } from "@app/service/api/drone-list/drone-list.service";
import { EnvironmentService } from "@app/service/api/environment/environment.service";

@Component({
    selector: 'app-drone-position',
    templateUrl: './drone-position.component.html',
    styleUrls: ['./drone-position.component.scss']
  })
  export class DronePositionComponent {
    numberOfDrone: Number;
    positionsDrone: MinimalDrone[];
    hexRegex = '[0-9a-fA-F]+';
    formArray = new FormArray([]);

    constructor(
      public environmentService: EnvironmentService,
      private dialogRef: MatDialogRef<DronePositionComponent>,
      public route: Router,
      @Inject(MAT_DIALOG_DATA) data){
        this.numberOfDrone = data.numberOfDrone.value;
    }

    ngOnInit(): void {
      this.positionsDrone = [];
      for(let i = 0; i<this.numberOfDrone; i++) {
        this.positionsDrone.push(new MinimalDrone(i));

        let formGroup: FormGroup = new FormGroup({
          address: new FormControl('', {
              validators: [Validators.required, Validators.pattern(this.hexRegex)],
              updateOn: 'blur',
            }),
          x: new FormControl('', {
            validators: [Validators.required, Validators.max(100), Validators.min(-100)],
            updateOn: 'blur',
          }),
          y: new FormControl('', {
            validators: [Validators.required, Validators.max(100), Validators.min(-100)],
            updateOn: 'blur',
          })
        });

        this.formArray.push(formGroup);
      }
    }

    closeModal(): void {
      if (!this.formArray.valid) {
        return;
      }
      for(let i = 0; i < this.numberOfDrone; i++) {
        let vec2 :Vec2 = new Vec2(
          this.formArray.controls[i].value.x,
          this.formArray.controls[i].value.y,
        );
        this.positionsDrone[i].address = this.formArray.controls[i].value.address;
        this.positionsDrone[i].initRealPos = vec2;
      }
      this.dialogRef.close();
      this.environmentService.sendInitialPosition(this.positionsDrone);
      this.environmentService.sendModeToServer(ServerMode.REAL, this.numberOfDrone);
      this.route.navigate(['/control']);
    }
  }
  