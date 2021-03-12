import { Component, OnInit } from "@angular/core";
import { StatsBarChart } from "../../../../assets/dummy/data";
import * as d3 from "d3";

// import * as d3 from "d3-selection";
// import * as d3Scale from "d3-scale";
// import * as d3Array from "d3-array";
// import * as d3Axis from "d3-axis";
// export * from 'd3-format';
// export * from 'd3-interpolate';
// export * from 'd3-scale';
// export * from 'd3-shape';

const MARGIN_LEFT: number = 60;
const MARGIN_RIGHT: number = 30;
const MARGIN_TOP: number = 10;
const MARGIN_BOTTOM: number = 30;

const WIDTH: number = 460;
@Component({
  selector: "app-map",
  templateUrl: "./map.component.html",
  styleUrls: ["./map.component.scss"],
})
export class MapComponent {
  height: number = 1000;
  width: number = 1000;
  svg: any;
  g: any;
  gx: any;
  gy: any;

  constructor() {
    // this.svg = d3
    //   .create("svg")
    //   .attr("viewBox", [0, 0, this.width, this.height]);

    // append the svg object to the body of the page
    this.svg = d3
      .select("#bar-chart")
      .append("svg")
      .attr("width", this.width + MARGIN_LEFT + MARGIN_RIGHT)
      .attr("height", this.height + MARGIN_TOP + MARGIN_BOTTOM)
      // .append("g")
      // .attr("transform", "translate(" + MARGIN_LEFT + "," + MARGIN_TOP + ")");

    this.g = this.svg
      .append("g")
      .attr("fill", "none")
      .attr("stroke-linecap", "round");

    const k = this.height / this.width;

    const data = this.data();

    const x = d3.scaleLinear().domain([-4.5, 4.5]).range([0, this.width]);
    const y = d3
      .scaleLinear()
      .domain([-4.5 * k, 4.5 * k])
      .range([this.height, 0]);
    const z = d3
      .scaleOrdinal()
      .domain(data.map((d) => d[2]))
      .range(d3.schemeCategory10);

    this.g
      .selectAll("path")
      .data(data)
      .join("path")
      .attr("d", (d) => `M${x(d[0])},${y(d[1])}h0`)
      .attr("stroke", (d) => z(d[2]));

    this.gx = this.svg.append("g");
    this.gy = this.svg.append("g");

    this.svg.node();
  }

  data() {
    // const random = d3.randomNormal(0, 0.2);
    const random = () => Math.floor(Math.random() * 11);
    const sqrt3 = Math.sqrt(3);
    return [].concat(
      Array.from({ length: 300 }, () => [random() + sqrt3, random() + 1, 0]),
      Array.from({ length: 300 }, () => [random() - sqrt3, random() + 1, 1]),
      Array.from({ length: 300 }, () => [random(), random() - 1, 2])
    );
  }

  // ================================================================================================== //

  // width: number;
  // height: number;
  // svg: any;

  // constructor() {
  //   // set the dimensions and margins of the graph
  //   this.width = WIDTH - MARGIN_LEFT - MARGIN_RIGHT;
  //   this.height = WIDTH - MARGIN_TOP - MARGIN_BOTTOM;

  //   // append the svg object to the body of the page
  //   this.svg = d3
  //     .select("#bar-chart")
  //     .append("svg")
  //     .attr("width", this.width + MARGIN_LEFT + MARGIN_RIGHT)
  //     .attr("height", this.height + MARGIN_TOP + MARGIN_BOTTOM)
  //     .append("g")
  //     .attr("transform", "translate(" + MARGIN_LEFT + "," + MARGIN_TOP + ")");

  //   //Read the data
  //   d3.csv(
  //     "https://raw.githubusercontent.com/holtzy/data_to_viz/master/Example_dataset/2_TwoNum.csv",
  //     (data) => {
  //       // Add X axis
  //       const x = d3.scaleLinear().domain([0, 4000]).range([0, this.width]);
  //       this.svg
  //         .append("g")
  //         .attr("transform", "translate(0," + this.height + ")")
  //         .call(d3.axisBottom(x));

  //       // Add Y axis
  //       const y = d3.scaleLinear().domain([0, 500000]).range([this.height, 0]);
  //       this.svg.append("g").call(d3.axisLeft(y));

  //       // Add dots
  //       this.svg
  //         .append("g")
  //         .selectAll("dot")
  //         .data(data)
  //         .enter()
  //         .append("circle")
  //         .attr("cx", function (d) {
  //           return x(d.GrLivArea);
  //         })
  //         .attr("cy", function (d) {
  //           return y(d.SalePrice);
  //         })
  //         .attr("r", 1.5)
  //         .style("fill", "#69b3a2");
  //     }
  //   );
  // }
}
