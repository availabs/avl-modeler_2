/*
/ LayerTree.cpp
/ tree control to handle map layers
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

#include "wx/imaglist.h"
#include "wx/spinctrl.h"
#include "wx/colordlg.h"
#include "wx/clipbrd.h"

//
// ICONs in XPM format [universally portable]
//
#include "icons/map_root.xpm"
#include "icons/wms.xpm"
#include "icons/wms_off.xpm"
#include "icons/linestrings_on.xpm"
#include "icons/linestrings_off.xpm"
#include "icons/points_on.xpm"
#include "icons/points_off.xpm"
#include "icons/polygons_on.xpm"
#include "icons/polygons_off.xpm"
#include "icons/vector.xpm"
#include "icons/vector_off.xpm"
#include "icons/topo_geo.xpm"
#include "icons/topo_geo_off.xpm"
#include "icons/topo_net.xpm"
#include "icons/topo_net_off.xpm"
#include "icons/coverage.xpm"
#include "icons/coverage_off.xpm"
#include "icons/map_table.xpm"
#include "icons/layer_on.xpm"
#include "icons/layer_off.xpm"
#include "icons/quick_style.xpm"
#include "icons/remove_layer.xpm"
#include "icons/layer_config.xpm"
#include "icons/center.xpm"

MyLayerTree::MyLayerTree(MyMapPanel * parent, wxWindowID id):wxTreeCtrl(parent,
           id)
{
//
// constructor: TREE control to manage Map Layers
//
  MapPanel = parent;
  Changed = true;
  CurrentItem = wxTreeItemId();
  DraggedItem = wxTreeItemId();
  Root = AddRoot(wxT("Map Layers"));
// setting up icons 
  Images = new wxImageList(16, 16, true);
  wxIcon icons[17];
  icons[0] = wxIcon(map_root_xpm);
  icons[1] = wxIcon(wms_xpm);
  icons[2] = wxIcon(wms_off_xpm);
  icons[3] = wxIcon(points_on_xpm);
  icons[4] = wxIcon(points_off_xpm);
  icons[5] = wxIcon(linestrings_on_xpm);
  icons[6] = wxIcon(linestrings_off_xpm);
  icons[7] = wxIcon(polygons_on_xpm);
  icons[8] = wxIcon(polygons_off_xpm);
  icons[9] = wxIcon(vector_xpm);
  icons[10] = wxIcon(vector_off_xpm);
  icons[11] = wxIcon(topo_geo_xpm);
  icons[12] = wxIcon(topo_geo_off_xpm);
  icons[13] = wxIcon(topo_net_xpm);
  icons[14] = wxIcon(topo_net_off_xpm);
  icons[15] = wxIcon(coverage_xpm);
  icons[16] = wxIcon(coverage_off_xpm);
  Images->Add(icons[0]);
  Images->Add(icons[1]);
  Images->Add(icons[2]);
  Images->Add(icons[3]);
  Images->Add(icons[4]);
  Images->Add(icons[5]);
  Images->Add(icons[6]);
  Images->Add(icons[7]);
  Images->Add(icons[8]);
  Images->Add(icons[9]);
  Images->Add(icons[10]);
  Images->Add(icons[11]);
  Images->Add(icons[12]);
  Images->Add(icons[13]);
  Images->Add(icons[14]);
  Images->Add(icons[15]);
  Images->Add(icons[16]);
  SetImageList(Images);
  SetItemImage(Root, 0);

// setting up event handlers 
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_SEL_CHANGED,
          (wxObjectEventFunction) & MyLayerTree::OnSelChanged);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
          (wxObjectEventFunction) & MyLayerTree::OnItemActivated);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
          (wxObjectEventFunction) & MyLayerTree::OnRightClick);
  Connect(Tree_MapRemoveAll, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdRemoveAll);
  Connect(Tree_MapShowAll, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdShowAll);
  Connect(Tree_MapHideAll, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdHideAll);
  Connect(Tree_MapConfigure, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdMapConfigure);
  Connect(Tree_MapVisible, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdVisible);
  Connect(Tree_MapFullExtent, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdMapFullExtent);
  Connect(Tree_MapLayerFullExtent, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdMapLayerFullExtent);
  Connect(Tree_SqlSample, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdMapSqlSample);
  Connect(Tree_UrlSample, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdMapUrlSample);
  Connect(Tree_SqlLayer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdSqlLayer);
  Connect(Tree_MapLayerConfigure, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdMapLayerConfigure);
  Connect(Tree_QuickStyleEdit, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdQuickStyleEdit);
  Connect(Tree_MapLayerInfo, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdLayerInfo);
  Connect(Tree_MapRemoveLayer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdRemoveLayer);
  Connect(Tree_MapDeleteItem, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyLayerTree::OnCmdDeleteItem);
// Drag & Drop event handlers
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_BEGIN_DRAG,
          (wxObjectEventFunction) & MyLayerTree::OnDragStart);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_BEGIN_RDRAG,
          (wxObjectEventFunction) & MyLayerTree::OnDragStart);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_END_DRAG,
          (wxObjectEventFunction) & MyLayerTree::OnDragEnd);
}

MyLayerTree::~MyLayerTree()
{
// destructor
  if (Images)
    delete Images;
}

int MyLayerTree::GetIconIndex(MapLayer * layer)
{
//
// returns the appropriate Icon 
//
  int idx = 0;
  switch (layer->GetType())
    {
      case MAP_LAYER_WMS:
        if (layer->IsVisible() == false)
          idx = 2;
        else
          idx = 1;
        break;
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
        switch (layer->GetGeometryType())
          {
            case 1:
            case 1001:
            case 2001:
            case 3001:
            case 4:
            case 1004:
            case 2004:
            case 3004:
              if (layer->IsVisible() == false)
                idx = 4;
              else
                idx = 3;
              break;
            case 2:
            case 1002:
            case 2002:
            case 3002:
            case 5:
            case 1005:
            case 2005:
            case 3005:
              if (layer->IsVisible() == false)
                idx = 6;
              else
                idx = 5;
              break;
            case 3:
            case 1003:
            case 2003:
            case 3003:
            case 6:
            case 1006:
            case 2006:
            case 3006:
              if (layer->IsVisible() == false)
                idx = 8;
              else
                idx = 7;
              break;
            default:
              if (layer->IsVisible() == false)
                idx = 10;
              else
                idx = 9;
              break;
          }
        break;
      case MAP_LAYER_TOPOLOGY:
        if (layer->IsVisible() == false)
          idx = 12;
        else
          idx = 11;
        break;
      case MAP_LAYER_NETWORK:
        if (layer->IsVisible() == false)
          idx = 14;
        else
          idx = 13;
        break;
      case MAP_LAYER_RASTER:
        if (layer->IsVisible() == false)
          idx = 16;
        else
          idx = 15;
        break;
    };
  return idx;
}

void MyLayerTree::AddLayer(MapLayer * layer)
{
//
// adds a layer to the TREE list
//
  wxTreeItemId item;
  wxString name = layer->GetDbPrefix();
  if (layer->GetType() == MAP_LAYER_VECTOR
      || layer->GetType() == MAP_LAYER_VECTOR_VIEW
      || layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    name += wxT(".") + layer->GetVectorPrefix();
  name += wxT(".") + layer->GetName();
  if (GetChildrenCount(Root) == 0)
    item = AppendItem(Root, name, 1);
  else
    item = InsertItem(Root, 0, name, 1);
  MapLayerObject *obj = new MapLayerObject(layer);
  SetItemData(item, (wxTreeItemData *) obj);
  wxColour color(0, 0, 0);
  int idx = GetIconIndex(layer);
  if (layer->IsVisible() == false)
    color = wxColour(192, 192, 192);
  SetItemImage(item, idx);
  SetItemTextColour(item, color);
}

void MyLayerTree::MarkCurrentItem()
{
//
// setting the Current Item as BOLD
//
  wxTreeItemId item;
  wxTreeItemIdValue cookie;
// unsetting the currently active Map Layer
  MapPanel->SetActiveMapLayer(NULL);
  item = GetFirstChild(GetRoot(), cookie);
  while (item.IsOk() == true)
    {
      if (item == CurrentItem)
        {
          SetItemBold(item, true);
          MapLayerObject *obj = (MapLayerObject *) GetItemData(item);
          if (obj != NULL)
            {
              MapLayer *layer = obj->GetLayer();
              if (layer->IsVisible() == true)
                {
                  // making this Map Layer to be currently active
                  MapPanel->SetActiveMapLayer(layer);
                }
            }
      } else
        SetItemBold(item, false);
      item = GetNextChild(GetRoot(), cookie);
    }
}

void MyLayerTree::SelectActiveItem(MapLayer * lyr)
{
//
// setting the Current Active Item as BOLD
//
  wxTreeItemId item;
  wxTreeItemIdValue cookie;
// unsetting the currently active Map Layer
  MapPanel->SetActiveMapLayer(NULL);
  item = GetFirstChild(GetRoot(), cookie);
  while (item.IsOk() == true)
    {
      MapLayerObject *obj = (MapLayerObject *) GetItemData(item);
      if (obj != NULL)
        {
          MapLayer *layer = obj->GetLayer();
          if (layer == lyr && layer->IsVisible() == true)
            {
              // making this Map Layer to be currently active
              CurrentItem = item;
              SetItemBold(item, true);
              SelectItem(item, true);
              MapPanel->SetActiveMapLayer(layer);
            }
      } else
        SetItemBold(item, false);
      item = GetNextChild(GetRoot(), cookie);
    }
}

void MyLayerTree::OnSelChanged(wxTreeEvent & event)
{
//
// selecting some node [mouse action]
//
  wxTreeItemId item = event.GetItem();
  if (item == Root)
    {
      CurrentItem = wxTreeItemId();
      MarkCurrentItem();
      return;
    }
  CurrentItem = item;
  MarkCurrentItem();
}

void MyLayerTree::OnItemActivated(wxTreeEvent & event)
{
//
// activating some node [mouse action]
//
  int idx = 1;
  wxColour color(0, 0, 0);
  wxTreeItemId item = event.GetItem();
  if (item == Root)
    return;
  MapLayerObject *obj = (MapLayerObject *) GetItemData(item);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  if (layer->IsVisible() == 1)
    {
      layer->Hide();
      idx = 2;
      color = wxColour(192, 192, 192);
  } else
    layer->Show();
  SetItemImage(item, idx);
  SetItemTextColour(item, color);
  MapPanel->RefreshMap();
}

void MyLayerTree::OnRightClick(wxTreeEvent & event)
{
//
// right click on some node [mouse action]
//
  wxMenuItem *menuItem;
  wxMenu menu;
  wxBitmap bmp;
  wxTreeItemId item = event.GetItem();
  SelectItem(item);
  wxPoint pt = event.GetPoint();
  if (item == Root)
    {
      CurrentItem = wxTreeItemId();
      MarkCurrentItem();
      menuItem = new wxMenuItem(&menu, Tree_MapHideAll, wxT("Hide All Layers"));
      bmp = wxBitmap(layer_off_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_MapShowAll, wxT("Show All Layers"));
      bmp = wxBitmap(layer_on_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_MapRemoveAll, wxT("Remove All Layers"));
      bmp = wxBitmap(remove_layer_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_MapFullExtent, wxT("Zoom to Full Extent"));
      bmp = wxBitmap(center_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Tree_MapConfigure, wxT("Configure Map"));
      bmp = wxBitmap(layer_config_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      PopupMenu(&menu, pt);
      return;
    }
  MapLayerObject *obj = (MapLayerObject *) GetItemData(item);
  if (obj == NULL)
    return;
  else
    {
      CurrentItem = item;
      MarkCurrentItem();
      MapLayer *layer = obj->GetLayer();
      wxString name = layer->GetTitle();
      menu.SetTitle(name);
      if (layer->IsVisible() == true)
        {
          menuItem = new wxMenuItem(&menu, Tree_MapVisible, wxT("Hide"));
          bmp = wxBitmap(layer_off_xpm);
          menuItem->SetBitmap(bmp);
          menu.Append(menuItem);
      } else
        {
          menuItem = new wxMenuItem(&menu, Tree_MapVisible, wxT("Show"));
          bmp = wxBitmap(layer_on_xpm);
          menuItem->SetBitmap(bmp);
          menu.Append(menuItem);
        }
      menuItem = new wxMenuItem(&menu, Tree_MapRemoveLayer, wxT("Remove"));
      bmp = wxBitmap(remove_layer_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_MapLayerFullExtent,
                       wxT("Zoom to Full Extent"));
      bmp = wxBitmap(center_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Tree_SqlSample, wxT("SQL sample"));
      menu.Append(menuItem);
      if (layer->GetType() == MAP_LAYER_WMS)
        {
          menuItem =
            new wxMenuItem(&menu, Tree_UrlSample, wxT("Request URL sample"));
          menu.Append(menuItem);
        }
      if (layer->GetType() == MAP_LAYER_VECTOR
          || layer->GetType() == MAP_LAYER_VECTOR_VIEW
          || layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)

        {
          menu.AppendSeparator();
          menuItem =
            new wxMenuItem(&menu, Tree_SqlLayer, wxT("Query all Features"));
          bmp = wxBitmap(map_table_xpm);
          menuItem->SetBitmap(bmp);
          menu.Append(menuItem);
        }
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Tree_MapLayerInfo, wxT("Metadata"));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_MapLayerConfigure, wxT("Configure"));
      bmp = wxBitmap(layer_config_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_QuickStyleEdit, wxT("QuickStyle Edit"));
      bmp = wxBitmap(quick_style_xpm);
      menuItem->SetBitmap(bmp);
      menu.Append(menuItem);
      PopupMenu(&menu, pt);
      return;
    }
}

void MyLayerTree::OnCmdLayerInfo(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - metadata from some Map layer
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  MapLayerInfoDialog dlg;
  dlg.Create(MapPanel, layer);
  dlg.ShowModal();
}

void MyLayerTree::OnCmdMapFullExtent(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - zooming the Map to Full Extent
//
  MapPanel->SetMapToFullExtent();
}

void MyLayerTree::OnCmdMapLayerFullExtent(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - zooming a Layer to Full Extent
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  MapPanel->SetMapLayerToFullExtent(layer);
}

void MyLayerTree::OnCmdMapSqlSample(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - showing an SQL sample corresponding to the current Map settings
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  switch (layer->GetType())
    {
      case MAP_LAYER_WMS:
        DoWmsSqlSample(layer);
        break;
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
        DoVectorSqlSample(layer);
        break;
      case MAP_LAYER_TOPOLOGY:
      case MAP_LAYER_NETWORK:
        DoVectorSqlSample(layer);
        break;
      case MAP_LAYER_RASTER:
        DoRasterSqlSample(layer);
        break;
    }
}

void MyLayerTree::OnCmdMapUrlSample(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - showing a Request URL sample corresponding to the current Map settings
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  if (layer->GetType() != MAP_LAYER_WMS)
    return;
  DoWmsUrlSample(layer);
}

void MyLayerTree::OnCmdSqlLayer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - showing  a full Table List
//
  char *sql = NULL;
  char *db_prefix = NULL;
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  if (layer == NULL)
    return;

  if (layer->GetType() == MAP_LAYER_VECTOR)
    {
      db_prefix = (char *) malloc(layer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
      sql =
        MapPanel->GetParent()->MapAuxPrepareSqlQuery(db_prefix,
                                                     layer->GetTableName(),
                                                     "ROWID");
    }
  if (layer->GetType() == MAP_LAYER_VECTOR_VIEW)
    {
      db_prefix = (char *) malloc(layer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
      sql =
        MapPanel->GetParent()->MapAuxPrepareSqlQuery(db_prefix,
                                                     layer->GetTableName(),
                                                     layer->GetViewRowidColumn
                                                     ());
    }
  if (layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    {
      db_prefix = (char *) malloc(layer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
      sql =
        MapPanel->GetParent()->MapAuxPrepareSqlQuery(db_prefix,
                                                     layer->GetTableName(),
                                                     "ROWID");
    }
  if (db_prefix != NULL)
    free(db_prefix);
  if (sql != NULL)
    {
      wxString table = wxString::FromUTF8(layer->GetTableName());
      wxString geom = wxString::FromUTF8(layer->GetGeometryColumn());
      MapPanel->InitializeSqlFilters(layer->GetDbPrefix(), table, true, geom);
      wxString sqlstr = wxString::FromUTF8(sql);
      sqlite3_free(sql);
      MapPanel->SetSql(sqlstr, true);
      MapPanel->GetParent()->GetQueryView()->EnableFilterButton();
    }
}

char *MyFrame::MapAuxPrepareSqlQuery(const char *db_prefix,
                                     const char *table, const char *rowid)
{
//
// preparing a full table SQL query
//
  char *sql;
  int i;
  char **results;
  int rows;
  int columns;
  char *xdb_prefix;
  char *xtable;
  char *xrowid;
  char *xcolname;
  char *prev;
  int ret;
  sqlite3 *sqlite = MapPanel->GetSqlite();

  xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  xtable = gaiaDoubleQuotedSql(table);
  sql = sqlite3_mprintf("PRAGMA \"%s\".table_info(\"%s\")", xdb_prefix, xtable);
  free(xdb_prefix);
  free(xtable);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    goto error;
  if (rows < 1)
    ;
  else
    {
      xrowid = gaiaDoubleQuotedSql(rowid);
      sql = sqlite3_mprintf("SELECT \"%s\" AS ROWID", xrowid);
      free(xrowid);
      for (i = 1; i <= rows; i++)
        {
          const char *colname = results[(i * columns) + 1];
          xcolname = gaiaDoubleQuotedSql(colname);
          prev = sql;
          sql = sqlite3_mprintf("%s, \"%s\"", prev, xcolname);
          sqlite3_free(prev);
          free(xcolname);
        }
    }
  sqlite3_free_table(results);
  prev = sql;
  xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  xtable = gaiaDoubleQuotedSql(table);
  sql = sqlite3_mprintf("%s FROM \"%s\".\"%s\"", prev, xdb_prefix, xtable);
  free(xdb_prefix);
  free(xtable);
  sqlite3_free(prev);
  return sql;

error:
  return NULL;
}

void MyLayerTree::DoWmsSqlSample(MapLayer * lyr)
{
//
// WMS - showing an SQL sample corresponding to the current Map settings
//
  WmsSqlSampleDialog dlg;
  dlg.Create(MapPanel, lyr);
  dlg.ShowModal();
}

void MyLayerTree::DoWmsUrlSample(MapLayer * lyr)
{
//
// WMS - showing a Request URL sample corresponding to the current Map settings
//
  WmsSqlSampleDialog dlg;
  dlg.Create(MapPanel, lyr, true);
  dlg.ShowModal();
}

void MyLayerTree::DoVectorSqlSample(MapLayer * lyr)
{
//
// Vector - showing an SQL sample corresponding to the current Map settings
//
  VectorSqlSampleDialog dlg;
  dlg.Create(MapPanel, lyr);
  dlg.ShowModal();
}

void MyLayerTree::DoRasterSqlSample(MapLayer * lyr)
{
//
// Raster - showing an SQL sample corresponding to the current Map settings
//
  RasterSqlSampleDialog dlg;
  dlg.Create(MapPanel, lyr);
  dlg.ShowModal();
}

void MyLayerTree::DoFetchWmsUrls(const char *db_prefix, const char *name,
                                 char **getMapUrl, char **getFeatureInfoUrl)
{
// attempting to retrieve the URLs for some WMS
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int len;

  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  char *xname = gaiaDoubleQuotedSql(name);
  sql =
    sqlite3_mprintf
    ("SELECT url, getfeatureinfo_url FROM \"%s\".wms_getmap WHERE Lower(layer_name) = Lower(%Q)",
     xdb_prefix, xname);
  free(xdb_prefix);
  free(xname);
  ret =
    sqlite3_get_table(MapPanel->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *getmap = results[(i * columns) + 0];
      const char *getfeatureinfo = results[(i * columns) + 1];
      if (*getMapUrl != NULL)
        free(*getMapUrl);
      if (getmap == NULL)
        *getMapUrl = NULL;
      else
        {
          len = strlen(getmap);
          *getMapUrl = (char *) malloc(len + 1);
          strcpy(*getMapUrl, getmap);
        }
      if (*getFeatureInfoUrl != NULL)
        free(*getFeatureInfoUrl);
      if (getfeatureinfo == NULL)
        *getFeatureInfoUrl = NULL;
      else
        {
          len = strlen(getfeatureinfo);
          *getFeatureInfoUrl = (char *) malloc(len + 1);
          strcpy(*getFeatureInfoUrl, getfeatureinfo);
        }
    }
  sqlite3_free_table(results);
}

char *MyLayerTree::XmlClean(const char *dirty)
{
//
// properly formatting an XML string
//
  char *clean = NULL;
  const char *in = dirty;
  while (*in != '\0')
    {
      char str[16];
      switch (*in)
        {
          case '&':
            sprintf(str, "&amp;");
            break;
          case '>':
            sprintf(str, "&gt;");
            break;
          case '<':
            sprintf(str, "&lt;");
            break;
          case '"':
            sprintf(str, "&quot;");
            break;
          case '\'':
            sprintf(str, "&apos;");
            break;
          default:
            sprintf(str, "%c", *in);
            break;
        };
      in++;
      if (clean == NULL)
        clean = sqlite3_mprintf("%s", str);
      else
        {
          char *prev = clean;
          clean = sqlite3_mprintf("%s%s", prev, str);
          sqlite3_free(prev);
        }
    }
  return clean;
}

void MyLayerTree::OnCmdRemoveLayer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - removing some Map layer (Step #1)
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  MapPanel->RemoveMapLayer(layer);
  MapPanel->RefreshMap();
  MapPanel->UpdateTools();
// appending a delayed event so to really delete the layer node
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_MapDeleteItem);
  AddPendingEvent(evt);
}

void MyLayerTree::OnCmdDeleteItem(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - deleting a Map layer node from the Tree (Step #2)
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  Delete(CurrentItem);
  CurrentItem = wxTreeItemId();
}

void MyLayerTree::CleanUp()
{
//
// removing all Map layers [Remove ALL]
//
  MapLayerObject *obj;
  wxTreeItemId layer;
  wxTreeItemIdValue cookie;
  ::wxBeginBusyCursor();
  Hide();
  layer = GetFirstChild(Root, cookie);
  while (layer.IsOk() == true)
    {
      // removing a Layer
      obj = (MapLayerObject *) GetItemData(layer);
      MapLayer *lyr = obj->GetLayer();
      if (obj == NULL)
        return;
      MapPanel->RemoveMapLayer(lyr);
      layer = GetNextChild(Root, cookie);
    }
  DeleteChildren(Root);
  Show();
// unsetting the currently active Map Layer
  MapPanel->SetActiveMapLayer(NULL);
  MapPanel->UpdateTools();
  ::wxEndBusyCursor();
  CurrentItem = wxTreeItemId();
  MapPanel->RefreshMap();
}

void MyLayerTree::OnCmdRemoveAll(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - removing all Map layers [Remove ALL]
//
  CleanUp();
}

void MyLayerTree::OnCmdShowAll(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - changing the visibility state [SHOW ALL]
//
  MapLayerObject *obj;
  wxTreeItemId layer;
  wxTreeItemIdValue cookie;
  ::wxBeginBusyCursor();
  Hide();
  layer = GetFirstChild(Root, cookie);
  while (layer.IsOk() == true)
    {
      // making a Layer to be visible
      obj = (MapLayerObject *) GetItemData(layer);
      MapLayer *lyr = obj->GetLayer();
      lyr->Show();
      int idx = GetIconIndex(lyr);
      SetItemImage(layer, idx);
      SetItemTextColour(layer, wxColour(0, 0, 0));
      layer = GetNextChild(Root, cookie);
    }
  Show();
  ::wxEndBusyCursor();
  MapPanel->RefreshMap();
}

void MyLayerTree::OnCmdHideAll(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - changing the visibility state [HIDE ALL]
//
  MapLayerObject *obj;
  wxTreeItemId layer;
  wxTreeItemIdValue cookie;
  ::wxBeginBusyCursor();
  Hide();
  layer = GetFirstChild(Root, cookie);
  while (layer.IsOk() == true)
    {
      // making a Layer to be invisible
      obj = (MapLayerObject *) GetItemData(layer);
      MapLayer *lyr = obj->GetLayer();
      lyr->Hide();
      int idx = GetIconIndex(lyr);
      SetItemImage(layer, idx);
      SetItemTextColour(layer, wxColour(192, 192, 192));
      layer = GetNextChild(Root, cookie);
    }
// unsetting the currently active Map Layer
  MapPanel->SetActiveMapLayer(NULL);
  MapPanel->UpdateTools();
  Show();
  ::wxEndBusyCursor();
  MapPanel->RefreshMap();
}

void MyLayerTree::OnCmdMapConfigure(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - changing the Map Configuration
//
  MapConfigDialog dlg;
  dlg.Create(MapPanel);
  dlg.ShowModal();
}

void MyLayerTree::OnCmdVisible(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - changing the visibility state
//
  wxColour color(0, 0, 0);
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  if (layer->IsVisible() == 1)
    {
      layer->Hide();
      color = wxColour(192, 192, 192);
      if (MapPanel->GetActiveLayer() == layer)
        {
          // unsetting the currently active Map Layer
          MapPanel->SetActiveMapLayer(NULL);
          MapPanel->UpdateTools();
        }
  } else
    {
      if ((layer->GetType() == MAP_LAYER_RASTER
           || layer->GetType() == MAP_LAYER_WMS)
          && MapPanel->IsRasterWmsAutoSwitch() == true)
        {
          // AutoSwitching Raster/WMS Coverages
          wxTreeItemId xlayer;
          wxTreeItemIdValue cookie;
          xlayer = GetFirstChild(Root, cookie);
          while (xlayer.IsOk() == true)
            {
              MapLayerObject *obj2 = (MapLayerObject *) GetItemData(xlayer);
              if (obj != obj2)
                {
                  MapLayer *lyr = obj2->GetLayer();
                  if (lyr->GetType() == MAP_LAYER_RASTER
                      || lyr->GetType() == MAP_LAYER_WMS)
                    {
                      // making a Raster/WMS Layer to be invisible
                      lyr->Hide();
                      int idx = GetIconIndex(lyr);
                      SetItemImage(xlayer, idx);
                      SetItemTextColour(xlayer, wxColour(192, 192, 192));
                    }
                }
              xlayer = GetNextChild(Root, cookie);
            }
          layer->Show();
          SetItemBold(CurrentItem, true);
          MapPanel->SetActiveMapLayer(layer);
      } else
        {
          layer->Show();
          SetItemBold(CurrentItem, true);
          MapPanel->SetActiveMapLayer(layer);
        }
    }

  int idx = GetIconIndex(layer);
  SetItemImage(CurrentItem, idx);
  SetItemTextColour(CurrentItem, color);
  MapPanel->RefreshMap();
}

void MyLayerTree::OnCmdMapLayerConfigure(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - changing the Layer Configuration
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  bool bboxChanged = false;
  bool configChanged = false;
  if (layer->GetType() == MAP_LAYER_WMS)
    {
      WmsLayerConfigDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          bboxChanged = dlg.BBoxChanged();
          configChanged = dlg.ConfigChanged();
        }
    }
  if (layer->GetType() == MAP_LAYER_RASTER)
    {
      RasterLayerConfigDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          bboxChanged = dlg.BBoxChanged();
          configChanged = dlg.ConfigChanged();
        }
    }
  if (layer->GetType() == MAP_LAYER_VECTOR
      || layer->GetType() == MAP_LAYER_VECTOR_VIEW
      || layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    {
      VectorLayerConfigDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          bboxChanged = dlg.BBoxChanged();
          configChanged = dlg.ConfigChanged();
        }
    }
  if (layer->GetType() == MAP_LAYER_TOPOLOGY)
    {
      TopologyLayerConfigDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          bboxChanged = dlg.BBoxChanged();
          configChanged = dlg.ConfigChanged();
          if (configChanged == true)
            layer->Invalidate();
        }
    }
  if (layer->GetType() == MAP_LAYER_NETWORK)
    {
      NetworkLayerConfigDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          bboxChanged = dlg.BBoxChanged();
          configChanged = dlg.ConfigChanged();
          if (configChanged == true)
            layer->Invalidate();
        }
    }
  if (bboxChanged == true)
    MapPanel->SetMapToFullExtent();
  if (configChanged == true)
    MapPanel->RefreshMap();
}

void MyLayerTree::OnCmdQuickStyleEdit(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - editing Layer's Quick Style
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  MapLayer *layer = obj->GetLayer();
  bool configChanged = false;
  if (layer->GetType() == MAP_LAYER_VECTOR
      || layer->GetType() == MAP_LAYER_VECTOR_VIEW
      || layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    {
      QuickStyleVectorDialog dlg;
      int type = layer->GetVectorConfig()->GetGeometryType();
      dlg.Create(MapPanel, layer, type);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        configChanged = dlg.ConfigChanged();
    }
  if (layer->GetType() == MAP_LAYER_TOPOLOGY
      || layer->GetType() == MAP_LAYER_NETWORK)
    {
      QuickStyleTopologyDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        configChanged = dlg.ConfigChanged();
    }
  if (layer->GetType() == MAP_LAYER_RASTER)
    {
      QuickStyleRasterDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        configChanged = dlg.ConfigChanged();
    }
  if (layer->GetType() == MAP_LAYER_WMS)
    {
      QuickStyleWmsDialog dlg;
      dlg.Create(MapPanel, layer);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        configChanged = true;
    }
  if (configChanged == true)
    MapPanel->RefreshMap();
}

void MyLayerTree::OnDragStart(wxTreeEvent & event)
{
//
// user event: starting DRAG [drag&drop]
//
  DraggedItem = event.GetItem();
  MapLayerObject *obj = (MapLayerObject *) GetItemData(DraggedItem);
  if (obj == NULL)
    {
      DraggedItem = wxTreeItemId();
      return;
    }
  event.Allow();
}

void MyLayerTree::OnDragEnd(wxTreeEvent & event)
{
//
// user event: DRAG ended [drag&drop]
//
  MapLayerObject *obj = (MapLayerObject *) GetItemData(DraggedItem);
  if (obj == NULL)
    return;
  wxTreeItemId currentItem = event.GetItem();
  MapLayer *layer = obj->GetLayer();
// 
// creating the new node
//
  wxTreeItemId position;
  wxString name = layer->GetDbPrefix();
  if (layer->GetType() == MAP_LAYER_VECTOR
      || layer->GetType() == MAP_LAYER_VECTOR_VIEW
      || layer->GetType() == MAP_LAYER_VECTOR_VIRTUAL)
    name += wxT(".") + layer->GetVectorPrefix();
  name += wxT(".") + layer->GetName();
  wxColour color(0, 0, 0);
  int idx = GetIconIndex(layer);
  if (layer->IsVisible() == false)
    {
      color = wxColour(192, 192, 192);
      if (idx > 0)
        idx++;
    }
  if (currentItem == Root)
    position = PrependItem(Root, name, idx);
  else if (currentItem == GetLastChild(Root))
    position = AppendItem(Root, name, idx);
  else
    position = InsertItem(Root, currentItem, name, idx);
  obj = new MapLayerObject(layer);
  SetItemData(position, (wxTreeItemData *) obj);
  SetItemTextColour(position, color);
  Delete(DraggedItem);
  DraggedItem = wxTreeItemId();
// updating the Map Layer list
  MapPanel->ResetMapLayers();
  wxTreeItemIdValue cookie;
  wxTreeItemId lyr = GetFirstChild(Root, cookie);
  while (lyr.IsOk() == true)
    {
      obj = (MapLayerObject *) GetItemData(lyr);
      layer = obj->GetLayer();
      MapPanel->ReinsertMapLayer(layer);
      lyr = GetNextChild(Root, cookie);
    }
  MapPanel->RefreshMap();
}

bool RasterLayerConfigDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (wxDialog::Create
      (parent, wxID_ANY, wxT("Map Layer (Raster) Configuration")) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void RasterLayerConfigDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_WMS_LAYER, fullName,
                                        wxDefaultPosition, wxSize(470, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: Raster options
  wxBoxSizer *rstSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(rstSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("SRID"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sridBoxSizer = new wxStaticBoxSizer(sridBox, wxVERTICAL);
  xxSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *sridList =
    new wxComboBox(this, ID_RASTER_SRID, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateSRIDs(sridList);
  sridBoxSizer->Add(sridList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_RASTER_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxButton *rst_ok = new wxButton(this, ID_RASTER_OK, wxT("&Apply Changes"));
  buttonSizer->Add(rst_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *rst_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(rst_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_RASTER_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterLayerConfigDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterLayerConfigDialog::OnQuit);
}

void RasterLayerConfigDialog::InitData()
{
// initializing the current configuration
  int len;
  RasterLayerConfig *config = Layer->GetRasterConfig();
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
  Srid = Layer->GetMapSRID();
}

void RasterLayerConfigDialog::PopulateSRIDs(wxComboBox * sridList)
{
// populating the SRIDs List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT r.srid, y.ref_sys_name FROM \"%s\".raster_coverages AS r "
     "JOIN \"%s\".spatial_ref_sys AS y ON (r.srid = y.srid) "
     "WHERE r.coverage_name = %Q UNION "
     "SELECT s.srid, y.ref_sys_name FROM \"%s\".raster_coverages AS r "
     "JOIN \"%s\".raster_coverages_srid AS s ON (s.coverage_name = r.coverage_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (s.srid = y.srid) "
     "WHERE r.coverage_name = %Q AND "
     "s.extent_miny IS NOT NULL AND s.extent_maxx IS NOT NULL AND "
     "s.extent_maxx IS NOT NULL ORDER BY s.srid", xdb_prefix, xdb_prefix,
     layer, xdb_prefix, xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *srid = results[(i * columns) + 0];
      const char *ref_sys_name = results[(i * columns) + 1];
      wxString str =
        wxString::FromUTF8(srid) + wxT(" ") + wxString::FromUTF8(ref_sys_name);
      sridList->Append(str);
      if (Srid == atoi(srid))
        sridList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void RasterLayerConfigDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int has_default = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT s.style_name FROM \"%s\".raster_coverages AS r "
     "JOIN \"%s\".SE_raster_styled_layers AS x ON (x.coverage_name = r.coverage_name) "
     "JOIN \"%s\".SE_raster_styles AS s ON (s.style_id = x.style_id) "
     "WHERE r.coverage_name = %Q ORDER BY s.style_name", xdb_prefix,
     xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      if (strcasecmp(style, "default") == 0)
        has_default = 1;
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
    }
  sqlite3_free_table(results);

  QuickStyleRasterObj *quickStyle = Layer->GetQuickStyleRaster();
  if (quickStyle != NULL)
    {
      wxString str = wxString::FromUTF8(quickStyle->GetUUID());
      stlList->Append(str);
    }

  if (!has_default)
    stlList->Append(wxT("default"));
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (Style == NULL)
            {
              if (stlList->GetString(i).Cmp(wxT("default")) == 0)
                stlList->SetSelection(i);
          } else
            {
              if (stlList->GetString(i).Cmp(wxString::FromUTF8(Style)) == 0)
                stlList->SetSelection(i);
            }
        }
    }
}

void RasterLayerConfigDialog::DoConfigureMapLayer()
{
// updating the Map Layer definitions
  RasterLayerConfig *config = Layer->GetRasterConfig();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (Srid != Layer->GetMapSRID())
    {
      Layer->SetMapSRID(Srid);
      IsConfigChanged = true;
    }
  if ((Style == NULL && config->GetStyle() != NULL)
      || (Style != NULL && config->GetStyle() == NULL))
    {
      config->SetStyle(Style);
      IsConfigChanged = true;
  } else if (Style == NULL && config->GetStyle() == NULL)
    ;
  else
    {
      if (strcmp(Style, config->GetStyle()) != 0)
        {
          config->SetStyle(Style);
          IsConfigChanged = true;
        }
    }
}

void RasterLayerConfigDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void RasterLayerConfigDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxComboBox *sridList = (wxComboBox *) FindWindow(ID_RASTER_SRID);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_RASTER_STYLE);

  wxString str = sridList->GetValue();
  int cut = str.Find(wxT(" "));
  if (cut != wxNOT_FOUND)
    str.Truncate(cut);
  long srid;
  str.ToLong(&srid);
  Srid = srid;
  wxString stl = stlList->GetValue();
  if (Style != NULL)
    free(Style);
  Style = (char *) malloc((stl.Len() * 4) + 1);
  strcpy(Style, stl.ToUTF8());
  DoConfigureMapLayer();
  wxDialog::EndModal(wxID_OK);
}

bool VectorLayerConfigDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (wxDialog::Create
      (parent, wxID_ANY, wxT("Map Layer (Vector) Configuration")) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void VectorLayerConfigDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_LAYER, fullName,
                                        wxDefaultPosition, wxSize(470, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: Vector options
  wxBoxSizer *rstSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(rstSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("SRID"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sridBoxSizer = new wxStaticBoxSizer(sridBox, wxVERTICAL);
  xxSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *sridList =
    new wxComboBox(this, ID_VECTOR_SRID, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateSRIDs(sridList);
  sridBoxSizer->Add(sridList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_VECTOR_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxButton *rst_ok = new wxButton(this, ID_VECTOR_OK, wxT("&Apply Changes"));
  buttonSizer->Add(rst_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *rst_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(rst_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_VECTOR_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorLayerConfigDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorLayerConfigDialog::OnQuit);
}

void VectorLayerConfigDialog::InitData()
{
// initializing the current configuration
  int len;
  VectorLayerConfig *config = Layer->GetVectorConfig();
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
  Srid = Layer->GetMapSRID();
}

void VectorLayerConfigDialog::PopulateSRIDs(wxComboBox * sridList)
{
// populating the SRIDs List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  int type = DoGuessVectorLayerType();
  switch (type)
    {
      case MAP_LAYER_VECTOR_VIEW:
        return PopulateViewSRIDs(sridList);
      case MAP_LAYER_VECTOR_VIRTUAL:
        return PopulateVirtualTableSRIDs(sridList);
      case MAP_LAYER_UNKNOWN:
        return;
      default:
        break;
    };

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT g.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".geometry_columns AS g ON (c.f_table_name = g.f_table_name AND "
     "c.f_geometry_column = g.f_geometry_column) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (g.srid = y.srid) "
     "WHERE c.coverage_name = %Q UNION "
     "SELECT s.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".vector_coverages_srid AS s ON (s.coverage_name = c.coverage_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (s.srid = y.srid) "
     "WHERE c.coverage_name = %Q AND s.extent_minx IS NOT NULL AND "
     "s.extent_miny IS NOT NULL AND s.extent_maxx IS NOT NULL AND "
     "s.extent_maxx IS NOT NULL ORDER BY s.srid", xdb_prefix, xdb_prefix,
     xdb_prefix, layer, xdb_prefix, xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *srid = results[(i * columns) + 0];
      const char *ref_sys_name = results[(i * columns) + 1];
      wxString str =
        wxString::FromUTF8(srid) + wxT(" ") + wxString::FromUTF8(ref_sys_name);
      sridList->Append(str);
      if (Srid == atoi(srid))
        sridList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void VectorLayerConfigDialog::PopulateViewSRIDs(wxComboBox * sridList)
{
// populating the SRIDs List - Spatial Views
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT g.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".views_geometry_columns AS v ON (c.view_name = v.view_name AND "
     "c.view_geometry = v.view_geometry) "
     "JOIN \"%s\".geometry_columns AS g ON (v.f_table_name = g.f_table_name AND "
     "v.f_geometry_column = g.f_geometry_column) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (g.srid = y.srid) "
     "WHERE c.coverage_name = %Q UNION "
     "SELECT s.srid, ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".vector_coverages_srid AS s ON (s.coverage_name = c.coverage_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (s.srid = y.srid) "
     "WHERE c.coverage_name = %Q AND s.extent_minx IS NOT NULL AND "
     "s.extent_miny IS NOT NULL AND s.extent_maxx IS NOT NULL AND "
     "s.extent_maxx IS NOT NULL ORDER BY s.srid", xdb_prefix, xdb_prefix,
     xdb_prefix, xdb_prefix, layer, xdb_prefix, xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *srid = results[(i * columns) + 0];
      const char *ref_sys_name = results[(i * columns) + 1];
      wxString str =
        wxString::FromUTF8(srid) + wxT(" ") + wxString::FromUTF8(ref_sys_name);
      sridList->Append(str);
      if (Srid == atoi(srid))
        sridList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void VectorLayerConfigDialog::PopulateVirtualTableSRIDs(wxComboBox * sridList)
{
// populating the SRIDs List - Virtual Tables
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT v.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".virts_geometry_columns AS v ON (c.virt_name = v.virt_name AND "
     "c.virt_geometry = v.virt_geometry) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (v.srid = y.srid) "
     "WHERE c.coverage_name = %Q UNION "
     "SELECT s.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".vector_coverages_srid AS s ON (s.coverage_name = c.coverage_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (s.srid = y.srid) "
     "WHERE c.coverage_name = %Q AND "
     "s.extent_miny IS NOT NULL AND s.extent_maxx IS NOT NULL AND "
     "s.extent_maxx IS NOT NULL ORDER BY s.srid", xdb_prefix, xdb_prefix,
     xdb_prefix, layer, xdb_prefix, xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *srid = results[(i * columns) + 0];
      const char *ref_sys_name = results[(i * columns) + 1];
      wxString str =
        wxString::FromUTF8(srid) + wxT(" ") + wxString::FromUTF8(ref_sys_name);
      sridList->Append(str);
      if (Srid == atoi(srid))
        sridList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

int VectorLayerConfigDialog::DoGuessVectorLayerType()
{
// guessing the VectorLayer sub-type
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int ok_table = 0;
  int ok_table_geom = 0;
  int ok_view = 0;
  int ok_view_geom = 0;
  int ok_virt = 0;
  int ok_virt_geom = 0;
  int ok_topogeo = 0;
  int ok_toponet = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT f_table_name, f_geometry_column, view_name, view_geometry, "
     "virt_name, virt_geometry, topology_name, network_name FROM \"%s\".vector_coverages "
     "WHERE coverage_name = %Q", xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return MAP_LAYER_UNKNOWN;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *str = results[(i * columns) + 0];
      if (str != NULL)
        ok_table = 1;
      str = results[(i * columns) + 1];
      if (str != NULL)
        ok_table_geom = 1;
      str = results[(i * columns) + 2];
      if (str != NULL)
        ok_view = 1;
      str = results[(i * columns) + 3];
      if (str != NULL)
        ok_view_geom = 1;
      str = results[(i * columns) + 4];
      if (str != NULL)
        ok_virt = 1;
      str = results[(i * columns) + 5];
      if (str != NULL)
        ok_virt_geom = 1;
      str = results[(i * columns) + 6];
      if (str != NULL)
        ok_topogeo = 1;
      str = results[(i * columns) + 7];
      if (str != NULL)
        ok_toponet = 1;
    }
  sqlite3_free_table(results);

  if (ok_topogeo)
    return MAP_LAYER_TOPOLOGY;
  if (ok_toponet)
    return MAP_LAYER_NETWORK;
  if (ok_virt && ok_virt_geom)
    return MAP_LAYER_VECTOR_VIRTUAL;
  if (ok_view && ok_view_geom)
    return MAP_LAYER_VECTOR_VIEW;
  if (ok_table && ok_table_geom)
    return MAP_LAYER_VECTOR;
  return MAP_LAYER_UNKNOWN;
}

void VectorLayerConfigDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int has_default = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT s.style_name FROM \"%s\".vector_coverages AS r "
     "JOIN \"%s\".SE_vector_styled_layers AS x ON (x.coverage_name = r.coverage_name) "
     "JOIN \"%s\".SE_vector_styles AS s ON (s.style_id = x.style_id) "
     "WHERE r.coverage_name = %Q ORDER BY s.style_name", xdb_prefix,
     xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      if (strcasecmp(style, "default") == 0)
        has_default = 1;
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
    }
  sqlite3_free_table(results);

  QuickStyleObj *quickStyle = Layer->GetQuickStyle();
  if (quickStyle != NULL)
    {
      wxString str = wxString::FromUTF8(quickStyle->GetUUID());
      stlList->Append(str);
    }

  if (!has_default)
    stlList->Append(wxT("default"));
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (Style == NULL)
            {
              if (stlList->GetString(i).Cmp(wxT("default")) == 0)
                stlList->SetSelection(i);
          } else
            {
              if (stlList->GetString(i).Cmp(wxString::FromUTF8(Style)) == 0)
                stlList->SetSelection(i);
            }
        }
    }
}

void VectorLayerConfigDialog::DoConfigureMapLayer()
{
// updating the Map Layer definitions
  VectorLayerConfig *config = Layer->GetVectorConfig();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (Srid != Layer->GetMapSRID())
    {
      Layer->SetMapSRID(Srid);
      IsConfigChanged = true;
    }
  if ((Style == NULL && config->GetStyle() != NULL)
      || (Style != NULL && config->GetStyle() == NULL))
    {
      config->SetStyle(Style);
      IsConfigChanged = true;
  } else if (Style == NULL && config->GetStyle() == NULL)
    ;
  else
    {
      if (strcmp(Style, config->GetStyle()) != 0)
        {
          config->SetStyle(Style);
          IsConfigChanged = true;
        }
    }
}

void VectorLayerConfigDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void VectorLayerConfigDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxComboBox *sridList = (wxComboBox *) FindWindow(ID_VECTOR_SRID);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_VECTOR_STYLE);

  wxString str = sridList->GetValue();
  int cut = str.Find(wxT(" "));
  if (cut != wxNOT_FOUND)
    str.Truncate(cut);
  long srid;
  str.ToLong(&srid);
  Srid = srid;;
  wxString stl = stlList->GetValue();
  if (Style != NULL)
    free(Style);
  Style = (char *) malloc((stl.Len() * 4) + 1);
  strcpy(Style, stl.ToUTF8());
  DoConfigureMapLayer();
  wxDialog::EndModal(wxID_OK);
}

bool TopologyLayerConfigDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (wxDialog::Create
      (parent, wxID_ANY, wxT("Map Layer (Topology) Configuration")) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void TopologyLayerConfigDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  VectorLayerConfig *config = Layer->GetVectorConfig();
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_TOPOGEO_LAYER, fullName,
                                        wxDefaultPosition, wxSize(470, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: Topology options
  wxBoxSizer *rstSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(rstSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("SRID"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sridBoxSizer = new wxStaticBoxSizer(sridBox, wxVERTICAL);
  xxSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *sridList =
    new wxComboBox(this, ID_TOPOGEO_SRID, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateSRIDs(sridList);
  sridBoxSizer->Add(sridList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_TOPOGEO_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// third row: Primitives Visibility
  wxStaticBox *visBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("TopoGeo Primitives Visibility"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *visBoxSizer = new wxStaticBoxSizer(visBox, wxVERTICAL);
  xxSizer->Add(visBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *vis1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  visBoxSizer->Add(vis1BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxBoxSizer *vis2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  visBoxSizer->Add(vis2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxCheckBox *faceCtrl = new wxCheckBox(this, ID_TOPOGEO_FACE,
                                        wxT("Faces"),
                                        wxDefaultPosition, wxDefaultSize);
  faceCtrl->SetValue(config->AreFacesVisible());
  vis1BoxSizer->Add(faceCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *edgeCtrl = new wxCheckBox(this, ID_TOPOGEO_EDGE,
                                        wxT("Edges"),
                                        wxDefaultPosition, wxDefaultSize);
  edgeCtrl->SetValue(config->AreEdgesVisible());
  vis1BoxSizer->Add(edgeCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *nodeCtrl = new wxCheckBox(this, ID_TOPOGEO_NODE,
                                        wxT("Nodes"),
                                        wxDefaultPosition, wxDefaultSize);
  nodeCtrl->SetValue(config->AreNodesVisible());
  vis1BoxSizer->Add(nodeCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *faceSeedCtrl = new wxCheckBox(this, ID_TOPOGEO_FACE_SEED,
                                            wxT("FaceSeeds"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  faceSeedCtrl->SetValue(config->AreFaceSeedsVisible());
  vis2BoxSizer->Add(faceSeedCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *edgeSeedCtrl = new wxCheckBox(this, ID_TOPOGEO_EDGE_SEED,
                                            wxT("EdgeSeeds"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  edgeSeedCtrl->SetValue(config->AreEdgeSeedsVisible());
  vis2BoxSizer->Add(edgeSeedCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// Apply/Quit buttons  
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxButton *rst_ok = new wxButton(this, ID_TOPOGEO_OK, wxT("&Apply Changes"));
  buttonSizer->Add(rst_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *rst_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(rst_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_TOPOGEO_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & TopologyLayerConfigDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & TopologyLayerConfigDialog::OnQuit);
}

void TopologyLayerConfigDialog::InitData()
{
// initializing the current configuration
  int len;
  VectorLayerConfig *config = Layer->GetVectorConfig();
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
  Srid = Layer->GetMapSRID();
}

void TopologyLayerConfigDialog::PopulateSRIDs(wxComboBox * sridList)
{
// populating the SRIDs List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT t.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".topologies AS t ON (c.topology_name = t.topology_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (t.srid = y.srid) "
     "WHERE c.coverage_name = %Q UNION "
     "SELECT s.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".vector_coverages_srid AS s ON (s.coverage_name = c.coverage_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (s.srid = y.srid) "
     "WHERE c.coverage_name = %Q AND "
     "s.extent_miny IS NOT NULL AND s.extent_maxx IS NOT NULL AND "
     "s.extent_maxx IS NOT NULL ORDER BY s.srid", xdb_prefix, xdb_prefix,
     xdb_prefix, layer, xdb_prefix, xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *srid = results[(i * columns) + 0];
      const char *ref_sys_name = results[(i * columns) + 1];
      wxString str =
        wxString::FromUTF8(srid) + wxT(" ") + wxString::FromUTF8(ref_sys_name);
      sridList->Append(str);
      if (Srid == atoi(srid))
        sridList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void TopologyLayerConfigDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int has_default = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT s.style_name FROM \"%s\".vector_coverages AS r "
     "JOIN \"%s\".SE_vector_styled_layers AS x ON (x.coverage_name = r.coverage_name) "
     "JOIN \"%s\".SE_vector_styles AS s ON (s.style_id = x.style_id) "
     "WHERE r.coverage_name = %Q ORDER BY s.style_name", xdb_prefix,
     xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      if (strcasecmp(style, "default") == 0)
        has_default = 1;
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
    }
  sqlite3_free_table(results);

  QuickStyleTopologyObj *quickStyleTopo = Layer->GetQuickStyleTopology();
  if (quickStyleTopo != NULL)
    {
      wxString str = wxString::FromUTF8(quickStyleTopo->GetUUID());
      stlList->Append(str);
    }

  if (!has_default)
    stlList->Append(wxT("default"));
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (Style == NULL)
            {
              if (stlList->GetString(i).Cmp(wxT("default")) == 0)
                stlList->SetSelection(i);
          } else
            {
              if (stlList->GetString(i).Cmp(wxString::FromUTF8(Style)) == 0)
                stlList->SetSelection(i);
            }
        }
    }
}

void TopologyLayerConfigDialog::DoConfigureMapLayer()
{
// updating the Map Layer definitions
  VectorLayerConfig *config = Layer->GetVectorConfig();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (Srid != Layer->GetMapSRID())
    {
      Layer->SetMapSRID(Srid);
      IsConfigChanged = true;
    }
  if ((Style == NULL && config->GetStyle() != NULL)
      || (Style != NULL && config->GetStyle() == NULL))
    {
      config->SetStyle(Style);
      IsConfigChanged = true;
  } else if (Style == NULL && config->GetStyle() == NULL)
    ;
  else
    {
      if (strcmp(Style, config->GetStyle()) != 0)
        {
          config->SetStyle(Style);
          IsConfigChanged = true;
        }
    }

  wxCheckBox *faceCtrl = (wxCheckBox *) FindWindow(ID_TOPOGEO_FACE);
  wxCheckBox *edgeCtrl = (wxCheckBox *) FindWindow(ID_TOPOGEO_EDGE);
  wxCheckBox *nodeCtrl = (wxCheckBox *) FindWindow(ID_TOPOGEO_NODE);
  wxCheckBox *faceSeedCtrl = (wxCheckBox *) FindWindow(ID_TOPOGEO_FACE_SEED);
  wxCheckBox *edgeSeedCtrl = (wxCheckBox *) FindWindow(ID_TOPOGEO_EDGE_SEED);
  bool oldval = config->AreFacesVisible();
  bool newval = faceCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetFacesVisible(newval);
      IsConfigChanged = true;
    }
  oldval = config->AreEdgesVisible();
  newval = edgeCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetEdgesVisible(newval);
      IsConfigChanged = true;
    }
  oldval = config->AreNodesVisible();
  newval = nodeCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetNodesVisible(newval);
      IsConfigChanged = true;
    }
  oldval = config->AreFaceSeedsVisible();
  newval = faceSeedCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetFaceSeedsVisible(newval);
      IsConfigChanged = true;
    }
  oldval = config->AreEdgeSeedsVisible();
  newval = edgeSeedCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetEdgeSeedsVisible(newval);
      IsConfigChanged = true;
    }
}

void TopologyLayerConfigDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void TopologyLayerConfigDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxComboBox *sridList = (wxComboBox *) FindWindow(ID_TOPOGEO_SRID);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_TOPOGEO_STYLE);

  wxString str = sridList->GetValue();
  int cut = str.Find(wxT(" "));
  if (cut != wxNOT_FOUND)
    str.Truncate(cut);
  long srid;
  str.ToLong(&srid);
  Srid = srid;
  wxString stl = stlList->GetValue();
  if (Style != NULL)
    free(Style);
  Style = (char *) malloc((stl.Len() * 4) + 1);
  strcpy(Style, stl.ToUTF8());
  DoConfigureMapLayer();
  wxDialog::EndModal(wxID_OK);
}

bool NetworkLayerConfigDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (wxDialog::Create
      (parent, wxID_ANY, wxT("Map Layer (Network) Configuration")) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void NetworkLayerConfigDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  VectorLayerConfig *config = Layer->GetVectorConfig();
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_TOPONET_LAYER, fullName,
                                        wxDefaultPosition, wxSize(470, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: Topology options
  wxBoxSizer *rstSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(rstSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("SRID"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sridBoxSizer = new wxStaticBoxSizer(sridBox, wxVERTICAL);
  xxSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *sridList =
    new wxComboBox(this, ID_TOPONET_SRID, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateSRIDs(sridList);
  sridBoxSizer->Add(sridList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_TOPONET_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// third row: Primitives Visibility
  wxStaticBox *visBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("TopoNet Primitives Visibility"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *visBoxSizer = new wxStaticBoxSizer(visBox, wxVERTICAL);
  xxSizer->Add(visBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *vis1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  visBoxSizer->Add(vis1BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxBoxSizer *vis2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  visBoxSizer->Add(vis2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxCheckBox *linkCtrl = new wxCheckBox(this, ID_TOPONET_LINK,
                                        wxT("Links"),
                                        wxDefaultPosition, wxDefaultSize);
  linkCtrl->SetValue(config->AreLinksVisible());
  vis1BoxSizer->Add(linkCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  vis1BoxSizer->AddSpacer(40);
  wxCheckBox *nodeCtrl = new wxCheckBox(this, ID_TOPONET_NODE,
                                        wxT("Nodes"),
                                        wxDefaultPosition, wxDefaultSize);
  nodeCtrl->SetValue(config->AreNodesVisible());
  vis1BoxSizer->Add(nodeCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *linkSeedCtrl = new wxCheckBox(this, ID_TOPONET_LINK_SEED,
                                            wxT("LinkSeeds"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  linkSeedCtrl->SetValue(config->AreLinkSeedsVisible());
  vis2BoxSizer->Add(linkSeedCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// Apply/Quit buttons  
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(buttonSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxButton *rst_ok = new wxButton(this, ID_TOPONET_OK, wxT("&Apply Changes"));
  buttonSizer->Add(rst_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *rst_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(rst_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_TOPONET_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & NetworkLayerConfigDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & NetworkLayerConfigDialog::OnQuit);
}

void NetworkLayerConfigDialog::InitData()
{
// initializing the current configuration
  int len;
  VectorLayerConfig *config = Layer->GetVectorConfig();
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
  Srid = Layer->GetMapSRID();
}

void NetworkLayerConfigDialog::PopulateSRIDs(wxComboBox * sridList)
{
// populating the SRIDs List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT n.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".networks AS n ON (c.network_name = n.network_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (n.srid = y.srid) "
     "WHERE c.coverage_name = %Q UNION "
     "SELECT s.srid, y.ref_sys_name FROM \"%s\".vector_coverages AS c "
     "JOIN \"%s\".vector_coverages_srid AS s ON (s.coverage_name = c.coverage_name) "
     "JOIN \"%s\".spatial_ref_sys AS y ON (s.srid = y.srid) "
     "WHERE c.coverage_name = %Q AND "
     "s.extent_miny IS NOT NULL AND s.extent_maxx IS NOT NULL AND "
     "s.extent_maxx IS NOT NULL ORDER BY s.srid", xdb_prefix, xdb_prefix,
     xdb_prefix, layer, xdb_prefix, xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *srid = results[(i * columns) + 0];
      const char *ref_sys_name = results[(i * columns) + 1];
      wxString str =
        wxString::FromUTF8(srid) + wxT(" ") + wxString::FromUTF8(ref_sys_name);
      sridList->Append(str);
      if (Srid == atoi(srid))
        sridList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void NetworkLayerConfigDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int has_default = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT s.style_name FROM \"%s\".vector_coverages AS r "
     "JOIN \"%s\".SE_vector_styled_layers AS x ON (x.coverage_name = r.coverage_name) "
     "JOIN \"%s\".SE_vector_styles AS s ON (s.style_id = x.style_id) "
     "WHERE r.coverage_name = %Q ORDER BY s.style_name", xdb_prefix,
     xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      if (strcasecmp(style, "default") == 0)
        has_default = 1;
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
    }
  sqlite3_free_table(results);

  QuickStyleTopologyObj *quickStyleTopo = Layer->GetQuickStyleTopology();
  if (quickStyleTopo != NULL)
    {
      wxString str = wxString::FromUTF8(quickStyleTopo->GetUUID());
      stlList->Append(str);
    }

  if (!has_default)
    stlList->Append(wxT("default"));
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (Style == NULL)
            {
              if (stlList->GetString(i).Cmp(wxT("default")) == 0)
                stlList->SetSelection(i);
          } else
            {
              if (stlList->GetString(i).Cmp(wxString::FromUTF8(Style)) == 0)
                stlList->SetSelection(i);
            }
        }
    }
}

void NetworkLayerConfigDialog::DoConfigureMapLayer()
{
// updating the Map Layer definitions
  VectorLayerConfig *config = Layer->GetVectorConfig();
  IsBBoxChanged = false;
  IsConfigChanged = false;
  if (Srid != Layer->GetMapSRID())
    {
      Layer->SetMapSRID(Srid);
      IsConfigChanged = true;
    }
  if ((Style == NULL && config->GetStyle() != NULL)
      || (Style != NULL && config->GetStyle() == NULL))
    {
      config->SetStyle(Style);
      IsConfigChanged = true;
  } else if (Style == NULL && config->GetStyle() == NULL)
    ;
  else
    {
      if (strcmp(Style, config->GetStyle()) != 0)
        {
          config->SetStyle(Style);
          IsConfigChanged = true;
        }
    }

  wxCheckBox *edgeCtrl = (wxCheckBox *) FindWindow(ID_TOPONET_LINK);
  wxCheckBox *nodeCtrl = (wxCheckBox *) FindWindow(ID_TOPONET_NODE);
  wxCheckBox *edgeSeedCtrl = (wxCheckBox *) FindWindow(ID_TOPONET_LINK_SEED);
  bool oldval = config->AreLinksVisible();
  bool newval = edgeCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetLinksVisible(newval);
      IsConfigChanged = true;
    }
  oldval = config->AreNodesVisible();
  newval = nodeCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetNodesVisible(newval);
      IsConfigChanged = true;
    }
  oldval = config->AreLinkSeedsVisible();
  newval = edgeSeedCtrl->IsChecked();
  if (oldval != newval)
    {
      config->SetLinkSeedsVisible(newval);
      IsConfigChanged = true;
    }
}

void NetworkLayerConfigDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void NetworkLayerConfigDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxComboBox *sridList = (wxComboBox *) FindWindow(ID_TOPONET_SRID);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_TOPONET_STYLE);

  wxString str = sridList->GetValue();
  int cut = str.Find(wxT(" "));
  if (cut != wxNOT_FOUND)
    str.Truncate(cut);
  long srid;
  str.ToLong(&srid);
  Srid = srid;
  wxString stl = stlList->GetValue();
  if (Style != NULL)
    free(Style);
  Style = (char *) malloc((stl.Len() * 4) + 1);
  strcpy(Style, stl.ToUTF8());
  DoConfigureMapLayer();
  wxDialog::EndModal(wxID_OK);
}

bool VectorSqlSampleDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  BgColor = wxColour(255, 255, 255);
  if (wxDialog::Create
      (parent, wxID_ANY, wxT("SQL sample: Map Request configuration")) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  DoUpdateSql();
  return true;
}

void VectorSqlSampleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_LAYER, fullName,
                                        wxDefaultPosition, wxSize(470, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: Vector options
  wxBoxSizer *rstSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(rstSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
// Style
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_VECTOR_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// Third row: Image options
  wxBoxSizer *imageSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(imageSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// MIME type
  wxString mime[4];
  mime[0] = wxT("&PNG");
  mime[1] = wxT("&JPEG");
  mime[2] = wxT("&TIFF");
  mime[3] = wxT("PDF");
  wxRadioBox *mimeBox = new wxRadioBox(this, ID_MIME_TYPE,
                                       wxT("&MIME Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 4,
                                       mime, 2,
                                       wxRA_SPECIFY_COLS);
  imageSizer->Add(mimeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  mimeBox->SetSelection(0);
// image Quality
  wxStaticBox *qtyBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Quality"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *qtyBoxSizer = new wxStaticBoxSizer(qtyBox, wxVERTICAL);
  imageSizer->Add(qtyBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSlider *qualityCtrl = new wxSlider(this, ID_IMAGE_QUALITY, 85, 0, 100,
                                       wxDefaultPosition, wxSize(100, 50),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  qtyBoxSizer->Add(qualityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  qualityCtrl->Enable(false);
// Map background
  wxStaticBox *bgBox = new wxStaticBox(this, wxID_STATIC,
                                       wxT("Map Background"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  wxBoxSizer *bg1Sizer = new wxStaticBoxSizer(bgBox, wxVERTICAL);
  imageSizer->Add(bg1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *bg2Sizer = new wxBoxSizer(wxHORIZONTAL);
  bg1Sizer->Add(bg2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxCheckBox *checkeredCtrl =
    new wxCheckBox(this, ID_MAPOPT_CHECKERED_BACKGROUND,
                   wxT("&Transparent"),
                   wxDefaultPosition, wxDefaultSize);
  checkeredCtrl->SetValue(true);
  bg2Sizer->Add(checkeredCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *bgColorBox = new wxStaticBox(this, wxID_ANY,
                                            wxT("Background Color"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *bgColorBoxSizer = new wxStaticBoxSizer(bgColorBox, wxHORIZONTAL);
  bg2Sizer->Add(bgColorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bgColorBmp;
  GetButtonBitmap(BgColor, bgColorBmp);
  wxBitmapButton *bgColorCtrl =
    new wxBitmapButton(this, ID_MAPOPT_SOLID_BACKGROUND, bgColorBmp);
  bgColorCtrl->Enable(false);
  bgColorBoxSizer->Add(bgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// SQL sample
  wxStaticBox *sqlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("SQL sample"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *sqlSizer = new wxStaticBoxSizer(sqlBox, wxVERTICAL);
  boxSizer->Add(sqlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *sqlCtrl = new wxTextCtrl(this, ID_SQL_SAMPLE, Sql,
                                       wxDefaultPosition, wxSize(470, 100),
                                       wxTE_MULTILINE | wxTE_WORDWRAP |
                                       wxTE_READONLY);
  sqlSizer->Add(sqlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// buttons
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *rst_ok = new wxButton(this, ID_VECTOR_COPY, wxT("&Copy"));
  buttonSizer->Add(rst_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *rst_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(rst_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_VECTOR_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorSqlSampleDialog::OnCopy);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorSqlSampleDialog::OnQuit);
  Connect(ID_VECTOR_STYLE, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & VectorSqlSampleDialog::OnStyleChanged);
  Connect(ID_MIME_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & VectorSqlSampleDialog::OnMimeTypeChanged);
  Connect(ID_IMAGE_QUALITY, wxEVT_SLIDER,
          (wxObjectEventFunction) & VectorSqlSampleDialog::OnQualityChanged);
  Connect(ID_MAPOPT_CHECKERED_BACKGROUND, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          VectorSqlSampleDialog::OnTransparentChanged);
  Connect(ID_MAPOPT_SOLID_BACKGROUND, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorSqlSampleDialog::OnBackgroundChanged);
}

void VectorSqlSampleDialog::GetButtonBitmap(wxColour & bgcolor, wxBitmap & bmp)
{
// creating a Bitmap representing some Color
  bmp = wxBitmap(96, 18);
  wxMemoryDC *dc = new wxMemoryDC(bmp);
  dc->SetBrush(wxBrush(bgcolor));
  dc->DrawRectangle(-1, -1, 98, 20);
  delete dc;
}

void VectorSqlSampleDialog::InitData()
{
// initializing the current configuration
  int len;
  VectorLayerConfig *config = Layer->GetVectorConfig();
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
}

void VectorSqlSampleDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int has_default = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT s.style_name FROM \"%s\".vector_coverages AS r "
     "JOIN \"%s\".SE_vector_styled_layers AS x ON (x.coverage_name = r.coverage_name) "
     "JOIN \"%s\".SE_vector_styles AS s ON (s.style_id = x.style_id) "
     "WHERE r.coverage_name = %Q ORDER BY s.style_name", xdb_prefix,
     xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      if (strcasecmp(style, "default") == 0)
        has_default = 1;
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
    }
  sqlite3_free_table(results);

  if (!has_default)
    stlList->Append(wxT("default"));
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (Style == NULL)
            {
              if (stlList->GetString(i).Cmp(wxT("default")) == 0)
                stlList->SetSelection(i);
          } else
            {
              if (stlList->GetString(i).Cmp(wxString::FromUTF8(Style)) == 0)
                stlList->SetSelection(i);
            }
        }
    }
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      // the current Style probably is a QuickStyle
      free(Style);
      Style = NULL;
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (stlList->GetString(i).Cmp(wxT("default")) == 0)
            stlList->SetSelection(i);
        }
    }
}

void VectorSqlSampleDialog::DoUpdateSql()
{
//
// updating the SQL query
//
  char *str;
  char *buf;
  wxString col;
  wxTextCtrl *sqlCtrl = (wxTextCtrl *) FindWindow(ID_SQL_SAMPLE);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_VECTOR_STYLE);
  wxRadioBox *mimeBox = (wxRadioBox *) FindWindow(ID_MIME_TYPE);
  wxCheckBox *transparentCtrl =
    (wxCheckBox *) FindWindow(ID_MAPOPT_CHECKERED_BACKGROUND);
  wxSlider *qualityCtrl = (wxSlider *) FindWindow(ID_IMAGE_QUALITY);
  Sql = wxT("SELECT RL2_GetMapImageFromVector(");
  if (DbPrefix.Len() == 0)
    Sql += wxT("NULL, ");
  else
    {
      str = (char *) malloc((DbPrefix.Len() * 4) + 1);
      strcpy(str, DbPrefix.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// coverage name
  str = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(str, LayerName.ToUTF8());
  buf = sqlite3_mprintf("%Q, ", str);
  free(str);
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// bounding box
  wxString bbox;
  MapPanel->DoPrepareBBox(bbox);
  Sql += bbox;
// image width and height
  buf =
    sqlite3_mprintf("%d, %d, ", MapPanel->GetImageWidth(),
                    MapPanel->GetImageHeight());
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// style
  wxString style = stlList->GetValue();
  if (style.Len() == 0)
    Sql += wxT("'default', ");
  else
    {
      str = (char *) malloc((style.Len() * 4) + 1);
      strcpy(str, style.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// MIME type
  switch (mimeBox->GetSelection())
    {
      case 1:
        Sql += wxT("'image/jpeg', ");
        break;
      case 2:
        Sql += wxT("'image/tiff', ");
        break;
      case 3:
        Sql += wxT("'application/x-pdf', ");
        break;
      default:
        Sql += wxT("'image/png', ");
        break;
    };
// bgColor
  buf =
    sqlite3_mprintf("'#%02x%02x%02x', ", BgColor.Red(), BgColor.Green(),
                    BgColor.Blue());
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// transparent
  if (transparentCtrl->GetValue())
    Sql += wxT("1, ");
  else
    Sql += wxT("0, ");
// quality
  if (mimeBox->GetSelection() == 1)
    {
      buf = sqlite3_mprintf("%d", qualityCtrl->GetValue());
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
  } else
    Sql += wxT("100");
  Sql += wxT(");");
  sqlCtrl->SetValue(Sql);
}

void VectorSqlSampleDialog::OnStyleChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Style selection changed
//
  DoUpdateSql();
}

void VectorSqlSampleDialog::OnMimeTypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// MIME Type selection changed
//
  wxRadioBox *mimeBox = (wxRadioBox *) FindWindow(ID_MIME_TYPE);
  wxSlider *qualityCtrl = (wxSlider *) FindWindow(ID_IMAGE_QUALITY);
  if (mimeBox->GetSelection() == 1)
    qualityCtrl->Enable(true);
  else
    qualityCtrl->Enable(false);
  DoUpdateSql();
}

void VectorSqlSampleDialog::OnQualityChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Image Quality selection changed
//
  DoUpdateSql();
}

void
  VectorSqlSampleDialog::OnTransparentChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Transparent Background selection changed
//
  wxCheckBox *transparentCtrl =
    (wxCheckBox *) FindWindow(ID_MAPOPT_CHECKERED_BACKGROUND);
  wxBitmapButton *btn =
    (wxBitmapButton *) FindWindow(ID_MAPOPT_SOLID_BACKGROUND);
  if (transparentCtrl->GetValue() == false)
    btn->Enable(true);
  else
    btn->Enable(false);
  DoUpdateSql();
}

void
  VectorSqlSampleDialog::OnBackgroundChanged(wxCommandEvent & WXUNUSED(event))
{
// Map Background Solid Color selection
  wxColourData initColor;
  int ret;
  initColor.SetChooseFull(false);
  initColor.SetColour(BgColor);
  wxColourDialog colorDialog(this, &initColor);
  ret = colorDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxColourData colorData = colorDialog.GetColourData();
      BgColor = colorData.GetColour();
      wxBitmap bmp;
      GetButtonBitmap(BgColor, bmp);
      wxBitmapButton *btn =
        (wxBitmapButton *) FindWindow(ID_MAPOPT_SOLID_BACKGROUND);
      btn->SetBitmapLabel(bmp);
      DoUpdateSql();
    }
}

void VectorSqlSampleDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the Map Request (SQL statement) 
//
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(Sql));
      wxTheClipboard->Close();
    }
}

void VectorSqlSampleDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

bool RasterSqlSampleDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  BgColor = wxColour(255, 255, 255);
  if (wxDialog::Create
      (parent, wxID_ANY, wxT("SQL sample: Map Request configuration")) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  DoUpdateSql();
  return true;
}

void RasterSqlSampleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_VECTOR_LAYER, fullName,
                                        wxDefaultPosition, wxSize(470, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: Raster options
  wxBoxSizer *rstSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(rstSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *xxSizer = new wxBoxSizer(wxVERTICAL);
  rstSizer->Add(xxSizer, 0, wxALIGN_TOP | wxALL, 5);
// Style
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  xxSizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxComboBox *stlList =
    new wxComboBox(this, ID_VECTOR_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(350, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// Third row: Image options
  wxBoxSizer *imageSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(imageSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// MIME type
  wxString mime[4];
  mime[0] = wxT("&PNG");
  mime[1] = wxT("&JPEG");
  mime[2] = wxT("&TIFF");
  mime[3] = wxT("PDF");
  wxRadioBox *mimeBox = new wxRadioBox(this, ID_MIME_TYPE,
                                       wxT("&MIME Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 4,
                                       mime, 2,
                                       wxRA_SPECIFY_COLS);
  imageSizer->Add(mimeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  mimeBox->SetSelection(0);
// image Quality
  wxStaticBox *qtyBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Quality"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *qtyBoxSizer = new wxStaticBoxSizer(qtyBox, wxVERTICAL);
  imageSizer->Add(qtyBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSlider *qualityCtrl = new wxSlider(this, ID_IMAGE_QUALITY, 85, 0, 100,
                                       wxDefaultPosition, wxSize(100, 50),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  qtyBoxSizer->Add(qualityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  qualityCtrl->Enable(false);
// Map background
  wxStaticBox *bgBox = new wxStaticBox(this, wxID_STATIC,
                                       wxT("Map Background"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  wxBoxSizer *bg1Sizer = new wxStaticBoxSizer(bgBox, wxVERTICAL);
  imageSizer->Add(bg1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *bg2Sizer = new wxBoxSizer(wxHORIZONTAL);
  bg1Sizer->Add(bg2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxCheckBox *checkeredCtrl =
    new wxCheckBox(this, ID_MAPOPT_CHECKERED_BACKGROUND,
                   wxT("&Transparent"),
                   wxDefaultPosition, wxDefaultSize);
  checkeredCtrl->SetValue(true);
  bg2Sizer->Add(checkeredCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *bgColorBox = new wxStaticBox(this, wxID_ANY,
                                            wxT("Background Color"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *bgColorBoxSizer = new wxStaticBoxSizer(bgColorBox, wxHORIZONTAL);
  bg2Sizer->Add(bgColorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bgColorBmp;
  GetButtonBitmap(BgColor, bgColorBmp);
  wxBitmapButton *bgColorCtrl =
    new wxBitmapButton(this, ID_MAPOPT_SOLID_BACKGROUND, bgColorBmp);
  bgColorCtrl->Enable(false);
  bgColorBoxSizer->Add(bgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// SQL sample
  wxStaticBox *sqlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("SQL sample"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *sqlSizer = new wxStaticBoxSizer(sqlBox, wxVERTICAL);
  boxSizer->Add(sqlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *sqlCtrl = new wxTextCtrl(this, ID_SQL_SAMPLE, Sql,
                                       wxDefaultPosition, wxSize(470, 100),
                                       wxTE_MULTILINE | wxTE_WORDWRAP |
                                       wxTE_READONLY);
  sqlSizer->Add(sqlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// buttons
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *rst_ok = new wxButton(this, ID_VECTOR_COPY, wxT("&Copy"));
  buttonSizer->Add(rst_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *rst_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(rst_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_VECTOR_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSqlSampleDialog::OnCopy);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSqlSampleDialog::OnQuit);
  Connect(ID_VECTOR_STYLE, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & RasterSqlSampleDialog::OnStyleChanged);
  Connect(ID_MIME_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & RasterSqlSampleDialog::OnMimeTypeChanged);
  Connect(ID_IMAGE_QUALITY, wxEVT_SLIDER,
          (wxObjectEventFunction) & RasterSqlSampleDialog::OnQualityChanged);
  Connect(ID_MAPOPT_CHECKERED_BACKGROUND, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          RasterSqlSampleDialog::OnTransparentChanged);
  Connect(ID_MAPOPT_SOLID_BACKGROUND, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSqlSampleDialog::OnBackgroundChanged);
}

void RasterSqlSampleDialog::GetButtonBitmap(wxColour & bgcolor, wxBitmap & bmp)
{
// creating a Bitmap representing some Color
  bmp = wxBitmap(96, 18);
  wxMemoryDC *dc = new wxMemoryDC(bmp);
  dc->SetBrush(wxBrush(bgcolor));
  dc->DrawRectangle(-1, -1, 98, 20);
  delete dc;
}

void RasterSqlSampleDialog::InitData()
{
// initializing the current configuration
  int len;
  RasterLayerConfig *config = Layer->GetRasterConfig();
  Style = NULL;
  if (config->GetStyle() != NULL)
    {
      len = strlen(config->GetStyle());
      Style = (char *) malloc(len + 1);
      strcpy(Style, config->GetStyle());
    }
}

void RasterSqlSampleDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  int has_default = 0;

  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT s.style_name FROM \"%s\".raster_coverages AS r "
     "JOIN \"%s\".SE_raster_styled_layers AS x ON (x.coverage_name = r.coverage_name) "
     "JOIN \"%s\".SE_raster_styles AS s ON (s.style_id = x.style_id) "
     "WHERE r.coverage_name = %Q ORDER BY s.style_name", xdb_prefix,
     xdb_prefix, xdb_prefix, layer);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      if (strcasecmp(style, "default") == 0)
        has_default = 1;
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
    }
  sqlite3_free_table(results);

  if (!has_default)
    stlList->Append(wxT("default"));
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (Style == NULL)
            {
              if (stlList->GetString(i).Cmp(wxT("default")) == 0)
                stlList->SetSelection(i);
          } else
            {
              if (stlList->GetString(i).Cmp(wxString::FromUTF8(Style)) == 0)
                stlList->SetSelection(i);
            }
        }
    }
  if (stlList->GetSelection() == wxNOT_FOUND)
    {
      // the current Style probably is a QuickStyle
      free(Style);
      Style = NULL;
      for (int i = 0; i < (int) stlList->GetCount(); i++)
        {
          if (stlList->GetString(i).Cmp(wxT("default")) == 0)
            stlList->SetSelection(i);
        }
    }
}

void RasterSqlSampleDialog::DoUpdateSql()
{
//
// updating the SQL query
//
  char *str;
  char *buf;
  wxString col;
  wxTextCtrl *sqlCtrl = (wxTextCtrl *) FindWindow(ID_SQL_SAMPLE);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_VECTOR_STYLE);
  wxRadioBox *mimeBox = (wxRadioBox *) FindWindow(ID_MIME_TYPE);
  wxCheckBox *transparentCtrl =
    (wxCheckBox *) FindWindow(ID_MAPOPT_CHECKERED_BACKGROUND);
  wxSlider *qualityCtrl = (wxSlider *) FindWindow(ID_IMAGE_QUALITY);
  Sql = wxT("SELECT RL2_GetMapImageFromRaster(");
  if (DbPrefix.Len() == 0)
    Sql += wxT("NULL, ");
  else
    {
      str = (char *) malloc((DbPrefix.Len() * 4) + 1);
      strcpy(str, DbPrefix.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// coverage name
  str = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(str, LayerName.ToUTF8());
  buf = sqlite3_mprintf("%Q, ", str);
  free(str);
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// bounding box
  wxString bbox;
  MapPanel->DoPrepareBBox(bbox);
  Sql += bbox;
// image width and height
  buf =
    sqlite3_mprintf("%d, %d, ", MapPanel->GetImageWidth(),
                    MapPanel->GetImageHeight());
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// style
  wxString style = stlList->GetValue();
  if (style.Len() == 0)
    Sql += wxT("'default', ");
  else
    {
      str = (char *) malloc((style.Len() * 4) + 1);
      strcpy(str, style.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// MIME type
  switch (mimeBox->GetSelection())
    {
      case 1:
        Sql += wxT("'image/jpeg', ");
        break;
      case 2:
        Sql += wxT("'image/tiff', ");
        break;
      case 3:
        Sql += wxT("'application/x-pdf', ");
        break;
      default:
        Sql += wxT("'image/png', ");
        break;
    };
// bgColor
  buf =
    sqlite3_mprintf("'#%02x%02x%02x', ", BgColor.Red(), BgColor.Green(),
                    BgColor.Blue());
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// transparent
  if (transparentCtrl->GetValue())
    Sql += wxT("1, ");
  else
    Sql += wxT("0, ");
// quality
  if (mimeBox->GetSelection() == 1)
    {
      buf = sqlite3_mprintf("%d", qualityCtrl->GetValue());
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
  } else
    Sql += wxT("100");
  Sql += wxT(");");
  sqlCtrl->SetValue(Sql);
}

void RasterSqlSampleDialog::OnStyleChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Style selection changed
//
  DoUpdateSql();
}

void RasterSqlSampleDialog::OnMimeTypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// MIME Type selection changed
//
  wxRadioBox *mimeBox = (wxRadioBox *) FindWindow(ID_MIME_TYPE);
  wxSlider *qualityCtrl = (wxSlider *) FindWindow(ID_IMAGE_QUALITY);
  if (mimeBox->GetSelection() == 1)
    qualityCtrl->Enable(true);
  else
    qualityCtrl->Enable(false);
  DoUpdateSql();
}

void RasterSqlSampleDialog::OnQualityChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Image Quality selection changed
//
  DoUpdateSql();
}

void RasterSqlSampleDialog::
OnTransparentChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Transparent Background selection changed
//
  wxCheckBox *transparentCtrl =
    (wxCheckBox *) FindWindow(ID_MAPOPT_CHECKERED_BACKGROUND);
  wxBitmapButton *btn =
    (wxBitmapButton *) FindWindow(ID_MAPOPT_SOLID_BACKGROUND);
  if (transparentCtrl->GetValue() == false)
    btn->Enable(true);
  else
    btn->Enable(false);
  DoUpdateSql();
}

void RasterSqlSampleDialog::
OnBackgroundChanged(wxCommandEvent & WXUNUSED(event))
{
// Map Background Solid Color selection
  wxColourData initColor;
  int ret;
  initColor.SetChooseFull(false);
  initColor.SetColour(BgColor);
  wxColourDialog colorDialog(this, &initColor);
  ret = colorDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxColourData colorData = colorDialog.GetColourData();
      BgColor = colorData.GetColour();
      wxBitmap bmp;
      GetButtonBitmap(BgColor, bmp);
      wxBitmapButton *btn =
        (wxBitmapButton *) FindWindow(ID_MAPOPT_SOLID_BACKGROUND);
      btn->SetBitmapLabel(bmp);
      DoUpdateSql();
    }
}

void RasterSqlSampleDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the Map Request (SQL statement) 
//
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(Sql));
      wxTheClipboard->Close();
    }
}

void RasterSqlSampleDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

bool WmsSqlSampleDialog::Create(MyMapPanel * parent, MapLayer * layer,
                                bool request_url)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  RequestURL = request_url;
  Url = wxString::FromUTF8(layer->GetWmsGetMapURL());
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  wxString title = wxT("SQL sample: Map Request configuration");
  if (RequestURL == true)
    title = wxT("URL sample: Map Request configuration");
  if (wxDialog::Create(parent, wxID_ANY, title) == false)
    return false;
// populates individual controls
  InitData();
  CreateControls();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  DoUpdateSql();
  return true;
}

WmsSqlSampleDialog::~WmsSqlSampleDialog()
{
  if (Version != NULL)
    free(Version);
  if (MaxVersion != NULL)
    free(MaxVersion);
  if (Style != NULL)
    free(Style);
  if (ImageFormat != NULL)
    free(ImageFormat);
  if (BgColor != NULL)
    free(BgColor);
}

void WmsSqlSampleDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// First row: URL
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *urlSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(urlSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *urlBox = new wxStaticBox(this, wxID_ANY,
                                        wxT("WMS GetMap URL"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *urlBoxSizer = new wxStaticBoxSizer(urlBox, wxHORIZONTAL);
  urlSizer->Add(urlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *urlCtrl = new wxTextCtrl(this, ID_WMS_URL, Url,
                                       wxDefaultPosition, wxSize(800, 22),
                                       wxTE_READONLY);
  urlBoxSizer->Add(urlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Second row: WMS options
  wxBoxSizer *optsSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(optsSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// WMS version
  wxBoxSizer *wmsSizer = new wxBoxSizer(wxHORIZONTAL);
  optsSizer->Add(wmsSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString ver[4];
  ver[0] = wxT("WMS &1.0.0");
  ver[1] = wxT("WMS &1.1.0");
  ver[2] = wxT("WMS &1.1.1");
  ver[3] = wxT("WMS &1.3.0");
  wxRadioBox *versionBox = new wxRadioBox(this, ID_WMS_VERSION,
                                          wxT("WMS &Version"),
                                          wxDefaultPosition,
                                          wxDefaultSize, 4,
                                          ver, 4,
                                          wxRA_SPECIFY_ROWS);
  FindMaxVersion();
  if (MaxVersion == NULL)
    {
      versionBox->Enable(1, false);
      versionBox->Enable(2, false);
      versionBox->Enable(3, false);
  } else
    {
      if (strcmp(MaxVersion, "1.1.0") == 0)
        {
          versionBox->Enable(2, false);
          versionBox->Enable(3, false);
      } else if (strcmp(MaxVersion, "1.1.1") == 0)
        {
          versionBox->Enable(3, false);
      } else if (strcmp(MaxVersion, "1.3.0") == 0)
        ;
      else
        {
          versionBox->Enable(1, false);
          versionBox->Enable(2, false);
          versionBox->Enable(3, false);
        }
    }
  if (strcmp(Version, "1.0.0") == 0)
    versionBox->SetSelection(0);
  else if (strcmp(Version, "1.1.0") == 0)
    versionBox->SetSelection(1);
  else if (strcmp(Version, "1.1.1") == 0)
    versionBox->SetSelection(2);
  else
    versionBox->SetSelection(3);
  wmsSizer->Add(versionBox, 0, wxALIGN_TOP | wxALL, 5);
// layer Name
  wxBoxSizer *opt2Sizer = new wxBoxSizer(wxVERTICAL);
  optsSizer->Add(opt2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  opt2Sizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer Name"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_WMS_LAYER, LayerName,
                                        wxDefaultPosition, wxSize(420, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Style
  wxBoxSizer *opt3Sizer = new wxBoxSizer(wxHORIZONTAL);
  opt2Sizer->Add(opt3Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *stlBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *stlBoxSizer = new wxStaticBoxSizer(stlBox, wxVERTICAL);
  opt3Sizer->Add(stlBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *stlList =
    new wxComboBox(this, ID_WMS_STYLE, wxT(""), wxDefaultPosition,
                   wxSize(250, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateStyles(stlList);
  stlBoxSizer->Add(stlList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// image format
  wxStaticBox *fmtBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Image Format"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *fmtBoxSizer = new wxStaticBoxSizer(fmtBox, wxVERTICAL);
  opt3Sizer->Add(fmtBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *fmtList =
    new wxComboBox(this, ID_WMS_FORMAT, wxT(""), wxDefaultPosition,
                   wxSize(150, 21), 0, NULL,
                   wxCB_DROPDOWN | wxCB_READONLY);
  PopulateImageFormats(fmtList);
  fmtBoxSizer->Add(fmtList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// transparent / background color
  wxStaticBox *bgColorBox = new wxStaticBox(this, wxID_ANY,
                                            wxT("Background Color"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *bgColorBoxSizer = new wxStaticBoxSizer(bgColorBox, wxHORIZONTAL);
  optsSizer->Add(bgColorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableBgColorCtrl = new wxCheckBox(this, ID_WMS_ENABLE_BGCOLOR,
                                                 wxT("Transparent"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  if (BgColor == NULL)
    {
      Transparent = 1;
      enableBgColorCtrl->SetValue(true);
  } else
    {
      Transparent = 0;
      enableBgColorCtrl->SetValue(false);
    }
  bgColorBoxSizer->Add(enableBgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                       5);
  wxBitmap bgColorBmp;
  GetButtonBitmap(BgColor, bgColorBmp);
  wxBitmapButton *bgColorCtrl =
    new wxBitmapButton(this, ID_WMS_BGCOLOR, bgColorBmp);
  if (Transparent)
    bgColorCtrl->Enable(false);
  else
    bgColorCtrl->Enable(true);
  bgColorBoxSizer->Add(bgColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// SQL sample
  wxString title = wxT("SQL sample");
  if (RequestURL == true)
    title = wxT("URL sample");
  wxStaticBox *sqlBox = new wxStaticBox(this, wxID_STATIC,
                                        title,
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *sqlSizer = new wxStaticBoxSizer(sqlBox, wxVERTICAL);
  lyrBoxSizer->Add(sqlSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *sqlCtrl = new wxTextCtrl(this, ID_SQL_SAMPLE, Sql,
                                       wxDefaultPosition, wxSize(800, 70),
                                       wxTE_MULTILINE | wxTE_WORDWRAP |
                                       wxTE_READONLY);
  sqlSizer->Add(sqlCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  lyrBoxSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *wms_ok = new wxButton(this, ID_VECTOR_COPY, wxT("&Copy"));
  buttonSizer->Add(wms_ok, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *wms_quit = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  buttonSizer->Add(wms_quit, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// appends event handlers
  Connect(ID_VECTOR_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnCopy);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnQuit);
  Connect(ID_WMS_VERSION, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnVersionChanged);
  Connect(ID_WMS_STYLE, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnStyleChanged);
  Connect(ID_WMS_FORMAT, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnMimeTypeChanged);
  Connect(ID_WMS_ENABLE_BGCOLOR, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnTransparentChanged);
  Connect(ID_WMS_BGCOLOR, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & WmsSqlSampleDialog::OnBgColorChanged);
}

unsigned char WmsSqlSampleDialog::ParseHex(const char *byte)
{
// parsing an Hex Byte
  unsigned char value = 0;
  switch (byte[0])
    {
      case '0':
        value = 16 * 0;
        break;
      case '1':
        value = 16 * 1;
        break;
      case '2':
        value = 16 * 2;
        break;
      case '3':
        value = 16 * 3;
        break;
      case '4':
        value = 16 * 4;
        break;
      case '5':
        value = 16 * 5;
        break;
      case '6':
        value = 16 * 6;
        break;
      case '7':
        value = 16 * 7;
        break;
      case '8':
        value = 16 * 8;
        break;
      case '9':
        value = 16 * 9;
        break;
      case 'A':
      case 'a':
        value = 16 * 10;
        break;
      case 'B':
      case 'b':
        value = 16 * 11;
        break;
      case 'C':
      case 'c':
        value = 16 * 12;
        break;
      case 'D':
      case 'd':
        value = 16 * 13;
        break;
      case 'E':
      case 'e':
        value = 16 * 14;
        break;
      case 'F':
      case 'f':
        value = 16 * 15;
        break;
    };
  switch (byte[1])
    {
      case '0':
        value += 0;
        break;
      case '1':
        value += 1;
        break;
      case '2':
        value += 2;
        break;
      case '3':
        value += 3;
        break;
      case '4':
        value += 4;
        break;
      case '5':
        value += 5;
        break;
      case '6':
        value += 6;
        break;
      case '7':
        value += 7;
        break;
      case '8':
        value += 8;
        break;
      case '9':
        value += 9;
        break;
      case 'A':
      case 'a':
        value += 10;
        break;
      case 'B':
      case 'b':
        value += 11;
        break;
      case 'C':
      case 'c':
        value += 12;
        break;
      case 'D':
      case 'd':
        value += 13;
        break;
      case 'E':
      case 'e':
        value += 14;
        break;
      case 'F':
      case 'f':
        value += 15;
        break;
    };
  return value;
}

void
  WmsSqlSampleDialog::ParseBgColor(const char *color, unsigned char *red,
                                   unsigned char *green, unsigned char *blue)
{
// parsing an Hex RGB color
  char byte[2];
  *red = 0x80;
  *green = 0x80;
  *blue = 0x80;
  if (color == NULL)
    return;

  byte[0] = *(color + 0);
  byte[1] = *(color + 1);
  *red = ParseHex(byte);
  byte[0] = *(color + 2);
  byte[1] = *(color + 3);
  *green = ParseHex(byte);
  byte[0] = *(color + 4);
  byte[1] = *(color + 5);
  *blue = ParseHex(byte);
}

void WmsSqlSampleDialog::GetButtonBitmap(const char *bgcolor, wxBitmap & bmp)
{
// creating a Bitmap representing some Color
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  ParseBgColor(bgcolor, &red, &green, &blue);
  wxColour color = wxColour(red, green, blue);
  bmp = wxBitmap(96, 64);
  wxMemoryDC *dc = new wxMemoryDC(bmp);
  dc->SetBrush(wxBrush(color));
  dc->DrawRectangle(-1, -1, 98, 66);
  delete dc;
}

void WmsSqlSampleDialog::InitData()
{
// initializing the current configuration
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT version, srs, format, style, is_queryable, flip_axes, "
     "is_cached, tiled, tile_width, tile_height, bgcolor, transparent "
     "FROM \"%s\".wms_getmap WHERE url = %Q AND layer_name = %Q",
     xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      Version = NULL;
      ImageFormat = NULL;
      Style = NULL;
      Transparent = 1;
      BgColor = NULL;
      return;
    }

  for (int i = 1; i <= rows; i++)
    {
      // fetching data
      int len;
      const char *version = results[(i * columns) + 0];
      const char *format = results[(i * columns) + 2];
      const char *style = results[(i * columns) + 3];
      const char *bgcolor = results[(i * columns) + 10];
      Transparent = atoi(results[(i * columns) + 11]);
      if (version == NULL)
        Version = NULL;
      else
        {
          len = strlen(version);
          Version = (char *) malloc(len + 1);
          strcpy(Version, version);
        }
      if (format == NULL)
        ImageFormat = NULL;
      else
        {
          len = strlen(format);
          ImageFormat = (char *) malloc(len + 1);
          strcpy(ImageFormat, format);
        }
      if (style == NULL)
        Style = NULL;
      else
        {
          len = strlen(style);
          Style = (char *) malloc(len + 1);
          strcpy(Style, style);
        }
      if (bgcolor == NULL)
        BgColor = NULL;
      else
        {
          if (strlen(bgcolor) == 6)
            {
              BgColor = (char *) malloc(7);
              strcpy(BgColor, bgcolor);
          } else
            BgColor = NULL;
        }
    }
  sqlite3_free_table(results);
}

void WmsSqlSampleDialog::FindMaxVersion()
{
// identifying the Max WMS Version
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;
  MaxVersion = NULL;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT Max(s.value) FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_settings AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.key = 'version'",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // fetching data
      int len;
      const char *version = results[(i * columns) + 0];
      if (version == NULL)
        MaxVersion = NULL;
      else
        {
          len = strlen(version);
          MaxVersion = (char *) malloc(len + 1);
          strcpy(MaxVersion, version);
        }
    }
  sqlite3_free_table(results);
}

void WmsSqlSampleDialog::PopulateStyles(wxComboBox * stlList)
{
// populating the Styles List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT s.value FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_settings AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.key = 'style' ORDER BY s.id",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *style = results[(i * columns) + 0];
      wxString str = wxString::FromUTF8(style);
      stlList->Append(str);
      if (strcmp(Style, style) == 0)
        stlList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

void WmsSqlSampleDialog::PopulateImageFormats(wxComboBox * fmtList)
{
// populating the Image Formats List
  int ret;
  char *sql;
  char **results;
  int rows;
  int columns;

  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  char *db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
  strcpy(db_prefix, DbPrefix.ToUTF8());
  char *xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  sql = sqlite3_mprintf("SELECT s.value FROM \"%s\".wms_getmap AS m "
                        "JOIN \"%s\".wms_settings AS s ON (s.parent_id = m.id) "
                        "WHERE m.url = %Q AND m.layer_name = %Q AND s.key = 'format' ORDER BY s.id",
                        xdb_prefix, xdb_prefix, url, layer);
  free(url);
  free(layer);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      const char *format = results[(i * columns) + 0];
      wxString str = wxString::FromUTF8(format);
      fmtList->Append(str);
      if (strcmp(ImageFormat, format) == 0)
        fmtList->SetSelection(i - 1);
    }
  sqlite3_free_table(results);
}

int WmsSqlSampleDialog::CheckMarker(wxString & url)
{
// testing if some "?" marker is already defined
  int ret = url.Find('?');
  if (ret == wxNOT_FOUND)
    return 0;
  return 1;
}

int
  WmsSqlSampleDialog::DoQueryWmsCoverage(const char *db_prefix,
                                         const char *cvg_name, int srid,
                                         char **url, int *swap_axes)
{
// querying the WMS Coverage definitions
  int len;
  int i;
  char **results;
  int rows;
  int columns;
  char *sql;
  int ret;
  char *xdb_prefix;
  int ok = 0;
  if (db_prefix == NULL)
    db_prefix = "MAIN";
  xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  sql = sqlite3_mprintf("SELECT w.url, s.has_flipped_axes "
                        "FROM \"%s\".wms_getmap AS w, \"%s\".spatial_ref_sys_aux AS s "
                        "WHERE w.layer_name = %Q AND s.srid = %d",
                        xdb_prefix, xdb_prefix, cvg_name, srid);
  free(xdb_prefix);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows,
                      &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return 0;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *val = results[(i * columns) + 0];
          if (*url != NULL)
            free(*url);
          len = strlen(val);
          *url = (char *) malloc(len + 1);
          strcpy(*url, val);
          val = results[(i * columns) + 1];
          *swap_axes = atoi(val);
          ok = 1;
        }
    }
  sqlite3_free_table(results);
  if (!ok)
    return 0;
  return 1;
}

void WmsSqlSampleDialog::DoUpdateUrl()
{
//
// updating the Request URL
//
  wxString col;
  wxTextCtrl *sqlCtrl = (wxTextCtrl *) FindWindow(ID_SQL_SAMPLE);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  wxComboBox *mimeList = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  wxCheckBox *transparentCtrl =
    (wxCheckBox *) FindWindow(ID_WMS_ENABLE_BGCOLOR);
  char *db_prefix;
  int srid;
  const char *crs_prefix = "CRS";
  char *crs = NULL;
  char *url = (char *) malloc((Url.Len() * 4) + 1);
  strcpy(url, Url.ToUTF8());
  const char *version = Version;
  char *layer = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(layer, LayerName.ToUTF8());
  char *style;
  char *format = ImageFormat;
  int swap_xy;
  double minx;
  double miny;
  double maxx;
  double maxy;
  int width = MapPanel->GetImageWidth();
  int height = MapPanel->GetImageHeight();
  int opaque;
  char *bg_color;
  int valid_bg_color = 1;
  char *request;
  int force_marker = CheckMarker(Url);

  MapPanel->GetBBox(&srid, &minx, &miny, &maxx, &maxy);
  if (DbPrefix.Len() == 0)
    db_prefix = NULL;
  else
    {
      db_prefix = (char *) malloc((DbPrefix.Len() * 4) + 1);
      strcpy(db_prefix, DbPrefix.ToUTF8());
    }
  if (!DoQueryWmsCoverage(db_prefix, layer, srid, &url, &swap_xy))
    return;

  crs = sqlite3_mprintf("EPSG:%d", srid);
  wxString xstyle = stlList->GetValue();
  if (xstyle.Len() == 0)
    style = NULL;
  else
    {
      style = (char *) malloc((xstyle.Len() * 4) + 1);
      strcpy(style, xstyle.ToUTF8());
    }
  wxString xformat = mimeList->GetValue();
  if (xformat.Len() == 0)
    format = NULL;
  else
    {
      format = (char *) malloc((xformat.Len() * 4) + 1);
      strcpy(format, xformat.ToUTF8());
    }

/* masking NULL arguments */
  if (version == NULL)
    version = "";
  if (strcmp(version, "1.3.0") != 0)
    swap_xy = 0;
  if (style == NULL)
    {
      style = (char *) malloc(1);
      *style = '\0';
    }
  if (format == NULL)
    {
      format = (char *) malloc(1);
      *format = '\0';
    }
