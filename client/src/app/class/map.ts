import { Vec3 } from "./vec3";

export const UNSET_MAP_INDEX: number = -1;
export class Map {
  id: Number;
  name: String;
  date: String;
  points: Vec3[];

  constructor(id: Number = UNSET_MAP_INDEX, name: String = "", date: String = "", mapPoints: Vec3[] = []) {
    this.id = id;
    this.name = name;
    this.date = date;
    this.points = mapPoints;
  }
}
