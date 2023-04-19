/*
/ TableTree.cpp
/ tree control to show tables, columns, indices and triggers
/
/ version 1.7, 2013 May 8
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2008-2013  Alessandro Furieri
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

#include <float.h>

#include "Classdef.h"

#include "wx/filename.h"
#include "wx/imaglist.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#define strcasecmp	_stricmp
#endif

//
// ICONs in XPM format [universally portable]
//
#include "icons/db.xpm"
#include "icons/table.xpm"
#include "icons/geotable.xpm"
#include "icons/vtable.xpm"
#include "icons/geovtable.xpm"
#include "icons/composer.xpm"
#include "icons/view.xpm"
#include "icons/geoview.xpm"
#include "icons/gpkgtable.xpm"
#include "icons/gpkgvtable.xpm"
#include "icons/pkey.xpm"
#include "icons/column.xpm"
#include "icons/index.xpm"
#include "icons/trigger.xpm"
#include "icons/geometry.xpm"
#include "icons/gpkggeom.xpm"
#include "icons/virtshp.xpm"
#include "icons/virtgpkg.xpm"
#include "icons/spatialidx.xpm"
#include "icons/mbrcache.xpm"
#include "icons/kill_spindex.xpm"
#include "icons/gpkg_spatialidx.xpm"
#include "icons/dumpshp.xpm"
#include "icons/dumpgeojs.xpm"
#include "icons/tmp_table.xpm"
#include "icons/tmp_view.xpm"
#include "icons/malformed_geoms.xpm"
#include "icons/statistics.xpm"
#include "icons/map_preview.xpm"
#include "icons/tables.xpm"
#include "icons/foreign_key.xpm"
#include "icons/primary_key.xpm"
#include "icons/topo_geo.xpm"
#include "icons/topo_net.xpm"
#include "icons/attach.xpm"
#include "icons/checkgeom.xpm"
#include "icons/sanegeom.xpm"
#include "icons/coverage.xpm"
#include "icons/coverage_tiles.xpm"
#include "icons/vector.xpm"
#include "icons/linestrings_on.xpm"
#include "icons/points_on.xpm"
#include "icons/polygons_on.xpm"
#include "icons/clone.xpm"
#include "icons/wms.xpm"
#include "icons/postgres.xpm"
#include "icons/postgres_schema.xpm"
#include "icons/postgres_table.xpm"
#include "icons/postgres_view.xpm"

MyTableTree::MyTableTree(MyFrame * parent, wxWindowID id):wxTreeCtrl(parent, id)
{
//
// constructor: TREE control to show DB objects
//
  MainFrame = parent;
  IsGeoPackage = false;
  IsFdoOgr = false;
  Root = AddRoot(wxT("no current DB"));
  SetItemData(Root, (wxTreeItemData *) (new MyObject(MY_ROOT_NODE)));
  RootUserData = AppendItem(Root, wxT("User Data"));
  SetItemData(RootUserData,
              (wxTreeItemData *) (new MyObject(MY_ROOT_USERDATA)));
  RootIsoMetadata = AppendItem(Root, wxT("ISO / INSPIRE Metadata"));
  SetItemData(RootIsoMetadata,
              (wxTreeItemData *) (new MyObject(MY_ROOT_ISOMETADATA)));
  RootStyling = AppendItem(Root, wxT("Styling (SLD/SE)"));
  SetItemData(RootStyling, (wxTreeItemData *) (new MyObject(MY_ROOT_STYLING)));
  RootTopologies = AppendItem(Root, wxT("Topology-Geometry"));
  SetItemData(RootTopologies,
              (wxTreeItemData *) (new MyObject(MY_ROOT_TOPOLOGIES)));
  RootNetworks = AppendItem(Root, wxT("Topology-Network"));
  SetItemData(RootNetworks,
              (wxTreeItemData *) (new MyObject(MY_ROOT_NETWORKS)));
  RootRasterCoverages = AppendItem(Root, wxT("Raster Coverages"));
  SetItemData(RootRasterCoverages,
              (wxTreeItemData *) (new MyObject(MY_ROOT_RASTER)));
  RootVectorCoverages = AppendItem(Root, wxT("Vector Coverages"));
  SetItemData(RootVectorCoverages,
              (wxTreeItemData *) (new MyObject(MY_ROOT_VECTOR)));
  RootWMS = AppendItem(Root, wxT("Registered WMS layers"));
  SetItemData(RootWMS, (wxTreeItemData *) (new MyObject(MY_ROOT_WMS)));
  RootPostgreSQL = AppendItem(Root, wxT("PostgreSQL"));
  SetItemData(RootPostgreSQL,
              (wxTreeItemData *) (new MyObject(MY_ROOT_POSTGRESQL)));
  RootMetadata = AppendItem(Root, wxT("Metadata"));
  SetItemData(RootMetadata,
              (wxTreeItemData *) (new MyObject(MY_ROOT_METADATA)));
  RootInternal = AppendItem(Root, wxT("Internal Data"));
  SetItemData(RootInternal,
              (wxTreeItemData *) (new MyObject(MY_ROOT_INTERNAL)));
  RootSpatialIndex = AppendItem(Root, wxT("Spatial Index"));
  SetItemData(RootSpatialIndex,
              (wxTreeItemData *) (new MyObject(MY_ROOT_RTREE)));
// setting up icons 
  Images = new wxImageList(16, 16, true);
  wxIcon icons[40];
  icons[0] = wxIcon(db_xpm);
  icons[1] = wxIcon(table_xpm);
  icons[2] = wxIcon(pkey_xpm);
  icons[3] = wxIcon(column_xpm);
  icons[4] = wxIcon(index_xpm);
  icons[5] = wxIcon(trigger_xpm);
  icons[6] = wxIcon(geometry_xpm);
  icons[7] = wxIcon(spatialidx_xpm);
  icons[8] = wxIcon(vtable_xpm);
  icons[9] = wxIcon(view_xpm);
  icons[10] = wxIcon(geotable_xpm);
  icons[11] = wxIcon(mbrcache_xpm);
  icons[12] = wxIcon(geoview_xpm);
  icons[13] = wxIcon(geovtable_xpm);
  icons[14] = wxIcon(tmp_table_xpm);
  icons[15] = wxIcon(tmp_view_xpm);
  icons[16] = wxIcon(malformed_geoms_xpm);
  icons[17] = wxIcon(tables_xpm);
  icons[18] = wxIcon(foreign_key_xpm);
  icons[19] = wxIcon(primary_key_xpm);
  icons[20] = wxIcon(topo_geo_xpm);
  icons[21] = wxIcon(attach_xpm);
  icons[22] = wxIcon(coverage_xpm);
  icons[23] = wxIcon(coverage_tiles_xpm);
  icons[24] = wxIcon(vector_xpm);
  icons[25] = wxIcon(topo_net_xpm);
  icons[26] = wxIcon(wms_xpm);
  icons[27] = wxIcon(points_on_xpm);
  icons[28] = wxIcon(linestrings_on_xpm);
  icons[29] = wxIcon(polygons_on_xpm);
  icons[30] = wxIcon(gpkgvtable_xpm);
  icons[31] = wxIcon(gpkgtable_xpm);
  icons[32] = wxIcon(gpkggeom_xpm);
  icons[33] = wxIcon(virtshp_xpm);
  icons[34] = wxIcon(virtgpkg_xpm);
  icons[35] = wxIcon(gpkg_spatialidx_xpm);
  icons[36] = wxIcon(postgres_xpm);
  icons[37] = wxIcon(postgres_schema_xpm);
  icons[38] = wxIcon(postgres_table_xpm);
  icons[39] = wxIcon(postgres_view_xpm);
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
  Images->Add(icons[17]);
  Images->Add(icons[18]);
  Images->Add(icons[19]);
  Images->Add(icons[20]);
  Images->Add(icons[21]);
  Images->Add(icons[22]);
  Images->Add(icons[23]);
  Images->Add(icons[24]);
  Images->Add(icons[25]);
  Images->Add(icons[26]);
  Images->Add(icons[27]);
  Images->Add(icons[28]);
  Images->Add(icons[29]);
  Images->Add(icons[30]);
  Images->Add(icons[31]);
  Images->Add(icons[32]);
  Images->Add(icons[33]);
  Images->Add(icons[34]);
  Images->Add(icons[35]);
  Images->Add(icons[36]);
  Images->Add(icons[37]);
  Images->Add(icons[38]);
  Images->Add(icons[39]);
  SetImageList(Images);
  SetItemImage(Root, 0);
  SetItemImage(RootUserData, 17);
  SetItemImage(RootTopologies, 20);
  SetItemImage(RootNetworks, 25);
  SetItemImage(RootRasterCoverages, 22);
  SetItemImage(RootVectorCoverages, 24);
  SetItemImage(RootWMS, 26);
  SetItemImage(RootPostgreSQL, 36);
  SetItemImage(RootStyling, 17);
  SetItemImage(RootIsoMetadata, 17);
  SetItemImage(RootMetadata, 17);
  SetItemImage(RootInternal, 17);
  SetItemImage(RootSpatialIndex, 17);
  wxString empty = wxEmptyString;

// setting up event handlers 
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_ITEM_COLLAPSED,
          (wxObjectEventFunction) & MyTableTree::OnItemCollapsed);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_ITEM_EXPANDING,
          (wxObjectEventFunction) & MyTableTree::OnItemExpanding);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_SEL_CHANGED,
          (wxObjectEventFunction) & MyTableTree::OnSelChanged);
  Connect(wxID_ANY, wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
          (wxObjectEventFunction) & MyTableTree::OnRightClick);
  Connect(Tree_QueryViewComposer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdQueryViewComposer);
  Connect(Tree_CloneTable, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCloneTable);
  Connect(Tree_NewTable, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewTable);
  Connect(Tree_NewView, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewView);
  Connect(Tree_NewIndex, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewIndex);
  Connect(Tree_NewTrigger, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewTrigger);
  Connect(Tree_NewColumn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewColumn);
  Connect(Tree_Show, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdShow);
  Connect(Tree_Drop, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDrop);
  Connect(Tree_Detach, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDetachDB);
  Connect(Tree_Rename, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRename);
  Connect(Tree_Select, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSelect);
  Connect(Tree_SelectTiles, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSelectTiles);
  Connect(Tree_Refresh, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRefresh);
  Connect(Tree_RefreshDeferred, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnRefreshDeferred);
  Connect(Tree_Recover, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRecover);
  Connect(Tree_ShowSql, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdShowSql);
  Connect(Tree_SpatialIndex, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSpatialIndex);
  Connect(Tree_CheckSpatialIndex, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCheckSpatialIndex);
  Connect(Tree_RecoverSpatialIndex, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRecoverSpatialIndex);
  Connect(Tree_MbrCache, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdMbrCache);
  Connect(Tree_RebuildTriggers, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRebuildTriggers);
  Connect(Tree_CheckGeometry, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCheckGeometry);
  Connect(Tree_Extent, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdExtent);
  Connect(Tree_UpdateLayerStatistics, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUpdateLayerStatistics);
  Connect(Tree_UpdateLayerStatisticsAll, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUpdateLayerStatisticsAll);
  Connect(Tree_ElementaryGeoms, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdElementaryGeometries);
  Connect(Tree_MalformedGeometries, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdMalformedGeometries);
  Connect(Tree_RepairPolygons, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRepairPolygons);
  Connect(Tree_SetSrid, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSetSrid);
  Connect(Tree_DumpShp, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpShp);
  Connect(Tree_DumpGeoJSON, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpGeoJSON);
  Connect(Tree_DumpKml, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpKml);
  Connect(Tree_DumpTxtTab, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpTxtTab);
  Connect(Tree_DumpCsv, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpCsv);
  Connect(Tree_DumpHtml, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpHtml);
  Connect(Tree_DumpDif, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpDif);
  Connect(Tree_DumpSylk, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpSylk);
  Connect(Tree_DumpDbf, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpDbf);
  Connect(Tree_DumpXlsx, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpXlsx);
  Connect(Tree_DumpPostGIS, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDumpPostGIS);
  Connect(Tree_Edit, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdEdit);
  Connect(Tree_DropColumn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDropColumn);
  Connect(Tree_RenameColumn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRenameColumn);
  Connect(Tree_ColumnStats, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdColumnStats);
  Connect(Tree_MapPreview, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdMapPreview);
  Connect(Tree_CreateVectorCoverage, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCreateVectorCoverage);
  Connect(Tree_CreateTopologyCoverage, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCreateTopologyCoverage);
  Connect(Tree_CreateNetworkCoverage, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCreateNetworkCoverage);
  Connect(Tree_CheckDuplicates, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCheckDuplicates);
  Connect(Tree_RemoveDuplicates, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRemoveDuplicates);
  Connect(Tree_CheckGeom, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCheckGeometries);
  Connect(Tree_SaneGeom, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSanitizeGeometries);
  Connect(Tree_NewMapConfig, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewMapConfig);
  Connect(Tree_ReloadMapConfig, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdReloadMapConfig);
  Connect(Tree_UnregisterMapConfig, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUnregisterMapConfig);
  Connect(Tree_VerifyExternalMapConfig, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdVerifyExternalMapConfig);
  Connect(Tree_VerifyRegisteredMapConfig, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdVerifyRegisteredMapConfig);
  Connect(Tree_NewRasterStyle, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewRasterStyle);
  Connect(Tree_ReloadRasterStyle, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdReloadRasterStyle);
  Connect(Tree_UnregisterRasterStyle, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUnregisterRasterStyle);
  Connect(Tree_RasterSymbolizerContrast, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRasterSymbolizerContrast);
  Connect(Tree_RasterSymbolizerChannelRgb, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdRasterSymbolizerChannelRgb);
  Connect(Tree_RasterSymbolizerChannelGray, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdRasterSymbolizerChannelGray);
  Connect(Tree_RasterSymbolizerCategorize, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdRasterSymbolizerCategorize);
  Connect(Tree_RasterSymbolizerInterpolate, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdRasterSymbolizerInterpolate);
  Connect(Tree_RasterSymbolizerShadedRelief, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdRasterSymbolizerShadedRelief);
  Connect(Tree_RasterSymbolizerMonochrome, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdRasterSymbolizerMonochrome);
  Connect(Tree_RegisterExternalGraphic, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRegisterExternalGraphic);
  Connect(Tree_UnregisterExternalGraphic, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnCmdUnregisterExternalGraphic);
  Connect(Tree_RegisterTextFont, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRegisterTextFont);
  Connect(Tree_UnregisterTextFont, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUnregisterTextFont);
  Connect(Tree_NewVectorStyle, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdNewVectorStyle);
  Connect(Tree_ReloadVectorStyle, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdReloadVectorStyle);
  Connect(Tree_UnregisterVectorStyle, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUnregisterVectorStyle);
  Connect(Tree_SimpleLineSymbolizer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSimpleLineSymbolizer);
  Connect(Tree_SimplePolygonSymbolizer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSimplePolygonSymbolizer);
  Connect(Tree_SimplePointSymbolizer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSimplePointSymbolizer);
  Connect(Tree_SimpleTextSymbolizer, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSimpleTextSymbolizer);
  Connect(Tree_ImportRaster, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdImportRaster);
  Connect(Tree_Pyramidize, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdPyramidize);
  Connect(Tree_PyramidizeMonolithic, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdPyramidizeMonolithic);
  Connect(Tree_DePyramidize, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDePyramidize);
  Connect(Tree_RasterDrop, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRasterDrop);
  Connect(Tree_AddAllRastersSrid, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdAddAllRastersSrid);
  Connect(Tree_UpdateRasterExtent, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUpdateRasterExtent);
  Connect(Tree_RasterInfos, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRasterInfos);
  Connect(Tree_Raster_SRIDs, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRasterSRIDs);
  Connect(Tree_Raster_Keywords, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRasterKeywords);
  Connect(Tree_UpdateRasterExtentAll, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUpdateRasterExtentAll);
  Connect(Tree_VectorUnregister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdVectorUnregister);
  Connect(Tree_VectorInfos, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdVectorInfos);
  Connect(Tree_UpdateVectorExtent, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUpdateVectorExtent);
  Connect(Tree_Vector_SRIDs, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdVectorSRIDs);
  Connect(Tree_Vector_Keywords, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdVectorKeywords);
  Connect(Tree_AddAllVectorsSrid, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdAddAllVectorsSrid);
  Connect(Tree_UpdateVectorExtentAll, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdUpdateVectorExtentAll);
  Connect(Tree_SldSeRasterStyles, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSldSeRasterStyles);
  Connect(Tree_SldSeVectorStyles, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdSldSeVectorStyles);
  Connect(Tree_CreateRasterCoverage, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCreateRasterCoverage);
  Connect(Tree_VectorRegister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnRegisterVectorCoverage);
  Connect(Tree_SpatialViewRegister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnRegisterSpatialViewCoverage);
  Connect(Tree_VirtualTableRegister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          MyTableTree::OnRegisterVirtualTableCoverage);
  Connect(Tree_TopoGeoRegister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnRegisterTopoGeoCoverage);
  Connect(Tree_TopoNetRegister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnRegisterTopoNetCoverage);
  Connect(Tree_WmsLayerRegister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdRegisterWmsLayer);
  Connect(Tree_WmsLayerUnregister, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdWmsLayerUnregister);
  Connect(Tree_WmsLayerInfos, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdWmsLayerInfos);
  Connect(Tree_WmsLayerConfigure, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdWmsLayerConfigure);
  Connect(Tree_CreateTopoGeo, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCreateTopoGeo);
  Connect(Tree_DropTopoGeo, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDropTopoGeo);
  Connect(Tree_CreateTopoNet, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCreateTopoNet);
  Connect(Tree_DropTopoNet, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdDropTopoNet);
  Connect(Tree_CreatePostgreSqlConn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCreatePostgreSqlConn);
  Connect(Tree_CloseAllPostgreSqlConns, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdCloseAllPostgreSqlConns);
  Connect(Tree_ClosePostgreSqlConn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdClosePostgreSqlConn);
  Connect(Tree_PostgreSqlInfos, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdPostgreSqlInfos);
  Connect(Tree_PostgreSqlDropOrphans, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyTableTree::OnCmdPostgreSqlDropOrphans);
}

MyTableTree::~MyTableTree()
{
  delete Images;
}

void MyTableTree::FlushAll()
{
// resetting to the initial empty state
  wxTreeItemId child;
  wxTreeItemIdValue cookie;
  bool ok = true;
  DeleteChildren(RootUserData);
  DeleteTopologies(RootTopologies);
  DeleteNetworks(RootNetworks);
  DeleteRasterCoverages(RootRasterCoverages);
  DeleteVectorCoverages(RootVectorCoverages);
  DeleteChildren(RootStyling);
  DeleteChildren(RootWMS);
  DeleteChildren(RootPostgreSQL);
  DeleteChildren(RootIsoMetadata);
  DeleteChildren(RootMetadata);
  DeleteChildren(RootInternal);
  DeleteChildren(RootSpatialIndex);
  while (ok)
    {
      ok = false;
      child = GetFirstChild(Root, cookie);
      while (true)
        {
          bool kill = true;
          if (child.IsOk() == false)
            break;
          if (child == RootUserData)
            kill = false;
          if (child == RootTopologies)
            kill = false;
          if (child == RootNetworks)
            kill = false;
          if (child == RootRasterCoverages)
            kill = false;
          if (child == RootVectorCoverages)
            kill = false;
          if (child == RootStyling)
            kill = false;
          if (child == RootWMS)
            kill = false;
          if (child == RootPostgreSQL)
            kill = false;
          if (child == RootIsoMetadata)
            kill = false;
          if (child == RootMetadata)
            kill = false;
          if (child == RootInternal)
            kill = false;
          if (child == RootSpatialIndex)
            kill = false;
          if (kill == true)
            {
              Delete(child);
              ok = true;
              break;
            }
          child = GetNextChild(Root, cookie);
        }
    }
  CurrentItem = wxTreeItemId();
}

void MyTableTree::OnItemCollapsed(wxTreeEvent & event)
{
// a TreeItem was collapsed - freeing all children
  wxTreeItemId item = event.GetItem();
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  switch (obj->GetType())
    {
      case MY_TABLE:
      case MY_VTABLE:
      case MY_TILE_DATA:
      case MY_VIEW:
      case MY_POSTGRES_TABLE:
      case MY_POSTGRES_VIEW:
      case MY_POSTGIS_VIEW:
        DeleteChildren(item);
        AppendItem(item, wxT("placeholder"), 0);
        break;
    };
  event.Skip();
}

void MyTableTree::OnItemExpanding(wxTreeEvent & event)
{
// a TreeItem is to be expanded - appending all children
  wxTreeItemId item = event.GetItem();
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  switch (obj->GetType())
    {
      case MY_TABLE:
      case MY_VTABLE:
      case MY_GPKG_TABLE:
      case MY_GPKG_VTABLE:
      case MY_FDO_TABLE:
      case MY_FDO_VTABLE:
      case MY_TILE_DATA:
        DeleteChildren(item);
        if (obj->GetDbAlias().Len() == 0)
          ExpandTable(item);
        else
          ExpandAttachedTable(item);
        EnsureVisible(item);
        return;
      case MY_VIEW:
        DeleteChildren(item);
        if (obj->GetDbAlias().Len() == 0)
          ExpandView(item);
        else
          ExpandAttachedView(item);
        EnsureVisible(item);
        return;
      case MY_POSTGRES_TABLE:
      case MY_POSTGRES_VIEW:
        DeleteChildren(item);
        ExpandPostgresTable(item);
        EnsureVisible(item);
        return;
      case MY_POSTGIS_VIEW:
        DeleteChildren(item);
        ExpandPostGisView(item);
        EnsureVisible(item);
        return;
    };
  event.Skip();
}

void MyTableTree::DeleteTopologies(wxTreeItemId & RootTopologies)
{
// deleting Topology Nodes
  Topologies.Flush();
  DeleteChildren(RootTopologies);
}

void MyTableTree::DeleteNetworks(wxTreeItemId & RootNetworks)
{
// deleting Network Nodes
  Networks.Flush();
  DeleteChildren(RootNetworks);
}

void MyTableTree::DeleteRasterCoverages(wxTreeItemId & RootRasterCoverages)
{
// deleting Raster Coverage Nodes
  RasterCoverages.Flush();
  DeleteChildren(RootRasterCoverages);
}

void MyTableTree::DeleteVectorCoverages(wxTreeItemId & RootVectorCoverages)
{
// deleting Vector Coverage Nodes
  VectorCoverages.Flush();
  DeleteChildren(RootVectorCoverages);
}

wxTreeItemId & MyTableTree::GetRootNode(wxString & tableName, bool *restricted,
                                        bool *topoObject, bool *tile_data)
{
//
// determines the Table Root Node 
//
  bool isTileData = false;
  if (topoObject != NULL)
    *topoObject = false;
  if (tile_data != NULL)
    *tile_data = false;
  if (restricted != NULL)
    *restricted = false;
  if (tableName == wxT("geometry_columns"))
    return RootMetadata;
  if (tableName == wxT("views_geometry_columns"))
    return RootMetadata;
  if (tableName == wxT("virts_geometry_columns"))
    return RootMetadata;
  if (tableName == wxT("spatial_ref_sys"))
    return RootMetadata;
  if (tableName == wxT("spatial_ref_sys_aux"))
    return RootMetadata;
  if (tableName == wxT("spatial_ref_sys_area"))
    return RootMetadata;
  if (tableName == wxT("spatial_ref_sys_all"))
    return RootMetadata;
  if (tableName == wxT("geom_cols_ref_sys"))
    return RootMetadata;
  if (tableName == wxT("geometry_columns_time"))
    return RootMetadata;
  if (tableName == wxT("spatialite_history"))
    return RootMetadata;
  if (tableName == wxT("raster_coverages"))
    return RootMetadata;
  if (tableName == wxT("raster_coverages_srid"))
    return RootMetadata;
  if (tableName == wxT("raster_coverages_keyword"))
    return RootMetadata;
  if (tableName == wxT("raster_coverages_ref_sys"))
    return RootMetadata;
  if (tableName == wxT("vector_coverages"))
    return RootMetadata;
  if (tableName == wxT("vector_coverages_srid"))
    return RootMetadata;
  if (tableName == wxT("vector_coverages_keyword"))
    return RootMetadata;
  if (tableName == wxT("vector_coverages_ref_sys"))
    return RootMetadata;
  if (tableName == wxT("vector_layers"))
    return RootMetadata;
  if (tableName == wxT("topologies"))
    return RootMetadata;
  if (tableName == wxT("networks"))
    return RootMetadata;
  if (tableName == wxT("data_licenses"))
    return RootMetadata;
  if (tableName == wxT("stored_procedures"))
    return RootMetadata;
  if (tableName == wxT("stored_variables"))
    return RootMetadata;

  if (tableName == wxT("wms_getcapabilities"))
    return RootMetadata;
  if (tableName == wxT("wms_getmap"))
    return RootMetadata;
  if (tableName == wxT("wms_settings"))
    return RootMetadata;
  if (tableName == wxT("wms_ref_sys"))
    return RootMetadata;

  if (tableName == wxT("gpkg_spatial_ref_sys"))
    return RootMetadata;
  if (tableName == wxT("gpkg_contents"))
    return RootMetadata;
  if (tableName == wxT("gpkg_geometry_columns"))
    return RootMetadata;
  if (tableName == wxT("gpkg_tile_matrix_set"))
    return RootMetadata;
  if (tableName == wxT("gpkg_tile_matrix"))
    return RootMetadata;
  if (tableName == wxT("gpkg_data_columns"))
    return RootMetadata;
  if (tableName == wxT("gpkg_data_column_constraints"))
    return RootMetadata;
  if (tableName == wxT("gpkg_metadata"))
    return RootMetadata;
  if (tableName == wxT("gpkg_metadata_reference"))
    return RootMetadata;
  if (tableName == wxT("gpkg_extensions"))
    return RootMetadata;

  if (tableName == wxT("sqlite_stat1"))
    return RootInternal;
  if (tableName == wxT("sqlite_stat3"))
    return RootInternal;
  if (tableName == wxT("sqlite_sequence"))
    return RootInternal;
  if (tableName == wxT("layer_params"))
    return RootInternal;
  if (tableName == wxT("layer_statistics"))
    return RootInternal;
  if (tableName == wxT("geometry_columns_statistics"))
    return RootInternal;
  if (tableName == wxT("views_layer_statistics"))
    return RootInternal;
  if (tableName == wxT("views_geometry_columns_statistics"))
    return RootInternal;
  if (tableName == wxT("virts_layer_statistics"))
    return RootInternal;
  if (tableName == wxT("virts_geometry_columns_statistics"))
    return RootInternal;
  if (tableName == wxT("geometry_columns_field_infos"))
    return RootInternal;
  if (tableName == wxT("views_geometry_columns_field_infos"))
    return RootInternal;
  if (tableName == wxT("virts_geometry_columns_field_infos"))
    return RootInternal;
  if (tableName == wxT("geometry_columns_auth"))
    return RootInternal;
  if (tableName == wxT("views_geometry_columns_auth"))
    return RootInternal;
  if (tableName == wxT("virts_geometry_columns_auth"))
    return RootInternal;
  if (tableName == wxT("vector_layers_auth"))
    return RootInternal;
  if (tableName == wxT("vector_layers_statistics"))
    return RootInternal;
  if (tableName == wxT("vector_layers_field_infos"))
    return RootInternal;
  if (tableName == wxT("layer_sub_classes"))
    return RootInternal;
  if (tableName == wxT("layer_table_layout"))
    return RootInternal;
  if (tableName == wxT("pattern_bitmaps"))
    return RootInternal;
  if (tableName == wxT("symbol_bitmaps"))
    return RootInternal;
  if (tableName == wxT("project_defs"))
    return RootInternal;
  if (tableName == wxT("raster_pyramids"))
    return RootInternal;
  if (tableName == wxT("rasterlite2_styles"))
    return RootInternal;
  if (tableName == wxT("rasterlite2_metadata"))
    return RootInternal;
  if (tableName == wxT("sql_statements_log"))
    return RootInternal;
  if (tableName == wxT("wms_server_log"))
    return RootInternal;
  if (tableName == wxT("ElementaryGeometries"))
    return RootInternal;

  if (tableName == wxT("SE_external_graphics"))
    return RootStyling;
  if (tableName == wxT("SE_fonts"))
    return RootStyling;
  if (tableName == wxT("SE_external_graphics_view"))
    return RootStyling;
  if (tableName == wxT("SE_fonts_view"))
    return RootStyling;
  if (tableName == wxT("SE_raster_styles"))
    return RootStyling;
  if (tableName == wxT("rl2map_configurations"))
    return RootStyling;
  if (tableName == wxT("SE_raster_styled_layers"))
    return RootStyling;
  if (tableName == wxT("SE_raster_styles_view"))
    return RootStyling;
  if (tableName == wxT("SE_raster_styled_layers_view"))
    return RootStyling;
  if (tableName == wxT("SE_vector_styles"))
    return RootStyling;
  if (tableName == wxT("SE_vector_styled_layers"))
    return RootStyling;
  if (tableName == wxT("SE_vector_styles_view"))
    return RootStyling;
  if (tableName == wxT("SE_vector_styled_layers_view"))
    return RootStyling;
  if (tableName == wxT("rl2map_configurations_view"))
    return RootStyling;

  if (tableName == wxT("ISO_metadata"))
    return RootIsoMetadata;
  if (tableName == wxT("ISO_metadata_reference"))
    return RootIsoMetadata;
  if (tableName == wxT("ISO_metadata_view"))
    return RootIsoMetadata;

  if (tableName == wxT("SpatialIndex"))
    return RootSpatialIndex;
  if (tableName == wxT("KNN"))
    return RootSpatialIndex;
  if (MainFrame->IsSpatialIndex(tableName) == true)
    return RootSpatialIndex;
  if (IsGeoPackage)
    {
      if (MainFrame->IsGeoPackageSpatialIndex(tableName) == true)
        return RootSpatialIndex;
    }
  if (MainFrame->IsTopoFaceSpatialIndex(tableName) == true)
    return RootSpatialIndex;
  wxTreeItemId *topologyNode = Topologies.FindNode(tableName);
  if (topologyNode != NULL)
    {
      if (restricted != NULL)
        *restricted = true;
      if (topoObject != NULL)
        *topoObject = true;
      return *topologyNode;
    }
  wxTreeItemId *networkNode = Networks.FindNode(tableName);
  if (networkNode != NULL)
    {
      if (restricted != NULL)
        *restricted = true;
      if (topoObject != NULL)
        *topoObject = true;
      return *networkNode;
    }
  wxTreeItemId *coverageNode = RasterCoverages.FindNode(tableName, &isTileData);
  if (coverageNode != NULL)
    {
      if (restricted != NULL)
        *restricted = true;
      if (tile_data != NULL)
        *tile_data = isTileData;
      return *coverageNode;
    }
  return RootUserData;
}

wxTreeItemId & MyTableTree::GetAltRootNode(wxString & tableName,
                                           RootNodes * nodes, bool *topoObject,
                                           bool *tile_data)
{
//
// determines the Table Root Node [Attached DB]
//
  bool isTileData = false;
  if (topoObject != NULL)
    *topoObject = false;
  if (tile_data != NULL)
    *tile_data = false;
  if (tableName == wxT("geometry_columns"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("views_geometry_columns"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("virts_geometry_columns"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("spatial_ref_sys"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("spatial_ref_sys_aux"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("spatial_ref_sys_area"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("spatial_ref_sys_all"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("geom_cols_ref_sys"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("geometry_columns_time"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("spatialite_history"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("raster_coverages"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("raster_coverages_srid"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("raster_coverages_keyword"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("raster_coverages_ref_sys"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("vector_coverages"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("vector_coverages_srid"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("vector_coverages_keyword"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("vector_coverages_ref_sys"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("vector_layers"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("vector_layers"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("topologies"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("networks"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("data_licenses"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("stored_procedures"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("stored_variables"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("rl2map_configurations"))
    return nodes->GetRootMetadata();

  if (tableName == wxT("gpkg_spatial_ref_sys"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_contents"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_geometry_columns"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_tile_matrix_set"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_tile_matrix"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_data_columns"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_data_column_constraints"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_metadata"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_metadata_reference"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("gpkg_extensions"))
    return nodes->GetRootMetadata();

  if (tableName == wxT("wms_getcapabilities"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("wms_getmap"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("wms_settings"))
    return nodes->GetRootMetadata();
  if (tableName == wxT("wms_ref_sys"))
    return nodes->GetRootMetadata();

  if (tableName == wxT("sqlite_stat1"))
    return nodes->GetRootInternal();
  if (tableName == wxT("sqlite_stat3"))
    return nodes->GetRootInternal();
  if (tableName == wxT("sqlite_sequence"))
    return nodes->GetRootInternal();
  if (tableName == wxT("layer_params"))
    return nodes->GetRootInternal();
  if (tableName == wxT("layer_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("geometry_columns_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("views_layer_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("views_geometry_columns_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("virts_layer_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("virts_geometry_columns_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("geometry_columns_field_infos"))
    return nodes->GetRootInternal();
  if (tableName == wxT("views_geometry_columns_field_infos"))
    return nodes->GetRootInternal();
  if (tableName == wxT("virts_geometry_columns_field_infos"))
    return nodes->GetRootInternal();
  if (tableName == wxT("geometry_columns_auth"))
    return nodes->GetRootInternal();
  if (tableName == wxT("views_geometry_columns_auth"))
    return nodes->GetRootInternal();
  if (tableName == wxT("virts_geometry_columns_auth"))
    return nodes->GetRootInternal();
  if (tableName == wxT("vector_layers_auth"))
    return nodes->GetRootInternal();
  if (tableName == wxT("vector_layers_statistics"))
    return nodes->GetRootInternal();
  if (tableName == wxT("vector_layers_field_infos"))
    return nodes->GetRootInternal();
  if (tableName == wxT("layer_sub_classes"))
    return nodes->GetRootInternal();
  if (tableName == wxT("layer_table_layout"))
    return nodes->GetRootInternal();
  if (tableName == wxT("pattern_bitmaps"))
    return nodes->GetRootInternal();
  if (tableName == wxT("symbol_bitmaps"))
    return nodes->GetRootInternal();
  if (tableName == wxT("project_defs"))
    return nodes->GetRootInternal();
  if (tableName == wxT("raster_pyramids"))
    return nodes->GetRootInternal();
  if (tableName == wxT("rasterlite2_styles"))
    return nodes->GetRootInternal();
  if (tableName == wxT("rasterlite2_metadata"))
    return nodes->GetRootInternal();
  if (tableName == wxT("sql_statements_log"))
    return nodes->GetRootInternal();
  if (tableName == wxT("wms_server_log"))
    return nodes->GetRootInternal();
  if (tableName == wxT("ElementaryGeometries"))
    return nodes->GetRootInternal();

  if (tableName == wxT("SE_external_graphics"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_fonts"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_external_graphics_view"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_fonts_view"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_raster_styles"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_raster_styled_layers"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_raster_styles_view"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_raster_styled_layers_view"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_vector_styles"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_vector_styled_layers"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_vector_styles_view"))
    return nodes->GetRootStyling();
  if (tableName == wxT("SE_vector_styled_layers_view"))
    return nodes->GetRootStyling();
  if (tableName == wxT("rl2map_configurations_views"))
    return nodes->GetRootStyling();

  if (tableName == wxT("ISO_metadata"))
    return nodes->GetRootIsoMetadata();
  if (tableName == wxT("ISO_metadata_reference"))
    return nodes->GetRootIsoMetadata();
  if (tableName == wxT("ISO_metadata_view"))
    return nodes->GetRootIsoMetadata();

  if (tableName == wxT("SpatialIndex"))
    return nodes->GetRootSpatialIndex();
  if (tableName == wxT("KNN"))
    return nodes->GetRootSpatialIndex();
  if (MainFrame->IsSpatialIndex(nodes->GetDbAlias(), tableName) == true)
    return nodes->GetRootSpatialIndex();
  if (MainFrame->IsGeoPackageSpatialIndex(nodes->GetDbAlias(), tableName) ==
      true)
    return nodes->GetRootSpatialIndex();
  if (MainFrame->IsTopoFaceSpatialIndex(nodes->GetDbAlias(), tableName) == true)
    return nodes->GetRootSpatialIndex();
  wxTreeItemId *topologyNode = AltTopologies.FindNode(tableName);
  if (topologyNode != NULL)
    {
      if (topoObject != NULL)
        *topoObject = true;
      return *topologyNode;
    }
  wxTreeItemId *networkNode = AltNetworks.FindNode(tableName);
  if (networkNode != NULL)
    {
      if (topoObject != NULL)
        *topoObject = true;
      return *networkNode;
    }
  wxTreeItemId *coverageNode =
    AltRasterCoverages.FindNode(tableName, &isTileData);
  if (coverageNode != NULL)
    {
      if (tile_data != NULL)
        *tile_data = isTileData;
      return *coverageNode;
    }
  return nodes->GetRootUserData();
}

void MyTableTree::AddTable(wxString & tableName, bool virtualTable,
                           bool geometry)
{
//
// appends a table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  bool isTopoObject = false;
  bool isTileData = false;
  int icon = 1;
  if (virtualTable == true)
    icon = 8;
  if (geometry == true)
    {
      if (virtualTable == false)
        icon = 10;
      else
        icon = 13;
    }
  bool restricted = false;
  bool topoObj = false;
  wxTreeItemId rootNode =
    GetRootNode(tableName, &restricted, &isTopoObject, &isTileData);
  if (rootNode == RootStyling || rootNode == RootIsoMetadata
      || rootNode == RootMetadata || rootNode == RootInternal
      || rootNode == RootSpatialIndex)
    restricted = true;
  if (isTopoObject == true)
    topoObj = true;
  if (isTileData == true)
    icon = 23;
  wxString empty = wxEmptyString;
  if (virtualTable == true)
    data = (wxTreeItemData *) (new MyObject(MY_VTABLE, empty, tableName));
  else if (isTileData == true)
    data = (wxTreeItemData *) (new MyObject(MY_TILE_DATA, empty, tableName));
  else
    data =
      (wxTreeItemData *) (new
                          MyObject(MY_TABLE, empty, tableName, restricted,
                                   topoObj));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddTable(wxString & dbAlias, wxString & tableName,
                           bool virtualTable, bool geometry)
{
//
// appends a table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  bool isTopoObject = false;
  bool isTileData = false;
  int icon = 1;
  if (virtualTable == true)
    icon = 8;
  if (geometry == true)
    {
      if (virtualTable == false)
        icon = 10;
      else
        icon = 13;
    }
  wxTreeItemId rootNode =
    GetRootNode(tableName, NULL, &isTopoObject, &isTileData);
  if (isTileData == true)
    icon = 23;
  wxString empty = wxEmptyString;
  if (virtualTable == true)
    data = (wxTreeItemData *) (new MyObject(MY_VTABLE, dbAlias, tableName));
  else if (isTileData == true)
    data = (wxTreeItemData *) (new MyObject(MY_TILE_DATA, dbAlias, tableName));
  else
    data =
      (wxTreeItemData *) (new
                          MyObject(MY_TABLE, dbAlias, tableName, isTopoObject));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddTmpMetadata(wxString & name)
{
// adding some TEMPORARY Metadata table
  wxTreeItemId item;
  wxString dbAlias = wxT("temp");
  wxTreeItemData *data =
    (wxTreeItemData *) (new MyObject(MY_TABLE, dbAlias, name, true));
  item = AppendItem(RootMetadata, name, 1, -1, data);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddGeoPackageTable(wxString & tableName)
{
//
// appends a GPKG table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 31;
  wxTreeItemId rootNode = RootUserData;
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_GPKG_TABLE, empty, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(168, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddGeoPackageTable(wxString & dbAlias, wxString & tableName,
                                     RootNodes * nodes)
{
//
// appends a GPKG table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 31;
  bool isTopoObject = false;
  bool isTileData = false;
  wxTreeItemId rootNode =
    GetAltRootNode(tableName, nodes, &isTopoObject, &isTileData);
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_GPKG_TABLE, dbAlias, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(168, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddGeoPackageVirtualTable(wxString & tableName)
{
//
// appends a GPKG Virtual table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 30;
  wxTreeItemId rootNode = RootUserData;
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_GPKG_VTABLE, empty, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(128, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddGeoPackageVirtualTable(wxString & dbAlias,
                                            wxString & tableName,
                                            RootNodes * nodes)
{
//
// appends a GPKG Virtual table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 30;
  bool isTopoObject = false;
  bool isTileData = false;
  wxTreeItemId rootNode =
    GetAltRootNode(tableName, nodes, &isTopoObject, &isTileData);
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new
                        MyObject(MY_GPKG_VTABLE, dbAlias, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(128, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddFdoOgrTable(wxString & tableName)
{
//
// appends an FDO/OGR table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 31;
  wxTreeItemId rootNode = RootUserData;
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_FDO_TABLE, empty, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(168, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddFdoOgrTable(wxString & dbAlias, wxString & tableName,
                                 RootNodes * nodes)
{
//
// appends an FDO/OGR table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 31;
  bool isTopoObject = false;
  bool isTileData = false;
  wxTreeItemId rootNode =
    GetAltRootNode(tableName, nodes, &isTopoObject, &isTileData);
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_FDO_TABLE, dbAlias, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(168, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddFdoOgrVirtualTable(wxString & tableName)
{
//
// appends an FDO/OGR Virtual table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 30;
  wxTreeItemId rootNode = RootUserData;
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_FDO_VTABLE, empty, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(128, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddFdoOgrVirtualTable(wxString & dbAlias,
                                        wxString & tableName, RootNodes * nodes)
{
//
// appends an FDO/OGR Virtual table to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 30;
  bool isTopoObject = false;
  bool isTileData = false;
  wxTreeItemId rootNode =
    GetAltRootNode(tableName, nodes, &isTopoObject, &isTileData);
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new MyObject(MY_FDO_VTABLE, dbAlias, tableName, false));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  SetItemTextColour(item, wxColour(128, 64, 64));
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::ExpandTable(wxTreeItemId & item)
{
//
// expands a TreeItem of the TABLE type
//
  MyTableInfo list;
  MyColumnInfo *col;
  MyIndexInfo *idx;
  MyTriggerInfo *trgr;
  wxTreeItemId item2;
  wxTreeItemId item3;
  wxString columnInfo;
  wxString indexInfo;
  wxString triggerInfo;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  wxString tableName = obj->GetMainName();
  bool topo_obj = obj->IsTopoObject();
  bool virtualTable = false;
  if (obj->GetType() == MY_TABLE && obj->IsTopoObject() == true)
    topo_obj = true;
  if (obj->GetType() == MY_VTABLE)
    virtualTable = true;
  bool gpkgTable = false;
  if (obj->GetType() == MY_GPKG_TABLE)
    gpkgTable = true;
  bool gpkgVirtualTable = false;
  if (obj->GetType() == MY_GPKG_VTABLE)
    gpkgVirtualTable = true;
  bool gpkg_geom = false;
  bool fdoOgrTable = false;
  if (obj->GetType() == MY_FDO_TABLE)
    fdoOgrTable = true;
  bool fdoOgrVirtualTable = false;
  if (obj->GetType() == MY_FDO_VTABLE)
    fdoOgrVirtualTable = true;
  bool fdoOgr_geom = false;

  MainFrame->GetTableColumns(tableName, &list);
  MainFrame->GetTableIndices(tableName, &list);
  MainFrame->GetTableTriggers(tableName, &list);
  if (gpkgTable == true || gpkgVirtualTable == true)
    MainFrame->CheckGPKG(tableName, &list);
  if (fdoOgrTable == true || fdoOgrVirtualTable == true)
    MainFrame->CheckFdoOgr(tableName, &list);
  wxString empty = wxEmptyString;
  col = list.GetFirstColumn();
  while (col)
    {
      int icon;
      if (col->IsPrimaryKey() == true)
        icon = 2;
      else
        {
          if (gpkgTable == true)
            {
              if (col->IsGPKGGeometryIndex() == true)
                {
                  gpkg_geom = true;
                  icon = 35;
              } else if (col->IsGPKGGeometry() == true)
                {
                  gpkg_geom = true;
                  icon = 32;
              } else
                icon = 3;
          } else if (gpkgVirtualTable == true
                     && col->IsGPKGVirtualGeometry() == true)
            {
              gpkg_geom = true;
              icon = 34;
          } else if (fdoOgrTable == true)
            {
              if (col->IsFdoOgrGeometry() == true)
                {
                  fdoOgr_geom = true;
                  icon = 32;
              } else
                icon = 3;
          } else if (fdoOgrVirtualTable == true
                     && col->IsFdoOgrVirtualGeometry() == true)
            {
              fdoOgr_geom = true;
              icon = 34;
          } else if (virtualTable == true && col->IsGeometry() == true)
            icon = 33;
          else if (col->IsGeometry() == true)
            {
              if (col->IsGeometryIndex() == true)
                icon = 7;
              else if (col->IsMbrCache() == true)
                icon = 11;
              else
                icon = 6;
          } else
            icon = 3;
        }
      columnInfo = col->GetName();
      item2 = AppendItem(item, columnInfo, icon);
      if (gpkg_geom == true || fdoOgr_geom == true)
        SetItemTextColour(item2, wxColour(168, 64, 64));
      if (virtualTable == true)
        {
          if (col->IsGeometry() == true)
            {
              SetItemData(item2,
                          (wxTreeItemData *) (new
                                              MyObject(MY_VIRTUAL_GEOMETRY,
                                                       empty, tableName,
                                                       col->GetName())));
          } else
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIRTUAL_COLUMN, empty,
                                                     tableName,
                                                     col->GetName())));
      } else
        {
          if (col->IsGeometry() == true)
            {
              if (col->IsGeometryIndex() == true)
                SetItemData(item2,
                            (wxTreeItemData *) (new
                                                MyObject(MY_GEOMETRY_INDEX,
                                                         empty, tableName,
                                                         topo_obj,
                                                         col->GetName())));
              else if (col->IsMbrCache() == true)
                SetItemData(item2,
                            (wxTreeItemData *) (new
                                                MyObject(MY_GEOMETRY_CACHED,
                                                         empty, tableName,
                                                         topo_obj,
                                                         col->GetName())));
              else
                SetItemData(item2,
                            (wxTreeItemData *) (new
                                                MyObject(MY_GEOMETRY, empty,
                                                         tableName, topo_obj,
                                                         col->GetName())));
          } else if (col->IsGPKGGeometryIndex() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_GPKG_GEOMETRY_INDEX,
                                                     empty, tableName,
                                                     col->GetName())));
          else if (col->IsGPKGGeometry() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_GPKG_GEOMETRY,
                                                     empty, tableName,
                                                     col->GetName())));
          else if (col->IsGPKGVirtualGeometry() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIRTUAL_GPKG_GEOMETRY,
                                                     empty, tableName,
                                                     col->GetName())));
          else if (col->IsFdoOgrGeometry() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_FDO_GEOMETRY,
                                                     empty, tableName,
                                                     col->GetName())));
          else if (col->IsFdoOgrVirtualGeometry() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIRTUAL_FDO_GEOMETRY,
                                                     empty, tableName,
                                                     col->GetName())));
          else
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_COLUMN, empty,
                                                     tableName,
                                                     col->GetName())));
        }
      col = col->GetNext();
    }
// setting up Primary Key
  bool ok_pk = false;
  idx = list.GetFirstIndex();
  while (idx)
    {
      indexInfo = idx->GetName();
      if (idx->ContainsOnlyPrimaryKeyColumns(MainFrame->GetSqlite(), indexInfo,
                                             list.GetFirstColumn()) == true)
        {
          if (indexInfo.StartsWith(wxT("sqlite_autoindex_")))
            {
              item2 = AppendItem(item, wxT("PrimaryKey"), 19);
              SetItemData(item2,
                          (wxTreeItemData *) (new
                                              MyObject(MY_PRIMARY_KEY, empty,
                                                       tableName)));
              MainFrame->GetPrimaryKeyFields(indexInfo, tableName, item2);
              ok_pk = true;
              break;
            }
        }
      idx = idx->GetNext();
    }
  if (ok_pk == false)
    {
      col = list.GetFirstColumn();
      while (col)
        {
          if (col->IsPrimaryKey() == true)
            {
              ok_pk = true;
              break;
            }
          col = col->GetNext();
        }
      if (ok_pk == true)
        {
          item2 = AppendItem(item, wxT("PrimaryKey"), 19);
          SetItemData(item2,
                      (wxTreeItemData *) (new
                                          MyObject(MY_PRIMARY_KEY, empty,
                                                   tableName)));
          col = list.GetFirstColumn();
          while (col)
            {
              if (col->IsPrimaryKey() == true)
                {
                  item3 = AppendItem(item2, col->GetName(), 2);
                  SetItemData(item3,
                              (wxTreeItemData *) (new
                                                  MyObject(MY_PRIMARY_KEY_FLD,
                                                           empty, tableName,
                                                           col->GetName())));
                  break;
                }
              col = col->GetNext();
            }
        }
    }
// setting up Foreign Keys
  MainFrame->GetForeignKeys(tableName, item);
  idx = list.GetFirstIndex();
  while (idx)
    {
      // setting up Indices
      indexInfo = idx->GetName();
      if (indexInfo.StartsWith(wxT("sqlite_autoindex_")) == true)
        {
          idx = idx->GetNext();
          continue;
        }
      item2 = AppendItem(item, indexInfo, 4);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_INDEX, empty,
                                               idx->GetName())));
      MainFrame->GetIndexFields(indexInfo, tableName, item2);
      idx = idx->GetNext();
    }
  trgr = list.GetFirstTrigger();
  while (trgr)
    {
      // setting up Triggers
      triggerInfo = trgr->GetName();
      item2 = AppendItem(item, triggerInfo, 5);
      wxString db = empty;
      if (obj->IsAttached() == true)
        db = obj->GetDbAlias();
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_TRIGGER, db,
                                               trgr->GetName())));
      trgr = trgr->GetNext();
    }
}

void MyTableTree::ExpandPostgresTable(wxTreeItemId & item)
{
//
// expands a TreeItem of the POSTGRES TABLE type
//
  MyTableInfo list;
  MyColumnInfo *col;
  wxString columnInfo;
  wxTreeItemId item2;
  MyPostgresTable *table = NULL;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  wxString tableName = obj->GetVirtName();
  if (obj->GetType() == MY_POSTGRES_TABLE)
    table = MainFrame->FindPostgresTable(tableName);

  MainFrame->GetTableColumns(tableName, &list);
  wxString empty = wxEmptyString;
  col = list.GetFirstColumn();
  while (col)
    {
      int icon = 3;
      columnInfo = col->GetName();
      if (table != NULL)
        {
          // checking for a PK column
          if (table->IsPkColumn(columnInfo) == true)
            icon = 2;
        }
      item2 = AppendItem(item, columnInfo, icon);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_POSTGRES_COLUMN, empty, empty,
                                               -1, empty, empty, empty, empty,
                                               columnInfo, tableName)));
      col = col->GetNext();
    }
}

void MyTableTree::AddPostgresConnection(int num, MyPostgresConn * conn,
                                        wxTreeItemId & connId)
{
//
// appends a PostgreSQL Connection to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 36;
  data =
    (wxTreeItemData *) (new
                        MyObject(MY_POSTGRES_CONN, conn->GetHost(),
                                 conn->GetHostAddr(), conn->GetPort(),
                                 conn->GetDbName(), conn->GetUser(),
                                 conn->IsReadOnly()));
  char dummy[128];
  sprintf(dummy, "connection #%d", num);
  item = AppendItem(RootPostgreSQL, wxString::FromUTF8(dummy), icon, -1, data);
  connId = item;
}

void MyTableTree::AddPostgresSchema(wxTreeItemId & parent,
                                    MyPostgresConn * conn, wxString & schema,
                                    wxTreeItemId & item)
{
//
// appends a PostgreSQL Schema to the TREE list
//
  wxTreeItemData *data;
  int icon = 37;
  data =
    (wxTreeItemData *) (new
                        MyObject(MY_POSTGRES_SCHEMA, conn->GetHost(),
                                 conn->GetHostAddr(), conn->GetPort(),
                                 conn->GetDbName(), conn->GetUser(), schema));
  item = AppendItem(parent, schema, icon, -1, data);
}

void MyTableTree::AddPostgresTable(MyPostgres * list, wxString & virtName)
{
//
// appends a PostgreSQL Table or View to the TREE list
//
  int icon;
  int type = -1;
  MyPostgresConn *conn;
  wxTreeItemId parent;
  wxString schema;
  wxString table;
  bool readOnly;
  bool hasPK;
  bool canSelect;
  bool canInsertUpdateDelete;
  MyPostgresConn *pC = list->GetFirst();
  while (pC != NULL)
    {
      readOnly = pC->IsReadOnly();
      MyPostgresSchema *pS = pC->GetFirst();
      while (pS != NULL)
        {
          MyPostgresTable *pT = pS->GetFirstTable();
          while (pT != NULL)
            {
              if (pT->GetFirst() != NULL)
                {
                  if (virtName.Cmp(pT->GetPostGisName()) == 0)
                    {
                      conn = pC;
                      parent = pS->GetTreeNode();
                      icon = 12;
                      type = MY_POSTGIS_VIEW;
                      schema = pS->GetName();
                      table = pT->GetName();
                      hasPK = pT->HasPK();
                      canSelect = pT->CanSelect();
                      canInsertUpdateDelete = pT->CanInsertUpdateDelete();
                      goto done;
                    }
                }
              if (virtName.Cmp(pT->GetVirtName()) == 0)
                {
                  MainFrame->InitPostgresPkColumns(pC, pS, pT);
                  conn = pC;
                  parent = pS->GetTreeNode();
                  icon = 38;
                  type = MY_POSTGRES_TABLE;
                  schema = pS->GetName();
                  table = pT->GetName();
                  hasPK = pT->HasPK();
                  canSelect = pT->CanSelect();
                  canInsertUpdateDelete = pT->CanInsertUpdateDelete();
                  goto done;
                }
              pT = pT->GetNext();
            }
          MyPostgresView *pV = pS->GetFirstView();
          while (pV != NULL)
            {
              if (virtName.Cmp(pV->GetVirtName()) == 0)
                {
                  conn = pC;
                  parent = pS->GetTreeNode();
                  icon = 39;
                  type = MY_POSTGRES_VIEW;
                  schema = pS->GetName();
                  table = pV->GetName();
                  hasPK = false;
                  canSelect = pV->CanSelect();
                  canInsertUpdateDelete = pV->CanInsertUpdateDelete();
                  goto done;
                }
              pV = pV->GetNext();
            }
          pS = pS->GetNext();
        }
      pC = pC->GetNext();
    }
done:
  if (type == -1)
    return;

// adding the TREE node
  wxTreeItemId item;
  wxTreeItemData *data;
  if (type == MY_POSTGIS_VIEW)
    data =
      (wxTreeItemData *) (new
                          MyObject(type, conn->GetHost(), conn->GetHostAddr(),
                                   conn->GetPort(), conn->GetDbName(),
                                   conn->GetUser(), schema, table,
                                   virtName, readOnly, hasPK, canSelect,
                                   canInsertUpdateDelete));
  else
    data =
      (wxTreeItemData *) (new
                          MyObject(type, conn->GetHost(), conn->GetHostAddr(),
                                   conn->GetPort(), conn->GetDbName(),
                                   conn->GetUser(), schema, table, virtName,
                                   readOnly, hasPK, canSelect,
                                   canInsertUpdateDelete));
  item = AppendItem(parent, virtName, icon, -1, data);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddView(wxString & viewName, bool geometry)
{
//
// appends a view to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 9;
  bool topo_obj;
  bool tile_data;
  wxTreeItemId rootNode = GetRootNode(viewName, NULL, &topo_obj, &tile_data);
  wxString empty = wxEmptyString;
  data =
    (wxTreeItemData *) (new
                        MyObject(MY_VIEW, empty, viewName, false, topo_obj));
  item = AppendItem(rootNode, viewName, icon, -1, data);
  if (geometry == true)
    SetItemImage(item, 12);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::AddView(wxString & dbAlias, wxString & viewName,
                          bool geometry)
{
//
// appends a view to the TREE list
//
  wxTreeItemId item;
  wxTreeItemData *data;
  int icon = 9;
  bool topo_obj;
  bool tile_data;
  wxTreeItemId rootNode = GetRootNode(viewName, NULL, &topo_obj, &tile_data);
  wxString empty = wxEmptyString;
  data = (wxTreeItemData *) (new MyObject(MY_VIEW, dbAlias, viewName));
  item = AppendItem(rootNode, viewName, icon, -1, data);
  if (geometry == true)
    SetItemImage(item, 12);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::ExpandView(wxTreeItemId & item)
{
//
// expands a TreeItem of the View Type
//
  MyViewInfo list;
  MyColumnInfo *col;
  MyTriggerInfo *trgr;
  wxTreeItemId item2;
  wxString columnInfo;
  wxString triggerInfo;
  int icon = 9;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  wxString viewName = obj->GetMainName();
  bool topo_obj = obj->IsTopoObject();

  MainFrame->GetViewColumns(viewName, &list);
  MainFrame->GetViewTriggers(viewName, &list);
  wxString empty = wxEmptyString;
  col = list.GetFirst();
  while (col)
    {
      columnInfo = col->GetName();
      if (col->IsGeometry() == true)
        {
          if (col->IsGeometryIndex() == true)
            icon = 7;
          else if (col->IsMbrCache() == true)
            icon = 11;
          else
            icon = 6;
      } else
        icon = 3;
      item2 = AppendItem(item, columnInfo, icon);
      if (col->IsGeometry() == true)
        {
          if (col->IsGeometryIndex() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIEW_GEOMETRY_INDEX,
                                                     empty, viewName, topo_obj,
                                                     col->GetName())));
          else if (col->IsMbrCache() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIEW_GEOMETRY_CACHED,
                                                     empty, viewName, topo_obj,
                                                     col->GetName())));
          else
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIEW_GEOMETRY, empty,
                                                     viewName, topo_obj,
                                                     col->GetName())));
      } else
        SetItemData(item2,
                    (wxTreeItemData *) (new
                                        MyObject(MY_VIEW_COLUMN, empty,
                                                 viewName, col->GetName())));
      col = col->GetNext();
    }
  trgr = list.GetFirstTrigger();
  while (trgr)
    {
      // setting up Triggers
      triggerInfo = trgr->GetName();
      item2 = AppendItem(item, triggerInfo, 5);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_TRIGGER, empty,
                                               trgr->GetName())));
      trgr = trgr->GetNext();
    }
}

void MyTableTree::ExpandPostGisView(wxTreeItemId & item)
{
//
// expands a TreeItem of the PostGIS View Type (SpatiaLite wrapper)
//
  MyViewInfo list;
  MyColumnInfo *col;
  MyTriggerInfo *trgr;
  wxString columnInfo;
  wxTreeItemId item2;
  wxString empty = wxEmptyString;
  MyPostgresTable *table = NULL;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  wxString viewName = obj->GetVirtName();
  table = MainFrame->FindPostGisView(viewName);

  MainFrame->GetViewColumns(viewName, &list);
  MainFrame->GetViewTriggers(viewName, &list);
  col = list.GetFirst();
  while (col)
    {
      int icon = 3;
      columnInfo = col->GetName();
      if (MainFrame->DoCheckPostGisGeometry(viewName, columnInfo) == true)
        {
          icon = 6;
          item2 = AppendItem(item, columnInfo, icon);
          SetItemData(item2,
                      (wxTreeItemData *) (new
                                          MyObject(MY_POSTGIS_GEOMETRY, empty,
                                                   empty, -1, empty, empty,
                                                   empty, empty, columnInfo,
                                                   viewName)));
      } else
        {
          if (table != NULL)
            {
              // checking for a PK column
              if (table->IsPkColumn(columnInfo) == true)
                icon = 2;
            }
          item2 = AppendItem(item, columnInfo, icon);
          SetItemData(item2,
                      (wxTreeItemData *) (new
                                          MyObject(MY_POSTGRES_COLUMN, empty,
                                                   empty, -1, empty, empty,
                                                   empty, empty, columnInfo,
                                                   viewName)));
        }
      col = col->GetNext();
    }
  trgr = list.GetFirstTrigger();
  while (trgr)
    {
      // setting up Triggers
      wxString triggerInfo = trgr->GetName();
      item2 = AppendItem(item, triggerInfo, 5);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_TRIGGER, empty,
                                               trgr->GetName())));
      trgr = trgr->GetNext();
    }
}

void MyTableTree::AddTable(wxString & dbAlias,
                           wxString & tableName, bool virtualTable,
                           bool geometry, RootNodes * nodes)
{
//
// appends a table to the TREE list [ATTACHED DB]
//
  wxTreeItemId item;
  wxTreeItemId item2;
  wxTreeItemData *data;
  bool isTopoObject = false;
  bool isTileData = false;
  int icon = 1;
  if (virtualTable == true)
    icon = 8;
  if (geometry == true)
    {
      if (virtualTable == false)
        icon = 10;
      else
        icon = 13;
    }
  if (dbAlias.IsSameAs(wxT("temp"), false))
    icon = 14;
  wxTreeItemId rootNode =
    GetAltRootNode(tableName, nodes, &isTopoObject, &isTileData);
  if (isTileData == true)
    icon = 23;
  wxString empty = wxEmptyString;
  if (virtualTable == true)
    data = (wxTreeItemData *) (new MyObject(MY_VTABLE, dbAlias, tableName));
  else if (isTileData == true)
    data = (wxTreeItemData *) (new MyObject(MY_TILE_DATA, dbAlias, tableName));
  else
    data = (wxTreeItemData *) (new MyObject(MY_TABLE, dbAlias, tableName));
  item = AppendItem(rootNode, tableName, icon, -1, data);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::ExpandAttachedTable(wxTreeItemId & item)
{
//
// expands a TreeItem of the TABLE type (ATTACHED DB)
//
  MyTableInfo list;
  MyColumnInfo *col;
  MyIndexInfo *idx;
  MyTriggerInfo *trgr;
  wxTreeItemId item2;
  wxTreeItemId item3;
  wxString columnInfo;
  wxString indexInfo;
  wxString triggerInfo;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  wxString tableName = obj->GetMainName();
  wxString dbAlias = obj->GetDbAlias();
  bool virtualTable = false;
  if (obj->GetType() == MY_VTABLE)
    virtualTable = true;
  MainFrame->GetTableColumns(dbAlias, tableName, &list);
  MainFrame->GetTableIndices(dbAlias, tableName, &list);
  MainFrame->GetTableTriggers(dbAlias, tableName, &list);
  wxString empty = wxEmptyString;
  col = list.GetFirstColumn();
  while (col)
    {
      int icon;
      if (col->IsPrimaryKey() == true)
        icon = 2;
      else
        {
          if (col->IsGeometry() == true || col->IsGPKGGeometry() == true)
            {
              if (col->IsGeometryIndex() == true)
                icon = 7;
              else if (col->IsMbrCache() == true)
                icon = 11;
              else
                icon = 6;
          } else
            icon = 3;
        }
      columnInfo = col->GetName();
      item2 = AppendItem(item, columnInfo, icon);
      if (virtualTable == true)
        {
          if (col->IsGeometry() == true)
            {
              SetItemData(item2,
                          (wxTreeItemData *) (new
                                              MyObject(MY_VIRTUAL_GEOMETRY,
                                                       dbAlias, tableName,
                                                       col->GetName())));
          } else if (col->IsGPKGGeometry() == true)
            {
              SetItemData(item2,
                          (wxTreeItemData *) (new
                                              MyObject(MY_VIRTUAL_GPKG_GEOMETRY,
                                                       dbAlias, tableName,
                                                       col->GetName())));
          } else
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIRTUAL_COLUMN, dbAlias,
                                                     tableName,
                                                     col->GetName())));
      } else
        {
          if (col->IsGeometry() == true)
            {
              if (col->IsGeometryIndex() == true)
                SetItemData(item2,
                            (wxTreeItemData *) (new
                                                MyObject(MY_GEOMETRY_INDEX,
                                                         dbAlias, tableName,
                                                         col->GetName())));
              else if (col->IsMbrCache() == true)
                SetItemData(item2,
                            (wxTreeItemData *) (new
                                                MyObject(MY_GEOMETRY_CACHED,
                                                         dbAlias, tableName,
                                                         col->GetName())));
              else
                SetItemData(item2,
                            (wxTreeItemData *) (new
                                                MyObject(MY_GEOMETRY, dbAlias,
                                                         tableName,
                                                         col->GetName())));
          } else
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_COLUMN, dbAlias,
                                                     tableName,
                                                     col->GetName())));
        }
      col = col->GetNext();
    }
// setting up Primary Key
  bool ok_pk = false;
  idx = list.GetFirstIndex();
  while (idx)
    {
      indexInfo = idx->GetName();
      if (indexInfo.StartsWith(wxT("sqlite_autoindex_")) == true)
        {
          item2 = AppendItem(item, wxT("PrimaryKey"), 19);
          SetItemData(item2,
                      (wxTreeItemData *) (new
                                          MyObject(MY_PRIMARY_KEY, dbAlias,
                                                   indexInfo)));
          MainFrame->GetPrimaryKeyFields(dbAlias, indexInfo, tableName, item2);
          ok_pk = true;
          break;
        }
      idx = idx->GetNext();
    }
  if (ok_pk == false)
    {
      col = list.GetFirstColumn();
      while (col)
        {
          if (col->IsPrimaryKey() == true)
            {
              ok_pk = true;
              break;
            }
          col = col->GetNext();
        }
      if (ok_pk == true)
        {
          item2 = AppendItem(item, wxT("PrimaryKey"), 19);
          SetItemData(item2,
                      (wxTreeItemData *) (new
                                          MyObject(MY_PRIMARY_KEY, dbAlias,
                                                   tableName)));
          col = list.GetFirstColumn();
          while (col)
            {
              if (col->IsPrimaryKey() == true)
                {
                  item3 = AppendItem(item2, col->GetName(), 2);
                  SetItemData(item3,
                              (wxTreeItemData *) (new
                                                  MyObject(MY_PRIMARY_KEY_FLD,
                                                           dbAlias, tableName,
                                                           col->GetName())));
                  break;
                }
              col = col->GetNext();
            }
        }
    }
// setting up Foreign Keys
  MainFrame->GetForeignKeys(dbAlias, tableName, item);
  idx = list.GetFirstIndex();
  while (idx)
    {
      // setting up Indices
      indexInfo = idx->GetName();
      if (indexInfo.StartsWith(wxT("sqlite_autoindex_")) == true)
        {
          idx = idx->GetNext();
          continue;
        }
      item2 = AppendItem(item, indexInfo, 4);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_INDEX, dbAlias,
                                               idx->GetName())));
      MainFrame->GetIndexFields(dbAlias, indexInfo, tableName, item2);
      idx = idx->GetNext();
    }
  trgr = list.GetFirstTrigger();
  while (trgr)
    {
      // setting up Triggers
      triggerInfo = trgr->GetName();
      item2 = AppendItem(item, triggerInfo, 5);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_TRIGGER, dbAlias,
                                               trgr->GetName())));
      trgr = trgr->GetNext();
    }
}

void MyTableTree::AddView(wxString & dbAlias,
                          wxString & viewName, bool geometry, RootNodes * nodes)
{
//
// appends a view to the TREE list [ATTACHED DB]
//
  wxTreeItemId item;
  wxTreeItemId item2;
  wxTreeItemData *data;
  int icon = 9;
  if (geometry == true)
    icon = 12;
  if (dbAlias.IsSameAs(wxT("temp"), false))
    icon = 14;
  bool topo_obj;
  bool tile_data;
  wxTreeItemId rootNode =
    GetAltRootNode(viewName, nodes, &topo_obj, &tile_data);
  wxString empty = wxEmptyString;
  data = (wxTreeItemData *) (new MyObject(MY_VIEW, dbAlias, viewName));
  item = AppendItem(rootNode, viewName, icon, -1, data);
  AppendItem(item, wxT("placeholder"), 0);
}

void MyTableTree::ExpandAttachedView(wxTreeItemId & item)
{
//
// expands a TreeItem of the VIEW type (ATTACHED DB)
//
  MyViewInfo list;
  MyColumnInfo *col;
  MyTriggerInfo *trgr;
  wxTreeItemId item2;
  wxString columnInfo;
  wxString triggerInfo;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  wxString viewName = obj->GetMainName();
  wxString dbAlias = obj->GetDbAlias();
  MainFrame->GetViewColumns(dbAlias, viewName, &list);
  MainFrame->GetViewTriggers(dbAlias, viewName, &list);
  wxString empty = wxEmptyString;
  col = list.GetFirst();
  while (col)
    {
      int icon;
      columnInfo = col->GetName();
      if (col->IsGeometry() == true)
        {
          if (col->IsGeometryIndex() == true)
            icon = 7;
          else if (col->IsMbrCache() == true)
            icon = 11;
          else
            icon = 6;
      } else
        icon = 3;
      item2 = AppendItem(item, columnInfo, icon);
      if (col->IsGeometry() == true)
        {
          if (col->IsGeometryIndex() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIEW_GEOMETRY_INDEX,
                                                     dbAlias, viewName,
                                                     col->GetName())));
          else if (col->IsMbrCache() == true)
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIEW_GEOMETRY_CACHED,
                                                     dbAlias, viewName,
                                                     col->GetName())));
          else
            SetItemData(item2,
                        (wxTreeItemData *) (new
                                            MyObject(MY_VIEW_GEOMETRY, dbAlias,
                                                     viewName,
                                                     col->GetName())));
      } else
        SetItemData(item2,
                    (wxTreeItemData *) (new
                                        MyObject(MY_VIEW_COLUMN, dbAlias,
                                                 viewName, col->GetName())));
      col = col->GetNext();
    }
  trgr = list.GetFirstTrigger();
  while (trgr)
    {
      // setting up Triggers
      triggerInfo = trgr->GetName();
      item2 = AppendItem(item, triggerInfo, 5);
      SetItemData(item2,
                  (wxTreeItemData *) (new
                                      MyObject(MY_TRIGGER, dbAlias,
                                               trgr->GetName())));
      trgr = trgr->GetNext();
    }
}

void MyTableTree::OnSelChanged(wxTreeEvent & event)
{
//
// selecting some node [mouse action]
//
  wxTreeItemId item = event.GetItem();
  if (item == Root || item == RootUserData || item == RootTopologies
      || item == RootNetworks || item == RootRasterCoverages
      || item == RootVectorCoverages || item == RootStyling || item == RootWMS
      || item == RootPostgreSQL || item == RootIsoMetadata
      || item == RootMetadata || item == RootInternal
      || item == RootSpatialIndex)
    return;
  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  CurrentItem = item;
}

void MyTableTree::DoRootRasterCoveragesContextMenu(wxPoint & pt)
{
// MAIN: ROOT RASTER COVERAGES context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("Raster Coverages on MAIN DB"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_CreateRasterCoverage,
                   wxT("Create New &Raster Coverage"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_AddAllRastersSrid,
                   wxT("Add an alternative SRID to all Raster Coverages"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateRasterExtentAll,
                   wxT("Update Raster &Coverages Extent"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
  return;
}

void MyTableTree::DoRootVectorCoveragesContextMenu(wxPoint & pt)
{
// MAIN: ROOT VECTOR COVERAGES context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("Vector Coverages on MAIN DB"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  wxMenu *vectorMenu = new wxMenu();
  menuItem =
    new wxMenuItem(vectorMenu, Tree_VectorRegister,
                   wxT("Register New &SpatialTable Coverage"));
  vectorMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorMenu, Tree_SpatialViewRegister,
                   wxT("Register New &SpatialView Coverage"));
  vectorMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorMenu, Tree_VirtualTableRegister,
                   wxT("Register New &VirtualTable Coverage"));
  vectorMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorMenu, Tree_TopoGeoRegister,
                   wxT("Register New Topology-&Geometry Coverage"));
  vectorMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorMenu, Tree_TopoNetRegister,
                   wxT("Register New Topology-&Network Coverage"));
  vectorMenu->Append(menuItem);
  menu.AppendSubMenu(vectorMenu, wxT("&Register New Vector Coverage"));
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_AddAllVectorsSrid,
                   wxT("Add an alternative SRID to all Vector Coverages"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateVectorExtentAll,
                   wxT("Update Vector &Coverages Extent"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootWmsLayersContextMenu(wxPoint & pt)
{
// MAIN: ROOT WMS LAYERS context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("WMS Layers on MAIN DB"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_WmsLayerRegister,
                   wxT("Register New &WMS Layer"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootTopoGeoContextMenu(wxPoint & pt)
{
// MAIN: ROOT TOPO-GEO context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("Topologies on MAIN DB"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_CreateTopoGeo,
                   wxT("Create New &Topology-Geometry"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootTopoNetContextMenu(wxPoint & pt)
{
// MAIN: ROOT TOPO-NET context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("Networks on MAIN DB"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_CreateTopoNet,
                   wxT("Create New &Topology-Network"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootStylingContextMenu(wxPoint & pt)
{
// MAIN: ROOT STYLING context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("Styling (SLD/SE) on MAIN DB"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  wxMenu *externalMenu = new wxMenu();
  menuItem =
    new wxMenuItem(externalMenu, Tree_RegisterExternalGraphic,
                   wxT("&Register New External Graphic"));
  externalMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(externalMenu, Tree_UnregisterExternalGraphic,
                   wxT("&Unregister External Graphic"));
  externalMenu->Append(menuItem);
  menu.AppendSubMenu(externalMenu, wxT("External &Graphics"));
  wxMenu *fontMenu = new wxMenu();
  menuItem =
    new wxMenuItem(fontMenu, Tree_RegisterTextFont,
                   wxT("&Register New Text Font"));
  fontMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(fontMenu, Tree_UnregisterTextFont,
                   wxT("&Unregister Text Font"));
  fontMenu->Append(menuItem);
  menu.AppendSubMenu(fontMenu, wxT("Text &Fonts"));
  wxMenu *styleMenu = new wxMenu();
  wxMenu *rasterMenu = new wxMenu();
  menuItem =
    new wxMenuItem(rasterMenu, Tree_NewRasterStyle,
                   wxT("Add New SLD/SE &Raster Style"));
  rasterMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(rasterMenu, Tree_ReloadRasterStyle,
                   wxT("Reload SLD/SE Raster &Style"));
  rasterMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(rasterMenu, Tree_UnregisterRasterStyle,
                   wxT("&Unregister SLD/SE Raster Style"));
  rasterMenu->Append(menuItem);
  styleMenu->AppendSubMenu(rasterMenu, wxT("SLD/SE &Raster Styles"));
  styleMenu->AppendSeparator();
  wxMenu *vectorMenu = new wxMenu();
  menuItem =
    new wxMenuItem(vectorMenu, Tree_NewVectorStyle,
                   wxT("Add New SLD/SE &Vector Style"));
  vectorMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(vectorMenu, Tree_ReloadVectorStyle,
                   wxT("Reload SLD/SE Vector &Style"));
  vectorMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(vectorMenu, Tree_UnregisterVectorStyle,
                   wxT("&Unregister SLD/SE Vector Style"));
  vectorMenu->Append(menuItem);
  styleMenu->AppendSubMenu(vectorMenu, wxT("SLD/SE &Vector Styles"));
  menu.AppendSubMenu(styleMenu, wxT("&SLD/SE &Styles"));
  wxMenu *toolsMenu = new wxMenu();
  wxMenu *rasterSymbolizerMenu = new wxMenu();
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerContrast,
                   wxT("&Contrast Enhancement"));
  rasterSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerChannelRgb,
                   wxT("Channel Selection [&false-colors]"));
  rasterSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerChannelGray,
                   wxT("Channel Selection [single &gray band]"));
  rasterSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerCategorize,
                   wxT("Color Map: &Categorize"));
  rasterSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerInterpolate,
                   wxT("Color Map: &Interpolate"));
  rasterSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerShadedRelief,
                   wxT("Shaded Relief (&brightness only)"));
  rasterSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(rasterSymbolizerMenu, Tree_RasterSymbolizerMonochrome,
                   wxT("Recolored &Monochrome"));
  rasterSymbolizerMenu->Append(menuItem);
  toolsMenu->AppendSubMenu(rasterSymbolizerMenu,
                           wxT("Raster&Symbolizer tools"));
  toolsMenu->AppendSeparator();
  wxMenu *vectorSymbolizerMenu = new wxMenu();
  menuItem =
    new wxMenuItem(vectorSymbolizerMenu, Tree_SimplePointSymbolizer,
                   wxT("Simple &Point Symbolizer"));
  vectorSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorSymbolizerMenu, Tree_SimpleLineSymbolizer,
                   wxT("Simple &Line Symbolizer"));
  vectorSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorSymbolizerMenu, Tree_SimplePolygonSymbolizer,
                   wxT("Simple Pol&ygon Symbolizer"));
  vectorSymbolizerMenu->Append(menuItem);
  menuItem =
    new wxMenuItem(vectorSymbolizerMenu, Tree_SimpleTextSymbolizer,
                   wxT("Simple &Text Symbolizer"));
  vectorSymbolizerMenu->Append(menuItem);
  toolsMenu->AppendSubMenu(vectorSymbolizerMenu,
                           wxT("VectorS&ymbolizer tools"));
  menu.AppendSubMenu(toolsMenu, wxT("SLD/SE &Tools"));
  wxMenu *mapConfigMenu = new wxMenu();
  menuItem =
    new wxMenuItem(mapConfigMenu, Tree_NewMapConfig,
                   wxT("Add New &Map Configuration"));
  mapConfigMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(mapConfigMenu, Tree_ReloadMapConfig,
                   wxT("Reload Map &Configuration"));
  mapConfigMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(mapConfigMenu, Tree_UnregisterMapConfig,
                   wxT("&Unregister Map Configuration"));
  mapConfigMenu->Append(menuItem);
  mapConfigMenu->AppendSeparator();
  menuItem =
    new wxMenuItem(mapConfigMenu, Tree_VerifyExternalMapConfig,
                   wxT("Verify &External Map Configuration"));
  mapConfigMenu->Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(mapConfigMenu, Tree_VerifyRegisteredMapConfig,
                   wxT("Verify &Registered Map Configuration"));
  mapConfigMenu->Append(menuItem);
  menu.AppendSubMenu(mapConfigMenu, wxT("Map Configurations"));
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateLayerStatisticsAll,
                   wxT("Update Layer &Statistics"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootPostgreSqlContextMenu(wxPoint & pt)
{
// MAIN: ROOT POTGRESQL context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(wxT("PostgreSQL"));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_CreatePostgreSqlConn,
                   wxT("Connect to a PostgreSQL &DBMS"));
  menu.Append(menuItem);
  if (MainFrame->HasPostgreSqlConnections() == true)
    {
      menuItem =
        new wxMenuItem(&menu, Tree_CloseAllPostgreSqlConns,
                       wxT("Close all connections to &PostgreSQL"));
      menu.Append(menuItem);
    }
  menuItem =
    new wxMenuItem(&menu, Tree_PostgreSqlDropOrphans,
                   wxT("Drop all PostgreSQL &Orphans"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootOthersContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN: OTHER ROOT NODES context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxString title;
  switch (obj->GetSubType())
    {
      case MY_ROOT_NODE:
        title = wxT("MAIN DB Connection");
        break;
      case MY_ROOT_USERDATA:
        title = wxT("User Data on MAIN DB");
        break;
      case MY_ROOT_ISOMETADATA:
        title = wxT("ISO/INSPIRE Metadata on MAIN DB");
        break;
      case MY_ROOT_METADATA:
        title = wxT("Metadata on MAIN DB");
        break;
      case MY_ROOT_INTERNAL:
        title = wxT("Internal Data on MAIN DB");
        break;
      case MY_ROOT_RTREE:
        title = wxT("Spatial Index on MAIN DB");
        break;
      default:
        title = wxT("??");
        break;
    };
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_QueryViewComposer, wxT("Query/View &Composer"));
  menuItem->SetBitmap(wxBitmap(composer_xpm));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_NewTable, wxT("Create New &Table"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_NewView, wxT("Create New &View"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateLayerStatisticsAll,
                   wxT("Update Layer &Statistics"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedDbContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED-DB NODE context menu
  wxMenuItem *menuItem;
  wxMenu menu(wxT("ATTACHED-DB \"") + obj->GetDbAlias() + wxT("\""));
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Detach, wxT("&Detach Database"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRootAttachedContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED-DB ROOT NODES context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxString title;
  switch (obj->GetSubType())
    {
      case MY_ROOT_USERDATA:
        title = wxT("User Data on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_ISOMETADATA:
        title =
          wxT("ISO/INSPIRE Metadata pn \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_TOPOLOGIES:
        title = wxT("Topologies on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_NETWORKS:
        title = wxT("Networks on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_RASTER:
        title =
          wxT("Raster Coverages on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_VECTOR:
        title =
          wxT("Vector Coverages on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_WMS:
        title = wxT("WMS Layers on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_STYLING:
        title =
          wxT("Styling (SLD/SE) on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_METADATA:
        title = wxT("Metadata on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_INTERNAL:
        title = wxT("Internal Data on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
      case MY_ROOT_RTREE:
        title = wxT("Spatial Index on \"") + obj->GetDbAlias() + wxT("\" DB");
        break;
    };
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoGenericAttachedContextMenu(wxPoint & pt, wxString & title)
{
// ATTACHED-DB GENERIC NODES context menu
  wxMenuItem *menuItem;
  CurrentItem = wxTreeItemId();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoTopoGeoContextMenu(wxPoint & pt, wxString & name)
{
// TOPO-GEO NODES context menu
  wxMenuItem *menuItem;
  wxMenu menu(wxT("TOPOLOGY: main.") + name);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (IsAlreadyDefinedTopologyCoverage(name) == false)
    {
      menuItem =
        new wxMenuItem(&menu, Tree_CreateTopologyCoverage,
                       wxT("&Create Topology Coverage"));
      menuItem->SetBitmap(wxBitmap(vector_xpm));
      menu.Append(menuItem);
      menu.AppendSeparator();
    }
  menuItem =
    new wxMenuItem(&menu, Tree_DropTopoGeo, wxT("&Drop Topology-Geometry"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoTopoNetContextMenu(wxPoint & pt, wxString & name)
{
// TOPO-NET NODES context menu
  wxMenuItem *menuItem;
  wxMenu menu(wxT("NETWORK: main.") + name);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (IsAlreadyDefinedNetworkCoverage(name) == false)
    {
      menuItem =
        new wxMenuItem(&menu, Tree_CreateNetworkCoverage,
                       wxT("&Create Network Coverage"));
      menuItem->SetBitmap(wxBitmap(vector_xpm));
      menu.Append(menuItem);
      menu.AppendSeparator();
    }
  menuItem =
    new wxMenuItem(&menu, Tree_DropTopoNet, wxT("&Drop Topology-Network"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoRasterCoverageContextMenu(wxPoint & pt, wxString & name)
{
// RASTER COVERAGE NODES context menu
  wxMenuItem *menuItem;
  wxMenu menu(wxT("RASTER COVERAGE: main.") + name);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_ImportRaster, wxT("Import &Raster file(s)"));
  menu.Append(menuItem);
  if (MainFrame->IsSecurityLevelRelaxed() != true)
    menuItem->Enable(false);
  menuItem =
    new wxMenuItem(&menu, Tree_Pyramidize, wxT("Build &Pyramids (by Section)"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_PyramidizeMonolithic,
                   wxT("Build Pyramids (&Monolithic)"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DePyramidize, wxT("&Remove Pyramids"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_RasterDrop, wxT("&Drop Raster Coverage"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateRasterExtent,
                   wxT("Update Raster &Coverage Extent"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_RasterInfos, wxT("&Edit Raster Coverage Infos"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_SldSeRasterStyles,
                   wxT("Supported SLD/SE Raster &Styles"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_Raster_SRIDs, wxT("&Alternative SRIDs"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Raster_Keywords, wxT("&Keywords"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoVectorCoverageContextMenu(wxPoint & pt, wxString & name)
{
// VECTOR COVERAGE NODES context menu
  wxMenuItem *menuItem;
  wxMenu menu(wxT("VECTOR COVERAGE: main.") + name);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_VectorUnregister,
                   wxT("&Unregister Vector Coverage"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateVectorExtent,
                   wxT("Update Vector &Coverage Extent"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_VectorInfos, wxT("&Edit Vector Coverage Infos"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_SldSeVectorStyles,
                   wxT("Supported SLD/SE Vector &Styles"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_Vector_SRIDs, wxT("&Alternative SRIDs"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Vector_Keywords, wxT("&Keywords"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoWmsLayerContextMenu(wxPoint & pt, wxString & name)
{
// WMS LAYER NODES context menu
  wxMenuItem *menuItem;
  wxMenu menu(wxT("WMS LAYER: main.") + name);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_WmsLayerUnregister,
                   wxT("&Unregister WMS Layer"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_WmsLayerInfos, wxT("&Edit WMS Layer Infos"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_WmsLayerConfigure, wxT("&Configure WMS Layer"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainTableContextMenu(wxPoint & pt, MyObject * obj, int icon)
{
// MAIN TABLE NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (obj->IsTemporary())
    {
      switch (icon)
        {
          case 1:
            title = wxT("TEMPORARY TABLE: temp.");
            break;
          case 8:
            title = wxT("TEMPORARY VirtualTABLE: temp.");
            break;
          case 10:
            title = wxT("TEMPORARY SpatialTable: temp.");
            break;
          case 13:
            title = wxT("TEMPORARY VirtualSHP: temp.");
            break;
          default:
            title = wxT("TEMPORARY ?? undefined ??");
            break;
        };
  } else
    {
      switch (icon)
        {
          case 1:
            title = wxT("TABLE: main.");
            break;
          case 8:
            title = wxT("VirtualTABLE: main.");
            break;
          case 10:
            title = wxT("SpatialTable: main.");
            break;
          case 13:
            title = wxT("VirtualSHP: main.");
            break;
          case 23:
            title = wxT("TileDataTable: main.");
            break;
          case 30:
            if (obj->GetType() == MY_GPKG_VTABLE)
              title = wxT("VirtualGeoPackageTable: main.");
            else
              title = wxT("VirtualFdoOgrTable: main.");
            break;
          case 31:
            if (obj->GetType() == MY_GPKG_TABLE)
              title = wxT("GeoPackageTable: main.");
            else
              title = wxT("FdoOgrTable: main.");
            break;
          default:
            title = wxT("?? undefined ??");
            break;
        };
    }
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  if (obj->GetType() == MY_GPKG_TABLE || obj->GetType() == MY_GPKG_VTABLE
      || obj->GetType() == MY_FDO_TABLE || obj->GetType() == MY_FDO_VTABLE)
    ;
  else
    {
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Tree_CloneTable, wxT("Clone Ta&ble"));
      menuItem->SetBitmap(wxBitmap(clone_xpm));
      menu.Append(menuItem);
    }
  if (obj->IsEditable())
    {
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_QueryViewComposer,
                       wxT("Query/View &Composer"));
      menuItem->SetBitmap(wxBitmap(composer_xpm));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_NewTable, wxT("Create New &Table"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_NewView, wxT("Create New &View"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_UpdateLayerStatisticsAll,
                       wxT("Update Layer &Statistics"));
      menu.Append(menuItem);
    }
  menu.AppendSeparator();
  if (obj->IsEditable())
    {
      menuItem = new wxMenuItem(&menu, Tree_Edit, wxT("&Edit table rows"));
      menu.Append(menuItem);
  } else
    {
      if (obj->GetType() == MY_TILE_DATA)
        menuItem = new wxMenuItem(&menu, Tree_SelectTiles, wxT("&Query table"));
      else
        menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query table"));
      menu.Append(menuItem);
    }
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  if (obj->GetType() == MY_VTABLE)
    {
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Tree_Drop, wxT("&Drop table"));
      menu.Append(menuItem);
      menu.AppendSeparator();
    }
  if (obj->IsEditable())
    {
      menu.AppendSeparator();
      wxMenu *maintenanceMenu = new wxMenu();
      menuItem =
        new wxMenuItem(maintenanceMenu, Tree_NewColumn, wxT("Add New &Column"));
      maintenanceMenu->Append(menuItem);
      menuItem =
        new wxMenuItem(maintenanceMenu, Tree_Rename, wxT("&Rename table"));
      maintenanceMenu->Append(menuItem);
      menuItem = new wxMenuItem(maintenanceMenu, Tree_Drop, wxT("&Drop table"));
      maintenanceMenu->Append(menuItem);
      maintenanceMenu->AppendSeparator();
      menuItem =
        new wxMenuItem(maintenanceMenu, Tree_NewIndex,
                       wxT("Create New &Index"));
      maintenanceMenu->Append(menuItem);
      menuItem =
        new wxMenuItem(maintenanceMenu, Tree_NewTrigger,
                       wxT("Create New &Trigger"));
      maintenanceMenu->Append(menuItem);

      maintenanceMenu->AppendSeparator();
      menuItem =
        new wxMenuItem(maintenanceMenu, Tree_CheckDuplicates,
                       wxT("Check &Duplicate rows"));
      maintenanceMenu->Append(menuItem);
      menuItem =
        new wxMenuItem(maintenanceMenu, Tree_RemoveDuplicates,
                       wxT("&Remove Duplicate rows"));
      maintenanceMenu->Append(menuItem);

      menu.AppendSubMenu(maintenanceMenu, wxT("&Maintenance"));
      menu.AppendSeparator();
  } else
    menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_DumpTxtTab, wxT("Export as &Txt/Tab"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpCsv, wxT("Export as &CSV"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpHtml, wxT("Export as &HTML"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDif, wxT("Export as &DIF"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpSylk, wxT("Export as &SYLK"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDbf, wxT("Export as &DBF"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpXlsx, wxT("Export as MS &XLSX Spreadsheet"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpPostGIS, wxT("SQL Dump for &PostGIS"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainViewContextMenu(wxPoint & pt, MyObject * obj, int icon)
{
// MAIN VIEW NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (obj->IsTemporary())
    {
      switch (icon)
        {
          case 9:
            title = wxT("TEMPORARY VIEW: temp.");
            break;
          case 12:
            title = wxT("TEMPORARY SpatialView: temp.");
            break;
          default:
            title = wxT("?? undefined ??");
            break;
        };
  } else
    {
      switch (icon)
        {
          case 9:
            title = wxT("VIEW: main.");
            break;
          case 12:
            title = wxT("SpatialView: main.");
            break;
          default:
            title = wxT("?? undefined ??");
            break;
        };
    }
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (MainFrame->IsWritableView(obj->GetMainName()))
    menuItem = new wxMenuItem(&menu, Tree_Edit, wxT("&Edit view rows"));
  else
    menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query view"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Drop, wxT("&Drop view"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_DumpTxtTab, wxT("Export as &Txt/Tab"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpCsv, wxT("Export as &CSV"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpHtml, wxT("Export as &HTML"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDif, wxT("Export as &DIF"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpSylk, wxT("Export as &SYLK"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDbf, wxT("Export as &DBF"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpXlsx, wxT("Export as MS &XLSX Spreadsheet"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpPostGIS, wxT("SQL Dump for &PostGIS"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedTableContextMenu(wxPoint & pt, MyObject * obj,
                                             int icon)
{
// ATTACHED TABLE NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  switch (icon)
    {
      case 1:
        title = wxT("TABLE: ") + obj->GetDbAlias();
        break;
      case 8:
        title = wxT("VirtualTABLE: ") + obj->GetDbAlias();
        break;
      case 10:
        title = wxT("SpatialTable: ") + obj->GetDbAlias();
        break;
      case 13:
        title = wxT("VirtualSHP: ") + obj->GetDbAlias();
        break;
      case 23:
        title = wxT("TileDataTable: ") + obj->GetDbAlias();
        break;
      case 30:
        if (obj->GetType() == MY_GPKG_VTABLE)
          title = wxT("VirtualGeoPackageTable: ") + obj->GetDbAlias();
        else
          title = wxT("VirtualFdoOgrTable: ") + obj->GetDbAlias();
        break;
      case 31:
        if (obj->GetType() == MY_GPKG_TABLE)
          title = wxT("GeoPackageTable: ") + obj->GetDbAlias();
        else
          title = wxT("FdoOgrTable: ") + obj->GetDbAlias();
        break;
      default:
        title = wxT("?? undefined ??");
        break;
    };
  title += wxT(".") + obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_CloneTable, wxT("Clone Ta&ble"));
  menuItem->SetBitmap(wxBitmap(clone_xpm));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (obj->GetType() == MY_TILE_DATA)
    menuItem = new wxMenuItem(&menu, Tree_SelectTiles, wxT("&Query table"));
  else
    menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query table"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_DumpTxtTab, wxT("Export as &Txt/Tab"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpCsv, wxT("Export as &CSV"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpHtml, wxT("Export as &HTML"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDif, wxT("Export as &DIF"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpSylk, wxT("Export as &SYLK"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDbf, wxT("Export as &DBF"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpXlsx, wxT("Export as MS &XLSX Spreadsheet"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpPostGIS, wxT("SQL Dump for &PostGIS"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedViewContextMenu(wxPoint & pt, MyObject * obj,
                                            int icon)
{
// ATTACHED VIEW NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  switch (icon)
    {
      case 9:
        title = wxT("VIEW: ") + obj->GetDbAlias();
        break;
      case 12:
        title = wxT("SpatialView: ") + obj->GetDbAlias();
        break;
      default:
        title = wxT("?? undefined ??");
        break;
    };
  title += wxT(".") + obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query view"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_DumpTxtTab, wxT("Export as &Txt/Tab"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpCsv, wxT("Export as &CSV"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpHtml, wxT("Export as &HTML"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDif, wxT("Export as &DIF"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpSylk, wxT("Export as &SYLK"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpDbf, wxT("Export as &DBF"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpXlsx, wxT("Export as MS &XLSX Spreadsheet"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_DumpPostGIS, wxT("SQL Dump for &PostGIS"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainColumnContextMenu(wxPoint & pt, MyObject * obj,
                                          int icon, bool metadata)
{
// MAIN COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (icon == 2)
    title = wxT("PK COLUMN: main.");
  else
    title = wxT("COLUMN: main.");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (MainFrame->IsPrimaryKey(obj->GetMainName(), obj->GetColName()) == false)
    {
      menuItem = new wxMenuItem(&menu, Tree_DropColumn, wxT("&Drop Column"));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_RenameColumn, wxT("&Rename Column"));
      menu.Append(menuItem);
      menu.AppendSeparator();
    }
  menuItem =
    new wxMenuItem(&menu, Tree_CheckGeometry, wxT("&Check geometries"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_MapPreview, wxT("&Map Preview"));
  menuItem->SetBitmap(wxBitmap(map_preview_xpm));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Extent, wxT("&Extent"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_UpdateLayerStatistics,
                   wxT("Update Layer &Statistics"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_SetSrid, wxT("&Set SRID"));
  menu.Append(menuItem);
  if (metadata == true)
    {
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_Recover, wxT("&Recover geometry column"));
      menu.Append(menuItem);
    }
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_ColumnStats, wxT("&Statistic snapshot"));
  menuItem->SetBitmap(wxBitmap(statistics_xpm));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainLimitedColumnContextMenu(wxPoint & pt, MyObject * obj,
                                                 int icon)
{
// MAIN (LIMITED) COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (icon == 2)
    title = wxT("PK COLUMN: main.");
  else
    title = wxT("COLUMN: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_ColumnStats, wxT("&Statistic snapshot"));
  menuItem->SetBitmap(wxBitmap(statistics_xpm));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainGpkgColumnContextMenu(wxPoint & pt, MyObject * obj,
                                              int icon)
{
// MAIN (GPKG) COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (icon == 2)
    title = wxT("PK COLUMN: main.");
  else
    title = wxT("COLUMN: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainFdoOgrColumnContextMenu(wxPoint & pt, MyObject * obj,
                                                int icon)
{
// MAIN (FDO/OGR) COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (icon == 2)
    title = wxT("PK COLUMN: main.");
  else
    title = wxT("COLUMN: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedColumnContextMenu(wxPoint & pt, MyObject * obj,
                                              int icon)
{
// ATTACHED COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (icon == 2)
    title = wxT("PK COLUMN: ");
  else
    title = wxT("COLUMN: ");
  title += obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_ColumnStats, wxT("&Statistic snapshot"));
  menuItem->SetBitmap(wxBitmap(statistics_xpm));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainGeometryColumnContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN GEOMETRY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (obj->IsGpkgGeometryColumn() == true)
    title = wxT("GeoPackageGeometry: main.");
  else if (obj->IsFdoOgrGeometryColumn() == true)
    title = wxT("FdoOgrGeometry: main.");
  else
    title = wxT("GEOMETRY: main.");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  if (obj->IsGpkgGeometryColumn() == false
      && obj->IsGpkgVirtualGeometryColumn() == false
      && obj->IsFdoOgrGeometryColumn() == false
      && obj->IsFdoOgrVirtualGeometryColumn() == false)
    {
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_RenameColumn, wxT("&Rename Column"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_Show, wxT("&Show Spatial Metadata"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_CheckGeom, wxT("&RTTOPO Check geometries"));
      menuItem->SetBitmap(wxBitmap(checkgeom_xpm));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_SaneGeom,
                       wxT("&RTTOPO Sanitize geometries"));
      menuItem->SetBitmap(wxBitmap(sanegeom_xpm));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_CheckGeometry, wxT("&Check geometries"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_Extent, wxT("&Extent"));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_UpdateLayerStatistics,
                       wxT("Update Layer &Statistics"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      if (obj->GetType() == MY_GEOMETRY)
        {
          menuItem =
            new wxMenuItem(&menu, Tree_SpatialIndex,
                           wxT("&Build Spatial Index"));
          menuItem->SetBitmap(wxBitmap(spatialidx_xpm));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Tree_MbrCache, wxT("Build &MBR cache"));
          menuItem->SetBitmap(wxBitmap(mbrcache_xpm));
          menu.Append(menuItem);
        }
      if (obj->GetType() == MY_GEOMETRY_INDEX)
        {
          menuItem =
            new wxMenuItem(&menu, Tree_SpatialIndex,
                           wxT("&Remove Spatial Index"));
          menuItem->SetBitmap(wxBitmap(kill_spindex_xpm));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Tree_CheckSpatialIndex,
                           wxT("&Check Spatial Index"));
          menu.Append(menuItem);
          menuItem =
            new wxMenuItem(&menu, Tree_RecoverSpatialIndex,
                           wxT("&Recover Spatial Index"));
          menu.Append(menuItem);
        }
      if (obj->GetType() == MY_GEOMETRY_CACHED)
        {
          menuItem =
            new wxMenuItem(&menu, Tree_MbrCache, wxT("&Remove MBR cache"));
          menuItem->SetBitmap(wxBitmap(kill_spindex_xpm));
          menu.Append(menuItem);
        }
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_RebuildTriggers,
                       wxT("Rebuild Geometry &Triggers"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_DumpShp, wxT("Export as &Shapefile"));
      menuItem->SetBitmap(wxBitmap(dumpshp_xpm));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_DumpGeoJSON, wxT("Export as &GeoJSON"));
      menuItem->SetBitmap(wxBitmap(dumpgeojs_xpm));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_DumpKml, wxT("Export as &KML"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_ElementaryGeoms,
                       wxT("&separating elementary Geometries"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_MalformedGeometries,
                       wxT("&Malformed geometries"));
      menuItem->SetBitmap(wxBitmap(malformed_geoms_xpm));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_RepairPolygons, wxT("&Repair Polygons"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      if (obj->IsAlreadyDefinedVectorCoverage(GetSQLiteHandle()) == false)
        {
          if (obj->IsTopoObject() == false)
            {
              // excluding Topology or Network Tables
              menuItem =
                new wxMenuItem(&menu, Tree_CreateVectorCoverage,
                               wxT("&Create Vector Coverage"));
              menuItem->SetBitmap(wxBitmap(vector_xpm));
              menu.Append(menuItem);
            }
        }
      menuItem = new wxMenuItem(&menu, Tree_MapPreview, wxT("&Map Preview"));
      menuItem->SetBitmap(wxBitmap(map_preview_xpm));
      menu.Append(menuItem);
    }
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainLimitedGeometryColumnContextMenu(wxPoint & pt,
                                                         MyObject * obj)
{
// MAIN (LIMITED) GEOMETRY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (obj->IsGpkgGeometryColumn() == true)
    title = wxT("GeoPackageGeometry: main.");
  else if (obj->IsFdoOgrGeometryColumn() == true)
    title = wxT("FdoOgrGeometry: main.");
  else
    title = wxT("GEOMETRY: main.");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  if (obj->IsGpkgGeometryColumn() == false
      && obj->IsGpkgVirtualGeometryColumn() == false
      && obj->IsFdoOgrGeometryColumn() == false
      && obj->IsFdoOgrVirtualGeometryColumn() == false)
    {
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_Show, wxT("&Show Spatial Metadata"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_Extent, wxT("&Extent"));
      menu.Append(menuItem);
      menuItem =
        new wxMenuItem(&menu, Tree_UpdateLayerStatistics,
                       wxT("Update Layer &Statistics"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_DumpShp, wxT("Export as &Shapefile"));
      menuItem->SetBitmap(wxBitmap(dumpshp_xpm));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_DumpKml, wxT("Export as &KML"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_ElementaryGeoms,
                       wxT("&separating elementary Geometries"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_MalformedGeometries,
                       wxT("&Malformed geometries"));
      menuItem->SetBitmap(wxBitmap(malformed_geoms_xpm));
      menu.Append(menuItem);
      menu.AppendSeparator();
      if (obj->IsAlreadyDefinedVectorCoverage(GetSQLiteHandle()) == false)
        {
          if (obj->IsTopoObject() == false)
            {
              // excluding Topology or Network Tables
              menuItem =
                new wxMenuItem(&menu, Tree_CreateVectorCoverage,
                               wxT("&Create Vector Coverage"));
              menuItem->SetBitmap(wxBitmap(vector_xpm));
              menu.Append(menuItem);
            }
        }
      menuItem = new wxMenuItem(&menu, Tree_MapPreview, wxT("&Map Preview"));
      menuItem->SetBitmap(wxBitmap(map_preview_xpm));
      menu.Append(menuItem);
    }
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedGeometryColumnContextMenu(wxPoint & pt,
                                                      MyObject * obj)
{
// ATTACHED GEOMETRY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title;
  if (obj->IsGpkgGeometryColumn() == true)
    title = wxT("GeoPackageGeometry: ");
  else if (obj->IsFdoOgrGeometryColumn() == true)
    title = wxT("FdoOgrGeometry: ");
  else
    title = wxT("GEOMETRY: ");
  title += obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName() + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  if (obj->IsGpkgGeometryColumn() == false
      && obj->IsGpkgVirtualGeometryColumn() == false
      && obj->IsFdoOgrGeometryColumn() == false
      && obj->IsFdoOgrVirtualGeometryColumn() == false)
    {
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_Show, wxT("&Show Spatial Metadata"));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_Extent, wxT("&Extent"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_DumpShp, wxT("Export as &Shapefile"));
      menuItem->SetBitmap(wxBitmap(dumpshp_xpm));
      menu.Append(menuItem);
      menuItem = new wxMenuItem(&menu, Tree_DumpKml, wxT("Export as &KML"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_ElementaryGeoms,
                       wxT("&separating elementary Geometries"));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem =
        new wxMenuItem(&menu, Tree_MalformedGeometries,
                       wxT("&Malformed geometries"));
      menuItem->SetBitmap(wxBitmap(malformed_geoms_xpm));
      menu.Append(menuItem);
      menu.AppendSeparator();
      menuItem = new wxMenuItem(&menu, Tree_MapPreview, wxT("&Map Preview"));
      menuItem->SetBitmap(wxBitmap(map_preview_xpm));
      menu.Append(menuItem);
    }
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainIndexContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN INDEX NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("INDEX: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show index"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Drop, wxT("&Drop index"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedIndexContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED INDEX NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("INDEX: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Drop, wxT("&Drop index"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainIndexColumnContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN INDEX COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("IDX FIELD: main.");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedIndexColumnContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED INDEX COLUMN KEY NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("IDX FIELD: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainTriggerContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN TRIGGER NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("TRIGGER: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show trigger"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Drop, wxT("&Drop trigger"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedTriggerContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED TRIGGER NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("TRIGGER: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show trigger"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainPrimaryKeyContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN PRIMARY KEY NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PRIMARY KEY: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedPrimaryKeyContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED PRIMARY KEY NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PRIMARY KEY: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainPrimaryKeyColumnContextMenu(wxPoint & pt,
                                                    MyObject * obj)
{
// MAIN PRIMARY KEY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PK FIELD: main.");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedPrimaryKeyColumnContextMenu(wxPoint & pt,
                                                        MyObject * obj)
{
// ATTACHED PRIMARY KEY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PK FIELD: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainForeignKeyContextMenu(wxPoint & pt, MyObject * obj)
{
// MAIN FOREIGN KEY NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("FOREIGN KEY: main.");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedForeignKeyContextMenu(wxPoint & pt, MyObject * obj)
{
// ATTACHED FOREIGN KEY NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("FOREIGN KEY: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoMainForeignKeyColumnContextMenu(wxPoint & pt,
                                                    MyObject * obj)
{
// MAIN FOREIGN KEY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("FK FIELD: main.");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoAttachedForeignKeyColumnContextMenu(wxPoint & pt,
                                                        MyObject * obj)
{
// ATTACHED FOREIGN KEY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("FK FIELD: ") + obj->GetDbAlias() + wxT(".");
  title += obj->GetMainName() + wxT(".") + obj->GetColName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostgreSqlContextMenu(wxPoint & pt, MyObject * obj)
{
// PostgreSQL objects: context menu dispatcher
  switch (obj->GetType())
    {
      case MY_POSTGRES_CONN:
        DoPostgresConnContextMenu(pt);
        break;
      case MY_POSTGRES_SCHEMA:
        DoPostgresSchemaContextMenu(pt);
        break;
      case MY_POSTGRES_TABLE:
        DoPostgresTableContextMenu(pt, obj);
        break;
      case MY_POSTGRES_VIEW:
        DoPostgresViewContextMenu(pt, obj);
        break;
      case MY_POSTGIS_VIEW:
        DoPostGisViewContextMenu(pt, obj);
        break;
      case MY_POSTGRES_COLUMN:
        DoPostgresColumnContextMenu(pt, obj);
        break;
      case MY_POSTGIS_GEOMETRY:
        DoPostGisGeometryContextMenu(pt, obj);
        break;
    };
}

void MyTableTree::DoPostgresConnContextMenu(wxPoint & pt)
{
// POSTGRES CONNECTION NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PostgreSQL Connection");
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_ClosePostgreSqlConn,
                   wxT("Close this connection to PostgreSQL"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_PostgreSqlInfos, wxT("&Infos"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostgresSchemaContextMenu(wxPoint & pt)
{
// POSTGRES SCHEMA NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PostgreSQL Schema");
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_PostgreSqlInfos, wxT("&Infos"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostgresTableContextMenu(wxPoint & pt, MyObject * obj)
{
// POSTGRES TABLE NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PostgreSQL Table: ") + obj->GetVirtName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (obj->IsReadOnly() == false && obj->HasPK() == true
      && obj->CanInsertUpdateDelete() == true)
    {
      menuItem = new wxMenuItem(&menu, Tree_Edit, wxT("&Edit table rows"));
      menu.Append(menuItem);
  } else if (obj->CanSelect() == true)
    {
      menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query table"));
      menu.Append(menuItem);
    }
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_PostgreSqlInfos, wxT("PostgreSQL &Infos"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostgresViewContextMenu(wxPoint & pt, MyObject * obj)
{
// POSTGRES VIEW NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PostgreSQL View: ") + obj->GetVirtName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (obj->CanSelect() == true)
    {
      menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query View"));
      menu.Append(menuItem);
    }
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_PostgreSqlInfos, wxT("PostgreSQL &Infos"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostGisViewContextMenu(wxPoint & pt, MyObject * obj)
{
// POSTGIS VIEW NODES context menu
  wxMenuItem *menuItem;
  wxString title = wxT("PostGIS-to-SpatiaLite wrapper: ") + obj->GetVirtName();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  if (obj->IsReadOnly() == false && obj->HasPK() == true
      && obj->CanInsertUpdateDelete() == true)
    {
      menuItem = new wxMenuItem(&menu, Tree_Edit, wxT("&Edit table rows"));
      menu.Append(menuItem);
  } else if (obj->CanSelect() == true)
    {
      menuItem = new wxMenuItem(&menu, Tree_Select, wxT("&Query table"));
      menu.Append(menuItem);
    }
  menuItem = new wxMenuItem(&menu, Tree_Show, wxT("&Show columns"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_ShowSql, wxT("&Show CREATE statement"));
  menu.Append(menuItem);
  menuItem =
    new wxMenuItem(&menu, Tree_PostgreSqlInfos, wxT("PostgreSQL &Infos"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostgresColumnContextMenu(wxPoint & pt, MyObject * obj)
{
// POSTGRESQL COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title =
    wxT("PostgreSQL Column: ") + obj->GetVirtName() + wxT(".") +
    obj->GetColumn();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::DoPostGisGeometryContextMenu(wxPoint & pt, MyObject * obj)
{
// POSTGIS GEOMETRY COLUMN NODES context menu
  wxMenuItem *menuItem;
  wxString title =
    wxT("PostGIS Geometry: ") + obj->GetVirtName() + wxT(".") +
    obj->GetColumn();
  wxMenu menu(title);
  menuItem = new wxMenuItem(&menu, Tree_Refresh, wxT("&Refresh"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem =
    new wxMenuItem(&menu, Tree_CheckGeometry, wxT("&Check geometries"));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_Extent, wxT("&Extent"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_DumpShp, wxT("Export as &Shapefile"));
  menuItem->SetBitmap(wxBitmap(dumpshp_xpm));
  menu.Append(menuItem);
  menuItem = new wxMenuItem(&menu, Tree_DumpKml, wxT("Export as &KML"));
  menu.Append(menuItem);
  menu.AppendSeparator();
  menuItem = new wxMenuItem(&menu, Tree_MapPreview, wxT("&Map Preview"));
  menuItem->SetBitmap(wxBitmap(map_preview_xpm));
  menu.Append(menuItem);
  PopupMenu(&menu, pt);
}

void MyTableTree::OnRightClick(wxTreeEvent & event)
{
//
// right click on some node [mouse action] Context Menu
//
  if (MainFrame->IsConnected() == false)
    return;
  bool metadata = MainFrame->CheckMetadata();

  wxTreeItemId item = event.GetItem();
  SelectItem(item);
  wxPoint pt = event.GetPoint();

// special cases: MAIN ROOT NODES
  if (item == RootRasterCoverages)
    {
      // ROOT RASTER COVERAGES
      DoRootRasterCoveragesContextMenu(pt);
      return;
    }
  if (item == RootVectorCoverages)
    {
      // ROOT VECTOR COVERAGES
      DoRootVectorCoveragesContextMenu(pt);
      return;
    }
  if (item == RootWMS)
    {
      // ROOT WMS LAYER
      DoRootWmsLayersContextMenu(pt);
      return;
    }
  if (item == RootTopologies)
    {
      // ROOT TOPOGEO
      DoRootTopoGeoContextMenu(pt);
      return;
    }
  if (item == RootNetworks)
    {
      // ROOT TOPONET
      DoRootTopoNetContextMenu(pt);
      return;
    }
  if (item == RootStyling)
    {
      // ROOT STYLING
      DoRootStylingContextMenu(pt);
      return;
    }
  if (item == RootPostgreSQL)
    {
      // ROOT POSTGRESQL
      DoRootPostgreSqlContextMenu(pt);
      return;
    }

  MyObject *obj = (MyObject *) GetItemData(item);
  if (obj == NULL)
    return;
  if (item == Root || item == RootUserData || item == RootIsoMetadata
      || item == RootMetadata || item == RootInternal
      || item == RootSpatialIndex)
    {
      // any other MAIN ROOT items
      DoRootOthersContextMenu(pt, obj);
      return;
    }
// special cases: ATTACHED-DB ROOT NODES
  if (obj->IsAttachedDB())
    {
      // ATTACHED DB main node    
      CurrentItem = item;
      DoAttachedDbContextMenu(pt, obj);
      return;
    }
  if (obj->IsRootAttached())
    {
      // ATTACHED ROOT items
      DoRootAttachedContextMenu(pt, obj);
      return;
    }
// special cases: PostgreSQL objects
  if (obj->IsPostgreSQL() == true)
    {
      DoPostgreSqlContextMenu(pt, obj);
      return;
    }
// special cases: TOPO-GEO and TOPO-NET objects
  if (obj->GetType() == MY_TOPO_GEO || obj->GetType() == MY_TOPO_NET)
    {
      if (obj->IsAttached() == true)
        {
          wxString title;
          if (obj->GetType() == MY_TOPO_GEO)
            title +=
              wxT("TOPOLOGY: ") + obj->GetDbAlias() + wxT(".") +
              obj->GetMainName();
          else
            title +=
              wxT("NETWORK: ") + obj->GetDbAlias() + wxT(".") +
              obj->GetMainName();
          DoGenericAttachedContextMenu(pt, title);
          return;
        }
      if (obj->GetType() == MY_TOPO_GEO)
        {
          CurrentItem = item;
          DoTopoGeoContextMenu(pt, obj->GetMainName());
          return;
        }
      if (obj->GetType() == MY_TOPO_NET)
        {
          CurrentItem = item;
          DoTopoNetContextMenu(pt, obj->GetMainName());
          return;
        }
    }
// special cases: RASTER COVERAGE, VECTOR COVERAGE or WMS LAYER objects
  if (obj->GetType() == MY_RASTER_COVERAGE
      || obj->GetType() == MY_VECTOR_COVERAGE || obj->GetType() == MY_WMS_LAYER)
    {
      if (obj->IsAttached() == true)
        {
          wxString title;
          if (obj->GetType() == MY_RASTER_COVERAGE)
            title +=
              wxT("RASTER COVERAGE: ") + obj->GetDbAlias() + wxT(".") +
              obj->GetMainName();
          else if (obj->GetType() == MY_VECTOR_COVERAGE)
            title +=
              wxT("VECTOR COVERAGE: ") + obj->GetDbAlias() + wxT(".") +
              obj->GetMainName();
          else
            title +=
              wxT("WMS LAYER: ") + obj->GetDbAlias() + wxT(".") +
              obj->GetMainName();
          DoGenericAttachedContextMenu(pt, title);
          return;
        }
      if (obj->GetType() == MY_RASTER_COVERAGE)
        {
          CurrentRasterCoverageName = obj->GetMainName();
          CurrentItem = item;
          DoRasterCoverageContextMenu(pt, obj->GetMainName());
          return;
        }
      if (obj->GetType() == MY_VECTOR_COVERAGE)
        {
          CurrentVectorCoverageName = obj->GetMainName();
          CurrentItem = item;
          DoVectorCoverageContextMenu(pt, obj->GetMainName());
          return;
        }
      if (obj->GetType() == MY_WMS_LAYER)
        {
          CurrentItem = item;
          CurrentWmsLayerURL = obj->GetPathOrURL();
          CurrentWmsLayerName = obj->GetMainName();
          DoWmsLayerContextMenu(pt, obj->GetMainName());
          return;
        }
    }
// TABLE or VIEW objects
  if (obj->IsTable() || obj->IsView())
    {
      if (obj->IsAttached())
        {
          if (obj->IsTable())
            {
              CurrentItem = item;
              DoAttachedTableContextMenu(pt, obj, GetItemImage(CurrentItem));
              return;
            }
          if (obj->IsView())
            {
              CurrentItem = item;
              DoAttachedViewContextMenu(pt, obj, GetItemImage(CurrentItem));
              return;
            }
        }
      if (obj->IsTable())
        {
          CurrentItem = item;
          DoMainTableContextMenu(pt, obj, GetItemImage(CurrentItem));
          return;
        }
      if (obj->IsView())
        {
          CurrentItem = item;
          DoMainViewContextMenu(pt, obj, GetItemImage(CurrentItem));
          return;
        }
    }
// PLAIN COLUMN objects
  if (obj->IsColumn() || obj->IsViewColumn() || obj->IsVirtualColumn()
      || obj->IsGpkgColumn() || obj->IsFdoOgrColumn())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedColumnContextMenu(pt, obj, GetItemImage(CurrentItem));
          return;
        }
      if (obj->IsViewColumn() || obj->IsVirtualColumn())
        {
          CurrentItem = item;
          DoMainLimitedColumnContextMenu(pt, obj, GetItemImage(CurrentItem));
          return;
        }
      if (obj->IsGpkgColumn())
        {
          CurrentItem = item;
          DoMainGpkgColumnContextMenu(pt, obj, GetItemImage(CurrentItem));
          return;
        }
      if (obj->IsFdoOgrColumn())
        {
          CurrentItem = item;
          DoMainFdoOgrColumnContextMenu(pt, obj, GetItemImage(CurrentItem));
          return;
        }
      CurrentItem = item;
      DoMainColumnContextMenu(pt, obj, GetItemImage(CurrentItem), metadata);
      return;
    }
// GEOMETRY COLUMN objects
  if (obj->IsAnyGeometryColumn())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedGeometryColumnContextMenu(pt, obj);
          return;
        }
      if (obj->IsGeometryColumn())
        {
          CurrentItem = item;
          DoMainGeometryColumnContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainLimitedGeometryColumnContextMenu(pt, obj);
      return;
    }
// INDEX objects
  if (obj->IsIndex())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedIndexContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainIndexContextMenu(pt, obj);
      return;
    }
// INDEX COLUMN objects
  if (obj->IsIndexColumn())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedIndexColumnContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainIndexColumnContextMenu(pt, obj);
      return;
    }
// TRIGGER objects
  if (obj->IsTrigger())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedTriggerContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainTriggerContextMenu(pt, obj);
      return;
    }
// PRIMARY KEY objects
  if (obj->IsPrimaryKey())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedPrimaryKeyContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainPrimaryKeyContextMenu(pt, obj);
      return;
    }
// PRIMARY KEY COLUMN objects
  if (obj->IsPrimaryKeyColumn())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedPrimaryKeyColumnContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainPrimaryKeyColumnContextMenu(pt, obj);
      return;
    }
// FOREIGN KEY objects
  if (obj->IsForeignKey())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedForeignKeyContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainForeignKeyContextMenu(pt, obj);
      return;
    }
// FOREIGN KEY COLUMN objects
  if (obj->IsForeignKeyColumn())
    {
      if (obj->IsAttached())
        {
          CurrentItem = item;
          DoAttachedForeignKeyColumnContextMenu(pt, obj);
          return;
        }
      CurrentItem = item;
      DoMainForeignKeyColumnContextMenu(pt, obj);
      return;
    }
}

void MyTableTree::OnCmdQueryViewComposer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - query/view composer invoked
//
  MainFrame->QueryViewComposer();
}

void MyTableTree::OnCmdCloneTable(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - clone table required
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      wxString dbPrefix;
      if (obj->GetDbAlias().Len() == 0)
        dbPrefix = wxT("MAIN");
      else
        dbPrefix = obj->GetDbAlias();
      wxString inTable = obj->GetMainName();
      CloneTableDialog dlg;
      dlg.Create(MainFrame, dbPrefix, inTable);
      int ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          wxString sql;
          dlg.GetSQL(sql);
          MainFrame->GetRsView()->ResetEmpty();
          MainFrame->SetSql(sql, false);
        }
    }
}

void MyTableTree::OnCmdNewTable(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new table creation required
//
  wxString sql;
  sql = wxT("CREATE TABLE ...table-name... (\n");
  sql += wxT("...column1,\n...column2,\n...columnN)");
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, false);
}

void MyTableTree::OnCmdNewView(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new view creation required
//
  wxString sql;
  sql = wxT("CREATE VIEW ...view-name... AS\n");
  sql += wxT("SELECT ...sql-select-statement...");
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, false);
}

void MyTableTree::OnCmdNewIndex(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new index creation required
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      sql = wxT("CREATE [ UNIQUE ] INDEX ...index-name...\nON \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"\n(\n...column1, column2, columnN...\n)");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, false);
    }
}

void MyTableTree::OnCmdNewTrigger(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new trigger creation required
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      sql = wxT("CREATE TRIGGER ...trigger-name...\n[ BEFORE | AFTER ]\n");
      sql += wxT("[ INSERT | UPDATE | DELETE ]\nON \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"\n...sql-statement...");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, false);
    }
}

void MyTableTree::OnCmdNewColumn(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new column creation required
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      sql = wxT("ALTER TABLE \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"\nADD COLUMN ...column-name column-type...");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, false);
    }
}

void MyTableTree::OnCmdSelect(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - examining table rows required
//
  wxString sql;
  wxString dummy;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_POSTGRES_TABLE || obj->GetType() == MY_POSTGRES_VIEW
      || obj->GetType() == MY_POSTGIS_VIEW)
    {
      // special case: PostgreSQL objects
      bool hasGeom = false;
      wxString viewName;
      if (obj->GetType() == MY_POSTGRES_TABLE)
        {
          MyPostgresTable *table =
            MainFrame->FindPostgresTable(obj->GetVirtName());
          if (table->GetFirst() != NULL)
            hasGeom = true;
          viewName = table->GetPostGisName();
        }
      if (hasGeom == true)
        {
          int ret = wxMessageBox(wxT
                                 ("Directly querying a PostgreSQL table containing PostGIS geometries "
                                  "is a strongly discouraged operation.\n\n"
                                  "You should instead query the corresponding Spatial View: ")
                                 + viewName + wxT("\n\n") +
                                 wxT("Do you really intend to continue ?"),
                                 wxT("spatialite_gui"),
                                 wxYES_NO | wxICON_QUESTION, this);
          if (ret != wxYES)
            return;
        }
      sql = wxT("SELECT * FROM \"");
      xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(xname, obj->GetVirtName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"");
      wxString prefix = wxT("MAIN");
      wxString noGeom;
      MainFrame->InitializeSqlFilters(prefix, obj->GetVirtName(), true, noGeom);
      MainFrame->SetSql(sql, true, false, dummy, dummy, true);
      MainFrame->GetQueryView()->EnableFilterButton();
      return;
    }
  sql = wxT("SELECT * FROM \"");
  if (obj->IsAttached() == true)
    {
      wxString dbAlias2 = obj->GetDbAlias();
      MyFrame::DoubleQuoted(dbAlias2);
      sql += dbAlias2 + wxT("\".\"");
    }
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
  wxString noGeom;
  MainFrame->InitializeSqlFilters(obj->GetDbAlias(), obj->GetMainName(), true,
                                  noGeom);
  MainFrame->SetSql(sql, true, false, dummy, dummy, true);
  MainFrame->GetQueryView()->EnableFilterButton();
}

void MyTableTree::OnCmdSelectTiles(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - examining table rows required
//
  wxString tile_data_table;
  wxString tile_data_db_prefix;
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TILE_DATA)
    {
      tile_data_db_prefix = obj->GetDbAlias();
      tile_data_table = obj->GetMainName();
    }
  sql = wxT("SELECT * FROM \"");
  if (obj->IsAttached() == true)
    {
      xname = (char *) malloc((obj->GetDbAlias().Len() * 4) + 1);
      strcpy(xname, obj->GetDbAlias().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2) + wxT("\".\"");
      free(xname2);
    }
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2) + wxT("\"");
  free(xname2);
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true, true, tile_data_db_prefix, tile_data_table,
                    true);
}

void MyTableTree::OnCmdShow(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - examining full infos required
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE || obj->GetType() == MY_VTABLE
      || obj->GetType() == MY_VIEW || obj->GetType() == MY_GPKG_TABLE
      || obj->GetType() == MY_GPKG_VTABLE || obj->GetType() == MY_FDO_TABLE
      || obj->GetType() == MY_FDO_VTABLE)
    {
      if (obj->IsAttached() == true)
        {
          wxString dbAlias2 = obj->GetDbAlias();
          MyFrame::DoubleQuoted(dbAlias2);
          sql = wxT("PRAGMA \"") + dbAlias2 + wxT("\".table_info(\"");
      } else
        sql = wxT("PRAGMA table_info(\"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")");
    }
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED)
    {
      sql = wxT("SELECT *\nFROM geom_cols_ref_sys\n");
      sql += wxT("WHERE Lower(f_table_name) = Lower('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')\nAND Lower(f_geometry_column) = Lower('");
      xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xname, obj->GetColName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
    }
  if (obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED)
    {
      sql = wxT("SELECT *\nFROM views_geometry_columns AS a\n");
      sql += wxT("JOIN geometry_columns AS b ON (");
      sql += wxT("Lower(a.f_table_name) = Lower(b.f_table_name) AND ");
      sql += wxT("Lower(a.f_geometry_column) = Lower(b.f_geometry_column))\n");
      sql += wxT("JOIN spatial_ref_sys USING (srid)\n");
      sql += wxT("WHERE Lower(view_name) = Lower('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')\nAND Lower(view_geometry) = Lower('");
      xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xname, obj->GetColName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
    }
  if (obj->GetType() == MY_VIRTUAL_GEOMETRY)
    {
      sql = wxT("SELECT *\nFROM virts_geometry_columns\n");
      sql += wxT("JOIN spatial_ref_sys USING (srid)\n");
      sql += wxT("WHERE Lower(virt_name) = Lower('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')\nAND Lower(virt_geometry) = Lower('");
      xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xname, obj->GetColName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
    }
  if (obj->GetType() == MY_INDEX)
    {
      sql = wxT("PRAGMA index_info(\"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")");
    }
  if (obj->GetType() == MY_TRIGGER)
    {
      if (obj->IsAttached() == true)
        {
          wxString dbAlias2 = obj->GetDbAlias();
          MyFrame::DoubleQuoted(dbAlias2);
          sql =
            wxT("SELECT sql FROM \"") + dbAlias2 +
            wxT("\".sqlite_master\nWHERE type = 'trigger' AND name = '");
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
          xname2 = gaiaSingleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sql += wxT("'");
      } else
        {
          sql =
            wxT
            ("SELECT sql FROM sqlite_master\nWHERE type = 'trigger' AND name = '");
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
          xname2 = gaiaSingleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sql += wxT("'");
        }
    }
  if (obj->GetType() == MY_POSTGRES_TABLE || obj->GetType() == MY_POSTGRES_VIEW
      || obj->GetType() == MY_POSTGIS_VIEW)
    {
      sql = wxT("PRAGMA table_info(\"");
      xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(xname, obj->GetVirtName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")");
    }
  if (sql.Len() < 1)
    return;
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true);
}

void MyTableTree::OnCmdDrop(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dropping some object required
//
  wxString sql;
  wxString sql2;
  wxString msg;
  char *xsql;
  char *errMsg = NULL;
  int ret;
  wxString name;
  char xprefix[1024];
  char *xname;
  char *xname2;
  char *error_message = NULL;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      msg = wxT("Do you really intend to drop the Table named: ");
      msg += obj->GetMainName();
      msg += wxT("\n[and any other related object] ?");
    }
  if (obj->GetType() == MY_VTABLE)
    {
      msg = wxT("Do you really intend to drop the VirtualTable named: ");
      msg += obj->GetMainName();
      msg += wxT(" ?");
    }
  if (obj->GetType() == MY_VIEW)
    {
      msg = wxT("Do you really intend to drop the View named: ");
      msg += obj->GetMainName();
      msg += wxT(" ?");
    }
  if (obj->GetType() == MY_INDEX)
    {
      msg = wxT("Do you really intend to drop the Index named: ");
      msg += obj->GetMainName();
      msg += wxT(" ?");
      sql = wxT("DROP INDEX IF EXISTS ");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
    }
  if (obj->GetType() == MY_TRIGGER)
    {
      msg = wxT("Do you really intend to drop the Trigger named: ");
      msg += obj->GetMainName();
      msg += wxT(" ?");
      sql = wxT("DROP TRIGGER IF EXISTS \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2) + wxT("\"");
      free(xname2);
    }
  wxMessageDialog confirm(this, msg, wxT("Confirming DROP"),
                          wxOK | wxCANCEL | wxICON_QUESTION);
  ret = confirm.ShowModal();
  if (ret != wxID_OK)
    return;
  ::wxBeginBusyCursor();
  if (obj->GetType() == MY_TABLE)
    {
      strcpy(xprefix, obj->GetDbAlias().ToUTF8());
      if (obj->IsTemporary())
        strcpy(xprefix, "temp");
      if (*xprefix == '\0')
        strcpy(xprefix, "main");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      ret = gaiaDropTableEx3(sqlite, xprefix, xname, 1, &error_message);
      free(xname);
      if (ret)
        goto done;
      else
        goto err;
    }
  if (obj->GetType() == MY_VTABLE)
    {
      strcpy(xprefix, obj->GetDbAlias().ToUTF8());
      if (obj->IsTemporary())
        strcpy(xprefix, "temp");
      if (*xprefix == '\0')
        strcpy(xprefix, "main");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      ret = gaiaDropTableEx3(sqlite, xprefix, xname, 1, &error_message);
      free(xname);
      if (ret)
        goto done;
      else
        goto err;
    }
  if (obj->GetType() == MY_VIEW)
    {
      strcpy(xprefix, obj->GetDbAlias().ToUTF8());
      if (obj->IsTemporary())
        strcpy(xprefix, "temp");
      if (*xprefix == '\0')
        strcpy(xprefix, "main");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      ret = gaiaDropTableEx3(sqlite, xprefix, xname, 1, &error_message);
      free(xname);
      if (ret)
        goto done;
      else
        goto err;
    }
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      goto rollback;
    }
done:
  ::wxEndBusyCursor();
  wxMessageBox(wxT("Selected object '") + obj->GetMainName() +
               wxT("' was successfully removed"), wxT("spatialite_gui"),
               wxOK | wxICON_INFORMATION, this);
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
  return;
err:
  ::wxEndBusyCursor();
  if (error_message != NULL)
    {
      wxString cause = wxString::FromUTF8(error_message);
      wxMessageBox(wxT("Unable to remove this object: '") + obj->GetMainName() +
                   wxT("'\n\n") + cause, wxT("spatialite_gui"),
                   wxOK | wxICON_WARNING, this);
      sqlite3_free(error_message);
  } else
    wxMessageBox(wxT("Unable to remove this object: '") + obj->GetMainName() +
                 wxT("'"), wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
  return;
rollback:
  ret = sqlite3_exec(sqlite, "ROLLBACK", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT
               ("An error occurred\n\na ROLLBACK was automatically performed"),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdRename(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - table renaming required
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE || obj->GetType() == MY_VTABLE)
    {
      sql = wxT("ALTER TABLE \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"\nRENAME TO ...new-table-name...");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, false);
    }
}

void MyTableTree::OnCmdDetachDB(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - DETACH DATABASE
//
  wxString sql;
  char *xsql;
  int ret;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_ATTACHED)
    {
      wxString msg = wxT("Do you really intend to detach this database ?\n\n");
      msg += obj->GetMainName();
      msg += wxT(" AS ");
      msg += obj->GetDbAlias();
      wxMessageDialog confirm(this, msg, wxT("Confirming DETACH DATABASE"),
                              wxOK | wxCANCEL | wxICON_QUESTION);
      ret = confirm.ShowModal();
      if (ret != wxID_OK)
        return;

      char *errMsg = NULL;
      sqlite3 *sqlite = MainFrame->GetSqlite();
      MainFrame->AutoFDOStop(obj->GetDbAlias());
      if (MainFrame->GetAutoFDOmsg().Len() > 0)
        {
          wxMessageBox(MainFrame->GetAutoFDOmsg(), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          MainFrame->AutoFDOmsgReset();
        }
      MainFrame->AutoGPKGStop(obj->GetDbAlias());
      if (MainFrame->GetAutoGPKGmsg().Len() > 0)
        {
          wxMessageBox(MainFrame->GetAutoGPKGmsg(), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          MainFrame->AutoGPKGmsgReset();
        }
      wxString sql = wxT("DETACH DATABASE ");
      wxString dbAlias2 = obj->GetDbAlias();
      MyFrame::DoubleQuoted(dbAlias2);
      sql += wxT("\"") + dbAlias2 + wxT("\"");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") +
                       wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      MainFrame->RemoveAttachedDB(obj->GetDbAlias());
      // appending a delayed event so to really update the Tree
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
    }
}

void MyTableTree::OnCmdRefresh(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - refreshing the Tree (via deferrend event)
//
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  AddPendingEvent(evt);
}

void MyTableTree::OnRefreshDeferred(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - refreshing the Tree
//
// safe deferred event processing so to avoid any attempt from wxWidgets
// itself to continue using invalid Tree Items (already being destroied)
//
  MainFrame->InitTableTree();
}

void MyTableTree::OnCmdRecover(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Recover Geometry
//
  char *errMsg = NULL;
  int ret;
  wxString sql;
  char *xsql;
  int i;
  char **results;
  int rows;
  int columns;
  int retval = 0;
  int srid = -1;
  char dummy[128];
  wxString type;
  wxString dims;
  RecoverDialog dlg;
  char *xname;
  char *xname2;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_COLUMN)
    {
      // trying to recover a geometry column
      if (MainFrame->CheckMetadata() == false)
        {
          wxMessageBox(wxT
                       ("Missing Spatial Metadata tables\n\ntry to run the 'init_spatialite.sql' script ..."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
        }
      dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName());
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          srid = dlg.GetSRID();
          type = dlg.GetType();
          dims = dlg.GetDimension();
          ::wxBeginBusyCursor();
          ret = sqlite3_exec(sqlite, "BEGIN", NULL, NULL, &errMsg);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              ::wxEndBusyCursor();
              return;
            }
          sql = wxT("SELECT RecoverGeometryColumn('");
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
          xname2 = gaiaSingleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sql += wxT("', '");
          xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
          strcpy(xname, obj->GetColName().ToUTF8());
          xname2 = gaiaSingleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sprintf(dummy, "', %d, '", srid);
          sql += wxString::FromUTF8(dummy);
          sql += type;
          sql += wxT("', '");
          sql += dims;
          sql += wxT("')");
          xsql = (char *) malloc((sql.Len() * 4) + 1);
          strcpy(xsql, sql.ToUTF8());
          ret =
            sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
          free(xsql);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              goto rollback;
            }
          if (rows < 1)
            ;
          else
            {
              for (i = 1; i <= rows; i++)
                {
                  if (results[(i * columns) + 0])
                    retval = atoi(results[(i * columns) + 0]);
                }
            }
          sqlite3_free_table(results);
          if (!retval)
            goto rollback;
          ret = sqlite3_exec(sqlite, "COMMIT", NULL, NULL, &errMsg);
          if (ret != SQLITE_OK)
            {
              wxMessageBox(wxT("SQLite SQL error: ") +
                           wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(errMsg);
              ::wxEndBusyCursor();
              return;
            }
          ::wxEndBusyCursor();
          wxMessageBox(wxT("Geometry column ") + obj->GetMainName() +
                       wxT(".") + obj->GetColName() +
                       wxT(" was successfully recovered"),
                       wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
        }
    }
  return;
rollback:
  ret = sqlite3_exec(sqlite, "ROLLBACK", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT
               ("Geometry column doesn't satisfies required constraints\n\na ROLLBACK was automatically performed"),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdShowSql(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Showing CREATE TABLE statement
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE || obj->GetType() == MY_VTABLE
      || obj->GetType() == MY_GPKG_TABLE || obj->GetType() == MY_GPKG_VTABLE
      || obj->GetType() == MY_FDO_TABLE || obj->GetType() == MY_FDO_VTABLE)
    {
      sql = wxT("SELECT sql FROM ");
      if (obj->IsAttached() == true)
        {
          wxString dbAlias2 = obj->GetDbAlias();
          MyFrame::DoubleQuoted(dbAlias2);
          sql += wxT("\"") + dbAlias2 + wxT("\".");
        }
      if (obj->IsTemporary() == true)
        sql += wxT("sqlite_temp_master");
      else
        sql += wxT("sqlite_master");
      sql += wxT("\nWHERE type = 'table' AND name = '");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("'");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
  if (obj->GetType() == MY_VIEW)
    {
      sql = wxT("SELECT sql FROM ");
      if (obj->IsAttached() == true)
        {
          wxString dbAlias2 = obj->GetDbAlias();
          MyFrame::DoubleQuoted(dbAlias2);
          sql += wxT("\"") + dbAlias2 + wxT("\".");
        }
      if (obj->IsTemporary() == true)
        sql += wxT("sqlite_temp_master");
      else
        sql += wxT("sqlite_master");
      sql += wxT("\nWHERE type = 'view' AND name = '");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("'");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
  if (obj->GetType() == MY_POSTGRES_TABLE || obj->GetType() == MY_POSTGRES_VIEW)
    {
      sql = wxT("SELECT sql FROM ");
      sql += wxT("sqlite_master");
      sql += wxT("\nWHERE type = 'table' AND name = '");
      xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(xname, obj->GetVirtName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("'");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
  if (obj->GetType() == MY_POSTGIS_VIEW)
    {
      sql = wxT("SELECT sql FROM ");
      sql += wxT("sqlite_master");
      sql += wxT("\nWHERE type = 'view' AND name = '");
      xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(xname, obj->GetVirtName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("'");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
}

void MyTableTree::OnCmdCheckSpatialIndex(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Check Spatial Index for validity 
//
  char *errMsg = NULL;
  int ret;
  wxString sql;
  char *xsql;
  wxString msg;
  int i;
  char **results;
  int rows;
  int columns;
  int retval = 0;
  char *xtable;
  char *xcolumn;
  char *xtable2;
  char *xcolumn2;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj->GetType() == MY_GEOMETRY_INDEX)
    {
      // checking the Spatial Index
      ::wxBeginBusyCursor();
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT CheckSpatialIndex('");
      sql += wxString::FromUTF8(xtable2);
      free(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xcolumn2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
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
              if (results[(i * columns) + 0])
                retval = atoi(results[(i * columns) + 0]);
            }
        }
      sqlite3_free_table(results);
      ::wxEndBusyCursor();
      if (retval < 0)
        wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                     wxT("_") + obj->GetColName() +
                     wxT(" is badly damaged.\n\n") +
                     wxT
                     ("a physical column named \"rowid\" exists shadowing the real ROWID"),
                     wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      else if (retval)
        wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                     wxT("_") + obj->GetColName() +
                     wxT(" is valid and consistent"), wxT("spatialite_gui"),
                     wxOK | wxICON_INFORMATION, this);
      else
        wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                     wxT("_") + obj->GetColName() +
                     wxT(" is invalid and inconsistent\n") +
                     wxT
                     ("Please, recover this Spatial Index as soon as possible"),
                     wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
    }
}

void MyTableTree::OnCmdRecoverSpatialIndex(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Recover Spatial Index (rebuilding from scratch) 
//
  char *errMsg = NULL;
  int ret;
  wxString sql;
  char *xsql;
  wxString msg;
  int i;
  char **results;
  int rows;
  int columns;
  int retval = 0;
  char *xtable;
  char *xcolumn;
  char *xtable2;
  char *xcolumn2;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj->GetType() == MY_GEOMETRY_INDEX)
    {
      // recovering the Spatial Index
      ::wxBeginBusyCursor();
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT RecoverSpatialIndex('");
      sql += wxString::FromUTF8(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xtable2);
      free(xcolumn2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
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
              if (results[(i * columns) + 0])
                retval = atoi(results[(i * columns) + 0]);
            }
        }
      sqlite3_free_table(results);
      ::wxEndBusyCursor();
      if (retval < 0)
        wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                     wxT("_") + obj->GetColName() +
                     wxT(" is badly damaged.\n\n") +
                     wxT
                     ("a physical column named \"rowid\" exists shadowing the real ROWID"),
                     wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      else if (retval)
        wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                     wxT("_") + obj->GetColName() +
                     wxT(" was successfully recovered"), wxT("spatialite_gui"),
                     wxOK | wxICON_INFORMATION, this);
      else
        wxMessageBox(wxT("ERROR: unable to recover Spatial Index idx_") +
                     obj->GetMainName() + wxT("_") + obj->GetColName(),
                     wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
    }
}

void MyTableTree::OnCmdSpatialIndex(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Spatial Index creation-destruction
//
  char *errMsg = NULL;
  int ret;
  wxString sql;
  char *xsql;
  wxString msg;
  int i;
  char **results;
  int rows;
  int columns;
  int retval = 0;
  wxString name;
  char *xname;
  char *xname2;
  char *xtable;
  char *xcolumn;
  char *xtable2;
  char *xcolumn2;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY)
    {
      // creating the Spatial Index
      ::wxBeginBusyCursor();
      ret = sqlite3_exec(sqlite, "BEGIN", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT CreateSpatialIndex('");
      sql += wxString::FromUTF8(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xtable2);
      free(xcolumn2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          goto rollback;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              if (results[(i * columns) + 0])
                retval = atoi(results[(i * columns) + 0]);
            }
        }
      sqlite3_free_table(results);
      if (!retval)
        goto rollback;
      ret = sqlite3_exec(sqlite, "COMMIT", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      ::wxEndBusyCursor();
      wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                   wxT("_") + obj->GetColName() +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
// appending a delayed event so to really update the Tree
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
  } else if (obj->GetType() == MY_GEOMETRY_INDEX)
    {
      // dropping the Spatial Index
      ::wxBeginBusyCursor();
      ret = sqlite3_exec(sqlite, "BEGIN", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT DisableSpatialIndex('");
      sql += wxString::FromUTF8(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xtable2);
      free(xcolumn2);
      sql += wxT("')");
      msg = wxT("Do you really intend to delete the SpatialIndex\n");
      msg += wxT("on column ");
      msg += obj->GetMainName();
      msg += wxT(".");
      msg += obj->GetColName();
      msg += wxT(" ?");
      wxMessageDialog confirm(this, msg, wxT("Confirming Delete Spatial Index"),
                              wxOK | wxCANCEL | wxICON_QUESTION);
      ret = confirm.ShowModal();
      if (ret != wxID_OK)
        return;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          goto rollback;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              if (results[(i * columns) + 0])
                retval = atoi(results[(i * columns) + 0]);
            }
        }
      sqlite3_free_table(results);
      if (!retval)
        goto rollback;
      sql = wxT("DROP TABLE IF EXISTS ");
      name = wxT("idx_");
      name += obj->GetMainName();
      name += wxT("_");
      name += obj->GetColName();
      xname = (char *) malloc((name.Len() * 4) + 1);
      strcpy(xname, name.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
      free(xname2);
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      ret = sqlite3_exec(sqlite, "COMMIT", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      ::wxEndBusyCursor();
      wxMessageBox(wxT("Spatial Index idx_") + obj->GetMainName() +
                   wxT("_") + obj->GetColName() +
                   wxT(" was successfully removed"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      // appending a delayed event so to really update the Tree
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
    }
  return;
rollback:
  ret = sqlite3_exec(sqlite, "ROLLBACK", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT
               ("An error occurred\n\na ROLLBACK was automatically performed"),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdMbrCache(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - MBR cache creation-destruction
//
  char *errMsg = NULL;
  int ret;
  wxString sql;
  char *xsql;
  wxString msg;
  int i;
  char **results;
  int rows;
  int columns;
  int retval = 0;
  wxString name;
  char *xname;
  char *xtable;
  char *xcolumn;
  char *xname2;
  char *xtable2;
  char *xcolumn2;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY)
    {
      // creating the MBR cache
      ::wxBeginBusyCursor();
      ret = sqlite3_exec(sqlite, "BEGIN", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT CreateMbrCache('");
      sql += wxString::FromUTF8(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xtable2);
      free(xcolumn2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          goto rollback;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              if (results[(i * columns) + 0])
                retval = atoi(results[(i * columns) + 0]);
            }
        }
      sqlite3_free_table(results);
      if (!retval)
        goto rollback;
      ret = sqlite3_exec(sqlite, "COMMIT", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      ::wxEndBusyCursor();
      wxMessageBox(wxT("MBR cache cache_") + obj->GetMainName() +
                   wxT("_") + obj->GetColName() +
                   wxT(" was successfully created"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      // appending a delayed event so to really update the Tree
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
  } else if (obj->GetType() == MY_GEOMETRY_CACHED)
    {
      // dropping the MBR cache
      ::wxBeginBusyCursor();
      ret = sqlite3_exec(sqlite, "BEGIN", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT DisableSpatialIndex('");
      sql += wxString::FromUTF8(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xtable2);
      free(xcolumn2);
      sql += wxT("')");
      msg = wxT("Do you really intend to delete the MBR cache\n");
      msg += wxT("on column ");
      msg += obj->GetMainName();
      msg += wxT(".");
      msg += obj->GetColName();
      msg += wxT(" ?");
      wxMessageDialog confirm(this, msg, wxT("Confirming Delete MBR cache"),
                              wxOK | wxCANCEL | wxICON_QUESTION);
      ret = confirm.ShowModal();
      if (ret != wxID_OK)
        return;
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(sqlite, xsql, &results, &rows, &columns, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          goto rollback;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              if (results[(i * columns) + 0])
                retval = atoi(results[(i * columns) + 0]);
            }
        }
      sqlite3_free_table(results);
      if (!retval)
        goto rollback;
      sql = wxT("DROP TABLE IF EXISTS ");
      name = wxT("cache_");
      name += obj->GetMainName();
      name += wxT("_");
      name += obj->GetColName();
      xname = (char *) malloc((name.Len() * 4) + 1);
      strcpy(xname, name.ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_exec(sqlite, xsql, NULL, NULL, &errMsg);
      free(xsql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      ret = sqlite3_exec(sqlite, "COMMIT", NULL, NULL, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      ::wxEndBusyCursor();
      wxMessageBox(wxT("MBR cache cache_") + obj->GetMainName() +
                   wxT("_") + obj->GetColName() +
                   wxT(" was successfully removed"), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
      // appending a delayed event so to really update the Tree
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
    }
  return;
rollback:
  ret = sqlite3_exec(sqlite, "ROLLBACK", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT
               ("An error occurred\n\na ROLLBACK was automatically performed"),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdRebuildTriggers(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - rebuilding Geometry Triggers
//
  wxString sql;
  char *xtable;
  char *xcolumn;
  char *xtable2;
  char *xcolumn2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_COLUMN || obj->GetType() == MY_GEOMETRY
      || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED)
    {
      xtable = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xtable, obj->GetMainName().ToUTF8());
      xtable2 = gaiaSingleQuotedSql(xtable);
      free(xtable);
      xcolumn = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xcolumn, obj->GetColName().ToUTF8());
      xcolumn2 = gaiaSingleQuotedSql(xcolumn);
      free(xcolumn);
      sql = wxT("SELECT RebuildGeometryTriggers('");
      sql += wxString::FromUTF8(xtable2);
      sql += wxT("', '");
      sql += wxString::FromUTF8(xcolumn2);
      free(xtable2);
      free(xcolumn2);
      sql += wxT("')");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
}

void MyTableTree::OnCmdCheckGeometry(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - checking geometries
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_COLUMN || obj->GetType() == MY_GEOMETRY
      || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED
      || obj->GetType() == MY_POSTGIS_GEOMETRY)
    {
      sql = wxT("SELECT Count(*), GeometryType(\"");
      if (obj->GetType() == MY_POSTGIS_GEOMETRY)
        {
          xname = (char *) malloc((obj->GetColumn().Len() * 4) + 1);
          strcpy(xname, obj->GetColumn().ToUTF8());
      } else
        {
          xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
          strcpy(xname, obj->GetColName().ToUTF8());
        }
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      sql += wxT("\"), Srid(\"");
      sql += wxString::FromUTF8(xname2);
      sql += wxT("\"), CoordDimension(\"");
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")\nFROM \"");
      if (obj->GetType() == MY_POSTGIS_GEOMETRY)
        {
          xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
          strcpy(xname, obj->GetVirtName().ToUTF8());
      } else
        {
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
        }
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"\nGROUP BY 2, 3, 4");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
}

void MyTableTree::OnCmdExtent(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - computing Extent
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_COLUMN || obj->GetType() == MY_GEOMETRY
      || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED
      || obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED
      || obj->GetType() == MY_VIRTUAL_COLUMN
      || obj->GetType() == MY_VIRTUAL_GEOMETRY
      || obj->GetType() == MY_POSTGIS_GEOMETRY)
    {
      sql = wxT("SELECT Min(MbrMinX(\"");
      if (obj->GetType() == MY_POSTGIS_GEOMETRY)
        {
          xname = (char *) malloc((obj->GetColumn().Len() * 4) + 1);
          strcpy(xname, obj->GetColumn().ToUTF8());
      } else
        {
          xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
          strcpy(xname, obj->GetColName().ToUTF8());
        }
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      sql += wxT("\")), Min(MbrMinY(\"");
      sql += wxString::FromUTF8(xname2);
      sql += wxT("\")), Max(MbrMaxX(\"");
      sql += wxString::FromUTF8(xname2);
      sql += wxT("\")), Max(MbrMaxY(\"");
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"))\nFROM \"");
      if (obj->GetType() == MY_POSTGIS_GEOMETRY)
        {
          xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
          strcpy(xname, obj->GetVirtName().ToUTF8());
      } else
        {
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
        }
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2) + wxT("\"");
      free(xname2);
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
}

void MyTableTree::OnCmdUpdateLayerStatistics(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Updating Layer Statistics
//
  wxString sql;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_COLUMN || obj->GetType() == MY_GEOMETRY
      || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED
      || obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED
      || obj->GetType() == MY_VIRTUAL_COLUMN
      || obj->GetType() == MY_VIRTUAL_GEOMETRY)
    {
      sql = wxT("SELECT UpdateLayerStatistics('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("', '");
      xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(xname, obj->GetColName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
  if (obj->GetType() == MY_VTABLE || obj->GetType() == MY_TABLE
      || obj->GetType() == MY_VIEW)
    {
      sql = wxT("SELECT UpdateLayerStatistics('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
}

void MyTableTree::
OnCmdUpdateLayerStatisticsAll(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Updating Layer Statistics [ALL]
//
  wxString sql = wxT("SELECT UpdateLayerStatistics()");
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true);
}

void MyTableTree::OnCmdAddAllRastersSrid(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - conditionally adding some SRID to all Rasters
//
  AddAllRastersSridDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdUpdateRasterExtent(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Updating Raster Coverage Extent
//
  wxString coverage = CurrentRasterCoverageName;
  char *xcov = new char[strlen(coverage.ToUTF8()) + 1];
  strcpy(xcov, coverage.ToUTF8());
  char *xsql =
    sqlite3_mprintf("SELECT SE_UpdateRasterCoverageExtent(%Q, 1)", xcov);
  delete[]xcov;
  wxString sql = wxString::FromUTF8(xsql);
  sqlite3_free(xsql);
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true);
}

void MyTableTree::OnCmdUpdateRasterExtentAll(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Updating Raster Coverage Extent [ALL]
//
  wxString sql = wxT("SELECT SE_UpdateRasterCoverageExtent(1)");
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true);
}

void MyTableTree::OnCmdUpdateVectorExtent(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Updating Vector Coverage Extent
//
  wxString coverage = CurrentVectorCoverageName;
  char *xcov = new char[strlen(coverage.ToUTF8()) + 1];
  strcpy(xcov, coverage.ToUTF8());
  char *xsql =
    sqlite3_mprintf("SELECT SE_UpdateVectorCoverageExtent(%Q, 1)", xcov);
  delete[]xcov;
  wxString sql = wxString::FromUTF8(xsql);
  sqlite3_free(xsql);
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true);
}

void MyTableTree::OnCmdAddAllVectorsSrid(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - conditionally adding some SRID to all Vectors
//
  AddAllVectorsSridDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdUpdateVectorExtentAll(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Updating Vector Coverage Extent [ALL]
//
  wxString sql = wxT("SELECT SE_UpdateVectorCoverageExtent(1)");
  MainFrame->GetRsView()->ResetEmpty();
  MainFrame->SetSql(sql, true);
}

void MyTableTree::OnCmdElementaryGeometries(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - creating a derived table (elementary geometries)
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED
      || obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED
      || obj->GetType() == MY_VIRTUAL_GEOMETRY)
    {
      ElementaryGeomsDialog dlg;
      dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName());
      if (dlg.ShowModal() == wxID_OK)
        {
          wxString outTable = dlg.GetOutTable();
          wxString pKey = dlg.GetPrimaryKey();
          wxString multiID = dlg.GetMultiID();
          wxString type = dlg.GetType();
          int srid = dlg.GetSRID();
          wxString coordDims = dlg.GetCoordDims();
          bool spIdx = dlg.IsSpatialIndex();
          ::wxBeginBusyCursor();
          bool ret = MainFrame->DoElementaryGeometries(obj->GetMainName(),
                                                       obj->GetColName(),
                                                       outTable, pKey, multiID,
                                                       type,
                                                       srid, coordDims, spIdx);
          ::wxEndBusyCursor();
          if (ret)
            {
              // appending a delayed event so to really update the Tree
              wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED,
                                 Tree_RefreshDeferred);
              AddPendingEvent(evt);
            }
        }
    }
}

void MyTableTree::OnCmdMalformedGeometries(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - identifying malformed geometries
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED
      || obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED
      || obj->GetType() == MY_VIRTUAL_GEOMETRY
      || obj->GetType() == MY_VIRTUAL_GPKG_GEOMETRY)
    {
      bool repair = true;
      if (obj->GetType() == MY_VIEW_GEOMETRY
          || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
          || obj->GetType() == MY_VIEW_GEOMETRY_CACHED
          || obj->GetType() == MY_VIRTUAL_GEOMETRY)
        repair = false;
      MalformedGeomsList *list =
        MainFrame->FindMalformedGeoms(obj->GetMainName(), obj->GetColName(),
                                      repair);
      if (list->GetFirst() == NULL)
        {
          delete list;
          wxString msg = wxT("Any geometry found in ");
          msg += obj->GetMainName();
          msg += wxT(".");
          msg += obj->GetColName();
          msg += wxT(" is valid");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                       this);
          return;
      } else
        {
          MalformedGeomsDialog dlg;
          dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName(), list);
          dlg.ShowModal();
        }
    }
}

void MyTableTree::OnCmdRepairPolygons(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - attempting to repair malformed polygons
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED)
    {
      int count;
      ::wxBeginBusyCursor();
      MainFrame->PreRepairPolygons(obj->GetMainName(), obj->GetColName(),
                                   &count);
      if (count == 0)
        {
          wxString msg = wxT("No Polygon to be repaired found in ");
          msg += obj->GetMainName();
          msg += wxT(".");
          msg += obj->GetColName();
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION,
                       this);
          ::wxEndBusyCursor();
          return;
        }
      MainFrame->RepairPolygons(obj->GetMainName(), obj->GetColName(), &count);
      if (count > 0)
        {
          char str[256];
          sprintf(str, "%d Geometries were successfully updated", count);
          wxMessageBox(wxString::FromUTF8(str), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          ::wxEndBusyCursor();
          return;
        }
      ::wxEndBusyCursor();
    }
}

void MyTableTree::OnCmdSetSrid(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - setting SRID for geometries
//
  SetSridDialog dlg;
  wxString sql;
  int srid;
  int oldSrid;
  int ret;
  char dummy[128];
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_COLUMN)
    {
      dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName());
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          srid = dlg.GetSRID();
          oldSrid = dlg.GetOldSRID();
          sql = wxT("UPDATE \"");
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sql += wxT("\" SET \"");
          xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
          strcpy(xname, obj->GetColName().ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sql += wxT("\" = SetSrid(\"");
          xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
          strcpy(xname, obj->GetColName().ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sprintf(dummy, "\", %d)", srid);
          sql += wxString::FromUTF8(dummy);
          sql += wxT("\nWHERE Srid(\"");
          xname = (char *) malloc((obj->GetColName().Len() * 4) + 1);
          strcpy(xname, obj->GetColName().ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          free(xname2);
          sprintf(dummy, "\") = %d", oldSrid);
          sql += wxString::FromUTF8(dummy);
          MainFrame->GetRsView()->ResetEmpty();
          MainFrame->SetSql(sql, true);
        }
    }
}

void MyTableTree::OnCmdDumpShp(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as Shapefile
//
  int ret;
  wxString path;
  wxString lastDir;
  bool isView = false;
  bool isPostGIS = false;
  int metadata_type = MainFrame->GetMetaDataType();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_POSTGIS_GEOMETRY)
    isPostGIS = true;
  if (obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED)
    isView = true;
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED || isView == true || isPostGIS)
    {
      wxFileDialog fileDialog(this, wxT("Dump Shapefile"),
                              wxT(""), wxT("shapefile.shp"),
                              wxT
                              ("Shapefile (*.shp)|*.shp|All files (*.*)|*.*"),
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                              wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
      lastDir = MainFrame->GetLastDirectory();
      if (lastDir.Len() >= 1)
        fileDialog.SetDirectory(lastDir);
      ret = fileDialog.ShowModal();
      if (ret == wxID_OK)
        {
          char x_path[1024];
          char x_table[1024];
          char x_column[1024];
          char x_type[1024];
          char x_charset[1024];
          char err_msg[1024];
          int rt;
          char **results;
          int rows;
          int columns;
          int i;
          char *errMsg = NULL;
          *x_type = '\0';
          wxFileName file(fileDialog.GetPath());
          path = file.GetPath();
          path += file.GetPathSeparator();
          path += file.GetName();
          lastDir = file.GetPath();
          strcpy(x_path, path.ToUTF8());
          if (isPostGIS == true)
            {
              strcpy(x_table, obj->GetVirtName().ToUTF8());
              strcpy(x_column, obj->GetColumn().ToUTF8());
          } else
            {
              strcpy(x_table, obj->GetMainName().ToUTF8());
              strcpy(x_column, obj->GetColName().ToUTF8());
            }
          if (isPostGIS == true)
            {
              char *geom = (char *) malloc(obj->GetColumn().Len() * 4 + 1);
              strcpy(geom, obj->GetColumn().ToUTF8());
              char *xgeom = gaiaDoubleQuotedSql(geom);
              free(geom);
              char *view = (char *) malloc(obj->GetVirtName().Len() * 4 + 1);
              strcpy(view, obj->GetVirtName().ToUTF8());
              char *xview = gaiaDoubleQuotedSql(view);
              free(view);
              char *sql = sqlite3_mprintf("SELECT GeometryType(\"%s\") "
                                          "FROM \"%s\" GROUP BY 1", xgeom,
                                          xview);
              free(xgeom);
              free(xview);
              ret =
                sqlite3_get_table(MainFrame->GetSqlite(), sql,
                                  &results, &rows, &columns, &errMsg);
              sqlite3_free(sql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("dump shapefile error:") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                  return;
                }
              if (rows < 1)
                ;
              else
                {
                  for (i = 1; i <= rows; i++)
                    strcpy(x_type, results[(i * columns) + 0]);
                }
              sqlite3_free_table(results);
              if (strcmp(x_type, "MULTILINESTRING") == 0)
                strcpy(x_type, "LINESTRING");
              if (strcmp(x_type, "MULTIPOLYGON") == 0)
                strcpy(x_type, "POLYGON");
          } else if (isView == true)
            {
              wxString sql;
              char *xsql;
              if (metadata_type == METADATA_CURRENT)
                sql =
                  wxT("SELECT geometry_type FROM views_geometry_columns AS a ");
              else if (metadata_type == METADATA_LEGACY)
                sql = wxT("SELECT type FROM views_geometry_columns AS a ");
              sql += wxT("JOIN geometry_columns AS b ON (");
              sql += wxT("Lower(a.f_table_name) = Lower(b.f_table_name) AND ");
              sql +=
                wxT
                ("Lower(a.f_geometry_column) = Lower(b.f_geometry_column)) ");
              sql += wxT("WHERE Lower(view_name) = Lower('");
              sql += obj->GetMainName();
              sql += wxT("') AND Lower(view_geometry) = Lower('");
              sql += obj->GetColName();
              sql += wxT("')");
              xsql = (char *) malloc((sql.Len() * 4) + 1);
              strcpy(xsql, sql.ToUTF8());
              ret =
                sqlite3_get_table(MainFrame->GetSqlite(), xsql,
                                  &results, &rows, &columns, &errMsg);
              free(xsql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("dump shapefile error:") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                  return;
                }
              if (rows < 1)
                ;
              else
                {
                  for (i = 1; i <= rows; i++)
                    {
                      if (metadata_type == METADATA_LEGACY)
                        strcpy(x_type, results[(i * columns) + 0]);
                      else if (metadata_type == METADATA_CURRENT)
                        {
                          switch (atoi(results[(i * columns) + 0]))
                            {
                              case 0:
                              case 1000:
                              case 2000:
                              case 3000:
                                strcpy(x_type, "GEOMETRY");
                                break;
                              case 1:
                              case 1001:
                              case 2001:
                              case 3001:
                                strcpy(x_type, "POINT");
                                break;
                              case 2:
                              case 1002:
                              case 2002:
                              case 3002:
                                strcpy(x_type, "LINESTRING");
                                break;
                              case 3:
                              case 1003:
                              case 2003:
                              case 3003:
                                strcpy(x_type, "POLYGON");
                                break;
                              case 4:
                              case 1004:
                              case 2004:
                              case 3004:
                                strcpy(x_type, "MULTIPOINT");
                                break;
                              case 5:
                              case 1005:
                              case 2005:
                              case 3005:
                                strcpy(x_type, "MULTILINESTRING");
                                break;
                              case 6:
                              case 1006:
                              case 2006:
                              case 3006:
                                strcpy(x_type, "MULTIPOLYGON");
                                break;
                              case 7:
                              case 1007:
                              case 2007:
                              case 3007:
                                strcpy(x_type, "GEOMETRYCOLLECTION");
                                break;
                            };
                        }
                    }
                }
              sqlite3_free_table(results);
          } else
            {
              wxString sql;
              char *xsql;
              if (metadata_type == METADATA_CURRENT)
                sql =
                  wxT
                  ("SELECT geometry_type FROM geometry_columns WHERE Lower(f_table_name) = Lower('");
              else if (metadata_type == METADATA_LEGACY)
                sql =
                  wxT
                  ("SELECT type FROM geometry_columns WHERE Lower(f_table_name) = Lower('");
              sql += obj->GetMainName();
              sql += wxT("') AND Lower(f_geometry_column) = Lower('");
              sql += obj->GetColName();
              sql += wxT("')");
              xsql = (char *) malloc((sql.Len() * 4) + 1);
              strcpy(xsql, sql.ToUTF8());
              ret =
                sqlite3_get_table(MainFrame->GetSqlite(), xsql,
                                  &results, &rows, &columns, &errMsg);
              free(xsql);
              if (ret != SQLITE_OK)
                {
                  wxMessageBox(wxT("dump shapefile error:") +
                               wxString::FromUTF8(errMsg),
                               wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                               this);
                  sqlite3_free(errMsg);
                  return;
                }
              if (rows < 1)
                ;
              else
                {
                  for (i = 1; i <= rows; i++)
                    {
                      if (metadata_type == METADATA_LEGACY)
                        strcpy(x_type, results[(i * columns) + 0]);
                      else if (metadata_type == METADATA_CURRENT)
                        {
                          switch (atoi(results[(i * columns) + 0]))
                            {
                              case 0:
                              case 1000:
                              case 2000:
                              case 3000:
                                strcpy(x_type, "GEOMETRY");
                                break;
                              case 1:
                              case 1001:
                              case 2001:
                              case 3001:
                                strcpy(x_type, "POINT");
                                break;
                              case 2:
                              case 1002:
                              case 2002:
                              case 3002:
                                strcpy(x_type, "LINESTRING");
                                break;
                              case 3:
                              case 1003:
                              case 2003:
                              case 3003:
                                strcpy(x_type, "POLYGON");
                                break;
                              case 4:
                              case 1004:
                              case 2004:
                              case 3004:
                                strcpy(x_type, "MULTIPOINT");
                                break;
                              case 5:
                              case 1005:
                              case 2005:
                              case 3005:
                                strcpy(x_type, "MULTILINESTRING");
                                break;
                              case 6:
                              case 1006:
                              case 2006:
                              case 3006:
                                strcpy(x_type, "MULTIPOLYGON");
                                break;
                              case 7:
                              case 1007:
                              case 2007:
                              case 3007:
                                strcpy(x_type, "GEOMETRYCOLLECTION");
                                break;
                            };
                        }
                    }
                }
              sqlite3_free_table(results);
            }
          if (MainFrame->IsSetAskCharset() == false)
            {
              // using the default output charset
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              strcpy(x_charset, MainFrame->GetDefaultCharset().ToUTF8());
              rt =
                dump_shapefile(MainFrame->GetSqlite(), x_table, x_column,
                               x_path, x_charset, x_type, 0, &rows, err_msg);
              ::wxEndBusyCursor();
              if (rt)
                wxMessageBox(wxT("dump shp:") +
                             wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                             wxOK | wxICON_INFORMATION, this);
              else
                wxMessageBox(wxT("dump shp error:") +
                             wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                             wxOK | wxICON_ERROR, this);
          } else
            {
              // asking the charset to be used
              DumpShpDialog dlg;
              if (isPostGIS == true)
                dlg.Create(MainFrame, path, obj->GetVirtName(),
                           obj->GetColumn(), MainFrame->GetDefaultCharset());
              else
                dlg.Create(MainFrame, path, obj->GetMainName(),
                           obj->GetColName(), MainFrame->GetDefaultCharset());
              ret = dlg.ShowModal();
              if (ret == wxID_OK)
                {
                  MainFrame->SetLastDirectory(lastDir);
                  ::wxBeginBusyCursor();
                  strcpy(x_charset, dlg.GetCharset().ToUTF8());
                  rt =
                    dump_shapefile(MainFrame->GetSqlite(), x_table, x_column,
                                   x_path, x_charset, x_type, 0, &rows,
                                   err_msg);
                  ::wxEndBusyCursor();
                  if (rt)
                    wxMessageBox(wxT("dump shp:") +
                                 wxString::FromUTF8(err_msg),
                                 wxT("spatialite_gui"),
                                 wxOK | wxICON_INFORMATION, this);
                  else
                    wxMessageBox(wxT("dump shp error:") +
                                 wxString::FromUTF8(err_msg),
                                 wxT("spatialite_gui"), wxOK | wxICON_ERROR,
                                 this);
                }
            }
        }
    }
}

void MyTableTree::OnCmdDumpGeoJSON(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as GeoJSON
//
  int ret;
  wxString path;
  wxString lastDir;
  bool isView = false;
  bool isPostGIS = false;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_POSTGIS_GEOMETRY)
    isPostGIS = true;
  if (obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED)
    isView = true;
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED || isView == true || isPostGIS)
    {
      wxFileDialog fileDialog(this, wxT("Dump GeoJSON"),
                              wxT(""), wxT("export.geojson"),
                              wxT
                              ("GeoJSON (*.geojson)|*.geojson|All files (*.*)|*.*"),
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                              wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
      lastDir = MainFrame->GetLastDirectory();
      if (lastDir.Len() >= 1)
        fileDialog.SetDirectory(lastDir);
      ret = fileDialog.ShowModal();
      if (ret == wxID_OK)
        {
          char x_path[1024];
          char x_table[1024];
          char x_column[1024];
          char x_type[1024];
          char x_charset[1024];
          char *err_msg = NULL;
          int rows;
          int rt;
          *x_type = '\0';
          wxFileName file(fileDialog.GetPath());
          path = file.GetPath();
          path += file.GetPathSeparator();
          path += file.GetName();
          lastDir = file.GetPath();
          strcpy(x_path, path.ToUTF8());
          if (isPostGIS == true)
            {
              strcpy(x_table, obj->GetVirtName().ToUTF8());
              strcpy(x_column, obj->GetColumn().ToUTF8());
          } else
            {
              strcpy(x_table, obj->GetMainName().ToUTF8());
              strcpy(x_column, obj->GetColName().ToUTF8());
            }
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          strcpy(x_charset, MainFrame->GetDefaultCharset().ToUTF8());
          rt =
            dump_geojson2(MainFrame->GetSqlite(), x_table, x_column, x_path, 8,
                          1, 0, 1, GAIA_DBF_COLNAME_LOWERCASE, &rows, &err_msg);
          ::wxEndBusyCursor();
          if (rt)
            {
              err_msg = sqlite3_mprintf("exported %d rows", rows);
              wxMessageBox(wxT("dump GeoJson: ") +
                           wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                           wxOK | wxICON_INFORMATION, this);
              sqlite3_free(err_msg);
          } else
            {
              wxMessageBox(wxT("dump GeoJson error: ") +
                           wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                           wxOK | wxICON_ERROR, this);
              sqlite3_free(err_msg);
            }
        }
    }
}

void MyTableTree::OnCmdDumpKml(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as KML
//
  int ret;
  wxString path;
  wxString lastDir;
  bool isView = false;
  bool isVirtual = false;
  bool isPostGIS = false;
  bool isNameConst = false;
  bool isDescConst = false;
  wxString Name;
  wxString Desc;
  int precision = -1;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_POSTGIS_GEOMETRY)
    isPostGIS = true;
  if (obj->GetType() == MY_VIEW_GEOMETRY
      || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
      || obj->GetType() == MY_VIEW_GEOMETRY_CACHED)
    isView = true;
  if (obj->GetType() == MY_VIRTUAL_GEOMETRY)
    isVirtual = true;
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED || isView == true
      || isVirtual == true || isPostGIS == true)
    {
      DumpKmlDialog dlg;
      if (isPostGIS == true)
        dlg.Create(MainFrame, obj->GetVirtName(), obj->GetColumn());
      else
        dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName());
      ret = dlg.ShowModal();
      if (ret == wxID_OK)
        {
          isNameConst = dlg.IsNameConst();
          Name = dlg.GetName();
          isDescConst = dlg.IsDescConst();
          Desc = dlg.GetDesc();
          precision = dlg.GetPrecision();
      } else
        return;
      wxFileDialog fileDialog(this, wxT("Dump KML"),
                              wxT(""), wxT("export_file.kml"),
                              wxT
                              ("KML file (*.kml)|*.kml|All files (*.*)|*.*"),
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                              wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
      lastDir = MainFrame->GetLastDirectory();
      if (lastDir.Len() >= 1)
        fileDialog.SetDirectory(lastDir);
      ret = fileDialog.ShowModal();
      if (ret == wxID_OK)
        {

          wxFileName file(fileDialog.GetPath());
          path = file.GetPath();
          path += file.GetPathSeparator();
          path += file.GetName();
          path += wxT(".kml");
          lastDir = file.GetPath();
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          if (isPostGIS == true)
            MainFrame->DumpKml(path, obj->GetVirtName(), obj->GetColumn(),
                               precision, Name, isNameConst, Desc, isDescConst);
          else
            MainFrame->DumpKml(path, obj->GetMainName(), obj->GetColName(),
                               precision, Name, isNameConst, Desc, isDescConst);
          ::wxEndBusyCursor();
        }
    }
}

void MyTableTree::OnCmdDumpTxtTab(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as TxtTab
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  wxFileDialog fileDialog(this, wxT("Dump Txt/Tab file"),
                          wxT(""), wxT("table.txt"),
                          wxT("Txt/Tab file (*.txt)|*.txt|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".txt");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->DumpTxtTab(path, obj->GetMainName(),
                                MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("TXT / TAB");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->DumpTxtTab(path, obj->GetMainName(), dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyTableTree::OnCmdDumpCsv(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as CSV
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  wxFileDialog fileDialog(this, wxT("Dump CSV file"),
                          wxT(""), wxT("table.csv"),
                          wxT("CSV file (*.csv)|*.csv|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".csv");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->DumpCsv(path, obj->GetMainName(),
                             MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("CSV");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->DumpCsv(path, obj->GetMainName(), dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyTableTree::OnCmdDumpHtml(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as Html
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  wxFileDialog fileDialog(this, wxT("Dump HTML file"),
                          wxT(""), wxT("table.html"),
                          wxT
                          ("HTML web page (*.html)|*.html|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".html");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->DumpHtml(path, obj->GetMainName(),
                              MainFrame->GetSqlitePath(),
                              MainFrame->GetDefaultCharset());
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("HTML");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->DumpHtml(path, obj->GetMainName(),
                                  MainFrame->GetSqlitePath(), dlg.GetCharset());
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyTableTree::OnCmdDumpDif(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as DIF
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
// asking Decimal Point / Date-Times params
  DumpSpreadsheetDialog sheet_dlg;
  char decimal_point;
  bool date_times;
  sheet_dlg.Create(MainFrame);
  ret = sheet_dlg.ShowModal();
  if (ret == wxID_OK)
    {
      decimal_point = sheet_dlg.GetDecimalPoint();
      date_times = sheet_dlg.IsDateTimes();
  } else
    return;
  wxFileDialog fileDialog(this, wxT("Dump DIF file"),
                          wxT(""), wxT("table.dif"),
                          wxT
                          ("DIF spreadsheet document (*.dif)|*.dif|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".dif");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->DumpDif(path, obj->GetMainName(),
                             MainFrame->GetDefaultCharset(), decimal_point,
                             date_times);
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("DIF spreadsheet");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->DumpDif(path, obj->GetMainName(), dlg.GetCharset(),
                                 decimal_point, date_times);
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyTableTree::OnCmdDumpSylk(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as SYLK
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
// asking Decimal Point / Date-Times params
  DumpSpreadsheetDialog sheet_dlg;
  bool date_times;
  sheet_dlg.Create(MainFrame);
  ret = sheet_dlg.ShowModal();
  if (ret == wxID_OK)
    date_times = sheet_dlg.IsDateTimes();
  else
    return;
  wxFileDialog fileDialog(this, wxT("Dump SYLK file"),
                          wxT(""), wxT("table.slk"),
                          wxT
                          ("SYLK spreadsheet document (*.slk)|*.slk|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".slk");
      lastDir = file.GetPath();
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          MainFrame->DumpSylk(path, obj->GetMainName(),
                              MainFrame->GetDefaultCharset(), date_times);
          ::wxEndBusyCursor();
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("SYLK spreadsheet");
          dlg.Create(MainFrame, path, target, MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              ::wxBeginBusyCursor();
              MainFrame->DumpSylk(path, obj->GetMainName(), dlg.GetCharset(),
                                  date_times);
              ::wxEndBusyCursor();
            }
        }
    }
}

void MyTableTree::OnCmdDumpDbf(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as DBF
//
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  wxFileDialog fileDialog(this, wxT("Dump DBF file"),
                          wxT(""), wxT("table.dbf"),
                          wxT
                          ("DBF archive (*.dbf)|*.dbf|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      char x_path[1024];
      char x_table[1024];
      char x_charset[1024];
      char err_msg[1024];
      int rt;
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".dbf");
      lastDir = file.GetPath();
      strcpy(x_path, path.ToUTF8());
      strcpy(x_table, obj->GetMainName().ToUTF8());
      if (MainFrame->IsSetAskCharset() == false)
        {
          // using the default output charset
          MainFrame->SetLastDirectory(lastDir);
          ::wxBeginBusyCursor();
          strcpy(x_charset, MainFrame->GetDefaultCharset().ToUTF8());
          rt =
            dump_dbf(MainFrame->GetSqlite(), x_table, x_path, x_charset,
                     err_msg);
          ::wxEndBusyCursor();
          if (rt)
            wxMessageBox(wxT("dump dbf:") +
                         wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                         wxOK | wxICON_INFORMATION, this);
          else
            wxMessageBox(wxT("dump dbf error:") +
                         wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                         wxOK | wxICON_ERROR, this);
      } else
        {
          // asking the charset to be used
          DumpTxtDialog dlg;
          target = wxT("DBF archive");
          dlg.Create(MainFrame, path, obj->GetMainName(),
                     MainFrame->GetDefaultCharset());
          ret = dlg.ShowModal();
          if (ret == wxID_OK)
            {
              MainFrame->SetLastDirectory(lastDir);
              strcpy(x_charset, dlg.GetCharset().ToUTF8());
              ::wxBeginBusyCursor();
              rt =
                dump_dbf(MainFrame->GetSqlite(), x_table, x_path, x_charset,
                         err_msg);
              ::wxEndBusyCursor();
              if (rt)
                wxMessageBox(wxT("dump dbf:") +
                             wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                             wxOK | wxICON_INFORMATION, this);
              else
                wxMessageBox(wxT("dump dbf error:") +
                             wxString::FromUTF8(err_msg), wxT("spatialite_gui"),
                             wxOK | wxICON_ERROR, this);
            }
        }
    }
}

void MyTableTree::OnCmdDumpXlsx(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dumping as MS Excell XLSX
//
#ifdef ENABLE_XLSXWRITER	// only if XlsxWriter is supported
  int ret;
  wxString path;
  wxString lastDir;
  wxString target;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  wxFileDialog fileDialog(this, wxT("Dump MS XLSX spreadsheet"),
                          wxT(""), wxT("table.xlsx"),
                          wxT
                          ("MS Excell spreadsheet (*.xlsx)|*.xlsx|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxString sql;
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".xlsx");
      lastDir = file.GetPath();
      sql = wxT("SELECT * FROM \"") + obj->GetMainName() + wxT("\"");
      MainFrame->SetLastDirectory(lastDir);
      ::wxBeginBusyCursor();
      MainFrame->DoExportXLSX(path, sql);
      ::wxEndBusyCursor();
    }
#else
    wxMessageBox(wxT("This copy of SpatialiteGUI has been buil by disabling libXlsxWriter support\n... Sorry, cowardly quitting"), 
    wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                 MainFrame);
#endif
}

void MyTableTree::OnCmdDumpPostGIS(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - SQL dump for PostGIS
//
  wxString sql;
  char *xsql;
  sqlite3_stmt *stmt;
  char *xname;
  char *xname2;
  wxString lastDir;
  int ret;
  DumpPostGISDialog postgis_dlg;
  bool lowercase;
  bool create_table;
  bool spatial_index;
  wxString schema_name;
  wxString table_name;
  int rows = 0;
  int n_cols;
  int i;
  gaiaGeomCollPtr geom;
  PostGISHelper postgis;
  wxFileDialog fileDialog(this, wxT("SQL Dump for PostGIS"),
                          wxT(""), wxT("postgis.sql"),
                          wxT
                          ("SQL dump (*.sql)|*.sql|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;

// asking PostGIS options
  postgis_dlg.Create(MainFrame, obj->GetMainName());
  ret = postgis_dlg.ShowModal();
  if (ret == wxID_OK)
    {
      lowercase = postgis_dlg.IsLowercase();
      create_table = postgis_dlg.IsCreateTable();
      spatial_index = postgis_dlg.IsSpatialIndex();
      schema_name = postgis_dlg.GetSchemaName();
      table_name = postgis_dlg.GetTableName();
  } else
    return;

//
// preparing SQL statement 
//
  sql = wxT("SELECT * FROM \"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\"");
//
// compiling SQL prepared statement 
//
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), xsql, strlen(xsql), &stmt, NULL);
  free(xsql);
  if (ret != SQLITE_OK)
    goto sql_error;

  ::wxBeginBusyCursor();
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          n_cols = sqlite3_column_count(stmt);
          if (rows == 0)
            {
              // setting the column names
              postgis.Alloc(n_cols);
              for (i = 0; i < n_cols; i++)
                postgis.SetName(i, (const char *) sqlite3_column_name(stmt, i));
            }
          rows++;
          for (i = 0; i < n_cols; i++)
            {
              if (sqlite3_column_type(stmt, i) == SQLITE_INTEGER)
                postgis.Eval(i, sqlite3_column_int64(stmt, i));
              else if (sqlite3_column_type(stmt, i) == SQLITE_FLOAT)
                postgis.Eval(i, sqlite3_column_double(stmt, i));
              else if (sqlite3_column_type(stmt, i) == SQLITE_TEXT)
                postgis.Eval(i, (const char *) sqlite3_column_text(stmt, i));
              else if (sqlite3_column_type(stmt, i) == SQLITE_BLOB)
                {
                  const void *blob_value = sqlite3_column_blob(stmt, i);
                  int len = sqlite3_column_bytes(stmt, i);
                  geom =
                    gaiaFromSpatiaLiteBlobWkb((unsigned char *) blob_value,
                                              len);
                  if (geom)
                    {
                      postgis.Eval(i, geom);
                      gaiaFreeGeomColl(geom);
                  } else
                    postgis.EvalBlob(i);
              } else
                postgis.Eval(i);
            }
      } else
        {
          sqlite3_finalize(stmt);
          goto sql_error;
        }
    }
  postgis.GetKeys(MainFrame, obj->GetMainName());
  postgis.Prepare();
  ::wxEndBusyCursor();

// rewinding the result-set
  ret = sqlite3_reset(stmt);
  if (ret != SQLITE_OK)
    {
      sqlite3_finalize(stmt);
      goto sql_error;
    }

  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      wxString path;
      wxFileName file(fileDialog.GetPath());
      path = file.GetPath();
      path += file.GetPathSeparator();
      path += file.GetName();
      path += wxT(".sql");
      lastDir = file.GetPath();
      char x_path[1024];
      strcpy(x_path, path.ToUTF8());
      FILE *out = fopen(x_path, "wb");
      if (out == NULL)
        {
          wxMessageBox(wxT
                       ("PostGIS SQL dump error: unable to create output file"),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          goto stop;
        }
      ::wxBeginBusyCursor();
      fprintf(out,
              "--\n-- SQL dump automatically generated by \"spatialite_gui\" [GPLv3]\n");
      wxDateTime now = wxDateTime::Now();
      fprintf(out, "-- created on: %04d-%02d-%02d %02d:%02d:%02d\n--\n",
              now.GetYear(), now.GetMonth() + 1, now.GetDay(), now.GetHour(),
              now.GetMinute(), now.GetSecond());
      strcpy(x_path, MainFrame->GetSqlitePath().ToUTF8());
      fprintf(out, "-- DB-file origin: %s\n", x_path);
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      fprintf(out, "-- Table origin: %s\n--\n", xname);
      free(xname);
      fprintf(out, "-- intended target is: PostgreSQL + PostGIS\n--\n\n");
      if (create_table)
        {
          fprintf(out, "CREATE TABLE ");
          if (schema_name.Len() > 0)
            {
              xname = (char *) malloc((schema_name.Len() * 4) + 1);
              strcpy(xname, schema_name.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              fprintf(out, "%s.", xname2);
              free(xname2);
            }
          xname = (char *) malloc((table_name.Len() * 4) + 1);
          strcpy(xname, table_name.ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          fprintf(out, "%s (\n", xname2);
          free(xname2);
          for (i = 0; i < postgis.GetCount(); i++)
            {
              if (postgis.IsGeometry(i) == true)
                {
                  // skipping any Geometry column
                  continue;
                }
              if (postgis.GetDataType(i) == postgis.DATA_TYPE_UNDEFINED)
                {
                  // skipping any invalid column
                  continue;
                }
              xname =
                (char *) malloc((postgis.GetName(i, lowercase).Len() * 4) + 1);
              strcpy(xname, postgis.GetName(i, lowercase).ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              char data_type[128];
              postgis.GetDataType(i, data_type);
              if (i == 0)
                fprintf(out, "\t%s %s", xname2, data_type);
              else
                fprintf(out, ",\n\t%s %s", xname2, data_type);
              free(xname2);
            }
// definining any Primary Key Constraint (if any)
          PostGISIndex *idx = postgis.GetFirstIndex();
          while (idx)
            {
              if (idx->IsPrimaryKey() != true)
                {
                  idx = idx->GetNext();
                  continue;
                }
              if (postgis.IsSingleFieldPrimaryKey() == true)
                {
                  idx = idx->GetNext();
                  continue;
                }
              fprintf(out, ",\nCONSTRAINT ");
              xname = (char *) malloc((idx->GetName().Len() * 4) + 1);
              strcpy(xname, idx->GetName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              fprintf(out, "%s PRIMARY KEY (", xname2);
              free(xname2);
              PostGISIndexField *idx_fld = idx->GetFirst();
              while (idx_fld)
                {
                  if (idx_fld == idx->GetFirst())
                    ;
                  else
                    fprintf(out, ", ");
                  xname =
                    (char *)
                    malloc((idx_fld->GetColumnRef()->GetName().Len() * 4) + 1);
                  strcpy(xname, idx_fld->GetColumnRef()->GetName().ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s", xname2);
                  free(xname2);
                  idx_fld = idx_fld->GetNext();
                }
              fprintf(out, ")");
              idx = idx->GetNext();
            }
          fprintf(out, ");\n\n");
          for (i = 0; i < postgis.GetCount(); i++)
            {
              if (postgis.IsGeometry(i) != true)
                {
                  // skipping any not-Geometry column
                  continue;
                }
              fprintf(out, "SELECT AddGeometryColumn(");
              if (schema_name.Len() > 0)
                {
                  xname = (char *) malloc((schema_name.Len() * 4) + 1);
                  strcpy(xname, schema_name.ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "'%s', ", xname2);
                  free(xname2);
                }
              xname = (char *) malloc((table_name.Len() * 4) + 1);
              strcpy(xname, table_name.ToUTF8());
              xname2 = gaiaSingleQuotedSql(xname);
              free(xname);
              fprintf(out, "'%s', ", xname2);
              free(xname2);
              xname =
                (char *) malloc((postgis.GetName(i, lowercase).Len() * 4) + 1);
              strcpy(xname, postgis.GetName(i, lowercase).ToUTF8());
              xname2 = gaiaSingleQuotedSql(xname);
              free(xname);
              fprintf(out, "'%s', %d, ", xname2, postgis.GetSrid(i));
              free(xname2);
              switch (postgis.GetDataType(i))
                {
                  case PostGISHelper::DATA_TYPE_POINT:
                    fprintf(out, "'POINT");
                    break;
                  case PostGISHelper::DATA_TYPE_MULTIPOINT:
                    fprintf(out, "'MULTIPOINT");
                    break;
                  case PostGISHelper::DATA_TYPE_LINESTRING:
                    fprintf(out, "'LINESTRING");
                    break;
                  case PostGISHelper::DATA_TYPE_MULTILINESTRING:
                    fprintf(out, "'MULTILINESTRING");
                    break;
                  case PostGISHelper::DATA_TYPE_POLYGON:
                    fprintf(out, "'POLYGON");
                    break;
                  case PostGISHelper::DATA_TYPE_MULTIPOLYGON:
                    fprintf(out, "'MULTIPOLYGON");
                    break;
                  case PostGISHelper::DATA_TYPE_GEOMETRYCOLLECTION:
                    fprintf(out, "'GEOMETRYCOLLECTION");
                    break;
                  default:
                    fprintf(out, "'GEOMETRY");
                    break;
                };
              switch (postgis.GetCoordDims(i))
                {
                  case GAIA_XY_Z_M:
                    fprintf(out, "', 4");
                    break;
                  case GAIA_XY_Z:
                    fprintf(out, "', 3");
                    break;
                  case GAIA_XY_M:
                    fprintf(out, "M', 3");
                    break;
                  default:
                    fprintf(out, "', 2");
                    break;
                }
              fprintf(out, ");\n");
              if (spatial_index)
                {
                  fprintf(out, "CREATE INDEX ");
                  wxString idx_name = wxT("idx_");
                  idx_name += table_name;
                  idx_name += wxT("_");
                  idx_name += postgis.GetName(i, lowercase);
                  xname = (char *) malloc((idx_name.Len() * 4) + 1);
                  strcpy(xname, idx_name.ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s ON ", xname2);
                  free(xname2);
                  if (schema_name.Len() > 0)
                    {
                      xname = (char *) malloc((schema_name.Len() * 4) + 1);
                      strcpy(xname, schema_name.ToUTF8());
                      xname2 = gaiaDoubleQuotedSql(xname);
                      free(xname);
                      fprintf(out, "%s.", xname2);
                      free(xname2);
                    }
                  xname = (char *) malloc((table_name.Len() * 4) + 1);
                  strcpy(xname, table_name.ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s USING GIST (", xname2);
                  free(xname2);
                  xname =
                    (char *) malloc((postgis.GetName(i, lowercase).Len() * 4) +
                                    1);
                  strcpy(xname, postgis.GetName(i, lowercase).ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s);\n", xname2);
                  free(xname2);
                }
            }

          idx = postgis.GetFirstIndex();
          while (idx)
            {
              if (idx->IsPrimaryKey() == true)
                {
                  idx = idx->GetNext();
                  continue;
                }
              if (idx->IsUnique() == true)
                fprintf(out, "CREATE UNIQUE INDEX ");
              else
                fprintf(out, "CREATE INDEX ");
              xname = (char *) malloc((idx->GetName().Len() * 4) + 1);
              strcpy(xname, idx->GetName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              fprintf(out, "%s ON ", xname2);
              free(xname2);
              if (schema_name.Len() > 0)
                {
                  xname = (char *) malloc((schema_name.Len() * 4) + 1);
                  strcpy(xname, schema_name.ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s.", xname2);
                  free(xname2);
                }
              xname = (char *) malloc((table_name.Len() * 4) + 1);
              strcpy(xname, table_name.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              fprintf(out, "%s (", xname2);
              free(xname2);
              PostGISIndexField *idx_fld = idx->GetFirst();
              while (idx_fld)
                {
                  if (idx_fld != idx->GetFirst())
                    fprintf(out, ", ");
                  xname =
                    (char *)
                    malloc((idx_fld->GetColumnRef()->GetName().Len() * 4) + 1);
                  strcpy(xname, idx_fld->GetColumnRef()->GetName().ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s", xname2);
                  free(xname2);
                  idx_fld = idx_fld->GetNext();
                }
              fprintf(out, ");\n");
              idx = idx->GetNext();
            }
          fprintf(out, "\n");
        }

      rows = 0;
      fprintf(out, "BEGIN;\n");
      while (1)
        {
          ret = sqlite3_step(stmt);
          if (ret == SQLITE_DONE)
            break;              // end of result set
          if (ret == SQLITE_ROW)
            {
              if (rows > 0)
                {
                  if ((rows % 1000) == 0)
                    {
                      // COMMIT and then restarts a new Transaction
                      fprintf(out, "COMMIT;\n\n");
                      fprintf(out, "-- %d rows\n\nBEGIN;\n", rows);
                    }
                }
              rows++;
              fprintf(out, "INSERT INTO ");
              if (schema_name.Len() > 0)
                {
                  xname = (char *) malloc((schema_name.Len() * 4) + 1);
                  strcpy(xname, schema_name.ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  fprintf(out, "%s.", xname2);
                  free(xname2);
                }
              xname = (char *) malloc((table_name.Len() * 4) + 1);
              strcpy(xname, table_name.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              fprintf(out, "%s (", xname2);
              free(xname2);
              for (i = 0; i < postgis.GetCount(); i++)
                {
                  if (postgis.GetDataType(i) == postgis.DATA_TYPE_UNDEFINED)
                    {
                      // skipping any invalid column
                      continue;
                    }
                  xname =
                    (char *)
                    malloc((postgis.GetName(i, lowercase).Len() * 4) + 1);
                  strcpy(xname, postgis.GetName(i, lowercase).ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  if (i == 0)
                    fprintf(out, "%s", xname2);
                  else
                    fprintf(out, ", %s", xname2);
                  free(xname2);
                }
              fprintf(out, ") VALUES (");
              for (i = 0; i < n_cols; i++)
                {
                  if (i > 0)
                    fprintf(out, ", ");
                  int type = sqlite3_column_type(stmt, i);
                  int data_type = postgis.GetDataType(i);
                  switch (type)
                    {
                      case SQLITE_NULL:
                        fprintf(out, "NULL");
                        break;
                      case SQLITE_INTEGER:
                        if (data_type == PostGISHelper::DATA_TYPE_BOOLEAN)
                          postgis.OutputBooleanValue(out,
                                                     sqlite3_column_int64
                                                     (stmt, i));
                        else
                          postgis.OutputValue(out,
                                              sqlite3_column_int64(stmt, i));
                        break;
                      case SQLITE_FLOAT:
                        postgis.OutputValue(out,
                                            sqlite3_column_double(stmt, i));
                        break;
                      case SQLITE_TEXT:
                        postgis.OutputValue(out,
                                            (const char *)
                                            sqlite3_column_text(stmt, i));
                        break;
                      case SQLITE_BLOB:
                        if (postgis.IsGeometry(i))
                          {
                            const void *blob_value;
                            blob_value = sqlite3_column_blob(stmt, i);
                            int len = sqlite3_column_bytes(stmt, i);
                            geom =
                              gaiaFromSpatiaLiteBlobWkb((unsigned char *)
                                                        blob_value, len);
                            if (geom == NULL)
                              fprintf(out, "NULL");
                            else
                              {
                                postgis.OutputValue(out, geom);
                                gaiaFreeGeomColl(geom);
                              }
                        } else
                          {
                            int len = sqlite3_column_bytes(stmt, i);
                            postgis.OutputValue(out,
                                                (unsigned char *)
                                                sqlite3_column_blob(stmt,
                                                                    i), len);
                          }
                        break;
                    };
                }
              fprintf(out, ");\n");
          } else
            {
              sqlite3_finalize(stmt);
              goto sql_error;
            }
        }
      fprintf(out, "COMMIT;\n");


      fprintf(out, "\n--\n-- end SQL dump\n");
      fprintf(out, "--\n");
      ::wxEndBusyCursor();
      fclose(out);
    }

stop:
  sqlite3_finalize(stmt);
  return;

sql_error:
//
// some SQL error occurred
//
  sqlite3_finalize(stmt);
  ::wxEndBusyCursor();
  wxMessageBox(wxT("PostGIS SQL dump error:") +
               wxString::FromUTF8(sqlite3_errmsg(MainFrame->GetSqlite())),
               wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  return;
}

void MyTableTree::OnCmdCreateVectorCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Create Vector Coverage
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;

  char *coverage_name = obj->DoFindUnusedCoverageName(GetSQLiteHandle());
  if (coverage_name == NULL)
    return;

  char *sql;
  char *table = (char *) malloc(obj->GetMainName().Len() * 4);
  char *column = (char *) malloc(obj->GetColName().Len() * 4);
  strcpy(table, obj->GetMainName().ToUTF8());
  strcpy(column, obj->GetColName().ToUTF8());
  if (obj->GetType() == MY_GEOMETRY || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED)
    sql =
      sqlite3_mprintf
      ("INSERT INTO vector_coverages (coverage_name, f_table_name, f_geometry_column, "
       "is_queryable, is_editable) VALUES (Lower(%Q), Lower(%Q), Lower(%Q), 1, 1)",
       coverage_name, table, column);
  else if (obj->GetType() == MY_VIEW_GEOMETRY
           || obj->GetType() == MY_VIEW_GEOMETRY_INDEX
           || obj->GetType() == MY_VIEW_GEOMETRY_CACHED)
    sql =
      sqlite3_mprintf
      ("INSERT INTO vector_coverages (coverage_name, view_name, view_geometry, "
       "is_queryable, is_editable) VALUES (Lower(%Q), Lower(%Q), Lower(%Q), 1, 0)",
       coverage_name, table, column);
  else if (obj->GetType() == MY_VIRTUAL_GEOMETRY)
    sql =
      sqlite3_mprintf
      ("INSERT INTO vector_coverages (coverage_name, virt_name, virt_geometry, "
       "is_queryable, is_editable) VALUES (Lower(%Q), Lower(%Q), Lower(%Q), 1, 0)",
       coverage_name, table, column);
  else
    {
      // unknown Geometry type - quitting
      sqlite3_free(coverage_name);
      free(table);
      free(column);
      return;
    }
  char *errMsg;
  int ret = sqlite3_exec(GetSQLiteHandle(), sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
  } else
    wxMessageBox(wxT("Vector Coverage \"") + wxString::FromUTF8(coverage_name) +
                 wxT("\" was succesfully created"), wxT("spatialite_gui"),
                 wxOK | wxICON_INFORMATION, this);

  sqlite3_free(coverage_name);
  free(table);
  free(column);
}

void MyTableTree::OnCmdCreateTopologyCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Create Topology Coverage
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;

  char *coverage_name = obj->DoFindUnusedCoverageName(GetSQLiteHandle());
  if (coverage_name == NULL)
    return;

  char *sql;
  char *topo_name = (char *) malloc(obj->GetMainName().Len() * 4);
  strcpy(topo_name, obj->GetMainName().ToUTF8());
  sql = sqlite3_mprintf("SELECT SE_RegisterTopoGeoCoverage(%Q, %Q)",
                        coverage_name, topo_name);
  char *errMsg;
  int ret = sqlite3_exec(GetSQLiteHandle(), sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
  } else
    wxMessageBox(wxT("Topology Coverage \"") +
                 wxString::FromUTF8(coverage_name) +
                 wxT("\" was succesfully created"), wxT("spatialite_gui"),
                 wxOK | wxICON_INFORMATION, this);

  sqlite3_free(coverage_name);
  free(topo_name);
}

void MyTableTree::OnCmdCreateNetworkCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Create Network Coverage
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;

  char *coverage_name = obj->DoFindUnusedCoverageName(GetSQLiteHandle());
  if (coverage_name == NULL)
    return;

  char *sql;
  char *net_name = (char *) malloc(obj->GetMainName().Len() * 4);
  strcpy(net_name, obj->GetMainName().ToUTF8());
  sql = sqlite3_mprintf("SELECT SE_RegisterTopoNetCoverage(%Q, %Q)",
                        coverage_name, net_name);
  char *errMsg;
  int ret = sqlite3_exec(GetSQLiteHandle(), sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
  } else
    wxMessageBox(wxT("Network Coverage \"") +
                 wxString::FromUTF8(coverage_name) +
                 wxT("\" was succesfully created"), wxT("spatialite_gui"),
                 wxOK | wxICON_INFORMATION, this);

  sqlite3_free(coverage_name);
  free(net_name);
}

void MyTableTree::OnCmdMapPreview(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Map Preview
//
  char *table_name;
  char *column_name;
  char *table_name2;
  char *column_name2;
  char sql[4192];
  char sql2[2048];
  int ret;
  char err_msg[2048];
  sqlite3_stmt *stmt;
  double minx = DBL_MAX;
  double miny = DBL_MAX;
  double maxx = DBL_MAX;
  double maxy = DBL_MAX;
  bool isPostGIS = false;

  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_POSTGIS_GEOMETRY)
    isPostGIS = true;

  ::wxBeginBusyCursor();
  if (isPostGIS == true)
    {
      table_name = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(table_name, obj->GetVirtName().ToUTF8());
      column_name = (char *) malloc((obj->GetColumn().Len() * 4) + 1);
      strcpy(column_name, obj->GetColumn().ToUTF8());
  } else
    {
      table_name = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(table_name, obj->GetMainName().ToUTF8());
      column_name = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(column_name, obj->GetColName().ToUTF8());
    }
  table_name2 = gaiaDoubleQuotedSql(table_name);
  column_name2 = gaiaDoubleQuotedSql(column_name);
  free(table_name);
  free(column_name);
  sprintf(sql, "SELECT Min(MbrMinX(\"%s\")), Min(MbrMinY(\"%s\")), ",
          column_name2, column_name2);
  sprintf(sql2, "Max(MbrMaxX(\"%s\")), Max(MbrMaxY(\"%s\")) ", column_name2,
          column_name2);
  strcat(sql, sql2);
  sprintf(sql2, "FROM \"%s\"", table_name2);
  free(table_name2);
  free(column_name2);
  strcat(sql, sql2);

  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      sprintf(err_msg, "SQL error: %s", sqlite3_errmsg(MainFrame->GetSqlite()));
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(err_msg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      ::wxEndBusyCursor();
      return;
    }
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
          if (sqlite3_column_type(stmt, 0) == SQLITE_FLOAT)
            minx = sqlite3_column_double(stmt, 0);
          if (sqlite3_column_type(stmt, 1) == SQLITE_FLOAT)
            miny = sqlite3_column_double(stmt, 1);
          if (sqlite3_column_type(stmt, 2) == SQLITE_FLOAT)
            maxx = sqlite3_column_double(stmt, 2);
          if (sqlite3_column_type(stmt, 3) == SQLITE_FLOAT)
            maxy = sqlite3_column_double(stmt, 3);
      } else
        {
          sqlite3_finalize(stmt);
          sprintf(err_msg, "SQL error: %s",
                  sqlite3_errmsg(MainFrame->GetSqlite()));
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(err_msg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          ::wxEndBusyCursor();
          return;
        }
    }
  sqlite3_finalize(stmt);
  ::wxEndBusyCursor();

  if (minx == DBL_MAX || miny == DBL_MAX || maxx == DBL_MAX || maxy == DBL_MAX)
    {
      wxMessageBox(wxT("This Column doesn't contains any Geometry: sorry ..."),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      return;
    }

  MapPreviewDialog dlg;
  if (isPostGIS == true)
    dlg.Create(MainFrame, obj->GetVirtName(), obj->GetColumn(), minx, miny,
               maxx, maxy);
  else
    dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName(), minx, miny,
               maxx, maxy);
  dlg.ShowModal();
}

void MyTableTree::OnCmdColumnStats(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - column stats
//
  char *table_name;
  char *column_name;
  char *table_name2;
  char *column_name2;
  char sql[4192];
  char sql2[4192];
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  const char *value;
  char err_msg[2048];
  sqlite3_stmt *stmt;
  int count;
  int null_count = 0;
  int text_count = 0;
  int integer_count = 0;
  int real_count = 0;
  int blob_count = 0;
  double min;
  double max;
  double avg;
  double stddev_pop;
  double stddev_samp;
  double var_pop;
  double var_samp;
  int distinct_values = 0;

  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;

  ::wxBeginBusyCursor();
  table_name = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(table_name, obj->GetMainName().ToUTF8());
  column_name = (char *) malloc((obj->GetColName().Len() * 4) + 1);
  strcpy(column_name, obj->GetColName().ToUTF8());
  table_name2 = gaiaDoubleQuotedSql(table_name);
  column_name2 = gaiaDoubleQuotedSql(column_name);
  free(table_name);
  free(column_name);
  sprintf(sql,
          "SELECT Typeof(\"%s\"), Count(*) FROM \"%s\" GROUP BY Typeof(\"%s\")",
          column_name2, table_name2, column_name2);
  free(table_name2);
  free(column_name2);

  ret = sqlite3_get_table(MainFrame->GetSqlite(), sql, &results,
                          &rows, &columns, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 0];
          count = atoi(results[(i * columns) + 1]);
          if (strcasecmp(value, "null") == 0)
            null_count += count;
          if (strcasecmp(value, "text") == 0)
            text_count += count;
          if (strcasecmp(value, "integer") == 0)
            integer_count += count;
          if (strcasecmp(value, "real") == 0)
            real_count += count;
          if (strcasecmp(value, "blob") == 0)
            blob_count += count;
        }
    }
  sqlite3_free_table(results);

  if ((real_count + integer_count) > 0)
    {
      // computing statistic analysis
      table_name = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(table_name, obj->GetMainName().ToUTF8());
      column_name = (char *) malloc((obj->GetColName().Len() * 4) + 1);
      strcpy(column_name, obj->GetColName().ToUTF8());
      table_name2 = gaiaDoubleQuotedSql(table_name);
      column_name2 = gaiaDoubleQuotedSql(column_name);
      free(table_name);
      free(column_name);
      sprintf(sql, "SELECT Min(\"%s\"), Max(\"%s\"), Avg(\"%s\"), ",
              column_name2, column_name2, column_name2);
      sprintf(sql2, "StdDev_pop(\"%s\"), StdDev_samp(\"%s\"), ", column_name2,
              column_name2);
      strcat(sql, sql2);
      sprintf(sql2, "Var_pop(\"%s\"), Var_samp(\"%s\") FROM \"%s\"",
              column_name2, column_name2, table_name2);
      strcat(sql, sql2);
      free(table_name2);
      free(column_name2);

      ret = sqlite3_get_table(MainFrame->GetSqlite(), sql, &results,
                              &rows, &columns, &errMsg);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          ::wxEndBusyCursor();
          return;
        }
      if (rows < 1)
        ;
      else
        {
          for (i = 1; i <= rows; i++)
            {
              if (results[(i * columns) + 0] != NULL)
                min = atof(results[(i * columns) + 0]);
              if (results[(i * columns) + 1] != NULL)
                max = atof(results[(i * columns) + 1]);
              if (results[(i * columns) + 2] != NULL)
                avg = atof(results[(i * columns) + 2]);
              if (results[(i * columns) + 3] != NULL)
                stddev_pop = atof(results[(i * columns) + 3]);
              if (results[(i * columns) + 4] != NULL)
                stddev_samp = atof(results[(i * columns) + 4]);
              if (results[(i * columns) + 5] != NULL)
                var_pop = atof(results[(i * columns) + 5]);
              if (results[(i * columns) + 6] != NULL)
                var_samp = atof(results[(i * columns) + 6]);
            }
        }
      sqlite3_free_table(results);
    }
// computing DISTINCT values
  table_name = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(table_name, obj->GetMainName().ToUTF8());
  column_name = (char *) malloc((obj->GetColName().Len() * 4) + 1);
  strcpy(column_name, obj->GetColName().ToUTF8());
  table_name2 = gaiaDoubleQuotedSql(table_name);
  column_name2 = gaiaDoubleQuotedSql(column_name);
  free(table_name);
  free(column_name);
  sprintf(sql, "SELECT DISTINCT \"%s\" FROM \"%s\"", column_name2, table_name2);
  free(table_name2);
  free(column_name2);
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      sprintf(err_msg, "SQL error: %s", sqlite3_errmsg(MainFrame->GetSqlite()));
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(err_msg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      ::wxEndBusyCursor();
      return;
    }
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
          distinct_values++;
      } else
        {
          sqlite3_finalize(stmt);
          sprintf(err_msg, "SQL error: %s",
                  sqlite3_errmsg(MainFrame->GetSqlite()));
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(err_msg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          ::wxEndBusyCursor();
          return;
        }
    }
  sqlite3_finalize(stmt);

  ::wxEndBusyCursor();

  ColumnStatsDialog dlg;
  dlg.Create(MainFrame, obj->GetMainName(), obj->GetColName(), null_count,
             text_count, integer_count, real_count, blob_count, min, max, avg,
             stddev_pop, stddev_samp, var_pop, var_samp, distinct_values);
  dlg.ShowModal();
}

bool MyTableTree::DropRenameAux1(MyObject * obj, GeomColsList * Geometries,
                                 bool *autoincrement)
{
//
// common tasks: drop/rename column auxiliaries
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  bool check_autoincrement = false;
  wxString geomColumn;
  wxString geomType;
  wxString coordDims;
  int geomSrid;
  int indexType;
  wxString indexName;
  char *xname;
  char *xname2;
  char *value;
  int metadata_type;

// checking if the SQLITE_SEQUENCE table exists
  sql = wxT("PRAGMA table_info(sqlite_sequence)");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
          check_autoincrement = true;
        }
    }
  sqlite3_free_table(results);
  if (check_autoincrement == true)
    {
// checking if there is an AUTOINCREMENT Primary Key
      sql = wxT("SELECT name FROM sqlite_sequence WHERE Lower(name) = Lower('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("')");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
              *autoincrement = true;
            }
        }
      sqlite3_free_table(results);
    }
// checking if there is some Spatial Index already defined
  metadata_type = MainFrame->GetMetaDataType();
  if (metadata_type == METADATA_LEGACY)
    sql =
      wxT
      ("SELECT f_geometry_column, type, coord_dimension, srid, spatial_index_enabled ");
  else if (metadata_type == METADATA_CURRENT)
    sql =
      wxT
      ("SELECT f_geometry_column, geometry_type, srid, spatial_index_enabled ");
  else
    return false;
  sql += wxT("FROM geometry_columns WHERE Lower(f_table_name) = Lower('");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaSingleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("')");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
          if (metadata_type == METADATA_LEGACY)
            {
              // legacy Spatial MetaData layout
              value = results[(i * columns) + 0];
              geomColumn = wxString::FromUTF8(value);
              value = results[(i * columns) + 1];
              geomType = wxString::FromUTF8(value);
              value = results[(i * columns) + 2];
              coordDims = wxString::FromUTF8(value);
              value = results[(i * columns) + 3];
              geomSrid = atoi(value);
              value = results[(i * columns) + 4];
              indexType = atoi(value);
          } else
            {
              // current Spatial MetaData layout
              value = results[(i * columns) + 0];
              geomColumn = wxString::FromUTF8(value);
              switch (atoi(results[(i * columns) + 1]))
                {
                  case 0:
                    geomType = wxT("GEOMETRY");
                    coordDims = wxT("XY");
                    break;
                  case 1000:
                    geomType = wxT("GEOMETRY");
                    coordDims = wxT("XYZ");
                    break;
                  case 2000:
                    geomType = wxT("GEOMETRY");
                    coordDims = wxT("XYM");
                    break;
                  case 3000:
                    geomType = wxT("GEOMETRY");
                    coordDims = wxT("XYZM");
                    break;
                  case 1:
                    geomType = wxT("POINT");
                    coordDims = wxT("XY");
                    break;
                  case 1001:
                    geomType = wxT("POINT");
                    coordDims = wxT("XYZ");
                    break;
                  case 2001:
                    geomType = wxT("POINT");
                    coordDims = wxT("XYM");
                    break;
                  case 3001:
                    geomType = wxT("POINT");
                    coordDims = wxT("XYZM");
                    break;
                  case 2:
                    geomType = wxT("LINESTRING");
                    coordDims = wxT("XY");
                    break;
                  case 1002:
                    geomType = wxT("LINESTRING");
                    coordDims = wxT("XYZ");
                    break;
                  case 2002:
                    geomType = wxT("LINESTRING");
                    coordDims = wxT("XYM");
                    break;
                  case 3002:
                    geomType = wxT("LINESTRING");
                    coordDims = wxT("XYZM");
                    break;
                  case 3:
                    geomType = wxT("POLYGON");
                    coordDims = wxT("XY");
                    break;
                  case 1003:
                    geomType = wxT("POLYGON");
                    coordDims = wxT("XYZ");
                    break;
                  case 2003:
                    geomType = wxT("POLYGON");
                    coordDims = wxT("XYM");
                    break;
                  case 3003:
                    geomType = wxT("POLYGON");
                    coordDims = wxT("XYZM");
                    break;
                  case 4:
                    geomType = wxT("MULTIPOINT");
                    coordDims = wxT("XY");
                    break;
                  case 1004:
                    geomType = wxT("MULTIPOINT");
                    coordDims = wxT("XYZ");
                    break;
                  case 2004:
                    geomType = wxT("MULTIPOINT");
                    coordDims = wxT("XYM");
                    break;
                  case 3004:
                    geomType = wxT("MULTIPOINT");
                    coordDims = wxT("XYZM");
                    break;
                  case 5:
                    geomType = wxT("MULTILINESTRING");
                    coordDims = wxT("XY");
                    break;
                  case 1005:
                    geomType = wxT("MULTILINESTRING");
                    coordDims = wxT("XYZ");
                    break;
                  case 2005:
                    geomType = wxT("MULTILINESTRING");
                    coordDims = wxT("XYM");
                    break;
                  case 3005:
                    geomType = wxT("MULTILINESTRING");
                    coordDims = wxT("XYZM");
                    break;
                  case 6:
                    geomType = wxT("MULTIPOLYGON");
                    coordDims = wxT("XY");
                    break;
                  case 1006:
                    geomType = wxT("MULTIPOLYGON");
                    coordDims = wxT("XYZ");
                    break;
                  case 2006:
                    geomType = wxT("MULTIPOLYGON");
                    coordDims = wxT("XYM");
                    break;
                  case 3006:
                    geomType = wxT("MULTIPOLYGON");
                    coordDims = wxT("XYZM");
                    break;
                  case 7:
                    geomType = wxT("GEOMETRYCOLLECTION");
                    coordDims = wxT("XY");
                    break;
                  case 1007:
                    geomType = wxT("GEOMETRYCOLLECTION");
                    coordDims = wxT("XYZ");
                    break;
                  case 2007:
                    geomType = wxT("GEOMETRYCOLLECTION");
                    coordDims = wxT("XYM");
                    break;
                  case 3007:
                    geomType = wxT("GEOMETRYCOLLECTION");
                    coordDims = wxT("XYZM");
                    break;
                };
              value = results[(i * columns) + 2];
              geomSrid = atoi(value);
              value = results[(i * columns) + 3];
              indexType = atoi(value);
            }
          Geometries->Add(geomColumn, geomType, coordDims, geomSrid, indexType);
        }
    }
  sqlite3_free_table(results);
  return true;
}

void MyTableTree::DropRenameAux2(MyObject * obj, GeomColsList * Geometries,
                                 wxString & aliasTable, wxString & new_column,
                                 wxString & renameSql, wxString & dropSql,
                                 wxString & disableSpatialIdxSql,
                                 wxString & dropSpatialIdxSql,
                                 wxString & createSpatialIdxSql,
                                 wxString & discardGeometrySql)
{
//
// common tasks: drop/rename column auxiliaries
//
  GeomColumn *pG;
  wxString name;
  char *xname;
  char *xname2;
  char column[1024];

// creating the SQL fragments
  aliasTable = wxT("tmp_alias ");
  aliasTable += obj->GetMainName();
  aliasTable += wxT(" tmp_alias");
  xname = (char *) malloc((aliasTable.Len() * 4) + 1);
  strcpy(xname, aliasTable.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  aliasTable = wxString::FromUTF8(xname2);
  free(xname2);
  renameSql = wxT("ALTER TABLE \"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  renameSql += wxString::FromUTF8(xname2);
  free(xname2);
  renameSql += wxT("\" RENAME TO \"");
  xname = (char *) malloc((aliasTable.Len() * 4) + 1);
  strcpy(xname, aliasTable.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  renameSql += wxString::FromUTF8(xname2);
  free(xname2);
  renameSql += wxT("\";\n");

  dropSql = wxT("DROP TABLE \"");
  xname = (char *) malloc((aliasTable.Len() * 4) + 1);
  strcpy(xname, aliasTable.ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  dropSql += wxString::FromUTF8(xname2);
  free(xname2);
  dropSql += wxT("\";\n");
  strcpy(column, obj->GetColName().ToUTF8());
  pG = Geometries->GetFirst();
  while (pG)
    {
      if (pG->IsRTree() == true || pG->IsMbrCache() == true)
        {
          // disabling a Spatial Index
          disableSpatialIdxSql += wxT("SELECT DisableSpatialIndex('");
          xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
          strcpy(xname, obj->GetMainName().ToUTF8());
          xname2 = gaiaSingleQuotedSql(xname);
          free(xname);
          disableSpatialIdxSql += wxString::FromUTF8(xname2);
          free(xname2);
          disableSpatialIdxSql += wxT("', '");
          xname = (char *) malloc((pG->GetGeometryName().Len() * 4) + 1);
          strcpy(xname, pG->GetGeometryName().ToUTF8());
          xname2 = gaiaSingleQuotedSql(xname);
          free(xname);
          disableSpatialIdxSql += wxString::FromUTF8(xname2);
          free(xname2);
          disableSpatialIdxSql += wxT("');\n");
          dropSpatialIdxSql += wxT("DROP TABLE IF EXISTS \"");
          name = wxT("idx_");
          name += obj->GetMainName();
          name += wxT("_");
          name += pG->GetGeometryName();
          xname = (char *) malloc((name.Len() * 4) + 1);
          strcpy(xname, name.ToUTF8());
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          dropSpatialIdxSql += wxString::FromUTF8(xname2);
          free(xname2);
          dropSpatialIdxSql += wxT("\";\n");
          wxString name1 = pG->GetGeometryName();
          wxString name2 = obj->GetColName();
          if (name1.CmpNoCase(name2) != 0)
            {
              if (pG->IsRTree() == true)
                {
                  // creating an RTree Spatial Index
                  createSpatialIdxSql += wxT("SELECT CreateSpatialIndex('");
                  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
                  strcpy(xname, obj->GetMainName().ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("', '");
                  xname =
                    (char *) malloc((pG->GetGeometryName().Len() * 4) + 1);
                  strcpy(xname, pG->GetGeometryName().ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("');\n");
              } else
                {
                  // creating an MbrCache Spatial Index
                  createSpatialIdxSql += wxT("SELECT CreateMbrCache('");
                  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
                  strcpy(xname, obj->GetMainName().ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("', '");
                  xname =
                    (char *) malloc((pG->GetGeometryName().Len() * 4) + 1);
                  strcpy(xname, pG->GetGeometryName().ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("');\n");
                }
          } else if (new_column.Len() > 0)
            {
              if (pG->IsRTree() == true)
                {
                  // creating an RTree Spatial Index
                  createSpatialIdxSql += wxT("SELECT CreateSpatialIndex('");
                  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
                  strcpy(xname, obj->GetMainName().ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("', '");
                  xname = (char *) malloc((new_column.Len() * 4) + 1);
                  strcpy(xname, new_column.ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("');\n");
              } else
                {
                  // creating an MbrCache Spatial Index
                  createSpatialIdxSql += wxT("SELECT CreateMbrCache('");
                  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
                  strcpy(xname, obj->GetMainName().ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("', '");
                  xname = (char *) malloc((new_column.Len() * 4) + 1);
                  strcpy(xname, new_column.ToUTF8());
                  xname2 = gaiaSingleQuotedSql(xname);
                  free(xname);
                  createSpatialIdxSql += wxString::FromUTF8(xname2);
                  free(xname2);
                  createSpatialIdxSql += wxT("');\n");
                }
            }
        }
      // discarding a Geometry Column
      discardGeometrySql += wxT("SELECT DiscardGeometryColumn('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      discardGeometrySql += wxString::FromUTF8(xname2);
      free(xname2);
      discardGeometrySql += wxT("', '");
      xname = (char *) malloc((pG->GetGeometryName().Len() * 4) + 1);
      strcpy(xname, pG->GetGeometryName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      discardGeometrySql += wxString::FromUTF8(xname2);
      free(xname2);
      discardGeometrySql += wxT("');\n");
      pG = pG->GetNext();
    }
}

void MyTableTree::DropRenameAux3(MyObject * obj, wxString & new_column,
                                 GeomColsList * Geometries,
                                 TblIndexList * Index,
                                 wxString & addGeometrySql)
{
//
// common tasks: drop/rename column auxiliaries
//
  GeomColumn *pG;
  TblIndex *pI;
  char *xname;
  char *xname2;
  char dummy[64];
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *value;
  wxString indexName;
  bool uniqueIndex;

  pG = Geometries->GetFirst();
  while (pG)
    {
      // adding a Geometry Column
      wxString name1 = pG->GetGeometryName();
      wxString name2 = obj->GetColName();
      if (name1.CmpNoCase(name2) == 0)
        {
          if (new_column.Len() > 0)
            {
              addGeometrySql += wxT("SELECT AddGeometryColumn('");
              xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
              strcpy(xname, obj->GetMainName().ToUTF8());
              xname2 = gaiaSingleQuotedSql(xname);
              free(xname);
              addGeometrySql += wxString::FromUTF8(xname2);
              free(xname2);
              addGeometrySql += wxT("', '");
              xname = (char *) malloc((new_column.Len() * 4) + 1);
              strcpy(xname, new_column.ToUTF8());
              xname2 = gaiaSingleQuotedSql(xname);
              free(xname);
              addGeometrySql += wxString::FromUTF8(xname2);
              free(xname2);
              sprintf(dummy, "', %d", pG->GetSRID());
              addGeometrySql += wxString::FromUTF8(dummy);
              addGeometrySql += wxT(", '");
              addGeometrySql += pG->GetGeometryType();
              if (pG->GetCoordDims() == wxT('2')
                  || pG->GetCoordDims() == wxT('3'))
                {
                  addGeometrySql += wxT("', ");
                  addGeometrySql += pG->GetCoordDims();
              } else
                {
                  addGeometrySql += wxT("', '");
                  addGeometrySql += pG->GetCoordDims();
                  addGeometrySql += wxT("'");
                }
              if (pG->IsNotNull() == false)
                addGeometrySql += wxT(");\n");
              else
                addGeometrySql += wxT(", 1);\n");
            }
          pG = pG->GetNext();
          continue;
        }
      addGeometrySql += wxT("SELECT AddGeometryColumn('");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      addGeometrySql += wxString::FromUTF8(xname2);
      free(xname2);
      addGeometrySql += wxT("', '");
      xname = (char *) malloc((pG->GetGeometryName().Len() * 4) + 1);
      strcpy(xname, pG->GetGeometryName().ToUTF8());
      xname2 = gaiaSingleQuotedSql(xname);
      free(xname);
      addGeometrySql += wxString::FromUTF8(xname2);
      free(xname2);
      sprintf(dummy, "', %d", pG->GetSRID());
      addGeometrySql += wxString::FromUTF8(dummy);
      addGeometrySql += wxT(", '");
      addGeometrySql += pG->GetGeometryType();
      if (pG->GetCoordDims() == wxT('2') || pG->GetCoordDims() == wxT('3'))
        {
          addGeometrySql += wxT("', ");
          addGeometrySql += pG->GetCoordDims();
      } else
        {
          addGeometrySql += wxT("', '");
          addGeometrySql += pG->GetCoordDims();
          addGeometrySql += wxT("'");
        }
      if (pG->IsNotNull() == false)
        addGeometrySql += wxT(");\n");
      else
        addGeometrySql += wxT(", 1);\n");
      pG = pG->GetNext();
    }

// retrieving any related Index
  sql = wxT("PRAGMA index_list(\"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
          value = results[(i * columns) + 1];
          if (strncmp(value, "sqlite_autoindex_", 17) == 0)
            {
              // sandro 2011-01-03: discarding any Primary Key Index
              continue;
            }
          indexName = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          if (atoi(value) == 0)
            uniqueIndex = false;
          else
            uniqueIndex = true;
          Index->Add(indexName, uniqueIndex);
        }
    }
  sqlite3_free_table(results);
  pI = Index->GetFirst();
  while (pI)
    {
      // retrieving any Index Column
      sql = wxT("PRAGMA index_info(\"");
      xname = (char *) malloc((pI->GetIndexName().Len() * 4) + 1);
      strcpy(xname, pI->GetIndexName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
              value = results[(i * columns) + 2];
              indexName = wxString::FromUTF8(value);
              pI->Add(indexName);
            }
        }
      sqlite3_free_table(results);
      pI = pI->GetNext();
    }

}

void MyTableTree::OnCmdDropColumn(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - drop column
//
  char column[1024];
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString new_column;
  wxString sql;
  char *xsql;
  wxString createSql;
  wxString insertSql;
  wxString insertFromSql;
  wxString renameSql;
  wxString dropSql;
  wxString dropIndexSql;
  wxString createIndexSql;
  wxString disableSpatialIdxSql;
  wxString dropSpatialIdxSql;
  wxString createSpatialIdxSql;
  wxString discardGeometrySql;
  wxString addGeometrySql;
  wxString geomColumn;
  bool comma = false;
  char *value;
  bool autoincrement = false;
  wxString aliasTable;
  bool isGeom;
  wxString msg;
  GeomColsList Geometries;
  GeomColumn *pG;
  TblIndexList Index;
  TblIndex *pI;
  IndexColumn *pC;
  wxString name;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  if (obj == NULL)
    return;

  strcpy(column, obj->GetColName().ToUTF8());
  if (DropRenameAux1(obj, &Geometries, &autoincrement) == false)
    return;
  DropRenameAux2(obj, &Geometries, aliasTable, new_column, renameSql, dropSql,
                 disableSpatialIdxSql, dropSpatialIdxSql, createSpatialIdxSql,
                 discardGeometrySql);

// retrieving the Column names
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
      createSql = wxT("CREATE TABLE \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      createSql += wxString::FromUTF8(xname2);
      free(xname2);
      createSql += wxT("\" (\n");
      insertSql = wxT("INSERT INTO \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      insertSql += wxString::FromUTF8(xname2);
      free(xname2);
      insertSql += wxT("\" (");
      insertFromSql = wxT("SELECT ");
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 1];
          if (strcasecmp(value, column) == 0)
            continue;
          isGeom = false;
          pG = Geometries.GetFirst();
          while (pG)
            {
              char geom[1024];
              strcpy(geom, pG->GetGeometryName().ToUTF8());
              if (strcasecmp(geom, value) == 0)
                {
                  isGeom = true;
                  geomColumn = pG->GetGeometryName();
                  break;
                }
              pG = pG->GetNext();
            }
          if (comma == true)
            {
              if (isGeom == false)
                createSql += wxT(",\n");
              insertSql += wxT(", ");
              insertFromSql += wxT(", ");
            }
          if (isGeom == false)
            {
              xname = (char *) malloc(strlen(value) + 1);
              strcpy(xname, value);
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              createSql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
              free(xname2);
              createSql += wxT(" ");
            }
          xname = (char *) malloc(strlen(value) + 1);
          strcpy(xname, value);
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          insertSql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
          free(xname2);
          xname = (char *) malloc(strlen(value) + 1);
          strcpy(xname, value);
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          insertFromSql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
          free(xname2);
          value = results[(i * columns) + 2];
          if (isGeom == false)
            createSql += wxT("\"") + wxString::FromUTF8(value) + wxT("\"");
          value = results[(i * columns) + 5];
          if (value)
            {
              if (atoi(value) != 0)
                {
                  if (isGeom == false)
                    {
                      createSql += wxT(" PRIMARY KEY");
                      if (autoincrement == true)
                        createSql += wxT(" AUTOINCREMENT");
                    }
                }
            }
          value = results[(i * columns) + 3];
          if (value)
            {
              if (atoi(value) != 0)
                {
                  if (isGeom == true)
                    Geometries.SetNotNull(geomColumn);
                  else
                    createSql += wxT(" NOT NULL");
                }
            }
          value = results[(i * columns) + 4];
          if (value && isGeom == false)
            {
              createSql += wxT(" DEFAULT ");
              createSql += wxString::FromUTF8(value);
            }
          comma = true;
        }
      createSql += wxT(");\n");
      insertSql += wxT(")\n");
      insertFromSql += wxT("\nFROM \"");
      wxString aliasTable2 = aliasTable;
      MyFrame::DoubleQuoted(aliasTable2);
      insertFromSql += aliasTable2;
      insertFromSql += wxT("\";\n");
      insertSql += insertFromSql;
    }
  sqlite3_free_table(results);

  DropRenameAux3(obj, new_column, &Geometries, &Index, addGeometrySql);

// setting up the Index SQL fragments
  Index.Invalidate(obj->GetColName());
  pI = Index.GetFirst();
  while (pI)
    {
      dropIndexSql += wxT("DROP INDEX \"");
      xname = (char *) malloc((pI->GetIndexName().Len() * 4) + 1);
      strcpy(xname, pI->GetIndexName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      dropIndexSql += wxString::FromUTF8(xname2);
      free(xname2);
      dropIndexSql += wxT("\";\n");
      if (pI->IsValid() == true)
        {
          if (pI->GetFirst())
            {
              if (pI->IsUnique() == true)
                createIndexSql += wxT("CREATE UNIQUE INDEX \"");
              else
                createIndexSql += wxT("CREATE INDEX \"");
              xname = (char *) malloc((pI->GetIndexName().Len() * 4) + 1);
              strcpy(xname, pI->GetIndexName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              createIndexSql += wxString::FromUTF8(xname2);
              free(xname2);
              createIndexSql += wxT("\" ON \"");
              xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
              strcpy(xname, obj->GetMainName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              createIndexSql += wxString::FromUTF8(xname2);
              free(xname2);
              createIndexSql += wxT("\" (");
            }
          comma = false;
          pC = pI->GetFirst();
          while (pC)
            {
              if (comma == true)
                createIndexSql += wxT(", ");
              xname = (char *) malloc((pC->GetColumnName().Len() * 4) + 1);
              strcpy(xname, pC->GetColumnName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              createIndexSql += wxString::FromUTF8(xname2);
              free(xname2);
              comma = true;
              pC = pC->GetNext();
            }
          if (pI->GetFirst())
            createIndexSql += wxT("\");\n");
        }
      pI = pI->GetNext();
    }

// setting up the SQL complex statement
  sql = wxT("BEGIN;\n");
  sql += disableSpatialIdxSql;
  sql += discardGeometrySql;
  sql += dropSpatialIdxSql;
  sql += dropIndexSql;
  sql += renameSql;
  sql += createSql;
  sql += addGeometrySql;
  sql += createSpatialIdxSql;
  sql += createIndexSql;
  sql += insertSql;
  sql += dropSql;
  sql += wxT("COMMIT;");
  if (sql.Len() < 1)
    return;
  msg = wxT("Do you really intend to drop the Column ");
  msg += obj->GetColName();
  msg += wxT("\nfrom the Table ");
  msg += obj->GetMainName();
  msg += wxT(" ?");
  wxMessageDialog confirm(this, msg, wxT("Confirming DROP COLUMN"),
                          wxOK | wxCANCEL | wxICON_QUESTION);
  ret = confirm.ShowModal();
  if (ret != wxID_OK)
    return;

// executing
  ::wxBeginBusyCursor();
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      goto rollback;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT("The column ") + obj->GetColName() +
               wxT("\nwas successfully removed\nfrom the Table ") +
               obj->GetMainName(), wxT("spatialite_gui"),
               wxOK | wxICON_INFORMATION, this);
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
  return;
rollback:
  ret = sqlite3_exec(MainFrame->GetSqlite(), "ROLLBACK", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT
               ("An error occurred\n\na ROLLBACK was automatically performed"),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdRenameColumn(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - rename column
//
  char column[1024];
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  wxString createSql;
  wxString insertSql;
  wxString insertFromSql;
  wxString renameSql;
  wxString dropSql;
  wxString dropIndexSql;
  wxString createIndexSql;
  wxString disableSpatialIdxSql;
  wxString dropSpatialIdxSql;
  wxString createSpatialIdxSql;
  wxString discardGeometrySql;
  wxString addGeometrySql;
  wxString geomColumn;
  bool comma = false;
  char *value;
  bool autoincrement = false;
  wxString aliasTable;
  bool isGeom;
  wxString msg;
  GeomColsList Geometries;
  GeomColumn *pG;
  TblIndexList Index;
  TblIndex *pI;
  IndexColumn *pC;
  wxString name;
  char *xname;
  char *xname2;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  wxCommandEvent evt =
    wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  if (obj == NULL)
    return;

// asking the new column name
  wxString newColumn =
    wxGetTextFromUser(wxT("Please, insert the new Column Name"),
                      wxT("Rename Column"), obj->GetColName(),
                      MainFrame, wxDefaultCoord, wxDefaultCoord, false);

  strcpy(column, obj->GetColName().ToUTF8());
  if (DropRenameAux1(obj, &Geometries, &autoincrement) == false)
    return;
  DropRenameAux2(obj, &Geometries, aliasTable, newColumn, renameSql, dropSql,
                 disableSpatialIdxSql, dropSpatialIdxSql, createSpatialIdxSql,
                 discardGeometrySql);

// retrieving the Column names
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
      createSql = wxT("CREATE TABLE \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      createSql += wxString::FromUTF8(xname2);
      free(xname2);
      createSql += wxT("\" (\n");
      insertSql = wxT("INSERT INTO \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      insertSql += wxString::FromUTF8(xname2);
      free(xname2);
      insertSql += wxT("\" (");
      insertFromSql = wxT("SELECT ");
      for (i = 1; i <= rows; i++)
        {
          value = results[(i * columns) + 1];
          isGeom = false;
          pG = Geometries.GetFirst();
          while (pG)
            {
              char geom[1024];
              strcpy(geom, pG->GetGeometryName().ToUTF8());
              if (strcasecmp(geom, value) == 0)
                {
                  isGeom = true;
                  geomColumn = pG->GetGeometryName();
                  break;
                }
              pG = pG->GetNext();
            }
          if (comma == true)
            {
              if (isGeom == false)
                createSql += wxT(",\n");
              insertSql += wxT(", ");
              insertFromSql += wxT(", ");
            }
          if (isGeom == false)
            {
              if (strcasecmp(value, column) == 0)
                {
                  xname = (char *) malloc((newColumn.Len() * 4) + 1);
                  strcpy(xname, newColumn.ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  createSql +=
                    wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
                  free(xname2);
              } else
                {
                  xname = (char *) malloc(strlen(value) + 1);
                  strcpy(xname, value);
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  createSql +=
                    wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
                  free(xname2);
                }
              createSql += wxT(" ");
            }
          if (strcasecmp(value, column) == 0)
            {
              xname = (char *) malloc((newColumn.Len() * 4) + 1);
              strcpy(xname, newColumn.ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              insertSql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
              free(xname2);
          } else
            {
              xname = (char *) malloc(strlen(value) + 1);
              strcpy(xname, value);
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              insertSql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
              free(xname2);
            }
          xname = (char *) malloc(strlen(value) + 1);
          strcpy(xname, value);
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          insertFromSql += wxT("\"") + wxString::FromUTF8(xname2) + wxT("\"");
          free(xname2);
          value = results[(i * columns) + 2];
          if (isGeom == false)
            createSql += wxT("") + wxString::FromUTF8(value) + wxT("");
          value = results[(i * columns) + 5];
          if (value)
            {
              if (atoi(value) != 0)
                {
                  if (isGeom == false)
                    {
                      createSql += wxT(" PRIMARY KEY");
                      if (autoincrement == true)
                        createSql += wxT(" AUTOINCREMENT");
                    }
                }
            }
          value = results[(i * columns) + 3];
          if (value)
            {
              if (atoi(value) != 0)
                {
                  if (isGeom == true)
                    Geometries.SetNotNull(geomColumn);
                  else
                    createSql += wxT(" NOT NULL");
                }
            }
          value = results[(i * columns) + 4];
          if (value && isGeom == false)
            {
              createSql += wxT(" DEFAULT ");
              createSql += wxString::FromUTF8(value);
            }
          comma = true;
        }
      createSql += wxT(");\n");
      insertSql += wxT(")\n");
      insertFromSql += wxT("\nFROM \"");
      wxString aliasTable2 = aliasTable;
      MyFrame::DoubleQuoted(aliasTable2);
      insertFromSql += aliasTable2;
      insertFromSql += wxT("\";\n");
      insertSql += insertFromSql;
    }
  sqlite3_free_table(results);

  DropRenameAux3(obj, newColumn, &Geometries, &Index, addGeometrySql);

// setting up the Index SQL fragments
  pI = Index.GetFirst();
  while (pI)
    {
      dropIndexSql += wxT("DROP INDEX \"");
      xname = (char *) malloc((pI->GetIndexName().Len() * 4) + 1);
      strcpy(xname, pI->GetIndexName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      dropIndexSql += wxString::FromUTF8(xname2);
      free(xname2);
      dropIndexSql += wxT("\";\n");
      if (pI->IsValid() == true)
        {
          if (pI->GetFirst())
            {
              if (pI->IsUnique() == true)
                createIndexSql += wxT("CREATE UNIQUE INDEX \"");
              else
                createIndexSql += wxT("CREATE INDEX \"");
              xname = (char *) malloc((pI->GetIndexName().Len() * 4) + 1);
              strcpy(xname, pI->GetIndexName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              createIndexSql += wxString::FromUTF8(xname2);
              free(xname2);
              createIndexSql += wxT("\" ON \"");
              xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
              strcpy(xname, obj->GetMainName().ToUTF8());
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              createIndexSql += wxString::FromUTF8(xname2);
              free(xname2);
              createIndexSql += wxT("\" (");
            }
          comma = false;
          pC = pI->GetFirst();
          while (pC)
            {
              char xvalue[1024];
              strcpy(xvalue, pC->GetColumnName().ToUTF8());
              if (comma == true)
                createIndexSql += wxT(", ");
              if (strcasecmp(xvalue, column) == 0)
                {
                  xname = (char *) malloc((newColumn.Len() * 4) + 1);
                  strcpy(xname, newColumn.ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  createIndexSql += wxString::FromUTF8(xname2);
                  free(xname2);
              } else
                {
                  xname = (char *) malloc((pC->GetColumnName().Len() * 4) + 1);
                  strcpy(xname, pC->GetColumnName().ToUTF8());
                  xname2 = gaiaDoubleQuotedSql(xname);
                  free(xname);
                  createIndexSql += wxString::FromUTF8(xname2);
                  free(xname2);
                }
              comma = true;
              pC = pC->GetNext();
            }
          if (pI->GetFirst())
            createIndexSql += wxT(");\n");
        }
      pI = pI->GetNext();
    }

// setting up the SQL complex statement
  sql = wxT("BEGIN;\n");
  sql += disableSpatialIdxSql;
  sql += discardGeometrySql;
  sql += dropSpatialIdxSql;
  sql += dropIndexSql;
  sql += renameSql;
  sql += createSql;
  sql += addGeometrySql;
  sql += createSpatialIdxSql;
  sql += createIndexSql;
  sql += insertSql;
  sql += dropSql;
  sql += wxT("COMMIT;");
  if (sql.Len() < 1)
    return;
  msg = wxT("Do you really intend to rename the Column ");
  msg += obj->GetColName();
  msg += wxT(" as ");
  msg += newColumn;
  msg += wxT("\ninto the Table ");
  msg += obj->GetMainName();
  msg += wxT(" ?");
  wxMessageDialog confirm(this, msg, wxT("Confirming RENAME COLUMN"),
                          wxOK | wxCANCEL | wxICON_QUESTION);
  ret = confirm.ShowModal();
  if (ret != wxID_OK)
    return;

// executing
  ::wxBeginBusyCursor();
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  ret = sqlite3_exec(MainFrame->GetSqlite(), xsql, NULL, NULL, &errMsg);
  free(xsql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      goto rollback;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT("The column ") + obj->GetColName() +
               wxT("\nwas successfully renamed as ") + newColumn +
               wxT("\ninto the Table ") + obj->GetMainName(),
               wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
  return;
rollback:
  ret = sqlite3_exec(MainFrame->GetSqlite(), "ROLLBACK", NULL, NULL, &errMsg);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      return;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT
               ("An error occurred\n\na ROLLBACK was automatically performed"),
               wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdEdit(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - editing row values
//
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  wxString sql;
  char *xsql;
  char *column;
  char *type;
  int pk = 0;
  int pb = 0;
  int primaryKeys[1024];
  int blobCols[1024];
  char *xname;
  char *xname2;
  bool isView = false;
  for (i = 0; i < 1024; i++)
    {
      primaryKeys[i] = -1;
      blobCols[i] = -1;
    }
  primaryKeys[pk++] = 0;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_POSTGRES_TABLE || obj->GetType() == MY_POSTGIS_VIEW)
    {
      // special case: PostgreSQL table
      bool hasGeom = false;
      wxString viewName;
      if (obj->GetType() == MY_POSTGRES_TABLE)
        {
          MyPostgresTable *table =
            MainFrame->FindPostgresTable(obj->GetVirtName());
          if (table->GetFirst() != NULL)
            hasGeom = true;
          viewName = table->GetPostGisName();
        }
      if (hasGeom == true)
        {
          int ret = wxMessageBox(wxT
                                 ("Directly editing a PostgreSQL table containing PostGIS geometries "
                                  "is a strongly discouraged operation.\n\n"
                                  "You should instead edit the corresponding Spatial View: ")
                                 + viewName + wxT("\n\n") +
                                 wxT("Do you really intend to continue ?"),
                                 wxT("spatialite_gui"),
                                 wxYES_NO | wxICON_QUESTION, this);
          if (ret != wxYES)
            return;
        }
      sql = wxT("PRAGMA table_info(\"");
      xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(xname, obj->GetVirtName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      int ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
          sql = wxT("SELECT ROWID, ");
          for (i = 1; i <= rows; i++)
            {
              column = results[(i * columns) + 1];
              sql += wxT("\"");
              xname = (char *) malloc(strlen(column) + 1);
              strcpy(xname, column);
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              sql += wxString::FromUTF8(xname2);
              sql += wxT("\"");
              free(xname2);
              type = results[(i * columns) + 2];
              if (i < rows)
                sql += wxT(", ");
            }
        }
      sqlite3_free_table(results);
      if (sql.Len() < 1)
        return;
      sql += wxT("\nFROM \"");
      xname = (char *) malloc((obj->GetVirtName().Len() * 4) + 1);
      strcpy(xname, obj->GetVirtName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      if (isView != true)
        sql += wxT("\"\nORDER BY ROWID");
      else
        sql += wxT("\"");
      wxString prefix = wxT("MAIN");
      wxString noGeom;
      MainFrame->InitializeSqlFilters(prefix, obj->GetVirtName(), false,
                                      noGeom);
      MainFrame->EditTable(sql, primaryKeys, blobCols, obj->GetVirtName());
      return;
    }
  isView = MainFrame->IsView(obj->GetMainName());
  sql = wxT("PRAGMA table_info(\"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  sql += wxT("\")");
  xsql = (char *) malloc((sql.Len() * 4) + 1);
  strcpy(xsql, sql.ToUTF8());
  int ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
      if (isView == true)
        sql = wxT("SELECT ");
      else
        sql = wxT("SELECT ROWID, ");
      for (i = 1; i <= rows; i++)
        {
          column = results[(i * columns) + 1];
          sql += wxT("\"");
          xname = (char *) malloc(strlen(column) + 1);
          strcpy(xname, column);
          xname2 = gaiaDoubleQuotedSql(xname);
          free(xname);
          sql += wxString::FromUTF8(xname2);
          sql += wxT("\"");
          free(xname2);
          type = results[(i * columns) + 2];
          if (strcasecmp(type, "BLOB") == 0)
            blobCols[pb++] = i;
          if (atoi(results[(i * columns) + 5]) == 0)
            ;
          else
            primaryKeys[pk++] = i;
          if (i < rows)
            sql += wxT(", ");
        }
    }
  sqlite3_free_table(results);
  if (sql.Len() < 1)
    return;

  sql += wxT("\nFROM \"");
  xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(xname, obj->GetMainName().ToUTF8());
  xname2 = gaiaDoubleQuotedSql(xname);
  free(xname);
  sql += wxString::FromUTF8(xname2);
  free(xname2);
  if (isView != true)
    sql += wxT("\"\nORDER BY ROWID");
  else
    sql += wxT("\"");
  wxString noGeom;
  MainFrame->InitializeSqlFilters(obj->GetDbAlias(), obj->GetMainName(), false,
                                  noGeom);
  MainFrame->EditTable(sql, primaryKeys, blobCols, obj->GetMainName());
}

void MyTableTree::OnCmdCheckDuplicates(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Checking for Duplicate rows
//
  wxString sql;
  char *xsql;
  wxString col_list;
  bool first = true;
  char *xname;
  char *xname2;
  int pk;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *errMsg = NULL;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      // extracting the column names (excluding any Primary Key)
      sql = wxT("PRAGMA table_info(");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT(")");
      xsql = (char *) malloc((sql.Len() * 4) + 1);
      strcpy(xsql, sql.ToUTF8());
      ret = sqlite3_get_table(MainFrame->GetSqlite(), xsql, &results,
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
              const char *xnm = results[(i * columns) + 1];
              int len = strlen(xnm);
              xname = (char *) malloc(len + 1);
              strcpy(xname, xnm);
              xname2 = gaiaDoubleQuotedSql(xname);
              free(xname);
              pk = atoi(results[(i * columns) + 5]);
              if (!pk)
                {
                  if (first)
                    first = false;
                  else
                    col_list += wxT(", ");
                  col_list += wxString::FromUTF8(xname2);
                }
              free(xname2);
            }
        }
      sqlite3_free_table(results);
      // preparing the SQL statement
      sql = wxT("SELECT Count(*) AS \"[dupl-count]\", ");
      sql += col_list;
      sql += wxT("\nFROM \"");
      xname = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
      strcpy(xname, obj->GetMainName().ToUTF8());
      xname2 = gaiaDoubleQuotedSql(xname);
      free(xname);
      sql += wxString::FromUTF8(xname2);
      free(xname2);
      sql += wxT("\"\nGROUP BY ");
      sql += col_list;
      sql += wxT("\nHAVING \"[dupl-count]\" > 1");
      sql += wxT("\nORDER BY \"[dupl-count]\" DESC");
      MainFrame->GetRsView()->ResetEmpty();
      MainFrame->SetSql(sql, true);
    }
}

void MyTableTree::OnCmdRemoveDuplicates(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Removing Duplicate rows
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TABLE)
    {
      wxString msg;
      int count = 0;
      char *table = new char[obj->GetMainName().Len() + 1];
      strcpy(table, obj->GetMainName().ToUTF8());
      ::wxBeginBusyCursor();
      remove_duplicated_rows_ex2(MainFrame->GetSqlite(), table, &count, 1);
      delete[]table;
      ::wxEndBusyCursor();
      if (!count)
        {
          msg = wxT("No duplicated rows have been identified on ");
          msg += obj->GetMainName();
          wxMessageBox(msg, wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
      } else
        {
          char dummy[128];
          sprintf(dummy, "%d duplicated rows deleted from ", count);
          msg = wxString::FromUTF8(dummy);
          msg += obj->GetMainName();
          wxMessageBox(msg, wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
}

void MyTableTree::OnCmdCheckGeometries(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Checking a Geometry Column
//
  int ret;
  wxString table;
  wxString geom;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY
      || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED)
    {
      table = obj->GetMainName();
      geom = obj->GetColName();
  } else
    return;
  CheckGeometryDialog dlg;
  dlg.Create(MainFrame, table, geom);
  ret = dlg.ShowModal();
  if (ret != wxYES)
    return;

  char xtable[1024];
  char xgeometry[1024];
  char *err_msg = NULL;
  char report_path[1024];
  wxString msg;
  strcpy(xtable, table.ToUTF8());
  strcpy(xgeometry, geom.ToUTF8());

  wxFileDialog fileDialog(this, wxT("Diagnostic Report"),
                          wxT(""), wxT("report.html"),
                          wxT
                          ("HTML document (*.html)|*.html|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(report_path, fileDialog.GetPath().ToUTF8());
      ::wxBeginBusyCursor();
      int n_invalids;
      ret =
        check_geometry_column_r(MainFrame->GetSpliteInternalCache(),
                                MainFrame->GetSqlite(), xtable, xgeometry,
                                report_path, NULL, &n_invalids, &err_msg);
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
          if (n_invalids > 0)
            {
              msg =
                wxT
                ("ATTENTION: some invalid Geometries have been detected !!!\n\n");
              mode = wxICON_WARNING;
          } else
            {
              msg =
                wxT
                ("No invalid Geometries have been detected; this layer is full valid\n\n");
              mode = wxICON_INFORMATION;
            }
          msg += wxT("A full diagnostic report has been created.\n");
          msg +=
            wxT
            ("Please point your WEB Browser at the following HTML document containing the report:\n\n");
          msg += fileDialog.GetPath();
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | mode, this);
        }
    }
}

void MyTableTree::OnCmdSanitizeGeometries(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Attempting to sanitize a Geometry Column
//
  int ret;
  wxString table;
  wxString geom;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_GEOMETRY
      || obj->GetType() == MY_GEOMETRY_INDEX
      || obj->GetType() == MY_GEOMETRY_CACHED)
    {
      table = obj->GetMainName();
      geom = obj->GetColName();
  } else
    return;
  SanitizeGeometryDialog dlg;
  dlg.Create(MainFrame, table, geom);
  ret = dlg.ShowModal();
  if (ret != wxYES)
    return;

  char tmp_prefix[1024];
  char xtable[1024];
  char xgeometry[1024];
  char *err_msg = NULL;
  char report_path[1024];
  wxString msg;
  strcpy(tmp_prefix, dlg.GetTmpPrefix().ToUTF8());
  strcpy(xtable, table.ToUTF8());
  strcpy(xgeometry, geom.ToUTF8());

  wxFileDialog fileDialog(this, wxT("Diagnostic Report"),
                          wxT(""), wxT("report.html"),
                          wxT
                          ("HTML document (*.html)|*.html|All files (*.*)|*.*"),
                          wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      strcpy(report_path, fileDialog.GetPath().ToUTF8());
      ::wxBeginBusyCursor();
      int n_failures;
      ret =
        sanitize_geometry_column_r(MainFrame->GetSpliteInternalCache(),
                                   MainFrame->GetSqlite(), xtable, xgeometry,
                                   tmp_prefix, report_path, NULL, NULL, NULL,
                                   &n_failures, &err_msg);
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
          if (n_failures > 0)
            {
              msg =
                wxT
                ("ATTENTION: some invalid Geometries still remain invalid !!!\n\n");
              mode = wxICON_WARNING;
          } else
            {
              msg =
                wxT
                ("All invalid Geometries have been saned; this layer is now full valid\n\n");
              mode = wxICON_INFORMATION;
            }
          msg += wxT("A full diagnostic report has been created.\n");
          msg +=
            wxT
            ("Please point your WEB Browser at the following HTML document containing the report:\n\n");
          msg += fileDialog.GetPath();
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | mode, this);
        }
    }
}

void MyTableTree::OnCmdSldSeRasterStyles(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Raster Coverage SLD/SE styles
//
  RasterCoverageStylesDialog dlg;
  dlg.Create(MainFrame, CurrentRasterCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnCmdImportRaster(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Importing one (or even more) external Raster file(s)
//
  int ret;
  wxString lastDir;
  wxString path;
  ImportRasterDialog dlg;
  wxString suffixList =
    wxT("Raster files (*.tif;*.jpg;*.jp2;*.asc)|*.tif;*.jpg;*.jp2;*.asc|");
  suffixList += wxT("TIFF or GeoTIFF image (*.tif)|*.tif|");
  suffixList += wxT("JPEG image (*.jpg)|*.jpg|");
  suffixList += wxT("JPEG2000 image (*.jp2)|*.jp2|");
  suffixList += wxT("ASCII Grid (*.asc)|*.asc|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Selecting Raster File(s) to be imported"),
                          wxT(""), wxT(""), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      int count;
      wxArrayString paths;
      fileDialog.GetPaths(paths);
      count = paths.GetCount();
      path = paths.Item(0);
      int more = count - 1;
      if (count >= 2)
        {
          path += wxT("\n") + paths.Item(1);
          more--;
        }
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      if (more > 0)
        {
          char dummy[128];
          sprintf(dummy, "\n(and %d more %s)", more,
                  (more > 1) ? "files" : "file");
          path += wxString::FromUTF8(dummy);
        }
      wxString title;
      wxString abstract;
      wxString copyright;
      wxString data_license;
      wxString sample;
      wxString pixel;
      wxString compression;
      int srid;
      bool mixed_resolutions;

      wxString coverage = CurrentRasterCoverageName;
      if (MainFrame->DoGetRasterCoverageInfos
          (coverage, title, abstract, copyright, data_license, sample, pixel,
           compression, &srid, &mixed_resolutions) == false)
        return;
      dlg.Create(MainFrame, coverage, paths, path, title, abstract, sample,
                 pixel, compression, srid);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdPyramidize(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Pyramidize
//
  PyramidizeDialog dlg;
  wxString title;
  wxString abstract;
  wxString copyright;
  wxString data_license;
  wxString sample;
  wxString pixel;
  wxString compression;
  int srid;
  bool mixed_resolutions;
  wxString coverage = CurrentRasterCoverageName;
  if (MainFrame->DoGetRasterCoverageInfos
      (coverage, title, abstract, copyright, data_license, sample, pixel,
       compression, &srid, &mixed_resolutions) == false)
    return;
  dlg.Create(MainFrame, coverage, title, abstract, sample, pixel, compression);
  dlg.ShowModal();
}

void MyTableTree::OnCmdPyramidizeMonolithic(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - PyramidizeMonolithic
//
  PyramidizeMonolithicDialog dlg;
  wxString title;
  wxString abstract;
  wxString copyright;
  wxString data_license;
  wxString sample;
  wxString pixel;
  wxString compression;
  int srid;
  bool mixed_resolutions;
  wxString coverage = CurrentRasterCoverageName;
  if (MainFrame->DoGetRasterCoverageInfos
      (coverage, title, abstract, copyright, data_license, sample, pixel,
       compression, &srid, &mixed_resolutions) == false)
    return;
  if (mixed_resolutions)
    {
      wxMessageBox(wxT
                   ("Coverage \"")
                   + coverage + wxT("\" declares MixedResolutions.\n\n") +
                   wxT("Building a Monolithic Pyramid is not possible, only\n")
                   +
                   wxT("Section Pyramids are supported for MixedResolutions."),
                   wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
      return;
    }
  dlg.Create(MainFrame, coverage, title, abstract, sample, pixel, compression);
  dlg.ShowModal();
}

void MyTableTree::OnCmdDePyramidize(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - DePyramidize
//
  DePyramidizeDialog dlg;
  wxString title;
  wxString abstract;
  wxString copyright;
  wxString data_license;
  wxString sample;
  wxString pixel;
  wxString compression;
  int srid;
  bool mixed_resolutions;
  wxString coverage = CurrentRasterCoverageName;
  if (MainFrame->DoGetRasterCoverageInfos
      (coverage, title, abstract, copyright, data_license, sample, pixel,
       compression, &srid, &mixed_resolutions) == false)
    return;
  dlg.Create(MainFrame, coverage, title, abstract, sample, pixel, compression);
  dlg.ShowModal();
}

void MyTableTree::OnCmdRasterDrop(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Drop Raster Coverage
//
  RasterDropDialog dlg;
  wxString title;
  wxString abstract;
  wxString copyright;
  wxString data_license;
  wxString sample;
  wxString pixel;
  wxString compression;
  int srid;
  bool mixed_resolutions;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  wxString coverage = CurrentRasterCoverageName;
  if (MainFrame->DoGetRasterCoverageInfos
      (coverage, title, abstract, copyright, data_license, sample, pixel,
       compression, &srid, &mixed_resolutions) == false)
    return;
  dlg.Create(MainFrame, coverage, title, abstract, copyright, data_license,
             sample, pixel, compression);
  dlg.ShowModal();
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
}

void MyTableTree::OnCmdRasterInfos(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Raster Coverage Infos
//
  MainFrame->DoCreateStylingTables();
  RasterInfosDialog dlg;
  dlg.Create(MainFrame, CurrentRasterCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnRegisterVectorCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Register Vector Coverage - SpatialTable
//
  VectorRegisterDialog dlg;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  MainFrame->DoCreateStylingTables();
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      wxString name = dlg.GetCoverageName();
      wxString table = dlg.GetTableName();
      wxString geometry = dlg.GetGeometryColumn();
      wxString title = dlg.GetTitle();
      wxString abstract = dlg.GetAbstract();
      wxString copyright = dlg.GetCopyright();
      wxString license = dlg.GetDataLicense();
      bool isQueryable = dlg.IsQueryable();
      bool isEditable = dlg.IsEditable();
      if (MainFrame->DoRegisterVectorCoverage(name, table, geometry, title,
                                              abstract, copyright, license,
                                              isQueryable, isEditable) != true)
        {
          wxMessageBox(wxT("Sorry, some fatal error occurred."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
      } else
        {
          wxMessageBox(wxT("Vector Coverage \"") + name +
                       wxT("\" successfully registered"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
      // appending a delayed event so to really update the Tree
      AddPendingEvent(evt);
    }
}

void MyTableTree::
OnRegisterSpatialViewCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Register Vector Coverage - SpatialView
//
  SpatialViewRegisterDialog dlg;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  MainFrame->DoCreateStylingTables();
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      wxString name = dlg.GetCoverageName();
      wxString view = dlg.GetViewName();
      wxString geometry = dlg.GetViewGeometry();
      wxString title = dlg.GetTitle();
      wxString abstract = dlg.GetAbstract();
      wxString copyright = dlg.GetCopyright();
      wxString license = dlg.GetDataLicense();
      bool isQueryable = dlg.IsQueryable();
      bool isEditable = dlg.IsEditable();
      if (MainFrame->DoRegisterSpatialViewCoverage(name, view, geometry, title,
                                                   abstract, copyright, license,
                                                   isQueryable,
                                                   isEditable) != true)
        {
          wxMessageBox(wxT("Sorry, some fatal error occurred."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
      } else
        {
          wxMessageBox(wxT("Vector Coverage \"") + name +
                       wxT("\" successfully registered"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
      // appending a delayed event so to really update the Tree
      AddPendingEvent(evt);
    }
}

void MyTableTree::
OnRegisterVirtualTableCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Register Vector Coverage - VirtualTable
//
  VirtualTableRegisterDialog dlg;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  MainFrame->DoCreateStylingTables();
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      wxString name = dlg.GetCoverageName();
      wxString table = dlg.GetVirtName();
      wxString geometry = dlg.GetVirtGeometry();
      wxString title = dlg.GetTitle();
      wxString abstract = dlg.GetAbstract();
      wxString copyright = dlg.GetCopyright();
      wxString license = dlg.GetDataLicense();
      bool isQueryable = dlg.IsQueryable();
      if (MainFrame->DoRegisterVirtualTableCoverage
          (name, table, geometry, title, abstract, copyright, license,
           isQueryable) != true)
        {
          wxMessageBox(wxT("Sorry, some fatal error occurred."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
      } else
        {
          wxMessageBox(wxT("Vector Coverage \"") + name +
                       wxT("\" successfully registered"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
      // appending a delayed event so to really update the Tree
      AddPendingEvent(evt);
    }
}

void MyTableTree::OnRegisterTopoGeoCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Register Topology-Geometry Coverage
//
  TopoGeoRegisterDialog dlg;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  MainFrame->DoCreateStylingTables();
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      wxString name = dlg.GetCoverageName();
      wxString topology = dlg.GetTopologyName();
      wxString title = dlg.GetTitle();
      wxString abstract = dlg.GetAbstract();
      wxString copyright = dlg.GetCopyright();
      wxString license = dlg.GetDataLicense();
      bool isQueryable = dlg.IsQueryable();
      bool isEditable = dlg.IsEditable();
      if (MainFrame->DoRegisterTopoGeoCoverage(name, topology, title,
                                               abstract, copyright, license,
                                               isQueryable, isEditable) != true)
        {
          wxMessageBox(wxT("Sorry, some fatal error occurred."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
      } else
        {
          wxMessageBox(wxT("topoGeo Coverage \"") + name +
                       wxT("\" successfully registered"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
      // appending a delayed event so to really update the Tree
      AddPendingEvent(evt);
    }
}

void MyTableTree::OnRegisterTopoNetCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Register Topology-Network Coverage
//
  TopoNetRegisterDialog dlg;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  MainFrame->DoCreateStylingTables();
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      wxString name = dlg.GetCoverageName();
      wxString network = dlg.GetNetworkName();
      wxString title = dlg.GetTitle();
      wxString abstract = dlg.GetAbstract();
      wxString copyright = dlg.GetCopyright();
      wxString license = dlg.GetDataLicense();
      bool isQueryable = dlg.IsQueryable();
      bool isEditable = dlg.IsEditable();
      if (MainFrame->DoRegisterTopoNetCoverage(name, network, title,
                                               abstract, copyright, license,
                                               isQueryable, isEditable) != true)
        {
          wxMessageBox(wxT("Sorry, some fatal error occurred."),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
          return;
      } else
        {
          wxMessageBox(wxT("topoNet Coverage \"") + name +
                       wxT("\" successfully registered"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
      // appending a delayed event so to really update the Tree
      AddPendingEvent(evt);
    }
}

void MyTableTree::OnCmdVectorUnregister(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Unregister Vector Coverage
//
  VectorUnregisterDialog dlg;
  wxString title;
  wxString abstract;
  wxString copyright;
  wxString data_license;
  int origin;
  wxString type;
  int srid;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  wxString coverage = CurrentVectorCoverageName;
  if (MainFrame->DoGetVectorCoverageInfos
      (coverage, title, abstract, copyright, data_license, &origin, type,
       &srid) == false)
    return;
  dlg.Create(MainFrame, coverage, title, abstract, copyright, data_license,
             origin, type);
  dlg.ShowModal();
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
}

void MyTableTree::OnCmdNewRasterStyle(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new Raster Style creation required
//
  int ret;
  wxString lastDir;
  wxString path;
  LoadRasterStyleDialog dlg;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Add New SLD/SE Raster Style(s)"),
                          wxT(""), wxT("style.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      int count;
      wxArrayString paths;
      fileDialog.GetPaths(paths);
      count = paths.GetCount();
      path = paths.Item(0);
      int more = count - 1;
      if (count >= 2)
        {
          path += wxT("\n") + paths.Item(1);
          more--;
        }
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      if (more > 0)
        {
          char dummy[128];
          sprintf(dummy, "\n(and %d more %s)", more,
                  (more > 1) ? "files" : "file");
          path += wxString::FromUTF8(dummy);
        }
      dlg.Create(MainFrame, paths, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdReloadRasterStyle(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - reloading a Raster Style was required
//
  int ret;
  wxString lastDir;
  wxString path;
  ReloadRasterStyleDialog dlg;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this,
                          wxT
                          ("Reloading an already existing SLD/SE Raster Style"),
                          wxT(""), wxT("style.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      dlg.Create(MainFrame, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdUnregisterRasterStyle(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - UnRegistering a Raster Style was required
//
  UnregisterRasterStyleDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdNewMapConfig(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new Map Configuration creation required
//
  int ret;
  wxString lastDir;
  wxString path;
  LoadMapConfigDialog dlg;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Add New XML Map Configuration(s)"),
                          wxT(""), wxT("config.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      int count;
      wxArrayString paths;
      fileDialog.GetPaths(paths);
      count = paths.GetCount();
      path = paths.Item(0);
      int more = count - 1;
      if (count >= 2)
        {
          path += wxT("\n") + paths.Item(1);
          more--;
        }
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      if (more > 0)
        {
          char dummy[128];
          sprintf(dummy, "\n(and %d more %s)", more,
                  (more > 1) ? "files" : "file");
          path += wxString::FromUTF8(dummy);
        }
      dlg.Create(MainFrame, paths, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdReloadMapConfig(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - reloading a Map Configuration was required
//
  int ret;
  wxString lastDir;
  wxString path;
  ReloadMapConfigDialog dlg;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this,
                          wxT
                          ("Reloading an already existing XML Map Configuration"),
                          wxT(""), wxT("config.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      dlg.Create(MainFrame, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdUnregisterMapConfig(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - UnRegistering a Map Configuration was required
//
  UnregisterMapConfigDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdVerifyExternalMapConfig(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Verifying an External Map Configuration
//
  wxString report;
  unsigned char *xml = NULL;
  int ret;
  wxString lastDir;
  wxString path;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this,
                          wxT
                          ("Verifying an XML Map Configuration"),
                          wxT(""), wxT("style.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      wxFile in(path);
      if (in.IsOpened())
        {
          int len = in.Length();
          xml = (unsigned char *) malloc(len + 1);
          in.Read(xml, len);
          *(xml + len) = '\0';
          in.Close();
        }
    }
  if (xml != NULL)
    {
      // verifying the XML Map Configuration
      DoVerifyMapConfig(xml, report);
      free(xml);
      MapConfigHtmlDialog html;
      html.Create(MainFrame, report);
      html.ShowModal();
    }
}

void MyTableTree::
OnCmdVerifyRegisteredMapConfig(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Verifying aRegistered Map Configuration
//
  wxString report;
  VerifyMapConfigDialog dlg;
  dlg.Create(MainFrame);
  if (dlg.ShowModal() == wxID_OK)
    {
      // verifying the XML Map Configuration
      DoVerifyMapConfig(dlg.GetXML(), report);
    }
  MapConfigHtmlDialog html;
  html.Create(MainFrame, report);
  html.ShowModal();
}

void MyTableTree::OnCmdRasterSRIDs(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Raster Coverage alternative SRIDs
//
  RasterSRIDsDialog dlg;
  dlg.Create(MainFrame, CurrentRasterCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnCmdRasterKeywords(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Raster Coverage Keywords
//
  RasterKeywordsDialog dlg;
  dlg.Create(MainFrame, CurrentRasterCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnCmdRegisterExternalGraphic(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new External Graphic creation required
//
  int ret;
  wxString lastDir;
  wxString path;
  LoadExternalGraphicDialog dlg;
  wxString suffixList =
    wxT("Graphic resource (*.png;*.jpg;*.gif;*.svg)|*.png;*.jpg;*.gif;*.svg|");
  suffixList += wxT("PNG image (*.png)|*.png|");
  suffixList += wxT("JPEG image (*.jpg)|*.jpg|");
  suffixList += wxT("GIF image (*.gif)|*.gif|");
  suffixList += wxT("SVG symbol (*.svg)|*.svg|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Add New External Graphic resource(s)"),
                          wxT(""), wxT(""), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      int count;
      wxArrayString paths;
      fileDialog.GetPaths(paths);
      count = paths.GetCount();
      path = paths.Item(0);
      int more = count - 1;
      if (count >= 2)
        {
          path += wxT("\n") + paths.Item(1);
          more--;
        }
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      if (more > 0)
        {
          char dummy[128];
          sprintf(dummy, "\n(and %d more %s)", more,
                  (more > 1) ? "files" : "file");
          path += wxString::FromUTF8(dummy);
        }
      dlg.Create(MainFrame, paths, path);
      dlg.ShowModal();
    }
}

void MyTableTree::
OnCmdUnregisterExternalGraphic(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - UnRegistering an External Graphic was required
//
  UnregisterExternalGraphicDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdRegisterTextFont(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new Text Font creation required
//
  int ret;
  wxString lastDir;
  wxString path;
  LoadTextFontDialog dlg;
  wxString suffixList = wxT("Text Font (*.ttf)|*.ttf|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Add New Text Font(s)"),
                          wxT(""), wxT(""), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      int count;
      wxArrayString paths;
      fileDialog.GetPaths(paths);
      count = paths.GetCount();
      path = paths.Item(0);
      int more = count - 1;
      if (count >= 2)
        {
          path += wxT("\n") + paths.Item(1);
          more--;
        }
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      if (more > 0)
        {
          char dummy[128];
          sprintf(dummy, "\n(and %d more %s)", more,
                  (more > 1) ? "files" : "file");
          path += wxString::FromUTF8(dummy);
        }
      dlg.Create(MainFrame, paths, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdUnregisterTextFont(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - UnRegistering a Text Font was required
//
  MainFrame->DoCreateStylingTables();
  UnregisterTextFontDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdNewVectorStyle(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - new Vector Style creation required
//
  int ret;
  wxString lastDir;
  wxString path;
  LoadVectorStyleDialog dlg;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this, wxT("Add New SLD/SE Vector Style(s)"),
                          wxT(""), wxT("style.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE,
                          wxDefaultPosition, wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      int count;
      wxArrayString paths;
      fileDialog.GetPaths(paths);
      count = paths.GetCount();
      path = paths.Item(0);
      int more = count - 1;
      if (count >= 2)
        {
          path += wxT("\n") + paths.Item(1);
          more--;
        }
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      if (more > 0)
        {
          char dummy[128];
          sprintf(dummy, "\n(and %d more %s)", more,
                  (more > 1) ? "files" : "file");
          path += wxString::FromUTF8(dummy);
        }
      dlg.Create(MainFrame, paths, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdReloadVectorStyle(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - reloading a Vector Style was required
//
  int ret;
  wxString lastDir;
  wxString path;
  ReloadVectorStyleDialog dlg;
  wxString suffixList = wxT("XML Document (*.xml)|*.xml|");
  suffixList += wxT("All files (*.*)|*.*");
  wxFileDialog fileDialog(this,
                          wxT
                          ("Reloading an already existing SLD/SE Vector Style"),
                          wxT(""), wxT("style.xml"), suffixList,
                          wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition,
                          wxDefaultSize, wxT("filedlg"));
  lastDir = MainFrame->GetLastDirectory();
  if (lastDir.Len() >= 1)
    fileDialog.SetDirectory(lastDir);
  ret = fileDialog.ShowModal();
  if (ret == wxID_OK)
    {
      path = fileDialog.GetPath();
      wxFileName file(path);
      lastDir = file.GetPath();
      MainFrame->SetLastDirectory(lastDir);
      dlg.Create(MainFrame, path);
      dlg.ShowModal();
    }
}

void MyTableTree::OnCmdUnregisterVectorStyle(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - UnRegistering a Vector Style was required
//
  UnregisterVectorStyleDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerContrast(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerContrastDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerChannelRgb(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerChannelRgbDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerChannelGray(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerChannelGrayDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerCategorize(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerCategorizeDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerInterpolate(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerInterpolateDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerShadedRelief(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerShadedReliefDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::
OnCmdRasterSymbolizerMonochrome(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the RasterSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  RasterSymbolizerMonochromeDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdSimpleLineSymbolizer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the LineSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  SimpleLineSymbolizerDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdSimplePolygonSymbolizer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the PolygonSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  SimplePolygonSymbolizerDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdSimplePointSymbolizer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the PointSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  SimplePointSymbolizerDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

void MyTableTree::OnCmdSimpleTextSymbolizer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - the TextSymbolizer tool was invoked
//
  MainFrame->DoCreateStylingTables();
  SimpleTextSymbolizerDialog dlg;
  dlg.Create(MainFrame);
  dlg.ShowModal();
}

bool MyTableTree::GetCurrentlySelectedTable(wxString & table_name)
{
// attempting to retrieve the currenlty selected Table Name
  if (CurrentItem.IsOk() != true)
    return false;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  if (obj == NULL)
    return false;
  if (obj->GetType() == MY_TABLE && obj->IsAttached() != true)
    {
      table_name = obj->GetMainName();
      return true;
    }
  return false;
}

void MyTableTree::OnCmdSldSeVectorStyles(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Vector Coverage SLD/SE styles
//
  VectorCoverageStylesDialog dlg;
  dlg.Create(MainFrame, CurrentVectorCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnCmdVectorInfos(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Vector Coverage Infos
//
  MainFrame->DoCreateStylingTables();
  VectorInfosDialog dlg;
  dlg.Create(MainFrame, CurrentVectorCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnCmdVectorSRIDs(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Vector Coverage alternative SRIDs
//
  VectorSRIDsDialog dlg;
  if (dlg.Create(MainFrame, CurrentVectorCoverageName) == true)
    dlg.ShowModal();
}

void MyTableTree::OnCmdVectorKeywords(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Handling Vector Coverage Keywords
//
  VectorKeywordsDialog dlg;
  dlg.Create(MainFrame, CurrentVectorCoverageName);
  dlg.ShowModal();
}

void MyTableTree::OnCreateRasterCoverage(wxCommandEvent & WXUNUSED(event))
{
//
// creating a Raster Coverage
//
  int ret;
  CreateRasterCoverageDialog dlg;
  dlg.Create(MainFrame);
  ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      wxString CoverageName = dlg.GetCoverageName();
      wxString Title = dlg.GetTitle();
      wxString Abstract = dlg.GetAbstract();
      int SampleType = dlg.GetSampleType();
      int PixelType = dlg.GetPixelType();
      int NumBands = dlg.GetNumBands();
      int Compression = dlg.GetCompression();
      int Quality = dlg.GetQuality();
      int TileWidth = dlg.GetTileWidth();
      int TileHeight = dlg.GetTileHeight();
      bool NotGeoreferenced = dlg.IsNotGeoreferenced();
      int Srid = dlg.GetSRID();
      double HorzResolution = dlg.GetHorzResolution();
      double VertResolution = dlg.GetVertResolution();
      wxString NoData = dlg.GetNoData();
      bool StrictResolution = dlg.IsStrictResolution();
      bool MixedResolutions = dlg.IsMixedResolutions();
      bool InputPaths = dlg.IsInputPaths();
      bool MD5 = dlg.IsMD5();
      bool Summary = dlg.IsSummary();
      int RedBand = dlg.GetRedBand();
      int GreenBand = dlg.GetGreenBand();
      int BlueBand = dlg.GetBlueBand();
      int NIRband = dlg.GetNIRband();
      bool AutoNDVI = dlg.IsAutoNDVI();
      bool isQueryable = dlg.IsQueryable();
      if (MainFrame->CreateRasterCoverage
          (CoverageName, Title, Abstract, SampleType, PixelType, NumBands,
           Compression, Quality, TileWidth, TileHeight, NotGeoreferenced, Srid,
           HorzResolution, VertResolution, NoData, StrictResolution,
           MixedResolutions, InputPaths, MD5, Summary, RedBand, GreenBand,
           BlueBand, NIRband, AutoNDVI, isQueryable) == true)
        {
          wxMessageBox(wxT("Raster Coverage ") + CoverageName +
                       wxT(" successfully created"), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
          wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_Refresh);
          OnCmdRefresh(evt);
        }
    }
}

void MyTableTree::OnCmdRegisterWmsLayer(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - registering a new WMS layer
//
  MainFrame->DoRegisterWMS();
}

void MyTableTree::OnCmdWmsLayerUnregister(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Unregister a WMS layer
//
  WmsLayerUnregisterDialog dlg;
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  wxString url = CurrentWmsLayerURL;
  wxString name = CurrentWmsLayerName;
  wxString title;
  wxString abstract;
  wxString copyright;
  int license_id;
  wxString data_license;
  bool is_queryable;
  wxString getfeatureinfo_url;
  if (MainFrame->DoGetWmsLayerInfos
      (url, name, title, abstract, copyright, &license_id, data_license,
       &is_queryable, getfeatureinfo_url) == false)
    return;
  dlg.Create(MainFrame, url, name, title, abstract, copyright, data_license);
  dlg.ShowModal();
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
}

void MyTableTree::OnCmdWmsLayerInfos(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - editing WMS layer Infos
//
  WmsLayerInfosDialog dlg;
  wxString url = CurrentWmsLayerURL;
  wxString name = CurrentWmsLayerName;
  wxString title;
  wxString abstract;
  wxString copyright;
  int license_id;
  wxString data_license;
  bool is_queryable;
  wxString getfeatureinfo_url;
  if (MainFrame->DoGetWmsLayerInfos
      (url, name, title, abstract, copyright, &license_id, data_license,
       &is_queryable, getfeatureinfo_url) == false)
    return;
  dlg.Create(MainFrame, url, name, title, abstract, copyright, license_id,
             is_queryable, getfeatureinfo_url);
  dlg.ShowModal();
}

void MyTableTree::OnCmdWmsLayerConfigure(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - configuring a WMS layer
//
  WmsLayerConfigDialog dlg;
  wxString url = CurrentWmsLayerURL;
  wxString name = CurrentWmsLayerName;
  dlg.Create(MainFrame, url, name);
  dlg.ShowModal();
}

void MyTableTree::OnCmdCreateTopoGeo(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - creating a new Topology-Geometry
//
  MainFrame->DoCreateTopoGeo();
}

void MyTableTree::OnCmdCreateTopoNet(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - creating a new Topology-Network
//
  MainFrame->DoCreateTopoNet();
}

void MyTableTree::OnCmdDropTopoGeo(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dropping a Topology-Geometry
//
  wxString msg;
  char *errMsg = NULL;
  int ret;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TOPO_GEO)
    {
      msg = wxT("Do you really intend to drop the Topology-Geometry named: ");
      msg += obj->GetMainName();
      msg += wxT("\n[and any other related object] ?");
    }
  wxMessageDialog confirm(this, msg, wxT("Confirming DROP"),
                          wxOK | wxCANCEL | wxICON_QUESTION);
  ret = confirm.ShowModal();
  if (ret != wxID_OK)
    return;
  ::wxBeginBusyCursor();
  char *name = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(name, obj->GetMainName().ToUTF8());
  char *sql = sqlite3_mprintf("SELECT DropTopology(%Q)", name);
  free(name);
  ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      goto err;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT("Topology-Geometry '") + obj->GetMainName() +
               wxT("' was successfully removed"), wxT("spatialite_gui"),
               wxOK | wxICON_INFORMATION, this);
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
  return;
err:
  ::wxEndBusyCursor();
  wxMessageBox(wxT("Error: unable to remove Topology-Geometry '") +
               obj->GetMainName() + wxT("'"), wxT("spatialite_gui"),
               wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdDropTopoNet(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - dropping a Topology-Network
//
  wxString msg;
  char *errMsg = NULL;
  int ret;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  if (obj == NULL)
    return;
  if (obj->GetType() == MY_TOPO_NET)
    {
      msg = wxT("Do you really intend to drop the Topology-Network named: ");
      msg += obj->GetMainName();
      msg += wxT("\n[and any other related object] ?");
    }
  wxMessageDialog confirm(this, msg, wxT("Confirming DROP"),
                          wxOK | wxCANCEL | wxICON_QUESTION);
  ret = confirm.ShowModal();
  if (ret != wxID_OK)
    return;
  ::wxBeginBusyCursor();
  char *name = (char *) malloc((obj->GetMainName().Len() * 4) + 1);
  strcpy(name, obj->GetMainName().ToUTF8());
  char *sql = sqlite3_mprintf("SELECT DropNetwork(%Q)", name);
  free(name);
  ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      ::wxEndBusyCursor();
      goto err;
    }
  ::wxEndBusyCursor();
  wxMessageBox(wxT("Topology-Network '") + obj->GetMainName() +
               wxT("' was successfully removed"), wxT("spatialite_gui"),
               wxOK | wxICON_INFORMATION, this);
  // appending a delayed event so to really update the Tree
  AddPendingEvent(evt);
  return;
err:
  ::wxEndBusyCursor();
  wxMessageBox(wxT("Error: unable to remove Topology-Network '") +
               obj->GetMainName() + wxT("'"), wxT("spatialite_gui"),
               wxOK | wxICON_WARNING, this);
}

void MyTableTree::OnCmdCreatePostgreSqlConn(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - creating a new connection to a PostgreSQL DBMS
//
  MainFrame->DoPostgreSqlConnection();
}

void MyTableTree::OnCmdCloseAllPostgreSqlConns(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - closing all connections to PostgreSQL
//
  ::wxBeginBusyCursor();
  MainFrame->AutoPostgresStop();
  ::wxEndBusyCursor();
  wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
  AddPendingEvent(evt);
}

void MyTableTree::OnCmdClosePostgreSqlConn(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - closing a connection to PostgreSQL
//
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  ::wxBeginBusyCursor();
  bool ret =
    MainFrame->DoClosePostgreSqlConn(obj->GetHost(), obj->GetHostAddr(),
                                     obj->GetPort(), obj->GetDbName(),
                                     obj->GetUser());
  ::wxEndBusyCursor();
  if (ret == true)
    {
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
    }
}

void MyTableTree::OnCmdPostgreSqlDropOrphans(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Dropping all PostgreSQL orphans
//
  bool refresh = false;
  int ret;
  char *errMsg = NULL;
  ::wxBeginBusyCursor();
  char *sql = MainFrame->DropPostgreSqlOrphans();
  if (sql != NULL)
    {
      // exec
      ret = sqlite3_exec(MainFrame->GetSqlite(), sql, NULL, 0, &errMsg);
      sqlite3_free(sql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("DropPostgresOrphans: ") +
                       wxString::FromUTF8(errMsg), wxT("spatialite_gui"),
                       wxOK | wxICON_ERROR, this);
          sqlite3_free(errMsg);
          return;
        }
      refresh = true;
    }
  ::wxEndBusyCursor();
  if (refresh == true)
    {
      wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, Tree_RefreshDeferred);
      AddPendingEvent(evt);
    }
}

void MyTableTree::OnCmdPostgreSqlInfos(wxCommandEvent & WXUNUSED(event))
{
//
// menu event - Infos about some PostgreSQL object
//
  char dummy[128];
  wxString msg;
  MyPostgresTable *table;
  MyObject *obj = (MyObject *) GetItemData(CurrentItem);
  switch (obj->GetType())
    {
      case MY_POSTGRES_CONN:
        msg = wxT("Connection to PostgreSQL\n\n");
        msg += wxT("Host: ") + obj->GetHost() + wxT("\n");
        msg += wxT("HostAddr: ") + obj->GetHostAddr() + wxT("\n");
        sprintf(dummy, "%d", obj->GetPort());
        msg += wxT("Port: ") + wxString::FromUTF8(dummy) + wxT("\n");
        msg += wxT("DbName: ") + obj->GetDbName() + wxT("\n");
        msg += wxT("User: ") + obj->GetUser() + wxT("\n\n");
        if (obj->IsReadOnly() == true)
          msg += wxT("READ-ONLY Connection");
        else
          msg += wxT("READ-WRITE Connection");
        break;
      case MY_POSTGRES_SCHEMA:
        msg += wxT("Role: PostgreSQL Schema\n");
        msg += wxT("PostgreSQL Name: ") + obj->GetSchema() + wxT("\n");
        break;
      case MY_POSTGRES_TABLE:
        msg += wxT("Role: PostgreSQL Table\n");
        msg +=
          wxT("PostgreSQL Name: ") + obj->GetSchema() + wxT(".") +
          obj->GetName() + wxT("\n\n");
        table = MainFrame->FindPostgresTable(obj->GetVirtName());
        if (table != NULL)
          {
            MyPostGisGeometry *geom = table->GetFirst();
            while (geom != NULL)
              {
                msg += wxT("PostGIS Geometry: ") + geom->GetName();
                msg += wxT(" ") + geom->GetGeomType();
                char dummy[64];
                sprintf(dummy, "%d", geom->GetSrid());
                wxString xSrid = wxString::FromUTF8(dummy);
                msg += wxT(" srid=") + xSrid;
                sprintf(dummy, "%d", geom->GetDims());
                wxString xDims = wxString::FromUTF8(dummy);
                msg += wxT(" dims=") + xDims + wxT("\n");
                geom = geom->GetNext();
              }
            if (table->GetFirst() != NULL)
              msg += wxT("\n");
          }
        if (obj->HasPK() == true)
          msg += wxT("Primary Key: YES\n");
        else
          msg += wxT("Primary Key: NO\n");
        if (obj->CanSelect() == true)
          msg += wxT("Select permission: YES\n");
        else
          msg += wxT("Select permission: NO\n");
        if (obj->CanInsertUpdateDelete() == true)
          msg += wxT("Insert/Update/Delete permission: YES\n");
        else
          msg += wxT("Insert/Update/Delete permission: NO\n");
        if (obj->IsReadOnly() == true)
          msg += wxT("\nREAD-ONLY Connection");
        else
          msg += wxT("\nREAD-WRITE Connection");
        break;
      case MY_POSTGRES_VIEW:
        msg += wxT("Role: PostgreSQL View\n");
        msg +=
          wxT("PostgreSQL Name: ") + obj->GetSchema() + wxT(".") +
          obj->GetName() + wxT("\n\n");
        if (obj->HasPK() == true)
          msg += wxT("Primary Key: YES\n");
        else
          msg += wxT("Primary Key: NO\n");
        if (obj->CanSelect() == true)
          msg += wxT("Select permission: YES\n");
        else
          msg += wxT("Select permission: NO\n");
        if (obj->CanInsertUpdateDelete() == true)
          msg += wxT("Insert/Update/Delete permission: YES\n");
        else
          msg += wxT("Insert/Update/Delete permission: NO\n");
        if (obj->IsReadOnly() == true)
          msg += wxT("\nREAD-ONLY Connection");
        else
          msg += wxT("\nREAD-WRITE Connection");
        break;
      case MY_POSTGIS_VIEW:
        msg += wxT("Role: PostGIS Table wrapper (Spatial View)\n");
        msg +=
          wxT("PostgreSQL Name: ") + obj->GetSchema() + wxT(".") +
          obj->GetName() + wxT("\n\n");
        table = MainFrame->FindPostGisView(obj->GetVirtName());
        if (table != NULL)
          {
            MyPostGisGeometry *geom = table->GetFirst();
            while (geom != NULL)
              {
                msg += wxT("PostGIS Geometry: ") + geom->GetName();
                msg += wxT(" ") + geom->GetGeomType();
                char dummy[64];
                sprintf(dummy, "%d", geom->GetSrid());
                wxString xSrid = wxString::FromUTF8(dummy);
                msg += wxT(" srid=") + xSrid;
                sprintf(dummy, "%d", geom->GetDims());
                wxString xDims = wxString::FromUTF8(dummy);
                msg += wxT(" dims=") + xDims + wxT("\n");
                geom = geom->GetNext();
              }
            if (table->GetFirst() != NULL)
              msg += wxT("\n");
          }
        if (obj->HasPK() == true)
          msg += wxT("Primary Key: YES\n");
        else
          msg += wxT("Primary Key: NO\n");
        if (obj->CanSelect() == true)
          msg += wxT("Select permission: YES\n");
        else
          msg += wxT("Select permission: NO\n");
        if (obj->CanInsertUpdateDelete() == true)
          msg += wxT("Insert/Update/Delete permission: YES\n");
        else
          msg += wxT("Insert/Update/Delete permission: NO\n");
        if (obj->IsReadOnly() == true)
          msg += wxT("\nREAD-ONLY Connection");
        else
          msg += wxT("\nREAD-WRITE Connection");
        break;
    };
  if (msg.Len() > 0)
    wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
}

sqlite3 *MyTableTree::GetSQLiteHandle()
{
  return MainFrame->GetSqlite();
}

bool MyTableTree::IsAlreadyDefinedTopologyCoverage(wxString & name)
{
//
// testing if a Topology Coverage is already defined 
//
  int count = 1;
  int i;
  char **results;
  int rows;
  int columns;
  char *sql;
  sqlite3 *sqlite = GetSQLiteHandle();
  char *coverage = (char *) malloc(name.Len() * 4);
  strcpy(coverage, name.ToUTF8());
  sql =
    sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                    "Upper(topology_name) = Upper(%Q)", coverage);
  free(coverage);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return true;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        count = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
  if (count >= 1)
    return true;
  return false;
}

bool MyTableTree::IsAlreadyDefinedNetworkCoverage(wxString & name)
{
//
// testing if a Network Coverage is already defined 
//
  int count = 1;
  int i;
  char **results;
  int rows;
  int columns;
  char *sql;
  sqlite3 *sqlite = GetSQLiteHandle();
  char *coverage = (char *) malloc(name.Len() * 4);
  strcpy(coverage, name.ToUTF8());
  sql =
    sqlite3_mprintf("SELECT Count(*) FROM vector_coverages WHERE "
                    "Upper(network_name) = Upper(%Q)", coverage);
  free(coverage);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return true;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        count = atoi(results[(i * columns) + 0]);
    }
  sqlite3_free_table(results);
  if (count >= 1)
    return true;
  return false;
}