/* validating the background color */
  if (BgColor == NULL)
    valid_bg_color = 0;
  else if (strlen(BgColor) != 6)
    valid_bg_color = 0;
  else
    {
      int i;
      for (i = 0; i < 6; i++)
        {
          char h = *(BgColor + i);
          if ((h >= '0' && h <= '9') || (h >= 'a' && h <= 'f')
              || (h >= 'A' && h <= 'F'))
            ;
          else
            valid_bg_color = 0;
        }
    }
  if (valid_bg_color)
    bg_color = sqlite3_mprintf("0x%s", BgColor);
  else
    bg_color = sqlite3_mprintf("0xFFFFFF");
  if (transparentCtrl->GetValue())
    opaque = 0;
  else
    opaque = 1;

/* preparing the request URL */
  if (strcmp(version, "1.3.0") < 0)
    {
      /* earlier versions of the protocol require SRS instead of CRS */
      crs_prefix = "SRS";
    }
  if (force_marker)
    {
      /* "?" marker not declared */
      if (swap_xy)
        request =
          sqlite3_mprintf("%s?SERVICE=WMS&REQUEST=GetMap&VERSION=%s"
                          "&LAYERS=%s&%s=%s&BBOX=%1.6f,%1.6f,%1.6f,%1.6f"
                          "&WIDTH=%d&HEIGHT=%d&STYLES=%s&FORMAT=%s"
                          "&TRANSPARENT=%s&BGCOLOR=%s", url,
                          version, layer, crs_prefix, crs, miny,
                          minx, maxy, maxx, width, height, style,
                          format, (opaque == 0) ? "TRUE" : "FALSE", bg_color);
      else
        request =
          sqlite3_mprintf("%s?SERVICE=WMS&REQUEST=GetMap&VERSION=%s"
                          "&LAYERS=%s&%s=%s&BBOX=%1.6f,%1.6f,%1.6f,%1.6f"
                          "&WIDTH=%d&HEIGHT=%d&STYLES=%s&FORMAT=%s"
                          "&TRANSPARENT=%s&BGCOLOR=%s", url,
                          version, layer, crs_prefix, crs, minx,
                          miny, maxx, maxy, width, height, style,
                          format, (opaque == 0) ? "TRUE" : "FALSE", bg_color);
  } else
    {
      /* "?" marker already defined */
      if (swap_xy)
        request =
          sqlite3_mprintf("%sSERVICE=WMS&REQUEST=GetMap&VERSION=%s"
                          "&LAYERS=%s&%s=%s&BBOX=%1.6f,%1.6f,%1.6f,%1.6f"
                          "&WIDTH=%d&HEIGHT=%d&STYLES=%s&FORMAT=%s"
                          "&TRANSPARENT=%s&BGCOLOR=%s", url,
                          version, layer, crs_prefix, crs, miny,
                          minx, maxy, maxx, width, height, style,
                          format, (opaque == 0) ? "TRUE" : "FALSE", bg_color);
      else
        request =
          sqlite3_mprintf("%sSERVICE=WMS&REQUEST=GetMap&VERSION=%s"
                          "&LAYERS=%s&%s=%s&BBOX=%1.6f,%1.6f,%1.6f,%1.6f"
                          "&WIDTH=%d&HEIGHT=%d&STYLES=%s&FORMAT=%s"
                          "&TRANSPARENT=%s&BGCOLOR=%s", url,
                          version, layer, crs_prefix, crs, minx,
                          miny, maxx, maxy, width, height, style,
                          format, (opaque == 0) ? "TRUE" : "FALSE", bg_color);
    }
  Sql = wxString::FromUTF8(request);
  sqlite3_free(request);
  sqlCtrl->SetValue(Sql);

  if (url != NULL)
    free(url);
  if (layer != NULL)
    free(layer);
  if (db_prefix != NULL)
    free(db_prefix);
  if (crs != NULL)
    sqlite3_free(crs);
  if (style != NULL)
    free(style);
  if (format != NULL)
    free(format);
  if (bg_color != NULL)
    sqlite3_free(bg_color);
}

