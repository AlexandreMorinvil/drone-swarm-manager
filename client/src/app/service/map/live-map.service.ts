import { Injectable } from "@angular/core";
import { Vec3 } from "@app/class/vec3";

import * as d3 from "d3";
import { io, Socket } from "socket.io-client/build/index";

const TOTAL_WIDTH: number = 700;
const TOTAL_HEIGHT: number = 350;

const BORDER_FACTOR: number = 0.1;
const MIN_WIDTH: number = 50;
const MIN_HEIGHT: number = 50;

const NUMBER_TICKS: number = 10;
const GRID_OPACITY: number = 0.25;

@Injectable({
  providedIn: "root",
})
export class LiveMapService {

  currentRate = 8;

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

  zoom: any;
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

  public setPlot(isFirstTime = false): void {
    this.initSvg(isFirstTime);
    this.updateAxisRange(true);
    this.drawAxis();
    this.drawWalls();
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

  private drawAxis(): void {
    this.gAxis.selectAll("#map-axis > *").remove();

    this.xAxis = d3.axisTop(this.xScale).ticks(NUMBER_TICKS);
    this.yAxis = d3.axisLeft(this.yScale).ticks(NUMBER_TICKS);

    this.gAxis.append("g").attr("class", "axis axis-x").call(this.xAxis);
    this.gAxis.append("g").attr("class", "axis axis-y").call(this.yAxis);

    // Grid display
    this.gAxis
      .append("g")
      .attr("class", "map-grid")
      .style("opacity", GRID_OPACITY)
      .call(this.yAxis.tickSize(-this.width, 0, 0).tickFormat(""));

    this.gAxis
      .append("g")
      .attr("class", "map-grid")
      .style("opacity", GRID_OPACITY)
      .call(this.xAxis.tickSize(-this.height, 0, 0).tickFormat(""));
  }

  private drawWalls(): void {
    this.gWall
      .selectAll("circle")
      .data(this.wallPoints)
      .enter()
      .append("circle")
      .attr("cx", (d) => this.xScale(d.x))
      .attr("cy", (d) => this.yScale(d.y))
      .attr("r", 1.5)
      .style("fill", "#69b3a2");
  }

  private updateAxisRange(mustComputeRange: Boolean = false): void {
    // Compute the display range
    if (mustComputeRange) this.computeGlobalDataRange();
    this.computeGlobalDisplayRange();

    // Assign the display range
    this.xScale = d3
      .scaleLinear()
      .domain([this.display_min_x, this.display_max_x])
      .range([0, this.width]);
    this.yScale = d3
      .scaleLinear()
      .domain([this.display_min_y, this.display_max_y])
      .range([0, this.height]);

    // Redraw map
    this.drawAxis();
    this.erasePlot();
    this.drawWalls();
  }

  private computeGlobalDisplayRange() {
    const avg_x = (this.max_x - this.min_x) / 2;
    const avg_y = (this.max_y - this.min_y) / 2;

    // Add border to the range
    this.display_min_x = this.min_x - avg_x * BORDER_FACTOR;
    this.display_max_x = this.max_x + avg_x * BORDER_FACTOR;
    this.display_min_y = this.min_y - avg_y * BORDER_FACTOR;
    this.display_max_y = this.max_y + avg_y * BORDER_FACTOR;

    const width = this.max_x - this.min_x;
    const height = this.max_y - this.min_y;

    // Readjust borders to the minimum accepted dimensions
    if (this.max_x - this.min_x < MIN_WIDTH) {
      this.display_min_x = avg_x - MIN_WIDTH / 2;
      this.display_max_x = avg_x + MIN_WIDTH / 2;
    }
    if (this.max_y - this.min_y < MIN_HEIGHT) {
      this.display_min_y = avg_y - MIN_HEIGHT / 2;
      this.display_max_y = avg_y + MIN_HEIGHT / 2;
    }

    // Readjust the range to have the x and y ratio similar
    const ratioDifference = Math.abs(height - width);
    if (width < height) {
      this.display_min_x -= ratioDifference / 2;
      this.display_max_x += ratioDifference / 2;
    } else if (height < width) {
      this.display_min_y -= ratioDifference / 2;
      this.display_max_y += ratioDifference / 2;
    }
  }

  private erasePlot(): void {
    this.chart.selectAll("#wall-points > *").remove();
  }

  private computeGlobalDataRange() {
    // Compute
    if (this.wallPoints.length) {
      this.min_x = d3.min(this.wallPoints, (d) => d.x);
      this.max_x = d3.max(this.wallPoints, (d) => d.x);
      this.min_y = d3.min(this.wallPoints, (d) => d.y);
      this.max_y = d3.max(this.wallPoints, (d) => d.y);
    } else {
      this.min_x = 0;
      this.max_x = 0;
      this.min_y = 0;
      this.max_y = 0;
    }
  }

  public deleteMap(): void {
    this.chart.selectAll("svg > *").remove();
  }

  resetMap(isFirstTime = false): void {
    this.deleteMap();
    this.setPlot(isFirstTime);
  }

  setBaseMap(points: Vec3[]): void {
    this.wallPoints = points;
    this.resetMap(false);
  }

  addWallPoint(point: Vec3): void {
    let hasBordersChanged: Boolean = false;
    if (point.x > this.max_x) {
      this.max_x = point.x;
      hasBordersChanged = true;
    }
    if (point.x < this.min_x) {
      this.min_x = point.x;
      hasBordersChanged = true;
    }
    if (point.y > this.max_y) {
      this.max_y = point.y;
      hasBordersChanged = true;
    }
    if (point.y < this.min_y) {
      this.min_y = point.y;
      hasBordersChanged = true;
    }
    this.wallPoints.push(point);
    if (hasBordersChanged) this.updateAxisRange();
    this.drawWalls();
  }
  
}
