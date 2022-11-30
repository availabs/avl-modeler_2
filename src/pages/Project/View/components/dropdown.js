import React, { useState, useEffect } from "react";
import ReactDOM from "react-dom";

import Charts from "./charts";

const Dropdown = ({ varList, projectId, selectedBlockGroups }) => {
  const [selectedValue, setSelectedValue] = useState("");
  const [metaVariables, setMetaVariables] = useState({});

  const handleChange = (e) => {
    if (e.target.value) {
      setSelectedValue([e.target.value]);
    }
  };

  useEffect(() => {
    const getVariables = async () => {
      const response = await fetch("/data/model_variables.json");
      const data = await response.json();
      // console.log("metaData--", data);

      setMetaVariables(data);
    };
    getVariables();
  }, []);

  return (
    <div>
      <div>
        <label>Select a variable</label>

        <select onChange={handleChange}>
          <option key={0} value={""}></option>
          {varList.map((k, i) => {
            return (
              <option key={i} value={k}>
                {k}
              </option>
            );
          })}
        </select>
      </div>

      <div>
        <Charts
          selectedValue={selectedValue}
          metaVariables={metaVariables}
          projectId={projectId}
          selectedBlockGroups={selectedBlockGroups}
        />
      </div>
    </div>
  );
};

export default Dropdown;
