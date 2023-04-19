/*
/ MapConfig.cpp
/ Map Configuration - helper methods
/
/ version 2.0, 2020 February 8
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2020  Alessandro Furieri
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
#include "wx/html/htmlwin.h"

char *MapConfigDialog::DoCreateMapConfigXML()
{
//
// generating an XML Map Configuration
//
  char *xml;
  XmlMapConfigPrinter *xmlPrinter = new XmlMapConfigPrinter(MapPanel);
  xmlPrinter->InitializeFromDialog(this);
  xml = xmlPrinter->GetXML();
  delete xmlPrinter;
  return xml;
}

XmlMapConfigPrinter::XmlMapConfigPrinter(MyMapPanel * parent)
{
// ctor
  MapPanel = parent;
}

void XmlMapConfigPrinter::InitializeFromParent()
{
//
// initializing from Parent
//
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
}

void XmlMapConfigPrinter::InitializeFromDialog(MapConfigDialog * dlg)
{
//
// initializing from Dialog
//
  MapName = dlg->GetMapName();
  Title = dlg->GetTitle();
  Abstract = dlg->GetAbstract();
  MultiThreadingEnabled = dlg->IsMultiThreadingEnabled();
  MaxThreads = dlg->GetMaxThreads();
  AutoTransformEnabled = dlg->IsAutoTransformEnabled();
  MapSRID = dlg->GetMapSRID();
  OldMapSRID = MapSRID;
  RasterWmsAutoSwitch = dlg->IsRasterWmsAutoSwitch();
  LabelAntiCollision = dlg->IsLabelAntiCollision();
  LabelWrapText = dlg->IsLabelWrapText();
  LabelAutoRotate = dlg->IsLabelAutoRotate();
  LabelShiftPosition = dlg->IsLabelShiftPosition();
  GeographicCoordsDMS = dlg->IsGeographicCoordsDMS();
  CheckeredMapBackground = dlg->IsCheckeredMapBackground();
  SolidMapBackground = dlg->GetSolidMapBackground();
}

char *XmlMapConfigPrinter::GetXML()
{
//
// printing the current XML MapConfiguration
//
  char color[16];
  const char *tf;
  char *str;

  MapXmlConfig xmlcfg;
  MapLayerXmlConfig *cfg;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RL2MapConfig version=\"1.0\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf
    ("%sxmlns=\"http://www.gaia-gis.it/RL2MapConfig\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxsi:schemaLocation=\"http://www.gaia-gis.it/RL2MapConfig http://www.gaia-gis.it/RL2MapConfig_1_0.xsd\" >\r\n",
     prev);
  sqlite3_free(prev);
  prev = xml;
  str = (char *) malloc(MapName.Len() * 4);
  strcpy(str, MapName.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  free(str);
  sqlite3_free(prev);
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      if (Title.Len() > 0)
        {
          str = (char *) malloc(Title.Len() * 4);
          strcpy(str, Title.ToUTF8());
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          free(str);
          sqlite3_free(prev);
        }
      if (Abstract.Len() > 0)
        {
          str = (char *) malloc(Abstract.Len() * 4);
          strcpy(str, Abstract.ToUTF8());
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          free(str);
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
    }
  prev = xml;
  xml = sqlite3_mprintf("%s\t<MapOptions>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MultiThreadingEnabled == false)
    tf = "false";
  else
    tf = "true";
  xml =
    sqlite3_mprintf
    ("%s\t\t<MultiThreading Enabled=\"%s\" MaxThreads=\"%d\" />\r\n", prev, tf,
     MaxThreads);
  sqlite3_free(prev);
  prev = xml;
  if (AutoTransformEnabled == false)
    tf = "false";
  else
    tf = "true";
  xml =
    sqlite3_mprintf
    ("%s\t\t<MapCrs Crs=\"EPSG:%d\" AutoTransformEnabled=\"%s\" />\r\n", prev,
     MapSRID, tf);
  sqlite3_free(prev);
  prev = xml;
  if (GeographicCoordsDMS == false)
    tf = "false";
  else
    tf = "true";
  xml = sqlite3_mprintf("%s\t\t<GeographicCoords DMS=\"%s\" />\r\n", prev, tf);
  sqlite3_free(prev);
  prev = xml;
  if (CheckeredMapBackground == false)
    tf = "false";
  else
    tf = "true";
  wxColour clr = SolidMapBackground;
  if (clr.IsOk() == true)
    sprintf(color, "#%02x%02x%02x", clr.Red(), clr.Green(), clr.Blue());
  else
    strcpy(color, "#ffffff");
  xml =
    sqlite3_mprintf
    ("%s\t\t<MapBackground Color=\"%s\" Transparent=\"%s\" />\r\n", prev, color,
     tf);
  sqlite3_free(prev);
  prev = xml;
  if (RasterWmsAutoSwitch == false)
    tf = "false";
  else
    tf = "true";
  xml =
    sqlite3_mprintf
    ("%s\t\t<RasterWmsAutoSwitch Enabled=\"%s\" />\r\n", prev, tf);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t<LabelAdvancedOptions ", prev);
  sqlite3_free(prev);
  prev = xml;
  if (LabelAntiCollision == false)
    tf = "false";
  else
    tf = "true";
  xml = sqlite3_mprintf("%sAntiCollisionEnabled=\"%s\" ", prev, tf);
  sqlite3_free(prev);
  prev = xml;
  if (LabelWrapText == false)
    tf = "false";
  else
    tf = "true";
  xml = sqlite3_mprintf("%sWrapTextEnabled=\"%s\" ", prev, tf);
  sqlite3_free(prev);
  prev = xml;
  if (LabelAutoRotate == false)
    tf = "false";
  else
    tf = "true";
  xml = sqlite3_mprintf("%sAutoRotateEnabled=\"%s\" ", prev, tf);
  sqlite3_free(prev);
  prev = xml;
  if (LabelShiftPosition == false)
    tf = "false";
  else
    tf = "true";
  xml = sqlite3_mprintf("%sShiftPositionEnabled=\"%s\" />\r\n", prev, tf);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</MapOptions>\r\n", prev);
  sqlite3_free(prev);

  int srid;
  double minx;
  double miny;
  double maxx;
  double maxy;
  MapPanel->GetBBox(&srid, &minx, &miny, &maxx, &maxy);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t<MapBoundingBox MinX=\"%1.6f\" MinY=\"%1.6f\" MaxX=\"%1.6f\" MaxY=\"%1.6f\" />\r\n",
     prev, minx, miny, maxx, maxy);
  sqlite3_free(prev);

  wxTreeItemId item;
  wxTreeItemIdValue cookie;
  MyLayerTree *tree = MapPanel->GetLayerTree();
  item = tree->GetFirstChild(tree->GetRoot(), cookie);
  while (item.IsOk() == true)
    {
      // looping on layers
      MapLayerObject *obj = (MapLayerObject *) tree->GetItemData(item);
      if (obj != NULL)
        {
          MapLayer *layer = obj->GetLayer();
          VectorLayerConfig *vector;
          RasterLayerConfig *raster;
          WmsLayerSettings *wms;
          QuickStyleWmsObj *wms_obj;
          int len;
          char *db_prefix = NULL;
          char *name = NULL;
          len = layer->GetDbPrefix().Len();
          if (len > 0)
            {
              db_prefix = (char *) malloc(len * 4);
              strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
            }
          len = layer->GetName().Len();
          name = (char *) malloc(len * 4);
          strcpy(name, layer->GetName().ToUTF8());
          cfg =
            xmlcfg.AddLayer(layer->GetType(), db_prefix, name,
                            layer->IsVisible());
          switch (cfg->GetType())
            {
              case MAP_LAYER_WMS:
                wms_obj = layer->GetQuickStyleWms();
                wms = layer->GetWmsConfig();
                if (wms != NULL)
                  {
                    char *getMapUrl = NULL;
                    char *getFeatureInfoUrl = NULL;
                    int len;
                    char *db_prefix = NULL;
                    char *name = NULL;
                    bool min_scale_enabled = false;
                    bool max_scale_enabled = false;
                    double min_scale = 0.0;
                    double max_scale = 0.0;
                    if (wms_obj != NULL)
                      {
                        min_scale_enabled = wms_obj->IsMinScaleEnabled();
                        max_scale_enabled = wms_obj->IsMaxScaleEnabled();
                        min_scale = wms_obj->GetScaleMin();
                        max_scale = wms_obj->GetScaleMax();
                      }
                    len = layer->GetDbPrefix().Len();
                    if (len > 0)
                      {
                        db_prefix = (char *) malloc(len * 4);
                        strcpy(db_prefix, layer->GetDbPrefix().ToUTF8());
                      }
                    len = layer->GetName().Len();
                    name = (char *) malloc(len * 4);
                    strcpy(name, layer->GetName().ToUTF8());
                    tree->DoFetchWmsUrls(db_prefix, name, &getMapUrl,
                                         &getFeatureInfoUrl);
                    if (db_prefix != NULL)
                      free(db_prefix);
                    free(name);
                    cfg->AddWms(min_scale_enabled, max_scale_enabled,
                                min_scale, max_scale, getMapUrl,
                                getFeatureInfoUrl, wms->GetVersion(),
                                wms->GetRefSys(), wms->GetStyle(),
                                wms->GetImageFormat(), wms->GetBgColor(),
                                wms->IsBgColorEnabled(), wms->IsOpaque(),
                                wms->IsSwapXY(), wms->IsTiled(),
                                wms->GetTileWidth(), wms->GetTileHeight());
                    if (getMapUrl != NULL)
                      free(getMapUrl);
                    if (getFeatureInfoUrl != NULL)
                      free(getFeatureInfoUrl);
                  }
                break;
              case MAP_LAYER_VECTOR:
                vector = layer->GetVectorConfig();
                if (vector != NULL)
                  {
                    cfg->AddVector(vector->GetStyle(), layer->GetQuickStyle());
                  }
                break;
              case MAP_LAYER_VECTOR_VIEW:
                vector = layer->GetVectorConfig();
                if (vector != NULL)
                  cfg->AddVector(vector->GetStyle(), layer->GetQuickStyle());
                break;
              case MAP_LAYER_VECTOR_VIRTUAL:
                vector = layer->GetVectorConfig();
                if (vector != NULL)
                  cfg->AddVector(vector->GetStyle(), layer->GetQuickStyle());
                break;
              case MAP_LAYER_TOPOLOGY:
                vector = layer->GetVectorConfig();
                if (vector != NULL)
                  cfg->AddTopology(vector->GetStyle(),
                                   layer->GetQuickStyleTopology(),
                                   vector->AreFacesVisible(),
                                   vector->AreEdgesVisible(),
                                   vector->AreNodesVisible(),
                                   vector->AreFaceSeedsVisible(),
                                   vector->AreEdgeSeedsVisible());
                break;
              case MAP_LAYER_NETWORK:
                vector = layer->GetVectorConfig();
                if (vector != NULL)
                  cfg->AddNetwork(vector->GetStyle(),
                                  layer->GetQuickStyleTopology(),
                                  vector->AreLinksVisible(),
                                  vector->AreNodesVisible(),
                                  vector->AreLinkSeedsVisible());
                break;
              case MAP_LAYER_RASTER:
                raster = layer->GetRasterConfig();
                if (raster != NULL)
                  cfg->AddRaster(raster->GetStyle(),
                                 layer->GetQuickStyleRaster());
                break;
              case MAP_LAYER_UNKNOWN:
              default:
                break;
            };
        }
      item = tree->GetNextChild(tree->GetRoot(), cookie);
    }

// defining the list of required Attached DBs
  cfg = xmlcfg.GetLast();
  while (cfg != NULL)
    {
      // adding any eventual Attached DB
      const char *db_prefix;
      if (cfg->GetDbPrefix() == NULL)
        db_prefix = "main";
      else
        db_prefix = cfg->GetDbPrefix();
      xmlcfg.AddAttached(db_prefix);
      cfg = cfg->GetPrev();
    }
  MapPanel->ResolveAttachedDbPaths(&xmlcfg);
  if (xmlcfg.HasAttached() == true)
    {
      prev = xml;
      xml = sqlite3_mprintf("%s\t<MapAttachedDatabases>\r\n", prev);
      sqlite3_free(prev);
      MapAttachedXmlConfig *db = xmlcfg.GetFirstDB();
      while (db != NULL)
        {
          const char *db_prefix = db->GetDbPrefix();
          const char *path = db->GetPath();
          if (db_prefix == NULL)
            db_prefix = "main";
          if (strcasecmp(db_prefix, "main") == 0)
            {
              // skipping MAIN
              db = db->GetNext();
              continue;
            }
          if (path == NULL)
            path = ":memory:";
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t<MapAttachedDB DbPrefix=\"%s\" Path=\"%s\" />\r\n", prev,
             db_prefix, path);
          sqlite3_free(prev);
          db = db->GetNext();
        }
      prev = xml;
      xml = sqlite3_mprintf("%s\t</MapAttachedDatabases>\r\n", prev);
      sqlite3_free(prev);
    }

  cfg = xmlcfg.GetLast();
  while (cfg != NULL)
    {
      // creating XML defs for each layer accordingly to "painter order"
      MapLayerXmlVector *vector = cfg->GetVector();
      MapLayerXmlTopology *topology = cfg->GetTopology();
      MapLayerXmlNetwork *network = cfg->GetNetwork();
      MapLayerXmlRaster *raster = cfg->GetRaster();
      MapLayerXmlWms *wms = cfg->GetWms();
      bool dontPaintGeometrySymbolizer = false;
      const char *visible = "true";

      if (cfg->IsVisible() != true)
        visible = "false";
      prev = xml;
      if (cfg->GetDbPrefix() != NULL)
        xml =
          sqlite3_mprintf
          ("%s\t<MapLayer Type=\"%s\" DbPrefix=\"%s\" Name=\"%s\" Visible=\"%s\">\r\n",
           prev, cfg->GetTypeName(), cfg->GetDbPrefix(), cfg->GetName(),
           visible);
      else
        xml =
          sqlite3_mprintf
          ("%s\t<MapLayer Type=\"%s\" Name=\"%s\" Visible=\"%s\">\r\n", prev,
           cfg->GetTypeName(), cfg->GetName(), visible);
      sqlite3_free(prev);
      if (vector != NULL)
        {
          QuickStyleObj *quick = vector->GetQuickStyle();
          if (quick != NULL)
            {
              // Vector Layer using a Quick Style
              bool complexStyle = false;
              prev = xml;
              dontPaintGeometrySymbolizer =
                vector->IsDontPaintGeometrySymbolizer();
              if (dontPaintGeometrySymbolizer == true)
                xml =
                  sqlite3_mprintf
                  ("%s\t\t<VectorLayerStyle TextSymbolizerAlone=\"true\">\r\n",
                   prev);
              else
                xml = sqlite3_mprintf("%s\t\t<VectorLayerStyle>\r\n", prev);
              sqlite3_free(prev);
              if (quick->IsMinScaleEnabled() == true
                  || quick->IsMaxScaleEnabled() == true)
                complexStyle = true;
              if (quick->IsLabelPrint() == true)
                complexStyle = true;
              if (quick->GetType() == QUICK_STYLE_GEOMETRY)
                complexStyle = true;
              if (quick->IsLine2Enabled() == true)
                complexStyle = true;
              if (complexStyle == true)
                xml = tree->PrintQuickStyleComplex(xml, quick);
              else
                {
                  if (quick->GetType() == QUICK_STYLE_POINT)
                    xml = tree->PrintQuickStylePoint(xml, "", quick);
                  if (quick->GetType() == QUICK_STYLE_LINE)
                    xml = tree->PrintQuickStyleLine(xml, "", quick);
                  if (quick->GetType() == QUICK_STYLE_POLYGON)
                    xml = tree->PrintQuickStylePolygon(xml, "", quick);
                }
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t</VectorLayerStyle>\r\n", prev);
              sqlite3_free(prev);
          } else
            {
              // Vector Layer using an internal style
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t<VectorLayerInternalStyle name=\"%s\" />\r\n",
                 prev, vector->GetStyle());
              sqlite3_free(prev);
            }
        }
      if (topology != NULL)
        {
          QuickStyleTopologyObj *quick = topology->GetQuickStyle();
          if (quick != NULL)
            {
              // Topology Layer using a Quick Style
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t<TopologyLayerStyle", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowFaces() == true)
                xml = sqlite3_mprintf("%s ShowFaces=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowFaces=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowEdges() == true)
                xml = sqlite3_mprintf("%s ShowEdges=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowEdges=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowNodes() == true)
                xml = sqlite3_mprintf("%s ShowNodes=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowNodes=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowFaceSeeds() == true)
                xml = sqlite3_mprintf("%s ShowFaceSeeds=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowFaceSeeds=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowEdgeSeeds() == true)
                xml = sqlite3_mprintf("%s ShowEdgeSeeds=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowEdgeSeeds=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              xml = sqlite3_mprintf("%s>\r\n", prev);
              sqlite3_free(prev);
              prev = xml;
              if (quick != NULL)
                {
                  const char *indent = "\t\t\t";
                  char *xml2 = tree->PrintQuickStyleTopology(quick, indent);
                  if (xml2 != NULL)
                    {
                      prev = xml;
                      xml = sqlite3_mprintf("%s%s", prev, xml2);
                      sqlite3_free(prev);
                      sqlite3_free(xml2);
                    }
                }
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t</TopologyLayerStyle>\r\n", prev);
              sqlite3_free(prev);
          } else
            {
              // Topology Layer using an internal style
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t<TopologyLayerInternalStyle name=\"%s\"",
                 prev, topology->GetStyle());
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowFaces() == true)
                xml = sqlite3_mprintf("%s ShowFaces=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowFaces=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowEdges() == true)
                xml = sqlite3_mprintf("%s ShowEdges=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowEdges=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowNodes() == true)
                xml = sqlite3_mprintf("%s ShowNodes=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowNodes=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowFaceSeeds() == true)
                xml = sqlite3_mprintf("%s ShowFaceSeeds=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowFaceSeeds=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (topology->GetShowEdgeSeeds() == true)
                xml = sqlite3_mprintf("%s ShowEdgeSeeds=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowEdgeSeeds=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              xml = sqlite3_mprintf("%s />\r\n", prev);
              sqlite3_free(prev);
            }
        }
      if (network != NULL)
        {
          QuickStyleTopologyObj *quick = network->GetQuickStyle();
          if (quick != NULL)
            {
              // Network Layer using a Quick Style
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t<NetworkLayerStyle", prev);
              sqlite3_free(prev);
              prev = xml;
              if (network->GetShowLinks() == true)
                xml = sqlite3_mprintf("%s ShowLinks=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowLinks=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (network->GetShowNodes() == true)
                xml = sqlite3_mprintf("%s ShowNodes=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowNodes=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (network->GetShowLinkSeeds() == true)
                xml = sqlite3_mprintf("%s ShowLinkSeeds=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowLinkSeeds=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              xml = sqlite3_mprintf("%s>\r\n", prev);
              sqlite3_free(prev);
              prev = xml;
              if (quick != NULL)
                {
                  const char *indent = "\t\t\t";
                  char *xml2 = tree->PrintQuickStyleNetwork(quick, indent);
                  if (xml2 != NULL)
                    {
                      prev = xml;
                      xml = sqlite3_mprintf("%s%s", prev, xml2);
                      sqlite3_free(prev);
                      sqlite3_free(xml2);
                    }
                }
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t</NetworkLayerStyle>\r\n", prev);
              sqlite3_free(prev);
          } else
            {
              // Network Layer using an internal style
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t<NetworkLayerInternalStyle name=\"%s\"",
                 prev, network->GetStyle());
              sqlite3_free(prev);
              prev = xml;
              if (network->GetShowLinks() == true)
                xml = sqlite3_mprintf("%s ShowLinks=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowLinks=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (network->GetShowNodes() == true)
                xml = sqlite3_mprintf("%s ShowNodes=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowNodes=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              if (network->GetShowLinkSeeds() == true)
                xml = sqlite3_mprintf("%s ShowLinkSeeds=\"true\"", prev);
              else
                xml = sqlite3_mprintf("%s ShowLinkSeeds=\"false\"", prev);
              sqlite3_free(prev);
              prev = xml;
              xml = sqlite3_mprintf("%s />\r\n", prev);
              sqlite3_free(prev);
            }
        }
      if (raster != NULL)
        {
          QuickStyleRasterObj *quick = raster->GetQuickStyle();
          if (quick != NULL)
            {
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t<RasterLayerStyle>\r\n", prev, raster->GetStyle());
              sqlite3_free(prev);
              if (quick != NULL)
                {
                  char *xml2 = tree->PrintQuickStyleRaster(quick);
                  if (xml2 != NULL)
                    {
                      prev = xml;
                      xml = sqlite3_mprintf("%s%s", prev, xml2);
                      sqlite3_free(prev);
                      sqlite3_free(xml2);
                    }
                }
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t</RasterLayerStyle>\r\n", prev);
              sqlite3_free(prev);
          } else
            {
              // Raster Layer using an internal style
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t<RasterLayerInternalStyle name=\"%s\" />\r\n",
                 prev, raster->GetStyle());
              sqlite3_free(prev);
            }
        }
      if (wms != NULL)
        {
          char *clean;
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t<WmsLayerStyle>\r\n", prev);
          sqlite3_free(prev);
          if (wms->IsMinScaleEnabled() == true
              || wms->IsMaxScaleEnabled() == true)
            {
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t\t<Rule>\r\n", prev);
              sqlite3_free(prev);
              if (wms->IsMinScaleEnabled() == true)
                {
                  prev = xml;
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n",
                     prev, wms->GetScaleMin());
                  sqlite3_free(prev);
                }
              if (wms->IsMaxScaleEnabled() == true)
                {
                  prev = xml;
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n",
                     prev, wms->GetScaleMax());
                  sqlite3_free(prev);
                }
              prev = xml;
              xml = sqlite3_mprintf("%s\t\t\t</Rule>\r\n", prev);
              sqlite3_free(prev);
            }
          if (wms->GetWmsGetMap_URL() != NULL)
            {
              prev = xml;
              clean = tree->XmlClean(wms->GetWmsGetMap_URL());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t<GetMap URL=\"%s\" />\r\n", prev, clean);
              sqlite3_free(clean);
              sqlite3_free(prev);
            }
          if (wms->GetWmsGetFeatureInfo_URL() != NULL)
            {
              prev = xml;
              clean = tree->XmlClean(wms->GetWmsGetFeatureInfo_URL());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t<GetFeatureInfo URL=\"%s\" />\r\n", prev, clean);
              sqlite3_free(clean);
              sqlite3_free(prev);
            }
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<WmsProtocol Version=\"%s\" />\r\n", prev,
             wms->GetVersion());
          sqlite3_free(prev);
          prev = xml;
          if (wms->IsSwapXY() == true)
            xml =
              sqlite3_mprintf
              ("%s\t\t\t<ReferenceSystem Crs=\"%s\" SwapXY=\"true\" />\r\n",
               prev, wms->GetRefSys());
          else
            xml =
              sqlite3_mprintf("%s\t\t\t<ReferenceSystem Crs=\"%s\" />\r\n",
                              prev, wms->GetRefSys());
          sqlite3_free(prev);
          if (strlen(wms->GetStyle()) > 0)
            {
              prev = xml;
              xml =
                sqlite3_mprintf("%s\t\t\t<Style>%s</Style>\r\n", prev,
                                wms->GetStyle());
              sqlite3_free(prev);
            }
          prev = xml;
          if (wms->IsOpaque() == true)
            xml =
              sqlite3_mprintf
              ("%s\t\t\t<ImageFormat MimeType=\"%s\" Opaque=\"true\" />\r\n",
               prev, wms->GetImageFormat());
          else
            xml =
              sqlite3_mprintf
              ("%s\t\t\t<ImageFormat MimeType=\"%s\" Opaque=\"false\" />\r\n",
               prev, wms->GetImageFormat());
          sqlite3_free(prev);
          if (wms->IsBgColorEnabled() == true && wms->GetBgColor() != NULL)
            {
              prev = xml;
              xml =
                sqlite3_mprintf("%s\t\t\t<BgColor>%s</BgColor>\r\n", prev,
                                wms->GetBgColor());
              sqlite3_free(prev);
            }
          if (wms->IsTiled() == true)
            {
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t<TileSize Width=\"%d\" Height=\"%d\" />\r\n", prev,
                 wms->GetTileWidth(), wms->GetTileHeight());
              sqlite3_free(prev);
            }
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t</WmsLayerStyle>\r\n", prev);
          sqlite3_free(prev);
        }
      prev = xml;
      xml = sqlite3_mprintf("%s\t</MapLayer>\r\n", prev);
      sqlite3_free(prev);
      cfg = cfg->GetPrev();
    }

  prev = xml;
  xml = sqlite3_mprintf("%s</RL2MapConfig>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void MyMapPanel::ResolveAttachedDbPaths(MapXmlConfig * cfg)
{
//
// attempting to resolve the Paths of any required Attached DB
//
  int ret;
  const char *sql;
  int i;
  char **results;
  int rows;
  int columns;

  sql = "PRAGMA database_list";
  ret =
    sqlite3_get_table(Parent->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *db_prefix = results[(i * columns) + 1];
          const char *path = results[(i * columns) + 2];
          cfg->SetDbPath(db_prefix, path);
        }
    }
  sqlite3_free_table(results);
}

MapLayerXmlVector::MapLayerXmlVector(const char *style, QuickStyleObj * quick)
{
// ctor
  int len;
  if (style == NULL)
    Style = NULL;
  else
    {
      len = strlen(style);
      Style = (char *) malloc(len + 1);
      strcpy(Style, style);
    }
  QuickStyle = quick;
}

MapLayerXmlVector::~MapLayerXmlVector()
{
// dtor
  if (Style != NULL)
    free(Style);
}

bool MapLayerXmlVector::IsDontPaintGeometrySymbolizer()
{
// testing for a naked Text Symbolizer
  QuickStyleObj *quick = GetQuickStyle();
  if (quick == NULL)
    return false;
  if (quick->IsLabelPrint() != true)
    return false;
  if (quick->GetType() == QUICK_STYLE_POINT
      || quick->GetType() == QUICK_STYLE_LINE
      || quick->GetType() == QUICK_STYLE_POLYGON)
    return quick->IsDontPaintGeomSymbolizer();
  return false;
}

MapLayerXmlTopology::MapLayerXmlTopology(const char *style,
                                         QuickStyleTopologyObj * quick,
                                         bool faces, bool edges, bool nodes,
                                         bool face_seeds, bool edge_seeds)
{
// ctor
  int len;
  if (style == NULL)
    Style = NULL;
  else
    {
      len = strlen(style);
      Style = (char *) malloc(len + 1);
      strcpy(Style, style);
    }
  QuickStyle = quick;
  ShowFaces = faces;
  ShowEdges = edges;
  ShowNodes = nodes;
  ShowFaceSeeds = face_seeds;
  ShowEdgeSeeds = edge_seeds;
}

MapLayerXmlTopology::~MapLayerXmlTopology()
{
// dtor
  if (Style != NULL)
    free(Style);
}

MapLayerXmlNetwork::MapLayerXmlNetwork(const char *style,
                                       QuickStyleTopologyObj * quick,
                                       bool links, bool nodes, bool link_seeds)
{
// ctor
  int len;
  if (style == NULL)
    Style = NULL;
  else
    {
      len = strlen(style);
      Style = (char *) malloc(len + 1);
      strcpy(Style, style);
    }
  QuickStyle = quick;
  ShowLinks = links;
  ShowNodes = nodes;
  ShowLinkSeeds = link_seeds;
}

MapLayerXmlNetwork::~MapLayerXmlNetwork()
{
// dtor
  if (Style != NULL)
    free(Style);
}

MapLayerXmlRaster::MapLayerXmlRaster(const char *style,
                                     QuickStyleRasterObj * quick)
{
// ctor
  int len;
  if (style == NULL)
    Style = NULL;
  else
    {
      len = strlen(style);
      Style = (char *) malloc(len + 1);
      strcpy(Style, style);
    }
  QuickStyle = quick;
}

MapLayerXmlRaster::~MapLayerXmlRaster()
{
// dtor
  if (Style != NULL)
    free(Style);
}

MapLayerXmlWms::MapLayerXmlWms(bool min_scale_enabled, bool max_scale_enabled,
                               double min_scale, double max_scale,
                               const char *getMapUrl,
                               const char *getFeatureInfoUrl,
                               const char *version, const char *ref_sys,
                               const char *style, const char *image_format,
                               const char *bg_color, bool bg_color_enabled,
                               bool opaque, bool swap_xy, bool tiles,
                               int tile_w, int tile_h)
{
// ctor
  int len;
  MinScaleEnabled = min_scale_enabled;
  MaxScaleEnabled = max_scale_enabled;
  ScaleMin = min_scale;
  ScaleMax = max_scale;
  if (getMapUrl == NULL)
    getMapURL = NULL;
  else
    {
      len = strlen(getMapUrl);
      getMapURL = (char *) malloc(len + 1);
      strcpy(getMapURL, getMapUrl);
    }
  if (getFeatureInfoUrl == NULL)
    getFeatureInfoURL = NULL;
  else
    {
      len = strlen(getFeatureInfoUrl);
      getFeatureInfoURL = (char *) malloc(len + 1);
      strcpy(getFeatureInfoURL, getFeatureInfoUrl);
    }
  len = strlen(version);
  Version = (char *) malloc(len + 1);
  strcpy(Version, version);
  len = strlen(ref_sys);
  RefSys = (char *) malloc(len + 1);
  strcpy(RefSys, ref_sys);
  len = strlen(style);
  Style = (char *) malloc(len + 1);
  strcpy(Style, style);
  len = strlen(image_format);
  ImageFormat = (char *) malloc(len + 1);
  strcpy(ImageFormat, image_format);
  if (bg_color == NULL)
    BgColor = NULL;
  else
    {
      len = strlen(bg_color);
      BgColor = (char *) malloc(len + 1);
      strcpy(BgColor, bg_color);
    }
  BgColorEnabled = bg_color_enabled;
  Opaque = opaque;
  SwapXY = swap_xy;
  Tiled = tiles;
  TileWidth = tile_w;
  TileHeight = tile_h;
}

MapLayerXmlWms::~MapLayerXmlWms()
{
// dtor
  if (getMapURL != NULL)
    free(getMapURL);
  if (getFeatureInfoURL != NULL)
    free(getFeatureInfoURL);
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

MapLayerXmlConfig::MapLayerXmlConfig(int type, char *db_prefix, char *name,
                                     bool visible)
{
// ctor
  Type = type;
  switch (Type)
    {
      case MAP_LAYER_WMS:
        TypeName = sqlite3_mprintf("wms");
        break;
      case MAP_LAYER_VECTOR:
        TypeName = sqlite3_mprintf("vector");
        break;
      case MAP_LAYER_VECTOR_VIEW:
        TypeName = sqlite3_mprintf("vector_view");
        break;
      case MAP_LAYER_VECTOR_VIRTUAL:
        TypeName = sqlite3_mprintf("vector_virtual");
        break;
      case MAP_LAYER_TOPOLOGY:
        TypeName = sqlite3_mprintf("topology");
        break;
      case MAP_LAYER_NETWORK:
        TypeName = sqlite3_mprintf("network");
        break;
      case MAP_LAYER_RASTER:
        TypeName = sqlite3_mprintf("raster");
        break;
      case MAP_LAYER_UNKNOWN:
      default:
        TypeName = sqlite3_mprintf("unknown");
        break;
    };
  DbPrefix = db_prefix;
  Name = name;
  Visible = visible;
  Vector = NULL;
  Topology = NULL;
  Network = NULL;
  Raster = NULL;
  Wms = NULL;
  Prev = NULL;
  Next = NULL;
}

MapLayerXmlConfig::~MapLayerXmlConfig()
{
// dtor
  if (TypeName != NULL)
    sqlite3_free(TypeName);
  if (DbPrefix != NULL)
    free(DbPrefix);
  if (Name != NULL)
    free(Name);
  if (Vector != NULL)
    delete Vector;
  if (Topology != NULL)
    delete Topology;
  if (Network != NULL)
    delete Network;
  if (Raster != NULL)
    delete Raster;
  if (Wms != NULL)
    delete Wms;
}

void MapLayerXmlConfig::AddVector(const char *style, QuickStyleObj * quick)
{
//
// adding a Vector specific configuration
//
  if (Vector != NULL)
    delete Vector;
  Vector = new MapLayerXmlVector(style, quick);
}

void MapLayerXmlConfig::AddTopology(const char *style,
                                    QuickStyleTopologyObj * quick, bool faces,
                                    bool edges, bool nodes, bool face_seeds,
                                    bool edge_seeds)
{
//
// adding a Topology specific configuration
//
  if (Topology != NULL)
    delete Topology;
  Topology =
    new MapLayerXmlTopology(style, quick, faces, edges, nodes, face_seeds,
                            edge_seeds);
}

void MapLayerXmlConfig::AddNetwork(const char *style,
                                   QuickStyleTopologyObj * quick, bool links,
                                   bool nodes, bool link_seeds)
{
//
// adding a Topology/Network specific configuration
//
  if (Network != NULL)
    delete Network;
  Network = new MapLayerXmlNetwork(style, quick, links, nodes, link_seeds);
}

void MapLayerXmlConfig::AddRaster(const char *style,
                                  QuickStyleRasterObj * quick)
{
//
// adding a Raster specific configuration
//
  if (Raster != NULL)
    delete Raster;
  Raster = new MapLayerXmlRaster(style, quick);
}

void MapLayerXmlConfig::AddWms(bool min_scale_enabled, bool max_scale_enabled,
                               double min_scale, double max_scale,
                               const char *getMapUrl,
                               const char *getFeatureInfoUrl,
                               const char *version, const char *ref_sys,
                               const char *style, const char *image_format,
                               const char *bg_color, bool bg_color_enabled,
                               bool opaque, bool swap_xy, bool tiles,
                               int tile_w, int tile_h)
{
//
// adding a WMS specific configuration
//
  if (Wms != NULL)
    delete Wms;
  Wms =
    new MapLayerXmlWms(min_scale_enabled, max_scale_enabled, min_scale,
                       max_scale, getMapUrl, getFeatureInfoUrl, version,
                       ref_sys, style, image_format, bg_color, bg_color_enabled,
                       opaque, swap_xy, tiles, tile_w, tile_h);
}

MapXmlConfig::~MapXmlConfig()
{
// dtor
  MapLayerXmlConfig *cfg;
  MapLayerXmlConfig *n_cfg;
  MapAttachedXmlConfig *db;
  MapAttachedXmlConfig *n_db;
  cfg = First;
  while (cfg != NULL)
    {
      n_cfg = cfg->GetNext();
      delete cfg;
      cfg = n_cfg;
    }
  db = FirstDB;
  while (db != NULL)
    {
      n_db = db->GetNext();
      delete db;
      db = n_db;
    }
}

MapLayerXmlConfig *MapXmlConfig::AddLayer(int type, char *db_prefix, char *name,
                                          bool visible)
{
// adding a new Layer to the Map Configuration
  MapLayerXmlConfig *cfg;
  cfg = new MapLayerXmlConfig(type, db_prefix, name, visible);

  if (First == NULL)
    First = cfg;
  if (Last != NULL)
    Last->SetNext(cfg);
  cfg->SetPrev(Last);
  Last = cfg;
  return cfg;
}

MapAttachedXmlConfig *MapXmlConfig::AddAttached(const char *db_prefix)
{
// adding a new Attached DB to the Map Configuration
  MapAttachedXmlConfig *db;
  bool attached = true;

  db = FirstDB;
  while (db != NULL)
    {
      // testing if already defined
      if (strcasecmp(db_prefix, db->GetDbPrefix()) == 0)
        return db;
      db = db->GetNext();
    }

// not yet defined: adding into the list
  db = new MapAttachedXmlConfig(db_prefix);
  if (FirstDB == NULL)
    FirstDB = db;
  if (LastDB != NULL)
    LastDB->SetNext(db);
  LastDB = db;
  if (db_prefix == NULL)
    attached = false;
  if (strcasecmp(db_prefix, "main") == 0)
    attached = false;
  if (strcasecmp(db_prefix, "temp") == 0)
    attached = false;
  if (attached == true)
    Attached = true;
  return db;
}

void MapXmlConfig::SetDbPath(const char *db_prefix, const char *path)
{
//
// assigning Paths to Attached DBs
//
  MapAttachedXmlConfig *db;

  if (db_prefix == NULL)
    db_prefix = "main";

  db = FirstDB;
  while (db != NULL)
    {
      if (strcasecmp(db_prefix, db->GetDbPrefix()) == 0)
        db->SetPath(path);
      db = db->GetNext();
    }
}

MapAttachedXmlConfig::MapAttachedXmlConfig(const char *db_prefix)
{
// ctor
  int len = strlen(db_prefix);
  DbPrefix = (char *) malloc(len + 1);
  strcpy(DbPrefix, db_prefix);
  Path = NULL;
  Next = NULL;
}

MapAttachedXmlConfig::~MapAttachedXmlConfig()
{
// dtor
  if (DbPrefix != NULL)
    free(DbPrefix);
  if (Path != NULL)
    free(Path);
}

void MapAttachedXmlConfig::SetPath(const char *path)
{
// setting Path
  if (Path != NULL)
    free(Path);
  if (path == NULL)
    Path = NULL;
  else
    {
      int len = strlen(path);
      Path = (char *) malloc(len + 1);
      strcpy(Path, path);
    }
}

MapLayer *MyMapPanel::DoFetchLayerVector(const char *prefix, const char *name)
{
//
// attempting to fetch a Vector Coverage (based on a Spatial Table)
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT %Q AS db_prefix, v.coverage_name, v.title, v.abstract, "
     "v.copyright, l.name, v.is_queryable, v.is_editable, v.f_table_name, "
     "v.f_geometry_column, g.srid, g.geometry_type, g.spatial_index_enabled, "
     "v.geo_minx, v.geo_miny, v.geo_maxx, v.geo_maxy, v.extent_minx, "
     "v.extent_miny, v.extent_maxx, v.extent_maxy "
     "FROM \"%s\".vector_coverages AS v "
     "JOIN \"%s\".geometry_columns AS g ON (v.f_table_name = g.f_table_name AND "
     "v.f_geometry_column = g.f_geometry_column) "
     "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
     "WHERE Upper(v.coverage_name) = Upper(%Q) AND "
     "v.topology_name IS NULL AND v.network_name IS NULL AND "
     "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
     "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
     "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
     "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", prefix, qprefix,
     qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
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
          const char *f_table_name = results[(i * columns) + 8];
          const char *f_geometry_column = results[(i * columns) + 9];
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          value = results[(i * columns) + 11];
          int geom_type = atoi(value);
          bool spatial_index = false;
          value = results[(i * columns) + 12];
          if (atoi(value) == 1)
            spatial_index = true;
          wxString lyr_prefix = wxT("table");
          double geo_minx = atof(results[(i * columns) + 13]);
          double geo_miny = atof(results[(i * columns) + 14]);
          double geo_maxx = atof(results[(i * columns) + 15]);
          double geo_maxy = atof(results[(i * columns) + 16]);
          double extent_minx = atof(results[(i * columns) + 17]);
          double extent_miny = atof(results[(i * columns) + 18]);
          double extent_maxx = atof(results[(i * columns) + 19]);
          double extent_maxy = atof(results[(i * columns) + 20]);
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              lyr_prefix, str_name, title,
                              abstract, copyright,
                              data_license, f_table_name,
                              f_geometry_column, geom_type, srid,
                              queryable, editable, spatial_index);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

MapLayer *MyMapPanel::DoFetchLayerVectorView(const char *prefix,
                                             const char *name)
{
//
// attempting to fetch a Vector Coverage (based on a Spatial View)
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT %Q AS db_prefix, v.coverage_name, v.title, v.abstract, "
     "v.copyright, l.name, v.is_queryable, v.is_editable, v.view_name, "
     "v.view_geometry, g.srid, g.geometry_type, g.spatial_index_enabled, "
     "v.geo_minx, v.geo_miny, v.geo_maxx, v.geo_maxy, v.extent_minx, "
     "v.extent_miny, v.extent_maxx, v.extent_maxy, "
     "x.f_table_name, x.f_geometry_column, x.view_rowid "
     "FROM \"%s\".vector_coverages AS v "
     "JOIN \"%s\".views_geometry_columns AS x ON (v.view_name = x.view_name "
     "AND v.view_geometry = x.view_geometry) "
     "JOIN \"%s\".geometry_columns AS g ON (x.f_table_name = g.f_table_name "
     "AND x.f_geometry_column = g.f_geometry_column) "
     "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
     "WHERE Upper(v.coverage_name) = Upper(%Q) AND "
     "v.view_name IS NOT NULL AND v.view_geometry IS NOT NULL AND "
     "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
     "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
     "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
     "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", prefix, qprefix,
     qprefix, qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
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
          const char *f_table_name = results[(i * columns) + 8];
          const char *f_geometry_column = results[(i * columns) + 9];
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          value = results[(i * columns) + 11];
          int geom_type = atoi(value);
          bool spatial_index = false;
          value = results[(i * columns) + 12];
          if (atoi(value) == 1)
            spatial_index = true;
          wxString lyr_prefix = wxT("view");
          double geo_minx = atof(results[(i * columns) + 13]);
          double geo_miny = atof(results[(i * columns) + 14]);
          double geo_maxx = atof(results[(i * columns) + 15]);
          double geo_maxy = atof(results[(i * columns) + 16]);
          double extent_minx = atof(results[(i * columns) + 17]);
          double extent_miny = atof(results[(i * columns) + 18]);
          double extent_maxx = atof(results[(i * columns) + 19]);
          double extent_maxy = atof(results[(i * columns) + 20]);
          const char *view_table_name = results[(i * columns) + 21];
          const char *view_geometry_column = results[(i * columns) + 22];
          const char *view_rowid_column = results[(i * columns) + 23];
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              lyr_prefix, str_name, title,
                              abstract, copyright,
                              data_license, f_table_name,
                              f_geometry_column, geom_type, srid,
                              queryable, editable, spatial_index,
                              view_table_name, view_geometry_column,
                              view_rowid_column);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

MapLayer *MyMapPanel::DoFetchLayerVectorVirtual(const char *prefix,
                                                const char *name)
{
//
// attempting to fetch a Vector Coverage (based on a Virtual Shapefile)
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT %Q AS db_prefix, v.coverage_name, v.title, v.abstract, "
     "v.copyright, l.name, v.is_queryable, 0, s.virt_name, s.virt_geometry, s.srid, "
     "s.geometry_type, 0, v.geo_minx, v.geo_miny, v.geo_maxx, v.geo_maxy, v.extent_minx, "
     "v.extent_miny, v.extent_maxx, v.extent_maxy "
     "FROM \"%s\".vector_coverages AS v "
     "JOIN \"%s\".virts_geometry_columns AS s ON (v.virt_name = s.virt_name "
     "AND v.virt_geometry = s.virt_geometry) "
     "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
     "WHERE Upper(v.coverage_name) = Upper(%Q) AND "
     "v.virt_name IS NOT NULL AND v.virt_geometry IS NOT NULL AND "
     "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
     "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
     "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
     "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", prefix, qprefix,
     qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
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
          const char *f_table_name = results[(i * columns) + 8];
          const char *f_geometry_column = results[(i * columns) + 9];
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          value = results[(i * columns) + 11];
          int geom_type = atoi(value);
          bool spatial_index = false;
          value = results[(i * columns) + 12];
          if (atoi(value) == 1)
            spatial_index = true;
          wxString lyr_prefix = wxT("shp");
          double geo_minx = atof(results[(i * columns) + 13]);
          double geo_miny = atof(results[(i * columns) + 14]);
          double geo_maxx = atof(results[(i * columns) + 15]);
          double geo_maxy = atof(results[(i * columns) + 16]);
          double extent_minx = atof(results[(i * columns) + 17]);
          double extent_miny = atof(results[(i * columns) + 18]);
          double extent_maxx = atof(results[(i * columns) + 19]);
          double extent_maxy = atof(results[(i * columns) + 20]);
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              lyr_prefix, str_name, title,
                              abstract, copyright,
                              data_license, f_table_name,
                              f_geometry_column, geom_type, srid,
                              queryable, editable, spatial_index);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

MapLayer *MyMapPanel::DoFetchLayerTopology(const char *prefix, const char *name)
{
//
// attempting to fetch a Vector Coverage (based on Topology)
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT %Q AS db_prefix, v.coverage_name, v.title, v.abstract, "
     "v.copyright, l.name, v.is_queryable, v.is_editable, v.topology_name, t.has_z, t.srid, "
     "v.geo_minx, v.geo_miny, v.geo_maxx, v.geo_maxy, v.extent_minx, "
     "v.extent_miny, v.extent_maxx, v.extent_maxy "
     "FROM \"%s\".vector_coverages AS v "
     "JOIN \"%s\".topologies AS t ON (v.topology_name = t.topology_name) "
     "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
     "WHERE Upper(v.coverage_name) = Upper(%Q) AND "
     "v.topology_name IS NOT NULL AND "
     "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
     "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
     "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
     "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", prefix, qprefix,
     qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
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
          const char *topo_name = results[(i * columns) + 8];
          value = results[(i * columns) + 9];
          int has_z = atoi(value);
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          double geo_minx = atof(results[(i * columns) + 11]);
          double geo_miny = atof(results[(i * columns) + 12]);
          double geo_maxx = atof(results[(i * columns) + 13]);
          double geo_maxy = atof(results[(i * columns) + 14]);
          double extent_minx = atof(results[(i * columns) + 15]);
          double extent_miny = atof(results[(i * columns) + 16]);
          double extent_maxx = atof(results[(i * columns) + 17]);
          double extent_maxy = atof(results[(i * columns) + 18]);
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              MAP_LAYER_TOPOLOGY, str_name, title, abstract,
                              copyright,
                              data_license, topo_name,
                              has_z, srid,
                              queryable, editable);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

MapLayer *MyMapPanel::DoFetchLayerNetwork(const char *prefix, const char *name)
{
//
// attempting to fetch a Vector Coverage (based on Network)
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT %Q AS db_prefix, v.coverage_name, v.title, v.abstract, "
     "v.copyright, l.name, v.is_queryable, v.is_editable, v.network_name, n.has_z, n.srid, "
     "v.geo_minx, v.geo_miny, v.geo_maxx, v.geo_maxy, v.extent_minx, "
     "v.extent_miny, v.extent_maxx, v.extent_maxy "
     "FROM \"%s\".vector_coverages AS v "
     "JOIN \"%s\".networks AS n ON (v.network_name = n.network_name) "
     "JOIN \"%s\".data_licenses AS l ON (v.license = l.id) "
     "WHERE Upper(v.coverage_name) = Upper(%Q) AND "
     "v.network_name IS NOT NULL AND "
     "v.geo_minx IS NOT NULL AND v.geo_miny IS NOT NULL AND "
     "v.geo_maxx IS NOT NULL AND v.geo_maxy IS NOT NULL AND "
     "v.extent_minx IS NOT NULL AND v.extent_miny IS NOT NULL AND "
     "v.extent_maxx IS NOT NULL AND v.extent_maxy IS NOT NULL", prefix, qprefix,
     qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
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
          const char *topo_name = results[(i * columns) + 8];
          value = results[(i * columns) + 9];
          int has_z = atoi(value);
          value = results[(i * columns) + 10];
          int srid = atoi(value);
          double geo_minx = atof(results[(i * columns) + 11]);
          double geo_miny = atof(results[(i * columns) + 12]);
          double geo_maxx = atof(results[(i * columns) + 13]);
          double geo_maxy = atof(results[(i * columns) + 14]);
          double extent_minx = atof(results[(i * columns) + 15]);
          double extent_miny = atof(results[(i * columns) + 16]);
          double extent_maxx = atof(results[(i * columns) + 17]);
          double extent_maxy = atof(results[(i * columns) + 18]);
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              MAP_LAYER_NETWORK, str_name, title, abstract,
                              copyright,
                              data_license, topo_name,
                              has_z, srid,
                              queryable, editable);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

MapLayer *MyMapPanel::DoFetchLayerWMS(const char *prefix, const char *name)
{
//
// attempting to fetch a Raster Coverage (based on WMS)
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql = sqlite3_mprintf("SELECT %Q AS db_prefix, "
                        "w.layer_name, w.title, w.abstract, w.copyright, l.name, "
                        "w.srs, w.is_queryable, g.minx, g.miny, g.maxx, g.maxy, "
                        "d.minx, d.miny, d.maxx, d.maxy "
                        "FROM \"%s\".wms_getmap AS w "
                        "JOIN \"%s\".data_licenses AS l ON (w.license = l.id)"
                        "LEFT JOIN \"%s\".wms_ref_sys AS g ON (w.id = g.parent_id AND g.srs = 'EPSG:4326') "
                        "LEFT JOIN \"%s\".wms_ref_sys AS d ON (w.id = d.parent_id AND d.is_default = 1) "
                        "WHERE Upper(w.layer_name) = Upper(%Q)",
                        prefix, qprefix, qprefix, qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString data_license = wxString::FromUTF8(value);
          value = results[(i * columns) + 6];
          int srid = AddMapLayerDialog::ParseSRID(value);
          bool queryable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            queryable = true;
          double geo_minx = atof(results[(i * columns) + 8]);
          double geo_miny = atof(results[(i * columns) + 9]);
          double geo_maxx = atof(results[(i * columns) + 10]);
          double geo_maxy = atof(results[(i * columns) + 11]);
          double extent_minx = atof(results[(i * columns) + 12]);
          double extent_miny = atof(results[(i * columns) + 13]);
          double extent_maxx = atof(results[(i * columns) + 14]);
          double extent_maxy = atof(results[(i * columns) + 15]);
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              str_name, title, abstract, copyright,
                              data_license, queryable, srid);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

MapLayer *MyMapPanel::DoFetchLayerRaster(const char *prefix, const char *name)
{
//
// attempting to fetch a Raster Coverage 
//
  char *sql;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  MapLayer *layer = NULL;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql = sqlite3_mprintf("SELECT %Q AS db_prefix, "
                        "r.coverage_name, r.title, r.abstract, r.copyright, l.name, "
                        "r.srid, r.is_queryable, r.geo_minx, r.geo_miny, r.geo_maxx, r.geo_maxy, "
                        "r.extent_minx, r.extent_miny, r.extent_maxx, r.extent_maxy, "
                        "r.sample_type, r.pixel_type, r.num_bands, r.compression, r.quality, "
                        "r.tile_width, r.tile_height, r.horz_resolution, r.vert_resolution "
                        "FROM \"%s\".raster_coverages AS r "
                        "JOIN \"%s\".data_licenses AS l ON (r.license = l.id) "
                        "WHERE  Upper(r.coverage_name) = Upper(%Q) AND "
                        "r.geo_minx IS NOT NULL AND r.geo_miny IS NOT NULL AND "
                        "r.geo_maxx IS NOT NULL AND r.geo_maxy IS NOT NULL AND "
                        "r.extent_minx IS NOT NULL AND r.extent_miny IS NOT NULL AND "
                        "r.extent_maxx IS NOT NULL AND r.extent_maxy IS NOT NULL",
                        prefix, qprefix, qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(errMsg);
      return NULL;
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
          wxString str_name = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          wxString title = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          wxString abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 4];
          wxString copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 5];
          wxString data_license = wxString::FromUTF8(value);
          int srid = atoi(results[(i * columns) + 6]);
          bool queryable = false;
          value = results[(i * columns) + 7];
          if (atoi(value))
            queryable = true;
          double geo_minx = atof(results[(i * columns) + 8]);
          double geo_miny = atof(results[(i * columns) + 9]);
          double geo_maxx = atof(results[(i * columns) + 10]);
          double geo_maxy = atof(results[(i * columns) + 11]);
          double extent_minx = atof(results[(i * columns) + 12]);
          double extent_miny = atof(results[(i * columns) + 13]);
          double extent_maxx = atof(results[(i * columns) + 14]);
          double extent_maxy = atof(results[(i * columns) + 15]);
          value = results[(i * columns) + 16];
          wxString sample_type = wxString::FromUTF8(value);
          value = results[(i * columns) + 17];
          wxString pixel_type = wxString::FromUTF8(value);
          int bands = atoi(results[(i * columns) + 18]);
          value = results[(i * columns) + 19];
          wxString compression = wxString::FromUTF8(value);
          int quality = atoi(results[(i * columns) + 20]);
          int tile_width = atoi(results[(i * columns) + 21]);
          int tile_height = atoi(results[(i * columns) + 22]);
          double horz_resolution = atof(results[(i * columns) + 23]);
          double vert_resolution = atof(results[(i * columns) + 24]);
          LayerListItem *item =
            new LayerListItem(GetParent()->GetRL2PrivateData(), db_prefix,
                              str_name, title, abstract, copyright,
                              data_license, srid, queryable);
          if (item != NULL)
            {
              layer = new MapLayer(GetRL2PrivateData(), item);
              if (layer != NULL)
                {
                  layer->GetRasterConfig()->SetConfig(sample_type, pixel_type,
                                                      bands, compression,
                                                      quality, tile_width,
                                                      tile_height,
                                                      horz_resolution,
                                                      vert_resolution);
                  layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
                  layer->SetExtent(extent_minx, extent_miny, extent_maxx,
                                   extent_maxy);
                }
              delete item;
            }
        }
    }
  sqlite3_free_table(results);
  return layer;
}

int MyMapPanel::DoFindBrushId(const char *resource)
{
//
// attempting to retrieve a Graphic Brush
//
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_horz.png") == 0)
    return GRAPHIC_BRUSH_HORZ;
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_vert.png") == 0)
    return GRAPHIC_BRUSH_VERT;
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_cross.png") == 0)
    return GRAPHIC_BRUSH_CROSS;
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_diag1.png") == 0)
    return GRAPHIC_BRUSH_DIAG1;
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_diag2.png") == 0)
    return GRAPHIC_BRUSH_DIAG2;
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_crossdiag.png") == 0)
    return GRAPHIC_BRUSH_CROSS_DIAG;
  if (strcmp(resource, "http://www.utopia.gov/stdbrush_dots.png") == 0)
    return GRAPHIC_BRUSH_DOTS;
  return GRAPHIC_BRUSH_HORZ;
}

void MyMapPanel::GenericVector(MapLayer * mapLyr, rl2MapLayerPtr lyr)
{
//
// preparing a generic Vector Layer
//
  QuickStyleObj *qstyle;
  char hexRGB[32];
  rl2MapLineSymbolizerPtr line_sym;
  int len;
  char *label;
  char *font_family;

  if (mapLyr != NULL)
    {
      int line_count = 0;
      if (lyr->vector_style == NULL && lyr->vector_style_internal_name == NULL)
        {
          // skipping an invalid VectorLayer
          return;
        }
      if (lyr->visible)
        mapLyr->Show();
      else
        mapLyr->Hide();
      if (lyr->vector_style_internal_name != NULL)
        {
          // applying an internal style name
          mapLyr->GetVectorConfig()->SetStyle(lyr->vector_style_internal_name);
          return;
        }
      if (lyr->vector_style->point_sym != NULL)
        {
          // setting up a POINT Symbolizer
          qstyle = new QuickStyleObj(QUICK_STYLE_POINT);
          if (lyr->ok_min_scale)
            {
              qstyle->EnableMinScale(true);
              qstyle->SetScaleMin(lyr->min_scale);
          } else
            qstyle->EnableMinScale(false);
          if (lyr->ok_max_scale)
            {
              qstyle->EnableMaxScale(true);
              qstyle->SetScaleMax(lyr->max_scale);
          } else
            qstyle->EnableMaxScale(false);
          if (lyr->vector_style->point_sym->mark != NULL)
            {
              qstyle->SetSymbolWellKnownMark(lyr->vector_style->point_sym->
                                             mark->type);
              if (lyr->vector_style->point_sym->mark->fill != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->vector_style->point_sym->mark->fill->red,
                          lyr->vector_style->point_sym->mark->fill->green,
                          lyr->vector_style->point_sym->mark->fill->blue);
                  qstyle->SetSymbolFillColor(hexRGB);
                }
              if (lyr->vector_style->point_sym->mark->stroke != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->vector_style->point_sym->mark->stroke->red,
                          lyr->vector_style->point_sym->mark->stroke->green,
                          lyr->vector_style->point_sym->mark->stroke->blue);
                  qstyle->SetSymbolStrokeColor(hexRGB);
                }
            }
          qstyle->SetSymbolOpacity(lyr->vector_style->point_sym->opacity);
          qstyle->SetSymbolSize(lyr->vector_style->point_sym->size);
          qstyle->SetSymbolAnchorX(lyr->vector_style->point_sym->anchor_x);
          qstyle->SetSymbolAnchorY(lyr->vector_style->point_sym->anchor_y);
          qstyle->SetSymbolDisplacementX(lyr->vector_style->point_sym->
                                         displacement_x);
          qstyle->SetSymbolDisplacementY(lyr->vector_style->point_sym->
                                         displacement_y);
          qstyle->SetSymbolRotation(lyr->vector_style->point_sym->rotation);
        }
      line_sym = lyr->vector_style->first_line_sym;
      if (line_sym != NULL)
        {
          qstyle = new QuickStyleObj(QUICK_STYLE_LINE);
          if (lyr->ok_min_scale)
            {
              qstyle->EnableMinScale(true);
              qstyle->SetScaleMin(lyr->min_scale);
          } else
            qstyle->EnableMinScale(false);
          if (lyr->ok_max_scale)
            {
              qstyle->EnableMaxScale(true);
              qstyle->SetScaleMax(lyr->max_scale);
          } else
            qstyle->EnableMaxScale(false);
        }
      while (line_sym != NULL)
        {
          if (line_sym->stroke != NULL)
            {
              int dot_style;
              if (line_count == 0)
                {
                  qstyle->SetLineOpacity(line_sym->stroke->opacity);
                  qstyle->SetLineStrokeWidth(line_sym->stroke->width);
                  sprintf(hexRGB, "#%02x%02x%02x",
                          line_sym->stroke->red,
                          line_sym->stroke->green, line_sym->stroke->blue);
                  qstyle->SetLineStrokeColor(hexRGB);
                  switch (line_sym->stroke->dot_style)
                    {
                      case EXT_QUICK_STYLE_DASH_DOT_LINE:
                        dot_style = QUICK_STYLE_DASH_DOT_LINE;
                        break;
                      case EXT_QUICK_STYLE_DASH_LINE:
                        dot_style = QUICK_STYLE_DASH_LINE;
                        break;
                      case EXT_QUICK_STYLE_DOT_LINE:
                        dot_style = QUICK_STYLE_DOT_LINE;
                        break;
                      case EXT_QUICK_STYLE_SOLID_LINE:
                      default:
                        dot_style = QUICK_STYLE_SOLID_LINE;
                        break;
                    }
                  qstyle->SetLineDotStyle(dot_style);
                  qstyle->SetLinePerpendicularOffset
                    (line_sym->perpendicular_offset);
              } else
                {
                  qstyle->SetLine2Enabled(true);
                  qstyle->SetLine2StrokeWidth(line_sym->stroke->width);
                  sprintf(hexRGB, "#%02x%02x%02x",
                          line_sym->stroke->red,
                          line_sym->stroke->green, line_sym->stroke->blue);
                  qstyle->SetLine2StrokeColor(hexRGB);
                  switch (line_sym->stroke->dot_style)
                    {
                      case EXT_QUICK_STYLE_DASH_DOT_LINE:
                        dot_style = QUICK_STYLE_DASH_DOT_LINE;
                        break;
                      case EXT_QUICK_STYLE_DASH_LINE:
                        dot_style = QUICK_STYLE_DASH_LINE;
                        break;
                      case EXT_QUICK_STYLE_DOT_LINE:
                        dot_style = QUICK_STYLE_DOT_LINE;
                        break;
                      case EXT_QUICK_STYLE_SOLID_LINE:
                      default:
                        dot_style = QUICK_STYLE_SOLID_LINE;
                        break;
                    }
                  qstyle->SetLine2DotStyle(dot_style);
                }
              line_count++;
            }
          line_sym = line_sym->next;
        }
      if (lyr->vector_style->polygon_sym != NULL)
        {
          qstyle = new QuickStyleObj(QUICK_STYLE_POLYGON);
          if (lyr->ok_min_scale)
            {
              qstyle->EnableMinScale(true);
              qstyle->SetScaleMin(lyr->min_scale);
          } else
            qstyle->EnableMinScale(false);
          if (lyr->ok_max_scale)
            {
              qstyle->EnableMaxScale(true);
              qstyle->SetScaleMax(lyr->max_scale);
          } else
            qstyle->EnableMaxScale(false);
          if (lyr->vector_style->polygon_sym->fill == NULL)
            qstyle->SetPolygonFill(false);
          else
            {
              qstyle->SetPolygonFill(true);
              qstyle->SetPolygonFillOpacity(lyr->vector_style->polygon_sym->
                                            fill->opacity);
              if (lyr->vector_style->polygon_sym->fill->graphic != NULL)
                {
                  int brush_id =
                    DoFindBrushId(lyr->vector_style->polygon_sym->fill->
                                  graphic->resource);
                  qstyle->SetPolygonSolidFill(false);
                  qstyle->SetPolygonFillBrushId(brush_id);
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->vector_style->polygon_sym->fill->graphic->
                          color->red,
                          lyr->vector_style->polygon_sym->fill->graphic->
                          color->green,
                          lyr->vector_style->polygon_sym->fill->graphic->
                          color->blue);
              } else
                {
                  qstyle->SetPolygonSolidFill(true);
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->vector_style->polygon_sym->fill->red,
                          lyr->vector_style->polygon_sym->fill->green,
                          lyr->vector_style->polygon_sym->fill->blue);
                }
              qstyle->SetPolygonFillColor(hexRGB);
            }
          if (lyr->vector_style->polygon_sym->stroke == NULL)
            qstyle->SetPolygonStroke(false);
          else
            {
              qstyle->SetPolygonStroke(true);
              qstyle->SetPolygonStrokeOpacity
                (lyr->vector_style->polygon_sym->stroke->opacity);
              qstyle->SetPolygonStrokeWidth(lyr->vector_style->polygon_sym->
                                            stroke->width);
              sprintf(hexRGB, "#%02x%02x%02x",
                      lyr->vector_style->polygon_sym->stroke->red,
                      lyr->vector_style->polygon_sym->stroke->green,
                      lyr->vector_style->polygon_sym->stroke->blue);
              qstyle->SetPolygonStrokeColor(hexRGB);
            }
          qstyle->SetPolygonDisplacementX(lyr->vector_style->polygon_sym->
                                          displacement_x);
          qstyle->SetPolygonDisplacementY(lyr->vector_style->polygon_sym->
                                          displacement_y);
          qstyle->SetPolygonPerpendicularOffset(lyr->vector_style->polygon_sym->
                                                perpendicular_offset);
        }
      if (lyr->vector_style->text_alone)
        qstyle->SetDontPaintGeomSymbolizer(true);
      else
        qstyle->SetDontPaintGeomSymbolizer(false);
      if (lyr->vector_style->text_sym != NULL)
        {
          qstyle->SetLabelPrint(true);
          len = strlen(lyr->vector_style->text_sym->label);
          if (*(lyr->vector_style->text_sym->label + 0) == '@'
              && *(lyr->vector_style->text_sym->label + 0) == '@')
            {
              label = (char *) malloc(len);
              strcpy(label, lyr->vector_style->text_sym->label + 1);
              *(label + len - 2) = '\0';
          } else
            {
              label = (char *) malloc(len + 1);
              strcpy(label, lyr->vector_style->text_sym->label);
            }
          qstyle->SetLabelColumn(label);
          free(label);
          if (lyr->vector_style->text_sym->font != NULL)
            {
              len = strlen(lyr->vector_style->text_sym->font->family);
              font_family = (char *) malloc(len + 10);
              sprintf(font_family, "ToyFont: %s",
                      lyr->vector_style->text_sym->font->family);
              qstyle->SetFontFacename(font_family);
              free(font_family);
              qstyle->SetFontStyle(lyr->vector_style->text_sym->font->style);
              qstyle->SetFontWeight(lyr->vector_style->text_sym->font->weight);
              qstyle->SetFontSize(lyr->vector_style->text_sym->font->size);
            }
          if (lyr->vector_style->text_sym->placement != NULL)
            {
              if (lyr->vector_style->text_sym->placement->point != NULL)
                {
                  qstyle->SetLabelLinePlacement(false);
                  qstyle->SetLabelAnchorX(lyr->vector_style->text_sym->
                                          placement->point->anchor_x);
                  qstyle->SetLabelAnchorY(lyr->vector_style->text_sym->
                                          placement->point->anchor_y);
                  qstyle->SetLabelDisplacementX(lyr->vector_style->text_sym->
                                                placement->point->
                                                displacement_x);
                  qstyle->SetLabelDisplacementY(lyr->vector_style->text_sym->
                                                placement->point->
                                                displacement_y);
                  qstyle->SetLabelRotation(lyr->vector_style->text_sym->
                                           placement->point->rotation);
                }
              if (lyr->vector_style->text_sym->placement->line != NULL)
                {
                  qstyle->SetLabelLinePlacement(true);
                  qstyle->SetLabelPerpendicularOffset
                    (lyr->vector_style->text_sym->placement->line->
                     perpendicular_offset);
                  if (lyr->vector_style->text_sym->placement->line->repeated)
                    qstyle->SetRepeatedLabel(true);
                  else
                    qstyle->SetRepeatedLabel(false);
                  qstyle->SetLabelInitialGap(lyr->vector_style->text_sym->
                                             placement->line->initial_gap);
                  qstyle->SetLabelGap(lyr->vector_style->text_sym->placement->
                                      line->gap);
                  if (lyr->vector_style->text_sym->placement->line->aligned)
                    qstyle->SetLabelIsAligned(true);
                  else
                    qstyle->SetLabelIsAligned(false);
                  if (lyr->vector_style->text_sym->placement->line->generalize)
                    qstyle->SetLabelGeneralizeLine(true);
                  else
                    qstyle->SetLabelGeneralizeLine(false);
                }
            }
          if (lyr->vector_style->text_sym->fill != NULL)
            {
              qstyle->SetFontOpacity(lyr->vector_style->text_sym->fill->
                                     opacity);
              sprintf(hexRGB, "#%02x%02x%02x",
                      lyr->vector_style->text_sym->fill->red,
                      lyr->vector_style->text_sym->fill->green,
                      lyr->vector_style->text_sym->fill->blue);
              qstyle->SetFontColor(hexRGB);
            }
          if (lyr->vector_style->text_sym->halo != NULL)
            {
              qstyle->EnableHalo(true);
              qstyle->SetHaloRadius(lyr->vector_style->text_sym->halo->radius);
              if (lyr->vector_style->text_sym->halo->fill != NULL)
                {
                  qstyle->SetHaloOpacity(lyr->vector_style->text_sym->halo->
                                         fill->opacity);
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->vector_style->text_sym->halo->fill->red,
                          lyr->vector_style->text_sym->halo->fill->green,
                          lyr->vector_style->text_sym->halo->fill->blue);
                  qstyle->SetHaloColor(hexRGB);
                }
            }
        }
      mapLyr->GetVectorConfig()->SetStyle(qstyle->GetUUID());
      mapLyr->SetQuickStyle(qstyle);
    }
}

void MyMapPanel::TopologyLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr)
{
//
// preparing a Topology Layer
//
  QuickStyleTopologyObj *qstyle;
  char hexRGB[32];

  if (mapLyr != NULL)
    {
      if (lyr->topology_style == NULL && lyr->topology_internal_style == NULL)
        {
          // skipping an invalid TopologyLayer
          return;
        }
      if (lyr->visible)
        mapLyr->Show();
      else
        mapLyr->Hide();
      qstyle = new QuickStyleTopologyObj(MAP_LAYER_TOPOLOGY);
      if (lyr->ok_min_scale)
        {
          qstyle->EnableMinScale(true);
          qstyle->SetScaleMin(lyr->min_scale);
      } else
        qstyle->EnableMinScale(false);
      if (lyr->ok_max_scale)
        {
          qstyle->EnableMaxScale(true);
          qstyle->SetScaleMax(lyr->max_scale);
      } else
        qstyle->EnableMaxScale(false);
      if (lyr->topology_internal_style != NULL)
        {
          // applying an internal style name
          mapLyr->GetVectorConfig()->SetStyle(lyr->topology_internal_style->
                                              style_internal_name);
          if (lyr->topology_internal_style->show_faces)
            mapLyr->GetVectorConfig()->SetFacesVisible(true);
          else
            mapLyr->GetVectorConfig()->SetFacesVisible(false);
          if (lyr->topology_internal_style->show_edges)
            mapLyr->GetVectorConfig()->SetEdgesVisible(true);
          else
            mapLyr->GetVectorConfig()->SetNodesVisible(false);
          if (lyr->topology_internal_style->show_nodes)
            mapLyr->GetVectorConfig()->SetNodesVisible(true);
          else
            mapLyr->GetVectorConfig()->SetNodesVisible(false);
          if (lyr->topology_internal_style->show_edge_seeds)
            mapLyr->GetVectorConfig()->SetEdgeSeedsVisible(true);
          else
            mapLyr->GetVectorConfig()->SetEdgeSeedsVisible(false);
          if (lyr->topology_internal_style->show_face_seeds)
            mapLyr->GetVectorConfig()->SetFaceSeedsVisible(true);
          else
            mapLyr->GetVectorConfig()->SetFaceSeedsVisible(false);
          return;
        }
      if (lyr->topology_style->faces_sym != NULL)
        {
          // setting up the Faces (POLYGON) Symbolizer
          if (lyr->topology_style->faces_sym->fill == NULL)
            qstyle->SetFaceFill(false);
          else
            {
              qstyle->SetFaceFill(true);
              qstyle->SetFaceFillOpacity(lyr->topology_style->faces_sym->fill->
                                         opacity);
              sprintf(hexRGB, "#%02x%02x%02x",
                      lyr->topology_style->faces_sym->fill->red,
                      lyr->topology_style->faces_sym->fill->green,
                      lyr->topology_style->faces_sym->fill->blue);
              qstyle->SetFaceFillColor(hexRGB);
            }
          if (lyr->topology_style->faces_sym->stroke == NULL)
            qstyle->SetFaceStroke(false);
          else
            {
              qstyle->SetFaceStroke(true);
              qstyle->SetFaceStrokeOpacity
                (lyr->topology_style->faces_sym->stroke->opacity);
              qstyle->SetFaceStrokeWidth(lyr->topology_style->faces_sym->
                                         stroke->width);
              sprintf(hexRGB, "#%02x%02x%02x",
                      lyr->topology_style->faces_sym->stroke->red,
                      lyr->topology_style->faces_sym->stroke->green,
                      lyr->topology_style->faces_sym->stroke->blue);
              qstyle->SetFaceStrokeColor(hexRGB);
            }
          qstyle->SetFaceDisplacementX(lyr->topology_style->faces_sym->
                                       displacement_x);
          qstyle->SetFaceDisplacementY(lyr->topology_style->faces_sym->
                                       displacement_y);
          qstyle->SetFacePerpendicularOffset(lyr->topology_style->faces_sym->
                                             perpendicular_offset);
        }
      if (lyr->topology_style->edges_sym != NULL)
        {
          // setting up the Edges (LINE) Symbolizer
          if (lyr->topology_style->edges_sym->stroke != NULL)
            {
              int dot_style;
              qstyle->SetEdgeLinkOpacity(lyr->topology_style->edges_sym->
                                         stroke->opacity);
              qstyle->SetEdgeLinkStrokeWidth(lyr->topology_style->edges_sym->
                                             stroke->width);
              sprintf(hexRGB, "#%02x%02x%02x",
                      lyr->topology_style->edges_sym->stroke->red,
                      lyr->topology_style->edges_sym->stroke->green,
                      lyr->topology_style->edges_sym->stroke->blue);
              qstyle->SetEdgeLinkStrokeColor(hexRGB);
              switch (lyr->topology_style->edges_sym->stroke->dot_style)
                {
                  case EXT_QUICK_STYLE_DASH_DOT_LINE:
                    dot_style = QUICK_STYLE_DASH_DOT_LINE;
                    break;
                  case EXT_QUICK_STYLE_DASH_LINE:
                    dot_style = QUICK_STYLE_DASH_LINE;
                    break;
                  case EXT_QUICK_STYLE_DOT_LINE:
                    dot_style = QUICK_STYLE_DOT_LINE;
                    break;
                  case EXT_QUICK_STYLE_SOLID_LINE:
                  default:
                    dot_style = QUICK_STYLE_SOLID_LINE;
                    break;
                }
              qstyle->SetEdgeLinkDotStyle(dot_style);
              qstyle->SetEdgeLinkPerpendicularOffset
                (lyr->topology_style->edges_sym->perpendicular_offset);
            }
        }
      if (lyr->topology_style->nodes_sym != NULL)
        {
          // setting up the Nodes (POINT) Symbolizer
          if (lyr->topology_style->nodes_sym->mark != NULL)
            {
              qstyle->SetNodeWellKnownMark(lyr->topology_style->nodes_sym->
                                           mark->type);
              if (lyr->topology_style->nodes_sym->mark->fill != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->topology_style->nodes_sym->mark->fill->red,
                          lyr->topology_style->nodes_sym->mark->fill->green,
                          lyr->topology_style->nodes_sym->mark->fill->blue);
                  qstyle->SetNodeFillColor(hexRGB);
                }
              if (lyr->topology_style->nodes_sym->mark->stroke != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->topology_style->nodes_sym->mark->stroke->red,
                          lyr->topology_style->nodes_sym->mark->stroke->green,
                          lyr->topology_style->nodes_sym->mark->stroke->blue);
                  qstyle->SetNodeStrokeColor(hexRGB);
                }
            }
          qstyle->SetNodeOpacity(lyr->topology_style->nodes_sym->opacity);
          qstyle->SetNodeSize(lyr->topology_style->nodes_sym->size);
          qstyle->SetNodeAnchorX(lyr->topology_style->nodes_sym->anchor_x);
          qstyle->SetNodeAnchorY(lyr->topology_style->nodes_sym->anchor_y);
          qstyle->SetNodeDisplacementX(lyr->topology_style->nodes_sym->
                                       displacement_x);
          qstyle->SetNodeDisplacementY(lyr->topology_style->nodes_sym->
                                       displacement_y);
          qstyle->SetNodeRotation(lyr->topology_style->nodes_sym->rotation);
        }
      if (lyr->topology_style->edge_seeds_sym != NULL)
        {
          // setting up the EdgeSeeds (POINT) Symbolizer
          if (lyr->topology_style->edge_seeds_sym->mark != NULL)
            {
              qstyle->SetEdgeLinkSeedWellKnownMark(lyr->topology_style->
                                                   edge_seeds_sym->mark->type);
              if (lyr->topology_style->edge_seeds_sym->mark->fill != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->topology_style->edge_seeds_sym->mark->fill->red,
                          lyr->topology_style->edge_seeds_sym->mark->fill->
                          green,
                          lyr->topology_style->edge_seeds_sym->mark->fill->
                          blue);
                  qstyle->SetEdgeLinkSeedFillColor(hexRGB);
                }
              if (lyr->topology_style->edge_seeds_sym->mark->stroke != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->topology_style->edge_seeds_sym->mark->stroke->
                          red,
                          lyr->topology_style->edge_seeds_sym->mark->stroke->
                          green,
                          lyr->topology_style->edge_seeds_sym->mark->stroke->
                          blue);
                  qstyle->SetEdgeLinkSeedStrokeColor(hexRGB);
                }
            }
          qstyle->SetEdgeLinkSeedOpacity(lyr->topology_style->edge_seeds_sym->
                                         opacity);
          qstyle->SetEdgeLinkSeedSize(lyr->topology_style->edge_seeds_sym->
                                      size);
          qstyle->SetEdgeLinkSeedAnchorX(lyr->topology_style->edge_seeds_sym->
                                         anchor_x);
          qstyle->SetEdgeLinkSeedAnchorY(lyr->topology_style->edge_seeds_sym->
                                         anchor_y);
          qstyle->SetEdgeLinkSeedDisplacementX(lyr->topology_style->
                                               edge_seeds_sym->displacement_x);
          qstyle->SetEdgeLinkSeedDisplacementY(lyr->topology_style->
                                               edge_seeds_sym->displacement_y);
          qstyle->SetEdgeLinkSeedRotation(lyr->topology_style->edge_seeds_sym->
                                          rotation);
        }
      if (lyr->topology_style->face_seeds_sym != NULL)
        {
          // setting up the FaceSeeds (POINT) Symbolizer
          if (lyr->topology_style->face_seeds_sym->mark != NULL)
            {
              qstyle->SetFaceSeedWellKnownMark(lyr->topology_style->
                                               face_seeds_sym->mark->type);
              if (lyr->topology_style->face_seeds_sym->mark->fill != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->topology_style->face_seeds_sym->mark->fill->red,
                          lyr->topology_style->face_seeds_sym->mark->fill->
                          green,
                          lyr->topology_style->face_seeds_sym->mark->fill->
                          blue);
                  qstyle->SetFaceSeedFillColor(hexRGB);
                }
              if (lyr->topology_style->face_seeds_sym->mark->stroke != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->topology_style->face_seeds_sym->mark->stroke->
                          red,
                          lyr->topology_style->face_seeds_sym->mark->stroke->
                          green,
                          lyr->topology_style->face_seeds_sym->mark->stroke->
                          blue);
                  qstyle->SetFaceSeedStrokeColor(hexRGB);
                }
            }
          qstyle->SetFaceSeedOpacity(lyr->topology_style->face_seeds_sym->
                                     opacity);
          qstyle->SetFaceSeedSize(lyr->topology_style->face_seeds_sym->size);
          qstyle->SetFaceSeedAnchorX(lyr->topology_style->face_seeds_sym->
                                     anchor_x);
          qstyle->SetFaceSeedAnchorY(lyr->topology_style->face_seeds_sym->
                                     anchor_y);
          qstyle->SetFaceSeedDisplacementX(lyr->topology_style->face_seeds_sym->
                                           displacement_x);
          qstyle->SetFaceSeedDisplacementY(lyr->topology_style->face_seeds_sym->
                                           displacement_y);
          qstyle->SetFaceSeedRotation(lyr->topology_style->face_seeds_sym->
                                      rotation);
        }
      mapLyr->GetVectorConfig()->SetStyle(qstyle->GetUUID());
      mapLyr->SetQuickStyle(qstyle);
      if (lyr->topology_style->show_faces)
        mapLyr->GetVectorConfig()->SetFacesVisible(true);
      else
        mapLyr->GetVectorConfig()->SetFacesVisible(false);
      if (lyr->topology_style->show_edges)
        mapLyr->GetVectorConfig()->SetEdgesVisible(true);
      else
        mapLyr->GetVectorConfig()->SetNodesVisible(false);
      if (lyr->topology_style->show_nodes)
        mapLyr->GetVectorConfig()->SetNodesVisible(true);
      else
        mapLyr->GetVectorConfig()->SetNodesVisible(false);
      if (lyr->topology_style->show_edge_seeds)
        mapLyr->GetVectorConfig()->SetEdgeSeedsVisible(true);
      else
        mapLyr->GetVectorConfig()->SetEdgeSeedsVisible(false);
      if (lyr->topology_style->show_face_seeds)
        mapLyr->GetVectorConfig()->SetFaceSeedsVisible(true);
      else
        mapLyr->GetVectorConfig()->SetFaceSeedsVisible(false);
    }
}

void MyMapPanel::NetworkLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr)
{
//
// preparing a Network Layer
//
  QuickStyleTopologyObj *qstyle;
  char hexRGB[32];

  if (mapLyr != NULL)
    {
      if (lyr->network_style == NULL && lyr->network_internal_style == NULL)
        {
          // skipping an invalid NetworkLayer
          return;
        }
      if (lyr->visible)
        mapLyr->Show();
      else
        mapLyr->Hide();
      qstyle = new QuickStyleTopologyObj(MAP_LAYER_NETWORK);
      if (lyr->ok_min_scale)
        {
          qstyle->EnableMinScale(true);
          qstyle->SetScaleMin(lyr->min_scale);
      } else
        qstyle->EnableMinScale(false);
      if (lyr->ok_max_scale)
        {
          qstyle->EnableMaxScale(true);
          qstyle->SetScaleMax(lyr->max_scale);
      } else
        qstyle->EnableMaxScale(false);
      if (lyr->network_internal_style != NULL)
        {
          // applying an internal style name
          mapLyr->GetVectorConfig()->SetStyle(lyr->network_internal_style->
                                              style_internal_name);
          if (lyr->network_internal_style->show_links)
            mapLyr->GetVectorConfig()->SetLinksVisible(true);
          else
            mapLyr->GetVectorConfig()->SetNodesVisible(false);
          if (lyr->network_internal_style->show_nodes)
            mapLyr->GetVectorConfig()->SetNodesVisible(true);
          else
            mapLyr->GetVectorConfig()->SetNodesVisible(false);
          if (lyr->network_internal_style->show_link_seeds)
            mapLyr->GetVectorConfig()->SetLinkSeedsVisible(true);
          else
            mapLyr->GetVectorConfig()->SetLinkSeedsVisible(false);
          return;
        }
      if (lyr->network_style->links_sym != NULL)
        {
          // setting up the Links (LINE) Symbolizer 
          if (lyr->network_style->links_sym->stroke != NULL)
            {
              int dot_style;
              qstyle->SetEdgeLinkOpacity(lyr->network_style->links_sym->
                                         stroke->opacity);
              qstyle->SetEdgeLinkStrokeWidth(lyr->network_style->links_sym->
                                             stroke->width);
              sprintf(hexRGB, "#%02x%02x%02x",
                      lyr->network_style->links_sym->stroke->red,
                      lyr->network_style->links_sym->stroke->green,
                      lyr->network_style->links_sym->stroke->blue);
              qstyle->SetEdgeLinkStrokeColor(hexRGB);
              switch (lyr->network_style->links_sym->stroke->dot_style)
                {
                  case EXT_QUICK_STYLE_DASH_DOT_LINE:
                    dot_style = QUICK_STYLE_DASH_DOT_LINE;
                    break;
                  case EXT_QUICK_STYLE_DASH_LINE:
                    dot_style = QUICK_STYLE_DASH_LINE;
                    break;
                  case EXT_QUICK_STYLE_DOT_LINE:
                    dot_style = QUICK_STYLE_DOT_LINE;
                    break;
                  case EXT_QUICK_STYLE_SOLID_LINE:
                  default:
                    dot_style = QUICK_STYLE_SOLID_LINE;
                    break;
                }
              qstyle->SetEdgeLinkDotStyle(dot_style);
              qstyle->SetEdgeLinkPerpendicularOffset
                (lyr->network_style->links_sym->perpendicular_offset);
            }
        }
      if (lyr->network_style->nodes_sym != NULL)
        {
          // setting up the Nodes (POINT) Symbolizer
          if (lyr->network_style->nodes_sym->mark != NULL)
            {
              qstyle->SetNodeWellKnownMark(lyr->network_style->nodes_sym->
                                           mark->type);
              if (lyr->network_style->nodes_sym->mark->fill != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->network_style->nodes_sym->mark->fill->red,
                          lyr->network_style->nodes_sym->mark->fill->green,
                          lyr->network_style->nodes_sym->mark->fill->blue);
                  qstyle->SetNodeFillColor(hexRGB);
                }
              if (lyr->network_style->nodes_sym->mark->stroke != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->network_style->nodes_sym->mark->stroke->red,
                          lyr->network_style->nodes_sym->mark->stroke->green,
                          lyr->network_style->nodes_sym->mark->stroke->blue);
                  qstyle->SetNodeStrokeColor(hexRGB);
                }
            }
          qstyle->SetNodeOpacity(lyr->network_style->nodes_sym->opacity);
          qstyle->SetNodeSize(lyr->network_style->nodes_sym->size);
          qstyle->SetNodeAnchorX(lyr->network_style->nodes_sym->anchor_x);
          qstyle->SetNodeAnchorY(lyr->network_style->nodes_sym->anchor_y);
          qstyle->SetNodeDisplacementX(lyr->network_style->nodes_sym->
                                       displacement_x);
          qstyle->SetNodeDisplacementY(lyr->network_style->nodes_sym->
                                       displacement_y);
          qstyle->SetNodeRotation(lyr->network_style->nodes_sym->rotation);
        }
      if (lyr->network_style->link_seeds_sym != NULL)
        {
          // setting up the EdgeSeeds (POINT) Symbolizer 
          if (lyr->network_style->link_seeds_sym->mark != NULL)
            {
              qstyle->SetEdgeLinkSeedWellKnownMark(lyr->network_style->
                                                   link_seeds_sym->mark->type);
              if (lyr->network_style->link_seeds_sym->mark->fill != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->network_style->link_seeds_sym->mark->fill->red,
                          lyr->network_style->link_seeds_sym->mark->fill->green,
                          lyr->network_style->link_seeds_sym->mark->fill->blue);
                  qstyle->SetEdgeLinkSeedFillColor(hexRGB);
                }
              if (lyr->network_style->link_seeds_sym->mark->stroke != NULL)
                {
                  sprintf(hexRGB, "#%02x%02x%02x",
                          lyr->network_style->link_seeds_sym->mark->stroke->red,
                          lyr->network_style->link_seeds_sym->mark->stroke->
                          green,
                          lyr->network_style->link_seeds_sym->mark->stroke->
                          blue);
                  qstyle->SetEdgeLinkSeedStrokeColor(hexRGB);
                }
            }
          qstyle->SetEdgeLinkSeedOpacity(lyr->network_style->link_seeds_sym->
                                         opacity);
          qstyle->SetEdgeLinkSeedSize(lyr->network_style->link_seeds_sym->size);
          qstyle->SetEdgeLinkSeedAnchorX(lyr->network_style->link_seeds_sym->
                                         anchor_x);
          qstyle->SetEdgeLinkSeedAnchorY(lyr->network_style->link_seeds_sym->
                                         anchor_y);
          qstyle->SetEdgeLinkSeedDisplacementX(lyr->network_style->
                                               link_seeds_sym->displacement_x);
          qstyle->SetEdgeLinkSeedDisplacementY(lyr->network_style->
                                               link_seeds_sym->displacement_y);
          qstyle->SetEdgeLinkSeedRotation(lyr->network_style->link_seeds_sym->
                                          rotation);
        }
      mapLyr->GetVectorConfig()->SetStyle(qstyle->GetUUID());
      mapLyr->SetQuickStyle(qstyle);
      if (lyr->network_style->show_links)
        mapLyr->GetVectorConfig()->SetLinksVisible(true);
      else
        mapLyr->GetVectorConfig()->SetNodesVisible(false);
      if (lyr->network_style->show_nodes)
        mapLyr->GetVectorConfig()->SetNodesVisible(true);
      else
        mapLyr->GetVectorConfig()->SetNodesVisible(false);
      if (lyr->network_style->show_link_seeds)
        mapLyr->GetVectorConfig()->SetLinkSeedsVisible(true);
      else
        mapLyr->GetVectorConfig()->SetLinkSeedsVisible(false);
    }
}

void MyMapPanel::DoWmsLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr)
{
//
// preparing a WMS Layer
//
  QuickStyleWmsObj *qstyle;

  if (mapLyr != NULL)
    {
      if (lyr->wms_style == NULL)
        {
          // skipping an invalid WMS Layer 
          return;
        }
      if (lyr->visible)
        mapLyr->Show();
      else
        mapLyr->Hide();
      qstyle = new QuickStyleWmsObj();
      if (lyr->ok_min_scale)
        {
          qstyle->EnableMinScale(true);
          qstyle->SetScaleMin(lyr->min_scale);
      } else
        qstyle->EnableMinScale(false);
      if (lyr->ok_max_scale)
        {
          qstyle->EnableMaxScale(true);
          qstyle->SetScaleMax(lyr->max_scale);
      } else
        qstyle->EnableMaxScale(false);
      // setting up the WMS configuration 
      if (lyr->wms_style->wms_protocol != NULL)
        mapLyr->GetWmsConfig()->SetVersion(lyr->wms_style->wms_protocol);
      if (lyr->wms_style->crs != NULL)
        mapLyr->GetWmsConfig()->SetRefSys(lyr->wms_style->crs);
      if (lyr->wms_style->swap_xy)
        mapLyr->GetWmsConfig()->SetSwapXY(true);
      if (lyr->wms_style->style != NULL)
        mapLyr->GetWmsConfig()->SetStyle(lyr->wms_style->style);
      else
        mapLyr->GetWmsConfig()->SetStyle("default");
      if (lyr->wms_style->image_format != NULL)
        mapLyr->GetWmsConfig()->SetImageFormat(lyr->wms_style->image_format);
      if (lyr->wms_style->background_color != NULL)
        {
          mapLyr->GetWmsConfig()->SetBgColorEnabled(true);
          mapLyr->GetWmsConfig()->SetBgColor(lyr->wms_style->background_color);
        }
      if (lyr->wms_style->opaque)
        mapLyr->GetWmsConfig()->SetOpaque(true);
      if (lyr->wms_style->is_tiled)
        {
          mapLyr->GetWmsConfig()->SetTiled(true);
          mapLyr->GetWmsConfig()->SetTileWidth(lyr->wms_style->tile_width);
          mapLyr->GetWmsConfig()->SetTileHeight(lyr->wms_style->tile_height);
        }
    }
  if (lyr->wms_style->get_map_url != NULL)
    mapLyr->SetWmsGetMapURL(lyr->wms_style->get_map_url);
  if (lyr->wms_style->get_feature_info_url != NULL)
    mapLyr->SetWmsGetFeatureInfoURL(lyr->wms_style->get_feature_info_url);
  mapLyr->SetQuickStyle(qstyle);
}

bool MyMapPanel::DoFetchRasterMaxMinPixelValues(sqlite3 * sqlite,
                                                const char *prefix,
                                                const char *name, double *min,
                                                double *max)
{
//
// retrieving Min and Max pixel values for a Raster Coverage
//
  char *sql;
  char *xprefix;
  int ret;
  char **results;
  int rows;
  int columns;
  bool ok_min = false;
  bool ok_max = false;
  double mmin = 0.0;
  double mmax = 0.0;

  if (prefix == NULL)
    prefix = "main";
  xprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT RL2_GetBandStatistics_Min(statistics, 0), RL2_GetBandStatistics_Max(statistics, 0) "
     "FROM \"%s\".raster_coverages WHERE Upper(coverage_name) = Upper(%Q)",
     xprefix, name);
  free(xprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  for (int i = 1; i <= rows; i++)
    {
      if (results[(i * columns) + 0] == NULL)
        ok_min = false;
      else
        {
          ok_min = true;
          mmin = atof(results[(i * columns) + 0]);
        }
      if (results[(i * columns) + 1] == NULL)
        ok_max = false;
      else
        {
          ok_max = true;
          mmax = atof(results[(i * columns) + 1]);
        }
    }
  sqlite3_free_table(results);
  if (ok_min == true && ok_max == true)
    {
      *min = mmin;
      *max = mmax;
      return true;
    }
  return false;
}

void MyMapPanel::RasterLayer(MapLayer * mapLyr, rl2MapLayerPtr lyr)
{
//
// preparing a Raster Layer
//
  QuickStyleRasterObj *qstyle;
  bool max_min = false;

  if (mapLyr != NULL)
    {
      if (lyr->raster_style == NULL && lyr->raster_style_internal_name == NULL)
        {
          // skipping an invalid RasterLayer
          return;
        }
      if (lyr->visible)
        mapLyr->Show();
      else
        mapLyr->Hide();
      if (lyr->raster_style_internal_name != NULL)
        {
          // applying an internal style name
          mapLyr->GetRasterConfig()->SetStyle(lyr->raster_style_internal_name);
          return;
        }
      qstyle = new QuickStyleRasterObj();
      if (lyr->ok_min_scale)
        {
          qstyle->EnableMinScale(true);
          qstyle->SetScaleMin(lyr->min_scale);
      } else
        qstyle->EnableMinScale(false);
      if (lyr->ok_max_scale)
        {
          qstyle->EnableMaxScale(true);
          qstyle->SetScaleMax(lyr->max_scale);
      } else
        qstyle->EnableMaxScale(false);
      qstyle->SetOpacity(lyr->raster_style->opacity);
      if (lyr->raster_style->channel_selection != NULL)
        {
          if (lyr->raster_style->channel_selection->rgb)
            {
              qstyle->SetTripleBand(true);
              qstyle->SetRedBand(lyr->raster_style->channel_selection->
                                 red_channel - 1);
              qstyle->SetGreenBand(lyr->raster_style->channel_selection->
                                   green_channel - 1);
              qstyle->SetBlueBand(lyr->raster_style->channel_selection->
                                  blue_channel - 1);
          } else
            {
              qstyle->SetSingleBand(true);
              qstyle->SetGrayBand(lyr->raster_style->channel_selection->
                                  gray_channel - 1);
            }
        }
      if (lyr->raster_style->color_map_name != NULL)
        {
          if (strcmp(lyr->raster_style->color_map_name, "etopo2") == 0)
            qstyle->SetColorMap(COLOR_MAP_ETOPO2);
          if (strcmp(lyr->raster_style->color_map_name, "srtm") == 0)
            qstyle->SetColorMap(COLOR_MAP_SRTM);
          if (strcmp(lyr->raster_style->color_map_name, "terrain") == 0)
            qstyle->SetColorMap(COLOR_MAP_TERRAIN);
          if (strcmp(lyr->raster_style->color_map_name, "elevation") == 0)
            {
              qstyle->SetColorMap(COLOR_MAP_ELEVATION);
              max_min = true;
            }
          if (strcmp(lyr->raster_style->color_map_name, "aspect_color") == 0)
            {
              qstyle->SetColorMap(COLOR_MAP_ASPECT_COLOR);
              max_min = true;
            }
          if (strcmp(lyr->raster_style->color_map_name, "rainbow") == 0)
            {
              qstyle->SetColorMap(COLOR_MAP_RAINBOW);
              max_min = true;
            }
          if (strcmp(lyr->raster_style->color_map_name, "wave") == 0)
            {
              qstyle->SetColorMap(COLOR_MAP_WAVE);
              max_min = true;
            }
          if (strcmp(lyr->raster_style->color_map_name, "sepia") == 0)
            {
              qstyle->SetColorMap(COLOR_MAP_SEPIA);
              max_min = true;
            }
          if (strcmp(lyr->raster_style->color_map_name, "ndvi") == 0)
            qstyle->SetNdviColorMap(true);
          if (strcmp(lyr->raster_style->color_map_name, "ndwi") == 0)
            qstyle->SetNdwiColorMap(true);
        }
      if (lyr->raster_style->color_ramp != NULL)
        {
          qstyle->SetColorRamp(true);
          qstyle->SetMinValue(lyr->raster_style->color_ramp->min_value);
          qstyle->SetMinValueColor(lyr->raster_style->color_ramp->min_color);
          qstyle->SetMaxValue(lyr->raster_style->color_ramp->max_value);
          qstyle->SetMaxValueColor(lyr->raster_style->color_ramp->max_color);
          max_min = true;
        }
      if (lyr->raster_style->contrast_enhancement != NULL)
        {
          if (lyr->raster_style->contrast_enhancement->normalize)
            qstyle->SetNormalize(true);
          if (lyr->raster_style->contrast_enhancement->histogram)
            qstyle->SetHistogram(true);
          if (lyr->raster_style->contrast_enhancement->gamma)
            {
              qstyle->SetGamma(true);
              qstyle->SetGammaValue(lyr->raster_style->contrast_enhancement->
                                    gamma_value);
            }
        }
      if (lyr->raster_style->shaded_relief)
        {
          qstyle->SetShadedRelief(true);
          qstyle->SetShadedReliefFactor(lyr->raster_style->relief_factor);
        }
    }
  if (max_min == true)
    {
      double min;
      double max;
      if (DoFetchRasterMaxMinPixelValues
          (GetSqlite(), lyr->prefix, lyr->name, &min, &max))
        {
          qstyle->SetMinValue(min);
          qstyle->SetMaxValue(max);
        }
    }
  mapLyr->GetRasterConfig()->SetStyle(qstyle->GetUUID());
  mapLyr->SetQuickStyle(qstyle);
}

void MyMapPanel::OnLoadMapConfig(wxCommandEvent & WXUNUSED(event))
{
//
// loading a predefined Map Configuration
//
  bool fullExtent = true;
  double minX;
  double minY;
  double maxX;
  double maxY;
  rl2MapConfigPtr map_config = NULL;
  rl2MapAttachedDbPtr db_obj;
  MapConfigAttached *db_list = NULL;
  ListMapConfigDialog dlg;
  dlg.Create(Parent, this);
  int ret = dlg.ShowModal();
  if (ret == wxID_OK)
    {
      int max;
      wxString str;
      wxColour clr;
      unsigned char *xml = dlg.GetXML();
      if (xml != NULL)
        {
          MapLayer *mapLyr;
          rl2MapLayerPtr lyr;
          map_config = rl2_parse_map_config_xml(xml);
          free(xml);
          LayerTree->CleanUp();
          // setting Map Options
          str = wxString::FromUTF8(map_config->name);

          SetMapName(str);
          str = wxString::FromUTF8(map_config->title);
          SetTitle(str);
          str = wxString::FromUTF8(map_config->abstract);
          SetAbstract(str);
          if (map_config->multithread_enabled)
            EnableMapMultiThreading(true);
          else
            EnableMapMultiThreading(false);
          if (!IsMapMultiThreadingEnabled())
            max = 1;
          else
            {
              max = map_config->max_threads;
              if (max < 0)
                max = 1;
              if (max > 32)
                max = 32;
            }
          SetMapMaxThreads(max);
          UpdateMaxThreads();
          if (map_config->map_background_transparent)
            SetCheckeredMapBackground(true);
          else
            SetCheckeredMapBackground(false);
          clr =
            wxColour(map_config->map_background_red,
                     map_config->map_background_green,
                     map_config->map_background_blue);
          SetSolidMapBackground(clr);
          if (map_config->raster_wms_auto_switch)
            SetRasterWmwAutoSwitch(true);
          else
            SetRasterWmwAutoSwitch(false);
          if (map_config->autotransform_enabled)
            EnableMapAutoTransform(true);
          else
            EnableMapAutoTransform(false);
          SetMapSRID(map_config->srid);
          if (map_config->dms)
            SetGeographicCoordsDMS(true);
          else
            SetGeographicCoordsDMS(false);
          if (map_config->label_anti_collision)
            SetLabelAntiCollision(true);
          else
            SetLabelAntiCollision(false);
          if (map_config->label_wrap_text)
            SetLabelWrapText(true);
          else
            SetLabelWrapText(false);
          if (map_config->label_auto_rotate)
            SetLabelAutoRotate(true);
          else
            SetLabelAutoRotate(false);
          if (map_config->label_shift_position)
            SetLabelShiftPosition(true);
          else
            SetLabelShiftPosition(false);

          if (map_config->bbox != NULL)
            {
              // saving the Map Config Extent
              fullExtent = false;
              minX = map_config->bbox->minx;
              minY = map_config->bbox->miny;
              maxX = map_config->bbox->maxx;
              maxY = map_config->bbox->maxy;
            }
          // handling any required ATTACH DATABASE
          db_list = CreateAttachedList();
          db_obj = map_config->first_db;
          while (db_obj != NULL)
            {
              // loading the ATTACHED DB list
              db_list->Add(db_obj->prefix, db_obj->path);
              db_obj = db_obj->next;
            }
          if (db_list->GetFirst() != NULL)
            db_list->Verify();

          // loading Layers
          lyr = map_config->first_lyr;
          while (lyr != NULL)
            {
              const char *alias;
              switch (lyr->type)
                {
                  case MAP_LAYER_RASTER:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerRaster(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            RasterLayer(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                  case MAP_LAYER_WMS:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerWMS(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            DoWmsLayer(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                  case MAP_LAYER_VECTOR:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerVector(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            GenericVector(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                  case MAP_LAYER_VECTOR_VIEW:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerVectorView(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            GenericVector(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                  case MAP_LAYER_VECTOR_VIRTUAL:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerVectorVirtual(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            GenericVector(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                  case MAP_LAYER_TOPOLOGY:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerTopology(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            TopologyLayer(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                  case MAP_LAYER_NETWORK:
                    if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
                      {
                        mapLyr = DoFetchLayerNetwork(alias, lyr->name);
                        if (mapLyr != NULL)
                          {
                            NetworkLayer(mapLyr, lyr);
                            MapView->InsertMapLayer(mapLyr);
                          }
                      }
                    break;
                };
              lyr = lyr->next;
            }
        }
      RebuildLayerTree();
      if (fullExtent == true)
        SetMapToFullExtent();
      else
        SetMapExtent(minX, minY, maxX, maxY);
    }
  SetLoadedMapConfiguration(map_config);
}

bool MyTableTree::DoVerifyLayer(int type, const char *prefix, const char *name)
{
//
// verifying if a Layer do really exists
//
  char *sql = NULL;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  bool exists = false;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  switch (type)
    {
      case MAP_LAYER_RASTER:
        sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".raster_coverages "
                              "WHERE Upper(coverage_name) = Upper(%Q)", qprefix,
                              name);
        break;
      case MAP_LAYER_WMS:
        sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".wms_getmap "
                              "WHERE Upper(layer_name) = Upper(%Q)", qprefix,
                              name);
        break;
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
      case MAP_LAYER_TOPOLOGY:
      case MAP_LAYER_NETWORK:
        sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".vector_coverages "
                              "WHERE Upper(coverage_name) = Upper(%Q)", qprefix,
                              name);
        break;
    };
  free(qprefix);
  if (sql == NULL)
    return false;
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          if (atoi(results[(i * columns) + 0]) > 0)
            exists = true;
        }
    }
  sqlite3_free_table(results);
  return exists;
}

bool MyTableTree::DoVerifyRasterInternalStyle(const char *prefix,
                                              const char *name)
{
//
// verifying if a Raster Internal Style do really exists
//
  char *sql = NULL;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  bool exists = false;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".SE_raster_styles "
                        "WHERE Upper(style_name) = Upper(%Q)", qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          if (atoi(results[(i * columns) + 0]) > 0)
            exists = true;
        }
    }
  sqlite3_free_table(results);
  return exists;
}

bool MyTableTree::DoVerifyVectorInternalStyle(const char *prefix,
                                              const char *name)
{
//
// verifying if a Vector Internal Style do really exists
//
  char *sql = NULL;
  char *qprefix;
  int ret;
  sqlite3 *sqlite = MainFrame->GetSqlite();
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  bool exists = false;

  if (prefix == NULL)
    qprefix = gaiaDoubleQuotedSql("MAIN");
  else
    qprefix = gaiaDoubleQuotedSql(prefix);
  sql = sqlite3_mprintf("SELECT Count(*) FROM \"%s\".SE_vector_styles "
                        "WHERE Upper(style_name) = Upper(%Q)", qprefix, name);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, &errMsg);
  sqlite3_free(sql);
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
          if (atoi(results[(i * columns) + 0]) > 0)
            exists = true;
        }
    }
  sqlite3_free_table(results);
  return exists;
}

void MyTableTree::DoVerifyMapConfig(const unsigned char *xml, wxString & report)
{
//
// Verifying a Map Configuration
//
  rl2MapLayerPtr lyr;
  rl2MapAttachedDbPtr db_obj;
  MapConfigAttached *db_list = NULL;
  MapConfigAttachedDb *db;

  report =
    wxT
    ("<html><body><h1>Sorry, Map Configuration Verify report is currently unavailable.</h1></body></html>");
  rl2MapConfigPtr map_config = rl2_parse_map_config_xml(xml);
  if (map_config == NULL)
    return;

  // HTML header
  report = wxT("<html><body bgcolor=\"#fffff0\">");

  // preparing all Attached DBs
  db_list = new MapConfigAttached(MainFrame);
  db_obj = map_config->first_db;
  while (db_obj != NULL)
    {
      // loading the ATTACHED DB list
      db_list->Add(db_obj->prefix, db_obj->path);
      db_obj = db_obj->next;
    }
  if (db_list->GetFirst() == NULL)
    goto skip_db;
  db_list->Verify();

  // Attached DBs Table
  report += wxT("<h3>Attached Databases</h3>");
  report += wxT("<table border=\"1\" bgcolor=\"#f0fff0\">");
  report +=
    wxT
    ("<tr><th bgcolor=\"#c0c0c0\">Prefix</th><th bgcolor=\"#c0c0c0\">Prefix remapped as</th><th bgcolor=\"#c0c0c0\">DB Path</th><th bgcolor=\"#c0c0c0\">Valid</th></tr>");
  db = db_list->GetFirst();
  while (db != NULL)
    {
      // verifying ATTACHED DBs
      wxString prefix = wxString::FromUTF8(db->GetPrefix());
      report += wxT("<tr><td>") + prefix + wxT("</td>");
      if (db->GetRemapped() == NULL)
        report += wxT("<td></td>");
      else
        {
          wxString remapped = wxString::FromUTF8(db->GetRemapped());
          report += wxT("<td bgcolor=\"#ffd080\">") + remapped + wxT("</td>");
        }
      wxString path = wxString::FromUTF8(db->GetPath());
      report += wxT("<td>") + path + wxT("</td>");
      if (db->IsValid() == true)
        report += wxT("<td bgcolor=\"#00ff00\" align=\"center\">Ok</td>");
      else
        report +=
          wxT
          ("<td bgcolor=\"#ff0000\">Error: not found or not accessible</td>");
      db = db->GetNext();
    }
  report += wxT("</table><br><hr><br>");

skip_db:
  // Layers Table
  report += wxT("<h3>Layers</h3>");
  report += wxT("<table border=\"1\" bgcolor=\"#f0fff0\">");
  report +=
    wxT
    ("<tr><th bgcolor=\"#c0c0c0\">Type</th><th bgcolor=\"#c0c0c0\">Layer Name</th><th bgcolor=\"#c0c0c0\">Valid</th>");
  report += wxT("<th bgcolor=\"#c0c0c0\">Internal Style</th></tr>");
  lyr = map_config->first_lyr;
  while (lyr != NULL)
    {
      // verifying layers
      report += wxT("<tr>");

      // Layer Type
      wxString type;
      switch (lyr->type)
        {
          case MAP_LAYER_RASTER:
            type = wxT("Raster");
            break;
          case MAP_LAYER_WMS:
            type = wxT("WMS");
            break;
          case MAP_LAYER_VECTOR:
            type = wxT("Vector (Table)");
            break;
          case MAP_LAYER_VECTOR_VIEW:
            type = wxT("Vector (View)");
            break;
          case MAP_LAYER_VECTOR_VIRTUAL:
            type = wxT("Vector (Virtual)");
            break;
          case MAP_LAYER_TOPOLOGY:
            type = wxT("Topology");
            break;
          case MAP_LAYER_NETWORK:
            type = wxT("Network");
            break;
        };
      if (type.Len() == 0)
        report += wxT("<td bgcolor=\"#ff0000\">Unknown</td>");
      else
        report += wxT("<td>") + type + wxT("</td>");
      const char *alias = "main";
      wxString prefix = wxT("main");
      bool remapped = false;
      if (db_list->DoCheckPrefix(lyr->prefix, &alias) == true)
        {
          prefix = wxString::FromUTF8(alias);
          if (strcasecmp(lyr->prefix, alias) != 0)
            remapped = true;
      } else
        {
          prefix = wxString::FromUTF8(lyr->prefix);
          alias = lyr->prefix;
        }
      wxString name = wxString::FromUTF8(lyr->name);
      if (remapped == true)
        report += wxT("<td bgcolor=\"#ffd080\">");
      else
        report += wxT("<td>");
      report += prefix + wxT(".") + name + wxT("</td>");
      if (DoVerifyLayer(lyr->type, alias, lyr->name) == false)
        report += wxT("<td bgcolor=\"#ff0000\">Error: not existing Layer</td>");
      else
        report += wxT("<td bgcolor=\"#00ff00\" align=\"center\">Ok</td>");
      bool ok = true;
      wxString style;
      switch (lyr->type)
        {
          case MAP_LAYER_RASTER:
            if (lyr->raster_style_internal_name != NULL)
              {
                ok =
                  DoVerifyRasterInternalStyle(lyr->prefix,
                                              lyr->raster_style_internal_name);
                style = wxString::FromUTF8(lyr->raster_style_internal_name);
              }
            break;
          case MAP_LAYER_VECTOR:
          case MAP_LAYER_VECTOR_VIEW:
          case MAP_LAYER_VECTOR_VIRTUAL:
            if (lyr->vector_style_internal_name != NULL)
              {
                ok =
                  DoVerifyVectorInternalStyle(lyr->prefix,
                                              lyr->vector_style_internal_name);
                style = wxString::FromUTF8(lyr->vector_style_internal_name);
              }
            break;
          case MAP_LAYER_TOPOLOGY:
            if (lyr->topology_internal_style != NULL)
              {
                ok =
                  DoVerifyVectorInternalStyle(lyr->prefix,
                                              lyr->topology_internal_style->
                                              style_internal_name);
                style =
                  wxString::FromUTF8(lyr->topology_internal_style->
                                     style_internal_name);
              }
            break;
          case MAP_LAYER_NETWORK:
            if (lyr->network_internal_style != NULL)
              {
                ok =
                  DoVerifyVectorInternalStyle(lyr->prefix,
                                              lyr->network_internal_style->
                                              style_internal_name);
                style =
                  wxString::FromUTF8(lyr->network_internal_style->
                                     style_internal_name);
              }
            break;
        };
      if (style.Len() == 0)
        report += wxT("<td></td>");
      else
        {
          if (ok == false)
            report +=
              wxT("<td bgcolor=\"#ff0000\">\"") + style +
              wxT("\": not found, assuming \"default\"</td>");
          else
            report +=
              wxT("<td bgcolor=\"#00ff00\">\"") + style + wxT("\": Ok</td>");
        }
      report += wxT("</tr>");
      lyr = lyr->next;
    }
  report += wxT("</table>");

  // HTML footer
  report += wxT("</body></html>");

  rl2_destroy_map_config(map_config);
  if (db_list != NULL)
    {
      db_list->DetachAll();
      delete db_list;
    }
}

bool MapConfigHtmlDialog::Create(MyFrame * parent, wxString & html)
{
//
// creating the dialog
//
  MainFrame = parent;
  Html = html;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Map Configuration Verify report"))
      == false)
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

void MapConfigHtmlDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxHtmlWindow *htmlWin = new wxHtmlWindow(this, ID_MAPCFG_HTML,
                                           wxDefaultPosition, wxSize(600,
                                                                     300));
  htmlWin->SetPage(Html);
  boxSizer->Add(htmlWin, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// QUIT button
  wxBoxSizer *quitBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(quitBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *quit = new wxButton(this, wxID_OK, wxT("&Quit"));
  quitBox->Add(quit, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// setting up the event handlers
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapConfigHtmlDialog::OnQuit);
  Connect(wxID_ANY, wxEVT_SIZE,
          (wxObjectEventFunction) & MapConfigHtmlDialog::OnSize);
}

void MapConfigHtmlDialog::OnSize(wxSizeEvent & WXUNUSED(event))
{
//
// this window has changed its size
//
  wxSize sz = GetClientSize();
  wxHtmlWindow *htmlWin = (wxHtmlWindow *) FindWindow(ID_MAPCFG_HTML);
  wxButton *quit = (wxButton *) FindWindow(wxID_OK);
  wxSize btnSz = quit->GetSize();
  htmlWin->SetSize(sz.GetWidth() - 6, sz.GetHeight() - 20 - btnSz.GetHeight());
  int x = (sz.GetWidth() - btnSz.GetWidth()) / 2;
  int y = sz.GetHeight() - 6 - btnSz.GetHeight();
  quit->SetSize(x, y, btnSz.GetWidth(), btnSz.GetHeight());
}

void MapConfigHtmlDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

MapConfigAttachedDb::MapConfigAttachedDb(const char *prefix, const char *path)
{
// ctor
  int len;
  if (prefix == NULL)
    prefix = "MAIN";
  len = strlen(prefix);
  Prefix = (char *) malloc(len + 1);
  strcpy(Prefix, prefix);
  Remapped = NULL;
  len = strlen(path);
  Path = (char *) malloc(len + 1);
  strcpy(Path, path);
  Valid = false;
  DontDetach = true;
  Next = NULL;
}

MapConfigAttachedDb::~MapConfigAttachedDb()
{
// dtor
  if (Prefix != NULL)
    free(Prefix);
  if (Remapped != NULL)
    free(Remapped);
  if (Path != NULL)
    free(Path);
}

bool MapConfigAttachedDb::IsAlreadyAttached(sqlite3 * sqlite,
                                            const char *prefix,
                                            const char *path)
{
//
// testing for an already attached DB
//
  int ret;
  const char *sql;
  int i;
  char **results;
  int rows;
  int columns;
  bool found = false;

  sql = "PRAGMA database_list";
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return false;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *db_prefix = results[(i * columns) + 1];
          const char *db_path = results[(i * columns) + 2];
          if (strcasecmp(db_prefix, prefix) == 0
              && strcasecmp(db_path, path) == 0)
            found = true;
        }
    }
  sqlite3_free_table(results);
  return found;
}

bool MapConfigAttachedDb::NotAlreadyAttached(sqlite3 * sqlite,
                                             const char *alias)
{
//
// testing for an unused DB Prefix
//
  int ret;
  const char *sql;
  int i;
  char **results;
  int rows;
  int columns;
  bool not_used = true;

  sql = "PRAGMA database_list";
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return true;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *db_prefix = results[(i * columns) + 1];
          if (strcasecmp(db_prefix, alias) == 0)
            not_used = false;
        }
    }
  sqlite3_free_table(results);
  return not_used;
}

bool MapConfigAttachedDb::CheckValidDbFile(const char *path)
{
//
// testing if the given DB-file do really exists and is accessible
//
  int ret;
  sqlite3 *sqlite;
  ret = sqlite3_open_v2(path, &sqlite, SQLITE_OPEN_READWRITE, NULL);
  if (ret)
    return false;
  sqlite3_close(sqlite);
  return true;
}

void MapConfigAttachedDb::Verify(sqlite3 * sqlite, MyFrame * parent)
{
//
// verifying an ATTACHed DB
//
  char *sql;
  int ret;
  char *errMsg = NULL;
  char *qprefix;
  char *qpath;
  char *prefix;
  char alias[64];

  if (IsAlreadyAttached(sqlite, Prefix, Path) == true)
    {
      // already attached
      Valid = true;
      DontDetach = true;
      return;
    }
  if (CheckValidDbFile(Path) == false)
    {
      // not existing (or not accessible) DB-file
      Valid = false;
      DontDetach = true;
      return;
    }

  if (NotAlreadyAttached(sqlite, Prefix) == true)
    prefix = Prefix;
  else
    {
      int idx = 0;
      while (1)
        {
          // searching an Alias Prefix
          sprintf(alias, "alias_#%d", ++idx);
          if (NotAlreadyAttached(sqlite, alias) == true)
            {
              prefix = alias;
              break;
            }
        }
    }
  qpath = gaiaDoubleQuotedSql(Path);
  qprefix = gaiaDoubleQuotedSql(prefix);
  sql = sqlite3_mprintf("ATTACH DATABASE \"%s\" AS \"%s\"", qpath, qprefix);
  free(qpath);
  free(qprefix);
  ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, parent);
      sqlite3_free(errMsg);
      Valid = false;
      return;
    }
  Valid = true;
  DontDetach = false;
  if (Remapped != NULL)
    free(Remapped);
  Remapped = NULL;
  if (strcasecmp(prefix, Prefix) != 0)
    {
      int len = strlen(prefix);
      Remapped = (char *) malloc(len + 1);
      strcpy(Remapped, prefix);
    }
}

MapConfigAttached::MapConfigAttached(MyFrame * parent)
{
// ctor
  Parent = parent;
  First = NULL;
  Last = NULL;
}

MapConfigAttached::~MapConfigAttached()
{
// dtor
  MapConfigAttachedDb *db;
  MapConfigAttachedDb *n_db;
  db = First;
  while (db != NULL)
    {
      n_db = db->GetNext();
      delete db;
      db = n_db;
    }
}

void MapConfigAttached::Add(const char *prefix, const char *path)
{
//
// inserting a further ATTACHed DB into the lists
//
  MapConfigAttachedDb *db = new MapConfigAttachedDb(prefix, path);
  if (First == NULL)
    First = db;
  if (Last != NULL)
    Last->SetNext(db);
  Last = db;
}

void MapConfigAttached::Verify()
{
//
// verifying all ATTACHed DBs
//
  MapConfigAttachedDb *db = First;
  while (db != NULL)
    {
      db->Verify(Parent->GetSqlite(), Parent);
      db = db->GetNext();
    }
}

void MapConfigAttached::DetachAll()
{
//
// Detaching all ATTACHed DBs
//
  int ret;
  char *errMsg = NULL;
  char *qprefix;
  char *sql;

  if (Parent->GetSqlite() == NULL)
    return;

  MapConfigAttachedDb *db = First;
  while (db != NULL)
    {
      if (db->IsDontDetach() == true)
        {
          db = db->GetNext();
          continue;
        }
      // detaching a DB
      if (db->GetRemapped() != NULL)
        qprefix = gaiaDoubleQuotedSql(db->GetRemapped());
      else
        qprefix = gaiaDoubleQuotedSql(db->GetPrefix());
      sql = sqlite3_mprintf("DETACH DATABASE \"%s\"", qprefix);
      free(qprefix);
      ret = sqlite3_exec(Parent->GetSqlite(), sql, NULL, NULL, &errMsg);
      sqlite3_free(sql);
      if (ret != SQLITE_OK)
        {
          wxMessageBox(wxT("SQLite SQL error: ") + wxString::FromUTF8(errMsg),
                       wxT("spatialite_gui"), wxOK | wxICON_ERROR, Parent);
          sqlite3_free(errMsg);
        }
      db = db->GetNext();
    }
}

bool MapConfigAttached::DoCheckPrefix(const char *prefix, const char **alias)
{
//
// checking if some DB Prefix is valid (may be by adopting an alias name)
//
  MapConfigAttachedDb *db;
  *alias = NULL;

  if (strcasecmp(prefix, "main") == 0)
    {
      // MAIN db - never requires remapping 
      *alias = prefix;
      return true;
    }

  db = First;
  while (db != NULL)
    {
      if (strcasecmp(db->GetPrefix(), prefix) == 0)
        {
          if (db->IsValid() == false)
            return false;
          if (db->GetRemapped() == NULL)
            *alias = db->GetPrefix();
          else
            *alias = db->GetRemapped();
          return true;
        }
      db = db->GetNext();
    }
  return false;
}
