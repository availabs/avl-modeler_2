/*
/ Main.cpp
/ the main core of spatialite_gui  - a SQLite /SpatiaLite GUI tool
/
/ version 1.7, 2014 May 8
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2008-2014  Alessandro Furieri
/
/    This program is free software: you can redistribute it and/or modify
/    it under the terms of the GNU General Public License as published by
/    the Free Software Foundation, either version 3 of the License, or
/    (at your option) any later version.
/
/    This program is distributed in the hope that it will be useful,
/    but WITHOUT ANY WARRANTY; without even the implied warranty of
/    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/    GNU General Public License for more details.
/
/    You should have received a copy of the GNU General Public License
/    along with this program.  If not, see <http://www.gnu.org/licenses/>.
/
*/

#include "Classdef.h"

#include "wx/menu.h"
#include "wx/aboutdlg.h"
#include "wx/filename.h"
#include "wx/config.h"
#include "wx/tokenzr.h"

#include <spatialite.h>

#ifdef PROJ_NEW                 /* supporting new PROJ.6 */
#include <proj.h>
#else /* supporting old PROJ.4 */
#include <proj_api.h>
#endif

#include <geos_c.h>

#include <virtualpg.h>

//
// ICONs in XPM format [universally portable]
//
#include "icons/icon.xpm"
#include "icons/icon_info.xpm"
#include "icons/create_new.xpm"
#include "icons/connect.xpm"
#include "icons/connect_ro.xpm"
#include "icons/disconnect.xpm"
#include "icons/memdb_load.xpm"
#include "icons/memdb_new.xpm"
#include "icons/memdb_clock.xpm"
#include "icons/memdb_save.xpm"
#include "icons/vacuum.xpm"
#include "icons/sql_script.xpm"
#include "icons/composer.xpm"
#include "icons/loadshp.xpm"
#include "icons/virtshp.xpm"
#include "icons/loadgeojs.xpm"
#include "icons/virtgeojs.xpm"
#include "icons/loadtxt.xpm"
#include "icons/virttxt.xpm"
#include "icons/loaddbf.xpm"
#include "icons/virtdbf.xpm"
#include "icons/loadxl.xpm"
#include "icons/virtxl.xpm"
#include "icons/network.xpm"
#include "icons/exif.xpm"
#include "icons/gps_pics.xpm"
#include "icons/loadxml.xpm"
#include "icons/srids.xpm"
#include "icons/charset.xpm"
#include "icons/help.xpm"
#include "icons/about.xpm"
#include "icons/exit.xpm"
#include "icons/attach.xpm"
#include "icons/sql_log.xpm"
#include "icons/db_status.xpm"
#include "icons/checkgeom.xpm"
#include "icons/sanegeom.xpm"
#include "icons/wfs.xpm"
#include "icons/dxf.xpm"
#include "icons/map_go.xpm"
#include "icons/security_lock.xpm"
#include "icons/security_relaxed.xpm"
#include "icons/security_rdonly.xpm"
#include "icons/postgres.xpm"

#if defined(_WIN32) && !defined(__MINGW32__)
#define unlink	_unlink
#endif

IMPLEMENT_APP(MyApp)
     bool MyApp::OnInit()
{
//
// main APP implementation
//
  wxString path;
  if (argc > 1)
    path = argv[1];
  MyFrame *frame =
    new MyFrame(wxT("spatialite_gui      [a GUI tool for SQLite/SpatiaLite]"),
                wxPoint(0, 0), wxSize(700, 480));
  frame->Show(true);
  SetTopWindow(frame);
  frame->LoadConfig(path);
  // frame->CheckUpdates();
  return true;
}

MyFrame::MyFrame(const wxString & title, const wxPoint & pos,
                 const wxSize & size):wxFrame((wxFrame *) NULL, -1, title, pos,
                                              size)
{
//
// main GUI frame constructor
//
  setlocale(LC_ALL, "");
// forcing DECIMAL POINT IS COMMA
  setlocale(LC_NUMERIC, "C");

// setting up the LibPQ virtual API
  DoInitVirtualPQapi();

// creating the internal WMS Cache
  WmsCache = create_wms_cache();

  ::wxInitAllImageHandlers();

  MapPanel = NULL;
  Old_SPATIALITE_SECURITY_ENV = NULL;
  RL2MaxThreads = 1;
  MapMultiThreadingEnabled = true;
  MapMaxThreads = 1;
  MapAutoTransformEnabled = true;
  MemoryDatabase = false;
  AutoSaveInterval = 0;
  LastTotalChanges = 0;
  TimerAutoSave = NULL;
  SqlLogEnabled = false;
  VirtualPQapiOK = false;

//
// initializing CHARSET lists
//
  CharsetsLen = 79;
  Charsets = new wxString[CharsetsLen];
  CharsetsNames = new wxString[CharsetsLen];
  *(CharsetsNames + 0) = wxT("ARMSCII-8    Armenian");
  *(CharsetsNames + 1) = wxT("ASCII        US-ASCII");
  *(CharsetsNames + 2) = wxT("BIG5         Chinese/Traditional");
  *(CharsetsNames + 3) = wxT("BIG5-HKSCS   Chinese/Hong Kong");
  *(CharsetsNames + 4) = wxT("BIG5-HKSCS:1999");
  *(CharsetsNames + 5) = wxT("BIG5-HKSCS:2001");
  *(CharsetsNames + 6) = wxT("CP850        DOS/OEM Western Europe");
  *(CharsetsNames + 7) = wxT("CP862        DOS/OEM Hebrew");
  *(CharsetsNames + 8) = wxT("CP866        DOS/OEM Cyrillic");
  *(CharsetsNames + 9) = wxT("CP874        DOS/OEM Thai");
  *(CharsetsNames + 10) = wxT("CP932        DOS/OEM Japanese");
  *(CharsetsNames + 11) = wxT("CP936        DOS/OEM Chinese");
  *(CharsetsNames + 12) = wxT("CP949        DOS/OEM Korean");
  *(CharsetsNames + 13) = wxT("CP950        DOS/OEM Chinese/Big5");
  *(CharsetsNames + 14) = wxT("CP1133       Laotian");
  *(CharsetsNames + 15) = wxT("CP1250       Windows Central Europe");
  *(CharsetsNames + 16) = wxT("CP1251       Windows Cyrillic");
  *(CharsetsNames + 17) = wxT("CP1252       Windows Latin 1");
  *(CharsetsNames + 18) = wxT("CP1253       Windows Greek");
  *(CharsetsNames + 19) = wxT("CP1254       Windows Turkish");
  *(CharsetsNames + 20) = wxT("CP1255       Windows Hebrew");
  *(CharsetsNames + 21) = wxT("CP1256       Windows Arabic");
  *(CharsetsNames + 22) = wxT("CP1257       Windows Baltic");
  *(CharsetsNames + 23) = wxT("CP1258       Windows Vietnamese");
  *(CharsetsNames + 24) = wxT("EUC-CN       Chinese");
  *(CharsetsNames + 25) = wxT("EUC-JP       Japanese");
  *(CharsetsNames + 26) = wxT("EUC-KR       Korean");
  *(CharsetsNames + 27) = wxT("EUC-TW       Taiwan");
  *(CharsetsNames + 28) = wxT("GB18030      Chinese/National Standard");
  *(CharsetsNames + 29) = wxT("GBK          Chinese/Simplified");
  *(CharsetsNames + 30) = wxT("Georgian-Academy");
  *(CharsetsNames + 31) = wxT("Georgian-PS");
  *(CharsetsNames + 32) = wxT("HZ           Chinese");
  *(CharsetsNames + 33) = wxT("ISO-2022-CN  Chinese");
  *(CharsetsNames + 34) = wxT("ISO-2022-CN-EXT");
  *(CharsetsNames + 35) = wxT("ISO-2022-JP  Japanese");
  *(CharsetsNames + 36) = wxT("ISO-2022-JP-1");
  *(CharsetsNames + 37) = wxT("ISO-2022-JP-2");
  *(CharsetsNames + 38) = wxT("ISO-2022-KR  Korean");
  *(CharsetsNames + 39) = wxT("ISO-8859-1   Latin-1 Western European");
  *(CharsetsNames + 40) = wxT("ISO-8859-2   Latin-2 Central European");
  *(CharsetsNames + 41) = wxT("ISO-8859-3   Latin-3 South European");
  *(CharsetsNames + 42) = wxT("ISO-8859-4   Latin-4 North European");
  *(CharsetsNames + 43) = wxT("ISO-8859-5   Latin/Cyrillic");
  *(CharsetsNames + 44) = wxT("ISO-8859-6   Latin/Arabic");
  *(CharsetsNames + 45) = wxT("ISO-8859-7   Latin/Greek");
  *(CharsetsNames + 46) = wxT("ISO-8859-8   Latin/Hebrew");
  *(CharsetsNames + 47) = wxT("ISO-8859-9   Latin-5 Turkish");
  *(CharsetsNames + 48) = wxT("ISO-8859-10  Latin-6 Nordic");
  *(CharsetsNames + 49) = wxT("ISO-8859-11  Latin/Thai");
  *(CharsetsNames + 50) = wxT("ISO-8859-13  Latin-7 Baltic Rim");
  *(CharsetsNames + 51) = wxT("ISO-8859-14  Latin-8 Celtic");
  *(CharsetsNames + 52) = wxT("ISO-8859-15  Latin-9");
  *(CharsetsNames + 53) = wxT("ISO-8859-16  Latin-10 South-Eastern European");
  *(CharsetsNames + 54) = wxT("JOHAB        Korean");
  *(CharsetsNames + 55) = wxT("KOI8-R       Russian");
  *(CharsetsNames + 56) = wxT("KOI8-U       Ukrainian");
  *(CharsetsNames + 57) = wxT("KOI8-RU      Belarusian");
  *(CharsetsNames + 58) = wxT("KOI8-T       Tajik");
  *(CharsetsNames + 59) = wxT("MacArabic    MAC Arabic");
  *(CharsetsNames + 60) = wxT("MacCentralEurope");
  *(CharsetsNames + 61) = wxT("MacCroatian  MAC Croatian");
  *(CharsetsNames + 62) = wxT("MacCyrillic  MAC Cyrillic");
  *(CharsetsNames + 63) = wxT("MacGreek     MAC Greek");
  *(CharsetsNames + 64) = wxT("MacHebrew    MAC Hebrew");
  *(CharsetsNames + 65) = wxT("MacIceland   MAC Iceland");
  *(CharsetsNames + 66) = wxT("Macintosh");
  *(CharsetsNames + 67) = wxT("MacRoman     MAC European/Western languages");
  *(CharsetsNames + 68) = wxT("MacRomania   MAC Romania");
  *(CharsetsNames + 69) = wxT("MacThai      MAC Thai");
  *(CharsetsNames + 70) = wxT("MacTurkish   MAC Turkish");
  *(CharsetsNames + 71) = wxT("MacUkraine   MAC Ukraine");
  *(CharsetsNames + 72) = wxT("MuleLao-1    Laotian");
  *(CharsetsNames + 73) = wxT("PT154        Kazakh");
  *(CharsetsNames + 74) = wxT("RK1048       Kazakh");
  *(CharsetsNames + 75) = wxT("SHIFT_JIS    Japanese");
  *(CharsetsNames + 76) = wxT("TCVN         Vietnamese");
  *(CharsetsNames + 77) = wxT("TIS-620      Thai");
  *(CharsetsNames + 77) = wxT("UTF-8        UNICODE/Universal");
  *(CharsetsNames + 78) = wxT("VISCII       Vietnamese");
  *(Charsets + 0) = wxT("ARMSCII-8");
  *(Charsets + 1) = wxT("ASCII");
  *(Charsets + 2) = wxT("BIG5");
  *(Charsets + 3) = wxT("BIG5-HKSCS");
  *(Charsets + 4) = wxT("BIG5-HKSCS:1999");
  *(Charsets + 5) = wxT("BIG5-HKSCS:2001");
  *(Charsets + 6) = wxT("CP850");
  *(Charsets + 7) = wxT("CP862");
  *(Charsets + 8) = wxT("CP866");
  *(Charsets + 9) = wxT("CP874");
  *(Charsets + 10) = wxT("CP932");
  *(Charsets + 11) = wxT("CP936");
  *(Charsets + 12) = wxT("CP949");
  *(Charsets + 13) = wxT("CP950");
  *(Charsets + 14) = wxT("CP1133");
  *(Charsets + 15) = wxT("CP1250");
  *(Charsets + 16) = wxT("CP1251");
  *(Charsets + 17) = wxT("CP1252");
  *(Charsets + 18) = wxT("CP1253");
  *(Charsets + 19) = wxT("CP1254");
  *(Charsets + 20) = wxT("CP1255");
  *(Charsets + 21) = wxT("CP1256");
  *(Charsets + 22) = wxT("CP1257");
  *(Charsets + 23) = wxT("CP1258");
  *(Charsets + 24) = wxT("EUC-CN");
  *(Charsets + 25) = wxT("EUC-JP");
  *(Charsets + 26) = wxT("EUC-KR");
  *(Charsets + 27) = wxT("EUC-TW");
  *(Charsets + 28) = wxT("GB18030");
  *(Charsets + 29) = wxT("GBK");
  *(Charsets + 30) = wxT("Georgian-Academy");
  *(Charsets + 31) = wxT("Georgian-PS");
  *(Charsets + 32) = wxT("HZ");
  *(Charsets + 33) = wxT("ISO-2022-CN");
  *(Charsets + 34) = wxT("ISO-2022-CN-EXT");
  *(Charsets + 35) = wxT("ISO-2022-JP");
  *(Charsets + 36) = wxT("ISO-2022-JP-1");
  *(Charsets + 37) = wxT("ISO-2022-JP-2");
  *(Charsets + 38) = wxT("ISO-2022-KR");
  *(Charsets + 39) = wxT("ISO-8859-1");
  *(Charsets + 40) = wxT("ISO-8859-2");
  *(Charsets + 41) = wxT("ISO-8859-3");
  *(Charsets + 42) = wxT("ISO-8859-4");
  *(Charsets + 43) = wxT("ISO-8859-5");
  *(Charsets + 44) = wxT("ISO-8859-6");
  *(Charsets + 45) = wxT("ISO-8859-7");
  *(Charsets + 46) = wxT("ISO-8859-8");
  *(Charsets + 47) = wxT("ISO-8859-9");
  *(Charsets + 48) = wxT("ISO-8859-10");
  *(Charsets + 49) = wxT("ISO-8859-11");
  *(Charsets + 50) = wxT("ISO-8859-13");
  *(Charsets + 51) = wxT("ISO-8859-14");
  *(Charsets + 52) = wxT("ISO-8859-15");
  *(Charsets + 53) = wxT("ISO-8859-16");
  *(Charsets + 54) = wxT("JOHAB");
  *(Charsets + 55) = wxT("KOI8-R");
  *(Charsets + 56) = wxT("KOI8-U");
  *(Charsets + 57) = wxT("KOI8-RU");
  *(Charsets + 58) = wxT("KOI8-T");
  *(Charsets + 59) = wxT("MacArabic");
  *(Charsets + 60) = wxT("MacCentralEurope");
  *(Charsets + 61) = wxT("MacCroatian");
  *(Charsets + 62) = wxT("MacCyrillic");
  *(Charsets + 63) = wxT("MacGreek");
  *(Charsets + 64) = wxT("MacHebrew");
  *(Charsets + 65) = wxT("MacIceland");
  *(Charsets + 66) = wxT("Macintosh");
  *(Charsets + 67) = wxT("MacRoman");
  *(Charsets + 68) = wxT("MacRomania");
  *(Charsets + 69) = wxT("MacThai");
  *(Charsets + 70) = wxT("MacTurkish");
  *(Charsets + 71) = wxT("MacUkraine");
  *(Charsets + 72) = wxT("MuleLao-1");
  *(Charsets + 73) = wxT("PT154");
  *(Charsets + 74) = wxT("RK1048");
  *(Charsets + 75) = wxT("SHIFT_JIS");
  *(Charsets + 76) = wxT("TCVN");
  *(Charsets + 77) = wxT("TIS-620");
  *(Charsets + 77) = wxT("UTF-8");
  *(Charsets + 78) = wxT("VISCII");
  LocaleCharset = wxString::FromUTF8(gaiaGetLocaleCharset());
  DefaultCharset = LocaleCharset;
  AskCharset = false;

  HelpPane = false;
  SqliteHandle = NULL;
  SpliteInternalCache = NULL;
  RL2PrivateData = rl2_alloc_private();
  SqlitePath = wxT("");
  BtnConnect = new wxBitmap(connect_xpm);
  BtnConnectReadOnly = new wxBitmap(connect_ro_xpm);
  BtnCreateNew = new wxBitmap(create_new_xpm);
  BtnDisconnect = new wxBitmap(disconnect_xpm);
  BtnMemDbLoad = new wxBitmap(memdb_load_xpm);
  BtnMemDbNew = new wxBitmap(memdb_new_xpm);
  BtnMemDbClock = new wxBitmap(memdb_clock_xpm);
  BtnMemDbSave = new wxBitmap(memdb_save_xpm);
  BtnVacuum = new wxBitmap(vacuum_xpm);
  BtnSqlScript = new wxBitmap(sql_script_xpm);
  BtnQueryComposer = new wxBitmap(composer_xpm);
  BtnLoadShp = new wxBitmap(loadshp_xpm);
  BtnVirtualShp = new wxBitmap(virtshp_xpm);
  BtnLoadGeoJSON = new wxBitmap(loadgeojs_xpm);
  BtnVirtualGeoJSON = new wxBitmap(virtgeojs_xpm);
  BtnLoadTxt = new wxBitmap(loadtxt_xpm);
  BtnVirtualTxt = new wxBitmap(virttxt_xpm);
  BtnLoadDbf = new wxBitmap(loaddbf_xpm);
  BtnVirtualDbf = new wxBitmap(virtdbf_xpm);
  BtnLoadXL = new wxBitmap(loadxl_xpm);
  BtnVirtualXL = new wxBitmap(virtxl_xpm);
  BtnNetwork = new wxBitmap(network_xpm);
  BtnExif = new wxBitmap(exif_xpm);
  BtnGpsPics = new wxBitmap(gps_pics_xpm);
  BtnLoadXml = new wxBitmap(loadxml_xpm);
  BtnSrids = new wxBitmap(srids_xpm);
  BtnCharset = new wxBitmap(charset_xpm);
  BtnHelp = new wxBitmap(help_xpm);
  BtnAbout = new wxBitmap(about_xpm);
  BtnExit = new wxBitmap(exit_xpm);
  BtnAttach = new wxBitmap(attach_xpm);
  BtnSqlLog = new wxBitmap(sql_log_xpm);
  BtnDbStatus = new wxBitmap(db_status_xpm);
  BtnCheckGeom = new wxBitmap(checkgeom_xpm);
  BtnSaneGeom = new wxBitmap(sanegeom_xpm);
  BtnWFS = new wxBitmap(wfs_xpm);
  BtnDXF = new wxBitmap(dxf_xpm);
  BtnMap = new wxBitmap(map_go_xpm);
  BtnPostgres = new wxBitmap(postgres_xpm);

//
// setting up the application icon
//      
  wxIcon MyIcon(icon_xpm);
  SetIcon(MyIcon);

//
// setting up panes
//
  TableTree = new MyTableTree(this);
  QueryView = new MyQueryView(this);
  RsView = new MyResultSetView(this);
  Manager.SetManagedWindow(this);
  wxAuiPaneInfo paneSql = wxAuiPaneInfo().Top();
  paneSql.Name(wxT("sql_stmt"));
  paneSql.CaptionVisible(false);
  paneSql.Floatable(true);
  paneSql.Dockable(true);
  paneSql.Movable(true);
  paneSql.Gripper(true);
  paneSql.CloseButton(false);
  paneSql.BestSize(wxSize(200, 120));
  Manager.AddPane(QueryView, paneSql);
  wxAuiPaneInfo paneView = wxAuiPaneInfo().Centre();
  paneView.Name(wxT("result_set"));
  paneView.CaptionVisible(false);
  paneView.Floatable(true);
  paneView.Dockable(true);
  paneView.Movable(true);
  paneView.Gripper(false);
  paneView.CloseButton(false);
  Manager.AddPane(RsView, paneView);
  wxAuiPaneInfo paneTree = wxAuiPaneInfo().Left();
  paneTree.Name(wxT("tree_view"));
  paneTree.CaptionVisible(false);
  paneTree.Floatable(true);
  paneTree.Dockable(true);
  paneTree.Movable(true);
  paneTree.Gripper(true);
  paneTree.CloseButton(false);
  paneTree.BestSize(wxSize(200, 480));
  Manager.AddPane(TableTree, paneTree, wxPoint(0, 10));
  Manager.Update();
  Centre();

//
// setting up the status bar
//
  StatusBar = new MyStatusBar(this);
  SetStatusBar(StatusBar);

//
// setting up the menu bar
//
  wxMenu *menuFile = new wxMenu;
  wxMenuItem *menuItem;
  menuItem =
    new wxMenuItem(menuFile, ID_Connect,
                   wxT("&Connecting an existing SQLite DB"));
  menuItem->SetBitmap(*BtnConnect);
  menuFile->Append(menuItem);
  menuItem =
    new wxMenuItem(menuFile, ID_Connect_RO,
                   wxT("&Connecting an existing SQLite DB (Read Only)"));
  menuItem->SetBitmap(*BtnConnectReadOnly);
  menuFile->Append(menuItem);
  menuItem =
    new wxMenuItem(menuFile, ID_CreateNew,
                   wxT("Creating a &New (empty) SQLite DB"));
  menuItem->SetBitmap(*BtnCreateNew);
  menuFile->Append(menuItem);
  wxMenu *memoryMenu = new wxMenu();
  menuItem =
    new wxMenuItem(memoryMenu, ID_MemoryDbLoad,
                   wxT("&Loading an existing DB into the MEMORY-DB"));
  menuItem->SetBitmap(*BtnMemDbLoad);
  memoryMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(memoryMenu, ID_MemoryDbNew,
                   wxT("Creating a &New (empty) MEMORY-DB"));
  menuItem->SetBitmap(*BtnMemDbNew);
  memoryMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(memoryMenu, ID_MemoryDbClock,
                   wxT("&AutoSaving the current MEMORY-DB"));
  menuItem->SetBitmap(*BtnMemDbClock);
  memoryMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(memoryMenu, ID_MemoryDbSave,
                   wxT("&Saving the current MEMORY-DB"));
  menuItem->SetBitmap(*BtnMemDbSave);
  memoryMenu->Append(menuItem);
  menuFile->AppendSubMenu(memoryMenu, wxT("&MEMORY-DB"));
  menuItem =
    new wxMenuItem(menuFile, ID_Disconnect,
                   wxT("&Disconnecting current SQLite DB"));
  menuItem->SetBitmap(*BtnDisconnect);
  menuFile->Append(menuItem);
  menuItem =
    new wxMenuItem(menuFile, ID_Vacuum,
                   wxT("&Optimizing current SQLite DB [VACUUM]"));
  menuItem->SetBitmap(*BtnVacuum);
  menuFile->Append(menuItem);
  menuItem = new wxMenuItem(menuFile, ID_Attach, wxT("&Attach DataBase"));
  menuItem->SetBitmap(*BtnAttach);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem =
    new wxMenuItem(menuFile, ID_Postgres, wxT("&Connecting to PostgreSQL"));
  menuItem->SetBitmap(*BtnPostgres);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem =
    new wxMenuItem(menuFile, ID_MapPanel, wxT("Map &Panel"), wxT("Map &Panel"),
                   wxITEM_CHECK);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  wxMenu *advancedMenu = new wxMenu();
  menuItem =
    new wxMenuItem(advancedMenu, ID_SqlScript, wxT("&Execute SQL script"));
  menuItem->SetBitmap(*BtnSqlScript);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem =
    new wxMenuItem(advancedMenu, ID_QueryViewComposer,
                   wxT("Query/View &Composer"));
  menuItem->SetBitmap(*BtnQueryComposer);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_LoadShp, wxT("&Load Shapefile"));
  menuItem->SetBitmap(*BtnLoadShp);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_VirtualShp, wxT("&Virtual Shapefile"));
  menuItem->SetBitmap(*BtnVirtualShp);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_LoadGeoJSON, wxT("&Load GeoJSON"));
  menuItem->SetBitmap(*BtnLoadGeoJSON);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_VirtualGeoJSON, wxT("&Virtual GeoJSON"));
  menuItem->SetBitmap(*BtnVirtualGeoJSON);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_LoadTxt, wxT("Load CSV/&TXT"));
  menuItem->SetBitmap(*BtnLoadTxt);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_VirtualTxt, wxT("Virtual &CSV/TXT"));
  menuItem->SetBitmap(*BtnVirtualTxt);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_LoadDbf, wxT("Load &DBF"));
  menuItem->SetBitmap(*BtnLoadDbf);
  advancedMenu->Append(menuItem);
  menuItem = new wxMenuItem(advancedMenu, ID_VirtualDbf, wxT("Virtual &DBF"));
  menuItem->SetBitmap(*BtnVirtualDbf);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_LoadXL, wxT("Load &XLS"));
  menuItem->SetBitmap(*BtnLoadXL);
  advancedMenu->Append(menuItem);
  menuItem = new wxMenuItem(advancedMenu, ID_VirtualXL, wxT("Virtual &XLS"));
  menuItem->SetBitmap(*BtnVirtualXL);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_Network, wxT("Create &Network"));
  menuItem->SetBitmap(*BtnNetwork);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem = new wxMenuItem(advancedMenu, ID_Exif, wxT("Import &EXIF photos"));
  menuItem->SetBitmap(*BtnExif);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_GpsPics, wxT("Import &GPS photos"));
  menuItem->SetBitmap(*BtnGpsPics);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_LoadXml, wxT("Import &XML Documents"));
  menuItem->SetBitmap(*BtnLoadXml);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_WFS,
                   wxT("Import data from &WFS datasource"));
  menuItem->SetBitmap(*BtnWFS);
  advancedMenu->Append(menuItem);
  menuItem = new wxMenuItem(advancedMenu, ID_DXF, wxT("Import &DXF drawings"));
  menuItem->SetBitmap(*BtnDXF);
  advancedMenu->Append(menuItem);
  advancedMenu->AppendSeparator();
  menuItem =
    new wxMenuItem(advancedMenu, ID_Srids, wxT("&Search SRID by name"));
  menuItem->SetBitmap(*BtnSrids);
  advancedMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(advancedMenu, ID_Charset, wxT("&Default Output Charset"));
  menuItem->SetBitmap(*BtnCharset);
  advancedMenu->Append(menuItem);
  menuFile->AppendSubMenu(advancedMenu, wxT("&Advanced"));
  menuFile->AppendSeparator();
  menuItem =
    new wxMenuItem(menuFile, ID_SqlLog, wxT("&SQL Log"), wxT("&SQL Log"),
                   wxITEM_CHECK);
  menuFile->Append(menuItem);
  menuItem = new wxMenuItem(menuFile, ID_DbStatus, wxT("&DB Status"));
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem = new wxMenuItem(menuFile, ID_CheckGeom, wxT("&Check Geometries"));
  menuFile->Append(menuItem);
  menuItem = new wxMenuItem(menuFile, ID_SaneGeom, wxT("&Sanitize Geometries"));
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem =
    new wxMenuItem(menuFile, ID_PROJ_LIB, wxT("External variable &PROJ_LIB"));
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem = new wxMenuItem(menuFile, ID_Help, wxT("&Help"));
  menuItem->SetBitmap(*BtnHelp);
  menuFile->Append(menuItem);
  menuItem = new wxMenuItem(menuFile, wxID_ABOUT, wxT("&About ..."));
  menuItem->SetBitmap(*BtnAbout);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem = new wxMenuItem(menuFile, wxID_EXIT, wxT("&Quit"));
  menuItem->SetBitmap(*BtnExit);
  menuFile->Append(menuItem);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, wxT("&Menu"));
  SetMenuBar(menuBar);

//
// setting up menu initial state 
//
  menuBar->Enable(ID_Disconnect, false);
  menuBar->Enable(ID_MemoryDbClock, false);
  menuBar->Enable(ID_MemoryDbSave, false);
  menuBar->Enable(ID_Vacuum, false);
  menuBar->Enable(ID_Postgres, false);
  menuBar->Enable(ID_MapPanel, false);
  menuBar->Enable(ID_SqlScript, false);
  menuBar->Enable(ID_QueryViewComposer, false);
  menuBar->Enable(ID_LoadShp, false);
  menuBar->Enable(ID_VirtualShp, false);
  menuBar->Enable(ID_LoadTxt, false);
  menuBar->Enable(ID_VirtualTxt, false);
  menuBar->Enable(ID_LoadDbf, false);
  menuBar->Enable(ID_VirtualDbf, false);
  menuBar->Enable(ID_LoadXL, false);
  menuBar->Enable(ID_VirtualXL, false);
  menuBar->Enable(ID_Network, false);
  menuBar->Enable(ID_Exif, false);
  menuBar->Enable(ID_GpsPics, false);
  menuBar->Enable(ID_LoadXml, false);
  menuBar->Enable(ID_Srids, false);
  menuBar->Enable(ID_Attach, false);
  menuBar->Enable(ID_SqlLog, false);
  menuBar->Enable(ID_DbStatus, false);
  menuBar->Enable(ID_CheckGeom, false);
  menuBar->Enable(ID_SaneGeom, false);
  menuBar->Enable(ID_WFS, false);
  menuBar->Enable(ID_DXF, false);

//
// setting up the toolbar
//      
  wxToolBar *toolBar = CreateToolBar();
  toolBar->AddTool(ID_Connect, wxT("Connecting an existing SQLite DB"),
                   *BtnConnect, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Connecting an existing SQLite DB"));
  toolBar->AddTool(ID_Connect_RO,
                   wxT("Connecting an existing SQLite DB (Read Only)"),
                   *BtnConnectReadOnly, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Connecting an existing SQLite DB (Read Only)"));
  toolBar->AddTool(ID_CreateNew, wxT("Creating a &New (empty) SQLite DB"),
                   *BtnCreateNew, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Creating a &New (empty) SQLite DB"));
  toolBar->AddTool(ID_MemoryDbLoad,
                   wxT("Loading an existing DB into the MEMORY-DB"),
                   *BtnMemDbLoad, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Loading an existing DB into the MEMORY-DB"));
  toolBar->AddTool(ID_MemoryDbNew, wxT("Creating a New (empty) MEMORY-DB"),
                   *BtnMemDbNew, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Creating a New (empty) MEMORY-DB"));
  toolBar->AddTool(ID_MemoryDbClock, wxT("AutoSaving the current MEMORY-DB"),
                   *BtnMemDbClock, wxNullBitmap, wxITEM_NORMAL,
                   wxT("AutoSaving the current MEMORY-DB"));
  toolBar->AddTool(ID_MemoryDbSave, wxT("Saving the current MEMORY-DB"),
                   *BtnMemDbSave, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Saving the current MEMORY-DB"));
  toolBar->AddTool(ID_Disconnect, wxT("Disconnecting current SQLite DB"),
                   *BtnDisconnect, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Disconnecting current SQLite DB"));
  toolBar->AddTool(ID_Vacuum, wxT("Optimizing current SQLite DB [VACUUM]"),
                   *BtnVacuum, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Optimizing current SQLite DB [VACUUM]"));
  toolBar->AddTool(ID_Attach, wxT("Attach DataBase"),
                   *BtnAttach, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Attach DataBase"));
  toolBar->AddTool(ID_Postgres, wxT("Connecting to PostgreSQL"),
                   *BtnPostgres, wxNullBitmap, wxITEM_CHECK,
                   wxT("Connecting to PostgreSQL"));
  toolBar->AddTool(ID_MapPanel, wxT("Map Panel"), *BtnMap, wxNullBitmap,
                   wxITEM_CHECK, wxT("Map Panel"));
  toolBar->AddTool(ID_SqlLog, wxT("SQL Log"), *BtnSqlLog, wxNullBitmap,
                   wxITEM_CHECK, wxT("SQL Log"));
  toolBar->AddTool(ID_DbStatus, wxT("DB Status"), *BtnDbStatus, wxNullBitmap,
                   wxITEM_NORMAL, wxT("DB Status"));
  toolBar->AddTool(ID_CheckGeom, wxT("Check Geometries"), *BtnCheckGeom,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Check Geometries"));
  toolBar->AddTool(ID_SaneGeom, wxT("Sanitize Geometries"), *BtnSaneGeom,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Sanitize Geometries"));
  toolBar->AddTool(ID_SqlScript, wxT("Execute SQL script"), *BtnSqlScript,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Execute SQL script"));
  toolBar->AddTool(ID_QueryViewComposer, wxT("Query/View Composer"),
                   *BtnQueryComposer, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Query/View Composer"));
  toolBar->AddTool(ID_LoadShp, wxT("Load Shapefile"), *BtnLoadShp, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Load Shapefile"));
  toolBar->AddTool(ID_VirtualShp, wxT("Virtual Shapefile"), *BtnVirtualShp,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Virtual Shapefile"));
  toolBar->AddTool(ID_LoadGeoJSON, wxT("Load GeoJSON"), *BtnLoadGeoJSON,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Load GeoJSON"));
  toolBar->AddTool(ID_VirtualGeoJSON, wxT("Virtual GeoJSON"),
                   *BtnVirtualGeoJSON, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Virtual GeoJSON"));
  toolBar->AddTool(ID_LoadTxt, wxT("Load CSV/TXT"), *BtnLoadTxt, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Load CSV/TXT"));
  toolBar->AddTool(ID_VirtualTxt, wxT("Virtual CSV/TXT"), *BtnVirtualTxt,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Virtual CSV/TXT"));
  toolBar->AddTool(ID_LoadDbf, wxT("Load DBF"), *BtnLoadDbf, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Load DBF"));
  toolBar->AddTool(ID_VirtualDbf, wxT("Virtual DBF"), *BtnVirtualDbf,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Virtual DBF"));
  toolBar->AddTool(ID_LoadXL, wxT("Load XLS"), *BtnLoadXL, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Load XLS"));
  toolBar->AddTool(ID_VirtualXL, wxT("Virtual XLS"), *BtnVirtualXL,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Virtual XLS"));
  toolBar->AddTool(ID_Network, wxT("Create Network"), *BtnNetwork, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Create Network"));
  toolBar->AddTool(ID_Exif, wxT("Import EXIF photos"), *BtnExif, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Import EXIF photos"));
  toolBar->AddTool(ID_GpsPics, wxT("Import GPS photos"), *BtnGpsPics,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Import GPS photos"));
  toolBar->AddTool(ID_LoadXml, wxT("Import XML Documents"), *BtnLoadXml,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Import XML Documents"));
  toolBar->AddTool(ID_WFS, wxT("Import data from WFS datasource"), *BtnWFS,
                   wxNullBitmap, wxITEM_NORMAL,
                   wxT("Import data from WFS datasource"));
  toolBar->AddTool(ID_DXF, wxT("Import DXF drawings"), *BtnDXF, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Import DXF drawings"));
  toolBar->AddTool(ID_Srids, wxT("Search SRID by name"), *BtnSrids,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Search SRID by name"));
  toolBar->AddTool(ID_Charset, wxT("Default Output Charset"), *BtnCharset,
                   wxNullBitmap, wxITEM_NORMAL, wxT("Default Output Charset"));
  toolBar->AddTool(ID_Help, wxT("Help"), *BtnHelp, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Help"));
  toolBar->AddTool(wxID_ABOUT, wxT("About ..."), *BtnAbout, wxNullBitmap,
                   wxITEM_NORMAL, wxT("About ..."));
  toolBar->AddTool(wxID_EXIT, wxT("Quit"), *BtnExit, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Quit"));
  toolBar->Realize();
  SetToolBar(toolBar);

//
// setting up the toolbar initial state
//
  toolBar->EnableTool(ID_Disconnect, false);
  toolBar->EnableTool(ID_MemoryDbClock, false);
  toolBar->EnableTool(ID_MemoryDbSave, false);
  toolBar->EnableTool(ID_Vacuum, false);
  toolBar->EnableTool(ID_Postgres, false);
  toolBar->EnableTool(ID_MapPanel, false);
  toolBar->EnableTool(ID_SqlScript, false);
  toolBar->EnableTool(ID_QueryViewComposer, false);
  toolBar->EnableTool(ID_LoadShp, false);
  toolBar->EnableTool(ID_VirtualShp, false);
  toolBar->EnableTool(ID_LoadGeoJSON, false);
  toolBar->EnableTool(ID_VirtualGeoJSON, false);
  toolBar->EnableTool(ID_LoadTxt, false);
  toolBar->EnableTool(ID_VirtualTxt, false);
  toolBar->EnableTool(ID_LoadDbf, false);
  toolBar->EnableTool(ID_VirtualDbf, false);
  toolBar->EnableTool(ID_LoadXL, false);
  toolBar->EnableTool(ID_VirtualXL, false);
  toolBar->EnableTool(ID_Network, false);
  toolBar->EnableTool(ID_Exif, false);
  toolBar->EnableTool(ID_GpsPics, false);
  toolBar->EnableTool(ID_LoadXml, false);
  toolBar->EnableTool(ID_Srids, false);
  toolBar->EnableTool(ID_Attach, false);
  toolBar->EnableTool(ID_SqlLog, false);
  toolBar->EnableTool(ID_DbStatus, false);
  toolBar->EnableTool(ID_CheckGeom, false);
  toolBar->EnableTool(ID_SaneGeom, false);
  toolBar->EnableTool(ID_WFS, false);
  toolBar->EnableTool(ID_DXF, false);

// updating the status bar
  UpdateStatusBar();

//
// setting up event handlers for menu and toolbar
//
  Connect(ID_Connect, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnConnect);
  Connect(ID_Connect_RO, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnConnectReadOnly);
  Connect(ID_CreateNew, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnCreateNew);
  Connect(ID_Disconnect, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnDisconnect);
  Connect(ID_MemoryDbLoad, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnMemoryDbLoad);
  Connect(ID_MemoryDbNew, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnMemoryDbNew);
  Connect(ID_MemoryDbClock, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnMemoryDbClock);
  Connect(ID_MemoryDbSave, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnMemoryDbSave);
  Connect(ID_Vacuum, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnVacuum);
  Connect(ID_Postgres, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnPostgreSQL);
  Connect(ID_MapPanel, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnMapPanel);
  Connect(ID_SqlScript, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnSqlScript);
  Connect(ID_QueryViewComposer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnQueryViewComposer);
  Connect(ID_LoadShp, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnLoadShp);
  Connect(ID_VirtualShp, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnVirtualShp);
  Connect(ID_LoadGeoJSON, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnLoadGeoJSON);
  Connect(ID_VirtualGeoJSON, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnVirtualGeoJSON);
  Connect(ID_LoadTxt, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnLoadTxt);
  Connect(ID_VirtualTxt, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnVirtualTxt);
  Connect(ID_LoadDbf, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnLoadDbf);
  Connect(ID_VirtualDbf, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnVirtualDbf);
  Connect(ID_LoadXL, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnLoadXL);
  Connect(ID_VirtualXL, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnVirtualXL);
  Connect(ID_Network, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnNetwork);
  Connect(ID_Exif, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnImportExifPhotos);
  Connect(ID_GpsPics, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnImportGpsPhotos);
  Connect(ID_LoadXml, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnImportXmlDocuments);
  Connect(ID_WFS, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnImportWFS);
  Connect(ID_DXF, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnImportDXF);
  Connect(ID_Srids, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnSrids);
  Connect(ID_Charset, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnCharset);
  Connect(ID_Attach, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnAttachDatabase);
  Connect(ID_SqlLog, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnSqlLog);
  Connect(ID_DbStatus, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnDbStatus);
  Connect(ID_CheckGeom, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnCheckGeometries);
  Connect(ID_SaneGeom, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnSanitizeGeometries);
  Connect(ID_PROJ_LIB, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnProjLib);
  Connect(ID_Help, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnHelp);
  Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnAbout);
  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyFrame::OnQuit);
  Connect(wxID_ANY, wxEVT_CLOSE_WINDOW,
          (wxObjectEventFunction) & MyFrame::OnClose);
  Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(MyFrame::OnMouseMove),
          NULL, this);

//
// setting up event handlers for mouse
//
  TableTree->Connect(wxID_ANY, wxEVT_MOTION,
                     wxMouseEventHandler(MyFrame::OnMouseMove), NULL, this);
  GetMenuBar()->Connect(wxID_ANY, wxEVT_MOTION,
                        wxMouseEventHandler(MyFrame::OnMouseMove), NULL, this);
  GetStatusBar()->Connect(wxID_ANY, wxEVT_MOTION,
                          wxMouseEventHandler(MyFrame::OnMouseMove), NULL,
                          this);
//
// setting up a Timer event handler for AutoSave
//
  Connect(ID_AUTO_SAVE_TIMER, wxEVT_TIMER,
          wxTimerEventHandler(MyFrame::OnTimerAutoSave), NULL, this);
}

MyFrame::~MyFrame()
{
//
// main GUI frame destructor
//
  if (MapPanel != NULL)
    MapPanel->ParentQuit();
  if (TimerAutoSave)
    {
      TimerAutoSave->Stop();
      delete TimerAutoSave;
    }
  LastDitchMemoryDbSave();
  ConfigLayout = Manager.SavePerspective();
  GetPosition(&ConfigPaneX, &ConfigPaneY);
  GetSize(&ConfigPaneWidth, &ConfigPaneHeight);
  if (SpliteInternalCache)
    spatialite_finalize_topologies(SpliteInternalCache);
  if (SqliteHandle)
    {
      DoAutoDetachDatabases();
      sqlite3_close(SqliteHandle);
      AttachedList.Flush();
    }
  if (SpliteInternalCache)
    spatialite_cleanup_ex(SpliteInternalCache);
  spatialite_shutdown();
  SaveConfig();
  Manager.UnInit();
  if (MapPanel != NULL)
    MapPanel->ParentQuit();
  if (BtnConnect != NULL)
    delete BtnConnect;
  if (BtnConnectReadOnly != NULL)
    delete BtnConnectReadOnly;
  if (BtnCreateNew != NULL)
    delete BtnCreateNew;
  if (BtnDisconnect != NULL)
    delete BtnDisconnect;
  if (BtnMemDbLoad != NULL)
    delete BtnMemDbLoad;
  if (BtnMemDbNew != NULL)
    delete BtnMemDbNew;
  if (BtnMemDbClock != NULL)
    delete BtnMemDbClock;
  if (BtnMemDbSave != NULL)
    delete BtnMemDbSave;
  if (BtnVacuum != NULL)
    delete BtnVacuum;
  if (BtnMap != NULL)
    delete BtnMap;
  if (BtnPostgres != NULL)
    delete BtnPostgres;
  if (BtnSqlScript != NULL)
    delete BtnSqlScript;
  if (BtnQueryComposer != NULL)
    delete BtnQueryComposer;
  if (BtnLoadShp != NULL)
    delete BtnLoadShp;
  if (BtnVirtualShp != NULL)
    delete BtnVirtualShp;
  if (BtnLoadGeoJSON != NULL)
    delete BtnLoadGeoJSON;
  if (BtnVirtualGeoJSON != NULL)
    delete BtnVirtualGeoJSON;
  if (BtnLoadTxt != NULL)
    delete BtnLoadTxt;
  if (BtnVirtualTxt != NULL)
    delete BtnVirtualTxt;
  if (BtnLoadDbf != NULL)
    delete BtnLoadDbf;
  if (BtnVirtualDbf != NULL)
    delete BtnVirtualDbf;
  if (BtnLoadXL != NULL)
    delete BtnLoadXL;
  if (BtnVirtualXL != NULL)
    delete BtnVirtualXL;
  if (BtnNetwork != NULL)
    delete BtnNetwork;
  if (BtnExif != NULL)
    delete BtnExif;
  if (BtnGpsPics != NULL)
    delete BtnGpsPics;
  if (BtnLoadXml != NULL)
    delete BtnLoadXml;
  if (BtnSrids != NULL)
    delete BtnSrids;
  if (BtnCharset != NULL)
    delete BtnCharset;
  if (BtnWFS != NULL)
    delete BtnWFS;
  if (BtnDXF != NULL)
    delete BtnDXF;
  if (BtnHelp != NULL)
    delete BtnHelp;
  if (BtnAttach != NULL)
    delete BtnAttach;
  if (BtnSqlLog != NULL)
    delete BtnSqlLog;
  if (BtnDbStatus != NULL)
    delete BtnDbStatus;
  if (BtnCheckGeom != NULL)
    delete BtnCheckGeom;
  if (BtnSaneGeom != NULL)
    delete BtnSaneGeom;
  if (BtnAbout != NULL)
    delete BtnAbout;
  if (BtnExit != NULL)
    delete BtnExit;
  if (Charsets)
    delete[]Charsets;
  if (CharsetsNames)
    delete[]CharsetsNames;
  if (WmsCache != NULL)
    destroy_wms_cache(WmsCache);
  if (RL2PrivateData)
    rl2_cleanup_private(RL2PrivateData);
  if (DynamicLibPQ.IsLoaded() == true)
    DynamicLibPQ.Unload();
}

void MyFrame::SaveConfig()
{
//
// saves layout configuration
//
  wxConfig *config = new wxConfig(wxT("SpatialiteGui"));
  config->Write(wxT("Layout"), ConfigLayout);
  config->Write(wxT("PaneX"), ConfigPaneX);
  config->Write(wxT("PaneY"), ConfigPaneY);
  config->Write(wxT("PaneWidth"), ConfigPaneWidth);
  config->Write(wxT("PaneHeight"), ConfigPaneHeight);
  config->Write(wxT("SqlitePath"), SqlitePath);
  config->Write(wxT("LastDirectory"), LastDirectory);
  config->Write(wxT("WfsGetCapabilitiesURL"), WfsGetCapabilitiesURL);
  config->Write(wxT("HttpProxy"), HttpProxy);
  config->Write(wxT("ReadOnlyConnection"), ReadOnlyConnection);
  config->Write(wxT("RL2MaxThreads"), RL2MaxThreads);
  config->Write(wxT("MapMultiThreadingEnabled"), MapMultiThreadingEnabled);
  config->Write(wxT("MapMaxThreads"), MapMaxThreads);
  config->Write(wxT("MapAutoTransformEnabled"), MapAutoTransformEnabled);
  config->Write(wxT("PathLibPQ"), PathLibPQ);
  wxString proj_lib = wxString::FromUTF8(getenv("PROJ_LIB"));
  config->Write(wxT("ProjLib"), proj_lib);
  delete config;
}

void MyFrame::LoadConfig(wxString & externalPath)
{
//
// loads layout configuration
//
  ConfigLayout = wxT("");
  ConfigDbPath = wxT("");
  ConfigDir = wxT("");
  wxConfig *config = new wxConfig(wxT("SpatialiteGui"));
  config->Read(wxT("Layout"), &ConfigLayout);
  config->Read(wxT("PaneX"), &ConfigPaneX, -1);
  config->Read(wxT("PaneY"), &ConfigPaneY, -1);
  config->Read(wxT("PaneWidth"), &ConfigPaneWidth, -1);
  config->Read(wxT("PaneHeight"), &ConfigPaneHeight, -1);
  config->Read(wxT("SqlitePath"), &ConfigDbPath);
  config->Read(wxT("LastDirectory"), &ConfigDir);
  config->Read(wxT("WfsGetCapabilitiesURL"), &WfsGetCapabilitiesURL);
  config->Read(wxT("HttpProxy"), &HttpProxy);
  config->Read(wxT("ReadOnlyConnection"), &ReadOnlyConnection);
  config->Read(wxT("RL2MaxThreads"), &RL2MaxThreads);
  config->Read(wxT("MapMultiThreadingEnabled"), &MapMultiThreadingEnabled);
  config->Read(wxT("MapMaxThreads"), &MapMaxThreads);
  config->Read(wxT("MapAutoTransformEnabled"), &MapAutoTransformEnabled);
  config->Read(wxT("PathLibPQ"), &PathLibPQ);
  wxString proj_lib;
  config->Read(wxT("ProjLib"), &proj_lib);
  delete config;
  Hide();
  if (proj_lib.Len() > 0 && getenv("PROJ_LIB") == NULL)
    {
      // setting up PROJ_LIB if not already set
      char *dir = (char *) malloc(proj_lib.Len() * 4);
      strcpy(dir, proj_lib.ToUTF8());
#ifdef _WIN32
      char *var = sqlite3_mprintf("PROJ_LIB=%s", dir);
      putenv(var);
      sqlite3_free(var);
#else
      setenv("PROJ_LIB", dir, 1);
#endif
      free(dir);
    }
  if (externalPath.Len() > 0)
    {
      // applying the external path
      wxFileName file(externalPath);
      ConfigDir = file.GetPath();
      ConfigDbPath = externalPath;
    }
  if (ConfigLayout.Len() > 0)
    Manager.LoadPerspective(ConfigLayout, true);
  if (ConfigPaneX >= 0 && ConfigPaneY >= 0 && ConfigPaneWidth > 0
      && ConfigPaneHeight > 0)
    SetSize(ConfigPaneX, ConfigPaneY, ConfigPaneWidth, ConfigPaneHeight);
  if (ConfigDir.Len() > 0)
    LastDirectory = ConfigDir;
  if (ConfigDbPath.Len() > 0)
    {
      SqlitePath = ConfigDbPath;
      if (OpenDB(ReadOnlyConnection) == false)
        SqlitePath = wxT("");
      else
        {
          bool metadata = CheckMetadata();
          wxMenuBar *menuBar = GetMenuBar();
          menuBar->Enable(ID_Connect, false);
          menuBar->Enable(ID_Connect_RO, false);
          menuBar->Enable(ID_MemoryDbLoad, false);
          menuBar->Enable(ID_MemoryDbNew, false);
          if (MemoryDatabase == true)
            {
              menuBar->Enable(ID_MemoryDbSave, true);
              menuBar->Enable(ID_MemoryDbClock, true);
          } else
            {
              menuBar->Enable(ID_MemoryDbSave, false);
              menuBar->Enable(ID_MemoryDbClock, false);
            }
          menuBar->Enable(ID_CreateNew, false);
          menuBar->Enable(ID_Disconnect, true);
          menuBar->Enable(ID_Vacuum, true);
          menuBar->Enable(ID_Postgres, true);
          menuBar->Enable(ID_MapPanel, true);
          menuBar->Enable(ID_SqlScript, true);
          menuBar->Enable(ID_QueryViewComposer, HasViewsMetadata());
          menuBar->Enable(ID_LoadShp, true);
          menuBar->Enable(ID_VirtualShp, true);
          menuBar->Enable(ID_LoadGeoJSON, true);
          menuBar->Enable(ID_VirtualGeoJSON, true);
          menuBar->Enable(ID_LoadTxt, true);
          menuBar->Enable(ID_VirtualTxt, true);
          menuBar->Enable(ID_LoadDbf, true);
          menuBar->Enable(ID_VirtualDbf, true);
          menuBar->Enable(ID_LoadXL, true);
          menuBar->Enable(ID_VirtualXL, true);
          menuBar->Enable(ID_Network, true);
          menuBar->Enable(ID_Exif, true);
          menuBar->Enable(ID_GpsPics, true);
          menuBar->Enable(ID_LoadXml, true);
          menuBar->Enable(ID_WFS, true);
          menuBar->Enable(ID_DXF, true);
          menuBar->Enable(ID_Srids, metadata);
          menuBar->Enable(ID_Attach, true);
          menuBar->Enable(ID_SqlLog, true);
          menuBar->Enable(ID_DbStatus, true);
          menuBar->Enable(ID_CheckGeom, true);
          menuBar->Enable(ID_SaneGeom, true);
          EnableSqlLog();
          menuBar->Check(ID_SqlLog, SqlLogEnabled);
          wxToolBar *toolBar = GetToolBar();
          toolBar->EnableTool(ID_Connect, false);
          toolBar->EnableTool(ID_Connect_RO, false);
          toolBar->EnableTool(ID_MemoryDbLoad, false);
          toolBar->EnableTool(ID_MemoryDbNew, false);
          if (MemoryDatabase == true)
            {
              toolBar->EnableTool(ID_MemoryDbSave, true);
              toolBar->EnableTool(ID_MemoryDbClock, true);
          } else
            {
              toolBar->EnableTool(ID_MemoryDbSave, false);
              toolBar->EnableTool(ID_MemoryDbClock, false);
            }
          toolBar->EnableTool(ID_CreateNew, false);
          toolBar->EnableTool(ID_Disconnect, true);
          toolBar->EnableTool(ID_Vacuum, true);
          toolBar->EnableTool(ID_Postgres, true);
          toolBar->EnableTool(ID_MapPanel, true);
          toolBar->EnableTool(ID_SqlScript, true);
          toolBar->EnableTool(ID_QueryViewComposer, HasViewsMetadata());
          toolBar->EnableTool(ID_LoadShp, true);
          toolBar->EnableTool(ID_VirtualShp, true);
          toolBar->EnableTool(ID_LoadGeoJSON, true);
          toolBar->EnableTool(ID_VirtualGeoJSON, true);
          toolBar->EnableTool(ID_LoadTxt, true);
          toolBar->EnableTool(ID_VirtualTxt, true);
          toolBar->EnableTool(ID_LoadDbf, true);
          toolBar->EnableTool(ID_VirtualDbf, true);
          toolBar->EnableTool(ID_LoadXL, true);
          toolBar->EnableTool(ID_VirtualXL, true);
          toolBar->EnableTool(ID_Network, true);
          toolBar->EnableTool(ID_Exif, true);
          toolBar->EnableTool(ID_GpsPics, true);
          toolBar->EnableTool(ID_LoadXml, true);
          toolBar->EnableTool(ID_WFS, true);
          toolBar->EnableTool(ID_DXF, true);
          toolBar->EnableTool(ID_Srids, metadata);
          toolBar->EnableTool(ID_Attach, true);
          toolBar->EnableTool(ID_SqlLog, true);
          toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
          toolBar->EnableTool(ID_DbStatus, true);
          toolBar->EnableTool(ID_CheckGeom, true);
          toolBar->EnableTool(ID_SaneGeom, true);
          UpdateStatusBar();
        }
    }
  if (VirtualPQapiOK == false && SqliteHandle != NULL)
    {
      // attempting to attempting to load LibPQ
      if (PathLibPQ.Len() > 0)
        {
          // using the same path from the previous session
          DoLoadLibPQ(PathLibPQ);
        }
      if (VirtualPQapiOK == false)
        {
          // using just the library name
          wxString path;
#ifdef _WIN32
          path = wxDynamicLibrary::CanonicalizeName(wxT("libpq"));
#else
          path = wxDynamicLibrary::CanonicalizeName(wxT("pq"));
#endif
          DoLoadLibPQ(path);
        }
    }
  Show();
  if (AutoFDOmsg.Len() > 0)
    {
      wxMessageBox(AutoFDOmsg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                   this);
      AutoFDOmsg = wxT("");
    }
  if (AutoGPKGmsg.Len() > 0)
    {
      wxMessageBox(AutoGPKGmsg, wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      AutoGPKGmsg = wxT("");
    }
}

void MyFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// EXIT - event handler
//
  Close(true);
}

void MyFrame::OnClose(wxCloseEvent & WXUNUSED(event))
{
//
// immedtiately before closing the main window
//
  if (MapPanel != NULL)
    MapPanel->DetachAll();
  wxString db_path = SqlitePath;
  CloseDB();
  SqlitePath = db_path;
  Destroy();
}

void MyFrame::OnAttachDatabase(wxCommandEvent & WXUNUSED(event))
{
//
// Attach Database - event handler
//
  int ret;
  wxString lastDir;
  wxString path;
  wxString suffixList =
    wxT
    ("SpatiaLite DB (*.sqlite;*.atlas;*.db;*.gpkg)|*.sqlite;*.atlas;*.db;*.gpkg|");
  suffixList += wxT("SQLite DB (*.sqlite)|*.sqlite|");
  suffixList += wxT("LibreAtlas DB (*.atlas)|*.atlas|");
  suffixList += wxT("SQLite DB (*.db)|*.db|");
  suffixList += wxT("OGC GeoPackage (*.gpkg)|*.gpkg|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Attach DataBase"),
                          wxT(""), wxT("db.sqlite"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      if (IsValidSqliteFile(path) == false)
        return;
      if (DoAttachDatabase(path) == true)
        {
          wxFileName file(fileDialog.GetPath());
          lastDir = file.GetPath();
          SetLastDirectory(lastDir);
          InitTableTree();
        }
    }
}

bool MyFrame::IsValidSqliteFile(wxString & db_path)
{
// checking the internal Magic Number
  wxString msg = wxT("Unable to connect: ") + db_path;
  bool ret = false;
  char path[1024];
  char magic[16];
  FILE *in;
  strcpy(path, db_path.ToUTF8());
  in = fopen(path, "rb");
  if (in == NULL)
    {
      msg += wxT("\n\nNot existing file or forbidden access");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      return false;
    }
  if (fread(magic, 1, 16, in) == 16)
    {
      int count = 0;
      if (magic[0] == 0x53)
        count++;
      if (magic[1] == 0x51)
        count++;
      if (magic[2] == 0x4c)
        count++;
      if (magic[3] == 0x69)
        count++;
      if (magic[4] == 0x74)
        count++;
      if (magic[5] == 0x65)
        count++;
      if (magic[6] == 0x20)
        count++;
      if (magic[7] == 0x66)
        count++;
      if (magic[8] == 0x6f)
        count++;
      if (magic[9] == 0x72)
        count++;
      if (magic[10] == 0x6d)
        count++;
      if (magic[11] == 0x61)
        count++;
      if (magic[12] == 0x74)
        count++;
      if (magic[13] == 0x20)
        count++;
      if (magic[14] == 0x33)
        count++;
      if (magic[15] == 0x00)
        count++;
      if (count == 16)
        ret = true;
    }
  fclose(in);
  if (ret == false)
    {
      msg += wxT("\n\nNot a valid SQLite DB-file: invalid magic number");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }
  return ret;
}

void MyFrame::EnableSqlLog()
{
//
// enables the SQL Log if the currently connected DB is v.4.0.0
//
  if (GetMetaDataType() == METADATA_CURRENT)
    SqlLogEnabled = true;
  else
    SqlLogEnabled = false;
}

void MyFrame::OnSqlLog(wxCommandEvent & WXUNUSED(event))
{
//
// SQL Log - event handler
//
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  if (SqlLogEnabled == true)
    {
      SqlLogEnabled = false;
      menuBar->Check(ID_SqlLog, false);
      toolBar->ToggleTool(ID_SqlLog, false);
  } else
    {
      EnableSqlLog();
      menuBar->Check(ID_SqlLog, SqlLogEnabled);
      toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
    }
}

void MyFrame::OnDbStatus(wxCommandEvent & WXUNUSED(event))
{
//
// DB Status and internal statistics
//
  DbStatusDialog *stats = new DbStatusDialog();
  stats->Create(this);
  stats->Show();
}

void MyFrame::OnCheckGeometries(wxCommandEvent & WXUNUSED(event))
{
//
// Checking all geometry columns
//
  wxString msg =
    wxT("Do you really intend checking for validity all Geometries ?\n\n");
  msg +=
    wxT
    ("This will imply evaluating ST_IsValid() for each single Geometry stored\n");
  msg +=
    wxT
    ("within any \"layer\" defined in \"geometry_columns\", and could require\n");
  msg += wxT("a substantial time for a huge DB\n\n");
  msg += wxT("A HTML diagnostic report will be created.");
  int ret = wxMessageBox(msg, wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION,
                         this);
  if (ret != wxYES)
    return;

  char *err_msg = NULL;
  char output_dir[1024];
  wxDirDialog dirDialog(this, wxT("Directory for Diagnostic Report"));
  ret = dirDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(output_dir, dirDialog.GetPath().ToUTF8());
      ::wxBeginBusyCursor();
      int invalids;
      ret =
        check_all_geometry_columns_r(GetSpliteInternalCache(), GetSqlite(),
                                     output_dir, &invalids, &err_msg);
      ::wxEndBusyCursor();
      if (ret == 0)
        {
          // reporting some error condition
          msg = wxT("Some unexpected error occurred:\n\n");
          if (err_msg != NULL)
            {
              msg += wxString::FromUTF8(err_msg);
              free(err_msg);
          } else
            msg += wxT("Sorry, no further details are available");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      } else
        {
          int mode;
          if (invalids > 0)
            {
              msg =
                wxT
                ("ATTENTION: some invalid Geometries have been detected !!!\n\n");
              mode = wxICON_WARNING;
          } else
            {
              msg =
                wxT
                ("No invalid Geometries have been detected; the whole DB is full valid\n\n");
              mode = wxICON_INFORMATION;
            }
          msg += wxT("A full diagnostic report has been created.\n");
          msg +=
            wxT
            ("Please point your WEB Browser at the following HTML document containing the report:\n\n");
#if defined(_WIN32)
          const char *delim = "\\";
#else
          const char *delim = "/";
#endif
          msg +=
            dirDialog.GetPath() + wxString::FromUTF8(delim) + wxT("index.html");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | mode, this);
        }
    }
}

void MyFrame::OnSanitizeGeometries(wxCommandEvent & WXUNUSED(event))
{
//
// Sanitizing all invalid geometries
//
  int ret;
  SanitizeAllGeometriesDialog dlg;
  dlg.Create(this);
  ret = dlg.ShowModal();
  if (ret != wxYES)
    return;

  char tmp_prefix[1024];
  char *err_msg = NULL;
  char output_dir[1024];
  wxString msg;
  strcpy(tmp_prefix, dlg.GetTmpPrefix().ToUTF8());

  wxDirDialog dirDialog(this, wxT("Directory for Diagnostic Report"));
  ret = dirDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(output_dir, dirDialog.GetPath().ToUTF8());
      ::wxBeginBusyCursor();
      int not_validated;
      ret =
        sanitize_all_geometry_columns_r(GetSpliteInternalCache(), GetSqlite(),
                                        tmp_prefix, output_dir, &not_validated,
                                        &err_msg);
      ::wxEndBusyCursor();
      if (ret == 0)
        {
          // reporting some error condition
          msg = wxT("Some unexpected error occurred:\n\n");
          if (err_msg != NULL)
            {
              msg += wxString::FromUTF8(err_msg);
              free(err_msg);
          } else
            msg += wxT("Sorry, no further details are available");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      } else
        {
          int mode;
          if (not_validated > 0)
            {
              msg =
                wxT
                ("ATTENTION: some invalid Geometries still remain invalid !!!\n\n");
              mode = wxICON_WARNING;
          } else
            {
              msg =
                wxT
                ("All invalid Geometries have been saned; the whole DB is now full valid\n\n");
              mode = wxICON_INFORMATION;
            }
          msg += wxT("A full diagnostic report has been created.\n");
          msg +=
            wxT
            ("Please point your WEB Browser at the following HTML document containing the report:\n\n");
#if defined(_WIN32)
          const char *delim = "\\";
#else
          const char *delim = "/";
#endif
          msg +=
            dirDialog.GetPath() + wxString::FromUTF8(delim) + wxT("index.html");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | mode, this);
        }
    }
}

void MyFrame::OnProjLib(wxCommandEvent & WXUNUSED(event))
{
//
// Setting the PROJ_LIB environment variable
//
  int ret;
  wxString lastDir;
  wxString path;
  wxString suffixList = wxT("PROJ.6 database (*.db)|*.db|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("PROJ.6 own DataBase"),
                          wxT(""), wxT("proj.db"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = wxString::FromUTF8(getenv("PROJ_LIB"));
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      char *dir = (char *) malloc(lastDir.Len() * 4);
      strcpy(dir, lastDir.ToUTF8());
#ifdef _WIN32
      char *var = sqlite3_mprintf("PROJ_LIB=%s", dir);
      putenv(var);
      sqlite3_free(var);
#else
      setenv("PROJ_LIB", dir, 1);
#endif
      free(dir);
    }
}

void MyFrame::OnHelp(wxCommandEvent & WXUNUSED(event))
{
//
// HELP - event handler
//
  if (HelpPane == true)
    return;
  HelpDialog *help = new HelpDialog(this);
  help->Show();
}

void MyFrame::OnAbout(wxCommandEvent & WXUNUSED(event))
{
//
// ABOUT dialog - event handler
//
  char ver[128];
  wxAboutDialogInfo dlg;
  dlg.SetIcon(wxIcon(icon_info_xpm));
  dlg.SetName(wxT("spatialite_gui"));
  const char *version = VERSION;
  dlg.SetVersion(wxString::FromUTF8(version));
  wxString str = wxT("a GUI-tool for SQLite / SpatiaLite\n\n");
  sprintf(ver, "%d.%d.%d", wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);
  str += wxT("wxWidgets version ") + wxString::FromUTF8(ver) + wxT("\n");
  strcpy(ver, spatialite_version());
  str += wxT("SpatiaLite version ") + wxString::FromUTF8(ver) + wxT("\n");
  strcpy(ver, rl2_version());
  str += wxT("RasterLite2 version ") + wxString::FromUTF8(ver) + wxT("\n");
  strcpy(ver, sqlite3_libversion());
  str += wxT("SQLite version ") + wxString::FromUTF8(ver) + wxT("\n");
  strcpy(ver, GEOSversion());
  str += wxT("GEOS version ") + wxString::FromUTF8(ver) + wxT("\n");
#ifdef PROJ_NEW                 /* supporting new PROJ.6 */
  strcpy(ver, pj_release);
#else /* supporting old PROJ.4 */
  strcpy(ver, pj_get_release());
#endif
  str += wxT("PROJ version ") + wxString::FromUTF8(ver) + wxT("\n");
  if (GetRtTopoVersion(ver))
    {
      // printing out the RTTOPO version if supported
      str += wxT("RTTOPO version ") + wxString::FromUTF8(ver) + wxT("\n");
    }
  if (GetLibXml2Version(ver))
    {
      // printing out the LIBXML2 version if supported
      str += wxT("LIBXML2 version ") + wxString::FromUTF8(ver) + wxT("\n");
    }
  strcpy(ver, virtualpg_version());
  str += wxT("VirtualPG version ") + wxString::FromUTF8(ver) + wxT("\n");
  if (VirtualPQapiOK == true)
    {
      // only if LiqPQ has already been loaded
      wxString pqver;
      GetPQlibVersion(pqver);
      str += wxT("PQlib (PostgreSQL client) version ") + pqver + wxT("\n");
    }
  str +=
    wxT("\nSQLite's extensions: 'SpatiaLite', 'VirtualShape', 'VirtualDbf',\n");
  str += wxT("'VirtualXL', 'VirtualText', 'VirtualXPath', 'VirtualNetwork',\n");
  str +=
    wxT("'RTree', 'MbrCache', 'VirtualBBox', 'VirtualFDO', 'VirtualGPKG',\n");
  str += wxT("'RasterLite2'\n\n");
  strcpy(ver, spatialite_target_cpu());
  str += wxT("Target CPU ") + wxString::FromUTF8(ver) + wxT("\n");
  dlg.SetDescription(str);
  dlg.SetCopyright(wxT("by Alessandro Furieri - 2008/2017"));
  dlg.SetWebSite(wxT("http://www.gaia-gis.it"));
  wxString license =
    wxT("This program is free software; you can redistribute it\n");
  license +=
    wxT("and/or modify it under the terms of the GNU General Public License\n");
  license += wxT("(GPL) as published by the Free Software Foundation\n\n");
  license +=
    wxT
    ("A copy of the GPL can be found at\nhttp://www.gnu.org/licenses/gpl.txt");
  dlg.SetLicense(license);
  ::wxAboutBox(dlg);
}

void MyFrame::OnMouseMove(wxMouseEvent & WXUNUSED(event))
{
//
// MOUSE motion - event handler
//
  UpdateStatusBar(false);
}

void MyFrame::UpdateStatusBar(bool changeIcon)
{
//
// updating the status bar
//
  wxString msg;
  if (StatusBar == NULL)
    return;
  if (changeIcon == true)
    {
      // changing the current Icon 
      if (IsConnected() == true)
        {
          if (ReadOnlyConnection == true)
            StatusBar->SetReadOnlyIcon();
          else
            {
              // showing the currently set SECURITY LEVEL
              if (SecurityRelaxed == true)
                StatusBar->SetSecurityRelaxedIcon();
              else
                StatusBar->SetSecurityStrongIcon();
            }
      } else
        StatusBar->SetNotConnectedIcon();
    }
  if (MemoryDatabase == true)
    {
      msg = wxT("Current SQLite DB: MEMORY-DB");
      StatusBar->SetText(msg);
      QueryView->ShowControls();
  } else
    {
      if (SqlitePath.Len() < 1)
        {
          msg = wxT("not connected");
          StatusBar->SetText(msg);
      } else
        {
          if (ReadOnlyConnection)
            msg = wxT("Current SQLite DB [READ-ONLY]: ") + SqlitePath;
          else
            msg = wxT("Current SQLite DB: ") + SqlitePath;
          StatusBar->SetText(msg);
        }
      if (SqlitePath.Len() < 1)
        {
          QueryView->HideControls();
          RsView->HideControls();
      } else
        QueryView->ShowControls();
    }
}

void MyFrame::OnConnect(wxCommandEvent & WXUNUSED(event))
{
//
// connecting to an existent SQLite DB
//
  int ret;
  wxString lastDir;
  wxString suffixList =
    wxT
    ("SpatiaLite DB (*.sqlite;*.atlas;*.db;*.gpkg)|*.sqlite;*.atlas;*.db;*.gpkg|");
  suffixList += wxT("SQLite DB (*.sqlite)|*.sqlite|");
  suffixList += wxT("LibreAtlas DB (*.atlas)|*.atlas|");
  suffixList += wxT("SQLite DB (*.db)|*.db|");
  suffixList += wxT("OGC GeoPackage (*.gpkg)|*.gpkg|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("DB connection"),
                          wxT(""), wxT("db.sqlite"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      SqlitePath = fileDialog.GetPath();
      if (IsValidSqliteFile(SqlitePath) == false)
        {
          SqlitePath = wxT("");
          return;
        }
      if (OpenDB(false) == false)
        SqlitePath = wxT("");
      else
        {
          wxFileName file(fileDialog.GetPath());
          lastDir = file.GetPath();
          SetLastDirectory(lastDir);
          bool metadata = CheckMetadata();
          wxMenuBar *menuBar = GetMenuBar();
          menuBar->Enable(ID_Connect, false);
          menuBar->Enable(ID_Connect_RO, false);
          menuBar->Enable(ID_CreateNew, false);
          menuBar->Enable(ID_Disconnect, true);
          menuBar->Enable(ID_MemoryDbLoad, false);
          menuBar->Enable(ID_MemoryDbNew, false);
          if (MemoryDatabase == true)
            {
              menuBar->Enable(ID_MemoryDbSave, true);
              menuBar->Enable(ID_MemoryDbClock, true);
          } else
            {
              menuBar->Enable(ID_MemoryDbSave, false);
              menuBar->Enable(ID_MemoryDbClock, false);
            }
          menuBar->Enable(ID_Vacuum, true);
          menuBar->Enable(ID_Postgres, true);
          menuBar->Enable(ID_MapPanel, true);
          menuBar->Enable(ID_SqlScript, true);
          menuBar->Enable(ID_QueryViewComposer, HasViewsMetadata());
          menuBar->Enable(ID_LoadShp, true);
          menuBar->Enable(ID_VirtualShp, true);
          menuBar->Enable(ID_LoadGeoJSON, true);
          menuBar->Enable(ID_VirtualGeoJSON, true);
          menuBar->Enable(ID_LoadTxt, true);
          menuBar->Enable(ID_VirtualTxt, true);
          menuBar->Enable(ID_LoadDbf, true);
          menuBar->Enable(ID_VirtualDbf, true);
          menuBar->Enable(ID_LoadXL, true);
          menuBar->Enable(ID_VirtualXL, true);
          menuBar->Enable(ID_Network, true);
          menuBar->Enable(ID_Exif, true);
          menuBar->Enable(ID_GpsPics, true);
          menuBar->Enable(ID_LoadXml, true);
          menuBar->Enable(ID_WFS, true);
          menuBar->Enable(ID_DXF, true);
          menuBar->Enable(ID_Srids, metadata);
          menuBar->Enable(ID_Attach, true);
          menuBar->Enable(ID_SqlLog, true);
          menuBar->Enable(ID_DbStatus, true);
          menuBar->Enable(ID_CheckGeom, true);
          menuBar->Enable(ID_SaneGeom, true);
          EnableSqlLog();
          menuBar->Check(ID_SqlLog, SqlLogEnabled);
          wxToolBar *toolBar = GetToolBar();
          toolBar->EnableTool(ID_Connect, false);
          toolBar->EnableTool(ID_Connect_RO, false);
          toolBar->EnableTool(ID_CreateNew, false);
          toolBar->EnableTool(ID_Disconnect, true);
          toolBar->EnableTool(ID_MemoryDbLoad, false);
          toolBar->EnableTool(ID_MemoryDbNew, false);
          if (MemoryDatabase == true)
            {
              toolBar->EnableTool(ID_MemoryDbSave, true);
              toolBar->EnableTool(ID_MemoryDbClock, true);
          } else
            {
              toolBar->EnableTool(ID_MemoryDbSave, false);
              toolBar->EnableTool(ID_MemoryDbClock, false);
            }
          toolBar->EnableTool(ID_Vacuum, true);
          toolBar->EnableTool(ID_Postgres, true);
          toolBar->EnableTool(ID_MapPanel, true);
          toolBar->EnableTool(ID_SqlScript, true);
          toolBar->EnableTool(ID_QueryViewComposer, HasViewsMetadata());
          toolBar->EnableTool(ID_LoadShp, true);
          toolBar->EnableTool(ID_VirtualShp, true);
          toolBar->EnableTool(ID_LoadGeoJSON, true);
          toolBar->EnableTool(ID_VirtualGeoJSON, true);
          toolBar->EnableTool(ID_LoadTxt, true);
          toolBar->EnableTool(ID_VirtualTxt, true);
          toolBar->EnableTool(ID_LoadDbf, true);
          toolBar->EnableTool(ID_VirtualDbf, true);
          toolBar->EnableTool(ID_LoadXL, true);
          toolBar->EnableTool(ID_VirtualXL, true);
          toolBar->EnableTool(ID_Network, true);
          toolBar->EnableTool(ID_Exif, true);
          toolBar->EnableTool(ID_GpsPics, true);
          toolBar->EnableTool(ID_LoadXml, true);
          toolBar->EnableTool(ID_WFS, true);
          toolBar->EnableTool(ID_DXF, true);
          toolBar->EnableTool(ID_Srids, metadata);
          toolBar->EnableTool(ID_Attach, true);
          toolBar->EnableTool(ID_SqlLog, true);
          toolBar->EnableTool(ID_DbStatus, true);
          toolBar->EnableTool(ID_CheckGeom, true);
          toolBar->EnableTool(ID_SaneGeom, true);
          toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
          UpdateStatusBar();
        }
    }
  if (AutoFDOmsg.Len() > 0)
    {
      wxMessageBox(AutoFDOmsg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                   this);
      AutoFDOmsg = wxT("");
    }
  if (AutoGPKGmsg.Len() > 0)
    {
      wxMessageBox(AutoGPKGmsg, wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      AutoGPKGmsg = wxT("");
    }
}

void MyFrame::EnableAllTools(bool mode)
{
//
// enabling - disabling all buttons while executing
// a threaded sql query
//
  bool mode2;
  bool mode3;
  if (mode == true)
    {
      mode2 = HasViewsMetadata();
      mode3 = CheckMetadata();
  } else
    {
      mode2 = false;
      mode3 = false;
    }

  wxMenuBar *menuBar = GetMenuBar();
  menuBar->Enable(ID_Disconnect, mode);
  menuBar->Enable(ID_Vacuum, mode);
  menuBar->Enable(ID_Postgres, mode);
  menuBar->Enable(ID_MapPanel, mode);
  menuBar->Enable(ID_SqlScript, mode);
  menuBar->Enable(ID_QueryViewComposer, mode2);
  menuBar->Enable(ID_LoadShp, mode);
  menuBar->Enable(ID_VirtualShp, mode);
  menuBar->Enable(ID_LoadGeoJSON, mode);
  menuBar->Enable(ID_VirtualGeoJSON, mode);
  menuBar->Enable(ID_LoadTxt, mode);
  menuBar->Enable(ID_VirtualTxt, mode);
  menuBar->Enable(ID_LoadDbf, mode);
  menuBar->Enable(ID_VirtualDbf, mode);
  menuBar->Enable(ID_LoadXL, mode);
  menuBar->Enable(ID_VirtualXL, mode);
  menuBar->Enable(ID_Network, mode);
  menuBar->Enable(ID_Exif, mode);
  menuBar->Enable(ID_GpsPics, mode);
  menuBar->Enable(ID_LoadXml, mode);
  menuBar->Enable(ID_WFS, mode);
  menuBar->Enable(ID_DXF, mode);
  menuBar->Enable(ID_Srids, mode3);
  menuBar->Enable(ID_Attach, mode);
  menuBar->Enable(ID_SqlLog, mode);
  menuBar->Enable(ID_DbStatus, mode);
  menuBar->Enable(ID_CheckGeom, mode);
  menuBar->Enable(ID_SaneGeom, mode);
  wxToolBar *toolBar = GetToolBar();
  toolBar->EnableTool(ID_Disconnect, mode);
  toolBar->EnableTool(ID_Vacuum, mode);
  toolBar->EnableTool(ID_Postgres, mode);
  toolBar->EnableTool(ID_MapPanel, mode);
  toolBar->EnableTool(ID_SqlScript, mode);
  toolBar->EnableTool(ID_QueryViewComposer, mode2);
  toolBar->EnableTool(ID_LoadShp, mode);
  toolBar->EnableTool(ID_VirtualShp, mode);
  toolBar->EnableTool(ID_LoadGeoJSON, mode);
  toolBar->EnableTool(ID_VirtualGeoJSON, mode);
  toolBar->EnableTool(ID_LoadTxt, mode);
  toolBar->EnableTool(ID_VirtualTxt, mode);
  toolBar->EnableTool(ID_LoadDbf, mode);
  toolBar->EnableTool(ID_VirtualDbf, mode);
  toolBar->EnableTool(ID_LoadXL, mode);
  toolBar->EnableTool(ID_VirtualXL, mode);
  toolBar->EnableTool(ID_Network, mode);
  toolBar->EnableTool(ID_Exif, mode);
  toolBar->EnableTool(ID_GpsPics, mode);
  toolBar->EnableTool(ID_LoadXml, mode);
  toolBar->EnableTool(ID_WFS, mode);
  toolBar->EnableTool(ID_DXF, mode);
  toolBar->EnableTool(ID_Srids, mode3);
  toolBar->EnableTool(ID_Attach, mode);
  toolBar->EnableTool(ID_SqlLog, mode);
  toolBar->EnableTool(ID_DbStatus, mode);
  toolBar->EnableTool(ID_CheckGeom, mode);
  toolBar->EnableTool(ID_SaneGeom, mode);
}

void MyFrame::OnConnectReadOnly(wxCommandEvent & WXUNUSED(event))
{
//
// connecting to an existent SQLite DB (Read Only)
//
  int ret;
  wxString lastDir;
  wxString suffixList =
    wxT
    ("SpatiaLite DB (*.sqlite;*.atlas;*.db;*.gpkg)|*.sqlite;*.atlas;*.db;*.gpkg|");
  suffixList += wxT("SQLite DB (*.sqlite)|*.sqlite|");
  suffixList += wxT("LibreAtlas DB (*.atlas)|*.atlas|");
  suffixList += wxT("SQLite DB (*.db)|*.db|");
  suffixList += wxT("OGC GeoPackage (*.gpkg)|*.gpkg|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("DB connection"),
                          wxT(""), wxT("db.sqlite"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      SqlitePath = fileDialog.GetPath();
      if (IsValidSqliteFile(SqlitePath) == false)
        {
          SqlitePath = wxT("");
          return;
        }
      if (OpenDB(true) == false)
        SqlitePath = wxT("");
      else
        {
          wxFileName file(fileDialog.GetPath());
          lastDir = file.GetPath();
          SetLastDirectory(lastDir);
          bool metadata = CheckMetadata();
          wxMenuBar *menuBar = GetMenuBar();
          menuBar->Enable(ID_Connect, false);
          menuBar->Enable(ID_Connect_RO, false);
          menuBar->Enable(ID_CreateNew, false);
          menuBar->Enable(ID_Disconnect, true);
          menuBar->Enable(ID_MemoryDbLoad, false);
          menuBar->Enable(ID_MemoryDbNew, false);
          if (MemoryDatabase == true)
            {
              menuBar->Enable(ID_MemoryDbSave, true);
              menuBar->Enable(ID_MemoryDbClock, true);
          } else
            {
              menuBar->Enable(ID_MemoryDbSave, false);
              menuBar->Enable(ID_MemoryDbClock, false);
            }
          menuBar->Enable(ID_Vacuum, true);
          menuBar->Enable(ID_Postgres, true);
          menuBar->Enable(ID_MapPanel, true);
          menuBar->Enable(ID_SqlScript, true);
          menuBar->Enable(ID_QueryViewComposer, HasViewsMetadata());
          menuBar->Enable(ID_LoadShp, true);
          menuBar->Enable(ID_VirtualShp, true);
          menuBar->Enable(ID_LoadGeoJSON, true);
          menuBar->Enable(ID_VirtualGeoJSON, true);
          menuBar->Enable(ID_LoadTxt, true);
          menuBar->Enable(ID_VirtualTxt, true);
          menuBar->Enable(ID_LoadDbf, true);
          menuBar->Enable(ID_VirtualDbf, true);
          menuBar->Enable(ID_LoadXL, true);
          menuBar->Enable(ID_VirtualXL, true);
          menuBar->Enable(ID_Network, true);
          menuBar->Enable(ID_Exif, true);
          menuBar->Enable(ID_GpsPics, true);
          menuBar->Enable(ID_LoadXml, true);
          menuBar->Enable(ID_WFS, true);
          menuBar->Enable(ID_DXF, true);
          menuBar->Enable(ID_Srids, metadata);
          menuBar->Enable(ID_Attach, true);
          menuBar->Enable(ID_SqlLog, true);
          menuBar->Enable(ID_DbStatus, true);
          menuBar->Enable(ID_CheckGeom, true);
          menuBar->Enable(ID_SaneGeom, true);
          EnableSqlLog();
          menuBar->Check(ID_SqlLog, SqlLogEnabled);
          wxToolBar *toolBar = GetToolBar();
          toolBar->EnableTool(ID_Connect, false);
          toolBar->EnableTool(ID_Connect_RO, false);
          toolBar->EnableTool(ID_CreateNew, false);
          toolBar->EnableTool(ID_Disconnect, true);
          toolBar->EnableTool(ID_MemoryDbLoad, false);
          toolBar->EnableTool(ID_MemoryDbNew, false);
          if (MemoryDatabase == true)
            {
              toolBar->EnableTool(ID_MemoryDbSave, true);
              toolBar->EnableTool(ID_MemoryDbClock, true);
          } else
            {
              toolBar->EnableTool(ID_MemoryDbSave, false);
              toolBar->EnableTool(ID_MemoryDbClock, false);
            }
          toolBar->EnableTool(ID_Vacuum, true);
          toolBar->EnableTool(ID_Postgres, true);
          toolBar->EnableTool(ID_MapPanel, true);
          toolBar->EnableTool(ID_SqlScript, true);
          toolBar->EnableTool(ID_QueryViewComposer, HasViewsMetadata());
          toolBar->EnableTool(ID_LoadShp, true);
          toolBar->EnableTool(ID_VirtualShp, true);
          toolBar->EnableTool(ID_LoadGeoJSON, true);
          toolBar->EnableTool(ID_VirtualGeoJSON, true);
          toolBar->EnableTool(ID_LoadTxt, true);
          toolBar->EnableTool(ID_VirtualTxt, true);
          toolBar->EnableTool(ID_LoadDbf, true);
          toolBar->EnableTool(ID_VirtualDbf, true);
          toolBar->EnableTool(ID_LoadXL, true);
          toolBar->EnableTool(ID_VirtualXL, true);
          toolBar->EnableTool(ID_Network, true);
          toolBar->EnableTool(ID_Exif, true);
          toolBar->EnableTool(ID_GpsPics, true);
          toolBar->EnableTool(ID_LoadXml, true);
          toolBar->EnableTool(ID_WFS, true);
          toolBar->EnableTool(ID_DXF, true);
          toolBar->EnableTool(ID_Srids, metadata);
          toolBar->EnableTool(ID_Attach, true);
          toolBar->EnableTool(ID_SqlLog, true);
          toolBar->EnableTool(ID_DbStatus, true);
          toolBar->EnableTool(ID_CheckGeom, true);
          toolBar->EnableTool(ID_SaneGeom, true);
          toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
          UpdateStatusBar();
        }
    }
  if (AutoFDOmsg.Len() > 0)
    {
      wxMessageBox(AutoFDOmsg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                   this);
      AutoFDOmsg = wxT("");
    }
  if (AutoGPKGmsg.Len() > 0)
    {
      wxMessageBox(AutoGPKGmsg, wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      AutoGPKGmsg = wxT("");
    }
}

void MyFrame::OnDisconnect(wxCommandEvent & WXUNUSED(event))
{
//
// disconnecting current SQLite DB
//
  if (TimerAutoSave)
    {
      TimerAutoSave->Stop();
      delete TimerAutoSave;
      TimerAutoSave = NULL;
    }
  CloseDB();
  ExternalSqlitePath = wxT("");
  GetRsView()->ResetEmpty();
  wxMenuBar *menuBar = GetMenuBar();
  menuBar->Enable(ID_Connect, true);
  menuBar->Enable(ID_Connect_RO, true);
  menuBar->Enable(ID_CreateNew, true);
  menuBar->Enable(ID_Disconnect, false);
  menuBar->Enable(ID_MemoryDbLoad, true);
  menuBar->Enable(ID_MemoryDbNew, true);
  menuBar->Enable(ID_MemoryDbSave, false);
  menuBar->Enable(ID_MemoryDbClock, false);
  menuBar->Enable(ID_Vacuum, false);
  menuBar->Enable(ID_Postgres, false);
  menuBar->Enable(ID_MapPanel, false);
  menuBar->Enable(ID_SqlScript, false);
  menuBar->Enable(ID_QueryViewComposer, false);
  menuBar->Enable(ID_LoadShp, false);
  menuBar->Enable(ID_VirtualShp, false);
  menuBar->Enable(ID_LoadGeoJSON, false);
  menuBar->Enable(ID_VirtualGeoJSON, false);
  menuBar->Enable(ID_LoadTxt, false);
  menuBar->Enable(ID_VirtualTxt, false);
  menuBar->Enable(ID_LoadDbf, false);
  menuBar->Enable(ID_VirtualDbf, false);
  menuBar->Enable(ID_LoadXL, false);
  menuBar->Enable(ID_VirtualXL, false);
  menuBar->Enable(ID_Network, false);
  menuBar->Enable(ID_Exif, false);
  menuBar->Enable(ID_GpsPics, false);
  menuBar->Enable(ID_LoadXml, false);
  menuBar->Enable(ID_WFS, false);
  menuBar->Enable(ID_DXF, false);
  menuBar->Enable(ID_Srids, false);
  menuBar->Enable(ID_Attach, false);
  menuBar->Enable(ID_SqlLog, false);
  menuBar->Enable(ID_DbStatus, false);
  menuBar->Enable(ID_CheckGeom, false);
  menuBar->Enable(ID_SaneGeom, false);
  SqlLogEnabled = false;
  wxToolBar *toolBar = GetToolBar();
  toolBar->EnableTool(ID_Connect, true);
  toolBar->EnableTool(ID_Connect_RO, true);
  toolBar->EnableTool(ID_CreateNew, true);
  toolBar->EnableTool(ID_Disconnect, false);
  toolBar->EnableTool(ID_MemoryDbLoad, true);
  toolBar->EnableTool(ID_MemoryDbNew, true);
  toolBar->EnableTool(ID_MemoryDbSave, false);
  toolBar->EnableTool(ID_MemoryDbClock, false);
  toolBar->EnableTool(ID_Vacuum, false);
  toolBar->EnableTool(ID_Postgres, false);
  toolBar->EnableTool(ID_MapPanel, false);
  toolBar->EnableTool(ID_SqlScript, false);
  toolBar->EnableTool(ID_QueryViewComposer, false);
  toolBar->EnableTool(ID_LoadShp, false);
  toolBar->EnableTool(ID_VirtualShp, false);
  toolBar->EnableTool(ID_LoadGeoJSON, false);
  toolBar->EnableTool(ID_VirtualGeoJSON, false);
  toolBar->EnableTool(ID_LoadTxt, false);
  toolBar->EnableTool(ID_VirtualTxt, false);
  toolBar->EnableTool(ID_LoadDbf, false);
  toolBar->EnableTool(ID_VirtualDbf, false);
  toolBar->EnableTool(ID_LoadXL, false);
  toolBar->EnableTool(ID_VirtualXL, false);
  toolBar->EnableTool(ID_Network, false);
  toolBar->EnableTool(ID_Exif, false);
  toolBar->EnableTool(ID_GpsPics, false);
  toolBar->EnableTool(ID_LoadXml, false);
  toolBar->EnableTool(ID_WFS, false);
  toolBar->EnableTool(ID_DXF, false);
  toolBar->EnableTool(ID_Srids, false);
  toolBar->EnableTool(ID_Attach, false);
  toolBar->EnableTool(ID_SqlLog, false);
  toolBar->ToggleTool(ID_SqlLog, false);
  toolBar->EnableTool(ID_DbStatus, false);
  toolBar->ToggleTool(ID_DbStatus, false);
  toolBar->EnableTool(ID_CheckGeom, false);
  toolBar->ToggleTool(ID_CheckGeom, false);
  toolBar->EnableTool(ID_SaneGeom, false);
  toolBar->ToggleTool(ID_SaneGeom, false);
  UpdateStatusBar();
}

void MyFrame::OnCreateNew(wxCommandEvent & WXUNUSED(event))
{
//
// creating a new, empty SQLite DB
//
  int retdlg;
  int ret;
  wxString lastDir;
  bool metadata;
  wxFileDialog fileDialog(this, wxT("Creating a new, empty DB"),
                          wxT(""), wxT("db.sqlite"),
                          wxT
                          ("SQLite DB (*.sqlite)|*.sqlite|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  retdlg = fileDialog.ShowModal();
  if (retdlg == wxID_OK)
    {
      // creating the new DB
      SqlitePath = fileDialog.GetPath();
      ret = CreateDB();
      if (ret == false)
        goto error;
      metadata = CheckMetadata();
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      wxMenuBar *menuBar = GetMenuBar();
      menuBar->Enable(ID_Connect, false);
      menuBar->Enable(ID_Connect_RO, false);
      menuBar->Enable(ID_CreateNew, false);
      menuBar->Enable(ID_Disconnect, true);
      menuBar->Enable(ID_MemoryDbLoad, false);
      menuBar->Enable(ID_MemoryDbNew, false);
      menuBar->Enable(ID_MemoryDbSave, false);
      menuBar->Enable(ID_MemoryDbClock, false);
      menuBar->Enable(ID_Vacuum, true);
      menuBar->Enable(ID_Postgres, true);
      menuBar->Enable(ID_MapPanel, true);
      menuBar->Enable(ID_SqlScript, true);
      menuBar->Enable(ID_QueryViewComposer, HasViewsMetadata());
      menuBar->Enable(ID_LoadShp, true);
      menuBar->Enable(ID_VirtualShp, true);
      menuBar->Enable(ID_LoadGeoJSON, true);
      menuBar->Enable(ID_VirtualGeoJSON, true);
      menuBar->Enable(ID_LoadTxt, true);
      menuBar->Enable(ID_VirtualTxt, true);
      menuBar->Enable(ID_LoadDbf, true);
      menuBar->Enable(ID_VirtualDbf, true);
      menuBar->Enable(ID_LoadXL, true);
      menuBar->Enable(ID_VirtualXL, true);
      menuBar->Enable(ID_Network, true);
      menuBar->Enable(ID_Exif, true);
      menuBar->Enable(ID_GpsPics, true);
      menuBar->Enable(ID_LoadXml, true);
      menuBar->Enable(ID_WFS, true);
      menuBar->Enable(ID_DXF, true);
      menuBar->Enable(ID_Srids, metadata);
      menuBar->Enable(ID_Attach, true);
      menuBar->Enable(ID_SqlLog, true);
      menuBar->Enable(ID_DbStatus, true);
      menuBar->Enable(ID_CheckGeom, true);
      menuBar->Enable(ID_SaneGeom, true);
      EnableSqlLog();
      menuBar->Check(ID_SqlLog, SqlLogEnabled);
      wxToolBar *toolBar = GetToolBar();
      toolBar->EnableTool(ID_Connect, false);
      toolBar->EnableTool(ID_Connect_RO, false);
      toolBar->EnableTool(ID_CreateNew, false);
      toolBar->EnableTool(ID_Disconnect, true);
      toolBar->EnableTool(ID_MemoryDbLoad, false);
      toolBar->EnableTool(ID_MemoryDbNew, false);
      toolBar->EnableTool(ID_MemoryDbSave, false);
      toolBar->EnableTool(ID_MemoryDbClock, false);
      toolBar->EnableTool(ID_Vacuum, true);
      toolBar->EnableTool(ID_Postgres, true);
      toolBar->EnableTool(ID_MapPanel, true);
      toolBar->EnableTool(ID_SqlScript, true);
      toolBar->EnableTool(ID_QueryViewComposer, HasViewsMetadata());
      toolBar->EnableTool(ID_LoadShp, true);
      toolBar->EnableTool(ID_VirtualShp, true);
      toolBar->EnableTool(ID_LoadGeoJSON, true);
      toolBar->EnableTool(ID_VirtualGeoJSON, true);
      toolBar->EnableTool(ID_LoadTxt, true);
      toolBar->EnableTool(ID_VirtualTxt, true);
      toolBar->EnableTool(ID_LoadDbf, true);
      toolBar->EnableTool(ID_VirtualDbf, true);
      toolBar->EnableTool(ID_LoadXL, true);
      toolBar->EnableTool(ID_VirtualXL, true);
      toolBar->EnableTool(ID_Network, true);
      toolBar->EnableTool(ID_Exif, true);
      toolBar->EnableTool(ID_GpsPics, true);
      toolBar->EnableTool(ID_LoadXml, true);
      toolBar->EnableTool(ID_WFS, true);
      toolBar->EnableTool(ID_DXF, true);
      toolBar->EnableTool(ID_Srids, metadata);
      toolBar->EnableTool(ID_Attach, true);
      toolBar->EnableTool(ID_SqlLog, true);
      toolBar->EnableTool(ID_DbStatus, true);
      toolBar->EnableTool(ID_CheckGeom, true);
      toolBar->EnableTool(ID_SaneGeom, true);
      toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
      UpdateStatusBar();
      return;
  } else
    return;
error:
  unlink(SqlitePath.ToUTF8());
  wxString msg = wxT("An error occurred\nno DB was created");
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyFrame::OnMemoryDbLoad(wxCommandEvent & WXUNUSED(event))
{
//
// loading an external DB into the MEMORY-DB
//
  sqlite3 *extSqlite = NULL;
  sqlite3_backup *backup;
  int retdlg;
  int ret;
  wxString lastDir;
  char path[1024];
  wxString error;
  char *errMsg = NULL;
  wxFileDialog fileDialog(this,
                          wxT("Loading an existing DB into the MEMORY-DB"),
                          wxT(""), wxT("db.sqlite"),
                          wxT
                          ("SQLite DB (*.sqlite)|*.sqlite|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  retdlg = fileDialog.ShowModal();
  if (retdlg == wxID_OK)
    {
      // opening the external DB
      ExternalSqlitePath = fileDialog.GetPath();
      strcpy(path, ExternalSqlitePath.ToUTF8());
      // testing for an appropriate Security level
      TestSecurityRelaxed(path);
      ret = sqlite3_open_v2(path, &extSqlite, SQLITE_OPEN_READWRITE, NULL);
      if (ret)
        {
          // an error occurred
          wxString errCause = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
          error = wxT("Failure while connecting to DB\n\n");
          error += errCause;
          error += wxT("\n");
          goto stop;
        }
      ret =
        sqlite3_open_v2(":memory:", &SqliteHandle,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
      if (ret)
        {
          // an error occurred
          wxString errCause = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
          error = wxT("MEMORY-DB: an error occurred \n\n");
          error += errCause;
          error += +wxT("\n");
          goto stop;
        }
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      backup = sqlite3_backup_init(SqliteHandle, "main", extSqlite, "main");
      if (!backup)
        goto stop;
      while (1)
        {
          ret = sqlite3_backup_step(backup, 1024);
          if (ret == SQLITE_DONE)
            break;
        }
      ret = sqlite3_backup_finish(backup);
      sqlite3_close(extSqlite);
// setting up the internal cache
      SpliteInternalCache = spatialite_alloc_connection();
      spatialite_init_ex(SqliteHandle, SpliteInternalCache, 0);
      rl2_init(SqliteHandle, RL2PrivateData, 0);
      DoInitVirtualPG();
// enabling LOAD_EXTENSION
      ret = sqlite3_enable_load_extension(SqliteHandle, 1);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("Unable to enable LOAD_EXTENSION"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
          sqlite3_free(errMsg);
        }
// activating Foreign Key constraints
      ret =
        sqlite3_exec(SqliteHandle, "PRAGMA foreign_keys = 1", NULL, 0, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("Unable to activate FOREIGN_KEY constraints"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
          goto stop;
        }
// setting RL2 MaxThreads
      char sqlmax[1024];
      sprintf(sqlmax, "SELECT RL2_SetMaxThreads(%d)", RL2MaxThreads);
      sqlite3_exec(SqliteHandle, sqlmax, NULL, 0, &errMsg);
      AutoPostgresStart();
      MemoryDatabase = true;
      AutoSaveInterval = 120;
      AutoFDOStart();
      AutoGPKGStart();
      InitTableTree();
      bool metadata = CheckMetadata();
      wxMenuBar *menuBar = GetMenuBar();
      menuBar->Enable(ID_Connect, false);
      menuBar->Enable(ID_Connect_RO, false);
      menuBar->Enable(ID_CreateNew, false);
      menuBar->Enable(ID_Disconnect, true);
      menuBar->Enable(ID_MemoryDbLoad, false);
      menuBar->Enable(ID_MemoryDbNew, false);
      if (MemoryDatabase == true)
        {
          menuBar->Enable(ID_MemoryDbSave, true);
          menuBar->Enable(ID_MemoryDbClock, true);
      } else
        {
          menuBar->Enable(ID_MemoryDbSave, false);
          menuBar->Enable(ID_MemoryDbClock, false);
        }
      menuBar->Enable(ID_Vacuum, true);
      menuBar->Enable(ID_Postgres, true);
      menuBar->Enable(ID_MapPanel, true);
      menuBar->Enable(ID_SqlScript, true);
      menuBar->Enable(ID_QueryViewComposer, HasViewsMetadata());
      menuBar->Enable(ID_LoadShp, true);
      menuBar->Enable(ID_VirtualShp, true);
      menuBar->Enable(ID_LoadGeoJSON, true);
      menuBar->Enable(ID_VirtualGeoJSON, true);
      menuBar->Enable(ID_LoadTxt, true);
      menuBar->Enable(ID_VirtualTxt, true);
      menuBar->Enable(ID_LoadDbf, true);
      menuBar->Enable(ID_VirtualDbf, true);
      menuBar->Enable(ID_LoadXL, true);
      menuBar->Enable(ID_VirtualXL, true);
      menuBar->Enable(ID_Network, true);
      menuBar->Enable(ID_Exif, true);
      menuBar->Enable(ID_GpsPics, true);
      menuBar->Enable(ID_LoadXml, true);
      menuBar->Enable(ID_WFS, true);
      menuBar->Enable(ID_DXF, true);
      menuBar->Enable(ID_Srids, metadata);
      menuBar->Enable(ID_Attach, true);
      menuBar->Enable(ID_SqlLog, true);
      menuBar->Enable(ID_DbStatus, true);
      menuBar->Enable(ID_CheckGeom, true);
      menuBar->Enable(ID_SaneGeom, true);
      EnableSqlLog();
      menuBar->Check(ID_SqlLog, SqlLogEnabled);
      wxToolBar *toolBar = GetToolBar();
      toolBar->EnableTool(ID_Connect, false);
      toolBar->EnableTool(ID_Connect_RO, false);
      toolBar->EnableTool(ID_CreateNew, false);
      toolBar->EnableTool(ID_Disconnect, true);
      toolBar->EnableTool(ID_MemoryDbLoad, false);
      toolBar->EnableTool(ID_MemoryDbNew, false);
      if (MemoryDatabase == true)
        {
          toolBar->EnableTool(ID_MemoryDbSave, true);
          toolBar->EnableTool(ID_MemoryDbClock, true);
      } else
        {
          toolBar->EnableTool(ID_MemoryDbSave, false);
          toolBar->EnableTool(ID_MemoryDbClock, false);
        }
      toolBar->EnableTool(ID_Vacuum, true);
      toolBar->EnableTool(ID_Postgres, true);
      toolBar->EnableTool(ID_MapPanel, true);
      toolBar->EnableTool(ID_SqlScript, true);
      toolBar->EnableTool(ID_QueryViewComposer, HasViewsMetadata());
      toolBar->EnableTool(ID_LoadShp, true);
      toolBar->EnableTool(ID_VirtualShp, true);
      toolBar->EnableTool(ID_LoadGeoJSON, true);
      toolBar->EnableTool(ID_VirtualGeoJSON, true);
      toolBar->EnableTool(ID_LoadTxt, true);
      toolBar->EnableTool(ID_VirtualTxt, true);
      toolBar->EnableTool(ID_LoadDbf, true);
      toolBar->EnableTool(ID_VirtualDbf, true);
      toolBar->EnableTool(ID_LoadXL, true);
      toolBar->EnableTool(ID_VirtualXL, true);
      toolBar->EnableTool(ID_Network, true);
      toolBar->EnableTool(ID_Exif, true);
      toolBar->EnableTool(ID_GpsPics, true);
      toolBar->EnableTool(ID_LoadXml, true);
      toolBar->EnableTool(ID_WFS, true);
      toolBar->EnableTool(ID_DXF, true);
      toolBar->EnableTool(ID_Srids, metadata);
      toolBar->EnableTool(ID_Attach, true);
      toolBar->EnableTool(ID_SqlLog, true);
      toolBar->EnableTool(ID_DbStatus, true);
      toolBar->EnableTool(ID_CheckGeom, true);
      toolBar->EnableTool(ID_SaneGeom, true);
      toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
      UpdateStatusBar();
      if (AutoSaveInterval <= 0)
        {
          if (TimerAutoSave)
            {
              TimerAutoSave->Stop();
              delete TimerAutoSave;
              TimerAutoSave = NULL;
            }
      } else
        {
          //
          // starting the AutoSave timer
          //
          if (!TimerAutoSave)
            TimerAutoSave = new wxTimer(this, ID_AUTO_SAVE_TIMER);
          else
            TimerAutoSave->Stop();
          LastTotalChanges = 0;
          TimerAutoSave->Start(AutoSaveInterval * 1000, wxTIMER_ONE_SHOT);
        }
    }
  if (AutoFDOmsg.Len() > 0)
    {
      wxMessageBox(AutoFDOmsg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                   this);
      AutoFDOmsg = wxT("");
    }
  if (AutoGPKGmsg.Len() > 0)
    {
      wxMessageBox(AutoGPKGmsg, wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      AutoGPKGmsg = wxT("");
    }
  return;
stop:
  MemoryDatabase = false;
  if (SpliteInternalCache)
    spatialite_finalize_topologies(SpliteInternalCache);
  if (SqliteHandle)
    sqlite3_close(SqliteHandle);
  if (extSqlite)
    sqlite3_close(extSqlite);
  if (SpliteInternalCache)
    spatialite_cleanup_ex(SpliteInternalCache);
  wxString msg = wxT("MEMORY-DB wasn't loaded\n\n");
  msg += error;
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  SqliteHandle = NULL;
  SpliteInternalCache = NULL;
}

void MyFrame::OnMemoryDbNew(wxCommandEvent & WXUNUSED(event))
{
//
//creating a new MEMORY-DB
//
  int ret;
  wxToolBar *toolBar;
  wxMenuBar *menuBar;
  bool metadata;
  MemoryDatabase = true;
  AutoSaveInterval = 120;
  ExternalSqlitePath = wxT("");
// creating the new MEMORY-DB
  ret = CreateDB();
  if (ret == false)
    goto error;
  metadata = CheckMetadata();
  menuBar = GetMenuBar();
  menuBar->Enable(ID_Connect, false);
  menuBar->Enable(ID_Connect_RO, false);
  menuBar->Enable(ID_CreateNew, false);
  menuBar->Enable(ID_Disconnect, true);
  menuBar->Enable(ID_MemoryDbLoad, false);
  menuBar->Enable(ID_MemoryDbNew, false);
  menuBar->Enable(ID_MemoryDbSave, true);
  menuBar->Enable(ID_MemoryDbClock, true);
  menuBar->Enable(ID_Vacuum, true);
  menuBar->Enable(ID_Postgres, true);
  menuBar->Enable(ID_MapPanel, true);
  menuBar->Enable(ID_SqlScript, true);
  menuBar->Enable(ID_QueryViewComposer, HasViewsMetadata());
  menuBar->Enable(ID_LoadShp, true);
  menuBar->Enable(ID_VirtualShp, true);
  menuBar->Enable(ID_LoadGeoJSON, true);
  menuBar->Enable(ID_VirtualGeoJSON, true);
  menuBar->Enable(ID_LoadTxt, true);
  menuBar->Enable(ID_VirtualTxt, true);
  menuBar->Enable(ID_LoadDbf, true);
  menuBar->Enable(ID_VirtualDbf, true);
  menuBar->Enable(ID_LoadXL, true);
  menuBar->Enable(ID_VirtualXL, true);
  menuBar->Enable(ID_Network, true);
  menuBar->Enable(ID_Exif, true);
  menuBar->Enable(ID_GpsPics, true);
  menuBar->Enable(ID_LoadXml, true);
  menuBar->Enable(ID_WFS, true);
  menuBar->Enable(ID_DXF, true);
  menuBar->Enable(ID_Srids, metadata);
  menuBar->Enable(ID_Attach, true);
  menuBar->Enable(ID_SqlLog, true);
  menuBar->Enable(ID_DbStatus, true);
  menuBar->Enable(ID_CheckGeom, true);
  menuBar->Enable(ID_SaneGeom, true);
  EnableSqlLog();
  menuBar->Check(ID_SqlLog, SqlLogEnabled);
  toolBar = GetToolBar();
  toolBar->EnableTool(ID_Connect, false);
  toolBar->EnableTool(ID_Connect_RO, false);
  toolBar->EnableTool(ID_CreateNew, false);
  toolBar->EnableTool(ID_Disconnect, true);
  toolBar->EnableTool(ID_MemoryDbLoad, false);
  toolBar->EnableTool(ID_MemoryDbNew, false);
  toolBar->EnableTool(ID_MemoryDbSave, true);
  toolBar->EnableTool(ID_MemoryDbClock, true);
  toolBar->EnableTool(ID_Vacuum, true);
  toolBar->EnableTool(ID_Postgres, true);
  toolBar->EnableTool(ID_MapPanel, true);
  toolBar->EnableTool(ID_SqlScript, true);
  toolBar->EnableTool(ID_QueryViewComposer, HasViewsMetadata());
  toolBar->EnableTool(ID_LoadShp, true);
  toolBar->EnableTool(ID_VirtualShp, true);
  toolBar->EnableTool(ID_LoadGeoJSON, true);
  toolBar->EnableTool(ID_VirtualGeoJSON, true);
  toolBar->EnableTool(ID_LoadTxt, true);
  toolBar->EnableTool(ID_VirtualTxt, true);
  toolBar->EnableTool(ID_LoadDbf, true);
  toolBar->EnableTool(ID_VirtualDbf, true);
  toolBar->EnableTool(ID_LoadXL, true);
  toolBar->EnableTool(ID_VirtualXL, true);
  toolBar->EnableTool(ID_Network, true);
  toolBar->EnableTool(ID_Exif, true);
  toolBar->EnableTool(ID_GpsPics, true);
  toolBar->EnableTool(ID_LoadXml, true);
  toolBar->EnableTool(ID_WFS, true);
  toolBar->EnableTool(ID_DXF, true);
  toolBar->EnableTool(ID_Srids, metadata);
  toolBar->EnableTool(ID_Attach, true);
  toolBar->EnableTool(ID_SqlLog, true);
  toolBar->EnableTool(ID_DbStatus, true);
  toolBar->EnableTool(ID_CheckGeom, true);
  toolBar->EnableTool(ID_SaneGeom, true);
  toolBar->ToggleTool(ID_SqlLog, SqlLogEnabled);
  UpdateStatusBar();
  if (AutoSaveInterval <= 0)
    {
      if (TimerAutoSave)
        {
          TimerAutoSave->Stop();
          delete TimerAutoSave;
          TimerAutoSave = NULL;
        }
  } else
    {
      //
      // starting the AutoSave timer
      //
      if (!TimerAutoSave)
        TimerAutoSave = new wxTimer(this, ID_AUTO_SAVE_TIMER);
      else
        TimerAutoSave->Stop();
      LastTotalChanges = 0;
      TimerAutoSave->Start(AutoSaveInterval * 1000, wxTIMER_ONE_SHOT);
    }
  return;
error:
  wxString msg = wxT("An error occurred\nno MEMORY-DB was created");
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

bool MyFrame::MemoryDbSave()
{
//
// trying to export the MEMORY-DB into an external DB
//
  sqlite3 *extSqlite = NULL;
  sqlite3_backup *backup;
  char path[1024];
  char bak_path[1024];
  int ret;
  wxString error;
  if (ExternalSqlitePath.Len() == 0)
    return false;
  ::wxBeginBusyCursor();
  strcpy(path, ExternalSqlitePath.ToUTF8());
  strcpy(bak_path, path);
  strcat(bak_path, ".bak");
  unlink(bak_path);
  rename(path, bak_path);
  ret =
    sqlite3_open_v2(path, &extSqlite,
                    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  if (ret)
    {
      // an error occurred
      wxString errCause = wxString::FromUTF8(sqlite3_errmsg(extSqlite));
      error = wxT("An error occurred\n\n");
      error += errCause;
      error += +wxT("\n");
      error += ExternalSqlitePath;
      goto stop;
    }
  backup = sqlite3_backup_init(extSqlite, "main", SqliteHandle, "main");
  if (!backup)
    goto stop;
  while (1)
    {
      ret = sqlite3_backup_step(backup, 1024);
      if (ret == SQLITE_DONE)
        break;
    }
  ret = sqlite3_backup_finish(backup);
  sqlite3_close(extSqlite);
  unlink(bak_path);
  ::wxEndBusyCursor();
  LastTotalChanges = sqlite3_total_changes(SqliteHandle);
  return true;
stop:
  if (extSqlite)
    sqlite3_close(extSqlite);
  wxString msg = wxT("Backup failure: MEMORY-DB wasn't saved\n\n");
  msg += error;
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  ExternalSqlitePath = wxT("");
  ::wxEndBusyCursor();
  return false;
}

void MyFrame::OnMemoryDbSave(wxCommandEvent & WXUNUSED(event))
{
//
//  exporting the MEMORY-DB into an external DB 
//
  int retdlg;
  wxString lastDir;
  if (ExternalSqlitePath.Len() > 0)
    {
      if (MemoryDbSave() == true)
        {
          wxMessageBox(wxT("Ok, MEMORY-DB was successfully saved"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
          if (AutoSaveInterval <= 0)
            {
              if (TimerAutoSave)
                {
                  TimerAutoSave->Stop();
                  delete TimerAutoSave;
                  TimerAutoSave = NULL;
                }
          } else
            {
              //
              // restarting the AutoSave timer
              //
              if (!TimerAutoSave)
                TimerAutoSave = new wxTimer(this, ID_AUTO_SAVE_TIMER);
              else
                TimerAutoSave->Stop();
              TimerAutoSave->Start(AutoSaveInterval * 1000, wxTIMER_ONE_SHOT);
            }
          return;
        }
    }
  wxFileDialog fileDialog(this, wxT("Saving the MEMORY-DB"),
                          wxT(""), wxT("db.sqlite"),
                          wxT
                          ("SQLite DB (*.sqlite)|*.sqlite|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  retdlg = fileDialog.ShowModal();
  if (retdlg == wxID_OK)
    {
      // exporting the external DB
      ExternalSqlitePath = fileDialog.GetPath();
      if (MemoryDbSave() == true)
        {
          wxMessageBox(wxT("Ok, MEMORY-DB was successfully saved"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
          wxFileName file(fileDialog.GetPath());
          lastDir = file.GetPath();
          SetLastDirectory(lastDir);
          if (AutoSaveInterval <= 0)
            {
              if (TimerAutoSave)
                {
                  TimerAutoSave->Stop();
                  delete TimerAutoSave;
                  TimerAutoSave = NULL;
                }
          } else
            {
              //
              // restarting the AutoSave timer
              //
              if (!TimerAutoSave)
                TimerAutoSave = new wxTimer(this, ID_AUTO_SAVE_TIMER);
              else
                TimerAutoSave->Stop();
              TimerAutoSave->Start(AutoSaveInterval * 1000, wxTIMER_ONE_SHOT);
            }
        }
    }
}

void MyFrame::OnMemoryDbClock(wxCommandEvent & WXUNUSED(event))
{
//
//  setting up AutoSave for MEMORY-DB 
//
  AutoSaveDialog dlg;
  dlg.Create(this, ExternalSqlitePath, AutoSaveInterval);
  int ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      AutoSaveInterval = dlg.GetSeconds();
      if (AutoSaveInterval <= 0)
        {
          //
          // stopping the AutoSave timer
          //
          if (TimerAutoSave)
            {
              TimerAutoSave->Stop();
              delete TimerAutoSave;
              TimerAutoSave = NULL;
            }
      } else
        {
          //
          // restarting the AutoSave timer
          //
          if (!TimerAutoSave)
            TimerAutoSave = new wxTimer(this, ID_AUTO_SAVE_TIMER);
          else
            TimerAutoSave->Stop();
          TimerAutoSave->Start(AutoSaveInterval * 1000, wxTIMER_ONE_SHOT);
        }
    }
}

void MyFrame::DbPagesCount(int *total, int *frees)
{
//
// computing the DB pages count
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *value;
  *total = 0;
  *frees = 0;
  sql = wxT("PRAGMA page_count");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(GetSqlite(), xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          *total = atoi(value);
        }
    }
  sqlite3_free_table(results);
  sql = wxT("PRAGMA freelist_count");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(GetSqlite(), xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          *frees = atoi(value);
        }
    }
  sqlite3_free_table(results);
}

bool MyFrame::GetRtTopoVersion(char *buf)
{
//
// retieving the RTTOPO version (if enabled)
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  bool ok = false;

  if (GetSqlite() == NULL)
    return false;
  ret = sqlite3_get_table(GetSqlite(), "SELECT rttopo_version()", &results,
                          &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return false;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *version = results[(i * columns) + 0];
          if (version != NULL)
            {
              strcpy(buf, version);
              ok = true;
            }
        }
    }
  sqlite3_free_table(results);
  return ok;
}

bool MyFrame::GetLibXml2Version(char *buf)
{
//
// retieving the LIBXML2 version (if enabled)
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  bool ok = false;

  if (GetSqlite() == NULL)
    return false;
  ret = sqlite3_get_table(GetSqlite(), "SELECT libxml2_version()", &results,
                          &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return false;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *version = results[(i * columns) + 0];
          if (version != NULL)
            {
              strcpy(buf, version);
              ok = true;
            }
        }
    }
  sqlite3_free_table(results);
  return ok;
}

void MyFrame::CreateNetwork(wxString & table, wxString & from, wxString & to,
                            bool isNoGeometry, wxString & geom,
                            bool isNameEnabled, wxString & name,
                            bool isGeomLength, wxString & cost,
                            bool isBidirectional, bool isOneWays,
                            wxString & oneWayFromTo, wxString & oneWayToFrom,
                            bool aStarSupported, wxString & dataTableName,
                            wxString & virtualTableName, bool overwrite)
{
//
// creating VirtualRouting tables
//
  char *xtable = (char *) malloc(table.Len() * 4);
  strcpy(xtable, table.ToUTF8());
  char *xfrom = (char *) malloc(from.Len() * 4);
  strcpy(xfrom, from.ToUTF8());
  char *xto = (char *) malloc(to.Len() * 4);
  strcpy(xto, to.ToUTF8());
  char *xgeom = NULL;
  if (isNoGeometry == false && geom.Len() > 0)
    {
      xgeom = (char *) malloc(geom.Len() * 4);
      strcpy(xgeom, geom.ToUTF8());
    }
  char *xname = NULL;
  if (isNameEnabled == true && name.Len() > 0)
    {
      xname = (char *) malloc(name.Len() * 4);
      strcpy(xname, name.ToUTF8());
    }
  char *xcost = NULL;
  if (isGeomLength == false && cost.Len() > 0)
    {
      xcost = (char *) malloc(cost.Len() * 4);
      strcpy(xcost, cost.ToUTF8());
    }
  int xbidirectional = 0;
  if (isBidirectional == true)
    xbidirectional = 1;
  char *xonewayfrom = NULL;
  char *xonewayto = NULL;
  if (isOneWays == true && oneWayFromTo.Len() > 0 && oneWayToFrom.Len() > 0)
    {
      xonewayfrom = (char *) malloc(oneWayFromTo.Len() * 4);
      strcpy(xonewayfrom, oneWayFromTo.ToUTF8());
      xonewayto = (char *) malloc(oneWayToFrom.Len() * 4);
      strcpy(xonewayto, oneWayToFrom.ToUTF8());
    }
  int xastarsupported = 0;
  if (aStarSupported == true)
    xastarsupported = 1;
  char *xdatatablename = (char *) malloc(dataTableName.Len() * 4);
  strcpy(xdatatablename, dataTableName.ToUTF8());
  char *xvirtualtablename = (char *) malloc(virtualTableName.Len() * 4);
  strcpy(xvirtualtablename, virtualTableName.ToUTF8());
  int xoverwrite = 0;
  if (overwrite == true)
    xoverwrite = 1;

// preparing the SQL statement
  char *sql = sqlite3_mprintf("SELECT CreateRouting(%Q, %Q, %Q, %Q, %Q",
                              xdatatablename, xvirtualtablename, xtable, xfrom,
                              xto);
  char *prev = sql;
  if (xgeom == NULL)
    sql = sqlite3_mprintf("%s, NULL", prev);
  else
    sql = sqlite3_mprintf("%s, %Q", prev, xgeom);
  sqlite3_free(prev);
  prev = sql;
  if (xcost == NULL)
    sql = sqlite3_mprintf("%s, NULL", prev);
  else
    sql = sqlite3_mprintf("%s, %Q", prev, xcost);
  sqlite3_free(prev);
  prev = sql;
  if (xname == NULL)
    sql = sqlite3_mprintf("%s, NULL", prev);
  else
    sql = sqlite3_mprintf("%s, %Q", prev, xname);
  sqlite3_free(prev);
  prev = sql;
  sql = sqlite3_mprintf("%s, %d, %d", prev, xastarsupported, xbidirectional);
  sqlite3_free(prev);
  prev = sql;
  if (xonewayfrom != NULL && xonewayto != NULL)
    sql = sqlite3_mprintf("%s, %Q, %Q", prev, xonewayfrom, xonewayto);
  else
    sql = sqlite3_mprintf("%s, NULL, NULL", prev);
  sqlite3_free(prev);
  prev = sql;
  sql = sqlite3_mprintf("%s, %d)", prev, xoverwrite);
  sqlite3_free(prev);

// executing the SQL statement
  int i;
  char **results;
  int rows;
  int columns;
  int success = 0;
  int ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    goto end;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        success = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);

end:
  free(xtable);
  free(xfrom);
  free(xto);
  if (xgeom != NULL)
    free(xgeom);
  if (xname != NULL)
    free(xname);
  if (xcost != NULL)
    free(xcost);
  if (xonewayfrom != NULL)
    free(xonewayfrom);
  if (xonewayto != NULL)
    free(xonewayto);
  free(xdatatablename);
  free(xvirtualtablename);
  if (success)
    {
      wxString msg = wxT("The VirtualRouting Tables were succesfully created");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      InitTableTree();
  } else
    {
      // failure: reporting the error cause
      wxString errCause = wxT("Unknown Cause");
      GetLastRoutingError(errCause);
      wxString msg =
        wxT
        ("Some error occurred forbidding to create the VirtualRouting Tables\n\n")
        + errCause;
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }

}

void MyFrame::GetLastRoutingError(wxString & msg)
{
//
// attempting to retrieve the most recent failure cause - CreateRouting
//
  int i;
  char **results;
  int rows;
  int columns;
  int ret =
    sqlite3_get_table(SqliteHandle, "SELECT CreateRouting_GetLastError()",
                      &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *message = results[(i * columns) + 0];
          msg = wxString::FromUTF8(message);
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::OnVacuum(wxCommandEvent & WXUNUSED(event))
{
//
// performing a VACUUM in order to reorganize the current DB
//
  char *errMsg = NULL;
  int totalPages;
  int freePages;
  int totalPages2;
  int freePages2;
  wxString msg;
  char dummy[128];
  DbPagesCount(&totalPages, &freePages);
  if (!freePages)
    {
      msg = wxT("The current DB doesn't requires to be VACUUMed\n\n");
      msg += wxT("Total Pages: ");
      sprintf(dummy, "%d\n", totalPages);
      msg += wxString::FromUTF8(dummy);
      msg += wxT("Free Pages: 0\n\n");
      msg += wxT("Free Ratio: 0.0%");
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      return;
  } else
    {
      double ratio = 100.0 / ((double) totalPages / (double) freePages);
      if (ratio >= 33.33)
        msg = wxT("The current DB urgently requires to be VACUUMed\n\n");
      else if (ratio >= 10.0)
        msg = wxT("The current DB may usefully be VACUUMed\n\n");
      else
        msg =
          wxT("The current DB doesn't strictly requires to be VACUUMed\n\n");
      msg += wxT("Total Pages: ");
      sprintf(dummy, "%d\n", totalPages);
      msg += wxString::FromUTF8(dummy);
      msg += wxT("Free Pages: ");
      sprintf(dummy, "%d\n\n", freePages);
      msg += wxString::FromUTF8(dummy);
      msg += wxT("Free Ratio: ");
      sprintf(dummy, "%1.2f%%\n", ratio);
      msg += wxString::FromUTF8(dummy);
      msg += wxT("\n\nDo you confirm VACUUMing the current DB ?");
      int ret =
        wxMessageBox(msg, wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION,
                     this);
      if (ret != wxYES)
        return;
    }
  ::wxBeginBusyCursor();
  int ret = sqlite3_exec(SqliteHandle, "ANALYZE; VACUUM;", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
  } else
    {
      DbPagesCount(&totalPages2, &freePages2);
      msg = wxT("Current DB was successfully optimized");
      if (totalPages2 < totalPages)
        {
          sprintf(dummy, "\n\n%d unused pages were reclaimed",
                  totalPages - totalPages2);
          msg += wxString::FromUTF8(dummy);
        }
      wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
    }
  ::wxEndBusyCursor();
}

void MyFrame::MapPanelClosing()
{
//
// the Map Panel is closing
//
  MapPanel = NULL;
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  menuBar->Check(ID_MapPanel, false);
  toolBar->ToggleTool(ID_MapPanel, false);
}

void MyFrame::OnMapPanel(wxCommandEvent & WXUNUSED(event))
{
//
// Opening / Closing the Map Panel
//
  if (MapPanel == NULL)
    {
      // opening the Map Panel
      MapPanel = new MyMapPanel(this, wxT("SpatiaLiteMap"),
                                wxPoint(0, 0), wxSize(640, 480));
      MapPanel->Show(true);
  } else
    {
      // closing the Map Panel
      MapPanel->Show(false);
      MapPanel->Close(true);
    }
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  if (MapPanel == NULL)
    {
      menuBar->Check(ID_MapPanel, false);
      toolBar->ToggleTool(ID_MapPanel, false);
  } else
    {
      menuBar->Check(ID_MapPanel, true);
      toolBar->ToggleTool(ID_MapPanel, true);
    }
}

char *MyFrame::ReadSqlLine(FILE * in, int *len, int *eof)
{
//
// reading an SQL script line
//
  int c;
  *eof = 0;
  int size = 4096;
  char *line = (char *) malloc(size);
  int off = 0;
  while ((c = getc(in)) != EOF)
    {
      // consuming input one chat at each time
      if (off == size)
        {
          // buffer overflow; reallocating a bigger one
          // presumably this is because there is some BLOB, so we'll grow by 1MB at each time
          size += 1024 * 1024;
          line = (char *) realloc(line, size);
        }
      *(line + off) = c;
      off++;
      if (c == '\n')
        {
          // end of line marker
          *(line + off) = '\0';
          *len = off;
          return line;
        }
      if (c == ';')
        {
          // end of SQL statement marker
          *(line + off) = '\0';
          *len = off;
          return line;
        }
    }
// EOF reached
  *len = off;
  *eof = 1;
  return line;
}

void MyFrame::OnQueryViewComposer(wxCommandEvent & WXUNUSED(event))
{
//
// invoking the Query/View composer tool
//
  QueryViewComposer();
}

void MyFrame::OnSqlScript(wxCommandEvent & WXUNUSED(event))
{
//
// executing an SQL Script
//
  int ret;
  wxString lastDir;
  wxString path;
  wxString charset;
  FILE *sql;
  char *line = NULL;
  char *statement = NULL;
  int stmt_len = 0;
  char *prev_stmt;
  int prev_len;
  int eof;
  int rowNo = 1;
  int stmt = 0;
  int len;
  wxString msg;
  char dummy[128];
  void *cvtCS = NULL;
  char *utf8stmt = NULL;
  int cvtErr;
  char x_path[1024];
  char x_table[1024];
  char x_column[1024];
  char x_charset[1024];
  char x_type[1024];
  int rows;
  unsigned int urows;
  wxString extPath;
  wxString extTable;
  wxString extCharset;
  wxString extColumn;
  wxString extType;
  int extWorksheetIndex;
  int extFirstTitle;
  int srid;
  bool coerce2D;
  bool compressed;
  int command;
  bool stmtDone;
  wxString workingDir;
  wxFileDialog fileDialog(this, wxT("SQL Script"),
                          wxT(""),
                          wxT("init_spatialite.sql"),
                          wxT("SQL script (*.sql)|*.sql|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      SqlScriptDialog dlg;
      dlg.Create(this, path, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        charset = dlg.GetCharset();
      else
        return;
      // opening the SQL script
      sql = fopen(path.ToUTF8(), "r");
      if (sql == NULL)
        {
          wxMessageBox(wxT("can't open: ") + fileDialog.GetPath(),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      } else
        {
          wxFileName file(fileDialog.GetPath());
          lastDir = file.GetPath();
          SetLastDirectory(lastDir);
          cvtCS = gaiaCreateUTF8Converter(charset.ToUTF8());
          if (!cvtCS)
            {
              msg = charset + wxT(": unsupported CharacterSet");
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              goto stop;
            }
          ::wxBeginBusyCursor();
          workingDir = wxGetCwd();
          wxSetWorkingDirectory(lastDir);
          while (1)
            {
              // reading the SQL script lines
              line = ReadSqlLine(sql, &len, &eof);
              if (len > 0)
                {
                  if (statement == NULL)
                    {
                      statement = line;
                      stmt_len = len;
                  } else
                    {
                      // appending line to SQL statement
                      prev_stmt = statement;
                      prev_len = stmt_len;
                      stmt_len = prev_len + len;
                      statement = (char *) malloc(stmt_len + 1);
                      memcpy(statement, prev_stmt, prev_len);
                      memcpy(statement + prev_len, line, len);
                      *(statement + stmt_len) = '\0';
                      free(prev_stmt);
                      free(line);
                      line = NULL;
                    }
              } else
                {
                  free(line);
                  line = NULL;
                }
              if (statement)
                {
                  stmtDone = false;
                  command = TestDotCommand(statement);
                  if (command == CMD_SQLLOG)
                    {
                      // simply ignoring
                      free(statement);
                      statement = NULL;
                      stmt_len = 0;
                      stmt++;
                      stmtDone = true;
                    }
                  if (command == CMD_LOADSHP || command == CMD_LOADDBF
                      || command == CMD_LOADXL || command == CMD_DUMPSHP
                      || command == CMD_DUMPDBF)
                    {
                      utf8stmt =
                        gaiaConvertToUTF8(cvtCS, statement + 9, stmt_len,
                                          &cvtErr);
                      if (cvtErr || !utf8stmt)
                        {
                          Rollback();
                          msg =
                            wxT
                            ("SQL Script abnormal termination\nillegal character sequence");
                          msg +=
                            wxT("\n\nROLLBACK was automatically performed");
                          wxMessageBox(msg, wxT("spatialite_gui"),
                                       wxOK | wxICON_WARNING, this);
                          goto stop;
                        }
                      if (command == CMD_LOADSHP)
                        {
                          if (IsDotCommandLoadShp
                              (utf8stmt, x_path, x_table, x_charset,
                               x_column, &srid, &coerce2D, &compressed))
                            {
                              load_shapefile(SqliteHandle, x_path, x_table,
                                             x_charset, srid, x_column,
                                             coerce2D, compressed, 0, 0, &rows,
                                             NULL);
                              free(statement);
                              statement = NULL;
                              stmt_len = 0;
                              stmt++;
                              stmtDone = true;
                            }
                        }
                      if (command == CMD_LOADDBF)
                        {
                          if (IsDotCommandLoadDbf
                              (utf8stmt, x_path, x_table, x_charset) == true)
                            {
                              load_dbf(SqliteHandle, x_path, x_table, x_charset,
                                       0, &rows, NULL);
                              free(statement);
                              statement = NULL;
                              stmt_len = 0;
                              stmt++;
                              stmtDone = true;
                            }
                        }
                      if (command == CMD_LOADXL)
                        {
                          if (IsDotCommandLoadXL
                              (utf8stmt, x_path, x_table, &extWorksheetIndex,
                               &extFirstTitle) == true)
                            {
#ifndef OMIT_FREEXL             // only if FreeXL is enabled
                              load_XL(SqliteHandle, x_path, x_table,
                                      extWorksheetIndex, extFirstTitle, &urows,
                                      NULL);
                              free(statement);
                              statement = NULL;
                              stmt_len = 0;
                              stmt++;
                              stmtDone = true;
#else // FreeXL isn't enabled
                              stmtDone = false;
#endif // end FreeXL conditional support
                            }
                        }
                      if (command == CMD_DUMPSHP)
                        {
                          if (IsDotCommandDumpShp
                              (utf8stmt, x_table, x_column, x_path,
                               x_charset, x_type) == true)
                            {
                              dump_shapefile(SqliteHandle, x_table, x_column,
                                             x_path, x_charset, x_type, 0,
                                             &rows, NULL);
                              free(statement);
                              statement = NULL;
                              stmt_len = 0;
                              stmt++;
                              stmtDone = true;
                            }
                        }
                      free(utf8stmt);
                      utf8stmt = NULL;
                    }
                  if (stmtDone)
                    ;
                  else if (sqlite3_complete(statement))
                    {
                      // executing the SQL statement
                      utf8stmt =
                        gaiaConvertToUTF8(cvtCS, statement, stmt_len, &cvtErr);
                      free(statement);
                      statement = NULL;
                      stmt_len = 0;
                      if (cvtErr || !utf8stmt)
                        {
                          Rollback();
                          msg =
                            wxT
                            ("SQL Script abnormal termination\nillegal character sequence");
                          msg +=
                            wxT("\n\nROLLBACK was automatically performed");
                          wxMessageBox(msg, wxT("spatialite_gui"),
                                       wxOK | wxICON_WARNING, this);
                          goto stop;
                        }
                      if (ExecuteSql(utf8stmt, rowNo) == false)
                        {
                          Rollback();
                          msg =
                            wxT
                            ("SQL Script abnormal termination\nan error occurred");
                          msg +=
                            wxT("\n\nROLLBACK was automatically performed");
                          wxMessageBox(msg, wxT("spatialite_gui"),
                                       wxOK | wxICON_WARNING, this);
                          goto stop;
                      } else
                        {
                          stmt++;
                          free(utf8stmt);
                          utf8stmt = NULL;
                        }
                    }
                }
              rowNo++;
              if (eof)
                break;
            }
          sprintf(dummy,
                  "SQL Script normal termination\n\n%d SQL statements were performed",
                  stmt);
          msg = wxString::FromUTF8(dummy);
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                       this);
        stop:
          if (cvtCS)
            gaiaFreeUTF8Converter(cvtCS);
          if (utf8stmt)
            free(utf8stmt);
          if (statement)
            free(statement);
          if (line)
            free(line);
          fclose(sql);
          wxSetWorkingDirectory(workingDir);
          ::wxEndBusyCursor();
        }
    }
}

bool MyFrame::DoGuessSridFromSHP(wxString & path, int *xsrid)
{
// attempting to guess the SRID from some Shapefile
#ifdef PROJ_NEW                 // only if new PROJ.6 is supported
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char *value;
  int srid = *xsrid;
  char pth[1024];
  strcpy(pth, path.ToUTF8());
  char *sql = sqlite3_mprintf("SELECT PROJ_GuessSridFromSHP(%Q)", pth);
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto end;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          if (value != NULL)
            srid = atoi(value);
        }
    }
  sqlite3_free_table(results);
  if (srid > 0)
    {
      *xsrid = srid;
      return true;
    }
end:
#endif
  if (path.Len() == 0 && xsrid == NULL)
    path = wxT("");             // silencing stupid compiler warnings about unused args
  return false;
}

bool MyFrame::DoGuessSridFromZipSHP(const char *zip_path, const char *basename,
                                    int *xsrid)
{
// attempting to guess the SRID from some Shapefile contained within a Zipfile
  int i = 0;
#ifdef PROJ_NEW                 // only if new PROJ.6 is supported
  int ret;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  int srid = *xsrid;
  char *sql =
    sqlite3_mprintf("SELECT PROJ_GuessSridFromZipSHP(%Q, %Q)", zip_path,
                    basename);
  ret =
    sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto end;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          if (value != NULL)
            srid = atoi(value);
        }
    }
  sqlite3_free_table(results);
  if (srid > 0)
    {
      *xsrid = srid;
      return true;
    }
end:
#endif
  if (i == -54321 && xsrid == NULL)
    i = 12345;                  // silencing stupid compiler warnings about unused args
  return false;
}

void MyFrame::OnLoadShp(wxCommandEvent & WXUNUSED(event))
{
//
// loading a shapefile
//
  int ret;
  wxString table;
  wxString column = wxT("Geometry");
  int srid = 0;
  int text_dates;
  int colname_case;
  int coerce2D;
  int compressed;
  int spatial_index;
  wxString path;
  wxString lastDir;
  wxFileDialog fileDialog(this, wxT("Load Shapefile"),
                          wxT(""),
                          wxT("shapefile.shp"),
                          wxT
                          ("Shapefile (*.shp)|*.shp|Zipfile Archive (*.zip)|*.zip|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  if (GetMetaDataType() != METADATA_CURRENT)
    srid = -1;                  // old default SRID
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      if (file.GetExt().CmpNoCase(wxT("zip")) == 0)
        {
          wxString path = fileDialog.GetPath();
          return DoLoadZipShp(path);
        }
      table = file.GetName();
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      LoadShpDialog dlg;
#ifdef PROJ_NEW                 // only if new PROJ.6 is supported
      int xsrid;
      if (DoGuessSridFromSHP(path, &xsrid))
        srid = xsrid;
#endif
      dlg.Create(this, path, table, srid, column, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          int rt;
          int rows;
          char x_path[1024];
          char x_table[1024];
          char x_column[1024];
          char x_gtype[1024];
          char *gtype;
          char x_pkey[1024];
          char *pkey;
          char x_charset[1024];
          char err_msg[1024];
          SetLastDirectory(lastDir);
          strcpy(x_path, path.ToUTF8());
          strcpy(x_table, dlg.GetTable().ToUTF8());
          srid = dlg.GetSRID();
          strcpy(x_column, dlg.GetColumn().ToUTF8());
          strcpy(x_charset, dlg.GetCharset().ToUTF8());
          if (dlg.ApplyCoertion2D() == true)
            coerce2D = 1;
          else
            coerce2D = 0;
          if (dlg.ApplyCompression() == true)
            compressed = 1;
          else
            compressed = 0;
          if (dlg.CreateSpatialIndex() == true)
            spatial_index = 1;
          else
            spatial_index = 0;
          if (dlg.IsUserDefinedGType() == false)
            gtype = NULL;
          else
            {
              strcpy(x_gtype, dlg.GetGeometryType().ToUTF8());
              gtype = x_gtype;
            }
          if (dlg.IsUserDefinedPKey() == false)
            pkey = NULL;
          else
            {
              strcpy(x_pkey, dlg.GetPKColumn().ToUTF8());
              pkey = x_pkey;
            }
          text_dates = dlg.IsTextDates();
          colname_case = dlg.GetColnameCase();
          ::wxBeginBusyCursor();
          rt =
            load_shapefile_ex3(SqliteHandle, x_path, x_table, x_charset, srid,
                               x_column, gtype, pkey, coerce2D, compressed, 0,
                               spatial_index, text_dates, &rows, colname_case,
                               err_msg);
          if (rt && dlg.IsUpdateStatistics())
            {
              // immediately updating Layer Statistics
              char *emsg = NULL;
              char *sql =
                sqlite3_mprintf("SELECT UpdateLayerStatistics(%Q)", x_table);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &emsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("UpdateLayerStatistics error:") +
                               wxString::FromUTF8(emsg), wxT("spatialite_gui"),
                               wxOK | wxICON_INFORMATION, this);
                  sqlite3_free(emsg);
                }
            }
          ::wxEndBusyCursor();
          if (rt)
            {
              wxMessageBox(wxT("load shp OK:") +
                           wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
              InitTableTree();
          } else
            wxMessageBox(wxT("load shp error:") +
                         wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                         wxOK | wxICON_ERROR, this);
        }
    }
}

void MyFrame::DoLoadZipShp(wxString & str_path)
{
//
// loading a Shapefile from within a Zipfile
//
  int count;
  bool err = false;
  int ret;
  wxString table;
  wxString column = wxT("Geometry");
  wxString lastDir;
  int srid = 0;
  int text_dates;
  int colname_case;
  int coerce2D;
  int compressed;
  int spatial_index;
  wxFileName file(str_path);
  ChoooseZipShpDialog dlg0;
  LoadZipShpDialog dlg;
  const char *basename;
  char *zip_path = (char *) malloc(str_path.Len() * 4);
  strcpy(zip_path, str_path.ToUTF8());
  if (!gaiaZipfileNumSHP(zip_path, &count))
    err = true;
  if (count < 1)
    err = true;
  if (err)
    {
      wxMessageBox(wxT("No Shapefile found in:") + str_path,
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      goto end;
    }
  lastDir = file.GetPath();
  dlg0.Create(this, zip_path, count);
  ret = dlg0.ShowModal();
  if (ret != wxID_OK)
    goto end;
  basename = dlg0.GetBasename();
  file = wxFileName(wxString::FromUTF8(basename));
  table = file.GetName();
#ifdef PROJ_NEW                 // only if new PROJ.6 is supported
  int xsrid;
  if (DoGuessSridFromZipSHP(zip_path, basename, &xsrid))
    srid = xsrid;
#endif
  dlg.Create(this, zip_path, basename, table, srid, column, LocaleCharset);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      int rt;
      int rows;
      char x_table[1024];
      char x_column[1024];
      char x_gtype[1024];
      char *gtype;
      char x_pkey[1024];
      char *pkey;
      char x_charset[1024];
      char err_msg[1024];
      SetLastDirectory(lastDir);
      strcpy(x_table, dlg.GetTable().ToUTF8());
      srid = dlg.GetSRID();
      strcpy(x_column, dlg.GetColumn().ToUTF8());
      strcpy(x_charset, dlg.GetCharset().ToUTF8());
      if (dlg.ApplyCoertion2D() == true)
        coerce2D = 1;
      else
        coerce2D = 0;
      if (dlg.ApplyCompression() == true)
        compressed = 1;
      else
        compressed = 0;
      if (dlg.CreateSpatialIndex() == true)
        spatial_index = 1;
      else
        spatial_index = 0;
      if (dlg.IsUserDefinedGType() == false)
        gtype = NULL;
      else
        {
          strcpy(x_gtype, dlg.GetGeometryType().ToUTF8());
          gtype = x_gtype;
        }
      if (dlg.IsUserDefinedPKey() == false)
        pkey = NULL;
      else
        {
          strcpy(x_pkey, dlg.GetPKColumn().ToUTF8());
          pkey = x_pkey;
        }
      text_dates = dlg.IsTextDates();
      colname_case = dlg.GetColnameCase();
      ::wxBeginBusyCursor();
      rt =
        load_zip_shapefile(SqliteHandle, zip_path, basename, x_table, x_charset,
                           srid, x_column, gtype, pkey, coerce2D, compressed, 0,
                           spatial_index, text_dates, &rows, colname_case,
                           err_msg);
      if (rt && dlg.IsUpdateStatistics())
        {
          // immediately updating Layer Statistics
          char *emsg = NULL;
          char *sql =
            sqlite3_mprintf("SELECT UpdateLayerStatistics(%Q)", x_table);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &emsg);
          sqlite3_free(sql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("UpdateLayerStatistics error:") +
                           wxString::FromUTF8(emsg), wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
              sqlite3_free(emsg);
            }
        }
      ::wxEndBusyCursor();
      if (rt)
        {
          wxMessageBox(wxT("load shp OK:") +
                       wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          InitTableTree();
      } else
        wxMessageBox(wxT("load shp error:") +
                     wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                     wxOK | wxICON_ERROR, this);
    }


end:
  if (zip_path != NULL)
    free(zip_path);
}

void MyFrame::OnVirtualShp(wxCommandEvent & WXUNUSED(event))
{
//
// creating a VirtualShape
//
  int ret;
  wxString charset;
  int srid;
  char dummy[128];
  wxString sql;
  char *xsql;
  wxString path;
  wxString table;
  wxString lastDir;
  wxString geometryType;
  char *errMsg = NULL;
  char *name;
  char *xname;
  bool text_dates = false;
  sqlite3 *sqlite = GetSqlite();
  wxFileDialog fileDialog(this, wxT("VirtualShape"),
                          wxT(""),
                          wxT("shapefile.shp"),
                          wxT("Shapefile (*.shp)|*.shp|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      VirtualShpDialog dlg;
#ifdef PROJ_NEW                 // only if new PROJ.6 is supported
      int xsrid;
      wxString pth;
      pth = file.GetPath();
      pth += file.GetPathSeparator();
      pth += file.GetName();
      if (DoGuessSridFromSHP(pth, &xsrid))
        srid = xsrid;
#else
      srid = -1;
#endif
      dlg.Create(this, path, table, srid, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          table = dlg.GetTable();
          srid = dlg.GetSRID();
          charset = dlg.GetCharset();
          text_dates = dlg.IsTextDates();
      } else
        return;
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      sql = wxT("CREATE VIRTUAL TABLE \"");
      name = (char *) malloc((table.Len() * 4) + 1);
      strcpy(name, table.ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("\"\nUSING VirtualShape('");
      sql += file.GetPath();
      sql += file.GetPathSeparator();
      sql += file.GetName();
      sql += wxT("',\n'");
      sql += charset;
      sprintf(dummy, "', %d", srid);
      sql += wxString::FromUTF8(dummy);
      if (text_dates)
        sql += wxT(", 1)");
      else
        sql += wxT(")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      wxMessageBox(wxT("Virtual Table ") + table +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      InitTableTree();
    }
}

void MyFrame::OnLoadGeoJSON(wxCommandEvent & WXUNUSED(event))
{
//
// loading a GeoJSON file
//
  int ret;
  wxString table;
  wxString column = wxT("Geometry");
  int srid = 4326;
  int colname_case;
  int spatial_index;
  wxString path;
  wxString lastDir;
  wxFileDialog fileDialog(this, wxT("Load GeoJSON file"),
                          wxT(""),
                          wxT("file.geojson"),
                          wxT
                          ("GeoJSON (*.geojson)|*.geojson|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  if (GetMetaDataType() != METADATA_CURRENT)
    srid = -1;                  // old default SRID
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      lastDir = file.GetPath();
      table = file.GetName();
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetFullName();
      LoadGeoJsonDialog dlg;
      dlg.Create(this, path, table, srid, column);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          int rt;
          int rows;
          char x_path[1024];
          char x_table[1024];
          char x_column[1024];
          char *err_msg;
          SetLastDirectory(lastDir);
          strcpy(x_path, path.ToUTF8());
          strcpy(x_table, dlg.GetTable().ToUTF8());
          srid = dlg.GetSRID();
          strcpy(x_column, dlg.GetColumn().ToUTF8());
          if (dlg.CreateSpatialIndex() == true)
            spatial_index = 1;
          else
            spatial_index = 0;
          colname_case = dlg.GetColnameCase();
          ::wxBeginBusyCursor();
          rt =
            load_geojson(SqliteHandle, x_path, x_table, x_column, spatial_index,
                         srid, colname_case, &rows, &err_msg);
          if (rt && dlg.IsUpdateStatistics())
            {
              // immediately updating Layer Statistics
              char *emsg = NULL;
              char *sql =
                sqlite3_mprintf("SELECT UpdateLayerStatistics(%Q)", x_table);
              ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &emsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("UpdateLayerStatistics error:") +
                               wxString::FromUTF8(emsg), wxT("spatialite_gui"),
                               wxOK | wxICON_INFORMATION, this);
                  sqlite3_free(emsg);
                }
            }
          ::wxEndBusyCursor();
          if (rt)
            {
              char *msg =
                sqlite3_mprintf
                ("load GeoJSON OK: Inserted %d rows into \"%s\" from GeoJSON",
                 rows);
              wxMessageBox(wxString::FromUTF8(msg), wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
              sqlite3_free(msg);
              InitTableTree();
          } else
            wxMessageBox(wxT("load GeoJSON error:") +
                         wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                         wxOK | wxICON_ERROR, this);
        }
    }
}

void MyFrame::OnVirtualGeoJSON(wxCommandEvent & WXUNUSED(event))
{
//
// creating a VirtualGeoJSON
//
  int ret;
  wxString charset;
  int srid;
  char dummy[128];
  wxString sql;
  char *xsql;
  wxString path;
  wxString table;
  wxString lastDir;
  wxString geometryType;
  char *errMsg = NULL;
  char *name;
  char *xname;
  int colname_case;
  sqlite3 *sqlite = GetSqlite();
  wxFileDialog fileDialog(this, wxT("VirtualGeoJSON"),
                          wxT(""),
                          wxT("file.geojson"),
                          wxT
                          ("GeoJSON (*.geojson)|*.geojson|All files (*.*)|*.*"),
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      VirtualGeoJsonDialog dlg;
      dlg.Create(this, path, table);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          table = dlg.GetTable();
          srid = dlg.GetSRID();
          colname_case = dlg.GetColnameCase();
      } else
        return;
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      sql = wxT("CREATE VIRTUAL TABLE \"");
      name = (char *) malloc((table.Len() * 4) + 1);
      strcpy(name, table.ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("\"\nUSING VirtualGeoJSON('");
      sql += file.GetPath();
      sql += file.GetPathSeparator();
      sql += file.GetFullName();
      sprintf(dummy, "', %d", srid);
      sql += wxString::FromUTF8(dummy);
      switch (colname_case)
        {
          case GAIA_DBF_COLNAME_LOWERCASE:
            sql += wxT(", 'LOWER')");
            break;
          case GAIA_DBF_COLNAME_UPPERCASE:
            sql += wxT(", 'UPPER')");
            break;
          case GAIA_DBF_COLNAME_CASE_IGNORE:
          default:
            sql += wxT(", 'SAME')");
            break;
        };
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      wxMessageBox(wxT("Virtual Table ") + table +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      InitTableTree();
    }
}

void MyFrame::OnLoadTxt(wxCommandEvent & WXUNUSED(event))
{
//
// loading a CSV/TXT
//
  int ret;
  wxString charset;
  wxString sql;
  wxString path;
  wxString table;
  wxString lastDir;
  bool first_titles;
  bool decimal_comma;
  char separator;
  char text_separator;
  wxString filelist = wxT("TXT and CSV files (*.txt;*.csv)|*.txt;*.csv");
  filelist +=
    wxT("|Text file (*.txt)|*.txt|CSV file (*.csv)|*.csv|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Load CSV/TXT"),
                          wxT(""),
                          wxT("textfile.txt"),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      LoadTxtDialog dlg;
      dlg.Create(this, path, table, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          SetLastDirectory(lastDir);
          table = dlg.GetTable();
          charset = dlg.GetCharset();
          first_titles = dlg.IsFirstLineTitles();
          decimal_comma = dlg.IsDecimalPointComma();
          separator = dlg.GetSeparator();
          text_separator = dlg.GetTextSeparator();
          char decimal_separator = '.';
          if (decimal_comma == true)
            decimal_separator = ',';
          LoadText(path, table, charset, first_titles, decimal_separator,
                   separator, text_separator);
        }
    }
}

void MyFrame::OnVirtualTxt(wxCommandEvent & WXUNUSED(event))
{
//
// creating a VirtualText
//
  int ret;
  wxString charset;
  wxString sql;
  char *xsql;
  wxString path;
  wxString table;
  wxString lastDir;
  bool first_titles;
  bool decimal_comma;
  char separator;
  char text_separator;
  char dummy[16];
  char *errMsg = NULL;
  char *name;
  char *xname;
  sqlite3 *sqlite = GetSqlite();
  wxString filelist = wxT("TXT and CSV files (*.txt;*.csv)|*.txt;*.csv");
  filelist +=
    wxT("|Text file (*.txt)|*.txt|CSV file (*.csv)|*.csv|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("VirtualText"),
                          wxT(""),
                          wxT("textfile.txt"),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      VirtualTxtDialog dlg;
      dlg.Create(this, path, table, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          table = dlg.GetTable();
          charset = dlg.GetCharset();
          first_titles = dlg.IsFirstLineTitles();
          decimal_comma = dlg.IsDecimalPointComma();
          separator = dlg.GetSeparator();
          text_separator = dlg.GetTextSeparator();
      } else
        return;
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      sql = wxT("CREATE VIRTUAL TABLE \"");
      name = (char *) malloc((table.Len() * 4) + 1);
      strcpy(name, table.ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("\"\nUSING VirtualText('");
      sql += path;
      sql += wxT("',\n'");
      sql += charset;
      if (first_titles == true)
        sql += wxT("', 1");
      else
        sql += wxT("', 0");
      if (decimal_comma == true)
        sql += wxT(", COMMA");
      else
        sql += wxT(", POINT");
      if (text_separator == '\'')
        sql += wxT(", SINGLEQUOTE");
      else if (text_separator == '"')
        sql += wxT(", DOUBLEQUOTE");
      else
        sql += wxT(", NONE");
      if (separator == '\t')
        sql += wxT(", TAB");
      else
        {
          sprintf(dummy, ", '%c'", separator);
          sql += wxString::FromUTF8(dummy);
        }
      sql += wxT(")");
      ::wxBeginBusyCursor();
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      ::wxEndBusyCursor();
      wxMessageBox(wxT("Virtual Table ") + table +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      InitTableTree();
    }
}

void MyFrame::OnLoadDbf(wxCommandEvent & WXUNUSED(event))
{
//
// loading a DBF
//
  int ret;
  wxString charset;
  wxString sql;
  wxString path;
  wxString table;
  wxString lastDir;
  wxString filelist =
    wxT
    ("DBF files (*.dbf)|*.dbf|Zipfile Archive (*.zip)|*.zip|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Load DBF"), wxT(""), wxT("dbfile.dbf"),
                          filelist, wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      if (file.GetExt().CmpNoCase(wxT("zip")) == 0)
        {
          wxString path = fileDialog.GetPath();
          return DoLoadZipDbf(path);
        }
      table = file.GetName();
      LoadDbfDialog dlg;
      dlg.Create(this, path, table, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          int rt;
          int rows;
          char x_path[1024];
          char x_table[1024];
          char x_charset[1024];
          char x_pkey[1024];
          char *pkey;
          char err_msg[1024];
          int text_dates = 0;
          int colname_case;
          SetLastDirectory(lastDir);
          strcpy(x_path, path.ToUTF8());
          strcpy(x_table, dlg.GetTable().ToUTF8());
          strcpy(x_charset, dlg.GetCharset().ToUTF8());
          if (dlg.IsUserDefinedPKey() == false)
            pkey = NULL;
          else
            {
              strcpy(x_pkey, dlg.GetPKColumn().ToUTF8());
              pkey = x_pkey;
            }
          text_dates = dlg.IsTextDates();
          colname_case = dlg.GetColnameCase();

          rt =
            load_dbf_ex3(SqliteHandle, x_path, x_table, pkey, x_charset, 0,
                         text_dates, &rows, colname_case, err_msg);
          if (rt)
            wxMessageBox(wxT("load dbf OK:") +
                         wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                         wxOK | wxICON_INFORMATION, this);
          else
            wxMessageBox(wxT("load dbf error:") +
                         wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                         wxOK | wxICON_ERROR, this);
        }
    }
}

void MyFrame::DoLoadZipDbf(wxString & str_path)
{
//
// loading a DBF file from within a Zipfile
//
  int count;
  bool err = false;
  int ret;
  wxString table;
  wxString lastDir;
  int text_dates;
  int colname_case;
  wxFileName file(str_path);
  ChoooseZipDbfDialog dlg0;
  LoadZipDbfDialog dlg;
  wxString filename;
  char *zip_path = (char *) malloc(str_path.Len() * 4);
  strcpy(zip_path, str_path.ToUTF8());
  if (!gaiaZipfileNumDBF(zip_path, &count))
    err = true;
  if (count < 1)
    err = true;
  if (err)
    {
      wxMessageBox(wxT("No DBF file found in:") + str_path,
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      goto end;
    }
  lastDir = file.GetPath();
  dlg0.Create(this, zip_path, count);
  ret = dlg0.ShowModal();
  if (ret != wxID_OK)
    goto end;
  filename = dlg0.GetFilename();
  file = wxFileName(wxString::FromUTF8(filename));
  table = file.GetName();
  dlg.Create(this, str_path, filename, table, LocaleCharset);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      int rt;
      int rows;
      char x_table[1024];
      char x_pkey[1024];
      char *pkey;
      char x_charset[1024];
      char err_msg[1024];
      char *x_filename;
      SetLastDirectory(lastDir);
      strcpy(x_table, dlg.GetTable().ToUTF8());
      strcpy(x_charset, dlg.GetCharset().ToUTF8());
      if (dlg.IsUserDefinedPKey() == false)
        pkey = NULL;
      else
        {
          strcpy(x_pkey, dlg.GetPKColumn().ToUTF8());
          pkey = x_pkey;
        }
      text_dates = dlg.IsTextDates();
      colname_case = dlg.GetColnameCase();
      x_filename = (char *) malloc(filename.Len() * 4);
      strcpy(x_filename, filename.ToUTF8());
      ::wxBeginBusyCursor();
      rt =
        load_zip_dbf(SqliteHandle, zip_path, x_filename, x_table,
                     pkey, x_charset, 0, text_dates, &rows, colname_case,
                     err_msg);
      ::wxEndBusyCursor();
      free(x_filename);
      if (rt)
        {
          wxMessageBox(wxT("load DBF OK:") +
                       wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          InitTableTree();
      } else
        wxMessageBox(wxT("load DBF error:") +
                     wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                     wxOK | wxICON_ERROR, this);
    }

end:
  if (zip_path != NULL)
    free(zip_path);
}

void MyFrame::OnVirtualDbf(wxCommandEvent & WXUNUSED(event))
{
//
// creating a VirtualDbf
//
  int ret;
  wxString charset;
  wxString sql;
  char *xsql;
  wxString path;
  wxString table;
  wxString lastDir;
  char *errMsg = NULL;
  char *name;
  char *xname;
  bool text_dates = false;
  sqlite3 *sqlite = GetSqlite();
  wxString filelist = wxT("DBF files (*.dbf)|*.dbf|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("VirtualDbf"),
                          wxT(""),
                          wxT("dbfile.dbf"),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      VirtualDbfDialog dlg;
      dlg.Create(this, path, table, LocaleCharset);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          table = dlg.GetTable();
          charset = dlg.GetCharset();
          text_dates = dlg.IsTextDates();
      } else
        return;
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      sql = wxT("CREATE VIRTUAL TABLE \"");
      name = (char *) malloc((table.Len() * 4) + 1);
      strcpy(name, table.ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("\"\nUSING VirtualDbf('");
      sql += path;
      sql += wxT("', '");
      sql += charset;
      if (text_dates)
        sql += wxT("', 1)");
      else
        sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      wxMessageBox(wxT("Virtual Table ") + table +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      InitTableTree();
    }
}

void MyFrame::OnLoadXL(wxCommandEvent & WXUNUSED(event))
{
//
// loading an XLS
//
#ifndef OMIT_FREEXL             // FreeXL is enabled
  int ret;
  wxString charset;
  wxString sql;
  wxString path;
  wxString table;
  int sheetIndex;
  int firstLineTitles;
  char err_msg[1024];
  unsigned int rows;
  wxString lastDir;
  wxString filelist =
    wxT("Microsoft Excel spreadsheets (*.xls)|*.xls|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Load XLS"), wxT(""),
                          wxT("spreadsheet.xls"), filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      LoadXLDialog dlg;
      dlg.Create(this, path, table);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          char x_path[1024];
          char x_table[1024];
          table = dlg.GetTable();
          sheetIndex = dlg.GetWorksheetIndex();
          if (dlg.IsFirstLineTitles() == true)
            firstLineTitles = 1;
          else
            firstLineTitles = 0;
          strcpy(x_path, path.ToUTF8());
          strcpy(x_table, table.ToUTF8());
          if (!load_XL
              (SqliteHandle, x_path, x_table, sheetIndex, firstLineTitles,
               &rows, err_msg))
            {
              wxMessageBox(wxT("load XL error:") +
                           wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
            }
        }
    }
#else // FreeXL disabled
  wxString msg = wxT("This copy of spatialite_gui was built explictly\n");
  msg += wxT("disabling the FreeXL support.\n");
  msg += wxT("Sorry ...");
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_EXCLAMATION, this);
#endif // end FreeXL conditional
}

void MyFrame::OnVirtualXL(wxCommandEvent & WXUNUSED(event))
{
//
// creating a VirtualXL
//
#ifndef OMIT_FREEXL             // FreeXL is enabled
  int ret;
  wxString charset;
  wxString sql;
  char *xsql;
  wxString path;
  wxString table;
  int sheetIndex;
  bool firstLineTitles;
  wxString lastDir;
  char *errMsg = NULL;
  char *name;
  char *xname;
  sqlite3 *sqlite = GetSqlite();
  wxString filelist =
    wxT("Microsoft Excel spreadsheets (*.xls)|*.xls|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("VirtualXL"), wxT(""),
                          wxT("spreadsheet.xls"), filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      table = file.GetName();
      VirtualXLDialog dlg;
      dlg.Create(this, path, table);
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          table = dlg.GetTable();
          sheetIndex = dlg.GetWorksheetIndex();
          firstLineTitles = dlg.IsFirstLineTitles();
      } else
        return;
      lastDir = file.GetPath();
      SetLastDirectory(lastDir);
      sql = wxT("CREATE VIRTUAL TABLE \"");
      name = (char *) malloc((table.Len() * 4) + 1);
      strcpy(name, table.ToUTF8());
      xname = gaiaDoubleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("\"\nUSING VirtualXL('");
      sql += path;
      xname = sqlite3_mprintf("', %d, %d)", sheetIndex,
                              (firstLineTitles == true) ? 1 : 0);
      sql += wxString::FromUTF8(xname);
      sqlite3_free(xname);
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      wxMessageBox(wxT("Virtual Table ") + table +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      InitTableTree();
    }
#else // FreeXL disabled
  wxString msg = wxT("This copy of spatialite_gui was built explictly\n");
  msg += wxT("disabling the FreeXL support.\n");
  msg += wxT("Sorry ...");
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_EXCLAMATION, this);
#endif // end FreeXL conditional
}

void MyFrame::OnNetwork(wxCommandEvent & WXUNUSED(event))
{
//
// building a Network
//
  NetworkDialog dlg;
  int ret;
  wxString table;
  wxString from;
  wxString to;
  bool isNoGeometry;
  wxString geom;
  bool isNameEnabled;
  wxString name;
  bool isGeomLength;
  wxString cost;
  bool isBidirectional;
  bool isOneWays;
  wxString oneWayToFrom;
  wxString oneWayFromTo;
  bool aStarSupported;
  wxString dataTableName;
  wxString virtualTableName;
  bool overwrite;
  dlg.Create(this);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      table = dlg.GetTableName();
      from = dlg.GetFromColumn();
      to = dlg.GetToColumn();
      isNoGeometry = dlg.IsNoGeometry();
      geom = dlg.GetGeomColumn();
      isNameEnabled = dlg.IsNameEnabled();
      name = dlg.GetNameColumn();
      isGeomLength = dlg.IsGeomLength();
      cost = dlg.GetCostColumn();
      isBidirectional = dlg.IsBidirectional();
      isOneWays = dlg.IsOneWays();
      oneWayToFrom = dlg.GetOneWayToFrom();
      oneWayFromTo = dlg.GetOneWayFromTo();
      aStarSupported = dlg.IsAStarSupported();
      dataTableName = dlg.GetDataTable();
      virtualTableName = dlg.GetVirtualTable();
      overwrite = dlg.IsOverwriteEnabled();
      ::wxBeginBusyCursor();
      CreateNetwork(table, from, to, isNoGeometry, geom, isNameEnabled, name,
                    isGeomLength, cost, isBidirectional, isOneWays,
                    oneWayFromTo, oneWayToFrom, aStarSupported, dataTableName,
                    virtualTableName, overwrite);
      ::wxEndBusyCursor();
    }
}

void MyFrame::OnImportXmlDocuments(wxCommandEvent & WXUNUSED(event))
{
//
// importing XML Documents
//
#ifdef ENABLE_LIBXML2           // only if LIBXML2 is enabled
  XmlDocumentsDialog dlg;
  int ret;
  wxString path;
  bool isFolder;
  int compressed;
  bool isInternalSchema;
  char *schemaURI;
  char xschema[8192];
  wxString lastDir;
  wxString dir_path;
  wxString xml_path;
  wxString suffix;
  wxString table;
  wxString pkName;
  wxString xmlColumn;
  wxString inPathColumn;
  wxString parseErrColumn;
  wxString validateErrColumn;
  wxString schemaUriColumn;
  wxString filelist = wxT("XML Documents (*.xml)|*.xml|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("XML File/Folder selection"),
                          wxT(""),
                          wxT(""),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      xml_path = fileDialog.GetPath();
      wxFileName file(xml_path);
      dir_path = file.GetPath();
  } else
    return;
  dlg.Create(this, dir_path, xml_path);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      ::wxBeginBusyCursor();
      SetLastDirectory(dir_path);
      isFolder = dlg.IsFolder();
      if (isFolder == true)
        path = dlg.GetDirPath();
      else
        path = dlg.GetXmlPath();
      suffix = dlg.GetSuffix();
      table = dlg.GetTargetTable();
      pkName = dlg.GetPkName();
      xmlColumn = dlg.GetXmlColumn();
      compressed = dlg.IsCompressed();
      wxString schema = dlg.GetSchemaURI();
      isInternalSchema = dlg.IsInternalSchemaURI();
      if (schema.Len() == 0)
        schemaURI = NULL;
      else
        {
          strcpy((char *) xschema, schema.ToUTF8());
          schemaURI = xschema;
        }
      inPathColumn = dlg.GetInPathColumn();
      schemaUriColumn = dlg.GetSchemaUriColumn();
      parseErrColumn = dlg.GetParseErrorColumn();
      validateErrColumn = dlg.GetValidateErrorColumn();
      wxString currDir =::wxGetCwd();
      ::wxSetWorkingDirectory(dir_path);
      ImportXmlDocuments(path, isFolder, suffix, table, pkName, xmlColumn,
                         inPathColumn, schemaUriColumn, parseErrColumn,
                         validateErrColumn, compressed, schemaURI,
                         isInternalSchema);
      ::wxSetWorkingDirectory(currDir);
      ::wxEndBusyCursor();
      InitTableTree();
    }
#else

  wxMessageBox(wxT
               ("Sorry, spatialite_gui was built disabling LIBXML2\n\nUnsupported operation"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);

#endif // end LIBXML2 conditionals
}

void MyFrame::OnImportWFS(wxCommandEvent & WXUNUSED(event))
{
//
// importing data from WFS datasource
//
#ifdef ENABLE_LIBXML2           // only if LIBXML2 is enabled
  WfsDialog dlg;
  dlg.Create(this, WfsGetCapabilitiesURL, HttpProxy);
  dlg.ShowModal();

#else

  wxMessageBox(wxT
               ("Sorry, spatialite_gui was built disabling LIBXML2\n\nUnsupported operation"),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);

#endif // end LIBXML2 conditionals
}

void MyFrame::OnImportDXF(wxCommandEvent & WXUNUSED(event))
{
//
// importing DXF file(s)
//
  DxfDialog dlg;
  int ret;
  wxString path;
  bool isFolder;
  wxString lastDir;
  wxString dir_path;
  wxString dxf_path;
  wxString prefix;
  wxString layer;
  int srid;
  bool force2d;
  bool force3d;
  bool mixed;
  bool linked;
  bool unlinked;
  bool append;
  wxString filelist = wxT("DXF drawing file (*.dxf)|*.dxf|All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("DXF File/Folder selection"),
                          wxT(""),
                          wxT(""),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      dxf_path = fileDialog.GetPath();
      wxFileName file(dxf_path);
      dir_path = file.GetPath();
  } else
    return;
  dlg.Create(this, dir_path, dxf_path);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      ::wxBeginBusyCursor();
      SetLastDirectory(dir_path);
      isFolder = dlg.IsFolder();
      if (isFolder == true)
        path = dlg.GetDirPath();
      else
        path = dlg.GetDxfPath();
      prefix = dlg.GetPrefix();
      layer = dlg.GetSingleLayer();
      srid = dlg.GetSRID();
      force2d = dlg.IsForce2D();
      force3d = dlg.IsForce3D();
      mixed = dlg.IsImportMixed();
      linked = dlg.IsLinkedRings();
      unlinked = dlg.IsUnlinkedRings();
      append = dlg.IsAppendMode();
      wxString currDir =::wxGetCwd();
      ::wxSetWorkingDirectory(dir_path);
      ImportDXFfiles(path, isFolder, prefix, layer, srid, force2d, force3d,
                     mixed, linked, unlinked, append);
      ::wxSetWorkingDirectory(currDir);
      ::wxEndBusyCursor();
      InitTableTree();
    }
}

rl2PixelPtr MyFrame::DefaultNoData(unsigned char sample, unsigned char pixel,
                                   unsigned char num_bands)
{
// creating a default NO-DATA value
  int nb;
  rl2PixelPtr pxl = rl2_create_pixel(sample, pixel, num_bands);
  if (pxl == NULL)
    return NULL;
  switch (pixel)
    {
      case RL2_PIXEL_MONOCHROME:
        rl2_set_pixel_sample_1bit(pxl, 0);
        break;
      case RL2_PIXEL_PALETTE:
        switch (sample)
          {
            case RL2_SAMPLE_1_BIT:
              rl2_set_pixel_sample_1bit(pxl, 0);
              break;
            case RL2_SAMPLE_2_BIT:
              rl2_set_pixel_sample_2bit(pxl, 0);
              break;
            case RL2_SAMPLE_4_BIT:
              rl2_set_pixel_sample_4bit(pxl, 0);
              break;
            case RL2_SAMPLE_UINT8:
              rl2_set_pixel_sample_uint8(pxl, 0, 0);
              break;
          };
        break;
      case RL2_PIXEL_GRAYSCALE:
        switch (sample)
          {
            case RL2_SAMPLE_1_BIT:
              rl2_set_pixel_sample_1bit(pxl, 1);
              break;
            case RL2_SAMPLE_2_BIT:
              rl2_set_pixel_sample_2bit(pxl, 3);
              break;
            case RL2_SAMPLE_4_BIT:
              rl2_set_pixel_sample_4bit(pxl, 15);
              break;
            case RL2_SAMPLE_UINT8:
              rl2_set_pixel_sample_uint8(pxl, 0, 255);
              break;
            case RL2_SAMPLE_UINT16:
              rl2_set_pixel_sample_uint16(pxl, 0, 0);
              break;
          };
        break;
      case RL2_PIXEL_RGB:
        switch (sample)
          {
            case RL2_SAMPLE_UINT8:
              rl2_set_pixel_sample_uint8(pxl, 0, 255);
              rl2_set_pixel_sample_uint8(pxl, 1, 255);
              rl2_set_pixel_sample_uint8(pxl, 2, 255);
              break;
            case RL2_SAMPLE_UINT16:
              rl2_set_pixel_sample_uint16(pxl, 0, 0);
              rl2_set_pixel_sample_uint16(pxl, 1, 0);
              rl2_set_pixel_sample_uint16(pxl, 2, 0);
              break;
          };
        break;
      case RL2_PIXEL_DATAGRID:
        switch (sample)
          {
            case RL2_SAMPLE_INT8:
              rl2_set_pixel_sample_int8(pxl, 0);
              break;
            case RL2_SAMPLE_UINT8:
              rl2_set_pixel_sample_uint8(pxl, 0, 0);
              break;
            case RL2_SAMPLE_INT16:
              rl2_set_pixel_sample_int16(pxl, 0);
              break;
            case RL2_SAMPLE_UINT16:
              rl2_set_pixel_sample_uint16(pxl, 0, 0);
              break;
            case RL2_SAMPLE_INT32:
              rl2_set_pixel_sample_int32(pxl, 0);
              break;
            case RL2_SAMPLE_UINT32:
              rl2_set_pixel_sample_uint32(pxl, 0);
              break;
            case RL2_SAMPLE_FLOAT:
              rl2_set_pixel_sample_float(pxl, 0.0);
              break;
            case RL2_SAMPLE_DOUBLE:
              rl2_set_pixel_sample_double(pxl, 0.0);
              break;
          };
        break;
      case RL2_PIXEL_MULTIBAND:
        switch (sample)
          {
            case RL2_SAMPLE_UINT8:
              for (nb = 0; nb < num_bands; nb++)
                rl2_set_pixel_sample_uint8(pxl, nb, 255);
              break;
            case RL2_SAMPLE_UINT16:
              for (nb = 0; nb < num_bands; nb++)
                rl2_set_pixel_sample_uint16(pxl, nb, 0);
              break;
          };
        break;
    };
  return pxl;
}

char *MyFrame::GetNum(const char *start, const char *end)
{
// extracting a token
  char *buf;
  int len = end - start;
  if (len <= 0)
    return NULL;
  buf = (char *) malloc(len + 1);
  memcpy(buf, start, len);
  *(buf + len) = '\0';
  return buf;
}

rl2PixelPtr MyFrame::ParseNoData(wxString & NoData, int SampleType,
                                 int PixelType, int NumBands)
{
//
// parsing a NO-DATA string
//
  int band = 0;
  char *num = NULL;
  const char *p;
  const char *start;
  const char *end;
  char int8_value = 0;
  unsigned char uint8_value = 0;
  short int16_value = 0;
  unsigned short uint16_value = 0;
  int int32_value = 0;
  unsigned int uint32_value = 0;
  float flt_value = 0.0;
  double dbl_value = 0.0;
  rl2PixelPtr pixel = NULL;
  unsigned char num_bands = 0;
  char buf[4192];
  const char *in;

  strcpy(buf, NoData.ToUTF8());
  in = buf;

  switch (PixelType)
    {
      case RL2_PIXEL_MONOCHROME:
        num_bands = 1;
        break;
      case RL2_PIXEL_PALETTE:
        num_bands = 1;
        break;
      case RL2_PIXEL_GRAYSCALE:
        num_bands = 1;
        break;
      case RL2_PIXEL_RGB:
        num_bands = 3;
        break;
      case RL2_PIXEL_DATAGRID:
        num_bands = 1;
        break;
      case RL2_PIXEL_MULTIBAND:
        num_bands = NumBands;
        break;
    };
  if (num_bands != NumBands)
    return NULL;
  pixel = rl2_create_pixel(SampleType, PixelType, num_bands);
  if (pixel == NULL)
    return NULL;

  start = in;
  p = in;
  while (1)
    {
      if (*p == ',' || *p == '\0')
        {
          end = p;
          switch (SampleType)
            {
              case RL2_SAMPLE_1_BIT:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                uint8_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_1bit(pixel, int8_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_2_BIT:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                uint8_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_2bit(pixel, int8_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_4_BIT:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                uint8_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_4bit(pixel, int8_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_INT8:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                int8_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_int8(pixel, int8_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_UINT8:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                uint8_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_uint8(pixel, band, uint8_value)
                    != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_INT16:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                int16_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_int16(pixel, int16_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_UINT16:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                uint16_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_uint16
                    (pixel, band, uint16_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_INT32:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                int32_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_int32(pixel, int32_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_UINT32:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                uint32_value = atoi(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_uint32(pixel, uint32_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_FLOAT:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                flt_value = atof(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_float(pixel, flt_value) != RL2_OK)
                  goto error;
                break;
              case RL2_SAMPLE_DOUBLE:
                num = GetNum(start, end);
                if (num == NULL)
                  goto error;
                dbl_value = atof(num);
                free(num);
                num = NULL;
                if (rl2_set_pixel_sample_double(pixel, dbl_value) != RL2_OK)
                  goto error;
                break;
            };
          if (*p == '\0')
            break;
          start = p + 1;
          band++;
        }
      p++;
    }
  return pixel;

error:
  if (num != NULL)
    free(num);
  if (pixel != NULL)
    rl2_destroy_pixel(pixel);
  return NULL;
}

bool MyFrame::CreateRasterCoverage(wxString & CoverageName, wxString & Title,
                                   wxString & Abstract, int SampleType,
                                   int PixelType, int NumBands, int Compression,
                                   int Quality, int TileWidth, int TileHeight,
                                   bool NotGeoreferenced, int Srid,
                                   double HorzResolution, double VertResolution,
                                   wxString NoData, bool StrictResolution,
                                   bool MixedResolutions, bool InputPaths,
                                   bool MD5, bool Summary, int RedBand,
                                   int GreenBand, int BlueBand, int NIRband,
                                   bool AutoNDVI, bool isQueryable)
{
//
// actual creation of a Raster Coverage 
//
  int ret;
  const char *sql;
  char *errMsg = NULL;
  sqlite3_stmt *stmt = NULL;
  char *err_msg = NULL;
  int strict_resolution = 0;
  int mixed_resolutions = 0;
  int section_paths = 0;
  int section_md5 = 0;
  int section_summary = 0;
  int queryable = 0;
  rl2PalettePtr palette = NULL;
  rl2PixelPtr no_data = NULL;
  bool retval = false;
  char *dummy;

// starting a transaction 
  ret = sqlite3_exec(SqliteHandle, "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("BEGIN error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
// just in case, attempting to (re)create raster meta-tables
  ret =
    sqlite3_exec(SqliteHandle, "SELECT CreateRasterCoveragesTable()", NULL,
                 NULL, &err_msg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CreateRasterCoveragesTable() error: ") +
                   wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(err_msg);
      return false;
    }
  ret =
    sqlite3_exec(SqliteHandle, "SELECT CreateStylingTables()", NULL, NULL,
                 &err_msg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CreateStylingTables() error: ") +
                   wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(err_msg);
      return false;
    }

  if (NoData.Len() == 0)
    {
      // creating a default NO-DATA value
      no_data = DefaultNoData(SampleType, PixelType, NumBands);
  } else
    {
      if (NoData.CmpNoCase(wxT("NONE")) == 0)
        {
          // creating a NoData pixel of the NONE type
          no_data = rl2_create_pixel_none();
      } else
        {
          // creating the NoData pixel
          no_data = ParseNoData(NoData, SampleType, PixelType, NumBands);
        }
      if (no_data == NULL)
        {
          wxMessageBox(wxT("ERROR: invalid NO-DATA string"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return false;
        }
    }
  if (PixelType == RL2_PIXEL_PALETTE)
    {
      // creating a default PALETTE
      palette = rl2_create_palette(1);
      rl2_set_palette_color(palette, 0, 255, 255, 255);
    }
  if (NotGeoreferenced == true)
    {
      Srid = -1;
      HorzResolution = 1.0;
      VertResolution = 1.0;
    }
  if (StrictResolution)
    strict_resolution = 1;
  if (MixedResolutions)
    mixed_resolutions = 1;
  if (InputPaths)
    section_paths = 1;
  if (MD5)
    section_md5 = 1;
  if (Summary)
    section_summary = 1;
  if (isQueryable)
    queryable = 1;

  if (rl2_create_dbms_coverage
      (SqliteHandle, CoverageName.ToUTF8(), SampleType, PixelType, NumBands,
       Compression, Quality, TileWidth, TileHeight, Srid, HorzResolution,
       VertResolution, no_data, palette, strict_resolution, mixed_resolutions,
       section_paths, section_md5, section_summary, queryable) != RL2_OK)
    {
      wxMessageBox(wxT("ERROR: unable to create Raster Coverage ") +
                   CoverageName, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                   this);
      return false;
    }
  if (PixelType == RL2_PIXEL_MULTIBAND)
    {
      if (RedBand >= 0 && GreenBand >= 0 && BlueBand >= 0 && NIRband >= 0)
        {
          // attempting to register the default Band Settings
          rl2_set_dbms_coverage_default_bands(SqliteHandle,
                                              CoverageName.ToUTF8(), RedBand,
                                              GreenBand, BlueBand, NIRband);
          if (AutoNDVI == true)
            rl2_enable_dbms_coverage_auto_ndvi(SqliteHandle,
                                               CoverageName.ToUTF8(), 1);
        }
    }
// committing the still pending transaction 
  ret = sqlite3_exec(SqliteHandle, "COMMIT", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("COMMIT error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
// updating Title and Abstract
  sql = "SELECT RL2_SetRasterCoverageInfos(?, ?, ?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(SqliteHandle);
      wxMessageBox(wxT("ERROR - SetRasterCoverageInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return false;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Title.Len() * 4) + 1);
  strcpy(dummy, Title.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((Abstract.Len() * 4) + 1);
  strcpy(dummy, Abstract.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    retval = true;
  else
    {
      const char *msg = sqlite3_errmsg(SqliteHandle);
      wxMessageBox(wxT("ERROR - SetRasterCoverageInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      retval = false;
    }
  sqlite3_finalize(stmt);
  return retval;
}

bool MyFrame::DoGetRasterCoverageInfos(wxString & coverage, wxString & title,
                                       wxString & abstract,
                                       wxString & copyright,
                                       wxString & data_license,
                                       wxString & sample, wxString & pixel,
                                       wxString & compression, int *srid,
                                       bool *mixed_resolutions)
{
//
// attempting to retrieve basic Raster Coverage Infos
//
  const char *sql;
  int ret;
  int count = 0;
  int err = 0;
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  int mixed;

  sql =
    "SELECT c.title, c.abstract, c.sample_type, c.pixel_type, c.num_bands, "
    "c.compression, c.srid, c.mixed_resolutions, c.copyright, l.name "
    "FROM raster_coverages AS c JOIN data_licenses AS l ON (c.license = l.id) "
    "WHERE Lower(coverage_name) = Lower(?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((coverage.Len() * 4) + 1);
  strcpy(dummy, coverage.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, coverage.Len(), SQLITE_TRANSIENT);
  free(dummy);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          count++;
          const char *str;
          if (sqlite3_column_type(stmt, 0) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 0);
              title = wxString::FromUTF8(str);
          } else
            title = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 1) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 1);
              abstract = wxString::FromUTF8(str);
          } else
            abstract = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 2) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 2);
              if (strcasecmp(str, "1-BIT") == 0)
                sample = wxT("1-BIT");
              else if (strcasecmp(str, "2-BIT") == 0)
                sample = wxT("2-BIT");
              else if (strcasecmp(str, "4-BIT") == 0)
                sample = wxT("4-BIT");
              else if (strcasecmp(str, "INT8") == 0)
                sample = wxT("Signed Integer 8 bit");
              else if (strcasecmp(str, "UINT8") == 0)
                sample = wxT("Unsigned Integer 8 bit");
              else if (strcasecmp(str, "INT16") == 0)
                sample = wxT("Signed Integer 16 bit");
              else if (strcasecmp(str, "UINT16") == 0)
                sample = wxT("Unsigned Integer 16 bit");
              else if (strcasecmp(str, "INT32") == 0)
                sample = wxT("Signed Integer 32 bit");
              else if (strcasecmp(str, "UINT32") == 0)
                sample = wxT("Unsigned Integer 32 bit");
              else if (strcasecmp(str, "FLOAT") == 0)
                sample = wxT("Floating point single precision: 32 bit");
              else if (strcasecmp(str, "DOUBLE") == 0)
                sample = wxT("Floating point double precision: 64 bit");
              else
                {
                  sample = wxT("*** Unknown ***");
                  err = 1;
                }
          } else
            {
              sample = wxT("*** Unknown ***");
              err = 1;
            }
          if (sqlite3_column_type(stmt, 3) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 3);
              if (strcasecmp(str, "MONOCHROME") == 0)
                pixel = wxT("MonoChrome");
              else if (strcasecmp(str, "PALETTE") == 0)
                pixel = wxT("Palette-based");
              else if (strcasecmp(str, "GRAYSCALE") == 0)
                pixel = wxT("GrayScale (black&white)");
              else if (strcasecmp(str, "RGB") == 0)
                pixel = wxT("RGB (TrueColor)");
              else if (strcasecmp(str, "MULTIBAND") == 0)
                {
                  pixel = wxT("MultiBand");
                  if (sqlite3_column_type(stmt, 4) == SQLITE_INTEGER)
                    {
                      char dummy[128];
                      sprintf(dummy, ": %d bands", sqlite3_column_int(stmt, 4));
                      pixel += wxString::FromUTF8(dummy);
                  } else
                    {
                      pixel += wxT(": unknown bands");
                      err = 1;
                    }
              } else if (strcasecmp(str, "DATAGRID") == 0)
                pixel = wxT("Data Grid");
              else
                {
                  pixel = wxT("*** Unknown ***");
                  err = 1;
                }
          } else
            {
              pixel = wxT("*** Unknown ***");
              err = 1;
            }
          if (sqlite3_column_type(stmt, 5) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 5);
              if (strcasecmp(str, "NONE") == 0)
                compression = wxT("None, not compressed");
              else if (strcasecmp(str, "DEFLATE") == 0)
                compression =
                  wxT("Deflate (ZIP) with Delta-encoding: lossless");
              else if (strcasecmp(str, "DEFLATE_NO") == 0)
                compression = wxT("Deflate (ZIP), no Delta-encoding: lossless");
              else if (strcasecmp(str, "LZMA") == 0)
                compression = wxT("LZMA (7-Zip) with Delta-encoding: lossless");
              else if (strcasecmp(str, "LZMA_NO") == 0)
                compression = wxT("LZMA (7-Zip), no Delta-encoding: lossless");
              else if (strcasecmp(str, "LZMA") == 0)
                compression = wxT("LZ4 (7-Zip) with Delta-encoding: lossless");
              else if (strcasecmp(str, "LZMA_NO") == 0)
                compression = wxT("LZ4 (fast lossless), no Delta-encoding");
              else if (strcasecmp(str, "ZSTD") == 0)
                compression = wxT("ZSTD (7-Zip) with Delta-encoding: lossless");
              else if (strcasecmp(str, "ZSTD_NO") == 0)
                compression =
                  wxT("ZSTD (Zstandad), no Delta-encoding: lossless");
              else if (strcasecmp(str, "PNG") == 0)
                compression = wxT("PNG: lossless");
              else if (strcasecmp(str, "JPEG") == 0)
                compression = wxT("JPEG: lossy");
              else if (strcasecmp(str, "LOSSY_WEBP") == 0)
                compression = wxT("WebP: lossy mode");
              else if (strcasecmp(str, "LOSSLESS_WEBP") == 0)
                compression = wxT("WebP: lossless mode");
              else if (strcasecmp(str, "CCITTFAX4") == 0)
                compression = wxT("Fax Group 4: lossless");
              else if (strcasecmp(str, "LOSSY_JP2") == 0)
                compression = wxT("JPEG2000: lossy mode");
              else if (strcasecmp(str, "LOSSLESS_JP2") == 0)
                compression = wxT("JPEG2000: lossless mode");
              else
                {
                  compression = wxT("*** Unknown ***");
                  err = 1;
                }
          } else
            {
              compression = wxT("*** Unknown ***");
              err = 1;
            }
          if (sqlite3_column_type(stmt, 6) == SQLITE_INTEGER)
            *srid = sqlite3_column_int(stmt, 6);
          else
            {
              *srid = -1;
              err = 1;
            }
          if (sqlite3_column_type(stmt, 7) == SQLITE_INTEGER)
            mixed = sqlite3_column_int(stmt, 7);
          else
            {
              mixed = -1;
              err = 1;
            }
          if (sqlite3_column_type(stmt, 8) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 8);
              copyright = wxString::FromUTF8(str);
          } else
            copyright = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 9) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 9);
              data_license = wxString::FromUTF8(str);
          } else
            data_license = wxT("*** Unknown ***");
      } else
        break;
    }
  sqlite3_finalize(stmt);
  if (count != 1 || err)
    {
      wxMessageBox(wxT("Unable to retrieve a valid Raster Coverage named: ") +
                   coverage, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      return false;
    }
  if (mixed)
    *mixed_resolutions = true;
  else
    *mixed_resolutions = false;
  return true;
}

bool MyFrame::DoGetVectorCoverageInfos(wxString & coverage, wxString & title,
                                       wxString & abstract,
                                       wxString & copyright,
                                       wxString & data_license, int *origin,
                                       wxString & type, int *srid)
{
//
// attempting to retrieve basic Vector Coverage Infos
//
  const char *sql;
  int ret;
  int count = 0;
  int err = 0;
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  char cvg[1024];

  wxCharBuffer buffer = coverage.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  int xtype = FindVectorType(cvg);

  switch (xtype)
    {
      case VECTOR_GEOTABLE:
        sql =
          "SELECT c.title, c.abstract, g.geometry_type, g.srid, "
          "c.copyright, l.name FROM MAIN.vector_coverages AS c "
          "JOIN MAIN.geometry_columns AS g ON ("
          "Lower(c.f_table_name) = Lower(g.f_table_name) AND "
          "Lower(c.f_geometry_column) = Lower(g.f_geometry_column)) "
          "JOIN data_licenses AS l ON (c.license = l.id) "
          "WHERE Lower(c.coverage_name) = Lower(?)";
        break;
      case VECTOR_SPATIALVIEW:
        sql =
          "SELECT c.title, c.abstract, g.geometry_type, g.srid, "
          "c.copyright, l.name FROM MAIN.vector_coverages AS c "
          "JOIN MAIN.views_geometry_columns AS x ON ("
          "Lower(c.view_name) = Lower(x.view_name) AND "
          "Lower(c.view_geometry) = Lower(x.view_geometry)) "
          "JOIN MAIN.geometry_columns AS g ON ("
          "Lower(x.f_table_name) = Lower(g.f_table_name) AND "
          "Lower(x.f_geometry_column) = Lower(g.f_geometry_column)) "
          "JOIN MAIN.data_licenses AS l ON (c.license = l.id) "
          "WHERE Lower(c.coverage_name) = Lower(?)";
        break;
      case VECTOR_VIRTUALTABLE:
        sql =
          "SELECT c.title, c.abstract, g.geometry_type, g.srid, "
          "c.copyright, l.name FROM MAIN.vector_coverages AS c "
          "JOIN MAIN.virts_geometry_columns AS g ON ("
          "Lower(c.virt_name) = Lower(g.virt_name) AND "
          "Lower(c.virt_geometry) = Lower(g.virt_geometry)) "
          "JOIN MAIN.data_licenses AS l ON (c.license = l.id) "
          "WHERE Lower(c.coverage_name) = Lower(?)";
        break;
      case VECTOR_TOPOGEO:
        sql =
          "SELECT c.title, c.abstract, 7777 AS geometry_type, g.srid, "
          "c.copyright, l.name FROM MAIN.vector_coverages AS c "
          "JOIN MAIN.topologies AS g ON ("
          "Lower(c.topology_name) = Lower(g.topology_name)) "
          "JOIN MAIN.data_licenses AS l ON (c.license = l.id) "
          "WHERE Lower(c.coverage_name) = Lower(?)";
        break;
      case VECTOR_TOPONET:
        sql =
          "SELECT c.title, c.abstract, 8888 AS geometry_type, g.srid, "
          "c.copyright, l.name FROM MAIN.vector_coverages AS c "
          "JOIN MAIN.networks AS g ON ("
          "Lower(c.network_name) = Lower(g.network_name)) "
          "JOIN MAIN.data_licenses AS l ON (c.license = l.id) "
          "WHERE Lower(c.coverage_name) = Lower(?)";
        break;
      case VECTOR_UNKNOWN:
      default:
        return false;
        break;
    };
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((coverage.Len() * 4) + 1);
  strcpy(dummy, coverage.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, coverage.Len(), SQLITE_TRANSIENT);
  free(dummy);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          count++;
          const char *str;
          if (sqlite3_column_type(stmt, 0) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 0);
              title = wxString::FromUTF8(str);
          } else
            title = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 1) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 1);
              abstract = wxString::FromUTF8(str);
          } else
            abstract = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 2) == SQLITE_INTEGER)
            {
              int gtype = sqlite3_column_int(stmt, 2);
              switch (gtype)
                {
                  case 0:
                  case 1000:
                  case 2000:
                  case 3000:
                    type = wxT("GEOMETRY");
                    break;
                  case 1:
                  case 1001:
                  case 2001:
                  case 3001:
                    type = wxT("POINT");
                    break;
                  case 2:
                  case 1002:
                  case 2002:
                  case 3002:
                    type = wxT("LINESTRING");
                    break;
                  case 3:
                  case 1003:
                  case 2003:
                  case 3003:
                    type = wxT("POLYGON");
                    break;
                  case 4:
                  case 1004:
                  case 2004:
                  case 3004:
                    type = wxT("MULTIPOINT");
                    break;
                  case 5:
                  case 1005:
                  case 2005:
                  case 3005:
                    type = wxT("MULTILINESTRING");
                    break;
                  case 6:
                  case 1006:
                  case 2006:
                  case 3006:
                    type = wxT("MULTIPOLYGON");
                    break;
                  case 7:
                  case 1007:
                  case 2007:
                  case 3007:
                    type = wxT("GEOMETRYCOLLECTION");
                    break;
                  case 7777:
                    type = wxT("TOPOLOGY-GEOMETRY");
                    break;
                  case 8888:
                    type = wxT("TOPOLOGY-NETWORK");
                    break;
                  default:
                    type = wxT("***  UNKNOWN  ***");
                    break;
                };
              switch (gtype)
                {
                  case 0:
                  case 1:
                  case 2:
                  case 3:
                  case 4:
                  case 5:
                  case 6:
                  case 7:
                    type += wxT(" XY");
                    break;
                  case 1000:
                  case 1001:
                  case 1002:
                  case 1003:
                  case 1004:
                  case 1005:
                  case 1006:
                  case 1007:
                    type += wxT(" XYZ");
                    break;
                  case 2000:
                  case 2001:
                  case 2002:
                  case 2003:
                  case 2004:
                  case 2005:
                  case 2006:
                  case 2007:
                    type += wxT(" XYM");
                    break;
                  case 3000:
                  case 3001:
                  case 3002:
                  case 3003:
                  case 3004:
                  case 3005:
                  case 3006:
                  case 3007:
                    type += wxT(" XYZM");
                    break;
                };
          } else
            {
              type = wxT("***  UNKNOWN  ***");
              err = 1;
            }
          if (sqlite3_column_type(stmt, 3) == SQLITE_INTEGER)
            *srid = sqlite3_column_int(stmt, 3);
          else
            {
              *srid = -1;
              err = 1;
            }
          if (sqlite3_column_type(stmt, 4) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 4);
              copyright = wxString::FromUTF8(str);
          } else
            copyright = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 5) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 5);
              data_license = wxString::FromUTF8(str);
          } else
            data_license = wxT("*** Unknown ***");

      } else
        break;
    }
  sqlite3_finalize(stmt);
  if (count != 1 || err)
    {
      wxMessageBox(wxT("Unable to retrieve a valid Vector Coverage named: ") +
                   coverage, wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      return false;
    }
  *origin = xtype;
  return true;
}

bool MyFrame::DoGetWmsLayerInfos(wxString & url, wxString & layer_name,
                                 wxString & title, wxString & abstract,
                                 wxString & copyright, int *license_id,
                                 wxString & data_license, bool *is_queryable,
                                 wxString & getfeatureinfo_url)
{
//
// attempting to retrieve basic WMS Layer Infos
//
  const char *sql;
  int ret;
  int count = 0;
  sqlite3_stmt *stmt = NULL;
  char *dummy;

  sql =
    "SELECT w.title, w.abstract, w.copyright, w.license, l.name, "
    "w.is_queryable, w.getfeatureinfo_url FROM MAIN.wms_getmap AS w "
    "JOIN MAIN.data_licenses AS l ON (w.license = l.id) "
    "WHERE Lower(w.url) = Lower(?) AND Lower(w.layer_name) = Lower(?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((url.Len() * 4) + 1);
  strcpy(dummy, url.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, url.Len(), SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((layer_name.Len() * 4) + 1);
  strcpy(dummy, layer_name.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, layer_name.Len(), SQLITE_TRANSIENT);
  free(dummy);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          count++;
          const char *str;
          if (sqlite3_column_type(stmt, 0) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 0);
              title = wxString::FromUTF8(str);
          } else
            title = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 1) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 1);
              abstract = wxString::FromUTF8(str);
          } else
            abstract = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 2) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 2);
              copyright = wxString::FromUTF8(str);
          } else
            copyright = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 3) == SQLITE_INTEGER)
            *license_id = sqlite3_column_int(stmt, 3);
          else
            *license_id = 0;
          if (sqlite3_column_type(stmt, 4) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 4);
              data_license = wxString::FromUTF8(str);
          } else
            data_license = wxT("*** Unknown ***");
          if (sqlite3_column_type(stmt, 5) == SQLITE_INTEGER)
            {
              int flag = sqlite3_column_int(stmt, 5);
              if (flag)
                *is_queryable = true;
              else
                *is_queryable = false;
          } else
            *is_queryable = false;
          if (sqlite3_column_type(stmt, 6) == SQLITE_TEXT)
            {
              str = (const char *) sqlite3_column_text(stmt, 6);
              if (str == NULL)
                getfeatureinfo_url = wxEmptyString;
              else
                getfeatureinfo_url = wxString::FromUTF8(str);
          } else
            data_license = wxEmptyString;
        }
    }
  sqlite3_finalize(stmt);
  if (count != 1)
    {
      wxMessageBox(wxT("Unable to retrieve a valid WMS Layer named: ") +
                   layer_name + wxT(" on MAIN DB"), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return false;
    }
  return true;
}

bool MyFrame::DoImportRaster(sqlite3_stmt * stmt, wxString & coverage,
                             wxString & path, int forced_srid,
                             bool with_worldfile, bool pyramidize)
{
//
// attempting to import a single Raster file into a Raster Coverage
//
  int ret;
  bool retval = false;
  char *dummy;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((coverage.Len() * 4) + 1);
  strcpy(dummy, coverage.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  dummy = (char *) malloc((path.Len() * 4) + 1);
  strcpy(dummy, path.ToUTF8());
  sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (with_worldfile == false)
    sqlite3_bind_int(stmt, 3, 0);
  else
    sqlite3_bind_int(stmt, 3, 1);
  sqlite3_bind_int(stmt, 4, forced_srid);
  if (pyramidize == false)
    sqlite3_bind_int(stmt, 5, 0);
  else
    sqlite3_bind_int(stmt, 5, 1);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          int status = sqlite3_column_int(stmt, 0);
          if (status <= 0)
            retval = false;
          else
            retval = true;
      } else
        retval = false;
    }
  return retval;
}

void MyFrame::DoCreateStylingTables()
{
//
// attempting to create (just in case) the Styling Tables
//
  int ret;
  char *errMsg = NULL;

// starting a transaction 
  ret = sqlite3_exec(SqliteHandle, "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("BEGIN error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
// just in case, attempting to (re)create styling meta-tables
  ret =
    sqlite3_exec(SqliteHandle, "SELECT CreateStylingTables()", NULL,
                 NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CreateStylingTables() error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
// committing the still pending transaction 
  ret = sqlite3_exec(SqliteHandle, "COMMIT", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("COMMIT error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
}

void MyFrame::OnImportExifPhotos(wxCommandEvent & WXUNUSED(event))
{
//
// importing EXIF Photos
//
  ExifDialog dlg;
  int ret;
  wxString path;
  bool isFolder;
  bool isMetadata;
  bool isGpsOnly;
  wxString lastDir;
  wxString dir_path;
  wxString img_path;
  wxString filelist = wxT("JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|");
  filelist += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("EXIF File/Folder selection"),
                          wxT(""),
                          wxT(""),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      img_path = fileDialog.GetPath();
      wxFileName file(img_path);
      dir_path = file.GetPath();
  } else
    return;
  dlg.Create(this, dir_path, img_path);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      SetLastDirectory(dir_path);
      isFolder = dlg.IsFolder();
      if (isFolder == true)
        path = dlg.GetDirPath();
      else
        path = dlg.GetImgPath();
      isMetadata = dlg.IsMetadata();
      isGpsOnly = dlg.IsGpsOnly();
      ImportExifPhotos(path, isFolder, isMetadata, isGpsOnly);
    }
}

void MyFrame::OnImportGpsPhotos(wxCommandEvent & WXUNUSED(event))
{
//
// importing GPS Photos
//
  GpsPicsDialog dlg;
  int ret;
  wxString path;
  wxString table;
  wxString geometry;
  bool isFolder;
  bool isUpdateStatistics;
  bool isSpatialIndex;
  wxString lastDir;
  wxString dir_path;
  wxString img_path;
  wxString filelist = wxT("JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|");
  filelist += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("EXIF-GPS File/Folder selection"),
                          wxT(""),
                          wxT(""),
                          filelist,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      img_path = fileDialog.GetPath();
      wxFileName file(img_path);
      dir_path = file.GetPath();
  } else
    return;
  dlg.Create(this, dir_path, img_path);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      SetLastDirectory(dir_path);
      table = dlg.GetTable();
      geometry = dlg.GetGeometry();
      isFolder = dlg.IsFolder();
      if (isFolder == true)
        path = dlg.GetDirPath();
      else
        path = dlg.GetImgPath();
      isUpdateStatistics = dlg.IsUpdateStatistics();
      isSpatialIndex = dlg.IsSpatialIndex();
      ImportGpsPhotos(path, isFolder, table, geometry, isUpdateStatistics,
                      isSpatialIndex);
    }
}

void MyFrame::OnSrids(wxCommandEvent & WXUNUSED(event))
{
//
// searching a SRID by name
//
  SearchSridDialog dlg;
  int ret;
  wxString string;
  wxString sql;
  dlg.Create(this);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      sql = wxT("SELECT * FROM spatial_ref_sys\n");
      if (dlg.IsSearchBySrid() == true)
        {
          // searching by SRID
          char txt[128];
          sprintf(txt, "WHERE srid = %d", dlg.GetSRID());
          sql += wxString::FromUTF8(txt);
      } else
        {
          // searching by NAME
          string = dlg.GetString();
          sql += wxT("WHERE ref_sys_name LIKE '%");
          sql += string;
          sql += wxT("%'\nORDER BY srid");
        }
      QueryView->SetSql(sql, true);
    }
}

void MyFrame::OnCharset(wxCommandEvent & WXUNUSED(event))
{
//
// setting the default CHARSET
//
  DefaultCharsetDialog dlg;
  int ret;
  dlg.Create(this, DefaultCharset, AskCharset);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      DefaultCharset = dlg.GetCharset();
      AskCharset = dlg.IsSetAskCharset();
    }
}

void MyFrame::OnTimerAutoSave(wxTimerEvent & WXUNUSED(event))
{
//
// AutoSave - Timer event handler
// 
  int tc = sqlite3_total_changes(SqliteHandle);
  if (tc != LastTotalChanges)
    MemoryDbSave();
  if (AutoSaveInterval <= 0)
    {
      delete TimerAutoSave;
      TimerAutoSave = NULL;
  } else
    TimerAutoSave->Start(AutoSaveInterval * 1000, wxTIMER_ONE_SHOT);
}

bool MyFrame::ExecuteSql(const char *sql, int rowNo)
{
//
// executes an SQL statement from the SQL script
//
  int ret;
  char *errMsg = NULL;
  wxString msg;
  char dummy[128];
  ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      sprintf(dummy, "row %d\n\nSQLite SQL error: ", rowNo);
      msg = wxString::FromUTF8(dummy);
      wxMessageBox(msg + wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  return true;
}

void MyFrame::Rollback()
{
//
// performing a ROLLBACK
//
  sqlite3_exec(SqliteHandle, "ROLLBACK", NULL, NULL, NULL);
}

bool MyFrame::OpenDB(bool read_only)
{
//
// establishing a physical connection to some DB SQLite
//
  int ret;
  char *errMsg = NULL;
  char *xpath;
  int mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
  ReadOnlyConnection = false;
// checking for existence
  wxFileName flnm(SqlitePath);
  if (flnm.FileExists() == false)
    {
      wxMessageBox(wxT("Not existing DB\n") + SqlitePath, wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      SqliteHandle = NULL;
      SpliteInternalCache = NULL;
      ClearTableTree();
      MemoryDatabase = false;
      return false;
    }
  if (read_only)
    {
      mode = SQLITE_OPEN_READONLY;
      ReadOnlyConnection = true;
  } else
    {
      // testing for an appropriate Security level
      char path[1024];
      strcpy(path, SqlitePath.ToUTF8());
      TestSecurityRelaxed(path);
    }
  xpath = (char *) malloc((SqlitePath.Len() * 4) + 1);
  strcpy(xpath, SqlitePath.ToUTF8());
  ret = sqlite3_open_v2(xpath, &SqliteHandle, mode, NULL);
  free(xpath);
  if (ret)
    {
      // an error occurred
      wxString errCause;
      errCause = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
      sqlite3_close(SqliteHandle);
      wxMessageBox(wxT("Failure while connecting to DB\n\n") + errCause +
                   wxT("\n") + SqlitePath, wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      SqliteHandle = NULL;
      SpliteInternalCache = NULL;
      ClearTableTree();
      MemoryDatabase = false;
      return false;
    }
// setting up the internal cache
  SpliteInternalCache = spatialite_alloc_connection();
  spatialite_init_ex(SqliteHandle, SpliteInternalCache, 0);
  rl2_init(SqliteHandle, RL2PrivateData, 0);
  DoInitVirtualPG();
// enabling LOAD_EXTENSION
  ret = sqlite3_enable_load_extension(SqliteHandle, 1);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Unable to enable LOAD_EXTENSION"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      sqlite3_free(errMsg);
    }
// activating Foreign Key constraints
  ret = sqlite3_exec(SqliteHandle, "PRAGMA foreign_keys = 1", NULL, 0, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Unable to activate FOREIGN_KEY constraints"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      sqlite3_free(errMsg);
    }
// setting RL2 MaxThreads
  char sqlmax[1024];
  sprintf(sqlmax, "SELECT RL2_SetMaxThreads(%d)", RL2MaxThreads);
  sqlite3_exec(SqliteHandle, sqlmax, NULL, 0, NULL);
  AutoPostgresStart();

  AutoFDOStart();
  AutoGPKGStart();
  InitTableTree();
  LoadHistory();
  return true;
}

void MyFrame::AutoPostgresStart()
{
// creating POSTGIS_GEOMETRY_COLUMNS (Temporary Table)
  const char *sql =
    "CREATE TEMPORARY TABLE IF NOT EXISTS postgis_geometry_columns (\n"
    "\tf_table_name TEXT NOT NULL,\n\tf_geometry_column TEXT NOT NULL,\n"
    "\tcoord_dimension INTEGER NOT NULL,\n\tsrid INTEGER NOT NULL,\n"
    "\tgeometry_type TEXT NOT NULL,\n\tCONSTRAINT tmp_postgis_pk PRIMARY "
    "KEY (f_table_name, f_geometry_column))";
  sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
// creating TMP_VECTOR_COVERAGES (Temporary Table)
  sql =
    "CREATE TEMPORARY TABLE IF NOT EXISTS tmp_vector_coverages (\n"
    "\tcoverage_name TEXT NOT NULL PRIMARY KEY,\n"
    "\tf_table_name TEXT,\n\tf_geometry_column TEXT,\n"
    "\tview_name TEXT,\n\tview_geometry TEXT,\n\tvirt_name TEXT,\n"
    "\tvirt_geometry TEXT,\n\ttopology_name TEXT,\n\tnetwork_name TEXT,\n"
    "\tpostgis_table TEXT,\n\tpostgis_geometry TEXT,\n"
    "\tgeo_minx DOUBLE,\n\tgeo_miny DOUBLE,\n\tgeo_maxx DOUBLE,\n"
    "\tgeo_maxy DOUBLE,\n\textent_minx DOUBLE,\n\textent_miny DOUBLE,\n"
    "\textent_maxx DOUBLE,\n\textent_maxy DOUBLE)";
  sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
}

void MyFrame::LastDitchMemoryDbSave()
{
//
// performing the last desperate attempt to save a MEMORY-DB
//
  int tc;
  int ret;
  wxString lastDir;
  if (MemoryDatabase == false)
    return;
  if (!SqliteHandle)
    return;
  tc = sqlite3_total_changes(SqliteHandle);
  if (tc == LastTotalChanges)
    return;
  while (1)
    {
      // OK, this MEMORY-DB needs to be saved
      if (MemoryDbSave() == true)
        break;
      // we must ask the user
      wxString msg =
        wxT("WARNING: the MEMORY-DB contains uncommitted changes\n\n");
      msg += wxT("The MEMORY_DB is intrinsecally volatile, so these changes\n");
      msg +=
        wxT("will be irremediably lost if you don't export them to some\n");
      msg += wxT("persistent storage [i.e. on the file-system]\n\n");
      msg +=
        wxT
        ("Do you want to export [SAVE] the MEMORY-DB to some external database ?");
      ret =
        wxMessageBox(msg, wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION,
                     this);
      if (ret != wxYES)
        break;
      // asking a PATHNAME to the user
      wxFileDialog fileDialog(this, wxT("Saving the MEMORY-DB"),
                              wxT(""), wxT("db.sqlite"),
                              wxT
                              ("SQLite DB (*.sqlite)|*.sqlite|All files (*.*)|*.*"),
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                              wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
      lastDir = GetLastDirectory();
      if (lastDir.Len() >= 1)
        fileDialog.SetDirectory(lastDir);
      ret = fileDialog.ShowModal();
      if (ret == wxID_OK)
        {
          // exporting the external DB
          ExternalSqlitePath = fileDialog.GetPath();
          if (MemoryDbSave() == true)
            {
              wxMessageBox(wxT("Ok, MEMORY-DB was successfully saved"),
                           wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                           this);
              break;
            }
        }
    }
}

int MyFrame::GetDecimalPrecision()
{
//
// querying the currently set decimal precision
//
  int precision = -1;
  const char *sql = "SELECT GetDecimalPrecision()";
  int ret;
  sqlite3_stmt *stmt = NULL;
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return precision;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            precision = sqlite3_column_int(stmt, 0);
      } else
        break;
    }
  sqlite3_finalize(stmt);
  return precision;
}

void MyFrame::DoUpdateRL2MaxThreads()
{
//
// updating RL2MaxThreads
//
  const char *sql = "SELECT RL2_GetMaxThreads()";
  int ret;
  sqlite3_stmt *stmt = NULL;
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  RL2MaxThreads = 1;
  if (ret != SQLITE_OK)
    return;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            RL2MaxThreads = sqlite3_column_int(stmt, 0);
      } else
        break;
    }
  sqlite3_finalize(stmt);
}

int MyFrame::GetRL2MaxThreads()
{
//
// retrieving RL2MaxThreads
//
  const char *sql = "SELECT RL2_GetMaxThreads()";
  int ret;
  sqlite3_stmt *stmt = NULL;
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  int maxThreads = 1;
  if (ret != SQLITE_OK)
    return maxThreads;
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            maxThreads = sqlite3_column_int(stmt, 0);
      } else
        break;
    }
  sqlite3_finalize(stmt);
  return maxThreads;
}

void MyFrame::DoAutoDetachDatabases()
{
//
// detaching all attached databases before
// closing the main DB connection
//
  MyAttachedDB *db = AttachedList.GetFirst();
  while (db != NULL)
    {
      int ret;
      char *errMsg = NULL;
      wxString symbol;
      char *xsql;
      if (db->IsGeoPackage() == true)
        AutoGPKGStop(db->GetDbPrefix());
      if (db->IsFdoOgr() == true)
        AutoFDOStop(db->GetDbPrefix());
      wxString sql = wxT("DETACH DATABASE \"");
      sql += db->GetDbPrefix() + wxT("\"");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(SqliteHandle, xsql, NULL, 0, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("DetachDatabase: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
        }
      db = db->GetNext();
    }
}

void MyFrame::CloseDB()
{
//
// disconnecting current SQLite DB
//
  if (!SqliteHandle)
    return;
  AutoPostgresStop();
  AutoFDOStop();
  AutoGPKGStop();
  if (AutoFDOmsg.Len() > 0)
    wxMessageBox(AutoFDOmsg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                 this);
  if (AutoGPKGmsg.Len() > 0)
    wxMessageBox(AutoGPKGmsg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                 this);
  LastDitchMemoryDbSave();
  DoUpdateRL2MaxThreads();
  DoAutoDetachDatabases();
  sqlite3_close(SqliteHandle);
  SqliteHandle = NULL;
  spatialite_cleanup_ex(SpliteInternalCache);
  SpliteInternalCache = NULL;
  SqlitePath = wxT("");
  ReadOnlyConnection = false;
  MemoryDatabase = false;
  ResetSecurity();
  AttachedList.Flush();
  ClearTableTree();
// dropping POSTGIS_GEOMETRY_COLUMNS (Temporary Table)
  const char *sql = "DROP TABLE IF EXISTS temp.postgis_geometry_columns";
  sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
// dropping TMP_VECTOR_COVERAGES (Temporary Table)
  sql = "DROP TABLE IF EXISTS temp.tmp_vector_coverages";
  sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
}

bool MyFrame::CreateDB()
{
// creating a new, empty SQLite DB
  int ret;
  char path[1024];
  char *errMsg = NULL;
  ReadOnlyConnection = false;
  if (MemoryDatabase == true)
    strcpy(path, ":memory:");
  else
    {
      strcpy(path, SqlitePath.ToUTF8());
      unlink(path);
    }
  SetSecurityRelaxed();
  ret =
    sqlite3_open_v2(path, &SqliteHandle,
                    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  if (ret)
    {
      // an error occurred
      wxString errCause;
      errCause = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
      sqlite3_close(SqliteHandle);
      wxMessageBox(wxT("An error occurred\n\n") + errCause + wxT("\n") +
                   SqlitePath, wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                   this);
      SqliteHandle = NULL;
      SpliteInternalCache = NULL;
      ClearTableTree();
      MemoryDatabase = false;
      return false;
    }
// setting up the internal cache
  SpliteInternalCache = spatialite_alloc_connection();
  spatialite_init_ex(SqliteHandle, SpliteInternalCache, 0);
  rl2_init(SqliteHandle, RL2PrivateData, 0);
  DoInitVirtualPG();
// enabling LOAD_EXTENSION
  ret = sqlite3_enable_load_extension(SqliteHandle, 1);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Unable to enable LOAD_EXTENSION"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      sqlite3_free(errMsg);
    }
// activating Foreign Key constraints
  ret = sqlite3_exec(SqliteHandle, "PRAGMA foreign_keys = 1", NULL, 0, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Unable to activate FOREIGN_KEY constraints"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      sqlite3_free(errMsg);
      SqliteHandle = NULL;
      ClearTableTree();
      MemoryDatabase = false;
      return false;
    }
// setting RL2 MaxThreads
  char sqlmax[1024];
  sprintf(sqlmax, "SELECT RL2_SetMaxThreads(%d)", RL2MaxThreads);
  sqlite3_exec(SqliteHandle, sqlmax, NULL, 0, &errMsg);
  InitializeSpatialMetadata();
  AutoPostgresStart();
  AutoFDOStart();
  AutoGPKGStart();
  InitTableTree();
  return true;
}

void MyFrame::InitializeSpatialMetadata()
{
// attempting to perform self-initialization for a newly created DB
  int ret;
  char sql[1024];
  char *errMsg = NULL;
  int count;
  int i;
  char **results;
  int rows;
  int columns;

  if (SqliteHandle == NULL)
    return;
// checking if this DB is really empty 
  strcpy(sql, "SELECT Count(*) from sqlite_master");
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        count = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);

  if (count > 0)
    return;

// all right, it's empty: proceding to initialize
  strcpy(sql, "SELECT InitSpatialMetadataFull(1)");
  ret = sqlite3_exec(SqliteHandle, sql, NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("Unable to initialite SpatialMetadataFull: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
}

bool MyFrame::HasHistory(void)
{
// checks if the "sql_statement" table exists
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  wxString sql;
  char *xsql;
  bool ok_sql = false;
  bool ok_cause = false;
  bool ok_time = false;
  char *errMsg = NULL;

  sql = wxT("PRAGMA table_info(sql_statements_log)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *name = results[(i * columns) + 1];
          wxString x = wxString::FromUTF8(name);
          if (x.CmpNoCase(wxT("sql_statement")) == 0)
            ok_sql = true;
          if (x.CmpNoCase(wxT("error_cause")) == 0)
            ok_cause = true;
          if (x.CmpNoCase(wxT("sql_statement")) == 0)
            ok_time = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_sql && ok_cause && ok_time)
    return true;
  return false;
}

void MyFrame::LoadHistory(void)
{
// attempting to load the most recent SQL history
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  wxString sql;
  char *xsql;
  char *errMsg = NULL;
  if (!HasHistory())
    return;

  sql = wxT("SELECT sql_statement FROM sql_statements_log ");
  sql += wxT("WHERE error_cause = 'success' ORDER BY time_end DESC ");
  sql += wxT("LIMIT 25");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i < rows; i++)
        {
          const char *name = results[(i * columns) + 1];
          wxString x = wxString::FromUTF8(name);
          QueryView->GetHistory()->Prepend(x);
        }
    }
  sqlite3_free_table(results);
  QueryView->SetHistoryStates();
}

bool MyFrame::IsSpatialIndex(wxString & tableName)
{
// testing if this table belongs to some R*Tree Spatial Index
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *geom;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
  bool status = false;

  if (tableName.StartsWith(wxT("cache_")))
    return IsMbrCache(tableName);

// fetching any defined Spatial Index
  sql =
    wxT
    ("SELECT f_table_name, f_geometry_column FROM main.geometry_columns WHERE spatial_index_enabled IN (1,3)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      if (errMsg != NULL)
        sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          geom = results[(i * columns) + 1];
          sprintf(dummy, "idx_%s_%s", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "idx_%s_%s_node", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "idx_%s_%s_parent", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "idx_%s_%s_rowid", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
        }
    }
end:
  sqlite3_free_table(results);
  return status;
}

bool MyFrame::IsMbrCache(wxString & tableName)
{
// testing if this table belongs to some MBR cache
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *geom;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
  bool status = false;

// fetching any defined MBR cache
  sql =
    wxT
    ("SELECT f_table_name, f_geometry_column FROM main.geometry_columns WHERE spatial_index_enabled = 2");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      if (errMsg != NULL)
        sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          geom = results[(i * columns) + 1];
          sprintf(dummy, "cache_%s_%s", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
        }
    }
end:
  sqlite3_free_table(results);
  return status;
}

bool MyFrame::IsGeoPackageSpatialIndex(wxString & tableName)
{
// testing if this table belongs to some GeoPackage R*Tree Spatial Index
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *geom;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
  bool status = false;
// fetching any defined Spatial Index
  sql =
    wxT
    ("SELECT g.table_name, g.column_name FROM main.gpkg_geometry_columns AS g "
     "JOIN main.gpkg_extensions AS e ON (g.table_name LIKE e.table_name AND "
     "g.column_name LIKE e.column_name AND e.extension_name LIKE 'gpkg_rtree_index')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      if (errMsg != NULL)
        sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          geom = results[(i * columns) + 1];
          sprintf(dummy, "rtree_%s_%s", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "rtree_%s_%s_node", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "rtree_%s_%s_parent", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "rtree_%s_%s_rowid", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
        }
    }
end:
  sqlite3_free_table(results);
  return status;
}

bool MyFrame::IsSpatialIndex(wxString & dbAlias, wxString & tableName)
{
// testing if this table belongs to some R*Tree Spatial Index
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *geom;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;

  if (tableName.StartsWith(wxT("cache_")))
    return IsMbrCache(dbAlias, tableName);

// fetching any defined Spatial Index
  sql = wxT("SELECT f_table_name, f_geometry_column FROM \"");
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql +=
    dbAlias2 + wxT("\".geometry_columns WHERE spatial_index_enabled IN (1, 3)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      if (errMsg != NULL)
        sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          geom = results[(i * columns) + 1];
          sprintf(dummy, "idx_%s_%s", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
          sprintf(dummy, "idx_%s_%s_node", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
          sprintf(dummy, "idx_%s_%s_parent", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
          sprintf(dummy, "idx_%s_%s_rowid", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
        }
    }
  sqlite3_free_table(results);
  return false;

exit_ok:
  sqlite3_free_table(results);
  return true;
}

bool MyFrame::IsMbrCache(wxString & dbAlias, wxString & tableName)
{
// testing if this table belongs to some MBR cache
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *geom;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
// fetching any defined MBR cache
  sql = wxT("SELECT f_table_name, f_geometry_column FROM \"");
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql += dbAlias2 + wxT("\".geometry_columns WHERE spatial_index_enabled = 2");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      if (errMsg != NULL)
        sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          geom = results[(i * columns) + 1];
          sprintf(dummy, "cache_%s_%s", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
        }
    }
  sqlite3_free_table(results);
  return false;

exit_ok:
  sqlite3_free_table(results);
  return true;
}

bool MyFrame::IsGeoPackageSpatialIndex(wxString & dbAlias, wxString & tableName)
{
// testing if this table belongs to some GeoPackage R*Tree Spatial Index
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *geom;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
  bool status = false;
// fetching any defined Spatial Index
  sql = wxT("SELECT g.table_name, g.column_name FROM \"");
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql += dbAlias2 + wxT("\".gpkg_geometry_columns AS g JOIN \"");
  sql +=
    dbAlias2 +
    wxT("\".gpkg_extensions AS e ON (g.table_name LIKE e.table_name AND "
        "g.column_name LIKE e.column_name AND e.extension_name LIKE 'gpkg_rtree_index')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      if (errMsg != NULL)
        sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          geom = results[(i * columns) + 1];
          sprintf(dummy, "rtree_%s_%s", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "rtree_%s_%s_node", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "rtree_%s_%s_parent", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "rtree_%s_%s_rowid", name, geom);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
        }
    }
end:
  sqlite3_free_table(results);
  return status;
}

bool MyFrame::IsTopoFaceSpatialIndex(wxString & tableName)
{
// testing if this table belongs to some Topo-Face Spatial Index
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
  bool status = false;
// fetching all Topo-Geo
  sql = wxT("SELECT topology_name FROM topologies");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          sprintf(dummy, "idx_%s_face_rtree", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "idx_%s_face_rtree_node", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "idx_%s_face_rtree_parent", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
          sprintf(dummy, "idx_%s_face_rtree_rowid", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            {
              status = true;
              goto end;
            }
        }
    }
end:
  sqlite3_free_table(results);
  return status;
}

void MyFrame::DoubleQuoted(wxString & str)
{
// double quoting a text string
  char *name;
  char *name2;

  if (str.Len() == 0)
    return;
  name = (char *) malloc((str.Len() * 4) + 1);
  strcpy(name, str.ToUTF8());
  name2 = gaiaDoubleQuotedSql(name);
  free(name);
  str = wxString::FromUTF8(name2);
  free(name2);
}

bool MyFrame::IsTopoFaceSpatialIndex(wxString & dbAlias, wxString & tableName)
{
// testing if this table belongs to some Topo-Face Spatial Index
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char dummy[2048];
  wxString tblName;
  wxString sql;
  char *xsql;
// fetching all Topo-Geo
  sql = wxT("SELECT topology_name FROM \"");
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql += dbAlias2 + wxT("\".topologies");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          sprintf(dummy, "idx_%s_face_rtree", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
          sprintf(dummy, "idx_%s_face_rtree_node", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
          sprintf(dummy, "idx_%s_face_rtree_parent", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
          sprintf(dummy, "idx_%s_face_rtree_rowid", name);
          tblName = wxString::FromUTF8(dummy);
          if (tableName.CmpNoCase(tblName) == 0)
            goto exit_ok;
        }
    }
  sqlite3_free_table(results);
  return false;

exit_ok:
  sqlite3_free_table(results);
  return true;
}

bool MyFrame::IsView(wxString & view)
{
// testing for a View
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *dummy;
  char *xdummy;
  wxString sql;
  char *xsql;
  bool is_view = false;

  sql = wxT("SELECT Count(*) FROM sqlite_master ");
  sql += wxT("WHERE Lower(tbl_name) = Lower('");
  dummy = (char *) malloc((view.Len() * 4) + 1);
  strcpy(dummy, view.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("') AND type = 'view'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          if (atoi(results[(i * columns) + 0]) == 1)
            is_view = true;
        }
    }
  sqlite3_free_table(results);
  return is_view;
}

bool MyFrame::IsWritableView(wxString & view)
{
// testing for a Writable View
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *dummy;
  char *xdummy;
  wxString sql;
  char *xsql;
  bool is_writable = false;

  if (ExistsViewsGeometryColumns() == false)
    return false;

  sql = wxT("SELECT Count(*) FROM views_geometry_columns ");
  sql += wxT("WHERE Lower(view_name) = Lower('");
  dummy = (char *) malloc((view.Len() * 4) + 1);
  strcpy(dummy, view.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("') AND read_only = 0");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          if (atoi(results[(i * columns) + 0]) == 1)
            is_writable = true;
        }
    }
  sqlite3_free_table(results);
  return is_writable;
}

void MyFrame::GetWritableViewPK(wxString & view, wxString & pk_name)
{
// attempting to retrieve the PK name for a Writable View
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *dummy;
  char *xdummy;
  wxString sql;
  char *xsql;
  int error = false;
  wxString u_name;

  pk_name = wxEmptyString;
  if (ExistsViewsGeometryColumns() == false)
    return;

  sql = wxT("SELECT view_rowid FROM views_geometry_columns ");
  sql += wxT("WHERE Lower(view_name) = Lower('");
  dummy = (char *) malloc((view.Len() * 4) + 1);
  strcpy(dummy, view.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("') AND read_only = 0");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *value = results[(i * columns) + 0];
          pk_name = wxString::FromUTF8(value);
          if (u_name.Len() > 0)
            {
              if (pk_name.CmpNoCase(u_name) != 0)
                error = true;
            }
          u_name = pk_name;
        }
    }
  sqlite3_free_table(results);
  if (error == true)
    pk_name = wxEmptyString;
}

bool MyFrame::ElementaryViewGeoms(wxString & inTable, wxString & geometry,
                                  wxString & type, int *srid,
                                  wxString & coordDims, bool *spIdx)
{
// ancillary SQL for ElementaryGeoms - SpatialView
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *gtp;
  char *dims;
  char *dummy;
  char *xdummy;
  wxString sql;
  char *xsql;
  bool found = false;
  int metadata_type;

// fetching metadata
  metadata_type = GetMetaDataType();
  if (metadata_type == METADATA_LEGACY)
    sql =
      wxT("SELECT g.type, g.coord_dimension, g.srid, g.spatial_index_enabled ");
  else if (metadata_type == METADATA_CURRENT)
    sql = wxT("SELECT g.geometry_type, g.srid, g.spatial_index_enabled ");
  else
    return false;
  sql = sql += wxT("FROM views_geometry_columns as v ");
  sql += wxT("JOIN geometry_columns AS g ON (g.f_table_name = v.f_table_name ");
  sql += wxT("AND g.f_geometry_column = v.f_geometry_column) ");
  sql += wxT("WHERE Lower(v.view_name) = Lower('");
  dummy = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(dummy, inTable.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("') AND Lower(v.view_geometry) = Lower('");
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      found = false;
      for (i = 1; i <= rows; i++)
        {
          if (metadata_type == METADATA_LEGACY)
            {
              // legacy Spatial MetaData layout
              found = true;
              gtp = results[(i * columns) + 0];
              dims = results[(i * columns) + 1];
              *srid = atoi(results[(i * columns) + 2]);
              if (atoi(results[(i * columns) + 3]) == 0)
                *spIdx = false;
              else
                *spIdx = true;
              if (strcasecmp(gtp, "POINT") == 0
                  || strcasecmp(gtp, "MULTIPOINT") == 0)
                type = wxString::FromUTF8("POINT");
              else if (strcasecmp(gtp, "LINESTRING") == 0
                       || strcasecmp(gtp, "MULTILINESTRING") == 0)
                type = wxString::FromUTF8("LINESTRING");
              else if (strcasecmp(gtp, "POLYGON") == 0
                       || strcasecmp(gtp, "MULTIPOLYGON") == 0)
                type = wxString::FromUTF8("POLYGON");
              else
                type = wxString::FromUTF8("GEOMETRY");
              coordDims = wxString::FromUTF8(dims);
          } else
            {
              // current Spatial MetaData layout
              found = true;
              switch (atoi(results[(i * columns) + 0]))
                {
                  case 0:
                  case 1:
                  case 2:
                  case 3:
                  case 7:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XY");
                    break;
                  case 1000:
                  case 1001:
                  case 1002:
                  case 1003:
                  case 1007:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XYZ");
                    break;
                  case 2000:
                  case 2001:
                  case 2002:
                  case 2003:
                  case 2007:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XYM");
                    break;
                  case 3000:
                  case 3001:
                  case 3002:
                  case 3003:
                  case 3007:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XYZM");
                    break;
                  case 4:
                    type = wxT("POINT");
                    coordDims = wxT("XY");
                    break;
                  case 1004:
                    type = wxT("POINT");
                    coordDims = wxT("XYZ");
                    break;
                  case 2004:
                    type = wxT("POINT");
                    coordDims = wxT("XYM");
                    break;
                  case 3004:
                    type = wxT("POINT");
                    coordDims = wxT("XYZM");
                    break;
                  case 5:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XY");
                    break;
                  case 1005:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XYZ");
                    break;
                  case 2005:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XYM");
                    break;
                  case 3005:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XYZM");
                    break;
                  case 6:
                    type = wxT("POLYGON");
                    coordDims = wxT("XY");
                    break;
                  case 1006:
                    type = wxT("POLYGON");
                    coordDims = wxT("XYZ");
                    break;
                  case 2006:
                    type = wxT("POLYGON");
                    coordDims = wxT("XYM");
                    break;
                  case 3006:
                    type = wxT("POLYGON");
                    coordDims = wxT("XYZM");
                    break;
                };
              *srid = atoi(results[(i * columns) + 1]);
              if (atoi(results[(i * columns) + 2]) == 0)
                *spIdx = false;
              else
                *spIdx = true;
            }
        }
    }
  sqlite3_free_table(results);
  return found;
}

bool MyFrame::ElementaryVirtGeoms(wxString & inTable, wxString & geometry,
                                  wxString & type, int *srid,
                                  wxString & coordDims)
{
// ancillary SQL for ElementaryGeoms - VirtualTable
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *dummy;
  char *xdummy;
  wxString sql;
  char *xsql;
  bool found = false;

// fetching metadata
  sql = wxT("SELECT geometry_type, coord_dimension, srid ");
  sql += wxT("FROM virts_geometry_columns ");
  sql += wxT("WHERE Lower(virt_name) = Lower('");
  dummy = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(dummy, inTable.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("') AND Lower(virt_geometry) = Lower('");
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      found = false;
      for (i = 1; i <= rows; i++)
        {
          found = true;
          switch (atoi(results[(i * columns) + 0]))
            {
              case 0:
              case 1:
              case 2:
              case 3:
              case 7:
                type = wxT("GEOMETRY");
                coordDims = wxT("XY");
                break;
              case 1000:
              case 1001:
              case 1002:
              case 1003:
              case 1007:
                type = wxT("GEOMETRY");
                coordDims = wxT("XYZ");
                break;
              case 2000:
              case 2001:
              case 2002:
              case 2003:
              case 2007:
                type = wxT("GEOMETRY");
                coordDims = wxT("XYM");
                break;
              case 3000:
              case 3001:
              case 3002:
              case 3003:
              case 3007:
                type = wxT("GEOMETRY");
                coordDims = wxT("XYZM");
                break;
              case 4:
                type = wxT("POINT");
                coordDims = wxT("XY");
                break;
              case 1004:
                type = wxT("POINT");
                coordDims = wxT("XYZ");
                break;
              case 2004:
                type = wxT("POINT");
                coordDims = wxT("XYM");
                break;
              case 3004:
                type = wxT("POINT");
                coordDims = wxT("XYZM");
                break;
              case 5:
                type = wxT("LINESTRING");
                coordDims = wxT("XY");
                break;
              case 1005:
                type = wxT("LINESTRING");
                coordDims = wxT("XYZ");
                break;
              case 2005:
                type = wxT("LINESTRING");
                coordDims = wxT("XYM");
                break;
              case 3005:
                type = wxT("LINESTRING");
                coordDims = wxT("XYZM");
                break;
              case 6:
                type = wxT("POLYGON");
                coordDims = wxT("XY");
                break;
              case 1006:
                type = wxT("POLYGON");
                coordDims = wxT("XYZ");
                break;
              case 2006:
                type = wxT("POLYGON");
                coordDims = wxT("XYM");
                break;
              case 3006:
                type = wxT("POLYGON");
                coordDims = wxT("XYZM");
                break;
            };
          *srid = atoi(results[(i * columns) + 2]);
        }
    }
  sqlite3_free_table(results);
  return found;
}

void MyFrame::ElementaryGeoms(wxString & inTable, wxString & geometry,
                              wxString & outTable, wxString & pKey,
                              wxString & multiID, wxString & type, int *srid,
                              wxString & coordDims, bool *spIdx)
{
// ancillary SQL for ElementaryGeoms
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *gtp;
  char *dims;
  char *dummy;
  char *xdummy;
  wxString sql;
  char *xsql;
  bool ok;
  bool found = false;
  int metadata_type;

  outTable = inTable + wxT("_elem");
  pKey = wxT("pk_elem");
  multiID = wxT("multi_id");
  type = wxT("*** Error ***");
  *srid = 0;
  coordDims = wxT("*** Error ***");
  *spIdx = false;

// fetching metadata
  metadata_type = GetMetaDataType();
  if (metadata_type == METADATA_LEGACY)
    sql =
      wxT
      ("SELECT type, coord_dimension, srid, spatial_index_enabled FROM geometry_columns ");
  else if (metadata_type == METADATA_CURRENT)
    sql =
      wxT
      ("SELECT geometry_type, srid, spatial_index_enabled FROM geometry_columns ");
  else
    return;
  sql += wxT("WHERE Lower(f_table_name) = Lower('");
  dummy = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(dummy, inTable.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("') AND Lower(f_geometry_column) = Lower('");
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      found = false;
      for (i = 1; i <= rows; i++)
        {
          if (metadata_type == METADATA_LEGACY)
            {
              // legacy Spatial MetaData layout
              found = true;
              gtp = results[(i * columns) + 0];
              dims = results[(i * columns) + 1];
              *srid = atoi(results[(i * columns) + 2]);
              if (atoi(results[(i * columns) + 3]) == 0)
                *spIdx = false;
              else
                *spIdx = true;
              if (strcasecmp(gtp, "POINT") == 0
                  || strcasecmp(gtp, "MULTIPOINT") == 0)
                type = wxString::FromUTF8("POINT");
              else if (strcasecmp(gtp, "LINESTRING") == 0
                       || strcasecmp(gtp, "MULTILINESTRING") == 0)
                type = wxString::FromUTF8("LINESTRING");
              else if (strcasecmp(gtp, "POLYGON") == 0
                       || strcasecmp(gtp, "MULTIPOLYGON") == 0)
                type = wxString::FromUTF8("POLYGON");
              else
                type = wxString::FromUTF8("GEOMETRY");
              coordDims = wxString::FromUTF8(dims);
          } else
            {
              // current Spatial MetaData layout
              found = true;
              switch (atoi(results[(i * columns) + 0]))
                {
                  case 0:
                  case 1:
                  case 2:
                  case 3:
                  case 7:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XY");
                    break;
                  case 1000:
                  case 1001:
                  case 1002:
                  case 1003:
                  case 1007:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XYZ");
                    break;
                  case 2000:
                  case 2001:
                  case 2002:
                  case 2003:
                  case 2007:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XYM");
                    break;
                  case 3000:
                  case 3001:
                  case 3002:
                  case 3003:
                  case 3007:
                    type = wxT("GEOMETRY");
                    coordDims = wxT("XYZM");
                    break;
                  case 4:
                    type = wxT("POINT");
                    coordDims = wxT("XY");
                    break;
                  case 1004:
                    type = wxT("POINT");
                    coordDims = wxT("XYZ");
                    break;
                  case 2004:
                    type = wxT("POINT");
                    coordDims = wxT("XYM");
                    break;
                  case 3004:
                    type = wxT("POINT");
                    coordDims = wxT("XYZM");
                    break;
                  case 5:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XY");
                    break;
                  case 1005:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XYZ");
                    break;
                  case 2005:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XYM");
                    break;
                  case 3005:
                    type = wxT("LINESTRING");
                    coordDims = wxT("XYZM");
                    break;
                  case 6:
                    type = wxT("POLYGON");
                    coordDims = wxT("XY");
                    break;
                  case 1006:
                    type = wxT("POLYGON");
                    coordDims = wxT("XYZ");
                    break;
                  case 2006:
                    type = wxT("POLYGON");
                    coordDims = wxT("XYM");
                    break;
                  case 3006:
                    type = wxT("POLYGON");
                    coordDims = wxT("XYZM");
                    break;
                };
              *srid = atoi(results[(i * columns) + 1]);
              if (atoi(results[(i * columns) + 2]) == 0)
                *spIdx = false;
              else
                *spIdx = true;
            }
        }
    }
  sqlite3_free_table(results);

  if (found == false)
    {
      // may it be a SpatialView ?
      if (ElementaryViewGeoms(inTable, geometry, type, srid, coordDims, spIdx)
          == true)
        found = true;
    }
  if (found == false)
    {
      // may it be a VirtualTable ?
      if (ElementaryVirtGeoms(inTable, geometry, type, srid, coordDims) == true)
        found = true;
    }

  ok = true;
  while (ok)
    {
      // creating an unique PrimaryKey name
      ok = false;
      sql = wxT("PRAGMA table_info(\"");
      dummy = (char *) malloc((inTable.Len() * 4) + 1);
      strcpy(dummy, inTable.ToUTF8());
      xdummy = gaiaDoubleQuotedSql(dummy);
      free(dummy);
      sql += wxString::FromUTF8(xdummy);
      free(xdummy);
      sql += wxT("\")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 1];
              wxString x = wxString::FromUTF8(name);
              if (x.CmpNoCase(pKey) == 0)
                {
                  pKey += wxT("_1");
                  ok = true;
                  break;
                }
            }
        }
      sqlite3_free_table(results);
    }

  ok = true;
  while (ok)
    {
      // creating an unique MultiID name
      ok = false;
      sql = wxT("PRAGMA table_info(\"");
      dummy = (char *) malloc((inTable.Len() * 4) + 1);
      strcpy(dummy, inTable.ToUTF8());
      xdummy = gaiaDoubleQuotedSql(dummy);
      free(dummy);
      sql += wxString::FromUTF8(xdummy);
      free(xdummy);
      sql += wxT("\")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 1];
              wxString x = wxString::FromUTF8(name);
              if (x.CmpNoCase(multiID) == 0)
                {
                  multiID += wxT("_1");
                  ok = true;
                  break;
                }
            }
        }
      sqlite3_free_table(results);
    }

  ok = true;
  while (ok)
    {
      // creating an unique Table name
      ok = false;
      sql =
        wxT
        ("SELECT Count(*) FROM sqlite_master WHERE type = 'table' AND Lower(tbl_name) = Lower('");
      dummy = (char *) malloc((outTable.Len() * 4) + 1);
      strcpy(dummy, outTable.ToUTF8());
      xdummy = gaiaSingleQuotedSql(dummy);
      free(dummy);
      sql += wxString::FromUTF8(xdummy);
      free(xdummy);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              if (atoi(results[(i * columns) + 0]) != 0)
                {
                  outTable += wxT("_1");
                  ok = true;
                  break;
                }
            }
        }
      sqlite3_free_table(results);
    }
}

bool MyFrame::DoElementaryGeometries(wxString & inTable, wxString & geometry,
                                     wxString & outTable, wxString & pKey,
                                     wxString & multiID, wxString & type,
                                     int srid, wxString & coordDims, bool spIdx)
{
// testing if this table belongs to some R*Tree Spatial Index
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  wxString sql;
  wxString sql2;
  wxString sql3;
  wxString sql4;
  wxString sql_geom;
  wxString sql_spidx;
  wxString sqlx;
  char *dummy;
  char *xdummy;
  char *errMsg = NULL;
  bool comma = false;
  sqlite3_stmt *stmt_in = NULL;
  sqlite3_stmt *stmt_out = NULL;
  int n_columns;
  sqlite3_int64 id = 0;
  int geom_idx = -1;

// starts a transaction 
  ret = sqlite3_exec(SqliteHandle, "BEGIN", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("BEGIN error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto abort;
    }

  sql = wxT("SELECT ");
  sql2 = wxT("INSERT INTO ");
  sql3 = wxT(") VALUES (NULL, ?");
  sql4 = wxT("CREATE TABLE ");
  dummy = (char *) malloc((outTable.Len() * 4) + 1);
  strcpy(dummy, outTable.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sql2 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  sql2 += wxT(" (");
  sql4 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  dummy = (char *) malloc((pKey.Len() * 4) + 1);
  strcpy(dummy, pKey.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sql2 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  sql2 += wxT(", ");
  dummy = (char *) malloc((multiID.Len() * 4) + 1);
  strcpy(dummy, multiID.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sql2 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  sql4 += wxT(" (\n\t");
  dummy = (char *) malloc((pKey.Len() * 4) + 1);
  strcpy(dummy, pKey.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sql4 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  sql4 += wxT(" INTEGER PRIMARY KEY AUTOINCREMENT");
  sql4 += wxT(",\n\t");
  dummy = (char *) malloc((multiID.Len() * 4) + 1);
  strcpy(dummy, multiID.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sql4 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  sql4 += wxT(" INTEGER NOT NULL");

  sqlx = wxT("PRAGMA table_info(");
  dummy = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(dummy, inTable.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sqlx += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  sqlx += wxT(")");
  dummy = (char *) malloc((sqlx.Len() * 4) + 1);
  strcpy(dummy, sqlx.ToUTF8());
  ret = sqlite3_get_table(SqliteHandle, dummy, &results,
                          &rows, &columns, &errMsg);
  free(dummy);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto abort;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          dummy = (char *) malloc(strlen(results[(i * columns) + 1]) + 1);
          strcpy(dummy, results[(i * columns) + 1]);
          xdummy = gaiaDoubleQuotedSql(dummy);
          free(dummy);
          if (comma)
            sql += wxT(", ");
          else
            comma = true;
          sql += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
          sql2 += wxT(", ");
          sql2 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
          sql3 += wxT(", ?");

          wxString x = wxString::FromUTF8(results[(i * columns) + 1]);
          if (x.CmpNoCase(geometry) == 0)
            geom_idx = i - 1;
          else
            {
              sql4 += wxT(",\n\t");
              sql4 += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
              sql4 += wxT(" ") + wxString::FromUTF8(results[(i * columns) + 2]);
              if (atoi(results[(i * columns) + 3]) != 0)
                sql4 += wxT(" NOT NULL");
            }
          free(xdummy);
        }
    }
  sqlite3_free_table(results);
  if (geom_idx < 0)
    goto abort;

  sql += wxT(" FROM ");
  dummy = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(dummy, inTable.ToUTF8());
  xdummy = gaiaDoubleQuotedSql(dummy);
  free(dummy);
  sql += wxT("\"") + wxString::FromUTF8(xdummy) + wxT("\"");
  free(xdummy);
  sql2 += sql3;
  sql2 += wxT(")");
  sql4 += wxT(")");

  sql_geom = wxT("SELECT AddGeometryColumn('");
  dummy = (char *) malloc((outTable.Len() * 4) + 1);
  strcpy(dummy, outTable.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql_geom += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql_geom += wxT("', '");
  dummy = (char *) malloc((geometry.Len() * 4) + 1);
  strcpy(dummy, geometry.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql_geom += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql_geom += wxT("', ");
  dummy = (char *) malloc(1024);
  sprintf(dummy, "%d, '", srid);
  sql_geom += wxString::FromUTF8(dummy);
  free(dummy);
  dummy = (char *) malloc((type.Len() * 4) + 1);
  strcpy(dummy, type.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql_geom += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql_geom += wxT("', '");
  dummy = (char *) malloc((coordDims.Len() * 4) + 1);
  strcpy(dummy, coordDims.ToUTF8());
  xdummy = gaiaSingleQuotedSql(dummy);
  free(dummy);
  sql_geom += wxString::FromUTF8(xdummy);
  free(xdummy);
  sql_geom += wxT("')");
  if (spIdx == true)
    {
      sql_spidx = wxT("SELECT CreateSpatialIndex('");
      dummy = (char *) malloc((outTable.Len() * 4) + 1);
      strcpy(dummy, outTable.ToUTF8());
      xdummy = gaiaSingleQuotedSql(dummy);
      free(dummy);
      sql_spidx += wxString::FromUTF8(xdummy);
      free(xdummy);
      sql_spidx += wxT("', '");
      dummy = (char *) malloc((geometry.Len() * 4) + 1);
      strcpy(dummy, geometry.ToUTF8());
      xdummy = gaiaSingleQuotedSql(dummy);
      free(dummy);
      sql_spidx += wxString::FromUTF8(xdummy);
      free(xdummy);
      sql_spidx += wxT("')");
    }
// creating the output table
  dummy = (char *) malloc((sql4.Len() * 4) + 1);
  strcpy(dummy, sql4.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, dummy, NULL, NULL, &errMsg);
  free(dummy);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("CREATE TABLE error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto abort;
    }
// creating the output Geometry
  dummy = (char *) malloc((sql_geom.Len() * 4) + 1);
  strcpy(dummy, sql_geom.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, dummy, NULL, NULL, &errMsg);
  free(dummy);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("AddGeometryColumn error: ") +
                   wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto abort;
    }
  if (spIdx == true)
    {
      // creating the R*Tree Spatial Index
      dummy = (char *) malloc((sql_spidx.Len() * 4) + 1);
      strcpy(dummy, sql_spidx.ToUTF8());
      ret = sqlite3_exec(SqliteHandle, dummy, NULL, NULL, &errMsg);
      free(dummy);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("CreateSpatialIndex error: ") +
                       wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          goto abort;
        }
    }
// preparing the INPUT statement
  dummy = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(dummy, sql.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt_in, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    {
      wxString err = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
      wxMessageBox(wxT("[IN]SQL error: ") + err, wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      goto abort;
    }
// preparing the OUTPUT statement
  dummy = (char *) malloc((sql2.Len() * 4) + 1);
  strcpy(dummy, sql2.ToUTF8());
  ret = sqlite3_prepare_v2(SqliteHandle, dummy, strlen(dummy), &stmt_out, NULL);
  free(dummy);
  if (ret != SQLITE_OK)
    {
      wxString err = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
      wxMessageBox(wxT("[OUT]SQL error: ") + err, wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      goto abort;
    }
// data transfer
  n_columns = sqlite3_column_count(stmt_in);
  while (1)
    {
      ret = sqlite3_step(stmt_in);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          gaiaGeomCollPtr g =
            gaiaFromSpatiaLiteBlobWkb((const unsigned char *)
                                      sqlite3_column_blob(stmt_in, geom_idx),
                                      sqlite3_column_bytes(stmt_in, geom_idx));
          if (!g)
            {
              // NULL input geometry    
              sqlite3_reset(stmt_out);
              sqlite3_clear_bindings(stmt_out);
              sqlite3_bind_int64(stmt_out, 1, id);
              sqlite3_bind_null(stmt_out, geom_idx + 2);

              for (i = 0; i < n_columns; i++)
                {
                  int type = sqlite3_column_type(stmt_in, i);
                  if (i == geom_idx)
                    continue;
                  switch (type)
                    {
                      case SQLITE_INTEGER:
                        sqlite3_bind_int64(stmt_out, i + 2,
                                           sqlite3_column_int64(stmt_in, i));
                        break;
                      case SQLITE_FLOAT:
                        sqlite3_bind_double(stmt_out, i + 2,
                                            sqlite3_column_double(stmt_in, i));
                        break;
                      case SQLITE_TEXT:
                        sqlite3_bind_text(stmt_out, i + 2,
                                          (const char *)
                                          sqlite3_column_text(stmt_in, i),
                                          sqlite3_column_bytes(stmt_in, i),
                                          SQLITE_STATIC);
                        break;
                      case SQLITE_BLOB:
                        sqlite3_bind_blob(stmt_out, i + 2,
                                          sqlite3_column_blob(stmt_in, i),
                                          sqlite3_column_bytes(stmt_in, i),
                                          SQLITE_STATIC);
                        break;
                      case SQLITE_NULL:
                      default:
                        sqlite3_bind_null(stmt_out, i + 2);
                        break;
                    };
                }

              ret = sqlite3_step(stmt_out);
              if (ret == SQLITE_DONE || ret == SQLITE_ROW)
                ;
              else
                {
                  wxString err =
                    wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
                  wxMessageBox(wxT("[OUT]step error: ") + err,
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  goto abort;
                }
          } else
            {
              // separating Elementary Geoms
              gaiaPointPtr pt;
              gaiaLinestringPtr ln;
              gaiaPolygonPtr pg;
              gaiaGeomCollPtr outGeom;
              pt = g->FirstPoint;
              while (pt)
                {
                  // separating Points
                  outGeom = GeomFromPoint(pt, g->Srid);
                  sqlite3_reset(stmt_out);
                  sqlite3_clear_bindings(stmt_out);
                  sqlite3_bind_int64(stmt_out, 1, id);
                  if (!outGeom)
                    sqlite3_bind_null(stmt_out, geom_idx + 2);
                  else
                    {
                      unsigned char *blob;
                      int size;
                      gaiaToSpatiaLiteBlobWkb(outGeom, &blob, &size);
                      sqlite3_bind_blob(stmt_out, geom_idx + 2, blob, size,
                                        free);
                      gaiaFreeGeomColl(outGeom);
                    }

                  for (i = 0; i < n_columns; i++)
                    {
                      int type = sqlite3_column_type(stmt_in, i);
                      if (i == geom_idx)
                        continue;
                      switch (type)
                        {
                          case SQLITE_INTEGER:
                            sqlite3_bind_int64(stmt_out, i + 2,
                                               sqlite3_column_int64(stmt_in,
                                                                    i));
                            break;
                          case SQLITE_FLOAT:
                            sqlite3_bind_double(stmt_out, i + 2,
                                                sqlite3_column_double(stmt_in,
                                                                      i));
                            break;
                          case SQLITE_TEXT:
                            sqlite3_bind_text(stmt_out, i + 2,
                                              (const char *)
                                              sqlite3_column_text(stmt_in, i),
                                              sqlite3_column_bytes(stmt_in, i),
                                              SQLITE_STATIC);
                            break;
                          case SQLITE_BLOB:
                            sqlite3_bind_blob(stmt_out, i + 2,
                                              sqlite3_column_blob(stmt_in, i),
                                              sqlite3_column_bytes(stmt_in, i),
                                              SQLITE_STATIC);
                            break;
                          case SQLITE_NULL:
                          default:
                            sqlite3_bind_null(stmt_out, i + 2);
                            break;
                        };
                    }

                  ret = sqlite3_step(stmt_out);
                  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
                    ;
                  else
                    {
                      wxString err =
                        wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
                      wxMessageBox(wxT("[OUT]step error: ") + err,
                                   wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                   this);
                      goto abort;
                    }
                  pt = pt->Next;
                }
              ln = g->FirstLinestring;
              while (ln)
                {
                  // separating Linestrings
                  outGeom = GeomFromLinestring(ln, g->Srid);
                  sqlite3_reset(stmt_out);
                  sqlite3_clear_bindings(stmt_out);
                  sqlite3_bind_int64(stmt_out, 1, id);
                  if (!outGeom)
                    sqlite3_bind_null(stmt_out, geom_idx + 2);
                  else
                    {
                      unsigned char *blob;
                      int size;
                      gaiaToSpatiaLiteBlobWkb(outGeom, &blob, &size);
                      sqlite3_bind_blob(stmt_out, geom_idx + 2, blob, size,
                                        free);
                      gaiaFreeGeomColl(outGeom);
                    }

                  for (i = 0; i < n_columns; i++)
                    {
                      int type = sqlite3_column_type(stmt_in, i);
                      if (i == geom_idx)
                        continue;
                      switch (type)
                        {
                          case SQLITE_INTEGER:
                            sqlite3_bind_int64(stmt_out, i + 2,
                                               sqlite3_column_int64(stmt_in,
                                                                    i));
                            break;
                          case SQLITE_FLOAT:
                            sqlite3_bind_double(stmt_out, i + 2,
                                                sqlite3_column_double(stmt_in,
                                                                      i));
                            break;
                          case SQLITE_TEXT:
                            sqlite3_bind_text(stmt_out, i + 2,
                                              (const char *)
                                              sqlite3_column_text(stmt_in, i),
                                              sqlite3_column_bytes(stmt_in, i),
                                              SQLITE_STATIC);
                            break;
                          case SQLITE_BLOB:
                            sqlite3_bind_blob(stmt_out, i + 2,
                                              sqlite3_column_blob(stmt_in, i),
                                              sqlite3_column_bytes(stmt_in, i),
                                              SQLITE_STATIC);
                            break;
                          case SQLITE_NULL:
                          default:
                            sqlite3_bind_null(stmt_out, i + 2);
                            break;
                        };
                    }

                  ret = sqlite3_step(stmt_out);
                  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
                    ;
                  else
                    {
                      wxString err =
                        wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
                      wxMessageBox(wxT("[OUT]step error: ") + err,
                                   wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                   this);
                      goto abort;
                    }
                  ln = ln->Next;
                }
              pg = g->FirstPolygon;
              while (pg)
                {
                  // separating Polygons
                  outGeom = GeomFromPolygon(pg, g->Srid);
                  sqlite3_reset(stmt_out);
                  sqlite3_clear_bindings(stmt_out);
                  sqlite3_bind_int64(stmt_out, 1, id);
                  if (!outGeom)
                    sqlite3_bind_null(stmt_out, geom_idx + 2);
                  else
                    {
                      unsigned char *blob;
                      int size;
                      gaiaToSpatiaLiteBlobWkb(outGeom, &blob, &size);
                      sqlite3_bind_blob(stmt_out, geom_idx + 2, blob, size,
                                        free);
                      gaiaFreeGeomColl(outGeom);
                    }

                  for (i = 0; i < n_columns; i++)
                    {
                      int type = sqlite3_column_type(stmt_in, i);
                      if (i == geom_idx)
                        continue;
                      switch (type)
                        {
                          case SQLITE_INTEGER:
                            sqlite3_bind_int64(stmt_out, i + 2,
                                               sqlite3_column_int64(stmt_in,
                                                                    i));
                            break;
                          case SQLITE_FLOAT:
                            sqlite3_bind_double(stmt_out, i + 2,
                                                sqlite3_column_double(stmt_in,
                                                                      i));
                            break;
                          case SQLITE_TEXT:
                            sqlite3_bind_text(stmt_out, i + 2,
                                              (const char *)
                                              sqlite3_column_text(stmt_in, i),
                                              sqlite3_column_bytes(stmt_in, i),
                                              SQLITE_STATIC);
                            break;
                          case SQLITE_BLOB:
                            sqlite3_bind_blob(stmt_out, i + 2,
                                              sqlite3_column_blob(stmt_in, i),
                                              sqlite3_column_bytes(stmt_in, i),
                                              SQLITE_STATIC);
                            break;
                          case SQLITE_NULL:
                          default:
                            sqlite3_bind_null(stmt_out, i + 2);
                            break;
                        };
                    }

                  ret = sqlite3_step(stmt_out);
                  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
                    ;
                  else
                    {
                      wxString err =
                        wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
                      wxMessageBox(wxT("[OUT]step error: ") + err,
                                   wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                   this);
                      goto abort;
                    }
                  pg = pg->Next;
                }
              gaiaFreeGeomColl(g);
            }
          id++;
      } else
        {
          wxString err = wxString::FromUTF8(sqlite3_errmsg(SqliteHandle));
          wxMessageBox(wxT("[IN]step error: ") + err, wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          goto abort;
        }
    }
  sqlite3_finalize(stmt_in);
  sqlite3_finalize(stmt_out);

// commits the transaction 
  ret = sqlite3_exec(SqliteHandle, "COMMIT", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("COMMIT error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto abort;
    }
  return true;

abort:
  if (stmt_in)
    sqlite3_finalize(stmt_in);
  if (stmt_out)
    sqlite3_finalize(stmt_out);
  return false;
}

gaiaGeomCollPtr MyFrame::GeomFromPoint(gaiaPointPtr pt, int srid)
{
// creating a Geometry containing a single Point
  gaiaGeomCollPtr g = NULL;
  switch (pt->DimensionModel)
    {
      case GAIA_XY_Z_M:
        g = gaiaAllocGeomCollXYZM();
        break;
      case GAIA_XY_Z:
        g = gaiaAllocGeomCollXYZ();
        break;
      case GAIA_XY_M:
        g = gaiaAllocGeomCollXYM();
        break;
      default:
        g = gaiaAllocGeomColl();
        break;
    };
  if (!g)
    return NULL;
  g->Srid = srid;
  g->DeclaredType = GAIA_POINT;
  switch (pt->DimensionModel)
    {
      case GAIA_XY_Z_M:
        gaiaAddPointToGeomCollXYZM(g, pt->X, pt->Y, pt->Z, pt->M);
        break;
      case GAIA_XY_Z:
        gaiaAddPointToGeomCollXYZ(g, pt->X, pt->Y, pt->Z);
        break;
      case GAIA_XY_M:
        gaiaAddPointToGeomCollXYM(g, pt->X, pt->Y, pt->M);
        break;
      default:
        gaiaAddPointToGeomColl(g, pt->X, pt->Y);
        break;
    };
  return g;
}

gaiaGeomCollPtr MyFrame::GeomFromLinestring(gaiaLinestringPtr ln, int srid)
{
// creating a Geometry containing a single Linestring
  gaiaGeomCollPtr g = NULL;
  gaiaLinestringPtr ln2;
  int iv;
  double x;
  double y;
  double z;
  double m;
  switch (ln->DimensionModel)
    {
      case GAIA_XY_Z_M:
        g = gaiaAllocGeomCollXYZM();
        break;
      case GAIA_XY_Z:
        g = gaiaAllocGeomCollXYZ();
        break;
      case GAIA_XY_M:
        g = gaiaAllocGeomCollXYM();
        break;
      default:
        g = gaiaAllocGeomColl();
        break;
    };
  if (!g)
    return NULL;
  g->Srid = srid;
  g->DeclaredType = GAIA_LINESTRING;
  ln2 = gaiaAddLinestringToGeomColl(g, ln->Points);
  switch (ln->DimensionModel)
    {
      case GAIA_XY_Z_M:
        for (iv = 0; iv < ln->Points; iv++)
          {
            gaiaGetPointXYZM(ln->Coords, iv, &x, &y, &z, &m);
            gaiaSetPointXYZM(ln2->Coords, iv, x, y, z, m);
          }
        break;
      case GAIA_XY_Z:
        for (iv = 0; iv < ln->Points; iv++)
          {
            gaiaGetPointXYZ(ln->Coords, iv, &x, &y, &z);
            gaiaSetPointXYZ(ln2->Coords, iv, x, y, z);
          }
        break;
      case GAIA_XY_M:
        for (iv = 0; iv < ln->Points; iv++)
          {
            gaiaGetPointXYM(ln->Coords, iv, &x, &y, &m);
            gaiaSetPointXYM(ln2->Coords, iv, x, y, m);
          }
        break;
      default:
        for (iv = 0; iv < ln->Points; iv++)
          {
            gaiaGetPoint(ln->Coords, iv, &x, &y);
            gaiaSetPoint(ln2->Coords, iv, x, y);
          }
        break;
    };
  return g;
}

gaiaGeomCollPtr MyFrame::GeomFromPolygon(gaiaPolygonPtr pg, int srid)
{
// creating a Geometry containing a single Polygon
  gaiaGeomCollPtr g = NULL;
  gaiaPolygonPtr pg2;
  gaiaRingPtr rng;
  gaiaRingPtr rng2;
  int ib;
  int iv;
  double x;
  double y;
  double z;
  double m;
  switch (pg->DimensionModel)
    {
      case GAIA_XY_Z_M:
        g = gaiaAllocGeomCollXYZM();
        break;
      case GAIA_XY_Z:
        g = gaiaAllocGeomCollXYZ();
        break;
      case GAIA_XY_M:
        g = gaiaAllocGeomCollXYM();
        break;
      default:
        g = gaiaAllocGeomColl();
        break;
    };
  if (!g)
    return NULL;
  g->Srid = srid;
  g->DeclaredType = GAIA_POLYGON;
  rng = pg->Exterior;
  pg2 = gaiaAddPolygonToGeomColl(g, rng->Points, pg->NumInteriors);
  rng2 = pg2->Exterior;
  switch (pg->DimensionModel)
    {
      case GAIA_XY_Z_M:
        for (iv = 0; iv < rng->Points; iv++)
          {
            gaiaGetPointXYZM(rng->Coords, iv, &x, &y, &z, &m);
            gaiaSetPointXYZM(rng2->Coords, iv, x, y, z, m);
          }
        for (ib = 0; ib < pg->NumInteriors; ib++)
          {
            rng = pg->Interiors + ib;
            rng2 = gaiaAddInteriorRing(pg2, ib, rng->Points);
            for (iv = 0; iv < rng->Points; iv++)
              {
                gaiaGetPointXYZM(rng->Coords, iv, &x, &y, &z, &m);
                gaiaSetPointXYZM(rng2->Coords, iv, x, y, z, m);
              }
          }
        break;
      case GAIA_XY_Z:
        for (iv = 0; iv < rng->Points; iv++)
          {
            gaiaGetPointXYZ(rng->Coords, iv, &x, &y, &z);
            gaiaSetPointXYZ(rng2->Coords, iv, x, y, z);
          }
        for (ib = 0; ib < pg->NumInteriors; ib++)
          {
            rng = pg->Interiors + ib;
            rng2 = gaiaAddInteriorRing(pg2, ib, rng->Points);
            for (iv = 0; iv < rng->Points; iv++)
              {
                gaiaGetPointXYZ(rng->Coords, iv, &x, &y, &z);
                gaiaSetPointXYZ(rng2->Coords, iv, x, y, z);
              }
          }
        break;
      case GAIA_XY_M:
        for (iv = 0; iv < rng->Points; iv++)
          {
            gaiaGetPointXYM(rng->Coords, iv, &x, &y, &m);
            gaiaSetPointXYM(rng2->Coords, iv, x, y, m);
          }
        for (ib = 0; ib < pg->NumInteriors; ib++)
          {
            rng = pg->Interiors + ib;
            rng2 = gaiaAddInteriorRing(pg2, ib, rng->Points);
            for (iv = 0; iv < rng->Points; iv++)
              {
                gaiaGetPointXYM(rng->Coords, iv, &x, &y, &m);
                gaiaSetPointXYM(rng2->Coords, iv, x, y, m);
              }
          }
        break;
      default:
        for (iv = 0; iv < rng->Points; iv++)
          {
            gaiaGetPoint(rng->Coords, iv, &x, &y);
            gaiaSetPoint(rng2->Coords, iv, x, y);
          }
        for (ib = 0; ib < pg->NumInteriors; ib++)
          {
            rng = pg->Interiors + ib;
            rng2 = gaiaAddInteriorRing(pg2, ib, rng->Points);
            for (iv = 0; iv < rng->Points; iv++)
              {
                gaiaGetPoint(rng->Coords, iv, &x, &y);
                gaiaSetPoint(rng2->Coords, iv, x, y);
              }
          }
        break;
    };
  return g;
}

bool MyFrame::GetTilePreview(wxString & currentTileDataDbPrefix,
                             wxString & currentTileData, int currentTileId,
                             unsigned char **blob, int *blobSize)
{
// creating a TilePreview
  bool success = false;
  int ret;
  sqlite3_stmt *stmt;
  if (currentTileData.Len() == 0)
    return false;
  char db_prefix[1024];
  char coverage[1024];
  if (currentTileDataDbPrefix.Len() == 0)
    strcpy(db_prefix, "MAIN");
  else
    strcpy(db_prefix, currentTileDataDbPrefix.ToUTF8());
  wxString cvg = currentTileData;
  currentTileData.EndsWith(wxT("_tile_data"), &cvg);
  strcpy(coverage, cvg.ToUTF8());
  char *sql =
    sqlite3_mprintf("SELECT RL2_GetTileImage(%Q, %Q, %d)", db_prefix, coverage,
                    currentTileId);
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  while (1)
    {
      //
      // fetching the result set rows 
      //
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          //
          // fetching a row
          //
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const unsigned char *p_blob =
                (const unsigned char *) sqlite3_column_blob(stmt, 0);
              *blobSize = sqlite3_column_bytes(stmt, 0);
              *blob = new unsigned char[*blobSize];
              memcpy(*blob, p_blob, *blobSize);
              success = true;
            }
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (success == false)
    return false;
  return true;
}

void MyFrame::DoCreatePostgreSqlNodes()
{
// initializing the PostgreSQL nodes
  int c = 0;
  MyPostgresConn *pC = PostgresList.GetFirst();
  while (pC != NULL)
    {
      // adding a Connection node
      wxTreeItemId connId;
      TableTree->AddPostgresConnection(c++, pC, connId);
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          // adding a Schema node
          wxTreeItemId schemaId;
          TableTree->AddPostgresSchema(connId, pC, pS->GetName(), schemaId);
          pS->SetTreeNode(schemaId);
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
}

void MyFrame::InitTableTree()
{
// loads the table TREE list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *createSql;
  char *type;
  wxString tblName;
  wxString sql;
  char *xsql;
  TableViewList *list;
  TableViewItem *ptv;
  bool virtualTable = false;
  TableTree->Show(false);
  if (MemoryDatabase == true)
    {
      wxString memory = wxT("MEMORY-DB");
      TableTree->SetPath(memory);
  } else
    TableTree->SetPath(SqlitePath);
  TableTree->FlushAll();
  DoCreatePostgreSqlNodes();
  if (ExistsTopologies())
    {
      // fetching topologies
      sql = wxT("SELECT topology_name, srid, has_z ");
      sql += wxT(" FROM main.topologies");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Topology
              const char *name = results[(i * columns) + 0];
              int srid = atoi(results[(i * columns) + 1]);
              bool hasZ = false;
              if (atoi(results[(i * columns) + 2]))
                hasZ = true;
              TopoGeoSet topology("MAIN", name, srid, hasZ);
              TableTree->AddTopology(&topology);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsNetworks())
    {
      // fetching networks
      sql = wxT("SELECT network_name, spatial, srid, has_z ");
      sql += wxT(" FROM main.networks");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Network
              const char *name = results[(i * columns) + 0];
              bool spatial = false;
              if (atoi(results[(i * columns) + 1]))
                spatial = true;
              int srid = atoi(results[(i * columns) + 2]);
              bool hasZ = false;
              if (atoi(results[(i * columns) + 3]))
                hasZ = true;
              TopoNetSet network(name, spatial, srid, hasZ);
              TableTree->AddNetwork(&network);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsRasterCoverages())
    {
      // fetching Raster Coverages
      sql = wxT("SELECT coverage_name, srid FROM main.raster_coverages");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Raster Coverage
              const char *name = results[(i * columns) + 0];
              int srid = atoi(results[(i * columns) + 1]);
              RasterCoverageSet coverage(name, srid);
              TableTree->AddRasterCoverage(&coverage);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsVectorCoverages() && ExistsTopologies() && ExistsNetworks())
    {
      // fetching Vector Coverages
      sql = wxT("SELECT 'table', c.coverage_name, g.srid, g.geometry_type ");
      sql += wxT("FROM main.vector_coverages AS c ");
      sql += wxT("JOIN main.geometry_columns AS g ON (");
      sql += wxT("c.topology_name IS NULL AND c.network_name IS NULL AND ");
      sql += wxT("Lower(c.f_table_name) = Lower(g.f_table_name) AND ");
      sql += wxT("Lower(c.f_geometry_column) = Lower(g.f_geometry_column)) ");
      sql += wxT("UNION ");
      sql += wxT("SELECT 'view', c.coverage_name, g.srid, g.geometry_type ");
      sql += wxT("FROM main.vector_coverages AS c ");
      sql += wxT("JOIN main.views_geometry_columns AS v ON ");
      sql += wxT("(Lower(c.view_name) = Lower(v.view_name) AND ");
      sql += wxT("Lower(c.view_geometry) = Lower(v.view_geometry)) ");
      sql += wxT("JOIN main.geometry_columns AS g ON ");
      sql += wxT("(Lower(v.f_table_name) = Lower(g.f_table_name) AND ");
      sql += wxT("Lower(v.f_geometry_column) = Lower(g.f_geometry_column)) ");
      sql += wxT("UNION ");
      sql += wxT("SELECT 'shp', c.coverage_name, v.srid, v.geometry_type ");
      sql += wxT("FROM main.vector_coverages AS c ");
      sql += wxT("JOIN main.virts_geometry_columns AS v ON ");
      sql += wxT("(Lower(c.virt_name) = Lower(v.virt_name) AND ");
      sql += wxT("Lower(c.virt_geometry) = Lower(v.virt_geometry)) ");
      sql += wxT("UNION ");
      sql += wxT("SELECT 'topogeo', c.coverage_name, t.srid, 3 ");
      sql += wxT("FROM main.vector_coverages AS c ");
      sql +=
        wxT("JOIN main.topologies AS t ON (c.topology_name IS NOT NULL AND ");
      sql += wxT("Lower(c.topology_name) = Lower(t.topology_name)) ");
      sql += wxT("UNION ");
      sql += wxT("SELECT 'toponet', c.coverage_name, n.srid, 2 ");
      sql += wxT("FROM main.vector_coverages AS c ");
      sql += wxT("JOIN main.networks AS n ON (c.network_name IS NOT NULL AND ");
      sql += wxT("Lower(c.network_name) = Lower(n.network_name)) ");
      sql += wxT("ORDER BY 2");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Vector Coverage
              const char *prefix = results[(i * columns) + 0];
              const char *name = results[(i * columns) + 1];
              int srid = atoi(results[(i * columns) + 2]);
              int type = atoi(results[(i * columns) + 3]);
              VectorCoverageSet coverage(prefix, name, srid, type);
              TableTree->AddVectorCoverage(&coverage);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsWmsGetMap())
    {
      // fetching WMS Layers
      sql = wxT("SELECT layer_name, url FROM main.wms_getmap");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some WMS Layer
              const char *name = results[(i * columns) + 0];
              wxString xname = wxString::FromUTF8(name);
              const char *url = results[(i * columns) + 1];
              wxString xurl = wxString::FromUTF8(url);
              TableTree->AddWmsLayer(xname, xurl);
            }
        }
      sqlite3_free_table(results);
    }
// fetching persistent tables and views
  sql =
    wxT
    ("SELECT 'main', name, sql, type FROM main.sqlite_master WHERE (type = 'table' OR type = 'view') "
     "UNION SELECT 'temp', name, sql, type FROM sqlite_temp_master WHERE type = 'table' "
     "AND name IN ('postgis_geometry_columns', 'tmp_vector_coverages') ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  list = new TableViewList();
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *db = results[(i * columns) + 0];
          name = results[(i * columns) + 1];
          createSql = results[(i * columns) + 2];
          type = results[(i * columns) + 3];
          if (strstr(createSql, " VIRTUAL ") || strstr(createSql, " virtual "))
            virtualTable = true;
          else
            virtualTable = false;
          tblName = wxString::FromUTF8(name);
          wxString dbName = wxString::FromUTF8(db);
          if (dbName.Cmp(wxT("temp")) == 0
              && (tblName.Cmp(wxT("postgis_geometry_columns")) == 0 ||
                  tblName.Cmp(wxT("tmp_vector_coverages")) == 0))
            {
              list->Add(dbName, tblName);
              continue;
            }
          if (strcmp(type, "view") == 0)
            list->Add(tblName, true, false);
          else
            list->Add(tblName, false, virtualTable);
        }
    }
  sqlite3_free_table(results);

  if (TableTree->IsGeoPackageMode())
    FindGeoPackageGeometries(list);
  else if (TableTree->IsFdoOgrMode())
    FindFdoOgrGeometries(list);
  else
    FindGeometries(list);
  ptv = list->GetFirst();
  while (ptv != NULL)
    {
      // inserting items into the Tree Control View
      if (ptv->IsTmpMetadata() == true)
        TableTree->AddTmpMetadata(ptv->GetName());
      else if (ptv->IsPostgresTable(&PostgresList) == true)
        TableTree->AddPostgresTable(&PostgresList, ptv->GetName());
      else if (ptv->IsView() == true)
        TableTree->AddView(ptv->GetName(), ptv->IsGeometry());
      else if (ptv->IsGeoPackageGeometry() == true)
        TableTree->AddGeoPackageTable(ptv->GetName());
      else if (ptv->IsGeoPackageVirtualGeometry() == true)
        TableTree->AddGeoPackageVirtualTable(ptv->GetName());
      else if (ptv->IsFdoOgrGeometry() == true)
        TableTree->AddFdoOgrTable(ptv->GetName());
      else if (ptv->IsFdoOgrVirtualGeometry() == true)
        TableTree->AddFdoOgrVirtualTable(ptv->GetName());
      else
        TableTree->AddTable(ptv->GetName(), ptv->IsVirtual(),
                            ptv->IsGeometry());
      ptv = ptv->GetNext();
    }
  delete list;

// fetching temporary tables and views
  sql =
    wxT
    ("SELECT name, sql, type FROM sqlite_temp_master WHERE (type = 'table' OR type = 'view') "
     "AND name NOT IN ('postgis_geometry_columns', 'tmp_vector_coverages') ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                          &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  list = new TableViewList();
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          createSql = results[(i * columns) + 1];
          type = results[(i * columns) + 2];
          if (strstr(createSql, " VIRTUAL ") || strstr(createSql, " virtual "))
            virtualTable = true;
          else
            virtualTable = false;
          tblName = wxString::FromUTF8(name);
          if (strcmp(type, "view") == 0)
            list->Add(tblName, true, false);
          else
            list->Add(tblName, false, virtualTable);
        }
    }
  sqlite3_free_table(results);

  if (TableTree->IsGeoPackageMode())
    FindGeoPackageGeometries(list);
  else if (TableTree->IsFdoOgrMode())
    FindFdoOgrGeometries(list);
  else
    FindGeometries(list);
  ptv = list->GetFirst();
  while (ptv != NULL)
    {
      // inserting items into the Tree Control View
      wxString dbAlias = wxT("temp");
      if (ptv->IsView() == true)
        TableTree->AddView(dbAlias, ptv->GetName(), ptv->IsGeometry());
      else
        TableTree->AddTable(dbAlias, ptv->GetName(), ptv->IsVirtual(),
                            ptv->IsGeometry());
      ptv = ptv->GetNext();
    }
  delete list;

  ListAttached();
  TableTree->ExpandRoot();
  TableTree->Show(true);
}

void MyFrame::InitTableTree(wxString & dbAlias, wxString & path)
{
// loads the table TREE list [ATTACHED DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *createSql;
  char *type;
  wxString tblName;
  wxString sql;
  char *xsql;
  TableViewList *list;
  TableViewItem *ptv;
  bool virtualTable = false;
  TableTree->DeleteAltTopologies();
  TableTree->DeleteAltNetworks();
  TableTree->DeleteAltRasterCoverages();
  TableTree->DeleteAltVectorCoverages();
  wxString dbInfos = dbAlias + wxT(": ") + path;
  wxTreeItemId db = TableTree->AppendItem(TableTree->GetRootItem(), dbInfos);
  wxString empty = wxEmptyString;
  TableTree->SetItemData(db,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ATTACHED, dbAlias,
                                                      empty, empty, path)));
  TableTree->SetItemImage(db, 21);

  MyAttachedDB *attached_db = AttachedList.Find(dbAlias, path);
  if (attached_db != NULL)
    {
      if (attached_db->IsInitialized() == false)
        {
          AutoFDOStart(dbAlias, attached_db);
          AutoGPKGStart(dbAlias, attached_db);
          attached_db->SetInitialized();
        }
    }

  wxTreeItemId rootUserData = TableTree->AppendItem(db, wxT("User Data"));
  wxTreeItemId rootIsoMetadata =
    TableTree->AppendItem(db, wxT("ISO / INSPIRE Metadata"));
  wxTreeItemId rootStyling = TableTree->AppendItem(db, wxT("Styling (SLD/SE)"));
  wxTreeItemId rootTopologies =
    TableTree->AppendItem(db, wxT("Topology-Geometry"));
  wxTreeItemId rootNetworks =
    TableTree->AppendItem(db, wxT("Topology-Network"));
  wxTreeItemId rootRasterCoverages =
    TableTree->AppendItem(db, wxT("Raster Coverages"));
  wxTreeItemId rootVectorCoverages =
    TableTree->AppendItem(db, wxT("Vector Coverages"));
  wxTreeItemId rootWMS =
    TableTree->AppendItem(db, wxT("Registered WMS layers"));
  wxTreeItemId rootMetadata = TableTree->AppendItem(db, wxT("Metadata"));
  wxTreeItemId rootInternal = TableTree->AppendItem(db, wxT("Internal Data"));
  wxTreeItemId rootSpatialIndex =
    TableTree->AppendItem(db, wxT("Spatial Index"));
  TableTree->SetItemImage(rootUserData, 17);
  TableTree->SetItemImage(rootTopologies, 20);
  TableTree->SetItemImage(rootNetworks, 25);
  TableTree->SetItemImage(rootRasterCoverages, 22);
  TableTree->SetItemImage(rootVectorCoverages, 24);
  TableTree->SetItemImage(rootWMS, 26);
  TableTree->SetItemImage(rootStyling, 17);
  TableTree->SetItemImage(rootIsoMetadata, 17);
  TableTree->SetItemImage(rootMetadata, 17);
  TableTree->SetItemImage(rootInternal, 17);
  TableTree->SetItemImage(rootSpatialIndex, 17);
  TableTree->SetItemData(rootUserData,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_USERDATA,
                                                      dbAlias)));
  TableTree->SetItemData(rootIsoMetadata,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_ISOMETADATA,
                                                      dbAlias)));
  TableTree->SetItemData(rootTopologies,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_TOPOLOGIES,
                                                      dbAlias)));
  TableTree->SetItemData(rootNetworks,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_NETWORKS,
                                                      dbAlias)));
  TableTree->SetItemData(rootRasterCoverages,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_RASTER,
                                                      dbAlias)));
  TableTree->SetItemData(rootVectorCoverages,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_VECTOR,
                                                      dbAlias)));
  TableTree->SetItemData(rootWMS,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_WMS, dbAlias)));
  TableTree->SetItemData(rootStyling,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_STYLING,
                                                      dbAlias)));
  TableTree->SetItemData(rootMetadata,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_METADATA,
                                                      dbAlias)));
  TableTree->SetItemData(rootInternal,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_INTERNAL,
                                                      dbAlias)));
  TableTree->SetItemData(rootSpatialIndex,
                         (wxTreeItemData *) (new
                                             MyObject(MY_ROOT_RTREE, dbAlias)));
  RootNodes nodes =
    RootNodes(dbAlias, rootUserData, rootTopologies, rootNetworks,
              rootRasterCoverages, rootVectorCoverages, rootStyling, rootWMS,
              rootIsoMetadata, rootMetadata, rootInternal, rootSpatialIndex);
  if (ExistsTopologies(dbAlias))
    {
      // fetching topologies
      sql = wxT("SELECT topology_name, srid, has_z ");
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql += wxT(" FROM \"") + dbAlias2 + wxT("\".topologies");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Topology
              const char *name = results[(i * columns) + 0];
              int srid = atoi(results[(i * columns) + 1]);
              bool hasZ = false;
              if (atoi(results[(i * columns) + 2]))
                hasZ = true;
              TopoGeoSet topology(dbAlias.ToUTF8(), name, srid, hasZ);
              TableTree->AddAltTopology(rootTopologies, dbAlias, &topology);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsNetworks(dbAlias))
    {
      // fetching topologies
      sql = wxT("SELECT network_name, spatial, srid, has_z ");
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql += wxT(" FROM \"") + dbAlias2 + wxT("\".networks");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Network
              const char *name = results[(i * columns) + 0];
              bool spatial = false;
              if (atoi(results[(i * columns) + 1]))
                spatial = true;
              int srid = atoi(results[(i * columns) + 2]);
              bool hasZ = false;
              if (atoi(results[(i * columns) + 3]))
                hasZ = true;
              TopoNetSet network(name, spatial, srid, hasZ);
              TableTree->AddAltNetwork(rootNetworks, dbAlias, &network);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsRasterCoverages(dbAlias))
    {
      // fetching Raster Coverages
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql =
        wxT("SELECT coverage_name, srid FROM \"") + dbAlias2 +
        wxT("\".raster_coverages");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Raster Coverage
              const char *name = results[(i * columns) + 0];
              int srid = atoi(results[(i * columns) + 1]);
              RasterCoverageSet coverage(name, srid);
              TableTree->AddAltRasterCoverage(rootRasterCoverages, dbAlias,
                                              &coverage);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsVectorCoverages(dbAlias))
    {
      // fetching Vector Coverages
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql =
        wxT("SELECT 'table', c.coverage_name, g.srid, g.geometry_type FROM \"")
        + dbAlias2 + wxT("\".vector_coverages AS c ");
      sql += wxT("JOIN \"") + dbAlias2 + wxT("\".geometry_columns AS g ON (");
      sql += wxT("c.topology_name IS NULL AND c.network_name IS NULL AND ");
      sql += wxT("Lower(c.f_table_name) = Lower(g.f_table_name) AND ");
      sql += wxT("Lower(c.f_geometry_column) = Lower(g.f_geometry_column)) ");
      sql += wxT("UNION ");
      sql +=
        wxT("SELECT 'view', c.coverage_name, g.srid, g.geometry_type FROM \"") +
        dbAlias2 + wxT("\".vector_coverages AS c ");
      sql +=
        wxT("JOIN \"") + dbAlias2 + wxT("\".views_geometry_columns AS v ON (");
      sql += wxT("Lower(c.view_name) = Lower(v.view_name) AND ");
      sql += wxT("Lower(c.view_geometry) = Lower(v.view_geometry))");
      sql += wxT("JOIN \"") + dbAlias2 + wxT("\".geometry_columns AS g ON (");
      sql += wxT("Lower(v.f_table_name) = Lower(g.f_table_name) AND ");
      sql += wxT("Lower(v.f_geometry_column) = Lower(g.f_geometry_column)) ");
      sql += wxT("UNION ");
      sql +=
        wxT("SELECT 'shp', c.coverage_name, v.srid, v.geometry_type FROM \"") +
        dbAlias2 + wxT("\".vector_coverages AS c ");
      sql +=
        wxT("JOIN \"") + dbAlias2 + wxT("\".virts_geometry_columns AS v ON (");
      sql += wxT("Lower(c.virt_name) = Lower(v.virt_name) AND ");
      sql += wxT("Lower(c.virt_geometry) = Lower(v.virt_geometry))");
      sql += wxT("UNION ");
      sql += wxT("SELECT 'topogeo', c.coverage_name, t.srid, 3 ");
      sql += wxT("FROM \"") + dbAlias2 + wxT("\".vector_coverages AS c ");
      sql +=
        wxT("JOIN \"") + dbAlias2 +
        wxT("\".topologies AS t ON (c.topology_name IS NOT NULL AND ");
      sql += wxT("Lower(c.topology_name) = Lower(t.topology_name)) ");
      sql += wxT("UNION ");
      sql += wxT("SELECT 'toponet', c.coverage_name, n.srid, 2 ");
      sql += wxT("FROM \"") + dbAlias2 + wxT("\".vector_coverages AS c ");
      sql +=
        wxT("JOIN \"") + dbAlias2 +
        wxT("\".networks AS n ON (c.network_name IS NOT NULL AND ");
      sql += wxT("Lower(c.network_name) = Lower(n.network_name)) ");
      sql += wxT("ORDER BY 2");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some Vector Coverage
              const char *prefix = results[(i * columns) + 0];
              const char *name = results[(i * columns) + 1];
              int srid = atoi(results[(i * columns) + 2]);
              int type = atoi(results[(i * columns) + 3]);
              VectorCoverageSet coverage(prefix, name, srid, type);
              TableTree->AddAltVectorCoverage(rootVectorCoverages, dbAlias,
                                              &coverage);
            }
        }
      sqlite3_free_table(results);
    }
  if (ExistsWmsGetMap(dbAlias))
    {
      // fetching WMS Layers
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql =
        wxT("SELECT layer_name, url FROM \"") + dbAlias2 + wxT("\".wms_getmap");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                                  &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              // adding some WMS Layer
              const char *name = results[(i * columns) + 0];
              wxString xname = wxString::FromUTF8(name);
              const char *url = results[(i * columns) + 1];
              wxString xurl = wxString::FromUTF8(url);
              TableTree->AddWmsLayer(rootWMS, dbAlias, xname, xurl);
            }
        }
      sqlite3_free_table(results);
    }
// fetching tables and views
  sql = wxT("SELECT name, sql, type FROM \"");
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql += dbAlias2;
  sql +=
    wxT
    ("\".sqlite_master WHERE (type = 'table' OR type = 'view') ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  list = new TableViewList();
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          createSql = results[(i * columns) + 1];
          type = results[(i * columns) + 2];
          if (strstr(createSql, " VIRTUAL ") || strstr(createSql, " virtual "))
            virtualTable = true;
          else
            virtualTable = false;
          tblName = wxString::FromUTF8(name);
          if (strcmp(type, "view") == 0)
            list->Add(tblName, true, false);
          else
            list->Add(tblName, false, virtualTable);
        }
    }
  sqlite3_free_table(results);

  FindGeometries(dbAlias, list);
  FindGeoPackageGeometries(dbAlias, list);
  FindFdoOgrGeometries(dbAlias, list);
  ptv = list->GetFirst();
  while (ptv != NULL)
    {
      // inserting items into the Tree Control View
      if (ptv->IsView() == true)
        TableTree->AddView(dbAlias, ptv->GetName(), ptv->IsGeometry(), &nodes);
      else if (ptv->IsGeoPackageGeometry() == true)
        TableTree->AddGeoPackageTable(dbAlias, ptv->GetName(), &nodes);
      else if (ptv->IsGeoPackageVirtualGeometry() == true)
        TableTree->AddGeoPackageVirtualTable(dbAlias, ptv->GetName(), &nodes);
      else if (ptv->IsFdoOgrGeometry() == true)
        TableTree->AddFdoOgrTable(dbAlias, ptv->GetName(), &nodes);
      else if (ptv->IsFdoOgrVirtualGeometry() == true)
        TableTree->AddFdoOgrVirtualTable(dbAlias, ptv->GetName(), &nodes);
      else
        TableTree->AddTable(dbAlias, ptv->GetName(), ptv->IsVirtual(),
                            ptv->IsGeometry(), &nodes);
      ptv = ptv->GetNext();
    }
  delete list;
}

void MyFrame::ListAttached()
{
// 
// listing all Attached DBs
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  ret = sqlite3_get_table(GetSqlite(), "PRAGMA database_list", &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          if (strcasecmp("main", results[(i * columns) + 1]) == 0)
            continue;
          if (strcasecmp("temp", results[(i * columns) + 1]) == 0)
            continue;
          wxString dbAlias = wxString::FromUTF8(results[(i * columns) + 1]);
          wxString dbPath = wxString::FromUTF8(results[(i * columns) + 2]);
          MyAttachedDB *db = AttachedList.Find(dbAlias, dbPath);
          if (db == NULL)
            AttachedList.Insert(dbAlias, dbPath);
          InitTableTree(dbAlias, dbPath);
        }
    }
  sqlite3_free_table(results);
}

wxString *MyFrame::GetTables(int *n)
{
// loads the table list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString *tables = NULL;
  wxString sql;
  char *xsql;
  *n = 0;
  sql =
    wxT("SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return tables;
    }
  if (rows < 1)
    ;
  else
    {
      tables = new wxString[rows];
      *n = rows;
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          tables[i - 1] = wxString::FromUTF8(name);
        }
    }
  sqlite3_free_table(results);
  return tables;
}

wxString *MyFrame::GetTableColumns(wxString & dbPrefix, wxString & inTable,
                                   int *n)
{
// loads the column list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString *cols = NULL;
  wxString sql;
  char *xsql;
  *n = 0;
  char *xname = (char *) malloc((dbPrefix.Len() * 4) + 1);
  strcpy(xname, dbPrefix.ToUTF8());
  char *xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  wxString db = wxString::FromUTF8(xname2);
  free(xname2);
  xname = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(xname, inTable.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  wxString tbl = wxString::FromUTF8(xname2);
  free(xname2);
  sql = wxT("PRAGMA \"") + db + wxT("\".table_info(\"") + tbl + wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto empty;
    }
  if (rows < 1)
    goto empty;
  else
    {
      cols = new wxString[rows + 1];
      *n = rows + 1;
      cols[0] = wxString::FromUTF8("*****    none    *****");
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          cols[i] = wxString::FromUTF8(name);
        }
    }
  sqlite3_free_table(results);
  return cols;

empty:
  cols = new wxString[1];
  *n = 1;
  cols[0] = wxString::FromUTF8("*****    none    *****");
  return cols;
}

wxString *MyFrame::GetTableGeometries(wxString & dbPrefix, wxString & inTable,
                                      int *n)
{
// loads the geometry list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString *cols = NULL;
  wxString sql;
  char *xsql;
  *n = 0;
  char *xname = (char *) malloc((dbPrefix.Len() * 4) + 1);
  strcpy(xname, dbPrefix.ToUTF8());
  char *xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  wxString db = wxString::FromUTF8(xname2);
  free(xname2);
  xname = (char *) malloc((inTable.Len() * 4) + 1);
  strcpy(xname, inTable.ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  wxString tbl = wxString::FromUTF8(xname2);
  free(xname2);
  sql =
    wxT("SELECT f_geometry_column FROM \"") + db +
    wxT("\".geometry_columns WHERE f_table_name = '") + tbl + wxT("'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      goto empty;
    }
  if (rows < 1)
    goto empty;
  else
    {
      cols = new wxString[rows + 1];
      *n = rows + 1;
      cols[0] = wxString::FromUTF8("*****    none    *****");
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          cols[i] = wxString::FromUTF8(name);
        }
    }
  sqlite3_free_table(results);
  return cols;

empty:
  cols = new wxString[1];
  *n = 1;
  cols[0] = wxString::FromUTF8("*****    none    *****");
  return cols;
}

void MyFrame::GetTableColumns(wxString & tableName, MyTableInfo * list)
{
// loads the table's column list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *column;
  wxString Name;
  wxString Column;
  bool pKey;
  bool index;
  bool cached;
  wxString sql;
  char *xsql;
  char *xname;
  sql = wxT("PRAGMA table_info(\"");
  name = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(name, tableName.ToUTF8());
  xname = gaiaDoubleQuotedSql(name);
  free(name);
  sql += wxString::FromUTF8(xname);
  free(xname);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          if (atoi(results[(i * columns) + 5]) == 0)
            pKey = false;
          else
            pKey = true;
          Name = wxString::FromUTF8(name);
          list->AddColumn(Name, pKey);
        }
    }
  sqlite3_free_table(results);
  if (CheckMetadata() == true)
    {
      // ok, Spatial MetaData exists; retrieving Geometries and Spatial Indices
      sql =
        wxT
        ("SELECT f_geometry_column, spatial_index_enabled FROM geometry_columns ");
      sql += wxT("WHERE Lower(f_table_name) = Lower('");
      name = (char *) malloc((tableName.Len() * 4) + 1);
      strcpy(name, tableName.ToUTF8());
      xname = gaiaSingleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret =
        sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns,
                          &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              column = results[(i * columns) + 0];
              if (atoi(results[(i * columns) + 1]) == 1
                  || atoi(results[(i * columns) + 1]) == 3)
                index = true;
              else
                index = false;
              if (atoi(results[(i * columns) + 1]) == 2)
                cached = true;
              else
                cached = false;
              Column = wxString::FromUTF8(column);
              list->SetGeometry(Column, index, cached);
            }
        }
      sqlite3_free_table(results);

      if (HasVirtsMetadata() == true)
        {
          // may also be some VirtualShape
          sql = wxT("SELECT virt_geometry FROM virts_geometry_columns ");
          sql += wxT("WHERE Lower(virt_name) = Lower('");
          name = (char *) malloc((tableName.Len() * 4) + 1);
          strcpy(name, tableName.ToUTF8());
          xname = gaiaSingleQuotedSql(name);
          free(name);
          sql += wxString::FromUTF8(xname);
          free(xname);
          sql += wxT("')");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(SqliteHandle, xsql, &results, &rows,
                              &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              return;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  column = results[(i * columns) + 0];
                  Column = wxString::FromUTF8(column);
                  list->SetGeometry(Column, false, false);
                }
            }
          sqlite3_free_table(results);
        }
    }
}

void MyFrame::FindGeometries(TableViewList * list)
{
// Finds all registered Geometries within the current DB
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *table;

  list->PrepareSorted();
  if (CheckMetadata() == true)
    {
      // ok, Spatial MetaData exists; retrieving Geometries
      sql = wxT("SELECT f_table_name FROM main.geometry_columns");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret =
        sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns,
                          &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              table = results[(i * columns) + 0];
              wxString tbl = wxString::FromUTF8(table);
              list->SetGeometry(tbl);
            }
        }
      sqlite3_free_table(results);

      if (HasVirtsMetadata() == true)
        {
          // may also be some VirtualShape
          sql = wxT("SELECT virt_name FROM main.virts_geometry_columns");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(SqliteHandle, xsql, &results, &rows,
                              &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              return;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  table = results[(i * columns) + 0];
                  wxString tbl = wxString::FromUTF8(table);
                  list->SetGeometry(tbl);
                }
            }
          sqlite3_free_table(results);
        }

      if (HasViewsMetadata() == true)
        {
          // ok, Spatial MetaData exists; retrieving Geometries
          sql = wxT("SELECT view_name FROM main.views_geometry_columns");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(SqliteHandle, xsql, &results, &rows,
                              &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              return;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  table = results[(i * columns) + 0];
                  wxString tbl = wxString::FromUTF8(table);
                  list->SetGeometry(tbl);
                }
            }
          sqlite3_free_table(results);
        }
    }
}

void MyFrame::FindGeometries(wxString & dbAlias, TableViewList * list)
{
// Finds all registered Geometries within an Attached DB
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *table;

  list->PrepareSorted();
  if (CheckMetadata(dbAlias) == true)
    {
      // ok, Spatial MetaData exists; retrieving Geometries
      sql = wxT("SELECT f_table_name FROM \"");
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql += dbAlias2 + wxT("\".") + wxT("geometry_columns");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret =
        sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns,
                          &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              table = results[(i * columns) + 0];
              wxString tbl = wxString::FromUTF8(table);
              list->SetGeometry(tbl);
            }
        }
      sqlite3_free_table(results);

      if (HasVirtsMetadata(dbAlias) == true)
        {
          // may also be some VirtualShape
          sql = wxT("SELECT virt_name FROM \"");
          wxString dbAlias2 = dbAlias;
          DoubleQuoted(dbAlias2);
          sql += dbAlias2 + wxT("\".") + wxT("virts_geometry_columns");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(SqliteHandle, xsql, &results, &rows,
                              &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              return;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  table = results[(i * columns) + 0];
                  wxString tbl = wxString::FromUTF8(table);
                  list->SetGeometry(tbl);
                }
            }
          sqlite3_free_table(results);
        }

      if (HasViewsMetadata(dbAlias) == true)
        {
          // ok, Spatial MetaData exists; retrieving Geometries
          sql = wxT("SELECT view_name FROM \"");
          wxString dbAlias2 = dbAlias;
          DoubleQuoted(dbAlias2);
          sql += dbAlias2 + wxT("\".") + wxT("views_geometry_columns");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(SqliteHandle, xsql, &results, &rows,
                              &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              return;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  table = results[(i * columns) + 0];
                  wxString tbl = wxString::FromUTF8(table);
                  list->SetGeometry(tbl);
                }
            }
          sqlite3_free_table(results);
        }
    }
}

void MyFrame::FindGeoPackageGeometries(TableViewList * list)
{
// Finds all registered GPKG Geometries within the current GeoPackage DB
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *table;

  list->PrepareSorted();
  sql = wxT("SELECT table_name FROM main.gpkg_geometry_columns");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret =
    sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          table = results[(i * columns) + 0];
          wxString tbl = wxString::FromUTF8(table);
          list->SetGeoPackageGeometry(tbl);
          tbl = wxT("vgpkg_") + tbl;
          list->SetGeoPackageVirtualGeometry(tbl);
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::FindGeoPackageGeometries(wxString & dbAlias, TableViewList * list)
{
// Finds all registered GPKG Geometries within an ATTACHED GeoPackage DB
  MyAttachedDB *db = AttachedList.GetFirst();
  while (db != NULL)
    {
      if (db->GetDbPrefix() == dbAlias)
        {
          MyAttachedTable *tbl = db->GetFirst();
          while (tbl != NULL)
            {
              if (tbl->GetType() == MY_GPKG_TABLE)
                list->SetGeoPackageGeometry(tbl->GetName());
              if (tbl->GetType() == MY_GPKG_VTABLE)
                list->SetGeoPackageVirtualGeometry(tbl->GetName());
              tbl = tbl->GetNext();
            }
        }
      db = db->GetNext();
    }
}

void MyFrame::FindFdoOgrGeometries(TableViewList * list)
{
// Finds all registered FDO Geometries within the current FDO/OGR DB
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *table;

  list->PrepareSorted();
  sql = wxT("SELECT f_table_name, geometry_format FROM main.geometry_columns");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret =
    sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          table = results[(i * columns) + 0];
          wxString tbl = wxString::FromUTF8(table);
          list->SetFdoOgrGeometry(tbl);
          tbl = wxT("fdo_") + tbl;
          list->SetFdoOgrVirtualGeometry(tbl);
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::FindFdoOgrGeometries(wxString & dbAlias, TableViewList * list)
{
// Finds all registered FDO Geometries within an ATTACHED FDO/OGR DB
  MyAttachedDB *db = AttachedList.GetFirst();
  while (db != NULL)
    {
      if (db->GetDbPrefix() == dbAlias)
        {
          MyAttachedTable *tbl = db->GetFirst();
          while (tbl != NULL)
            {
              if (tbl->GetType() == MY_FDO_TABLE)
                list->SetFdoOgrGeometry(tbl->GetName());
              if (tbl->GetType() == MY_FDO_VTABLE)
                list->SetFdoOgrVirtualGeometry(tbl->GetName());
              tbl = tbl->GetNext();
            }
        }
      db = db->GetNext();
    }
}

void MyFrame::GetTableColumns(wxString & dbAlias, wxString & tableName,
                              MyTableInfo * list)
{
// loads the table's column list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *column;
  wxString Name;
  wxString Column;
  bool pKey;
  bool index;
  bool cached;
  wxString sql;
  char *xsql;
  char *xname;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".table_info(\"");
  name = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(name, tableName.ToUTF8());
  xname = gaiaDoubleQuotedSql(name);
  free(name);
  sql += wxString::FromUTF8(xname);
  free(xname);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          if (atoi(results[(i * columns) + 5]) == 0)
            pKey = false;
          else
            pKey = true;
          Name = wxString::FromUTF8(name);
          list->AddColumn(Name, pKey);
        }
    }
  sqlite3_free_table(results);
  if (CheckMetadata(dbAlias) == true)
    {
      // ok, Spatial MetaData exists; retrieving Geometries and Spatial Indices
      sql = wxT("SELECT f_geometry_column, spatial_index_enabled FROM \"");
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql += dbAlias2 + wxT("\".geometry_columns ");
      sql += wxT("WHERE Lower(f_table_name) = Lower('");
      name = (char *) malloc((tableName.Len() * 4) + 1);
      strcpy(name, tableName.ToUTF8());
      xname = gaiaSingleQuotedSql(name);
      free(name);
      sql += wxString::FromUTF8(xname);
      free(xname);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret =
        sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns,
                          &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              column = results[(i * columns) + 0];
              if (atoi(results[(i * columns) + 1]) == 1)
                index = true;
              else
                index = false;
              if (atoi(results[(i * columns) + 1]) == 2)
                cached = true;
              else
                cached = false;
              Column = wxString::FromUTF8(column);
              list->SetGeometry(Column, index, cached);
            }
        }
      sqlite3_free_table(results);

      if (HasVirtsMetadata(dbAlias) == true)
        {
          // may also be some VirtualShape
          sql = wxT("SELECT virt_geometry FROM \"");
          wxString dbAlias2 = dbAlias;
          DoubleQuoted(dbAlias2);
          sql += dbAlias2 + wxT("\".virts_geometry_columns ");
          sql += wxT("WHERE Lower(virt_name) = Lower('");
          name = (char *) malloc((tableName.Len() * 4) + 1);
          strcpy(name, tableName.ToUTF8());
          xname = gaiaSingleQuotedSql(name);
          free(name);
          sql += wxString::FromUTF8(xname);
          free(xname);
          sql += wxT("')");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(SqliteHandle, xsql, &results, &rows,
                              &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              return;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  column = results[(i * columns) + 0];
                  Column = wxString::FromUTF8(column);
                  list->SetGeometry(Column, false, false);
                }
            }
          sqlite3_free_table(results);
        }
    }
}

bool MyFrame::IsPrimaryKey(wxString & tableName, wxString & columnName)
{
// checking if some column is a PRIMARY KEY
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  bool pk = false;
  wxString sql;
  char *xsql;
  char *column;
  char *xname;
  char *xname2;
  column = (char *) malloc((columnName.Len() * 4) + 1);
  strcpy(column, columnName.ToUTF8());
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      free(column);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          if (strcasecmp(name, column) == 0)
            {
              if (atoi(results[(i * columns) + 5]) != 0)
                pk = true;
            }
        }
    }
  sqlite3_free_table(results);
  free(column);
  return pk;
}

void MyFrame::GetViewColumns(wxString & tableName, MyViewInfo * list)
{
// loads the view's column list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *column;
  wxString Name;
  wxString Column;
  bool index;
  bool cached;
  wxString sql;
  char *xname;
  char *xname2;
  char *xsql;
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          Name = wxString::FromUTF8(name);
          list->AddColumn(Name);
        }
    }
  sqlite3_free_table(results);
  if (HasViewsMetadata() == true)
    {
      // ok, Spatial MetaData exists; retrieving Geometries and Spatial Indices
      sql = wxT("SELECT a.view_geometry, b.spatial_index_enabled ");
      sql += wxT("FROM views_geometry_columns AS a ");
      sql += wxT("JOIN geometry_columns AS b ON (");
      sql += wxT("Lower(a.f_table_name) = Lower(b.f_table_name) AND ");
      sql += wxT("Lower(a.f_geometry_column) = Lower(b.f_geometry_column)) ");
      sql += wxT("WHERE Lower(view_name) = Lower('");
      xname = (char *) malloc((tableName.Len() * 4) + 1);
      strcpy(xname, tableName.ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret =
        sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns,
                          &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              column = results[(i * columns) + 0];
              if (atoi(results[(i * columns) + 1]) == 1)
                index = true;
              else
                index = false;
              if (atoi(results[(i * columns) + 1]) == 2)
                cached = true;
              else
                cached = false;
              Column = wxString::FromUTF8(column);
              list->SetGeometry(Column, index, cached);
            }
        }
      sqlite3_free_table(results);
    }
}

void MyFrame::GetViewColumns(wxString & dbAlias, wxString & tableName,
                             MyViewInfo * list)
{
// loads the view's column list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  char *column;
  wxString Name;
  wxString Column;
  bool index;
  bool cached;
  wxString sql;
  char *xname;
  char *xname2;
  char *xsql;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".table_info(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          Name = wxString::FromUTF8(name);
          list->AddColumn(Name);
        }
    }
  sqlite3_free_table(results);
  if (HasViewsMetadata(dbAlias) == true)
    {
      // ok, Spatial MetaData exists; retrieving Geometries and Spatial Indices
      sql = wxT("SELECT a.view_geometry, b.spatial_index_enabled ");
      wxString dbAlias2 = dbAlias;
      DoubleQuoted(dbAlias2);
      sql += wxT("FROM \"") + dbAlias2 + wxT("\".views_geometry_columns AS a ");
      sql += wxT("JOIN \"") + dbAlias2 + wxT("\".geometry_columns AS b ON (");
      sql += wxT("Lower(a.f_table_name) = Lower(b.f_table_name) AND ");
      sql += wxT("Lower(a.f_geometry_column) = Lower(b.f_geometry_column)) ");
      sql += wxT("WHERE Lower(view_name) = Lower('");
      xname = (char *) malloc((tableName.Len() * 4) + 1);
      strcpy(xname, tableName.ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret =
        sqlite3_get_table(SqliteHandle, xsql, &results, &rows, &columns,
                          &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              column = results[(i * columns) + 0];
              if (atoi(results[(i * columns) + 1]) == 1)
                index = true;
              else
                index = false;
              if (atoi(results[(i * columns) + 1]) == 2)
                cached = true;
              else
                cached = false;
              Column = wxString::FromUTF8(column);
              list->SetGeometry(Column, index, cached);
            }
        }
      sqlite3_free_table(results);
    }
}

void MyFrame::GetViewTriggers(wxString & tableName, MyViewInfo * list)
{
// loads the view's indices list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  sql = wxT("SELECT name FROM sqlite_master WHERE type = 'trigger' ");
  sql += wxT("AND Lower(tbl_name) = Lower('");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("') ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          Name = wxString::FromUTF8(name);
          list->AddTrigger(Name);
        }
    }
  sqlite3_free_table(results);
  TableTree->ExpandRoot();
}

void MyFrame::GetViewTriggers(wxString & dbAlias, wxString & tableName,
                              MyViewInfo * list)
{
// loads the view's indices list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql =
    wxT("SELECT name FROM \"") + dbAlias2 +
    wxT("\".sqlite_master WHERE type = 'trigger' AND tbl_name = '");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("' ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          Name = wxString::FromUTF8(name);
          list->AddTrigger(Name);
        }
    }
  sqlite3_free_table(results);
  TableTree->ExpandRoot();
}

void MyFrame::GetTableIndices(wxString & tableName, MyTableInfo * list)
{
// loads the table's indices list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  sql = wxT("PRAGMA index_list(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          Name = wxString::FromUTF8(name);
          list->AddIndex(Name);
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetTableIndices(wxString & dbAlias, wxString & tableName,
                              MyTableInfo * list)
{
// loads the table's indices list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".index_list(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 1];
          Name = wxString::FromUTF8(name);
          list->AddIndex(Name);
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetIndexFields(wxString & indexName, wxString & tableName,
                             wxTreeItemId & node)
{
// loads the index fields list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  sql = wxT("PRAGMA index_info(\"");
  xname = (char *) malloc((indexName.Len() * 4) + 1);
  strcpy(xname, indexName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 2];
          Name = wxString::FromUTF8(name);
          wxTreeItemId item = TableTree->AppendItem(node, Name, 3);
          wxString empty = wxEmptyString;
          TableTree->SetItemData(item,
                                 (wxTreeItemData *) (new
                                                     MyObject(MY_INDEX_FLD,
                                                              empty, tableName,
                                                              Name)));
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetIndexFields(wxString & dbAlias, wxString & indexName,
                             wxString & tableName, wxTreeItemId & node)
{
// loads the index fields list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".index_info(\"");
  xname = (char *) malloc((indexName.Len() * 4) + 1);
  strcpy(xname, indexName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 2];
          Name = wxString::FromUTF8(name);
          wxTreeItemId item = TableTree->AppendItem(node, Name, 3);
          TableTree->SetItemData(item,
                                 (wxTreeItemData *) (new
                                                     MyObject
                                                     (MY_INDEX_FLD,
                                                      dbAlias, tableName,
                                                      Name)));
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetPrimaryKeyFields(wxString & indexName, wxString & tableName,
                                  wxTreeItemId & node)
{
// loads the Primary Key fields list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  sql = wxT("PRAGMA index_info(\"");
  xname = (char *) malloc((indexName.Len() * 4) + 1);
  strcpy(xname, indexName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 2];
          Name = wxString::FromUTF8(name);
          wxTreeItemId item = TableTree->AppendItem(node, Name, 2);
          wxString empty = wxEmptyString;
          TableTree->SetItemData(item,
                                 (wxTreeItemData *) (new
                                                     MyObject
                                                     (MY_PRIMARY_KEY_FLD, empty,
                                                      tableName, Name)));
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetPrimaryKeyFields(wxString & dbAlias, wxString & indexName,
                                  wxString & tableName, wxTreeItemId & node)
{
// loads the Primary Key fields list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".index_info(\"");
  xname = (char *) malloc((indexName.Len() * 4) + 1);
  strcpy(xname, indexName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 2];
          Name = wxString::FromUTF8(name);
          wxTreeItemId item = TableTree->AppendItem(node, Name, 2);
          TableTree->SetItemData(item,
                                 (wxTreeItemData *) (new
                                                     MyObject
                                                     (MY_PRIMARY_KEY_FLD,
                                                      dbAlias, tableName,
                                                      Name)));
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetForeignKeys(wxString & tableName, wxTreeItemId & node)
{
// loads the Foreign Keys list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *ref_table;
  char *column;
  int id;
  int uid = INT_MIN;
  wxString refTable;
  wxString Column;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxTreeItemId item;
  sql = wxT("PRAGMA foreign_key_list(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      wxString empty = wxEmptyString;
      for (i = 1; i <= rows; i++)
        {
          id = atoi(results[(i * columns) + 0]);
          ref_table = results[(i * columns) + 2];
          column = results[(i * columns) + 3];
          refTable = wxString::FromUTF8(ref_table);
          Column = wxString::FromUTF8(column);
          if (i == 1)
            {
              wxString fkName = wxT("FK#");
              xname = sqlite3_mprintf("%d", id);
              fkName += wxString::FromUTF8(xname);
              sqlite3_free(xname);
              fkName += wxT(" ref: ") + refTable;
              item = TableTree->AppendItem(node, fkName, 18);
              wxString empty = wxEmptyString;
              TableTree->SetItemData(item,
                                     (wxTreeItemData *) (new
                                                         MyObject
                                                         (MY_FOREIGN_KEY, empty,
                                                          fkName)));
              uid = id;
          } else if (id != uid)
            {
              wxString fkName = wxT("FK#");
              xname = sqlite3_mprintf("%d", id);
              fkName += wxString::FromUTF8(xname);
              sqlite3_free(xname);
              fkName += wxT(" ref: ") + refTable;
              item = TableTree->AppendItem(node, fkName, 18);
              TableTree->SetItemData(item,
                                     (wxTreeItemData *) (new
                                                         MyObject
                                                         (MY_FOREIGN_KEY, empty,
                                                          fkName)));
              uid = id;
            }
          item = TableTree->AppendItem(item, Column, 3);
          TableTree->SetItemData(item,
                                 (wxTreeItemData *) (new
                                                     MyObject
                                                     (MY_FOREIGN_KEY_FLD,
                                                      empty, tableName,
                                                      Column)));
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetForeignKeys(wxString & dbAlias, wxString & tableName,
                             wxTreeItemId & node)
{
// loads the Foreign Keys list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *ref_table;
  char *column;
  int id;
  int uid = INT_MIN;
  wxString refTable;
  wxString Column;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxTreeItemId item;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".foreign_key_list(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          id = atoi(results[(i * columns) + 0]);
          ref_table = results[(i * columns) + 2];
          column = results[(i * columns) + 3];
          refTable = wxString::FromUTF8(ref_table);
          Column = wxString::FromUTF8(column);
          if (i == 1)
            {
              wxString fkName = wxT("FK#");
              xname = sqlite3_mprintf("%d", id);
              fkName += wxString::FromUTF8(xname);
              sqlite3_free(xname);
              fkName += wxT(" ref: ") + refTable;
              item = TableTree->AppendItem(node, fkName, 18);
              TableTree->SetItemData(item,
                                     (wxTreeItemData *) (new
                                                         MyObject
                                                         (MY_FOREIGN_KEY,
                                                          dbAlias, fkName)));
              uid = id;
          } else if (id != uid)
            {
              wxString fkName = wxT("FK#");
              xname = sqlite3_mprintf("%d", id);
              fkName += wxString::FromUTF8(xname);
              sqlite3_free(xname);
              fkName += wxT(" ref: ") + refTable;
              item = TableTree->AppendItem(node, fkName, 18);
              TableTree->SetItemData(item,
                                     (wxTreeItemData *) (new
                                                         MyObject
                                                         (MY_FOREIGN_KEY,
                                                          dbAlias, fkName)));
              uid = id;
            }
          item = TableTree->AppendItem(item, Column, 3);
          TableTree->SetItemData(item,
                                 (wxTreeItemData *) (new
                                                     MyObject
                                                     (MY_FOREIGN_KEY_FLD,
                                                      dbAlias, tableName,
                                                      Column)));
        }
    }
  sqlite3_free_table(results);
}

void MyFrame::GetTableTriggers(wxString & tableName, MyTableInfo * list)
{
// loads the table's indices list
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  sql = wxT("SELECT name FROM sqlite_master WHERE type = 'trigger' ");
  sql += wxT("AND Lower(tbl_name) = Lower('");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("') ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          Name = wxString::FromUTF8(name);
          list->AddTrigger(Name);
        }
    }
  sqlite3_free_table(results);
  TableTree->ExpandRoot();
}

void MyFrame::GetTableTriggers(wxString & dbAlias, wxString & tableName,
                               MyTableInfo * list)
{
// loads the table's indices list [Attached DB]
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *name;
  wxString Name;
  wxString sql;
  char *xsql;
  char *xname;
  char *xname2;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("SELECT name FROM \"") + dbAlias2;
  sql += wxT("\".sqlite_master WHERE type = 'trigger' AND tbl_name = '");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("' ORDER BY name");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          name = results[(i * columns) + 0];
          Name = wxString::FromUTF8(name);
          list->AddTrigger(Name);
        }
    }
  sqlite3_free_table(results);
  TableTree->ExpandRoot();
}

wxString *MyFrame::GetColumnNames(wxString & tableName, int *n_cols)
{
// loads the table's column names list
  wxString *cols = NULL;
  int nCols = 0;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *column;
  char *xname;
  char *xname2;
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((tableName.Len() * 4) + 1);
  strcpy(xname, tableName.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
    }
  sql = wxT("");
  if (rows < 1)
    ;
  else
    {
      nCols = rows;
      cols = new wxString[rows];
      for (i = 1; i <= rows; i++)
        {
          column = results[(i * columns) + 1];
          *(cols + i - 1) += wxString::FromUTF8(column);
        }
    }
  sqlite3_free_table(results);
  *n_cols = nCols;
  return cols;
}

int MyFrame::GetCharsetIndex(wxString & charset)
{
// identifies the INDEX for a given charset
  int i;
  for (i = 0; i < CharsetsLen; i++)
    {
      if (*(Charsets + i) == charset)
        return i;
    }
  return wxNOT_FOUND;
}

wxString & MyFrame::GetCharsetName(wxString & charset)
{
// identifies the full name for a given charset code
  int i;
  for (i = 0; i < CharsetsLen; i++)
    {
      if (*(Charsets + i) == charset)
        return *(CharsetsNames + i);
    }
  return charset;
}

void MyFrame::ClearTableTree()
{
// resets the table TREE list to the empty state
  wxString path = wxT("no current DB");
  TableTree->SetPath(path);
  TableTree->FlushAll();
}

void MyFrame::AutoFDOStart()
{
//
// trying to start the FDO-OGR auto-wrapper
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int spatial_type = 0;
  AutoFDOTables tables;
  AutoFDOTable *p;
  wxString fdoNames[5];
  char *xname;
  char *xname2;
  char *xname3;
  SpatiaLiteMetadata = false;
  AutoFDOmsg = wxT("");
  strcpy(sql, "SELECT CheckSpatialMetadata()");
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        spatial_type = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (spatial_type == 1 || spatial_type == 3)
    SpatiaLiteMetadata = true;
  if (spatial_type == 2)
    {
      //
      // ok, creating VirtualFDO tables 
      //
      strcpy(sql, "SELECT DISTINCT f_table_name FROM main.geometry_columns");
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualFDO table [if existing] 
          //
          xname = sqlite3_mprintf("fdo_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS main.\"%s\"", xname2);
          free(xname2);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          //
          // creating the VirtualFDO table 
          //
          xname = sqlite3_mprintf("fdo_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          xname3 = gaiaDoubleQuotedSql(p->GetName());
          sprintf(sql,
                  "CREATE VIRTUAL TABLE main.\"%s\" USING VirtualFDO(\"%s\")",
                  xname2, xname3);
          free(xname2);
          free(xname3);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          if (count < 5)
            fdoNames[count] =
              wxT("- VirtualTable: fdo_") + wxString::FromUTF8(p->GetName());
          else
            fdoNames[4] = wxT("- ... and others ...");
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        {
          AutoFDOmsg =
            wxT("FDO-OGR detected; activating FDO-OGR auto-wrapping ...\n\n");
          if (fdoNames[0].Len() > 0)
            AutoFDOmsg += fdoNames[0] + wxT("\n");
          if (fdoNames[1].Len() > 0)
            AutoFDOmsg += fdoNames[1] + wxT("\n");
          if (fdoNames[2].Len() > 0)
            AutoFDOmsg += fdoNames[2] + wxT("\n");
          if (fdoNames[3].Len() > 0)
            AutoFDOmsg += fdoNames[3] + wxT("\n");
          if (fdoNames[4].Len() > 0)
            AutoFDOmsg += fdoNames[4] + wxT("\n");
          AutoFDOmsg +=
            wxT
            ("\nAccessing these fdo_XX tables you can take full advantage of\n");
          AutoFDOmsg += wxT("FDO-OGR auto-wrapping facility\n");
          AutoFDOmsg +=
            wxT
            ("This allows you to access all FDO-OGR Geometries as if they\n");
          AutoFDOmsg +=
            wxT
            ("were native SpatiaLite ones in a completely transparent way.\n");
          TableTree->SetFdoOgrMode(true);
        }
      return;
    }
  TableTree->SetFdoOgrMode(false);
}

void MyFrame::AutoPostgresStop()
{
//
// trying to stop the PostgreSQL auto-wrapper
//
  DoDropPostgresTables();
  PostgresList.Clear();
}

void MyFrame::AutoFDOStop()
{
//
// trying to stop the FDO-OGR auto-wrapper
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int spatial_type = 0;
  char *xname;
  char *xxname;
  AutoFDOTables tables;
  AutoFDOTable *p;
  AutoFDOmsg = wxT("");
  strcpy(sql, "SELECT CheckSpatialMetadata()");
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        spatial_type = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (spatial_type == 2)
    {
      //
      // ok, destroying VirtualFDO tables 
      //
      strcpy(sql, "SELECT DISTINCT f_table_name FROM main.geometry_columns");
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualFDO table [if existing] 
          //
          xname = sqlite3_mprintf("fdo_%s", p->GetName());
          xxname = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS main.\"%s\"", xxname);
          free(xxname);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        AutoFDOmsg = wxT("FDO-OGR auto-wrapping shutdown done");
      TableTree->SetFdoOgrMode(false);
      return;
    }
  TableTree->SetFdoOgrMode(false);
}

void MyFrame::AutoFDOStart(wxString & dbAlias, MyAttachedDB * db)
{
//
// trying to start the FDO-OGR auto-wrapper - ATTACHED DB
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int spatial_type = 0;
  AutoFDOTables tables;
  AutoFDOTable *p;
  wxString fdoNames[5];
  char *xname;
  char *xname2;
  char *xname3;
  AutoFDOmsg = wxT("");
  char *db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
  strcpy(db_prefix, dbAlias.ToUTF8());
  char *xdb_prefix = gaiaSingleQuotedSql(db_prefix);
  free(db_prefix);
  sprintf(sql, "SELECT CheckSpatialMetadata('%s')", xdb_prefix);
  free(xdb_prefix);
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        spatial_type = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (spatial_type == 2)
    {
      //
      // ok, creating VirtualFDO tables 
      //
      db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
      strcpy(db_prefix, dbAlias.ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      free(db_prefix);
      sprintf(sql, "SELECT DISTINCT f_table_name FROM \"%s\".geometry_columns",
              xdb_prefix);
      free(xdb_prefix);
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualFDO table [if existing] 
          //
          db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
          strcpy(db_prefix, dbAlias.ToUTF8());
          xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
          free(db_prefix);
          xname = sqlite3_mprintf("fdo_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS \"%s\".\"%s\"", xdb_prefix,
                  xname2);
          free(xdb_prefix);
          free(xname2);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          //
          // creating the VirtualFDO table 
          //
          db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
          strcpy(db_prefix, dbAlias.ToUTF8());
          xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
          free(db_prefix);
          xname = sqlite3_mprintf("fdo_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          xname3 = gaiaDoubleQuotedSql(p->GetName());
          sprintf(sql,
                  "CREATE VIRTUAL TABLE \"%s\".\"%s\" USING VirtualFDO(\"%s\", \"%s\")",
                  xdb_prefix, xname2, xdb_prefix, xname3);
          free(xname2);
          free(xname3);
          free(xdb_prefix);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          wxString tbl_name = wxString::FromUTF8(p->GetName());
          AddAttachedTable(dbAlias, tbl_name, MY_FDO_TABLE);
          tbl_name = wxT("fdo_") + wxString::FromUTF8(p->GetName());
          AddAttachedTable(dbAlias, tbl_name, MY_FDO_VTABLE);
          if (count < 5)
            fdoNames[count] =
              wxT("- VirtualTable: ") + dbAlias + wxT(".fdo_") +
              wxString::FromUTF8(p->GetName());
          else
            fdoNames[4] = wxT("- ... and others ...");
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        {
          wxString autoFDOmsg =
            wxT("FDO-OGR detected; activating FDO-OGR auto-wrapping ...\n\n");
          if (fdoNames[0].Len() > 0)
            autoFDOmsg += fdoNames[0] + wxT("\n");
          if (fdoNames[1].Len() > 0)
            autoFDOmsg += fdoNames[1] + wxT("\n");
          if (fdoNames[2].Len() > 0)
            autoFDOmsg += fdoNames[2] + wxT("\n");
          if (fdoNames[3].Len() > 0)
            autoFDOmsg += fdoNames[3] + wxT("\n");
          if (fdoNames[4].Len() > 0)
            autoFDOmsg += fdoNames[4] + wxT("\n");
          autoFDOmsg +=
            wxT
            ("\nAccessing these fdo_XX tables you can take full advantage of\n");
          autoFDOmsg += wxT("FDO-OGR auto-wrapping facility\n");
          autoFDOmsg +=
            wxT
            ("This allows you to access all FDO-OGR Geometries as if they\n");
          autoFDOmsg +=
            wxT
            ("were native SpatiaLite ones in a completely transparent way.\n");
          wxMessageBox(autoFDOmsg, wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          db->SetFdoOgr();
        }
      return;
    }
}

void MyFrame::AutoFDOStop(wxString & dbAlias)
{
//
// trying to stop the FDO-OGR auto-wrapper - ATTACHED DB
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int spatial_type = 0;
  char *xname;
  char *xxname;
  AutoFDOTables tables;
  AutoFDOTable *p;
  AutoFDOmsg = wxT("");
  char *db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
  strcpy(db_prefix, dbAlias.ToUTF8());
  char *xdb_prefix = gaiaSingleQuotedSql(db_prefix);
  free(db_prefix);
  sprintf(sql, "SELECT CheckSpatialMetadata('%s')", xdb_prefix);
  free(xdb_prefix);
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        spatial_type = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (spatial_type == 2)
    {
      //
      // ok, destroying VirtualFDO tables 
      //
      db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
      strcpy(db_prefix, dbAlias.ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      free(db_prefix);
      sprintf(sql, "SELECT DISTINCT f_table_name FROM \"%s\".geometry_columns",
              xdb_prefix);
      free(xdb_prefix);
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualFDO table [if existing] 
          //
          db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
          strcpy(db_prefix, dbAlias.ToUTF8());
          xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
          free(db_prefix);
          xname = sqlite3_mprintf("fdo_%s", p->GetName());
          xxname = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS \"%s\".%s", xdb_prefix, xxname);
          free(xxname);
          free(xdb_prefix);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        AutoFDOmsg = wxT("FDO-OGR auto-wrapping shutdown done");
      return;
    }
}

void MyFrame::AutoGPKGStart()
{
//
// trying to start the OGC GeoPackage auto-wrapper
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int is_gpkg = 0;
  AutoGPKGTables tables;
  AutoGPKGTable *p;
  wxString gpkgNames[5];
  char *xname;
  char *xname2;
  char *xname3;
  AutoGPKGmsg = wxT("");
  strcpy(sql, "SELECT CheckGeoPackageMetadata()");
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        is_gpkg = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (is_gpkg)
    {
      //
      // ok, creating VirtualGPKG tables 
      //
      strcpy(sql, "SELECT DISTINCT table_name FROM gpkg_geometry_columns");
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualGPKG table [if existing] 
          //
          xname = sqlite3_mprintf("vgpkg_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS %s", xname2);
          free(xname2);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          //
          // creating the VirtualGPKG table 
          //
          xname = sqlite3_mprintf("vgpkg_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          xname3 = gaiaDoubleQuotedSql(p->GetName());
          sprintf(sql, "CREATE VIRTUAL TABLE \"%s\" USING VirtualGPKG(\"%s\")",
                  xname2, xname3);
          sqlite3_free(xname);
          free(xname2);
          free(xname3);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          if (count < 5)
            gpkgNames[count] =
              wxT("- VirtualTable: vgpkg_") + wxString::FromUTF8(p->GetName());
          else
            gpkgNames[4] = wxT("- ... and others ...");
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        {
          AutoGPKGmsg =
            wxT
            ("GPKG detected; activating OGC GeoPackage auto-wrapping ...\n\n");
          if (gpkgNames[0].Len() > 0)
            AutoGPKGmsg += gpkgNames[0] + wxT("\n");
          if (gpkgNames[1].Len() > 0)
            AutoGPKGmsg += gpkgNames[1] + wxT("\n");
          if (gpkgNames[2].Len() > 0)
            AutoGPKGmsg += gpkgNames[2] + wxT("\n");
          if (gpkgNames[3].Len() > 0)
            AutoGPKGmsg += gpkgNames[3] + wxT("\n");
          if (gpkgNames[4].Len() > 0)
            AutoGPKGmsg += gpkgNames[4] + wxT("\n");
          AutoGPKGmsg +=
            wxT
            ("\nAccessing these vgpkg_XX tables you can take full advantage of\n");
          AutoGPKGmsg += wxT("OGC GeoPackage auto-wrapping facility\n");
          AutoGPKGmsg +=
            wxT("This allows you to access all GPKG Geometries as if they\n");
          AutoGPKGmsg +=
            wxT
            ("were native SpatiaLite ones in a completely transparent way.\n");
          TableTree->SetGeoPackageMode(true);
        }
      return;
    }
  TableTree->SetGeoPackageMode(false);
}

void MyFrame::AutoGPKGStart(wxString & dbAlias, MyAttachedDB * db)
{
//
// trying to start the OGC GeoPackage auto-wrapper - ATTACHED-DB
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int is_gpkg = 0;
  AutoGPKGTables tables;
  AutoGPKGTable *p;
  wxString gpkgNames[5];
  char *xname;
  char *xname2;
  char *xname3;
  AutoGPKGmsg = wxT("");
  char *db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
  strcpy(db_prefix, dbAlias.ToUTF8());
  char *xdb_prefix = gaiaSingleQuotedSql(db_prefix);
  free(db_prefix);
  sprintf(sql, "SELECT CheckGeoPackageMetadata('%s')", xdb_prefix);
  free(xdb_prefix);
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        is_gpkg = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (is_gpkg)
    {
      //
      // ok, creating VirtualGPKG tables 
      //
      db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
      strcpy(db_prefix, dbAlias.ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      free(db_prefix);
      sprintf(sql,
              "SELECT DISTINCT table_name FROM \"%s\".gpkg_geometry_columns",
              xdb_prefix);
      free(xdb_prefix);
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualGPKG table [if existing] 
          //
          db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
          strcpy(db_prefix, dbAlias.ToUTF8());
          xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
          free(db_prefix);
          xname = sqlite3_mprintf("vgpkg_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS \"%s\".%s", xdb_prefix, xname2);
          free(xname2);
          free(xdb_prefix);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          //
          // creating the VirtualGPKG table 
          //
          db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
          strcpy(db_prefix, dbAlias.ToUTF8());
          xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
          free(db_prefix);
          xname = sqlite3_mprintf("vgpkg_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          xname3 = gaiaDoubleQuotedSql(p->GetName());
          sprintf(sql,
                  "CREATE VIRTUAL TABLE \"%s\".\"%s\" USING VirtualGPKG(\"%s\", \"%s\")",
                  xdb_prefix, xname2, xdb_prefix, xname3);
          sqlite3_free(xname);
          free(xname2);
          free(xname3);
          free(xdb_prefix);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          wxString tbl_name = wxString::FromUTF8(p->GetName());
          AddAttachedTable(dbAlias, tbl_name, MY_GPKG_TABLE);
          tbl_name = wxT("vgpkg_") + wxString::FromUTF8(p->GetName());
          AddAttachedTable(dbAlias, tbl_name, MY_GPKG_VTABLE);
          if (count < 5)
            gpkgNames[count] =
              wxT("- VirtualTable: vgpkg_") + wxString::FromUTF8(p->GetName());
          else
            gpkgNames[4] = wxT("- ... and others ...");
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        {
          wxString autoGPKGmsg =
            wxT
            ("GPKG detected; activating OGC GeoPackage auto-wrapping ...\n\n");
          if (gpkgNames[0].Len() > 0)
            autoGPKGmsg += gpkgNames[0] + wxT("\n");
          if (gpkgNames[1].Len() > 0)
            autoGPKGmsg += gpkgNames[1] + wxT("\n");
          if (gpkgNames[2].Len() > 0)
            autoGPKGmsg += gpkgNames[2] + wxT("\n");
          if (gpkgNames[3].Len() > 0)
            autoGPKGmsg += gpkgNames[3] + wxT("\n");
          if (gpkgNames[4].Len() > 0)
            autoGPKGmsg += gpkgNames[4] + wxT("\n");
          autoGPKGmsg +=
            wxT
            ("\nAccessing these vgpkg_XX tables you can take full advantage of\n");
          autoGPKGmsg += wxT("OCG GeoPackage auto-wrapping facility\n");
          autoGPKGmsg +=
            wxT("This allows you to access all GPKG Geometries as if they\n");
          autoGPKGmsg +=
            wxT
            ("were native SpatiaLite ones in a completely transparent way.\n");
          wxMessageBox(autoGPKGmsg, wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          db->SetGeoPackage();
        }
      return;
    }
}

void MyFrame::AutoGPKGStop()
{
//
// trying to stop the OGC GeoPackage auto-wrapper
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int is_gpkg = 0;
  char *xname;
  char *xname2;
  AutoGPKGTables tables;
  AutoGPKGTable *p;
  AutoGPKGmsg = wxT("");
  strcpy(sql, "SELECT CheckGeoPackageMetadata()");
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        is_gpkg = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (is_gpkg)
    {
      //
      // ok, destroying VirtualGPKG tables 
      //
      strcpy(sql, "SELECT DISTINCT table_name FROM gpkg_geometry_columns");
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualGPKG table [if existing] 
          //
          xname = sqlite3_mprintf("vgpkg_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS %s", xname2);
          free(xname2);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        AutoGPKGmsg = wxT("OGC GeoPackage auto-wrapping shutdown done");
      TableTree->SetGeoPackageMode(false);
      return;
    }
  TableTree->SetGeoPackageMode(false);
}

void MyFrame::AutoGPKGStop(wxString & dbAlias)
{
//
// trying to stop the OGC GeoPackage auto-wrapper
//
  int ret;
  const char *name;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int count = 0;
  int len;
  int is_gpkg = 0;
  char *xname;
  char *xname2;
  AutoGPKGTables tables;
  AutoGPKGTable *p;
  AutoGPKGmsg = wxT("");
  char *db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
  strcpy(db_prefix, dbAlias.ToUTF8());
  char *xdb_prefix = gaiaSingleQuotedSql(db_prefix);
  free(db_prefix);
  sprintf(sql, "SELECT CheckGeoPackageMetadata('%s')", xdb_prefix);
  free(xdb_prefix);
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        is_gpkg = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
error1:
  if (is_gpkg)
    {
      //
      // ok, destroying VirtualGPKG tables 
      //
      db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
      strcpy(db_prefix, dbAlias.ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      free(db_prefix);
      sprintf(sql,
              "SELECT DISTINCT table_name FROM \"%s\".gpkg_geometry_columns",
              xdb_prefix);
      free(xdb_prefix);
      ret =
        sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
      if (ret != SQLITE_OK)
        goto error;
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              name = results[(i * columns) + 0];
              if (name)
                {
                  len = strlen(name);
                  tables.Add(name, len);
                }
            }
        }
      sqlite3_free_table(results);
      p = tables.GetFirst();
      while (p)
        {
          //
          // destroying the VirtualGPKG table [if existing] 
          //
          db_prefix = (char *) malloc((dbAlias.Len() * 4) + 1);
          strcpy(db_prefix, dbAlias.ToUTF8());
          xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
          free(db_prefix);
          xname = sqlite3_mprintf("vgpkg_%s", p->GetName());
          xname2 = gaiaDoubleQuotedSql(xname);
          sqlite3_free(xname);
          sprintf(sql, "DROP TABLE IF EXISTS \"%s\".%s", xdb_prefix, xname2);
          free(xname2);
          free(xdb_prefix);
          ret = sqlite3_exec(SqliteHandle, sql, NULL, 0, NULL);
          if (ret != SQLITE_OK)
            goto error;
          count++;
          p = p->GetNext();
        }
    error:
      if (count)
        AutoGPKGmsg = wxT("OGC GeoPackage auto-wrapping shutdown done");
      return;
    }
}

bool MyFrame::HasVirtsMetadata()
{
//
// testing if the VIRTS_GEOMETRY_COLUMNS table exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  bool defined = false;
  sql = wxT("PRAGMA table_info(virts_geometry_columns)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        defined = true;
    }
  sqlite3_free_table(results);
  return defined;
}

bool MyFrame::HasVirtsMetadata(wxString & dbAlias)
{
//
// testing if the VIRTS_GEOMETRY_COLUMNS table exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  bool defined = false;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql =
    wxT("PRAGMA \"") + dbAlias2 + wxT("\".table_info(virts_geometry_columns)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        defined = true;
    }
  sqlite3_free_table(results);
  return defined;
}

bool MyFrame::HasViewsMetadata()
{
//
// testing if the VIEWS_GEOMETRY_COLUMNS table exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  bool defined = false;
  sql = wxT("PRAGMA table_info(views_geometry_columns)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        defined = true;
    }
  sqlite3_free_table(results);
  return defined;
}

bool MyFrame::HasViewsMetadata(wxString & dbAlias)
{
//
// testing if the VIEWS_GEOMETRY_COLUMNS table exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  bool defined = false;
  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql =
    wxT("PRAGMA \"") + dbAlias2 + wxT("\".table_info(views_geometry_columns)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        defined = true;
    }
  sqlite3_free_table(results);
  return defined;
}

int MyFrame::TestDotCommand(const char *p_stmt)
{
//
// identifying DOT-COMMANDS [SQL script]
//
  const char *stmt = p_stmt;
  int len;

  if (*p_stmt == '\n')
    stmt = p_stmt + 1;
  len = strlen(stmt);
  if (strncasecmp(stmt, ".loadxl ", 8) == 0)
    return CMD_LOADXL;
  if (len <= 9)
    return CMD_NONE;
  if (strncasecmp(stmt, ".loadshp ", 9) == 0)
    return CMD_LOADSHP;
  if (strncasecmp(stmt, ".loaddbf ", 9) == 0)
    return CMD_LOADDBF;
  if (strncasecmp(stmt, ".dumpshp ", 9) == 0)
    return CMD_DUMPSHP;
  if (strncasecmp(stmt, ".dumpdbf ", 9) == 0)
    return CMD_DUMPDBF;
  if (strncasecmp(stmt, ".sqllog ", 8) == 0)
    return CMD_SQLLOG;
  return CMD_NONE;
}

bool MyFrame::IsDotCommandLoadShp(const char *stmt, char *path,
                                  char *table, char *charset,
                                  char *column, int *srid, bool *coerce2D,
                                  bool *compressed)
{
//
// attempting to parse a .loadshp command [SQL script]
//
  wxString cmd = wxString::FromUTF8(stmt);
  wxString extPath;
  wxString extTable;
  wxString extCharset;
  wxString extColumn = wxT("Geometry");
  long extSrid = 0;
  bool extCoerce2D = false;
  bool extCompressed = false;
  int count = 0;
  wxStringTokenizer tok(cmd);
  while (tok.HasMoreTokens())
    {
      wxString token = tok.GetNextToken();
      switch (count)
        {
          case 0:
            extPath = token;
            break;
          case 1:
            extTable = token;
            break;
          case 2:
            extCharset = token;
            break;
          case 3:
            if (token.ToLong(&extSrid) == false)
              extSrid = 0;
            break;
          case 4:
            extColumn = token;
            break;
          case 5:
            extCoerce2D = true;
            break;
          case 6:
            extCompressed = true;
            break;
        };
      count++;
    }
  if (count == 3 || count == 4 || count == 5 || count == 6 || count == 7)
    {
      strcpy(path, extPath.ToUTF8());
      strcpy(table, extTable.ToUTF8());
      strcpy(charset, extCharset.ToUTF8());
      strcpy(column, extColumn.ToUTF8());
      *srid = extSrid;
      *coerce2D = extCoerce2D;
      *compressed = extCompressed;
      return true;
    }
  return false;
}

bool MyFrame::IsDotCommandLoadDbf(const char *stmt, char *path,
                                  char *table, char *charset)
{
//
// attempting to parse a .loaddbf command [SQL script]
//
  wxString cmd = wxString::FromUTF8(stmt);
  wxString extPath;
  wxString extTable;
  wxString extCharset;
  int count = 0;
  wxStringTokenizer tok(cmd);
  while (tok.HasMoreTokens())
    {
      wxString token = tok.GetNextToken();
      switch (count)
        {
          case 0:
            extPath = token;
            break;
          case 1:
            extTable = token;
            break;
          case 2:
            extCharset = token;
            break;
        };
      count++;
    }
  if (count == 3)
    {
      strcpy(path, extPath.ToUTF8());
      strcpy(table, extTable.ToUTF8());
      strcpy(charset, extCharset.ToUTF8());
      return true;
    }
  return false;
}

bool MyFrame::IsDotCommandLoadXL(const char *stmt, char *path,
                                 char *table, int *worksheetIndex,
                                 int *firstTitle)
{
//
// attempting to parse a .loadxl command [SQL script]
//
  wxString cmd = wxString::FromUTF8(stmt);
  wxString extPath;
  wxString extTable;
  int extWorksheetIndex = 0;
  bool extFirstTitle = false;
  int count = 0;
  char dummy[128];
  wxStringTokenizer tok(cmd);
  while (tok.HasMoreTokens())
    {
      wxString token = tok.GetNextToken();
      switch (count)
        {
          case 0:
            extPath = token;
            break;
          case 1:
            extTable = token;
            break;
          case 2:
            strcpy(dummy, token.ToUTF8());
            extWorksheetIndex = atoi(dummy);
            break;
          case 3:
            strcpy(dummy, token.ToUTF8());
            if (atoi(dummy) == 1)
              extFirstTitle = true;
            break;
        };
      count++;
    }
  if (count == 3 || count == 4 || count == 5)
    {
      if (extFirstTitle == true)
        *firstTitle = 1;
      else
        *firstTitle = 0;
      strcpy(path, extPath.ToUTF8());
      strcpy(table, extTable.ToUTF8());
      *worksheetIndex = extWorksheetIndex;
      return true;
    }
  return false;
}

bool MyFrame::IsViewGeometry(wxString & table, wxString & column)
{
//
// checking if some table.geometry corresponds to a VIEW
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql =
    wxT
    ("SELECT Count(*) FROM views_geometry_columns WHERE Lower(view_name) = Lower('");
  sql += table;
  sql += wxT("') AND view_geometry = Lower('");
  sql += column;
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsViewsGeometryColumns()
{
//
// checking if views_geometry_columns exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql = wxT("SELECT Count(*) FROM main.sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'views_geometry_columns'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsTopologies()
{
//
// checking if TOPOLOGIES exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql = wxT("SELECT Count(*) FROM main.sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'topologies'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsTopologies(wxString & dbAlias)
{
//
// checking if TOPOLOGIES exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("SELECT Count(*) FROM \"") + dbAlias2 + wxT("\".sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'topologies'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsNetworks()
{
//
// checking if NETWORKS exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql = wxT("SELECT Count(*) FROM main.sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'networks'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsNetworks(wxString & dbAlias)
{
//
// checking if NETWORKS exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("SELECT Count(*) FROM \"") + dbAlias2 + wxT("\".sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'networks'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsRasterCoverages()
{
//
// checking if RASTER_COVERAGES exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql = wxT("SELECT Count(*) FROM main.sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'raster_coverages'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsRasterCoverages(wxString & dbAlias)
{
//
// checking if RASTER_COVERAGES exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("SELECT Count(*) FROM \"") + dbAlias2 + wxT("\".sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'raster_coverages'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsVectorCoverages()
{
//
// checking if VECTOR_COVERAGES exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql = wxT("SELECT Count(*) FROM main.sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'vector_coverages'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsVectorCoverages(wxString & dbAlias)
{
//
// checking if VECTOR_COVERAGES exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("SELECT Count(*) FROM \"") + dbAlias2 + wxT("\".sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'vector_coverages'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsWmsGetMap()
{
//
// checking if WMS_GETMAP exists
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  sql = wxT("SELECT Count(*) FROM main.sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'wms_getmap'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

bool MyFrame::ExistsWmsGetMap(wxString & dbAlias)
{
//
// checking if WMS_GETMAP exists [Attached DB]
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  int count = 0;

  wxString dbAlias2 = dbAlias;
  DoubleQuoted(dbAlias2);
  sql = wxT("SELECT Count(*) FROM \"") + dbAlias2 + wxT("\".sqlite_master ");
  sql += wxT("WHERE type = 'table' AND ");
  sql += wxT("tbl_name = 'wms_getmap'");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(SqliteHandle, xsql, &results,
                              &rows, &columns, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          count = atoi(results[(i * columns) + 0]);
        }
    }
  sqlite3_free_table(results);
  if (count)
    return true;
  return false;
}

int MyFrame::GetMetaDataType()
{
//
// determining the MetaData type
//
  int ret;
  int i;
  char **results;
  int rows;
  int columns;
  char sql[1024];
  int spatial_type = 0;
  strcpy(sql, "SELECT CheckSpatialMetadata()");
  ret = sqlite3_get_table(SqliteHandle, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto error1;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        spatial_type = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
  if (spatial_type == 1)
    return METADATA_LEGACY;
  if (spatial_type == 3)
    return METADATA_CURRENT;
error1:
  return METADATA_UNKNOWN;
}

void MyFrame::InsertIntoLog(wxString & sql_stmt)
{
//
// inserting a row into sql_statements_log
//
  char *clean;
  int ret;
  char *errMsg = NULL;
  char *xsql;
  if (ReadOnlyConnection)
    return;
  wxString sql = wxT("INSERT INTO sql_statements_log ");
  sql += wxT("(id, time_start, user_agent, sql_statement) VALUES (");
  sql += wxT("NULL, strftime('%Y-%m-%dT%H:%M:%fZ', 'now'), ");
  sql += wxT("'spatialite_gui', '");
  clean = gaiaSingleQuotedSql(sql_stmt.ToUTF8());
  sql += wxString::FromUTF8(clean);
  free(clean);
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, xsql, NULL, 0, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("InsertIntoLog: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      LastSqlLogID = -1;
      return;
    }
  LastSqlLogID = sqlite3_last_insert_rowid(SqliteHandle);
}

void MyFrame::UpdateLog()
{
//
// updating sql_statements_log: success
//
  char dummy[64];
  int ret;
  char *errMsg = NULL;
  char *xsql;
  if (ReadOnlyConnection)
    return;
  wxString sql = wxT("UPDATE sql_statements_log SET ");
  sql += wxT("time_end = strftime('%Y-%m-%dT%H:%M:%fZ', 'now'), ");
  sql += wxT("success = 1, error_cause = 'success' ");
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
  sprintf(dummy, "WHERE id = %I64d", LastSqlLogID);
#else
  sprintf(dummy, "WHERE id = %lld", LastSqlLogID);
#endif
  sql += wxString::FromUTF8(dummy);
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, xsql, NULL, 0, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("UpdateLog: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
}

void MyFrame::UpdateLog(wxString & error_msg)
{
//
// updating sql_statements_log: failure
//
  char dummy[64];
  char *clean;
  int ret;
  char *errMsg = NULL;
  char *xsql;
  if (ReadOnlyConnection)
    return;
  wxString sql = wxT("UPDATE sql_statements_log SET ");
  sql += wxT("time_end = strftime('%Y-%m-%dT%H:%M:%fZ', 'now'), ");
  sql += wxT("success = 0, error_cause = '");
  clean = gaiaSingleQuotedSql(error_msg.ToUTF8());
  sql += wxString::FromUTF8(clean);
  free(clean);
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
  sprintf(dummy, "' WHERE id = %I64d", LastSqlLogID);
#else
  sprintf(dummy, "' WHERE id = %lld", LastSqlLogID);
#endif
  sql += wxString::FromUTF8(dummy);
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, xsql, NULL, 0, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("UpdateLog: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
}

void MyFrame::UpdateAbortedLog()
{
//
// updating sql_statements_log: success
//
  char dummy[64];
  int ret;
  char *errMsg = NULL;
  char *xsql;
  if (ReadOnlyConnection)
    return;
  wxString sql = wxT("UPDATE sql_statements_log SET ");
  sql += wxT("time_end = strftime('%Y-%m-%dT%H:%M:%fZ', 'now'), ");
  sql += wxT("success = 0, error_cause = 'aborted by the user' ");
#if defined(_WIN32) || defined(__MINGW32__)
// CAVEAT - M$ runtime doesn't supports %lld for 64 bits
  sprintf(dummy, "WHERE id = %I64d", LastSqlLogID);
#else
  sprintf(dummy, "WHERE id = %lld", LastSqlLogID);
#endif
  sql += wxString::FromUTF8(dummy);
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, xsql, NULL, 0, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("UpdateAbortedLog: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
}

void MyFrame::GetNextAttachedSymbol(wxString & symbol)
{
//
// return an unused DB alias name
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char sym[16];
  bool already_used = false;
  char x;
  char y;
  char z;
  ret = sqlite3_get_table(GetSqlite(), "PRAGMA database_list", &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (x = 'a'; x < 'z'; x++)
        {
          already_used = false;
          sprintf(sym, "%c", x);
          for (i = 1; i <= rows; i++)
            {
              if (strcasecmp(sym, results[(i * columns) + 1]) == 0)
                already_used = true;
            }
          if (already_used == false)
            {
              symbol = wxString::FromUTF8(sym);
              goto stop;
            }
        }
      for (y = 'a'; y < 'z'; y++)
        {
          for (x = 'a'; x < 'z'; x++)
            {
              already_used = false;
              sprintf(sym, "%c%c", y, x);
              for (i = 1; i <= rows; i++)
                {
                  if (strcasecmp(sym, results[(i * columns) + 1]) == 0)
                    already_used = true;
                }
              if (already_used == false)
                {
                  symbol = wxString::FromUTF8(sym);
                  goto stop;
                }
            }
        }
      for (z = 'a'; z < 'z'; z++)
        {
          for (y = 'a'; y < 'z'; y++)
            {
              for (x = 'a'; x < 'z'; x++)
                {
                  already_used = false;
                  sprintf(sym, "%c%c%c", z, y, x);
                  for (i = 1; i <= rows; i++)
                    {
                      if (strcasecmp(sym, results[(i * columns) + 1]) == 0)
                        already_used = true;
                    }
                  if (already_used == false)
                    {
                      symbol = wxString::FromUTF8(sym);
                      goto stop;
                    }
                }
            }
        }
    }
stop:
  sqlite3_free_table(results);
}

bool MyFrame::DoAttachDatabase(wxString & path)
{
//
// attempting to attach another DB
//
  int ret;
  char *errMsg = NULL;
  wxString symbol;
  char *xsql;
  wxString sql = wxT("ATTACH DATABASE \"");
  sql += path;
  sql += wxT("\" AS ");
  GetNextAttachedSymbol(symbol);
  sql += symbol;
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(SqliteHandle, xsql, NULL, 0, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("AttachDatabase: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return false;
    }
  return true;
}

bool MyFrame::IsDotCommandDumpShp(const char *stmt, char *table,
                                  char *column, char *path,
                                  char *charset, char *type)
{
//
// attempting to parse a .dumpshp command [SQL script]
//
  wxString cmd = wxString::FromUTF8(stmt);
  wxString extPath;
  wxString extTable;
  wxString extColumn;
  wxString extCharset;
  wxString extType;
  int count = 0;
  wxStringTokenizer tok(cmd);
  while (tok.HasMoreTokens())
    {
      wxString token = tok.GetNextToken();
      switch (count)
        {
          case 0:
            extTable = token;
            break;
          case 1:
            extColumn = token;
            break;
          case 2:
            extPath = token;
            break;
          case 3:
            extCharset = token;
            break;
          case 4:
            extType = token;
            break;
        };
      count++;
    }
  if (count == 4 || count == 5)
    {
      strcpy(path, extPath.ToUTF8());
      strcpy(table, extTable.ToUTF8());
      strcpy(column, extColumn.ToUTF8());
      strcpy(charset, extCharset.ToUTF8());
      strcpy(type, extType.ToUTF8());
      return true;
    }
  return false;
}

void MyFrame::CheckGPKG(wxString & tableName, MyTableInfo * list)
{
// testing for GPKG geometries
  list->CheckGPKG(this, SqliteHandle, tableName);
}

void MyFrame::CheckFdoOgr(wxString & tableName, MyTableInfo * list)
{
// testing for FDO/OGR geometries
  list->CheckFdoOgr(this, SqliteHandle, tableName);
}

void MyFrame::MapViewShowSelected(MapFeaturesList * list, bool zoom_mode)
{
// showing the selected Feature(s) on the Map Panel
  if (MapPanel != NULL)
    MapPanel->ShowSelectedFeatures(list, zoom_mode);
}

int MyFrame::GetMapSRID()
{
// returning the Map Panel current SRID (if any)
  if (MapPanel != NULL)
    return MapPanel->GetMapSRID();
  return -1;
}

void MyFrame::ResetSecurity()
{
// resetting the Security level
  if (Old_SPATIALITE_SECURITY_ENV != NULL)
    {
#ifdef _WIN32
      char *env = sqlite3_mprintf("SPATIALITE_SECURITY=%s",
                                  Old_SPATIALITE_SECURITY_ENV);
      putenv(env);
      sqlite3_free(env);
#else // not WIN32
      setenv("SPATIALITE_SECURITY", Old_SPATIALITE_SECURITY_ENV, 1);
#endif
  } else
    {
#ifdef _WIN32
      putenv("SPATIALITE_SECURITY=");
#else // not WIN32
      unsetenv("SPATIALITE_SECURITY");
#endif
    }
}

void MyFrame::SetSecurityRelaxed()
{
// enabling SPATIALIRE_SECURITY=relaxed
  Old_SPATIALITE_SECURITY_ENV = getenv("SPATIALITE_SECURITY");
#ifdef _WIN32
  putenv("SPATIALITE_SECURITY=relaxed");
#else // not WIN32
  setenv("SPATIALITE_SECURITY", "relaxed", 1);
#endif
  SecurityRelaxed = true;
}

void MyFrame::TestSecurityRelaxed(const char *path)
{
// testing for an appropriate security level
  if (IsSafeDB(path) == true)
    SetSecurityRelaxed();
}

bool MyFrame::IsSafeDB(const char *path)
{
// testing an existing DB for safety
  int ret;
  sqlite3 *handle;
  char **results;
  int rows;
  int columns;
  int i;
  int count = 1;
  void *cache = NULL;
  ret = sqlite3_open_v2(path, &handle, SQLITE_OPEN_READONLY, NULL);
  if (ret)
    {
      // an error occurred
      sqlite3_close(handle);
      return false;
    }
// setting up the internal cache
  cache = spatialite_alloc_connection();
  spatialite_init_ex(handle, cache, 0);
  ret = sqlite3_get_table(handle, "SELECT CountUnsafeTriggers()", &results,
                          &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    {
      // an error occurred
      sqlite3_close(handle);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *value = results[(i * columns) + 0];
          count = atoi(value);
        }
    }
  sqlite3_free_table(results);
  sqlite3_close(handle);
  spatialite_cleanup_ex(cache);
  if (count == 0)
    return true;
  wxString msg = wxT("SECURITY ALERT !!!\n\n");
  msg +=
    wxT
    ("The currently connected DB-file contains one or more harmful Triggers.\n");
  msg +=
    wxT("Such a condition could eventually cause serious security breaches.");
  wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_EXCLAMATION, this);
  return false;
}

void MyFrame::FeedZipHtml(unsigned char *zip, int offset, const char *data)
{
//
// feeding data into the Html ZIP buffer
//
  int i;
  int j;
  for (i = 0, j = 0; i < (int) strlen(data); i += 2, j++)
    {
      char byte_hi;
      char byte_lo;
      byte_hi = *(data + i);
      byte_lo = *(data + i + 1);
      *(zip + offset + j) = ColorMapEntry::ParseHex(byte_hi, byte_lo);
    }
}

void MyFrame::CheckUpdates()
{
//
// notifying if an eventual updated verrsion is available
//
  char *text =::GetUpdateVersion();
  if (text != NULL)
    {
      if (strcmp(text, "up-to-date") == 0)
        ;
      else
        {
          char *version;
          char *date;
          char *download_url;
          if (DoParseNewVersion(text, &version, &date, &download_url))
            {
              // there is an updated version available
              wxString msg = wxT("An updated version is now available:\n\n");
              msg += wxT("\tspatialite_gui v.");
              msg += wxString::FromUTF8(version);
              msg += wxT("\n\tReleased on ");
              msg += wxString::FromUTF8(date);
#ifdef _WIN32
              // only for MS Windows we'll support downloading the updated binary
              if (download_url != NULL)
                {
                  msg +=
                    wxT
                    ("\n\nDo you want to download the updated binary package ?");
                  int ret = wxMessageBox(msg, wxT("spatialite_gui"),
                                         wxYES_NO | wxICON_QUESTION, this);
                  if (ret == wxYES)
                    {
                      unsigned char *data = NULL;
                      int data_len = 0;
                      int ret;
                      ::wxBeginBusyCursor();
                      ret =
                        DoDownloadUpdatedPackage(download_url, &data,
                                                 &data_len);
                      ::wxEndBusyCursor();
                      if (ret)
                        {
                          wxString fileName;
                          DoGetPackageName(download_url, fileName);
                          DoSaveUpdatedPackage(data, data_len, fileName);
                        }
                      if (data != NULL)
                        free(data);
                      goto end;
                    }
              } else
                wxMessageBox(msg, wxT("spatialite_gui"),
                             wxOK | wxICON_INFORMATION, this);
#else
              // not MS Windows - just notifying
              wxMessageBox(msg, wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
#endif
            }
          if (version != NULL)
            free(version);
          if (date != NULL)
            free(date);
          if (download_url != NULL)
            free(download_url);
        }
#ifdef _WIN32
    end:
#endif
      free(text);
    }
}

void MyFrame::DoGetPackageName(const char *download_url, wxString & name)
{
//
// extracting the file name from the download URL
//
  const char *p_base = download_url;
  const char *p = download_url;
  while (*p != '\0')
    {
      if (*p == '/')
        p_base = p;
      p++;
    }
  if (*p_base == '/')
    p_base++;
  name = wxString::FromUTF8(p_base);
}

void MyFrame::DoSaveUpdatedPackage(const unsigned char *data, int data_len,
                                   wxString & fileName)
{
//
// saving the downloaded package
//
  char xpath[2048];
  FILE *out = NULL;
  int wr;
  int ret;
  wxString fileType = wxT("7-Zip Compressed Archive (*.7zip)|*.7zip");
  wxFileName name(fileName);
  wxFileDialog fileDialog(this, wxT("Saving the updated binary package"),
                          wxT(""), name.GetName(), fileType,
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(xpath, fileDialog.GetPath().ToUTF8());
      out = fopen(xpath, "wb");
      if (!out)
        {
          wxMessageBox(wxT("Cannot open '") + fileDialog.GetPath() +
                       wxT("' for writing"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          return;
        }
      ::wxBeginBusyCursor();
      wr = fwrite(data, 1, data_len, out);
      if (wr != data_len)
        {
          wxMessageBox(wxT("an I/O error occurred"), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
        }
      fclose(out);
      ::wxEndBusyCursor();
    }
}

bool MyFrame::DoParseNewVersion(const char *text, char **version, char **date,
                                char **download_url)
{
// parsing the "updated version available" string
  const char *in = text;
  char *ver = NULL;
  char *day = NULL;
  char *url = NULL;
  int len;
  char buf[8192];
  char *p = buf;
  int fld = 0;
  while (1)
    {
      if (*in == '\0' || *in == ';')
        {
          *p = '\0';
          if (fld == 0)
            {
              len = strlen(buf);
              ver = (char *) malloc(len + 1);
              strcpy(ver, buf);
            }
          if (fld == 1)
            {
              len = strlen(buf);
              day = (char *) malloc(len + 1);
              strcpy(day, buf);
            }
          if (fld == 2)
            {
              len = strlen(buf);
              url = (char *) malloc(len + 1);
              strcpy(url, buf);
            }
          if (*in == '\0')
            break;
          fld++;
          p = buf;
          in++;
          continue;
        }
      *p++ = *in++;
    }
  if (ver != NULL && day != NULL)
    {
      *version = ver;
      *date = day;
      *download_url = url;
      return true;
    }
  if (ver != NULL)
    free(ver);
  if (day != NULL)
    free(day);
  if (url != NULL)
    free(url);
  *version = NULL;
  *date = NULL;
  *download_url = NULL;
  return false;
}

MyStatusBar::MyStatusBar(MyFrame * parent):wxStatusBar(parent)
{
// constructor
  Parent = parent;
  int widths[2];
  widths[0] = 20;
  widths[1] = -1;
  SetFieldsCount(2);
  SetStatusWidths(2, widths);
  Bitmap = new wxStaticBitmap(this, wxID_ANY, wxIcon(disconnect_xpm));
// setting up the event handlers
  Connect(wxID_ANY, wxEVT_SIZE, (wxObjectEventFunction) & MyStatusBar::OnSize);
}

void MyStatusBar::OnSize(wxSizeEvent & WXUNUSED(event))
{
//
// this window has changed its size
//
  wxRect rect;
  GetFieldRect(0, rect);
  wxSize size = Bitmap->GetSize();
  Bitmap->Move(rect.x + (rect.width - size.x) / 2,
               rect.y + (rect.height - size.y) / 2);
}

void MyStatusBar::SetSecurityRelaxedIcon()
{
  Bitmap->SetIcon(wxIcon(security_relaxed_xpm));
}

void MyStatusBar::SetSecurityStrongIcon()
{
  Bitmap->SetIcon(wxIcon(security_lock_xpm));
}

void MyStatusBar::SetReadOnlyIcon()
{
  Bitmap->SetIcon(wxIcon(security_rdonly_xpm));
}

void MyStatusBar::SetNotConnectedIcon()
{
  Bitmap->SetIcon(wxIcon(disconnect_xpm));
}

void MyStatusBar::SetText(wxString & msg)
{
  wxStatusBar *sb = Parent->GetStatusBar();
  if (!sb)
    return;
  sb->SetStatusText(msg, 1);
}
