import React, { useState, useEffect } from "react";
// import BarChart from "./barCharts";
import VarSelector from "./varSelector";
import Dropdown from "./dropdown";

const ProjectView = ({ projectId, selectedBlockGroups, layer }) => {
  // console.log("projectview render");

  console.log("project_layers------", projectId, selectedBlockGroups, layer);

  const colors = {
    primary: "#333",
    light: "#aaa",
  };

  let [projectData, setProjectData] = useState({ Households: [], Persons: [] });
  let [varList, setVarList] = useState([]);

  useEffect(() => {
    fetch(`http://localhost:5000/project/${projectId}/list_vars`)
      .then((response) => response.json())
      .then((data) => {
        if (data !== null) {
          setVarList([...data]);
        }
      });
  }, [projectId]);

  useEffect(() => {
    fetch(`http://localhost:5000/project/${projectId}/view`)
      .then((response) => response.json())
      .then((data) => {
        if (data !== null) {
          setProjectData({
            ...data,
          });
        }
      });
  }, [projectId]);

  return (
    <div style={{ backgroundColor: "#fff", padding: 15 }}>
      <div>
        <div
          style={{
            fontSize: "1.3em",
            fontWeigh: 500,
            borderBottom: `1px solid ${colors.primary}`,
            color: colors.primary,
          }}
        >
          Overview Project: {projectId}
        </div>
        <div>
          <h3 className="mt-6 text-gray-900 text-sm font-medium">
            Total households: {projectData.Households.length}, Total population:
            {projectData.Persons.length}
          </h3>
        </div>
        <div>
          {/* <BarChart data={projectData} /> */}
          {/* <VarSelector projectData={projectData} varList={varList} /> */}
          <Dropdown
            varList={varList}
            projectId={projectId}
            selectedBlockGroups={selectedBlockGroups}
          />
        </div>
      </div>
    </div>
  );
};

export default ProjectView;
