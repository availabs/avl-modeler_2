/*
/ MapPanel.cpp
/ the Map Panel of spatialite_gui  - a SQLite /SpatiaLite GUI tool
/
/ version 2.0, 2017 January 8
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2017  Alessandro Furieri
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
#include "wx/config.h"
#include "wx/filename.h"
#include "wx/spinctrl.h"
#include "wx/colordlg.h"
#include "wx/clipbrd.h"

#include <float.h>

#include "icons/icon_map.xpm"
#include "icons/linestrings_on.xpm"
#include "icons/points_on.xpm"
#include "icons/polygons_on.xpm"
#include "icons/vector.xpm"
#include "icons/topo_geo.xpm"
#include "icons/topo_net.xpm"
#include "icons/coverage.xpm"
#include "icons/wms.xpm"
#include "icons/icon_info.xpm"
#include "icons/mapconfig.xpm"
#include "icons/map_add.xpm"
#include "icons/map_table.xpm"
#include "icons/identify.xpm"
#include "icons/zoom_in.xpm"
#include "icons/zoom_out.xpm"
#include "icons/pan.xpm"
#include "icons/map_image.xpm"
#include "icons/printer.xpm"
#include "icons/about.xpm"
#include "icons/exit.xpm"
#include "icons/red_light.xpm"
#include "icons/yellow_light.xpm"
#include "icons/green_light.xpm"

MyMapPanel::MyMapPanel(MyFrame * parent, const wxString & title,
                       const wxPoint & pos,
                       const wxSize & size):wxFrame((wxFrame *) NULL, -1, title,
                                                    pos, size)
{
//
// Map Panel frame constructor
//
  Parent = parent;

//
// setting up the application icon
//      
  wxIcon MyIcon(icon_map_xpm);
  SetIcon(MyIcon);

  MapName = wxT("default");
  Title = wxT("*** Anonymous ***");
  Abstract = wxT("*** Undefined ***");

  IsIdentify = false;
  IsZoomIn = false;
  IsZoomOut = false;
  IsPan = false;

  BtnLoadMapConfig = new wxBitmap(mapconfig_xpm);
  BtnAddLayer = new wxBitmap(map_add_xpm);
  BtnIdentify = new wxBitmap(identify_xpm);
  BtnZoomIn = new wxBitmap(zoom_in_xpm);
  BtnZoomOut = new wxBitmap(zoom_out_xpm);
  BtnPan = new wxBitmap(pan_xpm);
  BtnSqlLayer = new wxBitmap(map_table_xpm);
  BtnMapImage = new wxBitmap(map_image_xpm);
  BtnPrinter = new wxBitmap(printer_xpm);
  BtnAbout = new wxBitmap(about_xpm);
  BtnExit = new wxBitmap(exit_xpm);

//
// setting up panes
//
  LayerTree = new MyLayerTree(this);
  MapView = new MyMapView(this);
  Manager.SetManagedWindow(this);
  wxAuiPaneInfo paneView = wxAuiPaneInfo().Centre();
  paneView.Name(wxT("map_view"));
  paneView.CaptionVisible(false);
  paneView.Floatable(true);
  paneView.Dockable(true);
  paneView.Movable(true);
  paneView.Gripper(false);
  paneView.CloseButton(false);
  Manager.AddPane(MapView, paneView);
  wxAuiPaneInfo paneTree = wxAuiPaneInfo().Left();
  paneTree.Name(wxT("tree_view"));
  paneTree.DefaultPane();
  paneTree.CaptionVisible(false);
  paneTree.Floatable(true);
  paneTree.Dockable(true);
  paneTree.Movable(true);
  paneTree.Gripper(true);
  paneTree.CloseButton(false);
  paneTree.BestSize(wxSize(300, 480));
  Manager.AddPane(LayerTree, paneTree, wxPoint(0, 10));
  Manager.Update();
  Centre();
  //ClearLayerTree();

//
// setting up the status bar
//
  StatusBar = new MyMapPanelStatusBar(this);
  StatusBar->SetMinHeight(40);
  SetStatusBar((wxStatusBar *) StatusBar);

//
// setting up the menu bar
//
  wxMenu *menuFile = new wxMenu;
  wxMenuItem *menuItem;
  menuItem =
    new wxMenuItem(menuFile, ID_LoadMapConfig,
                   wxT("&Load a Map Configuration"));
  menuItem->SetBitmap(*BtnLoadMapConfig);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem = new wxMenuItem(menuFile, ID_AddLayer, wxT("Add a Map &Layer"));
  menuItem->SetBitmap(*BtnAddLayer);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem =
    new wxMenuItem(menuFile, ID_ZoomIn, wxT("Zoom &In"), wxT(""), wxITEM_RADIO);
  menuFile->Append(menuItem);
  menuItem =
    new wxMenuItem(menuFile, ID_ZoomOut,
                   wxT("Zoom &Out"), wxT(""), wxITEM_RADIO);
  menuFile->Append(menuItem);
  menuItem =
    new wxMenuItem(menuFile, ID_Pan, wxT("&Pan"), wxT(""), wxITEM_RADIO);
  menuFile->Append(menuItem);
  menuItem =
    new wxMenuItem(menuFile, ID_Identify,
                   wxT("Identify"), wxT(""), wxITEM_RADIO);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem = new wxMenuItem(menuFile, ID_SqlLayer, wxT("Query all Features"));
  menuItem->SetBitmap(*BtnSqlLayer);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
  menuItem = new wxMenuItem(menuFile, ID_MapImage, wxT("&Map Image"));
  menuItem->SetBitmap(*BtnMapImage);
  menuFile->Append(menuItem);
  menuItem = new wxMenuItem(menuFile, ID_Printer, wxT("&Printer (PDF)"));
  menuItem->SetBitmap(*BtnPrinter);
  menuFile->Append(menuItem);
  menuFile->AppendSeparator();
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
  menuBar->Enable(ID_LoadMapConfig, true);
  menuBar->Enable(ID_AddLayer, true);
  menuBar->Enable(ID_SqlLayer, false);
  menuBar->Enable(ID_Identify, false);
  menuBar->Enable(ID_ZoomIn, false);
  menuBar->Enable(ID_ZoomOut, false);
  menuBar->Enable(ID_Pan, false);
  menuBar->Enable(ID_MapImage, false);
  menuBar->Enable(ID_Printer, false);

//
// setting up the toolbar
//      
  wxToolBar *toolBar = CreateToolBar();
  toolBar->AddTool(ID_LoadMapConfig, wxT("Load a Map Configuration"),
                   *BtnLoadMapConfig, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Load a Map Configuration"));
  toolBar->AddSeparator();
  toolBar->AddTool(ID_AddLayer, wxT("Add a Map Layer"),
                   *BtnAddLayer, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Add a Map Layer"));
  toolBar->AddSeparator();
  toolBar->AddTool(ID_ZoomIn, wxT("Zoom In"), *BtnZoomIn, wxNullBitmap,
                   wxITEM_RADIO, wxT("Zoom In"));
  toolBar->AddTool(ID_ZoomOut, wxT("Zoom Out"), *BtnZoomOut, wxNullBitmap,
                   wxITEM_RADIO, wxT("Zoom Out"));
  toolBar->AddTool(ID_Pan, wxT("Pan"), *BtnPan, wxNullBitmap, wxITEM_RADIO,
                   wxT("Pan"));
  toolBar->AddTool(ID_Identify, wxT("Identify"), *BtnIdentify, wxNullBitmap,
                   wxITEM_RADIO, wxT("Identify"));
  toolBar->AddSeparator();
  toolBar->AddTool(ID_SqlLayer, wxT("Query Table"),
                   *BtnSqlLayer, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Query Table"));
  toolBar->AddSeparator();
  toolBar->AddTool(ID_MapImage, wxT("MapImage"),
                   *BtnMapImage, wxNullBitmap, wxITEM_NORMAL, wxT("Map Image"));
  toolBar->AddTool(ID_Printer, wxT("Printer (PDF)"),
                   *BtnPrinter, wxNullBitmap, wxITEM_NORMAL,
                   wxT("Printer (PDF)"));
  toolBar->AddSeparator();
  toolBar->AddTool(wxID_ABOUT, wxT("About ..."), *BtnAbout, wxNullBitmap,
                   wxITEM_NORMAL, wxT("About ..."));
  toolBar->AddSeparator();
  toolBar->AddTool(wxID_EXIT, wxT("Quit"), *BtnExit, wxNullBitmap,
                   wxITEM_NORMAL, wxT("Quit"));
  toolBar->Realize();
  SetToolBar(toolBar);

//
// setting up the toolbar initial state
//
  toolBar->EnableTool(ID_LoadMapConfig, true);
  toolBar->EnableTool(ID_AddLayer, true);
  toolBar->EnableTool(ID_SqlLayer, false);
  toolBar->EnableTool(ID_Identify, false);
  toolBar->EnableTool(ID_ZoomIn, false);
  toolBar->EnableTool(ID_ZoomOut, false);
  toolBar->EnableTool(ID_Pan, false);
  toolBar->EnableTool(ID_MapImage, false);
  toolBar->EnableTool(ID_Printer, false);

//
// setting up event handlers for menu and toolbar
//
  Connect(ID_LoadMapConfig, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnLoadMapConfig);
  Connect(ID_AddLayer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnAddLayer);
  Connect(ID_Identify, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnIdentify);
  Connect(ID_ZoomIn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnZoomIn);
  Connect(ID_ZoomOut, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnZoomOut);
  Connect(ID_Pan, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnPan);
  Connect(ID_SqlLayer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnSqlLayer);
  Connect(ID_MapImage, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnMapImage);
  Connect(ID_Printer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnPrinter);
  Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnAbout);
  Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapPanel::OnQuit);
}

MyMapPanel::~MyMapPanel()
{
//
// Map Panel frame destructor
//
  if (Parent != NULL)
    Parent->MapPanelClosing();
  MapView->MapPanelClosing();
  Manager.UnInit();
  if (BtnLoadMapConfig != NULL)
    delete BtnLoadMapConfig;
  if (BtnAddLayer != NULL)
    delete BtnAddLayer;
  if (BtnSqlLayer != NULL)
    delete BtnSqlLayer;
  if (BtnIdentify != NULL)
    delete BtnIdentify;
  if (BtnZoomIn != NULL)
    delete BtnZoomIn;
  if (BtnZoomOut != NULL)
    delete BtnZoomOut;
  if (BtnPan != NULL)
    delete BtnPan;
  if (BtnMapImage != NULL)
    delete BtnMapImage;
  if (BtnPrinter != NULL)
    delete BtnPrinter;
  if (BtnAbout != NULL)
    delete BtnAbout;
  if (BtnExit != NULL)
    delete BtnExit;
}

void MyMapPanel::UpdateTools()
{
//
// updating the Tools State
//
  wxMenuBar *menuBar = GetMenuBar();
  menuBar->Enable(ID_AddLayer, true);
  if (MapView->IsValidMap() == false)
    {
      menuBar->Enable(ID_ZoomIn, false);
      menuBar->Enable(ID_ZoomOut, false);
      menuBar->Enable(ID_Pan, false);
      menuBar->Enable(ID_SqlLayer, false);
      menuBar->Enable(ID_Identify, false);
      menuBar->Enable(ID_MapImage, false);
      menuBar->Enable(ID_Printer, false);
  } else
    {
      menuBar->Enable(ID_ZoomIn, true);
      menuBar->Enable(ID_ZoomOut, true);
      menuBar->Enable(ID_Pan, true);
      if (MapView->CanQueryTable() == true)
        menuBar->Enable(ID_SqlLayer, true);
      else
        menuBar->Enable(ID_SqlLayer, false);
      if (MapView->CanIdentify() == true)
        menuBar->Enable(ID_Identify, true);
      else
        menuBar->Enable(ID_Identify, false);
      menuBar->Enable(ID_MapImage, true);
      menuBar->Enable(ID_Printer, true);
    }

  wxToolBar *toolBar = GetToolBar();
  toolBar->EnableTool(ID_AddLayer, true);
  if (MapView->IsValidMap() == false)
    {
      toolBar->EnableTool(ID_ZoomIn, false);
      toolBar->EnableTool(ID_ZoomOut, false);
      toolBar->EnableTool(ID_Pan, false);
      toolBar->EnableTool(ID_SqlLayer, false);
      toolBar->EnableTool(ID_Identify, false);
      toolBar->EnableTool(ID_MapImage, false);
      toolBar->EnableTool(ID_Printer, false);
  } else
    {
      toolBar->EnableTool(ID_ZoomIn, true);
      toolBar->EnableTool(ID_ZoomOut, true);
      toolBar->EnableTool(ID_Pan, true);
      toolBar->EnableTool(ID_SqlLayer, true);
      if (MapView->CanQueryTable() == true)
        toolBar->EnableTool(ID_SqlLayer, true);
      else
        toolBar->EnableTool(ID_SqlLayer, false);
      if (MapView->CanIdentify() == true)
        toolBar->EnableTool(ID_Identify, true);
      else
        toolBar->EnableTool(ID_Identify, false);
      toolBar->EnableTool(ID_MapImage, true);
      toolBar->EnableTool(ID_Printer, true);
    }

  if (MapView->IsValidMap() == false)
    {
      IsIdentify = false;
      IsZoomIn = false;
      IsZoomOut = false;
      IsPan = false;
  } else
    {
      if (MapView->GetActiveLayer() == NULL)
        IsIdentify = false;
      if (IsIdentify == false && IsPan == false && IsZoomOut == false)
        IsZoomIn = true;
      if (IsIdentify == true)
        {
          menuBar->Check(ID_Identify, true);
          toolBar->ToggleTool(ID_Identify, true);
        }
      if (IsPan == true)
        {
          menuBar->Check(ID_Pan, true);
          toolBar->ToggleTool(ID_Pan, true);
        }
      if (IsZoomIn == true)
        {
          menuBar->Check(ID_ZoomIn, true);
          toolBar->ToggleTool(ID_ZoomIn, true);
        }
      if (IsZoomOut == true)
        {
          menuBar->Check(ID_ZoomOut, true);
          toolBar->ToggleTool(ID_ZoomOut, true);
        }
    }
}

IncompleteLayer::IncompleteLayer(const char *type, const char *name)
{
// ctor
  int len = strlen(type);
  Type = (char *) malloc(len + 1);
  strcpy(Type, type);
  len = strlen(name);
  CoverageName = (char *) malloc(len + 1);
  strcpy(CoverageName, name);
  Next = NULL;
}

IncompleteLayersList::~IncompleteLayersList()
{
// dtor
  IncompleteLayer *pN;
  IncompleteLayer *pNn;
  pN = First;
  while (pN != NULL)
    {
      pNn = pN->GetNext();
      delete pN;
      pN = pNn;
    }
}

void IncompleteLayersList::Add(const char *type, const char *name)
{
//
// adding an incomplete Layer to the List
//
  IncompleteLayer *pN = new IncompleteLayer(type, name);
  if (First == NULL)
    First = pN;
  if (Last != NULL)
    Last->SetNext(pN);
  Last = pN;
}

IncompleteLayersList *MyMapPanel::CheckIncompleteLayers()
{
//
// testing for the existence of incomplete Layer Definitions (missing Extents)
//
  IncompleteLayersList *list = new IncompleteLayersList();
  int i;
  char **results;
  int rows;
  int columns;
  const char *sql =
    "SELECT 'vector' AS class, coverage_name FROM vector_coverages "
    "WHERE geo_minx IS NULL OR geo_miny IS NULL OR geo_maxx IS NULL OR geo_maxy IS NULL OR "
    "extent_minx IS NULL OR extent_miny IS NULL OR extent_maxx IS NULL OR extent_maxy IS NULL "
    "UNION SELECT 'vector', coverage_name FROM vector_coverages_srid "
    "WHERE extent_minx IS NULL OR extent_miny IS NULL OR extent_maxx IS NULL OR extent_maxy IS NULL "
    "UNION SELECT 'raster' AS class, coverage_name FROM raster_coverages "
    "WHERE geo_minx IS NULL OR geo_miny IS NULL OR geo_maxx IS NULL OR geo_maxy IS NULL OR "
    "extent_minx IS NULL OR extent_miny IS NULL OR extent_maxx IS NULL OR extent_maxy IS NULL "
    "UNION SELECT 'raster', coverage_name FROM raster_coverages_srid "
    "WHERE extent_minx IS NULL OR extent_miny IS NULL OR extent_maxx IS NULL OR extent_maxy IS NULL";

  int ret = sqlite3_get_table(Parent->GetSqlite(), sql, &results,
                              &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    goto end;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *type = results[(i * columns) + 0];
          const char *name = results[(i * columns) + 1];
          list->Add(type, name);
        }
    }
  sqlite3_free_table(results);

end:
  if (list->GetFirst() == NULL)
    {
      delete list;
      list = NULL;
    }
  return list;
}

void MyMapPanel::OnAddLayer(wxCommandEvent & WXUNUSED(event))
{
//
// adding a Map Layer
//
  IncompleteLayersList *list = CheckIncompleteLayers();
  if (list != NULL)
    {
      int ret = wxMessageBox(wxT("Incomplete Layer definitions were found.\n\n"
                                 "These Layers will not be displayed on the Map,\nbecause their Extents are still undefined.\n\n"
                                 "Do you want to fix them all ?\n"
                                 "(computing missing Extents may take some time)"),
                             wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION,
                             this);
      if (ret == wxYES)
        {
          ::wxBeginBusyCursor();
          IncompleteLayer *pN = list->GetFirst();
          while (pN != NULL)
            {
              char *sql;
              if (strcmp(pN->GetType(), "vector") == 0)
                {
                  sql =
                    sqlite3_mprintf
                    ("SELECT SE_UpdateVectorCoverageExtent(%Q, 1)",
                     pN->GetCoverageName());
                  sqlite3_exec(Parent->GetSqlite(), sql, NULL, NULL, NULL);
                  sqlite3_free(sql);
                }
              if (strcmp(pN->GetType(), "raster") == 0)
                {
                  sql =
                    sqlite3_mprintf
                    ("SELECT SE_UpdateRasterCoverageExtent(%Q, 1)",
                     pN->GetCoverageName());
                  sqlite3_exec(Parent->GetSqlite(), sql, NULL, NULL, NULL);
                  sqlite3_free(sql);
                }
              pN = pN->GetNext();
            }
          ::wxEndBusyCursor();
        }
      delete list;
    }
  AddMapLayerDialog dlg;
  dlg.Create(this);
  int ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      MapView->PrepareMap();
    }
}

void MyMapPanel::OnSqlLayer(wxCommandEvent & WXUNUSED(event))
{
//
// current map click is: Query Table
//
  char *sql = NULL;
  char *db_prefix = NULL;
  MapLayer *layer = MapView->GetActiveLayer();
  if (layer == NULL)
    return;

  if (layer->GetType() == MAP_LAYER_VECTOR)
    {
      db_prefix = (char *) malloc(layer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
      sql =
        Parent->MapAuxPrepareSqlQuery(db_prefix, layer->GetTableName(),
                                      "ROWID");
    }
  if (layer->GetType() == MAP_LAYER_VECTOR_VIEW)
    {
      db_prefix = (char *) malloc(layer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
      sql =
        Parent->MapAuxPrepareSqlQuery(db_prefix, layer->GetTableName(),
                                      layer->GetViewRowidColumn());
    }
  if (layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    {
      db_prefix = (char *) malloc(layer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
      sql =
        Parent->MapAuxPrepareSqlQuery(db_prefix, layer->GetTableName(),
                                      "ROWID");
    }
  if (db_prefix != NULL)
    free(db_prefix);
  if (sql != NULL)
    {
      wxString table = wxString::FromUTF8(layer->GetTableName());
      wxString geom = wxString::FromUTF8(layer->GetGeometryColumn());
      InitializeSqlFilters(layer->GetDbPrefix(), table, true, geom);
      wxString sqlstr = wxString::FromUTF8(sql);
      sqlite3_free(sql);
      SetSql(sqlstr, true);
      Parent->GetQueryView()->EnableFilterButton();
    }

/*
  sql = wxT("SELECT * FROM \"");
  wxString dbAlias = layer->GetDbPrefix();
  MyFrame::DoubleQuoted(dbAlias);
  sql += dbAlias + wxT("\".\"");
  xname = gaiaDoubleQuotedSql(layer->GetTableName());
  sql += wxString::FromUTF8(xname);
  free(xname);
  sql += wxT("\"");
  wxString table = wxString::FromUTF8(layer->GetTableName());
  wxString geom = wxString::FromUTF8(layer->GetGeometryColumn());
  Parent->InitializeSqlFilters(layer->GetDbPrefix(), table, true, geom);
  Parent->SetSql(sql, true, false, dummy, dummy, true);
  Parent->GetQueryView()->EnableFilterButton();
  */
}

void MyMapPanel::OnIdentify(wxCommandEvent & WXUNUSED(event))
{
//
// current map click is: Identify
//
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  if (IsIdentify == false)
    {
      if (menuBar)
        menuBar->Check(ID_Identify, true);
      if (toolBar)
        toolBar->ToggleTool(ID_Identify, true);
      IsIdentify = true;
      IsZoomIn = false;
      IsZoomOut = false;
      IsPan = false;
      UpdateCurrentMapMode();
    }
}

void MyMapPanel::OnZoomIn(wxCommandEvent & WXUNUSED(event))
{
//
// current map click is: ZoomIn
//
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  if (IsZoomIn == false)
    {
      menuBar->Check(ID_ZoomIn, true);
      if (toolBar)
        toolBar->ToggleTool(ID_ZoomIn, true);
      IsIdentify = false;
      IsZoomIn = true;
      IsZoomOut = false;
      IsPan = false;
      UpdateCurrentMapMode();
    }
}

void MyMapPanel::OnZoomOut(wxCommandEvent & WXUNUSED(event))
{
//
// current map click is: ZoomOut
//
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  if (IsZoomOut == false)
    {
      if (menuBar)
        menuBar->Check(ID_ZoomOut, true);
      if (toolBar)
        toolBar->ToggleTool(ID_ZoomOut, true);
      IsIdentify = false;
      IsZoomIn = false;
      IsZoomOut = true;
      IsPan = false;
      UpdateCurrentMapMode();
    }
}

void MyMapPanel::UpdateCurrentMapMode()
{
// updating the current Mode on the Status Bar
  wxString mode;
  if (IsIdentify == true)
    {
      mode = wxT("Mode: IDENTIFY");
      UpdateMapMode(mode);
      return;
    }
  if (IsZoomIn == true)
    {
      mode = wxT("Mode: ZOOM IN");
      UpdateMapMode(mode);
      return;
    }
  if (IsZoomOut == true)
    {
      mode = wxT("Mode: ZOOM OUT");
      UpdateMapMode(mode);
      return;
    }
  if (IsPan == true)
    {
      mode = wxT("Mode: PAN");
      UpdateMapMode(mode);
      return;
    }
}