void WmsSqlSampleDialog::DoUpdateSql()
{
//
// updating the SQL query
//
  if (RequestURL == true)
    {
      DoUpdateUrl();
      return;
    }
  char *str;
  char *buf;
  wxString col;
  wxTextCtrl *sqlCtrl = (wxTextCtrl *) FindWindow(ID_SQL_SAMPLE);
  wxRadioBox *versionBox = (wxRadioBox *) FindWindow(ID_WMS_VERSION);
  wxComboBox *stlList = (wxComboBox *) FindWindow(ID_WMS_STYLE);
  wxComboBox *mimeList = (wxComboBox *) FindWindow(ID_WMS_FORMAT);
  wxCheckBox *transparentCtrl =
    (wxCheckBox *) FindWindow(ID_WMS_ENABLE_BGCOLOR);
  Sql = wxT("SELECT RL2_GetMapImageFromWMS(");
  if (DbPrefix.Len() == 0)
    Sql += wxT("NULL, ");
  else
    {
      str = (char *) malloc((DbPrefix.Len() * 4) + 1);
      strcpy(str, DbPrefix.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// coverage name
  str = (char *) malloc((LayerName.Len() * 4) + 1);
  strcpy(str, LayerName.ToUTF8());
  buf = sqlite3_mprintf("%Q, ", str);
  free(str);
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// bounding box
  wxString bbox;
  MapPanel->DoPrepareBBox(bbox);
  Sql += bbox;
// image width and height
  buf =
    sqlite3_mprintf("%d, %d, ", MapPanel->GetImageWidth(),
                    MapPanel->GetImageHeight());
  col = wxString::FromUTF8(buf);
  sqlite3_free(buf);
  Sql += col;
// WMS version
  switch (versionBox->GetSelection())
    {
      case 1:
        Sql += wxT("'1.1.0', ");
        break;
      case 2:
        Sql += wxT("'1.1.1', ");
        break;
      case 3:
        Sql += wxT("'1.3.0', ");
        break;
      default:
        Sql += wxT("'1.0.0', ");
        break;
    };
// style
  wxString style = stlList->GetValue();
  if (style.Len() == 0)
    Sql += wxT("'default', ");
  else
    {
      str = (char *) malloc((style.Len() * 4) + 1);
      strcpy(str, style.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// MIME type
  wxString format = mimeList->GetValue();
  if (format.Len() == 0)
    Sql += wxT("'default', ");
  else
    {
      str = (char *) malloc((format.Len() * 4) + 1);
      strcpy(str, format.ToUTF8());
      buf = sqlite3_mprintf("%Q, ", str);
      free(str);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// bgColor
  if (BgColor == NULL)
    Sql += wxT("'#ffffff', ");
  else
    {
      buf = sqlite3_mprintf("'#%s', ", BgColor);
      col = wxString::FromUTF8(buf);
      sqlite3_free(buf);
      Sql += col;
    }
// transparent
  if (transparentCtrl->GetValue())
    Sql += wxT("1");
  else
    Sql += wxT("0");
  Sql += wxT(");");
  sqlCtrl->SetValue(Sql);
}

void WmsSqlSampleDialog::OnVersionChanged(wxCommandEvent & WXUNUSED(event))
{
//
// WMS Version selection changed
//
  DoUpdateSql();
}

void WmsSqlSampleDialog::OnStyleChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Style selection changed
//
  DoUpdateSql();
}

void WmsSqlSampleDialog::OnMimeTypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// MIME Type selection changed
//
  DoUpdateSql();
}

void WmsSqlSampleDialog::OnTransparentChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Transparent selection changed
//
  wxBitmapButton *btn = (wxBitmapButton *) FindWindow(ID_WMS_BGCOLOR);
  if (Transparent == 1)
    {
      Transparent = 0;
      if (BgColor != NULL)
        free(BgColor);
      BgColor = (char *) malloc(7);
      strcpy(BgColor, "ffffff");
      wxBitmap bmp;
      GetButtonBitmap(BgColor, bmp);
      btn->SetBitmapLabel(bmp);
      btn->Enable(true);
  } else
    {
      Transparent = 0;
      btn->Enable(false);
      if (BgColor != NULL)
        {
          free(BgColor);
          BgColor = NULL;
        }
    }
  DoUpdateSql();
}

void WmsSqlSampleDialog::OnBgColorChanged(wxCommandEvent & WXUNUSED(event))
{
// BgColor selection
  wxColourData initColor;
  int ret;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  ParseBgColor(BgColor, &red, &green, &blue);
  wxColour color = wxColour(red, green, blue);
  initColor.SetChooseFull(false);
  initColor.SetColour(color);
  wxColourDialog colorDialog(this, &initColor);
  ret = colorDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxColourData colorData = colorDialog.GetColourData();
      color = colorData.GetColour();
      char byte[3];
      sprintf(byte, "%02x", color.Red());
      memcpy(BgColor, byte, 2);
      sprintf(byte, "%02x", color.Green());
      memcpy(BgColor + 2, byte, 2);
      sprintf(byte, "%02x", color.Blue());
      memcpy(BgColor + 4, byte, 2);
      *(BgColor + 6) = '\0';
      wxBitmap bmp;
      GetButtonBitmap(BgColor, bmp);
      wxBitmapButton *bgColorCtrl =
        (wxBitmapButton *) FindWindow(ID_WMS_BGCOLOR);
      bgColorCtrl->SetBitmapLabel(bmp);
      DoUpdateSql();
    }
}

void WmsSqlSampleDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the Map Request (SQL statement) 
//
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(Sql));
      wxTheClipboard->Close();
    }
}

void WmsSqlSampleDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}
