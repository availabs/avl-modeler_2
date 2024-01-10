import React from "react";
import {
  useTheme,
  TopNav,
  SideNav,
  FlyoutMenu,
} from "modules/avl-components/src/";
import { Link } from "react-router-dom";
import AuthMenu from "pages/Auth/AuthMenu";
import get from "lodash.get";

const Logo = ({ sideNav }) => {
  const theme = useTheme();
  const themeOptions = {
    size: get(sideNav, "size", "micro"),
    color: get(sideNav, "color", "dark"),
  };
  return (
    <>
      <Link
        to="/"
        className={`${
          theme.sidenav(themeOptions).logoWrapper
        } flex flex-col items-center justify-center`}
      >
        <div>
          <img
            src="/nys_logo.svg"
            className="w-full h-12"
            alt="New York State Logo"
          />
        </div>
      </Link>
    </>
  );
};

const Layout = ({ children, menus, sideNav, title, site }) => {
  const theme = useTheme();
  const themeOptions = {
    size: get(sideNav, "size", "micro"),
    color: get(sideNav, "color", "dark"),
  };

  return (
    <div className="flex">
      <div className={`hidden md:block`}>
        <div className="fixed h-screen">
          <SideNav
            topMenu={<Logo sideNav={sideNav} />}
            themeOptions={themeOptions}
            menuItems={menus}
          />
        </div>
      </div>
      <div
        className={`flex-1 flex items-start flex-col items-stretch min-h-screen`}
      >
        <div className={`${theme.sidenav(themeOptions).fixed}`}>
          <TopNav
            leftMenu={
              <>
                <div className="flex items-center justify-center h-12">
                  <Link
                    to="/"
                    className={`${
                      themeOptions.size === "none" ? "" : "md:hidden"
                    }`}
                  >
                    <div>
                      <img
                        src="/nys_logo_blue.svg"
                        className="w-full h-12"
                        alt="New York State Logo"
                      />
                    </div>
                  </Link>
                  <div
                    className={`text-lg font-bold text-gray-800 hover:text-gray-600 cursor-pointer px-4 `}
                  >
                    {site}{" "}
                    <span className="fal fa-angle-down pl-2 relative top-[2px]" />
                  </div>
                  <div className={`text-2xl font-thin text-blue-500  flex-1`}>
                    <div className="h-[34px] overflow-hidden"> {title} </div>
                  </div>
                </div>
              </>
            }
            rightMenu={<AuthMenu />}
          />
        </div>
        <div
          className={`h-full flex-1 bg-neutral-100 ${
            theme.sidenav(themeOptions).fixed
          }`}
        >
          {children}
        </div>
      </div>
    </div>
  );
};

export default Layout;