void MyMapPanel::OnMapImage(wxCommandEvent & WXUNUSED(event))
{
//
// Map Image
//
  MapImageDialog dlg;
  dlg.Create(this);
  dlg.ShowModal();
}

void MyMapPanel::OnPrinter(wxCommandEvent & WXUNUSED(event))
{
//
// Printer
//
  PrinterDialog dlg;
  dlg.Create(this);
  dlg.ShowModal();
}

void MyMapPanel::OnPan(wxCommandEvent & WXUNUSED(event))
{
//
// current map click is: Pan
//
  wxMenuBar *menuBar = GetMenuBar();
  wxToolBar *toolBar = GetToolBar();
  if (IsPan == false)
    {
      if (menuBar)
        menuBar->Check(ID_Pan, true);
      if (toolBar)
        toolBar->ToggleTool(ID_Pan, true);
      IsIdentify = false;
      IsZoomIn = false;
      IsZoomOut = false;
      IsPan = true;
      UpdateCurrentMapMode();
    }
}

void MyMapPanel::OnAbout(wxCommandEvent & WXUNUSED(event))
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
  strcpy(ver, spatialite_target_cpu());
  str += wxT("Target CPU ") + wxString::FromUTF8(ver) + wxT("\n");
  dlg.SetDescription(str);
  dlg.SetCopyright(wxT("by Alessandro Furieri - 2008/2014"));
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

void MyMapPanel::ParentQuit()
{
//
// the Parent is quitting
//
  Parent = NULL;
  Close(true);
}

void MyMapPanel::DetachAll()
{
//
// DETACHing all DATABASEs
//
  if (MapView != NULL)
    MapView->DetachAll();
}

char *MyMapPanel::DoFetchRefSysName()
{
//
// fetching the RefSys Name
//
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value = NULL;
  char *ref_sys_name;
  char *sql =
    sqlite3_mprintf("SELECT ref_sys_name FROM spatial_ref_sys WHERE srid = %d",
                    GetMapSRID());
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      sqlite3_free(errMsg);
      ref_sys_name = (char *) malloc(8);
      strcpy(ref_sys_name, "Unknown");
      return ref_sys_name;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        value = results[(i * columns) + 0];
    }
  if (value != NULL)
    {
      int len = strlen(value);
      ref_sys_name = (char *) malloc(len + 1);
      strcpy(ref_sys_name, value);
  } else
    {
      ref_sys_name = (char *) malloc(8);
      strcpy(ref_sys_name, "Unknown");
    }
  sqlite3_free_table(results);
  return ref_sys_name;
}

void MyMapPanel::UpdateMapSRID()
{
//
// updating the status bar - Map SRID
//
  if (GetStatusBar() == NULL)
    return;
  char *ref_sys_name = DoFetchRefSysName();
  char *srid = sqlite3_mprintf("SRID: %d [%s]", GetMapSRID(), ref_sys_name);
  free(ref_sys_name);
  wxString str = wxString::FromUTF8(srid);
  sqlite3_free(srid);
  GetStatusBar()->SetStatusText(str, 0);
}

void MyMapPanel::UpdateMaxThreads()
{
//
// Updating MaxThreads
//
  char *sql;
  int ret;
  char *errMsg = NULL;

  if (Parent->IsMapMultiThreadingEnabled() == false)
    Parent->SetMapMaxThreads(1);

  sql =
    sqlite3_mprintf("SELECT RL2_SetMaxThreads(%d)", Parent->GetMapMaxThreads());
  ret = sqlite3_exec(Parent->GetSqlite(), sql, NULL, 0, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("UpdateMaxThreads: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
    }
}

void MyMapPanel::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// EXIT - event handler
//
  Close(true);
}

void MyMapPanel::UpdateMapScale()
{
//
// updating the status bar - Current Scale
//
  if (GetStatusBar() == NULL)
    return;
  wxString scale;
  if (MapView->IsValidMap())
    {
      char dummy[256];
      sprintf(dummy, "Scale 1:%d", MapView->GetCurrentScale());
      scale = wxString::FromUTF8(dummy);
    }
  GetStatusBar()->SetStatusText(scale, 1);
}

void MyMapPanel::UpdateMapCoords(wxString & coords)
{
//
// updating the status bar [Coordinates]
//
  if (GetStatusBar() == NULL)
    return;
  GetStatusBar()->SetStatusText(coords, 2);
}

void MyMapPanel::UpdateMapMode(wxString & mode)
{
//
// updating the status bar [current Mode]
//
  if (GetStatusBar() == NULL)
    return;
  GetStatusBar()->SetStatusText(mode, 4);
}

bool MyMapPanel::IsGeographicSRID(int srid)
{
// testing if some SRID is of the Geographic [Long/Lat] type
  int geographic = 0;
  if (!srid_is_geographic(GetSqlite(), srid, &geographic))
    geographic = 0;
  if (geographic)
    return true;
  return false;
}

void MyBitmapCellRenderer::Draw(wxGrid & grid, wxGridCellAttr & attr,
                                wxDC & dc, const wxRect & rect, int row,
                                int col, bool isSelected)
{
// drawing a Graphic cell
  if (col == row && isSelected)
    row = col;                  // silencing stupid compiler warnings about unused args

  wxBitmap bmp = wxBitmap(Graphic);
  wxColour color = attr.GetBackgroundColour();
  if (grid.IsEnabled() == false)
    {
      color = wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
      wxImage img = Graphic.ConvertToGreyscale();
      bmp = wxBitmap(img);
    }
  dc.SetBrush(wxBrush(color));
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.DrawRectangle(rect);
  int x = (rect.GetWidth() - bmp.GetWidth()) / 2;
  int y = (rect.GetHeight() - bmp.GetHeight()) / 2;
  dc.DrawBitmap(bmp, rect.x + x, rect.y + y, true);
}

bool AddMapLayerDialog::Create(MyMapPanel * parent)
{
//
// creating the dialog
//
  MapPanel = parent;
  NumRows = 0;
  First = NULL;
  Last = NULL;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Add Map Layer(s)")) == false)
    return false;
// populates individual controls
  DoLoadVectorCoverages();
  DoLoadTopologyCoverages();
  DoLoadNetworkCoverages();
  DoLoadRasterCoverages();
  DoLoadWmsCoverages();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void AddMapLayerDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: Layers list
  wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(listSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  Layers =
    new wxGrid(this, ID_LAYERS_LIST, wxDefaultPosition, wxSize(700, 400));
  NumRows = 0;
  LayerListItem *pL = First;
  while (pL)
    {
      // counting how many layers are there
      NumRows++;
      pL = pL->GetNext();
    }
  Layers->CreateGrid(NumRows, 7, wxGrid::wxGridSelectRows);
  Layers->SetColLabelValue(0, wxT("DB"));
  Layers->SetColLabelValue(1, wxT("Type"));
  Layers->SetColLabelValue(2, wxT("Name"));
  Layers->SetColLabelValue(3, wxT("Title"));
  Layers->SetColLabelValue(4, wxT("Abstract"));
  Layers->SetColLabelValue(5, wxT("Copyright"));
  Layers->SetColLabelValue(6, wxT("Data License"));
  int count = 0;
  wxString cell;
  pL = First;
  while (pL)
    {
      // feeding grid rows
      wxColour color(0, 0, 0);
      if (MapPanel->IsAlreadyDefined(pL) == true)
        color = wxColour(192, 192, 192);
      MyBitmapCellRenderer *renderer = new MyBitmapCellRenderer;
      wxBitmap bmp;
      switch (pL->GetLayerType())
        {
          case MAP_LAYER_VECTOR:
          case MAP_LAYER_VECTOR_VIEW:
          case MAP_LAYER_VECTOR_VIRTUAL:
            switch (pL->GetGeometryType())
              {
                case 1:
                case 1001:
                case 2001:
                case 3001:
                case 4:
                case 1004:
                case 2004:
                case 3004:
                  bmp = wxBitmap(points_on_xpm);
                  break;
                case 2:
                case 1002:
                case 2002:
                case 3002:
                case 5:
                case 1005:
                case 2005:
                case 3005:
                  bmp = wxBitmap(linestrings_on_xpm);
                  break;
                case 3:
                case 1003:
                case 2003:
                case 3003:
                case 6:
                case 1006:
                case 2006:
                case 3006:
                  bmp = wxBitmap(polygons_on_xpm);
                  break;
                default:
                  bmp = wxBitmap(vector_xpm);
                  break;
              }
            break;
          case MAP_LAYER_TOPOLOGY:
            bmp = wxBitmap(topo_geo_xpm);
            break;
          case MAP_LAYER_NETWORK:
            bmp = wxBitmap(topo_net_xpm);
            break;
          case MAP_LAYER_RASTER:
            bmp = wxBitmap(coverage_xpm);
            break;
          case MAP_LAYER_WMS:
            bmp = wxBitmap(wms_xpm);
            break;
          default:
            pL = pL->GetNext();
            continue;
        };
      renderer->SetGraphic(bmp);
      Layers->SetCellValue(count, 0, pL->GetDbPrefix());
      Layers->SetCellTextColour(count, 0, color);
      Layers->SetCellRenderer(count, 1, renderer);
      if (pL->GetLayerType() == MAP_LAYER_VECTOR
          || pL->GetLayerType() == MAP_LAYER_VECTOR_VIEW
          || pL->GetLayerType() == MAP_LAYER_VECTOR_VIRTUAL)
        {
          wxString prefixName = pL->GetLayerPrefix();
          prefixName += wxT(".");
          prefixName += pL->GetLayerName();
          Layers->SetCellValue(count, 2, prefixName);
      } else
        Layers->SetCellValue(count, 2, pL->GetLayerName());
      Layers->SetCellTextColour(count, 2, color);
      Layers->SetCellValue(count, 3, pL->GetTitle());
      Layers->SetCellTextColour(count, 3, color);
      wxString abstract;
      DoSplitMultilines(pL->GetAbstract(), abstract);
      Layers->SetCellValue(count, 4, abstract);
      Layers->SetCellTextColour(count, 4, color);
      wxString copyright;
      DoSplitMultilines(pL->GetCopyright(), copyright);
      Layers->SetCellValue(count, 5, copyright);
      Layers->SetCellTextColour(count, 5, color);
      Layers->SetCellValue(count, 6, pL->GetDataLicense());
      Layers->SetCellTextColour(count, 6, color);
      count++;
      pL = pL->GetNext();
    }
  Layers->SetRowLabelSize(wxGRID_AUTOSIZE);
  Layers->AutoSize();
  Layers->EnableEditing(false);
  listSizer->Add(Layers, 0, wxALIGN_RIGHT | wxALL, 5);
// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&OK"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & AddMapLayerDialog::OnOk);
}

void AddMapLayerDialog::FlushList()
{
// resetting the Layers List
  LayerListItem *pL;
  LayerListItem *pLn;
  pL = First;
  while (pL)
    {
      pLn = pL->GetNext();
      delete pL;
      pL = pLn;
    }
  First = NULL;
  Last = NULL;
}

void AddMapLayerDialog::DoSplitMultilines(wxString & in, wxString & out)
{
// attempting to split a single line into a multiline
  int len = in.Len() * 4;
  char *inbuf = (char *) malloc(len + 1);
  strcpy(inbuf, in.ToUTF8());
  char *outbuf = (char *) malloc((len * 2) + 1);
  char *p_in = inbuf;
  char *p_out = outbuf;
  len = 0;
  while (1)
    {
      char c = *p_in++;
      if (c == '\0')
        {
          *p_out = '\0';
          break;
        }
      if (c == ' ' && len > 30)
        {
          *p_out++ = '\n';
          len = 0;
          continue;
        }
      *p_out++ = c;
      len++;
      if (c == '\n')
        len = 0;
    }
  out = wxString::FromUTF8(outbuf);
  free(inbuf);
  free(outbuf);
}

LayerListItem::LayerListItem(const void *priv_data, wxString & db_prefix,
                             wxString & name, wxString & title,
                             wxString & abstract, wxString & copyright,
                             wxString & data_license, bool queryable, int srid)
{
// ctor - WMS Layer
  PrivData = priv_data;
  DbPrefix = db_prefix;
  LayerType = MAP_LAYER_WMS;
  LayerName = name;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = data_license;
  f_table_name = NULL;
  f_geometry_column = NULL;
  view_table_name = NULL;
  view_geometry_column = NULL;
  view_rowid_column = NULL;
  topology_name = NULL;
  network_name = NULL;
  GeometryType = -1;
  HasZ = false;
  NativeSRID = srid;
  Queryable = queryable;
  Editable = false;
  SpatialIndex = false;
  Next = NULL;
}

LayerListItem::LayerListItem(const void *priv_data, wxString & db_prefix,
                             wxString & name, wxString & title,
                             wxString & abstract, wxString & copyright,
                             wxString & data_license, int srid, bool queryable)
{
// ctor - Raster Layer
  PrivData = priv_data;
  DbPrefix = db_prefix;
  LayerType = MAP_LAYER_RASTER;
  LayerName = name;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = data_license;
  f_table_name = NULL;
  f_geometry_column = NULL;
  view_table_name = NULL;
  view_geometry_column = NULL;
  view_rowid_column = NULL;
  topology_name = NULL;
  network_name = NULL;
  GeometryType = -1;
  HasZ = false;
  NativeSRID = srid;
  Queryable = queryable;
  Editable = false;
  SpatialIndex = false;
  Next = NULL;
}

LayerListItem::LayerListItem(const void *priv_data, wxString & db_prefix,
                             wxString & layer_prefix, wxString & name,
                             wxString & title, wxString & abstract,
                             wxString & copyright, wxString & data_license,
                             const char *table_name,
                             const char *geometry_column, int geom_type,
                             int srid, bool queryable, bool editable,
                             bool spatial_index, const char *mother_table_name,
                             const char *mother_geometry_column,
                             const char *mother_rowid_column)
{
// ctor - Vector Layer
  int len;
  PrivData = priv_data;
  DbPrefix = db_prefix;
  LayerPrefix = layer_prefix;
  LayerType = MAP_LAYER_VECTOR;
  if (LayerPrefix.Cmp(wxT("view")) == 0)
    LayerType = MAP_LAYER_VECTOR_VIEW;
  if (LayerPrefix.Cmp(wxT("shp")) == 0)
    LayerType = MAP_LAYER_VECTOR_VIRTUAL;
  LayerName = name;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = data_license;
  len = strlen(table_name);
  f_table_name = (char *) malloc(len + 1);
  strcpy(f_table_name, table_name);
  len = strlen(geometry_column);
  f_geometry_column = (char *) malloc(len + 1);
  strcpy(f_geometry_column, geometry_column);
  if (mother_table_name == NULL)
    view_table_name = NULL;
  else
    {
      len = strlen(mother_table_name);
      view_table_name = (char *) malloc(len + 1);
      strcpy(view_table_name, mother_table_name);
    }
  if (mother_geometry_column == NULL)
    view_geometry_column = NULL;
  else
    {
      len = strlen(mother_geometry_column);
      view_geometry_column = (char *) malloc(len + 1);
      strcpy(view_geometry_column, mother_geometry_column);
    }
  if (mother_rowid_column == NULL)
    view_rowid_column = NULL;
  else
    {
      len = strlen(mother_rowid_column);
      view_rowid_column = (char *) malloc(len + 1);
      strcpy(view_rowid_column, mother_rowid_column);
    }
  topology_name = NULL;
  network_name = NULL;
  GeometryType = geom_type;
  HasZ = false;
  NativeSRID = srid;
  Queryable = queryable;
  Editable = editable;
  SpatialIndex = spatial_index;
  Next = NULL;
}

LayerListItem::LayerListItem(const void *priv_data, wxString & db_prefix,
                             int type, wxString & name, wxString & title,
                             wxString & abstract, wxString & copyright,
                             wxString & data_license, const char *topo_name,
                             bool has_z, int srid, bool queryable,
                             bool editable)
{
// ctor - TopoGeo or TopoNet Layer
  int len;
  PrivData = priv_data;
  DbPrefix = db_prefix;
  if (type == MAP_LAYER_TOPOLOGY || type == MAP_LAYER_NETWORK)
    LayerType = type;
  else
    LayerType = MAP_LAYER_UNKNOWN;
  LayerName = name;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = data_license;
  f_table_name = NULL;
  f_geometry_column = NULL;
  view_table_name = NULL;
  view_geometry_column = NULL;
  view_rowid_column = NULL;
  if (type == MAP_LAYER_TOPOLOGY)
    {
      len = strlen(topo_name);
      topology_name = (char *) malloc(len + 1);
      strcpy(topology_name, topo_name);
  } else
    topology_name = NULL;
  if (type == MAP_LAYER_NETWORK)
    {
      len = strlen(topo_name);
      network_name = (char *) malloc(len + 1);
      strcpy(network_name, topo_name);
  } else
    network_name = NULL;
  GeometryType = -1;
  HasZ = has_z;
  NativeSRID = srid;
  Queryable = queryable;
  Editable = editable;
  SpatialIndex = false;
  Next = NULL;
}

LayerListItem::~LayerListItem()
{
// dtor
  if (f_table_name != NULL)
    free(f_table_name);
  if (f_geometry_column != NULL)
    free(f_geometry_column);
  if (view_table_name != NULL)
    free(view_table_name);
  if (view_geometry_column != NULL)
    free(view_geometry_column);
  if (view_rowid_column != NULL)
    free(view_rowid_column);
  if (topology_name != NULL)
    free(topology_name);
  if (network_name != NULL)
    free(network_name);
}

void AddMapLayerDialog::AddLayer2List(wxString & db_prefix, wxString & name,
                                      wxString & title, wxString & abstract,
                                      wxString & copyright,
                                      wxString & data_license, bool queryable,
                                      int srid)
{
// adding a layer item to the list - WMS Layer
  LayerListItem *pL =
    new LayerListItem(MapPanel->GetParent()->GetRL2PrivateData(), db_prefix,
                      name, title, abstract, copyright,
                      data_license, queryable, srid);
  if (First == NULL)
    First = pL;
  if (Last != NULL)
    Last->SetNext(pL);
  Last = pL;
}

void AddMapLayerDialog::AddLayer2List(wxString & db_prefix, wxString & name,
                                      wxString & title, wxString & abstract,
                                      wxString & copyright,
                                      wxString & data_license, int srid,
                                      bool queryable)
{
// adding a layer item to the list - Raster Layer
  LayerListItem *pL =
    new LayerListItem(MapPanel->GetParent()->GetRL2PrivateData(), db_prefix,
                      name, title, abstract, copyright,
                      data_license, srid, queryable);
  if (First == NULL)
    First = pL;
  if (Last != NULL)
    Last->SetNext(pL);
  Last = pL;
}

void AddMapLayerDialog::AddLayer2List(wxString & db_prefix,
                                      wxString & layer_prefix, wxString & name,
                                      wxString & title, wxString & abstract,
                                      wxString & copyright,
                                      wxString & data_license,
                                      const char *f_table_name,
                                      const char *f_geometry_column,
                                      int geom_type, int srid, bool queryable,
                                      bool editable, bool spatial_index,
                                      const char *view_table_name,
                                      const char *view_geometry_column,
                                      const char *view_rowid_column)
{
// adding a layer item to the list - Vector Layer
  LayerListItem *pL =
    new LayerListItem(MapPanel->GetParent()->GetRL2PrivateData(), db_prefix,
                      layer_prefix, name, title, abstract, copyright,
                      data_license,
                      f_table_name, f_geometry_column,
                      geom_type, srid, queryable, editable, spatial_index,
                      view_table_name, view_geometry_column, view_rowid_column);
  if (First == NULL)
    First = pL;
  if (Last != NULL)
    Last->SetNext(pL);
  Last = pL;
}

void AddMapLayerDialog::AddLayer2List(wxString & db_prefix, int layer_type,
                                      wxString & name, wxString & title,
                                      wxString & abstract, wxString & copyright,
                                      wxString & data_license,
                                      const char *topo_name, bool has_z,
                                      int srid, bool queryable, bool editable)
{
// adding a layer item to the list - TopoGeo/TopoNet Layer
  LayerListItem *pL =
    new LayerListItem(MapPanel->GetParent()->GetRL2PrivateData(), db_prefix,
                      layer_type, name, title, abstract, copyright,
                      data_license, topo_name,
                      has_z, srid,
                      queryable, editable);
  if (First == NULL)
    First = pL;
  if (Last != NULL)
    Last->SetNext(pL);
  Last = pL;
}

bool AddMapLayerDialog::DoCheckGeometryColumns(const char *db_prefix)
{
// checking GEOMETRY_COLUMNS for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_table_name = false;
  bool ok_geometry_column = false;
  bool ok_geometry_type = false;
  bool ok_srid = false;
  bool ok_spatial_index = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql =
    sqlite3_mprintf("PRAGMA \"%s\".table_info(geometry_columns)", qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "f_table_name") == 0)
            ok_table_name = true;
          if (strcasecmp(value, "f_geometry_column") == 0)
            ok_geometry_column = true;
          if (strcasecmp(value, "geometry_type") == 0)
            ok_geometry_type = true;
          if (strcasecmp(value, "srid") == 0)
            ok_srid = true;
          if (strcasecmp(value, "spatial_index_enabled") == 0)
            ok_spatial_index = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_table_name && ok_geometry_column && ok_geometry_type && ok_srid
      && ok_spatial_index)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckViewsGeometryColumns(const char *db_prefix)
{
// checking VIEWS_GEOMETRY_COLUMNS for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_view_name = false;
  bool ok_view_geometry = false;
  bool ok_view_rowid = false;
  bool ok_f_table_name = false;
  bool ok_f_geometry_column = false;
  bool ok_read_only = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql =
    sqlite3_mprintf("PRAGMA \"%s\".table_info(views_geometry_columns)",
                    qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "view_name") == 0)
            ok_view_name = true;
          if (strcasecmp(value, "view_geometry") == 0)
            ok_view_geometry = true;
          if (strcasecmp(value, "view_rowid") == 0)
            ok_view_rowid = true;
          if (strcasecmp(value, "f_table_name") == 0)
            ok_f_table_name = true;
          if (strcasecmp(value, "f_geometry_column") == 0)
            ok_f_geometry_column = true;
          if (strcasecmp(value, "read_only") == 0)
            ok_read_only = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_view_name && ok_view_geometry && ok_view_rowid && ok_f_table_name
      && ok_f_geometry_column && ok_read_only)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckVirtsGeometryColumns(const char *db_prefix)
{
// checking VIRTS_GEOMETRY_COLUMNS for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_virt_name = false;
  bool ok_virt_geometry = false;
  bool ok_geometry_type = false;
  bool ok_srid = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql =
    sqlite3_mprintf("PRAGMA \"%s\".table_info(virts_geometry_columns)",
                    qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "virt_name") == 0)
            ok_virt_name = true;
          if (strcasecmp(value, "virt_geometry") == 0)
            ok_virt_geometry = true;
          if (strcasecmp(value, "geometry_type") == 0)
            ok_geometry_type = true;
          if (strcasecmp(value, "srid") == 0)
            ok_srid = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_virt_name && ok_virt_geometry && ok_geometry_type && ok_srid)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckVectorCoverages(const char *db_prefix)
{
// checking VECTOR_COVERAGES for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_coverage_name = false;
  bool ok_table_name = false;
  bool ok_geometry_column = false;
  bool ok_topology_name = false;
  bool ok_network_name = false;
  bool ok_view_name = false;
  bool ok_view_geometry = false;
  bool ok_virt_name = false;
  bool ok_virt_geometry = false;
  bool ok_geo_minx = false;
  bool ok_geo_miny = false;
  bool ok_geo_maxx = false;
  bool ok_geo_maxy = false;
  bool ok_ext_minx = false;
  bool ok_ext_miny = false;
  bool ok_ext_maxx = false;
  bool ok_ext_maxy = false;
  bool ok_title = false;
  bool ok_abstract = false;
  bool ok_queryable = false;
  bool ok_editable = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql =
    sqlite3_mprintf("PRAGMA \"%s\".table_info(vector_coverages)", qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "coverage_name") == 0)
            ok_coverage_name = true;
          if (strcasecmp(value, "f_table_name") == 0)
            ok_table_name = true;
          if (strcasecmp(value, "f_geometry_column") == 0)
            ok_geometry_column = true;
          if (strcasecmp(value, "topology_name") == 0)
            ok_topology_name = true;
          if (strcasecmp(value, "network_name") == 0)
            ok_network_name = true;
          if (strcasecmp(value, "view_name") == 0)
            ok_view_name = true;
          if (strcasecmp(value, "view_geometry") == 0)
            ok_view_geometry = true;
          if (strcasecmp(value, "virt_name") == 0)
            ok_virt_name = true;
          if (strcasecmp(value, "virt_geometry") == 0)
            ok_virt_geometry = true;
          if (strcasecmp(value, "geo_minx") == 0)
            ok_geo_minx = true;
          if (strcasecmp(value, "geo_miny") == 0)
            ok_geo_miny = true;
          if (strcasecmp(value, "geo_maxx") == 0)
            ok_geo_maxx = true;
          if (strcasecmp(value, "geo_maxy") == 0)
            ok_geo_maxy = true;
          if (strcasecmp(value, "extent_minx") == 0)
            ok_ext_minx = true;
          if (strcasecmp(value, "extent_miny") == 0)
            ok_ext_miny = true;
          if (strcasecmp(value, "extent_maxx") == 0)
            ok_ext_maxx = true;
          if (strcasecmp(value, "extent_maxy") == 0)
            ok_ext_maxy = true;
          if (strcasecmp(value, "title") == 0)
            ok_title = true;
          if (strcasecmp(value, "abstract") == 0)
            ok_abstract = true;
          if (strcasecmp(value, "is_queryable") == 0)
            ok_queryable = true;
          if (strcasecmp(value, "is_editable") == 0)
            ok_editable = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_coverage_name && ok_table_name && ok_geometry_column
      && ok_topology_name && ok_network_name && ok_view_name && ok_view_geometry
      && ok_virt_name && ok_virt_geometry && ok_geo_minx && ok_geo_miny
      && ok_geo_maxx && ok_geo_maxy && ok_ext_minx && ok_ext_miny && ok_ext_maxx
      && ok_ext_maxy && ok_title && ok_abstract && ok_queryable && ok_editable)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckTopologies(const char *db_prefix)
{
// checking TOPOLOGIES for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_topology_name = false;
  bool ok_srid = false;
  bool ok_has_z = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql = sqlite3_mprintf("PRAGMA \"%s\".table_info(topologies)", qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "topology_name") == 0)
            ok_topology_name = true;
          if (strcasecmp(value, "srid") == 0)
            ok_srid = true;
          if (strcasecmp(value, "has_z") == 0)
            ok_has_z = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_topology_name && ok_srid && ok_has_z)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckNetworks(const char *db_prefix)
{
// checking NETWORKS for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_network_name = false;
  bool ok_srid = false;
  bool ok_has_z = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql = sqlite3_mprintf("PRAGMA \"%s\".table_info(networks)", qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "network_name") == 0)
            ok_network_name = true;
          if (strcasecmp(value, "srid") == 0)
            ok_srid = true;
          if (strcasecmp(value, "has_z") == 0)
            ok_has_z = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_network_name && ok_srid && ok_has_z)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckRasterCoverages(const char *db_prefix)
{
// checking RASTER_COVERAGES for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_coverage_name = false;
  bool ok_title = false;
  bool ok_abstract = false;
  bool ok_queryable = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql =
    sqlite3_mprintf("PRAGMA \"%s\".table_info(raster_coverages)", qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "coverage_name") == 0)
            ok_coverage_name = true;
          if (strcasecmp(value, "title") == 0)
            ok_title = true;
          if (strcasecmp(value, "abstract") == 0)
            ok_abstract = true;
          if (strcasecmp(value, "is_queryable") == 0)
            ok_queryable = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_coverage_name && ok_title && ok_abstract && ok_queryable)
    return true;
  return false;
}

