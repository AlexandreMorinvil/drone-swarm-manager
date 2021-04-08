import { Injectable } from "@angular/core";
import { Vec3 } from "@app/class/vec3";

import * as d3 from "d3";
import { io, Socket } from "socket.io-client/build/index";

const TOTAL_WIDTH: number = 700;
const TOTAL_HEIGHT: number = 350;

const BORDER_FACTOR: number = 0.1;
const MIN_WIDTH: number = 100;
const MIN_HEIGHT: number = 100;

const NUMBER_TICKS: number = 10;
const GRID_OPACITY: number = 0.25;

@Injectable({
  providedIn: "root",
})
export class LiveMapService {
  private socket: Socket;

  //currentRate = 8;

  width: number = 0;
  height: number = 0;
  margin = { top: 30, right: 30, bottom: 30, left: 30 };

  min_x: number = 0;
  max_x: number = 0;
  min_y: number = 0;
  max_y: number = 0;

  display_min_x: number = 0;
  display_max_x: number = 0;
  display_min_y: number = 0;
  display_max_y: number = 0;

  chart: d3.Selection<SVGElement>;
  gMain: d3.Selection<HTMLElement>;
  gAxis: d3.Selection<HTMLElement>;
  clipPath: d3.Selection<HTMLElement>;
  gWall: d3.Selection<HTMLElement>;

  //zoom: any;
  xScale: any;
  yScale: any;
  xAxis: any;
  yAxis: any;

  wallPoints: Vec3[] = [];

  mapId: String;

  valueToIncrease: number = 0;
  constructor() {
    this.width = TOTAL_WIDTH - this.margin.left - this.margin.right;
    this.height = TOTAL_HEIGHT - this.margin.top - this.margin.bottom;

    this.min_x = -MIN_WIDTH / 2;
    this.max_x = MIN_WIDTH / 2;
    this.min_y = -MIN_HEIGHT / 2;
    this.max_y = MIN_HEIGHT / 2;
  }

  public initSvg(isFirstTime = false ): void {
    if (isFirstTime)
      this.chart = d3
        .select(this.mapId)
        .append("svg")
        .attr("width", "100%")
        .attr("height", "100%")
        .attr("viewBox", [0, 0, TOTAL_WIDTH, TOTAL_HEIGHT])
        .attr("fill", "white");

    this.gAxis = this.chart
      .append("g")
      .attr("id", "map-axis")
      .attr(
        "transform",
        "translate(" + this.margin.left + "," + this.margin.top + ")"
      );

    this.clipPath = this.chart
      .append("defs")
      .append("clipPath")
      .attr("id", "clip");

    this.clipPath
      .append("svg:rect")
      .attr("id", "clip-rect")
      .attr("x", "0")
      .attr("y", "0")
      .attr("width", this.width)
      .attr("height", this.height)
      .attr(
        "transform",
        "translate(" + this.margin.left + "," + this.margin.top + ")"
      );

    this.gMain = this.chart
      .append("g")
      .attr("clip-path", "url(#clip)")
      .attr("id", "map-main")
      .attr("width", this.width)
      .attr("height", this.height)
      .attr("class", "main");

    this.gWall = this.gMain
      .append("g")
      .attr("id", "wall-points")
      .attr(
        "transform",
        "translate(" + this.margin.left + "," + this.margin.top + ")",
        "clip-path",
        "url(#clip)"
      );
  }
  
}
