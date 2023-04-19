/*
/ Classdef.h
/ class definitions for spatialite_gui  - a SQLite /SpatiaLite GUI tool
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

#include "wx/wx.h"
#include "wx/aui/aui.h"
#include "wx/treectrl.h"
#include "wx/grid.h"
#include "wx/listctrl.h"
#include "wx/textctrl.h"
#include "wx/propdlg.h"
#include "wx/generic/propdlg.h"
#include "wx/timer.h"
#include "wx/string.h"
#include "wx/dynlib.h"

#include "config.h"

#ifdef SPATIALITE_AMALGAMATION
#include <spatialite/sqlite3.h>
#else
#include <sqlite3.h>
#endif

#include <spatialite/gaiaaux.h>
#include <spatialite/gaiaexif.h>
#include <spatialite/gaiageo.h>
#include <spatialite.h>
#include <spatialite/gg_wfs.h>
#include <spatialite/geopackage.h>

#include <freexl.h>
#include <virtualpg.h>

#include <rasterlite2/rasterlite2.h>
#include <rasterlite2/rl2wms.h>
#include "rasterlite2/rl2graphics.h"
#include "rasterlite2/rl2mapconfig.h"

#include "AuxCurl.h"

//
// functions for threaded queries
//
int SqlProgressCallback(void *arg);
#if defined(__WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteSqlThread(void *arg);
#else
void *DoExecuteSqlThread(void *arg);
#endif

//
// functions for threaded WFS download
//
void WfsCallback(int rows, void *ptr);
#if defined(__WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteWfs(void *arg);
#else
void *DoExecuteWfs(void *arg);
#endif

//
// functions for threaded Raster Import
//
#if defined(__WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteRasterLoad(void *arg);
#else
void *DoExecuteRasterLoad(void *arg);
#endif

//
// functions for threaded Raster Style Import
//
#if defined(__WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteRasterStylesLoad(void *arg);
#else
void *DoExecuteRasterStylesLoad(void *arg);
#endif

//
// functions for threaded Vector Style Import
//
#if defined(__WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteVectorStylesLoad(void *arg);
#else
void *DoExecuteVectorStylesLoad(void *arg);
#endif

// constants for UOM
#define GUI_UOM_PIXEL	0xa0
#define GUI_UOM_METRE	0xb0
#define GUI_UOM_INCH	0xc0

// constants for Preview Background
#define GUI_PREVIEW_BACKGROUND_CHECKERED	0xfa
#define GUI_PREVIEW_BACKGROUND_WHITE		0xfb
#define GUI_PREVIEW_BACKGROUND_BLACK		0xfc

enum
{
// control IDs for main window and tree list control
  ID_Connect = 1,
  ID_Connect_RO,
  ID_CreateNew,
  ID_Disconnect,
  ID_MemoryDbLoad,
  ID_MemoryDbNew,
  ID_MemoryDbClock,
  ID_MemoryDbSave,
  ID_Vacuum,
  ID_MapPanel,
  ID_SqlScript,
  ID_QueryViewComposer,
  ID_LoadShp,
  ID_LoadGeoJSON,
  ID_LoadTxt,
  ID_LoadDbf,
  ID_LoadXL,
  ID_VirtualShp,
  ID_VirtualGeoJSON,
  ID_VirtualTxt,
  ID_VirtualDbf,
  ID_VirtualXL,
  ID_Network,
  ID_Exif,
  ID_GpsPics,
  ID_LoadXml,
  ID_Srids,
  ID_Charset,
  ID_Help,
  ID_Attach,
  ID_Postgres,
  ID_SqlLog,
  ID_DbStatus,
  ID_CheckGeom,
  ID_SaneGeom,
  ID_WFS,
  ID_DXF,
  ID_LoadMapConfig,
  ID_AddLayer,
  ID_SqlLayer,
  ID_Identify,
  ID_ZoomIn,
  ID_ZoomOut,
  ID_Pan,
  ID_MapImage,
  ID_Printer,
  ID_PROJ_LIB,
  Tree_NewTable,
  Tree_NewView,
  Tree_NewIndex,
  Tree_NewTrigger,
  Tree_NewColumn,
  Tree_NewRasterStyle,
  Tree_ReloadRasterStyle,
  Tree_UnregisterRasterStyle,
  Tree_NewVectorStyle,
  Tree_ReloadVectorStyle,
  Tree_UnregisterVectorStyle,
  Tree_NewMapConfig,
  Tree_ReloadMapConfig,
  Tree_UnregisterMapConfig,
  Tree_VerifyExternalMapConfig,
  Tree_VerifyRegisteredMapConfig,
  Tree_QueryViewComposer,
  Tree_Show,
  Tree_Drop,
  Tree_Rename,
  Tree_Select,
  Tree_SelectTiles,
  Tree_Refresh,
  Tree_RefreshDeferred,
  Tree_SpatialIndex,
  Tree_CheckSpatialIndex,
  Tree_RecoverSpatialIndex,
  Tree_MbrCache,
  Tree_RebuildTriggers,
  Tree_ShowSql,
  Tree_Recover,
  Tree_CheckGeometry,
  Tree_Extent,
  Tree_UpdateLayerStatistics,
  Tree_UpdateLayerStatisticsAll,
  Tree_ElementaryGeoms,
  Tree_MalformedGeometries,
  Tree_RepairPolygons,
  Tree_SetSrid,
  Tree_DumpShp,
  Tree_DumpGeoJSON,
  Tree_DumpKml,
  Tree_DumpTxtTab,
  Tree_DumpCsv,
  Tree_DumpHtml,
  Tree_DumpDif,
  Tree_DumpSylk,
  Tree_DumpDbf,
  Tree_DumpXlsx,
  Tree_DumpPostGIS,
  Tree_Edit,
  Tree_DropColumn,
  Tree_RenameColumn,
  Tree_ColumnStats,
  Tree_MapPreview,
  Tree_CreateVectorCoverage,
  Tree_CreateTopologyCoverage,
  Tree_CreateNetworkCoverage,
  Tree_CheckDuplicates,
  Tree_RemoveDuplicates,
  Tree_Detach,
  Tree_CheckGeom,
  Tree_SaneGeom,
  Tree_SldSeRasterStyles,
  Tree_SldSeVectorStyles,
  Tree_ImportRaster,
  Tree_Pyramidize,
  Tree_PyramidizeMonolithic,
  Tree_DePyramidize,
  Tree_RasterDrop,
  Tree_AddAllRastersSrid,
  Tree_RasterInfos,
  Tree_VectorRegister,
  Tree_SpatialViewRegister,
  Tree_VirtualTableRegister,
  Tree_TopoGeoRegister,
  Tree_TopoNetRegister,
  Tree_VectorUnregister,
  Tree_AddAllVectorsSrid,
  Tree_VectorInfos,
  Tree_CreateRasterCoverage,
  Tree_UpdateRasterExtent,
  Tree_UpdateRasterExtentAll,
  Tree_UpdateVectorExtent,
  Tree_UpdateVectorExtentAll,
  Tree_Raster_SRIDs,
  Tree_Vector_SRIDs,
  Tree_Raster_Keywords,
  Tree_Vector_Keywords,
  Tree_RegisterExternalGraphic,
  Tree_UnregisterExternalGraphic,
  Tree_RegisterTextFont,
  Tree_UnregisterTextFont,
  Tree_RasterSymbolizerContrast,
  Tree_RasterSymbolizerChannelRgb,
  Tree_RasterSymbolizerChannelGray,
  Tree_RasterSymbolizerCategorize,
  Tree_RasterSymbolizerInterpolate,
  Tree_RasterSymbolizerShadedRelief,
  Tree_RasterSymbolizerMonochrome,
  Tree_SimplePointSymbolizer,
  Tree_SimpleLineSymbolizer,
  Tree_SimplePolygonSymbolizer,
  Tree_SimpleTextSymbolizer,
  Tree_CloneTable,
  Tree_WmsLayerRegister,
  Tree_WmsLayerUnregister,
  Tree_WmsLayerInfos,
  Tree_WmsLayerConfigure,
  Tree_CreateTopoGeo,
  Tree_DropTopoGeo,
  Tree_CreateTopoNet,
  Tree_DropTopoNet,
  Tree_MapRemoveAll,
  Tree_MapShowAll,
  Tree_MapHideAll,
  Tree_MapConfigure,
  Tree_MapVisible,
  Tree_MapFullExtent,
  Tree_MapLayerFullExtent,
  Tree_SqlSample,
  Tree_UrlSample,
  Tree_SqlLayer,
  Tree_MapLayerConfigure,
  Tree_MapLayerInfo,
  Tree_QuickStyleEdit,
  Tree_MapRemoveLayer,
  Tree_MapDeleteItem,
  Tree_CreatePostgreSqlConn,
  Tree_CloseAllPostgreSqlConns,
  Tree_ClosePostgreSqlConn,
  Tree_PostgreSqlDropOrphans,
  Tree_PostgreSqlInfos,
  Grid_Clear,
  Grid_All,
  Grid_Column,
  Grid_Row,
  Grid_Copy,
  Grid_Blob,
  Grid_Delete,
  Grid_Insert,
  Grid_Abort,
  Grid_BlobIn,
  Grid_BlobOut,
  Grid_BlobNull,
  Grid_XmlBlobIn,
  Grid_XmlBlobOut,
  Grid_XmlBlobOutIndented,
  Grid_ExpTxtTab,
  Grid_ExpCsv,
  Grid_ExpHtml,
  Grid_ExpShp,
  Grid_ExpDif,
  Grid_ExpSylk,
  Grid_ExpDbf,
  Grid_ExpXlsx,
  Grid_TilePreview,
  Grid_Filter,
  Grid_MapShow,
  Grid_MapZoom,
  Tree_ShowAll,
  Tree_HideAll,
  Image_Copy,
  Wfs_Copy,
  Wfs_Layer
};

enum
{
// control IDs for dialogs
  ID_SQL = 10000,
  ID_SQL_GO,
  ID_SQL_FILTER,
  ID_SQL_ERASE,
  ID_SQL_ABORT,
  ID_HISTORY_BACK,
  ID_HISTORY_FORWARD,
  ID_RS_FIRST,
  ID_RS_LAST,
  ID_RS_NEXT,
  ID_RS_PREVIOUS,
  ID_REFRESH,
  ID_RS_BLOCK,
  ID_RS_THREAD_FINISHED,
  ID_RS_STATS_UPDATE,
  ID_RS_MAP_SHOW,
  ID_RS_MAP_ZOOM,
  ID_PANE_HEXADECIMAL,
  ID_PANE_GEOMETRY,
  ID_PANE_WKT,
  ID_PANE_EWKT,
  ID_PANE_SVG,
  ID_PANE_KML,
  ID_PANE_GML,
  ID_PANE_GEOJSON,
  ID_PANE_IMAGE,
  ID_HEX,
  ID_GEOM_TABLE,
  ID_WKT_TABLE,
  ID_WKT_COPY,
  ID_EWKT_TABLE,
  ID_EWKT_COPY,
  ID_SVG_TABLE,
  ID_SVG_RELATIVE,
  ID_SVG_PRECISION,
  ID_SVG_COPY,
  ID_KML_TABLE,
  ID_KML_PRECISION,
  ID_KML_COPY,
  ID_GML_TABLE,
  ID_GML_V2_V3,
  ID_GML_PRECISION,
  ID_GML_COPY,
  ID_GEOJSON_TABLE,
  ID_GEOJSON_OPTIONS,
  ID_GEOJSON_PRECISION,
  ID_GEOJSON_COPY,
  ID_GEOM_GRAPH,
  ID_GEOM_BOX,
  ID_IMAGE_TITLE,
  ID_IMG_BOX,
  ID_IMAGE,
  ID_XML_DOCUMENT_TABLE,
  ID_XML_DOCUMENT_COPY,
  ID_XML_INDENTED_TABLE,
  ID_XML_INDENTED_COPY,
  ID_VIRTSHP_TABLE,
  ID_VIRTSHP_SRID,
  ID_VIRTSHP_CHARSET,
  ID_VIRTSHP_TEXTDATES,
  ID_VIRTTXT_TABLE,
  ID_VIRTTXT_CHARSET,
  ID_VIRTTXT_TITLES,
  ID_VIRTTXT_SEPARATOR,
  ID_VIRTTXT_CHARSEPARATOR,
  ID_VIRTTXT_QUOTE,
  ID_VIRTTXT_POINT,
  ID_VIRTDBF_TABLE,
  ID_VIRTDBF_CHARSET,
  ID_VIRTDBF_TEXTDATES,
  ID_VIRTXL_TABLE,
  ID_VIRTXL_WORKSHEET,
  ID_VIRTXL_TITLES,
  ID_LDSHP_TABLE,
  ID_LDSHP_COLUMN,
  ID_LDSHP_SRID,
  ID_LDSHP_CHARSET,
  ID_LDSHP_COERCE_2D,
  ID_LDSHP_COMPRESSED,
  ID_LDSHP_RTREE,
  ID_LDSHP_USER_GTYPE,
  ID_LDSHP_GTYPE,
  ID_LDSHP_USER_PKEY,
  ID_LDSHP_PKCOL,
  ID_LDSHP_TEXTDATES,
  ID_LDSHP_COLNAME_CASE,
  ID_LDSHP_STATISTICS,
  ID_LDXL_TABLE,
  ID_LDXL_WORKSHEET,
  ID_LDXL_TITLES,
  ID_LDXML_COMPRESSED,
  ID_LDXML_VALIDATE,
  ID_LDXML_INTERNAL_SCHEMA,
  ID_LDXML_SCHEMA_URI,
  ID_LDTXT_TABLE,
  ID_LDTXT_CHARSET,
  ID_LDTXT_TITLES,
  ID_LDTXT_SEPARATOR,
  ID_LDTXT_CHARSEPARATOR,
  ID_LDTXT_QUOTE,
  ID_LDTXT_POINT,
  ID_LDDBF_TABLE,
  ID_LDDBF_CHARSET,
  ID_LDDBF_USER_PKEY,
  ID_LDDBF_PKCOL,
  ID_LDDBF_TEXTDATES,
  ID_LDDBF_COLNAME_CASE,
  ID_DMPSHP_CHARSET,
  ID_DMPTXT_CHARSET,
  ID_NET_TABLE,
  ID_NET_FROM,
  ID_NET_TO,
  ID_NET_NO_GEOM,
  ID_NET_GEOM,
  ID_NET_LENGTH,
  ID_NET_COST,
  ID_NET_BIDIR,
  ID_NET_ONEWAY,
  ID_NET_FROM_TO,
  ID_NET_TO_FROM,
  ID_NET_NAME_ENABLE,
  ID_NET_NAME,
  ID_NET_A_STAR,
  ID_NET_DATA,
  ID_NET_VIRTUAL,
  ID_NET_OVERWRITE,
  ID_EXIF_PATH,
  ID_EXIF_FOLDER,
  ID_EXIF_METADATA,
  ID_EXIF_GPS_ONLY,
  ID_GPS_PICS_PATH,
  ID_GPS_PICS_FOLDER,
  ID_GPS_PICS_TABLE,
  ID_GPS_PICS_GEOMETRY,
  ID_GPS_PICS_STATISTICS,
  ID_GPS_PICS_RTREE,
  ID_XML_OK_SUFFIX,
  ID_XML_SUFFIX,
  ID_XML_PATH,
  ID_XML_FOLDER,
  ID_XML_TARGET_TABLE,
  ID_XML_PK_NAME,
  ID_XML_BLOB_COLUMN,
  ID_XML_OK_PATH,
  ID_XML_PATH_COLUMN,
  ID_XML_OK_SCHEMA_URI,
  ID_XML_SCHEMA_URI_COLUMN,
  ID_XML_OK_PARSE_ERR,
  ID_XML_PARSE_ERR_COLUMN,
  ID_XML_OK_VALIDATE_ERR,
  ID_XML_VALIDATE_ERR_COLUMN,
  ID_XML_COMPRESSED,
  ID_XML_VALIDATED,
  ID_XML_INTERNAL_SCHEMA,
  ID_XML_SCHEMA_URI,
  ID_DXF_OK_PREFIX,
  ID_DXF_PREFIX,
  ID_DXF_PATH,
  ID_DXF_FOLDER,
  ID_DXF_OK_SINGLE,
  ID_DXF_SINGLE,
  ID_DXF_DIMS,
  ID_DXF_SRID,
  ID_DXF_RINGS,
  ID_DXF_MIXED,
  ID_DXF_APPEND,
  ID_DFLT_CHARSET,
  ID_DFLT_ASK,
  ID_SCRIPT_CHARSET,
  ID_RCVR_SRID,
  ID_RCVR_TYPE,
  ID_RCVR_DIMS,
  ID_SRID_OLD,
  ID_SRID_SRID,
  ID_SEARCH,
  ID_BY_SRID,
  ID_HELP_HTML,
  ID_MAPCFG_HTML,
  ID_AUTO_SAVE_PATH,
  ID_AUTO_SAVE_INTERVAL,
  ID_AUTO_SAVE_CHANGE_PATH,
  ID_QVC_SQL,
  ID_QVC_TAB,
  ID_QVC_TABLE_2,
  ID_QVC_TABLE_NAME_1,
  ID_QVC_TABLE_NAME_2,
  ID_QVC_TABLE_ALIAS_1,
  ID_QVC_TABLE_ALIAS_2,
  ID_QVC_COLUMNS_1,
  ID_QVC_COLUMNS_2,
  ID_QVC_JOIN_MODE,
  ID_QVC_MATCH_1_T1,
  ID_QVC_MATCH_1_T2,
  ID_QVC_MATCH_2_ENABLE,
  ID_QVC_MATCH_2_T1,
  ID_QVC_MATCH_2_T2,
  ID_QVC_MATCH_3_ENABLE,
  ID_QVC_MATCH_3_T1,
  ID_QVC_MATCH_3_T2,
  ID_QVC_WHERE_1_ENABLE,
  ID_QVC_WHERE_1_TABLE,
  ID_QVC_WHERE_1_COLUMN,
  ID_QVC_WHERE_1_OPERATOR,
  ID_QVC_WHERE_1_VALUE,
  ID_QVC_WHERE_2_ENABLE,
  ID_QVC_WHERE_2_TABLE,
  ID_QVC_WHERE_2_COLUMN,
  ID_QVC_WHERE_2_OPERATOR,
  ID_QVC_WHERE_2_VALUE,
  ID_QVC_WHERE_3_ENABLE,
  ID_QVC_WHERE_3_TABLE,
  ID_QVC_WHERE_3_COLUMN,
  ID_QVC_WHERE_3_OPERATOR,
  ID_QVC_WHERE_3_VALUE,
  ID_QVC_CONNECTOR_12,
  ID_QVC_CONNECTOR_23,
  ID_QVC_ORDER_1_ENABLE,
  ID_QVC_ORDER_1_TABLE,
  ID_QVC_ORDER_1_COLUMN,
  ID_QVC_ORDER_1_DESC,
  ID_QVC_ORDER_2_ENABLE,
  ID_QVC_ORDER_2_TABLE,
  ID_QVC_ORDER_2_COLUMN,
  ID_QVC_ORDER_2_DESC,
  ID_QVC_ORDER_3_ENABLE,
  ID_QVC_ORDER_3_TABLE,
  ID_QVC_ORDER_3_COLUMN,
  ID_QVC_ORDER_3_DESC,
  ID_QVC_ORDER_4_ENABLE,
  ID_QVC_ORDER_4_TABLE,
  ID_QVC_ORDER_4_COLUMN,
  ID_QVC_ORDER_4_DESC,
  ID_QVC_VIEW_TYPE,
  ID_QVC_VIEW_NAME,
  ID_QVC_VIEW_GEOTABLE,
  ID_QVC_VIEW_GEOMETRY,
  ID_QVC_WRITABLE_1,
  ID_QVC_WRITABLE_2,
  ID_SQL_FC_SQL,
  ID_SQL_FC_TAB,
  ID_SQL_FC_COLUMNS,
  ID_SQL_FC_WHERE_1_ENABLE,
  ID_SQL_FC_WHERE_1_COLUMN,
  ID_SQL_FC_WHERE_1_OPERATOR,
  ID_SQL_FC_WHERE_1_VALUE,
  ID_SQL_FC_WHERE_2_ENABLE,
  ID_SQL_FC_WHERE_2_COLUMN,
  ID_SQL_FC_WHERE_2_OPERATOR,
  ID_SQL_FC_WHERE_2_VALUE,
  ID_SQL_FC_WHERE_3_ENABLE,
  ID_SQL_FC_WHERE_3_COLUMN,
  ID_SQL_FC_WHERE_3_OPERATOR,
  ID_SQL_FC_WHERE_3_VALUE,
  ID_SQL_FC_CONNECTOR_12,
  ID_SQL_FC_CONNECTOR_23,
  ID_SQL_FC_ORDER_1_ENABLE,
  ID_SQL_FC_ORDER_1_COLUMN,
  ID_SQL_FC_ORDER_1_DESC,
  ID_SQL_FC_ORDER_2_ENABLE,
  ID_SQL_FC_ORDER_2_COLUMN,
  ID_SQL_FC_ORDER_2_DESC,
  ID_SQL_FC_ORDER_3_ENABLE,
  ID_SQL_FC_ORDER_3_COLUMN,
  ID_SQL_FC_ORDER_3_DESC,
  ID_SQL_FC_ORDER_4_ENABLE,
  ID_SQL_FC_ORDER_4_COLUMN,
  ID_SQL_FC_ORDER_4_DESC,
  ID_SQL_FC_FREE_HAND,
  ID_MALFORMED_GRID,
  ID_MALFORMED_CLOSE,
  ID_MALFORMED_REPAIR,
  ID_MALFORMED_CLEAR,
  ID_MALFORMED_ALL,
  ID_MALFORMED_ROW,
  ID_MALFORMED_COLUMN,
  ID_MALFORMED_COPY,
  ID_MALFORMED_BLOB,
  ID_DB_STATUS_GRID,
  ID_DB_STATUS_CLOSE,
  ID_DB_STATUS_RESET,
  ID_DB_STATUS_CLEAR,
  ID_DB_STATUS_ALL,
  ID_DB_STATUS_ROW,
  ID_DB_STATUS_COLUMN,
  ID_DB_STATUS_COPY,
  ID_STAT_CHART,
  ID_CHART_TYPE,
  ID_CHART_SIZE,
  ID_CHART_MODE,
  ID_CHART_CLASS,
  ID_CHART_COPY,
  ID_CHART_PNG,
  ID_CHART_SVG,
  ID_CHART_PDF,
  ID_MAP_SYMBOL,
  ID_MAP_SIZE,
  ID_MAP_SYM_SIZE,
  ID_MAP_THICKNESS,
  ID_MAP_FILL,
  ID_MAP_FILL_COL,
  ID_MAP_LINE_COL,
  ID_MAP_COPY,
  ID_MAP_PNG,
  ID_MAP_SVG,
  ID_MAP_PDF,
  ID_MAP_CONFIG_INSERT,
  ID_MAP_CONFIG_EXPORT,
  ID_MAP_CONFIG_COPY,
  ID_MAP_CONFIG_NAME,
  ID_MAP_CONFIG_TITLE,
  ID_MAP_CONFIG_ABSTRACT,
  ID_MAP_CONFIG_PANE_SRS,
  ID_MAP_CONFIG_PANE_OPTIONS,
  ID_KML_NAME,
  ID_KML_NAME_K,
  ID_KML_DESC,
  ID_KML_DESC_K,
  ID_SHEET_DECIMAL_POINT,
  ID_SHEET_DATE_TIME,
  ID_ELEMGEOM_TABLE,
  ID_ELEMGEOM_PKEY,
  ID_ELEMGEOM_MULTI_ID,
  ID_ELEMGEOM_TYPE,
  ID_ELEMGEOM_SRID,
  ID_ELEMGEOM_COORDS,
  ID_ELEMGEOM_RTREE,
  ID_POSTGIS_SCHEMA,
  ID_POSTGIS_TABLE,
  ID_POSTGIS_LOWER,
  ID_POSTGIS_CREATE,
  ID_POSTGIS_SPINDEX,
  ID_SANEGEOM_PREFIX,
  ID_CLONE_OUTPUT,
  ID_CLONE_JUST_CREATE,
  ID_CLONE_WITH_FK,
  ID_CLONE_WITH_TRIGGERS,
  ID_CLONE_RESEQUENCE,
  ID_CLONE_APPEND,
  ID_CLONE_IGNORE,
  ID_CLONE_CAST2MULTI,
  ID_WFS_URL,
  ID_WFS_CATALOG,
  ID_WFS_RESET,
  ID_WFS_NAME,
  ID_WFS_SRID,
  ID_WFS_VERSION,
  ID_WFS_LABEL,
  ID_WFS_PAGE,
  ID_WFS_MAX,
  ID_WFS_PAGING,
  ID_WFS_TABLE,
  ID_WFS_PK,
  ID_WFS_RTREE,
  ID_WFS_SWAP,
  ID_WFS_LOAD,
  ID_WFS_EXTRA,
  ID_WFS_KEYWORD,
  ID_WFS_KEYFILTER,
  ID_WFS_KEYRESET,
  ID_WFS_ENABLE_PROXY,
  ID_WFS_PROXY,
  ID_WFS_STATUS,
  ID_WFS_THREAD_FINISHED,
  ID_SLD_SE_GRID,
  ID_SLD_SE_REMOVE,
  ID_SLD_SE_ADD,
  ID_MAP_CONFIG_GRID,
  ID_WAIT_VALIDATING,
  ID_CVG_NAME,
  ID_CVG_TITLE,
  ID_CVG_ABSTRACT,
  ID_CVG_COPYRIGHT,
  ID_CVG_LICENSE,
  ID_CVG_SAMPLE,
  ID_CVG_PIXEL,
  ID_CVG_BANDS,
  ID_CVG_COMPRESSION,
  ID_CVG_QUALITY,
  ID_CVG_RED,
  ID_CVG_GREEN,
  ID_CVG_BLUE,
  ID_CVG_NIR,
  ID_CVG_AUTO_NDVI,
  ID_CVG_NODATA,
  ID_CVG_WIDTH,
  ID_CVG_SQTILE,
  ID_CVG_HEIGHT,
  ID_CVG_SRID,
  ID_CVG_NOREF,
  ID_CVG_HORZ_RES,
  ID_CVG_SAME_RES,
  ID_CVG_VERT_RES,
  ID_CVG_STRICT_RES,
  ID_CVG_MIXED_RES,
  ID_CVG_PATHS,
  ID_CVG_MD5,
  ID_CVG_SUMMARY,
  ID_CVG_QUERYABLE,
  ID_LOAD_FORCE_SRID,
  ID_LOAD_SRID,
  ID_LOAD_WITH_WORLDFILE,
  ID_LOAD_PYRAMIDIZE,
  ID_LOAD_LIST_DONE,
  ID_LOAD_ABORT,
  ID_LOAD_RASTER_THREAD_FINISHED,
  ID_LOAD_RASTER_START,
  ID_LOAD_RASTER_STOP,
  ID_PYRAMID_MODE,
  ID_LOAD_STYLE_DONE,
  ID_LOAD_RASTER_STYLE_THREAD_FINISHED,
  ID_LOAD_RASTER_STYLE_START,
  ID_LOAD_RASTER_STYLE_STOP,
  ID_LOAD_RASTER_STYLE_SKIP,
  ID_LOAD_VECTOR_STYLE_THREAD_FINISHED,
  ID_LOAD_VECTOR_STYLE_START,
  ID_LOAD_VECTOR_STYLE_STOP,
  ID_LOAD_VECTOR_STYLE_SKIP,
  ID_LOAD_MAP_CONFIG_THREAD_FINISHED,
  ID_LOAD_MAP_CONFIG_START,
  ID_LOAD_MAP_CONFIG_STOP,
  ID_LOAD_MAP_CONFIG_SKIP,
  ID_LOAD_EXTERNAL_THREAD_FINISHED,
  ID_LOAD_EXTERNAL_DONE,
  ID_LOAD_EXTERNAL_START,
  ID_LOAD_EXTERNAL_STOP,
  ID_LOAD_EXTERNAL_SKIP,
  ID_LOAD_FONT_THREAD_FINISHED,
  ID_LOAD_FONT_DONE,
  ID_LOAD_FONT_START,
  ID_LOAD_FONT_STOP,
  ID_LOAD_FONT_SKIP,
  ID_VECTOR_GRID,
  ID_VECTOR_COVERAGE,
  ID_VECTOR_TITLE,
  ID_VECTOR_ABSTRACT,
  ID_VECTOR_COPYRIGHT,
  ID_VECTOR_LICENSE,
  ID_VECTOR_QUERYABLE,
  ID_VECTOR_EDITABLE,
  ID_VECTOR_SRID_ADD,
  ID_VECTOR_SRID_REMOVE,
  ID_VECTOR_SRID,
  ID_VECTOR_SRID_GRID,
  ID_VECTOR_REFSYS,
  ID_RASTER_SRID_ADD,
  ID_RASTER_SRID_REMOVE,
  ID_RASTER_SRID,
  ID_RASTER_SRID_GRID,
  ID_RASTER_REFSYS,
  ID_VECTOR_KEYWORD_ADD,
  ID_VECTOR_KEYWORD_REMOVE,
  ID_VECTOR_KEYWORD,
  ID_VECTOR_KEYWORD_GRID,
  ID_RASTER_KEYWORD_ADD,
  ID_RASTER_KEYWORD_REMOVE,
  ID_RASTER_KEYWORD,
  ID_RASTER_KEYWORD_GRID,
  ID_SYMBOLIZER_NAME,
  ID_SYMBOLIZER_TITLE,
  ID_SYMBOLIZER_ABSTRACT,
  ID_SYMBOLIZER_OPACITY,
  ID_SYMBOLIZER_BAND_MODE,
  ID_SYMBOLIZER_RED,
  ID_SYMBOLIZER_GREEN,
  ID_SYMBOLIZER_BLUE,
  ID_SYMBOLIZER_GRAY,
  ID_SYMBOLIZER_CONTRAST,
  ID_SYMBOLIZER_GAMMA,
  ID_SYMBOLIZER_MAP,
  ID_SYMBOLIZER_FALLBACK,
  ID_SYMBOLIZER_VALUE,
  ID_SYMBOLIZER_COLOR,
  ID_SYMBOLIZER_PICKER_HEX,
  ID_SYMBOLIZER_PICKER_BTN,
  ID_SYMBOLIZER_SHADED,
  ID_SYMBOLIZER_RELIEF,
  ID_SYMBOLIZER_MINMAX_SCALE,
  ID_SYMBOLIZER_MIN_SCALE,
  ID_SYMBOLIZER_MAX_SCALE,
  ID_SYMBOLIZER_UOM,
  ID_SYMBOLIZER_STROKE1_ENABLE,
  ID_SYMBOLIZER_STROKE1_OPACITY,
  ID_SYMBOLIZER_STROKE1_PERPENDICULAR,
  ID_SYMBOLIZER_STROKE1_TYPE,
  ID_SYMBOLIZER_STROKE1_COLOR,
  ID_SYMBOLIZER_STROKE1_PICKER_HEX,
  ID_SYMBOLIZER_STROKE1_PICKER_BTN,
  ID_SYMBOLIZER_STROKE1_GRAPHIC,
  ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT,
  ID_SYMBOLIZER_STROKE1_REPLACEMENT,
  ID_SYMBOLIZER_STROKE1_REPLACEMENT_HEX,
  ID_SYMBOLIZER_STROKE1_WIDTH,
  ID_SYMBOLIZER_STROKE1_LINEJOIN,
  ID_SYMBOLIZER_STROKE1_LINECAP,
  ID_SYMBOLIZER_STROKE1_DASHARRAY,
  ID_SYMBOLIZER_STROKE1_DASHOFFSET,
  ID_SYMBOLIZER_STROKE2_ENABLE,
  ID_SYMBOLIZER_STROKE2_OPACITY,
  ID_SYMBOLIZER_STROKE2_PERPENDICULAR,
  ID_SYMBOLIZER_STROKE2_TYPE,
  ID_SYMBOLIZER_STROKE2_COLOR,
  ID_SYMBOLIZER_STROKE2_PICKER_HEX,
  ID_SYMBOLIZER_STROKE2_PICKER_BTN,
  ID_SYMBOLIZER_STROKE2_GRAPHIC,
  ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT,
  ID_SYMBOLIZER_STROKE2_REPLACEMENT,
  ID_SYMBOLIZER_STROKE2_REPLACEMENT_HEX,
  ID_SYMBOLIZER_STROKE2_WIDTH,
  ID_SYMBOLIZER_STROKE2_LINEJOIN,
  ID_SYMBOLIZER_STROKE2_LINECAP,
  ID_SYMBOLIZER_STROKE2_DASHARRAY,
  ID_SYMBOLIZER_STROKE2_DASHOFFSET,
  ID_SYMBOLIZER_STROKE3_ENABLE,
  ID_SYMBOLIZER_STROKE3_OPACITY,
  ID_SYMBOLIZER_STROKE3_PERPENDICULAR,
  ID_SYMBOLIZER_STROKE3_TYPE,
  ID_SYMBOLIZER_STROKE3_COLOR,
  ID_SYMBOLIZER_STROKE3_PICKER_HEX,
  ID_SYMBOLIZER_STROKE3_PICKER_BTN,
  ID_SYMBOLIZER_STROKE3_GRAPHIC,
  ID_SYMBOLIZER_STROKE3_ENABLE_REPLACEMENT,
  ID_SYMBOLIZER_STROKE3_REPLACEMENT,
  ID_SYMBOLIZER_STROKE3_REPLACEMENT_HEX,
  ID_SYMBOLIZER_STROKE3_WIDTH,
  ID_SYMBOLIZER_STROKE3_LINEJOIN,
  ID_SYMBOLIZER_STROKE3_LINECAP,
  ID_SYMBOLIZER_STROKE3_DASHARRAY,
  ID_SYMBOLIZER_STROKE3_DASHOFFSET,
  ID_SYMBOLIZER_SECOND_STROKE_ENABLE,
  ID_SYMBOLIZER_FILL1_ENABLE,
  ID_SYMBOLIZER_FILL1_OPACITY,
  ID_SYMBOLIZER_FILL1_TYPE,
  ID_SYMBOLIZER_FILL1_COLOR,
  ID_SYMBOLIZER_FILL1_PICKER_HEX,
  ID_SYMBOLIZER_FILL1_PICKER_BTN,
  ID_SYMBOLIZER_FILL1_GRAPHIC,
  ID_SYMBOLIZER_FILL1_ENABLE_REPLACEMENT,
  ID_SYMBOLIZER_FILL1_REPLACEMENT,
  ID_SYMBOLIZER_FILL1_REPLACEMENT_HEX,
  ID_SYMBOLIZER_FILL2_ENABLE,
  ID_SYMBOLIZER_FILL2_OPACITY,
  ID_SYMBOLIZER_FILL2_TYPE,
  ID_SYMBOLIZER_FILL2_COLOR,
  ID_SYMBOLIZER_FILL2_PICKER_HEX,
  ID_SYMBOLIZER_FILL2_PICKER_BTN,
  ID_SYMBOLIZER_FILL2_GRAPHIC,
  ID_SYMBOLIZER_FILL2_ENABLE_REPLACEMENT,
  ID_SYMBOLIZER_FILL2_REPLACEMENT,
  ID_SYMBOLIZER_FILL2_REPLACEMENT_HEX,
  ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X,
  ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_Y,
  ID_SYMBOLIZER_POLYGON1_PERPENDICULAR,
  ID_SYMBOLIZER_POLYGON2_ENABLE,
  ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_X,
  ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_Y,
  ID_SYMBOLIZER_POLYGON2_PERPENDICULAR,
  ID_SYMBOLIZER_SIZE,
  ID_SYMBOLIZER_ROTATION,
  ID_SYMBOLIZER_DISPLACEMENT_X,
  ID_SYMBOLIZER_DISPLACEMENT_Y,
  ID_SYMBOLIZER_ANCHOR_X,
  ID_SYMBOLIZER_ANCHOR_Y,
  ID_SYMBOLIZER_TYPE,
  ID_SYMBOLIZER_GRAPHIC,
  ID_SYMBOLIZER_MARK,
  ID_SYMBOLIZER_ENABLE_COLOR_REPLACEMENT,
  ID_SYMBOLIZER_COLOR_REPLACEMENT,
  ID_SYMBOLIZER_COLOR_REPLACEMENT_HEX,
  ID_SYMBOLIZER_REPLACEMENT_PICKER_BTN,
  ID_ONLY_RESCALE_SVG_SYMBOLS,
  ID_SYMBOLIZER_FILL_ENABLE,
  ID_SYMBOLIZER_FILL_COLOR,
  ID_SYMBOLIZER_FILL_PICKER_HEX,
  ID_SYMBOLIZER_FILL_PICKER_BTN,
  ID_SYMBOLIZER_STROKE_ENABLE,
  ID_SYMBOLIZER_STROKE_COLOR,
  ID_SYMBOLIZER_STROKE_PICKER_HEX,
  ID_SYMBOLIZER_STROKE_PICKER_BTN,
  ID_SYMBOLIZER_STROKE_WIDTH,
  ID_SYMBOLIZER_STROKE_LINEJOIN,
  ID_SYMBOLIZER_STROKE_LINECAP,
  ID_SYMBOLIZER_STROKE_DASHARRAY,
  ID_SYMBOLIZER_STROKE_DASHOFFSET,
  ID_SYMBOLIZER_DASH_DOT,
  ID_SYMBOLIZER_DASH_DOT_2,
  ID_SYMBOLIZER_LABEL,
  ID_SYMBOLIZER_FONT,
  ID_SYMBOLIZER_FONT_OPACITY,
  ID_SYMBOLIZER_HALO_ENABLE,
  ID_SYMBOLIZER_HALO_OPACITY,
  ID_SYMBOLIZER_HALO_RADIUS,
  ID_SYMBOLIZER_HALO_COLOR,
  ID_SYMBOLIZER_HALO_PICKER_HEX,
  ID_SYMBOLIZER_HALO_PICKER_BTN,
  ID_SYMBOLIZER_PERPENDICULAR,
  ID_SYMBOLIZER_IS_REPEATED,
  ID_SYMBOLIZER_INITIAL_GAP,
  ID_SYMBOLIZER_GAP,
  ID_SYMBOLIZER_IS_ALIGNED,
  ID_SYMBOLIZER_GENERALIZE,
  ID_SYMBOLIZER_PREVIEW,
  ID_SYMBOLIZER_BACKGROUND,
  ID_SYMBOLIZER_CROSSHAIR,
  ID_SYMBOLIZER_REFLINE,
  ID_SYMBOLIZER_INSERT,
  ID_SYMBOLIZER_EXPORT,
  ID_SYMBOLIZER_COPY,
  ID_SYMBOLIZER_ADD,
  ID_SYMBOLIZER_REMOVE,
  ID_LABEL_NO_GEOM_SYMBOLIZER,
  ID_SYMBOLIZER_TEXT1_ENABLE,
  ID_SYMBOLIZER_TEXT2_ENABLE,
  ID_SYMBOLIZER_TEXT1_BOLD,
  ID_SYMBOLIZER_TEXT2_BOLD,
  ID_SYMBOLIZER_TEXT1_ITALIC,
  ID_SYMBOLIZER_TEXT2_ITALIC,
  ID_SYMBOLIZER_TEXT1_LABEL,
  ID_SYMBOLIZER_FONT1_NAME,
  ID_SYMBOLIZER_FONT2_NAME,
  ID_SYMBOLIZER_TEXT2_LABEL,
  ID_SYMBOLIZER_FONT1_SIZE,
  ID_SYMBOLIZER_FONT2_SIZE,
  ID_SYMBOLIZER_FONT1_OPACITY,
  ID_SYMBOLIZER_FONT2_OPACITY,
  ID_SYMBOLIZER_FONT1_COLOR,
  ID_SYMBOLIZER_FONT2_COLOR,
  ID_SYMBOLIZER_FONT1_PICKER_HEX,
  ID_SYMBOLIZER_FONT2_PICKER_HEX,
  ID_SYMBOLIZER_FONT1_PICKER_BTN,
  ID_SYMBOLIZER_FONT2_PICKER_BTN,
  ID_SYMBOLIZER_HALO1_ENABLE,
  ID_SYMBOLIZER_HALO1_OPACITY,
  ID_SYMBOLIZER_HALO1_RADIUS,
  ID_SYMBOLIZER_HALO1_COLOR,
  ID_SYMBOLIZER_HALO1_PICKER_HEX,
  ID_SYMBOLIZER_HALO1_PICKER_BTN,
  ID_SYMBOLIZER_HALO2_ENABLE,
  ID_SYMBOLIZER_HALO2_OPACITY,
  ID_SYMBOLIZER_HALO2_RADIUS,
  ID_SYMBOLIZER_HALO2_COLOR,
  ID_SYMBOLIZER_HALO2_PICKER_HEX,
  ID_SYMBOLIZER_HALO2_PICKER_BTN,
  ID_SYMBOLIZER_TEXT_ANCHOR_X,
  ID_SYMBOLIZER_TEXT_ANCHOR_Y,
  ID_SYMBOLIZER_TEXT_DISPLACEMENT_X,
  ID_SYMBOLIZER_TEXT_DISPLACEMENT_Y,
  ID_SYMBOLIZER_TEXT_ROTATION,
  ID_SYMBOLIZER_TEXT_PERPENDICULAR,
  ID_SYMBOLIZER_TEXT_IS_REPEATED,
  ID_SYMBOLIZER_TEXT_INITIAL_GAP,
  ID_SYMBOLIZER_TEXT_GAP,
  ID_SYMBOLIZER_TEXT_IS_ALIGNED,
  ID_SYMBOLIZER_TEXT_GENERALIZE,
  ID_SYMBOLIZER_NODE_OPACITY,
  ID_SYMBOLIZER_NODE_MARK,
  ID_SYMBOLIZER_NODE_SIZE,
  ID_SYMBOLIZER_NODE_ROTATION,
  ID_SYMBOLIZER_NODE_DISPLACEMENT_X,
  ID_SYMBOLIZER_NODE_DISPLACEMENT_Y,
  ID_SYMBOLIZER_NODE_ANCHOR_X,
  ID_SYMBOLIZER_NODE_ANCHOR_Y,
  ID_SYMBOLIZER_NODE_FILL_COLOR,
  ID_SYMBOLIZER_NODE_FILL_PICKER_HEX,
  ID_SYMBOLIZER_NODE_FILL_PICKER_BTN,
  ID_SYMBOLIZER_NODE_STROKE_COLOR,
  ID_SYMBOLIZER_NODE_STROKE_PICKER_HEX,
  ID_SYMBOLIZER_NODE_STROKE_PICKER_BTN,
  ID_SYMBOLIZER_EDGELINK_SEED_OPACITY,
  ID_SYMBOLIZER_EDGELINK_SEED_MARK,
  ID_SYMBOLIZER_EDGELINK_SEED_SIZE,
  ID_SYMBOLIZER_EDGELINK_SEED_ROTATION,
  ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_X,
  ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_Y,
  ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_X,
  ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_Y,
  ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR,
  ID_SYMBOLIZER_EDGELINK_SEED_FILL_PICKER_HEX,
  ID_SYMBOLIZER_EDGELINK_SEED_FILL_PICKER_BTN,
  ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR,
  ID_SYMBOLIZER_EDGELINK_SEED_STROKE_PICKER_HEX,
  ID_SYMBOLIZER_EDGELINK_SEED_STROKE_PICKER_BTN,
  ID_SYMBOLIZER_FACE_SEED_OPACITY,
  ID_SYMBOLIZER_FACE_SEED_MARK,
  ID_SYMBOLIZER_FACE_SEED_SIZE,
  ID_SYMBOLIZER_FACE_SEED_ROTATION,
  ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_X,
  ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_Y,
  ID_SYMBOLIZER_FACE_SEED_ANCHOR_X,
  ID_SYMBOLIZER_FACE_SEED_ANCHOR_Y,
  ID_SYMBOLIZER_FACE_SEED_FILL_COLOR,
  ID_SYMBOLIZER_FACE_SEED_FILL_PICKER_HEX,
  ID_SYMBOLIZER_FACE_SEED_FILL_PICKER_BTN,
  ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR,
  ID_SYMBOLIZER_FACE_SEED_STROKE_PICKER_HEX,
  ID_SYMBOLIZER_FACE_SEED_STROKE_PICKER_BTN,
  ID_SYMBOLIZER_COLOR_MAP_MODE,
  ID_SYMBOLIZER_MIN_COLOR,
  ID_SYMBOLIZER_MIN_PICKER_HEX,
  ID_SYMBOLIZER_MIN_PICKER_BTN,
  ID_SYMBOLIZER_MAX_COLOR,
  ID_SYMBOLIZER_MAX_PICKER_HEX,
  ID_SYMBOLIZER_MAX_PICKER_BTN,
  ID_WMS_URL,
  ID_WMS_INFO_URL,
  ID_WMS_CATALOG,
  ID_WMS_RESET,
  ID_WMS_SERVER,
  ID_WMS_VERSION,
  ID_WMS_CRS,
  ID_WMS_STYLE,
  ID_WMS_FORMAT,
  ID_WMS_TRANSPARENT,
  ID_WMS_TILED,
  ID_WMS_WIDTH,
  ID_WMS_HEIGHT,
  ID_WMS_NAME,
  ID_WMS_TITLE,
  ID_WMS_ABSTRACT,
  ID_WMS_COPYRIGHT,
  ID_WMS_LICENSE,
  ID_WMS_QUERYABLE,
  ID_WMS_GETFEATUREINFO,
  ID_WMS_SWAP,
  ID_WMS_CACHED,
  ID_WMS_OK,
  ID_WMS_ENABLE_PROXY,
  ID_WMS_PROXY,
  ID_WMS_LIST,
  ID_WMS_LAYER,
  ID_WMS_ENABLE_BGCOLOR,
  ID_WMS_BGCOLOR,
  ID_RASTER_STYLE,
  ID_RASTER_OK,
  ID_VECTOR_LAYER,
  ID_VECTOR_TYPE,
  ID_VECTOR_UUID,
  ID_VECTOR_STYLE,
  ID_VECTOR_OK,
  ID_VECTOR_COPY,
  ID_MIME_TYPE,
  ID_IMAGE_QUALITY,
  ID_SQL_SAMPLE,
  ID_TOPOGEO_LAYER,
  ID_TOPOGEO_SRID,
  ID_TOPOGEO_STYLE,
  ID_TOPOGEO_FACE,
  ID_TOPOGEO_EDGE,
  ID_TOPOGEO_NODE,
  ID_TOPOGEO_FACE_SEED,
  ID_TOPOGEO_EDGE_SEED,
  ID_TOPOGEO_OK,
  ID_TOPONET_LAYER,
  ID_TOPONET_SRID,
  ID_TOPONET_STYLE,
  ID_TOPONET_LINK,
  ID_TOPONET_NODE,
  ID_TOPONET_LINK_SEED,
  ID_TOPONET_OK,
  ID_TOPO_NAME,
  ID_TOPO_SRID,
  ID_TOPO_3D,
  ID_TOPO_TOLERANCE,
  ID_TOPO_SPATIAL,
  ID_TOPO_COINCIDENT,
  ID_LAYER_TYPE,
  ID_PANE_MAIN,
  ID_PANE_STROKE1,
  ID_PANE_STROKE2,
  ID_PANE_STROKE3,
  ID_PANE_FILL1,
  ID_PANE_FILL2,
  ID_PANE_POSITION,
  ID_PANE_GRAPHIC,
  ID_PANE_MARK,
  ID_PANE_FONT,
  ID_PANE_PLACEMENT,
  ID_PANE_PREVIEW,
  ID_PANE_TEXT1,
  ID_PANE_TEXT2,
  ID_LAYERS_LIST,
  ID_MAPLAYER_NAME,
  ID_MAPLAYER_TITLE,
  ID_MAPLAYER_ABSTRACT,
  ID_MAPLAYER_COPYRIGHT,
  ID_MAPLAYER_LICENSE,
  ID_MAPLAYER_DATASOURCE,
  ID_MAPLAYER_EXTENTS,
  ID_MAPLAYER_QUERYABLE,
  ID_MAPLAYER_EDITABLE,
  ID_MAPOPT_MULTITHREAD,
  ID_MAPOPT_MAXTHREADS,
  ID_MAPOPT_AUTOTRANSFORM,
  ID_MAPOPT_SRID,
  ID_MAPOPT_NAME,
  ID_MAPOPT_EXTENTS,
  ID_MAPOPT_AUTOSWITCH,
  ID_MAPOPT_DD_DMS,
  ID_MAPOPT_CHECKERED_BACKGROUND,
  ID_MAPOPT_SOLID_BACKGROUND,
  ID_MAPOPT_ANTI_COLLISION,
  ID_MAPOPT_WRAP_TEXT,
  ID_MAPOPT_AUTO_ROTATE,
  ID_MAPOPT_SHIFT_POSITION,
  ID_ADDALL_SRID_NEW,
  ID_ADDALL_NAME_NEW,
  ID_ADDALL_SRID_OLD,
  ID_ADDALL_NAME_OLD,
  ID_QUICK_STYLE_APPLY,
  ID_QUICK_STYLE_EXPORT,
  ID_QUICK_STYLE_COPY,
  ID_PANE_POINT,
  ID_PANE_LINE,
  ID_PANE_POLYGON,
  ID_PANE_TEXT,
  ID_PANE_CONTRAST_ENHANCEMENT,
  ID_PANE_CHANNEL_SELECTION,
  ID_PANE_COLOR_MAP,
  ID_PAINT_MAP_STEP,
  ID_PAINT_MAP_THREAD_FINISHED,
  ID_POSTGRES_HOST,
  ID_POSTGRES_HOSTADDR,
  ID_POSTGRES_PORT,
  ID_POSTGRES_DBNAME,
  ID_POSTGRES_USER,
  ID_POSTGRES_PASSWORD,
  ID_POSTGRES_RDONLY,
  ID_POSTGRES_TEXTDATES,
  ID_PRINTER_FORMAT,
  ID_PRINTER_PORTRAIT,
  ID_PRINTER_DPI,
  ID_PRINTER_MARGIN,
  ID_PRINTER_WIDTH,
  ID_PRINTER_HEIGTH,
  ID_MAP_IMAGE_WIDTH,
  ID_MAP_IMAGE_HEIGTH,
  ID_MAP_IMAGE_FORMAT,
  ID_MAP_IMAGE_COMPRESSION,
  ID_MAP_IMAGE_QUALITY,
  ID_MAP_IMAGE_WORLD_FILE,
  ID_MAP_IMAGE_GEO_TIFF,
  ID_ZIPSHP,
  ID_ZIPDBF
};

enum
{
// tree item data types
  MY_ROOT_NODE = 0,
  MY_ROOT_USERDATA,
  MY_ROOT_ISOMETADATA,
  MY_ROOT_TOPOLOGIES,
  MY_ROOT_NETWORKS,
  MY_ROOT_RASTER,
  MY_ROOT_VECTOR,
  MY_ROOT_WMS,
  MY_ROOT_POSTGRESQL,
  MY_ROOT_STYLING,
  MY_ROOT_METADATA,
  MY_ROOT_INTERNAL,
  MY_ROOT_RTREE,
  MY_TABLE,
  MY_VTABLE,
  MY_VIEW,
  MY_TILE_DATA,
  MY_GPKG_TABLE,
  MY_GPKG_VTABLE,
  MY_FDO_TABLE,
  MY_FDO_VTABLE,
  MY_COLUMN,
  MY_VIEW_COLUMN,
  MY_VIRTUAL_COLUMN,
  MY_GEOMETRY,
  MY_GEOMETRY_INDEX,
  MY_GEOMETRY_CACHED,
  MY_VIEW_GEOMETRY,
  MY_VIEW_GEOMETRY_INDEX,
  MY_VIEW_GEOMETRY_CACHED,
  MY_VIRTUAL_GEOMETRY,
  MY_GPKG_COLUMN,
  MY_GPKG_GEOMETRY,
  MY_GPKG_GEOMETRY_INDEX,
  MY_VIRTUAL_GPKG_GEOMETRY,
  MY_FDO_COLUMN,
  MY_FDO_GEOMETRY,
  MY_VIRTUAL_FDO_GEOMETRY,
  MY_TOPO_GEO,
  MY_TOPO_NET,
  MY_VECTOR_COVERAGE,
  MY_RASTER_COVERAGE,
  MY_WMS_LAYER,
  MY_INDEX,
  MY_INDEX_FLD,
  MY_TRIGGER,
  MY_ATTACHED,
  MY_PRIMARY_KEY,
  MY_PRIMARY_KEY_FLD,
  MY_FOREIGN_KEY,
  MY_FOREIGN_KEY_FLD,
  MY_INT_VARIANT,
  MY_DBL_VARIANT,
  MY_TXT_VARIANT,
  MY_BLOB_VARIANT,
  MY_NULL_VARIANT,
  MY_UNDEFINED,
  MY_POSTGRES_CONN,
  MY_POSTGRES_SCHEMA,
  MY_POSTGRES_TABLE,
  MY_POSTGRES_VIEW,
  MY_POSTGIS_VIEW,
  MY_POSTGRES_COLUMN,
  MY_POSTGIS_GEOMETRY
};

enum
{
// control IDs for timers
  ID_AUTO_SAVE_TIMER = 20000,
  ID_DB_STATUS_TIMER,
  ID_WFS_TIMER,
  ID_WHEEL_TIMER,
  ID_MAP_PAINT_TIMER,
  ID_MAP_BLINK_TIMER,
  ID_MAP_TIP_TIMER
};

enum
{
// control IDs for Keyboard Shortcuts
  KEY_Center = 37000,
  KEY_Up,
  KEY_MicroUp,
  KEY_Down,
  KEY_MicroDown,
  KEY_Left,
  KEY_MicroLeft,
  KEY_Right,
  KEY_MicroRight,
  KEY_ZoomIn,
  KEY_MicroZoomIn,
  KEY_ZoomOut,
  KEY_MicroZoomOut
};

enum
{
// DOT-COMMANDS [SQL scripts]
  CMD_NONE = 0,
  CMD_LOADSHP,
  CMD_LOADDBF,
  CMD_LOADXL,
  CMD_DUMPSHP,
  CMD_DUMPDBF,
  CMD_SQLLOG
};

enum
{
// METADATA TYPEs
  METADATA_UNKNOWN = 0,
  METADATA_LEGACY,
  METADATA_CURRENT
};

enum
{
// Map Layer TYPEs
  MAP_LAYER_UNKNOWN = 0,
  MAP_LAYER_RASTER,
  MAP_LAYER_WMS,
  MAP_LAYER_VECTOR,
  MAP_LAYER_VECTOR_VIEW,
  MAP_LAYER_VECTOR_VIRTUAL,
  MAP_LAYER_TOPOLOGY,
  MAP_LAYER_NETWORK
};

enum
{
// Quick Style TYPEs and dot-styles
  QUICK_STYLE_UNKNOWN = 0,
  QUICK_STYLE_POINT,
  QUICK_STYLE_LINE,
  QUICK_STYLE_POLYGON,
  QUICK_STYLE_GEOMETRY,
  QUICK_STYLE_TOPOGEO,
  QUICK_STYLE_TOPONET,
  QUICK_STYLE_SOLID_LINE,
  QUICK_STYLE_DOT_LINE,
  QUICK_STYLE_DASH_LINE,
  QUICK_STYLE_DASH_DOT_LINE
};

enum
{
// Graphic Standard Brushes
  GRAPHIC_BRUSH_HORZ = 0,
  GRAPHIC_BRUSH_VERT,
  GRAPHIC_BRUSH_CROSS,
  GRAPHIC_BRUSH_DIAG1,
  GRAPHIC_BRUSH_DIAG2,
  GRAPHIC_BRUSH_CROSS_DIAG,
  GRAPHIC_BRUSH_DOTS
};

enum
{
// predefined Color Maps
  COLOR_MAP_NONE = 0,
  COLOR_MAP_ETOPO2,
  COLOR_MAP_SRTM,
  COLOR_MAP_TERRAIN,
  COLOR_MAP_ELEVATION,
  COLOR_MAP_ASPECT_COLOR,
  COLOR_MAP_RAINBOW,
  COLOR_MAP_WAVE,
  COLOR_MAP_SEPIA
};

enum
{
// constants for PrinterPlotter
  MY_PAPER_A0 = 0,
  MY_PAPER_A1,
  MY_PAPER_A2,
  MY_PAPER_A3,
  MY_PAPER_A4,
  MY_PAPER_A5,
  MY_IMAGE_PNG,
  MY_IMAGE_JPEG,
  MY_IMAGE_TIFF,
  MY_IMG_COMPRESSION_NONE,
  MY_IMG_COMPRESSION_DEFLATE,
  MY_IMG_COMPRESSION_JPEG,
  MY_IMG_COMPRESSION_LZMA
};

class RasterCoverageItem
{
//
// a class wrapping a Raster Coverage related Table or View
//
private:
  wxString Name;
  bool TileData;
  RasterCoverageItem *Next;
public:
    RasterCoverageItem(wxString & name, bool tile_data);
   ~RasterCoverageItem()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  bool IsTileData()
  {
    return TileData;
  }
  void SetNext(RasterCoverageItem * next)
  {
    Next = next;
  }
  RasterCoverageItem *GetNext()
  {
    return Next;
  }
};

class RasterCoverageSet
{
//
// a class representing a full Raster Coverage Set
//
private:
  wxString Name;
  int SRID;
public:
    RasterCoverageSet(const char *name, int srid);
   ~RasterCoverageSet()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  int GetSRID()
  {
    return SRID;
  }
};

class VectorCoverageItem
{
//
// a class wrapping a Vector Coverage related Table or View
//
private:
  wxString Name;
  VectorCoverageItem *Next;
public:
    VectorCoverageItem(wxString & name);
   ~VectorCoverageItem()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetNext(VectorCoverageItem * next)
  {
    Next = next;
  }
  VectorCoverageItem *GetNext()
  {
    return Next;
  }
};

class VectorCoverageSet
{
//
// a class representing a full Vector Coverage Set
//
private:
  wxString Prefix;
  wxString Name;
  int SRID;
  int GeometryType;
public:
    VectorCoverageSet(const char *prefix, const char *name, int srid, int type);
   ~VectorCoverageSet()
  {;
  }
  wxString & GetPrefix()
  {
    return Prefix;
  }
  wxString & GetName()
  {
    return Name;
  }
  int GetSRID()
  {
    return SRID;
  }
  int GetGeometryType()
  {
    return GeometryType;
  }
};

class MyObject:public wxTreeItemData
{
//
// a class to store TreeItemData
//
private:
  int Type;                     // the object type
  int SubType;                  // the object sub-type  
  wxString DbAlias;             // the DB alias [Attached DB] 
  wxString MainName;            // the object name [usually: table name]
  wxString ColName;             // the column name [optional]
  wxString PathOrURL;           // the Path or request URL [optional]
  bool Restricted;              // not editable
  bool TopoObject;              // Topology or Network Table
// PostgreSQL specific
  wxString Host;
  wxString HostAddr;
  int Port;
  wxString DbName;
  wxString User;
  wxString Schema;
  wxString Name;
  wxString Column;
  wxString VirtName;
  bool ReadOnly;
  bool PK;
  bool Select;
  bool InsertUpdateDelete;
public:
    MyObject(int type);
    MyObject(int type, wxString & dbAlias);
    MyObject(int type, wxString & dbAlias, wxString & name, bool restricted =
             false, bool topoObj = false);
    MyObject(int type, wxString & dbAlias, wxString & name, wxString & column);
    MyObject(int type, wxString & dbAlias, wxString & name, bool topo_obj,
             wxString & column);
    MyObject(int type, wxString & dbAlias, wxString & name, wxString & column,
             wxString & path_or_url);
    MyObject(int type, wxString & host, wxString & hostaddr, int port,
             wxString & dbname, wxString & user, bool readOnly);
    MyObject(int type, wxString & host, wxString & hostaddr, int port,
             wxString & dbname, wxString & user, wxString & schema);
    MyObject(int type, wxString & host, wxString & hostaddr, int port,
             wxString & dbname, wxString & user, wxString & schema,
             wxString & name, wxString & virtname, bool readOnly, bool pk,
             bool select, bool insertUpdateDelete);
    MyObject(int type, wxString & host, wxString & hostaddr, int port,
             wxString & dbname, wxString & user, wxString & schema,
             wxString & name, wxString & column, wxString & virtname);
    virtual ~ MyObject()
  {;
  }
  int GetType()
  {
    return Type;
  }
  int GetSubType()
  {
    return SubType;
  }
  bool IsTopoObject()
  {
    return TopoObject;
  }
  wxString & GetDbAlias()
  {
    return DbAlias;
  }
  wxString & GetMainName()
  {
    return MainName;
  }
  wxString & GetColName()
  {
    return ColName;
  }
  wxString & GetPathOrURL()
  {
    return PathOrURL;
  }
  wxString & GetHost()
  {
    return Host;
  }
  wxString & GetHostAddr()
  {
    return HostAddr;
  }
  int GetPort()
  {
    return Port;
  }
  wxString & GetDbName()
  {
    return DbName;
  }
  wxString & GetUser()
  {
    return User;
  }
  wxString & GetSchema()
  {
    return Schema;
  }
  wxString & GetName()
  {
    return Name;
  }
  wxString & GetColumn()
  {
    return Column;
  }
  wxString & GetVirtName()
  {
    return VirtName;
  }
  bool IsReadOnly()
  {
    return ReadOnly;
  }
  bool HasPK()
  {
    return PK;
  }
  bool CanSelect()
  {
    return Select;
  }
  bool CanInsertUpdateDelete()
  {
    return InsertUpdateDelete;
  }
  bool IsTable();
  bool IsView();
  bool IsTemporary();
  bool IsEditable();
  bool IsAttachedDB();
  bool IsRootAttached();
  bool IsAttached();
  bool IsColumn();
  bool IsGpkgColumn();
  bool IsFdoOgrColumn();
  bool IsVirtualColumn();
  bool IsViewColumn();
  bool IsAnyGeometryColumn();
  bool IsGpkgGeometryColumn();
  bool IsGpkgVirtualGeometryColumn();
  bool IsFdoOgrGeometryColumn();
  bool IsFdoOgrVirtualGeometryColumn();
  bool IsGeometryColumn();
  bool IsIndex();
  bool IsIndexColumn();
  bool IsTrigger();
  bool IsPrimaryKey();
  bool IsPrimaryKeyColumn();
  bool IsForeignKey();
  bool IsForeignKeyColumn();
  bool IsPostgreSQL();
  bool IsAlreadyDefinedVectorCoverage(sqlite3 * sqlite);
  char *DoFindUnusedCoverageName(sqlite3 * sqlite);
};

class MyColumnInfo
{
//
// a class to store a DB column
//
private:
  wxString Name;                // the column name
  bool PrimaryKey;              // Primary Key column
  bool Geometry;                // Geometry column
  bool GPKGGeometry;            // GPKG Geometry column
  bool GPKGVirtualGeometry;     // GPKG Virtual Geometry column
  bool GeometryIndex;           // Geometry column + SpatialIndex
  bool MbrCache;                // Geometry column + MbrCache
  bool GPKGGeometryIndex;       // GPKG Geometry column + SpatialIndex
  bool FdoOgrGeometry;          // FDO/OGR Geometry column
  bool FdoOgrVirtualGeometry;   // FDO/OGR Virtual Geometry column
  MyColumnInfo *Next;           // pointer to next element into the linked list
public:
    MyColumnInfo(wxString & name, bool pkey);
   ~MyColumnInfo()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  bool IsPrimaryKey()
  {
    return PrimaryKey;
  }
  void SetGeometry()
  {
    Geometry = true;
  }
  bool IsGeometry()
  {
    return Geometry;
  }
  void SetGPKGGeometry()
  {
    GPKGGeometry = true;
  }
  bool IsGPKGGeometry()
  {
    return GPKGGeometry;
  }
  void SetGPKGVirtualGeometry()
  {
    GPKGVirtualGeometry = true;
  }
  bool IsGPKGVirtualGeometry()
  {
    return GPKGVirtualGeometry;
  }
  void SetFdoOgrGeometry()
  {
    FdoOgrGeometry = true;
  }
  bool IsFdoOgrGeometry()
  {
    return FdoOgrGeometry;
  }
  void SetFdoOgrVirtualGeometry()
  {
    FdoOgrVirtualGeometry = true;
  }
  bool IsFdoOgrVirtualGeometry()
  {
    return FdoOgrVirtualGeometry;
  }
  void SetGeometryIndex()
  {
    GeometryIndex = true;
  }
  bool IsGeometryIndex()
  {
    return GeometryIndex;
  }
  void SetMbrCache()
  {
    MbrCache = true;
  }
  bool IsMbrCache()
  {
    return MbrCache;
  }
  void SetGPKGGeometryIndex()
  {
    GPKGGeometryIndex = true;
  }
  bool IsGPKGGeometryIndex()
  {
    return GPKGGeometryIndex;
  }
  void SetNext(MyColumnInfo * next)
  {
    Next = next;
  }
  MyColumnInfo *GetNext()
  {
    return Next;
  }
};

class MyIndexInfo
{
//
// a class to store a DB index
//
private:
  wxString Name;                // the index name
  MyIndexInfo *Next;            // pointer to next element into the linked list
public:
    MyIndexInfo(wxString & name);
    MyIndexInfo()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetNext(MyIndexInfo * next)
  {
    Next = next;
  }
  MyIndexInfo *GetNext()
  {
    return Next;
  }
  bool ContainsOnlyPrimaryKeyColumns(sqlite3 * sqlite, wxString & indexName,
                                     MyColumnInfo * first_column);
};

class MyTriggerInfo
{
//
// a class to store a DB trigger
//
private:
  wxString Name;                // the trigger name
  MyTriggerInfo *Next;          // pointer to next element into the linked list
public:
    MyTriggerInfo(wxString & name);
   ~MyTriggerInfo()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetNext(MyTriggerInfo * next)
  {
    Next = next;
  }
  MyTriggerInfo *GetNext()
  {
    return Next;
  }
};

class MyTableInfo
{
//
// a class to store DB table columns
//
private:
  MyColumnInfo * FirstColumn;   // first element into the columns linked list
  MyColumnInfo *LastColumn;     // last element into the columns linked list
  MyIndexInfo *FirstIndex;      // first element into the indices linked list
  MyIndexInfo *LastIndex;       // last element into the indices linked list
  MyTriggerInfo *FirstTrigger;  // first element into the triggers linked list
  MyTriggerInfo *LastTrigger;   // last element into the triggers linked list
public:
    MyTableInfo()
  {
    FirstColumn = NULL;
    LastColumn = NULL;
    FirstIndex = NULL;
    LastIndex = NULL;
    FirstTrigger = NULL;
    LastTrigger = NULL;
  }
   ~MyTableInfo();
  void AddColumn(wxString & name, bool pkey);
  void SetGeometry(wxString & name, bool index, bool cached);
  void AddIndex(wxString & name);
  void AddTrigger(wxString & name);
  MyColumnInfo *GetFirstColumn()
  {
    return FirstColumn;
  }
  MyIndexInfo *GetFirstIndex()
  {
    return FirstIndex;
  }
  MyTriggerInfo *GetFirstTrigger()
  {
    return FirstTrigger;
  }
  void CheckGPKG(class MyFrame * MainFrame, sqlite3 * handle, wxString & table);
  void CheckFdoOgr(MyFrame * MainFrame, sqlite3 * handle, wxString & table);
};

class MyViewInfo
{
//
// a class to store DB view columns
//
private:
  MyColumnInfo * First;         // first element into the columns linked list
  MyColumnInfo *Last;           // last element into the columns linked list
  MyTriggerInfo *FirstTrigger;  // first element into the triggers linked list
  MyTriggerInfo *LastTrigger;   // last element into the triggers linked list
public:
    MyViewInfo()
  {
    First = NULL;
    Last = NULL;
    FirstTrigger = NULL;
    LastTrigger = NULL;
  }
   ~MyViewInfo();
  void AddColumn(wxString & name);
  void AddTrigger(wxString & name);
  void SetGeometry(wxString & name, bool index, bool cached);
  MyColumnInfo *GetFirst()
  {
    return First;
  }
  MyTriggerInfo *GetFirstTrigger()
  {
    return FirstTrigger;
  }
};

class MyVariant
{
//
// a class to store Variant-Type values
//
private:
  int Type;                     // the Variant-Type
  sqlite3_int64 IntValue;       // the Integer value
  double DblValue;              // the Double value
  wxString TxtValue;            // the Text value
  unsigned char *Blob;          // the BLOB value
  int BlobSize;                 // the BLOB size
public:
    MyVariant()
  {
    Type = MY_NULL_VARIANT;
    Blob = NULL;
  }
   ~MyVariant()
  {
    if (Blob)
      delete[]Blob;
  }
  void Clear()
  {
    if (Blob)
      delete[]Blob;
    Blob = NULL;
    Type = MY_NULL_VARIANT;
  }
  void Set(sqlite3_int64 value)
  {
    Type = MY_INT_VARIANT;
    IntValue = value;
  }
  void Set(double value)
  {
    Type = MY_DBL_VARIANT;
    DblValue = value;
  }
  void Set(const unsigned char *text);
  void Set(wxString & string)
  {
    Type = MY_TXT_VARIANT;
    TxtValue = string;
  }
  void Set(const void *blob, int size);
  void Copy(MyVariant * other);
  int GetType()
  {
    return Type;
  }
  sqlite3_int64 GetIntValue()
  {
    return IntValue;
  }
  double GetDblValue()
  {
    return DblValue;
  }
  wxString & GetTxtValue()
  {
    return TxtValue;
  }
  int GetBlobSize()
  {
    return BlobSize;
  }
  unsigned char *GetBlob()
  {
    return Blob;
  }
};

class MyRowVariant
{
//
// a class to store a row composed of Variant-Type values
//
private:
  int NumCols;                  // number of columns
  MyVariant *ColumnArray;       // the column as an array
  bool Deleted;                 // switch to mark row deletion
  MyRowVariant *Next;           // pointer to next element into the linked list
public:
    MyRowVariant()
  {
    NumCols = 0;
    ColumnArray = NULL;
    Deleted = false;
    Next = NULL;
  }
  MyRowVariant(int cols)
  {
    NumCols = cols;
    ColumnArray = new MyVariant[cols];
    Next = NULL;
  }
  ~MyRowVariant()
  {
    if (ColumnArray)
      delete[]ColumnArray;
  }
  void Create(int cols);
  int GetNumCols()
  {
    return NumCols;
  }
  void Set(int col, sqlite3_int64 value);
  void Set(int col, double value);
  void Set(int col, const unsigned char *text);
  void Set(int col, const void *blob, int size);
  MyVariant *GetColumn(int col);
  void SetDeleted()
  {
    Deleted = true;
  }
  bool IsDeleted()
  {
    return Deleted;
  }
  void SetNext(MyRowVariant * next)
  {
    Next = next;
  }
  MyRowVariant *GetNext()
  {
    return Next;
  }
};

class MyVariantList
{
//
// a class to store a whole result set
//
private:
  int NumCols;                  // number of columns
  wxString *ColumnName;         // the column names
  MyRowVariant *First;          // first element into the linked list
  MyRowVariant *Last;           // last element into the linked list
public:
    MyVariantList();
   ~MyVariantList();
  void Reset(void);
  MyRowVariant *Add(int columns);
  void SetColumnName(int col, const char *colName);
  MyRowVariant *GetFirst()
  {
    return First;
  }
  int GetRows();
  int GetColumns()
  {
    return NumCols;
  }
  wxString & GetColumnName(int col);
};

class MyBlobs
{
//
// a class to store BLOBs
//
private:
  int NumRows;                  // the number of rows
  int NumCols;                  // the number of columns
  MyRowVariant *Rows;           // pointer to an array of rows
public:
    MyBlobs(int rows, int cols);
   ~MyBlobs();
  void SetBlob(int row, int col, MyVariant * blobVar);
  MyVariant *GetBlob(int row, int col);
};

class MyValues
{
//
// a class to store column values for editing
//
private:
  int NumRows;                  // the number of rows
  int NumCols;                  // the number of columns
  MyRowVariant *Rows;           // pointer to an array of rows
public:
    MyValues(int rows, int cols);
   ~MyValues();
  void SetValue(int row, int col, sqlite3_int64 value);
  void SetValue(int row, int col, double value);
  void SetValue(int row, int col, wxString & string);
  MyRowVariant *GetRow(int row);
  MyVariant *GetValue(int row, int col);
};

class MySqlQuery
{
//
// a class to store an SQL query - history
//
private:
  wxString Sql;
  MySqlQuery *Prev;
  MySqlQuery *Next;
public:
    MySqlQuery(wxString & sql)
  {
    Sql = sql;
    Prev = NULL;
    Next = NULL;
  }
   ~MySqlQuery()
  {;
  }
  wxString & GetSql()
  {
    return Sql;
  }
  void SetPrev(MySqlQuery * prev)
  {
    Prev = prev;
  }
  MySqlQuery *GetPrev()
  {
    return Prev;
  }
  void SetNext(MySqlQuery * next)
  {
    Next = next;
  }
  MySqlQuery *GetNext()
  {
    return Next;
  }
};

class MySqlHistory
{
//
// a class supporting SQL queries history
//
private:
  MySqlQuery * First;
  MySqlQuery *Last;
  MySqlQuery *Current;
public:
    MySqlHistory()
  {
    First = NULL;
    Last = NULL;
    Current = NULL;
  }
   ~MySqlHistory();
  void Prepend(wxString & sql);
  void Add(wxString & sql);
  MySqlQuery *GetCurrent()
  {
    return Current;
  }
  MySqlQuery *GetNext();
  MySqlQuery *GetPrev();
  bool TestNext();
  bool TestPrev();
};

class MyApp:public wxApp
{
//
// the main APP
//
  virtual bool OnInit();
};

class TopoGeoItem
{
//
// a class wrapping a TopoGeo related Table or View
//
private:
  wxString Name;
  TopoGeoItem *Next;
public:
    TopoGeoItem(wxString & name);
   ~TopoGeoItem()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetNext(TopoGeoItem * next)
  {
    Next = next;
  }
  TopoGeoItem *GetNext()
  {
    return Next;
  }
};

class TopoGeoSet
{
//
// a class representing a full TopoGeo Set
//
private:
  wxString DbPrefix;
  wxString Name;
  int SRID;
  bool HasZ;
public:
    TopoGeoSet(const char *name, int srid, bool has_z);
    TopoGeoSet(const char *db_prefix, const char *name, int srid, bool has_z);
   ~TopoGeoSet()
  {;
  }
  wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetName()
  {
    return Name;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool GetHasZ()
  {
    return HasZ;
  }
};

class TopoGeo
{
//
// TopoGeo container
//
private:
  sqlite3 * DbHandle;
  wxString DbPrefix;
  wxString Name;
  wxTreeItemId TopologyNode;
  TopoGeo *Next;
public:
    TopoGeo(class MyTableTree * tree, wxTreeItemId & root,
            wxString & topology, int srid, bool has_z);
    TopoGeo(class MyTableTree * tree, wxTreeItemId & root,
            wxString & db_prefix, wxString & topology, int srid, bool has_z);
   ~TopoGeo()
  {;
  }
  wxTreeItemId *Check(wxString & table);
  bool CheckTopoFeature(wxString & table);
  TopoGeo *GetNext()
  {
    return Next;
  }
  void SetNext(TopoGeo * next)
  {
    Next = next;
  }
};

class TopoGeoList
{
//
// TopoGeo container
//
private:
  TopoGeo * First;
  TopoGeo *Last;
  int Count;
public:
    TopoGeoList()
  {
    First = NULL;
    Last = NULL;
    Count = 0;
  }
   ~TopoGeoList()
  {
    Flush();
  }
  void Flush();
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & topology, int srid, bool has_z);
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & db_prefix, wxString & topology, int srid, bool has_z);
  wxTreeItemId *FindNode(wxString & table);
  int GetCount()
  {
    return Count;
  }
};

class TopoNetItem
{
//
// a class wrapping a TopoNet related Table or View
//
private:
  wxString Name;
  TopoNetItem *Next;
public:
    TopoNetItem(wxString & name);
   ~TopoNetItem()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetNext(TopoNetItem * next)
  {
    Next = next;
  }
  TopoNetItem *GetNext()
  {
    return Next;
  }
};

class TopoNetSet
{
//
// a class representing a full TopoNet Set
//
private:
  wxString DbPrefix;
  wxString Name;
  bool Spatial;
  int SRID;
  bool HasZ;
public:
    TopoNetSet(const char *name, bool spatial, int srid, bool has_z);
    TopoNetSet(const char *db_prefix, const char *name, bool spatial, int srid,
               bool has_z);
   ~TopoNetSet()
  {;
  }
  wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetName()
  {
    return Name;
  }
  bool IsSpatial()
  {
    return Spatial;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool GetHasZ()
  {
    return HasZ;
  }
};

class TopoNet
{
//
// TopoNet container
//
private:
  wxString DbPrefix;
  wxString Name;
  wxTreeItemId NetworkNode;
  TopoNet *Next;
public:
    TopoNet(class MyTableTree * tree, wxTreeItemId & root,
            wxString & network, bool spatial, int srid, bool has_z);
    TopoNet(class MyTableTree * tree, wxTreeItemId & root,
            wxString & dbPrefix, wxString & network, bool spatial, int srid,
            bool has_z);
   ~TopoNet()
  {;
  }
  wxTreeItemId *Check(wxString & table);
  TopoNet *GetNext()
  {
    return Next;
  }
  void SetNext(TopoNet * next)
  {
    Next = next;
  }
};

class TopoNetList
{
//
// TopoNet container
//
private:
  TopoNet * First;
  TopoNet *Last;
  int Count;
public:
    TopoNetList()
  {
    First = NULL;
    Last = NULL;
    Count = 0;
  }
   ~TopoNetList()
  {
    Flush();
  }
  void Flush();
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & network, bool spatial, int srid, bool has_z);
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & db_prefix, wxString & network, bool spatial, int srid,
           bool has_z);
  wxTreeItemId *FindNode(wxString & table);
  int GetCount()
  {
    return Count;
  }
};

class RasterCoverage
{
//
// Raster Coverage container
//
private:
  wxString DbPrefix;
  wxString Name;
  wxTreeItemId CoverageNode;
  RasterCoverage *Next;
public:
    RasterCoverage(class MyTableTree * tree, wxTreeItemId & root,
                   wxString & coverage, int srid);
    RasterCoverage(class MyTableTree * tree, wxTreeItemId & root,
                   wxString & dbPrefix, wxString & coverage, int srid);
   ~RasterCoverage()
  {;
  }
  wxTreeItemId *Check(wxString & table, bool *tile_data);
  RasterCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(RasterCoverage * next)
  {
    Next = next;
  }
};

class RasterCoverageList
{
//
// Raster Coverage container
//
private:
  RasterCoverage * First;
  RasterCoverage *Last;
  int Count;
public:
    RasterCoverageList()
  {
    First = NULL;
    Last = NULL;
    Count = 0;
  }
   ~RasterCoverageList()
  {
    Flush();
  }
  void Flush();
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & coverage, int srid);
  void Add(class MyTableTree * tree, wxTreeItemId & root, wxString & dbPrefix,
           wxString & coverage, int srid);
  wxTreeItemId *FindNode(wxString & table, bool *tile_data);
  int GetCount()
  {
    return Count;
  }
};

class VectorCoverage
{
//
// Vector Coverage container
//
private:
  wxString DbPrefix;
  wxString Prefix;
  wxString Name;
  wxTreeItemId CoverageNode;
  VectorCoverage *Next;
public:
    VectorCoverage(class MyTableTree * tree, wxTreeItemId & root,
                   wxString & prefix, wxString & coverage, int srid,
                   int geometry_type);
    VectorCoverage(class MyTableTree * tree, wxTreeItemId & root,
                   wxString & dbPrefix, wxString & prefix, wxString & coverage,
                   int srid, int geometry_type);
   ~VectorCoverage()
  {;
  }
  VectorCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(VectorCoverage * next)
  {
    Next = next;
  }
};

class VectorCoverageList
{
//
// Vector Coverage container
//
private:
  VectorCoverage * First;
  VectorCoverage *Last;
  int Count;
public:
    VectorCoverageList()
  {
    First = NULL;
    Last = NULL;
    Count = 0;
  }
   ~VectorCoverageList()
  {
    Flush();
  }
  void Flush();
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & prefix, wxString & coverage, int srid, int geometry_type);
  void Add(class MyTableTree * tree, wxTreeItemId & root, wxString & dbPrefix,
           wxString & prefix, wxString & coverage, int srid, int geometry_type);
  int GetCount()
  {
    return Count;
  }
};

class WmsLayer
{
//
// WMS Layers container
//
private:
  wxString DbPrefix;
  wxString LayerName;
  wxString URL;
  wxTreeItemId CoverageNode;
  WmsLayer *Next;
public:
    WmsLayer(class MyTableTree * tree, wxTreeItemId & root,
             wxString & layer_name, wxString & url);
    WmsLayer(class MyTableTree * tree, wxTreeItemId & root,
             wxString & dbPrefix, wxString & layer_name, wxString & url);
   ~WmsLayer()
  {;
  }
  WmsLayer *GetNext()
  {
    return Next;
  }
  void SetNext(WmsLayer * next)
  {
    Next = next;
  }
};

class WmsLayerList
{
//
// WMS Layers container
//
private:
  WmsLayer * First;
  WmsLayer *Last;
  int Count;
public:
    WmsLayerList()
  {
    First = NULL;
    Last = NULL;
    Count = 0;
  }
   ~WmsLayerList()
  {
    Flush();
  }
  void Flush();
  void Add(class MyTableTree * tree, wxTreeItemId & root,
           wxString & wms_layer_name, wxString & url);
  void Add(class MyTableTree * tree, wxTreeItemId & root, wxString & dbPrefix,
           wxString & wms_layer_name, wxString & url);
  int GetCount()
  {
    return Count;
  }
};

class IncompleteLayer
{
//
// an incomplete Layer identified by its Coverage Name
//
private:
  char *Type;
  char *CoverageName;
  IncompleteLayer *Next;
public:
    IncompleteLayer(const char *type, const char *name);
   ~IncompleteLayer()
  {
    if (Type != NULL)
      free(Type);
    if (CoverageName != NULL)
      free(CoverageName);
  }
  const char *GetType()
  {
    return Type;
  }
  const char *GetCoverageName()
  {
    return CoverageName;
  }
  void SetNext(IncompleteLayer * next)
  {
    Next = next;
  }
  IncompleteLayer *GetNext()
  {
    return Next;
  }
};

class IncompleteLayersList
{
//
// incomplete Layers container
//
private:
  IncompleteLayer * First;
  IncompleteLayer *Last;
public:
    IncompleteLayersList()
  {
    First = NULL;
    Last = NULL;
  }
   ~IncompleteLayersList();
  void Add(const char *type, const char *coverage_name);
  IncompleteLayer *GetFirst()
  {
    return First;
  }
};

class RootNodes
{
//
// a class wrapping root nodes for an Attached DB
//
private:
  wxString dbAlias;
  wxTreeItemId rootUserData;
  wxTreeItemId rootTopologies;
  wxTreeItemId rootNetworks;
  wxTreeItemId rootRasterCoverages;
  wxTreeItemId rootVectorCoverages;
  wxTreeItemId rootStyling;
  wxTreeItemId rootWMS;
  wxTreeItemId rootIsoMetadata;
  wxTreeItemId rootMetadata;
  wxTreeItemId rootInternal;
  wxTreeItemId rootSpatialIndex;
public:
    RootNodes(wxString & alias, wxTreeItemId userData, wxTreeItemId topologies,
              wxTreeItemId networks, wxTreeItemId raster_coverages,
              wxTreeItemId vector_coverages, wxTreeItemId styling,
              wxTreeItemId wms, wxTreeItemId isoMetadata, wxTreeItemId metadata,
              wxTreeItemId internal, wxTreeItemId spatialIndex)
  {
    dbAlias = alias;
    rootUserData = userData;
    rootTopologies = topologies;
    rootNetworks = networks;
    rootRasterCoverages = raster_coverages;
    rootVectorCoverages = vector_coverages;
    rootStyling = styling;
    rootWMS = wms;
    rootIsoMetadata = isoMetadata;
    rootMetadata = metadata;
    rootInternal = internal;
    rootSpatialIndex = spatialIndex;
  }
   ~RootNodes()
  {;
  }
  wxString & GetDbAlias()
  {
    return dbAlias;
  }
  wxTreeItemId & GetRootUserData()
  {
    return rootUserData;
  }
  wxTreeItemId & GetRootTopologies()
  {
    return rootTopologies;
  }
  wxTreeItemId & GetRootNetworks()
  {
    return rootNetworks;
  }
  wxTreeItemId & GetRootRasterCoverages()
  {
    return rootRasterCoverages;
  }
  wxTreeItemId & GetRootVectorCoverages()
  {
    return rootVectorCoverages;
  }
  wxTreeItemId & GetRootStyling()
  {
    return rootStyling;
  }
  wxTreeItemId & GetRootWMS()
  {
    return rootWMS;
  }
  wxTreeItemId & GetRootIsoMetadata()
  {
    return rootIsoMetadata;
  }
  wxTreeItemId & GetRootMetadata()
  {
    return rootMetadata;
  }
  wxTreeItemId & GetRootInternal()
  {
    return rootInternal;
  }
  wxTreeItemId & GetRootSpatialIndex()
  {
    return rootSpatialIndex;
  }
};

class MyTableTree:public wxTreeCtrl
{
//
// a tree-control used for SQLite DB tables
//
private:
  MyFrame * MainFrame;
  bool IsGeoPackage;
  bool IsFdoOgr;
  wxTreeItemId Root;            // the root node
  wxTreeItemId RootUserData;
  wxTreeItemId RootTopologies;
  wxTreeItemId RootNetworks;
  wxTreeItemId RootRasterCoverages;
  wxTreeItemId RootVectorCoverages;
  wxTreeItemId RootStyling;
  wxTreeItemId RootWMS;
  wxTreeItemId RootIsoMetadata;
  wxTreeItemId RootPostgreSQL;
  TopoGeoList Topologies;
  TopoNetList Networks;
  RasterCoverageList RasterCoverages;
  VectorCoverageList VectorCoverages;
  WmsLayerList WmsLayers;
  TopoGeoList AltTopologies;
  TopoNetList AltNetworks;
  RasterCoverageList AltRasterCoverages;
  VectorCoverageList AltVectorCoverages;
  WmsLayerList AltWmsLayers;
  wxTreeItemId RootMetadata;
  wxTreeItemId RootInternal;
  wxTreeItemId RootSpatialIndex;
  wxImageList *Images;          // the images list
  wxTreeItemId CurrentItem;     // the tree item holding the current context menu
  wxString CurrentRasterCoverageName;
  wxString CurrentVectorCoverageName;
  wxString CurrentWmsLayerURL;
  wxString CurrentWmsLayerName;
  void ExpandTable(wxTreeItemId & item);
  void ExpandView(wxTreeItemId & item);
  void ExpandAttachedTable(wxTreeItemId & item);
  void ExpandAttachedView(wxTreeItemId & item);
  void ExpandPostgresTable(wxTreeItemId & item);
  void ExpandPostGisView(wxTreeItemId & item);
// context menus
  void DoRootRasterCoveragesContextMenu(wxPoint & pt);
  void DoRootVectorCoveragesContextMenu(wxPoint & pt);
  void DoRootWmsLayersContextMenu(wxPoint & pt);
  void DoRootTopoGeoContextMenu(wxPoint & pt);
  void DoRootTopoNetContextMenu(wxPoint & pt);
  void DoRootStylingContextMenu(wxPoint & pt);
  void DoRootPostgreSqlContextMenu(wxPoint & pt);
  void DoRootOthersContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedDbContextMenu(wxPoint & pt, MyObject * obj);
  void DoRootAttachedContextMenu(wxPoint & pt, MyObject * obj);
  void DoGenericAttachedContextMenu(wxPoint & pt, wxString & title);
  void DoTopoGeoContextMenu(wxPoint & pt, wxString & name);
  void DoTopoNetContextMenu(wxPoint & pt, wxString & name);
  void DoRasterCoverageContextMenu(wxPoint & pt, wxString & name);
  void DoVectorCoverageContextMenu(wxPoint & pt, wxString & name);
  void DoWmsLayerContextMenu(wxPoint & pt, wxString & name);
  void DoMainTableContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoMainViewContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoAttachedTableContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoAttachedViewContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoMainColumnContextMenu(wxPoint & pt, MyObject * obj, int icon,
                               bool metadata);
  void DoMainGpkgColumnContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoMainFdoOgrColumnContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoMainLimitedColumnContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoAttachedColumnContextMenu(wxPoint & pt, MyObject * obj, int icon);
  void DoMainGeometryColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainLimitedGeometryColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedGeometryColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainIndexContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedIndexContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainIndexColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedIndexColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainTriggerContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedTriggerContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainPrimaryKeyContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedPrimaryKeyContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainPrimaryKeyColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedPrimaryKeyColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainForeignKeyContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedForeignKeyContextMenu(wxPoint & pt, MyObject * obj);
  void DoMainForeignKeyColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoAttachedForeignKeyColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoPostgreSqlContextMenu(wxPoint & pt, MyObject * obj);
  void DoPostgresConnContextMenu(wxPoint & pt);
  void DoPostgresSchemaContextMenu(wxPoint & pt);
  void DoPostgresTableContextMenu(wxPoint & pt, MyObject * obj);
  void DoPostgresViewContextMenu(wxPoint & pt, MyObject * obj);
  void DoPostGisViewContextMenu(wxPoint & pt, MyObject * obj);
  void DoPostgresColumnContextMenu(wxPoint & pt, MyObject * obj);
  void DoPostGisGeometryContextMenu(wxPoint & pt, MyObject * obj);
  bool IsAlreadyDefinedTopologyCoverage(wxString & name);
  bool IsAlreadyDefinedNetworkCoverage(wxString & name);
public:
    MyTableTree()
  {;
  }
  MyTableTree(MyFrame * parent, wxWindowID id = wxID_ANY);
  virtual ~ MyTableTree();
  void SetPath(wxString & path)
  {
    SetItemText(Root, path);
  }
  bool IsGeoPackageMode()
  {
    return IsGeoPackage;
  }
  void SetGeoPackageMode(bool mode)
  {
    IsGeoPackage = mode;
  }
  bool IsFdoOgrMode()
  {
    return IsFdoOgr;
  }
  void SetFdoOgrMode(bool mode)
  {
    IsFdoOgr = mode;
  }
  void FlushAll();
  wxTreeItemId & GetRootNode(wxString & tableName, bool *restricted,
                             bool *topoObj, bool *tile_data);
  wxTreeItemId & GetAltRootNode(wxString & tableName, RootNodes * nodes,
                                bool *topoObj, bool *tile_data);
  void AddTable(wxString & tableName, bool virtualTable, bool geometry);
  void AddTable(wxString & dbAlias, wxString & tableName, bool virtualTable,
                bool geometry);
  void AddTmpMetadata(wxString & tableName);
  void AddGeoPackageTable(wxString & tableName);
  void AddGeoPackageTable(wxString & dbAlias, wxString & tableName,
                          RootNodes * nodes);
  void AddGeoPackageVirtualTable(wxString & tableName);
  void AddGeoPackageVirtualTable(wxString & dbAlias, wxString & tableName,
                                 RootNodes * nodes);
  void AddFdoOgrTable(wxString & tableName);
  void AddFdoOgrTable(wxString & dbAlias, wxString & tableName,
                      RootNodes * nodes);
  void AddFdoOgrVirtualTable(wxString & tableName);
  void AddFdoOgrVirtualTable(wxString & dbAlias, wxString & tableName,
                             RootNodes * nodes);
  wxTreeItemId & AddAttached(wxString & dbAlias, wxString & path);
  void AddTable(wxString & dbAlias, wxString & tableName,
                bool virtualTable, bool geometry, RootNodes * list);
  void AddView(wxString & viewName, bool geometry);
  void AddView(wxString & dbAlias, wxString & viewName, bool geometry);
  void AddView(wxString & dbAlias, wxString & viewName, bool geometry,
               RootNodes * list);
  void AddPostgresTable(class MyPostgres * list, wxString & virtName);
  void ExpandRoot()
  {
    Expand(Root);
    Expand(RootUserData);
    CollapseAllChildren(RootTopologies);
    CollapseAllChildren(RootNetworks);
    CollapseAllChildren(RootRasterCoverages);
    CollapseAllChildren(RootVectorCoverages);
    CollapseAllChildren(RootStyling);
    CollapseAllChildren(RootWMS);
    CollapseAllChildren(RootPostgreSQL);
    CollapseAllChildren(RootIsoMetadata);
    Collapse(RootMetadata);
    Collapse(RootInternal);
    Collapse(RootSpatialIndex);
  }
  void AddTopology(TopoGeoSet * topology)
  {
    Topologies.Add(this, RootTopologies,
                   topology->GetName(), topology->GetSRID(),
                   topology->GetHasZ());
  }
  void AddAltTopology(wxTreeItemId & rootTopologies, wxString & dbAlias,
                      TopoGeoSet * topology)
  {
    AltTopologies.Add(this, rootTopologies, dbAlias,
                      topology->GetName(), topology->GetSRID(),
                      topology->GetHasZ());
  }
  void DeleteTopologies(wxTreeItemId & root_topologies);
  void DeleteAltTopologies(void)
  {
    AltTopologies.Flush();
  }
  void AddNetwork(TopoNetSet * network)
  {
    Networks.Add(this, RootNetworks, network->GetName(), network->IsSpatial(),
                 network->GetSRID(), network->GetHasZ());
  }
  void AddAltNetwork(wxTreeItemId & rootNetworks, wxString & dbAlias,
                     TopoNetSet * network)
  {
    AltNetworks.Add(this, rootNetworks, dbAlias, network->GetName(),
                    network->IsSpatial(), network->GetSRID(),
                    network->GetHasZ());
  }
  void DeleteNetworks(wxTreeItemId & root_networks);
  void DeleteAltNetworks(void)
  {
    AltNetworks.Flush();
  }
  void AddRasterCoverage(RasterCoverageSet * coverage)
  {
    RasterCoverages.Add(this, RootRasterCoverages, coverage->GetName(),
                        coverage->GetSRID());
  }
  void AddAltRasterCoverage(wxTreeItemId & rootRasterCoverages,
                            wxString & dbAlias, RasterCoverageSet * coverage)
  {
    AltRasterCoverages.Add(this, rootRasterCoverages, dbAlias,
                           coverage->GetName(), coverage->GetSRID());
  }
  void AddVectorCoverage(VectorCoverageSet * coverage)
  {
    VectorCoverages.Add(this, RootVectorCoverages, coverage->GetPrefix(),
                        coverage->GetName(), coverage->GetSRID(),
                        coverage->GetGeometryType());
  }
  void AddAltVectorCoverage(wxTreeItemId & rootVectorCoverages,
                            wxString & dbAlias, VectorCoverageSet * coverage)
  {
    AltVectorCoverages.Add(this, rootVectorCoverages, dbAlias,
                           coverage->GetPrefix(), coverage->GetName(),
                           coverage->GetSRID(), coverage->GetGeometryType());
  }
  void AddWmsLayer(wxString & name, wxString & url)
  {
    WmsLayers.Add(this, RootWMS, name, url);
  }
  void AddWmsLayer(wxTreeItemId & rootWMS,
                   wxString & dbAlias, wxString & name, wxString & url)
  {
    AltWmsLayers.Add(this, rootWMS, dbAlias, name, url);
  }
  bool GetCurrentlySelectedTable(wxString & table_name);
  void DeleteRasterCoverages(wxTreeItemId & root_coverages);
  void DeleteVectorCoverages(wxTreeItemId & root_coverages);
  void DeleteAltRasterCoverages(void)
  {
    AltRasterCoverages.Flush();
  }
  void DeleteAltVectorCoverages(void)
  {
    AltVectorCoverages.Flush();
  }
  void AddPostgresConnection(int num, class MyPostgresConn * conn,
                             wxTreeItemId & itemId);
  void AddPostgresSchema(wxTreeItemId & parent, MyPostgresConn * conn,
                         wxString & schema, wxTreeItemId & itemId);
  void AddPostgresTable(wxTreeItemId & parent, MyPostgresConn * conn,
                        wxString & schema, wxString & table,
                        wxString & virtName);
  void AddPostgresView(wxTreeItemId & parent, MyPostgresConn * conn,
                       wxString & schema, wxString & view, wxString & virtName);
  void AddPostGisView(wxTreeItemId & parent, MyPostgresConn * conn,
                      wxString & schema, wxString & table, wxString & geometry,
                      wxString & virtName);
  void DoVerifyMapConfig(const unsigned char *xml, wxString & report);
  bool DoVerifyLayer(int type, const char *prefix, const char *name);
  bool DoVerifyRasterInternalStyle(const char *prefix, const char *style);
  bool DoVerifyVectorInternalStyle(const char *prefix, const char *style);
  sqlite3 *GetSQLiteHandle();
  void OnSelChanged(wxTreeEvent & event);
  void OnRightClick(wxTreeEvent & event);
  void OnCmdQueryViewComposer(wxCommandEvent & event);
  void OnCmdCloneTable(wxCommandEvent & event);
  void OnCmdNewTable(wxCommandEvent & event);
  void OnCmdNewView(wxCommandEvent & event);
  void OnCmdNewIndex(wxCommandEvent & event);
  void OnCmdNewTrigger(wxCommandEvent & event);
  void OnCmdNewColumn(wxCommandEvent & event);
  void OnCmdNewRasterStyle(wxCommandEvent & event);
  void OnCmdReloadRasterStyle(wxCommandEvent & event);
  void OnCmdUnregisterRasterStyle(wxCommandEvent & event);
  void OnCmdRegisterExternalGraphic(wxCommandEvent & event);
  void OnCmdUnregisterExternalGraphic(wxCommandEvent & event);
  void OnCmdRegisterTextFont(wxCommandEvent & event);
  void OnCmdUnregisterTextFont(wxCommandEvent & event);
  void OnCmdNewVectorStyle(wxCommandEvent & event);
  void OnCmdReloadVectorStyle(wxCommandEvent & event);
  void OnCmdUnregisterVectorStyle(wxCommandEvent & event);
  void OnCmdNewMapConfig(wxCommandEvent & event);
  void OnCmdReloadMapConfig(wxCommandEvent & event);
  void OnCmdUnregisterMapConfig(wxCommandEvent & event);
  void OnCmdVerifyExternalMapConfig(wxCommandEvent & event);
  void OnCmdVerifyRegisteredMapConfig(wxCommandEvent & event);
  void OnCmdRasterSymbolizerContrast(wxCommandEvent & event);
  void OnCmdRasterSymbolizerChannelRgb(wxCommandEvent & event);
  void OnCmdRasterSymbolizerChannelGray(wxCommandEvent & event);
  void OnCmdRasterSymbolizerCategorize(wxCommandEvent & event);
  void OnCmdRasterSymbolizerInterpolate(wxCommandEvent & event);
  void OnCmdRasterSymbolizerShadedRelief(wxCommandEvent & event);
  void OnCmdRasterSymbolizerMonochrome(wxCommandEvent & event);
  void OnCmdSimpleLineSymbolizer(wxCommandEvent & event);
  void OnCmdSimplePolygonSymbolizer(wxCommandEvent & event);
  void OnCmdSimplePointSymbolizer(wxCommandEvent & event);
  void OnCmdSimpleTextSymbolizer(wxCommandEvent & event);
  void OnCmdImportRaster(wxCommandEvent & event);
  void OnCmdPyramidize(wxCommandEvent & event);
  void OnCmdPyramidizeMonolithic(wxCommandEvent & event);
  void OnCmdDePyramidize(wxCommandEvent & event);
  void OnCmdRasterDrop(wxCommandEvent & event);
  void OnCmdRasterInfos(wxCommandEvent & event);
  void OnCmdRasterSRIDs(wxCommandEvent & event);
  void OnCmdRasterKeywords(wxCommandEvent & event);
  void OnCmdAddAllRastersSrid(wxCommandEvent & event);
  void OnCmdUpdateRasterExtent(wxCommandEvent & event);
  void OnCmdUpdateRasterExtentAll(wxCommandEvent & event);
  void OnCmdVectorUnregister(wxCommandEvent & event);
  void OnCmdVectorInfos(wxCommandEvent & event);
  void OnCmdVectorSRIDs(wxCommandEvent & event);
  void OnCmdVectorKeywords(wxCommandEvent & event);
  void OnCmdUpdateVectorExtent(wxCommandEvent & event);
  void OnCmdAddAllVectorsSrid(wxCommandEvent & event);
  void OnCmdUpdateVectorExtentAll(wxCommandEvent & event);
  void OnCmdRegisterWmsLayer(wxCommandEvent & event);
  void OnCmdWmsLayerUnregister(wxCommandEvent & event);
  void OnCmdWmsLayerInfos(wxCommandEvent & event);
  void OnCmdWmsLayerConfigure(wxCommandEvent & event);
  void OnCmdCreateTopoGeo(wxCommandEvent & event);
  void OnCmdDropTopoGeo(wxCommandEvent & event);
  void OnCmdCreateTopoNet(wxCommandEvent & event);
  void OnCmdDropTopoNet(wxCommandEvent & event);
  void OnCmdShow(wxCommandEvent & event);
  void OnCmdDrop(wxCommandEvent & event);
  void OnCmdRename(wxCommandEvent & event);
  void OnCmdSelect(wxCommandEvent & event);
  void OnCmdSelectTiles(wxCommandEvent & event);
  void OnCmdRefresh(wxCommandEvent & event);
  void OnRefreshDeferred(wxCommandEvent & event);
  void OnCmdRecover(wxCommandEvent & event);
  void OnCmdShowSql(wxCommandEvent & event);
  void OnCmdSpatialIndex(wxCommandEvent & event);
  void OnCmdCheckSpatialIndex(wxCommandEvent & event);
  void OnCmdRecoverSpatialIndex(wxCommandEvent & event);
  void OnCmdMbrCache(wxCommandEvent & event);
  void OnCmdRebuildTriggers(wxCommandEvent & event);
  void OnCmdCheckGeometry(wxCommandEvent & event);
  void OnCmdExtent(wxCommandEvent & event);
  void OnCmdUpdateLayerStatistics(wxCommandEvent & event);
  void OnCmdUpdateLayerStatisticsAll(wxCommandEvent & event);
  void OnCmdElementaryGeometries(wxCommandEvent & event);
  void OnCmdMalformedGeometries(wxCommandEvent & event);
  void OnCmdRepairPolygons(wxCommandEvent & event);
  void OnCmdSetSrid(wxCommandEvent & event);
  void OnCmdDumpShp(wxCommandEvent & event);
  void OnCmdDumpGeoJSON(wxCommandEvent & event);
  void OnCmdDumpKml(wxCommandEvent & event);
  void OnCmdDumpTxtTab(wxCommandEvent & event);
  void OnCmdDumpCsv(wxCommandEvent & event);
  void OnCmdDumpHtml(wxCommandEvent & event);
  void OnCmdDumpDif(wxCommandEvent & event);
  void OnCmdDumpSylk(wxCommandEvent & event);
  void OnCmdDumpDbf(wxCommandEvent & event);
  void OnCmdDumpXlsx(wxCommandEvent & event);
  void OnCmdDumpPostGIS(wxCommandEvent & event);
  void OnCmdEdit(wxCommandEvent & event);
  void OnCmdCreatePostgreSqlConn(wxCommandEvent & event);
  void OnCmdCloseAllPostgreSqlConns(wxCommandEvent & event);
  void OnCmdClosePostgreSqlConn(wxCommandEvent & event);
  void OnCmdPostgreSqlDropOrphans(wxCommandEvent & event);
  void OnCmdPostgreSqlInfos(wxCommandEvent & event);
  bool DropRenameAux1(MyObject * obj, class GeomColsList * geometries,
                      bool *autoincrement);
  void DropRenameAux2(MyObject * obj, GeomColsList * geometries,
                      wxString & aliasTable, wxString & new_column,
                      wxString & renameSql, wxString & dropSql,
                      wxString & disableSpatialIdxSql,
                      wxString & dropSpatialIdxSql,
                      wxString & createSpatialIdxSql,
                      wxString & discardGeometrySql);
  void DropRenameAux3(MyObject * obj, wxString & new_column,
                      GeomColsList * geometries, class TblIndexList * index,
                      wxString & addGeometrySql);
  void OnCmdDropColumn(wxCommandEvent & event);
  void OnCmdRenameColumn(wxCommandEvent & event);
  void OnCmdColumnStats(wxCommandEvent & event);
  void OnCmdMapPreview(wxCommandEvent & event);
  void OnCmdCreateVectorCoverage(wxCommandEvent & event);
  void OnCmdCreateTopologyCoverage(wxCommandEvent & event);
  void OnCmdCreateNetworkCoverage(wxCommandEvent & event);
  void OnCmdCheckDuplicates(wxCommandEvent & event);
  void OnCmdRemoveDuplicates(wxCommandEvent & event);
  void OnCmdDetachDB(wxCommandEvent & event);
  void OnCmdCheckGeometries(wxCommandEvent & event);
  void OnCmdSanitizeGeometries(wxCommandEvent & event);
  void OnCmdSldSeRasterStyles(wxCommandEvent & event);
  void OnCmdSldSeVectorStyles(wxCommandEvent & event);
  void OnCreateRasterCoverage(wxCommandEvent & event);
  void OnRegisterVectorCoverage(wxCommandEvent & event);
  void OnRegisterSpatialViewCoverage(wxCommandEvent & event);
  void OnRegisterVirtualTableCoverage(wxCommandEvent & event);
  void OnRegisterTopoGeoCoverage(wxCommandEvent & event);
  void OnRegisterTopoNetCoverage(wxCommandEvent & event);
  void OnItemCollapsed(wxTreeEvent & event);
  void OnItemExpanding(wxTreeEvent & event);
};

class SqlThreadParams
{
//
// an auxiliary class used for SQL threaded queries
//
private:
  class MyResultSetView * Mother;
  wxString Sql;
  sqlite3_stmt *Stmt;
  int FromRow;
  int EndRow;
  int MaxRow;
  MyVariantList List;
  sqlite3 *Sqlite;
  clock_t Start;
  bool Error;
  int FetchedRows;
  int StatFullscanStep;
  int StatSort;
  int StatAutoindex;
  double ElapsedTime;
  bool AbortRequested;
  bool Valid;
public:
    SqlThreadParams()
  {
    Reset();
  }
   ~SqlThreadParams()
  {;
  }
  void Initialize(MyResultSetView * mother, wxString & sql, sqlite3_stmt * stmt,
                  int from, sqlite3 * sqlite, clock_t start);
  void Reset(void);
  bool IsValid()
  {
    return Valid;
  }
  MyResultSetView *GetMother()
  {
    return Mother;
  }
  wxString & GetSql()
  {
    return Sql;
  }
  sqlite3_stmt *GetStmt()
  {
    return Stmt;
  }
  int GetFromRow()
  {
    return FromRow;
  }
  void SetEndRow(int end)
  {
    EndRow = end;
  }
  int GetEndRow()
  {
    return EndRow;
  }
  void SetMaxRow(int max)
  {
    MaxRow = max;
  }
  int GetMaxRow()
  {
    return MaxRow;
  }
  MyVariantList *GetList()
  {
    return &List;
  }
  sqlite3 *GetSqlite()
  {
    return Sqlite;
  }
  clock_t GetStart()
  {
    return Start;
  }
  void FetchedRow()
  {
    FetchedRows++;
  }
  void UpdateStats(int fullscan, int sort, int autoindex, clock_t now);
  int GetFetchedRows()
  {
    return FetchedRows;
  }
  int GetStatFullscanStep()
  {
    return StatFullscanStep;
  }
  int GetStatSort()
  {
    return StatSort;
  }
  int GetStatAutoindex()
  {
    return StatAutoindex;
  }
  double GetElapsedTime()
  {
    return ElapsedTime;
  }
  void SetError()
  {
    Error = true;
    Valid = false;
  }
  bool IsError()
  {
    return Error;
  }
  void Abort()
  {
    AbortRequested = true;
  }
  bool IsAbortRequested()
  {
    return AbortRequested;
  }
  void Finalize()
  {
    sqlite3_finalize(Stmt);
    Stmt = NULL;
  }
};

class MyResultSetView:public wxPanel
{
//
// a panel to be used for SQL Queries
//
private:
  wxTimer * ProgressTimer;
  MyFrame *MainFrame;
  wxBitmapButton *BtnRsFirst;
  wxBitmapButton *BtnRsLast;
  wxBitmapButton *BtnRsNext;
  wxBitmapButton *BtnRsPrevious;
  wxBitmapButton *BtnRefresh;
  wxBitmapButton *BtnRsMapShow;
  wxBitmapButton *BtnRsMapZoom;
  wxStaticText *RsCurrentBlock;
  int RsBlock;
  int RsBeginRow;
  int RsEndRow;
  int RsMaxRow;
  bool IsMaxAlreadySet;
  int CurrentEvtRow;
  int CurrentEvtColumn;
  int CurrentTileId;
  wxString TileDataDbPrefix;
  wxString TileDataTable;
  MyVariant *CurrentBlob;
  wxGrid *TableView;
  MyBlobs *TableBlobs;
  MyValues *TableValues;
  bool ReadOnly;
  bool CoverageTiles;
  sqlite3_int64 *RowIds;
  int PrimaryKeys[1024];
  int BlobColumns[1024];
  wxString DbPrefix;
  wxString TableName;
  bool InsertPending;
  MyRowVariant *InsertRow;
  wxString SqlErrorMsg;
  SqlThreadParams ThreadParams;
  void XmlBlobOut(bool indented);
  const char *CleanSqlTail(const char *dirty);
  char *ConsumeSqlComment(char *sql, bool mode);
  char *ConsumeSqlEmptyLine(char *sql);
  void UpdateMaxRow(wxString & sql);
public:
    MyResultSetView()
  {;
  }
  MyResultSetView(MyFrame * parent, wxWindowID id = wxID_ANY);
  virtual ~ MyResultSetView();
  void HideControls();
  void ShowControls();
  wxString & GetSqlErrorMsg()
  {
    return SqlErrorMsg;
  }
  bool IsReadOnly()
  {
    return ReadOnly;
  }
  bool IsPrimaryKey(int column);
  bool IsBlobColumn(int column);
  void EditTable(wxString & sql, int *primaryKeys, int *blobCols,
                 wxString & tableName);
  wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetTableName()
  {
    return TableName;
  }
  int FindGeomColumnIndex(wxString & geometry_column);
  void CreateGrid(int rows, int cols);
  void CreateStatsGrid();
  void ResetEmpty();
  bool ExecuteSqlPre(wxString & sql, int from, bool read_only, bool coverage,
                     wxString & tile_data_db_prefix, wxString & tile_data_table,
                     bool reset);
  bool ExecuteSqlPost(void);
  void AbortRequested(void);
  wxStaticText *GetCurrentBlock()
  {
    return RsCurrentBlock;
  }
  static void FormatElapsedTime(double seconds, char *elapsed, bool simple =
                                false);
  int GetRsBlock()
  {
    return RsBlock;
  }
  void SetSqlErrorMsg(wxString & msg)
  {
    SqlErrorMsg = msg;
  }
  gaiaGeomCollPtr DoTransformMapGeometry(unsigned char *blob, int blob_size,
                                         int srid);
  void ResizeView(void);
  void DoInsert(bool confirmed);
  void HexBlobValue(unsigned char *blob, int size, wxString & hex);
  void OnSize(wxSizeEvent & event);
  void OnRsFirst(wxCommandEvent & event);
  void OnMapFeatureSelect(wxCommandEvent & event);
  void OnRsLast(wxCommandEvent & event);
  void OnRsNext(wxCommandEvent & event);
  void OnRsPrevious(wxCommandEvent & event);
  void OnRefresh(wxCommandEvent & event);
  void OnRsMapShow(wxCommandEvent & event);
  void OnRsMapZoom(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
  void OnTimerStatistics(wxTimerEvent & event);
  void OnCellSelected(wxGridEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCellChanged(wxGridEvent & event);
  void OnCmdDelete(wxCommandEvent & event);
  void OnCmdInsert(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnCmdClearSelection(wxCommandEvent & event);
  void OnCmdSelectAll(wxCommandEvent & event);
  void OnCmdSelectRow(wxCommandEvent & event);
  void OnCmdSelectColumn(wxCommandEvent & event);
  void OnCmdCopy(wxCommandEvent & event);
  void OnCmdBlob(wxCommandEvent & event);
  void OnCmdBlobIn(wxCommandEvent & event);
  void OnCmdBlobOut(wxCommandEvent & event);
  void OnCmdBlobNull(wxCommandEvent & event);
  void OnCmdXmlBlobIn(wxCommandEvent & event);
  void OnCmdXmlBlobOut(wxCommandEvent & event);
  void OnCmdXmlBlobOutIndented(wxCommandEvent & event);
  void OnCmdExpTxtTab(wxCommandEvent & event);
  void OnCmdExpCsv(wxCommandEvent & event);
  void OnCmdExpHtml(wxCommandEvent & event);
  void OnCmdExpShp(wxCommandEvent & event);
  void OnCmdExpDif(wxCommandEvent & event);
  void OnCmdExpSylk(wxCommandEvent & event);
  void OnCmdExpDbf(wxCommandEvent & event);
  void OnCmdExpXlsx(wxCommandEvent & event);
  void OnCmdTilePreview(wxCommandEvent & event);
  void OnCmdFilter(wxCommandEvent & event);
};

class MySqlControl:public wxTextCtrl
{
//
// the SQL text control
//
private:
  class MyQueryView * Parent;
public:
  MySqlControl(MyQueryView * parent, wxWindowID id, const wxString & value,
               const wxPoint & pos, const wxSize & size, long style);
    virtual ~ MySqlControl()
  {;
  }
  void OnSqlMousePosition(wxMouseEvent & event);
  void OnSqlArrowPosition(wxKeyEvent & event);
};

class SqlTokenizer
{
//
// a class used for tokenizing SQL statements
//
private:
  wxString ** TokenList;
  int Block;
  int Max;
  int Index;
  void Expand();
  void Insert(wxString * token);
  wxString CurrentToken;
public:
    SqlTokenizer(wxString & sql);
   ~SqlTokenizer();
  bool HasMoreTokens();
    wxString & GetNextToken();
};

class MyQueryView:public wxPanel
{
//
// a panel to be used for SQL Queries
//
private:
  MyFrame * MainFrame;
  MySqlHistory History;
  MySqlControl *SqlCtrl;
  wxBitmapButton *BtnSqlGo;
  wxBitmapButton *BtnSqlFilter;
  wxBitmapButton *BtnSqlErase;
  wxBitmapButton *BtnSqlAbort;
  wxBitmapButton *BtnHistoryBack;
  wxBitmapButton *BtnHistoryForward;
  int BracketStart;
  int BracketEnd;
  bool IgnoreEvent;
public:
    MyQueryView()
  {;
  }
  MyQueryView(MyFrame * parent, wxWindowID id = wxID_ANY);
  virtual ~ MyQueryView()
  {;
  }
  void HideControls();
  void ShowControls();
  static bool IsSqliteExtra(wxString & str);
  static bool IsSqlString(wxString & str);
  static bool IsSqlNumber(wxString & str);
  static bool IsSqlFunction(wxString & str, char next_c);
  static bool IsSqlGeoFunction(wxString & str, char next_c);
  static bool IsSqlRasterFunction(wxString & str, char next_c);
  bool IsIgnoreEvent()
  {
    return IgnoreEvent;
  }
  void EventBrackets();
  bool CheckBrackets(int pos, bool reverse_direction, int *on, int *off);
  void EvidBrackets(int on, int off);
  void DoSqlSyntaxColor();
  void EnableAbortButton()
  {
    BtnSqlAbort->Enable(true);
  }
  void DisableAbortButton()
  {
    BtnSqlAbort->Enable(false);
  }
  void EnableFilterButton()
  {
    BtnSqlFilter->Enable(true);
  }
  void DisableFilterButton()
  {
    BtnSqlFilter->Enable(false);
  }
  wxTextCtrl *GetSqlCtrl()
  {
    return SqlCtrl;
  }
  MySqlHistory *GetHistory()
  {
    return &History;
  }
  void SetSql(wxString & sql, bool execute);
  void SetSql(wxString & sql, bool execute, bool coverage,
              wxString & tile_data_db_prefix, wxString & tile_data_table,
              bool reset);
  void SetHistoryStates();
  void OnSize(wxSizeEvent & event);
  void OnSqlGo(wxCommandEvent & event);
  void OnSqlFilter(wxCommandEvent & event);
  void OnSqlErase(wxCommandEvent & event);
  void OnSqlAbort(wxCommandEvent & event);
  void OnHistoryBack(wxCommandEvent & event);
  void OnHistoryForward(wxCommandEvent & event);
  void OnSqlSyntaxColor(wxCommandEvent & event);
  void AddToHistory(wxString & sql);
};

class MalformedGeom
{
//
// a malformed geometry item
//
private:
  sqlite3_int64 RowId;
  int Severity;
  wxString Error;
  wxString GeosMsg;
  bool CanFix;
  MalformedGeom *Next;
public:
    MalformedGeom(sqlite3_int64 rowid, int severity, bool canFix,
                  wxString & error);
    MalformedGeom(sqlite3_int64 rowid, int severity, bool canFix,
                  wxString & error, wxString & geosMsg);
   ~MalformedGeom()
  {;
  }
  sqlite3_int64 GetRowId()
  {
    return RowId;
  }
  int GetSeverity()
  {
    return Severity;
  }
  bool CanBeFixed()
  {
    return CanFix;
  }
  wxString & GetError()
  {
    return Error;
  }
  wxString & GetGeosMsg()
  {
    return GeosMsg;
  }
  void SetNext(MalformedGeom * next)
  {
    Next = next;
  }
  MalformedGeom *GetNext()
  {
    return Next;
  }
};

class MalformedGeomsList
{
//
// a list of malformed geometries
//
private:
  MalformedGeom * First;
  MalformedGeom *Last;
public:
    MalformedGeomsList()
  {
    First = NULL;
    Last = NULL;
  }
   ~MalformedGeomsList();
  void AddEntity(sqlite3_int64 rowid, int severity, bool canFix,
                 wxString & error);
  void AddEntity(sqlite3_int64 rowid, int severity, bool CanFix,
                 wxString & error, wxString & geosMsg);
  MalformedGeom *GetFirst()
  {
    return First;
  }
};

class PostgresConnectionDialog:public wxDialog
{
// 
// a dialog for connecting to a PosgreSQL DBMS
//
private:
  MyFrame * MainFrame;
  char *host;
  char *hostaddr;
  unsigned int port;
  char *dbname;
  char *user;
  char *password;
  bool ReadOnly;
  bool TextDates;
public:
    PostgresConnectionDialog()
  {
    host = NULL;
    hostaddr = NULL;
    port = 5432;
    dbname = NULL;
    user = NULL;
    password = NULL;
    ReadOnly = true;
    TextDates = true;
  }
  virtual ~ PostgresConnectionDialog()
  {
    if (host != NULL)
      free(host);
    if (hostaddr != NULL)
      free(hostaddr);
    if (dbname != NULL)
      free(dbname);
    if (user != NULL)
      free(user);
    if (password != NULL)
      free(password);
  }
  bool Create(MyFrame * parent);
  void CreateControls();
  const char *GetHost()
  {
    return host;
  }
  const char *GetHostAddr()
  {
    return hostaddr;
  }
  int GetPort()
  {
    return port;
  }
  const char *GetDbName()
  {
    return dbname;
  }
  const char *GetUser()
  {
    return user;
  }
  const char *GetPassword()
  {
    return password;
  }
  bool IsReadOnly()
  {
    return ReadOnly;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  void OnOk(wxCommandEvent & event);
};

class SanitizeAllGeometriesDialog:public wxDialog
{
//
// a dialog supporting Sanitize All Geometries
//
private:
  MyFrame * MainFrame;
  wxString TmpPrefix;
public:
    SanitizeAllGeometriesDialog()
  {;
  }
  virtual ~ SanitizeAllGeometriesDialog()
  {;
  }
  bool Create(MyFrame * parent);
  void CreateControls();
  wxString & GetTmpPrefix()
  {
    return TmpPrefix;
  }
  void OnYes(wxCommandEvent & event);
  void OnNo(wxCommandEvent & event);
};

class CreateTopoGeoDialog:public wxDialog
{
//
// a Dialog for creating a new Topology-Geometry
//
private:
  MyFrame * MainFrame;
  wxString TopologyName;
  int SRID;
  bool HasZ;
  double Tolerance;
public:
    CreateTopoGeoDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ CreateTopoGeoDialog()
  {;
  }
  void CreateControls();

  wxString & GetTopologyName()
  {
    return TopologyName;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool Is3D()
  {
    return HasZ;
  }
  double GetTolerance()
  {
    return Tolerance;
  }
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class CreateTopoNetDialog:public wxDialog
{
//
// a Dialog for creating a new Topology-Network
//
private:
  MyFrame * MainFrame;
  wxString NetworkName;
  bool Spatial;
  int SRID;
  bool HasZ;
  bool Coincident;
public:
    CreateTopoNetDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ CreateTopoNetDialog()
  {;
  }
  void CreateControls();

  wxString & GetNetworkName()
  {
    return NetworkName;
  }
  bool IsSpatial()
  {
    return Spatial;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool Is3D()
  {
    return HasZ;
  }
  bool AllowsCoincident()
  {
    return Coincident;
  }
  void OnSpatialChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class WfsParams
{
// parameters for WFS download
private:
  class WfsDialog * Mother;
  wxGauge *ProgressCtrl;
  sqlite3 *sqlite;
  wxString wfs_version;
  wxString url;
  wxString alt_describe;
  wxString layer_name;
  int swap_axes;
  wxString table;
  wxString primary_key;
  int spatial_index;
  int page_size;
  wxString extra;
  void (*callback)(int, void *);
  int ret;
  char *err_msg;
  int rows;
  int ProgressCount;
  int LastProgressCount;
public:
    WfsParams()
  {;
  }
   ~WfsParams()
  {;
  }
  void Initialize(WfsDialog * mother, sqlite3 * sqlite, wxString & wfs_version,
                  wxString & url, wxString & alt_describe,
                  wxString & layer_name, int swap_axes, wxString & table,
                  wxString & primary_key, int spatial_index, int page_size,
                  wxString & extra, void (*callback)(int, void *))
  {
    Mother = mother;
    this->sqlite = sqlite;
    this->wfs_version = wfs_version;
    this->url = url;
    this->alt_describe = alt_describe;
    this->layer_name = layer_name;
    this->swap_axes = swap_axes;
    this->table = table;
    this->primary_key = primary_key;
    this->spatial_index = spatial_index;
    this->page_size = page_size;
    this->extra = extra;
    this->callback = callback;
    ProgressCount = 0;
    LastProgressCount = 0;
  }
  WfsDialog *GetMother()
  {
    return Mother;
  }
  sqlite3 *GetSqlite()
  {
    return sqlite;
  }
  wxString & GetWfsVersion()
  {
    return wfs_version;
  }
  wxString & GetUrl()
  {
    return url;
  }
  wxString & GetAltDescribeUri()
  {
    return alt_describe;
  }
  wxString & GetLayerName()
  {
    return layer_name;
  }
  int GetSwapAxes()
  {
    return swap_axes;
  }
  wxString & GetTable()
  {
    return table;
  }
  wxString & GetPrimaryKey()
  {
    return primary_key;
  }
  int GetSpatialIndex()
  {
    return spatial_index;
  }
  int GetPageSize()
  {
    return page_size;
  }
  wxString & GetExtra()
  {
    return extra;
  }
  void (*GetCallback())(int, void *)
  {
    return callback;
  }
  void SetRet(int ret)
  {
    this->ret = ret;
  }
  int GetRet()
  {
    return ret;
  }
  void SetErrMsg(char *err_msg)
  {
    this->err_msg = err_msg;
  }
  char *GetErrMsg()
  {
    return err_msg;
  }
  void SetRows(int rows)
  {
    this->rows = rows;
  }
  int GetRows()
  {
    return rows;
  }
  int *GetProgressCountPtr()
  {
    return &ProgressCount;
  }
  void SetLastProgressCount()
  {
    LastProgressCount = 0;
  }
  int GetProgressCount()
  {
    return ProgressCount;
  }
  int GetLastProgressCount()
  {
    return LastProgressCount;
  }
};

class WfsKey
{
// a WFS Keyword
private:
  wxString Keyword;
  WfsKey *Next;
public:
    WfsKey(wxString key)
  {
    Keyword = key;
    Next = NULL;
  }
   ~WfsKey()
  {;
  }
  wxString & GetKeyword()
  {
    return Keyword;
  }
  void SetNext(WfsKey * next)
  {
    Next = next;
  }
  WfsKey *GetNext()
  {
    return Next;
  }
};

class WfsKeywords
{
// an ancillary class storing the WFS Keywords dictionary
private:
  WfsKey * First;
  WfsKey *Last;
  WfsKey **SortedArray;
  int MaxSorted;
public:
    WfsKeywords()
  {
    First = NULL;
    Last = NULL;
    SortedArray = NULL;
    MaxSorted = 0;
  }
   ~WfsKeywords();
  void Add(const char *key);
  void Sort();
  int GetMaxSorted()
  {
    return MaxSorted;
  }
  WfsKey *GetKey(int index);
};

class WfsDialog:public wxDialog
{
//
// a dialog supporting data import from a WFS datasource
//
private:
  wxTimer * ProgressTimer;
  MyFrame *MainFrame;
  WfsParams Params;
  gaiaWFScatalogPtr Catalog;
  WfsKeywords *Keywords;
  wxGrid *WfsView;
  wxGauge *Progress;
  int CurrentEvtRow;
  int CurrentEvtColumn;
  bool ProxyEnabled;
  wxString WfsGetCapabilitiesURL;
  wxString PreviousHttpProxy;
  wxString HttpProxy;
  gaiaWFSitemPtr FindLayerByName(wxString & name);
  void SelectLayer();
public:
    WfsDialog()
  {
    Catalog = NULL;
    WfsView = NULL;
    Keywords = NULL;
    ProxyEnabled = false;
    ProgressTimer = NULL;
  }
  virtual ~ WfsDialog()
  {
    if (ProgressTimer)
      {
        ProgressTimer->Stop();
        delete ProgressTimer;
      }
    if (Catalog != NULL)
      destroy_wfs_catalog(Catalog);
    if (Keywords != NULL)
      delete Keywords;
  }
  void ResetProgress();
  void ProgressWait();
  void ProgressUpdate(int rows);
  bool Create(MyFrame * parent, wxString & wfs_url, wxString & proxy);
  void CreateControls();
  void OnProxy(wxCommandEvent & event);
  void OnPagingChanged(wxCommandEvent & event);
  void OnLeftClick(wxGridEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnKeyFilter(wxCommandEvent & event);
  void OnKeyReset(wxCommandEvent & event);
  void OnCatalog(wxCommandEvent & event);
  void OnReset(wxCommandEvent & event);
  void OnLoadFromWfs(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
  void OnCmdCopy(wxCommandEvent & event);
  void OnCmdSelectLayer(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
  void OnRefreshTimer(wxTimerEvent & event);
};

class CheckGeometryDialog:public wxDialog
{
//
// a dialog supporting Check Geometry Column
//
private:
  MyFrame * MainFrame;
  wxString Table;
  wxString Geometry;
public:
    CheckGeometryDialog()
  {;
  }
  virtual ~ CheckGeometryDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & table, wxString & geom);
  void CreateControls();
  void OnYes(wxCommandEvent & event);
  void OnNo(wxCommandEvent & event);
};

class SanitizeGeometryDialog:public wxDialog
{
//
// a dialog supporting Sanitize Geometry Column
//
private:
  MyFrame * MainFrame;
  wxString TmpPrefix;
  wxString Table;
  wxString Geometry;
public:
    SanitizeGeometryDialog()
  {;
  }
  virtual ~ SanitizeGeometryDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & table, wxString & geom);
  void CreateControls();
  wxString & GetTmpPrefix()
  {
    return TmpPrefix;
  }
  void OnYes(wxCommandEvent & event);
  void OnNo(wxCommandEvent & event);
};

class MalformedGeomsDialog:public wxDialog
{
//
// a dialog displaying a Malformed Geometries list
//
private:
  MyFrame * MainFrame;
  wxString Table;
  wxString Geometry;
  wxGrid *GridCtrl;
  MalformedGeomsList *List;
  int CurrentEvtRow;
  int CurrentEvtColumn;
public:
    MalformedGeomsDialog()
  {
    List = NULL;
  }
  MalformedGeomsDialog(MyFrame * parent, wxString & table, wxString & column,
                       MalformedGeomsList * list);
  bool Create(MyFrame * parent, wxString & table, wxString & column,
              MalformedGeomsList * list);
  virtual ~ MalformedGeomsDialog()
  {
    if (List)
      delete List;
  }
  void CreateControls();
  void OnClose(wxCommandEvent & event);
  void OnRepair(wxCommandEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCmdClearSelection(wxCommandEvent & event);
  void OnCmdSelectAll(wxCommandEvent & event);
  void OnCmdSelectRow(wxCommandEvent & event);
  void OnCmdSelectColumn(wxCommandEvent & event);
  void OnCmdCopy(wxCommandEvent & event);
  void OnCmdBlob(wxCommandEvent & event);
};

class TableViewItem
{
//
// an ancillary class wrapping Tables and Views
//
private:
  wxString DbName;
  wxString Name;
  bool View;
  bool Virtual;
  bool Geometry;
  bool GeoPackageGeometry;
  bool GeoPackageVirtualGeometry;
  bool FdoOgrGeometry;
  bool FdoOgrVirtualGeometry;
  TableViewItem *Next;
public:
    TableViewItem(wxString & db, wxString & name);
    TableViewItem(wxString & name, bool is_view, bool is_virtual);
   ~TableViewItem()
  {;
  }
  wxString & GetDbName()
  {
    return DbName;
  }
  wxString & GetName()
  {
    return Name;
  }
  bool IsView()
  {
    return View;
  }
  bool IsVirtual()
  {
    return Virtual;
  }
  void SetGeometry()
  {
    Geometry = true;
  }
  bool IsGeometry()
  {
    return Geometry;
  }
  void SetGeoPackageGeometry()
  {
    GeoPackageGeometry = true;
  }
  bool IsGeoPackageGeometry()
  {
    return GeoPackageGeometry;
  }
  void SetGeoPackageVirtualGeometry()
  {
    GeoPackageVirtualGeometry = true;
  }
  bool IsGeoPackageVirtualGeometry()
  {
    return GeoPackageVirtualGeometry;
  }
  void SetFdoOgrGeometry()
  {
    FdoOgrGeometry = true;
  }
  bool IsFdoOgrGeometry()
  {
    return FdoOgrGeometry;
  }
  void SetFdoOgrVirtualGeometry()
  {
    FdoOgrVirtualGeometry = true;
  }
  bool IsFdoOgrVirtualGeometry()
  {
    return FdoOgrVirtualGeometry;
  }
  bool IsPostgresTable(class MyPostgres * list);
  bool IsTmpMetadata();
  void SetNext(TableViewItem * next)
  {
    Next = next;
  }
  TableViewItem *GetNext()
  {
    return Next;
  }
};

class TableViewList
{
//
// an ancillary class used to build the Tree Control
//
private:
  TableViewItem * First;
  TableViewItem *Last;
  int Count;
  TableViewItem **Sorted;
public:
    TableViewList();
   ~TableViewList();
  void Add(wxString & db, wxString & name);
  void Add(wxString & name, bool isView, bool isVirtual);
  void PrepareSorted();
  void SetGeometry(wxString & name);
  void SetGeoPackageGeometry(wxString & name);
  void SetGeoPackageVirtualGeometry(wxString & name);
  void SetFdoOgrGeometry(wxString & name);
  void SetFdoOgrVirtualGeometry(wxString & name);
  TableViewItem *GetFirst()
  {
    return First;
  }
};

class DbStatusDialog:public wxDialog
{
//
// a dialog displaying DB Status infos
//
private:
  MyFrame * MainFrame;
  wxGrid *GridCtrl;
  wxStaticBitmap *Graph;
  int CurrentEvtRow;
  int CurrentEvtColumn;
  int *DynamicIds;
  int *DynamicModes;
  wxTimer *RefreshTimer;
  enum StatusModes
  {
    ModeNone,
    ModeStatusBoth,
    ModeStatusFirst,
    ModeStatusSecond,
    ModeStatusBothBytes,
    ModeStatusFirstBytes,
    ModeStatusSecondBytes,
    ModeDbStatusBoth,
    ModeDbStatusFirst,
    ModeDbStatusSecond,
    ModeDbStatusBothBytes,
    ModeDbStatusFirstBytes,
    ModeDbStatusSecondBytes
  };
public:
    DbStatusDialog();
  bool Create(MyFrame * parent);
    virtual ~ DbStatusDialog();
  void CreateControls();
  void OnClose(wxCommandEvent & event);
  void OnReset(wxCommandEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCmdClearSelection(wxCommandEvent & event);
  void OnCmdSelectAll(wxCommandEvent & event);
  void OnCmdSelectRow(wxCommandEvent & event);
  void OnCmdSelectColumn(wxCommandEvent & event);
  void OnCmdCopy(wxCommandEvent & event);
  void OnRefreshTimer(wxTimerEvent & event);
};

class MyStatusBar:public wxStatusBar
{
//
// a StatusBar with an Icon
//
private:
  MyFrame * Parent;
  wxStaticBitmap *Bitmap;
public:
    MyStatusBar(MyFrame * parent);
    virtual ~ MyStatusBar()
  {;
  }
  void SetSecurityRelaxedIcon();
  void SetSecurityStrongIcon();
  void SetReadOnlyIcon();
  void SetNotConnectedIcon();
  void SetText(wxString & msg);
  void OnSize(wxSizeEvent & event);
};

class CurrentSqlFilters
{
//
// a class storing current SQL Filters
//
private:
  bool Valid;
  wxString DbPrefix;
  wxString TableName;
  wxString GeometryColumn;
  bool ReadOnly;
  bool Where1Enabled;
  bool Where2Enabled;
  bool Where3Enabled;
  bool AndOr12;
  bool AndOr23;
  wxString Where1Column;
  wxString Where2Column;
  wxString Where3Column;
  wxString Where1Operator;
  wxString Where2Operator;
  wxString Where3Operator;
  wxString Where1Value;
  wxString Where2Value;
  wxString Where3Value;
  bool Order1Enabled;
  bool Order2Enabled;
  bool Order3Enabled;
  bool Order4Enabled;
  wxString Order1Column;
  wxString Order2Column;
  wxString Order3Column;
  wxString Order4Column;
  bool Order1Desc;
  bool Order2Desc;
  bool Order3Desc;
  bool Order4Desc;
  wxString FreeHand;
public:
    CurrentSqlFilters()
  {;
  }
   ~CurrentSqlFilters()
  {;
  }
  void Initialize(wxString & db_prefix, wxString & table, bool read_only,
                  wxString & geom_column);
  void Reset();
  void Save(class SqlFiltersDialog * dlg);
  bool IsValid()
  {
    return Valid;
  }
  bool IsMapLinked()
  {
    if (GeometryColumn.Len() == 0)
      return false;
    return true;
  }
  wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetTableName()
  {
    return TableName;
  }
  wxString & GetGeometryColumn()
  {
    return GeometryColumn;
  }
  bool IsReadOnly()
  {
    return ReadOnly;
  }
  bool IsWhere1Enabled()
  {
    return Where1Enabled;
  }
  bool IsWhere2Enabled()
  {
    return Where2Enabled;
  }
  bool IsWhere3Enabled()
  {
    return Where3Enabled;
  }
  bool IsAndOr12()
  {
    return AndOr12;
  }
  bool IsAndOr23()
  {
    return AndOr23;
  }
  wxString & GetWhere1Column()
  {
    return Where1Column;
  }
  wxString & GetWhere2Column()
  {
    return Where2Column;
  }
  wxString & GetWhere3Column()
  {
    return Where3Column;
  }
  wxString & GetWhere1Operator()
  {
    return Where1Operator;
  }
  wxString & GetWhere2Operator()
  {
    return Where2Operator;
  }
  wxString & GetWhere3Operator()
  {
    return Where3Operator;
  }
  wxString & GetWhere1Value()
  {
    return Where1Value;
  }
  wxString & GetWhere2Value()
  {
    return Where2Value;
  }
  wxString & GetWhere3Value()
  {
    return Where3Value;
  }
  bool IsOrder1Enabled()
  {
    return Order1Enabled;
  }
  bool IsOrder2Enabled()
  {
    return Order2Enabled;
  }
  bool IsOrder3Enabled()
  {
    return Order3Enabled;
  }
  bool IsOrder4Enabled()
  {
    return Order4Enabled;
  }
  wxString & GetOrder1Column()
  {
    return Order1Column;
  }
  wxString & GetOrder2Column()
  {
    return Order2Column;
  }
  wxString & GetOrder3Column()
  {
    return Order3Column;
  }
  wxString & GetOrder4Column()
  {
    return Order4Column;
  }
  bool IsOrder1Desc()
  {
    return Order1Desc;
  }
  bool IsOrder2Desc()
  {
    return Order2Desc;
  }
  bool IsOrder3Desc()
  {
    return Order3Desc;
  }
  bool IsOrder4Desc()
  {
    return Order4Desc;
  }
  wxString & GetFreeHand()
  {
    return FreeHand;
  }
};

class MyAttachedTable
{
//
// an object wrapping a GeoTable into an ATTACHED-DB
//
private:
  wxString Name;
  int Type;
  MyAttachedTable *Next;
public:
    MyAttachedTable(wxString & name, int type)
  {
    Name = name;
    Type = type;
    Next = NULL;
  }
   ~MyAttachedTable()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  int GetType()
  {
    return Type;
  }
  void SetNext(MyAttachedTable * next)
  {
    Next = next;
  }
  MyAttachedTable *GetNext()
  {
    return Next;
  }
};

class MyAttachedDB
{
//
// an object wrapping an ATTACHED-DB
//
private:
  wxString DbPrefix;
  wxString Path;
  bool GeoPackage;
  bool FdoOgr;
  bool Initialized;
  MyAttachedTable *First;
  MyAttachedTable *Last;
  MyAttachedDB *Prev;
  MyAttachedDB *Next;
public:
    MyAttachedDB(wxString & db_prefix, wxString & path);
   ~MyAttachedDB();
    wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetPath()
  {
    return Path;
  }
  void SetGeoPackage()
  {
    GeoPackage = true;
  }
  bool IsGeoPackage()
  {
    return GeoPackage;
  }
  void SetFdoOgr()
  {
    FdoOgr = true;
  }
  bool IsFdoOgr()
  {
    return FdoOgr;
  }
  void SetInitialized()
  {
    Initialized = true;
  }
  bool IsInitialized()
  {
    return Initialized;
  }
  void AddGeoTable(wxString & name, int type);
  MyAttachedTable *GetFirst()
  {
    return First;
  }
  void SetPrev(MyAttachedDB * prev)
  {
    Prev = prev;
  }
  MyAttachedDB *GetPrev()
  {
    return Prev;
  }
  void SetNext(MyAttachedDB * next)
  {
    Next = next;
  }
  MyAttachedDB *GetNext()
  {
    return Next;
  }
};

class MyAttachedDbList
{
//
// list of all currently ATTACHED-DBs
//
private:
  MyAttachedDB * First;
  MyAttachedDB *Last;
public:
    MyAttachedDbList();
   ~MyAttachedDbList();
  void Flush();
  MyAttachedDB *Find(wxString & db_prefix, wxString & path);
  void Insert(wxString & db_prefix, wxString & path);
  void Remove(MyAttachedDB * item);
  void Remove(wxString & db_prefix);
  void AddGeoTable(wxString & db_prefix, wxString & table_name, int type);
  MyAttachedTable *FindGeoTable(wxString & db_prefix, wxString & name);
  MyAttachedDB *GetFirst()
  {
    return First;
  }
};

class MyPostgresCol
{
//
// a PostgreSQL Column
//
private:
  wxString Name;
  bool PK;
  MyPostgresCol *Next;
public:
    MyPostgresCol(wxString & name)
  {
    Name = name;
    PK = false;
    Next = NULL;
  }
   ~MyPostgresCol()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetPK()
  {
    PK = true;
  }
  bool IsPK()
  {
    return PK;
  }
  void SetNext(MyPostgresCol * next)
  {
    Next = next;
  }
  MyPostgresCol *GetNext()
  {
    return Next;
  }
};

class MyPostgresColumns
{
//
// a list of PostgreSQL Columns
//
private:
  MyPostgresCol * First;
  MyPostgresCol *Last;
public:
    MyPostgresColumns()
  {
    First = NULL;
    Last = NULL;
  }
   ~MyPostgresColumns();
  void Add(wxString & name);
  void SetPK(wxString & name);
  MyPostgresCol *GetFirst()
  {
    return First;
  }
  char *BuildWhere();
};

class MyPostGisGeometry
{
//
// a PostGIS Geometry column
//
private:
  wxString Name;
  wxString GeomType;
  bool MultiType;
  int Srid;
  int Dims;
  MyPostGisGeometry *Next;
public:
    MyPostGisGeometry(wxString & name, wxString & type, int srid, int dims);
   ~MyPostGisGeometry()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  wxString & GetGeomType()
  {
    return GeomType;
  }
  bool IsMultiType()
  {
    return MultiType;
  }
  int GetSrid()
  {
    return Srid;
  }
  int GetDims()
  {
    return Dims;
  }
  void SetNext(MyPostGisGeometry * next)
  {
    Next = next;
  }
  MyPostGisGeometry *GetNext()
  {
    return Next;
  }
};

class MyPostgresPK
{
//
// a ProgresSQL PK column
//
private:
  wxString Name;
  MyPostgresPK *Next;
public:
    MyPostgresPK(wxString & name)
  {
    Name = name;
    Next = NULL;
  }
   ~MyPostgresPK()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetNext(MyPostgresPK * next)
  {
    Next = next;
  }
  MyPostgresPK *GetNext()
  {
    return Next;
  }
};

class MyPostgresTable
{
//
// a PostgreSQL Table
//
private:
  wxString Name;
  wxString VirtName;
  wxString PostGisName;
  bool PkChecked;
  bool GrantSelect;
  bool GrantInsertUpdateDelete;
  MyPostgresPK *FirstPK;
  MyPostgresPK *LastPK;
  MyPostGisGeometry *First;
  MyPostGisGeometry *Last;
  MyPostgresTable *Next;
public:
    MyPostgresTable(wxString & name);
   ~MyPostgresTable();
    wxString & GetName()
  {
    return Name;
  }
  void SetVirtName(wxString & name)
  {
    VirtName = name;
  }
  wxString & GetVirtName()
  {
    return VirtName;
  }
  void SetPostGisName(wxString & name)
  {
    PostGisName = name;
  }
  wxString & GetPostGisName()
  {
    return PostGisName;
  }
  void Add(wxString & geometry, wxString & type, int srid, int dims);
  void AddPK(wxString & column);
  MyPostGisGeometry *Find(wxString & column);
  bool IsPkColumn(wxString & column);
  bool HasPK();
  void SetGrants(bool grantSelect, bool granInsertUpdateDelete)
  {
    GrantSelect = grantSelect;
    GrantInsertUpdateDelete = granInsertUpdateDelete;
  }
  bool CanSelect()
  {
    return GrantSelect;
  }
  bool CanInsertUpdateDelete()
  {
    return GrantInsertUpdateDelete;
  }
  bool IsPkAlreadyChecked()
  {
    return PkChecked;
  }
  void SetPkChecked()
  {
    PkChecked = true;
  }
  MyPostgresPK *GetFirstPK()
  {
    return FirstPK;
  }
  MyPostGisGeometry *GetFirst()
  {
    return First;
  }
  void SetNext(MyPostgresTable * next)
  {
    Next = next;
  }
  MyPostgresTable *GetNext()
  {
    return Next;
  }
};

class MyPostgresView
{
//
// a PostgreSQL View
//
private:
  wxString Name;
  wxString VirtName;
  bool GrantSelect;
  bool GrantInsertUpdateDelete;
  MyPostgresView *Next;
public:
    MyPostgresView(wxString & name);
   ~MyPostgresView()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetVirtName(wxString & name)
  {
    VirtName = name;
  }
  wxString & GetVirtName()
  {
    return VirtName;
  }
  void SetGrants(bool grantSelect, bool granInsertUpdateDelete)
  {
    GrantSelect = grantSelect;
    GrantInsertUpdateDelete = granInsertUpdateDelete;
  }
  bool CanSelect()
  {
    return GrantSelect;
  }
  bool CanInsertUpdateDelete()
  {
    return GrantInsertUpdateDelete;
  }
  void SetNext(MyPostgresView * next)
  {
    Next = next;
  }
  MyPostgresView *GetNext()
  {
    return Next;
  }
};

class MyPostgresSchema
{
//
// a PostgreSQL Schema
//
private:
  wxString Name;
  wxTreeItemId TreeNode;
  MyPostgresTable *FirstTable;
  MyPostgresTable *LastTable;
  MyPostgresTable *CurrentTable;
  MyPostgresView *FirstView;
  MyPostgresView *LastView;
  MyPostgresSchema *Next;
public:
    MyPostgresSchema(wxString & name);
   ~MyPostgresSchema();
    wxString & GetName()
  {
    return Name;
  }
  void Add(wxString & name, wxString & geometry, wxString & type, int srid,
           int dims);
  void Add(wxString & name);
  void SetTreeNode(wxTreeItemId & node)
  {
    TreeNode = node;
  }
  wxTreeItemId & GetTreeNode()
  {
    return TreeNode;
  }
  MyPostgresTable *GetFirstTable()
  {
    return FirstTable;
  }
  MyPostgresView *GetFirstView()
  {
    return FirstView;
  }
  void SetNext(MyPostgresSchema * next)
  {
    Next = next;
  }
  MyPostgresSchema *GetNext()
  {
    return Next;
  }
};

class MyPostgresConn
{
//
// a connection to some PosgreSQL DBMS
//
private:
  wxString Host;
  wxString HostAddr;
  int Port;
  wxString DbName;
  wxString User;
  bool ReadOnly;
  bool TextDates;
  wxString ConnectionString;
  MyPostgresSchema *First;
  MyPostgresSchema *Last;
  MyPostgresSchema *Current;
  MyPostgresConn *Prev;
  MyPostgresConn *Next;
public:
    MyPostgresConn(wxString & host, wxString & hostaddr, int port,
                   wxString & dbname, wxString & user, bool readOnly,
                   bool textDates);
   ~MyPostgresConn();
    wxString & GetHost()
  {
    return Host;
  }
  wxString & GetHostAddr()
  {
    return HostAddr;
  }
  int GetPort()
  {
    return Port;
  }
  wxString & GetDbName()
  {
    return DbName;
  }
  wxString & GetUser()
  {
    return User;
  }
  bool IsReadOnly()
  {
    return ReadOnly;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  void SetConnectionString(const char *connection_string)
  {
    ConnectionString = wxString::FromUTF8(connection_string);
  }
  wxString & GetConnectionString()
  {
    return ConnectionString;
  }
  MyPostgresSchema *Add(wxString & schema);
  void Add(wxString & schema, wxString & table, wxString & geometry,
           wxString & type, int srid, int dims);
  void Add(wxString & schema, wxString & view);
  MyPostgresSchema *GetFirst()
  {
    return First;
  }
  void SetPrev(MyPostgresConn * conn)
  {
    Prev = conn;
  }
  MyPostgresConn *GetPrev()
  {
    return Prev;
  }
  void SetNext(MyPostgresConn * conn)
  {
    Next = conn;
  }
  MyPostgresConn *GetNext()
  {
    return Next;
  }
};

class MyPostgres
{
//
// general container for all PostgreSQL connections
//
private:
  MyPostgresConn * First;
  MyPostgresConn *Last;
  MyPostgresConn *Current;
public:
    MyPostgres()
  {
    First = NULL;
    Last = NULL;
    Current = NULL;
  }
   ~MyPostgres()
  {
    Clear();
  }
  void Clear();
  MyPostgresConn *Insert(wxString & host, wxString & hostaddr, int port,
                         wxString & dbname, wxString & user, bool readOnly,
                         bool textDates);
  MyPostgresConn *Find(wxString & host, wxString & hostaddr, int port,
                       wxString & dbname, wxString & user);
  MyPostgresTable *FindTable(class MyFrame * parent, wxString & virtName);
  MyPostgresTable *FindPostGisView(class MyFrame * parent, wxString & virtName);
  void Remove(MyPostgresConn * conn);
  bool CheckUniqueVirtName(wxString & virtName);
  void MakeUniqueVirtName(wxString & baseName, wxString & uniqueName);
  MyPostgresConn *GetFirst()
  {
    return First;
  }
};

class MyFrame:public wxFrame
{
//
// the main GUI frame
//
private:
  char *Old_SPATIALITE_SECURITY_ENV;
  class MyMapPanel *MapPanel;   // the Map Panel
  wxString AutoFDOmsg;
  wxString AutoGPKGmsg;
  MyAttachedDbList AttachedList;
  bool SpatiaLiteMetadata;
  wxAuiManager Manager;         // the GUI manager
  wxString ConfigLayout;        // PERSISTENCY - the layout configuration
  int ConfigPaneX;              // PERSISTENCY - the main pane screen origin X
  int ConfigPaneY;              // PERSISTENCY - the main pane screen origin Y
  int ConfigPaneWidth;          // PERSISTENCY - the main pane screen width
  int ConfigPaneHeight;         // PERSISTENCY - the main pane screen height
  wxString ConfigDbPath;        // PERSISTENCY - the last opened DB path
  wxString ConfigDir;           // PERSISTENCY - the last used directory
  wxString HttpProxy;           // PERSISTENCY - the last used HTTP Proxy
  wxString WfsGetCapabilitiesURL; // PERSISTENCY - the last used WFS GetCapabilities URL
  bool MapMultiThreadingEnabled;  // PERSISTENCY - Map MultiThreading Enabled
  int MapMaxThreads;            // PERSISTENCY - Map MaxThreads
  bool MapAutoTransformEnabled; // PERSISTENCY - Map AutoTransform Enabled
  MyTableTree *TableTree;       // the tables tree list
  MyQueryView *QueryView;       // the QueryResult panel
  MyResultSetView *RsView;      // the QueryResult panel
  bool HelpPane;                // is the HELP pane already opened ?
  bool SecurityRelaxed;         // is "SPATIALITE_SECURITY=relaxed" currently set ?
  int RL2MaxThreads;            // max concurrent threads for RL2
  sqlite3 *SqliteHandle;        // handle for SQLite DB
  wxString SqlitePath;          // path of SQLite DB
  void *SpliteInternalCache;    // pointer to the InternalCache supporting the DB connection
  void *RL2PrivateData;         // pointer to RL2 Private Data
  wxString ExternalSqlitePath;  // path of external SQLite DB [LOAD/SAVE MEMORY database]
  bool MemoryDatabase;          // TRUE if we are currently working on the MEMORY database
  wxString LastDirectory;       // path of directory used  
  int CharsetsLen;              // # charsets defined
  wxString *Charsets;           // table of charsets [code only]
  wxString *CharsetsNames;      // table of charsets [with description]
  wxString LocaleCharset;       // locale charset
  wxString DefaultCharset;      // default charset
  bool AskCharset;              // switch to set default charset for every output
  int TablesLen;                // # tables defined
  wxString *TableNames;         // array of tables
  wxBitmap *BtnCreateNew;       // button icon for DB CREATE&CONNECT
  wxBitmap *BtnConnect;         // button icon for DB CONNECT
  wxBitmap *BtnConnectReadOnly; // button icon for DB CONNECT - READ ONLY
  wxBitmap *BtnDisconnect;      // button icon for DB DISCONNECT
  wxBitmap *BtnMemDbLoad;       // button icon for MEMORY DB LOAD
  wxBitmap *BtnMemDbNew;        // button icon for MEMORY DB NEW
  wxBitmap *BtnMemDbClock;      // button icon for MEMORY DB CLOCK
  wxBitmap *BtnMemDbSave;       // button icon for MEMORY DB SAVE
  wxBitmap *BtnVacuum;          // button icon for DB VACUUM
  wxBitmap *BtnSqlScript;       // button icon for Execute SQL SCRIPT
  wxBitmap *BtnQueryComposer;   // button icon for Query/View Composer
  wxBitmap *BtnCharset;         // button icon for Default CHARSET
  wxBitmap *BtnLoadShp;         // button icon for LOAD SHP
  wxBitmap *BtnLoadGeoJSON;     // button icon for LOAD GeoJSON
  wxBitmap *BtnLoadTxt;         // button icon for LOAD TXT/CSV
  wxBitmap *BtnLoadDbf;         // button icon for LOAD DBF
  wxBitmap *BtnLoadXL;          // button icon for LOAD_XL
  wxBitmap *BtnVirtualShp;      // button icon for VIRTUAL SHP
  wxBitmap *BtnVirtualGeoJSON;  // button icon for VIRTUAL GeoJSON
  wxBitmap *BtnVirtualTxt;      // button icon for VIRTUAL TXT/CSV
  wxBitmap *BtnVirtualDbf;      // button icon for VIRTUAL DBF
  wxBitmap *BtnVirtualXL;       // button icon for VIRTUAL XL
  wxBitmap *BtnNetwork;         // button icon for BUILD NETWORK
  wxBitmap *BtnExif;            // button icon for EXIF LOAD
  wxBitmap *BtnGpsPics;         // button icon for GPS_PICS LOAD
  wxBitmap *BtnLoadXml;         // button icon for XML LOAD
  wxBitmap *BtnSrids;           // button icon for SEARCH SRIDs
  wxBitmap *BtnHelp;            // button icon for HELP
  wxBitmap *BtnAbout;           // button icon for ABOUT
  wxBitmap *BtnExit;            // button icon for EXIT
  wxBitmap *BtnAttach;          // button icon for ATTACH
  wxBitmap *BtnSqlLog;          // button icon for SQL LOG
  wxBitmap *BtnDbStatus;        // button icon for DB STATUS
  wxBitmap *BtnCheckGeom;       // button icon for CheckGeom
  wxBitmap *BtnSaneGeom;        // button icon for SaneGeom
  wxBitmap *BtnWFS;             // button icon for WFS
  wxBitmap *BtnDXF;             // button icon for DXF
  wxBitmap *BtnMap;             // button icon for Map Panel
  wxBitmap *BtnPostgres;        // button icon for PostgreSQL
  rl2WmsCachePtr WmsCache;      // internal WMS Cache
  bool ReadOnlyConnection;
// AutoSave timer
  int AutoSaveInterval;
  int LastTotalChanges;
  wxTimer *TimerAutoSave;
  gaiaGeomCollPtr GeomFromPoint(gaiaPointPtr pt, int srid);
  gaiaGeomCollPtr GeomFromLinestring(gaiaLinestringPtr ln, int srid);
  gaiaGeomCollPtr GeomFromPolygon(gaiaPolygonPtr pg, int srid);
  sqlite3_int64 LastSqlLogID;
  bool SqlLogEnabled;
  bool GetRtTopoVersion(char *buf);
  bool GetLibXml2Version(char *buf);
  MyStatusBar *StatusBar;
  void TestSecurityRelaxed(const char *path);
  bool IsSafeDB(const char *path);
  void SetSecurityRelaxed();
  void ResetSecurity();
  rl2PixelPtr DefaultNoData(unsigned char sample, unsigned char pixel,
                            unsigned char num_bands);
  rl2PixelPtr ParseNoData(wxString & NoData, int SampleType, int PixelType,
                          int NumBands);
  char *GetNum(const char *start, const char *end);
  bool IsValidSqliteFile(wxString & path);
  void DoUpdateRL2MaxThreads();
  void DoAutoDetachDatabases();
  CurrentSqlFilters SqlFilters;
  MyPostgres PostgresList;
  virtualPQ VirtualPQapi;
  wxDynamicLibrary DynamicLibPQ;
  wxString PathLibPQ;
  bool VirtualPQapiOK;
  void DoInitVirtualPQapi();
  void DoLocateLibPQ(wxString & path);
  void DoLoadLibPQ(wxString & path);
  bool DoCheckPostgres(wxString & host, wxString & hostaddr, int port,
                       wxString & dbname, wxString & user);
  bool DoInitPostgres(wxString & host, wxString & hostaddr, int port,
                      wxString & dbname, wxString & user, bool readOnly,
                      bool textDates, const char *conninfo);
  void DoSetUniqueVirtNames();
  void DoCreatePostgresTables();
  void DoDropPostgresTables();
  char *DoCreatePostGisSpatialView(MyPostgresTable * table, char *sql);
  void DoCreatePostGisSpatialViewTriggers(MyPostgresConn * conn,
                                          MyPostgresSchema * schema,
                                          MyPostgresTable * table);
  void GetPQlibVersion(wxString & ver);
  void DoCreatePostgreSqlNodes();
  MyPostgresColumns *DoGetPostgresColumns(MyPostgresTable * table);
  void DoCheckGrantPermissions(void *pg_conn, wxString & user,
                               MyPostgresSchema * schema,
                               MyPostgresTable * table);
  void DoCheckGrantPermissions(void *pg_conn, wxString & user,
                               MyPostgresSchema * schema,
                               MyPostgresView * view);
  void DoLoadZipShp(wxString & path);
  void DoLoadZipDbf(wxString & path);
public:
    MyFrame(const wxString & title, const wxPoint & pos, const wxSize & size);
    virtual ~ MyFrame();
  enum VectorTypes
  {
    VECTOR_UNKNOWN = 0,
    VECTOR_GEOTABLE,
    VECTOR_SPATIALVIEW,
    VECTOR_VIRTUALTABLE,
    VECTOR_TOPOGEO,
    VECTOR_TOPONET
  };

  void UpdateStatusBar(bool changeIcon = true);
  bool IsConnected()
  {
    if (SqliteHandle)
      return true;
    else
      return false;
  }
  bool IsSecurityLevelRelaxed()
  {
    return SecurityRelaxed;
  }
  int GetRL2MaxThreads();

  void InsertIntoLog(wxString & sql);
  void UpdateLog(void);
  void UpdateLog(wxString & error_msg);
  void UpdateAbortedLog(void);
  void EnableSqlLog();
  wxString & GetAutoFDOmsg()
  {
    return AutoFDOmsg;
  }
  void AutoFDOmsgReset()
  {
    AutoFDOmsg = wxT("");
  }
  wxString & GetAutoGPKGmsg()
  {
    return AutoGPKGmsg;
  }
  void AutoGPKGmsgReset()
  {
    AutoGPKGmsg = wxT("");
  }
  void RemoveAttachedDB(wxString & symbol)
  {
    AttachedList.Remove(symbol);
  }
  bool DoAttachDatabase(wxString & path);
  void GetNextAttachedSymbol(wxString & symbol);
  void AddAttachedTable(wxString & dbAlias, wxString & name, int type)
  {
    AttachedList.AddGeoTable(dbAlias, name, type);
  }

  void EnableMapMultiThreading(bool mode)
  {
    MapMultiThreadingEnabled = mode;
  }
  bool IsMapMultiThreadingEnabled()
  {
    return MapMultiThreadingEnabled;
  }
  void SetMapMaxThreads(int max)
  {
    MapMaxThreads = max;
  }
  int GetMapMaxThreads()
  {
    return MapMaxThreads;
  }
  void EnableMapAutoTransform(bool mode)
  {
    MapAutoTransformEnabled = mode;
  }
  bool IsMapAutoTransformEnabled()
  {
    return MapAutoTransformEnabled;
  }
  void MapViewShowSelected(class MapFeaturesList * list, bool zoom_mode);
  int GetMapSRID();

  void CleanTxtTab(char *str);
  char *CleanCsv(const char *str);
  char *CleanHtml(const char *str);
  char *DifQuote(const char *str);
  char *SylkQuote(const char *str);
  void DecimalNumber(double num, char *str, char decimal_point);
  bool TestDateValue(char *date);
  bool TestDateTimeValue(char *datetime);
  bool TestTimeValue(char *time);
  int ComputeSpreadsheetDate(int yy, int mm, int dd);
  double ComputeSpreadsheetTime(int hh, int mm, int ss);
  int GetDateValue(char *date);
  double GetDateTimeValue(char *datetime);
  double GetTimeValue(char *time);
  wxString & GetSqlitePath()
  {
    return SqlitePath;
  }
  wxString & GetExternalSqlitePath()
  {
    return ExternalSqlitePath;
  }
  void SetExternalSqlitePath(wxString & path)
  {
    ExternalSqlitePath = path;
  }
  sqlite3 *GetSqlite()
  {
    return SqliteHandle;
  }
  void *GetSpliteInternalCache()
  {
    return SpliteInternalCache;
  }
  int GetDecimalPrecision();
  void *GetRL2PrivateData()
  {
    return RL2PrivateData;
  }
  bool IsSecurityRelaxed()
  {
    return SecurityRelaxed;
  }
  void CloseHelpPane()
  {
    HelpPane = false;
  }
  void OpenHelpPane()
  {
    HelpPane = true;
  }

  bool OpenDB(bool read_only);
  bool CreateDB();
  void CloseDB();
  void InitializeSpatialMetadata();
  void AutoFDOStart();
  void AutoFDOStop();
  void AutoGPKGStart();
  void AutoGPKGStop();
  void AutoPostgresStart();
  void AutoPostgresStop();
  bool DoClosePostgreSqlConn(wxString & host, wxString & hostaddr, int port,
                             wxString & dbname, wxString & user);
  char *DropPostgreSqlOrphans();
  void DoInitVirtualPG();
  void InitTableTree();
  void LoadHistory();
  bool HasHistory();
  void ListAttached();
  void AutoFDOStart(wxString & dbAlias, MyAttachedDB * db);
  void AutoFDOStop(wxString & dbAlias);
  void AutoGPKGStart(wxString & dbAlias, MyAttachedDB * db);
  void AutoGPKGStop(wxString & dbAlias);
  void InitTableTree(wxString & dbAlias, wxString & path);
  void ClearTableTree();
  int GetMetaDataType();
  bool HasViewsMetadata();
  bool HasVirtsMetadata();
  bool HasViewsMetadata(wxString & dbAlias);
  bool HasVirtsMetadata(wxString & dbAlias);
  void FindGeometries(TableViewList * list);
  void FindGeometries(wxString & dbAlias, TableViewList * list);
  void FindGeoPackageGeometries(TableViewList * list);
  void FindGeoPackageGeometries(wxString & dbAlias, TableViewList * list);
  void FindFdoOgrGeometries(TableViewList * list);
  void FindFdoOgrGeometries(wxString & dbAlias, TableViewList * list);

  void GetTableColumns(wxString & tableName, MyTableInfo * list);
  void GetTableIndices(wxString & tableName, MyTableInfo * list);
  void GetTableTriggers(wxString & tableName, MyTableInfo * list);
  void GetViewColumns(wxString & viewName, MyViewInfo * list);
  void GetViewTriggers(wxString & viewName, MyViewInfo * list);
  void CheckGPKG(wxString & tableName, MyTableInfo * list);
  void CheckFdoOgr(wxString & tableName, MyTableInfo * list);
  void GetIndexFields(wxString & indexName, wxString & tableName,
                      wxTreeItemId & node);
  void GetPrimaryKeyFields(wxString & indexName, wxString & tableName,
                           wxTreeItemId & node);
  void GetForeignKeys(wxString & tableName, wxTreeItemId & node);
  void GetTableColumns(wxString & dbAlias, wxString & tableName,
                       MyTableInfo * list);
  void GetTableIndices(wxString & dbAlias, wxString & tableName,
                       MyTableInfo * list);
  void GetTableTriggers(wxString & dbAlias, wxString & tableName,
                        MyTableInfo * list);
  void GetViewColumns(wxString & dbAlias, wxString & viewName,
                      MyViewInfo * list);
  void GetViewTriggers(wxString & dbAlias, wxString & viewName,
                       MyViewInfo * list);
  void GetIndexFields(wxString & dbAlias, wxString & indexName,
                      wxString & tableName, wxTreeItemId & node);
  void GetPrimaryKeyFields(wxString & dbAlias, wxString & indexName,
                           wxString & tableName, wxTreeItemId & node);
  void GetForeignKeys(wxString & dbAlias, wxString & tableName,
                      wxTreeItemId & node);
  bool ExistsViewsGeometryColumns();
  bool ExistsTopologies();
  bool ExistsTopologies(wxString & dbAlias);
  bool ExistsNetworks();
  bool ExistsNetworks(wxString & dbAlias);
  bool ExistsRasterCoverages();
  bool ExistsRasterCoverages(wxString & dbAlias);
  bool ExistsVectorCoverages();
  bool ExistsVectorCoverages(wxString & dbAlias);
  bool ExistsWmsGetMap();
  bool ExistsWmsGetMap(wxString & dbAlias);
  void ElementaryGeoms(wxString & inTable, wxString & geometry,
                       wxString & outTable, wxString & pKey, wxString & multiID,
                       wxString & type, int *srid, wxString & coordDims,
                       bool *spIdx);
  bool ElementaryViewGeoms(wxString & inTable, wxString & geometry,
                           wxString & type, int *srid, wxString & coordDims,
                           bool *spIdx);
  bool ElementaryVirtGeoms(wxString & inTable, wxString & geometry,
                           wxString & type, int *srid, wxString & coordDims);
  bool DoElementaryGeometries(wxString & inTable, wxString & geometry,
                              wxString & outTable, wxString & pKey,
                              wxString & multiID, wxString & type, int srid,
                              wxString & coordDims, bool spIdx);
  void EditTable(wxString & sql, int *primaryKeys, int *blobCols,
                 wxString & table)
  {
    RsView->EditTable(sql, primaryKeys, blobCols, table);
  }
  bool IsSpatialIndex(wxString & tableName);
  bool IsSpatialIndex(wxString & dbAlias, wxString & tableName);
  bool IsMbrCache(wxString & tableName);
  bool IsMbrCache(wxString & dbAlias, wxString & tableName);
  bool IsGeoPackageSpatialIndex(wxString & tableName);
  bool IsGeoPackageSpatialIndex(wxString & dbAlias, wxString & tableName);
  bool IsTopoFaceSpatialIndex(wxString & tableName);
  bool IsTopoFaceSpatialIndex(wxString & dbAlias, wxString & tableName);
  void SetSql(wxString & sql, bool execute)
  {
    QueryView->SetSql(sql, execute);
  }
  void SetSql(wxString & sql, bool execute, bool coverage,
              wxString & tile_data_db_prefix, wxString & tile_data_table,
              bool reset)
  {
    QueryView->SetSql(sql, execute, coverage, tile_data_db_prefix,
                      tile_data_table, reset);
  }
  bool ExecuteSql(const char *sql, int rowNo);
  void Rollback();
  void InitializeSqlFilters(wxString & db_prefix, wxString & table,
                            bool read_only, wxString & geom_column)
  {
    SqlFilters.Initialize(db_prefix, table, read_only, geom_column);
  }
  bool IsValidSqlFilter()
  {
    return SqlFilters.IsValid();
  }
  void ResetSqlFilters()
  {
    return SqlFilters.Reset();
  }
  bool IsMapLinkedResultSet()
  {
    return SqlFilters.IsMapLinked();
  }
  wxString & GetGeometryColumnFromSqlFilter()
  {
    return SqlFilters.GetGeometryColumn();
  }
  void DoCreateStylingTables(void);
  bool TableAlreadyExists(wxString & name);
  bool CoverageAlreadyExists(wxString & name);
  bool SRIDnotExists(int srid);
  bool CheckMetadata();
  bool CheckMetadata(wxString & dbAlias);
  void SaveConfig();
  void LoadConfig(wxString & externalPath);
  void CheckUpdates();
  bool DoParseNewVersion(const char *text, char **version, char **date,
                         char **download_url);
  void DoGetPackageName(const char *download_url, wxString & name);
  void DoSaveUpdatedPackage(const unsigned char *data, int data_len,
                            wxString & fileName);
  wxString *GetCharsets()
  {
    return Charsets;
  }
  wxString *GetCharsetsNames()
  {
    return CharsetsNames;
  }
  int GetCharsetsLen()
  {
    return CharsetsLen;
  }
  gaiaDbfFieldPtr GetDbfField(gaiaDbfListPtr list, int index);
  void OutputPrjFile(const void *proj_ctx, wxString & path, int srid);
  bool OutputPrjFileProjNew(const void *proj_ctx, wxString & path, int srid);
  void LoadText(wxString & path, wxString & table, wxString & charset,
                bool first_titles, const char decimal_separator,
                const char separator, const char text_separator);
  void DumpTxtTab(wxString & path, wxString & table, wxString & charset);
  void DumpCsv(wxString & path, wxString & table, wxString & charset);
  void DumpHtml(wxString & path, wxString & table, wxString & dbPath,
                wxString & charset);
  void DumpDif(wxString & path, wxString & table, wxString & charset,
               char decimal_point, bool date_time);
  void DumpSylk(wxString & path, wxString & table, wxString & charset,
                bool date_time);
  void DumpKml(wxString & path, wxString & table, wxString & column,
               int precision, wxString & name, bool isNameConst,
               wxString & desc, bool isDescConst);
  void ExportResultSetAsTxtTab(wxString & path, wxString & sql,
                               wxString & charset);
  void ExportResultSetAsCsv(wxString & path, wxString & sql,
                            wxString & charset);
  void ExportResultSetAsHtml(wxString & path, wxString & sql, wxString & dbPath,
                             wxString & charset);
  bool ExportHtmlColorSqlSyntax(FILE * out, wxString & sql, char *out_cs);
  void ExportResultSetAsShp(const void *proj_ctx, wxString & path,
                            wxString & sql, wxString & charset);
  void ExportResultSetAsDif(wxString & path, wxString & sql, wxString & charset,
                            char decimal_point, bool date_time);
  void ExportResultSetAsSylk(wxString & path, wxString & sql,
                             wxString & charset, bool date_time);
  void ExportResultSetAsDbf(wxString & path, wxString & sql,
                            wxString & charset);
  void DoExportXLSX(wxString & path, wxString & sql);
  void GetHelp(wxString & html);
  void FeedZipHtml(unsigned char *zip, int offset, const char *data);
  wxString *GetColumnNames(wxString & table, int *columns);
  void SetLastDirectory(wxString & path)
  {
    LastDirectory = path;
  }
  wxString & GetLastDirectory()
  {
    return LastDirectory;
  }
  rl2WmsCachePtr GetWmsCache()
  {
    return WmsCache;
  }
  void SetHttpProxy(wxString & proxy)
  {
    HttpProxy = proxy;
  }
  wxString & GetHttpProxy()
  {
    return HttpProxy;
  }
  void SetWfsGetCapabilitiesURL(wxString & url)
  {
    WfsGetCapabilitiesURL = url;
  }
  wxString & GetWfsGetCapabilitiesURL()
  {
    return WfsGetCapabilitiesURL;
  }
  wxString & GetLocaleCharset()
  {
    return LocaleCharset;
  }
  wxString & GetDefaultCharset()
  {
    return DefaultCharset;
  }
  wxString & GetCharsetName(wxString & charset);
  int GetCharsetIndex(wxString & charset);
  bool IsSetAskCharset()
  {
    return AskCharset;
  }
  char *ReadSqlLine(FILE * fl, int *len, int *eof);
  MyQueryView *GetQueryView()
  {
    return QueryView;
  }
  MyResultSetView *GetRsView()
  {
    return RsView;
  }
  wxString *GetTables(int *cnt);
  wxString *GetTableColumns(wxString & dbPrefix, wxString & table, int *cnt);
  wxString *GetTableGeometries(wxString & dbPrefix, wxString & table, int *cnt);
  void ImportExifPhotos(wxString & path, bool folder, bool metadata,
                        bool gps_only);
  void ImportGpsPhotos(wxString & path, bool folder, wxString & table,
                       wxString & geometry, bool update_statistics,
                       bool spatial_index);
  void ImportXmlDocuments(wxString & path, bool folder, wxString & suffix,
                          wxString & table, wxString & pkName,
                          wxString & xmlColumn, wxString & inPathColumn,
                          wxString & schemaColumn, wxString & parseErrColumn,
                          wxString & validateErrColumn, int compressed,
                          const char *schemaURI, bool isInternaleSchemaUri);
  void ImportDXFfiles(wxString & path, bool folder, wxString & prefix,
                      wxString & layer, int srid, bool force2d, bool force3d,
                      bool mixed, bool linked, bool unlinked, bool append);
  bool CheckExifTables();
  int ExifLoadDir(wxString & path, bool gps_only, bool metadata);
  int ExifLoadFile(wxString & path, bool gps_only, bool metadata);
  bool UpdateExifTables(unsigned char *blob, int sz,
                        gaiaExifTagListPtr tag_list, bool metadata,
                        wxString & path);
  bool CheckGpsPicsTable(wxString & table, wxString & geometry, bool rtree);
  int GpsPicsLoadDir(wxString & path, sqlite3_stmt * stmt);
  int GpsPicsLoadFile(wxString & path, sqlite3_stmt * stmt);
  bool ImportExifGps(unsigned char *blob, int sz, gaiaExifTagListPtr ttag_list,
                     wxString & path, sqlite3_stmt * stmt);
  bool IsExifGps(gaiaExifTagListPtr tag_list);
  int DxfLoadDir(wxString & path, wxString & prefix, wxString & layer, int srid,
                 bool force2d, bool force3d, bool mixed, bool linked,
                 bool unlinked, bool append, int *failed);
  int DxfLoadFile(wxString & path, wxString & prefix, wxString & layer,
                  int srid, bool force2d, bool force3d, bool mixed, bool linked,
                  bool unlinked, bool append, int *failed);
  bool CheckOrCreateXmlTable(wxString & table, wxString & pkName,
                             wxString & xmlColumn, wxString & inPathColumn,
                             wxString & schemaUriColumn,
                             wxString & parseErrColumn,
                             wxString & validateErrColumn);
  bool IsValidSuffix(const char *fileName, wxString & suffix);
  int XmlDocumentLoadDir(wxString & path, wxString & suffix, int compressed,
                         const char *schemaURI, bool isInternalSchemaUri,
                         wxString & inPathColumn, wxString & schemaUriColumn,
                         wxString & parseErrColumn,
                         wxString & validateErrColumn, sqlite3_stmt * stmt,
                         int *failed);
  int XmlDocumentLoadFile(wxString & path, int compressed,
                          const char *schemaURI, bool isInternalSchemaUri,
                          wxString & inPathColumn, wxString & parseErrColumn,
                          wxString & validateErrColumn,
                          wxString & schemaUriColumn, sqlite3_stmt * stmt,
                          int *failed);
  bool InsertIntoXmlTable(sqlite3_stmt * stmt, char *blob, int sz,
                          wxString & inPathColumn, wxString & path,
                          wxString & schemaUriColumn, const char *schemaUri,
                          wxString & parseErrColumn, const char *parseError,
                          wxString & validateErrColumn,
                          const char *validateError);
  void GetMake(gaiaExifTagListPtr tag_list, wxString & str, bool *ok);
  void GetModel(gaiaExifTagListPtr tag_list, wxString & str, bool *ok);
  void GetGpsTimestamp(gaiaExifTagListPtr tag_list, wxString & str, bool *ok);
  void GetDate(gaiaExifTagListPtr tag_list, wxString & str, bool *ok);
  double GetGpsDirection(gaiaExifTagListPtr tag_list, bool *ok);
  void GetGpsSatellites(gaiaExifTagListPtr tag_list, wxString & str, bool *ok);
  void GetGpsCoords(gaiaExifTagListPtr tag_list, double *longitude,
                    double *latitude, bool *ok);
  sqlite3_int64 GetPixelX(gaiaExifTagListPtr tag_list, bool *ok);
  sqlite3_int64 GetPixelY(gaiaExifTagListPtr tag_list, bool *ok);

  bool MemoryDbSave();
  void LastDitchMemoryDbSave();
  void QueryViewComposer();
  void SqlFiltersComposer();
  MalformedGeomsList *FindMalformedGeoms(wxString & table, wxString & geom,
                                         bool allowRepair);
  void PreRepairPolygons(wxString & table, wxString & geom, int *count);
  void RepairPolygons(wxString & table, wxString & geom, int *count);

  bool IsPrimaryKey(wxString & table, wxString & column);
  void DbPagesCount(int *total, int *frees);

  void EnableAllTools(bool mode = true);
  void DisableAllTools()
  {
    EnableAllTools(false);
  };
  void DoResetSqlFilters()
  {
    SqlFilters.Reset();
  }
  void DoSaveSqlFilters(SqlFiltersDialog * dlg)
  {
    SqlFilters.Save(dlg);
  }

  int TestDotCommand(const char *stmt);
  bool IsDotCommandLoadShp(const char *stmt, char *path, char *table,
                           char *charset, char *column, int *srid,
                           bool *coerce2D, bool *compressed);
  bool IsDotCommandLoadDbf(const char *stmt, char *path, char *table,
                           char *charset);
  bool IsDotCommandLoadXL(const char *stmt, char *path, char *table,
                          int *worksheetIndex, int *firstTitle);
  bool IsDotCommandDumpShp(const char *stmt, char *table,
                           char *column, char *path, char *charset, char *type);
  bool IsViewGeometry(wxString & table, wxString & column);
  bool GetTilePreview(wxString & curretntTileDataDbPrefix,
                      wxString & currentTileData, int currentTileId,
                      unsigned char **blob, int *blobSize);
  class RasterCoverageStylesList *FindRasterCoverageStyles(wxString & coverage);
  class RasterCoverageStylesList *FindRasterStyles();
  class VectorCoverageStylesList *FindVectorCoverageStyles(wxString & coverage);
  class VectorCoverageStylesList *FindVectorStyles();
  class MapConfigurationsList *FindMapConfigurations();
  class CandidateVectorCoveragesList *FindUnregisteredVectorCoverages();
  class CandidateSpatialViewCoveragesList
    * FindUnregisteredSpatialViewCoverages();
  class CandidateVirtualTableCoveragesList
    * FindUnregisteredVirtualTableCoverages();
  class CandidateTopoGeoCoveragesList *FindUnregisteredTopoGeoCoverages();
  class CandidateTopoNetCoveragesList *FindUnregisteredTopoNetCoverages();
  bool DoRegisterVectorCoverage(wxString & name, wxString & table,
                                wxString & geometry, wxString & title,
                                wxString & abstract, wxString & copyright,
                                wxString & license, bool isQueryable,
                                bool isEditable);
  bool DoRegisterSpatialViewCoverage(wxString & name, wxString & view,
                                     wxString & geometry, wxString & title,
                                     wxString & abstract, wxString & copyright,
                                     wxString & license, bool isQueryable,
                                     bool isEditable);
  bool DoRegisterVirtualTableCoverage(wxString & name, wxString & virt_table,
                                      wxString & virt_geometry,
                                      wxString & title, wxString & abstract,
                                      wxString & copyright, wxString & license,
                                      bool isQueryable);
  bool DoRegisterTopoGeoCoverage(wxString & name, wxString & topology,
                                 wxString & title, wxString & abstract,
                                 wxString & copyright, wxString & license,
                                 bool isQueryable, bool isEditable);
  bool DoRegisterTopoNetCoverage(wxString & name, wxString & network,
                                 wxString & title, wxString & abstract,
                                 wxString & copyright, wxString & license,
                                 bool isQueryable, bool isEditable);
  bool ValidateRasterStyle(const char *path, void **blob, int *blob_size);
  bool ValidateRasterStyle(void **blob, int *blob_size, const char *xml);
  bool DoInsertRasterSymbolizer(char *xml);
  bool ValidateVectorStyle(const char *path, void **blob, int *blob_size);
  bool ValidateVectorStyle(void **blob, int *blob_size, const char *xml);
  bool DoInsertVectorSymbolizer(char *xml);
  bool ValidateMapConfig(const char *path, void **blob, int *blob_size);
  bool ValidateMapConfig(void **blob, int *blob_size, const char *xml);
  bool ValidateExternalGraphicResource(const char *path, void **blob,
                                       int *blob_size, wxString & abstract);
  class RasterCoverageSRIDsList *FindRasterAlternativeSRIDs(wxString &
                                                            coverage);
  class VectorCoverageSRIDsList *FindVectorAlternativeSRIDs(wxString &
                                                            coverage);
  int FindVectorType(char *cvg);
  class RasterCoverageKeywordsList *FindRasterKeywords(wxString & coverage);
  class VectorCoverageKeywordsList *FindVectorKeywords(wxString & coverage);
  class ExternalGraphicList *FindExternalGraphic(bool no_svg);
  class TextFontList *FindTextFont(void);
  void CheckTTFont(const char *facename, bool *bold, bool *italic);

  bool GetCurrentlySelectedTable(wxString & table_name)
  {
    return TableTree->GetCurrentlySelectedTable(table_name);
  }
  bool DoGetRasterCoverageInfos(wxString & coverage, wxString & title,
                                wxString & abstract, wxString & copyright,
                                wxString & data_license, wxString & sample,
                                wxString & pixel, wxString & compression,
                                int *srid, bool *mixed_resolutions);
  bool DoImportRasterFiles(wxString & coverage, wxString & title,
                           wxArrayString & paths, int forced_srid,
                           bool with_worldfile, bool pyramidize);
  bool DoImportRaster(sqlite3_stmt * stmt, wxString & coverage, wxString & path,
                      int forced_srid, bool with_worldfile, bool pyramidize);
  bool DoGetVectorCoverageInfos(wxString & coverage, wxString & title,
                                wxString & abstract, wxString & copyright,
                                wxString & data_license, int *origin,
                                wxString & type, int *srid);
  bool CreateRasterCoverage(wxString & CoverageName, wxString & Title,
                            wxString & Abstract, int SampleType, int PixelType,
                            int NumBands, int Compression, int Quality,
                            int TileWidth, int TileHeight,
                            bool NotGeoreferenced, int Srid,
                            double HorzResolution, double VertResolution,
                            wxString NoData, bool StrictResolution,
                            bool MixedResolutions, bool InputPaths, bool MD5,
                            bool Summary, int RedBand, int GreenBand,
                            int BlueBand, int NIRband, bool AutoNDVI,
                            bool isQueryable);
  static void DoubleQuoted(wxString & str);
  bool IsView(wxString & view);
  bool IsWritableView(wxString & view);
  void GetWritableViewPK(wxString & view, wxString & pk_name);

  bool HasFlippedAxes(const char *crs);
  bool BBoxFromLongLat(const char *crs_str, double *minx, double *maxx,
                       double *miny, double *maxy);
  char *GetProjParams(int srid);

  void CreateWmsTables();
  bool IsDefinedWmsGetCapabilities(const char *url);
  bool IsDefinedWmsGetMap(const char *getmap_url, const char *layer_name);
  bool DoGetWmsLayerInfos(wxString & url, wxString & layer_name,
                          wxString & title, wxString & abstract,
                          wxString & copyright, int *license_id,
                          wxString & data_license, bool *is_queryable,
                          wxString & getfeatureinfo_url);
  bool RegisterWmsGetCapabilities(const char *url, const char *title,
                                  const char *abstract);
  bool RegisterWmsGetMap(const char *getcapabilities_url,
                         const char *getmap_url, const char *layer_name,
                         const char *title, const char *abstract,
                         const char *version, const char *ref_sys,
                         const char *image_format, const char *style,
                         int transparent, int flip_axes, int is_queryable,
                         int tiled, int cached, int tile_width, int tile_height,
                         const char *bgcolor, const char *getfeatureinfo);
  bool RegisterWmsSetting(const char *url, const char *layer_name,
                          const char *key, const char *value, int is_default);
  bool RegisterWmsSRS(const char *url, const char *layer_name,
                      const char *ref_sys, double minx, double miny,
                      double maxx, double maxy, int is_default);
  bool TransformWmsBBox(const char *ref_sys, double geoMinX, double geoMinY,
                        double geoMaxX, double geoMaxY, double *minx,
                        double *miny, double *maxx, double *maxy);
  void DoRegisterWMS();
  void DoCreateTopoGeo();
  void DoCreateTopoNet();
  bool DoGuessSridFromSHP(wxString & path, int *srid);
  bool DoGuessSridFromZipSHP(const char *zip_path, const char *bansename,
                             int *srid);

  void MapPanelClosing();

  void CreateNetwork(wxString & table, wxString & from, wxString & to,
                     bool isNoGeometry, wxString & geom, bool isNameEnabled,
                     wxString & name, bool isGeomLength, wxString & cost,
                     bool isBidirectional, bool isOneWays,
                     wxString & oneWayFromTo, wxString & oneWayToFrom,
                     bool aStarSupported, wxString & dataTableName,
                     wxString & virtualTableName, bool overwrite);
  void GetLastRoutingError(wxString & errCause);

  bool HasPostgreSqlConnections();
  void DoPostgreSqlConnection();
  bool DoCheckPostGisGeometry(wxString & virtName, wxString & columnName);
  MyPostgresTable *FindPostgresTable(wxString & virtName)
  {
    return PostgresList.FindTable(this, virtName);
  }
  MyPostgresTable *FindPostGisView(wxString & virtName)
  {
    return PostgresList.FindPostGisView(this, virtName);
  }
  void InitPostgresPkColumns(MyPostgresConn * conn, MyPostgresSchema * schema,
                             MyPostgresTable * table);
  void InitPostgresPks(void *conn, MyPostgresSchema * schema,
                       MyPostgresTable * table);

  char *MapAuxPrepareSqlQuery(const char *db_prefix, const char *table,
                              const char *rowid);

  void OnQuit(wxCommandEvent & event);
  void OnClose(wxCloseEvent & event);
  void OnAbout(wxCommandEvent & event);
  void OnConnect(wxCommandEvent & event);
  void OnConnectReadOnly(wxCommandEvent & event);
  void OnCreateNew(wxCommandEvent & event);
  void OnDisconnect(wxCommandEvent & event);
  void OnMemoryDbLoad(wxCommandEvent & event);
  void OnMemoryDbNew(wxCommandEvent & event);
  void OnMemoryDbClock(wxCommandEvent & event);
  void OnMemoryDbSave(wxCommandEvent & event);
  void OnVacuum(wxCommandEvent & event);
  void OnPostgreSQL(wxCommandEvent & WXUNUSED(&event))
  {
    DoPostgreSqlConnection();
  }
  void OnMapPanel(wxCommandEvent & event);
  void OnSqlScript(wxCommandEvent & event);
  void OnQueryViewComposer(wxCommandEvent & event);
  void OnCharset(wxCommandEvent & event);
  void OnLoadShp(wxCommandEvent & event);
  void OnLoadGeoJSON(wxCommandEvent & event);
  void OnLoadTxt(wxCommandEvent & event);
  void OnLoadDbf(wxCommandEvent & event);
  void OnLoadXL(wxCommandEvent & event);
  void OnVirtualShp(wxCommandEvent & event);
  void OnVirtualGeoJSON(wxCommandEvent & event);
  void OnVirtualTxt(wxCommandEvent & event);
  void OnVirtualDbf(wxCommandEvent & event);
  void OnVirtualXL(wxCommandEvent & event);
  void OnNetwork(wxCommandEvent & event);
  void OnImportExifPhotos(wxCommandEvent & event);
  void OnImportGpsPhotos(wxCommandEvent & event);
  void OnImportXmlDocuments(wxCommandEvent & event);
  void OnImportWFS(wxCommandEvent & event);
  void OnImportDXF(wxCommandEvent & event);
  void OnSrids(wxCommandEvent & event);
  void OnAttachDatabase(wxCommandEvent & event);
  void OnSqlLog(wxCommandEvent & event);
  void OnDbStatus(wxCommandEvent & event);
  void OnCheckGeometries(wxCommandEvent & event);
  void OnSanitizeGeometries(wxCommandEvent & event);
  void OnProjLib(wxCommandEvent & event);
  void OnHelp(wxCommandEvent & event);
  void OnMouseMove(wxMouseEvent & event);
  void OnTimerAutoSave(wxTimerEvent & event);
};

class HelpDialog:public wxDialog
{
//
// the help dialog
//
private:
  MyFrame * MainFrame;
public:
  HelpDialog()
  {
    MainFrame = NULL;
  }
  HelpDialog(MyFrame * parent)
  {
    Create(parent);
  }
  bool Create(MyFrame * parent);
  virtual ~ HelpDialog()
  {;
  }
  void CreateControls();
  void OnCancel(wxCommandEvent & event);
  void OnClose(wxCloseEvent & event);
  void OnSize(wxSizeEvent & event);
};

class CloneTableDialog:public wxDialog
{
//
// the CloneTable dialog
//
private:
  MyFrame * MainFrame;
  wxString dbPrefix;
  wxString inTable;
public:
    CloneTableDialog()
  {
    MainFrame = NULL;
  }
  CloneTableDialog(MyFrame * parent, wxString & dbPrefix, wxString & inTable)
  {
    Create(parent, dbPrefix, inTable);
  }
  bool Create(MyFrame * parent, wxString & dbPrefix, wxString & inTable);
  virtual ~ CloneTableDialog()
  {;
  }
  void CreateControls();
  void GetSQL(wxString & sql);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class SearchSridDialog:public wxDialog
{
//
// a dialog preparing a Search SRID by name
//
private:
  MyFrame * MainFrame;
  wxString String;              // search string [name]
  int SRID;                     // search id [srid]
  bool SearchBySRID;
public:
    SearchSridDialog()
  {;
  }
  SearchSridDialog(MyFrame * parent);
  bool Create(MyFrame * parent);
  virtual ~ SearchSridDialog()
  {;
  }
  void CreateControls();
  wxString & GetString()
  {
    return String;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool IsSearchBySrid()
  {
    return SearchBySRID;
  }
  void OnOk(wxCommandEvent & event);
  void OnSearchBySrid(wxCommandEvent & event);
};

class SetSridDialog:public wxDialog
{
//
// a dialog preparing a SET SRID
//
private:
  MyFrame * MainFrame;
  wxString Table;               // the table's name 
  wxString Column;              // the column's name to be recovered
  int OldSRID;                  // SRID to substitute
  int SRID;                     // required SRID
public:
    SetSridDialog()
  {;
  }
  SetSridDialog(MyFrame * parent, wxString & table, wxString & column);
  bool Create(MyFrame * parent, wxString & table, wxString & column);
  virtual ~ SetSridDialog()
  {;
  }
  void CreateControls();
  int GetOldSRID()
  {
    return OldSRID;
  }
  int GetSRID()
  {
    return SRID;
  }
  void OnOk(wxCommandEvent & event);
};

class RecoverDialog:public wxDialog
{
//
// a dialog preparing a RECOVER GEOMETRY
//
private:
  MyFrame * MainFrame;
  wxString Table;               // the table's name 
  wxString Column;              // the column's name to be recovered
  int SRID;                     // required SRID
  wxString Type;                // required Geometry Type
  wxString Dimension;           // required CoordDimension
public:
    RecoverDialog()
  {;
  }
  RecoverDialog(MyFrame * parent, wxString & table, wxString & column);
  bool Create(MyFrame * parent, wxString & table, wxString & column);
  virtual ~ RecoverDialog()
  {;
  }
  void CreateControls();
  wxString & GetType()
  {
    return Type;
  }
  wxString & GetDimension()
  {
    return Dimension;
  }
  int GetSRID()
  {
    return SRID;
  }
  void OnOk(wxCommandEvent & event);
};

class ElementaryGeomsDialog:public wxDialog
{
//
// a dialog asking ElementaryGeoms args
//
private:
  MyFrame * MainFrame;
  wxString InTable;
  wxString Geometry;
  wxString OutTable;
  wxString PrimaryKey;
  wxString MultiID;
  wxString Type;
  int SRID;
  wxString CoordDims;
  bool SpatialIndex;
public:
    ElementaryGeomsDialog()
  {;
  }
  ElementaryGeomsDialog(MyFrame * parent, wxString & table, wxString & column);
  bool Create(MyFrame * parent, wxString & table, wxString & column);
  virtual ~ ElementaryGeomsDialog()
  {;
  }
  wxString & GetOutTable()
  {
    return OutTable;
  }
  wxString & GetPrimaryKey()
  {
    return PrimaryKey;
  }
  wxString & GetMultiID()
  {
    return MultiID;
  }
  wxString & GetType()
  {
    return Type;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetCoordDims()
  {
    return CoordDims;
  }
  bool IsSpatialIndex()
  {
    return SpatialIndex;
  }
  void CreateControls();
  void OnOk(wxCommandEvent & event);
};

class ColumnStatsDialog:public wxDialog
{
//
// a dialog showing Column Stats
//
private:
  MyFrame * MainFrame;
  wxString Table;               // the table's name 
  wxString Column;              // the column's name 
  int NullValues;
  int TextValues;
  int IntegerValues;
  int RealValues;
  int BlobValues;
  double Min;
  double Max;
  double Avg;
  double StdDevPop;
  double StdDevSamp;
  double VarPop;
  double VarSamp;
  int DistinctValues;
  void CleanDecimals(char *number);
public:
    ColumnStatsDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & table, wxString & column,
              int null_count, int text_count, int integer_count, int real_count,
              int blob_count, double min, double max, double avg,
              double stddev_pop, double stddev_samp, double var_pop,
              double var_samp, int distinct_values);
  virtual ~ ColumnStatsDialog()
  {;
  }
  void CreateControls();
  void OnShowChart(wxCommandEvent & event);
  void OnExit(wxCommandEvent & event);
};

class MyChartIntervalClass
{
// a Chart interval class
private:
  double Min;
  double Max;
  int Count;
public:
    MyChartIntervalClass()
  {;
  }
   ~MyChartIntervalClass()
  {;
  }
  void Create(double min, double max)
  {
    Min = min;
    Max = max;
    Count = 0;
  }
  double GetMin()
  {
    return Min;
  }
  double GetMax()
  {
    return Max;
  }
  void Add()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
};

class MyChartUniqueClass
{
// a Chart unique-value class
private:
  wxString Value;
  int Count;
  MyChartUniqueClass *Next;
public:
    MyChartUniqueClass(wxString & value, int count)
  {
    Value = value;
    Count = count;
    Next = NULL;
  }
   ~MyChartUniqueClass()
  {;
  }
  wxString & GetValue()
  {
    return Value;
  }
  int GetCount()
  {
    return Count;
  }
  void SetNext(MyChartUniqueClass * p)
  {
    Next = p;
  }
  MyChartUniqueClass *GetNext()
  {
    return Next;
  }
};

class MyChartData
{
// a container storing Chart data classes
private:
  bool Initialized;
  MyChartIntervalClass *Array;
  double Min;
  double Max;
  int MaxFreq;
  int TotFreq;
  MyChartUniqueClass *First;
  MyChartUniqueClass *Last;
  int MaxClasses;
  int NumClasses;
  int OtherUniquesFreq;
  int OtherUniquesCount;
  bool Valid;
  bool ByIntervals;
public:
    MyChartData();
   ~MyChartData();
  bool Create(int max_classes);
  bool Create(double min, double max, int classes);
  void Add(wxString & value, int count);
  void Add(double value);
  MyChartUniqueClass *GetFirst()
  {
    return First;
  }
  int GetMaxFreq()
  {
    return MaxFreq;
  }
  int GetTotFreq()
  {
    return TotFreq;
  }
  int GetNumClasses()
  {
    return NumClasses;
  }
  int GetOtherUniquesFreq()
  {
    return OtherUniquesFreq;
  }
  int GetOtherUniquesCount()
  {
    return OtherUniquesCount;
  }
  MyChartIntervalClass *GetClass(int idx);
  void SetValid()
  {
    Valid = true;
  }
  bool IsValid()
  {
    return Valid;
  }
  void CleanData();
  bool Check(bool by_intervals, int classes);
};

class MyChartScaleLabel
{
// a Chart Scale label
private:
  wxString Label;
  double Position;
  MyChartScaleLabel *Next;
public:
    MyChartScaleLabel(wxString & label, double pos)
  {
    Label = label;
    Position = pos;
    Next = NULL;
  }
   ~MyChartScaleLabel()
  {;
  }
  wxString & GetLabel()
  {
    return Label;
  }
  double GetPosition()
  {
    return Position;
  }
  void SetNext(MyChartScaleLabel * p)
  {
    Next = p;
  }
  MyChartScaleLabel *GetNext()
  {
    return Next;
  }
};

class MyPieChartLabel
{
// a PieChart label
private:
  wxString Label;
  double X;
  double Y;
  MyPieChartLabel *Next;
public:
    MyPieChartLabel(wxString & label, double x, double y)
  {
    Label = label;
    X = x;
    Y = y;
    Next = NULL;
  }
   ~MyPieChartLabel()
  {;
  }
  wxString & GetLabel()
  {
    return Label;
  }
  double GetX()
  {
    return X;
  }
  double GetY()
  {
    return Y;
  }
  void SetNext(MyPieChartLabel * p)
  {
    Next = p;
  }
  MyPieChartLabel *GetNext()
  {
    return Next;
  }
};

class MyChartScaleLabels
{
// a container storing Chart Scale labels
private:
  MyChartScaleLabel * First;
  MyChartScaleLabel *Last;
public:
    MyChartScaleLabels()
  {
    First = NULL;
    Last = NULL;
  }
   ~MyChartScaleLabels();
  void Initialize(double span, int max_freq);
  void Add(const char *label, double pos);
  MyChartScaleLabel *GetFirst()
  {
    return First;
  }
};

class MyPieChartLabels
{
// a container storing PieChart labels
private:
  MyPieChartLabel * First;
  MyPieChartLabel *Last;
  MyPieChartLabel **LeftLabels;
  int NumLeftLabels;
  MyPieChartLabel **RightLabels;
  int NumRightLabels;
public:
    MyPieChartLabels();
   ~MyPieChartLabels();
  void Add(const char *label, double x, double y);
  void Sort(double cx);
  int GetNumLeftLabels()
  {
    return NumLeftLabels;
  }
  MyPieChartLabel *GetLeftLabel(int idx);
  int GetNumRightLabels()
  {
    return NumRightLabels;
  }
  MyPieChartLabel *GetRightLabel(int idx);
};

class StatsChartDialog:public wxDialog
{
//
// a dialog generating a Stat Chart
//
private:
  MyFrame * MainFrame;
  wxString Table;               // the table's name 
  wxString Column;              // the column's name 
  bool NumericData;
  double Min;
  double Max;
  wxRadioBox *TypeCtrl;
  wxRadioBox *SizeCtrl;
  wxRadioBox *ModeCtrl;
  wxSpinCtrl *ClassCtrl;
  wxStaticBitmap *ChartShow;
  bool Histogram;
  bool LineChart;
  bool PieChart;
  bool ByInterval;
  int Classes;
  MyChartData ChartData;
  wxString ExportPath;
  enum Targets
  {
    CHART_TARGET_IS_PREVIEW,
    CHART_TARGET_IS_COPY,
    CHART_TARGET_IS_PNG,
    CHART_TARGET_IS_SVG,
    CHART_TARGET_IS_PDF
  };
public:
    StatsChartDialog()
  {;
  }
  bool Create(ColumnStatsDialog * parent, MyFrame * granny, wxString & table,
              wxString & column, bool numeric, double min, double max);
  virtual ~ StatsChartDialog()
  {;
  }
  void CreateControls();
  void CleanDecimals(char *number);
  void ReloadData();
  void UpdatePreview();
  void PrepareDataByInterval(int classes);
  void PrepareDataByUniqueValue(int classes);
  void DoIntervalHistogram(int hsize, int vsize, int target, int font_size);
  void DoIntervalLineChart(int hsize, int vsize, int target, int font_size);
  void DoIntervalPieChart(int hsize, int vsize, int target, int font_size);
  void DoUniqueHistogram(int hsize, int vsize, int target, int font_size);
  void DoUniqueLineChart(int hsize, int vsize, int target, int font_size);
  void DoUniquePieChart(int hsize, int vsize, int target, int font_size);
  void OnChartTypeChanged(wxCommandEvent & event);
  void OnChartModeChanged(wxCommandEvent & event);
  void OnChartClassesChanged(wxCommandEvent & event);
  void OnChartCopy(wxCommandEvent & event);
  void OnChartPng(wxCommandEvent & event);
  void OnChartSvg(wxCommandEvent & event);
  void OnChartPdf(wxCommandEvent & event);
  void OnExit(wxCommandEvent & event);
};

class MapPreviewDialog:public wxDialog
{
//
// a dialog generating a Map Preview
//
private:
  MyFrame * MainFrame;
  wxString Table;               // the table's name 
  wxString Column;              // the column's name 
  double MinX;
  double MinY;
  double MaxX;
  double MaxY;
  wxColour LineColor;
  wxColour FillColor;
  wxRadioBox *SizeCtrl;
  wxRadioBox *SymbolCtrl;
  wxRadioBox *FillCtrl;
  wxSpinCtrl *SymSizeCtrl;
  wxSpinCtrl *ThicknessCtrl;
  wxBitmapButton *LineColorCtrl;
  wxBitmapButton *FillColorCtrl;
  wxStaticBitmap *MapShow;
  wxString ExportPath;
  enum Targets
  {
    MAP_TARGET_IS_PREVIEW,
    MAP_TARGET_IS_COPY,
    MAP_TARGET_IS_PNG,
    MAP_TARGET_IS_SVG,
    MAP_TARGET_IS_PDF
  };
public:
    MapPreviewDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & table, wxString & column,
              double minx, double mixy, double maxx, double maxy);
  virtual ~ MapPreviewDialog()
  {;
  }
  void CreateControls();
  void UpdatePreview();
  void DoMap(int hsize, int vsize, int target);
  void GetButtonBitmap(wxColour & color, wxBitmap & bmp);
  void OnSizeChanged(wxCommandEvent & event);
  void OnSymbolTypeChanged(wxCommandEvent & event);
  void OnFillModeChanged(wxCommandEvent & event);
  void OnSymbolSizeChanged(wxCommandEvent & event);
  void OnLineThicknessChanged(wxCommandEvent & event);
  void OnLineColor(wxCommandEvent & event);
  void OnFillColor(wxCommandEvent & event);
  void OnMapCopy(wxCommandEvent & event);
  void OnMapPng(wxCommandEvent & event);
  void OnMapSvg(wxCommandEvent & event);
  void OnMapPdf(wxCommandEvent & event);
  void OnExit(wxCommandEvent & event);
};

class VirtualShpDialog:public wxDialog
{
//
// a dialog preparing a CREATE VIRTUAL SHAPE
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the SHP base path
  wxString Table;               // the table name
  wxString Default;             // the default charset
  wxString Charset;             // the SHP charset
  int SRID;                     // the SRID
  bool TextDates;
public:
    VirtualShpDialog()
  {;
  }
  VirtualShpDialog(MyFrame * parent, wxString & path, wxString & table,
                   int srid, wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table, int srid,
              wxString & defCs);
  virtual ~ VirtualShpDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  void OnOk(wxCommandEvent & event);
  void OnTextDates(wxCommandEvent & event);
};

class VirtualGeoJsonDialog:public wxDialog
{
//
// a dialog preparing a CREATE VIRTUAL GeoJSON
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the GeoJSON path
  wxString Table;               // the table name
  int SRID;                     // the SRID
  int ColnameCase;
public:
    VirtualGeoJsonDialog()
  {;
  }
  VirtualGeoJsonDialog(MyFrame * parent, wxString & path, wxString & table);
  bool Create(MyFrame * parent, wxString & path, wxString & table);
  virtual ~ VirtualGeoJsonDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  int GetSRID()
  {
    return SRID;
  }
  int GetColnameCase()
  {
    return ColnameCase;
  }
  void OnOk(wxCommandEvent & event);
};

class VirtualTxtDialog:public wxDialog
{
//
// a dialog preparing a CREATE VIRTUAL TEXT
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the CSV/TXT base path
  wxString Table;               // the table name
  wxString Default;             // the default charset
  wxString Charset;             // the CSV/TXT charset
  bool FirstLineTitles;         // TRUE if first line stores column titles
  char Separator;               // the character to be used as field separator
  char TextSeparator;           // the character to be used as text separator
  bool DecimalPointIsComma;     // TRUE if decimal separator is COMMA
public:
    VirtualTxtDialog()
  {;
  }
  VirtualTxtDialog(MyFrame * parent, wxString & path, wxString & table,
                   wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table,
              wxString & defCs);
  virtual ~ VirtualTxtDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  bool IsFirstLineTitles()
  {
    return FirstLineTitles;
  }
  char GetSeparator()
  {
    return Separator;
  }
  char GetTextSeparator()
  {
    return TextSeparator;
  }
  bool IsDecimalPointComma()
  {
    return DecimalPointIsComma;
  }
  void OnSeparator(wxCommandEvent & event);
  void OnDecimalSeparator(wxCommandEvent & event);
  void OnQuote(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class VirtualDbfDialog:public wxDialog
{
//
// a dialog preparing a CREATE VIRTUAL DBF
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the DBF path
  wxString Table;               // the table name
  wxString Default;             // the default charset
  wxString Charset;             // the DBF charset
  bool TextDates;
public:
    VirtualDbfDialog()
  {;
  }
  VirtualDbfDialog(MyFrame * parent, wxString & path, wxString & table,
                   wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table,
              wxString & defCs);
  virtual ~ VirtualDbfDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  void OnOk(wxCommandEvent & event);
  void OnTextDates(wxCommandEvent & event);
};

class LoadShpDialog:public wxDialog
{
//
// a dialog preparing a LOAD SHAPE
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the SHP base path
  wxString Table;               // the table's name to be created
  wxString Column;              // the column's name for Geometry
  wxString Default;             // the default charset
  wxString Charset;             // the SHP charset
  int SRID;                     // the SRID
  bool Coerce2D;                // coercing to 2D [x,y]
  bool Compressed;              // compressed geometries
  bool SpatialIndex;            // building the Spatial Index (or not)
  bool UserDefinedGType;        // mode: automatic / user defined Geometry Type
  wxString GeometryType;        // User Defined Geometry Type
  bool UserDefinedPKey;         // mode: automatic / user defined Primary Key
  wxString PKColumn;            // User Defined Primary Key 
  int PKCount;                  // # Primary Key Columns
  wxString *PKFields;           // array of Primary Key Columns
  wxString *PKFieldsEx;         // array of Primary Key Columns (full detail)
  bool TextDates;
  int ColnameCase;
  bool UpdateStatistics;
public:
    LoadShpDialog()
  {;
  }
  LoadShpDialog(MyFrame * parent, wxString & path, wxString & table, int srid,
                wxString & column, wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table, int srid,
              wxString & column, wxString & defCs);
  virtual ~ LoadShpDialog();
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetColumn()
  {
    return Column;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool ApplyCoertion2D()
  {
    return Coerce2D;
  }
  bool ApplyCompression()
  {
    return Compressed;
  }
  bool CreateSpatialIndex()
  {
    return SpatialIndex;
  }
  bool IsUserDefinedGType()
  {
    return UserDefinedGType;
  }
  wxString & GetGeometryType()
  {
    return GeometryType;
  }
  bool IsUserDefinedPKey()
  {
    return UserDefinedPKey;
  }
  wxString & GetPKColumn()
  {
    return PKColumn;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  int GetColnameCase()
  {
    return ColnameCase;
  }
  bool IsUpdateStatistics()
  {
    return UpdateStatistics;
  }
  void LoadPKFields();
  void OnOk(wxCommandEvent & event);
  void OnUserGType(wxCommandEvent & event);
  void OnUserPKey(wxCommandEvent & event);
  void OnTextDates(wxCommandEvent & event);
  void OnUpdateStatistics(wxCommandEvent & event);
};

class LoadZipShpDialog:public wxDialog
{
//
// a dialog preparing a LOAD SHAPE
//
private:
  MyFrame * MainFrame;
  wxString ZipPath;             // the Zipfile path
  wxString Basename;            // the SHP basename
  wxString Table;               // the table's name to be created
  wxString Column;              // the column's name for Geometry
  wxString Default;             // the default charset
  wxString Charset;             // the SHP charset
  int SRID;                     // the SRID
  bool Coerce2D;                // coercing to 2D [x,y]
  bool Compressed;              // compressed geometries
  bool SpatialIndex;            // building the Spatial Index (or not)
  bool UserDefinedGType;        // mode: automatic / user defined Geometry Type
  wxString GeometryType;        // User Defined Geometry Type
  bool UserDefinedPKey;         // mode: automatic / user defined Primary Key
  wxString PKColumn;            // User Defined Primary Key 
  int PKCount;                  // # Primary Key Columns
  wxString *PKFields;           // array of Primary Key Columns
  wxString *PKFieldsEx;         // array of Primary Key Columns (full detail)
  bool TextDates;
  int ColnameCase;
  bool UpdateStatistics;
public:
    LoadZipShpDialog()
  {;
  }
  LoadZipShpDialog(MyFrame * parent, const char *zip_path, const char *basename,
                   wxString & table, int srid, wxString & column,
                   wxString & defCs);
  bool Create(MyFrame * parent, const char *zip_path, const char *basename,
              wxString & table, int srid, wxString & column, wxString & defCs);
  virtual ~ LoadZipShpDialog();
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetColumn()
  {
    return Column;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool ApplyCoertion2D()
  {
    return Coerce2D;
  }
  bool ApplyCompression()
  {
    return Compressed;
  }
  bool CreateSpatialIndex()
  {
    return SpatialIndex;
  }
  bool IsUserDefinedGType()
  {
    return UserDefinedGType;
  }
  wxString & GetGeometryType()
  {
    return GeometryType;
  }
  bool IsUserDefinedPKey()
  {
    return UserDefinedPKey;
  }
  wxString & GetPKColumn()
  {
    return PKColumn;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  int GetColnameCase()
  {
    return ColnameCase;
  }
  bool IsUpdateStatistics()
  {
    return UpdateStatistics;
  }
  void LoadPKFields();
  void OnOk(wxCommandEvent & event);
  void OnUserGType(wxCommandEvent & event);
  void OnUserPKey(wxCommandEvent & event);
  void OnTextDates(wxCommandEvent & event);
  void OnUpdateStatistics(wxCommandEvent & event);
};

class ChoooseZipShpDialog:public wxDialog
{
//
// a dialog preparing a LOAD SHAPE from Zipfile
//
private:
  MyFrame * MainFrame;
  const char *ZipPath;          // the Zipfile path
  int Count;                    // number of Shapefiles
  char **Shapefiles;            // array of Shapefile's basenames
  const char *Basename;         // the selected Basename
public:
    ChoooseZipShpDialog()
  {;
  }
  ChoooseZipShpDialog(MyFrame * parent, const char *zip_path, int count);
  bool Create(MyFrame * parent, const char *zip_path, int count);
  virtual ~ ChoooseZipShpDialog();
  void CreateControls();
  const char *GetBasename()
  {
    return Basename;
  }
  void OnOk(wxCommandEvent & event);
};

class ChoooseZipDbfDialog:public wxDialog
{
//
// a dialog preparing a LOAD DBF file from Zipfile
//
private:
  MyFrame * MainFrame;
  const char *ZipPath;          // the Zipfile path
  int Count;                    // number of Shapefiles
  char **DBFs;                  // array of DBF's filenames
  const char *Filename;         // the selected filename
public:
    ChoooseZipDbfDialog()
  {;
  }
  ChoooseZipDbfDialog(MyFrame * parent, const char *zip_path, int count);
  bool Create(MyFrame * parent, const char *zip_path, int count);
  virtual ~ ChoooseZipDbfDialog();
  void CreateControls();
  const char *GetFilename()
  {
    return Filename;
  }
  void OnOk(wxCommandEvent & event);
};

class LoadGeoJsonDialog:public wxDialog
{
//
// a dialog preparing a LOAD GeoJSON
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the SHP base path
  wxString Table;               // the table's name to be created
  wxString Column;              // the column's name for Geometry
  int SRID;                     // the SRID
  bool SpatialIndex;            // building the Spatial Index (or not)
  bool UserDefinedGType;        // mode: automatic / user defined Geometry Type
  int ColnameCase;
  bool UpdateStatistics;
public:
    LoadGeoJsonDialog()
  {;
  }
  LoadGeoJsonDialog(MyFrame * parent, wxString & path, wxString & table,
                    int srid, wxString & column);
  bool Create(MyFrame * parent, wxString & path, wxString & table, int srid,
              wxString & column);
  virtual ~ LoadGeoJsonDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetColumn()
  {
    return Column;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool CreateSpatialIndex()
  {
    return SpatialIndex;
  }
  int GetColnameCase()
  {
    return ColnameCase;
  }
  bool IsUpdateStatistics()
  {
    return UpdateStatistics;
  }
  void OnOk(wxCommandEvent & event);
  void OnUpdateStatistics(wxCommandEvent & event);
};

class DumpPostGISDialog:public wxDialog
{
//
// a dialog preparing a SQL DUMP for PostGIS
//
private:
  MyFrame * MainFrame;
  wxString SchemaName;          // the PostGIS target schema
  wxString TableName;           // the PostGIS table name
  bool Lowercase;               // column-names to lowercase
  bool CreateTable;             // creating (or not) the PostGIS table
  bool SpatialIndex;            // creating (or not) the PostGIS Spatial Index
public:
    DumpPostGISDialog()
  {;
  }
  DumpPostGISDialog(MyFrame * parent, wxString & table);
  bool Create(MyFrame * parent, wxString & table);
  virtual ~ DumpPostGISDialog()
  {;
  }
  void CreateControls();
  wxString & GetSchemaName()
  {
    return SchemaName;
  }
  wxString & GetTableName()
  {
    return TableName;
  }
  bool IsLowercase()
  {
    return Lowercase;
  }
  bool IsCreateTable()
  {
    return CreateTable;
  }
  bool IsSpatialIndex()
  {
    return SpatialIndex;
  }
  void OnLowercase(wxCommandEvent & event);
  void OnCreateTable(wxCommandEvent & event);
  void OnSpatialIndex(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class DumpShpDialog:public wxDialog
{
//
// a dialog preparing a DUMP SHAPE
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the SHP base path
  wxString Table;               // the table's name to be created
  wxString Column;              // the column's name for Geometry
  wxString Default;             // the default charset
  wxString Charset;             // the SHP charset
public:
    DumpShpDialog()
  {;
  }
  DumpShpDialog(MyFrame * parent, wxString & path, wxString & table,
                wxString & column, wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table,
              wxString & column, wxString & defCs);
  virtual ~ DumpShpDialog()
  {;
  }
  void CreateControls();
  wxString & GetCharset()
  {
    return Charset;
  }
  void OnOk(wxCommandEvent & event);
};

class LoadXLDialog:public wxDialog
{
//
// a dialog preparing a LOAD XL
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the XLS path
  wxString Table;               // the table name
  wxString *Worksheets;         // Worksheet array
  int WorksheetCount;           // array items
  int WorksheetIndex;           // selected Worksheet Index
  bool FirstLineTitles;         // first line contains column names
  bool Invalid;
  void GetWorksheets();
public:
    LoadXLDialog()
  {;
  }
  LoadXLDialog(MyFrame * parent, wxString & path, wxString & table);
  bool Create(MyFrame * parent, wxString & path, wxString & table);
  virtual ~ LoadXLDialog()
  {
    if (Worksheets != NULL)
      delete[]Worksheets;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  int GetWorksheetIndex()
  {
    return WorksheetIndex;
  }
  bool IsFirstLineTitles()
  {
    return FirstLineTitles;
  }
  void OnOk(wxCommandEvent & event);
};

class VirtualXLDialog:public wxDialog
{
//
// a dialog preparing a CREATE VIRTUAL XL
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the XLS path
  wxString Table;               // the table name
  wxString *Worksheets;         // Worksheet array
  int WorksheetCount;           // array items
  int WorksheetIndex;           // selected Worksheet Index
  bool FirstLineTitles;         // first line contains column names
  bool Invalid;
  void GetWorksheets();
public:
    VirtualXLDialog()
  {;
  }
  VirtualXLDialog(MyFrame * parent, wxString & path, wxString & table);
  bool Create(MyFrame * parent, wxString & path, wxString & table);
  virtual ~ VirtualXLDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  int GetWorksheetIndex()
  {
    return WorksheetIndex;
  }
  bool IsFirstLineTitles()
  {
    return FirstLineTitles;
  }
  void OnOk(wxCommandEvent & event);
};

class LoadTxtDialog:public wxDialog
{
//
// a dialog preparing a LOAD TXT/CSV
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the CSV/TXT base path
  wxString Table;               // the table name
  wxString Default;             // the default charset
  wxString Charset;             // the CSV/TXT charset
  bool FirstLineTitles;         // TRUE if first line stores column titles
  char Separator;               // the character to be used as field separator
  char TextSeparator;           // the character to be used as text separator
  bool DecimalPointIsComma;     // TRUE if decimal separator is COMMA
public:
    LoadTxtDialog()
  {;
  }
  LoadTxtDialog(MyFrame * parent, wxString & path, wxString & table,
                wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table,
              wxString & defCs);
  virtual ~ LoadTxtDialog()
  {;
  }
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  bool IsFirstLineTitles()
  {
    return FirstLineTitles;
  }
  char GetSeparator()
  {
    return Separator;
  }
  char GetTextSeparator()
  {
    return TextSeparator;
  }
  bool IsDecimalPointComma()
  {
    return DecimalPointIsComma;
  }
  void OnSeparator(wxCommandEvent & event);
  void OnDecimalSeparator(wxCommandEvent & event);
  void OnQuote(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class DumpTxtDialog:public wxDialog
{
//
// a dialog preparing a DUMP generic text
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the SHP base path
  wxString Default;             // the default charset
  wxString Charset;             // the target charset
public:
    DumpTxtDialog()
  {;
  }
  DumpTxtDialog(MyFrame * parent, wxString & path, wxString & target,
                wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & target,
              wxString & defCs);
  virtual ~ DumpTxtDialog()
  {;
  }
  void CreateControls();
  wxString & GetCharset()
  {
    return Charset;
  }
  void OnOk(wxCommandEvent & event);
};

class LoadDbfDialog:public wxDialog
{
//
// a dialog preparing a LOAD DBF
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the DBF base path
  wxString Table;               // the table name
  wxString Default;             // the default charset
  wxString Charset;             // the DBF charset
  bool UserDefinedPKey;         // mode: automatic / user defined Primary Key
  wxString PKColumn;            // User Defined Primary Key 
  int PKCount;                  // # Primary Key Columns
  wxString *PKFields;           // array of Primary Key Columns
  wxString *PKFieldsEx;         // array of Primary Key Columns (full detail)
  bool TextDates;
  int ColnameCase;
public:
    LoadDbfDialog()
  {;
  }
  LoadDbfDialog(MyFrame * parent, wxString & path, wxString & table,
                wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & table,
              wxString & defCs);
  virtual ~ LoadDbfDialog();
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  bool IsUserDefinedPKey()
  {
    return UserDefinedPKey;
  }
  wxString & GetPKColumn()
  {
    return PKColumn;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  int GetColnameCase()
  {
    return ColnameCase;
  }
  void LoadPKFields();
  void OnOk(wxCommandEvent & event);
  void OnUserPKey(wxCommandEvent & event);
  void OnTextDates(wxCommandEvent & event);
};

class LoadZipDbfDialog:public wxDialog
{
//
// a dialog preparing a LOAD DBF (from Zipfile)
//
private:
  MyFrame * MainFrame;
  wxString ZipPath;             // the Zipfile path
  wxString Filename;            // the DBF filename
  wxString Table;               // the table name
  wxString Default;             // the default charset
  wxString Charset;             // the DBF charset
  bool UserDefinedPKey;         // mode: automatic / user defined Primary Key
  wxString PKColumn;            // User Defined Primary Key 
  int PKCount;                  // # Primary Key Columns
  wxString *PKFields;           // array of Primary Key Columns
  wxString *PKFieldsEx;         // array of Primary Key Columns (full detail)
  bool TextDates;
  int ColnameCase;
public:
    LoadZipDbfDialog()
  {;
  }
  LoadZipDbfDialog(MyFrame * parent, wxString & zip_path, wxString & filename,
                   wxString & table, wxString & defCs);
  bool Create(MyFrame * parent, wxString & zip_path, wxString & filename,
              wxString & table, wxString & defCs);
  virtual ~ LoadZipDbfDialog();
  void CreateControls();
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetCharset()
  {
    return Charset;
  }
  bool IsUserDefinedPKey()
  {
    return UserDefinedPKey;
  }
  wxString & GetPKColumn()
  {
    return PKColumn;
  }
  bool IsTextDates()
  {
    return TextDates;
  }
  int GetColnameCase()
  {
    return ColnameCase;
  }
  void LoadPKFields();
  void OnOk(wxCommandEvent & event);
  void OnUserPKey(wxCommandEvent & event);
  void OnTextDates(wxCommandEvent & event);
};

class NetworkDialog:public wxDialog
{
//
// a dialog preparing a BUILD NETWORK
//
private:
  MyFrame * MainFrame;
  wxString TableName;           // the table name
  wxString FromColumn;          // the NodeFrom column name
  wxString ToColumn;            // the NodeTo column name
  bool NoGeometry;              // no Geometry column
  wxString GeomColumn;          // the Geometry column name
  bool GeomLength;              // Cost is Geometry Length
  wxString CostColumn;          // the Cost column name
  bool Bidirectional;           // Bidirectional arcs
  bool OneWays;                 // OneWays columns supported
  wxString OneWayToFrom;        // the OneWay To-From column
  wxString OneWayFromTo;        // the OneWay From-To column
  bool NameEnabled;             // Name column supported
  wxString NameColumn;          // the Name column name
  bool AStarSupported;          // A* algorithm supported
  wxString DataTable;           // name of the binary data table
  wxString VirtualTable;        // name of the VirtualNetwork table
  bool Overwrite;               // enabled to overwrite existing tables
  void SetCurrentTable();
public:
    NetworkDialog()
  {;
  }
  NetworkDialog(MyFrame * parent);
  bool Create(MyFrame * parent);
  virtual ~ NetworkDialog()
  {;
  }
  void CreateControls();
  wxString & GetTableName()
  {
    return TableName;
  }
  wxString & GetFromColumn()
  {
    return FromColumn;
  }
  wxString & GetToColumn()
  {
    return ToColumn;
  }
  bool IsNoGeometry()
  {
    return NoGeometry;
  }
  wxString & GetGeomColumn()
  {
    return GeomColumn;
  }
  wxString & GetNameColumn()
  {
    return NameColumn;
  }
  bool IsGeomLength()
  {
    return GeomLength;
  }
  wxString & GetCostColumn()
  {
    return CostColumn;
  }
  bool IsBidirectional()
  {
    return Bidirectional;
  }
  bool IsOneWays()
  {
    return OneWays;
  }
  wxString & GetOneWayFromTo()
  {
    return OneWayFromTo;
  }
  wxString & GetOneWayToFrom()
  {
    return OneWayToFrom;
  }
  bool IsNameEnabled()
  {
    return NameEnabled;
  }
  bool IsAStarSupported()
  {
    return AStarSupported;
  }
  wxString & GetDataTable()
  {
    return DataTable;
  }
  wxString & GetVirtualTable()
  {
    return VirtualTable;
  }
  bool IsOverwriteEnabled()
  {
    return Overwrite;
  }
  void OnTable(wxCommandEvent & event);
  void OnDirection(wxCommandEvent & event);
  void OnCost(wxCommandEvent & event);
  void OnOneWay(wxCommandEvent & event);
  void OnNameEnabled(wxCommandEvent & event);
  void OnNoGeometry(wxCommandEvent & event);
  void OnOverwrite(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class ExifDialog:public wxDialog
{
//
// a dialog preparing an IMPORT EXIF PHOTOS
//
private:
  MyFrame * MainFrame;
  wxString ImgPath;             // the file name
  wxString DirPath;             // the folder path
  bool Folder;                  // import a whole folder
  bool Metadata;                // feed Metadata tables
  bool GpsOnly;                 // import only if GpsExif present
public:
    ExifDialog()
  {;
  }
  ExifDialog(MyFrame * parent, wxString & dir_path, wxString & img_path);
  bool Create(MyFrame * parent, wxString & dir_path, wxString & img_path);
  virtual ~ ExifDialog()
  {;
  }
  void CreateControls();
  wxString & GetImgPath()
  {
    return ImgPath;
  }
  wxString & GetDirPath()
  {
    return DirPath;
  }
  bool IsFolder()
  {
    return Folder;
  }
  bool IsMetadata()
  {
    return Metadata;
  }
  bool IsGpsOnly()
  {
    return GpsOnly;
  }
  void OnFolder(wxCommandEvent & event);
  void OnMetadata(wxCommandEvent & event);
  void OnGpsOnly(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class GpsPicsDialog:public wxDialog
{
//
// a dialog preparing an IMPORT GPS PHOTOS
//
private:
  MyFrame * MainFrame;
  wxString ImgPath;             // the file name
  wxString DirPath;             // the folder path
  wxString Table;               // the DB Table name
  wxString Geometry;            // the Geometry column name
  bool Folder;                  // import a whole folder
  bool UpdateStatistics;        // immediately updating layer statistics
  bool SpatialIndex;            // building a SpatialIndex
public:
    GpsPicsDialog()
  {;
  }
  GpsPicsDialog(MyFrame * parent, wxString & dir_path, wxString & img_path);
  bool Create(MyFrame * parent, wxString & dir_path, wxString & img_path);
  virtual ~ GpsPicsDialog()
  {;
  }
  void CreateControls();
  wxString & GetImgPath()
  {
    return ImgPath;
  }
  wxString & GetDirPath()
  {
    return DirPath;
  }
  wxString & GetTable()
  {
    return Table;
  }
  wxString & GetGeometry()
  {
    return Geometry;
  }
  bool IsFolder()
  {
    return Folder;
  }
  bool IsUpdateStatistics()
  {
    return UpdateStatistics;
  }
  bool IsSpatialIndex()
  {
    return SpatialIndex;
  }
  void OnFolder(wxCommandEvent & event);
  void OnSpatialIndex(wxCommandEvent & event);
  void OnUpdateStatistics(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class XmlDocumentsDialog:public wxDialog
{
//
// a dialog preparing an IMPORT XML DOCUMENTS
//
private:
  MyFrame * MainFrame;
  wxString XmlPath;             // the file name
  wxString DirPath;             // the folder path
  wxString Suffix;              // the optional file suffix
  wxString TargetTable;         // the target Table
  wxString PkName;              // the Primary Key name
  wxString XmlColumn;           // the XML Payload Column
  wxString SchemaUriColumn;     // the SchemaURI Column
  wxString InPathColumn;        // the InPath Column
  wxString ParseErrorColumn;    // the XmlParseError Column
  wxString ValidateErrorColumn; // the XmlValidateError Column
  bool Folder;                  // import a whole folder
  bool Compressed;              // compressed XmlBLOB
  bool Validated;               // apply Schema Validation
  bool InternalSchema;          // apply the Internally declared Schema URI
  bool OkSuffix;                // apply suffix restriction
  bool OkSchemaColumn;          // create a "schemaURI" column
  bool OkInPathColumn;          // create an "inPath" column
  bool OkParseErrorColumn;      // create the "XmlParseError" column
  bool OkValidateErrorColumn;   // create the "XmlSchemaValidateError" column
  wxString SchemaURI;           // the Schema URI for validation
public:
    XmlDocumentsDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & dir_path, wxString & xml_path);
  virtual ~ XmlDocumentsDialog()
  {;
  }
  void CreateControls();
  wxString & GetXmlPath()
  {
    return XmlPath;
  }
  wxString & GetDirPath()
  {
    return DirPath;
  }
  wxString & GetSuffix()
  {
    return Suffix;
  }
  wxString & GetTargetTable()
  {
    return TargetTable;
  }
  wxString & GetPkName()
  {
    return PkName;
  }
  wxString & GetXmlColumn()
  {
    return XmlColumn;
  }
  bool IsFolder()
  {
    return Folder;
  }
  bool IsCompressed()
  {
    return Compressed;
  }
  bool IsInternalSchemaURI()
  {
    return InternalSchema;
  }
  wxString & GetSchemaURI()
  {
    return SchemaURI;
  }
  wxString & GetSchemaUriColumn()
  {
    return SchemaUriColumn;
  }
  wxString & GetInPathColumn()
  {
    return InPathColumn;
  }
  wxString & GetParseErrorColumn()
  {
    return ParseErrorColumn;
  }
  wxString & GetValidateErrorColumn()
  {
    return ValidateErrorColumn;
  }
  void OnFolder(wxCommandEvent & event);
  void OnSuffixChanged(wxCommandEvent & event);
  void OnCompressionChanged(wxCommandEvent & event);
  void OnValidationChanged(wxCommandEvent & event);
  void OnInternalSchemaChanged(wxCommandEvent & event);
  void OnSchemaColumnChanged(wxCommandEvent & event);
  void OnInPathColumnChanged(wxCommandEvent & event);
  void OnParseErrorColumnChanged(wxCommandEvent & event);
  void OnValidateErrorColumnChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class DxfDialog:public wxDialog
{
//
// a dialog preparing an IMPORT DXF FILE(s)
//
private:
  MyFrame * MainFrame;
  wxString DxfPath;             // the file name
  wxString DirPath;             // the folder path
  wxString Prefix;              // the optional table-name prefix
  wxString SingleLayer;         // filtering a single DXF layer by its name
  int SRID;                     // the SRID to be applied
  bool Folder;                  // import a whole folder
  bool OkPrefix;                // apply table-name prefix
  bool OkSingle;                // apply single layer filter
  bool Force2D;                 // always forcing 2D
  bool Force3D;                 // always forcing 3D
  bool LinkedRings;             // special - linked rings
  bool UnlinkedRings;           // special - unlinked rings
  bool ImportMixed;             // mixed layers mode
  bool AppendMode;              // append mode
public:
    DxfDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & dir_path, wxString & dxf_path);
  virtual ~ DxfDialog()
  {;
  }
  void CreateControls();
  wxString & GetDxfPath()
  {
    return DxfPath;
  }
  wxString & GetDirPath()
  {
    return DirPath;
  }
  wxString & GetPrefix()
  {
    return Prefix;
  }
  wxString & GetSingleLayer()
  {
    return SingleLayer;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool IsFolder()
  {
    return Folder;
  }
  bool IsForce2D()
  {
    return Force2D;
  }
  bool IsForce3D()
  {
    return Force3D;
  }
  bool IsLinkedRings()
  {
    return LinkedRings;
  }
  bool IsUnlinkedRings()
  {
    return UnlinkedRings;
  }
  bool IsImportMixed()
  {
    return ImportMixed;
  }
  bool IsAppendMode()
  {
    return AppendMode;
  }
  void OnFolder(wxCommandEvent & event);
  void OnPrefixChanged(wxCommandEvent & event);
  void OnSingleLayerChanged(wxCommandEvent & event);
  void OnDimensionChanged(wxCommandEvent & event);
  void OnModeChanged(wxCommandEvent & event);
  void OnRingsChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class CreateRasterCoverageDialog:public wxDialog
{
//
// a dialog preparing an IMPORT DXF FILE(s)
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  int SampleType;
  int PixelType;
  int NumBands;
  int RedBand;
  int GreenBand;
  int BlueBand;
  int NIRband;
  bool AutoNDVI;
  int Compression;
  int Quality;
  int TileWidth;
  int TileHeight;
  bool NotGeoreferenced;
  int SRID;
  double HorzResolution;
  double VertResolution;
  wxString NoData;
  bool StrictResolution;
  bool MixedResolutions;
  bool InputPaths;
  bool MD5;
  bool Summary;
  bool Queryable;
  bool IsValidNoData(wxString & no_data, int sample, int num_bands);
public:
    CreateRasterCoverageDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ CreateRasterCoverageDialog()
  {;
  }
  void CreateControls();
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  int GetSampleType()
  {
    return SampleType;
  }
  int GetPixelType()
  {
    return PixelType;
  }
  int GetNumBands()
  {
    return NumBands;
  }
  int GetRedBand()
  {
    return RedBand;
  }
  int GetGreenBand()
  {
    return GreenBand;
  }
  int GetBlueBand()
  {
    return BlueBand;
  }
  int GetNIRband()
  {
    return NIRband;
  }
  bool IsAutoNDVI()
  {
    return AutoNDVI;
  }
  int GetCompression()
  {
    return Compression;
  }
  int GetQuality()
  {
    return Quality;
  }
  int GetTileWidth()
  {
    return TileWidth;
  }
  int GetTileHeight()
  {
    return TileHeight;
  }
  bool IsNotGeoreferenced()
  {
    return NotGeoreferenced;
  }
  int GetSRID()
  {
    return SRID;
  }
  double GetHorzResolution()
  {
    return HorzResolution;
  }
  double GetVertResolution()
  {
    return VertResolution;
  }
  wxString & GetNoData()
  {
    return NoData;
  }
  bool IsStrictResolution()
  {
    return StrictResolution;
  }
  bool IsMixedResolutions()
  {
    return MixedResolutions;
  }
  bool IsInputPaths()
  {
    return InputPaths;
  }
  bool IsMD5()
  {
    return MD5;
  }
  bool IsSummary()
  {
    return Summary;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  void OnSampleChanged(wxCommandEvent & event);
  void OnPixelChanged(wxCommandEvent & event);
  void OnCompressionChanged(wxCommandEvent & event);
  void OnNumBandsChanged(wxCommandEvent & event);
  void OnRedBandChanged(wxCommandEvent & event);
  void OnGreenBandChanged(wxCommandEvent & event);
  void OnBlueBandChanged(wxCommandEvent & event);
  void OnNIRbandChanged(wxCommandEvent & event);
  void OnAutoNDVIchanged(wxCommandEvent & event);
  void OnSquareTileChanged(wxCommandEvent & event);
  void OnNoGeorefChanged(wxCommandEvent & event);
  void OnSameResChanged(wxCommandEvent & event);
  void OnTileWidthChanged(wxCommandEvent & event);
  void OnTileHeightChanged(wxCommandEvent & event);
  void OnHorzResChanged(wxCommandEvent & event);
  void OnStrictChanged(wxCommandEvent & event);
  void OnMixedChanged(wxCommandEvent & event);
  void OnPathsChanged(wxCommandEvent & event);
  void OnMD5Changed(wxCommandEvent & event);
  void OnSummaryChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class SqlScriptDialog:public wxDialog
{
//
// a dialog preparing an SQL SCRIPT execute
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the SHP base path
  wxString Default;             // the default charset
  wxString Charset;             // the target charset
public:
    SqlScriptDialog()
  {;
  }
  SqlScriptDialog(MyFrame * parent, wxString & path, wxString & defCs);
  bool Create(MyFrame * parent, wxString & path, wxString & defCs);
  virtual ~ SqlScriptDialog()
  {;
  }
  void CreateControls();
  wxString & GetCharset()
  {
    return Charset;
  }
  void OnOk(wxCommandEvent & event);
};

class DefaultCharsetDialog:public wxDialog
{
//
// a dialog for selecting DEFAULT CHARSET
//
private:
  MyFrame * MainFrame;
  wxString Charset;             // the default charset
  bool AskCharset;              // true / false
public:
    DefaultCharsetDialog()
  {;
  }
  DefaultCharsetDialog(MyFrame * parent, wxString & charset, bool ask);
  bool Create(MyFrame * parent, wxString & charset, bool ask);
  virtual ~ DefaultCharsetDialog()
  {;
  }
  void CreateControls();
  wxString & GetCharset()
  {
    return Charset;
  }
  bool IsSetAskCharset()
  {
    return AskCharset;
  }
  void OnOk(wxCommandEvent & event);
};

class BlobExplorerDialog:public wxPropertySheetDialog
{
//
// a dialog to explore a BLOB value
//
private:
  MyFrame * MainFrame;
  int BlobSize;                 // the BLOB size
  unsigned char *Blob;          // the BLOB value
  int BlobType;                 // the BLOB type
  bool IsTextFont;
  wxString FontFamily;
  wxString FontStyle;
  bool IsFontBold;
  bool IsFontItalic;
  bool IsSVG;
  double SvgWidth;
  double SvgHeight;
  int SvgSize;
  gaiaGeomCollPtr Geometry;     // the geometry [optional]
  wxString XMLDocument;         // the XMLDocument [optional]
  wxString XMLIndented;         // the XMLDocument (indented) [optional]
  wxImage *Image;               // the image [optional]
  wxBitmap GeomPreview;         // the geometry preview
  wxString WKTstring;           // the WKT Geometry notation
  wxString EWKTstring;          // the EWKT Geometry notation
  wxString SVGstring;           // the SVG Geometry notation
  wxString KMLstring;           // the KML Geometry notation
  wxString GMLstring;           // the GML Geometry notation
  wxString GeoJSONstring;       // the GeoJSON Geometry notation
  bool SVGrelative;             // SVG relative / absolute mode
  int SVGprecision;             // SVG precision
  int KMLprecision;             // KML precision
  bool GMLv2v3;                 // GML version (v2 / v3)
  int GMLprecision;             // GML precision
  int GeoJSONoptions;           // GeoJSON options
  int GeoJSONprecision;         // GeoJSON precision
  void FormatWKT(wxTextCtrl * txtCtrl, wxString & in, wxString & out);
  void FormatSVG(wxTextCtrl * txtCtrl, wxString & in, wxString & out);
public:
    BlobExplorerDialog()
  {;
  }
  BlobExplorerDialog(MyFrame * parent, int blob_size, unsigned char *blob);
  bool Create(MyFrame * parent, int blob_size, unsigned char *blob);
  virtual ~ BlobExplorerDialog()
  {
    if (Geometry)
      gaiaFreeGeomColl(Geometry);
    if (Image)
      delete Image;
  }
  void DrawGeometry(int horz, int vert);
  wxPanel *CreateHexadecimalPage(wxWindow * book);
  wxPanel *CreateGeometryPage(wxWindow * book);
  wxPanel *CreateWKTPage(wxWindow * book);
  wxPanel *CreateEWKTPage(wxWindow * book);
  wxPanel *CreateSVGPage(wxWindow * book);
  wxPanel *CreateKMLPage(wxWindow * book);
  wxPanel *CreateGMLPage(wxWindow * book);
  wxPanel *CreateGeoJSONPage(wxWindow * book);
  wxPanel *CreateImagePage(wxWindow * book);
  wxPanel *CreateXmlDocumentPage(wxWindow * book);
  wxPanel *CreateXmlIndentedPage(wxWindow * book);
  void UpdateHexadecimalPage();
  void UpdateGeometryPage();
  void UpdateImagePage();
  void UpdateXmlDocumentPage();
  void UpdateXmlIndentedPage();
  void UpdateWKTPage();
  void UpdateEWKTPage();
  void UpdateSVGPage();
  void UpdateKMLPage();
  void UpdateGMLPage();
  void UpdateGeoJSONPage();
  gaiaGeomCollPtr GetGeometry()
  {
    return Geometry;
  }
  wxImage *GetImage()
  {
    return Image;
  }
  int GetBlobType()
  {
    return BlobType;
  }
  void OnOk(wxCommandEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCopyWKT(wxCommandEvent & event);
  void OnCopyEWKT(wxCommandEvent & event);
  void OnSVGRelative(wxCommandEvent & event);
  void OnSVGPrecision(wxCommandEvent & event);
  void OnCopySVG(wxCommandEvent & event);
  void OnKMLPrecision(wxCommandEvent & event);
  void OnCopyKML(wxCommandEvent & event);
  void OnGMLv2v3(wxCommandEvent & event);
  void OnGMLPrecision(wxCommandEvent & event);
  void OnCopyGML(wxCommandEvent & event);
  void OnGeoJSONOptions(wxCommandEvent & event);
  void OnGeoJSONPrecision(wxCommandEvent & event);
  void OnCopyGeoJSON(wxCommandEvent & event);
  void OnCopyXmlDocument(wxCommandEvent & event);
  void OnCopyXmlIndented(wxCommandEvent & event);
};

class TilePreviewDialog:public wxDialog
{
//
// a dialog to show a Raster Tile Preview
//
private:
  MyFrame * MainFrame;
  wxString DbPrefix;
  wxString CoverageTable;
  int TileId;
  wxImage *Image;               // the image
  bool Painted;
public:
    TilePreviewDialog()
  {;
  }
  TilePreviewDialog(MyFrame * parent, wxString & db_prefix, wxString & coverage,
                    int tile_id, int blob_size, unsigned char *blob);
  bool Create(MyFrame * parent, wxString & db_prefix, wxString & coverage,
              int tile_id, int blob_size, unsigned char *blob);
  virtual ~ TilePreviewDialog()
  {
    if (Image)
      delete Image;
  }
  void CreateControls();
  void OnPaint(wxPaintEvent & event);
  void OnOk(wxCommandEvent & event);
};

class GraphicsGeometry:public wxStaticBitmap
{
//
// a window to show some Geometry in a graphical fashion
//
private:
  BlobExplorerDialog * Parent;
public:
  GraphicsGeometry(BlobExplorerDialog * parent, wxWindow * panel, wxWindowID id,
                   const wxBitmap & bmp, wxSize const &size);
    virtual ~ GraphicsGeometry()
  {;
  }
};

class ImageShow:public wxStaticBitmap
{
//
// a window to show some Image [Jpeg-Png-Gif]
//
private:
  BlobExplorerDialog * Parent;
public:
  ImageShow(BlobExplorerDialog * parent, wxWindow * panel, wxWindowID id,
            const wxBitmap & bmp, const wxSize & size);
    virtual ~ ImageShow()
  {;
  }
  void OnRightClick(wxMouseEvent & event);
  void OnCmdCopy(wxCommandEvent & event);
};

class MyHexList:public wxListCtrl
{
//
// a class for Hexdecimal dumps
//
private:
  BlobExplorerDialog * Parent;
  int BlobSize;                 // the BLOB size
  unsigned char *Blob;          // the BLOB value
public:
    MyHexList(BlobExplorerDialog * parent, unsigned char *blob,
              int blob_size, wxWindow * panel, wxWindowID id,
              const wxPoint & pos = wxDefaultPosition, const wxSize & size =
              wxDefaultSize, long style = 0);
    virtual ~ MyHexList();
  virtual wxString OnGetItemText(long item, long column) const;
};

class AutoFDOTable
{
private:
  char *Name;
  AutoFDOTable *Next;
public:
    AutoFDOTable(const char *name, const int len)
  {
    Name = new char[len + 1];
      strcpy(Name, name);
      Next = NULL;
  }
   ~AutoFDOTable()
  {
    if (Name)
      delete[]Name;
  }
  char *GetName()
  {
    return Name;
  }
  void SetNext(AutoFDOTable * next)
  {
    Next = next;
  }
  AutoFDOTable *GetNext()
  {
    return Next;
  }
};

class AutoFDOTables
{
private:
  AutoFDOTable * First;
  AutoFDOTable *Last;
public:
    AutoFDOTables()
  {
    First = NULL;
    Last = NULL;
  }
   ~AutoFDOTables();
  void Add(const char *name, const int len);
  AutoFDOTable *GetFirst()
  {
    return First;
  }
};

class AutoGPKGTable
{
private:
  char *Name;
  AutoGPKGTable *Next;
public:
    AutoGPKGTable(const char *name, const int len)
  {
    Name = new char[len + 1];
      strcpy(Name, name);
      Next = NULL;
  }
   ~AutoGPKGTable()
  {
    if (Name)
      delete[]Name;
  }
  char *GetName()
  {
    return Name;
  }
  void SetNext(AutoGPKGTable * next)
  {
    Next = next;
  }
  AutoGPKGTable *GetNext()
  {
    return Next;
  }
};

class AutoGPKGTables
{
private:
  AutoGPKGTable * First;
  AutoGPKGTable *Last;
public:
    AutoGPKGTables()
  {
    First = NULL;
    Last = NULL;
  }
   ~AutoGPKGTables();
  void Add(const char *name, const int len);
  AutoGPKGTable *GetFirst()
  {
    return First;
  }
};

class AutoSaveDialog:public wxDialog
{
//
// a dialog to manage AutoSave
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the path to save
  int Seconds;                  // interval 
  wxRadioBox *IntervalCtrl;
  wxTextCtrl *PathCtrl;
public:
    AutoSaveDialog()
  {;
  }
  AutoSaveDialog(MyFrame * parent, wxString & path, int secs);
  bool Create(MyFrame * parent, wxString & path, int secs);
  virtual ~ AutoSaveDialog()
  {;
  }
  void CreateControls();
  int GetSeconds()
  {
    return Seconds;
  }
  void OnOk(wxCommandEvent & event);
  wxString & GetPath()
  {
    return Path;
  }
  void OnIntervalChanged(wxCommandEvent & event);
  void OnChangePath(wxCommandEvent & event);
};

class LoadXmlDialog:public wxDialog
{
//
// a dialog to load XML documents
//
private:
  MyFrame * MainFrame;
  wxString Path;                // the XML path
  bool Compressed;              // compressed XML 
  bool Validate;                // Schema validation
  wxString SchemaURI;           // the Schema URI (if validation is required)
public:
    LoadXmlDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & path);
  virtual ~ LoadXmlDialog()
  {;
  }
  void CreateControls();
  bool IsCompressed()
  {
    return Compressed;
  }
  wxString & GetSchemaURI()
  {
    return SchemaURI;
  }
  void OnOk(wxCommandEvent & event);
  void OnCompressionChanged(wxCommandEvent & event);
  void OnValidationChanged(wxCommandEvent & event);
};

class AuxTable
{
// a class used by ComposerDialog [table item]
private:
  wxString TableName;
  wxString Geometries[128];
  int MaxGeometryIndex;
  AuxTable *Next;
public:
    AuxTable(wxString & table);
   ~AuxTable()
  {;
  }
  void AddGeometryColumn(wxString & geom);
  wxString & GetTableName()
  {
    return TableName;
  }
  wxString & GetGeometryColumn(int ind);
  int GetGeometriesCount()
  {
    return MaxGeometryIndex;
  }
  void SetNext(AuxTable * next)
  {
    Next = next;
  }
  AuxTable *GetNext()
  {
    return Next;
  }
};

class AuxTableList
{
// a class used by ComposerDialog [tables list]
private:
  AuxTable * First;
  AuxTable *Last;
  int Count;
public:
    AuxTableList();
   ~AuxTableList();
  void Flush();
  void Populate(sqlite3 * handle);
  int GetCount()
  {
    return Count;
  }
  AuxTable *GetFirst()
  {
    return First;
  }
};

class AuxColumn
{
// a class used by ComposerDialog [table item]
private:
  wxString Name;
  wxString AliasName;
  bool Selected;
  AuxColumn *Next;
public:
    AuxColumn(wxString & name);
   ~AuxColumn()
  {;
  }
  wxString & GetName()
  {
    return Name;
  }
  void SetState(bool mode)
  {
    Selected = mode;
  }
  bool IsSelected()
  {
    return Selected;
  }
  void SetAliasName(wxString & alias)
  {
    AliasName = alias;
  }
  wxString & GetAliasName()
  {
    return AliasName;
  }
  void SetNext(AuxColumn * next)
  {
    Next = next;
  }
  AuxColumn *GetNext()
  {
    return Next;
  }
};

class AuxColumnList
{
// a class used by ComposerDialog [tables list]
private:
  AuxColumn * First;
  AuxColumn *Last;
  int Count;
public:
    AuxColumnList();
   ~AuxColumnList();
  void Flush();
  void Populate(sqlite3 * handle, wxString & table, bool force_rowid = true);
  int GetCount()
  {
    return Count;
  }
  AuxColumn *GetFirst()
  {
    return First;
  }
  void SetState(int ind, bool mode);
  void SetState(wxString & column);
  void SetAlias(wxString & column, wxString & alias);
  bool HasSelectedColumns();
};

class DumpKmlDialog:public wxDialog
{
//
// a dialog preparing a DUMP KML
//
private:
  MyFrame * MainFrame;
  wxString Table;               // the table's name
  wxString Column;              // the column's name for Geometry
  bool isNameConst;
  bool isDescConst;
  wxString Name;
  wxString Desc;
  int Precision;
  wxComboBox *NameCtrl;
  wxTextCtrl *NameConstCtrl;
  wxComboBox *DescCtrl;
  wxTextCtrl *DescConstCtrl;
  wxSpinCtrl *PrecisionCtrl;
  AuxColumnList ColumnList;
  void InitializeComboColumns(wxComboBox * ctrl);
public:
    DumpKmlDialog()
  {;
  }
  DumpKmlDialog(MyFrame * parent, wxString & table, wxString & column);
  bool Create(MyFrame * parent, wxString & table, wxString & column);
  virtual ~ DumpKmlDialog()
  {;
  }
  void CreateControls();
  bool IsNameConst()
  {
    return isNameConst;
  }
  wxString & GetName()
  {
    return Name;
  }
  bool IsDescConst()
  {
    return isDescConst;
  }
  wxString & GetDesc()
  {
    return Desc;
  }
  int GetPrecision()
  {
    return Precision;
  }
  void OnNameSelected(wxCommandEvent & event);
  void OnDescSelected(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class DumpSpreadsheetDialog:public wxDialog
{
//
// a dialog preparing a DUMP Speadsheet
//
private:
  MyFrame * MainFrame;
  char DecimalPoint;
  bool DateTimes;
public:
    DumpSpreadsheetDialog()
  {;
  }
  DumpSpreadsheetDialog(MyFrame * parent);
  bool Create(MyFrame * parent);
  virtual ~ DumpSpreadsheetDialog()
  {;
  }
  void CreateControls();
  char GetDecimalPoint()
  {
    return DecimalPoint;
  }
  bool IsDateTimes()
  {
    return DateTimes;
  }
  void OnDecimalPointSelected(wxCommandEvent & event);
  void OnDateTimesSelected(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class RasterCoverageStyle
{
// an SLD/SE Raster style
private:
  int StyleID;
  wxString Name;
  wxString Title;
  wxString Abstract;
  wxString SchemaValidated;
  wxString SchemaURI;
  bool Selected;
  RasterCoverageStyle *Next;
public:
    RasterCoverageStyle(int style_id, wxString & name, wxString & title,
                        wxString & abstract, wxString & validated,
                        wxString & schema_uri);
   ~RasterCoverageStyle()
  {;
  }
  int GetStyleID()
  {
    return StyleID;
  }
  wxString & GetName()
  {
    return Name;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetSchemaValidated()
  {
    return SchemaValidated;
  }
  wxString & GetSchemaURI()
  {
    return SchemaURI;
  }
  void MarkSelected()
  {
    Selected = true;
  }
  bool IsSelected()
  {
    return Selected;
  }
  RasterCoverageStyle *GetNext()
  {
    return Next;
  }
  void SetNext(RasterCoverageStyle * next)
  {
    Next = next;
  }
};

class RasterCoverageStylesList
{
// a container for Raster Coverage SLD/SE styles
private:
  RasterCoverageStyle * First;
  RasterCoverageStyle *Last;
public:
    RasterCoverageStylesList();
   ~RasterCoverageStylesList();
  void Add(int style_id, wxString & name, wxString & title, wxString & abstract,
           wxString & validated, wxString & schema_uri);
  void MarkSelected(int stileId);
  RasterCoverageStyle *GetFirst()
  {
    return First;
  }
};

class RasterLoadParams
{
// a class wrapping Raster Import arguments
private:
  MyFrame * MainFrame;
  class ImportRasterDialog *Dlg;
  wxString CoverageName;
  wxArrayString Paths;
  wxString CurrentPath;
  int SRID;
  bool WithWorldFile;
  bool Pyramidize;
  bool Error;
  int Count;
  bool AbortPending;
public:
    RasterLoadParams()
  {;
  }
   ~RasterLoadParams()
  {;
  }
  void Initialize(MyFrame * mother, ImportRasterDialog * dlg,
                  wxString & coverage, wxArrayString & paths, int srid,
                  bool with_worldfile, bool pyramidize)
  {
    MainFrame = mother;
    Dlg = dlg;
    CoverageName = coverage;
    Paths = paths;
    SRID = srid;
    WithWorldFile = with_worldfile;
    Pyramidize = pyramidize;
    Error = false;
    Count = 0;
    AbortPending = false;
  }
  MyFrame *GetMainFrame()
  {
    return MainFrame;
  }
  ImportRasterDialog *GetDlg()
  {
    return Dlg;
  }
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  int GetPathsCount()
  {
    return Paths.Count();
  }
  wxString & GetPathByIndex(int idx)
  {
    return Paths.Item(idx);
  }
  int GetSRID()
  {
    return SRID;
  }
  bool IsWithWorldFile()
  {
    return WithWorldFile;
  }
  bool IsPyramidize()
  {
    return Pyramidize;
  }
  void SetCurrentPath(wxString & path)
  {
    CurrentPath = path;
  }
  wxString & GetCurrentPath()
  {
    return CurrentPath;
  }
  void SetError()
  {
    Error = true;
  }
  bool GetError()
  {
    return Error;
  }
  void Done()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
  void RequestAbort()
  {
    AbortPending = true;
  }
  bool IsAbortPending()
  {
    return AbortPending;
  }
};

class ImportRasterDialog:public wxDialog
{
//
// a dialog for importing external Raster files into a Coverage
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxArrayString Paths;
  wxString Path;
  wxString Title;
  wxString Abstract;
  wxString Sample;
  wxString Pixel;
  wxString Compression;
  bool ForceSRID;
  int SRID;
  bool WithWorldFile;
  bool Pyramidize;
  wxString ListDone;
  RasterLoadParams Params;
public:
    ImportRasterDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage, wxArrayString & paths,
              wxString & path, wxString & title, wxString & abstract,
              wxString & sample, wxString & pixel, wxString & compression,
              int srid);
  virtual ~ ImportRasterDialog()
  {;
  }
  void CreateControls();
  void DoRunLoad(void);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnCmdForceSridChanged(wxCommandEvent & event);
  void OnRequestStart(wxCommandEvent & event);
  void OnRequestStop(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
};

class PyramidizeDialog:public wxDialog
{
//
// a dialog supporting Pyramidize
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Sample;
  wxString Pixel;
  wxString Compression;
public:
    PyramidizeDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage, wxString & title,
              wxString & abstract, wxString & sample, wxString & pixel,
              wxString & compression);
  virtual ~ PyramidizeDialog()
  {;
  }
  void CreateControls();
  bool DoPyramidize(void);
  void OnOk(wxCommandEvent & event);
};

class PyramidizeMonolithicDialog:public wxDialog
{
//
// a dialog supporting PyramidizeMonolithic
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Sample;
  wxString Pixel;
  wxString Compression;
public:
    PyramidizeMonolithicDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage, wxString & title,
              wxString & abstract, wxString & sample, wxString & pixel,
              wxString & compression);
  virtual ~ PyramidizeMonolithicDialog()
  {;
  }
  void CreateControls();
  bool DoPyramidizeMonolithic(void);
  void OnOk(wxCommandEvent & event);
};

class DePyramidizeDialog:public wxDialog
{
//
// a dialog supporting DePyramidize
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Sample;
  wxString Pixel;
  wxString Compression;
public:
    DePyramidizeDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage, wxString & title,
              wxString & abstract, wxString & sample, wxString & pixel,
              wxString & compression);
  virtual ~ DePyramidizeDialog()
  {;
  }
  void CreateControls();
  bool DoDePyramidize(void);
  void OnOk(wxCommandEvent & event);
};

class RasterDropDialog:public wxDialog
{
//
// a dialog supporting Drop Raster
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  wxString DataLicense;
  wxString Sample;
  wxString Pixel;
  wxString Compression;
public:
    RasterDropDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage, wxString & title,
              wxString & abstract, wxString & copyright,
              wxString & data_license, wxString & sample, wxString & pixel,
              wxString & compression);
  virtual ~ RasterDropDialog()
  {;
  }
  void CreateControls();
  bool DoDropCoverage(void);
  void OnOk(wxCommandEvent & event);
};

class RasterInfosDialog:public wxDialog
{
//
// a dialog for editing Raster Coverage Infos
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool IsQueryable;
  bool DoReadCoverage();
  void DoUpdateCoverage();
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    RasterInfosDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ RasterInfosDialog()
  {;
  }
  void CreateControls();
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class RasterCoverageStylesDialog:public wxDialog
{
//
// a dialog for handling SLD/SE Raster Coverage's Styles
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  int CurrentRow;
  int CurrentStyleID;
  RasterCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    RasterCoverageStylesDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ RasterCoverageStylesDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  void DoRegistetRasterCoverageStyles(class ListRasterStylesDialog * dlg);
  void OnRightClick(wxGridEvent & event);
  void OnCmdRemoveStyle(wxCommandEvent & event);
  void OnAddStyle(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class ListRasterStylesDialog:public wxDialog
{
//
// a dialog for listing and selecting SLD/SE Raster Styles
//
private:
  MyFrame * MainFrame;
  RasterCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    ListRasterStylesDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ ListRasterStylesDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  int GetSelectedCount();
  int GetSelectedStyleId(int idx);
  void OnOk(wxCommandEvent & event);
};

class RasterStylesLoadParams
{
// a class wrapping Raster Styles Import arguments
private:
  MyFrame * MainFrame;
  class LoadRasterStyleDialog *Dlg;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  bool AbortPending;
public:
    RasterStylesLoadParams()
  {;
  }
   ~RasterStylesLoadParams()
  {;
  }
  void Initialize(MyFrame * mother, LoadRasterStyleDialog * dlg,
                  wxArrayString & paths)
  {
    MainFrame = mother;
    Dlg = dlg;
    Paths = paths;
    Error = false;
    Count = 0;
    AbortPending = false;
  }
  MyFrame *GetMainFrame()
  {
    return MainFrame;
  }
  LoadRasterStyleDialog *GetDlg()
  {
    return Dlg;
  }
  int GetPathsCount()
  {
    return Paths.Count();
  }
  wxString & GetPathByIndex(int idx)
  {
    return Paths.Item(idx);
  }
  void SetCurrentPath(wxString & path)
  {
    CurrentPath = path;
  }
  wxString & GetCurrentPath()
  {
    return CurrentPath;
  }
  void SetError()
  {
    Error = true;
  }
  bool GetError()
  {
    return Error;
  }
  void Done()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
  void RequestAbort()
  {
    AbortPending = true;
  }
  bool IsAbortPending()
  {
    return AbortPending;
  }
};

class LoadRasterStyleDialog:public wxDialog
{
//
// a dialog for adding new SLD/SE Raster Style
//
private:
  MyFrame * MainFrame;
  wxString Path;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  wxString ListDone;
  RasterStylesLoadParams Params;
public:
    LoadRasterStyleDialog()
  {;
  }
  bool Create(MyFrame * parent, wxArrayString & paths, wxString & path);
  virtual ~ LoadRasterStyleDialog()
  {;
  }
  void CreateControls();
  void DoRunLoad(void);
  bool RegisterRasterStyle(sqlite3_stmt * stmt, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnRequestStart(wxCommandEvent & event);
  void OnRequestStop(wxCommandEvent & event);
  void OnRequestSkip(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
};

class ReloadRasterStyleDialog:public wxDialog
{
//
// a dialog for reloading an already existing SLD/SE Raster Style
//
private:
  MyFrame * MainFrame;
  wxString Path;
  RasterCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    ReloadRasterStyleDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & path);
  virtual ~ ReloadRasterStyleDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoReloadRasterStyle(int style_id, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class UnregisterRasterStyleDialog:public wxDialog
{
//
// a dialog for unregistering an already existing SLD/SE Raster Style
//
private:
  MyFrame * MainFrame;
  RasterCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    UnregisterRasterStyleDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ UnregisterRasterStyleDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoCheckUnreferencedRasterStyle(int style_id);
  bool DoUnregisterRasterStyle(int style_id);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class MapConfigLoadParams
{
// a class wrapping MapConfig Import arguments
private:
  MyFrame * MainFrame;
  class LoadMapConfigDialog *Dlg;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  bool AbortPending;
public:
    MapConfigLoadParams()
  {;
  }
   ~MapConfigLoadParams()
  {;
  }
  void Initialize(MyFrame * mother, LoadMapConfigDialog * dlg,
                  wxArrayString & paths)
  {
    MainFrame = mother;
    Dlg = dlg;
    Paths = paths;
    Error = false;
    Count = 0;
    AbortPending = false;
  }
  MyFrame *GetMainFrame()
  {
    return MainFrame;
  }
  LoadMapConfigDialog *GetDlg()
  {
    return Dlg;
  }
  int GetPathsCount()
  {
    return Paths.Count();
  }
  wxString & GetPathByIndex(int idx)
  {
    return Paths.Item(idx);
  }
  void SetCurrentPath(wxString & path)
  {
    CurrentPath = path;
  }
  wxString & GetCurrentPath()
  {
    return CurrentPath;
  }
  void SetError()
  {
    Error = true;
  }
  bool GetError()
  {
    return Error;
  }
  void Done()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
  void RequestAbort()
  {
    AbortPending = true;
  }
  bool IsAbortPending()
  {
    return AbortPending;
  }
};

class MapConfiguration
{
// an XML Map Configuration
private:
  int ID;
  wxString Name;
  wxString Title;
  wxString Abstract;
  wxString SchemaValidated;
  wxString SchemaURI;
  bool Selected;
  MapConfiguration *Next;
public:
    MapConfiguration(int id, wxString & name, wxString & title,
                     wxString & abstract, wxString & validated,
                     wxString & schema_uri);
   ~MapConfiguration()
  {;
  }
  int GetID()
  {
    return ID;
  }
  wxString & GetName()
  {
    return Name;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetSchemaValidated()
  {
    return SchemaValidated;
  }
  wxString & GetSchemaURI()
  {
    return SchemaURI;
  }
  void MarkSelected()
  {
    Selected = true;
  }
  bool IsSelected()
  {
    return Selected;
  }
  MapConfiguration *GetNext()
  {
    return Next;
  }
  void SetNext(MapConfiguration * next)
  {
    Next = next;
  }
};

class MapConfigurationsList
{
// a container for XML Map COnfigurations
private:
  MapConfiguration * First;
  MapConfiguration *Last;
public:
    MapConfigurationsList();
   ~MapConfigurationsList();
  void Add(int id, wxString & name, wxString & title, wxString & abstract,
           wxString & validated, wxString & schema_uri);
  void MarkSelected(int stileId);
  MapConfiguration *GetFirst()
  {
    return First;
  }
};

class ListMapConfigDialog:public wxDialog
{
//
// a dialog for listing and selecting Map Configurations
//
private:
  MyFrame * MainFrame;
  MyMapPanel *Parent;
  MapConfigurationsList *List;
  wxGrid *GridCtrl;
  unsigned char *XML;
  void DoLoadXML(int id);
public:
    ListMapConfigDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * main, MyMapPanel * parent);
  virtual ~ ListMapConfigDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  unsigned char *GetXML()
  {
    return XML;
  }
  void OnOk(wxCommandEvent & event);
};

class LoadMapConfigDialog:public wxDialog
{
//
// a dialog for adding new XML Map Configuration
//
private:
  MyFrame * MainFrame;
  wxString Path;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  wxString ListDone;
  MapConfigLoadParams Params;
public:
    LoadMapConfigDialog()
  {;
  }
  bool Create(MyFrame * parent, wxArrayString & paths, wxString & path);
  virtual ~ LoadMapConfigDialog()
  {;
  }
  void CreateControls();
  void DoRunLoad(void);
  bool RegisterMapConfig(sqlite3_stmt * stmt, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnRequestStart(wxCommandEvent & event);
  void OnRequestStop(wxCommandEvent & event);
  void OnRequestSkip(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
};

class ReloadMapConfigDialog:public wxDialog
{
//
// a dialog for reloading an already existing XML Map Configuration
//
private:
  MyFrame * MainFrame;
  wxString Path;
  MapConfigurationsList *List;
  wxGrid *GridCtrl;
public:
    ReloadMapConfigDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & path);
  virtual ~ ReloadMapConfigDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoReloadMapConfig(int id, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class UnregisterMapConfigDialog:public wxDialog
{
//
// a dialog for unregistering an already existing XML Map Configuration
//
private:
  MyFrame * MainFrame;
  MapConfigurationsList *List;
  wxGrid *GridCtrl;
public:
    UnregisterMapConfigDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ UnregisterMapConfigDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoUnregisterMapConfig(int style_id);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class VerifyMapConfigDialog:public wxDialog
{
//
// a dialog for verifying an already registered XML Map Configuration
//
private:
  MyFrame * MainFrame;
  MapConfigurationsList *List;
  wxGrid *GridCtrl;
  unsigned char *Xml;
  void FetchXmlMapConfig(int id);
public:
    VerifyMapConfigDialog()
  {
    List = NULL;
    Xml = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ VerifyMapConfigDialog()
  {
    if (List != NULL)
      delete List;
    if (Xml != NULL)
      free(Xml);
  }
  void CreateControls();
  const unsigned char *GetXML()
  {
    return Xml;
  }
  void OnOk(wxCommandEvent & event);
};

class MapConfigHtmlDialog:public wxDialog
{
//
// a dialog for reporting an HTML report about validating Map Config 
//
private:
  MyFrame * MainFrame;
  wxString Html;
public:
    MapConfigHtmlDialog()
  {
    MainFrame = NULL;
  }
  MapConfigHtmlDialog(MyFrame * parent, wxString & html)
  {
    Create(parent, html);
  }
  bool Create(MyFrame * parent, wxString & html);
  virtual ~ MapConfigHtmlDialog()
  {;
  }
  void CreateControls();
  void OnSize(wxSizeEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class RasterCoverageSRID
{
// a Raster Coverage alternative SRID
private:
  int SRID;
  wxString AuthName;
  int AuthSRID;
  wxString RefSysName;
  bool Native;
  bool Deleted;
  RasterCoverageSRID *Next;
public:
    RasterCoverageSRID(bool native, int srid, wxString & auth_name,
                       int auth_srid, wxString & name)
  {
    SRID = srid;
    AuthName = auth_name;
    AuthSRID = auth_srid;
    RefSysName = name;
    Native = native;
    Deleted = false;
    Next = NULL;
  }
   ~RasterCoverageSRID()
  {;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetAuthName()
  {
    return AuthName;
  }
  int GetAuthSRID()
  {
    return AuthSRID;
  }
  wxString & GetRefSysName()
  {
    return RefSysName;
  }
  bool IsNative()
  {
    return Native;
  }
  void MarkDeleted()
  {
    Deleted = true;
  }
  bool IsDeleted()
  {
    return Deleted;
  }
  RasterCoverageSRID *GetNext()
  {
    return Next;
  }
  void SetNext(RasterCoverageSRID * next)
  {
    Next = next;
  }
};

class RasterCoverageSRIDsList
{
// a container for Raster Coverage alternative SRIDs
private:
  RasterCoverageSRID * First;
  RasterCoverageSRID *Last;
public:
    RasterCoverageSRIDsList()
  {
    First = NULL;
    Last = NULL;
  }
   ~RasterCoverageSRIDsList();
  void Add(bool native, int srid, wxString & auth_name, int auth_srid,
           wxString & name);
  RasterCoverageSRID *GetFirst()
  {
    return First;
  }
  bool IsAlreadyDefinedSRID(int srid);
  bool IsNativeSRID(int srid);
  void MarkDeleted(int strid);
};

class RasterSRIDsDialog:public wxDialog
{
//
// a dialog for handling Raster Coverage's alternative SRIDs
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  int CurrentRow;
  int CurrentSRID;
  RasterCoverageSRIDsList *List;
  wxGrid *GridCtrl;
  bool UpdateRefSysName(int srid);
  void DoRemoveSRID();
public:
    RasterSRIDsDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ RasterSRIDsDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoRegistetRasterCoverageSrid(int srid);
  void OnSridChanged(wxCommandEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCellSelected(wxGridEvent & event);
  void OnCmdRemoveSrid(wxCommandEvent & event);
  void OnCmdAddSrid(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class RasterCoverageKeyword
{
// a Raster Coverage Keyword
private:
  wxString Keyword;
  bool Deleted;
  RasterCoverageKeyword *Next;
public:
    RasterCoverageKeyword(wxString & keyword)
  {
    Keyword = keyword;
    Deleted = false;
    Next = NULL;
  }
   ~RasterCoverageKeyword()
  {;
  }
  wxString & GetKeyword()
  {
    return Keyword;
  }
  void MarkDeleted()
  {
    Deleted = true;
  }
  bool IsDeleted()
  {
    return Deleted;
  }
  RasterCoverageKeyword *GetNext()
  {
    return Next;
  }
  void SetNext(RasterCoverageKeyword * next)
  {
    Next = next;
  }
};

class RasterCoverageKeywordsList
{
// a container for Raster Coverage Keywords
private:
  RasterCoverageKeyword * First;
  RasterCoverageKeyword *Last;
public:
    RasterCoverageKeywordsList()
  {
    First = NULL;
    Last = NULL;
  }
   ~RasterCoverageKeywordsList();
  void Add(wxString & keyword);
  RasterCoverageKeyword *GetFirst()
  {
    return First;
  }
  bool IsAlreadyDefinedKeyword(wxString & keyword);
  void MarkDeleted(wxString & keyword);
};

class RasterKeywordsDialog:public wxDialog
{
//
// a dialog for handling Raster Coverage's Keywords
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  int CurrentRow;
  wxString Keyword;
  RasterCoverageKeywordsList *List;
  wxGrid *GridCtrl;
public:
    RasterKeywordsDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ RasterKeywordsDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoRegistetRasterCoverageKeyword(wxString & keyword);
  void OnRightClick(wxGridEvent & event);
  void OnCmdRemoveKeyword(wxCommandEvent & event);
  void OnCmdAddKeyword(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class ExternalGraphicLoadParams
{
// a class wrapping External Graphic Import arguments
private:
  MyFrame * MainFrame;
  class LoadExternalGraphicDialog *Dlg;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  bool AbortPending;
public:
    ExternalGraphicLoadParams()
  {;
  }
   ~ExternalGraphicLoadParams()
  {;
  }
  void Initialize(MyFrame * mother, LoadExternalGraphicDialog * dlg,
                  wxArrayString & paths)
  {
    MainFrame = mother;
    Dlg = dlg;
    Paths = paths;
    Error = false;
    Count = 0;
    AbortPending = false;
  }
  MyFrame *GetMainFrame()
  {
    return MainFrame;
  }
  LoadExternalGraphicDialog *GetDlg()
  {
    return Dlg;
  }
  int GetPathsCount()
  {
    return Paths.Count();
  }
  wxString & GetPathByIndex(int idx)
  {
    return Paths.Item(idx);
  }
  void SetCurrentPath(wxString & path)
  {
    CurrentPath = path;
  }
  wxString & GetCurrentPath()
  {
    return CurrentPath;
  }
  void SetError()
  {
    Error = true;
  }
  bool GetError()
  {
    return Error;
  }
  void Done()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
  void RequestAbort()
  {
    AbortPending = true;
  }
  bool IsAbortPending()
  {
    return AbortPending;
  }
};

class LoadExternalGraphicDialog:public wxDialog
{
//
// a dialog for adding new External Graphic resource(s)
//
private:
  MyFrame * MainFrame;
  wxString Path;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  wxString ListDone;
  ExternalGraphicLoadParams Params;
public:
    LoadExternalGraphicDialog()
  {;
  }
  bool Create(MyFrame * parent, wxArrayString & paths, wxString & path);
  virtual ~ LoadExternalGraphicDialog()
  {;
  }
  void CreateControls();
  void DoRunLoad(void);
  bool RegisterExternalGraphic(sqlite3_stmt * stmt, const char *xlink_href,
                               const char *title, const char *abstract,
                               const char *filename, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnRequestStart(wxCommandEvent & event);
  void OnRequestStop(wxCommandEvent & event);
  void OnRequestSkip(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
};

class ExternalGraphic
{
// a class wrapping an External Graphic
private:
  wxString XLinkHref;
  wxString Title;
  wxString Abstract;
  wxString MimeType;
  wxImage Graphic;
  ExternalGraphic *Next;
  bool DoBuildGraphic(wxString & mime_type, const void *blob, int blob_sz);
  void DoBuildDefaultGraphic();
public:
    ExternalGraphic(wxString & xlink_href, wxString & title,
                    wxString & abstract, wxString & mime_type, const void *blob,
                    int blob_sz);
   ~ExternalGraphic()
  {;
  }
  wxString & GetXLinkHref()
  {
    return XLinkHref;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetMimeType()
  {
    return MimeType;
  }
  wxImage *GetGraphic()
  {
    return &Graphic;
  }
  void SetNext(ExternalGraphic * next)
  {
    Next = next;
  }
  ExternalGraphic *GetNext()
  {
    return Next;
  }
};

class ExternalGraphicList
{
// a class acting as a container of External Graphics
private:
  ExternalGraphic * First;
  ExternalGraphic *Last;
public:
    ExternalGraphicList()
  {
    First = NULL;
    Last = NULL;
  }
   ~ExternalGraphicList();
  void Add(wxString & xlink_href, wxString & title, wxString & abstract,
           wxString & mime_type, const void *blob, int blob_sz);
  ExternalGraphic *GetFirst()
  {
    return First;
  }
  void FindByIndex(int idx, wxString & xlink_href, wxString & mime_type);
  int FindByXLinkHref(wxString & xlink_href);
};

class UnregisterExternalGraphicDialog:public wxDialog
{
//
// a dialog for unregistering an already existing External Graphic resource
//
private:
  MyFrame * MainFrame;
  ExternalGraphicList *List;
  wxGrid *GridCtrl;
public:
    UnregisterExternalGraphicDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ UnregisterExternalGraphicDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoUnregisterExternalGraphic(const char *xlink_href);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class TextFontLoadParams
{
// a class wrapping Text Font Import arguments
private:
  MyFrame * MainFrame;
  class LoadTextFontDialog *Dlg;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  bool AbortPending;
public:
    TextFontLoadParams()
  {;
  }
   ~TextFontLoadParams()
  {;
  }
  void Initialize(MyFrame * mother, LoadTextFontDialog * dlg,
                  wxArrayString & paths)
  {
    MainFrame = mother;
    Dlg = dlg;
    Paths = paths;
    Error = false;
    Count = 0;
    AbortPending = false;
  }
  MyFrame *GetMainFrame()
  {
    return MainFrame;
  }
  LoadTextFontDialog *GetDlg()
  {
    return Dlg;
  }
  int GetPathsCount()
  {
    return Paths.Count();
  }
  wxString & GetPathByIndex(int idx)
  {
    return Paths.Item(idx);
  }
  void SetCurrentPath(wxString & path)
  {
    CurrentPath = path;
  }
  wxString & GetCurrentPath()
  {
    return CurrentPath;
  }
  void SetError()
  {
    Error = true;
  }
  bool GetError()
  {
    return Error;
  }
  void Done()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
  void RequestAbort()
  {
    AbortPending = true;
  }
  bool IsAbortPending()
  {
    return AbortPending;
  }
};

class LoadTextFontDialog:public wxDialog
{
//
// a dialog for adding new Text Font resource(s)
//
private:
  MyFrame * MainFrame;
  wxString Path;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  wxString ListDone;
  TextFontLoadParams Params;
public:
    LoadTextFontDialog()
  {;
  }
  bool Create(MyFrame * parent, wxArrayString & paths, wxString & path);
  virtual ~ LoadTextFontDialog()
  {;
  }
  void CreateControls();
  void DoRunLoad(void);
  bool RegisterTextFont(sqlite3_stmt * stmt, const char *path);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnRequestStart(wxCommandEvent & event);
  void OnRequestStop(wxCommandEvent & event);
  void OnRequestSkip(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
};

class TextFont
{
// a class wrapping a Text Font
private:
  wxString Facename;
  bool Bold;
  bool Italic;
  wxImage *FontExample;
  TextFont *Next;
public:
    TextFont(const void *priv_data, const unsigned char *blob, int blob_sz);
   ~TextFont()
  {
    delete FontExample;
  }
  wxString & GetFacename()
  {
    return Facename;
  }
  bool IsBold()
  {
    return Bold;
  }
  bool IsItalic()
  {
    return Italic;
  }
  wxImage *GetFontExample()
  {
    return FontExample;
  }
  void SetNext(TextFont * next)
  {
    Next = next;
  }
  TextFont *GetNext()
  {
    return Next;
  }
};

class TextFontList
{
// a class acting as a container of Text Font
private:
  TextFont * First;
  TextFont *Last;
public:
    TextFontList()
  {
    First = NULL;
    Last = NULL;
  }
   ~TextFontList();
  void Add(const void *priv_data, const unsigned char *blob, int blob_sz);
  TextFont *GetFirst()
  {
    return First;
  }
  void FindByIndex(int idx, wxString & face_name, int *style, int *weight);
  int FindByFaceName(wxString & face_name);
};

class UnregisterTextFontDialog:public wxDialog
{
//
// a dialog for unregistering an already existing Text Font
//
private:
  MyFrame * MainFrame;
  TextFontList *List;
  wxGrid *GridCtrl;
public:
    UnregisterTextFontDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ UnregisterTextFontDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoUnregisterTextFont(int font_id);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class VectorStylesLoadParams
{
// a class wrapping Vector Styles Import arguments
private:
  MyFrame * MainFrame;
  class LoadVectorStyleDialog *Dlg;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  bool AbortPending;
public:
    VectorStylesLoadParams()
  {;
  }
   ~VectorStylesLoadParams()
  {;
  }
  void Initialize(MyFrame * mother, LoadVectorStyleDialog * dlg,
                  wxArrayString & paths)
  {
    MainFrame = mother;
    Dlg = dlg;
    Paths = paths;
    Error = false;
    Count = 0;
    AbortPending = false;
  }
  MyFrame *GetMainFrame()
  {
    return MainFrame;
  }
  LoadVectorStyleDialog *GetDlg()
  {
    return Dlg;
  }
  int GetPathsCount()
  {
    return Paths.Count();
  }
  wxString & GetPathByIndex(int idx)
  {
    return Paths.Item(idx);
  }
  void SetCurrentPath(wxString & path)
  {
    CurrentPath = path;
  }
  wxString & GetCurrentPath()
  {
    return CurrentPath;
  }
  void SetError()
  {
    Error = true;
  }
  bool GetError()
  {
    return Error;
  }
  void Done()
  {
    Count++;
  }
  int GetCount()
  {
    return Count;
  }
  void RequestAbort()
  {
    AbortPending = true;
  }
  bool IsAbortPending()
  {
    return AbortPending;
  }
};

class VectorCoverageStyle
{
// an SLD/SE Vector style
private:
  int StyleID;
  wxString Name;
  wxString Title;
  wxString Abstract;
  wxString SchemaValidated;
  wxString SchemaURI;
  bool Selected;
  VectorCoverageStyle *Next;
public:
    VectorCoverageStyle(int style_id, wxString & name, wxString & title,
                        wxString & abstract, wxString & validated,
                        wxString & schema_uri);
   ~VectorCoverageStyle()
  {;
  }
  int GetStyleID()
  {
    return StyleID;
  }
  wxString & GetName()
  {
    return Name;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetSchemaValidated()
  {
    return SchemaValidated;
  }
  wxString & GetSchemaURI()
  {
    return SchemaURI;
  }
  void MarkSelected()
  {
    Selected = true;
  }
  bool IsSelected()
  {
    return Selected;
  }
  VectorCoverageStyle *GetNext()
  {
    return Next;
  }
  void SetNext(VectorCoverageStyle * next)
  {
    Next = next;
  }
};

class VectorCoverageStylesList
{
// a container for Vector Coverage SLD/SE styles
private:
  VectorCoverageStyle * First;
  VectorCoverageStyle *Last;
public:
    VectorCoverageStylesList();
   ~VectorCoverageStylesList();
  void Add(int style_id, wxString & name, wxString & title, wxString & abstract,
           wxString & validated, wxString & schema_uri);
  void MarkSelected(int stileId);
  VectorCoverageStyle *GetFirst()
  {
    return First;
  }
};

class LoadVectorStyleDialog:public wxDialog
{
//
// a dialog for adding new SLD/SE Vector Style
//
private:
  MyFrame * MainFrame;
  wxString Path;
  wxArrayString Paths;
  wxString CurrentPath;
  bool Error;
  int Count;
  wxString ListDone;
  VectorStylesLoadParams Params;
public:
    LoadVectorStyleDialog()
  {;
  }
  bool Create(MyFrame * parent, wxArrayString & paths, wxString & path);
  virtual ~ LoadVectorStyleDialog()
  {;
  }
  void CreateControls();
  void DoRunLoad(void);
  bool RegisterVectorStyle(sqlite3_stmt * stmt, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdAbort(wxCommandEvent & event);
  void OnRequestStart(wxCommandEvent & event);
  void OnRequestStop(wxCommandEvent & event);
  void OnRequestSkip(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
};

class ReloadVectorStyleDialog:public wxDialog
{
//
// a dialog for reloading an already existing SLD/SE Vector Style
//
private:
  MyFrame * MainFrame;
  wxString Path;
  VectorCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    ReloadVectorStyleDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & path);
  virtual ~ ReloadVectorStyleDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoReloadVectorStyle(int style_id, void *blob, int blob_size);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class UnregisterVectorStyleDialog:public wxDialog
{
//
// a dialog for unregistering an already existing SLD/SE Vector Style
//
private:
  MyFrame * MainFrame;
  VectorCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    UnregisterVectorStyleDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ UnregisterVectorStyleDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoCheckUnreferencedVectorStyle(int style_id);
  bool DoUnregisterVectorStyle(int style_id);
  void OnCancel(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
};

class VectorUnregisterDialog:public wxDialog
{
//
// a dialog supporting Unregister Vector
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  wxString DataLicense;
  int Origin;
  wxString Type;
public:
    VectorUnregisterDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage, wxString & title,
              wxString & abstract, wxString & copyright, wxString & license,
              int origin, wxString & type);
  virtual ~ VectorUnregisterDialog()
  {;
  }
  void CreateControls();
  bool DoVectorUnregister(void);
  void OnOk(wxCommandEvent & event);
};

class VectorCoverageStylesDialog:public wxDialog
{
//
// a dialog for handling SLD/SE Vector Coverage's Styles
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  int CurrentRow;
  int CurrentStyleID;
  VectorCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    VectorCoverageStylesDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ VectorCoverageStylesDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  void DoRegistetVectorCoverageStyles(class ListVectorStylesDialog * dlg);
  void OnRightClick(wxGridEvent & event);
  void OnCmdRemoveStyle(wxCommandEvent & event);
  void OnAddStyle(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class ListVectorStylesDialog:public wxDialog
{
//
// a dialog for listing and selecting SLD/SE Vector Styles
//
private:
  MyFrame * MainFrame;
  VectorCoverageStylesList *List;
  wxGrid *GridCtrl;
public:
    ListVectorStylesDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ ListVectorStylesDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  int GetSelectedCount();
  int GetSelectedStyleId(int idx);
  void OnOk(wxCommandEvent & event);
};

class CandidateVectorCoverage
{
// a candidate Vector Coverage - SpatialTable
private:
  wxString TableName;
  wxString GeometryColumn;
  int SRID;
  wxString GeometryType;
  bool VectorCoverage;
  bool RasterCoverage;
  bool TopoGeo;
  bool TopoNet;
  CandidateVectorCoverage *Next;
public:
    CandidateVectorCoverage(wxString & table, wxString & geometry, int srid,
                            wxString & type)
  {
    TableName = table;
    GeometryColumn = geometry;
    SRID = srid;
    GeometryType = type;
    VectorCoverage = false;
    RasterCoverage = false;
    TopoGeo = false;
    TopoNet = false;
    Next = NULL;
  }
   ~CandidateVectorCoverage()
  {;
  }
  wxString & GetTableName()
  {
    return TableName;
  }
  wxString & GetGeometryColumn()
  {
    return GeometryColumn;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetGeometryType()
  {
    return GeometryType;
  }
  void MarkVectorCoverage()
  {
    VectorCoverage = true;
  }
  bool IsVectorCoverage()
  {
    return VectorCoverage;
  }
  void MarkRasterCoverage()
  {
    RasterCoverage = true;
  }
  bool IsRasterCoverage()
  {
    return RasterCoverage;
  }
  void MarkTopoGeoCoverage()
  {
    TopoGeo = true;
  }
  bool IsTopoGeo()
  {
    return TopoGeo;
  }
  void MarkTopoNetCoverage()
  {
    TopoNet = true;
  }
  bool IsTopoNet()
  {
    return TopoNet;
  }
  CandidateVectorCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(CandidateVectorCoverage * next)
  {
    Next = next;
  }
};

class CandidateVectorCoveragesList
{
// a container for candidate Vector Coverages - SpatialTables
private:
  CandidateVectorCoverage * First;
  CandidateVectorCoverage *Last;
public:
    CandidateVectorCoveragesList()
  {
    First = NULL;
    Last = NULL;
  }
   ~CandidateVectorCoveragesList();
  void Add(wxString & table_name, wxString & geometry, int srid,
           wxString & type);
  void MarkVectorCoverage(wxString & table, wxString & geometry);
  void MarkRasterCoverage(wxString & table, wxString & geometry);
  void MarkTopoGeoCoverage(wxString & table, wxString & geometry);
  void MarkTopoNetCoverage(wxString & table, wxString & geometry);
  CandidateVectorCoverage *GetFirst()
  {
    return First;
  }
};

class VectorRegisterDialog:public wxDialog
{
//
// a dialog for Register Vector Coverage - SpatialTables
//
private:
  MyFrame * MainFrame;
  CandidateVectorCoveragesList *List;
  wxGrid *GridCtrl;
  wxString CoverageName;
  wxString TableName;
  wxString GeometryColumn;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool Queryable;
  bool Editable;
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    VectorRegisterDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ VectorRegisterDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  wxString & GetTableName()
  {
    return TableName;
  }
  wxString & GetGeometryColumn()
  {
    return GeometryColumn;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  bool IsEditable()
  {
    return Editable;
  }
  void OnOk(wxCommandEvent & event);
};

class CandidateSpatialViewCoverage
{
// a candidate Vector Coverage - SpatialView
private:
  wxString ViewName;
  wxString ViewGeometry;
  int SRID;
  wxString GeometryType;
  bool VectorCoverage;
  CandidateSpatialViewCoverage *Next;
public:
    CandidateSpatialViewCoverage(wxString & view_name, wxString & view_geometry,
                                 int srid, wxString & type)
  {
    ViewName = view_name;
    ViewGeometry = view_geometry;
    SRID = srid;
    GeometryType = type;
    VectorCoverage = false;
    Next = NULL;
  }
   ~CandidateSpatialViewCoverage()
  {;
  }
  wxString & GetViewName()
  {
    return ViewName;
  }
  wxString & GetViewGeometry()
  {
    return ViewGeometry;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetGeometryType()
  {
    return GeometryType;
  }
  void MarkVectorCoverage()
  {
    VectorCoverage = true;
  }
  bool IsVectorCoverage()
  {
    return VectorCoverage;
  }
  CandidateSpatialViewCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(CandidateSpatialViewCoverage * next)
  {
    Next = next;
  }
};

class CandidateSpatialViewCoveragesList
{
// a container for candidate Vector Coverages - SpatialViews
private:
  CandidateSpatialViewCoverage * First;
  CandidateSpatialViewCoverage *Last;
public:
    CandidateSpatialViewCoveragesList()
  {
    First = NULL;
    Last = NULL;
  }
   ~CandidateSpatialViewCoveragesList();
  void Add(wxString & view_name, wxString & geometry, int srid,
           wxString & type);
  void MarkVectorCoverage(wxString & view, wxString & geometry);
  CandidateSpatialViewCoverage *GetFirst()
  {
    return First;
  }
};

class SpatialViewRegisterDialog:public wxDialog
{
//
// a dialog for Register Vector Coverage - SpatialView
//
private:
  MyFrame * MainFrame;
  CandidateSpatialViewCoveragesList *List;
  wxGrid *GridCtrl;
  wxString CoverageName;
  wxString ViewName;
  wxString ViewGeometry;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool Queryable;
  bool Editable;
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    SpatialViewRegisterDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ SpatialViewRegisterDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  wxString & GetViewName()
  {
    return ViewName;
  }
  wxString & GetViewGeometry()
  {
    return ViewGeometry;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  bool IsEditable()
  {
    return Editable;
  }
  void OnOk(wxCommandEvent & event);
};

class CandidateVirtualTableCoverage
{
// a candidate Vector Coverage - VirtualTable
private:
  wxString VirtName;
  wxString VirtGeometry;
  int SRID;
  wxString GeometryType;
  bool VectorCoverage;
  CandidateVirtualTableCoverage *Next;
public:
    CandidateVirtualTableCoverage(wxString & virt_name,
                                  wxString & virt_geometry, int srid,
                                  wxString & type)
  {
    VirtName = virt_name;
    VirtGeometry = virt_geometry;
    SRID = srid;
    GeometryType = type;
    VectorCoverage = false;
    Next = NULL;
  }
   ~CandidateVirtualTableCoverage()
  {;
  }
  wxString & GetVirtName()
  {
    return VirtName;
  }
  wxString & GetVirtGeometry()
  {
    return VirtGeometry;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetGeometryType()
  {
    return GeometryType;
  }
  void MarkVectorCoverage()
  {
    VectorCoverage = true;
  }
  bool IsVectorCoverage()
  {
    return VectorCoverage;
  }
  CandidateVirtualTableCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(CandidateVirtualTableCoverage * next)
  {
    Next = next;
  }
};

class CandidateVirtualTableCoveragesList
{
// a container for candidate Vector Coverages - VirtualTable
private:
  CandidateVirtualTableCoverage * First;
  CandidateVirtualTableCoverage *Last;
public:
    CandidateVirtualTableCoveragesList()
  {
    First = NULL;
    Last = NULL;
  }
   ~CandidateVirtualTableCoveragesList();
  void Add(wxString & view_name, wxString & geometry, int srid,
           wxString & type);
  void MarkVectorCoverage(wxString & view, wxString & geometry);
  CandidateVirtualTableCoverage *GetFirst()
  {
    return First;
  }
};

class VirtualTableRegisterDialog:public wxDialog
{
//
// a dialog for Register Vector Coverage - VirtualTable
//
private:
  MyFrame * MainFrame;
  CandidateVirtualTableCoveragesList *List;
  wxGrid *GridCtrl;
  wxString CoverageName;
  wxString VirtName;
  wxString VirtGeometry;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool Queryable;
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    VirtualTableRegisterDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ VirtualTableRegisterDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  wxString & GetVirtName()
  {
    return VirtName;
  }
  wxString & GetVirtGeometry()
  {
    return VirtGeometry;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  void OnOk(wxCommandEvent & event);
};

class CandidateTopoGeoCoverage
{
// a candidate TopoGeo Coverage
private:
  wxString TopologyName;
  int SRID;
  wxString Dimensions;
  double Tolerance;
  bool VectorCoverage;
  CandidateTopoGeoCoverage *Next;
public:
    CandidateTopoGeoCoverage(wxString & topology, int srid,
                             wxString & dimensions, double tolerance)
  {
    TopologyName = topology;
    SRID = srid;
    Dimensions = dimensions;
    Tolerance = tolerance;
    VectorCoverage = false;
    Next = NULL;
  }
   ~CandidateTopoGeoCoverage()
  {;
  }
  wxString & GetTopologyName()
  {
    return TopologyName;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetDimensions()
  {
    return Dimensions;
  }
  double GetTolerance()
  {
    return Tolerance;
  }
  void MarkVectorCoverage()
  {
    VectorCoverage = true;
  }
  bool IsVectorCoverage()
  {
    return VectorCoverage;
  }
  CandidateTopoGeoCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(CandidateTopoGeoCoverage * next)
  {
    Next = next;
  }
};

class CandidateTopoGeoCoveragesList
{
// a container for candidate TopoGeo Coverages
private:
  CandidateTopoGeoCoverage * First;
  CandidateTopoGeoCoverage *Last;
public:
    CandidateTopoGeoCoveragesList()
  {
    First = NULL;
    Last = NULL;
  }
   ~CandidateTopoGeoCoveragesList();
  void Add(wxString & topology, int srid, wxString & dimensions,
           double tolerance);
  void MarkVectorCoverage(wxString & topology);
  CandidateTopoGeoCoverage *GetFirst()
  {
    return First;
  }
};

class TopoGeoRegisterDialog:public wxDialog
{
//
// a dialog for Register TopoGeo Coverage
//
private:
  MyFrame * MainFrame;
  CandidateTopoGeoCoveragesList *List;
  wxGrid *GridCtrl;
  wxString CoverageName;
  wxString TopologyName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool Queryable;
  bool Editable;
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    TopoGeoRegisterDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ TopoGeoRegisterDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  wxString & GetTopologyName()
  {
    return TopologyName;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  bool IsEditable()
  {
    return Editable;
  }
  void OnOk(wxCommandEvent & event);
};

class CandidateTopoNetCoverage
{
// a candidate TopoNet Coverage
private:
  wxString NetworkName;
  int SRID;
  wxString Dimensions;
  bool VectorCoverage;
  CandidateTopoNetCoverage *Next;
public:
    CandidateTopoNetCoverage(wxString & network, int srid,
                             wxString & dimensions)
  {
    NetworkName = network;
    SRID = srid;
    Dimensions = dimensions;
    VectorCoverage = false;
    Next = NULL;
  }
   ~CandidateTopoNetCoverage()
  {;
  }
  wxString & GetNetworkName()
  {
    return NetworkName;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetDimensions()
  {
    return Dimensions;
  }
  void MarkVectorCoverage()
  {
    VectorCoverage = true;
  }
  bool IsVectorCoverage()
  {
    return VectorCoverage;
  }
  CandidateTopoNetCoverage *GetNext()
  {
    return Next;
  }
  void SetNext(CandidateTopoNetCoverage * next)
  {
    Next = next;
  }
};

class CandidateTopoNetCoveragesList
{
// a container for candidate TopoNet Coverages
private:
  CandidateTopoNetCoverage * First;
  CandidateTopoNetCoverage *Last;
public:
    CandidateTopoNetCoveragesList()
  {
    First = NULL;
    Last = NULL;
  }
   ~CandidateTopoNetCoveragesList();
  void Add(wxString & network, int srid, wxString & dimensions);
  void MarkVectorCoverage(wxString & network);
  CandidateTopoNetCoverage *GetFirst()
  {
    return First;
  }
};

class TopoNetRegisterDialog:public wxDialog
{
//
// a dialog for Register TopoNet Coverage
//
private:
  MyFrame * MainFrame;
  CandidateTopoNetCoveragesList *List;
  wxGrid *GridCtrl;
  wxString CoverageName;
  wxString NetworkName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool Queryable;
  bool Editable;
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    TopoNetRegisterDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent);
  virtual ~ TopoNetRegisterDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  wxString & GetCoverageName()
  {
    return CoverageName;
  }
  wxString & GetNetworkName()
  {
    return NetworkName;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  bool IsEditable()
  {
    return Editable;
  }
  void OnOk(wxCommandEvent & event);
};

class VectorInfosDialog:public wxDialog
{
//
// a dialog for editing Vector Coverage Data
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  bool IsQueryable;
  bool IsEditable;
  bool DoReadCoverage();
  void DoUpdateCoverage();
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    VectorInfosDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ VectorInfosDialog()
  {;
  }
  void CreateControls();
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class VectorCoverageSRID
{
// a Vector Coverage alternative SRID
private:
  int SRID;
  wxString AuthName;
  int AuthSRID;
  wxString RefSysName;
  bool Native;
  bool Deleted;
  VectorCoverageSRID *Next;
public:
    VectorCoverageSRID(bool native, int srid, wxString & auth_name,
                       int auth_srid, wxString & name)
  {
    SRID = srid;
    AuthName = auth_name;
    AuthSRID = auth_srid;
    RefSysName = name;
    Native = native;
    Deleted = false;
    Next = NULL;
  }
   ~VectorCoverageSRID()
  {;
  }
  int GetSRID()
  {
    return SRID;
  }
  wxString & GetAuthName()
  {
    return AuthName;
  }
  int GetAuthSRID()
  {
    return AuthSRID;
  }
  wxString & GetRefSysName()
  {
    return RefSysName;
  }
  bool IsNative()
  {
    return Native;
  }
  void MarkDeleted()
  {
    Deleted = true;
  }
  bool IsDeleted()
  {
    return Deleted;
  }
  VectorCoverageSRID *GetNext()
  {
    return Next;
  }
  void SetNext(VectorCoverageSRID * next)
  {
    Next = next;
  }
};

class VectorCoverageSRIDsList
{
// a container for Vector Coverage alternative SRIDs
private:
  VectorCoverageSRID * First;
  VectorCoverageSRID *Last;
public:
    VectorCoverageSRIDsList()
  {
    First = NULL;
    Last = NULL;
  }
   ~VectorCoverageSRIDsList();
  void Add(bool native, int srid, wxString & auth_name, int auth_srid,
           wxString & name);
  VectorCoverageSRID *GetFirst()
  {
    return First;
  }
  bool IsAlreadyDefinedSRID(int srid);
  bool IsNativeSRID(int srid);
  void MarkDeleted(int strid);
};

class VectorSRIDsDialog:public wxDialog
{
//
// a dialog for handling Vector Coverage's alternative SRIDs
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  int CurrentRow;
  int CurrentSRID;
  VectorCoverageSRIDsList *List;
  wxGrid *GridCtrl;
  bool UpdateRefSysName(int srid);
  void DoRemoveSRID();
public:
    VectorSRIDsDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ VectorSRIDsDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoRegistetVectorCoverageSrid(int srid);
  void OnSridChanged(wxCommandEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCellSelected(wxGridEvent & event);
  void OnCmdRemoveSrid(wxCommandEvent & event);
  void OnCmdAddSrid(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class VectorCoverageKeyword
{
// a Vector Coverage Keyword
private:
  wxString Keyword;
  bool Deleted;
  VectorCoverageKeyword *Next;
public:
    VectorCoverageKeyword(wxString & keyword)
  {
    Keyword = keyword;
    Deleted = false;
    Next = NULL;
  }
   ~VectorCoverageKeyword()
  {;
  }
  wxString & GetKeyword()
  {
    return Keyword;
  }
  void MarkDeleted()
  {
    Deleted = true;
  }
  bool IsDeleted()
  {
    return Deleted;
  }
  VectorCoverageKeyword *GetNext()
  {
    return Next;
  }
  void SetNext(VectorCoverageKeyword * next)
  {
    Next = next;
  }
};

class VectorCoverageKeywordsList
{
// a container for Vector Coverage Keywords
private:
  VectorCoverageKeyword * First;
  VectorCoverageKeyword *Last;
public:
    VectorCoverageKeywordsList()
  {
    First = NULL;
    Last = NULL;
  }
   ~VectorCoverageKeywordsList();
  void Add(wxString & keyword);
  VectorCoverageKeyword *GetFirst()
  {
    return First;
  }
  bool IsAlreadyDefinedKeyword(wxString & keyword);
  void MarkDeleted(wxString & keyword);
};

class VectorKeywordsDialog:public wxDialog
{
//
// a dialog for handling Vector Coverage's Keywords
//
private:
  MyFrame * MainFrame;
  wxString CoverageName;
  int CurrentRow;
  wxString Keyword;
  VectorCoverageKeywordsList *List;
  wxGrid *GridCtrl;
public:
    VectorKeywordsDialog()
  {
    List = NULL;
  }
  bool Create(MyFrame * parent, wxString & coverage);
  virtual ~ VectorKeywordsDialog()
  {
    if (List != NULL)
      delete List;
  }
  void CreateControls();
  bool DoRegistetVectorCoverageKeyword(wxString & keyword);
  void OnRightClick(wxGridEvent & event);
  void OnCmdRemoveKeyword(wxCommandEvent & event);
  void OnCmdAddKeyword(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class RasterSymbolizerContrastDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the ContrastEnhancement type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  bool Normalize;
  bool Histogram;
  bool Gamma;
  double GammaValue;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
public:
    RasterSymbolizerContrastDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerContrastDialog()
  {;
  }
  void CreateControls();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnCmdModeChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class RasterSymbolizerChannelRgbDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the ChannelSelection (RGB) type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  int RedBand;
  int GreenBand;
  int BlueBand;
  bool Normalize;
  bool Histogram;
  bool Gamma;
  double GammaValue;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
public:
    RasterSymbolizerChannelRgbDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerChannelRgbDialog()
  {;
  }
  void CreateControls();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnCmdModeChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class RasterSymbolizerChannelGrayDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the ChannelSelection (Gray) type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  int GrayBand;
  bool Normalize;
  bool Histogram;
  bool Gamma;
  double GammaValue;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
public:
    RasterSymbolizerChannelGrayDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerChannelGrayDialog()
  {;
  }
  void CreateControls();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnCmdModeChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class ColorMapEntry
{
//
// a class wrapping a ColorMap entry
//
private:
  double Value;
  wxString Color;
  ColorMapEntry *Prev;
  ColorMapEntry *Next;
public:
    ColorMapEntry(double value, wxString & color)
  {
    Value = value;
    Color = color.MakeLower();
    Prev = NULL;
    Next = NULL;
  }
   ~ColorMapEntry()
  {;
  }
  double GetValue()
  {
    return Value;
  }
  void SetColor(wxString & color)
  {
    Color = color;
  }
  wxString & GetColor()
  {
    return Color;
  }
  void SetPrev(ColorMapEntry * ptr)
  {
    Prev = ptr;
  }
  ColorMapEntry *GetPrev()
  {
    return Prev;
  }
  void SetNext(ColorMapEntry * ptr)
  {
    Next = ptr;
  }
  ColorMapEntry *GetNext()
  {
    return Next;
  }
  static bool IsValidColor(wxString & color);
  static void GetWxColor(wxString & color, wxColour & clr);
  static unsigned char ParseHex(unsigned char hi, unsigned char lo);
  static void DoPaintColorSample(int width, int height, wxColour & color,
                                 wxBitmap & bmp);
};

class ColorMapCategorize
{
//
// a class wrapping a ColorMap of the Categorize Type
//
private:
  wxString FirstColor;
  ColorMapEntry *First;
  ColorMapEntry *Last;
public:
    ColorMapCategorize()
  {
    FirstColor = wxT("#000000");
    First = NULL;
    Last = NULL;
  }
   ~ColorMapCategorize();
  void SetFirstColor(wxString & color)
  {
    FirstColor = color;
  }
  wxString & GetFirstColor()
  {
    return FirstColor;
  }
  void Add(double value, wxString & color);
  void Remove(double value);
  ColorMapEntry *GetFirst()
  {
    return First;
  }
};

class ColorMapInterpolate
{
//
// a class wrapping a ColorMap of the Interpolate Type
//
private:
  ColorMapEntry * First;
  ColorMapEntry *Last;
public:
    ColorMapInterpolate()
  {
    First = NULL;
    Last = NULL;
  }
   ~ColorMapInterpolate();
  void Add(double value, wxString & color);
  void Remove(double value);
  ColorMapEntry *GetFirst()
  {
    return First;
  }
};

class RasterSymbolizerCategorizeDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the ColorMap Categorize type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  ColorMapCategorize Map;
  bool ShadedRelief;
  double ReliefFactor;
  wxGrid *GridCtrl;
  int CurrentRow;
  double CurrentValue;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
  void RefreshGrid();
public:
    RasterSymbolizerCategorizeDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerCategorizeDialog()
  {;
  }
  void CreateControls();
  void OnCmdAddEntry(wxCommandEvent & event);
  void OnCmdRemoveEntry(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnShadedChanged(wxCommandEvent & event);
  void OnCmdColorPicker(wxCommandEvent & event);
  void OnCmdAdd(wxCommandEvent & event);
  void OnCmdRemove(wxCommandEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCellSelected(wxGridEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class RasterSymbolizerInterpolateDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the ColorMap Interpolate type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  wxString Fallback;
  ColorMapInterpolate Map;
  bool ShadedRelief;
  double ReliefFactor;
  wxGrid *GridCtrl;
  int CurrentRow;
  double CurrentValue;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
  void RefreshGrid();
public:
    RasterSymbolizerInterpolateDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerInterpolateDialog()
  {;
  }
  void CreateControls();
  void OnCmdAddEntry(wxCommandEvent & event);
  void OnCmdRemoveEntry(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnShadedChanged(wxCommandEvent & event);
  void OnCmdColorPicker(wxCommandEvent & event);
  void OnCmdAdd(wxCommandEvent & event);
  void OnCmdRemove(wxCommandEvent & event);
  void OnRightClick(wxGridEvent & event);
  void OnCellSelected(wxGridEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class RasterSymbolizerShadedReliefDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the ChannelSelection (Gray) type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  double ReliefFactor;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
public:
    RasterSymbolizerShadedReliefDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerShadedReliefDialog()
  {;
  }
  void CreateControls();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class RasterSymbolizerMonochromeDialog:public wxDialog
{
//
// a dialog for creating an SLD/SE RasterSymbolizer 
// of the Recolored Monochrome type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  double Opacity;
  wxString Color;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  bool RetrieveParams();
  char *DoCreateSymbolizerXML();
  char *DoCreateCoverageXML();
public:
    RasterSymbolizerMonochromeDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ RasterSymbolizerMonochromeDialog()
  {;
  }
  void CreateControls();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdColorPicker(wxCommandEvent & event);
  void OnCmdColorChanged(wxCommandEvent & event);
};

class MyGraphicCellRenderer:public wxGridCellRenderer
{
// a Grid CellRenderer supporting a Graphic Image
private:
  wxImage * Graphic;
public:
  virtual wxGridCellRenderer * Clone() const
  {
    return new MyGraphicCellRenderer;
  }
  virtual void Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc,
                    const wxRect & rect, int row, int col, bool isSelected);
  virtual wxSize GetBestSize(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc,
                             int row, int col)
  {
    // silencing stupid compiler warnings about unused args
    if (grid.IsEditable() && attr.HasFont() && dc.IsOk())
      {
        if (row == col)
          col = row;
      }

    return wxSize(48, 24);
  }
  void SetGraphic(wxImage * graphic)
  {
    Graphic = graphic;
  }
};

class MyBitmapCellRenderer:public wxGridCellRenderer
{
// a Grid CellRenderer supporting a Bitmap
private:
  wxImage Graphic;
public:
  virtual wxGridCellRenderer * Clone() const
  {
    return new MyBitmapCellRenderer;
  }
  virtual void Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc,
                    const wxRect & rect, int row, int col, bool isSelected);
  virtual wxSize GetBestSize(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc,
                             int row, int col)
  {
    // silencing stupid compiler warnings about unused args
    if (grid.IsEditable() && attr.HasFont() && dc.IsOk())
      {
        if (row == col)
          col = row;
      }

    return wxSize(Graphic.GetWidth(), Graphic.GetHeight());
  }
  void SetGraphic(wxBitmap & graphic)
  {
    Graphic = graphic.ConvertToImage();
  }
};

class MyFontCellRenderer:public wxGridCellRenderer
{
// a Grid CellRenderer supporting a Text Font
private:
  wxImage * FontExample;
public:
  virtual wxGridCellRenderer * Clone() const
  {
    return new MyFontCellRenderer;
  }
  virtual void Draw(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc,
                    const wxRect & rect, int row, int col, bool isSelected);
  virtual wxSize GetBestSize(wxGrid & grid, wxGridCellAttr & attr, wxDC & dc,
                             int row, int col)
  {
    // silencing stupid compiler warnings about unused args
    if (grid.IsEditable() && attr.HasFont() && dc.IsOk())
      {
        if (row == col)
          col = row;
      }

    return wxSize(600, 22);
  }
  void SetFontExample(wxImage * example)
  {
    FontExample = example;
  }
  void SetFontExample(const void *priv_data, const char *facename);
};

class SymbolizerPreview:public wxStaticBitmap
{
//
// a window to show a Vector Symbolizer Preview
//
private:
  wxPropertySheetDialog * Parent;
public:
  SymbolizerPreview(wxPropertySheetDialog * parent, wxWindow * panel,
                    wxWindowID id, const wxBitmap & bmp, wxSize const &size);
    virtual ~ SymbolizerPreview()
  {;
  }
};

class SimpleLineSymbolizerDialog:public wxPropertySheetDialog
{
//
// a dialog for creating an SLD/SE LineSymbolizer 
// of the simple type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  unsigned char Uom;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  double PerpendicularOffset1;
  double Stroke1Opacity;
  bool HasGraphic1;
  wxString Stroke1Color;
  wxString Stroke1XLinkHref;
  wxString Stroke1MimeType;
  bool EnableStroke1Replacement;
  wxString Stroke1ColorReplacement;
  ExternalGraphicList *List;
  wxGrid *GridCtrl1;
  double Stroke1Width;
  int Stroke1LineJoin;
  int Stroke1LineCap;
  int Stroke1DashCount;
  double *Stroke1DashArray;
  double Stroke1DashOffset;
  bool EnableStroke2;
  double PerpendicularOffset2;
  double Stroke2Opacity;
  bool HasGraphic2;
  wxString Stroke2Color;
  wxString Stroke2XLinkHref;
  bool EnableStroke2Replacement;
  wxString Stroke2ColorReplacement;
  wxString Stroke2MimeType;
  wxGrid *GridCtrl2;
  double Stroke2Width;
  int Stroke2LineJoin;
  int Stroke2LineCap;
  int Stroke2DashCount;
  double *Stroke2DashArray;
  double Stroke2DashOffset;
  bool EnableStroke3;
  double PerpendicularOffset3;
  double Stroke3Opacity;
  bool HasGraphic3;
  wxString Stroke3Color;
  wxString Stroke3XLinkHref;
  bool EnableStroke3Replacement;
  wxString Stroke3ColorReplacement;
  wxString Stroke3MimeType;
  wxGrid *GridCtrl3;
  double Stroke3Width;
  int Stroke3LineJoin;
  int Stroke3LineCap;
  int Stroke3DashCount;
  double *Stroke3DashArray;
  double Stroke3DashOffset;
  unsigned char PreviewBackground;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreateStroke1Page(wxWindow * book);
  wxPanel *CreateStroke2Page(wxWindow * book);
  wxPanel *CreateStroke3Page(wxWindow * book);
  wxPanel *CreatePreviewPage(wxWindow * book);
  void CreateButtons();
  bool FinalValidityCheck();
  bool RetrieveMainPage();
  bool RetrieveStroke1Page(bool check = true);
  bool RetrieveStroke2Page(bool check = true);
  bool RetrieveStroke3Page(bool check = true);
  bool RetrievePreviewPage();
  void UpdateMainPage();
  void UpdateStroke1Page();
  void UpdateStroke2Page();
  void UpdateStroke3Page();
  void UpdatePreviewPage();
  char *DoCreateSymbolizerXML();
  char *DoCreateFeatureTypeXML();
  void DrawPreview(int horz, int vert);
  bool DoParseDashArray(wxString & str, int which);
  void NormalizedDashArray(wxString & str, int which, char delimiter = ',');
  void PrepareLinestringPath(void *ctx, double perpendicular_offset);
  wxBitmap PreviewBackBitmap;
public:
    SimpleLineSymbolizerDialog();
  bool Create(MyFrame * parent);
    virtual ~ SimpleLineSymbolizerDialog();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdUomChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdStroke1TypeChanged(wxCommandEvent & event);
  void OnCmdStroke2TypeChanged(wxCommandEvent & event);
  void OnCmdStroke3TypeChanged(wxCommandEvent & event);
  void OnCmdColor1Changed(wxCommandEvent & event);
  void OnCmdColor2Changed(wxCommandEvent & event);
  void OnCmdColor3Changed(wxCommandEvent & event);
  void OnCmdColor1Picker(wxCommandEvent & event);
  void OnCmdColor2Picker(wxCommandEvent & event);
  void OnCmdColor3Picker(wxCommandEvent & event);
  void OnCmdReplacement1Changed(wxCommandEvent & event);
  void OnCmdReplacement2Changed(wxCommandEvent & event);
  void OnCmdReplacement3Changed(wxCommandEvent & event);
  void OnCmdLineJoin1Changed(wxCommandEvent & event);
  void OnCmdLineJoin2Changed(wxCommandEvent & event);
  void OnCmdLineJoin3Changed(wxCommandEvent & event);
  void OnCmdLineCap1Changed(wxCommandEvent & event);
  void OnCmdLineCap2Changed(wxCommandEvent & event);
  void OnCmdLineCap3Changed(wxCommandEvent & event);
  void OnCmdStroke2Changed(wxCommandEvent & event);
  void OnCmdStroke3Changed(wxCommandEvent & event);
  void OnCmdStroke1EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdStroke2EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdStroke3EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdBackgroundChanged(wxCommandEvent & event);
};

class SimplePolygonSymbolizerDialog:public wxPropertySheetDialog
{
//
// a dialog for creating an SLD/SE PolygonSymbolizer 
// of the simple type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  unsigned char Uom;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  double DisplacementX1;
  double DisplacementY1;
  double PerpendicularOffset1;
  bool EnableFill1;
  double Fill1Opacity;
  bool HasGraphicFill1;
  wxString Fill1Color;
  wxString Fill1XLinkHref;
  wxString Fill1MimeType;
  bool EnableFill1Replacement;
  wxString Fill1ColorReplacement;
  wxGrid *GridCtrl1;
  bool EnableStroke1;
  double Stroke1Opacity;
  bool HasGraphicStroke1;
  wxString Stroke1Color;
  wxString Stroke1XLinkHref;
  wxString Stroke1MimeType;
  bool EnableStroke1Replacement;
  wxString Stroke1ColorReplacement;
  ExternalGraphicList *List;
  wxGrid *GridCtrl2;
  double Stroke1Width;
  int Stroke1LineJoin;
  int Stroke1LineCap;
  int Stroke1DashCount;
  double *Stroke1DashArray;
  double Stroke1DashOffset;
  bool EnablePolygon2;
  double DisplacementX2;
  double DisplacementY2;
  double PerpendicularOffset2;
  bool EnableFill2;
  wxString Fill2MimeType;
  bool EnableFill2Replacement;
  wxString Fill2ColorReplacement;
  double Fill2Opacity;
  bool HasGraphicFill2;
  wxString Fill2Color;
  wxString Fill2XLinkHref;
  wxGrid *GridCtrl3;
  bool EnableStroke2;
  double Stroke2Opacity;
  bool HasGraphicStroke2;
  wxString Stroke2Color;
  wxString Stroke2XLinkHref;
  wxString Stroke2MimeType;
  bool EnableStroke2Replacement;
  wxString Stroke2ColorReplacement;
  wxGrid *GridCtrl4;
  double Stroke2Width;
  int Stroke2LineJoin;
  int Stroke2LineCap;
  int Stroke2DashCount;
  double *Stroke2DashArray;
  double Stroke2DashOffset;
  unsigned char PreviewBackground;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreateStroke1Page(wxWindow * book);
  wxPanel *CreateFill1Page(wxWindow * book);
  wxPanel *CreateStroke2Page(wxWindow * book);
  wxPanel *CreateFill2Page(wxWindow * book);
  wxPanel *CreatePreviewPage(wxWindow * book);
  void CreateButtons();
  bool FinalValidityCheck();
  bool RetrieveMainPage();
  bool RetrieveStroke1Page(bool check = true);
  bool RetrieveFill1Page(bool check = true);
  bool RetrieveStroke2Page(bool check = true);
  bool RetrieveFill2Page(bool check = true);
  bool RetrievePreviewPage();
  void UpdateMainPage();
  void UpdateStroke1Page();
  void UpdateFill1Page();
  void UpdateStroke2Page();
  void UpdateFill2Page();
  void UpdatePreviewPage();
  char *DoCreateSymbolizerXML();
  char *DoCreateFeatureTypeXML();
  void DrawPreview(int horz, int vert);
  bool DoParseDashArray(wxString & str, int which);
  void NormalizedDashArray(wxString & str, int which, char delimiter = ',');
  void PreparePolygonPath(void *ctx, double perpendicular_offset,
                          double displacement_x, double displacement_y);
  wxBitmap PreviewBackBitmap;
public:
    SimplePolygonSymbolizerDialog();
  bool Create(MyFrame * parent);
    virtual ~ SimplePolygonSymbolizerDialog();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdUomChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdStroke1TypeChanged(wxCommandEvent & event);
  void OnCmdFill1TypeChanged(wxCommandEvent & event);
  void OnCmdStroke2TypeChanged(wxCommandEvent & event);
  void OnCmdFill2TypeChanged(wxCommandEvent & event);
  void OnCmdColor1Changed(wxCommandEvent & event);
  void OnCmdColor2Changed(wxCommandEvent & event);
  void OnCmdColor3Changed(wxCommandEvent & event);
  void OnCmdColor4Changed(wxCommandEvent & event);
  void OnCmdColor1Picker(wxCommandEvent & event);
  void OnCmdColor2Picker(wxCommandEvent & event);
  void OnCmdColor3Picker(wxCommandEvent & event);
  void OnCmdColor4Picker(wxCommandEvent & event);
  void OnCmdFill1ReplacementChanged(wxCommandEvent & event);
  void OnCmdStroke1ReplacementChanged(wxCommandEvent & event);
  void OnCmdFill2ReplacementChanged(wxCommandEvent & event);
  void OnCmdStroke2ReplacementChanged(wxCommandEvent & event);
  void OnCmdLineJoin1Changed(wxCommandEvent & event);
  void OnCmdLineJoin2Changed(wxCommandEvent & event);
  void OnCmdLineCap1Changed(wxCommandEvent & event);
  void OnCmdLineCap2Changed(wxCommandEvent & event);
  void OnCmdStroke1Changed(wxCommandEvent & event);
  void OnCmdStroke2Changed(wxCommandEvent & event);
  void OnCmdFill1Changed(wxCommandEvent & event);
  void OnCmdFill2Changed(wxCommandEvent & event);
  void OnCmdFill1EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdStroke1EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdFill2EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdStroke2EnableReplacementChanged(wxCommandEvent & event);
  void OnCmdPolygon2Changed(wxCommandEvent & event);
  void OnCmdBackgroundChanged(wxCommandEvent & event);
};

class SimplePointSymbolizerDialog:public wxPropertySheetDialog
{
//
// a dialog for creating an SLD/SE PointSymbolizer 
// of the simple type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  unsigned char Uom;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  double Opacity;
  double Size;
  double Rotation;
  double AnchorPointX;
  double AnchorPointY;
  double DisplacementX;
  double DisplacementY;
  bool OnlyRescaleSVG;
  bool HasGraphic;
  wxString XLinkHref;
  wxString MimeType;
  bool EnableColorReplacement;
  wxString ColorReplacement;
  int WellKnownMark;
  bool EnableFill;
  bool EnableStroke;
  wxString FillColor;
  wxString StrokeColor;
  double StrokeWidth;
  int StrokeLineJoin;
  int StrokeLineCap;
  int StrokeDashCount;
  double *StrokeDashArray;
  double StrokeDashOffset;
  ExternalGraphicList *List;
  wxGrid *GridCtrl;
  unsigned char PreviewBackground;
  bool Crosshair;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreatePositionPage(wxWindow * book);
  wxPanel *CreateGraphicPage(wxWindow * book);
  wxPanel *CreateMarkPage(wxWindow * book);
  wxPanel *CreatePreviewPage(wxWindow * book);
  void CreateButtons();
  bool FinalValidityCheck();
  bool RetrieveMainPage();
  bool RetrievePositionPage(bool check = true);
  bool RetrieveGraphicPage(bool check = true);
  bool RetrieveMarkPage(bool check = true);
  bool RetrievePreviewPage();
  void UpdateMainPage();
  void UpdatePositionPage();
  void UpdateGraphicPage();
  void UpdateMarkPage();
  void UpdatePreviewPage();
  char *DoCreateSymbolizerXML();
  char *DoCreateFeatureTypeXML();
  void DrawPreview(int horz, int vert);
  bool DoParseDashArray(wxString & str);
  void NormalizedDashArray(wxString & str, char delimiter = ',');
  wxBitmap PreviewBackBitmap;
public:
    SimplePointSymbolizerDialog();
  bool Create(MyFrame * parent);
    virtual ~ SimplePointSymbolizerDialog();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdUomChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdTypeChanged(wxCommandEvent & event);
  void OnCmdMarkChanged(wxCommandEvent & event);
  void OnCmdStrokeChanged(wxCommandEvent & event);
  void OnCmdFillChanged(wxCommandEvent & event);
  void OnCmdColorFillChanged(wxCommandEvent & event);
  void OnCmdColorStrokeChanged(wxCommandEvent & event);
  void OnCmdColorReplacementChanged(wxCommandEvent & event);
  void OnCmdColorFillPicker(wxCommandEvent & event);
  void OnCmdColorStrokePicker(wxCommandEvent & event);
  void OnCmdColorReplacementPicker(wxCommandEvent & event);
  void OnCmdOnlyRescaleSVG(wxCommandEvent & event);
  void OnCmdLineJoinChanged(wxCommandEvent & event);
  void OnCmdLineCapChanged(wxCommandEvent & event);
  void OnCmdEnableReplacementChanged(wxCommandEvent & event);
  void OnCmdBackgroundChanged(wxCommandEvent & event);
  void OnCmdCrosshairChanged(wxCommandEvent & event);
};

class SimpleTextSymbolizerDialog:public wxPropertySheetDialog
{
//
// a dialog for creating an SLD/SE TextSymbolizer 
// of the simple type
//
private:
  MyFrame * MainFrame;
  wxString Name;
  wxString Title;
  wxString Abstract;
  unsigned char Uom;
  bool MinScale;
  bool MaxScale;
  double MinScaleDenominator;
  double MaxScaleDenominator;
  wxString Label;
  wxString FontFamily;
  int FontStyle;
  int FontWeight;
  double FontSize;
  bool PointPlacement;
  double Rotation;
  double AnchorPointX;
  double AnchorPointY;
  double DisplacementX;
  double DisplacementY;
  double PerpendicularOffset;
  bool IsRepeated;
  double InitialGap;
  double Gap;
  bool IsAligned;
  bool GeneralizeLine;
  bool HasHalo;
  double HaloRadius;
  wxString HaloColor;
  double HaloOpacity;
  wxString FillColor;
  double FillOpacity;
  TextFontList *List;
  wxGrid *GridCtrl;
  unsigned char PreviewBackground;
  bool Crosshair;
  bool ReferenceLine;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreateFontPage(wxWindow * book);
  wxPanel *CreatePlacementPage(wxWindow * book);
  wxPanel *CreatePreviewPage(wxWindow * book);
  void CreateButtons();
  bool FinalValidityCheck();
  bool RetrieveMainPage();
  bool RetrieveFontPage(bool check = true);
  bool RetrievePlacementPage(bool check = true);
  bool RetrievePreviewPage();
  void UpdateMainPage();
  void UpdateFontPage();
  void UpdatePlacementPage();
  void UpdatePreviewPage();
  char *DoCreateSymbolizerXML();
  char *DoCreateFeatureTypeXML();
  gaiaGeomCollPtr PrepareLinestring(double perpendicular_offset);
  void PrepareLinestringPath(void *ctx, double perpendicular_offset);
  void GetLineCenterPoint(double perpendicular_offset, double *x, double *y);
  void CreateLineArray(double perpendicular_offset, int *points, double **x,
                       double **y, int generalize);
  void DrawPreview(int horz, int vert);
  wxBitmap PreviewBackBitmap;
public:
    SimpleTextSymbolizerDialog();
  bool Create(MyFrame * parent);
    virtual ~ SimpleTextSymbolizerDialog();
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdUomChanged(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdTypeChanged(wxCommandEvent & event);
  void OnCmdColorFillChanged(wxCommandEvent & event);
  void OnCmdHaloEnableChanged(wxCommandEvent & event);
  void OnCmdColorHaloChanged(wxCommandEvent & event);
  void OnCmdColorFillPicker(wxCommandEvent & event);
  void OnCmdColorHaloPicker(wxCommandEvent & event);
  void OnCmdIsRepeatedChanged(wxCommandEvent & event);
  void OnCmdIsAlignedChanged(wxCommandEvent & event);
  void OnCmdGeneralizeLineChanged(wxCommandEvent & event);
  void OnCmdBackgroundChanged(wxCommandEvent & event);
  void OnCmdCrosshairChanged(wxCommandEvent & event);
  void OnCmdReferenceLineChanged(wxCommandEvent & event);
};

class WmsCatalogObject:public wxTreeItemData
{
//
// a class to store TreeItemData - WMS Catalog wrapper
//
private:
  rl2WmsLayerPtr LayerHandle;
  const char *LayerName;
public:
    WmsCatalogObject(const char *lyr)
  {
    LayerHandle = NULL;
    LayerName = lyr;
  }
  WmsCatalogObject(rl2WmsLayerPtr handle, const char *lyr)
  {
    LayerHandle = handle;
    LayerName = lyr;
  }
  virtual ~ WmsCatalogObject()
  {;
  }
  rl2WmsLayerPtr GetHandle()
  {
    return LayerHandle;
  }
  const char *GetLayerName()
  {
    return LayerName;
  }
};

class WmsCatalogTree:public wxTreeCtrl
{
//
// a tree-control used for WMS Service Layers
//
private:
  class WmsDialog * MainDialog;
  wxTreeItemId Root;            // the root node
  wxImageList *Images;          // the images list
  wxTreeItemId CurrentItem;     // the tree item holding the current context menu
  void ExpandChildren(wxTreeItemId item, rl2WmsLayerPtr handle);
public:
    WmsCatalogTree()
  {;
  }
  WmsCatalogTree(WmsDialog * parent, wxSize sz, wxWindowID id = wxID_ANY);
  virtual ~ WmsCatalogTree();
  void FlushAll()
  {
    DeleteAllItems();
    Root = wxTreeItemId();
  }
  void AddLayer(rl2WmsLayerPtr handle, const char *layer);
  void AddTiledRoot(const char *name);
  void AddLayer(wxTreeItemId parent, rl2WmsLayerPtr handle, const char *layer);
  void SetLayerIcons();
  void MarkCurrentItem();
  void RootAutoSelect(void);
  void OnSelChanged(wxTreeEvent & event);
  void OnCmdShowAll(wxCommandEvent & event);
  void OnCmdHideAll(wxCommandEvent & event);
};

class WmsDialog:public wxDialog
{
//
// a dialog for selecting a WMS datasource
//
private:
  MyFrame * MainFrame;
  wxString URL;
  WmsCatalogTree *WmsTree;
  rl2WmsCatalogPtr Catalog;
  rl2WmsLayerPtr CurrentLayer;
  int MaxWidth;
  int MaxHeight;
  char *Version;
  int SwapXY;
  bool ProxyEnabled;
  wxString HttpProxy;
  char *NormalizeUrl(const char *url);
  void DestroyNormalizedUrl(char *url);
public:
    WmsDialog()
  {
    Catalog = NULL;
    WmsTree = NULL;
    Version = NULL;
    CurrentLayer = NULL;
    ProxyEnabled = false;
  }
  virtual ~ WmsDialog()
  {
    if (Catalog != NULL)
      destroy_wms_catalog(Catalog);
    if (Version != NULL)
      delete[]Version;
  }
  bool Create(MyFrame * parent, wxString & proxy);
  void CreateControls();
  void SelectLayer(void);
  void SelectLayer(rl2WmsLayerPtr layer);
  int IsTiled();
  int GetTileWidth();
  int GetTileHeight();
  rl2WmsLayerPtr GetLayerHandle()
  {
    return CurrentLayer;
  }
  int IsQueryable()
  {
    if (is_wms_layer_queryable(CurrentLayer) > 0)
      return 1;
    return 0;
  }
  int IsOpaque();
  wxString & GetURL()
  {
    return URL;
  }
  const char *GetServiceVersion()
  {
    return get_wms_version(Catalog);
  }
  const char *GetServiceName()
  {
    return get_wms_name(Catalog);
  }
  const char *GetServiceTitle()
  {
    return get_wms_title(Catalog);
  }
  const char *GetServiceAbstract()
  {
    return get_wms_abstract(Catalog);
  }
  const char *GetURL_GetMap_Get()
  {
    return get_wms_url_GetMap_get(Catalog);
  }
  const char *GetURL_GetMap_Post()
  {
    return get_wms_url_GetMap_post(Catalog);
  }
  const char *GetURL_GetFeatureInfo_Get()
  {
    return get_wms_url_GetFeatureInfo_get(Catalog);
  }
  const char *GetURL_GetFeatureInfo_Post()
  {
    return get_wms_url_GetFeatureInfo_post(Catalog);
  }
  const char *GetGmlMimeType()
  {
    return get_wms_gml_mime_type(Catalog);
  }
  const char *GetXmlMimeType()
  {
    return get_wms_xml_mime_type(Catalog);
  }
  const char *GetContactPerson()
  {
    return get_wms_contact_person(Catalog);
  }
  const char *GetContactOrganization()
  {
    return get_wms_contact_organization(Catalog);
  }
  const char *GetContactPosition()
  {
    return get_wms_contact_position(Catalog);
  }
  const char *GetPostalAddress()
  {
    return get_wms_contact_postal_address(Catalog);
  }
  const char *GetCity()
  {
    return get_wms_contact_city(Catalog);
  }
  const char *GetStateProvince()
  {
    return get_wms_contact_state_province(Catalog);
  }
  const char *GetPostCode()
  {
    return get_wms_contact_post_code(Catalog);
  }
  const char *GetCountry()
  {
    return get_wms_contact_country(Catalog);
  }
  const char *GetVoiceTelephone()
  {
    return get_wms_contact_voice_telephone(Catalog);
  }
  const char *GetFaxTelephone()
  {
    return get_wms_contact_fax_telephone(Catalog);
  }
  const char *GetEMailAddress()
  {
    return get_wms_contact_email_address(Catalog);
  }
  const char *GetFees()
  {
    return get_wms_fees(Catalog);
  }
  const char *GetAccessConstraints()
  {
    return get_wms_access_constraints(Catalog);
  }
  int GetLayerLimit()
  {
    return get_wms_layer_limit(Catalog);
  }
  int GetMaxWidth()
  {
    return get_wms_max_width(Catalog);
  }
  int GetMaxHeight()
  {
    return get_wms_max_height(Catalog);
  }
  const char *GetName()
  {
    return get_wms_layer_name(CurrentLayer);
  }
  const char *GetTitle()
  {
    return get_wms_layer_title(CurrentLayer);
  }
  const char *GetAbstract()
  {
    return get_wms_layer_abstract(CurrentLayer);
  }
  const char *GetVersion();
  const char *GetStyleName();
  const char *GetStyleTitle();
  const char *GetStyleAbstract();
  const char *GetFormat();
  const char *GetCRS();
  int IsSwapXY()
  {
    return SwapXY;
  }
  double GetMinX();
  double GetMaxX();
  double GetMinY();
  double GetMaxY();
  double GetGeoMinX();
  double GetGeoMaxX();
  double GetGeoMinY();
  double GetGeoMaxY();
  double GetMinScaleDenominator()
  {
    return get_wms_layer_min_scale_denominator(CurrentLayer);
  }
  double GetMaxScaleDenominator()
  {
    return get_wms_layer_max_scale_denominator(CurrentLayer);
  }
  int CountFormats()
  {
    return get_wms_format_count(Catalog, 1);
  }
  const char *GetFormatByIndex(int idx)
  {
    return get_wms_format(Catalog, idx, 1);
  }
  int CountStyles()
  {
    return get_wms_layer_style_count(CurrentLayer);
  }
  bool GetStyleByIndex(int idx, const char **name, const char **title,
                       const char **abstract);
  int CountCRS()
  {
    return get_wms_layer_crs_count(CurrentLayer);
  }
  const char *GetCrsByIndex(int idx)
  {
    return get_wms_layer_crs(CurrentLayer, idx);
  }
  wxString & GetHttpProxy()
  {
    return HttpProxy;
  }
  void PrepareCatalog(void);
  void UpdateSwapXY(void);
  void OnProxy(wxCommandEvent & event);
  void OnCrsChanged(wxCommandEvent & event);
  void OnVersionChanged(wxCommandEvent & event);
  void OnSwapXYChanged(wxCommandEvent & event);
  void OnTiledChanged(wxCommandEvent & event);
  void OnCatalog(wxCommandEvent & event);
  void OnReset(wxCommandEvent & event);
  void OnWmsServer(wxCommandEvent & event);
  void OnWmsOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class WmsServersDialog:public wxDialog
{
//
// a dialog for selecting a registered WMS server
//
private:
  MyFrame * MainFrame;
  wxListBox *WmsServers;
  wxString URL;
  int Count;
  wxString *List;
  void PopulateList();
public:
    WmsServersDialog()
  {;
  }
  virtual ~ WmsServersDialog()
  {
    if (List != NULL)
      delete[]List;
  }
  bool Create(MyFrame * grandparent);
  void CreateControls();
  wxString & GetURL()
  {
    return URL;
  }
  void OnUrlSelected(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class WmsLayerDialog:public wxDialog
{
//
// a dialog for selecting a registered WMS layer 
//
private:
  MyFrame * MainFrame;
  int CurrentEvtRow;
  int CurrentEvtColumn;
  wxGrid *WmsLayers;
  wxString URL;
  wxString LayerName;
  int Count;
  wxString *ListUrl;
  wxString *ListLayer;
  void PopulateList();
public:
    WmsLayerDialog()
  {;
  }
  virtual ~ WmsLayerDialog()
  {
    if (ListUrl != NULL)
      delete[]ListUrl;
    if (ListLayer != NULL)
      delete[]ListLayer;
  }
  bool Create(MyFrame * parent);
  void CreateControls();
  wxString & GetURL()
  {
    return URL;
  }
  wxString & GetLayerName()
  {
    return LayerName;
  }
  void SelectWmsLayer();
  void OnLeftClick(wxGridEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class WmsLayerUnregisterDialog:public wxDialog
{
//
// a dialog supporting Unregister WMS Layer
//
private:
  MyFrame * MainFrame;
  wxString URL;
  wxString LayerName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  wxString DataLicense;
public:
    WmsLayerUnregisterDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & url, wxString & name,
              wxString & title, wxString & abstract, wxString & copyright,
              wxString & data_license);
  virtual ~ WmsLayerUnregisterDialog()
  {;
  }
  void CreateControls();
  bool DoWmsLayerUnregister(void);
  void OnOk(wxCommandEvent & event);
};

class WmsLayerInfosDialog:public wxDialog
{
//
// a dialog for editing Vector Coverage Data
//
private:
  MyFrame * MainFrame;
  wxString URL;
  wxString LayerName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  int LicenseID;
  wxString DataLicense;
  wxString GetFeatureInfoUrl;
  bool IsQueryable;
  void DoUpdateWmsLayer();
  void PopulateDataLicenses(wxComboBox * licenseCtrl);
public:
    WmsLayerInfosDialog()
  {;
  }
  bool Create(MyFrame * parent, wxString & url, wxString & name,
              wxString & title, wxString & abstract, wxString & copyright,
              int license_id, bool is_queryable, wxString & getfeatureinfo_url);
  virtual ~ WmsLayerInfosDialog()
  {;
  }
  void CreateControls();
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
  void OnQueryableChanged(wxCommandEvent & event);
};

class WmsLayerConfigDialog:public wxDialog
{
//
// a dialog for configuring a WMS Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  class MapLayer *Layer;
  wxString Url;
  wxString InfoUrl;
  wxString DbPrefix;
  wxString LayerName;
  char *Version;
  char *MaxVersion;
  char *RefSys;
  char *Style;
  char *ImageFormat;
  char *BgColor;
  bool BgColorEnabled;
  int Opaque;
  int SwapXY;
  int Cached;
  int Tiled;
  int TileWidth;
  int TileHeight;
  bool IsBBoxChanged;
  bool IsConfigChanged;
  void GetButtonBitmap(const char *color, wxBitmap & bmp);
  void ParseBgColor(const char *color, unsigned char *red, unsigned char *green,
                    unsigned char *blue);
  unsigned char ParseHex(const char *byte);
  void DoConfigureWmsLayer();
  void DoConfigureMapLayer();
public:
    WmsLayerConfigDialog()
  {;
  }
  virtual ~ WmsLayerConfigDialog();
  bool Create(MyFrame * parent, wxString url, wxString layer);
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void LoadData();
  void InitData();
  void FindMaxVersion();
  void PopulateRefSys(wxComboBox * crsList);
  void PopulateImageFormats(wxComboBox * fmtList);
  void PopulateStyles(wxComboBox * stlList);
  void UpdateSwapXY(void);
  bool BBoxChanged()
  {
    return IsBBoxChanged;
  }
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnCrsChanged(wxCommandEvent & event);
  void OnVersionChanged(wxCommandEvent & event);
  void OnSwapXYChanged(wxCommandEvent & event);
  void OnCachedChanged(wxCommandEvent & event);
  void OnTiledChanged(wxCommandEvent & event);
  void OnBgColorEnabledChanged(wxCommandEvent & event);
  void OnBgColorChanged(wxCommandEvent & event);
  void OnWmsOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class RasterLayerConfigDialog:public wxDialog
{
//
// a dialog for configuring a Raster Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  int Srid;
  char *Style;
  bool IsBBoxChanged;
  bool IsConfigChanged;
  void DoConfigureMapLayer();
public:
    RasterLayerConfigDialog()
  {;
  }
  virtual ~ RasterLayerConfigDialog()
  {
    if (Style != NULL)
      free(Style);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void InitData();
  void PopulateSRIDs(wxComboBox * sridList);
  void PopulateStyles(wxComboBox * stlList);
  bool BBoxChanged()
  {
    return IsBBoxChanged;
  }
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class VectorLayerConfigDialog:public wxDialog
{
//
// a dialog for configuring a Vector Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  int Srid;
  int GeometryType;
  char *Style;
  bool IsBBoxChanged;
  bool IsConfigChanged;
  void DoConfigureMapLayer();
public:
    VectorLayerConfigDialog()
  {;
  }
  virtual ~ VectorLayerConfigDialog()
  {
    if (Style != NULL)
      free(Style);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void InitData();
  int DoGuessVectorLayerType();
  void PopulateSRIDs(wxComboBox * sridList);
  void PopulateViewSRIDs(wxComboBox * sridList);
  void PopulateVirtualTableSRIDs(wxComboBox * sridList);
  void PopulateStyles(wxComboBox * stlList);
  bool BBoxChanged()
  {
    return IsBBoxChanged;
  }
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class VectorSqlSampleDialog:public wxDialog
{
//
// a dialog for configuring an SQL Map Request
// based on some Vector Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  char *Style;
  wxColour BgColor;
  wxString Sql;
  void GetButtonBitmap(wxColour & bgcolor, wxBitmap & bmp);
  void DoUpdateSql();
public:
    VectorSqlSampleDialog()
  {;
  }
  virtual ~ VectorSqlSampleDialog()
  {
    if (Style != NULL)
      free(Style);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void InitData();
  void PopulateStyles(wxComboBox * stlList);
  void OnStyleChanged(wxCommandEvent & event);
  void OnMimeTypeChanged(wxCommandEvent & event);
  void OnQualityChanged(wxCommandEvent & event);
  void OnTransparentChanged(wxCommandEvent & event);
  void OnBackgroundChanged(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class RasterSqlSampleDialog:public wxDialog
{
//
// a dialog for configuring an SQL Map Request
// based on some Raster Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  char *Style;
  wxColour BgColor;
  wxString Sql;
  void GetButtonBitmap(wxColour & bgcolor, wxBitmap & bmp);
  void DoUpdateSql();
public:
    RasterSqlSampleDialog()
  {;
  }
  virtual ~ RasterSqlSampleDialog()
  {
    if (Style != NULL)
      free(Style);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void InitData();
  void PopulateStyles(wxComboBox * stlList);
  void OnStyleChanged(wxCommandEvent & event);
  void OnMimeTypeChanged(wxCommandEvent & event);
  void OnQualityChanged(wxCommandEvent & event);
  void OnTransparentChanged(wxCommandEvent & event);
  void OnBackgroundChanged(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class WmsSqlSampleDialog:public wxDialog
{
//
// a dialog for configuring an SQL Map Request
// based on some WMS Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  bool RequestURL;
  wxString Url;
  wxString DbPrefix;
  wxString LayerName;
  char *Version;
  char *MaxVersion;
  char *Style;
  char *ImageFormat;
  char *BgColor;
  int Transparent;
  wxString Sql;
  void GetButtonBitmap(const char *color, wxBitmap & bmp);
  void ParseBgColor(const char *color, unsigned char *red, unsigned char *green,
                    unsigned char *blue);
  unsigned char ParseHex(const char *byte);
  void DoUpdateSql();
  void DoUpdateUrl();
  int CheckMarker(wxString & url);
  int DoQueryWmsCoverage(const char *db_prefix, const char *cvg_name, int srid,
                         char **url, int *swap_axes);
public:
    WmsSqlSampleDialog()
  {;
  }
  virtual ~ WmsSqlSampleDialog();
  bool Create(MyMapPanel * parent, MapLayer * layer, bool request_url = false);
  void CreateControls();
  void InitData();
  void FindMaxVersion();
  void PopulateImageFormats(wxComboBox * fmtList);
  void PopulateStyles(wxComboBox * stlList);
  void OnStyleChanged(wxCommandEvent & event);
  void OnMimeTypeChanged(wxCommandEvent & event);
  void OnVersionChanged(wxCommandEvent & event);
  void OnTransparentChanged(wxCommandEvent & event);
  void OnBgColorChanged(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class TopologyLayerConfigDialog:public wxDialog
{
//
// a dialog for configuring a TopoGeo Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  int Srid;
  char *Style;
  bool IsBBoxChanged;
  bool IsConfigChanged;
  void DoConfigureMapLayer();
public:
    TopologyLayerConfigDialog()
  {;
  }
  virtual ~ TopologyLayerConfigDialog()
  {
    if (Style != NULL)
      free(Style);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void InitData();
  void PopulateSRIDs(wxComboBox * sridList);
  void PopulateStyles(wxComboBox * stlList);
  bool BBoxChanged()
  {
    return IsBBoxChanged;
  }
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class NetworkLayerConfigDialog:public wxDialog
{
//
// a dialog for configuring a TopoNet Layer
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  int Srid;
  char *Style;
  bool IsBBoxChanged;
  bool IsConfigChanged;
  void DoConfigureMapLayer();
public:
    NetworkLayerConfigDialog()
  {;
  }
  virtual ~ NetworkLayerConfigDialog()
  {
    if (Style != NULL)
      free(Style);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateControls();
  void InitData();
  void PopulateSRIDs(wxComboBox * sridList);
  void PopulateStyles(wxComboBox * stlList);
  bool BBoxChanged()
  {
    return IsBBoxChanged;
  }
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
};

class QuickStyleObj
{
//
// a class wrapping a Quick Style
//
private:
  char UUID[64];
  int Type;
  bool MinScaleEnabled;
  bool MaxScaleEnabled;
  double ScaleMin;
  double ScaleMax;
  double SymbolOpacity;
  double SymbolSize;
  double SymbolRotation;
  double SymbolAnchorX;
  double SymbolAnchorY;
  double SymbolDisplacementX;
  double SymbolDisplacementY;
  int SymbolWellKnownMark;
  char SymbolFillColor[8];
  char SymbolStrokeColor[8];
  double LineOpacity;
  double LinePerpendicularOffset;
  double LineStrokeWidth;
  char LineStrokeColor[8];
  int LineDotStyle;
  bool Line2Enabled;
  double Line2StrokeWidth;
  char Line2StrokeColor[8];
  int Line2DotStyle;
  bool PolygonFill;
  bool PolygonStroke;
  double PolygonDisplacementX;
  double PolygonDisplacementY;
  double PolygonPerpendicularOffset;
  double PolygonFillOpacity;
  char PolygonFillColor[8];
  bool PolygonSolidFill;
  int PolygonFillBrushId;
  double PolygonStrokeOpacity;
  double PolygonStrokeWidth;
  char PolygonStrokeColor[8];
  bool LabelPrint;
  bool DontPaintGeomSymbolizer;
  bool LabelLinePlacement;
  char *LabelColumn;
  char *FontFacename;
  double FontSize;
  int FontStyle;
  int FontWeight;
  double FontOpacity;
  char FontColor[8];
  bool HasHalo;
  double HaloRadius;
  char HaloColor[8];
  double HaloOpacity;
  double LabelAnchorX;
  double LabelAnchorY;
  double LabelDisplacementX;
  double LabelDisplacementY;
  double LabelRotation;
  double LabelPerpendicularOffset;
  bool RepeatedLabel;
  double LabelInitialGap;
  double LabelGap;
  bool LabelIsAligned;
  bool LabelGeneralizeLine;
  char *XmlStyle;
  char *DoCreateFeatureTypeXML();
  char *DoCreateSymbolizerXML(bool subordered);
  char *DoCreatePointSymbolizerXML(bool subordered);
  char *DoCreateLineSymbolizerXML(bool subordered);
  char *DoCreatePolygonSymbolizerXML(bool subordered);
  char *DoCreateTextPointSymbolizerXML();
  char *DoCreateTextLineSymbolizerXML();
public:
    QuickStyleObj(int Type);
   ~QuickStyleObj()
  {
    if (LabelColumn)
      free(LabelColumn);
    if (FontFacename)
      free(FontFacename);
    if (XmlStyle)
      sqlite3_free(XmlStyle);
  }
  QuickStyleObj *Clone();
  bool Compare(QuickStyleObj * style);
  const char *GetUUID()
  {
    return UUID;
  }
  int GetType()
  {
    return Type;
  }
  void EnableMinScale(bool mode)
  {
    MinScaleEnabled = mode;
  }
  bool IsMinScaleEnabled()
  {
    return MinScaleEnabled;
  }
  void EnableMaxScale(bool mode)
  {
    MaxScaleEnabled = mode;
  }
  bool IsMaxScaleEnabled()
  {
    return MaxScaleEnabled;
  }
  void SetScaleMin(double x)
  {
    ScaleMin = x;
  }
  double GetScaleMin()
  {
    return ScaleMin;
  }
  void SetScaleMax(double x)
  {
    ScaleMax = x;
  }
  double GetScaleMax()
  {
    return ScaleMax;
  }
  void SetSymbolOpacity(double x)
  {
    SymbolOpacity = x;
  }
  double GetSymbolOpacity()
  {
    return SymbolOpacity;
  }
  void SetSymbolSize(double x)
  {
    SymbolSize = x;
  }
  double GetSymbolSize()
  {
    return SymbolSize;
  }
  void SetSymbolRotation(double x)
  {
    SymbolRotation = x;
  }
  double GetSymbolRotation()
  {
    return SymbolRotation;
  }
  void SetSymbolAnchorX(double x)
  {
    SymbolAnchorX = x;
  }
  double GetSymbolAnchorX()
  {
    return SymbolAnchorX;
  }
  void SetSymbolAnchorY(double x)
  {
    SymbolAnchorY = x;
  }
  double GetSymbolAnchorY()
  {
    return SymbolAnchorY;
  }
  void SetSymbolDisplacementX(double x)
  {
    SymbolDisplacementX = x;
  }
  double GetSymbolDisplacementX()
  {
    return SymbolDisplacementX;
  }
  void SetSymbolDisplacementY(double x)
  {
    SymbolDisplacementY = x;
  }
  double GetSymbolDisplacementY()
  {
    return SymbolDisplacementY;
  }
  void SetSymbolWellKnownMark(int x)
  {
    SymbolWellKnownMark = x;
  }
  int GetSymbolWellKnownMark()
  {
    return SymbolWellKnownMark;
  }
  void SetSymbolFillColor(const char *x)
  {
    strcpy(SymbolFillColor, x);
  }
  const char *GetSymbolFillColor()
  {
    return SymbolFillColor;
  }
  void SetSymbolStrokeColor(const char *x)
  {
    strcpy(SymbolStrokeColor, x);
  }
  const char *GetSymbolStrokeColor()
  {
    return SymbolStrokeColor;
  }
  void SetLineOpacity(double x)
  {
    LineOpacity = x;
  }
  double GetLineOpacity()
  {
    return LineOpacity;
  }
  void SetLinePerpendicularOffset(double x)
  {
    LinePerpendicularOffset = x;
  }
  double GetLinePerpendicularOffset()
  {
    return LinePerpendicularOffset;
  }
  void SetLineStrokeWidth(double x)
  {
    LineStrokeWidth = x;
  }
  double GetLineStrokeWidth()
  {
    return LineStrokeWidth;
  }
  void SetLineStrokeColor(const char *x)
  {
    strcpy(LineStrokeColor, x);
  }
  const char *GetLineStrokeColor()
  {
    return LineStrokeColor;
  }
  void SetLineDotStyle(int x)
  {
    LineDotStyle = x;
  }
  int GetLineDotStyle()
  {
    return LineDotStyle;
  }
  void SetLine2Enabled(bool x)
  {
    Line2Enabled = x;
  }
  bool IsLine2Enabled()
  {
    return Line2Enabled;
  }
  void SetLine2StrokeWidth(double x)
  {
    Line2StrokeWidth = x;
  }
  double GetLine2StrokeWidth()
  {
    return Line2StrokeWidth;
  }
  void SetLine2StrokeColor(const char *x)
  {
    strcpy(Line2StrokeColor, x);
  }
  const char *GetLine2StrokeColor()
  {
    return Line2StrokeColor;
  }
  void SetLine2DotStyle(int x)
  {
    Line2DotStyle = x;
  }
  int GetLine2DotStyle()
  {
    return Line2DotStyle;
  }
  void SetPolygonFill(bool x)
  {
    PolygonFill = x;
  }
  bool IsPolygonFill()
  {
    return PolygonFill;
  }
  void SetPolygonStroke(bool x)
  {
    PolygonStroke = x;
  }
  bool IsPolygonStroke()
  {
    return PolygonStroke;
  }
  void SetPolygonDisplacementX(double x)
  {
    PolygonDisplacementX = x;
  }
  double GetPolygonDisplacementX()
  {
    return PolygonDisplacementX;
  }
  void SetPolygonDisplacementY(double x)
  {
    PolygonDisplacementY = x;
  }
  double GetPolygonDisplacementY()
  {
    return PolygonDisplacementY;
  }
  void SetPolygonPerpendicularOffset(double x)
  {
    PolygonPerpendicularOffset = x;
  }
  double GetPolygonPerpendicularOffset()
  {
    return PolygonPerpendicularOffset;
  }
  void SetPolygonFillOpacity(double x)
  {
    PolygonFillOpacity = x;
  }
  double GetPolygonFillOpacity()
  {
    return PolygonFillOpacity;
  }
  void SetPolygonFillColor(const char *x)
  {
    strcpy(PolygonFillColor, x);
  }
  const char *GetPolygonFillColor()
  {
    return PolygonFillColor;
  }
  void SetPolygonSolidFill(bool mode)
  {
    PolygonSolidFill = mode;
  }
  bool IsPolygonSolidFill()
  {
    return PolygonSolidFill;
  }
  void SetPolygonFillBrushId(int brush_id)
  {
    PolygonFillBrushId = brush_id;
  }
  int GetPolygonFillBrushId()
  {
    return PolygonFillBrushId;
  }
  void SetPolygonStrokeOpacity(double x)
  {
    PolygonStrokeOpacity = x;
  }
  double GetPolygonStrokeOpacity()
  {
    return PolygonStrokeOpacity;
  }
  void SetPolygonStrokeWidth(double x)
  {
    PolygonStrokeWidth = x;
  }
  double GetPolygonStrokeWidth()
  {
    return PolygonStrokeWidth;
  }
  void SetPolygonStrokeColor(const char *x)
  {
    strcpy(PolygonStrokeColor, x);
  }
  const char *GetPolygonStrokeColor()
  {
    return PolygonStrokeColor;
  }
  void SetLabelPrint(bool mode)
  {
    LabelPrint = mode;
  }
  bool IsLabelPrint()
  {
    return LabelPrint;
  }
  void SetDontPaintGeomSymbolizer(bool mode)
  {
    DontPaintGeomSymbolizer = mode;
  }
  bool IsDontPaintGeomSymbolizer()
  {
    return DontPaintGeomSymbolizer;
  }
  void SetLabelLinePlacement(bool mode)
  {
    LabelLinePlacement = mode;
  }
  bool IsLabelLinePlacement()
  {
    return LabelLinePlacement;
  }
  void SetLabelColumn(const char *x);
  const char *GetLabelColumn()
  {
    return LabelColumn;
  }
  void SetFontFacename(const char *x);
  const char *GetFontFacename()
  {
    return FontFacename;
  }
  void SetFontSize(double x)
  {
    FontSize = x;
  }
  double GetFontSize()
  {
    return FontSize;
  }
  void SetFontStyle(int x)
  {
    FontStyle = x;
  }
  int GetFontStyle()
  {
    return FontStyle;
  }
  void SetFontWeight(int x)
  {
    FontWeight = x;
  }
  int GetFontWeight()
  {
    return FontWeight;
  }
  void SetFontOpacity(double x)
  {
    FontOpacity = x;
  }
  double GetFontOpacity()
  {
    return FontOpacity;
  }
  void SetFontColor(const char *x)
  {
    strcpy(FontColor, x);
  }
  const char *GetFontColor()
  {
    return FontColor;
  }
  bool IsHaloEnabled()
  {
    return HasHalo;
  }
  void EnableHalo(bool mode)
  {
    HasHalo = mode;
  }
  double GetHaloRadius()
  {
    return HaloRadius;
  }
  void SetHaloRadius(double x)
  {
    HaloRadius = x;
  }
  const char *GetHaloColor()
  {
    return HaloColor;
  }
  void SetHaloColor(const char *x)
  {
    strcpy(HaloColor, x);
  }
  double GetHaloOpacity()
  {
    return HaloOpacity;
  }
  void SetHaloOpacity(double x)
  {
    HaloOpacity = x;
  }
  void SetLabelRotation(double x)
  {
    LabelRotation = x;
  }
  double GetLabelRotation()
  {
    return LabelRotation;
  }
  void SetLabelAnchorX(double x)
  {
    LabelAnchorX = x;
  }
  double GetLabelAnchorX()
  {
    return LabelAnchorX;
  }
  void SetLabelAnchorY(double x)
  {
    LabelAnchorY = x;
  }
  double GetLabelAnchorY()
  {
    return LabelAnchorY;
  }
  void SetLabelDisplacementX(double x)
  {
    LabelDisplacementX = x;
  }
  double GetLabelDisplacementX()
  {
    return LabelDisplacementX;
  }
  void SetLabelDisplacementY(double x)
  {
    LabelDisplacementY = x;
  }
  double GetLabelDisplacementY()
  {
    return LabelDisplacementY;
  }
  void SetLabelPerpendicularOffset(double x)
  {
    LabelPerpendicularOffset = x;
  }
  double GetLabelPerpendicularOffset()
  {
    return LabelPerpendicularOffset;
  }
  void SetRepeatedLabel(bool mode)
  {
    RepeatedLabel = mode;
  }
  bool IsRepeatedLabel()
  {
    return RepeatedLabel;
  }
  void SetLabelInitialGap(double x)
  {
    LabelInitialGap = x;
  }
  double GetLabelInitialGap()
  {
    return LabelInitialGap;
  }
  void SetLabelGap(double x)
  {
    LabelGap = x;
  }
  double GetLabelGap()
  {
    return LabelGap;
  }
  void SetLabelIsAligned(bool mode)
  {
    LabelIsAligned = mode;
  }
  bool IsLabelAligned()
  {
    return LabelIsAligned;
  }
  void SetLabelGeneralizeLine(bool mode)
  {
    LabelGeneralizeLine = mode;
  }
  bool IsLabelGeneralizeLine()
  {
    return LabelGeneralizeLine;
  }
  char *CreateXmlStyle();
  void UpdateXmlStyle();
  const char *GetXmlStyle()
  {
    return XmlStyle;
  }
  unsigned char *CloneXmlStyle();
  static int RandomWellKnownMark();
  static void RandomColor(char *color);
  static void DoGetUUID(char *uuid);
};

class QuickStyleTopologyObj
{
//
// a class wrapping a Quick Style - Topology
//
private:
  char UUID[64];
  int Type;
  bool MinScaleEnabled;
  bool MaxScaleEnabled;
  double ScaleMin;
  double ScaleMax;
  double NodeOpacity;
  double NodeSize;
  double NodeRotation;
  double NodeAnchorX;
  double NodeAnchorY;
  double NodeDisplacementX;
  double NodeDisplacementY;
  int NodeWellKnownMark;
  char NodeFillColor[8];
  char NodeStrokeColor[8];
  double EdgeLinkOpacity;
  double EdgeLinkPerpendicularOffset;
  double EdgeLinkStrokeWidth;
  char EdgeLinkStrokeColor[8];
  int EdgeLinkDotStyle;
  bool FaceFill;
  bool FaceStroke;
  double FaceDisplacementX;
  double FaceDisplacementY;
  double FacePerpendicularOffset;
  double FaceFillOpacity;
  char FaceFillColor[8];
  double FaceStrokeOpacity;
  double FaceStrokeWidth;
  char FaceStrokeColor[8];
  double EdgeLinkSeedOpacity;
  double EdgeLinkSeedSize;
  double EdgeLinkSeedRotation;
  double EdgeLinkSeedAnchorX;
  double EdgeLinkSeedAnchorY;
  double EdgeLinkSeedDisplacementX;
  double EdgeLinkSeedDisplacementY;
  int EdgeLinkSeedWellKnownMark;
  char EdgeLinkSeedFillColor[8];
  char EdgeLinkSeedStrokeColor[8];
  double FaceSeedOpacity;
  double FaceSeedSize;
  double FaceSeedRotation;
  double FaceSeedAnchorX;
  double FaceSeedAnchorY;
  double FaceSeedDisplacementX;
  double FaceSeedDisplacementY;
  int FaceSeedWellKnownMark;
  char FaceSeedFillColor[8];
  char FaceSeedStrokeColor[8];
  char *XmlStyle;
public:
    QuickStyleTopologyObj(int Type);
   ~QuickStyleTopologyObj()
  {
    if (XmlStyle)
      sqlite3_free(XmlStyle);
  }
  QuickStyleTopologyObj *Clone();
  bool Compare(QuickStyleTopologyObj * style);
  const char *GetUUID()
  {
    return UUID;
  }
  int GetType()
  {
    return Type;
  }
  char *DoCreateFaceXML(const char *indent, const char *prefix);
  char *DoCreateFaceXML(const char *indent);
  char *DoCreateEdgeLinkXML(const char *indent, const char *prefix);
  char *DoCreateEdgeLinkXML(const char *indent);
  char *DoCreateNodeXML(const char *indent, const char *prefix);
  char *DoCreateNodeXML(const char *indent);
  char *DoCreateEdgeLinkSeedXML(const char *indent, const char *prefix);
  char *DoCreateEdgeLinkSeedXML(const char *indent);
  char *DoCreateFaceSeedXML(const char *indent, const char *prefix);
  char *DoCreateFaceSeedXML(const char *indent);
  void EnableMinScale(bool mode)
  {
    MinScaleEnabled = mode;
  }
  bool IsMinScaleEnabled()
  {
    return MinScaleEnabled;
  }
  void EnableMaxScale(bool mode)
  {
    MaxScaleEnabled = mode;
  }
  bool IsMaxScaleEnabled()
  {
    return MaxScaleEnabled;
  }
  void SetScaleMin(double x)
  {
    ScaleMin = x;
  }
  double GetScaleMin()
  {
    return ScaleMin;
  }
  void SetScaleMax(double x)
  {
    ScaleMax = x;
  }
  double GetScaleMax()
  {
    return ScaleMax;
  }
  void SetNodeOpacity(double x)
  {
    NodeOpacity = x;
  }
  double GetNodeOpacity()
  {
    return NodeOpacity;
  }
  void SetNodeSize(double x)
  {
    NodeSize = x;
  }
  double GetNodeSize()
  {
    return NodeSize;
  }
  void SetNodeRotation(double x)
  {
    NodeRotation = x;
  }
  double GetNodeRotation()
  {
    return NodeRotation;
  }
  void SetNodeAnchorX(double x)
  {
    NodeAnchorX = x;
  }
  double GetNodeAnchorX()
  {
    return NodeAnchorX;
  }
  void SetNodeAnchorY(double x)
  {
    NodeAnchorY = x;
  }
  double GetNodeAnchorY()
  {
    return NodeAnchorY;
  }
  void SetNodeDisplacementX(double x)
  {
    NodeDisplacementX = x;
  }
  double GetNodeDisplacementX()
  {
    return NodeDisplacementX;
  }
  void SetNodeDisplacementY(double x)
  {
    NodeDisplacementY = x;
  }
  double GetNodeDisplacementY()
  {
    return NodeDisplacementY;
  }
  void SetNodeWellKnownMark(int x)
  {
    NodeWellKnownMark = x;
  }
  int GetNodeWellKnownMark()
  {
    return NodeWellKnownMark;
  }
  void SetNodeFillColor(const char *x)
  {
    strcpy(NodeFillColor, x);
  }
  const char *GetNodeFillColor()
  {
    return NodeFillColor;
  }
  void SetNodeStrokeColor(const char *x)
  {
    strcpy(NodeStrokeColor, x);
  }
  const char *GetNodeStrokeColor()
  {
    return NodeStrokeColor;
  }
  void SetEdgeLinkOpacity(double x)
  {
    EdgeLinkOpacity = x;
  }
  double GetEdgeLinkOpacity()
  {
    return EdgeLinkOpacity;
  }
  void SetEdgeLinkPerpendicularOffset(double x)
  {
    EdgeLinkPerpendicularOffset = x;
  }
  double GetEdgeLinkPerpendicularOffset()
  {
    return EdgeLinkPerpendicularOffset;
  }
  void SetEdgeLinkStrokeWidth(double x)
  {
    EdgeLinkStrokeWidth = x;
  }
  double GetEdgeLinkStrokeWidth()
  {
    return EdgeLinkStrokeWidth;
  }
  void SetEdgeLinkStrokeColor(const char *x)
  {
    strcpy(EdgeLinkStrokeColor, x);
  }
  const char *GetEdgeLinkStrokeColor()
  {
    return EdgeLinkStrokeColor;
  }
  void SetEdgeLinkDotStyle(int x)
  {
    EdgeLinkDotStyle = x;
  }
  int GetEdgeLinkDotStyle()
  {
    return EdgeLinkDotStyle;
  }
  void SetFaceFill(bool x)
  {
    FaceFill = x;
  }
  bool IsFaceFill()
  {
    return FaceFill;
  }
  void SetFaceStroke(bool x)
  {
    FaceStroke = x;
  }
  bool IsFaceStroke()
  {
    return FaceStroke;
  }
  void SetFaceDisplacementX(double x)
  {
    FaceDisplacementX = x;
  }
  double GetFaceDisplacementX()
  {
    return FaceDisplacementX;
  }
  void SetFaceDisplacementY(double x)
  {
    FaceDisplacementY = x;
  }
  double GetFaceDisplacementY()
  {
    return FaceDisplacementY;
  }
  void SetFacePerpendicularOffset(double x)
  {
    FacePerpendicularOffset = x;
  }
  double GetFacePerpendicularOffset()
  {
    return FacePerpendicularOffset;
  }
  void SetFaceFillOpacity(double x)
  {
    FaceFillOpacity = x;
  }
  double GetFaceFillOpacity()
  {
    return FaceFillOpacity;
  }
  void SetFaceFillColor(const char *x)
  {
    strcpy(FaceFillColor, x);
  }
  const char *GetFaceFillColor()
  {
    return FaceFillColor;
  }
  void SetFaceStrokeOpacity(double x)
  {
    FaceStrokeOpacity = x;
  }
  double GetFaceStrokeOpacity()
  {
    return FaceStrokeOpacity;
  }
  void SetFaceStrokeWidth(double x)
  {
    FaceStrokeWidth = x;
  }
  double GetFaceStrokeWidth()
  {
    return FaceStrokeWidth;
  }
  void SetFaceStrokeColor(const char *x)
  {
    strcpy(FaceStrokeColor, x);
  }
  const char *GetFaceStrokeColor()
  {
    return FaceStrokeColor;
  }
  void SetEdgeLinkSeedOpacity(double x)
  {
    EdgeLinkSeedOpacity = x;
  }
  double GetEdgeLinkSeedOpacity()
  {
    return EdgeLinkSeedOpacity;
  }
  void SetEdgeLinkSeedSize(double x)
  {
    EdgeLinkSeedSize = x;
  }
  double GetEdgeLinkSeedSize()
  {
    return EdgeLinkSeedSize;
  }
  void SetEdgeLinkSeedRotation(double x)
  {
    EdgeLinkSeedRotation = x;
  }
  double GetEdgeLinkSeedRotation()
  {
    return EdgeLinkSeedRotation;
  }
  void SetEdgeLinkSeedAnchorX(double x)
  {
    EdgeLinkSeedAnchorX = x;
  }
  double GetEdgeLinkSeedAnchorX()
  {
    return EdgeLinkSeedAnchorX;
  }
  void SetEdgeLinkSeedAnchorY(double x)
  {
    EdgeLinkSeedAnchorY = x;
  }
  double GetEdgeLinkSeedAnchorY()
  {
    return EdgeLinkSeedAnchorY;
  }
  void SetEdgeLinkSeedDisplacementX(double x)
  {
    EdgeLinkSeedDisplacementX = x;
  }
  double GetEdgeLinkSeedDisplacementX()
  {
    return EdgeLinkSeedDisplacementX;
  }
  void SetEdgeLinkSeedDisplacementY(double x)
  {
    EdgeLinkSeedDisplacementY = x;
  }
  double GetEdgeLinkSeedDisplacementY()
  {
    return EdgeLinkSeedDisplacementY;
  }
  void SetEdgeLinkSeedWellKnownMark(int x)
  {
    EdgeLinkSeedWellKnownMark = x;
  }
  int GetEdgeLinkSeedWellKnownMark()
  {
    return EdgeLinkSeedWellKnownMark;
  }
  void SetEdgeLinkSeedFillColor(const char *x)
  {
    strcpy(EdgeLinkSeedFillColor, x);
  }
  const char *GetEdgeLinkSeedFillColor()
  {
    return EdgeLinkSeedFillColor;
  }
  void SetEdgeLinkSeedStrokeColor(const char *x)
  {
    strcpy(EdgeLinkSeedStrokeColor, x);
  }
  const char *GetEdgeLinkSeedStrokeColor()
  {
    return EdgeLinkSeedStrokeColor;
  }
  void SetFaceSeedOpacity(double x)
  {
    FaceSeedOpacity = x;
  }
  double GetFaceSeedOpacity()
  {
    return FaceSeedOpacity;
  }
  void SetFaceSeedSize(double x)
  {
    FaceSeedSize = x;
  }
  double GetFaceSeedSize()
  {
    return FaceSeedSize;
  }
  void SetFaceSeedRotation(double x)
  {
    FaceSeedRotation = x;
  }
  double GetFaceSeedRotation()
  {
    return FaceSeedRotation;
  }
  void SetFaceSeedAnchorX(double x)
  {
    FaceSeedAnchorX = x;
  }
  double GetFaceSeedAnchorX()
  {
    return FaceSeedAnchorX;
  }
  void SetFaceSeedAnchorY(double x)
  {
    FaceSeedAnchorY = x;
  }
  double GetFaceSeedAnchorY()
  {
    return FaceSeedAnchorY;
  }
  void SetFaceSeedDisplacementX(double x)
  {
    FaceSeedDisplacementX = x;
  }
  double GetFaceSeedDisplacementX()
  {
    return FaceSeedDisplacementX;
  }
  void SetFaceSeedDisplacementY(double x)
  {
    FaceSeedDisplacementY = x;
  }
  double GetFaceSeedDisplacementY()
  {
    return FaceSeedDisplacementY;
  }
  void SetFaceSeedWellKnownMark(int x)
  {
    FaceSeedWellKnownMark = x;
  }
  int GetFaceSeedWellKnownMark()
  {
    return FaceSeedWellKnownMark;
  }
  void SetFaceSeedFillColor(const char *x)
  {
    strcpy(FaceSeedFillColor, x);
  }
  const char *GetFaceSeedFillColor()
  {
    return FaceSeedFillColor;
  }
  void SetFaceSeedStrokeColor(const char *x)
  {
    strcpy(FaceSeedStrokeColor, x);
  }
  const char *GetFaceSeedStrokeColor()
  {
    return FaceSeedStrokeColor;
  }
  char *CreateXmlStyle();
  void UpdateXmlStyle();
  const char *GetXmlStyle()
  {
    return XmlStyle;
  }
  unsigned char *CloneXmlStyle();
};

class QuickStyleRasterObj
{
//
// a class wrapping a Quick Style - Raster
//
private:
  char UUID[64];
  bool MinScaleEnabled;
  bool MaxScaleEnabled;
  double ScaleMin;
  double ScaleMax;
  double Opacity;
  bool Normalize;
  bool Histogram;
  bool Gamma;
  double GammaValue;
  bool TripleBand;
  bool SingleBand;
  unsigned char RedBand;
  unsigned char GreenBand;
  unsigned char BlueBand;
  unsigned char GrayBand;
  int ColorMap;
  bool NdviColorMap;
  bool NdwiColorMap;
  bool ColorRamp;
  double MinValue;
  char MinValueColor[8];
  double MaxValue;
  char MaxValueColor[8];
  bool ShadedRelief;
  double ShadedReliefFactor;
  char *XmlStyle;
  char *DoCreateRasterXML();
public:
    QuickStyleRasterObj();
   ~QuickStyleRasterObj()
  {
    if (XmlStyle)
      sqlite3_free(XmlStyle);
  }
  static char *DoCreatePredefinedEtopo2Style(const char *indent);
  static char *DoCreatePredefinedSrtmStyle(const char *indent);
  static char *DoCreatePredefinedTerrainStyle(const char *indent);
  static char *DoCreatePredefinedElevationStyle(const char *indent, double min,
                                                double max);
  static char *DoCreatePredefinedAspectColorStyle(const char *indent,
                                                  double min, double max);
  static char *DoCreatePredefinedRainbowStyle(const char *indent, double min,
                                              double max);
  static char *DoCreatePredefinedWaveStyle(const char *indent, double min,
                                           double max);
  static char *DoCreatePredefinedSepiaStyle(const char *indent, double min,
                                            double max);
  static char *DoCreatePredefinedNdviStyle(const char *indent);
  static char *DoCreatePredefinedNdwiStyle(const char *indent);
  QuickStyleRasterObj *Clone();
  bool Compare(QuickStyleRasterObj * style);
  const char *GetUUID()
  {
    return UUID;
  }
  void EnableMinScale(bool mode)
  {
    MinScaleEnabled = mode;
  }
  bool IsMinScaleEnabled()
  {
    return MinScaleEnabled;
  }
  void EnableMaxScale(bool mode)
  {
    MaxScaleEnabled = mode;
  }
  bool IsMaxScaleEnabled()
  {
    return MaxScaleEnabled;
  }
  void SetScaleMin(double x)
  {
    ScaleMin = x;
  }
  double GetScaleMin()
  {
    return ScaleMin;
  }
  void SetScaleMax(double x)
  {
    ScaleMax = x;
  }
  double GetScaleMax()
  {
    return ScaleMax;
  }
  void SetOpacity(double x)
  {
    Opacity = x;
  }
  double GetOpacity()
  {
    return Opacity;
  }
  void SetNormalize(bool mode)
  {
    Normalize = mode;
  }
  bool IsNormalize()
  {
    return Normalize;
  }
  void SetHistogram(bool mode)
  {
    Histogram = mode;
  }
  bool IsHistogram()
  {
    return Histogram;
  }
  void SetGamma(bool mode)
  {
    Gamma = mode;
  }
  bool IsGamma()
  {
    return Gamma;
  }
  void SetGammaValue(double value)
  {
    GammaValue = value;
  }
  double GetGammaValue()
  {
    return GammaValue;
  }
  void SetTripleBand(bool mode)
  {
    TripleBand = mode;
  }
  bool IsTripleBand()
  {
    return TripleBand;
  }
  void SetSingleBand(bool mode)
  {
    SingleBand = mode;
  }
  bool IsSingleBand()
  {
    return SingleBand;
  }
  void SetRedBand(unsigned char red)
  {
    RedBand = red;
  }
  unsigned char GetRedBand()
  {
    return RedBand;
  }
  void SetGreenBand(unsigned char green)
  {
    GreenBand = green;
  }
  unsigned char GetGreenBand()
  {
    return GreenBand;
  }
  void SetBlueBand(unsigned char blue)
  {
    BlueBand = blue;
  }
  unsigned char GetBlueBand()
  {
    return BlueBand;
  }
  void SetGrayBand(unsigned char gray)
  {
    GrayBand = gray;
  }
  unsigned char GetGrayBand()
  {
    return GrayBand;
  }
  void SetColorMap(int mode)
  {
    ColorMap = mode;
  }
  int GetColorMap()
  {
    return ColorMap;
  }
  void SetNdviColorMap(bool mode)
  {
    NdviColorMap = mode;
  }
  bool IsNdviColorMap()
  {
    return NdviColorMap;
  }
  void SetNdwiColorMap(bool mode)
  {
    NdwiColorMap = mode;
  }
  bool IsNdwiColorMap()
  {
    return NdwiColorMap;
  }
  void SetColorRamp(bool mode)
  {
    ColorRamp = mode;
  }
  bool IsColorRamp()
  {
    return ColorRamp;
  }
  void SetMinValue(double x)
  {
    MinValue = x;
  }
  double GetMinValue()
  {
    return MinValue;
  }
  void SetMinValueColor(const char *x)
  {
    strcpy(MinValueColor, x);
  }
  const char *GetMinValueColor()
  {
    return MinValueColor;
  }
  void SetMaxValue(double x)
  {
    MaxValue = x;
  }
  double GetMaxValue()
  {
    return MaxValue;
  }
  void SetMaxValueColor(const char *x)
  {
    strcpy(MaxValueColor, x);
  }
  const char *GetMaxValueColor()
  {
    return MaxValueColor;
  }
  void SetShadedRelief(bool mode)
  {
    ShadedRelief = mode;
  }
  bool IsShadedRelief()
  {
    return ShadedRelief;
  }
  void SetShadedReliefFactor(double x)
  {
    ShadedReliefFactor = x;
  }
  double GetShadedReliefFactor()
  {
    return ShadedReliefFactor;
  }
  char *CreateXmlStyle();
  void UpdateXmlStyle();
  const char *GetXmlStyle()
  {
    return XmlStyle;
  }
  unsigned char *CloneXmlStyle();
};

class QuickStyleWmsObj
{
//
// a class wrapping a Quick Style - WMS
//
private:
  char UUID[64];
  bool MinScaleEnabled;
  bool MaxScaleEnabled;
  double ScaleMin;
  double ScaleMax;
public:
    QuickStyleWmsObj();
   ~QuickStyleWmsObj()
  {;
  }
  QuickStyleWmsObj *Clone();
  bool Compare(QuickStyleWmsObj * style);
  const char *GetUUID()
  {
    return UUID;
  }
  void EnableMinScale(bool mode)
  {
    MinScaleEnabled = mode;
  }
  bool IsMinScaleEnabled()
  {
    return MinScaleEnabled;
  }
  void EnableMaxScale(bool mode)
  {
    MaxScaleEnabled = mode;
  }
  bool IsMaxScaleEnabled()
  {
    return MaxScaleEnabled;
  }
  void SetScaleMin(double x)
  {
    ScaleMin = x;
  }
  double GetScaleMin()
  {
    return ScaleMin;
  }
  void SetScaleMax(double x)
  {
    ScaleMax = x;
  }
  double GetScaleMax()
  {
    return ScaleMax;
  }
};

class QuickStyleVectorDialog:public wxPropertySheetDialog
{
//
// a dialog for configuring a Quick Style (Vector)
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  int Type;
  wxString DbPrefix;
  wxString LayerName;
  bool HasStandardBrushes;
  bool HasStandardBrushHorz;
  bool HasStandardBrushVert;
  bool HasStandardBrushCross;
  bool HasStandardBrushDiag1;
  bool HasStandardBrushDiag2;
  bool HasStandardBrushCrossDiag;
  bool HasStandardBrushDots;
  QuickStyleObj *Style;
  bool IsConfigChanged;
  int PagePointIndex;
  int PageLineIndex;
  int PagePolygonIndex;
  int PageTextPointIndex;
  int PageTextLineIndex;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreatePointPage(wxWindow * book);
  wxPanel *CreateLinePage(wxWindow * book);
  wxPanel *CreatePolygonPage(wxWindow * book);
  wxPanel *CreateTextPointPage(wxWindow * book);
  wxPanel *CreateTextLinePage(wxWindow * book);
  bool RetrieveMainPage();
  bool RetrievePointPage(bool check = true);
  bool RetrieveLinePage(bool check = true);
  bool RetrievePolygonPage(bool check = true);
  bool RetrieveTextPointPage(bool check = true);
  bool RetrieveTextLinePage(bool check = true);
  void UpdateMainPage();
  void UpdatePointPage();
  void UpdateLinePage();
  void UpdatePolygonPage();
  void UpdateTextPointPage();
  void UpdateTextLinePage();
  bool DoCheckDatasource(const char *prefix, const char *coverage, char *table,
                         char *geometry);
  void InitializeComboColumns(wxComboBox * ctrl);
  void InitializeComboFonts(wxComboBox * ctrl);
  bool UpdateStyle();
  void CheckStandardBrushes();
public:
    QuickStyleVectorDialog()
  {;
  }
  virtual ~ QuickStyleVectorDialog()
  {
    if (Style != NULL)
      delete Style;
  }
  bool Create(MyMapPanel * parent, MapLayer * layer, int type);
  void CreateButtons();
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnQuit(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnApply(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdMarkChanged(wxCommandEvent & event);
  void OnCmdPointColorStrokeChanged(wxCommandEvent & event);
  void OnCmdPointColorStrokePicker(wxCommandEvent & event);
  void OnCmdPointColorFillChanged(wxCommandEvent & event);
  void OnCmdPointColorFillPicker(wxCommandEvent & event);
  void OnCmdLineColorChanged(wxCommandEvent & event);
  void OnCmdLineColorPicker(wxCommandEvent & event);
  void OnCmdLine2ColorChanged(wxCommandEvent & event);
  void OnCmdLine2ColorPicker(wxCommandEvent & event);
  void OnCmdSecondStrokEnableChanged(wxCommandEvent & event);
  void OnCmdPolygonStrokeChanged(wxCommandEvent & event);
  void OnCmdPolygonFillChanged(wxCommandEvent & event);
  void OnCmdPolygonColorStrokeChanged(wxCommandEvent & event);
  void OnCmdPolygonColorStrokePicker(wxCommandEvent & event);
  void OnCmdPolygonColorFillChanged(wxCommandEvent & event);
  void OnCmdPolygonColorFillPicker(wxCommandEvent & event);
  void OnCmdPolygonFillStyleChanged(wxCommandEvent & event);
  void OnCmdPolygonFillBrushChanged(wxCommandEvent & event);
  void OnCmdLabel1Changed(wxCommandEvent & event);
  void OnCmdLabel2Changed(wxCommandEvent & event);
  void OnCmdDontPaintGeom1Changed(wxCommandEvent & event);
  void OnCmdDontPaintGeom2Changed(wxCommandEvent & event);
  void OnCmdFont1ColorPicker(wxCommandEvent & event);
  void OnCmdFont1ColorChanged(wxCommandEvent & event);
  void OnCmdFont2ColorPicker(wxCommandEvent & event);
  void OnCmdFont2ColorChanged(wxCommandEvent & event);
  void OnCmdHalo1EnableChanged(wxCommandEvent & event);
  void OnCmdHalo2EnableChanged(wxCommandEvent & event);
  void OnCmdColorHalo1Changed(wxCommandEvent & event);
  void OnCmdColorHalo2Changed(wxCommandEvent & event);
  void OnCmdColorHalo1Picker(wxCommandEvent & event);
  void OnCmdColorHalo2Picker(wxCommandEvent & event);
  void OnCmdIsRepeatedChanged(wxCommandEvent & event);
  void OnCmdIsAlignedChanged(wxCommandEvent & event);
  void OnCmdGeneralizeLineChanged(wxCommandEvent & event);
  void OnFont1Changed(wxCommandEvent & event);
  void OnFont2Changed(wxCommandEvent & event);
};

class QuickStyleRasterDialog:public wxPropertySheetDialog
{
//
// a dialog for configuring a Quick Style (Raster)
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  bool CanApplyContrastEnhancement;
  bool IsMultiband;
  unsigned char NumBands;
  unsigned char RedBand;
  unsigned char GreenBand;
  unsigned char BlueBand;
  unsigned char GrayBand;
  bool CanApplyColorMap;
  bool CanApplyNDVI;
  double MinPixelValue;
  double MaxPixelValue;
  bool CanApplyShadedRelief;
  QuickStyleRasterObj *Style;
  bool IsConfigChanged;
  int PageContrastEnhancementIndex;
  int PageChannelSelectionIndex;
  int PageColorMapIndex;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreateContrastEnhancementPage(wxWindow * book);
  wxPanel *CreateChannelSelectionPage(wxWindow * book);
  wxPanel *CreateColorMapPage(wxWindow * book);
  bool RetrieveMainPage();
  bool RetrieveContrastEnhancementPage();
  bool RetrieveChannelSelectionPage();
  bool RetrieveColorMapPage();
  void UpdateMainPage();
  void UpdateContrastEnhancementPage();
  void UpdateChannelSelectionPage();
  void UpdateColorMapPage();
  void GetCoverageInfos();
  bool UpdateStyle();
public:
    QuickStyleRasterDialog()
  {;
  }
  virtual ~ QuickStyleRasterDialog()
  {
    if (Style != NULL)
      delete Style;
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateButtons();
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnQuit(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnApply(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdContrastChanged(wxCommandEvent & event);
  void OnCmdBandModeChanged(wxCommandEvent & event);
  void OnCmdColorMapModeChanged(wxCommandEvent & event);
  void OnCmdColorMinChanged(wxCommandEvent & event);
  void OnCmdMinColorPicker(wxCommandEvent & event);
  void OnCmdColorMaxChanged(wxCommandEvent & event);
  void OnCmdMaxColorPicker(wxCommandEvent & event);
  void OnShadedChanged(wxCommandEvent & event);
};

class QuickStyleTopologyDialog:public wxPropertySheetDialog
{
//
// a dialog for configuring a Quick Style (Topology)
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  int Type;
  wxString DbPrefix;
  wxString LayerName;
  QuickStyleTopologyObj *Style;
  bool IsConfigChanged;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreateNodePage(wxWindow * book);
  wxPanel *CreateEdgeLinkPage(wxWindow * book);
  wxPanel *CreateFacePage(wxWindow * book);
  wxPanel *CreateEdgeLinkSeedPage(wxWindow * book);
  wxPanel *CreateFaceSeedPage(wxWindow * book);
  bool RetrieveMainPage();
  bool RetrieveNodePage(bool check = true);
  bool RetrieveEdgeLinkPage(bool check = true);
  bool RetrieveFacePage(bool check = true);
  bool RetrieveEdgeLinkSeedPage(bool check = true);
  bool RetrieveFaceSeedPage(bool check = true);
  void UpdateMainPage();
  void UpdateNodePage();
  void UpdateEdgeLinkPage();
  void UpdateFacePage();
  void UpdateEdgeLinkSeedPage();
  void UpdateFaceSeedPage();
  bool UpdateStyle();
public:
    QuickStyleTopologyDialog()
  {;
  }
  virtual ~ QuickStyleTopologyDialog()
  {
    if (Style != NULL)
      delete Style;
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void CreateButtons();
  bool ConfigChanged()
  {
    return IsConfigChanged;
  }
  void OnQuit(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnApply(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
  void OnCmdNodeMarkChanged(wxCommandEvent & event);
  void OnCmdNodeColorStrokeChanged(wxCommandEvent & event);
  void OnCmdNodeColorStrokePicker(wxCommandEvent & event);
  void OnCmdNodeColorFillChanged(wxCommandEvent & event);
  void OnCmdNodeColorFillPicker(wxCommandEvent & event);
  void OnCmdEdgeLinkColorChanged(wxCommandEvent & event);
  void OnCmdEdgeLinkColorPicker(wxCommandEvent & event);
  void OnCmdFaceStrokeChanged(wxCommandEvent & event);
  void OnCmdFaceFillChanged(wxCommandEvent & event);
  void OnCmdFaceColorStrokeChanged(wxCommandEvent & event);
  void OnCmdFaceColorStrokePicker(wxCommandEvent & event);
  void OnCmdFaceColorFillChanged(wxCommandEvent & event);
  void OnCmdFaceColorFillPicker(wxCommandEvent & event);
  void OnCmdEdgeLinkSeedMarkChanged(wxCommandEvent & event);
  void OnCmdEdgeLinkSeedColorStrokeChanged(wxCommandEvent & event);
  void OnCmdEdgeLinkSeedColorStrokePicker(wxCommandEvent & event);
  void OnCmdEdgeLinkSeedColorFillChanged(wxCommandEvent & event);
  void OnCmdEdgeLinkSeedColorFillPicker(wxCommandEvent & event);
  void OnCmdFaceSeedMarkChanged(wxCommandEvent & event);
  void OnCmdFaceSeedColorStrokeChanged(wxCommandEvent & event);
  void OnCmdFaceSeedColorStrokePicker(wxCommandEvent & event);
  void OnCmdFaceSeedColorFillChanged(wxCommandEvent & event);
  void OnCmdFaceSeedColorFillPicker(wxCommandEvent & event);
};

class QuickStyleWmsDialog:public wxDialog
{
//
// a dialog for configuring a Quick Style (WMS)
//
private:
  MyFrame * MainFrame;
  MyMapPanel *MapPanel;
  MapLayer *Layer;
  wxString DbPrefix;
  wxString LayerName;
  QuickStyleWmsObj *Style;
  void UpdateDialog();
  bool ValidateDialog();
public:
    QuickStyleWmsDialog()
  {;
  }
  virtual ~ QuickStyleWmsDialog()
  {
    if (Style != NULL)
      delete Style;
  }
  void CreateControls();
  bool Create(MyMapPanel * parent, MapLayer * layer);
  void OnQuit(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCmdScaleChanged(wxCommandEvent & event);
};

class ComposerMainPage:public wxPanel
{
//
// first page used by Query/View COMPOSER
//
private:
  class ComposerDialog * Parent;
  wxCheckBox *Table2Ctrl;
  wxComboBox *Table1NameCtrl;
  wxComboBox *Table2NameCtrl;
  wxTextCtrl *Table1AliasCtrl;
  wxTextCtrl *Table2AliasCtrl;
  wxListBox *Table1ColumnsCtrl;
  wxListBox *Table2ColumnsCtrl;
  wxRadioBox *JoinModeCtrl;
  wxCheckBox *Match2Ctrl;
  wxCheckBox *Match3Ctrl;
  wxComboBox *Match1Table1Ctrl;
  wxComboBox *Match1Table2Ctrl;
  wxComboBox *Match2Table1Ctrl;
  wxComboBox *Match2Table2Ctrl;
  wxComboBox *Match3Table1Ctrl;
  wxComboBox *Match3Table2Ctrl;
public:
    ComposerMainPage()
  {;
  }
  bool Create(ComposerDialog * parent);
  virtual ~ ComposerMainPage()
  {;
  }
  void CreateControls();
  void SelectGeometryColumn(wxString & column, bool table2);
  void InitializeComboColumns(wxComboBox * ctrl, bool table2);
  void InitializeListColumns(wxListBox * ctrl, bool table2);
  void OnTable2Enabled(wxCommandEvent & event);
  void OnTable1Selected(wxCommandEvent & event);
  void OnTable2Selected(wxCommandEvent & event);
  void OnColumns1Selected(wxCommandEvent & event);
  void OnColumns2Selected(wxCommandEvent & event);
  void OnTable1AliasChanged(wxCommandEvent & event);
  void OnTable2AliasChanged(wxCommandEvent & event);
  void OnJoinModeChanged(wxCommandEvent & event);
  void OnMatch2Enabled(wxCommandEvent & event);
  void OnMatch3Enabled(wxCommandEvent & event);
  void OnMatch1Table1Selected(wxCommandEvent & event);
  void OnMatch1Table2Selected(wxCommandEvent & event);
  void OnMatch2Table1Selected(wxCommandEvent & event);
  void OnMatch2Table2Selected(wxCommandEvent & event);
  void OnMatch3Table1Selected(wxCommandEvent & event);
  void OnMatch3Table2Selected(wxCommandEvent & event);
};

class ComposerFilterPage:public wxPanel
{
//
// second page used by Query/View COMPOSER
//
private:
  class ComposerDialog * Parent;
  wxCheckBox *Where1EnabledCtrl;
  wxRadioBox *Where1TableCtrl;
  wxComboBox *Where1ColumnCtrl;
  wxComboBox *Where1OperatorCtrl;
  wxTextCtrl *Where1ValueCtrl;
  wxCheckBox *Where2EnabledCtrl;
  wxRadioBox *Where2TableCtrl;
  wxComboBox *Where2ColumnCtrl;
  wxComboBox *Where2OperatorCtrl;
  wxTextCtrl *Where2ValueCtrl;
  wxCheckBox *Where3EnabledCtrl;
  wxRadioBox *Where3TableCtrl;
  wxComboBox *Where3ColumnCtrl;
  wxComboBox *Where3OperatorCtrl;
  wxTextCtrl *Where3ValueCtrl;
  wxRadioBox *Connector12Ctrl;
  wxRadioBox *Connector23Ctrl;
public:
    ComposerFilterPage()
  {;
  }
  bool Create(ComposerDialog * parent);
  virtual ~ ComposerFilterPage()
  {;
  }
  void CreateControls();
  void Table1Status(bool ok);
  void Table2Status(bool ok);
  void InitializeColumns(wxComboBox * ctrl, bool table2);
  void InitializeOperators(wxComboBox * ctrl);
  void OnWhere1Enabled(wxCommandEvent & event);
  void OnWhere1TableChanged(wxCommandEvent & event);
  void OnWhere1ColumnSelected(wxCommandEvent & event);
  void OnWhere1OperatorSelected(wxCommandEvent & event);
  void OnWhere1ValueChanged(wxCommandEvent & event);
  void OnWhere2Enabled(wxCommandEvent & event);
  void OnWhere2TableChanged(wxCommandEvent & event);
  void OnWhere2ColumnSelected(wxCommandEvent & event);
  void OnWhere2OperatorSelected(wxCommandEvent & event);
  void OnWhere2ValueChanged(wxCommandEvent & event);
  void OnWhere3Enabled(wxCommandEvent & event);
  void OnWhere3TableChanged(wxCommandEvent & event);
  void OnWhere3ColumnSelected(wxCommandEvent & event);
  void OnWhere3OperatorSelected(wxCommandEvent & event);
  void OnWhere3ValueChanged(wxCommandEvent & event);
  void OnConnector12Changed(wxCommandEvent & event);
  void OnConnector23Changed(wxCommandEvent & event);
};

class ComposerOrderPage:public wxPanel
{
//
// third page used by Query/View COMPOSER
//
private:
  class ComposerDialog * Parent;
  wxCheckBox *Order1EnabledCtrl;
  wxRadioBox *Order1TableCtrl;
  wxComboBox *Order1ColumnCtrl;
  wxRadioBox *Order1DescCtrl;
  wxCheckBox *Order2EnabledCtrl;
  wxRadioBox *Order2TableCtrl;
  wxComboBox *Order2ColumnCtrl;
  wxRadioBox *Order2DescCtrl;
  wxCheckBox *Order3EnabledCtrl;
  wxRadioBox *Order3TableCtrl;
  wxComboBox *Order3ColumnCtrl;
  wxRadioBox *Order3DescCtrl;
  wxCheckBox *Order4EnabledCtrl;
  wxRadioBox *Order4TableCtrl;
  wxComboBox *Order4ColumnCtrl;
  wxRadioBox *Order4DescCtrl;
public:
    ComposerOrderPage()
  {;
  }
  bool Create(ComposerDialog * parent);
  virtual ~ ComposerOrderPage()
  {;
  }
  void CreateControls();
  void Table1Status(bool ok);
  void Table2Status(bool ok);
  void InitializeColumns(wxComboBox * ctrl, bool table2);
  void OnOrder1Enabled(wxCommandEvent & event);
  void OnOrder1TableChanged(wxCommandEvent & event);
  void OnOrder1ColumnSelected(wxCommandEvent & event);
  void OnOrder1DescChanged(wxCommandEvent & event);
  void OnOrder2Enabled(wxCommandEvent & event);
  void OnOrder2TableChanged(wxCommandEvent & event);
  void OnOrder2ColumnSelected(wxCommandEvent & event);
  void OnOrder2DescChanged(wxCommandEvent & event);
  void OnOrder3Enabled(wxCommandEvent & event);
  void OnOrder3TableChanged(wxCommandEvent & event);
  void OnOrder3ColumnSelected(wxCommandEvent & event);
  void OnOrder3DescChanged(wxCommandEvent & event);
  void OnOrder4Enabled(wxCommandEvent & event);
  void OnOrder4TableChanged(wxCommandEvent & event);
  void OnOrder4ColumnSelected(wxCommandEvent & event);
  void OnOrder4DescChanged(wxCommandEvent & event);
};

class ComposerViewPage:public wxPanel
{
//
// fourth page used by Query/View COMPOSER
//
private:
  class ComposerDialog * Parent;
  wxRadioBox *ViewTypeCtrl;
  wxTextCtrl *ViewNameCtrl;
  wxRadioBox *GeomTableCtrl;
  wxComboBox *GeometryColumnCtrl;
  wxCheckBox *Writable1Ctrl;
  wxCheckBox *Writable2Ctrl;
public:
    ComposerViewPage()
  {;
  }
  bool Create(ComposerDialog * parent);
  virtual ~ ComposerViewPage()
  {;
  }
  void CreateControls();
  void Table1Status(bool ok);
  void Table2Status(bool ok);
  void OnGeomTableChanged(wxCommandEvent & event);
  void InitializeGeometries(bool table2);
  void OnGeometryColumnSelected(wxCommandEvent & event);
  void OnViewTypeChanged(wxCommandEvent & event);
  void OnViewNameChanged(wxCommandEvent & event);
  void OnWritable1Changed(wxCommandEvent & event);
  void OnWritable2Changed(wxCommandEvent & event);
};

class ComposerDialog:public wxDialog
{
//
// a dialog used by Query/View COMPOSER
//
private:
  MyFrame * MainFrame;
  AuxTableList TableList;
  AuxColumnList Column1List;
  AuxColumnList Column2List;
  bool IncompleteSql;
  bool Table2Enabled;
  wxString TableName1;
  wxString TableName2;
  wxString TableAlias1;
  wxString TableAlias2;
  bool LeftJoin;
  bool Match2Enabled;
  bool Match3Enabled;
  wxString Match1Table1;
  wxString Match1Table2;
  wxString Match2Table1;
  wxString Match2Table2;
  wxString Match3Table1;
  wxString Match3Table2;
  bool Where1Enabled;
  bool Where2Enabled;
  bool Where3Enabled;
  bool Where1Table2;
  bool Where2Table2;
  bool Where3Table2;
  bool AndOr12;
  bool AndOr23;
  wxString Where1Column;
  wxString Where2Column;
  wxString Where3Column;
  wxString Where1Operator;
  wxString Where2Operator;
  wxString Where3Operator;
  wxString Where1Value;
  wxString Where2Value;
  wxString Where3Value;
  bool Order1Enabled;
  bool Order2Enabled;
  bool Order3Enabled;
  bool Order4Enabled;
  bool Order1Table2;
  bool Order2Table2;
  bool Order3Table2;
  bool Order4Table2;
  wxString Order1Column;
  wxString Order2Column;
  wxString Order3Column;
  wxString Order4Column;
  bool Order1Desc;
  bool Order2Desc;
  bool Order3Desc;
  bool Order4Desc;
  bool PlainView;
  bool SpatialView;
  wxString ViewName;
  bool ViewGeomTable2;
  wxString GeometryColumn;
  bool Writable1;
  bool Writable2;
  wxString GeometryColumnAlias;
  wxString GeometryRowidAlias;
  wxString SqlSample;
  wxString SqlTriggerInsert;
  wxString SqlTriggerUpdate;
  wxString SqlTriggerDelete;
  wxTextCtrl *SqlCtrl;
  wxNotebook *TabCtrl;
  ComposerMainPage *Page1;
  ComposerFilterPage *Page2;
  ComposerOrderPage *Page3;
  ComposerViewPage *Page4;
public:
    ComposerDialog()
  {;
  }
  bool Create(MyFrame * parent);
  virtual ~ ComposerDialog()
  {;
  }
  void CreateControls();
  AuxTableList *GetTableList()
  {
    return &TableList;
  }
  void PopulateColumnList1()
  {
    Column1List.Populate(MainFrame->GetSqlite(), TableName1);
  }
  void PopulateColumnList2()
  {
    Column2List.Populate(MainFrame->GetSqlite(), TableName2);
  }
  AuxColumnList *GetColumn1List()
  {
    return &Column1List;
  }
  AuxColumnList *GetColumn2List()
  {
    return &Column2List;
  }
  void SetTable2Enabled(bool mode)
  {
    Table2Enabled = mode;
  }
  bool IsTable2Enabled()
  {
    return Table2Enabled;
  }
  void SetTableName1(wxString name)
  {
    TableName1 = name;
  }
  wxString & GetTableName1()
  {
    return TableName1;
  }
  void SetTableName2(wxString name)
  {
    TableName2 = name;
  }
  wxString & GetTableAlias2()
  {
    return TableAlias2;
  }
  void SetTableAlias1(wxString alias)
  {
    TableAlias1 = alias;
  }
  wxString & GetTableAlias1()
  {
    return TableAlias1;
  }
  void SetTableAlias2(wxString alias)
  {
    TableAlias2 = alias;
  }
  wxString & GetTableName2()
  {
    return TableName2;
  }
  void SetLeftJoin(bool mode)
  {
    LeftJoin = mode;
  }
  bool IsLeftJoin()
  {
    return LeftJoin;
  }
  void SetMatch2Enabled(bool mode)
  {
    Match2Enabled = mode;
  }
  bool IsMatch2Enabled()
  {
    return Match2Enabled;
  }
  void SetMatch3Enabled(bool mode)
  {
    Match3Enabled = mode;
  }
  bool IsMatch3Enabled()
  {
    return Match3Enabled;
  }
  void SetMatch1Table1(wxString name)
  {
    Match1Table1 = name;
  }
  wxString & GetMatch1Table1()
  {
    return Match1Table1;
  }
  void SetMatch1Table2(wxString name)
  {
    Match1Table2 = name;
  }
  wxString & GetMatch1Table2()
  {
    return Match1Table2;
  }
  void SetMatch2Table1(wxString name)
  {
    Match2Table1 = name;
  }
  wxString & GetMatch2Table1()
  {
    return Match2Table1;
  }
  void SetMatch2Table2(wxString name)
  {
    Match2Table2 = name;
  }
  wxString & GetMatch2Table2()
  {
    return Match2Table2;
  }
  void SetMatch3Table1(wxString name)
  {
    Match3Table1 = name;
  }
  wxString & GetMatch3Table1()
  {
    return Match3Table1;
  }
  void SetMatch3Table2(wxString name)
  {
    Match3Table2 = name;
  }
  wxString & GetMatch3Table2()
  {
    return Match3Table2;
  }
  void SetWhere1Enabled(bool mode)
  {
    Where1Enabled = mode;
  }
  bool IsWhere1Enabled()
  {
    return Where1Enabled;
  }
  void SetWhere2Enabled(bool mode)
  {
    Where2Enabled = mode;
  }
  bool IsWhere2Enabled()
  {
    return Where2Enabled;
  }
  void SetWhere3Enabled(bool mode)
  {
    Where3Enabled = mode;
  }
  bool IsWhere3Enabled()
  {
    return Where3Enabled;
  }
  void SetWhere1Table2(bool mode)
  {
    Where1Table2 = mode;
  }
  bool IsWhere1Table2()
  {
    return Where1Table2;
  }
  void SetWhere2Table2(bool mode)
  {
    Where2Table2 = mode;
  }
  bool IsWhere2Table2()
  {
    return Where2Table2;
  }
  void SetWhere3Table2(bool mode)
  {
    Where3Table2 = mode;
  }
  bool IsWhere3Table2()
  {
    return Where3Table2;
  }
  void SetAndOr12(bool mode)
  {
    AndOr12 = mode;
  }
  bool IsAndOr12()
  {
    return AndOr12;
  }
  void SetAndOr23(bool mode)
  {
    AndOr23 = mode;
  }
  bool IsAndOr23()
  {
    return AndOr23;
  }
  void SetWhere1Column(wxString name)
  {
    Where1Column = name;
  }
  wxString & GetWhere1Column()
  {
    return Where1Column;
  }
  void SetWhere2Column(wxString name)
  {
    Where2Column = name;
  }
  wxString & GetWhere2Column()
  {
    return Where2Column;
  }
  void SetWhere3Column(wxString name)
  {
    Where3Column = name;
  }
  wxString & GetWhere3Column()
  {
    return Where3Column;
  }
  void SetWhere1Operator(wxString name)
  {
    Where1Operator = name;
  }
  wxString & GetWhere1Operator()
  {
    return Where1Operator;
  }
  void SetWhere2Operator(wxString name)
  {
    Where2Operator = name;
  }
  wxString & GetWhere2Operator()
  {
    return Where2Operator;
  }
  void SetWhere3Operator(wxString name)
  {
    Where3Operator = name;
  }
  wxString & GetWhere3Operator()
  {
    return Where3Operator;
  }
  void SetWhere1Value(wxString name)
  {
    Where1Value = name;
  }
  wxString & GetWhere1Value()
  {
    return Where1Value;
  }
  void SetWhere2Value(wxString name)
  {
    Where2Value = name;
  }
  wxString & GetWhere2Value()
  {
    return Where2Value;
  }
  void SetWhere3Value(wxString name)
  {
    Where3Value = name;
  }
  wxString & GetWhere3Value()
  {
    return Where3Value;
  }
  void SetOrder1Enabled(bool mode)
  {
    Order1Enabled = mode;
  }
  bool IsOrder1Enabled()
  {
    return Order1Enabled;
  }
  void SetOrder2Enabled(bool mode)
  {
    Order2Enabled = mode;
  }
  bool IsOrder2Enabled()
  {
    return Order2Enabled;
  }
  void SetOrder3Enabled(bool mode)
  {
    Order3Enabled = mode;
  }
  bool IsOrder3Enabled()
  {
    return Order3Enabled;
  }
  void SetOrder4Enabled(bool mode)
  {
    Order4Enabled = mode;
  }
  bool IsOrder4Enabled()
  {
    return Order4Enabled;
  }
  void SetOrder1Table2(bool mode)
  {
    Order1Table2 = mode;
  }
  bool IsOrder1Table2()
  {
    return Order1Table2;
  }
  void SetOrder2Table2(bool mode)
  {
    Order2Table2 = mode;
  }
  bool IsOrder2Table2()
  {
    return Order2Table2;
  }
  void SetOrder3Table2(bool mode)
  {
    Order3Table2 = mode;
  }
  bool IsOrder3Table2()
  {
    return Order3Table2;
  }
  void SetOrder4Table2(bool mode)
  {
    Order4Table2 = mode;
  }
  bool IsOrder4Table2()
  {
    return Order4Table2;
  }
  void SetOrder1Column(wxString name)
  {
    Order1Column = name;
  }
  wxString & GetOrder1Column()
  {
    return Order1Column;
  }
  void SetOrder2Column(wxString name)
  {
    Order2Column = name;
  }
  wxString & GetOrder2Column()
  {
    return Order2Column;
  }
  void SetOrder3Column(wxString name)
  {
    Order3Column = name;
  }
  wxString & GetOrder3Column()
  {
    return Order3Column;
  }
  void SetOrder4Column(wxString name)
  {
    Order4Column = name;
  }
  wxString & GetOrder4Column()
  {
    return Order4Column;
  }
  void SetOrder1Desc(bool mode)
  {
    Order1Desc = mode;
  }
  bool IsOrder1Desc()
  {
    return Order1Desc;
  }
  void SetOrder2Desc(bool mode)
  {
    Order2Desc = mode;
  }
  bool IsOrder2Desc()
  {
    return Order2Desc;
  }
  void SetOrder3Desc(bool mode)
  {
    Order3Desc = mode;
  }
  bool IsOrder3Desc()
  {
    return Order3Desc;
  }
  void SetOrder4Desc(bool mode)
  {
    Order4Desc = mode;
  }
  bool IsOrder4Desc()
  {
    return Order4Desc;
  }
  void SetPlainView(bool mode)
  {
    PlainView = mode;
  }
  bool IsPlainView()
  {
    return PlainView;
  }
  void SetSpatialView(bool mode)
  {
    SpatialView = mode;
  }
  bool IsSpatialView()
  {
    return SpatialView;
  }
  void SetViewName(wxString name)
  {
    ViewName = name;
  }
  wxString & GetViewName()
  {
    return ViewName;
  }
  void SetViewGeomTable2(bool mode)
  {
    ViewGeomTable2 = mode;
  }
  bool IsViewGeomTable2()
  {
    return ViewGeomTable2;
  }
  void SetGeometryColumn(wxString name)
  {
    GeometryColumn = name;
  }
  wxString & GetGeometryColumn()
  {
    return GeometryColumn;
  }
  void SetWritable1(bool value)
  {
    Writable1 = value;
  }
  void SetWritable2(bool value)
  {
    Writable2 = value;
  }
  bool IsWritable1()
  {
    return Writable1;
  }
  bool IsWritable2()
  {
    return Writable2;
  }
  bool IsDuplicateAlias(wxString & alias);
  void SetAliases();
  wxString & GetGeometryColumnAlias()
  {
    return GeometryColumnAlias;
  }
  wxString & GetGeometryRowidAlias()
  {
    return GeometryRowidAlias;
  }
  wxString & GetSqlSample()
  {
    return SqlSample;
  }
  wxString & GetSqlTriggerInsert()
  {
    return SqlTriggerInsert;
  }
  wxString & GetSqlTriggerUpdate()
  {
    return SqlTriggerUpdate;
  }
  wxString & GetSqlTriggerDelete()
  {
    return SqlTriggerDelete;
  }
  wxNotebook *GetTabCtrl()
  {
    return TabCtrl;
  }
  void Table1Status(bool ok);
  void Table2Status(bool ok);
  bool SqlCleanString(wxString & dirty, wxString & clean);
  void SqlCleanList(wxString & list, wxString & clean, int *style, int *start,
                    int *stop, int *next, int base);
  void SelectGeometryColumn();
  void UpdateSqlSample();
  void PrepareSqlTriggers();
  bool GetCurrentlySelectedTable(wxString & table_name)
  {
    return MainFrame->GetCurrentlySelectedTable(table_name);
  }
  void OnOk(wxCommandEvent & event);
};

class SqlFiltersMainPage:public wxPanel
{
//
// first page used by SQL Filters COMPOSER
//
private:
  class SqlFiltersDialog * Parent;
  wxCheckBox *Where1EnabledCtrl;
  wxComboBox *Where1ColumnCtrl;
  wxComboBox *Where1OperatorCtrl;
  wxTextCtrl *Where1ValueCtrl;
  wxCheckBox *Where2EnabledCtrl;
  wxComboBox *Where2ColumnCtrl;
  wxComboBox *Where2OperatorCtrl;
  wxTextCtrl *Where2ValueCtrl;
  wxCheckBox *Where3EnabledCtrl;
  wxComboBox *Where3ColumnCtrl;
  wxComboBox *Where3OperatorCtrl;
  wxTextCtrl *Where3ValueCtrl;
  wxRadioBox *Connector12Ctrl;
  wxRadioBox *Connector23Ctrl;
public:
    SqlFiltersMainPage()
  {;
  }
  bool Create(SqlFiltersDialog * parent);
  virtual ~ SqlFiltersMainPage()
  {;
  }
  void CreateControls();
  void ResetWizard();
  void UpdatePage();
  void InitializeColumns(wxComboBox * ctrl);
  void InitializeOperators(wxComboBox * ctrl);
  void GetWhereOperator(wxString & value, wxString & compar);
  void OnWhere1Enabled(wxCommandEvent & event);
  void OnWhere1ColumnSelected(wxCommandEvent & event);
  void OnWhere1OperatorSelected(wxCommandEvent & event);
  void OnWhere1ValueChanged(wxCommandEvent & event);
  void OnWhere2Enabled(wxCommandEvent & event);
  void OnWhere2ColumnSelected(wxCommandEvent & event);
  void OnWhere2OperatorSelected(wxCommandEvent & event);
  void OnWhere2ValueChanged(wxCommandEvent & event);
  void OnWhere3Enabled(wxCommandEvent & event);
  void OnWhere3ColumnSelected(wxCommandEvent & event);
  void OnWhere3OperatorSelected(wxCommandEvent & event);
  void OnWhere3ValueChanged(wxCommandEvent & event);
  void OnConnector12Changed(wxCommandEvent & event);
  void OnConnector23Changed(wxCommandEvent & event);
};

class SqlFiltersOrderPage:public wxPanel
{
//
// second page used by SQL Filters COMPOSER
//
private:
  class SqlFiltersDialog * Parent;
  wxCheckBox *Order1EnabledCtrl;
  wxComboBox *Order1ColumnCtrl;
  wxRadioBox *Order1DescCtrl;
  wxCheckBox *Order2EnabledCtrl;
  wxComboBox *Order2ColumnCtrl;
  wxRadioBox *Order2DescCtrl;
  wxCheckBox *Order3EnabledCtrl;
  wxComboBox *Order3ColumnCtrl;
  wxRadioBox *Order3DescCtrl;
  wxCheckBox *Order4EnabledCtrl;
  wxComboBox *Order4ColumnCtrl;
  wxRadioBox *Order4DescCtrl;
public:
    SqlFiltersOrderPage()
  {;
  }
  bool Create(SqlFiltersDialog * parent);
  virtual ~ SqlFiltersOrderPage()
  {;
  }
  void CreateControls();
  void ResetWizard();
  void UpdatePage();
  void InitializeColumns(wxComboBox * ctrl);
  void OnOrder1Enabled(wxCommandEvent & event);
  void OnOrder1ColumnSelected(wxCommandEvent & event);
  void OnOrder1DescChanged(wxCommandEvent & event);
  void OnOrder2Enabled(wxCommandEvent & event);
  void OnOrder2ColumnSelected(wxCommandEvent & event);
  void OnOrder2DescChanged(wxCommandEvent & event);
  void OnOrder3Enabled(wxCommandEvent & event);
  void OnOrder3ColumnSelected(wxCommandEvent & event);
  void OnOrder3DescChanged(wxCommandEvent & event);
  void OnOrder4Enabled(wxCommandEvent & event);
  void OnOrder4ColumnSelected(wxCommandEvent & event);
  void OnOrder4DescChanged(wxCommandEvent & event);
};

class SqlFiltersFreeHandPage:public wxPanel
{
//
// third page used by SQL Filters COMPOSER
//
private:
  class SqlFiltersDialog * Parent;
  wxTextCtrl *FreeHandCtrl;
public:
    SqlFiltersFreeHandPage()
  {;
  }
  bool Create(SqlFiltersDialog * parent);
  virtual ~ SqlFiltersFreeHandPage()
  {;
  }
  void CreateControls();
  void UpdatePage();
  void OnFreeHandChanged(wxCommandEvent & event);
};

class SqlFiltersDialog:public wxDialog
{
//
// a dialog used by SQL Filters COMPOSER
//
private:
  MyFrame * MainFrame;
  wxString DbPrefix;
  wxString TableName;
  bool ReadOnly;
  AuxColumnList ColumnList;
  bool Where1Enabled;
  bool Where2Enabled;
  bool Where3Enabled;
  bool AndOr12;
  bool AndOr23;
  wxString Where1Column;
  wxString Where2Column;
  wxString Where3Column;
  wxString Where1Operator;
  wxString Where2Operator;
  wxString Where3Operator;
  wxString Where1Value;
  wxString Where2Value;
  wxString Where3Value;
  bool Order1Enabled;
  bool Order2Enabled;
  bool Order3Enabled;
  bool Order4Enabled;
  wxString Order1Column;
  wxString Order2Column;
  wxString Order3Column;
  wxString Order4Column;
  bool Order1Desc;
  bool Order2Desc;
  bool Order3Desc;
  bool Order4Desc;
  wxString FreeHand;
  wxString SqlSample;
  wxTextCtrl *SqlCtrl;
  wxNotebook *TabCtrl;
  SqlFiltersMainPage *Page1;
  SqlFiltersOrderPage *Page2;
  SqlFiltersFreeHandPage *Page3;
  bool IgnoreEvent;
  wxString Where1Clause;
  wxString Where2Clause;
  wxString Where3Clause;
  wxString OrderBy1;
  wxString OrderBy2;
  wxString OrderBy3;
  wxString OrderBy4;
  void DoWhereClause(int index);
  void DoOrderBy(int index);
  void DoSqlSyntaxColor();
  bool SqlCleanString(wxString & dirty, wxString & clean);
  void SqlCleanList(wxString & list, wxString & clean);
public:
    SqlFiltersDialog()
  {;
  }
  bool Create(MyFrame * parent, CurrentSqlFilters & filters);
  virtual ~ SqlFiltersDialog()
  {;
  }
  void CreateControls();
  void PopulateColumnList()
  {
    bool force_rowid = false;
    if (ReadOnly == false)
      force_rowid = true;
    ColumnList.Populate(MainFrame->GetSqlite(), TableName, force_rowid);
  }
  void ResetFreeHand();
  void ResetWizardClauses();
  AuxColumnList *GetColumnList()
  {
    return &ColumnList;
  }
  void SetWhere1Enabled(bool mode)
  {
    Where1Enabled = mode;
  }
  bool IsWhere1Enabled()
  {
    return Where1Enabled;
  }
  void SetWhere2Enabled(bool mode)
  {
    Where2Enabled = mode;
  }
  bool IsWhere2Enabled()
  {
    return Where2Enabled;
  }
  void SetWhere3Enabled(bool mode)
  {
    Where3Enabled = mode;
  }
  bool IsWhere3Enabled()
  {
    return Where3Enabled;
  }
  void SetAndOr12(bool mode)
  {
    AndOr12 = mode;
  }
  bool IsAndOr12()
  {
    return AndOr12;
  }
  void SetAndOr23(bool mode)
  {
    AndOr23 = mode;
  }
  bool IsAndOr23()
  {
    return AndOr23;
  }
  void SetWhere1Column(wxString name)
  {
    Where1Column = name;
  }
  wxString & GetWhere1Column()
  {
    return Where1Column;
  }
  void SetWhere2Column(wxString name)
  {
    Where2Column = name;
  }
  wxString & GetWhere2Column()
  {
    return Where2Column;
  }
  void SetWhere3Column(wxString name)
  {
    Where3Column = name;
  }
  wxString & GetWhere3Column()
  {
    return Where3Column;
  }
  void SetWhere1Operator(wxString name)
  {
    Where1Operator = name;
  }
  wxString & GetWhere1Operator()
  {
    return Where1Operator;
  }
  void SetWhere2Operator(wxString name)
  {
    Where2Operator = name;
  }
  wxString & GetWhere2Operator()
  {
    return Where2Operator;
  }
  void SetWhere3Operator(wxString name)
  {
    Where3Operator = name;
  }
  wxString & GetWhere3Operator()
  {
    return Where3Operator;
  }
  void SetWhere1Value(wxString name)
  {
    Where1Value = name;
  }
  wxString & GetWhere1Value()
  {
    return Where1Value;
  }
  void SetWhere2Value(wxString name)
  {
    Where2Value = name;
  }
  wxString & GetWhere2Value()
  {
    return Where2Value;
  }
  void SetWhere3Value(wxString name)
  {
    Where3Value = name;
  }
  wxString & GetWhere3Value()
  {
    return Where3Value;
  }
  void SetOrder1Enabled(bool mode)
  {
    Order1Enabled = mode;
  }
  bool IsOrder1Enabled()
  {
    return Order1Enabled;
  }
  void SetOrder2Enabled(bool mode)
  {
    Order2Enabled = mode;
  }
  bool IsOrder2Enabled()
  {
    return Order2Enabled;
  }
  void SetOrder3Enabled(bool mode)
  {
    Order3Enabled = mode;
  }
  bool IsOrder3Enabled()
  {
    return Order3Enabled;
  }
  void SetOrder4Enabled(bool mode)
  {
    Order4Enabled = mode;
  }
  bool IsOrder4Enabled()
  {
    return Order4Enabled;
  }
  void SetOrder1Column(wxString name)
  {
    Order1Column = name;
  }
  wxString & GetOrder1Column()
  {
    return Order1Column;
  }
  void SetOrder2Column(wxString name)
  {
    Order2Column = name;
  }
  wxString & GetOrder2Column()
  {
    return Order2Column;
  }
  void SetOrder3Column(wxString name)
  {
    Order3Column = name;
  }
  wxString & GetOrder3Column()
  {
    return Order3Column;
  }
  void SetOrder4Column(wxString name)
  {
    Order4Column = name;
  }
  wxString & GetOrder4Column()
  {
    return Order4Column;
  }
  void SetOrder1Desc(bool mode)
  {
    Order1Desc = mode;
  }
  bool IsOrder1Desc()
  {
    return Order1Desc;
  }
  void SetOrder2Desc(bool mode)
  {
    Order2Desc = mode;
  }
  bool IsOrder2Desc()
  {
    return Order2Desc;
  }
  void SetOrder3Desc(bool mode)
  {
    Order3Desc = mode;
  }
  bool IsOrder3Desc()
  {
    return Order3Desc;
  }
  void SetOrder4Desc(bool mode)
  {
    Order4Desc = mode;
  }
  bool IsOrder4Desc()
  {
    return Order4Desc;
  }
  void SetFreeHand(wxString freeHand)
  {
    FreeHand = freeHand;
  }
  wxString & GetFreeHand()
  {
    return FreeHand;
  }
  wxString & GetSqlSample()
  {
    return SqlSample;
  }
  wxNotebook *GetTabCtrl()
  {
    return TabCtrl;
  }
  void UpdateSqlSample();
  void OnOk(wxCommandEvent & event);
  void OnSqlSyntaxColor(wxCommandEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
};

class GeomColumn
{
//
// a class representing a Geometry Column
//
private:
  wxString GeometryName;
  wxString GeometryType;
  wxString CoordDims;
  int SRID;
  bool RTree;
  bool MbrCache;
  bool NotNull;
  GeomColumn *Next;
public:
    GeomColumn(wxString & name, wxString & type, wxString & dims, int srid,
               int idx);
   ~GeomColumn()
  {;
  }
  wxString & GetGeometryName()
  {
    return GeometryName;
  }
  wxString & GetGeometryType()
  {
    return GeometryType;
  }
  wxString & GetCoordDims()
  {
    return CoordDims;
  }
  int GetSRID()
  {
    return SRID;
  }
  bool IsRTree()
  {
    return RTree;
  }
  bool IsMbrCache()
  {
    return MbrCache;
  }
  void SetNotNull()
  {
    NotNull = true;
  }
  bool IsNotNull()
  {
    return NotNull;
  }
  void SetNext(GeomColumn * next)
  {
    Next = next;
  }
  GeomColumn *GetNext()
  {
    return Next;
  }
};

class GeomColsList
{
//
// a class representing a Geometry Columns list
//
private:
  GeomColumn * First;
  GeomColumn *Last;
public:
    GeomColsList();
   ~GeomColsList();
  void Add(wxString & name, wxString & type, wxString & dims, int srid,
           int idx);
  GeomColumn *GetFirst()
  {
    return First;
  }
  void SetNotNull(wxString & geom);
};

class IndexColumn
{
//
// a class representing an Index Column
//
private:
  wxString ColumnName;
  bool Valid;
  IndexColumn *Next;
public:
    IndexColumn(wxString & name);
   ~IndexColumn()
  {;
  }
  wxString & GetColumnName()
  {
    return ColumnName;
  }
  bool IsValid()
  {
    return Valid;
  }
  void Invalidate()
  {
    Valid = false;
  }
  void SetNext(IndexColumn * next)
  {
    Next = next;
  }
  IndexColumn *GetNext()
  {
    return Next;
  }
};

class TblIndex
{
//
// a class representing a Table Index
//
private:
  wxString IndexName;
  bool Unique;
  bool Valid;
  IndexColumn *First;
  IndexColumn *Last;
  TblIndex *Next;
public:
    TblIndex(wxString & name, bool unique);
   ~TblIndex();
  void Add(wxString & column);
    wxString & GetIndexName()
  {
    return IndexName;
  }
  bool IsUnique()
  {
    return Unique;
  }
  void Invalidate(wxString & colName);
  bool IsValid()
  {
    return Valid;
  }
  void SetNext(TblIndex * next)
  {
    Next = next;
  }
  TblIndex *GetNext()
  {
    return Next;
  }
  IndexColumn *GetFirst()
  {
    return First;
  }
};

class TblIndexList
{
//
// a class representing a Table Index list
//
private:
  TblIndex * First;
  TblIndex *Last;
public:
    TblIndexList();
   ~TblIndexList();
  void Add(wxString & name, bool unique);
  TblIndex *GetFirst()
  {
    return First;
  }
  void Invalidate(wxString & colName);
};

class ResultSetShapefileGeometry
{
//
// a class wrapping a (possible) Shapefile Geometry
//
private:
  int Type;
  int Dims;
  int SRID;
  int Count;
  ResultSetShapefileGeometry *Next;
public:
    ResultSetShapefileGeometry(int type, int dims, int srid)
  {
    Type = type;
    Dims = dims;
    SRID = srid;
    Count = 1;
    Next = NULL;
  }
   ~ResultSetShapefileGeometry()
  {;
  }
  int GetType()
  {
    return Type;
  }
  int GetDims()
  {
    return Dims;
  }
  int GetSRID()
  {
    return SRID;
  }
  int GetCount()
  {
    return Count;
  }
  void Update()
  {
    Count++;
  }
  void SetNext(ResultSetShapefileGeometry * next)
  {
    Next = next;
  }
  ResultSetShapefileGeometry *GetNext()
  {
    return Next;
  }
};

class ResultSetShapefileColumn
{
//
// a class wrapping a (possible) Shapefile column
//
private:
  char *Name;
  int NullCount;
  int TextCount;
  int MaxTextLen;
  int IntCount;
  int DoubleCount;
  int BlobCount;
  int DbfType;
  ResultSetShapefileGeometry *First;
  ResultSetShapefileGeometry *Last;
public:
    ResultSetShapefileColumn();
   ~ResultSetShapefileColumn();
  void SetName(const char *name);
  char *GetName()
  {
    return Name;
  }
  void UpdateNull()
  {
    NullCount++;
  }
  void UpdateText(int len)
  {
    TextCount++;
    if (len > MaxTextLen)
      MaxTextLen = len;
  }
  void UpdateInteger()
  {
    IntCount++;
  }
  void UpdateDouble()
  {
    DoubleCount++;
  }
  void UpdateBlob()
  {
    BlobCount++;
  }
  void UpdateGeometry(gaiaGeomCollPtr geom);
  bool Validate();
  int GetDbfType()
  {
    return DbfType;
  }
  int GetMaxTextLen()
  {
    return MaxTextLen;
  }
  ResultSetShapefileGeometry *GetFirst()
  {
    return First;
  }

};

class ResultSetShapefileAnalyzer
{
//
// a class representing a (possible) Shapefile 
// corresponding to some generic ResultSet
//
private:
  int ColumnCount;
  int GeometryColumn;
  ResultSetShapefileColumn *Columns;
public:
    ResultSetShapefileAnalyzer()
  {
    ColumnCount = 0;
    GeometryColumn = -1;
    Columns = NULL;
  }
   ~ResultSetShapefileAnalyzer();
  bool Validate();
  void SetColumnName(int column, const char *name);
  void Init(int count);
  int GetColumnCount()
  {
    return ColumnCount;
  }
  int GetGeometryColumn()
  {
    return GeometryColumn;
  }
  void UpdateNull(int column);
  void UpdateText(int column, int len);
  void UpdateInteger(int column);
  void UpdateDouble(int column);
  void UpdateGeometry(int column, gaiaGeomCollPtr geom);
  void UpdateBlob(int column);
  ResultSetShapefileColumn *GetColumn(int column);
  ResultSetShapefileColumn *GetGeometry();
};

class PostGISColumn
{
//
// a class wrapping a PostGIS column
//
private:
  wxString ColumnName;          // the column name
  bool PrimaryKey;              // Primary Key column
  bool Autoincrement;           // Autoincrement Primary Key
  bool Nullable;                // IS NULL
  int Null;
  int Boolean;
  int Int8;
  int UInt8;
  int Int16;
  int UInt16;
  int Int32;
  int UInt32;
  int Int64;
  int Double;
  int Text;
  int MaxTextLen;
  int Date;
  int DateTime;
  int Blob;
  int Point;
  int MultiPoint;
  int LineString;
  int MultiLineString;
  int Polygon;
  int MultiPolygon;
  int GeometryCollection;
  int Srid1;
  int Srid2;
  int CoordDims1;
  int CoordDims2;
  int DataType;
public:
    PostGISColumn();
   ~PostGISColumn()
  {;
  }
  void SetName(wxString & name)
  {
    ColumnName = name;
  }
  wxString & GetName()
  {
    return ColumnName;
  }
  void SetNotNull()
  {
    Nullable = false;
  }
  bool IsNotNull()
  {
    if (Nullable == true)
      return false;
    else
      return true;
  }
  void SetPrimaryKey()
  {
    PrimaryKey = true;
  }
  bool IsPrimaryKey()
  {
    return PrimaryKey;
  }
  void IncrNull()
  {
    Null++;
  }
  void IncrBoolean()
  {
    Boolean++;
  }
  void IncrInt8()
  {
    Int8++;
  }
  void IncrUInt8()
  {
    UInt8++;
  }
  void IncrInt16()
  {
    Int16++;
  }
  void IncrUInt16()
  {
    UInt16++;
  }
  void IncrInt32()
  {
    Int32++;
  }
  void IncrUInt32()
  {
    UInt32++;
  }
  void IncrInt64()
  {
    Int64++;
  }
  void IncrDouble()
  {
    Double++;
  }
  void IncrText(int len)
  {
    Text++;
    if (len > MaxTextLen)
      MaxTextLen = len;
  }
  void IncrDate()
  {
    Date++;
  }
  void IncrDateTime()
  {
    DateTime++;
  }
  void IncrBlob()
  {
    Blob++;
  }
  void IncrPoint(int srid, int coord_dims);
  void IncrMultiPoint(int srid, int coord_dims);
  void IncrLineString(int srid, int coord_dims);
  void IncrMultiLineString(int srid, int coord_dims);
  void IncrPolygon(int srid, int coord_dims);
  void IncrMultiPolygon(int srid, int coord_dims);
  void IncrGeometryCollection(int srid, int coord_dims);
  bool IsDate(const char *txt);
  bool IsDateTime(const char *txt);
  int GetMaxTextLen()
  {
    return MaxTextLen;
  }
  bool IsGeometry();
  int GetDataType()
  {
    return DataType;
  }
  void Prepare();
  int GetSrid()
  {
    return Srid1;
  }
  int GetCoordDims()
  {
    return CoordDims1;
  }
};

class PostGISIndexField
{
//
// a class wrapping a PostGIS Index field
private:
  int SeqNo;
  PostGISColumn *ColumnRef;
  PostGISIndexField *Next;
public:
    PostGISIndexField(int seq, PostGISColumn * col)
  {
    SeqNo = seq;
    ColumnRef = col;
    Next = NULL;
  }
   ~PostGISIndexField()
  {;
  }
  int GetSeqNo()
  {
    return SeqNo;
  }
  PostGISColumn *GetColumnRef()
  {
    return ColumnRef;
  }
  void SetNext(PostGISIndexField * next)
  {
    Next = next;
  }
  PostGISIndexField *GetNext()
  {
    return Next;
  }
};

class PostGISIndex
{
//
// a class wrapping a PostGIS Index
private:
  bool PrimaryKey;
  bool Unique;
  wxString Name;
  PostGISIndexField *First;
  PostGISIndexField *Last;
  PostGISIndex *Next;
public:
    PostGISIndex(wxString & name)
  {
    Name = name;
    PrimaryKey = true;
    Unique = true;
    First = NULL;
    Last = NULL;
    Next = NULL;
  }
  PostGISIndex(wxString & name, bool unique)
  {
    Name = name;
    PrimaryKey = false;
    Unique = unique;
    First = NULL;
    Last = NULL;
    Next = NULL;
  }
  ~PostGISIndex();
  wxString & GetName()
  {
    return Name;
  }
  bool IsPrimaryKey()
  {
    return PrimaryKey;
  }
  bool IsUnique()
  {
    return Unique;
  }
  void AddField(int seq, PostGISColumn * column);
  PostGISIndexField *GetFirst()
  {
    return First;
  }
  void SetNext(PostGISIndex * next)
  {
    Next = next;
  }
  PostGISIndex *GetNext()
  {
    return Next;
  }
};

class PostGISHelper
{
//
// a class wrapping a PostGIS table
//
private:
  wxString DumbName;
  int Count;                    // how many columns
  PostGISColumn *Columns;       // array of columns
  PostGISIndex *FirstIdx;
  PostGISIndex *LastIdx;
  bool Autoincrement;
public:
    PostGISHelper();
   ~PostGISHelper();
  void Alloc(int count);
  int GetCount()
  {
    return Count;
  }
  PostGISIndex *AddIndex(wxString & name, bool unique);
  PostGISIndex *AddIndex(wxString & name);
  PostGISColumn *Find(wxString & name);
  PostGISIndex *GetFirstIndex()
  {
    return FirstIdx;
  }
  void ExpandIndexFields(MyFrame * mother, PostGISIndex * index,
                         wxString & name);
  bool IsSingleFieldPrimaryKey();
  bool IsAutoincrement()
  {
    return Autoincrement;
  }
  void SetName(int pos, const char *name);
  void Eval(int pos, sqlite3_int64 val);
  void Eval(int pos, double val);
  void Eval(int pos, const char *val);
  void Eval(int pos, gaiaGeomCollPtr geom);
  void EvalBlob(int pos);
  void Eval(int pos);
  wxString & GetName(int pos, bool to_lower);
  bool IsGeometry(int pos);
  int GetDataType(int pos);
  void GetDataType(int pos, char *definition);
  int GetSrid(int pos);
  int GetCoordDims(int pos);
  void SetColumn(wxString & name, bool isNull, bool pKey);
  void GetKeys(MyFrame * mother, wxString & table);
  void Prepare();
  void OutputBooleanValue(FILE * out, sqlite3_int64 value);
  void OutputValue(FILE * out, sqlite3_int64 value);
  void OutputValue(FILE * out, double value);
  void OutputValue(FILE * out, const char *value);
  void OutputValue(FILE * out, gaiaGeomCollPtr value);
  void OutputValue(FILE * out, const unsigned char *value, int len);
  enum
  {
    // data types constants
    DATA_TYPE_UNDEFINED = 0,
    DATA_TYPE_BOOLEAN,
    DATA_TYPE_INT8,
    DATA_TYPE_UINT8,
    DATA_TYPE_INT16,
    DATA_TYPE_UINT16,
    DATA_TYPE_INT32,
    DATA_TYPE_UINT32,
    DATA_TYPE_INT64,
    DATA_TYPE_UINT64,
    DATA_TYPE_DOUBLE,
    DATA_TYPE_TEXT,
    DATA_TYPE_DATE,
    DATA_TYPE_DATETIME,
    DATA_TYPE_BLOB,
    DATA_TYPE_POINT,
    DATA_TYPE_LINESTRING,
    DATA_TYPE_POLYGON,
    DATA_TYPE_MULTIPOINT,
    DATA_TYPE_MULTILINESTRING,
    DATA_TYPE_MULTIPOLYGON,
    DATA_TYPE_GEOMETRYCOLLECTION,
    DATA_TYPE_GEOMETRY
  };
};

class LayerListItem
{
// a class wrapping a Layer
private:
  const void *PrivData;
  wxString DbPrefix;
  wxString LayerPrefix;
  int LayerType;
  wxString LayerName;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  wxString DataLicense;
  char *f_table_name;
  char *f_geometry_column;
  char *view_table_name;
  char *view_geometry_column;
  char *view_rowid_column;
  char *topology_name;
  char *network_name;
  int GeometryType;
  bool HasZ;
  int NativeSRID;
  bool Queryable;
  bool Editable;
  bool SpatialIndex;
  LayerListItem *Next;
public:
    LayerListItem(const void *priv_data, wxString & db_prefix, wxString & name,
                  wxString & title, wxString & abstract, wxString & copyright,
                  wxString & data_license, bool queryable, int srid);
    LayerListItem(const void *priv_data, wxString & db_prefix, wxString & name,
                  wxString & title, wxString & abstract, wxString & copyright,
                  wxString & data_license, int srid, bool queryable);
    LayerListItem(const void *priv_data, wxString & db_prefix,
                  wxString & layer_prefix, wxString & name, wxString & title,
                  wxString & abstract, wxString & copyright,
                  wxString & data_license, const char *f_table_name,
                  const char *f_geometry_column, int geom_type, int srid,
                  bool queryable, bool editable, bool spatial_index,
                  const char *view_table_name =
                  NULL, const char *view_geometry_column =
                  NULL, const char *view_rowid_column = NULL);
    LayerListItem(const void *priv_data, wxString & db_prefix,
                  wxString & layer_prefix, wxString & name, wxString & title,
                  wxString & abstract, wxString & copyright,
                  wxString & data_license, const char *f_table_name,
                  const char *f_geometry_column, int geom_type, int srid,
                  bool queryable);
    LayerListItem(const void *priv_data, wxString & db_prefix, int type,
                  wxString & name, wxString & title, wxString & abstract,
                  wxString & copyright, wxString & data_license,
                  const char *topo_name, bool has_z, int srid, bool queryable,
                  bool editable);
   ~LayerListItem();
    wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetLayerPrefix()
  {
    return LayerPrefix;
  }
  int GetLayerType()
  {
    return LayerType;
  }
  wxString & GetLayerName()
  {
    return LayerName;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  const char *GetTableName()
  {
    return f_table_name;
  }
  const char *GetGeometryColumn()
  {
    return f_geometry_column;
  }
  const char *GetViewMotherName()
  {
    return view_table_name;
  }
  const char *GetViewMotherGeometry()
  {
    return view_geometry_column;
  }
  const char *GetViewRowidColumn()
  {
    return view_rowid_column;
  }
  const char *GetTopologyName()
  {
    return topology_name;
  }
  const char *GetNetworkName()
  {
    return network_name;
  }
  int GetGeometryType()
  {
    return GeometryType;
  }
  bool GetHasZ()
  {
    return HasZ;
  }
  int GetSRID()
  {
    return NativeSRID;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  bool IsEditable()
  {
    return Editable;
  }
  bool HasSpatialIndex()
  {
    return SpatialIndex;
  }
  void SetNext(LayerListItem * next)
  {
    Next = next;
  }
  LayerListItem *GetNext()
  {
    return Next;
  }
};

class WmsLayerSettings
{
//
// a class for dynamically configuring WMS layers
//
private:
  char *Version;
  char *RefSys;
  char *Style;
  char *ImageFormat;
  char *BgColor;
  bool BgColorEnabled;
  int Opaque;
  int SwapXY;
  int Cached;
  int Tiled;
  int TileWidth;
  int TileHeight;
public:
    WmsLayerSettings();
   ~WmsLayerSettings();
  void SetVersion(const char *version);
  const char *GetVersion()
  {
    return Version;
  }
  void AdjustMapSRID(sqlite3 * sqlite, class MyFrame * MainFrame,
                     const char *url, wxString & db_prefix, wxString & name,
                     int srid);
  void SetRefSys(const char *ref_sys);
  const char *GetRefSys()
  {
    return RefSys;
  }
  void SetStyle(const char *style);
  const char *GetStyle()
  {
    return Style;
  }
  void SetImageFormat(const char *format);
  const char *GetImageFormat()
  {
    return ImageFormat;
  }
  void SetBgColor(const char *color);
  const char *GetBgColor()
  {
    return BgColor;
  }
  void SetBgColorEnabled(bool enabled)
  {
    BgColorEnabled = enabled;
  }
  bool IsBgColorEnabled()
  {
    return BgColorEnabled;
  }
  void SetOpaque(int mode)
  {
    Opaque = mode;
  }
  int IsOpaque()
  {
    return Opaque;
  }
  void SetSwapXY(int mode)
  {
    SwapXY = mode;
  }
  int IsSwapXY()
  {
    return SwapXY;
  }
  void SetCached(int mode)
  {
    Cached = mode;
  }
  int IsCached()
  {
    return Cached;
  }
  void SetTiled(int mode)
  {
    Tiled = mode;
  }
  int IsTiled()
  {
    return Tiled;
  }
  void SetTileWidth(int width)
  {
    TileWidth = width;
  }
  int GetTileWidth()
  {
    return TileWidth;
  }
  void SetTileHeight(int height)
  {
    TileHeight = height;
  }
  int GetTileHeight()
  {
    return TileHeight;
  }
};

class RasterLayerConfig
{
//
// a class for dynamically configuring Raster layers
//
private:
  wxString SampleType;
  wxString PixelType;
  int NumBands;
  wxString Compression;
  int Quality;
  int TileWidth;
  int TileHeight;
  double HorzResolution;
  double VertResolution;
  char *Style;
public:
    RasterLayerConfig();
   ~RasterLayerConfig();
  void SetConfig(wxString & sample_type, wxString & pixel_type, int bands,
                 wxString & compression, int quality, int tile_width,
                 int tile_height, double horz_resolution,
                 double vert_resolution);
    wxString & GetSampleType()
  {
    return SampleType;
  }
  wxString & GetPixelType()
  {
    return PixelType;
  }
  int GetNumBands()
  {
    return NumBands;
  }
  wxString GetCompression()
  {
    return Compression;
  }
  int GetQuality()
  {
    return Quality;
  }
  int GetTileWidth()
  {
    return TileWidth;
  }
  int GetTileHeight()
  {
    return TileHeight;
  }
  double GetHorzResolution()
  {
    return HorzResolution;
  }
  double GetVertResolution()
  {
    return VertResolution;
  }
  void SetStyle(const char *style);
  const char *GetStyle()
  {
    return Style;
  }
};

class VectorLayerConfig
{
//
// a class for dynamically configuring Vector layers
//
private:
  int GeometryType;
  char *Style;
  bool ShowFaces;
  bool ShowEdges;
  bool ShowNodes;
  bool ShowFaceSeeds;
  bool ShowEdgeSeeds;
  bool ShowLinks;
  bool ShowLinkSeeds;
public:
    VectorLayerConfig(int geom_type);
   ~VectorLayerConfig();
  int GetGeometryType()
  {
    return GeometryType;
  }
  void SetStyle(const char *style);
  const char *GetStyle()
  {
    return Style;
  }
  void SetFacesVisible(bool mode)
  {
    ShowFaces = mode;
  }
  bool AreFacesVisible()
  {
    return ShowFaces;
  }
  void SetEdgesVisible(bool mode)
  {
    ShowEdges = mode;
  }
  bool AreEdgesVisible()
  {
    return ShowEdges;
  }
  void SetNodesVisible(bool mode)
  {
    ShowNodes = mode;
  }
  bool AreNodesVisible()
  {
    return ShowNodes;
  }
  void SetFaceSeedsVisible(bool mode)
  {
    ShowFaceSeeds = mode;
  }
  bool AreFaceSeedsVisible()
  {
    return ShowFaceSeeds;
  }
  void SetEdgeSeedsVisible(bool mode)
  {
    ShowEdgeSeeds = mode;
  }
  bool AreEdgeSeedsVisible()
  {
    return ShowEdgeSeeds;
  }
  void SetLinksVisible(bool mode)
  {
    ShowLinks = mode;
  }
  bool AreLinksVisible()
  {
    return ShowLinks;
  }
  void SetLinkSeedsVisible(bool mode)
  {
    ShowLinkSeeds = mode;
  }
  bool AreLinkSeedsVisible()
  {
    return ShowLinkSeeds;
  }
};

class CachedFrame
{
//
// a class for caching a Layer rendered frame
//
private:
  const void *PrivData;
  int Width;
  int Height;
  double MinX;
  double MinY;
  double MaxX;
  double MaxY;
  char *Style;
  rl2GraphicsContextPtr Graphics;
  bool Ok;
public:
    CachedFrame(const void *priv_data);
   ~CachedFrame();
  bool IsValid(int width, int height, double minx, double miny, double maxx,
               double maxy, const char *style);
  void Reset(int width, int height, double minx, double miny, double maxx,
             double maxy, const char *style);
  rl2GraphicsContextPtr GetGraphicsContext()
  {
    return Graphics;
  }
  bool IsOk()
  {
    return Ok;
  }
  void Validate()
  {
    Ok = true;
  }
  void Invalidate();
};

class MapLayer
{
//
// a class corresponding to a Map layer 
//
private:
  const void *PrivData;
  int Type;
  WmsLayerSettings *WmsConfig;
  RasterLayerConfig *RasterConfig;
  VectorLayerConfig *VectorConfig;
  wxString DbPrefix;
  wxString VectorPrefix;
  wxString Name;
  wxString Title;
  wxString Abstract;
  wxString Copyright;
  wxString DataLicense;
  char *f_table_name;
  char *f_geometry_column;
  char *view_table_name;
  char *view_geometry_column;
  char *view_rowid_column;
  char *topology_name;
  char *network_name;
  char *getMap_url;
  char *getFeatureInfo_url;
  int GeometryType;
  bool HasZ;
  int NativeSRID;
  bool AutoTransformEnabled;
  int MapSRID;
  double GeoMinX;               // layer's BBOX - geographic coords
  double GeoMinY;
  double GeoMaxX;
  double GeoMaxY;
  double MinX;                  // layer's BBOX - native coords
  double MinY;
  double MaxX;
  double MaxY;
  double MapMinX;               // layer's BBOX - Map coords
  double MapMinY;
  double MapMaxX;
  double MapMaxY;
  bool Queryable;
  bool Editable;
  bool SpatialIndex;
  bool Visible;
  CachedFrame *CachedBase;
  CachedFrame *CachedLabels;
  CachedFrame *CachedNodes;
  CachedFrame *CachedEdges;
  CachedFrame *CachedLinks;
  CachedFrame *CachedFaces;
  CachedFrame *CachedEdgeSeeds;
  CachedFrame *CachedLinkSeeds;
  CachedFrame *CachedFaceSeeds;
  QuickStyleObj *QuickStyle;
  QuickStyleTopologyObj *QuickStyleTopology;
  QuickStyleRasterObj *QuickStyleRaster;
  QuickStyleWmsObj *QuickStyleWms;
  MapLayer *Prev;
  MapLayer *Next;
public:
    MapLayer(const void *priv_data, LayerListItem * layer);
   ~MapLayer();
  int GetType()
  {
    return Type;
  }
  QuickStyleObj *GetQuickStyle()
  {
    return QuickStyle;
  }
  QuickStyleTopologyObj *GetQuickStyleTopology()
  {
    return QuickStyleTopology;
  }
  QuickStyleRasterObj *GetQuickStyleRaster()
  {
    return QuickStyleRaster;
  }
  QuickStyleWmsObj *GetQuickStyleWms()
  {
    return QuickStyleWms;
  }
  QuickStyleObj *CloneQuickStyle()
  {
    return QuickStyle->Clone();
  }
  QuickStyleTopologyObj *CloneQuickStyleTopology()
  {
    return QuickStyleTopology->Clone();
  }
  QuickStyleRasterObj *CloneQuickStyleRaster()
  {
    return QuickStyleRaster->Clone();
  }
  QuickStyleWmsObj *CloneQuickStyleWms()
  {
    return QuickStyleWms->Clone();
  }
  bool UpdateQuickStyle(QuickStyleObj * style);
  bool UpdateQuickStyle(QuickStyleTopologyObj * style);
  bool UpdateQuickStyle(QuickStyleRasterObj * style);
  bool UpdateQuickStyle(QuickStyleWmsObj * style);
  WmsLayerSettings *GetWmsConfig()
  {
    return WmsConfig;
  }
  RasterLayerConfig *GetRasterConfig()
  {
    return RasterConfig;
  }
  VectorLayerConfig *GetVectorConfig()
  {
    return VectorConfig;
  }
  wxString & GetDbPrefix()
  {
    return DbPrefix;
  }
  wxString & GetVectorPrefix()
  {
    return VectorPrefix;
  }
  wxString & GetName()
  {
    return Name;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  wxString & GetCopyright()
  {
    return Copyright;
  }
  wxString & GetDataLicense()
  {
    return DataLicense;
  }
  const char *GetTableName()
  {
    return f_table_name;
  }
  const char *GetGeometryColumn()
  {
    return f_geometry_column;
  }
  const char *GetViewMotherName()
  {
    return view_table_name;
  }
  const char *GetViewMotherGeometry()
  {
    return view_geometry_column;
  }
  const char *GetViewRowidColumn()
  {
    return view_rowid_column;
  }
  const char *GetTopologyName()
  {
    return topology_name;
  }
  const char *GetNetworkName()
  {
    return network_name;
  }
  void SetWmsGetMapURL(const char *url);
  const char *GetWmsGetMapURL()
  {
    return getMap_url;
  }
  void SetWmsGetFeatureInfoURL(const char *url);
  const char *GetWmsGetFeatureInfoURL()
  {
    return getFeatureInfo_url;
  }
  int GetGeometryType()
  {
    return GeometryType;
  }
  bool GetHasZ()
  {
    return HasZ;
  }
  void SetNativeSRID(const char *srs);
  int GetNativeSRID()
  {
    return NativeSRID;
  }
  void SetMapSRID(int srid)
  {
    if (AutoTransformEnabled)
      MapSRID = srid;
  }
  void UpdateMapExtent(sqlite3 * sqlite);
  bool IsValidMapExtent();
  int GetMapSRID()
  {
    return MapSRID;
  }
  void EnableAutoTransform(bool mode)
  {
    AutoTransformEnabled = mode;
  }
  bool IsAutoTransformEnabled()
  {
    return AutoTransformEnabled;
  }
  bool DoCheckSupportedSRID(sqlite3 * sqlite, int srid);
  void AdjustMapSRID(sqlite3 * sqlite, MyFrame * MainFrame,
                     bool map_auto_transform_enabled, int srid);
  void AdjustDefaultStyle(sqlite3 * sqlite);
  void CreateQuickStyle();
  void SetQuickStyle(QuickStyleObj * quickStyle);
  void SetQuickStyle(QuickStyleTopologyObj * quickStyle);
  void SetQuickStyle(QuickStyleRasterObj * quickStyle);
  void SetQuickStyle(QuickStyleWmsObj * quickStyle);
  char *DoFindVectorStyle(sqlite3 * sqlite);
  char *DoFindRasterStyle(sqlite3 * sqlite);
  void SetRasterInfos(wxString & sample_type, wxString pixel_type, int bands,
                      wxString & compression, int quality, int tile_width,
                      int tile_height, double horz_res, double vert_res);
  void SetGeoExtent(double minx, double miny, double maxx, double maxy);
  void SetExtent(double minx, double miny, double maxx, double maxy);
  double GetGeoMinX()
  {
    return GeoMinX;
  }
  double GetGeoMinY()
  {
    return GeoMinY;
  }
  double GetGeoMaxX()
  {
    return GeoMaxX;
  }
  double GetGeoMaxY()
  {
    return GeoMaxY;
  }
  double GetMinX()
  {
    return MinX;
  }
  double GetMinY()
  {
    return MinY;
  }
  double GetMaxX()
  {
    return MaxX;
  }
  double GetMaxY()
  {
    return MaxY;
  }
  double GetMapMinX()
  {
    return MapMinX;
  }
  double GetMapMinY()
  {
    return MapMinY;
  }
  double GetMapMaxX()
  {
    return MapMaxX;
  }
  double GetMapMaxY()
  {
    return MapMaxY;
  }
  bool IsQueryable()
  {
    return Queryable;
  }
  bool IsEditable()
  {
    return Editable;
  }
  bool HasSpatialIndex()
  {
    return SpatialIndex;
  }
  bool IsSwapXY()
  {
    return false;
  }
  bool IsVisible()
  {
    return Visible;
  }
  bool IsReady();
  void Hide()
  {
    Visible = false;
  }
  void Show()
  {
    Visible = true;
  }
  void PrepareGraphicsContext(int width, int height, double minx, double miny,
                              double maxx, double maxy, const char *style);
  rl2CanvasPtr CreateCanvas();
  void Validate();
  void Validate(rl2CanvasPtr canvas);
  void Invalidate();
  rl2GraphicsContextPtr GetGraphicsContext();
  rl2GraphicsContextPtr GetLabelsGraphicsContext();
  void SetPrev(MapLayer * prev)
  {
    Prev = prev;
  }
  MapLayer *GetPrev()
  {
    return Prev;
  }
  void SetNext(MapLayer * next)
  {
    Next = next;
  }
  MapLayer *GetNext()
  {
    return Next;
  }
};

class MapLayerXmlVector
{
//
// an helper class for XML MapLayer's Configuration
//
private:
  char *Style;
  QuickStyleObj *QuickStyle;
public:
    MapLayerXmlVector(const char *style, QuickStyleObj * quick);
   ~MapLayerXmlVector();
  const char *GetStyle()
  {
    return Style;
  }
  QuickStyleObj *GetQuickStyle()
  {
    return QuickStyle;
  }
  bool IsDontPaintGeometrySymbolizer();
};

class MapLayerXmlTopology
{
//
// an helper class for XML MapLayer's Configuration
//
private:
  char *Style;
  QuickStyleTopologyObj *QuickStyle;
  bool ShowFaces;
  bool ShowEdges;
  bool ShowNodes;
  bool ShowFaceSeeds;
  bool ShowEdgeSeeds;
public:
    MapLayerXmlTopology(const char *style, QuickStyleTopologyObj * quick,
                        bool faces, bool edges, bool nodes, bool face_seeds,
                        bool edge_seeds);
   ~MapLayerXmlTopology();
  const char *GetStyle()
  {
    return Style;
  }
  QuickStyleTopologyObj *GetQuickStyle()
  {
    return QuickStyle;
  }
  bool GetShowFaces()
  {
    return ShowFaces;
  }
  bool GetShowEdges()
  {
    return ShowEdges;
  }
  bool GetShowNodes()
  {
    return ShowNodes;
  }
  bool GetShowFaceSeeds()
  {
    return ShowFaceSeeds;
  }
  bool GetShowEdgeSeeds()
  {
    return ShowEdgeSeeds;
  }
};

class MapLayerXmlNetwork
{
//
// an helper class for XML MapLayer's Configuration
//
private:
  char *Style;
  QuickStyleTopologyObj *QuickStyle;
  bool ShowLinks;
  bool ShowNodes;
  bool ShowLinkSeeds;
public:
    MapLayerXmlNetwork(const char *style, QuickStyleTopologyObj * quick,
                       bool links, bool nodes, bool link_seeds);
   ~MapLayerXmlNetwork();
  const char *GetStyle()
  {
    return Style;
  }
  QuickStyleTopologyObj *GetQuickStyle()
  {
    return QuickStyle;
  }
  bool GetShowLinks()
  {
    return ShowLinks;
  }
  bool GetShowNodes()
  {
    return ShowNodes;
  }
  bool GetShowLinkSeeds()
  {
    return ShowLinkSeeds;
  }
};

class MapLayerXmlRaster
{
//
// an helper class for XML MapLayer's Configuration
//
private:
  char *Style;
  QuickStyleRasterObj *QuickStyle;
public:
    MapLayerXmlRaster(const char *style, QuickStyleRasterObj * quick);
   ~MapLayerXmlRaster();
  const char *GetStyle()
  {
    return Style;
  }
  QuickStyleRasterObj *GetQuickStyle()
  {
    return QuickStyle;
  }
};

class MapLayerXmlWms
{
//
// an helper class for XML MapLayer's Configuration
//
private:
  bool MinScaleEnabled;
  bool MaxScaleEnabled;
  double ScaleMin;
  double ScaleMax;
  char *getMapURL;
  char *getFeatureInfoURL;
  char *Version;
  char *RefSys;
  char *Style;
  char *ImageFormat;
  char *BgColor;
  bool BgColorEnabled;
  bool Opaque;
  bool SwapXY;
  bool Tiled;
  int TileWidth;
  int TileHeight;
public:
    MapLayerXmlWms(bool min_scale_enabled, bool max_scale_enabled,
                   double min_scale, double max_scale, const char *getmap_url,
                   const char *getfeatureinfo_url, const char *version,
                   const char *ref_sys, const char *style,
                   const char *image_format, const char *bg_color,
                   bool bg_color_enabled, bool opaque, bool swap_xy, bool tiles,
                   int tile_w, int tile_h);
   ~MapLayerXmlWms();
  bool IsMinScaleEnabled()
  {
    return MinScaleEnabled;
  }
  bool IsMaxScaleEnabled()
  {
    return MaxScaleEnabled;
  }
  double GetScaleMin()
  {
    return ScaleMin;
  }
  double GetScaleMax()
  {
    return ScaleMax;
  }
  const char *GetWmsGetMap_URL()
  {
    return getMapURL;
  }
  const char *GetWmsGetFeatureInfo_URL()
  {
    return getFeatureInfoURL;
  }
  const char *GetVersion()
  {
    return Version;
  }
  const char *GetRefSys()
  {
    return RefSys;
  }
  const char *GetStyle()
  {
    return Style;
  }
  const char *GetImageFormat()
  {
    return ImageFormat;
  }
  const char *GetBgColor()
  {
    return BgColor;
  }
  bool IsBgColorEnabled()
  {
    return BgColorEnabled;
  }
  bool IsOpaque()
  {
    return Opaque;
  }
  bool IsSwapXY()
  {
    return SwapXY;
  }
  bool IsTiled()
  {
    return Tiled;
  }
  int GetTileWidth()
  {
    return TileWidth;
  }
  int GetTileHeight()
  {
    return TileHeight;
  }
};

class MapLayerXmlConfig
{
//
// an helper class for XML MapLayer's Configuration
//
private:
  int Type;
  char *TypeName;
  char *DbPrefix;
  char *Name;
  bool Visible;
  MapLayerXmlVector *Vector;
  MapLayerXmlTopology *Topology;
  MapLayerXmlNetwork *Network;
  MapLayerXmlRaster *Raster;
  MapLayerXmlWms *Wms;
  MapLayerXmlConfig *Prev;
  MapLayerXmlConfig *Next;
public:
    MapLayerXmlConfig(int type, char *db_prefix, char *name, bool visible);
   ~MapLayerXmlConfig();
  int GetType()
  {
    return Type;
  }
  const char *GetTypeName()
  {
    return TypeName;
  }
  const char *GetDbPrefix()
  {
    return DbPrefix;
  }
  const char *GetName()
  {
    return Name;
  }
  bool IsVisible()
  {
    return Visible;
  }
  MapLayerXmlVector *GetVector()
  {
    return Vector;
  }
  MapLayerXmlTopology *GetTopology()
  {
    return Topology;
  }
  MapLayerXmlNetwork *GetNetwork()
  {
    return Network;
  }
  MapLayerXmlRaster *GetRaster()
  {
    return Raster;
  }
  MapLayerXmlWms *GetWms()
  {
    return Wms;
  }
  void SetPrev(MapLayerXmlConfig * prev)
  {
    Prev = prev;
  }
  void AddVector(const char *style, QuickStyleObj * quick);
  void AddTopology(const char *style, QuickStyleTopologyObj * quick, bool faces,
                   bool edges, bool nodes, bool face_seeds, bool edge_seeds);
  void AddNetwork(const char *style, QuickStyleTopologyObj * quick, bool links,
                  bool nodes, bool link_seeds);
  void AddRaster(const char *style, QuickStyleRasterObj * quick);
  void AddWms(bool min_scale_enabled, bool max_scale_enabled, double min_scale,
              double max_scale, const char *getMapUrl,
              const char *getFeatureInfoUrl, const char *version,
              const char *ref_sys, const char *image_format, const char *style,
              const char *bg_color, bool bg_color_enabled, bool opaque,
              bool swap_xy, bool tiles, int tile_w, int tile_h);
  MapLayerXmlConfig *GetPrev()
  {
    return Prev;
  }
  void SetNext(MapLayerXmlConfig * next)
  {
    Next = next;
  }
  MapLayerXmlConfig *GetNext()
  {
    return Next;
  }
};

class MapAttachedXmlConfig
{
//
// an helper class for XML Map Configuration - Attached DBs
//
private:
  char *DbPrefix;
  char *Path;
  MapAttachedXmlConfig *Next;
public:
    MapAttachedXmlConfig(const char *db_prefix);
   ~MapAttachedXmlConfig();
  const char *GetDbPrefix()
  {
    return DbPrefix;
  }
  void SetPath(const char *path);
  const char *GetPath()
  {
    return Path;
  }
  void SetNext(MapAttachedXmlConfig * next)
  {
    Next = next;
  }
  MapAttachedXmlConfig *GetNext()
  {
    return Next;
  }
};

class MapXmlConfig
{
//
// an helper class for XML Map Configuration
//
private:
  bool Attached;
  MapAttachedXmlConfig *FirstDB;
  MapAttachedXmlConfig *LastDB;
  MapLayerXmlConfig *First;
  MapLayerXmlConfig *Last;
public:
    MapXmlConfig()
  {
    Attached = false;
    FirstDB = NULL;
    LastDB = NULL;
    First = NULL;
    Last = NULL;
  }
   ~MapXmlConfig();
  MapLayerXmlConfig *AddLayer(int type, char *db_prefix, char *name,
                              bool visible);
  MapAttachedXmlConfig *AddAttached(const char *db_prefix);
  MapAttachedXmlConfig *GetFirstDB()
  {
    return FirstDB;
  }
  bool HasAttached()
  {
    return Attached;
  }
  void SetDbPath(const char *db_prefix, const char *path);
  MapLayerXmlConfig *GetFirst()
  {
    return First;
  }
  MapLayerXmlConfig *GetLast()
  {
    return Last;
  }

};

class MyLayerTree:public wxTreeCtrl
{
//
// a tree-control used for Map Layers
//
private:
  MyMapPanel * MapPanel;
  bool Changed;
  wxTreeItemId Root;            // the root node
  wxImageList *Images;          // the images list
  wxTreeItemId CurrentItem;     // the tree item holding the current context menu
  wxTreeItemId DraggedItem;     // the tree item to be moved
  void DoWmsSqlSample(MapLayer * lyr);
  void DoWmsUrlSample(MapLayer * lyr);
  void DoVectorSqlSample(MapLayer * lyr);
  void DoRasterSqlSample(MapLayer * lyr);
  void DoPrepareHexColor(char *color, wxColour & background);
public:
    MyLayerTree()
  {;
  }
  MyLayerTree(MyMapPanel * parent, wxWindowID id = wxID_ANY);
  virtual ~ MyLayerTree();
  void FlushAll()
  {
    DeleteChildren(Root);
    Changed = true;
  }
  wxTreeItemId & GetRoot()
  {
    return Root;
  }
  void CleanUp();
  void DoFetchWmsUrls(const char *db_prefix, const char *name, char **getMapUrl,
                      char **getFeatureInfoUrl);
  char *PrintQuickStyleComplex(char *xml, class QuickStyleObj * quick);
  char *PrintQuickStylePoint(char *xml, const char *extra,
                             class QuickStyleObj * quick);
  char *PrintQuickStyleLine(char *xml, const char *extra,
                            QuickStyleObj * quick);
  char *PrintQuickStylePolygon(char *xml, const char *extra,
                               QuickStyleObj * quick);
  char *PrintQuickStyleTextPoint(QuickStyleObj * quick);
  char *PrintQuickStyleTextLine(QuickStyleObj * quick);
  char *PrintQuickStyleRaster(QuickStyleRasterObj * quick);
  char *PrintQuickStyleTopology(QuickStyleTopologyObj * quick,
                                const char *indent);
  char *PrintQuickStyleNetwork(QuickStyleTopologyObj * quick,
                               const char *indent);
  char *XmlClean(const char *dirty);
  void AddLayer(MapLayer * layer);
  int GetIconIndex(MapLayer * layer);
  void SetLayerIcons();
  void MarkCurrentItem();
  void SelectActiveItem(MapLayer * layer);
  void UpdateDefault(wxTreeItemId item, wxImageList * imageList);
  void OnSelChanged(wxTreeEvent & event);
  void OnItemActivated(wxTreeEvent & event);
  void OnRightClick(wxTreeEvent & event);
  void OnCmdRemoveAll(wxCommandEvent & event);
  void OnCmdShowAll(wxCommandEvent & event);
  void OnCmdHideAll(wxCommandEvent & event);
  void OnCmdMapConfigure(wxCommandEvent & event);
  void OnCmdVisible(wxCommandEvent & event);
  void OnCmdMapFullExtent(wxCommandEvent & event);
  void OnCmdMapLayerFullExtent(wxCommandEvent & event);
  void OnCmdMapSqlSample(wxCommandEvent & event);
  void OnCmdMapUrlSample(wxCommandEvent & event);
  void OnCmdSqlLayer(wxCommandEvent & event);
  void OnCmdRemoveLayer(wxCommandEvent & event);
  void OnCmdDeleteItem(wxCommandEvent & event);
  void OnCmdLayerInfo(wxCommandEvent & event);
  void OnCmdMapLayerConfigure(wxCommandEvent & event);
  void OnCmdQuickStyleEdit(wxCommandEvent & event);
  void OnDragStart(wxTreeEvent & event);
  void OnDragEnd(wxTreeEvent & event);
};

class MapLayerObject:public wxTreeItemData
{
//
// a class to store TreeItemData - Map Layer wrapper
//
private:
  MapLayer * Layer;
public:
  MapLayerObject(MapLayer * lyr)
  {
    Layer = lyr;
  }
  virtual ~ MapLayerObject()
  {;
  }
  MapLayer *GetLayer()
  {
    return Layer;
  }
};

class LayerThreadParams
{
// a class wrapping Layer Paint arguments
private:
  int ThreadIndex;
  class SingleLayerPainter *Layer;
  MyMapPanel *MapPanel;
  class MyMapView *MapView;
  class MultiLayerPainter *Canvas;
  class MapViewPaintParams *Mother;
public:
    LayerThreadParams(int index, SingleLayerPainter * lyr, MyMapPanel * panel,
                      MyMapView * view, MultiLayerPainter * canvas,
                      MapViewPaintParams * mother)
  {
    ThreadIndex = index;
    Layer = lyr;
    MapPanel = panel;
    MapView = view;
    Canvas = canvas;
    Mother = mother;
  }
   ~LayerThreadParams()
  {;
  }
  int GetThreadIndex()
  {
    return ThreadIndex;
  }
  SingleLayerPainter *GetLayer()
  {
    return Layer;
  }
  MyMapPanel *GetMapPanel()
  {
    return MapPanel;
  }
  MyMapView *GetMapView()
  {
    return MapView;
  }
  MultiLayerPainter *GetCanvas()
  {
    return Canvas;
  }
  MapViewPaintParams *GetMother()
  {
    return Mother;
  }
};

class MapViewPaintParams
{
// a class wrapping Map Paint arguments
private:
  MyMapPanel * MapPanel;
  MyMapView *MapView;
  MultiLayerPainter *Canvas;
  bool *UnusedThreads;
  int NumThreads;
public:
    MapViewPaintParams()
  {
    UnusedThreads = NULL;
    NumThreads = 0;
  }
   ~MapViewPaintParams()
  {
    ThreadsPoolCleanUp();
  }
  void Initialize(MyMapPanel * mother, MyMapView * map,
                  MultiLayerPainter * canvas)
  {
    MapPanel = mother;
    MapView = map;
    Canvas = canvas;
  }
  MyMapPanel *GetMapPanel()
  {
    return MapPanel;
  }
  MyMapView *GetMapView()
  {
    return MapView;
  }
  MultiLayerPainter *GetCanvas()
  {
    return Canvas;
  }
  void ThreadsPoolCleanUp()
  {
    if (UnusedThreads != NULL)
      delete[]UnusedThreads;
    UnusedThreads = NULL;
    NumThreads = 0;
  }
  void PrepareThreadsPool(int count);
  int FindUnusedThread();
  void ReleaseThread(int index);
};

class MapConfigAttachedDb
{
//
// a class wrapping an ATTACHed DB for Map Config
//
private:
  char *Prefix;
  char *Remapped;
  char *Path;
  bool Valid;
  bool DontDetach;
  MapConfigAttachedDb *Next;
  bool IsAlreadyAttached(sqlite3 * sqlite, const char *prefix,
                         const char *path);
  bool NotAlreadyAttached(sqlite3 * sqlite, const char *alias);
  bool CheckValidDbFile(const char *path);
public:
    MapConfigAttachedDb(const char *prefix, const char *path);
   ~MapConfigAttachedDb();
  const char *GetPrefix()
  {
    return Prefix;
  }
  const char *GetRemapped()
  {
    return Remapped;
  }
  const char *GetPath()
  {
    return Path;
  }
  bool IsValid()
  {
    return Valid;
  }
  bool IsDontDetach()
  {
    return DontDetach;
  }
  void Verify(sqlite3 * sqlite, MyFrame * parent);
  void SetNext(MapConfigAttachedDb * next)
  {
    Next = next;
  }
  MapConfigAttachedDb *GetNext()
  {
    return Next;
  }
};

class MapConfigAttached
{
//
// a list of ATTACHed DBs for Map Config
//
private:
  MyFrame * Parent;
  MapConfigAttachedDb *First;
  MapConfigAttachedDb *Last;
public:
    MapConfigAttached(MyFrame * parent);
   ~MapConfigAttached();
  MapConfigAttachedDb *GetFirst()
  {
    return First;
  }
  void Add(const char *prefix, const char *path);
  void Verify();
  void DetachAll();
  bool DoCheckPrefix(const char *prefix, const char **alias);
};

class MyMapView:public wxPanel
{
//
// a panel used to show the Map
//
private:
  MyMapPanel * MapPanel;
  wxBitmap MapBitmap;
  wxBitmap ScreenBitmap;
  wxBitmap BlinkBitmap;
  int BitmapWidth;
  int BitmapHeight;
  MapFeaturesList *SelectedFeatures;
  wxCursor CursorCross;
  wxCursor CursorHand;
  bool RasterWmsAutoSwitch;
  bool LabelAntiCollision;
  bool LabelWrapText;
  bool LabelAutoRotate;
  bool LabelShiftPosition;
  bool GeographicCoordsDMS;
  bool CheckeredMapBackground;
  wxColour SolidMapBackground;
  bool ValidMap;
  int MapSRID;
  bool MultiThreadingEnabled;
  int MaxThreads;
  bool AutoTransformEnabled;
  double MapCX;
  double MapCY;
  double MapMinX;
  double MapMaxX;
  double MapMinY;
  double MapMaxY;
  double MapExtentX;
  double MapExtentY;
  double GeoMinX;
  double GeoMaxX;
  double GeoMinY;
  double GeoMaxY;
  int FrameWidth;
  int FrameHeight;
  double FrameCX;
  double FrameCY;
  double FrameExtentX;
  double FrameExtentY;
  double FrameMinX;
  double FrameMinY;
  double FrameMaxX;
  double FrameMaxY;
  double PixelRatio;
  int DragStartX;
  int DragStartY;
  int LastDragX;
  int LastDragY;
  int WheelTics;
  wxTimer *TimerMouseWheel;
  wxTimer *TimerMapPaint;
  wxTimer *TimerMapBlink;
  wxTimer *TimerMapTip;
  int CurrentScale;
  int MapPaintPhase;
  int MapBlinkCount;
  bool DynamicBlink;
  MapLayer *FirstLayer;
  MapLayer *LastLayer;
  MapLayer *ActiveLayer;
  MultiLayerPainter *CurrentCanvas;
  MapViewPaintParams MapThreadParams;
  MapConfigAttached *AttachedList;
  rl2MapConfigPtr LoadedMapConfig;
  char *CurrentMapConfigXML;
  char *CurrentMapConfigName;
  void DoPaintCheckeredBackground(wxMemoryDC * dc);
  bool TestSelectedFeaturesBBox();
public:
    MyMapView()
  {;
  }
  MyMapView(MyMapPanel * parent, wxWindowID id = wxID_ANY);
  virtual ~ MyMapView();
  void MapPanelClosing();
  void ResetScreenBitmap();
  void ResetMapBitmap(wxImage & img, unsigned int width, unsigned int height);
  MyMapPanel *GetMapPanel()
  {
    return MapPanel;
  }
  MapConfigAttached *CreateAttachedList();
  void DetachAll();
  void SetLoadedMapConfiguration(rl2MapConfigPtr config);
  bool MapConfigurationChanged();
  void SaveMapConfiguration();
  void SetMapSRID(int srid)
  {
    if (AutoTransformEnabled)
      MapSRID = srid;
  }
  int GetMapSRID()
  {
    return MapSRID;
  }
  void EnableMultiThreading(bool mode)
  {
    MultiThreadingEnabled = mode;
  }
  int GetImageWidth();
  int GetImageHeight();
  double GetMapMinX()
  {
    return MapMinX;
  }
  double GetMapMinY()
  {
    return MapMinY;
  }
  double GetMapMaxX()
  {
    return MapMaxX;
  }
  double GetMapMaxY()
  {
    return MapMaxY;
  }
  double GetGeoMinX()
  {
    return GeoMinX;
  }
  double GetGeoMinY()
  {
    return GeoMinY;
  }
  double GetGeoMaxX()
  {
    return GeoMaxX;
  }
  double GetGeoMaxY()
  {
    return GeoMaxY;
  }
  void DoPrepareBBox(wxString & bbox);
  void GetBBox(int *srid, double *minx, double *miny, double *maxx,
               double *maxy);
  MapLayer *GetFirstLayer()
  {
    return FirstLayer;
  }
  bool IsMultiThreadingEnabled()
  {
    return MultiThreadingEnabled;
  }
  void SetMaxThreads(int max)
  {
    MaxThreads = max;
  }
  int GetMaxThreads()
  {
    return MaxThreads;
  }
  void EnableAutoTransform(bool mode)
  {
    AutoTransformEnabled = mode;
  }
  bool IsAutoTransformEnabled()
  {
    return AutoTransformEnabled;
  }
  bool IsRasterWmsAutoSwitch()
  {
    return RasterWmsAutoSwitch;
  }
  void SetRasterWmsAutoSwitch(bool mode)
  {
    RasterWmsAutoSwitch = mode;
  }
  bool IsLabelAntiCollision()
  {
    return LabelAntiCollision;
  }
  void SetLabelAntiCollision(bool mode)
  {
    LabelAntiCollision = mode;
  }
  bool IsLabelWrapText()
  {
    return LabelWrapText;
  }
  void SetLabelWrapText(bool mode)
  {
    LabelWrapText = mode;
  }
  bool IsLabelAutoRotate()
  {
    return LabelAutoRotate;
  }
  void SetLabelAutoRotate(bool mode)
  {
    LabelAutoRotate = mode;
  }
  bool IsLabelShiftPosition()
  {
    return LabelShiftPosition;
  }
  void SetLabelShiftPosition(bool mode)
  {
    LabelShiftPosition = mode;
  }
  bool IsGeographicCoordsDMS()
  {
    return GeographicCoordsDMS;
  }
  void SetGeographicCoordsDMS(bool mode)
  {
    GeographicCoordsDMS = mode;
  }
  bool IsCheckeredMapBackground()
  {
    return CheckeredMapBackground;
  }
  void SetCheckeredMapBackground(bool mode)
  {
    CheckeredMapBackground = mode;
  }
  wxColour & GetSolidMapBackground()
  {
    return SolidMapBackground;
  }
  void SetSolidMapBackground(wxColour & color)
  {
    SolidMapBackground = color;
  }
  void AdjustLayersMapSRID();
  int GetCurrentScale()
  {
    return CurrentScale;
  }
  void SetCurrentScale(int scale);
  void ShowSelectedFeatures(MapFeaturesList * list, bool zoom_mode);

  void Invalidate();
  void PrepareMap();
  void UpdateMapFullExtent();
  void SetMapToFullExtent();
  void SetMapExtent(double minX, double minY, double maxX, double maxY);
  void SetMapLayerToFullExtent(MapLayer * lyr);
  void UpdateMapViewPoint(int fromSrid, int toSrid);
  void RebuildLayerTree(MyLayerTree * LayerTree);
  void DoPaintMap();
  void DoPaintLayer(SingleLayerPainter * layer);
  void DoPaintLayerWms(SingleLayerPainter * layer);
  void DoPaintLayerRaster(SingleLayerPainter * layer);
  void DoPaintLayerVector(SingleLayerPainter * layer);
  void RunMonoThreadPaintMap();
  void RunMultiThreadPaintMap();
  void DoFinalMapImage();
  void DoPaintMapBlink(bool oddEven);
  void DoPaintBlinkingPoint(rl2GraphicsContextPtr ctx, bool oddEven,
                            gaiaPointPtr pt);
  void DoPaintBlinkingLinestring(rl2GraphicsContextPtr ctx, bool oddEven,
                                 gaiaLinestringPtr ln);
  void DoPaintBlinkingPolygon(rl2GraphicsContextPtr ctx, bool oddEven,
                              gaiaPolygonPtr pg);
  void DoIdentify(int x, int y);
  void DoIdentifyVector(double x, double y);
  void DoIdentifyTopology(double x, double y);
  void DoIdentifyNetwork(double x, double y);
  void DoIdentifyRaster(double x, double y);

  void SetActiveMapLayer(MapLayer * layer)
  {
    ActiveLayer = layer;
  }
  MapLayer *GetActiveLayer()
  {
    return ActiveLayer;
  }
  bool IsActiveLayer(MapLayer * layer)
  {
    if (ActiveLayer == layer)
      return true;
    return false;
  }
  bool IsValidMap()
  {
    return ValidMap;
  }
  void DragMap(int x, int y);
  bool CanQueryTable();
  bool CanIdentify();
  void ResetMapLayers();
  bool InsertMapLayer(LayerListItem * layer, MyFrame * MainFrame);
  bool InsertMapLayer(MapLayer * layer);
  void DoFetchRasterExtent(MapLayer * layer);
  void DoFetchVectorExtent(MapLayer * layer);
  void DoFetchWMSextent(MapLayer * layer);
  void DoFetchWMSconfig(MapLayer * layer);
  void DoFetchRasterInfos(MapLayer * layer);
  void DoFetchGetMapURL(MapLayer * layer);
  void RemoveMapLayer(MapLayer * layer);
  void ReinsertMapLayer(MapLayer * layer);
  bool IsAlreadyDefined(LayerListItem * layer);

  void OnEraseBackground(wxEraseEvent & WXUNUSED(event))
  {;
  }
  void OnSize(wxSizeEvent & event);
  void OnPaint(wxPaintEvent & event);
  void OnMapImagePaintStep(wxCommandEvent & event);
  void OnThreadFinished(wxCommandEvent & event);
  void OnMouseMove(wxMouseEvent & event);
  void OnMouseDragStop(wxMouseEvent & event);
  void OnMouseClick(wxMouseEvent & event);
  void OnMouseWheel(wxMouseEvent & event);
  void OnTimerMouseWheel(wxTimerEvent & event);
  void OnTimerMapPaint(wxTimerEvent & event);
  void OnTimerMapBlink(wxTimerEvent & event);
  void OnTimerMapTip(wxTimerEvent & event);
  void OnKeyCenter(wxCommandEvent & event);
  void OnKeyUp(wxCommandEvent & event);
  void OnKeyDown(wxCommandEvent & event);
  void OnKeyLeft(wxCommandEvent & event);
  void OnKeyRight(wxCommandEvent & event);
  void OnKeyZoomIn(wxCommandEvent & event);
  void OnKeyZoomOut(wxCommandEvent & event);
  void OnKeyMicroUp(wxCommandEvent & event);
  void OnKeyMicroDown(wxCommandEvent & event);
  void OnKeyMicroLeft(wxCommandEvent & event);
  void OnKeyMicroRight(wxCommandEvent & event);
  void OnKeyMicroZoomIn(wxCommandEvent & event);
  void OnKeyMicroZoomOut(wxCommandEvent & event);
};

class MyMapPanelStatusBar:public wxStatusBar
{
//
// a status bar supporting Map Mapel
//
private:
  class MyMapPanel * Parent;
  wxStaticBitmap *Semaphore;
public:
    MyMapPanelStatusBar(MyMapPanel * parent);
    virtual ~ MyMapPanelStatusBar()
  {;
  }
  void SetRedLight();
  void SetGreenLight();
  void SetYellowLight();
  void OnSize(wxSizeEvent & event);
};

class MyMapPanel:public wxFrame
{
//
// the main Map Panel
//
private:
  MyFrame * Parent;
  MyMapPanelStatusBar *StatusBar;
  wxString MapName;
  wxString Title;
  wxString Abstract;
  wxAuiManager Manager;         // the GUI manager
  wxString ConfigLayout;        // PERSISTENCY - the layout configuration
  int ConfigPaneX;              // PERSISTENCY - the map pane screen origin X
  int ConfigPaneY;              // PERSISTENCY - the map pane screen origin Y
  int ConfigPaneWidth;          // PERSISTENCY - the map pane screen width
  int ConfigPaneHeight;         // PERSISTENCY - the map pane screen height
  MyLayerTree *LayerTree;       // the layer tree list
  MyMapView *MapView;           // the map panel
  wxBitmap *BtnLoadMapConfig;   // button icon for loading a Map Configuration
  wxBitmap *BtnAddLayer;        // button icon for adding a Map Layer
  wxBitmap *BtnSqlLayer;        // button icon for querying a Map Layer
  wxBitmap *BtnIdentify;        // buttom icon for Identify
  wxBitmap *BtnZoomIn;          // button icon for Zoom In
  wxBitmap *BtnZoomOut;         // buttom icon for Zoom Out
  wxBitmap *BtnPan;             // button icon for Pan
  wxBitmap *BtnMapImage;        // button icon for MapImage
  wxBitmap *BtnPrinter;         // button icon for Printer (PDF)
  wxBitmap *BtnAbout;           // button icon for ABOUT
  wxBitmap *BtnExit;            // button icon for EXIT
  bool IsIdentify;              // current map click is: Identify
  bool IsZoomIn;                // current map click is: ZoomIn
  bool IsZoomOut;               // current map click is: ZoomOut
  bool IsPan;                   // current map click is: Pan
  IncompleteLayersList *CheckIncompleteLayers();
  MapLayer *DoFetchLayerVector(const char *prefix, const char *name);
  MapLayer *DoFetchLayerVectorView(const char *prefix, const char *name);
  MapLayer *DoFetchLayerVectorVirtual(const char *prefix, const char *name);
  MapLayer *DoFetchLayerTopology(const char *prefix, const char *name);
  MapLayer *DoFetchLayerNetwork(const char *prefix, const char *name);
  MapLayer *DoFetchLayerWMS(const char *prefix, const char *name);
  MapLayer *DoFetchLayerRaster(const char *prefix, const char *name);
  void GenericVector(MapLayer * mapLyr, rl2MapLayerPtr lyr);
  void TopologyLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr);
  void NetworkLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr);
  void DoWmsLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr);
  void RasterLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr);
  int DoFindBrushId(const char *resource);
  bool DoFetchRasterMaxMinPixelValues(sqlite3 * sqlite, const char *prefix,
                                      const char *name, double *min,
                                      double *max);
public:
    MyMapPanel(MyFrame * parent, const wxString & title, const wxPoint & pos,
               const wxSize & size);
    virtual ~ MyMapPanel();
  void DetachAll();
  MyFrame *GetParent()
  {
    return Parent;
  }
  void SetMapName(wxString & name)
  {
    MapName = name;
  }
  wxString & GetMapName()
  {
    return MapName;
  }
  void SetTitle(wxString & title)
  {
    Title = title;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  void SetAbstract(wxString & abstract)
  {
    Abstract = abstract;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  void UpdateTools();
  void UpdateMapScale();
  void UpdateMapCoords(wxString & coords);
  void UpdateMapMode(wxString & mode);
  void UpdateCurrentMapMode();
  char *DoFetchRefSysName();
  void UpdateMapSRID();
  void SetRedLight()
  {
    StatusBar->SetRedLight();
  }
  void SetGreenLight()
  {
    StatusBar->SetGreenLight();
  }
  void SetYellowLight()
  {
    StatusBar->SetYellowLight();
  }
  void SetLoadedMapConfiguration(rl2MapConfigPtr config)
  {
    MapView->SetLoadedMapConfiguration(config);
  }
  void ParentQuit();
  bool IsValidMap()
  {
    return MapView->IsValidMap();
  }
  void UpdateMapFullExtent()
  {
    MapView->UpdateMapFullExtent();
  }
  void SetMapToFullExtent()
  {
    MapView->SetMapToFullExtent();
    MapView->PrepareMap();
  }
  void SetMapExtent(double minX, double minY, double maxX, double maxY)
  {
    MapView->SetMapExtent(minX, minY, maxX, maxY);
    MapView->PrepareMap();
  }
  void SetMapLayerToFullExtent(MapLayer * lyr)
  {
    MapView->SetMapLayerToFullExtent(lyr);
    MapView->PrepareMap();
  }
  void ShowSelectedFeatures(MapFeaturesList * list, bool zoom_mode)
  {
    MapView->ShowSelectedFeatures(list, zoom_mode);
  }
  void InitializeSqlFilters(wxString & db_prefix, wxString & table,
                            bool read_only, wxString & geom_column)
  {
    Parent->InitializeSqlFilters(db_prefix, table, read_only, geom_column);
  }
  void UpdateMapViewPoint(int fromSrid, int toSrid)
  {
    MapView->UpdateMapViewPoint(fromSrid, toSrid);
  }
  int GetImageWidth()
  {
    return MapView->GetImageWidth();
  }
  int GetImageHeight()
  {
    return MapView->GetImageHeight();
  }
  void DoPrepareBBox(wxString & bbox)
  {
    MapView->DoPrepareBBox(bbox);
  }
  void GetBBox(int *srid, double *minx, double *miny, double *maxx,
               double *maxy)
  {
    MapView->GetBBox(srid, minx, miny, maxx, maxy);
  }
  double GetMapMinX()
  {
    return MapView->GetMapMinX();
  }
  double GetMapMinY()
  {
    return MapView->GetMapMinY();
  }
  double GetMapMaxX()
  {
    return MapView->GetMapMaxX();
  }
  double GetMapMaxY()
  {
    return MapView->GetMapMaxY();
  }
  double GetGeoMinX()
  {
    return MapView->GetGeoMinX();
  }
  double GetGeoMinY()
  {
    return MapView->GetGeoMinY();
  }
  double GetGeoMaxX()
  {
    return MapView->GetGeoMaxX();
  }
  double GetGeoMaxY()
  {
    return MapView->GetGeoMaxY();
  }
  void EnableMapMultiThreading(bool mode)
  {
    Parent->EnableMapMultiThreading(mode);
    MapView->EnableMultiThreading(mode);
  }
  bool IsMapMultiThreadingEnabled()
  {
    return Parent->IsMapMultiThreadingEnabled();
  }
  void SetMapMaxThreads(int max)
  {
    Parent->SetMapMaxThreads(max);
    MapView->SetMaxThreads(max);
  }
  int GetMapMaxThreads()
  {
    return Parent->GetMapMaxThreads();
  }
  void UpdateMaxThreads();
  rl2WmsCachePtr GetWmsCache()
  {
    return Parent->GetWmsCache();
  }
  wxString & GetHttpProxy()
  {
    return Parent->GetHttpProxy();
  }
  void EnableMapAutoTransform(bool mode)
  {
    Parent->EnableMapAutoTransform(mode);
    MapView->EnableAutoTransform(mode);
  }
  bool IsMapAutoTransformEnabled()
  {
    return Parent->IsMapAutoTransformEnabled();
  }
  bool IsRasterWmsAutoSwitch()
  {
    return MapView->IsRasterWmsAutoSwitch();
  }
  void SetRasterWmsAutoSwitch(bool mode)
  {
    MapView->SetRasterWmsAutoSwitch(mode);
  }
  bool IsLabelAntiCollision()
  {
    return MapView->IsLabelAntiCollision();
  }
  void SetLabelAntiCollision(bool mode)
  {
    MapView->SetLabelAntiCollision(mode);
  }
  bool IsLabelWrapText()
  {
    return MapView->IsLabelWrapText();
  }
  void SetLabelWrapText(bool mode)
  {
    MapView->SetLabelWrapText(mode);
  }
  bool IsLabelAutoRotate()
  {
    return MapView->IsLabelAutoRotate();
  }
  void SetLabelAutoRotate(bool mode)
  {
    MapView->SetLabelAutoRotate(mode);
  }
  bool IsLabelShiftPosition()
  {
    return MapView->IsLabelShiftPosition();
  }
  void SetLabelShiftPosition(bool mode)
  {
    MapView->SetLabelShiftPosition(mode);
  }
  bool IsGeographicCoordsDMS()
  {
    return MapView->IsGeographicCoordsDMS();
  }
  void SetGeographicCoordsDMS(bool mode)
  {
    MapView->SetGeographicCoordsDMS(mode);
  }
  void SetSql(wxString & str, bool execute)
  {
    wxString dummy;
    return Parent->SetSql(str, execute, false, dummy, dummy, true);
  }
  bool IsCheckeredMapBackground()
  {
    return MapView->IsCheckeredMapBackground();
  }
  void SetCheckeredMapBackground(bool mode)
  {
    MapView->SetCheckeredMapBackground(mode);
  }
  wxColour & GetSolidMapBackground()
  {
    return MapView->GetSolidMapBackground();
  }
  void SetSolidMapBackground(wxColour & color)
  {
    MapView->SetSolidMapBackground(color);
  }
  void SetRasterWmwAutoSwitch(bool mode)
  {
    MapView->SetRasterWmsAutoSwitch(mode);
  }
  MapConfigAttached *CreateAttachedList()
  {
    return MapView->CreateAttachedList();
  }
  bool IsModeIdentify()
  {
    return IsIdentify;
  }
  bool IsModeZoomIn()
  {
    return IsZoomIn;
  }
  bool IsModeZoomOut()
  {
    return IsZoomOut;
  }
  bool IsModePan()
  {
    return IsPan;
  }
  void SetMapSRID(int srid)
  {
    MapView->SetMapSRID(srid);
  }
  int GetMapSRID()
  {
    return MapView->GetMapSRID();
  }
  void AdjustLayersMapSRID()
  {
    MapView->AdjustLayersMapSRID();
  }
  bool IsGeographicSRID(int srid);

  void ResetMapLayers()
  {
    MapView->ResetMapLayers();
  }
  MapLayer *GetFirstMapLayer()
  {
    return MapView->GetFirstLayer();
  }
  void ReinsertMapLayer(MapLayer * layer)
  {
    MapView->ReinsertMapLayer(layer);
  }
  MapLayer *GetActiveLayer()
  {
    return MapView->GetActiveLayer();
  }
  void SetActiveMapLayer(MapLayer * layer)
  {
    MapView->SetActiveMapLayer(layer);
    UpdateTools();
  }
  bool IsActiveLayer(MapLayer * layer)
  {
    return MapView->IsActiveLayer(layer);
  }
  void RemoveMapLayer(MapLayer * layer)
  {
    MapView->RemoveMapLayer(layer);
  }
  bool InsertMapLayer(LayerListItem * layer, MyFrame * MainFrame)
  {
    return MapView->InsertMapLayer(layer, MainFrame);
  }
  MyLayerTree *GetLayerTree()
  {
    return LayerTree;
  }
  void RebuildLayerTree()
  {
    MapView->RebuildLayerTree(LayerTree);
  }
  bool IsAlreadyDefined(LayerListItem * layer)
  {
    return MapView->IsAlreadyDefined(layer);
  }
  void RefreshMap()
  {
    MapView->PrepareMap();
  }
  sqlite3 *GetSqlite()
  {
    return Parent->GetSqlite();
  }
  void *GetRL2PrivateData()
  {
    return Parent->GetRL2PrivateData();
  }
  void ResolveAttachedDbPaths(MapXmlConfig * cfg);

  void OnLoadMapConfig(wxCommandEvent & event);
  void OnAddLayer(wxCommandEvent & event);
  void OnSqlLayer(wxCommandEvent & event);
  void OnIdentify(wxCommandEvent & event);
  void OnZoomIn(wxCommandEvent & event);
  void OnZoomOut(wxCommandEvent & event);
  void OnPan(wxCommandEvent & event);
  void OnMapImage(wxCommandEvent & event);
  void OnPrinter(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
  void OnAbout(wxCommandEvent & event);
};

class AddMapLayerDialog:public wxDialog
{
//
// a Dialog for adding a Map Layer
//
private:
  MyMapPanel * MapPanel;
  LayerListItem *First;
  LayerListItem *Last;
  wxGrid *Layers;
  int NumRows;
  void FlushList();
  void AddLayer2List(wxString & db_prefix, wxString & name, wxString & title,
                     wxString & abstract, wxString & copyright,
                     wxString & data_license, bool queryable, int srid);
  void AddLayer2List(wxString & db_prefix, wxString & name, wxString & title,
                     wxString & abstract, wxString & copyright,
                     wxString & data_license, int srid, bool queryable);
  void AddLayer2List(wxString & db_prefix, wxString & layer_prefix,
                     wxString & name, wxString & title, wxString & abstract,
                     wxString & copyright, wxString & data_license,
                     const char *f_table_name, const char *f_geometry_column,
                     int geom_type, int srid, bool queryable, bool editable,
                     bool spatial_index, const char *view_table_name =
                     NULL, const char *view_geometry_column =
                     NULL, const char *view_rowid_column = NULL);
  void AddLayer2List(wxString & db_prefix, int type, wxString & name,
                     wxString & title, wxString & abstract,
                     wxString & copyright, wxString & data_license,
                     const char *topo_name, bool has_z, int srid,
                     bool queryable, bool editable);
  void DoLoadVectorCoverages();
  void DoLoadTopologyCoverages();
  void DoLoadNetworkCoverages();
  void DoLoadRasterCoverages();
  void DoLoadWmsCoverages();
  char *QueryVectorCoverages(const char *db_prefix, const char *old_sql);
  char *QuerySpatialViewCoverages(const char *db_prefix, const char *old_sql);
  char *QueryVirtualTableCoverages(const char *db_prefix, const char *old_sql);
  char *QueryTopologyCoverages(const char *db_prefix, const char *old_sql);
  char *QueryNetworkCoverages(const char *db_prefix, const char *old_sql);
  char *QueryRasterCoverages(const char *db_prefix, const char *old_sql);
  char *QueryWmsCoverages(const char *db_prefix, const char *old_sql);
  bool DoCheckGeometryColumns(const char *db_prefix);
  bool DoCheckViewsGeometryColumns(const char *db_prefix);
  bool DoCheckVirtsGeometryColumns(const char *db_prefix);
  bool DoCheckTopologies(const char *db_prefix);
  bool DoCheckNetworks(const char *db_prefix);
  bool DoCheckVectorCoverages(const char *db_prefix);
  bool DoCheckTopoGeoCoverages(const char *db_prefix);
  bool DoCheckTopoNetCoverages(const char *db_prefix);
  bool DoCheckRasterCoverages(const char *db_prefix);
  bool DoCheckWmsCoverages(const char *db_prefix);
  void DoSplitMultilines(wxString & in, wxString & out);
public:
    AddMapLayerDialog()
  {
    MapPanel = NULL;
    First = NULL;
    Last = NULL;
  }
  AddMapLayerDialog(MyMapPanel * parent)
  {
    Create(parent);
  }
  bool Create(MyMapPanel * parent);
  virtual ~ AddMapLayerDialog()
  {
    FlushList();
  }
  void CreateControls();
  static int ParseSRID(const char *crs);

  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class MapLayerInfoDialog:public wxDialog
{
//
// a Dialog for showing Map Layer Metadata
//
private:
  MyMapPanel * MapPanel;
  MapLayer *Layer;
  wxGrid *Extents;
  void DoFetchRefSysName(int srid, wxString & name);
public:
    MapLayerInfoDialog()
  {
    MapPanel = NULL;
    Layer = NULL;
  }
  MapLayerInfoDialog(MyMapPanel * parent, MapLayer * layer)
  {
    Create(parent, layer);
  }
  bool Create(MyMapPanel * parent, MapLayer * layer);
  virtual ~ MapLayerInfoDialog()
  {;
  }
  void CreateControls();
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class MapConfigDialog:public wxPropertySheetDialog
{
//
// a Dialog for setting global Map Options
//
private:
  MyMapPanel * MapPanel;
  wxString MapName;
  wxString BkpMapName;
  wxString Title;
  wxString BkpTitle;
  wxString Abstract;
  wxString BkpAbstract;
  bool MultiThreadingEnabled;
  int MaxThreads;
  bool AutoTransformEnabled;
  int OldMapSRID;
  int MapSRID;
  bool RasterWmsAutoSwitch;
  bool LabelAntiCollision;
  bool LabelWrapText;
  bool LabelAutoRotate;
  bool LabelShiftPosition;
  bool GeographicCoordsDMS;
  bool CheckeredMapBackground;
  wxColour SolidMapBackground;
  bool IsConfigChanged;
  wxPanel *CreateMainPage(wxWindow * book);
  wxPanel *CreateSrsPage(wxWindow * book);
  wxPanel *CreateOptionsPage(wxWindow * book);
  wxGrid *Extents;
  bool RetrieveMainPage();
  bool RetrieveSrsPage(bool check = true);
  bool RetrieveOptionsPage(bool check = true);
  void UpdateMainPage();
  void UpdateSrsPage();
  void UpdateOptionsPage();
  bool UpdateRefSysName();
  void GetButtonBitmap(wxColour & bgcolor, wxBitmap & bmp);
  void DoUpdateMapExtent();
  char *DoCreateMapConfigXML();
  bool FinalValidityCheck();
public:
    MapConfigDialog()
  {
    MapPanel = NULL;
  }
  MapConfigDialog(MyMapPanel * parent)
  {
    Create(parent);
  }
  bool Create(MyMapPanel * parent);
  virtual ~ MapConfigDialog()
  {;
  }
  static bool CheckExistingMapConfiguration(sqlite3 * sqlite, MyFrame * parent,
                                            const char *name, int *id);
  wxString & GetMapName()
  {
    return MapName;
  }
  wxString & GetTitle()
  {
    return Title;
  }
  wxString & GetAbstract()
  {
    return Abstract;
  }
  bool IsMultiThreadingEnabled()
  {
    return MultiThreadingEnabled;
  }
  int GetMaxThreads()
  {
    return MaxThreads;
  }
  int GetMapSRID()
  {
    return MapSRID;
  }
  bool IsAutoTransformEnabled()
  {
    return AutoTransformEnabled;
  }
  bool IsRasterWmsAutoSwitch()
  {
    return RasterWmsAutoSwitch;
  }
  bool IsLabelAntiCollision()
  {
    return LabelAntiCollision;
  }
  bool IsLabelWrapText()
  {
    return LabelWrapText;
  }
  bool IsLabelAutoRotate()
  {
    return LabelAutoRotate;
  }
  bool IsLabelShiftPosition()
  {
    return LabelShiftPosition;
  }
  bool IsGeographicCoordsDMS()
  {
    return GeographicCoordsDMS;
  }
  bool IsCheckeredMapBackground()
  {
    return CheckeredMapBackground;
  }
  wxColour & GetSolidMapBackground()
  {
    return SolidMapBackground;
  }
  void CreateButtons();
  void OnPageChanging(wxNotebookEvent & event);
  void OnPageChanged(wxNotebookEvent & event);
  void OnMultiThreadChanged(wxCommandEvent & event);
  void OnAutoTransformChanged(wxCommandEvent & event);
  void OnMapSridChanged(wxCommandEvent & event);
  void OnMapBackgroundChanged(wxCommandEvent & event);
  void OnMapBackgroundColorChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnQuit(wxCommandEvent & event);
  void OnInsert(wxCommandEvent & event);
  void OnExport(wxCommandEvent & event);
  void OnCopy(wxCommandEvent & event);
};

class XmlMapConfigPrinter
{
//
// a class for printing an XML MapConfig
//
private:
  MyMapPanel * MapPanel;
  wxString MapName;
  wxString Title;
  wxString Abstract;
  bool MultiThreadingEnabled;
  int MaxThreads;
  bool AutoTransformEnabled;
  int OldMapSRID;
  int MapSRID;
  wxGrid *Extents;
  bool RasterWmsAutoSwitch;
  bool LabelAntiCollision;
  bool LabelWrapText;
  bool LabelAutoRotate;
  bool LabelShiftPosition;
  bool GeographicCoordsDMS;
  bool CheckeredMapBackground;
  wxColour SolidMapBackground;
public:
    XmlMapConfigPrinter(MyMapPanel * MapPanel);
   ~XmlMapConfigPrinter()
  {;
  }
  void InitializeFromParent();
  void InitializeFromDialog(MapConfigDialog * dlg);
  char *GetXML();
};

class MapImageDialog:public wxDialog
{
//
// a Dialog for creating map images
//
private:
  MyMapPanel * MapPanel;
  int HorzPixels;
  int VertPixels;
  int ImageFormat;
  int CompressionMethod;
  int Quality;
  bool WorldFile;
  bool GeoTiff;
public:
    MapImageDialog()
  {
    MapPanel = NULL;
  }
  MapImageDialog(MyMapPanel * parent)
  {
    Create(parent);
  }
  bool Create(MyMapPanel * parent);
  virtual ~ MapImageDialog()
  {;
  }
  void CreateControls();
  int GetHorzPixels()
  {
    return HorzPixels;
  }
  int GetVertPixels()
  {
    return VertPixels;
  }
  int GetImageFormat()
  {
    return ImageFormat;
  }
  int GetCompressionMethos()
  {
    return CompressionMethod;
  }
  int GetQuality()
  {
    return Quality;
  }
  bool HasWorldFile()
  {
    return WorldFile;
  }
  bool IsGeoTiff()
  {
    return GeoTiff;
  }
  void OnImageFormatChanged(wxCommandEvent & event);
  void OnCompressionChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class PrinterDialog:public wxDialog
{
//
// a Dialog for Printing PDF maps
//
private:
  MyMapPanel * MapPanel;
  int HorzPixels;
  int VertPixels;
  int MarginWidth;
  int PaperFormat;
  int Dpi;
  bool Portrait;
  void DoUpdateSize();
public:
    PrinterDialog()
  {
    MapPanel = NULL;
  }
  PrinterDialog(MyMapPanel * parent)
  {
    Create(parent);
  }
  bool Create(MyMapPanel * parent);
  virtual ~ PrinterDialog()
  {;
  }
  void CreateControls();
  int GetHorzPixels()
  {
    return HorzPixels;
  }
  int GetVertPixels()
  {
    return VertPixels;
  }
  int GetMarginWidth()
  {
    return MarginWidth;
  }
  void OnPaperFormatChanged(wxCommandEvent & event);
  void OnDpiChanged(wxCommandEvent & event);
  void OnPortraitChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class AddAllRastersSridDialog:public wxDialog
{
//
// a Dialog for adding multiple alternative SRIDs
//
private:
  MyFrame * MainFrame;
  bool FetchRefSysName(int srid, wxString & name);
public:
    AddAllRastersSridDialog()
  {
    MainFrame = NULL;
  }
  AddAllRastersSridDialog(MyFrame * parent)
  {
    Create(parent);
  }
  bool Create(MyFrame * parent);
  virtual ~ AddAllRastersSridDialog()
  {;
  }
  void CreateControls();
  void OnNewSridChanged(wxCommandEvent & event);
  void OnOldSridChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class AddAllVectorsSridDialog:public wxDialog
{
//
// a Dialog for adding multiple alternative SRIDs
//
private:
  MyFrame * MainFrame;
  bool FetchRefSysName(int srid, wxString & name);
public:
    AddAllVectorsSridDialog()
  {
    MainFrame = NULL;
  }
  AddAllVectorsSridDialog(MyFrame * parent)
  {
    Create(parent);
  }
  bool Create(MyFrame * parent);
  virtual ~ AddAllVectorsSridDialog()
  {;
  }
  void CreateControls();
  void OnNewSridChanged(wxCommandEvent & event);
  void OnOldSridChanged(wxCommandEvent & event);
  void OnOk(wxCommandEvent & event);
  void OnCancel(wxCommandEvent & event);
};

class SingleLayerPainter
{
//
// an helper class to paint a MapLayer image
//
private:
  bool Locked;
  bool OnScreen;
  int Retry;
  unsigned int Width;
  unsigned int Height;
  MapLayer *Layer;
  SingleLayerPainter *Next;
public:
    SingleLayerPainter(unsigned int width, unsigned int height,
                       MapLayer * layer);
   ~SingleLayerPainter()
  {;
  }
  int GetRetryCount()
  {
    return Retry;
  }
  void IncrementRetryCount()
  {
    Retry++;
  }
  bool IsOnScreen()
  {
    return OnScreen;
  }
  void SetOnScreen(bool mode)
  {
    OnScreen = mode;
  }
  bool IsLocked()
  {
    return Locked;
  }
  void SetLocked(bool mode)
  {
    Locked = mode;
  }
  unsigned int GetWidth()
  {
    return Width;
  }
  unsigned int GetHeight()
  {
    return Height;
  }
  MapLayer *GetLayer()
  {
    return Layer;
  }
  bool IsReady();
  void SetNext(SingleLayerPainter * next)
  {
    Next = next;
  }
  SingleLayerPainter *GetNext()
  {
    return Next;
  }
};

class MultiLayerPainter
{
//
// an helper class to paint a Map image
//
private:
  MyMapView * Map;
  unsigned int Width;
  unsigned int Height;
  rl2GraphicsContextPtr Graphics;
  bool Changed;
  SingleLayerPainter *First;
  SingleLayerPainter *Last;
public:
    MultiLayerPainter(MyMapView * map, unsigned int width, unsigned int height);
   ~MultiLayerPainter();
  unsigned int GetWidth()
  {
    return Width;
  }
  unsigned int GetHeight()
  {
    return Height;
  }
  bool IsChanged()
  {
    return Changed;
  }
  void SetChanged(bool mode)
  {
    Changed = mode;
  }
  bool IsReady();
  bool IsExhausted();
  void InsertLayer(MapLayer * layer);
  void ResetGraphics();
  rl2GraphicsContextPtr GetGraphicsContext()
  {
    return Graphics;
  }
  unsigned char *GetRGB();
  unsigned char *GetAlpha();
  SingleLayerPainter *GetFirst()
  {
    return First;
  }
};

class MapFeatureGeom
{
//
// a class storing a Geometry (selected Map Feature)
//
private:
  gaiaGeomCollPtr Geometry;
  MapFeatureGeom *Next;
public:
    MapFeatureGeom(gaiaGeomCollPtr geom)
  {
    Geometry = geom;
    Next = NULL;
  }
   ~MapFeatureGeom()
  {
    gaiaFreeGeomColl(Geometry);
  }
  gaiaGeomCollPtr GetGeometry()
  {
    return Geometry;
  }
  MapFeatureGeom *GetNext()
  {
    return Next;
  }
  void SetNext(MapFeatureGeom * next)
  {
    Next = next;
  }
};

class MapFeaturesList
{
//
// a class storing a list of Geometries (selected Map Features)
//
private:
  int Srid;
  double MinX;
  double MinY;
  double MaxX;
  double MaxY;
  MapFeatureGeom *First;
  MapFeatureGeom *Last;
public:
    MapFeaturesList(int srid)
  {
    Srid = srid;
    First = NULL;
    Last = NULL;
  }
   ~MapFeaturesList();
  void Add(gaiaGeomCollPtr geom);
  MapFeatureGeom *GetFirst()
  {
    return First;
  }
  void UpdateBBox();
  int GetSRID()
  {
    return Srid;
  }
  double GetMinX()
  {
    return MinX;
  }
  double GetMinY()
  {
    return MinY;
  }
  double GetMaxX()
  {
    return MaxX;
  }
  double GetMaxY()
  {
    return MaxY;
  }
};