bool AddMapLayerDialog::DoCheckWmsCoverages(const char *db_prefix)
{
// checking WMS_GETMAP for validity
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool ok_layer_name = false;
  bool ok_title = false;
  bool ok_abstract = false;
  bool ok_queryable = false;
  char *qprefix = gaiaDoubleQuotedSql(db_prefix);
  char *sql = sqlite3_mprintf("PRAGMA \"%s\".table_info(wms_getmap)", qprefix);
  free(qprefix);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          value = results[(i * columns) + 1];
          if (strcasecmp(value, "layer_name") == 0)
            ok_layer_name = true;
          if (strcasecmp(value, "title") == 0)
            ok_title = true;
          if (strcasecmp(value, "abstract") == 0)
            ok_abstract = true;
          if (strcasecmp(value, "is_queryable") == 0)
            ok_queryable = true;
        }
    }
  sqlite3_free_table(results);
  if (ok_layer_name && ok_title && ok_abstract && ok_queryable)
    return true;
  return false;
}

char *AddMapLayerDialog::QueryVectorCoverages(const char *db_prefix,
                                              const char *old_sql)
{
// GeoTable Vector Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckVectorCoverages(db_prefix) == false
      || DoCheckGeometryColumns(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }

  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, 'table' AS prefix, "
                            "v.coverage_name, v.title, v.abstract, v.copyright, l.name, "
                            "v.is_queryable, v.is_editable, v.f_table_name, "
                            "v.f_geometry_column, g.srid, g.geometry_type, "
                            "g.spatial_index_enabled, NULL, NULL, NULL "
                            "FROM \"%s\".vector_coverages AS v "
                            "JOIN \"%s\".geometry_columns AS g ON (v.f_table_name = g.f_table_name AND "
                            "v.f_geometry_column = g.f_geometry_column) "
                            "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                            "WHERE v.topology_name IS NULL AND v.network_name IS NULL AND "
                            "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                            "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                            "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                            "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                            db_prefix, qprefix, qprefix, qprefix);
  } else
    {
      sql =
        sqlite3_mprintf("%s\nUNION\nSELECT %Q AS db_prefix, 'table' AS prefix, "
                        "v.coverage_name, v.title, v.abstract, v.copyright, l.name, "
                        "v.is_queryable, v.is_editable, v.f_table_name, "
                        "v.f_geometry_column, g.srid, g.geometry_type, "
                        "g.spatial_index_enabled, NULL, NULL, NULL "
                        "FROM \"%s\".vector_coverages AS v "
                        "JOIN \"%s\".geometry_columns AS g ON (v.f_table_name = g.f_table_name AND "
                        "v.f_geometry_column = g.f_geometry_column) "
                        "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                        "WHERE v.topology_name IS NULL AND v.network_name IS NULL AND "
                        "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                        "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                        "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                        "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                        old_sql, db_prefix, qprefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

char *AddMapLayerDialog::QuerySpatialViewCoverages(const char *db_prefix,
                                                   const char *old_sql)
{
// SpatialView Vector Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckVectorCoverages(db_prefix) == false
      || DoCheckViewsGeometryColumns(db_prefix) == false
      || DoCheckGeometryColumns(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }

  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, 'view' AS prefix, "
                            "v.coverage_name, v.title, v.abstract, v.copyright, l.name, "
                            "v.is_queryable, v.is_editable, v.view_name, v.view_geometry, "
                            "g.srid, g.geometry_type, g.spatial_index_enabled, "
                            "x.f_table_name, x.f_geometry_column, x.view_rowid "
                            "FROM \"%s\".vector_coverages AS v "
                            "JOIN \"%s\".views_geometry_columns AS x ON (v.view_name = x.view_name AND "
                            "v.view_geometry = x.view_geometry) "
                            "JOIN \"%s\".geometry_columns AS g ON (x.f_table_name = g.f_table_name AND "
                            "x.f_geometry_column = g.f_geometry_column) "
                            "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                            "WHERE v.view_name IS NOT NULL AND v.view_geometry IS NOT NULL AND "
                            "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                            "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                            "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                            "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                            db_prefix, qprefix, qprefix, qprefix, qprefix);
  } else
    {
      sql =
        sqlite3_mprintf("%s\nUNION\nSELECT %Q AS db_prefix, 'view' AS prefix, "
                        "v.coverage_name, v.title, v.abstract, v.copyright, l.name, "
                        "v.is_queryable, v.is_editable, v.view_name, v.view_geometry, "
                        "g.srid, g.geometry_type, g.spatial_index_enabled, "
                        "x.f_table_name, x.f_geometry_column, x.view_rowid "
                        "FROM \"%s\".vector_coverages AS v "
                        "JOIN \"%s\".views_geometry_columns AS x ON (v.view_name = x.view_name AND "
                        "v.view_geometry = x.view_geometry) "
                        "JOIN \"%s\".geometry_columns AS g ON (x.f_table_name = g.f_table_name AND "
                        "x.f_geometry_column = g.f_geometry_column) "
                        "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                        "WHERE v.view_name IS NOT NULL AND v.view_geometry IS NOT NULL AND "
                        "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                        "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                        "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                        "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                        old_sql, db_prefix, qprefix, qprefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

char *AddMapLayerDialog::QueryVirtualTableCoverages(const char *db_prefix,
                                                    const char *old_sql)
{
// VirtualShape Vector Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckVectorCoverages(db_prefix) == false
      || DoCheckVirtsGeometryColumns(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }

  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, 'shp' AS prefix, "
                            "v.coverage_name, v.title, v.abstract, v.copyright, l.name, "
                            "v.is_queryable, 0, s.virt_name, s.virt_geometry, s.srid, "
                            "s.geometry_type, 0, NULL, NULL, NULL "
                            "FROM \"%s\".vector_coverages AS v "
                            "JOIN \"%s\".virts_geometry_columns AS s ON (v.virt_name = s.virt_name "
                            "AND v.virt_geometry = s.virt_geometry) "
                            "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                            "WHERE v.virt_name IS NOT NULL AND v.virt_geometry IS NOT NULL AND "
                            "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                            "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                            "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                            "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                            db_prefix, qprefix, qprefix, qprefix);
  } else
    {
      sql =
        sqlite3_mprintf("%s\nUNION\nSELECT %Q AS db_prefix, 'shp' AS prefix, "
                        "v.coverage_name, v.title, v.abstract, v.copyright, l.name, "
                        "v.is_queryable, 0, s.virt_name, s.virt_geometry, s.srid, "
                        "s.geometry_type, 0, NULL, NULL, NULL "
                        "FROM \"%s\".vector_coverages AS v "
                        "JOIN \"%s\".virts_geometry_columns AS s ON (v.virt_name = s.virt_name "
                        "AND v.virt_geometry = s.virt_geometry) "
                        "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                        "WHERE v.virt_name IS NOT NULL AND v.virt_geometry IS NOT NULL AND "
                        "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                        "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                        "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                        "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                        old_sql, db_prefix, qprefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

char *AddMapLayerDialog::QueryTopologyCoverages(const char *db_prefix,
                                                const char *old_sql)
{
// Topology Vector Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckVectorCoverages(db_prefix) == false
      || DoCheckTopologies(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }
  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, v.coverage_name, "
                            "v.title, v.abstract, v.copyright, l.name, "
                            "v.is_queryable, v.is_editable, v.topology_name, t.has_z, t.srid "
                            "FROM \"%s\".vector_coverages AS v "
                            "JOIN \"%s\".topologies AS t ON (v.topology_name = t.topology_name) "
                            "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                            "WHERE v.topology_name IS NOT NULL AND "
                            "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                            "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                            "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                            "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                            db_prefix, qprefix, qprefix, qprefix);
  } else
    {
      sql =
        sqlite3_mprintf
        ("%s\nUNION\nSELECT %Q AS db_prefix, v.coverage_name, "
         "v.title, v.abstract, v.copyright, l.name, "
         "v.is_queryable, v.is_editable, v.topology_name, t.has_z, t.srid "
         "FROM \"%s\".vector_coverages AS v "
         "JOIN \"%s\".topologies AS t ON (v.topology_name = t.topology_name) "
         "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
         "WHERE v.topology_name IS NOT NULL  AND "
         "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
         "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
         "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
         "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", old_sql,
         db_prefix, qprefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

char *AddMapLayerDialog::QueryNetworkCoverages(const char *db_prefix,
                                               const char *old_sql)
{
// Network Vector Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckVectorCoverages(db_prefix) == false
      || DoCheckNetworks(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }

  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, v.coverage_name, "
                            "v.title, v.abstract, v.copyright, l.name, "
                            "v.is_queryable, v.is_editable, v.network_name, n.has_z, n.srid "
                            "FROM \"%s\".vector_coverages AS v "
                            "JOIN \"%s\".networks AS n ON (v.network_name = n.network_name) "
                            "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
                            "WHERE v.network_name IS NOT NULL  AND "
                            "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
                            "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
                            "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
                            "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL",
                            db_prefix, qprefix, qprefix, qprefix);
  } else
    {
      sql =
        sqlite3_mprintf
        ("%s\nUNION\nSELECT %Q AS db_prefix, v.coverage_name, "
         "v.title, v.abstract, v.copyright, l.name, "
         "v.is_queryable, v.is_editable, v.network_name, n.has_z, n.srid "
         "FROM \"%s\".vector_coverages AS v "
         "JOIN \"%s\".networks AS n ON (v.network_name = n.network_name) "
         "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
         "WHERE v.network_name IS NOT NULL  AND "
         "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
         "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
         "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
         "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", old_sql,
         db_prefix, qprefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

char *AddMapLayerDialog::QueryRasterCoverages(const char *db_prefix,
                                              const char *old_sql)
{
// Raster Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckRasterCoverages(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }

  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, "
                            "r.coverage_name, r.title, r.abstract, r.copyright, l.name, "
                            "r.srid, r.is_queryable FROM \"%s\".raster_coverages AS r "
                            "JOIN \"%s\".data_licenses AS l ON (r.license = l.id) "
                            "WHERE r.geo_minx IS NOT NULL AND r.geo_miny IS NOT NULL AND "
                            "r.geo_maxx IS NOT NULL AND r.geo_maxy IS NOT NULL AND "
                            "r.extent_minx IS NOT NULL AND r.extent_miny IS NOT NULL AND "
                            "r.extent_maxx IS NOT NULL AND r.extent_maxy IS NOT NULL",
                            db_prefix, qprefix, qprefix);
  } else
    {
      sql = sqlite3_mprintf("%s\nUNION\nSELECT %Q AS db_prefix, "
                            "r.coverage_name, r.title, r.abstract, r.copyright, l.name, "
                            "r.srid, r.is_queryable FROM \"%s\".raster_coverages AS r "
                            "JOIN \"%s\".data_licenses AS l ON (r.license = l.id) "
                            "WHERE r.geo_minx IS NOT NULL AND r.geo_miny IS NOT NULL AND "
                            "r.geo_maxx IS NOT NULL AND r.geo_maxy IS NOT NULL AND "
                            "r.extent_minx IS NOT NULL AND r.extent_miny IS NOT NULL AND "
                            "r.extent_maxx IS NOT NULL AND r.extent_maxy IS NOT NULL",
                            old_sql, db_prefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

char *AddMapLayerDialog::QueryWmsCoverages(const char *db_prefix,
                                           const char *old_sql)
{
// WMS Coverages - composing the SQL query
  char *sql = NULL;
  char *qprefix;

  if (DoCheckWmsCoverages(db_prefix) == false)
    {
      if (old_sql != NULL)
        return sqlite3_mprintf("%s", old_sql);
      else
        return NULL;
    }

  qprefix = gaiaDoubleQuotedSql(db_prefix);
  if (old_sql == NULL)
    {
      sql = sqlite3_mprintf("SELECT %Q AS db_prefix, "
                            "w.layer_name, w.title, w.abstract, w.copyright, l.name, "
                            "w.srs, w.is_queryable FROM \"%s\".wms_getmap AS w "
                            "JOIN \"%s\".data_licenses AS l ON (w.license = l.id)",
                            db_prefix, qprefix, qprefix);
  } else
    {
      sql = sqlite3_mprintf("%s\nUNION\nSELECT %Q AS db_prefix, "
                            "w.layer_name, w.title, w.abstract, w.copyright, l.name, "
                            "w.srs, w.is_queryable FROM \"%s\".wms_getmap AS w "
                            "JOIN \"%s\".data_licenses AS l ON (w.license = l.id)",
                            old_sql, db_prefix, qprefix, qprefix);
    }
  free(qprefix);
  return sql;
}

void AddMapLayerDialog::DoLoadVectorCoverages()
{
// populating the layers list (Vector Coverages)
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;
  char *sql2 = NULL;
  char *sql_save = NULL;

  sql = "pragma database_list";
  int ret = sqlite3_get_table(sqlite, sql, &results,
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
          value = results[(i * columns) + 1];
          // searching for GeoTables
          sql_save = sql2;
          sql2 = QueryVectorCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
          // searching for SpatialViews
          sql_save = sql2;
          sql2 = QuerySpatialViewCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
          // searching for VirtualShapes
          sql_save = sql2;
          sql2 = QueryVirtualTableCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
        }
    }
  sqlite3_free_table(results);
  if (sql2 == NULL)
    return;

  sql_save = sql2;
  sql2 =
    sqlite3_mprintf("%s\nORDER BY v.coverage_name, db_prefix, prefix",
                    sql_save);
  sqlite3_free(sql_save);
  ret = sqlite3_get_table(sqlite, sql2, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql2);
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
          wxString db_prefix = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString prefix = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 6];
          wxString data_license = wxString::FromUTF8(value);
          bool queryable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            queryable = true;
          bool editable = false;
          value = results[(i * columns) + 8];
          if (atoi(value))
            editable = true;
          const char *f_table_name = results[(i * columns) + 9];
          const char *f_geometry_column = results[(i * columns) + 10];
          value = results[(i * columns) + 11];
          int srid = atoi(value);
          value = results[(i * columns) + 12];
          int geom_type = atoi(value);
          bool spatial_index = false;
          value = results[(i * columns) + 13];
          if (atoi(value) == 1)
            spatial_index = true;
          const char *view_table_name = results[(i * columns) + 14];
          const char *view_geometry_column = results[(i * columns) + 15];
          const char *view_rowid_column = results[(i * columns) + 16];
          AddLayer2List(db_prefix, prefix, name, title, abstract, copyright,
                        data_license, f_table_name, f_geometry_column,
                        geom_type, srid, queryable, editable, spatial_index,
                        view_table_name, view_geometry_column,
                        view_rowid_column);
        }
    }
  sqlite3_free_table(results);
}

void AddMapLayerDialog::DoLoadTopologyCoverages()
{
// populating the layers list (TopoGeo Coverages)
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;
  char *sql2 = NULL;
  char *sql_save = NULL;

  sql = "pragma database_list";
  int ret = sqlite3_get_table(sqlite, sql, &results,
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
          value = results[(i * columns) + 1];
          sql_save = sql2;
          sql2 = QueryTopologyCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
        }
    }
  sqlite3_free_table(results);
  if (sql2 == NULL)
    return;

  sql_save = sql2;
  sql2 = sqlite3_mprintf("%s\nORDER BY v.coverage_name, db_prefix", sql_save);
  sqlite3_free(sql_save);
  ret = sqlite3_get_table(sqlite, sql2, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql2);
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
          wxString db_prefix = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString data_license = wxString::FromUTF8(value);
          bool queryable = false;
          value = results[(i * columns) + 6];
          if (atoi(value))
            queryable = true;
          bool editable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            editable = true;
          const char *topology_name = results[(i * columns) + 8];
          bool has_z = false;
          value = results[(i * columns) + 9];
          if (atoi(value))
            has_z = true;
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          AddLayer2List(db_prefix, MAP_LAYER_TOPOLOGY, name, title, abstract,
                        copyright, data_license, topology_name, has_z, srid,
                        queryable, editable);
        }
    }
  sqlite3_free_table(results);
}

void AddMapLayerDialog::DoLoadNetworkCoverages()
{
// populating the layers list (TopoNet Coverages)
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;
  char *sql2 = NULL;
  char *sql_save = NULL;

  sql = "pragma database_list";
  int ret = sqlite3_get_table(sqlite, sql, &results,
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
          value = results[(i * columns) + 1];
          sql_save = sql2;
          sql2 = QueryNetworkCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
        }
    }
  sqlite3_free_table(results);
  if (sql2 == NULL)
    return;

  sql_save = sql2;
  sql2 = sqlite3_mprintf("%s\nORDER BY v.coverage_name, db_prefix", sql_save);
  sqlite3_free(sql_save);
  ret = sqlite3_get_table(sqlite, sql2, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql2);
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
          wxString db_prefix = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString data_license = wxString::FromUTF8(value);
          bool queryable = false;
          value = results[(i * columns) + 6];
          if (atoi(value))
            queryable = true;
          bool editable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            editable = true;
          const char *network_name = results[(i * columns) + 8];
          bool has_z = false;
          value = results[(i * columns) + 9];
          if (atoi(value))
            has_z = true;
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          AddLayer2List(db_prefix, MAP_LAYER_NETWORK, name, title, abstract,
                        copyright, data_license, network_name, has_z, srid,
                        queryable, editable);
        }
    }
  sqlite3_free_table(results);
}

void AddMapLayerDialog::DoLoadRasterCoverages()
{
// populating the layers list (Raster Coverages)
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;
  char *sql2 = NULL;
  char *sql_save = NULL;

  sql = "pragma database_list";
  int ret = sqlite3_get_table(sqlite, sql, &results,
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
          value = results[(i * columns) + 1];
          sql_save = sql2;
          sql2 = QueryRasterCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
        }
    }
  sqlite3_free_table(results);
  if (sql2 == NULL)
    return;

  sql_save = sql2;
  sql2 = sqlite3_mprintf("%s\nORDER BY coverage_name, db_prefix", sql_save);
  sqlite3_free(sql_save);
  ret = sqlite3_get_table(sqlite, sql2, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql2);
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
          wxString db_prefix = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString data_license = wxString::FromUTF8(value);
          value = results[(i * columns) + 6];
          int srid = atoi(value);
          bool queryable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            queryable = true;
          AddLayer2List(db_prefix, name, title, abstract,
                        copyright, data_license, srid, queryable);
        }
    }
  sqlite3_free_table(results);
}

