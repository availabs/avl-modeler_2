import flatten from "lodash.flatten";
import { useFalcor, withAuth } from "modules/avl-components/src";

const host = "http://localhost:5000/";

export const generateCrosswalk = (layer) => {
  console.log("generateCrosswalk1Layer", layer);

  if (layer) {
    // console.log("selectedPumasBgs", layer.state.selectedPumasBgs);
    // console.log("generateCrosswalk1Layer", layer);

    let output = Object.keys(layer.state.selectedPumasBgs).map((key) => {
      // console.log("test---", key)
      return layer.state.selectedPumasBgs[key].map((bg) => {
        // console.log("BLKGRP", bg);
        return {
          BG: bg.slice(5).toString(), // to string
          BLKGRP: bg,
          STATEFP: bg.slice(0, 2), //first 2 dgitsd of bg
          COUNTYFP: parseInt(bg.slice(2, 5)),
          TRACTCE: parseInt(bg.slice(5, 11)),
          GEOID: bg.slice(0, 11),
          NAME: parseInt(bg.slice(5, 11)),
          NAMELSAD: `Census Tract ${parseInt(bg.slice(5, 11))}`,
          PUMA: key.slice(3, 7),
          REGION: 1,
        };
      });
    });

    return flatten(output);
  } else {
    // console.log("generateCrosswalk1Layer", layer);
    return {};
  }
};

export const generateSeedData = (layer, selectedVar) => {
  console.log("fetching households");

  let selectedPumas = layer.state.selectedPumas;

  console.log("selectedPumas---", selectedPumas);

  const household = selectedPumas.map((pumaId) => {
    let puma_id = pumaId.slice(2, 7);
    let url = `${host}pums/psam/h/${puma_id}`;
    return fetch(url)
      .then((r) => r.json())
      .then((d) => {
        return {
          type: "h",
          data: d,
        };
      });
  });

  const persons = selectedPumas.map((pumaId) => {
    let puma_id = pumaId.slice(2, 7);
    let url = `${host}pums/psam/p/${puma_id}`;
    return fetch(url)
      .then((r) => r.json())
      .then((d) => {
        return {
          type: "p",
          data: d,
        };
      });
  });

  console.time("load pums");
  // console.log("api-return---", household, persons);

  return Promise.all([...household, ...persons]).then((pumsData) => {
    console.timeEnd("load pums");
    // console.log("api-Data-return-----", pumsData);

    let hhdata = flatten(
      pumsData.filter((d) => d.type === "h").map((d) => d.data)
    );

    let pdata = flatten(
      pumsData.filter((d) => d.type === "p").map((d) => d.data)
    );

    // b/c papa parse output = { data: [ ... ], errors: [ ... ], meta: {	... }}

    console.log("api-return-seed data simple", hhdata, pdata);

    let hhindex = 1;

    let hhnumLookup = hhdata.reduce((lookup, hh) => {
      if (!lookup[hh.SERIALNO]) {
        lookup[hh.SERIALNO] = hhindex;
        hh.hhnum = hhindex;
        hhindex += 1;
      }
      hh.hhnum = lookup[hh.SERIALNO];
      return lookup;
    }, {});

    console.log("after hhnum", hhdata, hhnumLookup);

    pdata.forEach((d) => (d.hhnum = hhnumLookup[d.SERIALNO])); // better than .map b/c no need to return

    //adding ptype to the person data
    pdata.forEach((d) => {
      if (d.AGEP >= 18 && d.WKHP >= 30) {
        d.ptype = 1;
      } else if (d.AGEP >= 18 && d.WKHP < 30) {
        d.ptype = 2;
      } else if (d.AGEP >= 18 && d.AGEP <= 64 && d.SCHG === 15) {
        d.ptype = 3;
      } else if (d.AGEP >= 18 && d.WKHP < 1) {
        d.ptype = 4;
      } else if (d.AGEP >= 65 && d.WKHP < 1) {
        d.ptype = 5;
      } else if (d.AGEP >= 16 && d.AGEP <= 17 && d.SCHG >= 13 && d.SCHG <= 14) {
        d.ptype = 6;
      } else if (d.AGEP >= 6 && d.AGEP < 16 && d.SCHG >= 2 && d.SCHG <= 12) {
        d.ptype = 7;
      } else if (d.AGEP <= 5 && d.SCHG === 1) {
        d.ptype = 8;
      }
    });

    // let test = pdata.map(d => d.hhnum = hhnumLookup[d.SERIALNO] )

    console.log("after p.hhnum", pdata);

    //console.log('seedPdata----', pdata,pdata[0], flatten(pdata), newpdata, test)
    //console.log('seedData_flatten' ,flatten(hhdata), pdata[0], flatten(pdata)),

    //add ptype column

    // const data = [
    //   { age: 23, gender: 'male', status: 'single' },
    //   { age: 27, gender: 'female', status: 'married' },
    //   { age: 30, gender: 'male', status: 'married' },
    //   { age: 32, gender: 'female', status: 'single' }
    // ];

    //     const ptype = pdata.reduce((acc, item) => {
    //       if (item.AGEP >= 18  && item.WKHP <= 30 ) {
    //         acc.push({

    //           ptype: 1
    //         });
    //       }
    //       return acc;
    //     }, []);

    // console.log(ptype);

    return {
      household: flatten(hhdata), // flatten(hhdata),
      person: flatten(pdata), //flatten(pdata)
    };
  });
};

export const generateControlConfig = (selectedVar) => {
  let objectItem = selectedVar.map((variable) => {
    return Object.keys(variable.acs_vars).map((bin, i) => {
      let target = variable.var + bin;

      console.log("expression", variable.expression);
      return {
        target: variable.target ? variable.target : target,
        geography: variable.acs_type,
        seed_table: variable.synpop_type,
        importance: variable.importance,
        control_field: target,
        expression: variable.expression[i],
      };
    });
  });

  return objectItem;
};
