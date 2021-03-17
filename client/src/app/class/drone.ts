import { Vec3 } from "@app/class/vec3";

export const UNSET_DRONE_INDEX: number = -1;
export const ALL_DRONE_INDEX: number = -2;

const MIN_BATTERY_LEVEL: number = 0;
const MAX_BATTERY_LEVEL: number = 100;
const DECIMALS_TO_DISPLAY: number = 3;

enum DroneState {
  STANDBY = 0,
  TAKE_OFF = 1,
  RETURN_TO_BASE = 2,
  LANDING = 3,
  FAIL = 4,
  UPDATE = 5,
}

export class Drone {
  droneId: number;
  private state: DroneState;
  private batteryLevel: number;
  private isConnected: Boolean;
  private currentPosition: Vec3;
  private currentSpeed: Vec3;

  constructor(
    droneId: number = UNSET_DRONE_INDEX,
    state: number = DroneState.STANDBY,
    batteryLevel: number = 0.0,
    isConnected: Boolean = false,
    currentPosition: Vec3 = new Vec3(),
    currentSpeed: Vec3 = new Vec3()
  ) {
    this.droneId = droneId;
    this.state = state;
    this.batteryLevel = batteryLevel;
    this.isConnected = isConnected;
    this.currentPosition = currentPosition;
    this.currentSpeed = currentSpeed;
  }

  updateDrone(updatedDrone: Drone): void {
    this.batteryLevel = updatedDrone.batteryLevel;
    this.state = updatedDrone.state;
    this.isConnected = updatedDrone.isConnected;
    this.currentPosition = updatedDrone.currentPosition;
    this.currentSpeed = updatedDrone.currentSpeed;
  }

  getDroneId(): number {
    return this.droneId;
  }

  getConnectionStatus(): string {
    if (this.isConnected) return "ON";
    else return "OFF";
  }

  getBatteryLevel(): number {
    let batteryLevel = this.batteryLevel * 100;
    if (batteryLevel > MAX_BATTERY_LEVEL) return MAX_BATTERY_LEVEL;
    else if (batteryLevel < MIN_BATTERY_LEVEL) return MIN_BATTERY_LEVEL;
    return Math.ceil(batteryLevel);
  }

  getScalarSpeed(): string {
    return Math.sqrt(this.currentSpeed.x ** 2 + this.currentSpeed.y ** 2 + this.currentSpeed.z ** 2).toFixed(DECIMALS_TO_DISPLAY);
  }

  getPositionnX(): string {
    return this.currentPosition.x.toFixed(DECIMALS_TO_DISPLAY);
  }

  getPositionnY(): string {
    return this.currentPosition.y.toFixed(DECIMALS_TO_DISPLAY);
  }

  getPositionnZ(): string {
    return this.currentPosition.z.toFixed(DECIMALS_TO_DISPLAY);
  }

  getSpeedX(): string {
    return this.currentSpeed.x.toFixed(DECIMALS_TO_DISPLAY);
  }

  getSpeedY(): string {
    return this.currentSpeed.y.toFixed(DECIMALS_TO_DISPLAY);
  }

  getSpeedZ(): string {
    return this.currentSpeed.z.toFixed(DECIMALS_TO_DISPLAY);
  }

  getDroneStateText(): string {
    switch (this.state) {
      case DroneState.STANDBY:
        return "STANDBY";
      case DroneState.UPDATE:
        return "UPDATE";
      case DroneState.TAKE_OFF:
        return "IN MISSION";
      case DroneState.RETURN_TO_BASE:
        return "RETURN";
      case DroneState.LANDING:
        return "LANDING";
      case DroneState.FAIL:
        return "FAIL";
      default:
        return "IMPOSSIBLE";
    }
  }

  getSpeed(): Vec3 {
    return this.currentSpeed;
  }
}