int AddMapLayerDialog::ParseSRID(const char *srs)
{
//
// setting default SRS for WMS Layer
//
  int srid = -1;
  const char *p = srs;

  if (strncasecmp(srs, "EPSG:", 5) == 0)
    p = srs + 5;
  srid = atoi(p);
  return srid;
}

void AddMapLayerDialog::DoLoadWmsCoverages()
{
// populating the layers list (WMS Coverages)
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  const char *sql;
  char *sql2 = NULL;
  char *sql_save = NULL;

  sql = "pragma database_list";
  int ret = sqlite3_get_table(sqlite, sql, &results,
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
          value = results[(i * columns) + 1];
          sql_save = sql2;
          sql2 = QueryWmsCoverages(value, sql_save);
          if (sql_save != NULL)
            sqlite3_free(sql_save);
        }
    }
  sqlite3_free_table(results);
  if (sql2 == NULL)
    return;

  sql_save = sql2;
  sql2 = sqlite3_mprintf("%s\nORDER BY layer_name, db_prefix", sql_save);
  sqlite3_free(sql_save);
  ret = sqlite3_get_table(sqlite, sql2, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql2);
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
          wxString db_prefix = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          wxString name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString data_license = wxString::FromUTF8(value);
          value = results[(i * columns) + 6];
          int srid = ParseSRID(value);
          bool queryable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            queryable = true;
          AddLayer2List(db_prefix, name, title, abstract,
                        copyright, data_license, queryable, srid);
        }
    }
  sqlite3_free_table(results);
}

void AddMapLayerDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  bool changed = false;
  bool emptyMap = false;
  int i;

  if (MapPanel->GetFirstMapLayer() == NULL)
    emptyMap = true;

  for (i = 0; i < NumRows; i++)
    {
      if (Layers->IsInSelection(i, 0) == true)
        {
          // adding a further Layer
          int count = 0;
          LayerListItem *pL = First;
          while (pL)
            {
              if (count == i)
                {
                  if (MapPanel->GetMapSRID() == -1)
                    {
                      // first map layer: setting the Map Srid
                      MapPanel->SetMapSRID(pL->GetSRID());
                    }
                  if (MapPanel->InsertMapLayer(pL, MapPanel->GetParent()) ==
                      true)
                    changed = true;
                  break;
                }
              count++;
              pL = pL->GetNext();
            }
        }
    }
  if (changed == true)
    {
      // updating the Layers Tree
      MapPanel->RebuildLayerTree();
      if (emptyMap == true)
        {
          // setting an initial Map extent
          MapPanel->SetMapToFullExtent();
        }
    }
  wxDialog::EndModal(wxID_OK);
}

bool MapLayerInfoDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MapPanel = parent;
  Layer = layer;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Map Layer Metadata")) == false)
    return false;
