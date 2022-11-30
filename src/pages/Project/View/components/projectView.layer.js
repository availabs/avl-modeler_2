import { LayerContainer } from "modules/avl-map/src";
import React from "react";
import flatten from "lodash.flatten";
import mapboxgl from "mapbox-gl";
import ProjectView from "./projectView";

const HOST = "http://localhost:5000";

class PopSynthLayer extends LayerContainer {
  filters = {};

  state = {
    bgsGeometryIds: [],
    pumasGeometryIds: [],
    zoomed: false,
    geometry: {},
    coordinates: [],
    selectedBlockGroups: [],
  };

  // selectedBGs = this.state.selectedBlockGroups;

  sources = [
    {
      id: "pumas",
      source: {
        type: "vector",
        url: "https://tiles.availabs.org/data/census_puma10_ny_2019.json",
      },
    },
    {
      id: "bgs",
      source: {
        type: "vector",
        url: "https://tiles.availabs.org/data/census_block_groups_ny_2019.json",
      },
    },
  ];

  layers = [
    {
      id: "BG",
      "source-layer": "tl_2019_36_bg",
      source: "bgs",
      type: "fill",
      paint: {
        "fill-color": "blue",
        "fill-opacity": 0.2,
      },
    },
    {
      id: "BG-highlight",
      "source-layer": "tl_2019_36_bg",
      source: "bgs",
      type: "fill",
      paint: {
        "fill-color": "red",
        "fill-opacity": 0.3,
        "fill-outline-color": "red",
      },
      filter: ["in", "GEOID", ""],
    },

    {
      id: "BG-selected",
      "source-layer": "tl_2019_36_bg",
      source: "bgs",
      type: "line",
      paint: {
        "line-color": "yellow",
        "line-width": 2,
      },
      filter: ["in", "GEOID", ""],
    },

    {
      id: "PUMA-show",
      "source-layer": "tl_2019_36_puma10",
      source: "pumas",
      type: "line",
      paint: {
        "line-color": "white",
        "line-width": 2,
      },
      //   filter: ["in", "GEOID10", ""],
    },
  ];

  infoBoxes = [
    // {
    //   Component: ({ layer }) => {
    //     return <div>View Project: {layer.projectId}</div>;
    //   },
    //   show: true,
    // },

    {
      Component: ({ layer }) => {
        return React.useMemo(
          () => (
            <div>
              {
                <ProjectView
                  projectId={layer.projectId}
                  layer={layer}
                  selectedBlockGroups={layer.state.selectedBlockGroups}
                />
              }
            </div>
          ),
          [layer.state.selectedBlockGroups]
        );
      },
      show: true,
    },
    // {
    //   Component: CreateNewProject,
    //   show: false,
    // },
  ];

  // onHover = {
  //   layers: ["BG"],
  //   callback: (layerId, features, lngLat) => {
  //     // console.log("hover-----", layerId, features);
  //     let { GEOID } = features[0].properties;

  //     return [[GEOID]];
  //     // return [[NAMELSAD10], ["geoid_hover", GEOID10]];
  //   },
  // };

  onClick = {
    layers: ["BG"],
    callback: (layerId, features, lngLat) => {
      console.log("onClick", layerId, features, lngLat);
      let { GEOID } = features[0].properties;
      // console.log("onClick", GEOID, features);

      let selected = this.state.selectedBlockGroups;

      if (!selected.includes(GEOID)) {
        this.updateState({
          selectedBlockGroups: [...selected, GEOID],
        });
        // console.log("add", selected);
      } else {
        let removed = selected.filter((item) => item !== GEOID);
        this.updateState({
          selectedBlockGroups: [...removed],
        });
        // console.log("remove", selected);
      }

      //show selectedBlockGroups
      this.mapboxMap.setFilter("BG-selected", [
        "in",
        "GEOID",
        ...this.state.selectedBlockGroups,
      ]);

      this.mapboxMap.setFilter("BG-highlight", [
        "in",
        "GEOID",
        ...this.state.selectedBlockGroups,
      ]);
      // console.log("selectedBGs----", selectedBGs);
      // return [[GEOID]];
      // return [[NAMELSAD10], ["geoid_hover", GEOID10]];
    },
  };

  fetchData(falcor) {
    // console.log("fetch Layer----", this);

    return (
      fetch(`${HOST}/project/${this.projectId}/geometryIds`)
        //   fetch(`${HOST}/project/${layer.projectId}/geometryIds`)

        .then((r) => r.json())
        .then((d) => {
          // d =
          //     pumas: ['124124','3601242'],
          //     bgs: ['24141241254125',...]
          // }
          // this.geometryIds = d

          const projectBgs = [];
          const projectPUMAs = [];
          const projectGeometry = {
            bgs: projectBgs,
            pumas: projectPUMAs,
          };

          d.crosswalk.map((d) => {
            projectBgs.push(d.BLKGRP);
            projectPUMAs.push(d.PUMA);
          });

          // console.log(
          //   "geometryIds Fetch---",
          //   d,
          //   d.crosswalk,
          //   projectGeometry,
          //   [...new Set(projectGeometry.pumas)],
          //   [...new Set(projectGeometry.bgs)]
          // );

          this.updateState({
            bgsGeometryIds: [...new Set(projectGeometry.bgs)],
            pumasGeometryIds: [...new Set(projectGeometry.pumas)].map(
              (d) => "360" + d
            ),
          });
        })
    );
  }

  //   init(map) {
  //     const features = map.queryRenderedFeatures({ layers: ["PUMA-show"] });
  //     console.log("features init", features);
  //   }

  render(map) {
    // set filter on layer to only show geometry ids
    if (this.state.pumasGeometryIds) {
      map.setFilter("PUMA-show", [
        "in",
        "GEOID10",
        ...this.state.pumasGeometryIds,
      ]);

      map.setFilter("BG", ["in", "GEOID", ...this.state.bgsGeometryIds]);

      // map.setFilter("BG-highlight", [
      //   "in",
      //   "GEOID",
      //   ...this.state.selectedBlockGroups,
      // ]);

      //getting the geometry
      map.on("render", () => {
        if (!this.state.zoomed) {
          const features = map.queryRenderedFeatures({
            layers: ["PUMA-show"],
          });

          if (features.length > 0) {
            var coordinates = features[0].geometry.coordinates[0];

            const bounds = new mapboxgl.LngLatBounds(
              coordinates[0],
              coordinates[0]
            );

            for (const coord of coordinates) {
              bounds.extend(coord);
            }

            map.fitBounds(bounds, {
              padding: 20,
            });
            console.log("update state?");
            this.updateState({
              zoomed: true,
            });
            map.on("render", () => {});
          }
        }
      });
    }
  }
}

const PopSynthLayerFactory = (options = {}) => new PopSynthLayer(options);

export default PopSynthLayerFactory;
