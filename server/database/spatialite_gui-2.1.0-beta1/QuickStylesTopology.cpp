/*
/ QuickStylesTopology.cpp
/ Quick Styles wizards (Topology layers)
/
/ version 2.0, 2017 May 26
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

#include "wx/spinctrl.h"
#include "wx/imaglist.h"
#include "wx/colordlg.h"
#include "wx/filename.h"
#include "wx/clipbrd.h"

QuickStyleTopologyObj::QuickStyleTopologyObj(int type)
{
// ctor
  QuickStyleObj::DoGetUUID(UUID);
  Type = type;
  MinScaleEnabled = false;
  MaxScaleEnabled = false;
  ScaleMin = 0.0;
  ScaleMax = 0.0;
  NodeOpacity = 1.0;
  NodeSize = 16;
  NodeRotation = 0.0;
  NodeAnchorX = 0.5;
  NodeAnchorY = 0.5;
  NodeDisplacementX = 0.0;
  NodeDisplacementY = 0.0;
  NodeWellKnownMark = QuickStyleObj::RandomWellKnownMark();
  QuickStyleObj::RandomColor(NodeFillColor);
  QuickStyleObj::RandomColor(NodeStrokeColor);
  EdgeLinkOpacity = 1.0;
  EdgeLinkPerpendicularOffset = 0.0;
  EdgeLinkStrokeWidth = 1.0;
  QuickStyleObj::RandomColor(EdgeLinkStrokeColor);
  EdgeLinkDotStyle = QUICK_STYLE_SOLID_LINE;
  FaceFill = true;
  FaceStroke = true;
  FaceDisplacementX = 0.0;
  FaceDisplacementY = 0.0;
  FacePerpendicularOffset = 0.0;
  FaceFillOpacity = 1.0;
  QuickStyleObj::RandomColor(FaceFillColor);
  FaceStrokeOpacity = 1.0;
  FaceStrokeWidth = 1.0;
  QuickStyleObj::RandomColor(FaceStrokeColor);
  EdgeLinkSeedOpacity = 1.0;
  EdgeLinkSeedSize = 16;
  EdgeLinkSeedRotation = 0.0;
  EdgeLinkSeedAnchorX = 0.5;
  EdgeLinkSeedAnchorY = 0.5;
  EdgeLinkSeedDisplacementX = 0.0;
  EdgeLinkSeedDisplacementY = 0.0;
  EdgeLinkSeedWellKnownMark = QuickStyleObj::RandomWellKnownMark();
  QuickStyleObj::RandomColor(EdgeLinkSeedFillColor);
  QuickStyleObj::RandomColor(EdgeLinkSeedStrokeColor);
  FaceSeedOpacity = 1.0;
  FaceSeedSize = 16;
  FaceSeedRotation = 0.0;
  FaceSeedAnchorX = 0.5;
  FaceSeedAnchorY = 0.5;
  FaceSeedDisplacementX = 0.0;
  FaceSeedDisplacementY = 0.0;
  FaceSeedWellKnownMark = QuickStyleObj::RandomWellKnownMark();
  QuickStyleObj::RandomColor(FaceSeedFillColor);
  QuickStyleObj::RandomColor(FaceSeedStrokeColor);
  XmlStyle = NULL;
}

QuickStyleTopologyObj *QuickStyleTopologyObj::Clone()
{
//
// cloning a Quick Style Topology
//
  QuickStyleTopologyObj *Style = new QuickStyleTopologyObj(this->Type);
  strcpy(Style->UUID, this->UUID);
  Style->MinScaleEnabled = this->MinScaleEnabled;
  Style->MaxScaleEnabled = this->MaxScaleEnabled;
  Style->ScaleMin = this->ScaleMin;
  Style->ScaleMax = this->ScaleMax;
  Style->NodeOpacity = this->NodeOpacity;
  Style->NodeSize = this->NodeSize;
  Style->NodeRotation = this->NodeRotation;
  Style->NodeAnchorX = this->NodeAnchorX;
  Style->NodeAnchorY = this->NodeAnchorY;
  Style->NodeDisplacementX = this->NodeDisplacementX;
  Style->NodeDisplacementY = this->NodeDisplacementY;
  Style->NodeWellKnownMark = this->NodeWellKnownMark;
  strcpy(Style->NodeFillColor, this->NodeFillColor);
  strcpy(Style->NodeStrokeColor, this->NodeStrokeColor);
  Style->EdgeLinkOpacity = this->EdgeLinkOpacity;
  Style->EdgeLinkPerpendicularOffset = this->EdgeLinkPerpendicularOffset;
  Style->EdgeLinkStrokeWidth = this->EdgeLinkStrokeWidth;
  strcpy(Style->EdgeLinkStrokeColor, this->EdgeLinkStrokeColor);
  Style->EdgeLinkDotStyle = this->EdgeLinkDotStyle;
  Style->FaceFill = this->FaceFill;
  Style->FaceStroke = this->FaceStroke;
  Style->FaceDisplacementX = this->FaceDisplacementX;
  Style->FaceDisplacementY = this->FaceDisplacementY;
  Style->FacePerpendicularOffset = this->FacePerpendicularOffset;
  Style->FaceFillOpacity = this->FaceFillOpacity;
  strcpy(Style->FaceFillColor, this->FaceFillColor);
  Style->FaceStrokeOpacity = this->FaceStrokeOpacity;
  Style->FaceStrokeWidth = this->FaceStrokeWidth;
  strcpy(Style->FaceStrokeColor, this->FaceStrokeColor);
  Style->EdgeLinkSeedOpacity = this->EdgeLinkSeedOpacity;
  Style->EdgeLinkSeedSize = this->EdgeLinkSeedSize;
  Style->EdgeLinkSeedRotation = this->EdgeLinkSeedRotation;
  Style->EdgeLinkSeedAnchorX = this->EdgeLinkSeedAnchorX;
  Style->EdgeLinkSeedAnchorY = this->EdgeLinkSeedAnchorY;
  Style->EdgeLinkSeedDisplacementX = this->EdgeLinkSeedDisplacementX;
  Style->EdgeLinkSeedDisplacementY = this->EdgeLinkSeedDisplacementY;
  Style->EdgeLinkSeedWellKnownMark = this->EdgeLinkSeedWellKnownMark;
  strcpy(Style->EdgeLinkSeedFillColor, this->EdgeLinkSeedFillColor);
  strcpy(Style->EdgeLinkSeedStrokeColor, this->EdgeLinkSeedStrokeColor);
  Style->FaceSeedOpacity = this->FaceSeedOpacity;
  Style->FaceSeedSize = this->FaceSeedSize;
  Style->FaceSeedRotation = this->FaceSeedRotation;
  Style->FaceSeedAnchorX = this->FaceSeedAnchorX;
  Style->FaceSeedAnchorY = this->FaceSeedAnchorY;
  Style->FaceSeedDisplacementX = this->FaceSeedDisplacementX;
  Style->FaceSeedDisplacementY = this->FaceSeedDisplacementY;
  Style->FaceSeedWellKnownMark = this->FaceSeedWellKnownMark;
  strcpy(Style->FaceSeedFillColor, this->FaceSeedFillColor);
  strcpy(Style->FaceSeedStrokeColor, this->FaceSeedStrokeColor);
  Style->XmlStyle = NULL;
  return Style;
}

bool QuickStyleTopologyObj::Compare(QuickStyleTopologyObj * Style)
{
//
// comparing two Quick Style objects (Topology)
//
  if (strcmp(Style->UUID, this->UUID) != 0)
    return false;
  if (Style->MinScaleEnabled != this->MinScaleEnabled)
    return false;
  if (Style->MaxScaleEnabled != this->MaxScaleEnabled)
    return false;
  if (Style->ScaleMin != this->ScaleMin)
    return false;
  if (Style->ScaleMax != this->ScaleMax)
    return false;
  if (Style->NodeOpacity != this->NodeOpacity)
    return false;
  if (Style->NodeSize != this->NodeSize)
    return false;
  if (Style->NodeRotation != this->NodeRotation)
    return false;
  if (Style->NodeAnchorX != this->NodeAnchorX)
    return false;
  if (Style->NodeAnchorY != this->NodeAnchorY)
    return false;
  if (Style->NodeDisplacementX != this->NodeDisplacementX)
    return false;
  if (Style->NodeDisplacementY != this->NodeDisplacementY)
    return false;
  if (Style->NodeWellKnownMark != this->NodeWellKnownMark)
    return false;
  if (strcmp(Style->NodeFillColor, this->NodeFillColor) != 0)
    return false;
  if (strcmp(Style->NodeStrokeColor, this->NodeStrokeColor) != 0)
    return false;
  if (Style->EdgeLinkOpacity != this->EdgeLinkOpacity)
    return false;
  if (Style->EdgeLinkPerpendicularOffset != this->EdgeLinkPerpendicularOffset)
    return false;
  if (Style->EdgeLinkStrokeWidth != this->EdgeLinkStrokeWidth)
    return false;
  if (strcmp(Style->EdgeLinkStrokeColor, this->EdgeLinkStrokeColor) != 0)
    return false;
  if (Style->EdgeLinkDotStyle != this->EdgeLinkDotStyle)
    return false;
  if (Style->FaceFill != this->FaceFill)
    return false;
  if (Style->FaceStroke != this->FaceStroke)
    return false;
  if (Style->FaceDisplacementX != this->FaceDisplacementX)
    return false;
  if (Style->FaceDisplacementY != this->FaceDisplacementY)
    return false;
  if (Style->FacePerpendicularOffset != this->FacePerpendicularOffset)
    return false;
  if (Style->FaceFillOpacity != this->FaceFillOpacity)
    return false;
  if (strcmp(Style->FaceFillColor, this->FaceFillColor) != 0)
    return false;
  if (Style->FaceStrokeOpacity != this->FaceStrokeOpacity)
    return false;
  if (Style->FaceStrokeWidth != this->FaceStrokeWidth)
    return false;
  if (strcmp(Style->FaceStrokeColor, this->FaceStrokeColor) != 0)
    return false;
  if (Style->EdgeLinkSeedOpacity != this->EdgeLinkSeedOpacity)
    return false;
  if (Style->EdgeLinkSeedSize != this->EdgeLinkSeedSize)
    return false;
  if (Style->EdgeLinkSeedRotation != this->EdgeLinkSeedRotation)
    return false;
  if (Style->EdgeLinkSeedAnchorX != this->EdgeLinkSeedAnchorX)
    return false;
  if (Style->EdgeLinkSeedAnchorY != this->EdgeLinkSeedAnchorY)
    return false;
  if (Style->EdgeLinkSeedDisplacementX != this->EdgeLinkSeedDisplacementX)
    return false;
  if (Style->EdgeLinkSeedDisplacementY != this->EdgeLinkSeedDisplacementY)
    return false;
  if (Style->EdgeLinkSeedWellKnownMark != this->EdgeLinkSeedWellKnownMark)
    return false;
  if (strcmp(Style->EdgeLinkSeedFillColor, this->EdgeLinkSeedFillColor) != 0)
    return false;
  if (strcmp(Style->EdgeLinkSeedStrokeColor, this->EdgeLinkSeedStrokeColor) !=
      0)
    return false;
  if (Style->FaceSeedOpacity != this->FaceSeedOpacity)
    return false;
  if (Style->FaceSeedSize != this->FaceSeedSize)
    return false;
  if (Style->FaceSeedRotation != this->FaceSeedRotation)
    return false;
  if (Style->FaceSeedAnchorX != this->FaceSeedAnchorX)
    return false;
  if (Style->FaceSeedAnchorY != this->FaceSeedAnchorY)
    return false;
  if (Style->FaceSeedDisplacementX != this->FaceSeedDisplacementX)
    return false;
  if (Style->FaceSeedDisplacementY != this->FaceSeedDisplacementY)
    return false;
  if (Style->FaceSeedWellKnownMark != this->FaceSeedWellKnownMark)
    return false;
  if (strcmp(Style->FaceSeedFillColor, this->FaceSeedFillColor) != 0)
    return false;
  if (strcmp(Style->FaceSeedStrokeColor, this->FaceSeedStrokeColor) != 0)
    return false;
  return true;
}

void QuickStyleTopologyObj::UpdateXmlStyle()
{
//
// updating the XML Style
//
  if (XmlStyle != NULL)
    sqlite3_free(XmlStyle);
  XmlStyle = CreateXmlStyle();
}

char *QuickStyleTopologyObj::CreateXmlStyle()
{
//
// creating the XML Style
//
  char *prev;
  char *xml2;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<FeatureTypeStyle version=\"1.1.0\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxsi:schemaLocation=\"http://www.opengis.net/se http://schemas.opengis.net/se/1.1.0/FeatureStyle.xsd\" ",
     prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf
    ("%sxmlns=\"http://www.opengis.net/se\" xmlns:ogc=\"http://www.opengis.net/ogc\" ",
     prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, UUID);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (Type == MAP_LAYER_NETWORK)
    xml =
      sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev,
                      "Quick Style - Network");
  else
    xml =
      sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev,
                      "Quick Style - Topology");
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev,
                    "Created by SpatialiteGUI");
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (Type == MAP_LAYER_NETWORK)
    {
      const char *indent = "\t";
      // Links
      xml2 = DoCreateEdgeLinkXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      // Nodes
      xml2 = DoCreateNodeXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      // LinkSeeds
      xml2 = DoCreateEdgeLinkSeedXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
  } else
    {
      const char *indent = "\t";
      // Faces
      xml2 = DoCreateFaceXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      // Edges
      xml2 = DoCreateEdgeLinkXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      // Nodes
      xml2 = DoCreateNodeXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      // EdgeSeeds
      xml2 = DoCreateEdgeLinkSeedXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      // FaceSeeds
      xml2 = DoCreateFaceSeedXML(indent, "ogc:");
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
    }
  xml = sqlite3_mprintf("%s</FeatureTypeStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateFaceXML(const char *indent,
                                             const char *prefix)
{
//
// creating the XML Style - Face
//
  char *prev;
  char extra[128];
  char *xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
  sprintf(extra, "%s\t", indent);
  prev = xml;
  if (MinScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         indent, ScaleMin);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         indent, ScaleMax);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t<Name>Face</Name>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<ogc:Filter>\r\n%s\t\t<ogc:PropertyIsEqualTo>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<ogc:PropertyName>topoclass</ogc:PropertyName>\r\n", prev,
     indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>face</ogc:Literal>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t</ogc:PropertyIsEqualTo>\r\n%s\t</ogc:Filter>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s<PolygonSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  if (FaceFill == true)
    {
      // Polygon Fill
      xml = sqlite3_mprintf("%s%s\t<Fill>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      // using a Solid Color
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev,
         extra, FaceFillColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, extra, FaceFillOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t</Fill>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceStroke == true)
    {
      // Polygon Stroke
      xml = sqlite3_mprintf("%s%s\t<Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      // using a Solid Color
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
         extra, FaceStrokeColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, extra, FaceStrokeOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, extra, FaceStrokeWidth);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
         prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
         prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t</Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceDisplacementX != 0.0 || FaceDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, extra, FaceDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, extra, FaceDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FacePerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n", prev,
         extra, FacePerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s</PolygonSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateFaceXML(const char *indent)
{
//
// creating the XML Style - Face
//
  char *prev;
  char *xml = sqlite3_mprintf("%s<FacesStyle>\r\n", indent);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<PolygonSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (FaceFill == true)
    {
      // Polygon Fill
      xml = sqlite3_mprintf("%s%s\t\t<Fill>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      // using a Solid Color
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev,
         indent, FaceFillColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, indent, FaceFillOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</Fill>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceStroke == true)
    {
      // Polygon Stroke
      xml = sqlite3_mprintf("%s%s\t\t<Stroke>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      // using a Solid Color
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
         indent, FaceStrokeColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, indent, FaceStrokeOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, indent, FaceStrokeWidth);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
         prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
         prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</Stroke>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceDisplacementX != 0.0 || FaceDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t<Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, indent, FaceDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, indent, FaceDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FacePerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n", prev,
         indent, FacePerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t</PolygonSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</FacesStyle>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateEdgeLinkXML(const char *indent,
                                                 const char *prefix)
{
//
// creating the XML Style - Edge or Link
//
  char *prev;
  char extra[128];
  char *xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
  sprintf(extra, "%s\t", indent);
  prev = xml;
  if (MinScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         indent, ScaleMin);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         indent, ScaleMax);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Type == MAP_LAYER_NETWORK)
    xml = sqlite3_mprintf("%s%s\t<Name>Link</Name>\r\n", prev, indent);
  else
    xml = sqlite3_mprintf("%s%s\t<Name>Edge</Name>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<ogc:Filter>\r\n%s\t\t<ogc:PropertyIsEqualTo>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<ogc:PropertyName>topoclass</ogc:PropertyName>\r\n", prev,
     indent);
  sqlite3_free(prev);
  prev = xml;
  if (Type == MAP_LAYER_NETWORK)
    xml =
      sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>link</ogc:Literal>\r\n",
                      prev, indent);
  else
    xml =
      sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>edge</ogc:Literal>\r\n",
                      prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t</ogc:PropertyIsEqualTo>\r\n%s\t</ogc:Filter>\r\n", prev, indent,
     indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s<LineSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// using a Solid Color
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev, extra,
     EdgeLinkStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
     prev, extra, EdgeLinkOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, EdgeLinkStrokeWidth);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  const char *dashArray;
  switch (EdgeLinkDotStyle)
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
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
         prev, extra, dashArray);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t</Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  if (EdgeLinkPerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n", prev,
         extra, EdgeLinkPerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s</LineSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateEdgeLinkXML(const char *indent)
{
//
// creating the XML Style - Edge or Link
//
  char *prev;
  char *xml;
  if (Type == MAP_LAYER_NETWORK)
    xml = sqlite3_mprintf("%s<LinksStyle>\r\n", indent);
  else
    xml = sqlite3_mprintf("%s<EdgesStyle>\r\n", indent);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<LineSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// using a Solid Color
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
     indent, EdgeLinkStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
     prev, indent, EdgeLinkOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, indent, EdgeLinkStrokeWidth);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  const char *dashArray;
  switch (EdgeLinkDotStyle)
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
      xml =
        sqlite3_mprintf
        ("%s%s\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
         prev, indent, dashArray);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t\t</Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (EdgeLinkPerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n", prev,
         indent, EdgeLinkPerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t</LineSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (Type == MAP_LAYER_NETWORK)
    xml = sqlite3_mprintf("%s%s</LinksStyle>\r\n", prev, indent);
  else
    xml = sqlite3_mprintf("%s%s</EdgesStyle>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateNodeXML(const char *indent,
                                             const char *prefix)
{
//
// creating the XML Style - Node
//
  const char *cstr;
  char *prev;
  char extra[128];
  char *xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
  sprintf(extra, "%s\t", indent);
  prev = xml;
  if (MinScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         indent, ScaleMin);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         indent, ScaleMax);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t<Name>Node</Name>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<ogc:Filter>\r\n%s\t\t<ogc:PropertyIsEqualTo>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<ogc:PropertyName>topoclass</ogc:PropertyName>\r\n", prev,
     indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>node</ogc:Literal>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t</ogc:PropertyIsEqualTo>\r\n%s\t</ogc:Filter>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s<PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t<Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  switch (NodeWellKnownMark)
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
  xml =
    sqlite3_mprintf("%s%s\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                    extra, cstr);
  sqlite3_free(prev);
  prev = xml;
// Mark Fill
  xml = sqlite3_mprintf("%s%s\t\t\t<Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, extra, NodeFillColor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// Mark Stroke
  xml = sqlite3_mprintf("%s%s\t\t\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, extra, NodeStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  if (NodeOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Opacity>%1.2f</Opacity>\r\n", prev, extra,
                        NodeOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t<Size>%1.2f</Size>\r\n", prev, extra, NodeSize);
  sqlite3_free(prev);
  prev = xml;
  if (NodeRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Rotation>%1.2f</Rotation>\r\n", prev, extra,
                        NodeRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (NodeAnchorX != 0.5 || NodeAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t<AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, extra, NodeAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, extra, NodeAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (NodeDisplacementX != 0.0 || NodeDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, extra, NodeDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, extra, NodeDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t</Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateNodeXML(const char *indent)
{
//
// creating the XML Style - Node
//
  const char *cstr;
  char *prev;
  char *xml = sqlite3_mprintf("%s<NodesStyle>\r\n", indent);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<PointSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<Graphic>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t\t<Mark>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  switch (NodeWellKnownMark)
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
  xml =
    sqlite3_mprintf("%s%s\t\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                    indent, cstr);
  sqlite3_free(prev);
  prev = xml;
// Mark Fill
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Fill>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, indent, NodeFillColor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Fill>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// Mark Stroke
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, indent, NodeStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, indent, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Mark>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (NodeOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, indent,
                        NodeOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Size>%1.2f</Size>\r\n", prev, indent, NodeSize);
  sqlite3_free(prev);
  prev = xml;
  if (NodeRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev,
                        indent, NodeRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (NodeAnchorX != 0.5 || NodeAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t<AnchorPoint>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, indent, NodeAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, indent, NodeAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</AnchorPoint>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  if (NodeDisplacementX != 0.0 || NodeDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t<Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, indent, NodeDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, indent, NodeDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t\t</Graphic>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</PointSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</NodesStyle>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateEdgeLinkSeedXML(const char *indent,
                                                     const char *prefix)
{
//
// creating the XML Style - Edge or Link Seed
//
  const char *cstr;
  char *prev;
  char extra[128];
  char *xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
  sprintf(extra, "%s\t", indent);
  prev = xml;
  if (MinScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         indent, ScaleMin);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         indent, ScaleMax);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Type == MAP_LAYER_NETWORK)
    xml = sqlite3_mprintf("%s%s\t<Name>LinkSeed</Name>\r\n", prev, indent);
  else
    xml = sqlite3_mprintf("%s%s\t<Name>EdgeSeed</Name>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<ogc:Filter>\r\n%s\t\t<ogc:PropertyIsEqualTo>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<ogc:PropertyName>topoclass</ogc:PropertyName>\r\n", prev,
     indent);
  sqlite3_free(prev);
  prev = xml;
  if (Type == MAP_LAYER_NETWORK)
    xml =
      sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>link_seed</ogc:Literal>\r\n",
                      prev, indent);
  else
    xml =
      sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>edge_seed</ogc:Literal>\r\n",
                      prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t</ogc:PropertyIsEqualTo>\r\n%s\t</ogc:Filter>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s<PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t<Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  switch (EdgeLinkSeedWellKnownMark)
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
  xml =
    sqlite3_mprintf("%s%s\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                    extra, cstr);
  sqlite3_free(prev);
  prev = xml;
// Mark Fill
  xml = sqlite3_mprintf("%s%s\t\t\t<Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, extra, EdgeLinkSeedFillColor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// Mark Stroke
  xml = sqlite3_mprintf("%s%s\t\t\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, extra, EdgeLinkSeedStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  if (EdgeLinkSeedOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Opacity>%1.2f</Opacity>\r\n", prev, extra,
                        EdgeLinkSeedOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t<Size>%1.2f</Size>\r\n", prev, extra,
                    EdgeLinkSeedSize);
  sqlite3_free(prev);
  prev = xml;
  if (EdgeLinkSeedRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Rotation>%1.2f</Rotation>\r\n", prev, extra,
                        EdgeLinkSeedRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EdgeLinkSeedAnchorX != 0.5 || EdgeLinkSeedAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t<AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, extra, EdgeLinkSeedAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, extra, EdgeLinkSeedAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EdgeLinkSeedDisplacementX != 0.0 || EdgeLinkSeedDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, extra, EdgeLinkSeedDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, extra, EdgeLinkSeedDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t</Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateEdgeLinkSeedXML(const char *indent)
{
//
// creating the XML Style - Edge or Link Seed
//
  const char *cstr;
  char *prev;
  char *xml;
  if (Type == MAP_LAYER_NETWORK)
    xml = sqlite3_mprintf("%s<LinkSeedsStyle>\r\n", indent);
  else
    xml = sqlite3_mprintf("%s<EdgeSeedsStyle>\r\n", indent);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<PointSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<Graphic>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t\t<Mark>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  switch (EdgeLinkSeedWellKnownMark)
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
  xml =
    sqlite3_mprintf("%s%s\t\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                    indent, cstr);
  sqlite3_free(prev);
  prev = xml;
// Mark Fill
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Fill>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, indent, EdgeLinkSeedFillColor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Fill>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// Mark Stroke
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, indent, EdgeLinkSeedStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, indent, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Mark>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (EdgeLinkSeedOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, indent,
                        EdgeLinkSeedOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Size>%1.2f</Size>\r\n", prev, indent,
                    EdgeLinkSeedSize);
  sqlite3_free(prev);
  prev = xml;
  if (EdgeLinkSeedRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev,
                        indent, EdgeLinkSeedRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EdgeLinkSeedAnchorX != 0.5 || EdgeLinkSeedAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t<AnchorPoint>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, indent, EdgeLinkSeedAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, indent, EdgeLinkSeedAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</AnchorPoint>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EdgeLinkSeedDisplacementX != 0.0 || EdgeLinkSeedDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t<Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, indent, EdgeLinkSeedDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, indent, EdgeLinkSeedDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t\t</Graphic>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</PointSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (Type == MAP_LAYER_NETWORK)
    xml = sqlite3_mprintf("%s%s</LinkSeedsStyle>\r\n", prev, indent);
  else
    xml = sqlite3_mprintf("%s%s</EdgeSeedsStyle>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateFaceSeedXML(const char *indent,
                                                 const char *prefix)
{
//
// creating the XML Style - FaceSeed
//
  const char *cstr;
  char *prev;
  char extra[128];
  char *xml = sqlite3_mprintf("%s<Rule>\r\n", indent);
  sprintf(extra, "%s\t", indent);
  prev = xml;
  if (MinScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         indent, ScaleMin);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         indent, ScaleMax);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t<Name>FaceSeed</Name>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<ogc:Filter>\r\n%s\t\t<ogc:PropertyIsEqualTo>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t<ogc:PropertyName>topoclass</ogc:PropertyName>\r\n", prev,
     indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<ogc:Literal>face_seed</ogc:Literal>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t</ogc:PropertyIsEqualTo>\r\n%s\t</ogc:Filter>\r\n",
                    prev, indent, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s<PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t<Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  switch (FaceSeedWellKnownMark)
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
  xml =
    sqlite3_mprintf("%s%s\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                    extra, cstr);
  sqlite3_free(prev);
  prev = xml;
// Mark Fill
  xml = sqlite3_mprintf("%s%s\t\t\t<Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, extra, FaceSeedFillColor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Fill>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// Mark Stroke
  xml = sqlite3_mprintf("%s%s\t\t\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, extra, FaceSeedStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  if (FaceSeedOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Opacity>%1.2f</Opacity>\r\n", prev, extra,
                        FaceSeedOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t<Size>%1.2f</Size>\r\n", prev, extra,
                    FaceSeedSize);
  sqlite3_free(prev);
  prev = xml;
  if (FaceSeedRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Rotation>%1.2f</Rotation>\r\n", prev, extra,
                        FaceSeedRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceSeedAnchorX != 0.5 || FaceSeedAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t<AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, extra, FaceSeedAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, extra, FaceSeedAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceSeedDisplacementX != 0.0 || FaceSeedDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, extra, FaceSeedDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, extra, FaceSeedDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t</Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</PointSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</Rule>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleTopologyObj::DoCreateFaceSeedXML(const char *indent)
{
//
// creating the XML Style - FaceSeed
//
  const char *cstr;
  char *prev;
  char *xml = sqlite3_mprintf("%s<FaceSeedsStyle>\r\n", indent);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t<PointSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<Graphic>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t\t<Mark>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  switch (FaceSeedWellKnownMark)
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
  xml =
    sqlite3_mprintf("%s%s\t\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                    indent, cstr);
  sqlite3_free(prev);
  prev = xml;
// Mark Fill
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Fill>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
     prev, indent, FaceSeedFillColor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Fill>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
// Mark Stroke
  xml = sqlite3_mprintf("%s%s\t\t\t\t<Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
     prev, indent, FaceSeedStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, indent, 1.0);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
     prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t\t</Stroke>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Mark>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  if (FaceSeedOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, indent,
                        FaceSeedOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Size>%1.2f</Size>\r\n", prev, indent,
                    FaceSeedSize);
  sqlite3_free(prev);
  prev = xml;
  if (FaceSeedRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev,
                        indent, FaceSeedRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceSeedAnchorX != 0.5 || FaceSeedAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t<AnchorPoint>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, indent, FaceSeedAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, indent, FaceSeedAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</AnchorPoint>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  if (FaceSeedDisplacementX != 0.0 || FaceSeedDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t\t<Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, indent, FaceSeedDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, indent, FaceSeedDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t\t</Displacement>\r\n", prev, indent);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s\t\t</Graphic>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</PointSymbolizer>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</FaceSeedsStyle>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

unsigned char *QuickStyleTopologyObj::CloneXmlStyle()
{
//
// cloning the XML Style definition
//
  if (XmlStyle == NULL)
    XmlStyle = CreateXmlStyle();
  if (XmlStyle == NULL)
    return NULL;
  int len = strlen(XmlStyle);
  char *cloned = (char *) malloc(len + 1);
  strcpy(cloned, XmlStyle);
  return (unsigned char *) cloned;
}

bool QuickStyleTopologyDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  Type = Layer->GetType();
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsConfigChanged = false;

  wxString title = wxT("QuickStyle (Topology) Edit");
  if (Type == MAP_LAYER_NETWORK)
    title = wxT("QuickStyle (Network) Edit");
  if (wxPropertySheetDialog::Create(parent, wxID_ANY, title) == false)
    return false;
  if (Layer->GetQuickStyleTopology() != NULL)
    Style = Layer->CloneQuickStyleTopology();
  else
    Style = new QuickStyleTopologyObj(Type);

  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  if (Type == MAP_LAYER_NETWORK)
    {
      wxPanel *nodePage = CreateNodePage(book);
      book->AddPage(nodePage, wxT("Nodes"), false);
      wxPanel *edgeLinkPage = CreateEdgeLinkPage(book);
      book->AddPage(edgeLinkPage, wxT("Links"), false);
      wxPanel *edgeLinkSeedPage = CreateEdgeLinkSeedPage(book);
      book->AddPage(edgeLinkSeedPage, wxT("Link Seeds"), false);
  } else
    {
      wxPanel *nodePage = CreateNodePage(book);
      book->AddPage(nodePage, wxT("Nodes"), false);
      wxPanel *edgeLinkPage = CreateEdgeLinkPage(book);
      book->AddPage(edgeLinkPage, wxT("Edges"), false);
      wxPanel *facePage = CreateFacePage(book);
      book->AddPage(facePage, wxT("Faces"), false);
      wxPanel *edgeLinkSeedPage = CreateEdgeLinkSeedPage(book);
      book->AddPage(edgeLinkSeedPage, wxT("Edge Seeds"), false);
      wxPanel *faceSeedPage = CreateFaceSeedPage(book);
      book->AddPage(faceSeedPage, wxT("Face Seeds"), false);
    }

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnQuit);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnOk);
  Connect(ID_QUICK_STYLE_APPLY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnApply);
  Connect(ID_QUICK_STYLE_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnExport);
  Connect(ID_QUICK_STYLE_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleTopologyDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

void QuickStyleTopologyDialog::CreateButtons()
{
// 
// adding the common Buttons
//
  wxBoxSizer *topSizer = (wxBoxSizer *) (this->GetSizer());
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *save = new wxButton(this, ID_QUICK_STYLE_APPLY, wxT("&Apply"));
  btnBox->Add(save, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_QUICK_STYLE_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_QUICK_STYLE_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
}

wxPanel *QuickStyleTopologyDialog::CreateMainPage(wxWindow * parent)
{
//
// creating the MAIN page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MAIN);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// First row: Layer name
  boxSizer->AddSpacer(50);
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxVERTICAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *nameBox = new wxStaticBox(panel, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_VECTOR_LAYER, fullName,
                                        wxDefaultPosition, wxSize(370, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *typeBox = new wxStaticBox(panel, wxID_ANY,
                                         wxT("Geometry Type"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *typeBoxSizer = new wxStaticBoxSizer(typeBox, wxHORIZONTAL);
  nameSizer->Add(typeBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString typeName = wxT("UNKNOWN");
  if (Type == MAP_LAYER_NETWORK)
    typeName = wxT("Network (ISO TopoNet)");
  if (Type == MAP_LAYER_TOPOLOGY)
    typeName = wxT("Topology (ISO TopoGeo)");
  wxTextCtrl *typeCtrl = new wxTextCtrl(panel, ID_VECTOR_TYPE, typeName,
                                        wxDefaultPosition, wxSize(370, 22),
                                        wxTE_READONLY);
  typeBoxSizer->Add(typeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *uuidBox = new wxStaticBox(panel, wxID_ANY,
                                         wxT("QuickStyle Name"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *uuidBoxSizer = new wxStaticBoxSizer(uuidBox, wxHORIZONTAL);
  nameSizer->Add(uuidBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString uuid = wxString::FromUTF8(Style->GetUUID());
  wxTextCtrl *uuidCtrl = new wxTextCtrl(panel, ID_VECTOR_UUID, uuid,
                                        wxDefaultPosition, wxSize(370, 22),
                                        wxTE_READONLY);
  uuidBoxSizer->Add(uuidCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  boxSizer->AddSpacer(25);
// second row: Visibility Range
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *visibilityBox = new wxStaticBox(panel, wxID_STATIC,
                                               wxT("Visibility Range"),
                                               wxDefaultPosition,
                                               wxDefaultSize);
  wxBoxSizer *visibilitySizer =
    new wxStaticBoxSizer(visibilityBox, wxHORIZONTAL);
  visibilityBoxSizer->Add(visibilitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL,
                          5);
  wxString range[4];
  range[0] = wxT("&None");
  range[1] = wxT("&Min");
  range[2] = wxT("&Max");
  range[3] = wxT("&Both");
  wxRadioBox *rangeBox = new wxRadioBox(panel, ID_SYMBOLIZER_MINMAX_SCALE,
                                        wxT("&Range Type"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 4,
                                        range, 2,
                                        wxRA_SPECIFY_COLS);
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *minScaleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Min Scale:"));
  scaleMinSizer->Add(minScaleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *minScaleCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_MIN_SCALE, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  minScaleCtrl->Enable(false);
  scaleMinSizer->Add(minScaleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *scaleMaxSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMaxSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *maxScaleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Max Scale:"));
  scaleMaxSizer->Add(maxScaleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *maxScaleCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_MAX_SCALE, wxT("+Infinite"),
                   wxDefaultPosition, wxSize(100, 22));
  maxScaleCtrl->Enable(false);
  scaleMaxSizer->Add(maxScaleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdScaleChanged);
  return panel;
}

void QuickStyleTopologyDialog::
OnCmdScaleChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Visibility Range selection changed
//
  wxRadioBox *scaleModeCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MINMAX_SCALE);
  wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
  wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
  switch (scaleModeCtrl->GetSelection())
    {
      case 0:
        Style->EnableMinScale(false);
        Style->EnableMaxScale(false);
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        Style->EnableMinScale(true);
        Style->EnableMaxScale(false);
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        Style->EnableMinScale(false);
        Style->EnableMaxScale(true);
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        Style->EnableMinScale(true);
        Style->EnableMaxScale(true);
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

wxPanel *QuickStyleTopologyDialog::CreateNodePage(wxWindow * parent)
{
//
// creating the Node Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_POINT);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: Opacity 
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_NODE_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Well Known Mark Name
  wxBoxSizer *box1Sizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(box1Sizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *markSizer = new wxBoxSizer(wxHORIZONTAL);
  box1Sizer->Add(markSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString mark[6];
  mark[0] = wxT("&Square");
  mark[1] = wxT("&Circle");
  mark[2] = wxT("&Triangle");
  mark[3] = wxT("&Star");
  mark[4] = wxT("&Cross");
  mark[5] = wxT("&X");
  wxRadioBox *markBox = new wxRadioBox(panel, ID_SYMBOLIZER_NODE_MARK,
                                       wxT("&Mark"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 6,
                                       mark, 1,
                                       wxRA_SPECIFY_COLS);
  markSizer->Add(markBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  markBox->SetSelection(0);
// second row: Size and Rotation
  wxBoxSizer *box2Sizer = new wxBoxSizer(wxVERTICAL);
  box1Sizer->Add(box2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *sizeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(sizeBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Size
  wxStaticBox *sizeBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sizeSizer = new wxStaticBoxSizer(sizeBox, wxVERTICAL);
  sizeBoxSizer->Add(sizeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *size1Sizer = new wxBoxSizer(wxHORIZONTAL);
  sizeSizer->Add(size1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *sizeCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_SIZE, wxT("16.0"),
                   wxDefaultPosition, wxSize(100, 22));
  size1Sizer->Add(sizeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Rotation
  wxStaticBox *rotBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Rotation"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *rotSizer = new wxStaticBoxSizer(rotBox, wxVERTICAL);
  sizeBoxSizer->Add(rotSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *rot1Sizer = new wxBoxSizer(wxHORIZONTAL);
  rotSizer->Add(rot1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *rotCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_ROTATION, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  rot1Sizer->Add(rotCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: AnchorNode and Displacement
  wxBoxSizer *anchorBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(anchorBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row A: Anchor Node
  wxStaticBox *anchorBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Anchor Node"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *anchorSizer = new wxStaticBoxSizer(anchorBox, wxVERTICAL);
  anchorBoxSizer->Add(anchorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *anchor1Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  anchor1Sizer->Add(anchor1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_ANCHOR_X, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor1Sizer->Add(anchorXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *anchor2Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  anchor2Sizer->Add(anchor2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_ANCHOR_Y, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor2Sizer->Add(anchorYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: Displacement
  wxStaticBox *displacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                 wxT("Displacement"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  wxBoxSizer *displacementSizer =
    new wxStaticBoxSizer(displacementBox, wxVERTICAL);
  anchorBoxSizer->Add(displacementSizer, 0,
                      wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *displ1Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  displ1Sizer->Add(displ1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_DISPLACEMENT_X, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_DISPLACEMENT_Y, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ2Sizer->Add(displacementYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row: colors
  wxBoxSizer *box3Sizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(box3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// first row A: Fill Color
  wxBoxSizer *fillBoxSizer = new wxBoxSizer(wxVERTICAL);
  box3Sizer->Add(fillBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *colorFillBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Fill Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *colorFillSizer = new wxStaticBoxSizer(colorFillBox, wxVERTICAL);
  box3Sizer->Add(colorFillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *fill1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorFillSizer->Add(fill1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *fillColorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_FILL_COLOR, FillColor,
                   wxDefaultPosition, wxSize(80, 22));
  fill1Sizer->Add(fillColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleFillCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_NODE_FILL_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  fill1Sizer->Add(sampleFillCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickFill = new wxButton(panel, ID_SYMBOLIZER_NODE_FILL_PICKER_BTN,
                                    wxT("&Pick a color"));
  fill1Sizer->Add(pickFill, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Stroke Color
  box3Sizer->AddSpacer(30);
  wxStaticBox *colorStrokeBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Stroke Color"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *colorStrokeSizer =
    new wxStaticBoxSizer(colorStrokeBox, wxVERTICAL);
  box3Sizer->Add(colorStrokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorStrokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *strokeColorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_NODE_STROKE_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  stroke1Sizer->Add(strokeColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sampleStrokeCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_NODE_STROKE_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  stroke1Sizer->Add(sampleStrokeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickStroke =
    new wxButton(panel, ID_SYMBOLIZER_NODE_STROKE_PICKER_BTN,
                 wxT("&Pick a color"));
  stroke1Sizer->Add(pickStroke, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_NODE_MARK, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdNodeMarkChanged);
  Connect(ID_SYMBOLIZER_NODE_FILL_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdNodeColorFillPicker);
  Connect(ID_SYMBOLIZER_NODE_FILL_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdNodeColorFillChanged);
  Connect(ID_SYMBOLIZER_NODE_STROKE_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdNodeColorStrokePicker);
  Connect(ID_SYMBOLIZER_NODE_STROKE_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdNodeColorStrokeChanged);
  return panel;
}

void QuickStyleTopologyDialog::
OnCmdNodeMarkChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Mark selection changed
//
  wxRadioBox *markCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_NODE_MARK);
  switch (markCtrl->GetSelection())
    {
      case 1:
        Style->SetNodeWellKnownMark(RL2_GRAPHIC_MARK_CIRCLE);
        break;
      case 2:
        Style->SetNodeWellKnownMark(RL2_GRAPHIC_MARK_TRIANGLE);
        break;
      case 3:
        Style->SetNodeWellKnownMark(RL2_GRAPHIC_MARK_STAR);
        break;
      case 4:
        Style->SetNodeWellKnownMark(RL2_GRAPHIC_MARK_CROSS);
        break;
      case 5:
        Style->SetNodeWellKnownMark(RL2_GRAPHIC_MARK_X);
        break;
      default:
        Style->SetNodeWellKnownMark(RL2_GRAPHIC_MARK_SQUARE);
        break;
    };
}

void QuickStyleTopologyDialog::
OnCmdNodeColorFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_FILL_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_NODE_FILL_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdNodeColorFillPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_FILL_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

void QuickStyleTopologyDialog::
OnCmdNodeColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_STROKE_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_NODE_STROKE_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdNodeColorStrokePicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_STROKE_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

wxPanel *QuickStyleTopologyDialog::CreateEdgeLinkPage(wxWindow * parent)
{
//
// creating the EdgeLink Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxPanel *panel = new wxPanel(parent, ID_PANE_LINE);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #1 Opacity and Perpendicular Offset
  boxSizer->AddSpacer(50);
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxHORIZONTAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE1_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// first row B: PerpendicularOffset
  wxBoxSizer *perpendicularBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  opacityBoxSizer->Add(perpendicularBoxSizer, 0,
                       wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *perpendicularBox = new wxStaticBox(panel, wxID_STATIC,
                                                  wxT("Perpendicular Offset"),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
  wxBoxSizer *perpendicularSizer =
    new wxStaticBoxSizer(perpendicularBox, wxVERTICAL);
  perpendicularBoxSizer->Add(perpendicularSizer, 0,
                             wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *perp1Sizer = new wxBoxSizer(wxHORIZONTAL);
  perpendicularSizer->Add(perp1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *perpendicularCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_PERPENDICULAR, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  perp1Sizer->Add(perpendicularCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticText *perp1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Draw lines in parallel to the original geometry."));
  perp1Sizer->Add(perp1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticText *perp2Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Positive to the left-hand side. Negative numbers mean right."));
  perpendicularSizer->Add(perp2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row: Stroke color or Graphic
  wxBoxSizer *strokeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(strokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row B: Stroke Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeSizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE1_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_STROKE1_PICKER_BTN, wxT("&Pick a color"));
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row: Stroke-Width 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  colorBoxSizer->Add(miscSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row A: StrokeWidth
  wxBoxSizer *widthBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(widthBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *widthBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Width"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *widthSizer = new wxStaticBoxSizer(widthBox, wxVERTICAL);
  widthBoxSizer->Add(widthSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *widthCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row D: DashArray and DashOffset
  wxBoxSizer *dashBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(dashBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString dot[4];
  if (Type == MAP_LAYER_NETWORK)
    {
      dot[0] = wxT("&Solid Link");
      dot[1] = wxT("&Dotted Link");
      dot[2] = wxT("&Dashed Link");
      dot[3] = wxT("&Dashed/Dotted Link");
  } else
    {
      dot[0] = wxT("&Solid Edge");
      dot[1] = wxT("&Dotted Edge");
      dot[2] = wxT("&Dashed Edge");
      dot[3] = wxT("&Dashed/Dotted Edge");
    }
  wxRadioBox *dotBox = new wxRadioBox(panel, ID_SYMBOLIZER_DASH_DOT,
                                      wxT("&Dash/Dot Style"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 4,
                                      dot, 1,
                                      wxRA_SPECIFY_COLS);
  dotBox->SetSelection(0);
  dashBoxSizer->Add(dotBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkColorChanged);
  Connect(ID_SYMBOLIZER_STROKE1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkColorPicker);
  return panel;
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkColorChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE1_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

wxPanel *QuickStyleTopologyDialog::CreateFacePage(wxWindow * parent)
{
//
// creating the Face Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_FILL2);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the Face Displacement and Perpendicular Offset
  wxBoxSizer *polygonBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(polygonBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row A: Displacement
  wxBoxSizer *displacementBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  polygonBoxSizer->Add(displacementBoxSizer, 0,
                       wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *displacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                 wxT("Displacement"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  wxBoxSizer *displacementSizer =
    new wxStaticBoxSizer(displacementBox, wxVERTICAL);
  displacementBoxSizer->Add(displacementSizer, 0,
                            wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *displ1Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  displ1Sizer->Add(displ1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *displacementXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_Y, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ2Sizer->Add(displacementYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// first row B: PerpendicularOffset
  wxBoxSizer *perpendicularBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  polygonBoxSizer->Add(perpendicularBoxSizer, 0,
                       wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *perpendicularBox = new wxStaticBox(panel, wxID_STATIC,
                                                  wxT("Perpendicular Offset"),
                                                  wxDefaultPosition,
                                                  wxDefaultSize);
  wxBoxSizer *perpendicularSizer =
    new wxStaticBoxSizer(perpendicularBox, wxVERTICAL);
  perpendicularBoxSizer->Add(perpendicularSizer, 0,
                             wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *perp1Sizer = new wxBoxSizer(wxHORIZONTAL);
  perpendicularSizer->Add(perp1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *perpendicularCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_POLYGON1_PERPENDICULAR, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  perp1Sizer->Add(perpendicularCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticText *perp1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Positive: larger. / Negative: smaller."));
  perp1Sizer->Add(perp1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticText *perp2Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Drawing polygons smaller or larger than their actual geometry (Buffer)."));
  perpendicularSizer->Add(perp2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row: Fill Opacity
  wxBoxSizer *auxBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(auxBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *auxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Face Fill"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *auxSizer = new wxStaticBoxSizer(auxBox, wxHORIZONTAL);
  auxBoxSizer->Add(auxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(enableSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_FILL2_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(Style->IsFaceFill());
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  enableSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FILL2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(400, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityCtrl->Enable(Style->IsFaceFill());
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: Fill Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  auxSizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Fill Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FILL2_COLOR, FillColor,
                   wxDefaultPosition, wxSize(80, 22));
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  sampleCtrl->Enable(Style->IsFaceFill());
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_FILL2_PICKER_BTN, wxT("&Pick a color"));
  pick->Enable(Style->IsFaceFill());
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// first row A: the Stroke #1 Opacity 
  wxBoxSizer *opacity2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacity2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *enableBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Face Stroke"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *enable2Sizer = new wxStaticBoxSizer(enableBox, wxHORIZONTAL);
  opacity2BoxSizer->Add(enable2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enable2Ctrl = new wxCheckBox(panel, ID_SYMBOLIZER_STROKE2_ENABLE,
                                           wxT("Enable"),
                                           wxDefaultPosition, wxDefaultSize);
  enable2Ctrl->SetValue(Style->IsFaceStroke());
  enable2Sizer->Add(enable2Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *opacity2Box = new wxStaticBox(panel, wxID_STATIC,
                                             wxT("Opacity"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *opacity2Sizer = new wxStaticBoxSizer(opacity2Box, wxVERTICAL);
  enable2Sizer->Add(opacity2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacity2Ctrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(250, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacity2Ctrl->Enable(Style->IsFaceStroke());
  opacity2Sizer->Add(opacity2Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Stroke Color
  wxStaticBox *color2Box = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke Color"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *color2Sizer = new wxStaticBoxSizer(color2Box, wxVERTICAL);
  enable2Sizer->Add(color2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *color3Sizer = new wxBoxSizer(wxHORIZONTAL);
  color2Sizer->Add(color3Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *color2Ctrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  color2Ctrl->Enable(Style->IsFaceStroke());
  color3Sizer->Add(color2Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sample2Ctrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  sample2Ctrl->Enable(Style->IsFaceStroke());
  color3Sizer->Add(sample2Ctrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *picker2Sizer = new wxBoxSizer(wxHORIZONTAL);
  color2Sizer->Add(picker2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick2 =
    new wxButton(panel, ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxT("&Pick a color"));
  pick2->Enable(Style->IsFaceStroke());
  picker2Sizer->Add(pick2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row: Stroke-Width,
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  enable2Sizer->Add(miscSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row A: StrokeWidth
  wxBoxSizer *widthBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(widthBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *widthBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Width"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *widthSizer = new wxStaticBoxSizer(widthBox, wxVERTICAL);
  widthBoxSizer->Add(widthSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *widthCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  widthCtrl->Enable(false);
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceStrokeChanged);
  Connect(ID_SYMBOLIZER_FILL2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceFillChanged);
  Connect(ID_SYMBOLIZER_FILL2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceColorFillChanged);
  Connect(ID_SYMBOLIZER_FILL2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceColorFillPicker);
  Connect(ID_SYMBOLIZER_STROKE2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceColorStrokeChanged);
  Connect(ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceColorStrokePicker);
  return panel;
}

void QuickStyleTopologyDialog::
OnCmdFaceStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    Style->SetFaceStroke(true);
  else
    Style->SetFaceStroke(false);
  RetrieveFacePage(false);
  UpdateFacePage();
}

void QuickStyleTopologyDialog::
OnCmdFaceFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    Style->SetFaceFill(true);
  else
    Style->SetFaceFill(false);
  RetrieveFacePage(false);
  UpdateFacePage();
}

void QuickStyleTopologyDialog::
OnCmdFaceColorFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FILL2_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdFaceColorFillPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

void QuickStyleTopologyDialog::
OnCmdFaceColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdFaceColorStrokePicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

wxPanel *QuickStyleTopologyDialog::CreateEdgeLinkSeedPage(wxWindow * parent)
{
//
// creating the EdgeLinkSeed Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_POINT);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: Opacity 
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_EDGELINK_SEED_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Well Known Mark Name
  wxBoxSizer *box1Sizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(box1Sizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *markSizer = new wxBoxSizer(wxHORIZONTAL);
  box1Sizer->Add(markSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString mark[6];
  mark[0] = wxT("&Square");
  mark[1] = wxT("&Circle");
  mark[2] = wxT("&Triangle");
  mark[3] = wxT("&Star");
  mark[4] = wxT("&Cross");
  mark[5] = wxT("&X");
  wxRadioBox *markBox = new wxRadioBox(panel, ID_SYMBOLIZER_EDGELINK_SEED_MARK,
                                       wxT("&Mark"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 6,
                                       mark, 1,
                                       wxRA_SPECIFY_COLS);
  markSizer->Add(markBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  markBox->SetSelection(0);
// second row: Size and Rotation
  wxBoxSizer *box2Sizer = new wxBoxSizer(wxVERTICAL);
  box1Sizer->Add(box2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *sizeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(sizeBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Size
  wxStaticBox *sizeBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sizeSizer = new wxStaticBoxSizer(sizeBox, wxVERTICAL);
  sizeBoxSizer->Add(sizeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *size1Sizer = new wxBoxSizer(wxHORIZONTAL);
  sizeSizer->Add(size1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *sizeCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_SIZE, wxT("16.0"),
                   wxDefaultPosition, wxSize(100, 22));
  size1Sizer->Add(sizeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Rotation
  wxStaticBox *rotBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Rotation"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *rotSizer = new wxStaticBoxSizer(rotBox, wxVERTICAL);
  sizeBoxSizer->Add(rotSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *rot1Sizer = new wxBoxSizer(wxHORIZONTAL);
  rotSizer->Add(rot1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *rotCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_ROTATION, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  rot1Sizer->Add(rotCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: AnchorEdgeLinkSeed and Displacement
  wxBoxSizer *anchorBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(anchorBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row A: Anchor EdgeLinkSeed
  wxStaticBox *anchorBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Anchor EdgeLinkSeed"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *anchorSizer = new wxStaticBoxSizer(anchorBox, wxVERTICAL);
  anchorBoxSizer->Add(anchorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *anchor1Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  anchor1Sizer->Add(anchor1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_X, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor1Sizer->Add(anchorXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *anchor2Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  anchor2Sizer->Add(anchor2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_Y, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor2Sizer->Add(anchorYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: Displacement
  wxStaticBox *displacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                 wxT("Displacement"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  wxBoxSizer *displacementSizer =
    new wxStaticBoxSizer(displacementBox, wxVERTICAL);
  anchorBoxSizer->Add(displacementSizer, 0,
                      wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *displ1Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  displ1Sizer->Add(displ1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_X,
                   wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_Y,
                   wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ2Sizer->Add(displacementYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row: colors
  wxBoxSizer *box3Sizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(box3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// first row A: Fill Color
  wxBoxSizer *fillBoxSizer = new wxBoxSizer(wxVERTICAL);
  box3Sizer->Add(fillBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *colorFillBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Fill Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *colorFillSizer = new wxStaticBoxSizer(colorFillBox, wxVERTICAL);
  box3Sizer->Add(colorFillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *fill1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorFillSizer->Add(fill1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *fillColorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR, FillColor,
                   wxDefaultPosition, wxSize(80, 22));
  fill1Sizer->Add(fillColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleFillCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_EDGELINK_SEED_FILL_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  fill1Sizer->Add(sampleFillCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickFill =
    new wxButton(panel, ID_SYMBOLIZER_EDGELINK_SEED_FILL_PICKER_BTN,
                 wxT("&Pick a color"));
  fill1Sizer->Add(pickFill, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Stroke Color
  box3Sizer->AddSpacer(30);
  wxStaticBox *colorStrokeBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Stroke Color"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *colorStrokeSizer =
    new wxStaticBoxSizer(colorStrokeBox, wxVERTICAL);
  box3Sizer->Add(colorStrokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorStrokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *strokeColorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  stroke1Sizer->Add(strokeColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sampleStrokeCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_EDGELINK_SEED_STROKE_PICKER_HEX,
                       bmp,
                       wxDefaultPosition, wxSize(32, 32));
  stroke1Sizer->Add(sampleStrokeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickStroke =
    new wxButton(panel, ID_SYMBOLIZER_EDGELINK_SEED_STROKE_PICKER_BTN,
                 wxT("&Pick a color"));
  stroke1Sizer->Add(pickStroke, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_EDGELINK_SEED_MARK, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkSeedMarkChanged);
  Connect(ID_SYMBOLIZER_EDGELINK_SEED_FILL_PICKER_BTN,
          wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkSeedColorFillPicker);
  Connect(ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkSeedColorFillChanged);
  Connect(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_PICKER_BTN,
          wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkSeedColorStrokePicker);
  Connect(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdEdgeLinkSeedColorStrokeChanged);
  return panel;
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkSeedMarkChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Mark selection changed
//
  wxRadioBox *markCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_MARK);
  switch (markCtrl->GetSelection())
    {
      case 1:
        Style->SetEdgeLinkSeedWellKnownMark(RL2_GRAPHIC_MARK_CIRCLE);
        break;
      case 2:
        Style->SetEdgeLinkSeedWellKnownMark(RL2_GRAPHIC_MARK_TRIANGLE);
        break;
      case 3:
        Style->SetEdgeLinkSeedWellKnownMark(RL2_GRAPHIC_MARK_STAR);
        break;
      case 4:
        Style->SetEdgeLinkSeedWellKnownMark(RL2_GRAPHIC_MARK_CROSS);
        break;
      case 5:
        Style->SetEdgeLinkSeedWellKnownMark(RL2_GRAPHIC_MARK_X);
        break;
      default:
        Style->SetEdgeLinkSeedWellKnownMark(RL2_GRAPHIC_MARK_SQUARE);
        break;
    };
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkSeedColorFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_FILL_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkSeedColorFillPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkSeedColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *)
    FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdEdgeLinkSeedColorStrokePicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

wxPanel *QuickStyleTopologyDialog::CreateFaceSeedPage(wxWindow * parent)
{
//
// creating the FaceSeed Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_POINT);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: Opacity 
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FACE_SEED_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Well Known Mark Name
  wxBoxSizer *box1Sizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(box1Sizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *markSizer = new wxBoxSizer(wxHORIZONTAL);
  box1Sizer->Add(markSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString mark[6];
  mark[0] = wxT("&Square");
  mark[1] = wxT("&Circle");
  mark[2] = wxT("&Triangle");
  mark[3] = wxT("&Star");
  mark[4] = wxT("&Cross");
  mark[5] = wxT("&X");
  wxRadioBox *markBox = new wxRadioBox(panel, ID_SYMBOLIZER_FACE_SEED_MARK,
                                       wxT("&Mark"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 6,
                                       mark, 1,
                                       wxRA_SPECIFY_COLS);
  markSizer->Add(markBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  markBox->SetSelection(0);
// second row: Size and Rotation
  wxBoxSizer *box2Sizer = new wxBoxSizer(wxVERTICAL);
  box1Sizer->Add(box2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *sizeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(sizeBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Size
  wxStaticBox *sizeBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sizeSizer = new wxStaticBoxSizer(sizeBox, wxVERTICAL);
  sizeBoxSizer->Add(sizeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *size1Sizer = new wxBoxSizer(wxHORIZONTAL);
  sizeSizer->Add(size1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *sizeCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_SIZE, wxT("16.0"),
                   wxDefaultPosition, wxSize(100, 22));
  size1Sizer->Add(sizeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Rotation
  wxStaticBox *rotBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Rotation"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *rotSizer = new wxStaticBoxSizer(rotBox, wxVERTICAL);
  sizeBoxSizer->Add(rotSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *rot1Sizer = new wxBoxSizer(wxHORIZONTAL);
  rotSizer->Add(rot1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *rotCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_ROTATION, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  rot1Sizer->Add(rotCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: AnchorFaceSeed and Displacement
  wxBoxSizer *anchorBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(anchorBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row A: Anchor FaceSeed
  wxStaticBox *anchorBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Anchor FaceSeed"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *anchorSizer = new wxStaticBoxSizer(anchorBox, wxVERTICAL);
  anchorBoxSizer->Add(anchorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *anchor1Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  anchor1Sizer->Add(anchor1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_ANCHOR_X, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor1Sizer->Add(anchorXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *anchor2Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  anchor2Sizer->Add(anchor2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_ANCHOR_Y, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor2Sizer->Add(anchorYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: Displacement
  wxStaticBox *displacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                 wxT("Displacement"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  wxBoxSizer *displacementSizer =
    new wxStaticBoxSizer(displacementBox, wxVERTICAL);
  anchorBoxSizer->Add(displacementSizer, 0,
                      wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *displ1Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  displ1Sizer->Add(displ1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_X, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_Y, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ2Sizer->Add(displacementYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row: colors
  wxBoxSizer *box3Sizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(box3Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// first row A: Fill Color
  wxBoxSizer *fillBoxSizer = new wxBoxSizer(wxVERTICAL);
  box3Sizer->Add(fillBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *colorFillBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Fill Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *colorFillSizer = new wxStaticBoxSizer(colorFillBox, wxVERTICAL);
  box3Sizer->Add(colorFillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *fill1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorFillSizer->Add(fill1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *fillColorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_FILL_COLOR, FillColor,
                   wxDefaultPosition, wxSize(80, 22));
  fill1Sizer->Add(fillColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleFillCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FACE_SEED_FILL_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  fill1Sizer->Add(sampleFillCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickFill =
    new wxButton(panel, ID_SYMBOLIZER_FACE_SEED_FILL_PICKER_BTN,
                 wxT("&Pick a color"));
  fill1Sizer->Add(pickFill, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Stroke Color
  box3Sizer->AddSpacer(30);
  wxStaticBox *colorStrokeBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Stroke Color"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *colorStrokeSizer =
    new wxStaticBoxSizer(colorStrokeBox, wxVERTICAL);
  box3Sizer->Add(colorStrokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorStrokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxTextCtrl *strokeColorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  stroke1Sizer->Add(strokeColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sampleStrokeCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FACE_SEED_STROKE_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  stroke1Sizer->Add(sampleStrokeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickStroke =
    new wxButton(panel, ID_SYMBOLIZER_FACE_SEED_STROKE_PICKER_BTN,
                 wxT("&Pick a color"));
  stroke1Sizer->Add(pickStroke, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_FACE_SEED_MARK, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceSeedMarkChanged);
  Connect(ID_SYMBOLIZER_FACE_SEED_FILL_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceSeedColorFillPicker);
  Connect(ID_SYMBOLIZER_FACE_SEED_FILL_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceSeedColorFillChanged);
  Connect(ID_SYMBOLIZER_FACE_SEED_STROKE_PICKER_BTN,
          wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceSeedColorStrokePicker);
  Connect(ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleTopologyDialog::OnCmdFaceSeedColorStrokeChanged);
  return panel;
}

void QuickStyleTopologyDialog::
OnCmdFaceSeedMarkChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Mark selection changed
//
  wxRadioBox *markCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FACE_SEED_MARK);
  switch (markCtrl->GetSelection())
    {
      case 1:
        Style->SetFaceSeedWellKnownMark(RL2_GRAPHIC_MARK_CIRCLE);
        break;
      case 2:
        Style->SetFaceSeedWellKnownMark(RL2_GRAPHIC_MARK_TRIANGLE);
        break;
      case 3:
        Style->SetFaceSeedWellKnownMark(RL2_GRAPHIC_MARK_STAR);
        break;
      case 4:
        Style->SetFaceSeedWellKnownMark(RL2_GRAPHIC_MARK_CROSS);
        break;
      case 5:
        Style->SetFaceSeedWellKnownMark(RL2_GRAPHIC_MARK_X);
        break;
      default:
        Style->SetFaceSeedWellKnownMark(RL2_GRAPHIC_MARK_SQUARE);
        break;
    };
}

void QuickStyleTopologyDialog::
OnCmdFaceSeedColorFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_FILL_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FACE_SEED_FILL_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdFaceSeedColorFillPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_FILL_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

void QuickStyleTopologyDialog::
OnCmdFaceSeedColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FACE_SEED_STROKE_PICKER_HEX);
  wxColour back = wxColour(255, 255, 255);
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, back);
  wxBitmap bmp;
  ColorMapEntry::DoPaintColorSample(32, 32, back, bmp);
  sampleCtrl->SetBitmap(bmp);
  sampleCtrl->Refresh();
  sampleCtrl->Update();
}

void QuickStyleTopologyDialog::
OnCmdFaceSeedColorStrokePicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR);
  wxColour clr = wxNullColour;
  wxString str = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
}

bool QuickStyleTopologyDialog::RetrieveMainPage()
{
//
// retrieving params from the MAIN page
//
  double min = Style->GetScaleMin();
  double max = Style->GetScaleMax();
  if (Style->IsMinScaleEnabled() == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&min) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (min < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Style->IsMaxScaleEnabled() == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&max) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (max < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Style->IsMinScaleEnabled() == true && Style->IsMaxScaleEnabled() == true)
    {
      if (min >= max)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  Style->SetScaleMin(min);
  Style->SetScaleMax(max);
  return true;
}

void QuickStyleTopologyDialog::UpdateMainPage()
{
//
// updating the MAIN page
//
  wxRadioBox *rangeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MINMAX_SCALE);
  if (Style->IsMinScaleEnabled() != true && Style->IsMaxScaleEnabled() != true)
    rangeBox->SetSelection(0);
  else if (Style->IsMinScaleEnabled() == true
           && Style->IsMaxScaleEnabled() != true)
    rangeBox->SetSelection(1);
  else if (Style->IsMinScaleEnabled() != true
           && Style->IsMaxScaleEnabled() == true)
    rangeBox->SetSelection(2);
  else
    rangeBox->SetSelection(3);
  wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
  char dummy[64];
  wxString str;
  if (Style->IsMinScaleEnabled() == true)
    {
      sprintf(dummy, "%1.2f", Style->GetScaleMin());
      str = wxString::FromUTF8(dummy);
      minCtrl->SetValue(str);
      minCtrl->Enable(true);
  } else
    {
      str = wxT("0.0");
      minCtrl->SetValue(str);
      minCtrl->Enable(false);
    }
  wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
  if (Style->IsMaxScaleEnabled() == true)
    {
      sprintf(dummy, "%1.2f", Style->GetScaleMax());
      str = wxString::FromUTF8(dummy);
      maxCtrl->SetValue(str);
      maxCtrl->Enable(true);
  } else
    {
      str = wxT("+Infinite");
      maxCtrl->SetValue(str);
      maxCtrl->Enable(false);
    }
}

bool QuickStyleTopologyDialog::RetrieveNodePage(bool check)
{
//
// retrieving params from the Node Nodeizer page
//
  double opacity;
  double size;
  double rotation;
  double anchorNodeX;
  double anchorNodeY;
  double displacementX;
  double displacementY;
  char fillColor[8];
  char strokeColor[8];
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_NODE_OPACITY);
  opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&size) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (size < 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_ROTATION);
  value = rotationCtrl->GetValue();
  if (value.ToDouble(&rotation) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ROTATION isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_ANCHOR_X);
  value = anchorXCtrl->GetValue();
  if (value.ToDouble(&anchorNodeX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorNodeX < 0.0 || anchorNodeX > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_ANCHOR_Y);
  value = anchorYCtrl->GetValue();
  if (value.ToDouble(&anchorNodeY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorNodeY < 0.0 || anchorNodeY > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_DISPLACEMENT_X);
  value = displXCtrl->GetValue();
  if (value.ToDouble(&displacementX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_DISPLACEMENT_Y);
  value = displYCtrl->GetValue();
  if (value.ToDouble(&displacementY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_FILL_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FILL-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(fillColor, color.ToUTF8());
  colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_STROKE_COLOR);
  color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(strokeColor, color.ToUTF8());
  Style->SetNodeOpacity(opacity);
  Style->SetNodeSize(size);
  Style->SetNodeRotation(rotation);
  Style->SetNodeAnchorX(anchorNodeX);
  Style->SetNodeAnchorY(anchorNodeY);
  Style->SetNodeDisplacementX(displacementX);
  Style->SetNodeDisplacementY(displacementY);
  Style->SetNodeFillColor(fillColor);
  Style->SetNodeStrokeColor(strokeColor);
  return true;
}

void QuickStyleTopologyDialog::UpdateNodePage()
{
//
// updating the Node Nodeizer page
//
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_NODE_OPACITY);
  opacityCtrl->SetValue(Style->GetNodeOpacity() * 100.0);
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetNodeSize());
  wxString str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_ROTATION);
  sprintf(dummy, "%1.2f", Style->GetNodeRotation());
  str = wxString::FromUTF8(dummy);
  rotationCtrl->SetValue(str);
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_ANCHOR_X);
  sprintf(dummy, "%1.2f", Style->GetNodeAnchorX());
  str = wxString::FromUTF8(dummy);
  anchorXCtrl->SetValue(str);
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_ANCHOR_Y);
  sprintf(dummy, "%1.2f", Style->GetNodeAnchorY());
  str = wxString::FromUTF8(dummy);
  anchorYCtrl->SetValue(str);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_DISPLACEMENT_X);
  sprintf(dummy, "%1.2f", Style->GetNodeDisplacementX());
  str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetNodeDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxRadioBox *markCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_NODE_MARK);
  switch (Style->GetNodeWellKnownMark())
    {
      case RL2_GRAPHIC_MARK_CIRCLE:
        markCtrl->SetSelection(1);
        break;
      case RL2_GRAPHIC_MARK_TRIANGLE:
        markCtrl->SetSelection(2);
        break;
      case RL2_GRAPHIC_MARK_STAR:
        markCtrl->SetSelection(3);
        break;
      case RL2_GRAPHIC_MARK_CROSS:
        markCtrl->SetSelection(4);
        break;
      case RL2_GRAPHIC_MARK_X:
        markCtrl->SetSelection(5);
        break;
      default:
        markCtrl->SetSelection(0);
        break;
    };
  wxTextCtrl *colorFillCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_FILL_COLOR);
  wxString fillColor = wxString::FromUTF8(Style->GetNodeFillColor());
  colorFillCtrl->SetValue(fillColor);
  wxTextCtrl *colorStrokeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NODE_STROKE_COLOR);
  wxString strokeColor = wxString::FromUTF8(Style->GetNodeStrokeColor());
  colorStrokeCtrl->SetValue(strokeColor);
}

bool QuickStyleTopologyDialog::RetrieveEdgeLinkPage(bool check)
{
//
// retrieving params from the EdgeLink Symbolizer page
//
  double opacity;
  double perpendicularOffset;
  char strokeColor[8];
  double strokeWidth;
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_PERPENDICULAR);
  wxString value = perpCtrl->GetValue();
  if (value.ToDouble(&perpendicularOffset) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(strokeColor, color.ToUTF8());
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  value = widthCtrl->GetValue();
  if (value.ToDouble(&strokeWidth) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (strokeWidth <= 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxRadioBox *dotCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT);
  switch (dotCtrl->GetSelection())
    {
      case 1:
        Style->SetEdgeLinkDotStyle(QUICK_STYLE_DOT_LINE);
        break;
      case 2:
        Style->SetEdgeLinkDotStyle(QUICK_STYLE_DASH_LINE);
        break;
      case 3:
        Style->SetEdgeLinkDotStyle(QUICK_STYLE_DASH_DOT_LINE);
        break;
      default:
        Style->SetEdgeLinkDotStyle(QUICK_STYLE_SOLID_LINE);
        break;
    };
  Style->SetEdgeLinkOpacity(opacity);
  Style->SetEdgeLinkPerpendicularOffset(perpendicularOffset);
  Style->SetEdgeLinkStrokeWidth(strokeWidth);
  Style->SetEdgeLinkStrokeColor(strokeColor);
  return true;
}

void QuickStyleTopologyDialog::UpdateEdgeLinkPage()
{
//
// updating the EdgeLink Symbolizer page
//
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  opacityCtrl->SetValue(Style->GetEdgeLinkOpacity() * 100.0);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_PERPENDICULAR);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkPerpendicularOffset());
  wxString str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE1_PICKER_BTN);
  colorCtrl->Enable(true);
  pick->Enable(true);
  wxString strokeColor = wxString::FromUTF8(Style->GetEdgeLinkStrokeColor());
  colorCtrl->SetValue(strokeColor);
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkStrokeWidth());
  str = wxString::FromUTF8(dummy);
  wxRadioBox *dotCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT);
  switch (Style->GetEdgeLinkDotStyle())
    {
      case QUICK_STYLE_DOT_LINE:
        dotCtrl->SetSelection(1);
        break;
      case QUICK_STYLE_DASH_LINE:
        dotCtrl->SetSelection(2);
        break;
      case QUICK_STYLE_DASH_DOT_LINE:
        dotCtrl->SetSelection(3);
        break;
      default:
        dotCtrl->SetSelection(0);
        break;
    };
  widthCtrl->SetValue(str);
}

bool QuickStyleTopologyDialog::RetrieveFacePage(bool check)
{
//
// retrieving params from the Face Synbolizer page
//
  double fillOpacity;
  double displacementX;
  double displacementY;
  double perpendicularOffset;
  char fillColor[8];
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL2_OPACITY);
  fillOpacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X);
  wxString value = displXCtrl->GetValue();
  if (Style->IsFaceFill() != true && Style->IsFaceStroke() != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You can't disable both FILL and STROKE at the same time !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (value.ToDouble(&displacementX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_Y);
  value = displYCtrl->GetValue();
  if (value.ToDouble(&displacementY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_PERPENDICULAR);
  value = perpCtrl->GetValue();
  if (value.ToDouble(&perpendicularOffset) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FILL-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(fillColor, color.ToUTF8());
  Style->SetFaceFillOpacity(fillOpacity);
  Style->SetFaceDisplacementX(displacementX);
  Style->SetFaceDisplacementY(displacementY);
  Style->SetFacePerpendicularOffset(perpendicularOffset);
  Style->SetFaceFillColor(fillColor);
  return true;
}

void QuickStyleTopologyDialog::UpdateFacePage()
{
//
// updating the Face Symbolizer page
//
  wxCheckBox *enableBox = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE);
  enableBox->SetValue(Style->IsFaceFill());
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetFaceDisplacementX());
  wxString str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetFaceDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_PERPENDICULAR);
  sprintf(dummy, "%1.2f", Style->GetFacePerpendicularOffset());
  str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL2_OPACITY);
  opacityCtrl->SetValue(Style->GetFaceFillOpacity() * 100.0);
  opacityCtrl->Enable(Style->IsFaceFill());
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_FILL2_PICKER_BTN);
  wxColour color = wxNullColour;
  str = wxString::FromUTF8(Style->GetFaceFillColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
  colorCtrl->Enable(Style->IsFaceFill());
  pick->Enable(Style->IsFaceFill());
  wxSlider *opacity2Ctrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  opacity2Ctrl->SetValue(Style->GetFaceStrokeOpacity() * 100.0);
  opacity2Ctrl->Enable(Style->IsFaceStroke());
  wxTextCtrl *color2Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxButton *pick2 = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_BTN);
  color = wxNullColour;
  str = wxString::FromUTF8(Style->GetFaceStrokeColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      color2Ctrl->SetValue(str);
    }
  color2Ctrl->Enable(Style->IsFaceStroke());
  pick2->Enable(Style->IsFaceStroke());
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  sprintf(dummy, "%1.2f", Style->GetFaceStrokeWidth());
  str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  widthCtrl->Enable(Style->IsFaceStroke());
}

bool QuickStyleTopologyDialog::RetrieveEdgeLinkSeedPage(bool check)
{
//
// retrieving params from the EdgeLinkSeed EdgeLinkSeedizer page
//
  double opacity;
  double size;
  double rotation;
  double anchorEdgeLinkSeedX;
  double anchorEdgeLinkSeedY;
  double displacementX;
  double displacementY;
  char fillColor[8];
  char strokeColor[8];
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_OPACITY);
  opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *sizeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&size) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (size < 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_ROTATION);
  value = rotationCtrl->GetValue();
  if (value.ToDouble(&rotation) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ROTATION isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_X);
  value = anchorXCtrl->GetValue();
  if (value.ToDouble(&anchorEdgeLinkSeedX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorEdgeLinkSeedX < 0.0 || anchorEdgeLinkSeedX > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_Y);
  value = anchorYCtrl->GetValue();
  if (value.ToDouble(&anchorEdgeLinkSeedY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorEdgeLinkSeedY < 0.0 || anchorEdgeLinkSeedY > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_X);
  value = displXCtrl->GetValue();
  if (value.ToDouble(&displacementX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_Y);
  value = displYCtrl->GetValue();
  if (value.ToDouble(&displacementY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FILL-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(fillColor, color.ToUTF8());
  colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR);
  color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(strokeColor, color.ToUTF8());
  Style->SetEdgeLinkSeedOpacity(opacity);
  Style->SetEdgeLinkSeedSize(size);
  Style->SetEdgeLinkSeedRotation(rotation);
  Style->SetEdgeLinkSeedAnchorX(anchorEdgeLinkSeedX);
  Style->SetEdgeLinkSeedAnchorY(anchorEdgeLinkSeedY);
  Style->SetEdgeLinkSeedDisplacementX(displacementX);
  Style->SetEdgeLinkSeedDisplacementY(displacementY);
  Style->SetEdgeLinkSeedFillColor(fillColor);
  Style->SetEdgeLinkSeedStrokeColor(strokeColor);
  return true;
}

void QuickStyleTopologyDialog::UpdateEdgeLinkSeedPage()
{
//
// updating the EdgeLinkSeed EdgeLinkSeedizer page
//
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_OPACITY);
  opacityCtrl->SetValue(Style->GetEdgeLinkSeedOpacity() * 100.0);
  wxTextCtrl *sizeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkSeedSize());
  wxString str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_ROTATION);
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkSeedRotation());
  str = wxString::FromUTF8(dummy);
  rotationCtrl->SetValue(str);
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_X);
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkSeedAnchorX());
  str = wxString::FromUTF8(dummy);
  anchorXCtrl->SetValue(str);
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_ANCHOR_Y);
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkSeedAnchorY());
  str = wxString::FromUTF8(dummy);
  anchorYCtrl->SetValue(str);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_X);
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkSeedDisplacementX());
  str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetEdgeLinkSeedDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxRadioBox *markCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_MARK);
  switch (Style->GetEdgeLinkSeedWellKnownMark())
    {
      case RL2_GRAPHIC_MARK_CIRCLE:
        markCtrl->SetSelection(1);
        break;
      case RL2_GRAPHIC_MARK_TRIANGLE:
        markCtrl->SetSelection(2);
        break;
      case RL2_GRAPHIC_MARK_STAR:
        markCtrl->SetSelection(3);
        break;
      case RL2_GRAPHIC_MARK_CROSS:
        markCtrl->SetSelection(4);
        break;
      case RL2_GRAPHIC_MARK_X:
        markCtrl->SetSelection(5);
        break;
      default:
        markCtrl->SetSelection(0);
        break;
    };
  wxTextCtrl *colorFillCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_FILL_COLOR);
  wxString fillColor = wxString::FromUTF8(Style->GetEdgeLinkSeedFillColor());
  colorFillCtrl->SetValue(fillColor);
  wxTextCtrl *colorStrokeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_EDGELINK_SEED_STROKE_COLOR);
  wxString strokeColor =
    wxString::FromUTF8(Style->GetEdgeLinkSeedStrokeColor());
  colorStrokeCtrl->SetValue(strokeColor);
}

bool QuickStyleTopologyDialog::RetrieveFaceSeedPage(bool check)
{
//
// retrieving params from the FaceSeed FaceSeedizer page
//
  double opacity;
  double size;
  double rotation;
  double anchorFaceSeedX;
  double anchorFaceSeedY;
  double displacementX;
  double displacementY;
  char fillColor[8];
  char strokeColor[8];
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FACE_SEED_OPACITY);
  opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *sizeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&size) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (size < 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_ROTATION);
  value = rotationCtrl->GetValue();
  if (value.ToDouble(&rotation) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ROTATION isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_ANCHOR_X);
  value = anchorXCtrl->GetValue();
  if (value.ToDouble(&anchorFaceSeedX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorFaceSeedX < 0.0 || anchorFaceSeedX > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_ANCHOR_Y);
  value = anchorYCtrl->GetValue();
  if (value.ToDouble(&anchorFaceSeedY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorFaceSeedY < 0.0 || anchorFaceSeedY > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_X);
  value = displXCtrl->GetValue();
  if (value.ToDouble(&displacementX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_Y);
  value = displYCtrl->GetValue();
  if (value.ToDouble(&displacementY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_FILL_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FILL-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(fillColor, color.ToUTF8());
  colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR);
  color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(strokeColor, color.ToUTF8());
  Style->SetFaceSeedOpacity(opacity);
  Style->SetFaceSeedSize(size);
  Style->SetFaceSeedRotation(rotation);
  Style->SetFaceSeedAnchorX(anchorFaceSeedX);
  Style->SetFaceSeedAnchorY(anchorFaceSeedY);
  Style->SetFaceSeedDisplacementX(displacementX);
  Style->SetFaceSeedDisplacementY(displacementY);
  Style->SetFaceSeedFillColor(fillColor);
  Style->SetFaceSeedStrokeColor(strokeColor);
  return true;
}

void QuickStyleTopologyDialog::UpdateFaceSeedPage()
{
//
// updating the FaceSeed FaceSeedizer page
//
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FACE_SEED_OPACITY);
  opacityCtrl->SetValue(Style->GetFaceSeedOpacity() * 100.0);
  wxTextCtrl *sizeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetFaceSeedSize());
  wxString str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_ROTATION);
  sprintf(dummy, "%1.2f", Style->GetFaceSeedRotation());
  str = wxString::FromUTF8(dummy);
  rotationCtrl->SetValue(str);
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_ANCHOR_X);
  sprintf(dummy, "%1.2f", Style->GetFaceSeedAnchorX());
  str = wxString::FromUTF8(dummy);
  anchorXCtrl->SetValue(str);
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_ANCHOR_Y);
  sprintf(dummy, "%1.2f", Style->GetFaceSeedAnchorY());
  str = wxString::FromUTF8(dummy);
  anchorYCtrl->SetValue(str);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_X);
  sprintf(dummy, "%1.2f", Style->GetFaceSeedDisplacementX());
  str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetFaceSeedDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxRadioBox *markCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FACE_SEED_MARK);
  switch (Style->GetFaceSeedWellKnownMark())
    {
      case RL2_GRAPHIC_MARK_CIRCLE:
        markCtrl->SetSelection(1);
        break;
      case RL2_GRAPHIC_MARK_TRIANGLE:
        markCtrl->SetSelection(2);
        break;
      case RL2_GRAPHIC_MARK_STAR:
        markCtrl->SetSelection(3);
        break;
      case RL2_GRAPHIC_MARK_CROSS:
        markCtrl->SetSelection(4);
        break;
      case RL2_GRAPHIC_MARK_X:
        markCtrl->SetSelection(5);
        break;
      default:
        markCtrl->SetSelection(0);
        break;
    };
  wxTextCtrl *colorFillCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_FILL_COLOR);
  wxString fillColor = wxString::FromUTF8(Style->GetFaceSeedFillColor());
  colorFillCtrl->SetValue(fillColor);
  wxTextCtrl *colorStrokeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FACE_SEED_STROKE_COLOR);
  wxString strokeColor = wxString::FromUTF8(Style->GetFaceSeedStrokeColor());
  colorStrokeCtrl->SetValue(strokeColor);
}

void QuickStyleTopologyDialog::OnPageChanging(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changing
//
  bool ret = false;
  int idx = event.GetOldSelection();
  if (idx == 0)
    ret = RetrieveMainPage();
  else if (idx == 1)
    ret = RetrieveNodePage();
  else if (idx == 2)
    ret = RetrieveEdgeLinkPage();
  else
    {
      if (Type == MAP_LAYER_NETWORK)
        {
          if (idx == 3)
            ret = RetrieveEdgeLinkSeedPage();
      } else
        {
          if (idx == 3)
            ret = RetrieveFacePage();
          if (idx == 4)
            ret = RetrieveEdgeLinkSeedPage();
          if (idx == 5)
            ret = RetrieveFaceSeedPage();
        }
    }
  if (ret != true)
    event.Veto();
}

void QuickStyleTopologyDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  int idx = event.GetSelection();
  if (idx == 0)
    UpdateMainPage();
  else if (idx == 1)
    UpdateNodePage();
  else if (idx == 2)
    UpdateEdgeLinkPage();
  else
    {
      if (Type == MAP_LAYER_NETWORK)
        {
          if (idx == 3)
            UpdateEdgeLinkSeedPage();
      } else
        {
          if (idx == 3)
            UpdateFacePage();
          if (idx == 4)
            UpdateEdgeLinkSeedPage();
          if (idx == 5)
            UpdateFaceSeedPage();
        }
    }
}

bool QuickStyleTopologyDialog::UpdateStyle()
{
//
// updating the QuickStyle
//
  bool ret = false;
  int idx = GetBookCtrl()->GetSelection();
  if (idx == 0)
    ret = RetrieveMainPage();
  else if (idx == 1)
    ret = RetrieveNodePage();
  else if (idx == 2)
    ret = RetrieveEdgeLinkPage();
  else
    {
      if (Type == MAP_LAYER_NETWORK)
        {
          if (idx == 3)
            ret = RetrieveEdgeLinkSeedPage();
      } else
        {
          if (idx == 3)
            ret = RetrieveFacePage();
          if (idx == 4)
            ret = RetrieveEdgeLinkSeedPage();
          if (idx == 5)
            ret = RetrieveFaceSeedPage();
        }
    }
  if (ret == false)
    return false;

  VectorLayerConfig *config = Layer->GetVectorConfig();
  bool setCurrentStyle = false;
  if (config->GetStyle() == NULL)
    setCurrentStyle = true;
  else
    {
      if (strcmp(Style->GetUUID(), config->GetStyle()) != 0)
        setCurrentStyle = true;
    }
  if (setCurrentStyle == true)
    {
      config->SetStyle(Style->GetUUID());
      IsConfigChanged = true;
    }
  IsConfigChanged = Layer->UpdateQuickStyle(Style);
  return true;
}

void QuickStyleTopologyDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// permanently saving the QuickStyle and quitting
//
  if (UpdateStyle() == true)
    wxDialog::EndModal(wxID_OK);
}

void QuickStyleTopologyDialog::OnApply(wxCommandEvent & WXUNUSED(event))
{
//
// applying the QuickStyle and continuing
//
  if (UpdateStyle() == true)
    {
      if (IsConfigChanged == true)
        MapPanel->RefreshMap();
    }
}

void QuickStyleTopologyDialog::OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the Quick Style as an external file
//
  bool xret = false;
  int ret;
  wxString path;
  wxString lastDir;
  int idx = GetBookCtrl()->GetSelection();
  if (idx == 0)
    xret = RetrieveMainPage();
  else if (idx == 1)
    xret = RetrieveNodePage();
  else if (idx == 2)
    xret = RetrieveEdgeLinkPage();
  else
    {
      if (Type == MAP_LAYER_NETWORK)
        {
          if (idx == 3)
            xret = RetrieveEdgeLinkSeedPage();
      } else
        {
          if (idx == 3)
            xret = RetrieveFacePage();
          if (idx == 4)
            xret = RetrieveEdgeLinkSeedPage();
          if (idx == 5)
            xret = RetrieveFaceSeedPage();
        }
    }
  if (xret == false)
    return;

  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE QuickStyle to a file"),
                          wxT(""), wxT("style.xml"),
                          wxT("XML Document|*.xml|All files (*.*)|*.*"),
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
      lastDir = file.GetPath();
      path = fileDialog.GetPath();
      FILE *out = fopen(path.ToUTF8(), "wb");
      if (out == NULL)
        wxMessageBox(wxT("ERROR: unable to create:\n\n\"") + path + wxT("\""),
                     wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      else
        {
          char *xml = Style->CreateXmlStyle();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE QuickStyle successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
}

void QuickStyleTopologyDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the Quick Style into the Clipboard 
//
  bool ret = false;
  int idx = GetBookCtrl()->GetSelection();
  if (idx == 0)
    ret = RetrieveMainPage();
  else if (idx == 1)
    ret = RetrieveNodePage();
  else if (idx == 2)
    ret = RetrieveEdgeLinkPage();
  else
    {
      if (Type == MAP_LAYER_NETWORK)
        {
          if (idx == 3)
            ret = RetrieveEdgeLinkSeedPage();
      } else
        {
          if (idx == 3)
            ret = RetrieveFacePage();
          if (idx == 4)
            ret = RetrieveEdgeLinkSeedPage();
          if (idx == 5)
            ret = RetrieveFaceSeedPage();
        }
    }
  if (ret == false)
    return;

  char *xml = Style->CreateXmlStyle();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void QuickStyleTopologyDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}
