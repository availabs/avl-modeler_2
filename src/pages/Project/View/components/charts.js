import React, { Component, useState, useEffect, useMemo } from "react";
import * as d3 from "d3";
// import { ResponsiveBar } from "@nivo/bar";
import BarChart from "./barChart";

const Charts = ({
  metaVariables,
  selectedValue,
  projectId,
  selectedBlockGroups,
}) => {
  //  chart data processing here

  console.log(
    "charts_props---------",
    metaVariables,
    selectedValue,
    selectedValue[0]?.slice(2),
    projectId,
    selectedBlockGroups
  );

  let [selectedVarData, setSelectedVarData] = useState([]);
  let [selectedBGs, setSelectedBGs] = useState([]);

  let selectedVar = selectedValue[0]?.slice(2);

  useMemo(() => {
    // if (selectedValue) {
    fetch(`http://localhost:5000/project/${projectId}/${selectedValue}`)
      .then((response) => response.json())
      .then((data) => {
        if (selectedBlockGroups.length == 0) {
          console.log("selected data--", data[0]);
          setSelectedVarData(data);
        } else {
          // let newData = selectedBlockGroups.map((bg) =>
          //   data.filter((e) => e.BG == bg.slice(5))
          // );

          let gData = d3.group(data, (d) => d.BG);
          let newData = selectedBlockGroups.map((bg) => {
            console.log(
              "blockgroupIDs",
              bg.slice(5),
              bg.slice(5).replace(/^0+/, ""),
              parseInt(bg.slice(5)).toString()
            );

            return gData.get(bg.slice(5).replace(/^0+/, ""))
              ? gData.get(bg.slice(5).replace(/^0+/, ""))
              : [];
            // return gData.get(bg.slice(5)) ? gData.get(bg.slice(5)) : [];
          });

          // let dataNew = data.map((d) => d.BG);
          // const newData = selectedBlockGroups.some((bg) =>
          //   dataNew.includes(bg)
          // );
          setSelectedVarData(newData.flat());

          console.log("newData", gData, newData, newData.flat());
        }
      });
    // }
  }, [selectedValue, selectedBlockGroups]);

  // useEffect(() => {
  //   fetch(`http://localhost:5000/project/${projectId}/${selectedValue}`)
  //     .then((response) => response.json())
  //     .then((data) => {
  //       if (selectedBlockGroups.length == 0) {
  //         setSelectedVarData(data);

  //         console.log("selected data useEffect", data[0]);
  //       }
  //     });
  // }, [selectedValue]);

  // useMemo(() => {
  //   if (selectedValue && selectedBlockGroups.length > 0) {
  //     setSelectedBGs = selectedBlockGroups;

  //     console.log("selected data useMemo", selectedVarData);

  //     let newData = selectedBlockGroups.map((bg) =>
  //       selectedVarData.filter((e) => e.BG == bg.slice(5))
  //     );

  //     // let dataNew = data.map((d) => d.BG);

  //     // const newData = selectedBlockGroups.some((bg) =>
  //     //   dataNew.includes(bg)
  //     // );

  //     setSelectedVarData(newData.flat(2));

  //     console.log("newData", newData, newData.flat(2), newData.flat());
  //   }
  // }, [selectedBlockGroups]);

  console.log(
    "chart_states after useMemo--------------",
    selectedVar,
    selectedVarData,
    selectedBGs
  );

  let chartData = useMemo(() => {
    //console.log("updateCharData", selectedVar, selectedVarData[0]);

    if (selectedValue) {
      let selectedBins = metaVariables[selectedVar].binsCompare;

      const bins = selectedBins.map((d) => Function("v", d.comp));
      // setSelectedVar = selectedValue[0]?.slice(2);
      console.log("bins---------", bins);

      // console.time("evval bins");
      return selectedVarData
        .reduce(
          (a, c) => {
            const value = parseInt(c[selectedVar]) || 0;

            for (let i = 0; i < bins.length; i++) {
              if (bins[i](value)) {
                a[i] += +1;
                break;
              }
            }
            return a;
          },
          bins.map(() => 0)
        )
        .map((d, i) => {
          return { bin: selectedBins[i].name, [selectedVar]: d };
        });

      //console.timeEnd("evval bins");

      // return { bin: bin.name, [selectedVar]: binValue };
    }
  }, [selectedVarData, selectedBlockGroups]);

  console.log("finalChartData", chartData);

  return (
    <div>
      <div> charts data test: {selectedVarData.length} </div>
      <div style={{ height: 400 }}>
        <BarChart keys={[selectedVar]} data={chartData} />
      </div>
    </div>
  );
};

export default Charts;