// populates individual controls
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void MapLayerInfoDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  RasterLayerConfig *rst;
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: Layer's fully qualified name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Full Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString name = Layer->GetDbPrefix();
  if (Layer->GetType() == MAP_LAYER_VECTOR
      || Layer->GetType() == MAP_LAYER_VECTOR_VIEW
      || Layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    name += wxT(".") + Layer->GetVectorPrefix();
  name += wxT(".") + Layer->GetName();
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_MAPLAYER_NAME, name,
                                        wxDefaultPosition, wxSize(550, 22),
                                        wxTE_READONLY);
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl =
    new wxTextCtrl(this, ID_MAPLAYER_TITLE, Layer->GetTitle(),
                   wxDefaultPosition, wxSize(550, 22),
                   wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: Abstract
  wxBoxSizer *abstractSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(abstractSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *abstractLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  abstractSizer->Add(abstractLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_MAPLAYER_ABSTRACT, Layer->GetAbstract(),
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE | wxTE_READONLY);
  abstractSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyCtrl =
    new wxTextCtrl(this, ID_MAPLAYER_COPYRIGHT, Layer->GetCopyright(),
                   wxDefaultPosition, wxSize(550, 60),
                   wxTE_MULTILINE | wxTE_READONLY);
  copySizer->Add(copyCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Data License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *licCtrl =
    new wxTextCtrl(this, ID_MAPLAYER_LICENSE, Layer->GetDataLicense(),
                   wxDefaultPosition, wxSize(550, 22),
                   wxTE_READONLY);
  licSizer->Add(licCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: Datasource
  wxBoxSizer *srcSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(srcSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *srcLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Datasource:"));
  srcSizer->Add(srcLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString datasource;
  char dummy[1024];
  const char *geom_type;
  const char *dims;
  switch (Layer->GetType())
    {
      case MAP_LAYER_WMS:
        datasource = wxT("WMS Layer    GetMap URL:\n");
        datasource += wxString::FromUTF8(Layer->GetWmsGetMapURL());
        break;
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
        datasource = wxT("Vector Coverage\n");
        datasource += wxT("SpatiaLite ");
        if (Layer->GetVectorPrefix().IsSameAs(wxT("table")) == true)
          datasource +=
            wxT("GeoTable: ") + wxString::FromUTF8(Layer->GetTableName()) +
            wxT(" / ") + wxString::FromUTF8(Layer->GetGeometryColumn()) +
            wxT("\n");
        if (Layer->GetVectorPrefix().IsSameAs(wxT("view")) == true)
          datasource +=
            wxT("SpatialView: ") + wxString::FromUTF8(Layer->GetTableName()) +
            wxT(" / ") + wxString::FromUTF8(Layer->GetGeometryColumn()) +
            wxT("\n");
        if (Layer->GetVectorPrefix().IsSameAs(wxT("shp")) == true)
          datasource +=
            wxT("VirtualShape: ") + wxString::FromUTF8(Layer->GetTableName()) +
            wxT(" / ") + wxString::FromUTF8(Layer->GetGeometryColumn()) +
            wxT("\n");
        geom_type = "GEOMETRY";
        dims = "XY";
        switch (Layer->GetGeometryType())
          {
            case 0:
            case 1000:
            case 2000:
            case 3000:
              geom_type = "GEOMETRY";
              break;
            case 1:
            case 1001:
            case 2001:
            case 3001:
              geom_type = "POINT";
              break;
            case 4:
            case 1004:
            case 2004:
            case 3004:
              geom_type = "MULTIPOINT";
              break;
            case 2:
            case 1002:
            case 2002:
            case 3002:
              geom_type = "LINESTRING";
              break;
            case 5:
            case 1005:
            case 2005:
            case 3005:
              geom_type = "MULTILINESTRING";
              break;
            case 3:
            case 1003:
            case 2003:
            case 3003:
              geom_type = "POLYGON";
              break;
            case 6:
            case 1006:
            case 2006:
            case 3006:
              geom_type = "POLYGON";
              break;
            case 7:
            case 1007:
            case 2007:
            case 3007:
              geom_type = "GEOMETRYCOLLECTION";
              break;
            default:
              geom_type = "UNKNOWN";
              break;
          }
        switch (Layer->GetGeometryType())
          {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
              dims = "XY";
              break;
            case 1000:
            case 1001:
            case 1002:
            case 1003:
            case 1004:
            case 1005:
            case 1006:
            case 1007:
              dims = "XYZ";
              break;
            case 2000:
            case 2001:
            case 2002:
            case 2003:
            case 2004:
            case 2005:
            case 2006:
            case 2007:
              dims = "XYM";
              break;
            case 3000:
            case 3001:
            case 3002:
            case 3003:
            case 3004:
            case 3005:
            case 3006:
            case 3007:
              dims = "XYZM";
              break;
            default:
              dims = "UNKNOWN";
              break;
          }
        sprintf(dummy, "SRID: %d    Type: %s %s    SpatialIndex: %s",
                Layer->GetNativeSRID(), geom_type, dims,
                (Layer->HasSpatialIndex() == 1) ? "yes" : "no");
        datasource += wxString::FromUTF8(dummy);
        break;
      case MAP_LAYER_TOPOLOGY:
        datasource = wxT("Topology-Geometry\n");
        datasource +=
          wxT("SpatiaLite TopoGeo: ") +
          wxString::FromUTF8(Layer->GetTopologyName()) + wxT("\n");
        sprintf(dummy, "SRID: %d %s (%s)", Layer->GetNativeSRID(),
                (Layer->GetHasZ() == true) ? "3D" : "2D",
                (Layer->GetHasZ() == true) ? "XYZ" : "XY");
        datasource += wxString::FromUTF8(dummy);
        break;
      case MAP_LAYER_NETWORK:
        datasource = wxT("Topology-Network\n");
        datasource +=
          wxT("SpatiaLite TopoNet: ") +
          wxString::FromUTF8(Layer->GetNetworkName()) + wxT("\n");
        sprintf(dummy, "SRID: %d %s (%s)", Layer->GetNativeSRID(),
                (Layer->GetHasZ() == true) ? "3D" : "2D",
                (Layer->GetHasZ() == true) ? "XYZ" : "XY");
        datasource += wxString::FromUTF8(dummy);
        break;
      case MAP_LAYER_RASTER:
        datasource = wxT("RasterLite2 Coverage\n");
        sprintf(dummy, "SRID: %d", Layer->GetNativeSRID());
        datasource += wxString::FromUTF8(dummy);
        rst = Layer->GetRasterConfig();
        datasource += wxT("    Sample: ") + rst->GetSampleType();
        datasource += wxT("    Pixel: ") + rst->GetPixelType();
        sprintf(dummy, "    Bands: %d\n", rst->GetNumBands());
        datasource += wxString::FromUTF8(dummy);
        datasource += wxT("Compression: ") + rst->GetCompression();
        sprintf(dummy,
                "    Quality: %d    TileSize: %d x %d\nPixel Resolution    Horizontal: %1.8f    Vertical: %1.8f",
                rst->GetQuality(), rst->GetTileWidth(),
                rst->GetTileHeight(), rst->GetHorzResolution(),
                rst->GetVertResolution());
        datasource += wxString::FromUTF8(dummy);
        break;
      case MAP_LAYER_UNKNOWN:
      default:
        datasource = wxT("UNKNOWN !!!!");
        break;
    };
  wxTextCtrl *srcCtrl = new wxTextCtrl(this, ID_MAPLAYER_DATASOURCE, datasource,
                                       wxDefaultPosition, wxSize(550, 80),
                                       wxTE_MULTILINE | wxTE_READONLY);
  srcSizer->Add(srcCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: Extents
  wxBoxSizer *extSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(extSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *extGridBox = new wxStaticBox(this, wxID_STATIC,
                                            wxT("Full Extent"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *extGridSizer = new wxStaticBoxSizer(extGridBox, wxVERTICAL);
  extSizer->Add(extGridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *extGrid2Sizer = new wxBoxSizer(wxHORIZONTAL);
  extGridSizer->Add(extGrid2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  Extents =
    new wxGrid(this, ID_MAPLAYER_EXTENTS, wxDefaultPosition, wxSize(600, 125));
  Extents->CreateGrid(3, 7, wxGrid::wxGridSelectRows);
  Extents->SetColLabelValue(0, wxT("Context"));
  Extents->SetColLabelValue(1, wxT("Min X"));
  Extents->SetColLabelValue(2, wxT("Min Y"));
  Extents->SetColLabelValue(3, wxT("Max X"));
  Extents->SetColLabelValue(4, wxT("Max Y"));
  Extents->SetColLabelValue(5, wxT("SRID"));
  Extents->SetColLabelValue(6, wxT("RefSys Name"));
  Extents->SetCellValue(0, 0, wxT("Geographic"));
// Geographic Extent
  if (Layer->GetGeoMinX() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetGeoMinX());
  Extents->SetCellValue(0, 1, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetGeoMinY() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetGeoMinY());
  Extents->SetCellValue(0, 2, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 2, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetGeoMaxX() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetGeoMaxX());
  Extents->SetCellValue(0, 3, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 3, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetGeoMaxY() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetGeoMaxY());
  Extents->SetCellValue(0, 4, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 4, wxALIGN_RIGHT, wxALIGN_TOP);
  Extents->SetCellValue(0, 5, wxT("4326"));
  Extents->SetCellAlignment(0, 5, wxALIGN_RIGHT, wxALIGN_TOP);
  wxString refsys_name;
  DoFetchRefSysName(4326, refsys_name);
  Extents->SetCellValue(0, 6, refsys_name);
// Native Extent
  Extents->SetCellValue(1, 0, wxT("Layer Native"));
  if (Layer->GetMinX() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMinX());
  Extents->SetCellValue(1, 1, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetMinY() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMinY());
  Extents->SetCellValue(1, 2, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 2, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetMaxX() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMaxX());
  Extents->SetCellValue(1, 3, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 3, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetMaxY() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMaxY());
  Extents->SetCellValue(1, 4, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 4, wxALIGN_RIGHT, wxALIGN_TOP);
  sprintf(dummy, "%d", Layer->GetNativeSRID());
  Extents->SetCellValue(1, 5, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 5, wxALIGN_RIGHT, wxALIGN_TOP);
  DoFetchRefSysName(Layer->GetNativeSRID(), refsys_name);
  Extents->SetCellValue(1, 6, refsys_name);
// Map Extent
  Extents->SetCellValue(2, 0, wxT("Map RefSys"));
  if (Layer->GetMapMinX() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMapMinX());
  Extents->SetCellValue(2, 1, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(2, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetMapMinY() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMapMinY());
  Extents->SetCellValue(2, 2, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(2, 2, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetMapMaxX() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMapMaxX());
  Extents->SetCellValue(2, 3, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(2, 3, wxALIGN_RIGHT, wxALIGN_TOP);
  if (Layer->GetMapMaxY() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", Layer->GetMapMaxY());
  Extents->SetCellValue(2, 4, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(2, 4, wxALIGN_RIGHT, wxALIGN_TOP);
  sprintf(dummy, "%d", Layer->GetMapSRID());
  Extents->SetCellValue(2, 5, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(2, 5, wxALIGN_RIGHT, wxALIGN_TOP);
  DoFetchRefSysName(Layer->GetMapSRID(), refsys_name);
  Extents->SetCellValue(2, 6, refsys_name);

  Extents->SetRowLabelSize(wxGRID_AUTOSIZE);
  Extents->AutoSize();
  Extents->EnableEditing(false);
  extGrid2Sizer->Add(Extents, 0, wxALIGN_RIGHT | wxALL, 5);
// eighth row: Queryable and Editable
  wxBoxSizer *queryEditSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(queryEditSizer, 0, wxALIGN_CENTER | wxALL, 0);
  wxBoxSizer *querySizer = new wxBoxSizer(wxHORIZONTAL);
  queryEditSizer->Add(querySizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticText *queryLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Is Queryable:"));
  querySizer->Add(queryLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryEditSizer->AddSpacer(25);
  queryEditSizer->AddSpacer(25);
  queryEditSizer->AddSpacer(25);
  queryEditSizer->AddSpacer(25);
  wxString queryable = wxT("Yes");
  if (Layer->IsQueryable() == false)
    queryable = wxT("No");
  wxTextCtrl *queryCtrl = new wxTextCtrl(this, ID_MAPLAYER_QUERYABLE, queryable,
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxTE_READONLY);
  querySizer->Add(queryCtrl, 0, wxALIGN_LEFT | wxALL, 5);
  wxBoxSizer *editSizer = new wxBoxSizer(wxHORIZONTAL);
  queryEditSizer->Add(editSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticText *editLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Is Editable:"));
  editSizer->Add(editLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString editable = wxT("Yes");
  if (Layer->IsEditable() == false || Layer->GetDbPrefix() != wxT("main"))
    editable = wxT("No");
  wxTextCtrl *editCtrl = new wxTextCtrl(this, ID_MAPLAYER_EDITABLE, editable,
                                        wxDefaultPosition, wxSize(80, 22),
                                        wxTE_READONLY);
  editSizer->Add(editCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// QUIT button
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for QUIT button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapLayerInfoDialog::OnOk);
}

void MapLayerInfoDialog::DoFetchRefSysName(int srid, wxString & name)
{
//
// fetching the RefSys Name
//
  sqlite3 *sqlite = MapPanel->GetSqlite();
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char *value;
  name = wxT("Undefined");

  char *sql =
    sqlite3_mprintf("SELECT ref_sys_name FROM spatial_ref_sys WHERE srid = %d",
                    srid);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          name = wxString::FromUTF8(value);
        }
    }
  sqlite3_free_table(results);
}

void MapLayerInfoDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool MapConfigDialog::Create(MyMapPanel * parent)
{
//
// creating the dialog
//
  MapPanel = parent;
  MapName = MapPanel->GetMapName();
  Title = MapPanel->GetTitle();
  Abstract = MapPanel->GetAbstract();
  MultiThreadingEnabled = MapPanel->IsMapMultiThreadingEnabled();
  MaxThreads = MapPanel->GetMapMaxThreads();
  AutoTransformEnabled = MapPanel->IsMapAutoTransformEnabled();
  MapSRID = MapPanel->GetMapSRID();
  OldMapSRID = MapSRID;
  RasterWmsAutoSwitch = MapPanel->IsRasterWmsAutoSwitch();
  LabelAntiCollision = MapPanel->IsLabelAntiCollision();
  LabelWrapText = MapPanel->IsLabelWrapText();
  LabelAutoRotate = MapPanel->IsLabelAutoRotate();
  LabelShiftPosition = MapPanel->IsLabelShiftPosition();
  GeographicCoordsDMS = MapPanel->IsGeographicCoordsDMS();
  CheckeredMapBackground = MapPanel->IsCheckeredMapBackground();
  SolidMapBackground = MapPanel->GetSolidMapBackground();
  IsConfigChanged = false;

  if (wxPropertySheetDialog::Create(parent, wxID_ANY, wxT("Map Global Options"))
      == false)
    return false;
  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General Infos"), true);
  wxPanel *srsPage = CreateSrsPage(book);
  book->AddPage(srsPage, wxT("Spatial Reference System"), false);
  wxPanel *optionsPage = CreateOptionsPage(book);
  book->AddPage(optionsPage, wxT("Advanced Options"), false);
  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) & MapConfigDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & MapConfigDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnQuit);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnOk);
  Connect(ID_MAP_CONFIG_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnInsert);
  Connect(ID_MAP_CONFIG_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnExport);
  Connect(ID_MAP_CONFIG_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

void MapConfigDialog::CreateButtons()
{
// 
// adding the common Buttons
//
  wxBoxSizer *topSizer = (wxBoxSizer *) (this->GetSizer());
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_MAP_CONFIG_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_MAP_CONFIG_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_MAP_CONFIG_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
}

wxPanel *MapConfigDialog::CreateMainPage(wxWindow * parent)
{
//
// creating the MAIN page 
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MAIN);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// First row: Map name
  boxSizer->AddSpacer(50);
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *nameLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Map Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_MAP_CONFIG_NAME, MapName,
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Map Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(panel, ID_MAP_CONFIG_TITLE, Title,
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Map Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(panel, ID_MAP_CONFIG_ABSTRACT, Abstract,
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
  return panel;
}

void MapConfigDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

bool MapConfigDialog::CheckExistingMapConfiguration(sqlite3 * sqlite,
                                                    MyFrame * parent,
                                                    const char *name, int *id)
{
//
// checking for an already existing configuration of the same name
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  bool exists = false;
  char *sql = sqlite3_mprintf("SELECT id FROM rl2map_configurations "
                              "WHERE name = %Q", name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, parent);
      sqlite3_free(errMsg);
      return false;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          *id = atoi(value);
          exists = true;
        }
    }
  sqlite3_free_table(results);
  return exists;
}

void MapConfigDialog::OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the Map COnfiguration into the DBMS
//
  bool changed = false;
  int valid;
  const char *sql;
  int ret;
  sqlite3_stmt *stmt = NULL;
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrieveSrsPage();
        break;
      case 2:
        RetrieveOptionsPage();
        break;
    };

  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml = DoCreateMapConfigXML();

  // checking for already existing name */
  char *name = (char *) malloc(MapName.Len() * 4);
  strcpy(name, MapName.ToUTF8());
  int id;
  bool reload = false;
  if (CheckExistingMapConfiguration
      (MapPanel->GetSqlite(), MapPanel->GetParent(), name, &id))
    {
      ret =
        wxMessageBox(wxT("A Map Configuration named \"") + MapName +
                     wxT("\" already exists\n\n") +
                     wxT("Do you intend to overwrite it ?"),
                     wxT("spatialite_gui"), wxYES_NO | wxICON_QUESTION, this);
      if (ret != wxYES)
        return;
      sql = "SELECT RL2_ReloadMapConfiguration(?, XB_Create(?, 1, 1))";
      reload = true;
  } else
    sql = "SELECT RL2_RegisterMapConfiguration(XB_Create(?, 1, 1))";
  free(name);

  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("RL2_RegisterMapConfiguration error"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      free(xml);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  if (reload)
    {
      sqlite3_bind_int(stmt, 1, id);
      sqlite3_bind_blob(stmt, 2, xml, strlen(xml), sqlite3_free);
  } else
    sqlite3_bind_blob(stmt, 1, xml, strlen(xml), sqlite3_free);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_INTEGER)
            valid = sqlite3_column_int(stmt, 0);
      } else
        valid = 0;
    }
  sqlite3_finalize(stmt);
  if (valid > 0)
    {
      wxMessageBox(wxT("Current Map Configuration succesfully registered"),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      MapPanel->SetMapName(MapName);
      MapPanel->SetTitle(Title);
      MapPanel->SetAbstract(Abstract);
      MapPanel->EnableMapMultiThreading(MultiThreadingEnabled);
      MapPanel->SetMapMaxThreads(MaxThreads);
      MapPanel->UpdateMaxThreads();
      if (MapPanel->IsCheckeredMapBackground() != CheckeredMapBackground)
        {
          MapPanel->SetCheckeredMapBackground(CheckeredMapBackground);
          changed = true;
        }
      if (CheckeredMapBackground == false)
        {
          if (MapPanel->GetSolidMapBackground() != SolidMapBackground)
            changed = true;
        }
      MapPanel->SetSolidMapBackground(SolidMapBackground);
      MapPanel->EnableMapAutoTransform(AutoTransformEnabled);
      MapPanel->SetRasterWmsAutoSwitch(RasterWmsAutoSwitch);
      MapPanel->SetGeographicCoordsDMS(GeographicCoordsDMS);
      MapPanel->SetLabelAntiCollision(LabelAntiCollision);
      MapPanel->SetLabelWrapText(LabelWrapText);
      MapPanel->SetLabelAutoRotate(LabelAutoRotate);
      MapPanel->SetLabelShiftPosition(LabelShiftPosition);
      if (OldMapSRID != MapSRID)
        {
          MapPanel->SetMapSRID(MapSRID);
          MapPanel->AdjustLayersMapSRID();
          MapPanel->UpdateMapFullExtent();
          MapPanel->UpdateMapViewPoint(OldMapSRID, MapSRID);
          changed = true;
        }
      if (changed == true)
        MapPanel->RefreshMap();
      wxDialog::EndModal(wxID_OK);
  } else
    wxMessageBox(wxT("RL2_RegisterMapConfiguration error"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
}

void MapConfigDialog::OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the Map Configuration as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrieveSrsPage();
        break;
      case 2:
        RetrieveOptionsPage();
        break;
    };

  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  wxFileDialog fileDialog(this,
                          wxT
                          ("Exporting the current Map Configuration as an XML file"),
                          wxT(""), MapName + wxT(".xml"),
                          wxT("XML Document|*.xml|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MapPanel->GetParent()->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      lastDir = file.GetPath();
      path = fileDialog.GetPath();
      FILE *out = fopen(path.ToUTF8(), "wb");
      if (out == NULL)
        wxMessageBox(wxT("ERROR: unable to create:\n\n\"") + path + wxT("\""),
                     wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      else
        {
          char *xml = DoCreateMapConfigXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("XML Map Configuration successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
}

void MapConfigDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// copying the Map Configuration into the Clipboard
//
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrieveSrsPage();
        break;
      case 2:
        RetrieveOptionsPage();
        break;
    };

  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml = DoCreateMapConfigXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

bool MapConfigDialog::FinalValidityCheck()
{
//
// last check before generating the XML Map Configuration
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_NAME);
  MapName = nameCtrl->GetValue();
  if (MapName.Len() == 0)
    {
      wxMessageBox(wxT
                   ("You must specify some NAME for the current Map Configuration !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT
                   ("You must specify some TITLE for the current Map Configuration !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *abstractCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_ABSTRACT);
  Abstract = abstractCtrl->GetValue();
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT
                   ("You must specify some ABSTRACT for the current Map Configuration !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  return true;
}

void MapConfigDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  bool changed = false;
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_NAME);
  MapName = nameCtrl->GetValue();
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_TITLE);
  Title = titleCtrl->GetValue();
  wxTextCtrl *abstractCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_ABSTRACT);
  Abstract = abstractCtrl->GetValue();
  wxCheckBox *multiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_MULTITHREAD);
  MultiThreadingEnabled = multiCtrl->GetValue();
  wxSpinCtrl *maxCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_MAXTHREADS);
  MaxThreads = maxCtrl->GetValue();
  wxCheckBox *autoCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOTRANSFORM);
  AutoTransformEnabled = autoCtrl->GetValue();
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_SRID);
  MapSRID = sridCtrl->GetValue();
  wxCheckBox *autoSwitchCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOSWITCH);
  RasterWmsAutoSwitch = autoSwitchCtrl->GetValue();
  wxRadioBox *ddDmsBox = (wxRadioBox *) FindWindow(ID_MAPOPT_DD_DMS);
  if (ddDmsBox->GetSelection() == 1)
    GeographicCoordsDMS = true;
  else
    GeographicCoordsDMS = false;
  wxCheckBox *antiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_ANTI_COLLISION);
  LabelAntiCollision = antiCtrl->GetValue();
  wxCheckBox *wrapCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_WRAP_TEXT);
  LabelWrapText = wrapCtrl->GetValue();
  wxCheckBox *rotateCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTO_ROTATE);
  LabelAutoRotate = rotateCtrl->GetValue();
  wxCheckBox *shiftCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_SHIFT_POSITION);
  LabelShiftPosition = shiftCtrl->GetValue();

  MapPanel->SetMapName(MapName);
  MapPanel->SetTitle(Title);
  MapPanel->SetAbstract(Abstract);
  MapPanel->EnableMapMultiThreading(MultiThreadingEnabled);
  MapPanel->SetMapMaxThreads(MaxThreads);
  MapPanel->UpdateMaxThreads();
  if (MapPanel->IsCheckeredMapBackground() != CheckeredMapBackground)
    {
      MapPanel->SetCheckeredMapBackground(CheckeredMapBackground);
      changed = true;
    }
  if (CheckeredMapBackground == false)
    {
      if (MapPanel->GetSolidMapBackground() != SolidMapBackground)
        changed = true;
    }
  MapPanel->SetSolidMapBackground(SolidMapBackground);
  MapPanel->EnableMapAutoTransform(AutoTransformEnabled);
  MapPanel->SetRasterWmsAutoSwitch(RasterWmsAutoSwitch);
  MapPanel->SetGeographicCoordsDMS(GeographicCoordsDMS);
  MapPanel->SetLabelAntiCollision(LabelAntiCollision);
  MapPanel->SetLabelWrapText(LabelWrapText);
  MapPanel->SetLabelAutoRotate(LabelAutoRotate);
  MapPanel->SetLabelShiftPosition(LabelShiftPosition);
  if (OldMapSRID != MapSRID)
    {
      MapPanel->SetMapSRID(MapSRID);
      MapPanel->AdjustLayersMapSRID();
      MapPanel->UpdateMapFullExtent();
      MapPanel->UpdateMapViewPoint(OldMapSRID, MapSRID);
      changed = true;
    }
  if (changed == true)
    MapPanel->RefreshMap();
  wxDialog::EndModal(wxID_OK);
}

void MapConfigDialog::OnPageChanging(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changing
//
  bool ret = false;
  int idx = event.GetOldSelection();
  if (idx == 0)
    ret = RetrieveMainPage();
  else
    {
      if (idx == 1)
        ret = RetrieveSrsPage();
      if (idx == 2)
        ret = RetrieveOptionsPage();
    }
  if (ret != true)
    event.Veto();
}

void MapConfigDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  int idx = event.GetSelection();
  if (idx == 0)
    UpdateMainPage();
  else
    {
      if (idx == 1)
        UpdateSrsPage();
      if (idx == 2)
        UpdateOptionsPage();
    }
}

bool MapConfigDialog::RetrieveMainPage()
{
//
// retrieving params from the MAIN page
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_NAME);
  MapName = nameCtrl->GetValue();
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_TITLE);
  Title = titleCtrl->GetValue();
  wxTextCtrl *abstractCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_ABSTRACT);
  Abstract = abstractCtrl->GetValue();
  return true;
}

void MapConfigDialog::UpdateMainPage()
{
//
// updating the MAIN page
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_NAME);
  nameCtrl->SetValue(MapName);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_TITLE);
  titleCtrl->SetValue(Title);
  wxTextCtrl *abstractCtrl = (wxTextCtrl *) FindWindow(ID_MAP_CONFIG_ABSTRACT);
  abstractCtrl->SetValue(Abstract);
}

wxPanel *MapConfigDialog::CreateSrsPage(wxWindow * parent)
{
//
// creating the SRS page
//
  wxPanel *panel = new wxPanel(parent, ID_MAP_CONFIG_PANE_SRS);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: AutoTransform and MapSRID
  wxBoxSizer *autoSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(autoSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticBox *autoBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Map Reference System"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *auto1Sizer = new wxStaticBoxSizer(autoBox, wxVERTICAL);
  autoSizer->Add(auto1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *auto2Sizer = new wxBoxSizer(wxHORIZONTAL);
  auto1Sizer->Add(auto2Sizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxCheckBox *autoCtrl = new wxCheckBox(panel, ID_MAPOPT_AUTOTRANSFORM,
                                        wxT("&Enable AutoTransform"),
                                        wxDefaultPosition, wxDefaultSize);
  autoCtrl->SetValue(AutoTransformEnabled);
  auto2Sizer->Add(autoCtrl, 0, wxALIGN_LEFT | wxALL, 5);

  wxBoxSizer *auto3Sizer = new wxBoxSizer(wxHORIZONTAL);
  auto1Sizer->Add(auto3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *sridLabel = new wxStaticText(panel, wxID_STATIC, wxT("&SRID:"));
  auto3Sizer->Add(sridLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *sridCtrl = new wxSpinCtrl(panel, ID_MAPOPT_SRID, wxEmptyString,
                                        wxDefaultPosition, wxSize(80, 20),
                                        wxSP_ARROW_KEYS,
                                        -1, 1000000, MapSRID);
  sridCtrl->Enable(AutoTransformEnabled);
  auto3Sizer->Add(sridCtrl, 0, wxALIGN_LEFT | wxALL, 5);
//multi3Sizer->AddSpacer(25);
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_MAPOPT_NAME, wxT("Undefined"),
                                        wxDefaultPosition, wxSize(350, 22),
                                        wxTE_READONLY);
  UpdateRefSysName();
  nameCtrl->Enable(AutoTransformEnabled);
  auto3Sizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: Extents
  wxBoxSizer *extSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(extSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *extGridBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Full Extent"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *extGridSizer = new wxStaticBoxSizer(extGridBox, wxVERTICAL);
  extSizer->Add(extGridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *extGrid2Sizer = new wxBoxSizer(wxHORIZONTAL);
  extGridSizer->Add(extGrid2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  Extents =
    new wxGrid(panel, ID_MAPOPT_EXTENTS, wxDefaultPosition, wxSize(600, 80));
  Extents->CreateGrid(2, 5, wxGrid::wxGridSelectRows);
  Extents->SetColLabelValue(0, wxT("Context"));
  Extents->SetColLabelValue(1, wxT("Min X"));
  Extents->SetColLabelValue(2, wxT("Min Y"));
  Extents->SetColLabelValue(3, wxT("Max X"));
  Extents->SetColLabelValue(4, wxT("Max Y"));
  Extents->SetCellValue(0, 0, wxT("Geographic"));
// Geographic Extent
  char dummy[1024];
  if (MapPanel->GetGeoMinX() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetGeoMinX());
  Extents->SetCellValue(0, 1, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  if (MapPanel->GetGeoMinY() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetGeoMinY());
  Extents->SetCellValue(0, 2, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 2, wxALIGN_RIGHT, wxALIGN_TOP);
  if (MapPanel->GetGeoMaxX() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetGeoMaxX());
  Extents->SetCellValue(0, 3, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 3, wxALIGN_RIGHT, wxALIGN_TOP);
  if (MapPanel->GetGeoMaxY() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetGeoMaxY());
  Extents->SetCellValue(0, 4, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(0, 4, wxALIGN_RIGHT, wxALIGN_TOP);
// Map Extent
  Extents->SetCellValue(1, 0, wxT("Map RefSys"));
  if (MapPanel->GetMapMinX() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetMapMinX());
  Extents->SetCellValue(1, 1, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  if (MapPanel->GetMapMinY() == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetMapMinY());
  Extents->SetCellValue(1, 2, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 2, wxALIGN_RIGHT, wxALIGN_TOP);
  if (MapPanel->GetMapMaxX() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetMapMaxX());
  Extents->SetCellValue(1, 3, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 3, wxALIGN_RIGHT, wxALIGN_TOP);
  if (MapPanel->GetMapMaxY() == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", MapPanel->GetMapMaxY());
  Extents->SetCellValue(1, 4, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 4, wxALIGN_RIGHT, wxALIGN_TOP);

  Extents->SetRowLabelSize(wxGRID_AUTOSIZE);
  Extents->AutoSize();
  Extents->EnableEditing(false);
  extGrid2Sizer->Add(Extents, 0, wxALIGN_RIGHT | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_MAPOPT_AUTOTRANSFORM, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnAutoTransformChanged);
  Connect(ID_MAPOPT_SRID, wxEVT_COMMAND_SPINCTRL_UPDATED,
          (wxObjectEventFunction) & MapConfigDialog::OnMapSridChanged);
  Connect(ID_MAPOPT_SRID, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) & MapConfigDialog::OnMapSridChanged);
  return panel;
}

void MapConfigDialog::OnAutoTransformChanged(wxCommandEvent & WXUNUSED(event))
{
//
// AutoTransform: Enable/Disable
//
  wxCheckBox *autoCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOTRANSFORM);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_SRID);
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_MAPOPT_NAME);
  wxButton *ok = (wxButton *) FindWindow(wxID_OK);
  AutoTransformEnabled = autoCtrl->GetValue();
  bool valid = UpdateRefSysName();
  sridCtrl->Enable(AutoTransformEnabled);
  nameCtrl->Enable(AutoTransformEnabled);
  ok->Enable(valid);
}

void MapConfigDialog::OnMapSridChanged(wxCommandEvent & WXUNUSED(event))
{
//
// MapSrid changed
//
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_SRID);
  wxButton *ok = (wxButton *) FindWindow(wxID_OK);
  MapSRID = sridCtrl->GetValue();
  bool valid = UpdateRefSysName();
  if ((valid == true) && (OldMapSRID != MapSRID))
    DoUpdateMapExtent();
  ok->Enable(valid);
}

bool MapConfigDialog::RetrieveSrsPage(bool check)
{
//
// retrieving params from the SRS page
//
  wxCheckBox *autoCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOTRANSFORM);
  AutoTransformEnabled = autoCtrl->GetValue();
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_SRID);
  MapSRID = sridCtrl->GetValue();
  return true;
}

void MapConfigDialog::UpdateSrsPage()
{
//
// updating the SRS page
//
  wxCheckBox *autoCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOTRANSFORM);
  autoCtrl->SetValue(AutoTransformEnabled);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_SRID);
  sridCtrl->SetValue(MapSRID);
}

bool MapConfigDialog::UpdateRefSysName()
{
//
// updating the RefSys Name
//
  sqlite3 *sqlite = MapPanel->GetSqlite();
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  char *value;
  bool valid = false;
  wxString name(wxT("Undefined"));

  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_MAPOPT_NAME);
  if (AutoTransformEnabled == false)
    {
      nameCtrl->SetValue(wxT("Not Applicable"));
      return true;
    }

  sql =
    sqlite3_mprintf("SELECT ref_sys_name FROM spatial_ref_sys WHERE srid = %d",
                    MapSRID);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
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
          name = wxString::FromUTF8(value);
          valid = true;
        }
    }
  sqlite3_free_table(results);
end:
  nameCtrl->SetValue(name);
  return valid;
}

void MapConfigDialog::DoUpdateMapExtent()
{
//
// updating the Map Extent
//
  double minX = DBL_MAX;
  double minY = DBL_MAX;
  double maxX = -DBL_MAX;
  double maxY = -DBL_MAX;
  sqlite3_stmt *stmt = NULL;
  int ret;

  if (MapPanel->GetGeoMinX() == DBL_MAX || MapPanel->GetGeoMinY() == DBL_MAX
      || MapPanel->GetGeoMaxX() == -DBL_MAX
      || MapPanel->GetGeoMaxY() == -DBL_MAX)
    return;

  const char *sql =
    "SELECT MbrMinX(x.bbox), MbrMinY(x.bbox), MbrMaxX(x.bbox), MbrMaxY(x.bbox) "
    "FROM (SELECT ST_Transform(BuildMbr(?, ?, ?, ?, 4326), ?) AS bbox) AS x";
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_double(stmt, 1, MapPanel->GetGeoMinX());
  sqlite3_bind_double(stmt, 2, MapPanel->GetGeoMinY());
  sqlite3_bind_double(stmt, 3, MapPanel->GetGeoMaxX());
  sqlite3_bind_double(stmt, 4, MapPanel->GetGeoMaxY());
  sqlite3_bind_int(stmt, 5, MapSRID);
  while (1)
    {
      // scrolling the result set rows
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          minX = sqlite3_column_double(stmt, 0);
          minY = sqlite3_column_double(stmt, 1);
          maxX = sqlite3_column_double(stmt, 2);
          maxY = sqlite3_column_double(stmt, 3);
        }
    }
  sqlite3_finalize(stmt);

// updating the Map Extent
  char dummy[1024];
  Extents->SetCellValue(1, 0, wxT("Map RefSys"));
  if (minX == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", minX);
  Extents->SetCellValue(1, 1, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  if (minY == DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", minY);
  Extents->SetCellValue(1, 2, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 2, wxALIGN_RIGHT, wxALIGN_TOP);
  if (maxX == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", maxX);
  Extents->SetCellValue(1, 3, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 3, wxALIGN_RIGHT, wxALIGN_TOP);
  if (maxY == -DBL_MAX)
    strcpy(dummy, "?");
  else
    sprintf(dummy, "%1.6f", maxY);
  Extents->SetCellValue(1, 4, wxString::FromUTF8(dummy));
  Extents->SetCellAlignment(1, 4, wxALIGN_RIGHT, wxALIGN_TOP);
}

wxPanel *MapConfigDialog::CreateOptionsPage(wxWindow * parent)
{
//
// creating the Advanced Options page
//
  wxPanel *panel = new wxPanel(parent, ID_MAP_CONFIG_PANE_OPTIONS);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(leftSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(rightSizer, 0, wxALIGN_RIGHT | wxALL, 0);
// first row: MultiThreading
  wxStaticBox *multiBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Multi Threaded Rendering"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *multi2Sizer = new wxStaticBoxSizer(multiBox, wxVERTICAL);
  leftSizer->Add(multi2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *multi3Sizer = new wxBoxSizer(wxHORIZONTAL);
  multi2Sizer->Add(multi3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxCheckBox *multiCtrl = new wxCheckBox(panel, ID_MAPOPT_MULTITHREAD,
                                         wxT("&Enable"),
                                         wxDefaultPosition, wxDefaultSize);
  multiCtrl->SetValue(MultiThreadingEnabled);
  multi3Sizer->Add(multiCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  multi3Sizer->AddSpacer(25);
  multi3Sizer->AddSpacer(25);
  wxStaticText *maxLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Max Threads:"));
  multi3Sizer->Add(maxLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *maxCtrl =
    new wxSpinCtrl(panel, ID_MAPOPT_MAXTHREADS, wxEmptyString,
                   wxDefaultPosition, wxSize(80, 20),
                   wxSP_ARROW_KEYS,
                   1, 32, MaxThreads);
  maxCtrl->Enable(MultiThreadingEnabled);
  multi3Sizer->Add(maxCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
// Raster/WMS AutoSwitch
  wxStaticBox *rstBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Raster/WMS AutoSwitch"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *rst1Sizer = new wxStaticBoxSizer(rstBox, wxVERTICAL);
  leftSizer->Add(rst1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *autoSwitchCtrl = new wxCheckBox(panel, ID_MAPOPT_AUTOSWITCH,
                                              wxT
                                              ("&Automatically Hide Raster/WMS Coverages"),
                                              wxDefaultPosition, wxDefaultSize);
  autoSwitchCtrl->SetValue(RasterWmsAutoSwitch);
  rst1Sizer->Add(autoSwitchCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// DMS / DD
  wxString dd_dms[2];
  dd_dms[0] = wxT("Decimal Degrees");
  dd_dms[1] = wxT("Degrees Minutes Seconds");
  wxRadioBox *ddDmsBox = new wxRadioBox(panel, ID_MAPOPT_DD_DMS,
                                        wxT("Geographic Coords Format"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 2,
                                        dd_dms, 2,
                                        wxRA_SPECIFY_ROWS);
  if (GeographicCoordsDMS == true)
    ddDmsBox->SetSelection(1);
  else
    ddDmsBox->SetSelection(0);
  leftSizer->Add(ddDmsBox, 0, wxALIGN_TOP | wxALL, 5);
// Map Background
  wxStaticBox *bgBox = new wxStaticBox(panel, wxID_STATIC,
                                       wxT("Map Background"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  wxBoxSizer *bg1Sizer = new wxStaticBoxSizer(bgBox, wxVERTICAL);
  rightSizer->Add(bg1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *bg2Sizer = new wxBoxSizer(wxHORIZONTAL);
  bg1Sizer->Add(bg2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxCheckBox *checkeredCtrl =
    new wxCheckBox(panel, ID_MAPOPT_CHECKERED_BACKGROUND,
                   wxT("&Transparent"),
                   wxDefaultPosition, wxDefaultSize);
  checkeredCtrl->SetValue(CheckeredMapBackground);
  bg2Sizer->Add(checkeredCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *bgColorBox = new wxStaticBox(panel, wxID_ANY,
                                            wxT("Background Color"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *bgColorBoxSizer = new wxStaticBoxSizer(bgColorBox, wxHORIZONTAL);
  bg2Sizer->Add(bgColorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bgColorBmp;
  GetButtonBitmap(SolidMapBackground, bgColorBmp);
  wxBitmapButton *bgColorCtrl =
    new wxBitmapButton(panel, ID_MAPOPT_SOLID_BACKGROUND, bgColorBmp);
  if (CheckeredMapBackground == false)
    bgColorCtrl->Enable(true);
  else
    bgColorCtrl->Enable(false);
  bgColorBoxSizer->Add(bgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Label Options
  wxStaticBox *labelBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Label Advanced Options"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *label1Sizer = new wxStaticBoxSizer(labelBox, wxVERTICAL);
  rightSizer->Add(label1Sizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *label2Sizer = new wxBoxSizer(wxHORIZONTAL);
  label1Sizer->Add(label2Sizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxCheckBox *antiCtrl = new wxCheckBox(panel, ID_MAPOPT_ANTI_COLLISION,
                                        wxT("Anti &Collision"),
                                        wxDefaultPosition, wxDefaultSize);
  antiCtrl->SetValue(LabelAntiCollision);
  label2Sizer->Add(antiCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  label2Sizer->AddSpacer(25);
  wxCheckBox *wrapCtrl = new wxCheckBox(panel, ID_MAPOPT_WRAP_TEXT,
                                        wxT("&Wrap Text"),
                                        wxDefaultPosition, wxDefaultSize);
  wrapCtrl->SetValue(LabelWrapText);
  label2Sizer->Add(wrapCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *label3Sizer = new wxBoxSizer(wxHORIZONTAL);
  label1Sizer->Add(label3Sizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxCheckBox *rotateCtrl = new wxCheckBox(panel, ID_MAPOPT_AUTO_ROTATE,
                                          wxT("Auto &Rotate"),
                                          wxDefaultPosition, wxDefaultSize);
  rotateCtrl->SetValue(LabelAutoRotate);
  label3Sizer->Add(rotateCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  label3Sizer->AddSpacer(25);
  wxCheckBox *shiftCtrl = new wxCheckBox(panel, ID_MAPOPT_SHIFT_POSITION,
                                         wxT("&Shift Position"),
                                         wxDefaultPosition, wxDefaultSize);
  shiftCtrl->SetValue(LabelShiftPosition);
  label3Sizer->Add(shiftCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_MAPOPT_MULTITHREAD, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnMultiThreadChanged);
  Connect(ID_MAPOPT_CHECKERED_BACKGROUND, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & MapConfigDialog::OnMapBackgroundChanged);
  Connect(ID_MAPOPT_SOLID_BACKGROUND, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          MapConfigDialog::OnMapBackgroundColorChanged);
  return panel;
}

void MapConfigDialog::GetButtonBitmap(wxColour & bgcolor, wxBitmap & bmp)
{
// creating a Bitmap representing some Color
  bmp = wxBitmap(128, 48);
  wxMemoryDC *dc = new wxMemoryDC(bmp);
  dc->SetBrush(wxBrush(bgcolor));
  dc->DrawRectangle(-1, -1, 130, 50);
  delete dc;
}

void MapConfigDialog::OnMapBackgroundChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Map Background selection changed
//
  wxBitmapButton *btn =
    (wxBitmapButton *) FindWindow(ID_MAPOPT_SOLID_BACKGROUND);
  if (CheckeredMapBackground == true)
    {
      wxBitmap bmp;
      GetButtonBitmap(SolidMapBackground, bmp);
      btn->SetBitmapLabel(bmp);
      CheckeredMapBackground = false;
      btn->Enable(true);
  } else
    {
      CheckeredMapBackground = true;
      btn->Enable(false);
    }
}

void MapConfigDialog::
OnMapBackgroundColorChanged(wxCommandEvent & WXUNUSED(event))
{
// Map Background Solid Color selection
  wxColourData initColor;
  int ret;
  initColor.SetChooseFull(false);
  initColor.SetColour(SolidMapBackground);
  wxColourDialog colorDialog(this, &initColor);
  ret = colorDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxColourData colorData = colorDialog.GetColourData();
      SolidMapBackground = colorData.GetColour();
      wxBitmap bmp;
      GetButtonBitmap(SolidMapBackground, bmp);
      wxBitmapButton *btn =
        (wxBitmapButton *) FindWindow(ID_MAPOPT_SOLID_BACKGROUND);
      btn->SetBitmapLabel(bmp);
    }
}

void MapConfigDialog::OnMultiThreadChanged(wxCommandEvent & WXUNUSED(event))
{
//
// MultiThreading: Enable/Disable
//
  wxCheckBox *multiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_MULTITHREAD);
  wxSpinCtrl *maxCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_MAXTHREADS);
  maxCtrl->Enable(multiCtrl->GetValue());
}

bool MapConfigDialog::RetrieveOptionsPage(bool check)
{
//
// retrieving params from the ADVANCED OPTIONS page
//
  wxCheckBox *multiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_MULTITHREAD);
  MultiThreadingEnabled = multiCtrl->GetValue();
  wxSpinCtrl *maxCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_MAXTHREADS);
  MaxThreads = maxCtrl->GetValue();
  MultiThreadingEnabled = multiCtrl->GetValue();
  if (MultiThreadingEnabled == false)
    MaxThreads = 1;
  if (MaxThreads <= 0)
    MaxThreads = 1;
  if (MaxThreads > 64)
    MaxThreads = 64;
  wxCheckBox *autoSwitchCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOSWITCH);
  RasterWmsAutoSwitch = autoSwitchCtrl->GetValue();
  wxRadioBox *ddDmsBox = (wxRadioBox *) FindWindow(ID_MAPOPT_DD_DMS);
  if (ddDmsBox->GetSelection() == 1)
    GeographicCoordsDMS = true;
  else
    GeographicCoordsDMS = false;
  wxCheckBox *antiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_ANTI_COLLISION);
  LabelAntiCollision = antiCtrl->GetValue();
  wxCheckBox *wrapCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_WRAP_TEXT);
  LabelWrapText = wrapCtrl->GetValue();
  wxCheckBox *rotateCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTO_ROTATE);
  LabelAutoRotate = rotateCtrl->GetValue();
  wxCheckBox *shiftCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_SHIFT_POSITION);
  LabelShiftPosition = shiftCtrl->GetValue();
  return true;
}

void MapConfigDialog::UpdateOptionsPage()
{
//
// updating the ADVANCED OPTIONS page
//
  wxCheckBox *multiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_MULTITHREAD);
  multiCtrl->SetValue(MultiThreadingEnabled);
  wxSpinCtrl *maxCtrl = (wxSpinCtrl *) FindWindow(ID_MAPOPT_MAXTHREADS);
  maxCtrl->SetValue(MaxThreads);
  wxCheckBox *autoSwitchCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTOSWITCH);
  autoSwitchCtrl->SetValue(RasterWmsAutoSwitch);
  wxRadioBox *ddDmsBox = (wxRadioBox *) FindWindow(ID_MAPOPT_DD_DMS);
  if (GeographicCoordsDMS == false)
    ddDmsBox->SetSelection(0);
  else
    ddDmsBox->SetSelection(1);
  wxCheckBox *antiCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_ANTI_COLLISION);
  antiCtrl->SetValue(LabelAntiCollision);
  wxCheckBox *wrapCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_WRAP_TEXT);
  wrapCtrl->SetValue(LabelWrapText);
  wxCheckBox *rotateCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_AUTO_ROTATE);
  rotateCtrl->SetValue(LabelAutoRotate);
  wxCheckBox *shiftCtrl = (wxCheckBox *) FindWindow(ID_MAPOPT_SHIFT_POSITION);
  shiftCtrl->SetValue(LabelShiftPosition);
}

char *MyLayerTree::PrintQuickStyleComplex(char *xml, QuickStyleObj * quick)
{
//
// printing an XML QuickStyle (Complex type)
//
  char *prev;
  char *xml2;

  if (quick->IsMinScaleEnabled() == true || quick->IsMaxScaleEnabled() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t<Rule>\r\n", prev);
      sqlite3_free(prev);
      if (quick->IsMinScaleEnabled() == true)
        {
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n",
             prev, quick->GetScaleMin());
          sqlite3_free(prev);
        }
      if (quick->IsMaxScaleEnabled() == true)
        {
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n",
             prev, quick->GetScaleMax());
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</Rule>\r\n", prev);
      sqlite3_free(prev);
    }
  prev = xml;
  if (quick->GetType() == QUICK_STYLE_POINT)
    xml2 = PrintQuickStylePoint(NULL, "", quick);
  else if (quick->GetType() == QUICK_STYLE_LINE)
    xml2 = PrintQuickStyleLine(NULL, "", quick);
  else if (quick->GetType() == QUICK_STYLE_POLYGON)
    xml2 = PrintQuickStylePolygon(NULL, "", quick);
  else
    {
      // mixed-type symbolizer
      xml2 = PrintQuickStylePoint(NULL, "", quick);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          xml2 = NULL;
        }
      xml2 = PrintQuickStyleLine(NULL, "", quick);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          xml2 = NULL;
        }
      xml2 = PrintQuickStylePolygon(NULL, "", quick);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          xml2 = NULL;
        }
    }
  if (xml2 != NULL)
    {
      xml = sqlite3_mprintf("%s%s", prev, xml2);
      sqlite3_free(prev);
      sqlite3_free(xml2);
    }
  if (quick->IsLabelPrint() == true)
    {
      // adding a Text Symbolizer
      xml2 = NULL;
      prev = xml;
      if (quick->GetType() == QUICK_STYLE_POINT
          || quick->GetType() == QUICK_STYLE_POLYGON)
        xml2 = PrintQuickStyleTextPoint(quick);
      if (quick->GetType() == QUICK_STYLE_LINE)
        xml2 = PrintQuickStyleTextLine(quick);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  return xml;
}

char *MyLayerTree::PrintQuickStylePoint(char *xml, const char *extra,
                                        QuickStyleObj * quick)
{
//
// printing an XML QuickStyle (POINT type)
//
  char *prev;
  const char *cstr;

  if (xml == NULL)
    xml = sqlite3_mprintf("%s\t\t\t<PointSymbolizer>\r\n", extra);
  else
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<PointSymbolizer>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t\t<Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  switch (quick->GetSymbolWellKnownMark())
    {
      case RL2_GRAPHIC_MARK_CIRCLE:
        cstr = "circle";
        break;
      case RL2_GRAPHIC_MARK_TRIANGLE:
        cstr = "triangle";
        break;
      case RL2_GRAPHIC_MARK_STAR:
        cstr = "star";
        break;
      case RL2_GRAPHIC_MARK_CROSS:
        cstr = "cross";
        break;
      case RL2_GRAPHIC_MARK_X:
        cstr = "x";
        break;
      default:
        cstr = "square";
        break;
    };
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t\t\t\t<WellKnownName>%s</WellKnownName>\r\n",
                    prev, extra, cstr);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t<Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, extra, quick->GetSymbolFillColor());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t</Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, extra, quick->GetSymbolStrokeColor());
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">%s</SvgParameter>\r\n",
     prev, extra, "round");
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">%s</SvgParameter>\r\n",
     prev, extra, "round");
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t</Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t\t</Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  if (quick->GetSymbolOpacity() != 1.0)
    {
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev,
                        extra, quick->GetSymbolOpacity());
      sqlite3_free(prev);
    }
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t\t\t<Size>%1.2f</Size>\r\n", prev, extra,
                    quick->GetSymbolSize());
  sqlite3_free(prev);
  if (quick->GetSymbolRotation() != 0.0)
    {
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t\t<Rotation>%1.2f</Rotation>\r\n",
                        prev, extra, quick->GetSymbolRotation());
      sqlite3_free(prev);
    }
  if (quick->GetSymbolAnchorX() != 0.5 || quick->GetSymbolAnchorY() != 0.5)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t<AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n", prev,
         extra, quick->GetSymbolAnchorX());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n", prev,
         extra, quick->GetSymbolAnchorY());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t\t</AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  if (quick->GetSymbolDisplacementX() != 0.0
      || quick->GetSymbolDisplacementY() != 0.0)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
         extra, quick->GetSymbolDisplacementX());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
         extra, quick->GetSymbolDisplacementY());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  return xml;
}

