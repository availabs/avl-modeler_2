import React, { useState, useEffect } from "react";
// import get from "lodash.get";

// import Charts from "./charts";

const Dropdown = ({
  projectId,
  selectedBlockGroups,
  layer,
  selectedSenario,
}) => {
  //   const [selectedValue, setSelectedValue] = useState("");

  const [senarioData, setSenarioData] = useState([]);
  const [senarioOverview, setSenarioOverview] = useState({});
  const [senarioDestination, setSenarioDestination] = useState();
  // const [senarioId, setSenarioId] = useState("");

  console.log("senario_dropdown_props", projectId, layer);

  console.log("senarioDestination---", senarioDestination);

  const handleChange = (e) => {
    if (e.target.value) {
      console.log("e.target.value-------", e.target.value);
      // setSenarioId(e.target.value);
      selectedSenario(e.target.value);
      // selectedSenario(e.target.value);
      //   setSelectedSenario([e.target.value]);

      fetch(`http://localhost:5000/senarios/${e.target.value}/overview`)
        .then((response) => response.json())
        .then((data) => {
          if (data !== null) {
            console.log("view senario overview--------------------", data);
            setSenarioOverview(data);
          }
        });

      //make requestDestination function to get the counts for selectedBGs
      fetch(
        `http://localhost:5000/senarios/${e.target.value}/${projectId}/destination`
      )
        .then((response) => response.json())
        .then((data) => {
          if (data !== null) {
            console.log(
              "senario view destination------------------------",
              data
            );
            setSenarioDestination(data);
          }
        });
    }
  };

  const senarioMap = () => {
    // const raw = {
    //   item1: { key: 'sdfd', value:'sdfd' },
    //   item2: { key: 'sdfd', value:'sdfd' },
    //   item3: { key: 'sdfd', value:'sdfd' }
    // };

    // const allowed = ['item1', 'item3'];

    // const filtered = Object.keys(raw)
    //   .filter(key => allowed.includes(key))
    //   .reduce((obj, key) => {
    //     obj[key] = raw[key];
    //     return obj;
    //   }, {});

    // console.log(filtered);
    const selectedBlockGroupsNew = selectedBlockGroups.map((bg) =>
      parseInt(bg.slice(5, 12)).toString()
    );
    console.log("selectedBlockGroupsNew--", selectedBlockGroupsNew);

    // if (senarioDestination)
    // const senarioDestinationbyBgs = senarioDestination
    // .filter((bg) => selectedBlockGroupsNew.includes(bg))

    const selectedSenarioDestination = senarioDestination.filter((obj) =>
      selectedBlockGroupsNew.includes(obj.bg)
    );

    console.log(
      "selectedSenarioDestination------------",
      selectedSenarioDestination
    );

    // const senarioDestinationbyBgs = Object.values(senarioDestination.bg)
    //   .filter((bg) => selectedBlockGroupsNew.includes(bg))

    //  senarioDestination.filter(obj => selectedBlockGroupsNew.includes(obj.bg)).reduce((a, bg) => {
    //   a[bg] = senarioDestination.count;
    //     return a;
    //   });

    const senarioDestinationbyBgs = selectedSenarioDestination.reduce(
      (a, c) => {
        a[c.bg] = c.count;
        return a;
      }
    );

    console.log("senarioDestinationbyBgs---", senarioDestinationbyBgs);
  };

  //make requestDestination function to get the counts for selectedBGs

  // const requestDestination = async (data) => {
  //   // setProcess(true)

  //   // fetch destination test
  //     // [
  //     //   {
  //     //     "TAZ": "1",
  //     //     "bg": "702001",
  //     //     "count": 9504
  //     //   },
  //     //   {
  //     //     "TAZ": "2",
  //     //     "bg": "702002",
  //     //     "count": 70644
  //     //   }
  //     // ]

  //   await fetch(
  //     `http://localhost:5000/senarios/${senarioId}/${projectId}/destination`
  //   )
  //     .then((r) => r.json())
  //     .then((data) => {
  //       console.log("senario view destination------------------------", data);

  //       //do data filter to filter out only selectedBG's destination counts..

  //       // if (data[0].status === "complete") setProcess(false);
  //     });
  // };

  useEffect(() => {
    fetch(`http://localhost:5000/senarios/${projectId}`)
      .then((response) => response.json())
      .then((data) => {
        if (data !== null) {
          console.log("view senario names--------------------", data);
          setSenarioData(data);
        }
      });
  }, [projectId]);

  // useEffect(() => {
  //   fetch(`http://localhost:5000/${senarioId}/overview`)
  //     .then((response) => response.json())
  //     .then((data) => {
  //       if (data !== null) {
  //         console.log("view senario overview--------------------", data);
  //         setSenarioOverview(data);
  //       }
  //     });
  // }, [senarioId]);

  //   useEffect(() => {
  //     const getVariables = async () => {
  //       const response = await fetch("/senarios/<projectId>");
  //       const data = await response.json();
  //       console.log("metaData--", data);

  //       setMetaVariables(data);
  //     };
  //     getVariables();
  //   }, []);

  return (
    <div>
      <div>
        <label className="mt-2 text-gray-900 text-sm font-medium">
          Or select a senario :
          <br />
        </label>

        <select onChange={handleChange}>
          <option key={0} value={""}>
            choose your senario(s)
          </option>
          {senarioData.map((e, i) => {
            return (
              <option key={i} value={e.id}>
                {e.name}
              </option>
            );
          })}
        </select>
      </div>
      <div>
        Total Households: {senarioOverview.Households}, Total Persons:
        {senarioOverview.Persons}, Trips: {senarioOverview.Trips}
      </div>
      <div className="flex flex-shrink-0 justify-center px-4 py-4 mt-2">
        <button
          type="submit"
          className="ml-4 rounded-md border border-transparent bg-gray-600 py-2 px-4 text-sm font-medium text-white shadow-sm hover:bg-gray-600 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2"
          onClick={() => {
            // setOpen(true);
            senarioMap();

            // window.location.reload();
          }}
        >
          Destination Map
        </button>
      </div>

      {/* <div>
        <Charts
          selectedValue={selectedValue}
          projectId={projectId}
          selectedBlockGroups={selectedBlockGroups}
        />
      </div> */}
    </div>
  );
};

export default Dropdown;
