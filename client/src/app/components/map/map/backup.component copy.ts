import { Component, OnInit } from '@angular/core';
import { StatsBarChart } from '../../../../assets/dummy/data';

import * as d3 from 'd3-selection';
import * as d3Scale from 'd3-scale';
import * as d3Array from 'd3-array';
import * as d3Axis from 'd3-axis';

@Component({
  selector: "app-map",
  templateUrl: "./map.component.html",
  styleUrls: ["./map.component.scss"],
})
export class MapComponent {
  currentRate = 8;
  title = "D3 Barchart with Angular 10";
  width: number;
  height: number;
  margin = { top: 20, right: 20, bottom: 30, left: 40 };
  x: any;
  y: any;
  svg: any;
  g: any;

  constructor() {
    this.width = 900 - this.margin.left - this.margin.right;
    this.height = 500 - this.margin.top - this.margin.bottom;
  }

  ngOnInit() {
    this.initSvg();
    this.initAxis();
    this.drawAxis();
    this.drawBars();
  }

  initSvg() {
    this.svg = d3
      .select("#barChart")
      .append("svg")
      .attr("width", "100%")
      .attr("height", "100%")
      .attr("viewBox", "0 0 900 500");
    this.g = this.svg
      .append("g")
      .attr(
        "transform",
        "translate(" + this.margin.left + "," + this.margin.top + ")"
      );
  }

  initAxis() {
    this.x = d3Scale.scaleBand().rangeRound([0, this.width]).padding(0.1);
    this.y = d3Scale.scaleLinear().rangeRound([this.height, 0]);
    this.x.domain(StatsBarChart.map((d) => d.company));
    this.y.domain([0, d3Array.max(StatsBarChart, (d) => d.frequency)]);
  }

  drawAxis() {
    this.g
      .append("g")
      .attr("class", "axis axis--x")
      .attr("transform", "translate(0," + this.height + ")")
      .call(d3Axis.axisBottom(this.x));
    this.g
      .append("g")
      .attr("class", "axis axis--y")
      .call(d3Axis.axisLeft(this.y))
      .append("text")
      .attr("class", "axis-title")
      .attr("transform", "rotate(-90)")
      .attr("y", 6)
      .attr("dy", "0.71em")
      .attr("text-anchor", "end")
      .text("Frequency");
  }

  drawBars() {
    this.g
      .selectAll(".bar")
      .data(StatsBarChart)
      .enter()
      .append("rect")
      .attr("class", "bar")
      .attr("x", (d) => this.x(d.company))
      .attr("y", (d) => this.y(d.frequency))
      .attr("width", this.x.bandwidth())
      .attr("fill", "#498bfc")
      .attr("height", (d) => this.height - this.y(d.frequency));
  }

    // =========================================================================================================== //

  // height: number;
  // width: number;
  // svg: any;
  // g: any;
  // gx: any;
  // gy: any;

  // constructor() {
  //   this.svg = d3
  //     .create("svg")
  //     .attr("viewBox", [0, 0, this.width, this.height]);

  //   this.g = this.svg
  //     .append("g")
  //     .attr("fill", "none")
  //     .attr("stroke-linecap", "round");

  //     const k = this.height / this.width;

  //   const data = this.data();
  //   this.g
  //     .selectAll("path")
  //     .data(data)
  //     .join("path")
  //     .attr("d", (d) => `M${x(d[0])},${y(d[1])}h0`)
  //     .attr("stroke", (d) => z(d[2]));


  //   const z = d3.scaleOrdinal()
  //     .domain(data.map(d => d[2]))
  //     .range(d3.schemeCategory10)

  //   this.gx = this.svg.append("g");
  //   this.gy = this.svg.append("g");
  // }

  // data() {
  //   // const random = d3.randomNormal(0, 0.2);
  //   const random = () => Math.floor(Math.random() * 11);
  //   const sqrt3 = Math.sqrt(3);
  //   return [].concat(
  //     Array.from({ length: 300 }, () => [random() + sqrt3, random() + 1, 0]),
  //     Array.from({ length: 300 }, () => [random() - sqrt3, random() + 1, 1]),
  //     Array.from({ length: 300 }, () => [random(), random() - 1, 2])
  //   );
  // }


  // =========================================================================================================== //

  // currentRate = 8;
  // title = "D3 Barchart with Angular 10";
  // width: number;
  // height: number;
  // margin = { top: 20, right: 20, bottom: 30, left: 40 };
  // x: any;
  // y: any;
  // svg: any;
  // g: any;

  // constructor() {
  //   this.width = 900 - this.margin.left - this.margin.right;
  //   this.height = 500 - this.margin.top - this.margin.bottom;
  // }

  // ngOnInit() {
  //   this.initSvg();
  //   this.initAxis();
  //   this.drawAxis();
  //   this.drawBars();
  // }

  // initSvg() {
  //   this.svg = d3
  //     .select("#bar-chart")
  //     .append("svg")
  //     .attr("width", "100%")
  //     .attr("height", "100%")
  //     .attr("viewBox", "0 0 900 500");
  //   this.g = this.svg
  //     .append("g")
  //     .attr(
  //       "transform",
  //       "translate(" + this.margin.left + "," + this.margin.top + ")"
  //     );
  // }

  // initAxis() {
  //   this.x = d3Scale.scaleBand().rangeRound([0, this.width]).padding(0.1);
  //   this.y = d3Scale.scaleLinear().rangeRound([this.height, 0]);
  //   this.x.domain(StatsBarChart.map((d) => d.company));
  //   this.y.domain([0, d3Array.max(StatsBarChart, (d) => d.frequency)]);
  // }

  // drawAxis() {
  //   this.g
  //     .append("g")
  //     .attr("class", "axis axis--x")
  //     .attr("transform", "translate(0," + this.height + ")")
  //     .call(d3Axis.axisBottom(this.x));
  //   this.g
  //     .append("g")
  //     .attr("class", "axis axis--y")
  //     .call(d3Axis.axisLeft(this.y))
  //     .append("text")
  //     .attr("class", "axis-title")
  //     .attr("transform", "rotate(-90)")
  //     .attr("y", 6)
  //     .attr("dy", "0.71em")
  //     .attr("text-anchor", "end")
  //     .text("Frequency");
  // }

  // drawBars() {
  //   this.g
  //     .selectAll(".bar")
  //     .data(StatsBarChart)
  //     .enter()
  //     .append("rect")
  //     .attr("class", "bar")
  //     .attr("x", (d) => this.x(d.company))
  //     .attr("y", (d) => this.y(d.frequency))
  //     .attr("width", this.x.bandwidth())
  //     .attr("fill", "#498bfc")
  //     .attr("height", (d) => this.height - this.y(d.frequency));
  // }
}