char *MyLayerTree::PrintQuickStyleLine(char *xml, const char *extra,
                                       QuickStyleObj * quick)
{
//
// printing an XML QuickStyle (LINESTRING type)
//
  char *prev;
  const char *dashArray;

  if (xml == NULL)
    xml = sqlite3_mprintf("%s\t\t\t<LineSymbolizer>\r\n", extra);
  else
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<LineSymbolizer>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, extra, quick->GetLineStrokeColor());
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
     prev, extra, quick->GetLineOpacity());
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, quick->GetLineStrokeWidth());
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">%s</SvgParameter>\r\n",
     prev, extra, "round");
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">%s</SvgParameter>\r\n",
     prev, extra, "round");
  sqlite3_free(prev);
  switch (quick->GetLineDotStyle())
    {
      case QUICK_STYLE_DOT_LINE:
        dashArray = "5.0, 10.0";
        break;
      case QUICK_STYLE_DASH_LINE:
        dashArray = "20.0, 20.0";
        break;
      case QUICK_STYLE_DASH_DOT_LINE:
        dashArray = "20.0, 10.0, 5.0, 10.0";
        break;
      default:
        dashArray = NULL;
        break;
    };
  if (dashArray != NULL)
    {
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
         prev, extra, dashArray);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  if (quick->GetLinePerpendicularOffset() != 0.0)
    {
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n",
         prev, extra, quick->GetLinePerpendicularOffset());
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</LineSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  if (quick->IsLine2Enabled() == true)
    {
      // definint a second LineSymbolizer - double pass style
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<LineSymbolizer>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t<Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
         prev, extra, quick->GetLine2StrokeColor());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, extra, quick->GetLineOpacity());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, extra, quick->GetLine2StrokeWidth());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">%s</SvgParameter>\r\n",
         prev, extra, "round");
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">%s</SvgParameter>\r\n",
         prev, extra, "round");
      sqlite3_free(prev);
      switch (quick->GetLine2DotStyle())
        {
          case QUICK_STYLE_DOT_LINE:
            dashArray = "5.0, 10.0";
            break;
          case QUICK_STYLE_DASH_LINE:
            dashArray = "20.0, 20.0";
            break;
          case QUICK_STYLE_DASH_DOT_LINE:
            dashArray = "20.0, 10.0, 5.0, 10.0";
            break;
          default:
            dashArray = NULL;
            break;
        };
      if (dashArray != NULL)
        {
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
             prev, extra, dashArray);
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t</Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      if (quick->GetLinePerpendicularOffset() != 0.0)
        {
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n",
             prev, extra, quick->GetLinePerpendicularOffset());
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</LineSymbolizer>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  return xml;
}

char *MyLayerTree::PrintQuickStylePolygon(char *xml, const char *extra,
                                          QuickStyleObj * quick)
{
//
// printing an XML QuickStyle (POLYGON type)
//
  char *prev;

  if (xml == NULL)
    xml = sqlite3_mprintf("%s\t\t\t<PolygonSymbolizer>\r\n", extra);
  else
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<PolygonSymbolizer>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  prev = xml;
  if (quick->IsPolygonFill() == true)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t\t<Fill>\r\n", prev, extra);
      sqlite3_free(prev);
      if (quick->IsPolygonSolidFill() == true)
        {
          // using a solid color
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
             prev, extra, quick->GetPolygonFillColor());
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
             prev, extra, quick->GetPolygonFillOpacity());
          sqlite3_free(prev);
      } else
        {
          // using a pattern brush
          const char *brush_name;
          switch (quick->GetPolygonFillBrushId())
            {
              case 1:
                brush_name = "vert";
                break;
              case 2:
                brush_name = "cross";
                break;
              case 3:
                brush_name = "diag1";
                break;
              case 4:
                brush_name = "diag2";
                break;
              case 5:
                brush_name = "crossdiag";
                break;
              case 6:
                brush_name = "dots";
                break;
              default:
                brush_name = "horz";
                break;
            };
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t\t<GraphicFill>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t<Graphic>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t<ExternalGraphic>\r\n", prev,
                            extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t\t\t<OnlineResource>http://www.utopia.gov/stdbrush_%s.png</OnlineResource>\r\n",
             prev, extra, brush_name);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t\t\t<Format>image/png</Format>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t<ColorReplacement>\r\n",
                            prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
             prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphics</LookupValue>\r\n",
             prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t\t\t<MapItem>\r\n", prev,
                            extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n",
                            prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n", prev, extra,
             quick->GetPolygonFillColor());
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t\t\t</MapItem>\r\n", prev,
                            extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t\t</Recode>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                            prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t</ExternalGraphic>\r\n",
                            prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t</Graphic>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t</GraphicFill>\r\n", prev, extra);
          sqlite3_free(prev);

        }
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t</Fill>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  if (quick->IsPolygonStroke() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t<Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
         prev, extra, quick->GetPolygonStrokeColor());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, extra, quick->GetPolygonStrokeOpacity());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, extra, quick->GetPolygonStrokeWidth());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">%s</SvgParameter>\r\n",
         prev, extra, "round");
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">%s</SvgParameter>\r\n",
         prev, extra, "round");
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t</Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (quick->GetPolygonDisplacementX() != 0.0
      || quick->GetPolygonDisplacementY() != 0.0)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
         extra, quick->GetPolygonDisplacementX());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
         extra, quick->GetPolygonDisplacementY());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  if (quick->GetPolygonPerpendicularOffset() != 0.0)
    {
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n",
         prev, extra, quick->GetPolygonPerpendicularOffset());
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</PolygonSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  return xml;
}

char *MyLayerTree::PrintQuickStyleTextPoint(QuickStyleObj * quick)
{
//
// printing an XML QuickStyle TEXT (POINT/POLYGON placement)
//
  char *xml;
  char *prev;
  const char *param;

  xml = sqlite3_mprintf("\t\t\t<TextSymbolizer>\r\n");
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t<Label>@%s@</Label>\r\n", prev,
                    quick->GetLabelColumn());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t<Font>\r\n", prev);
  sqlite3_free(prev);
  if (strcmp(quick->GetFontFacename(), "ToyFont: sans-serif") == 0)
    param = "sans serif";
  else if (strcmp(quick->GetFontFacename(), "ToyFont: serif") == 0)
    param = "serif";
  else if (strcmp(quick->GetFontFacename(), "ToyFont: monospace") == 0)
    param = "monospace";
  else
    param = quick->GetFontFacename();
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-family\">%s</SvgParameter>\r\n",
     prev, param);
  sqlite3_free(prev);
  if (quick->GetFontStyle() == RL2_FONTSTYLE_ITALIC)
    param = "italic";
  else if (quick->GetFontStyle() == RL2_FONTSTYLE_OBLIQUE)
    param = "oblique";
  else
    param = "normal";
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-style\">%s</SvgParameter>\r\n",
     prev, param);
  sqlite3_free(prev);
  if (quick->GetFontWeight() == RL2_FONTWEIGHT_BOLD)
    param = "bold";
  else
    param = "normal";
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-weight\">%s</SvgParameter>\r\n",
     prev, param);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-size\">%1.2f</SvgParameter>\r\n",
     prev, quick->GetFontSize());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t</Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t<LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t\t<PointPlacement>\r\n", prev);
  sqlite3_free(prev);
  if (quick->GetLabelAnchorX() != 0.5 || quick->GetLabelAnchorY() != 0.5)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t\t<AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n", prev,
         quick->GetLabelAnchorX());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n", prev,
         quick->GetLabelAnchorY());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t\t</AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
    }
  if (quick->GetLabelDisplacementX() != 0.0
      || quick->GetLabelDisplacementY() != 0.0)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t\t<Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
         quick->GetLabelDisplacementX());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
         quick->GetLabelDisplacementY());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t\t</Displacement>\r\n", prev);
      sqlite3_free(prev);
    }
  if (quick->GetLabelRotation() != 0.0)
    {
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<Rotation>%1.2f</Rotation>\r\n",
                        prev, quick->GetLabelRotation());
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t\t</PointPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t</LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  if (quick->IsHaloEnabled() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t<Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<Radius>%1.2f</Radius>\r\n", prev,
                        quick->GetHaloRadius());
      sqlite3_free(prev);

      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
         prev, quick->GetHaloColor());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, quick->GetHaloOpacity());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</Halo>\r\n", prev);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t<Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, quick->GetFontColor());
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
     prev, quick->GetFontOpacity());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t</Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</TextSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *MyLayerTree::PrintQuickStyleTextLine(QuickStyleObj * quick)
{
//
// printing an XML QuickStyle TEXT (LINE placement)
//
  char *xml;
  char *prev;
  const char *param;

  xml = sqlite3_mprintf("\t\t\t<TextSymbolizer>\r\n");
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t<Label>@%s@</Label>\r\n", prev,
                    quick->GetLabelColumn());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t<Font>\r\n", prev);
  sqlite3_free(prev);
  if (strcmp(quick->GetFontFacename(), "ToyFont: sans-serif") == 0)
    param = "sans serif";
  else if (strcmp(quick->GetFontFacename(), "ToyFont: serif") == 0)
    param = "serif";
  else if (strcmp(quick->GetFontFacename(), "ToyFont: monospace") == 0)
    param = "monospace";
  else
    param = quick->GetFontFacename();
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-family\">%s</SvgParameter>\r\n",
     prev, param);
  sqlite3_free(prev);
  if (quick->GetFontStyle() == RL2_FONTSTYLE_ITALIC)
    param = "italic";
  else if (quick->GetFontStyle() == RL2_FONTSTYLE_OBLIQUE)
    param = "oblique";
  else
    param = "normal";
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-style\">%s</SvgParameter>\r\n",
     prev, param);
  sqlite3_free(prev);
  if (quick->GetFontWeight() == RL2_FONTWEIGHT_BOLD)
    param = "bold";
  else
    param = "normal";
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-weight\">%s</SvgParameter>\r\n",
     prev, param);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"font-size\">%1.2f</SvgParameter>\r\n",
     prev, quick->GetFontSize());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t</Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t<LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t\t<LinePlacement>\r\n", prev);
  sqlite3_free(prev);
  if (quick->GetLabelPerpendicularOffset() != 0.0)
    {
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n",
         prev, quick->GetLabelPerpendicularOffset());
      sqlite3_free(prev);
    }
  if (quick->IsRepeatedLabel() == true)
    {
      // Repeated: InitialGap and Gap
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<IsRepeated>true</IsRepeated>\r\n",
                        prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<InitialGap>%1.4f</InitialGap>\r\n", prev,
         quick->GetLabelInitialGap());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<Gap>%1.4f</Gap>\r\n", prev,
                        quick->GetLabelGap());
      sqlite3_free(prev);
    }
  if (quick->IsLabelAligned() == true)
    {
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<IsAligned>true</IsAligned>\r\n", prev);
      sqlite3_free(prev);
    }
  if (quick->IsLabelGeneralizeLine() == true)
    {
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<GeneralizeLine>true</GeneralizeLine>\r\n", prev);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t\t</LinePlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t</LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  if (quick->IsHaloEnabled() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t<Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<Radius>%1.2f</Radius>\r\n", prev,
                        quick->GetHaloRadius());
      sqlite3_free(prev);

      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
         prev, quick->GetHaloColor());
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, quick->GetHaloOpacity());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</Halo>\r\n", prev);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t<Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, quick->GetFontColor());
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
     prev, quick->GetFontOpacity());
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t</Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</TextSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *MyLayerTree::PrintQuickStyleRaster(QuickStyleRasterObj * quick)
{
//
// printing an XML QuickStyle (RASTER)
//
  char *xml = NULL;
  char *prev;
  const char *extra = "";

  if (quick->IsMinScaleEnabled() == false && quick->IsMaxScaleEnabled() == false
      && quick->GetOpacity() == 1.0 && quick->IsNormalize() == false
      && quick->IsHistogram() == false && quick->IsGamma() == false
      && quick->IsTripleBand() == false && quick->IsSingleBand() == false
      && quick->GetColorMap() == COLOR_MAP_NONE
      && quick->IsNdviColorMap() == false && quick->IsNdwiColorMap() == false
      && quick->IsColorRamp() == false && quick->IsShadedRelief() == false)
    return NULL;                // NULL Raster Style - no option is set

  if (quick->IsMinScaleEnabled() == true || quick->IsMaxScaleEnabled() == true)
    {
      xml = sqlite3_mprintf("\t\t\t<Rule>\r\n");
      if (quick->IsMinScaleEnabled() == true)
        {
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n",
             prev, quick->GetScaleMin());
          sqlite3_free(prev);
        }
      if (quick->IsMaxScaleEnabled() == true)
        {
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n",
             prev, quick->GetScaleMax());
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</Rule>\r\n", prev);
      sqlite3_free(prev);
    }
  if (xml == NULL)
    xml = sqlite3_mprintf("\t\t\t<RasterSymbolizer>\r\n");
  else
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t<RasterSymbolizer>\r\n", prev);
      sqlite3_free(prev);
    }
  extra = "\t";
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev,
                    extra, quick->GetOpacity());
  sqlite3_free(prev);
  if (quick->GetColorMap() == COLOR_MAP_ETOPO2)
    {
      // special case: predefined Etopo2 style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"etopo2\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_SRTM)
    {
      // special case: predefined SRTM style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"srtm\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_TERRAIN)
    {
      // special case: predefined Terrain style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"terrain\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_ELEVATION)
    {
      // special case: predefined Elevation style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"elevation\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_ASPECT_COLOR)
    {
      // special case: predefined AspectColor style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"aspect_color\" />\r\n",
                        prev, extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_RAINBOW)
    {
      // special case: predefined Rainbow style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"rainbow\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_WAVE)
    {
      // special case: predefined Wave style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"wave\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->GetColorMap() == COLOR_MAP_SEPIA)
    {
      // special case: predefined Sepia style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"sepia\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto shaded_relief;
    }
  if (quick->IsNdviColorMap() == true)
    {
      // special case: predefined NDVI style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"ndvi\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto foot;
    }
  if (quick->IsNdwiColorMap() == true)
    {
      // special case: predefined NDWI style
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<ColorMap name=\"ndwi\" />\r\n", prev,
                        extra);
      sqlite3_free(prev);
      goto foot;
    }
  if (quick->IsTripleBand() == true || quick->IsSingleBand() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<ChannelSelection>\r\n", prev, extra);
      sqlite3_free(prev);
      if (quick->IsTripleBand() == true)
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t<RedChannel>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SourceChannelName>%d</SourceChannelName>\r\n",
             prev, extra, quick->GetRedBand() + 1);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t</RedChannel>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t<GreenChannel>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SourceChannelName>%d</SourceChannelName>\r\n",
             prev, extra, quick->GetGreenBand() + 1);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t</GreenChannel>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t<BlueChannel>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SourceChannelName>%d</SourceChannelName>\r\n",
             prev, extra, quick->GetBlueBand() + 1);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t</BlueChannel>\r\n", prev, extra);
          sqlite3_free(prev);
        }
      if (quick->IsSingleBand() == true)
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t<GrayChannel>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<SourceChannelName>%d</SourceChannelName>\r\n",
             prev, extra, quick->GetGrayBand() + 1);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t</GrayChannel>\r\n", prev, extra);
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</ChannelSelection>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  if (quick->IsNormalize() == true || quick->IsHistogram() == true
      || quick->IsGamma() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<ContrastEnhancement>\r\n", prev, extra);
      sqlite3_free(prev);
      if (quick->IsNormalize() == true)
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t<Normalize />\r\n", prev, extra);
          sqlite3_free(prev);
      } else if (quick->IsHistogram() == true)
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t<Histogram />\r\n", prev, extra);
          sqlite3_free(prev);
      } else if (quick->IsGamma() == true)
        {
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t<Gamma>%1.2f</Gamma>\r\n", prev,
                            extra, quick->GetGammaValue());
          sqlite3_free(prev);
        }
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t</ContrastEnhancement>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  if (quick->IsColorRamp() == true)
    {
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<ColorRamp MinValue=\"%1.8f\" MinColor=\"%s\" MaxValue=\"%1.8f\" MaxColor=\"%s\" />\r\n",
         prev, extra, quick->GetMinValue(), quick->GetMinValueColor(),
         quick->GetMaxValue(), quick->GetMaxValueColor());
      sqlite3_free(prev);
    }
shaded_relief:
  if (quick->IsShadedRelief() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t<ShadedRelief>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n", prev,
         extra, quick->GetShadedReliefFactor());
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</ShadedRelief>\r\n", prev, extra);
      sqlite3_free(prev);
    }
foot:
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</RasterSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  return xml;
}

char *MyLayerTree::PrintQuickStyleTopology(QuickStyleTopologyObj * quick,
                                           const char *indent)
{
//
// printing an XML QuickStyle (TOPOLOGY)
//
  char *xml = NULL;
  char *xml2;
  char *prev;
  if (quick->IsMinScaleEnabled() == true || quick->IsMaxScaleEnabled() == true)
    {
      xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
      prev = xml;
      if (quick->IsMinScaleEnabled() == true)
        {
          xml =
            sqlite3_mprintf
            ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
             indent, quick->GetScaleMin());
          sqlite3_free(prev);
          prev = xml;
        }
      if (quick->IsMaxScaleEnabled() == true)
        {
          xml =
            sqlite3_mprintf
            ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
             indent, quick->GetScaleMax());
          sqlite3_free(prev);
          prev = xml;
        }
      prev = xml;
      xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
      sqlite3_free(prev);
    }
  // Faces
  xml2 = quick->DoCreateFaceXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  // Edges
  xml2 = quick->DoCreateEdgeLinkXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  // Nodes
  xml2 = quick->DoCreateNodeXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  // EdgeSeeds
  xml2 = quick->DoCreateEdgeLinkSeedXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  // FaceSeeds
  xml2 = quick->DoCreateFaceSeedXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  return xml;
}

char *MyLayerTree::PrintQuickStyleNetwork(QuickStyleTopologyObj * quick,
                                          const char *indent)
{
//
// printing an XML QuickStyle (NETWORK)
//
  char *xml = NULL;
  char *xml2;
  char *prev;
  if (quick->IsMinScaleEnabled() == true || quick->IsMaxScaleEnabled() == true)
    {
      xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
      prev = xml;
      if (quick->IsMinScaleEnabled() == true)
        {
          xml =
            sqlite3_mprintf
            ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
             indent, quick->GetScaleMin());
          sqlite3_free(prev);
          prev = xml;
        }
      if (quick->IsMaxScaleEnabled() == true)
        {
          xml =
            sqlite3_mprintf
            ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
             indent, quick->GetScaleMax());
          sqlite3_free(prev);
          prev = xml;
        }
      prev = xml;
      xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
      sqlite3_free(prev);
    }
  // Links
  xml2 = quick->DoCreateEdgeLinkXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  // Nodes
  xml2 = quick->DoCreateNodeXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  // LinkSeeds
  xml2 = quick->DoCreateEdgeLinkSeedXML(indent);
  if (xml2 != NULL)
    {
      if (xml == NULL)
        xml = xml2;
      else
        {
          prev = xml;
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
    }
  return xml;
}

MapLayer::MapLayer(const void *priv_data, LayerListItem * layer)
{
//
// ctor
//
  int len;
  const char *value;
  PrivData = priv_data;
  Type = layer->GetLayerType();
  WmsConfig = NULL;
  RasterConfig = NULL;
  VectorConfig = NULL;
  if (Type == MAP_LAYER_WMS)
    WmsConfig = new WmsLayerSettings();
  if (Type == MAP_LAYER_RASTER)
    RasterConfig = new RasterLayerConfig();
  if (Type == MAP_LAYER_VECTOR || Type == MAP_LAYER_VECTOR_VIEW
      || Type == MAP_LAYER_VECTOR_VIRTUAL || Type == MAP_LAYER_TOPOLOGY
      || Type == MAP_LAYER_NETWORK)
    {
      int type;
      if (Type == MAP_LAYER_TOPOLOGY || Type == MAP_LAYER_NETWORK)
        type = Type;
      else
        {
          switch (layer->GetGeometryType())
            {
              case 1:
              case 4:
              case 1001:
              case 1004:
              case 2001:
              case 2004:
              case 3001:
              case 3004:
                type = QUICK_STYLE_POINT;
                break;
              case 2:
              case 5:
              case 1002:
              case 1005:
              case 2002:
              case 2005:
              case 3002:
              case 3005:
                type = QUICK_STYLE_LINE;
                break;
              case 3:
              case 6:
              case 1003:
              case 1006:
              case 2003:
              case 2006:
              case 3003:
              case 3006:
                type = QUICK_STYLE_POLYGON;
                break;
              default:
                type = QUICK_STYLE_GEOMETRY;
                break;
            };
        }
      VectorConfig = new VectorLayerConfig(type);
    }
  DbPrefix = layer->GetDbPrefix();
  VectorPrefix = layer->GetLayerPrefix();
  Name = layer->GetLayerName();
  Title = layer->GetTitle();
  Abstract = layer->GetAbstract();
  Copyright = layer->GetCopyright();
  DataLicense = layer->GetDataLicense();
  value = layer->GetTableName();
  if (value == NULL)
    f_table_name = NULL;
  else
    {
      len = strlen(value);
      f_table_name = (char *) malloc(len + 1);
      strcpy(f_table_name, value);
    }
  value = layer->GetGeometryColumn();
  if (value == NULL)
    f_geometry_column = NULL;
  else
    {
      len = strlen(value);
      f_geometry_column = (char *) malloc(len + 1);
      strcpy(f_geometry_column, value);
    }
  value = layer->GetViewMotherName();
  if (value == NULL)
    view_table_name = NULL;
  else
    {
      len = strlen(value);
      view_table_name = (char *) malloc(len + 1);
      strcpy(view_table_name, value);
    }
  value = layer->GetViewMotherGeometry();
  if (value == NULL)
    view_geometry_column = NULL;
  else
    {
      len = strlen(value);
      view_geometry_column = (char *) malloc(len + 1);
      strcpy(view_geometry_column, value);
    }
  value = layer->GetViewRowidColumn();
  if (value == NULL)
    view_rowid_column = NULL;
  else
    {
      len = strlen(value);
      view_rowid_column = (char *) malloc(len + 1);
      strcpy(view_rowid_column, value);
    }
  value = layer->GetTopologyName();
  if (value == NULL)
    topology_name = NULL;
  else
    {
      len = strlen(value);
      topology_name = (char *) malloc(len + 1);
      strcpy(topology_name, value);
    }
  value = layer->GetNetworkName();
  if (value == NULL)
    network_name = NULL;
  else
    {
      len = strlen(value);
      network_name = (char *) malloc(len + 1);
      strcpy(network_name, value);
    }
  getMap_url = NULL;
  getFeatureInfo_url = NULL;
  GeometryType = layer->GetGeometryType();
  HasZ = layer->GetHasZ();
  NativeSRID = layer->GetSRID();
  AutoTransformEnabled = true;
  MapSRID = NativeSRID;
  GeoMinX = DBL_MAX;
  GeoMinY = DBL_MAX;
  GeoMaxX = -DBL_MAX;
  GeoMaxY = -DBL_MAX;
  MinX = DBL_MAX;
  MinY = DBL_MAX;
  MaxX = -DBL_MAX;
  MaxY = -DBL_MAX;
  MapMinX = DBL_MAX;
  MapMinY = DBL_MAX;
  MapMaxX = -DBL_MAX;
  MapMaxY = -DBL_MAX;
  Queryable = layer->IsQueryable();
  Editable = layer->IsEditable();
  SpatialIndex = layer->HasSpatialIndex();
  Visible = true;
  CachedBase = new CachedFrame(PrivData);
  CachedLabels = new CachedFrame(PrivData);
  CachedNodes = NULL;
  CachedEdges = NULL;
  CachedLinks = NULL;
  CachedFaces = NULL;
  CachedEdgeSeeds = NULL;
  CachedLinkSeeds = NULL;
  CachedFaceSeeds = NULL;
  if (Type == MAP_LAYER_TOPOLOGY)
    {
      CachedNodes = new CachedFrame(PrivData);
      CachedEdges = new CachedFrame(PrivData);
      CachedFaces = new CachedFrame(PrivData);
      CachedEdgeSeeds = new CachedFrame(PrivData);
      CachedFaceSeeds = new CachedFrame(PrivData);
    }
  if (Type == MAP_LAYER_NETWORK)
    {
      CachedNodes = new CachedFrame(PrivData);
      CachedLinks = new CachedFrame(PrivData);
      CachedLinkSeeds = new CachedFrame(PrivData);
    }
  QuickStyle = NULL;
  QuickStyleTopology = NULL;
  QuickStyleRaster = NULL;
  QuickStyleWms = NULL;
  Prev = NULL;
  Next = NULL;
}

MapLayer::~MapLayer()
{
//
// dtor
//
  if (WmsConfig != NULL)
    delete WmsConfig;
  if (RasterConfig != NULL)
    delete RasterConfig;
  if (VectorConfig != NULL)
    delete VectorConfig;
  if (f_table_name != NULL)
    free(f_table_name);
  if (f_geometry_column != NULL)
    free(f_geometry_column);
  if (view_table_name != NULL)
    free(view_table_name);
  if (view_geometry_column != NULL)
    free(view_geometry_column);
  if (view_rowid_column != NULL)
    free(view_rowid_column);
  if (topology_name != NULL)
    free(topology_name);
  if (network_name != NULL)
    free(network_name);
  if (getMap_url != NULL)
    free(getMap_url);
  if (getFeatureInfo_url != NULL)
    free(getFeatureInfo_url);
  if (CachedBase != NULL)
    delete CachedBase;
  if (CachedLabels != NULL)
    delete CachedLabels;
  if (CachedNodes != NULL)
    delete CachedNodes;
  if (CachedEdges != NULL)
    delete CachedEdges;
  if (CachedLinks != NULL)
    delete CachedLinks;
  if (CachedFaces != NULL)
    delete CachedFaces;
  if (CachedEdgeSeeds != NULL)
    delete CachedEdgeSeeds;
  if (CachedLinkSeeds != NULL)
    delete CachedLinkSeeds;
  if (CachedFaceSeeds != NULL)
    delete CachedFaceSeeds;
  if (QuickStyle != NULL)
    delete QuickStyle;
  if (QuickStyleTopology != NULL)
    delete QuickStyleTopology;
  if (QuickStyleRaster != NULL)
    delete QuickStyleRaster;
  if (QuickStyleWms != NULL)
    delete QuickStyleWms;
}

void MapLayer::Invalidate()
{
//
// the Style changed: invalidating all Caches
//
  if (CachedBase != NULL)
    CachedBase->Invalidate();
  if (CachedLabels != NULL)
    CachedLabels->Invalidate();
  if (CachedNodes != NULL)
    CachedNodes->Invalidate();
  if (CachedEdges != NULL)
    CachedEdges->Invalidate();
  if (CachedLinks != NULL)
    CachedLinks->Invalidate();
  if (CachedFaces != NULL)
    CachedFaces->Invalidate();
  if (CachedEdgeSeeds != NULL)
    CachedEdgeSeeds->Invalidate();
  if (CachedLinkSeeds != NULL)
    CachedLinkSeeds->Invalidate();
  if (CachedFaceSeeds != NULL)
    CachedFaceSeeds->Invalidate();
}

void MapLayer::SetQuickStyle(QuickStyleObj * quickStyle)
{
//
// setting a Vector QuickStyle
//
  if (QuickStyle != NULL)
    delete QuickStyle;
  QuickStyle = quickStyle;
}

void MapLayer::SetQuickStyle(QuickStyleTopologyObj * quickStyle)
{
//
// setting a Topology QuickStyle
//
  if (QuickStyleTopology != NULL)
    delete QuickStyleTopology;
  QuickStyleTopology = quickStyle;
}

void MapLayer::SetQuickStyle(QuickStyleRasterObj * quickStyle)
{
//
// setting a Raster QuickStyle
//
  if (QuickStyleRaster != NULL)
    delete QuickStyleRaster;
  QuickStyleRaster = quickStyle;
}

void MapLayer::SetQuickStyle(QuickStyleWmsObj * quickStyle)
{
//
// setting a WMS QuickStyle
//
  if (QuickStyleWms != NULL)
    delete QuickStyleWms;
  QuickStyleWms = quickStyle;
}

bool MapLayer::UpdateQuickStyle(QuickStyleObj * style)
{
//
// updating the current Quick Syle
//
  bool ret;
  if (QuickStyle == NULL)
    {
      QuickStyle = style->Clone();
      Invalidate();
      return true;
  } else
    {
      ret = style->Compare(QuickStyle);
      if (ret == false)
        {
          if (QuickStyle != NULL)
            delete QuickStyle;
          QuickStyle = style->Clone();
          Invalidate();
          return true;
        }
    }
  return false;
}

bool MapLayer::UpdateQuickStyle(QuickStyleTopologyObj * style)
{
//
// updating the current Quick Syle
//
  bool ret;
  if (QuickStyleTopology == NULL)
    {
      QuickStyleTopology = style->Clone();
      Invalidate();
      return true;
  } else
    {
      ret = style->Compare(QuickStyleTopology);
      if (ret == false)
        {
          if (QuickStyleTopology != NULL)
            delete QuickStyleTopology;
          QuickStyleTopology = style->Clone();
          Invalidate();
          return true;
        }
    }
  return false;
}

bool MapLayer::UpdateQuickStyle(QuickStyleRasterObj * style)
{
//
// updating the current Quick Syle
//
  bool ret;
  if (QuickStyleRaster == NULL)
    {
      QuickStyleRaster = style->Clone();
      QuickStyleRaster->UpdateXmlStyle();
      Invalidate();
      return true;
  } else
    {
      ret = style->Compare(QuickStyleRaster);
      if (ret == false)
        {
          if (QuickStyleRaster != NULL)
            delete QuickStyleRaster;
          QuickStyleRaster = style->Clone();
          QuickStyleRaster->UpdateXmlStyle();
          Invalidate();
          return true;
        }
    }
  return false;
}

bool MapLayer::UpdateQuickStyle(QuickStyleWmsObj * style)
{
//
// updating the current Quick Syle
//
  bool ret;
  if (QuickStyleWms == NULL)
    {
      QuickStyleWms = style->Clone();
      Invalidate();
      return true;
  } else
    {
      ret = style->Compare(QuickStyleWms);
      if (ret == false)
        {
          if (QuickStyleWms != NULL)
            delete QuickStyleWms;
          QuickStyleWms = style->Clone();
          Invalidate();
          return true;
        }
    }
  return false;
}

void MapLayer::PrepareGraphicsContext(int width, int height, double minx,
                                      double miny, double maxx, double maxy,
                                      const char *style)
{
//
// initializing the Graphics Context
//
  if (Type == MAP_LAYER_TOPOLOGY)
    {
      // Topology extra graphics
      if (CachedNodes->IsValid(width, height, minx, miny, maxx, maxy, style) !=
          true)
        CachedNodes->Reset(width, height, minx, miny, maxx, maxy, style);
      if (CachedEdges->IsValid(width, height, minx, miny, maxx, maxy, style) !=
          true)
        CachedEdges->Reset(width, height, minx, miny, maxx, maxy, style);
      if (CachedFaces->IsValid(width, height, minx, miny, maxx, maxy, style) !=
          true)
        CachedFaces->Reset(width, height, minx, miny, maxx, maxy, style);
      if (CachedEdgeSeeds->IsValid(width, height, minx, miny, maxx, maxy, style)
          != true)
        CachedEdgeSeeds->Reset(width, height, minx, miny, maxx, maxy, style);
      if (CachedFaceSeeds->IsValid(width, height, minx, miny, maxx, maxy, style)
          != true)
        CachedFaceSeeds->Reset(width, height, minx, miny, maxx, maxy, style);
    }
  if (Type == MAP_LAYER_NETWORK)
    {
      // Network extra graphics 
      if (CachedNodes->IsValid(width, height, minx, miny, maxx, maxy, style) !=
          true)
        CachedNodes->Reset(width, height, minx, miny, maxx, maxy, style);
      if (CachedLinks->IsValid(width, height, minx, miny, maxx, maxy, style) !=
          true)
        CachedLinks->Reset(width, height, minx, miny, maxx, maxy, style);
      if (CachedLinkSeeds->IsValid(width, height, minx, miny, maxx, maxy, style)
          != true)
        CachedLinkSeeds->Reset(width, height, minx, miny, maxx, maxy, style);
    }
  if (Type == MAP_LAYER_VECTOR || Type == MAP_LAYER_VECTOR_VIEW
      || Type == MAP_LAYER_VECTOR_VIRTUAL || Type == MAP_LAYER_TOPOLOGY
      || Type == MAP_LAYER_NETWORK)
    {
      // Labels
      if (CachedLabels->IsValid(width, height, minx, miny, maxx, maxy, style) !=
          true)
        CachedLabels->Reset(width, height, minx, miny, maxx, maxy, style);
    }
// base graphics context
  if (CachedBase->IsValid(width, height, minx, miny, maxx, maxy, style) != true)
    CachedBase->Reset(width, height, minx, miny, maxx, maxy, style);
}

rl2GraphicsContextPtr MapLayer::GetGraphicsContext()
{
//
// returning the Base Graphics Context
//
  if (CachedBase == NULL)
    return NULL;
  return CachedBase->GetGraphicsContext();
}

rl2GraphicsContextPtr MapLayer::GetLabelsGraphicsContext()
{
//
// returning the Labels Graphics Context
//
  if (CachedLabels == NULL)
    return NULL;
  return CachedLabels->GetGraphicsContext();
}

void MapLayer::SetNativeSRID(const char *srs)
{
//
// setting default SRS for WMS Layer
//
  int srid = -1;
  const char *p = srs;

  if (Type != MAP_LAYER_WMS)
    return;
  if (strncasecmp(srs, "EPSG:", 5) == 0)
    p = srs + 5;
  srid = atoi(p);
  NativeSRID = srid;
}

bool MapLayer::DoCheckSupportedSRID(sqlite3 * sqlite, int srid)
{
//
// checking if some SRID is actually supported by this MapLayer
//
  int i;
  char **results;
  int rows;
  int columns;
  char *value;
  char *sql;
  char cvg[1024];
  char db_prefix[1024];
  char *qprefix;
  bool result = false;

  wxCharBuffer buffer = DbPrefix.ToUTF8();
  sprintf(db_prefix, "%s", buffer.data());
  qprefix = gaiaDoubleQuotedSql(db_prefix);
  buffer = Name.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  if (Type == MAP_LAYER_VECTOR || Type == MAP_LAYER_VECTOR_VIEW
      || Type == MAP_LAYER_VECTOR_VIRTUAL || Type == MAP_LAYER_TOPOLOGY
      || Type == MAP_LAYER_NETWORK)
    {
      sql =
        sqlite3_mprintf("SELECT Count(*) FROM \"%s\".vector_coverages_ref_sys "
                        "WHERE coverage_name = %Q AND srid = %d AND "
                        "geo_minx IS NOT NULL AND geo_miny IS NOT NULL AND "
                        "geo_maxx IS NOT NULL AND geo_maxy IS NOT NULL AND "
                        "extent_minx IS NOT NULL AND extent_miny IS NOT NULL AND "
                        "extent_maxx IS NOT NULL AND extent_maxy IS NOT NULL",
                        qprefix, cvg, srid);
      free(qprefix);
  } else if (Type == MAP_LAYER_RASTER)
    {
      sql =
        sqlite3_mprintf("SELECT Count(*) FROM \"%s\".raster_coverages_ref_sys "
                        "WHERE coverage_name = %Q AND srid = %d AND "
                        "geo_minx IS NOT NULL AND geo_miny IS NOT NULL AND "
                        "geo_maxx IS NOT NULL AND geo_maxy IS NOT NULL AND "
                        "extent_minx IS NOT NULL AND extent_miny IS NOT NULL AND "
                        "extent_maxx IS NOT NULL AND extent_maxy IS NOT NULL",
                        qprefix, cvg, srid);
      free(qprefix);
  } else if (Type == MAP_LAYER_WMS)
    {
      sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".wms_getmap AS g "
                            "JOIN \"%s\".wms_ref_sys AS s ON (g.id = s.parent_id) "
                            "WHERE g.layer_name = %Q AND s.srs = 'EPSG:%d'",
                            qprefix, qprefix, cvg, srid);
      free(qprefix);
  } else
    return false;

  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          if (atoi(value) == 1)
            result = true;
        }
    }
  sqlite3_free_table(results);
  return result;
}

void MapLayer::UpdateMapExtent(sqlite3 * sqlite)
{
//
// updating the layer's full extent - Map coords
//
  sqlite3_stmt *stmt = NULL;
  int ret;
  char *sql;
  char cvg[1024];
  char db_prefix[1024];
  char *qprefix;

  MapMinX = DBL_MAX;
  MapMinY = DBL_MAX;
  MapMaxX = -DBL_MAX;
  MapMaxY = -DBL_MAX;

  if (NativeSRID == MapSRID)
    {
      MapMinX = MinX;
      MapMinY = MinY;
      MapMaxX = MaxX;
      MapMaxY = MaxY;
      return;
    }

  wxCharBuffer buffer = DbPrefix.ToUTF8();
  sprintf(db_prefix, "%s", buffer.data());
  qprefix = gaiaDoubleQuotedSql(db_prefix);
  buffer = Name.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  if (Type == MAP_LAYER_VECTOR || Type == MAP_LAYER_VECTOR_VIEW
      || Type == MAP_LAYER_VECTOR_VIRTUAL || Type == MAP_LAYER_TOPOLOGY
      || Type == MAP_LAYER_NETWORK)
    {
      sql =
        sqlite3_mprintf
        ("SELECT extent_minx, extent_miny, extent_maxx, extent_maxy "
         "FROM \"%s\".vector_coverages_srid "
         "WHERE coverage_name = %Q AND srid = %d", qprefix, cvg, MapSRID);
      free(qprefix);
  } else if (Type == MAP_LAYER_RASTER)
    {
      sql =
        sqlite3_mprintf
        ("SELECT extent_minx, extent_miny, extent_maxx, extent_maxy "
         "FROM \"%s\".raster_coverages_srid "
         "WHERE coverage_name = %Q AND srid = %d", qprefix, cvg, MapSRID);
      free(qprefix);
  } else if (Type == MAP_LAYER_WMS)
    {
      sql = sqlite3_mprintf("SELECT minx, miny, maxx, maxy "
                            "FROM \"%s\".wms_getmap AS g "
                            "JOIN \"%s\".wms_ref_sys AS s ON (g.id = s.parent_id) "
                            "WHERE g.layer_name = %Q AND s.srs = 'EPSG:%d'",
                            qprefix, qprefix, cvg, MapSRID);
      free(qprefix);
  } else
    return;

  ret = sqlite3_prepare_v2(sqlite, sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;
  while (1)
    {
      // scrolling the result set rows 
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          MapMinX = sqlite3_column_double(stmt, 0);
          MapMinY = sqlite3_column_double(stmt, 1);
          MapMaxX = sqlite3_column_double(stmt, 2);
          MapMaxY = sqlite3_column_double(stmt, 3);
        }
    }
  sqlite3_finalize(stmt);
}

void MapLayer::AdjustMapSRID(sqlite3 * sqlite, MyFrame * MainFrame,
                             bool map_auto_transform_enabled, int srid)
{
//
// adjusting the Layer's MapSrid
//
  if (AutoTransformEnabled == false)
    return;
  if (map_auto_transform_enabled == false)
    return;

  char db[1024];
  char cov[1024];
  strcpy(db, DbPrefix.ToUTF8());
  strcpy(cov, Name.ToUTF8());

  if (DoCheckSupportedSRID(sqlite, srid) == true)
    MapSRID = srid;
  else
    MapSRID = NativeSRID;
  if (WmsConfig != NULL)
    WmsConfig->AdjustMapSRID(sqlite, MainFrame, getMap_url, DbPrefix, Name,
                             srid);
  if (MapSRID == NativeSRID)
    {
      MapMinX = MinX;
      MapMinY = MinY;
      MapMaxX = MaxX;
      MapMaxY = MaxY;
  } else
    UpdateMapExtent(sqlite);
}

void MapLayer::AdjustDefaultStyle(sqlite3 * sqlite)
{
//
// adjusting the Layer's Style
//
  if (VectorConfig != NULL)
    {
      char *style = DoFindVectorStyle(sqlite);
      if (style != NULL)
        {
          VectorConfig->SetStyle(style);
          free(style);
        }
      if (VectorConfig->GetStyle() == NULL)
        {
          // creating a Quick Style
          CreateQuickStyle();
        }
    }
  if (RasterConfig != NULL)
    {
      char *style = DoFindRasterStyle(sqlite);
      if (style != NULL)
        {
          RasterConfig->SetStyle(style);
          free(style);
        }
    }
}

void MapLayer::CreateQuickStyle()
{
// 
// creating a new Quick Style for this Map Layer
//
  if (Type == MAP_LAYER_TOPOLOGY || Type == MAP_LAYER_NETWORK)
    {
      if (QuickStyleTopology != NULL)
        delete QuickStyleTopology;
      QuickStyleTopology = new QuickStyleTopologyObj(Type);
      if (QuickStyleTopology != NULL)
        VectorConfig->SetStyle(QuickStyleTopology->GetUUID());
  } else
    {
      if (QuickStyle != NULL)
        delete QuickStyle;
      QuickStyle = new QuickStyleObj(VectorConfig->GetGeometryType());
      if (QuickStyle != NULL)
        VectorConfig->SetStyle(QuickStyle->GetUUID());
    }
}

char *MapLayer::DoFindVectorStyle(sqlite3 * sqlite)
{
//
// fetching the first available Style
//
  int i;
  char **results;
  int rows;
  int columns;
  char *value;
  char *sql;
  char cvg[1024];
  char db_prefix[1024];
  char *qprefix;
  char *style = NULL;

  wxCharBuffer buffer = DbPrefix.ToUTF8();
  sprintf(db_prefix, "%s", buffer.data());
  qprefix = gaiaDoubleQuotedSql(db_prefix);
  buffer = Name.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf("SELECT name FROM \"%s\".SE_vector_styled_layers_view "
                    "WHERE coverage_name = %Q LIMIT 1", qprefix, cvg);
  free(qprefix);

  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return NULL;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          int len;
          value = results[(i * columns) + 0];
          len = strlen(value);
          style = (char *) malloc(len + 1);
          strcpy(style, value);

        }
    }
  sqlite3_free_table(results);
  return style;
}

char *MapLayer::DoFindRasterStyle(sqlite3 * sqlite)
{
//
// fetching the first available Style
//
  int i;
  char **results;
  int rows;
  int columns;
  char *value;
  char *sql;
  char cvg[1024];
  char db_prefix[1024];
  char *qprefix;
  char *style = NULL;

  wxCharBuffer buffer = DbPrefix.ToUTF8();
  sprintf(db_prefix, "%s", buffer.data());
  qprefix = gaiaDoubleQuotedSql(db_prefix);
  buffer = Name.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf("SELECT name FROM \"%s\".SE_raster_styled_layers_view "
                    "WHERE coverage_name = %Q LIMIT 1", qprefix, cvg);
  free(qprefix);

  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return NULL;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          int len;
          value = results[(i * columns) + 0];
          len = strlen(value);
          style = (char *) malloc(len + 1);
          strcpy(style, value);

        }
    }
  sqlite3_free_table(results);
  return style;
}

bool MapLayer::IsValidMapExtent()
{
// testing for a valid full extent - Map Coords
  if (MapMinX == DBL_MAX)
    return false;
  if (MapMinY == DBL_MAX)
    return false;
  if (MapMaxX == -DBL_MAX)
    return false;
  if (MapMaxY == -DBL_MAX)
    return false;
  return true;
}

void MapLayer::SetWmsGetMapURL(const char *url)
{
//
// setting the WMS GetMap URL
//
  int len;
  if (getMap_url != NULL)
    free(getMap_url);
  if (url == NULL)
    getMap_url = NULL;
  else
    {
      len = strlen(url);
      getMap_url = (char *) malloc(len + 1);
      strcpy(getMap_url, url);
    }
}

void MapLayer::SetWmsGetFeatureInfoURL(const char *url)
{
//
// setting the WMS GetFeatureInfo URL
//
  int len;
  if (getFeatureInfo_url != NULL)
    free(getFeatureInfo_url);
  if (url == NULL)
    getFeatureInfo_url = NULL;
  else
    {
      len = strlen(url);
      getFeatureInfo_url = (char *) malloc(len + 1);
      strcpy(getFeatureInfo_url, url);
    }
}

void MapLayer::SetRasterInfos(wxString & sample_type, wxString pixel_type,
                              int bands, wxString & compression, int quality,
                              int tile_width, int tile_height, double horz_res,
                              double vert_res)
{
//
// setting the Raster Coverage Infos
//
  RasterLayerConfig *config = GetRasterConfig();
  if (Type != MAP_LAYER_RASTER)
    return;
  if (config == NULL)
    return;
  config->SetConfig(sample_type, pixel_type, bands, compression, quality,
                    tile_width, tile_height, horz_res, vert_res);
}

void MapLayer::SetGeoExtent(double minx, double miny, double maxx, double maxy)
{
//
// setting the Geographic full extent
//
  GeoMinX = minx;
  GeoMinY = miny;
  GeoMaxX = maxx;
  GeoMaxY = maxy;
}

void MapLayer::SetExtent(double minx, double miny, double maxx, double maxy)
{
//
// setting the full extent - Native coords
//
  MinX = minx;
  MinY = miny;
  MaxX = maxx;
  MaxY = maxy;
}

bool MapLayer::IsReady()
{
//
// testing if the Layer has been rendered
//
  if (CachedBase == NULL)
    return true;
  else
    return CachedBase->IsOk();
}

rl2CanvasPtr MapLayer::CreateCanvas()
{
//
// creating an RL2 Canvas object supporting this Layer
//
  rl2GraphicsContextPtr ref_ctx = NULL;
  rl2GraphicsContextPtr ref_ctx_labels = NULL;
  rl2GraphicsContextPtr ref_ctx_nodes = NULL;
  rl2GraphicsContextPtr ref_ctx_edges = NULL;
  rl2GraphicsContextPtr ref_ctx_links = NULL;
  rl2GraphicsContextPtr ref_ctx_faces = NULL;
  rl2GraphicsContextPtr ref_ctx_edge_seeds = NULL;
  rl2GraphicsContextPtr ref_ctx_link_seeds = NULL;
  rl2GraphicsContextPtr ref_ctx_face_seeds = NULL;
  switch (Type)
    {
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
        if (CachedBase != NULL)
          ref_ctx = CachedBase->GetGraphicsContext();
        if (CachedLabels != NULL)
          ref_ctx_labels = CachedLabels->GetGraphicsContext();
        return rl2_create_vector_canvas(ref_ctx, ref_ctx_labels);
      case MAP_LAYER_RASTER:
        if (CachedBase != NULL)
          ref_ctx = CachedBase->GetGraphicsContext();
        return rl2_create_raster_canvas(ref_ctx);
      case MAP_LAYER_WMS:
        if (CachedBase != NULL)
          ref_ctx = CachedBase->GetGraphicsContext();
        return rl2_create_wms_canvas(ref_ctx);
      case MAP_LAYER_TOPOLOGY:
        if (CachedBase != NULL)
          ref_ctx = CachedBase->GetGraphicsContext();
        if (CachedLabels != NULL)
          ref_ctx_labels = CachedLabels->GetGraphicsContext();
        if (CachedNodes != NULL)
          ref_ctx_nodes = CachedNodes->GetGraphicsContext();
        if (CachedEdges != NULL)
          ref_ctx_edges = CachedEdges->GetGraphicsContext();
        if (CachedFaces != NULL)
          ref_ctx_faces = CachedFaces->GetGraphicsContext();
        if (CachedEdgeSeeds != NULL)
          ref_ctx_edge_seeds = CachedEdgeSeeds->GetGraphicsContext();
        if (CachedFaceSeeds != NULL)
          ref_ctx_face_seeds = CachedFaceSeeds->GetGraphicsContext();
        return rl2_create_topology_canvas(ref_ctx, ref_ctx_labels,
                                          ref_ctx_nodes, ref_ctx_edges,
                                          ref_ctx_faces, ref_ctx_edge_seeds,
                                          ref_ctx_face_seeds);
      case MAP_LAYER_NETWORK:
        if (CachedBase != NULL)
          ref_ctx = CachedBase->GetGraphicsContext();
        if (CachedLabels != NULL)
          ref_ctx_labels = CachedLabels->GetGraphicsContext();
        if (CachedNodes != NULL)
          ref_ctx_nodes = CachedNodes->GetGraphicsContext();
        if (CachedLinks != NULL)
          ref_ctx_links = CachedLinks->GetGraphicsContext();
        if (CachedLinkSeeds != NULL)
          ref_ctx_link_seeds = CachedLinkSeeds->GetGraphicsContext();
        return rl2_create_network_canvas(ref_ctx, ref_ctx_labels, ref_ctx_nodes,
                                         ref_ctx_links, ref_ctx_link_seeds);
    };
  return NULL;
}

void MapLayer::Validate()
{
// validating the Cached Frames status
  if (Type == MAP_LAYER_WMS)
    {
      if (CachedBase != NULL)
        CachedBase->Validate();
    }
}

void MapLayer::Validate(rl2CanvasPtr canvas)
{
// validating the Cached Frames status
  switch (Type)
    {
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
      case MAP_LAYER_RASTER:
        if (CachedBase != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_BASE_CTX) == RL2_TRUE)
              CachedBase->Validate();
          }
        break;
      case MAP_LAYER_TOPOLOGY:
        if (CachedBase != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_BASE_CTX) == RL2_TRUE)
              CachedBase->Validate();
          }
        if (CachedNodes != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_NODES_CTX) == RL2_TRUE)
              CachedNodes->Validate();
          }
        if (CachedEdges != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_EDGES_CTX) == RL2_TRUE)
              CachedEdges->Validate();
          }
        if (CachedFaces != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_FACES_CTX) == RL2_TRUE)
              CachedFaces->Validate();
          }
        if (CachedEdgeSeeds != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_EDGE_SEEDS_CTX) ==
                RL2_TRUE)
              CachedEdgeSeeds->Validate();
          }
        if (CachedFaceSeeds != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_FACE_SEEDS_CTX) ==
                RL2_TRUE)
              CachedFaceSeeds->Validate();
          }
        break;
      case MAP_LAYER_NETWORK:
        if (CachedBase != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_BASE_CTX) == RL2_TRUE)
              CachedBase->Validate();
          }
        if (CachedNodes != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_NODES_CTX) == RL2_TRUE)
              CachedNodes->Validate();
          }
        if (CachedLinks != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_LINKS_CTX) == RL2_TRUE)
              CachedLinks->Validate();
          }
        if (CachedLinkSeeds != NULL)
          {
            if (rl2_is_canvas_ready(canvas, RL2_CANVAS_LINK_SEEDS_CTX) ==
                RL2_TRUE)
              CachedLinkSeeds->Validate();
          }
        break;
    };
}

WmsLayerSettings::WmsLayerSettings()
{
// ctor
  Version = NULL;
  RefSys = NULL;
  Style = NULL;
  ImageFormat = NULL;
  BgColor = NULL;
  BgColorEnabled = false;
  Opaque = 0;
  SwapXY = 0;
  Cached = 0;
  Tiled = 0;
  TileWidth = 0;
  TileHeight = 0;
}

WmsLayerSettings::~WmsLayerSettings()
{
// dtor
  if (Version != NULL)
    free(Version);
  if (RefSys != NULL)
    free(RefSys);
  if (Style != NULL)
    free(Style);
  if (ImageFormat != NULL)
    free(ImageFormat);
  if (BgColor != NULL)
    free(BgColor);
}

void WmsLayerSettings::SetVersion(const char *version)
{
  int len;
  if (Version != NULL)
    free(Version);
  if (version == NULL)
    {
      Version = NULL;
      return;
    }
  len = strlen(version);
  Version = (char *) malloc(len + 1);
  strcpy(Version, version);
}

void WmsLayerSettings::SetRefSys(const char *ref_sys)
{
  int len;
  if (RefSys != NULL)
    free(RefSys);
  if (ref_sys == NULL)
    {
      RefSys = NULL;
      return;
    }
  len = strlen(ref_sys);
  RefSys = (char *) malloc(len + 1);
  strcpy(RefSys, ref_sys);
}

void WmsLayerSettings::AdjustMapSRID(sqlite3 * sqlite, MyFrame * MainFrame,
                                     const char *url, wxString & dbprefix,
                                     wxString & layername, int srid)
{
// adjusting the Map SRID
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int count = 0;

  char *db_prefix = (char *) malloc((dbprefix.Len() * 4) + 1);
  strcpy(db_prefix, dbprefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((layername.Len() * 4) + 1);
  strcpy(layer, layername.ToUTF8());
  sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_ref_sys AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.srs = 'EPSG:%d'",
                        xdb_prefix, xdb_prefix, url, layer, srid);
  free(layer);
  free(xdb_prefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    count = atoi(results[(i * columns) + 0]);
  sqlite3_free_table(results);

  if (count != 1)
    return;

  char *ref_sys = sqlite3_mprintf("EPSG:%d", srid);
  SetRefSys(ref_sys);
  if (MainFrame->HasFlippedAxes(ref_sys) == true)
    SwapXY = 1;
  else
    SwapXY = 0;
  sqlite3_free(ref_sys);
}

void WmsLayerSettings::SetStyle(const char *style)
{
  int len;
  if (Style != NULL)
    free(Style);
  if (style == NULL)
    {
      Style = NULL;
      return;
    }
  len = strlen(style);
  Style = (char *) malloc(len + 1);
  strcpy(Style, style);
}

void WmsLayerSettings::SetImageFormat(const char *image_format)
{
  int len;
  if (ImageFormat != NULL)
    free(ImageFormat);
  if (image_format == NULL)
    {
      ImageFormat = NULL;
      return;
    }
  len = strlen(image_format);
  ImageFormat = (char *) malloc(len + 1);
  strcpy(ImageFormat, image_format);
}

void WmsLayerSettings::SetBgColor(const char *color)
{
  int len;
  if (BgColor != NULL)
    free(BgColor);
  if (color == NULL)
    {
      BgColor = NULL;
      return;
    }
  len = strlen(color);
  BgColor = (char *) malloc(len + 1);
  strcpy(BgColor, color);
}

RasterLayerConfig::RasterLayerConfig()
{
// ctor
  NumBands = -1;
  Quality = -1;
  TileWidth = -1;
  TileHeight = -1;
  HorzResolution = -1.0;
  VertResolution = -1.0;
  Style = NULL;
}

RasterLayerConfig::~RasterLayerConfig()
{
// dtor
  if (Style != NULL)
    free(Style);
}

void RasterLayerConfig::SetConfig(wxString & sample_type, wxString & pixel_type,
                                  int bands, wxString & compression,
                                  int quality, int tile_width, int tile_height,
                                  double horz_res, double vert_res)
{
//
// setting the Raster Coverage Infos
//
  SampleType = sample_type;
  PixelType = pixel_type;
  NumBands = bands;
  Compression = compression;
  Quality = quality;
  TileWidth = tile_width;
  TileHeight = tile_height;
  HorzResolution = horz_res;
  VertResolution = vert_res;
}

void RasterLayerConfig::SetStyle(const char *style)
{
  int len;
  if (Style != NULL)
    free(Style);
  if (style == NULL)
    {
      Style = NULL;
      return;
    }
  len = strlen(style);
  Style = (char *) malloc(len + 1);
  strcpy(Style, style);
}

VectorLayerConfig::VectorLayerConfig(int type)
{
// ctor
  GeometryType = type;
  Style = NULL;
  ShowFaces = false;
  ShowEdges = true;
  ShowNodes = true;
  ShowFaceSeeds = true;
  ShowEdgeSeeds = true;
  ShowLinks = true;
  ShowLinkSeeds = true;
}

VectorLayerConfig::~VectorLayerConfig()
{
// dtor
  if (Style != NULL)
    free(Style);
}

void VectorLayerConfig::SetStyle(const char *style)
{
  int len;
  if (Style != NULL)
    free(Style);
  if (style == NULL)
    {
      Style = NULL;
      return;
    }
  len = strlen(style);
  Style = (char *) malloc(len + 1);
  strcpy(Style, style);
}

MyMapPanelStatusBar::MyMapPanelStatusBar(MyMapPanel * parent):wxStatusBar(parent)
{
//
// ctor
//
  SetFieldsCount(5);
  int fldWidth[4];
  fldWidth[0] = -1;
  fldWidth[1] = -1;
  fldWidth[2] = -1;
  fldWidth[3] = 32;
  fldWidth[4] = -1;
  SetStatusWidths(5, fldWidth);
  Semaphore = new wxStaticBitmap(this, wxID_ANY, wxIcon());
// setting up the event handlers 
  Connect(wxID_ANY, wxEVT_SIZE,
          (wxObjectEventFunction) & MyMapPanelStatusBar::OnSize);
}

void MyMapPanelStatusBar::OnSize(wxSizeEvent & WXUNUSED(event))
{
//
// the Status Bar has changed its size
//
  wxRect rect;
  GetFieldRect(3, rect);
  wxSize size = Semaphore->GetSize();
  Semaphore->Move(rect.x + (rect.width - size.x) / 2,
                  rect.y + (rect.height - size.y) / 2);
}

void MyMapPanelStatusBar::SetRedLight()
{
// Red Light
  wxRect rect;
  GetFieldRect(3, rect);
  int w = 32;
  int h = 32;
  if (w > rect.width - 2)
    w = rect.width - 2;
  if (h > rect.height - 2)
    h = rect.height - 2;
  if (w > h)
    w = h;
  if (h > w)
    h = w;
  wxImage img = wxImage(red_light_xpm);
  wxImage icon = img.Rescale(w, h);
  Semaphore->SetBitmap(icon);
}

void MyMapPanelStatusBar::SetGreenLight()
{
// Green Light
  wxRect rect;
  GetFieldRect(3, rect);
  int w = 32;
  int h = 32;
  if (w > rect.width - 2)
    w = rect.width - 2;
  if (h > rect.height - 2)
    h = rect.height - 2;
  if (w > h)
    w = h;
  if (h > w)
    h = w;
  wxImage img = wxImage(green_light_xpm);
  wxImage icon = img.Rescale(w, h);
  Semaphore->SetBitmap(icon);
}

void MyMapPanelStatusBar::SetYellowLight()
{
// Yellow Light
  wxRect rect;
  GetFieldRect(3, rect);
  int w = 32;
  int h = 32;
  if (w > rect.width - 2)
    w = rect.width - 2;
  if (h > rect.height - 2)
    h = rect.height - 2;
  if (w > h)
    w = h;
  if (h > w)
    h = w;
  wxImage img = wxImage(yellow_light_xpm);
  wxImage icon = img.Rescale(w, h);
  Semaphore->SetBitmap(icon);
}
