/*
/ QuickStylesVector.cpp
/ Quick Styles wizards (Vector layers)
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

QuickStyleObj::QuickStyleObj(int type)
{
// ctor
  DoGetUUID(UUID);
  Type = type;
  MinScaleEnabled = false;
  MaxScaleEnabled = false;
  ScaleMin = 0.0;
  ScaleMax = 0.0;
  SymbolOpacity = 1.0;
  SymbolSize = 16;
  SymbolRotation = 0.0;
  SymbolAnchorX = 0.5;
  SymbolAnchorY = 0.5;
  SymbolDisplacementX = 0.0;
  SymbolDisplacementY = 0.0;
  SymbolWellKnownMark = RandomWellKnownMark();
  RandomColor(SymbolFillColor);
  RandomColor(SymbolStrokeColor);
  LineOpacity = 1.0;
  LinePerpendicularOffset = 0.0;
  LineStrokeWidth = 1.0;
  RandomColor(LineStrokeColor);
  LineDotStyle = QUICK_STYLE_SOLID_LINE;
  Line2Enabled = false;
  Line2StrokeWidth = 1.0;
  RandomColor(Line2StrokeColor);
  Line2DotStyle = QUICK_STYLE_SOLID_LINE;
  PolygonFill = true;
  PolygonStroke = true;
  PolygonDisplacementX = 0.0;
  PolygonDisplacementY = 0.0;
  PolygonPerpendicularOffset = 0.0;
  PolygonFillOpacity = 1.0;
  RandomColor(PolygonFillColor);
  PolygonSolidFill = true;
  PolygonFillBrushId = 0;
  PolygonStrokeOpacity = 1.0;
  PolygonStrokeWidth = 1.0;
  RandomColor(PolygonStrokeColor);
  LabelLinePlacement = true;
  LabelPrint = false;
  DontPaintGeomSymbolizer = false;
  LabelColumn = NULL;
  FontFacename = NULL;
  FontSize = 10.0;
  FontStyle = RL2_FONTSTYLE_NORMAL;
  FontWeight = RL2_FONTWEIGHT_NORMAL;
  FontOpacity = 1.0;
  strcpy(FontColor, "#000000");
  HasHalo = false;
  HaloRadius = 1.0;
  HaloOpacity = 1.0;
  strcpy(HaloColor, "#ffffff");
  LabelAnchorX = 0.5;
  LabelAnchorY = 0.5;
  LabelDisplacementX = 0.0;
  LabelDisplacementY = 0.0;
  LabelRotation = 0.0;
  LabelPerpendicularOffset = 0.0;
  RepeatedLabel = false;
  LabelInitialGap = 0.0;
  LabelGap = 0.0;
  LabelIsAligned = false;
  LabelGeneralizeLine = false;
  XmlStyle = NULL;
}

QuickStyleObj *QuickStyleObj::Clone()
{
//
// cloning a Quick Style
//
  QuickStyleObj *Style = new QuickStyleObj(this->Type);
  strcpy(Style->UUID, this->UUID);
  Style->MinScaleEnabled = this->MinScaleEnabled;
  Style->MaxScaleEnabled = this->MaxScaleEnabled;
  Style->ScaleMin = this->ScaleMin;
  Style->ScaleMax = this->ScaleMax;
  Style->SymbolOpacity = this->SymbolOpacity;
  Style->SymbolSize = this->SymbolSize;
  Style->SymbolRotation = this->SymbolRotation;
  Style->SymbolAnchorX = this->SymbolAnchorX;
  Style->SymbolAnchorY = this->SymbolAnchorY;
  Style->SymbolDisplacementX = this->SymbolDisplacementX;
  Style->SymbolDisplacementY = this->SymbolDisplacementY;
  Style->SymbolWellKnownMark = this->SymbolWellKnownMark;
  strcpy(Style->SymbolFillColor, this->SymbolFillColor);
  strcpy(Style->SymbolStrokeColor, this->SymbolStrokeColor);
  Style->LineOpacity = this->LineOpacity;
  Style->LinePerpendicularOffset = this->LinePerpendicularOffset;
  Style->LineStrokeWidth = this->LineStrokeWidth;
  strcpy(Style->LineStrokeColor, this->LineStrokeColor);
  Style->LineDotStyle = this->LineDotStyle;
  Style->Line2Enabled = this->Line2Enabled;
  Style->Line2StrokeWidth = this->Line2StrokeWidth;
  strcpy(Style->Line2StrokeColor, this->Line2StrokeColor);
  Style->Line2DotStyle = this->Line2DotStyle;
  Style->PolygonFill = this->PolygonFill;
  Style->PolygonStroke = this->PolygonStroke;
  Style->PolygonDisplacementX = this->PolygonDisplacementX;
  Style->PolygonDisplacementY = this->PolygonDisplacementY;
  Style->PolygonPerpendicularOffset = this->PolygonPerpendicularOffset;
  Style->PolygonFillOpacity = this->PolygonFillOpacity;
  strcpy(Style->PolygonFillColor, this->PolygonFillColor);
  Style->PolygonSolidFill = this->PolygonSolidFill;
  Style->PolygonFillBrushId = this->PolygonFillBrushId;
  Style->PolygonStrokeOpacity = this->PolygonStrokeOpacity;
  Style->PolygonStrokeWidth = this->PolygonStrokeWidth;
  strcpy(Style->PolygonStrokeColor, this->PolygonStrokeColor);
  Style->LabelLinePlacement = this->LabelLinePlacement;
  Style->LabelPrint = this->LabelPrint;
  Style->DontPaintGeomSymbolizer = this->DontPaintGeomSymbolizer;
  if (this->LabelColumn == NULL)
    Style->LabelColumn = NULL;
  else
    {
      int len = strlen(this->LabelColumn);
      Style->LabelColumn = (char *) malloc(len + 1);
      strcpy(Style->LabelColumn, this->LabelColumn);
    }
  if (this->FontFacename == NULL)
    Style->FontFacename = NULL;
  else
    {
      int len = strlen(this->FontFacename);
      Style->FontFacename = (char *) malloc(len + 1);
      strcpy(Style->FontFacename, this->FontFacename);
    }
  Style->FontSize = this->FontSize;
  Style->FontStyle = this->FontStyle;
  Style->FontWeight = this->FontWeight;
  Style->FontOpacity = this->FontOpacity;
  strcpy(Style->FontColor, this->FontColor);
  Style->HasHalo = this->HasHalo;
  Style->HaloRadius = this->HaloRadius;
  Style->HaloOpacity = this->HaloOpacity;
  strcpy(Style->HaloColor, this->HaloColor);
  Style->LabelAnchorX = this->LabelAnchorX;
  Style->LabelAnchorY = this->LabelAnchorY;
  Style->LabelDisplacementX = this->LabelDisplacementX;
  Style->LabelDisplacementY = this->LabelDisplacementY;
  Style->LabelRotation = this->LabelRotation;
  Style->LabelPerpendicularOffset = this->LabelPerpendicularOffset;
  Style->RepeatedLabel = this->RepeatedLabel;
  Style->LabelInitialGap = this->LabelInitialGap;
  Style->LabelGap = this->LabelGap;
  Style->LabelIsAligned = this->LabelIsAligned;
  Style->LabelGeneralizeLine = this->LabelGeneralizeLine;
  Style->XmlStyle = NULL;
  return Style;
}

bool QuickStyleObj::Compare(QuickStyleObj * Style)
{
//
// comparing two Quick Style objects
//
  if (Style == NULL)
    return false;
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
  if (Style->SymbolOpacity != this->SymbolOpacity)
    return false;
  if (Style->SymbolSize != this->SymbolSize)
    return false;
  if (Style->SymbolRotation != this->SymbolRotation)
    return false;
  if (Style->SymbolAnchorX != this->SymbolAnchorX)
    return false;
  if (Style->SymbolAnchorY != this->SymbolAnchorY)
    return false;
  if (Style->SymbolDisplacementX != this->SymbolDisplacementX)
    return false;
  if (Style->SymbolDisplacementY != this->SymbolDisplacementY)
    return false;
  if (Style->SymbolWellKnownMark != this->SymbolWellKnownMark)
    return false;
  if (strcmp(Style->SymbolFillColor, this->SymbolFillColor) != 0)
    return false;
  if (strcmp(Style->SymbolStrokeColor, this->SymbolStrokeColor) != 0)
    return false;
  if (Style->LineOpacity != this->LineOpacity)
    return false;
  if (Style->LinePerpendicularOffset != this->LinePerpendicularOffset)
    return false;
  if (Style->LineStrokeWidth != this->LineStrokeWidth)
    return false;
  if (strcmp(Style->LineStrokeColor, this->LineStrokeColor) != 0)
    return false;
  if (Style->LineDotStyle != this->LineDotStyle)
    return false;
  if (Style->Line2Enabled != this->Line2Enabled)
    return false;
  if (Style->Line2StrokeWidth != this->Line2StrokeWidth)
    return false;
  if (strcmp(Style->Line2StrokeColor, this->Line2StrokeColor) != 0)
    return false;
  if (Style->Line2DotStyle != this->Line2DotStyle)
    return false;
  if (Style->PolygonFill != this->PolygonFill)
    return false;
  if (Style->PolygonStroke != this->PolygonStroke)
    return false;
  if (Style->PolygonDisplacementX != this->PolygonDisplacementX)
    return false;
  if (Style->PolygonDisplacementY != this->PolygonDisplacementY)
    return false;
  if (Style->PolygonPerpendicularOffset != this->PolygonPerpendicularOffset)
    return false;
  if (Style->PolygonFillOpacity != this->PolygonFillOpacity)
    return false;
  if (strcmp(Style->PolygonFillColor, this->PolygonFillColor) != 0)
    return false;
  if (Style->PolygonSolidFill != this->PolygonSolidFill)
    return false;
  if (Style->PolygonSolidFill == false && this->PolygonSolidFill == false)
    {
      if (Style->PolygonFillBrushId != this->PolygonFillBrushId)
        return false;
    }
  if (Style->PolygonStrokeOpacity != this->PolygonStrokeOpacity)
    return false;
  if (Style->PolygonStrokeWidth != this->PolygonStrokeWidth)
    return false;
  if (strcmp(Style->PolygonStrokeColor, this->PolygonStrokeColor) != 0)
    return false;
  if (Style->LabelLinePlacement != this->LabelLinePlacement)
    return false;
  if (Style->LabelPrint != this->LabelPrint)
    return false;
  if (Style->DontPaintGeomSymbolizer != this->DontPaintGeomSymbolizer)
    return false;
  if (Style->LabelColumn == NULL && this->LabelColumn == NULL)
    ;
  else if (Style->LabelColumn == NULL && this->LabelColumn != NULL)
    return false;
  else if (Style->LabelColumn != NULL && this->LabelColumn == NULL)
    return false;
  else if (strcmp(Style->LabelColumn, this->LabelColumn) != 0)
    return false;
  if (Style->FontFacename == NULL && this->FontFacename == NULL)
    ;
  else if (Style->FontFacename == NULL && this->FontFacename != NULL)
    return false;
  else if (Style->FontFacename != NULL && this->FontFacename == NULL)
    return false;
  else if (strcmp(Style->FontFacename, this->FontFacename) != 0)
    return false;
  if (Style->FontSize != this->FontSize)
    return false;
  if (Style->FontStyle != this->FontStyle)
    return false;
  if (Style->FontWeight != this->FontWeight)
    return false;
  if (Style->FontOpacity != this->FontOpacity)
    return false;
  if (strcmp(Style->FontColor, this->FontColor) != 0)
    return false;
  if (Style->HasHalo != this->HasHalo)
    return false;
  if (Style->HaloRadius != this->HaloRadius)
    return false;
  if (Style->HaloOpacity != this->HaloOpacity)
    return false;
  if (strcmp(Style->HaloColor, this->HaloColor) != 0)
    return false;
  if (Style->LabelAnchorX != this->LabelAnchorX)
    return false;
  if (Style->LabelAnchorY != this->LabelAnchorY)
    return false;
  if (Style->LabelDisplacementX != this->LabelDisplacementX)
    return false;
  if (Style->LabelDisplacementY != this->LabelDisplacementY)
    return false;
  if (Style->LabelRotation != this->LabelRotation)
    return false;
  if (Style->LabelPerpendicularOffset != this->LabelPerpendicularOffset)
    return false;
  if (Style->RepeatedLabel != this->RepeatedLabel)
    return false;
  if (Style->LabelInitialGap != this->LabelInitialGap)
    return false;
  if (Style->LabelGap != this->LabelGap)
    return false;
  if (Style->LabelIsAligned != this->LabelIsAligned)
    return false;
  if (Style->LabelGeneralizeLine != this->LabelGeneralizeLine)
    return false;
  return true;
}

void QuickStyleObj::SetLabelColumn(const char *x)
{
//
// setting the Label Column
//
  int len;
  if (LabelColumn != NULL)
    free(LabelColumn);
  LabelColumn = NULL;
  if (x == NULL)
    return;
  len = strlen(x);
  LabelColumn = (char *) malloc(len + 1);
  strcpy(LabelColumn, x);
}

void QuickStyleObj::SetFontFacename(const char *x)
{
//
// setting the Font Facename
//
  int len;
  if (FontFacename != NULL)
    free(FontFacename);
  FontFacename = NULL;
  if (x == NULL)
    return;
  len = strlen(x);
  FontFacename = (char *) malloc(len + 1);
  strcpy(FontFacename, x);
}

void QuickStyleObj::UpdateXmlStyle()
{
//
// updating the XML Style
//
  if (XmlStyle != NULL)
    sqlite3_free(XmlStyle);
  XmlStyle = CreateXmlStyle();
}

char *QuickStyleObj::CreateXmlStyle()
{
//
// creating the XML Style
//
  char *xml;
  if ((MinScaleEnabled == true || MaxScaleEnabled == true) || LabelPrint == true
      || Type == QUICK_STYLE_GEOMETRY || Line2Enabled == true)
    xml = DoCreateFeatureTypeXML();
  else
    xml = DoCreateSymbolizerXML(false);
  return xml;

}

char *QuickStyleObj::DoCreateFeatureTypeXML()
{
//
// creating a FeatureType XML Style
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
  xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, "Quick Style");
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
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         ScaleMin);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScaleEnabled == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         ScaleMax);
      sqlite3_free(prev);
      prev = xml;
    }
  xml2 = NULL;
  if (Type == QUICK_STYLE_POINT)
    xml2 = DoCreatePointSymbolizerXML(true);
  else if (Type == QUICK_STYLE_LINE)
    xml2 = DoCreateLineSymbolizerXML(true);
  else if (Type == QUICK_STYLE_POLYGON)
    xml2 = DoCreatePolygonSymbolizerXML(true);
  else
    {
      // mixed-type Symbolizers
      xml2 = DoCreatePointSymbolizerXML(true);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      xml2 = DoCreateLineSymbolizerXML(true);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
      xml2 = DoCreatePolygonSymbolizerXML(true);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          xml2 = NULL;
          prev = xml;
        }
    }
  if (xml2 != NULL)
    {
      xml = sqlite3_mprintf("%s%s", prev, xml2);
      sqlite3_free(prev);
      sqlite3_free(xml2);
      prev = xml;
    }
  if (LabelPrint == true)
    {
      // adding a Text Symbolizer
      xml2 = NULL;
      if (Type == QUICK_STYLE_POINT || Type == QUICK_STYLE_POLYGON)
        xml2 = DoCreateTextPointSymbolizerXML();
      if (Type == QUICK_STYLE_LINE)
        xml2 = DoCreateTextLineSymbolizerXML();
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
          prev = xml;
        }
    }
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</FeatureTypeStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleObj::DoCreateSymbolizerXML(bool subordered)
{
//
// creating a Symbolizer XML Style
//
  if (Type == QUICK_STYLE_POINT)
    return DoCreatePointSymbolizerXML(subordered);
  if (Type == QUICK_STYLE_LINE)
    return DoCreateLineSymbolizerXML(subordered);
  if (Type == QUICK_STYLE_POLYGON)
    return DoCreatePolygonSymbolizerXML(subordered);
  return NULL;
}

char *QuickStyleObj::DoCreatePointSymbolizerXML(bool subordered)
{
//
// creating a Point Symbolizer XML Style
//
  const char *cstr;
  char *prev;
  const char *extra = "";
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  if (subordered == true)
    {
      extra = "\t\t";
      sqlite3_free(xml);
      prev = (char *) "";
      xml = sqlite3_mprintf("%s%s<PointSymbolizer>\r\n", prev, extra);
  } else
    {
      prev = xml;
      xml = sqlite3_mprintf("%s<PointSymbolizer version=\"1.1.0\" ", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%sxsi:schemaLocation=\"http://www.opengis.net/se http://schemas.opengis.net/se/1.1.0/Symbolizer.xsd\" ",
         prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf
        ("%sxmlns=\"http://www.opengis.net/se\" xmlns:ogc=\"http://www.opengis.net/ogc\" ",
         prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ",
                        prev);
      sqlite3_free(prev);
      prev = xml;
      cstr = "http://www.opengeospatial.org/se/units/pixel";
      xml =
        sqlite3_mprintf
        ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" uom=\"%s\">\r\n",
         prev, cstr);
      sqlite3_free(prev);
    }
  prev = xml;
  if (subordered != true)
    {
      xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, UUID);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev,
                        "Quick Style - PointSymbolizer");
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
    }
  xml = sqlite3_mprintf("%s%s\t<Graphic>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// mark symbol
  xml = sqlite3_mprintf("%s%s\t\t<Mark>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
  switch (SymbolWellKnownMark)
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
     prev, extra, SymbolFillColor);
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
     prev, extra, SymbolStrokeColor);
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
  if (SymbolOpacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Opacity>%1.2f</Opacity>\r\n", prev, extra,
                        SymbolOpacity);
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf("%s%s\t\t<Size>%1.2f</Size>\r\n", prev, extra, SymbolSize);
  sqlite3_free(prev);
  prev = xml;
  if (SymbolRotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s%s\t\t<Rotation>%1.2f</Rotation>\r\n", prev, extra,
                        SymbolRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (SymbolAnchorX != 0.5 || SymbolAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s%s\t\t<AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, extra, SymbolAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, extra, SymbolAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t\t</AnchorPoint>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (SymbolDisplacementX != 0.0 || SymbolDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, extra, SymbolDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, extra, SymbolDisplacementY);
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
  return xml;
}

char *QuickStyleObj::DoCreateLineSymbolizerXML(bool subordered)
{
//
// creating a Line Symbolizer XML Style
//
  const char *cstr;
  char *prev;
  const char *extra = "";
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  if (subordered == true)
    {
      extra = "\t\t";
      sqlite3_free(xml);
      prev = (char *) "";
      xml = sqlite3_mprintf("%s%s<LineSymbolizer>\r\n", prev, extra);
  } else
    {
      xml = sqlite3_mprintf("%s<LineSymbolizer version=\"1.1.0\" ", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%sxsi:schemaLocation=\"http://www.opengis.net/se http://schemas.opengis.net/se/1.1.0/Symbolizer.xsd\" ",
         prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf
        ("%sxmlns=\"http://www.opengis.net/se\" xmlns:ogc=\"http://www.opengis.net/ogc\" ",
         prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ",
                        prev);
      sqlite3_free(prev);
      prev = xml;
      cstr = "http://www.opengeospatial.org/se/units/pixel";
      xml =
        sqlite3_mprintf
        ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" uom=\"%s\">\r\n",
         prev, cstr);
      sqlite3_free(prev);
    }
  prev = xml;
  if (subordered != true)
    {
      xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, UUID);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev,
                        "Quick Style - LineSymbolizer");
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
    }
  xml = sqlite3_mprintf("%s%s\t<Stroke>\r\n", prev, extra);
  sqlite3_free(prev);
  prev = xml;
// using a Solid Color
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev, extra,
     LineStrokeColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
     prev, extra, LineOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, extra, LineStrokeWidth);
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
  switch (LineDotStyle)
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
  if (LinePerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n", prev,
         extra, LinePerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s</LineSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  if (Line2Enabled == true && subordered == true)
    {
      // defining a second LineSymbolizer - double pass style
      extra = "\t\t";
      prev = xml;
      xml = sqlite3_mprintf("%s%s<LineSymbolizer>\r\n", prev, extra);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t<Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
// using a Solid Color
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
         extra, Line2StrokeColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, extra, LineOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, extra, Line2StrokeWidth);
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
      switch (Line2DotStyle)
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
      if (LinePerpendicularOffset != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s%s\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n", prev,
             extra, LinePerpendicularOffset);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s%s</LineSymbolizer>\r\n", prev, extra);
      sqlite3_free(prev);
    }
  return xml;
}

char *QuickStyleObj::DoCreatePolygonSymbolizerXML(bool subordered)
{
//
// creating a Polygon Symbolizer XML Style
//
  const char *cstr;
  char *prev;
  const char *extra = "";
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  if (subordered == true)
    {
      extra = "\t\t";
      sqlite3_free(xml);
      prev = (char *) "";
      xml = sqlite3_mprintf("%s%s<PolygonSymbolizer>\r\n", prev, extra);
  } else
    {
      xml = sqlite3_mprintf("%s<PolygonSymbolizer version=\"1.1.0\" ", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%sxsi:schemaLocation=\"http://www.opengis.net/se http://schemas.opengis.net/se/1.1.0/Symbolizer.xsd\" ",
         prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf
        ("%sxmlns=\"http://www.opengis.net/se\" xmlns:ogc=\"http://www.opengis.net/ogc\" ",
         prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ",
                        prev);
      sqlite3_free(prev);
      prev = xml;
      cstr = "http://www.opengeospatial.org/se/units/pixel";
      xml =
        sqlite3_mprintf
        ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" uom=\"%s\">\r\n",
         prev, cstr);
      sqlite3_free(prev);
    }
  prev = xml;
  if (subordered != true)
    {
      xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, UUID);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev,
                        "Quick Style - Polygon Symbolizer");
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
    }
  if (PolygonFill == true)
    {
      // Polygon Fill
      xml = sqlite3_mprintf("%s%s\t<Fill>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      if (PolygonSolidFill)
        {
          // using a Solid Color
          xml =
            sqlite3_mprintf
            ("%s%s\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev,
             extra, PolygonFillColor);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
             prev, extra, PolygonFillOpacity);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a pattern brush
          const char *brush_name;
          switch (PolygonFillBrushId)
            {
              case GRAPHIC_BRUSH_VERT:
                brush_name = "vert";
                break;
              case GRAPHIC_BRUSH_CROSS:
                brush_name = "cross";
                break;
              case GRAPHIC_BRUSH_DIAG1:
                brush_name = "diag1";
                break;
              case GRAPHIC_BRUSH_DIAG2:
                brush_name = "diag2";
                break;
              case GRAPHIC_BRUSH_CROSS_DIAG:
                brush_name = "crossdiag";
                break;
              case GRAPHIC_BRUSH_DOTS:
                brush_name = "dots";
                break;
              default:
                brush_name = "horz";
                break;
            };
          xml = sqlite3_mprintf("%s%s\t\t<GraphicFill>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t<Graphic>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t<ExternalGraphic>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"http://www.utopia.gov/stdbrush_%s.png\" />\r\n",
             prev, extra, brush_name);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t<Format>image/png</Format>\r\n",
                            prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t<ColorReplacement>\r\n", prev,
                            extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n", prev,
             extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s%s\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n",
             prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t\t<MapItem>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t<Data>1</Data>\r\n", prev,
                            extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n", prev,
                            extra, PolygonFillColor);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t\t\t</MapItem>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t\t\t\t</Recode>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t\t</ColorReplacement>\r\n", prev,
                            extra);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s%s\t\t\t\t</ExternalGraphic>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t\t</Graphic>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s%s\t\t</GraphicFill>\r\n", prev, extra);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s%s\t</Fill>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (PolygonStroke == true)
    {
      // Polygon Stroke
      xml = sqlite3_mprintf("%s%s\t<Stroke>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      // using a Solid Color
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
         extra, PolygonStrokeColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, extra, PolygonStrokeOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, extra, PolygonStrokeWidth);
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
  if (PolygonDisplacementX != 0.0 || PolygonDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s%s\t<Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, extra, PolygonDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s%s\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, extra, PolygonDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s%s\t</Displacement>\r\n", prev, extra);
      sqlite3_free(prev);
      prev = xml;
    }
  if (PolygonPerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s%s\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n", prev,
         extra, PolygonPerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s%s</PolygonSymbolizer>\r\n", prev, extra);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleObj::DoCreateTextPointSymbolizerXML()
{
//
// creating a Text Symbolizer (Point Placement)
//
  char *xml;
  char *prev;
  xml = sqlite3_mprintf("\t\t<TextSymbolizer>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Label>@%s@</Label>\r\n", prev, LabelColumn);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  const char *font;
  if (strcmp(FontFacename, "ToyFont: serif") == 0)
    font = "serif";
  else if (strcmp(FontFacename, "ToyFont: sans-serif") == 0)
    font = "sans serif";
  else if (strcmp(FontFacename, "ToyFont: monospace") == 0)
    font = "monospace";
  else
    font = FontFacename;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"font-family\">%s</SvgParameter>\r\n", prev,
     font);
  sqlite3_free(prev);
  prev = xml;
  if (FontStyle == RL2_FONTSTYLE_ITALIC)
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-style\">italic</SvgParameter>\r\n",
       prev);
  else if (FontStyle == RL2_FONTSTYLE_OBLIQUE)
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-style\">oblique</SvgParameter>\r\n",
       prev);
  else
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-style\">normal</SvgParameter>\r\n",
       prev);
  sqlite3_free(prev);
  prev = xml;
  if (FontWeight == RL2_FONTWEIGHT_BOLD)
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-weight\">bold</SvgParameter>\r\n",
       prev);
  else
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-weight\">normal</SvgParameter>\r\n",
       prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"font-size\">%1.2f</SvgParameter>\r\n",
     prev, FontSize);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<LabelPlacement>\n", prev);
  sqlite3_free(prev);
  prev = xml;
// PointPlacement
  xml = sqlite3_mprintf("%s\t\t\t\t<PointPlacement>\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (LabelAnchorX != 0.5 || LabelAnchorY != 0.5)
    {
      xml = sqlite3_mprintf("%s\t\t\t\t\t<AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n", prev,
         LabelAnchorX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n", prev,
         LabelAnchorY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t</AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (LabelDisplacementX != 0.0 || LabelDisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
         LabelDisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
         LabelDisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t</Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (LabelRotation != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev, LabelRotation);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t\t\t</PointPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasHalo == true)
    {
      // Halo
      xml = sqlite3_mprintf("%s\t\t\t<Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<Radius>%1.2f</Radius>\r\n", prev,
                        HaloRadius);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
         prev, HaloColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, HaloOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t\t<Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev,
     FontColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
     prev, FontOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</TextSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleObj::DoCreateTextLineSymbolizerXML()
{
//
// creating a Text Symbolizer (Line Placement)
//
  char *xml;
  char *prev;
  xml = sqlite3_mprintf("\t\t<TextSymbolizer>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Label>@%s@</Label>\r\n", prev, LabelColumn);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  const char *font;
  if (strcmp(FontFacename, "ToyFont: serif") == 0)
    font = "serif";
  else if (strcmp(FontFacename, "ToyFont: sans-serif") == 0)
    font = "sans serif";
  else if (strcmp(FontFacename, "ToyFont: monospace") == 0)
    font = "monospace";
  else
    font = FontFacename;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"font-family\">%s</SvgParameter>\r\n", prev,
     font);
  sqlite3_free(prev);
  prev = xml;
  if (FontStyle == RL2_FONTSTYLE_ITALIC)
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-style\">italic</SvgParameter>\r\n",
       prev);
  else if (FontStyle == RL2_FONTSTYLE_OBLIQUE)
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-style\">oblique</SvgParameter>\r\n",
       prev);
  else
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-style\">normal</SvgParameter>\r\n",
       prev);
  sqlite3_free(prev);
  prev = xml;
  if (FontWeight == RL2_FONTWEIGHT_BOLD)
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-weight\">bold</SvgParameter>\r\n",
       prev);
  else
    xml =
      sqlite3_mprintf
      ("%s\t\t\t\t<SvgParameter name=\"font-weight\">normal</SvgParameter>\r\n",
       prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"font-size\">%1.2f</SvgParameter>\r\n",
     prev, FontSize);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t</Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<LabelPlacement>\n", prev);
  sqlite3_free(prev);
  prev = xml;
// LinePlacement
  xml = sqlite3_mprintf("%s\t\t\t\t<LinePlacement>\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (LabelPerpendicularOffset != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n",
         prev, LabelPerpendicularOffset);
      sqlite3_free(prev);
      prev = xml;
    }
  if (RepeatedLabel == true)
    {
      // Repeated: InitialGap and Gap
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<IsRepeated>true</IsRepeated>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<InitialGap>%1.4f</InitialGap>\r\n", prev,
         LabelInitialGap);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Gap>%1.4f</Gap>\r\n", prev, LabelGap);
      sqlite3_free(prev);
      prev = xml;
    }
  if (LabelIsAligned == true)
    {
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<IsAligned>true</IsAligned>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (LabelGeneralizeLine == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<GeneralizeLine>true</GeneralizeLine>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t\t\t</LinePlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasHalo == true)
    {
      // Halo
      xml = sqlite3_mprintf("%s\t\t\t<Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<Radius>%1.2f</Radius>\r\n", prev,
                        HaloRadius);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
         prev, HaloColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, HaloOpacity);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t\t<Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev,
     FontColor);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
     prev, FontOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</TextSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

unsigned char *QuickStyleObj::CloneXmlStyle()
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

void QuickStyleObj::DoGetUUID(char *uuid)
{
//
// creating an UUID value
//
  unsigned char rnd[16];
  char *p = uuid;
  int i;
  sqlite3_randomness(16, rnd);
  for (i = 0; i < 16; i++)
    {
      if (i == 4 || i == 6 || i == 8 || i == 10)
        *p++ = '-';
      sprintf(p, "%02x", rnd[i]);
      p += 2;
    }
  *p = '\0';
  uuid[14] = '4';
  uuid[19] = '8';
}

int QuickStyleObj::RandomWellKnownMark()
{
//
// returning a random Symbol Well Known Mark
//
  int wkt;
  int mod = rand() % 6;
  switch (mod)
    {
      case 0:
        wkt = RL2_GRAPHIC_MARK_SQUARE;
        break;
      case 1:
        wkt = RL2_GRAPHIC_MARK_CIRCLE;
        break;
      case 2:
        wkt = RL2_GRAPHIC_MARK_TRIANGLE;
        break;
      case 3:
        wkt = RL2_GRAPHIC_MARK_STAR;
        break;
      case 4:
        wkt = RL2_GRAPHIC_MARK_CROSS;
        break;
      default:
        wkt = RL2_GRAPHIC_MARK_X;
        break;
    };
  return wkt;
}

void QuickStyleObj::RandomColor(char *color)
{
//
// setting a random Color
//
  unsigned char red = rand() % 256;
  unsigned char green = rand() % 256;
  unsigned char blue = rand() % 256;
  sprintf(color, "#%02x%02x%02x", red, green, blue);
}

bool QuickStyleVectorDialog::Create(MyMapPanel * parent, MapLayer * layer,
                                    int type)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  Type = type;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  IsConfigChanged = false;
  CheckStandardBrushes();

  if (wxPropertySheetDialog::Create
      (parent, wxID_ANY, wxT("QuickStyle (Vector) Edit")) == false)
    return false;
  if (Layer->GetQuickStyle() != NULL)
    Style = Layer->CloneQuickStyle();
  else
    Style = new QuickStyleObj(Type);

  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  int next = 1;
  PagePointIndex = 0;
  PageLineIndex = 0;
  PagePolygonIndex = 0;
  PageTextPointIndex = 0;
  PageTextLineIndex = 0;
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  if (Type == QUICK_STYLE_POINT || Type == QUICK_STYLE_GEOMETRY)
    {
      wxPanel *pointPage = CreatePointPage(book);
      book->AddPage(pointPage, wxT("Point Symbolizer"), false);
      PagePointIndex = next++;
    }
  if (Type == QUICK_STYLE_LINE || Type == QUICK_STYLE_GEOMETRY)
    {
      wxPanel *linePage = CreateLinePage(book);
      book->AddPage(linePage, wxT("Line Symbolyzer"), false);
      PageLineIndex = next++;
    }
  if (Type == QUICK_STYLE_POLYGON || Type == QUICK_STYLE_GEOMETRY)
    {
      wxPanel *polygonPage = CreatePolygonPage(book);
      book->AddPage(polygonPage, wxT("Polygon Symbolizer"), false);
      PagePolygonIndex = next++;
    }
  if (Type == QUICK_STYLE_POINT || Type == QUICK_STYLE_POLYGON)
    {
      wxPanel *textPointPage = CreateTextPointPage(book);
      book->AddPage(textPointPage, wxT("Text Symbolizer"), false);
      PageTextPointIndex = next++;
    }
  if (Type == QUICK_STYLE_LINE)
    {
      wxPanel *textLinePage = CreateTextLinePage(book);
      book->AddPage(textLinePage, wxT("Text Symbolizer"), false);
      PageTextLineIndex = next++;
    }

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnQuit);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnOk);
  Connect(ID_QUICK_STYLE_APPLY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnApply);
  Connect(ID_QUICK_STYLE_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnExport);
  Connect(ID_QUICK_STYLE_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

void QuickStyleVectorDialog::CreateButtons()
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

wxPanel *QuickStyleVectorDialog::CreateMainPage(wxWindow * parent)
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
  if (Type == QUICK_STYLE_POINT)
    typeName = wxT("POINT-Type Geometries");
  if (Type == QUICK_STYLE_LINE)
    typeName = wxT("LINE-Type Geometries");
  if (Type == QUICK_STYLE_POLYGON)
    typeName = wxT("POLYGON-Type Geometries");
  if (Type == QUICK_STYLE_GEOMETRY)
    typeName = wxT("MIXED-Type Geometries");
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
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnCmdScaleChanged);
  return panel;
}

void QuickStyleVectorDialog::
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

wxPanel *QuickStyleVectorDialog::CreatePointPage(wxWindow * parent)
{
//
// creating the Point Symbolizer page
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
    new wxSlider(panel, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 50),
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
  wxRadioBox *markBox = new wxRadioBox(panel, ID_SYMBOLIZER_MARK,
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
  wxTextCtrl *sizeCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_SIZE, wxT("16.0"),
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_ROTATION, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  rot1Sizer->Add(rotCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: AnchorPoint and Displacement
  wxBoxSizer *anchorBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  box2Sizer->Add(anchorBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row A: Anchor Point
  wxStaticBox *anchorBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Anchor Point"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *anchorSizer = new wxStaticBoxSizer(anchorBox, wxVERTICAL);
  anchorBoxSizer->Add(anchorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
  wxBoxSizer *anchor1Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  anchor1Sizer->Add(anchor1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_ANCHOR_X, wxT("0.5"),
                   wxDefaultPosition, wxSize(100, 22));
  anchor1Sizer->Add(anchorXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *anchor2Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  anchor2Sizer->Add(anchor2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_ANCHOR_Y, wxT("0.5"),
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_DISPLACEMENT_X, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_DISPLACEMENT_Y, wxT("0.0"),
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_FILL_COLOR, FillColor,
                   wxDefaultPosition, wxSize(80, 22));
  fill1Sizer->Add(fillColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleFillCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  fill1Sizer->Add(sampleFillCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickFill =
    new wxButton(panel, ID_SYMBOLIZER_FILL_PICKER_BTN, wxT("&Pick a color"));
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  stroke1Sizer->Add(strokeColorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sampleStrokeCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  stroke1Sizer->Add(sampleStrokeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pickStroke =
    new wxButton(panel, ID_SYMBOLIZER_STROKE_PICKER_BTN, wxT("&Pick a color"));
  stroke1Sizer->Add(pickStroke, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_MARK, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnCmdMarkChanged);
  Connect(ID_SYMBOLIZER_FILL_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPointColorFillPicker);
  Connect(ID_SYMBOLIZER_FILL_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPointColorFillChanged);
  Connect(ID_SYMBOLIZER_STROKE_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPointColorStrokePicker);
  Connect(ID_SYMBOLIZER_STROKE_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPointColorStrokeChanged);
  return panel;
}

void QuickStyleVectorDialog::OnCmdMarkChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Mark selection changed
//
  wxRadioBox *markCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MARK);
  switch (markCtrl->GetSelection())
    {
      case 1:
        Style->SetSymbolWellKnownMark(RL2_GRAPHIC_MARK_CIRCLE);
        break;
      case 2:
        Style->SetSymbolWellKnownMark(RL2_GRAPHIC_MARK_TRIANGLE);
        break;
      case 3:
        Style->SetSymbolWellKnownMark(RL2_GRAPHIC_MARK_STAR);
        break;
      case 4:
        Style->SetSymbolWellKnownMark(RL2_GRAPHIC_MARK_CROSS);
        break;
      case 5:
        Style->SetSymbolWellKnownMark(RL2_GRAPHIC_MARK_X);
        break;
      default:
        Style->SetSymbolWellKnownMark(RL2_GRAPHIC_MARK_SQUARE);
        break;
    };
}

void QuickStyleVectorDialog::CheckStandardBrushes()
{
//
// testing which of the standard brushes are currently available
//
  int ret;
  char **results;
  int rows;
  int columns;
  int i;

  HasStandardBrushes = false;
  HasStandardBrushHorz = false;
  HasStandardBrushVert = false;
  HasStandardBrushCross = false;
  HasStandardBrushDiag1 = false;
  HasStandardBrushDiag2 = false;
  HasStandardBrushCrossDiag = false;
  HasStandardBrushDots = false;

  const char *sql = "SELECT xlink_href FROM main.SE_external_graphics "
    "WHERE xlink_href LIKE 'http://www.utopia.gov/stdbrush_%.png'";
  ret = sqlite3_get_table(MainFrame->GetSqlite(), sql, &results,
                          &rows, &columns, NULL);
  if (ret != SQLITE_OK)
    return;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *value = results[(i * columns) + 0];
          if (value != NULL)
            {
              if (strcmp(value, "http://www.utopia.gov/stdbrush_horz.png") == 0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushHorz = true;
                }
              if (strcmp(value, "http://www.utopia.gov/stdbrush_vert.png") == 0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushVert = true;
                }
              if (strcmp(value, "http://www.utopia.gov/stdbrush_cross.png") ==
                  0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushCross = true;
                }
              if (strcmp(value, "http://www.utopia.gov/stdbrush_diag1.png") ==
                  0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushDiag1 = true;
                }
              if (strcmp(value, "http://www.utopia.gov/stdbrush_diag2.png") ==
                  0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushDiag2 = true;
                }
              if (strcmp(value, "http://www.utopia.gov/stdbrush_crossdiag.png")
                  == 0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushCrossDiag = true;
                }
              if (strcmp(value, "http://www.utopia.gov/stdbrush_dots.png") == 0)
                {
                  HasStandardBrushes = true;
                  HasStandardBrushDots = true;
                }
            }
        }
    }
  sqlite3_free_table(results);
}

void QuickStyleVectorDialog::
OnCmdPointColorFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FILL_PICKER_HEX);
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

void QuickStyleVectorDialog::
OnCmdPointColorFillPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
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

void QuickStyleVectorDialog::
OnCmdPointColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE_PICKER_HEX);
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

void QuickStyleVectorDialog::
OnCmdPointColorStrokePicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_COLOR);
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

wxPanel *QuickStyleVectorDialog::CreateLinePage(wxWindow * parent)
{
//
// creating the Line Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxPanel *panel = new wxPanel(parent, ID_PANE_LINE);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #1 Opacity and Perpendicular Offset
  //boxSizer->AddSpacer(50);
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
                 wxDefaultPosition, wxSize(600, 50),
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
// second row: Stroke color or Graphic - Pass #1
  wxBoxSizer *stroke0BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(stroke0BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *stroke0Box = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("First pass"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *stroke0Sizer = new wxStaticBoxSizer(stroke0Box, wxVERTICAL);
  stroke0BoxSizer->Add(stroke0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row B: Stroke Color - Pass #1
  wxBoxSizer *color0BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  stroke0Sizer->Add(color0BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *color0Box = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke Color"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *color0Sizer = new wxStaticBoxSizer(color0Box, wxVERTICAL);
  color0BoxSizer->Add(color0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *color01Sizer = new wxBoxSizer(wxHORIZONTAL);
  color0Sizer->Add(color01Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *color0Ctrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  color01Sizer->Add(color0Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sample0Ctrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE1_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color01Sizer->Add(sample0Ctrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *picker0Sizer = new wxBoxSizer(wxHORIZONTAL);
  color0Sizer->Add(picker0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick0 =
    new wxButton(panel, ID_SYMBOLIZER_STROKE1_PICKER_BTN, wxT("&Pick a color"));
  picker0Sizer->Add(pick0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row: Stroke-Width 
  wxBoxSizer *misc0Sizer = new wxBoxSizer(wxHORIZONTAL);
  color0BoxSizer->Add(misc0Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row A: StrokeWidth
  wxBoxSizer *width0BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  misc0Sizer->Add(width0BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *width0Box = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke Width"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *width0Sizer = new wxStaticBoxSizer(width0Box, wxVERTICAL);
  width0BoxSizer->Add(width0Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *width0Ctrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  width0Sizer->Add(width0Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row D: DashArray and DashOffset
  wxBoxSizer *dash0BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  misc0Sizer->Add(dash0BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString dot[4];
  dot[0] = wxT("&Solid Line");
  dot[1] = wxT("&Dotted Line");
  dot[2] = wxT("&Dashed Line");
  dot[3] = wxT("&Dashed/Dotted Line");
  wxRadioBox *dot0Box = new wxRadioBox(panel, ID_SYMBOLIZER_DASH_DOT,
                                       wxT("&Dash/Dot Style"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 4,
                                       dot, 2,
                                       wxRA_SPECIFY_COLS);
  dot0Box->SetSelection(0);
  dash0BoxSizer->Add(dot0Box, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row: Stroke color or Graphic - Pass #2
  wxBoxSizer *stroke1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(stroke1BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *stroke1Box = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Second pass"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *stroke1Sizer = new wxStaticBoxSizer(stroke1Box, wxHORIZONTAL);
  stroke1BoxSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  stroke1Sizer->Add(enableSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableCtrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_SECOND_STROKE_ENABLE,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  enableCtrl->SetValue(false);
// second row B: Stroke Color - Pass #2
  wxBoxSizer *color1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  stroke1Sizer->Add(color1BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *color1Box = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke Color"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *color1Sizer = new wxStaticBoxSizer(color1Box, wxVERTICAL);
  color1BoxSizer->Add(color1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *color11Sizer = new wxBoxSizer(wxHORIZONTAL);
  color1Sizer->Add(color11Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *color1Ctrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_COLOR, StrokeColor,
                   wxDefaultPosition, wxSize(80, 22));
  color1Ctrl->Enable(false);
  color11Sizer->Add(color1Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  color = wxColour(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sample1Ctrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color11Sizer->Add(sample1Ctrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *picker1Sizer = new wxBoxSizer(wxHORIZONTAL);
  color1Sizer->Add(picker1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick1 =
    new wxButton(panel, ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxT("&Pick a color"));
  pick1->Enable(false);
  picker1Sizer->Add(pick1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row: Stroke-Width 
  wxBoxSizer *misc1Sizer = new wxBoxSizer(wxHORIZONTAL);
  color1BoxSizer->Add(misc1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row A: StrokeWidth
  wxBoxSizer *width1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  misc1Sizer->Add(width1BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *width1Box = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke Width"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *width1Sizer = new wxStaticBoxSizer(width1Box, wxVERTICAL);
  width1BoxSizer->Add(width1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *width1Ctrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  width1Ctrl->Enable(false);
  width1Sizer->Add(width1Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row D: DashArray and DashOffset
  wxBoxSizer *dash1BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  misc1Sizer->Add(dash1BoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxRadioBox *dot1Box = new wxRadioBox(panel, ID_SYMBOLIZER_DASH_DOT_2,
                                       wxT("&Dash/Dot Style"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 4,
                                       dot, 1,
                                       wxRA_SPECIFY_COLS);
  dot1Box->SetSelection(0);
  dot1Box->Enable(false);
  dash1BoxSizer->Add(dot1Box, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdLineColorChanged);
  Connect(ID_SYMBOLIZER_STROKE1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdLineColorPicker);
  Connect(ID_SYMBOLIZER_STROKE2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdLine2ColorChanged);
  Connect(ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdLine2ColorPicker);
  Connect(ID_SYMBOLIZER_SECOND_STROKE_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdSecondStrokEnableChanged);
  return panel;
}

void QuickStyleVectorDialog::
OnCmdSecondStrokEnableChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Enable double stroke changed
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_SECOND_STROKE_ENABLE);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_BTN);
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  wxRadioBox *dotBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT_2);
  if (enableCtrl->IsChecked() == true)
    {
      colorCtrl->Enable(true);
      pick->Enable(true);
      widthCtrl->Enable(true);
      dotBox->Enable(true);
  } else
    {
      colorCtrl->Enable(false);
      pick->Enable(false);
      widthCtrl->Enable(false);
      dotBox->Enable(false);
    }
}

void QuickStyleVectorDialog::
OnCmdLineColorChanged(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::
OnCmdLine2ColorChanged(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::
OnCmdLineColorPicker(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::
OnCmdLine2ColorPicker(wxCommandEvent & WXUNUSED(event))
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

wxPanel *QuickStyleVectorDialog::CreatePolygonPage(wxWindow * parent)
{
//
// creating the Polygon Symbolizer page
//
  wxString StrokeColor = wxT("#000000");
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_FILL2);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the Polygon Displacement and Perpendicular Offset
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
  boxSizer->Add(auxBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *auxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Polygon Fill"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *aux0Sizer = new wxStaticBoxSizer(auxBox, wxVERTICAL);
  auxBoxSizer->Add(aux0Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *auxSizer = new wxBoxSizer(wxHORIZONTAL);
  aux0Sizer->Add(auxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(enableSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_FILL2_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(Style->IsPolygonFill());
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  enableSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FILL2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(275, 50),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityCtrl->Enable(Style->IsPolygonFill());
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: Fill Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  auxSizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Fill Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 2);
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
  sampleCtrl->Enable(Style->IsPolygonFill());
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_FILL2_PICKER_BTN, wxT("&Pick a color"));
  pick->Enable(Style->IsPolygonFill());
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// FILL Style
  wxString style[2];
  style[0] = wxT("&Solid");
  style[1] = wxT("&Brush");
  wxRadioBox *styleBox = new wxRadioBox(panel, ID_SYMBOLIZER_FILL1_TYPE,
                                        wxT("&Fill Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 2,
                                        style, 1,
                                        wxRA_SPECIFY_COLS);
  auxSizer->Add(styleBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);
  styleBox->SetSelection(0);
  if (HasStandardBrushes == false)
    styleBox->Enable(false);
// FILL Brush selection
  wxString brush[7];
  brush[0] = wxT("&Horizontal");
  brush[1] = wxT("&Vertical");
  brush[2] = wxT("&Cross");
  brush[3] = wxT("&Diag 1");
  brush[4] = wxT("&Diag 2");
  brush[5] = wxT("&Cross Diag");
  brush[6] = wxT("&Dots");
  wxRadioBox *brushBox = new wxRadioBox(panel, ID_SYMBOLIZER_FILL2_TYPE,
                                        wxT("&Brush Style"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 7,
                                        brush, 1,
                                        wxRA_SPECIFY_ROWS);
  aux0Sizer->Add(brushBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  brushBox->SetSelection(0);
  brushBox->Enable(false);
// first row A: the Stroke #1 Opacity 
  wxBoxSizer *opacity2BoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacity2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *enableBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Polygon Stroke"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *enable2Sizer = new wxStaticBoxSizer(enableBox, wxHORIZONTAL);
  opacity2BoxSizer->Add(enable2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enable2Ctrl = new wxCheckBox(panel, ID_SYMBOLIZER_STROKE2_ENABLE,
                                           wxT("Enable"),
                                           wxDefaultPosition, wxDefaultSize);
  enable2Ctrl->SetValue(Style->IsPolygonStroke());
  enable2Sizer->Add(enable2Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *opacity2Box = new wxStaticBox(panel, wxID_STATIC,
                                             wxT("Opacity"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *opacity2Sizer = new wxStaticBoxSizer(opacity2Box, wxVERTICAL);
  enable2Sizer->Add(opacity2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacity2Ctrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(250, 50),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacity2Ctrl->Enable(Style->IsPolygonStroke());
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
  color2Ctrl->Enable(Style->IsPolygonStroke());
  color3Sizer->Add(color2Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sample2Ctrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  sample2Ctrl->Enable(Style->IsPolygonStroke());
  color3Sizer->Add(sample2Ctrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *picker2Sizer = new wxBoxSizer(wxHORIZONTAL);
  color2Sizer->Add(picker2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick2 =
    new wxButton(panel, ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxT("&Pick a color"));
  pick2->Enable(Style->IsPolygonStroke());
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
          QuickStyleVectorDialog::OnCmdPolygonStrokeChanged);
  Connect(ID_SYMBOLIZER_FILL2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonFillChanged);
  Connect(ID_SYMBOLIZER_FILL2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonColorFillChanged);
  Connect(ID_SYMBOLIZER_FILL2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonColorFillPicker);
  Connect(ID_SYMBOLIZER_STROKE2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonColorStrokeChanged);
  Connect(ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonColorStrokePicker);
  Connect(ID_SYMBOLIZER_FILL1_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonFillStyleChanged);
  Connect(ID_SYMBOLIZER_FILL2_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdPolygonFillBrushChanged);
  return panel;
}

void QuickStyleVectorDialog::
OnCmdPolygonStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    Style->SetPolygonStroke(true);
  else
    Style->SetPolygonStroke(false);
  RetrievePolygonPage(false);
  UpdatePolygonPage();
}

void QuickStyleVectorDialog::
OnCmdPolygonFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    Style->SetPolygonFill(true);
  else
    Style->SetPolygonFill(false);
  RetrievePolygonPage(false);
  UpdatePolygonPage();
}

void QuickStyleVectorDialog::
OnCmdPolygonColorFillChanged(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::
OnCmdPolygonFillStyleChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill style changed
//
  wxRadioBox *styleBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL1_TYPE);
  if (styleBox->GetSelection() == 0)
    Style->SetPolygonSolidFill(true);
  else
    Style->SetPolygonSolidFill(false);
  RetrievePolygonPage(false);
  UpdatePolygonPage();
}

void QuickStyleVectorDialog::
OnCmdPolygonFillBrushChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill brush changed
//
  wxRadioBox *brushBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL2_TYPE);
  Style->SetPolygonFillBrushId(brushBox->GetSelection());
  RetrievePolygonPage(false);
  UpdatePolygonPage();
}

void QuickStyleVectorDialog::
OnCmdPolygonColorFillPicker(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::
OnCmdPolygonColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::
OnCmdPolygonColorStrokePicker(wxCommandEvent & WXUNUSED(event))
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

bool QuickStyleVectorDialog::DoCheckDatasource(const char *prefix,
                                               const char *coverage,
                                               char *table, char *geometry)
{
//
// retrieving the real table and geometry column names
//
  sqlite3 *sqlite = MainFrame->GetSqlite();
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char *qprefix;
  bool ok = false;

  qprefix = gaiaDoubleQuotedSql(prefix);
  sql = sqlite3_mprintf("SELECT f_table_name, f_geometry_column, view_name, "
                        "view_geometry, virt_name, virt_geometry "
                        "FROM \"%s\".vector_coverages WHERE coverage_name = %Q",
                        qprefix, coverage);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return false;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *t = results[(i * columns) + 0];
          const char *g = results[(i * columns) + 1];
          if (t != NULL && g != NULL)
            {
              strcpy(table, t);
              strcpy(geometry, g);
              ok = true;
            }
          t = results[(i * columns) + 2];
          g = results[(i * columns) + 3];
          if (t != NULL && g != NULL)
            {
              strcpy(table, t);
              strcpy(geometry, g);
              ok = true;
            }
          t = results[(i * columns) + 4];
          g = results[(i * columns) + 5];
          if (t != NULL && g != NULL)
            {
              strcpy(table, t);
              strcpy(geometry, g);
              ok = true;
            }
        }
    }
  sqlite3_free_table(results);
  return ok;
}

void QuickStyleVectorDialog::InitializeComboColumns(wxComboBox * ctrl)
{
//
// initializing a Column list ComboBox
//
  sqlite3 *sqlite = MainFrame->GetSqlite();
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char prefix[1024];
  char coverage[1024];
  char table[1024];
  char geometry[1024];
  char *qprefix;
  char *qtable;

  if (DbPrefix.Len() == 0)
    strcpy(prefix, "MAIN");
  else
    strcpy(prefix, DbPrefix.ToUTF8());
  strcpy(coverage, LayerName.ToUTF8());
  if (DoCheckDatasource(prefix, coverage, table, geometry) != true)
    return;

  qprefix = gaiaDoubleQuotedSql(prefix);
  qtable = gaiaDoubleQuotedSql(table);
  sql = sqlite3_mprintf("PRAGMA \"%s\".table_info(\"%s\")", qprefix, qtable);
  free(qprefix);
  free(qtable);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *value = results[(i * columns) + 1];
          if (strcasecmp(value, geometry) == 0)
            continue;           // skipping the Geometry column
          wxString col = wxString::FromUTF8(value);
          ctrl->Append(col);
        }
    }
  sqlite3_free_table(results);
}

void QuickStyleVectorDialog::InitializeComboFonts(wxComboBox * ctrl)
{
//
// initializing a Font list ComboBox
//
  sqlite3 *sqlite = MainFrame->GetSqlite();
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int i;
  char prefix[1024];
  char *qprefix;

// inserting the default TOY FONTS
  wxString font = wxT("ToyFont: serif");
  ctrl->Append(font);
  font = wxT("ToyFont: sans-serif");
  ctrl->Append(font);
  font = wxT("ToyFont: monospace");
  ctrl->Append(font);

  if (DbPrefix.Len() == 0)
    strcpy(prefix, "MAIN");
  else
    strcpy(prefix, DbPrefix.ToUTF8());
  qprefix = gaiaDoubleQuotedSql(prefix);
  sql =
    sqlite3_mprintf
    ("SELECT font_facename FROM \"%s\".SE_fonts ORDER BY font_facename",
     qprefix);
  free(qprefix);
  ret = sqlite3_get_table(sqlite, sql, &results, &rows, &columns, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;
  if (rows < 1)
    ;
  else
    {
      for (i = 1; i <= rows; i++)
        {
          const char *value = results[(i * columns) + 0];
          wxString col = wxString::FromUTF8(value);
          ctrl->Append(col);
        }
    }
  sqlite3_free_table(results);
}

wxPanel *QuickStyleVectorDialog::CreateTextPointPage(wxWindow * parent)
{
//
// creating the Text Symbolizer page (Point Placement)
//
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_TEXT1);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// enable/disable Labels
  wxBoxSizer *auxBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(auxBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *auxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Labels"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *auxSizer = new wxStaticBoxSizer(auxBox, wxVERTICAL);
  auxBoxSizer->Add(auxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(enableSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_TEXT1_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(Style->IsLabelPrint());
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  enableSizer->AddSpacer(20);
  enableSizer->AddSpacer(20);
  enableSizer->AddSpacer(20);
  enableSizer->AddSpacer(20);
  wxStaticText *columnLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Column:"));
  enableSizer->Add(columnLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxComboBox *columnList =
    new wxComboBox(panel, ID_SYMBOLIZER_TEXT1_LABEL, wxT(""), wxDefaultPosition,
                   wxSize(400, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  InitializeComboColumns(columnList);
  enableSizer->Add(columnList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString noGeom =
    wxT("Don't paint the Geometry Symbolizer; Text Symbolizer alone");
  if (Type == QUICK_STYLE_POINT)
    noGeom = wxT("Don't paint the Point Symbolizer; Text Symbolizer alone");
  if (Type == QUICK_STYLE_POLYGON)
    noGeom = wxT("Don't paint the Polygon Symbolizer; Text Symbolizer alone");
  wxCheckBox *noGeomSymbolizerCtrl =
    new wxCheckBox(panel, ID_LABEL_NO_GEOM_SYMBOLIZER, noGeom,
                   wxDefaultPosition, wxDefaultSize);
  noGeomSymbolizerCtrl->SetValue(Style->IsDontPaintGeomSymbolizer());
  wxBoxSizer *noGeomSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(noGeomSizer, 0, wxALIGN_LEFT | wxALL, 5);
  noGeomSizer->Add(noGeomSymbolizerCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *fontBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *font9Sizer = new wxStaticBoxSizer(fontBox, wxHORIZONTAL);
  auxSizer->Add(font9Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Second row: Font Name, Size and Fill 
  wxBoxSizer *fontSizer = new wxBoxSizer(wxHORIZONTAL);
  font9Sizer->Add(fontSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *font0Sizer = new wxBoxSizer(wxHORIZONTAL);
  fontSizer->Add(font0Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Font Name
  wxBoxSizer *fontXSizer = new wxBoxSizer(wxVERTICAL);
  font0Sizer->Add(fontXSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxComboBox *fontList =
    new wxComboBox(panel, ID_SYMBOLIZER_FONT1_NAME, wxT(""), wxDefaultPosition,
                   wxSize(250, 21), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  InitializeComboFonts(fontList);
  fontList->Select(0);
  fontXSizer->Add(fontList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Font Size
  wxBoxSizer *fontYSizer = new wxBoxSizer(wxHORIZONTAL);
  fontXSizer->Add(fontYSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticBox *sizeBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sizeSizer = new wxStaticBoxSizer(sizeBox, wxHORIZONTAL);
  fontYSizer->Add(sizeSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sizeCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FONT1_SIZE, wxT("15.0"),
                   wxDefaultPosition, wxSize(100, 22));
  sizeSizer->Add(sizeCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *boldBoxSizer = new wxBoxSizer(wxVERTICAL);
  fontYSizer->Add(boldBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *boldBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font Style"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *boldSizer = new wxStaticBoxSizer(boldBox, wxVERTICAL);
  boldBoxSizer->Add(boldSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *italicSizer = new wxBoxSizer(wxVERTICAL);
  boldSizer->Add(italicSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxCheckBox *italicCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_TEXT1_ITALIC,
                                          wxT("Italic"),
                                          wxDefaultPosition, wxDefaultSize);
  italicCtrl->SetValue(false);
  italicCtrl->Enable(false);
  italicSizer->Add(italicCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxCheckBox *boldCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_TEXT1_BOLD,
                                        wxT("Bold"),
                                        wxDefaultPosition, wxDefaultSize);
  boldCtrl->SetValue(false);
  boldCtrl->Enable(false);
  italicSizer->Add(boldCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Font Opacity
  wxStaticBox *opacityFontBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Opacity"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *opacityFontSizer =
    new wxStaticBoxSizer(opacityFontBox, wxVERTICAL);
  fontSizer->Add(opacityFontSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityFontCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FONT1_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(150, 50),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityFontSizer->Add(opacityFontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Font color
  wxStaticBox *fontColorBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *fontColorSizer = new wxStaticBoxSizer(fontColorBox, wxVERTICAL);
  fontSizer->Add(fontColorSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *font2Sizer = new wxBoxSizer(wxHORIZONTAL);
  fontColorSizer->Add(font2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *fontCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_FONT1_COLOR,
                                        FillColor,
                                        wxDefaultPosition, wxSize(80, 22));
  font2Sizer->Add(fontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleFontCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FONT1_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  font2Sizer->Add(sampleFontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *pickFont = new wxButton(panel, ID_SYMBOLIZER_FONT1_PICKER_BTN,
                                    wxT("&Pick a color"));
  fontColorSizer->Add(pickFont, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: PointPlacement
  wxBoxSizer *pointBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(pointBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *pointBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Label Point Placement"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *pointSizer = new wxStaticBoxSizer(pointBox, wxHORIZONTAL);
  pointBoxSizer->Add(pointSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// first row A: Anchor Point
  wxStaticBox *anchorBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Anchor Point"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *anchorSizer = new wxStaticBoxSizer(anchorBox, wxVERTICAL);
  pointSizer->Add(anchorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *anchor1Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  anchor1Sizer->Add(anchor1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_ANCHOR_X, wxT("0.5"),
                   wxDefaultPosition, wxSize(60, 22));
  anchor1Sizer->Add(anchorXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *anchor2Sizer = new wxBoxSizer(wxHORIZONTAL);
  anchorSizer->Add(anchor2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *anchor2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  anchor2Sizer->Add(anchor2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *anchorYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_ANCHOR_Y, wxT("0.5"),
                   wxDefaultPosition, wxSize(60, 22));
  anchor2Sizer->Add(anchorYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// first row B: Displacement
  wxStaticBox *displacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                 wxT("Displacement"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  wxBoxSizer *displacementSizer =
    new wxStaticBoxSizer(displacementBox, wxVERTICAL);
  pointSizer->Add(displacementSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *displ1Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ1Label = new wxStaticText(panel, wxID_STATIC, wxT("X"));
  displ1Sizer->Add(displ1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementXCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_DISPLACEMENT_X, wxT("0.0"),
                   wxDefaultPosition, wxSize(60, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_DISPLACEMENT_Y, wxT("0.0"),
                   wxDefaultPosition, wxSize(60, 22));
  displ2Sizer->Add(displacementYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// first row C: Rotation
  wxStaticBox *rotBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Rotation"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *rotSizer = new wxStaticBoxSizer(rotBox, wxVERTICAL);
  pointSizer->Add(rotSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *rot1Sizer = new wxBoxSizer(wxHORIZONTAL);
  rotSizer->Add(rot1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *rotCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_ROTATION, wxT("0.0"),
                   wxDefaultPosition, wxSize(60, 22));
  rot1Sizer->Add(rotCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Font Halo
  wxStaticBox *haloBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font Halo"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *haloSizer = new wxStaticBoxSizer(haloBox, wxVERTICAL);
  pointBoxSizer->Add(haloSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *halo1Sizer = new wxBoxSizer(wxHORIZONTAL);
  haloSizer->Add(halo1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableHaloCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_HALO1_ENABLE,
                                              wxT("Enable"),
                                              wxDefaultPosition, wxDefaultSize);
  enableHaloCtrl->SetValue(Style->IsHaloEnabled());
  enableHaloCtrl->Enable(false);
  halo1Sizer->Add(enableHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Halo Radius
  wxStaticBox *radiusBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Radius"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *radiusSizer = new wxStaticBoxSizer(radiusBox, wxVERTICAL);
  halo1Sizer->Add(radiusSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *radiusCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_HALO1_RADIUS, wxT("1.0"),
                   wxDefaultPosition, wxSize(50, 22));
  radiusCtrl->Enable(false);
  radiusSizer->Add(radiusCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Halo Opacity
  wxStaticBox *opacityHaloBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Opacity"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *opacityHaloSizer =
    new wxStaticBoxSizer(opacityHaloBox, wxVERTICAL);
  halo1Sizer->Add(opacityHaloSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxSlider *opacityHaloCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_HALO1_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(75, 50),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityHaloSizer->Add(opacityHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  opacityHaloCtrl->Enable(false);
// Halo color
  wxStaticBox *haloColorBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *haloColorSizer = new wxStaticBoxSizer(haloColorBox, wxHORIZONTAL);
  haloSizer->Add(haloColorSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *haloCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_HALO1_COLOR,
                                        FillColor,
                                        wxDefaultPosition, wxSize(80, 22));
  haloColorSizer->Add(haloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  haloCtrl->Enable(false);
  color = wxColour(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleHaloCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_HALO1_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  haloColorSizer->Add(sampleHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *pickHalo = new wxButton(panel, ID_SYMBOLIZER_HALO1_PICKER_BTN,
                                    wxT("&Pick a color"));
  haloColorSizer->Add(pickHalo, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  pickHalo->Enable(false);

  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_TEXT1_ENABLE,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnCmdLabel1Changed);
  Connect(ID_LABEL_NO_GEOM_SYMBOLIZER,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdDontPaintGeom1Changed);
  Connect(ID_SYMBOLIZER_FONT1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdFont1ColorPicker);
  Connect(ID_SYMBOLIZER_FONT1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdFont1ColorChanged);
  Connect(ID_SYMBOLIZER_FONT1_NAME, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnFont1Changed);
  Connect(ID_SYMBOLIZER_HALO1_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdHalo1EnableChanged);
  Connect(ID_SYMBOLIZER_HALO1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdColorHalo1Picker);
  Connect(ID_SYMBOLIZER_HALO1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdColorHalo1Changed);
  return panel;
}

void QuickStyleVectorDialog::
OnCmdLabel1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Label enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_ENABLE);
  if (enableCtrl->IsChecked() == true)
    {
      Style->SetLabelPrint(true);
      Style->SetLabelLinePlacement(false);
  } else
    Style->SetLabelPrint(false);
  RetrieveTextPointPage(false);
  UpdateTextPointPage();
}

void QuickStyleVectorDialog::
OnCmdDontPaintGeom1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Don't Paint Point Symbolizer: enable/disable 
//
  wxCheckBox *noGeomCtrl =
    (wxCheckBox *) FindWindow(ID_LABEL_NO_GEOM_SYMBOLIZER);
  if (noGeomCtrl->IsChecked() == true)
    Style->SetDontPaintGeomSymbolizer(true);
  else
    Style->SetDontPaintGeomSymbolizer(false);
  RetrieveTextPointPage(false);
  UpdateTextPointPage();
}

void QuickStyleVectorDialog::OnFont1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// the Font is changed
//
  wxCheckBox *boldCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_BOLD);
  wxCheckBox *italicCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_ITALIC);
  wxComboBox *fontList = (wxComboBox *) FindWindow(ID_SYMBOLIZER_FONT1_NAME);
  wxString font = fontList->GetValue();
  char facename[1024];
  strcpy(facename, font.ToUTF8());
  if (strncmp(facename, "ToyFont: ", 9) == 0)
    {
      boldCtrl->Enable(true);
      italicCtrl->Enable(true);
  } else
    {
      bool bold = false;
      bool italic = false;
      MainFrame->CheckTTFont(facename, &bold, &italic);
      boldCtrl->SetValue(bold);
      italicCtrl->SetValue(italic);
      boldCtrl->Enable(false);
      italicCtrl->Enable(false);
    }
}

void QuickStyleVectorDialog::
OnCmdFont1ColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT1_COLOR);
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

void QuickStyleVectorDialog::
OnCmdFont1ColorChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Font Color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT1_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FONT1_PICKER_HEX);
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

void QuickStyleVectorDialog::
OnCmdColorHalo1Picker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO1_COLOR);
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

void QuickStyleVectorDialog::
OnCmdColorHalo1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Halo Color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO1_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_HALO1_PICKER_HEX);
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

void QuickStyleVectorDialog::
OnCmdHalo1EnableChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Graphic Halo enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_HALO1_ENABLE);
  if (enableCtrl->IsChecked() == true)
    Style->EnableHalo(true);
  else
    Style->EnableHalo(false);
  RetrieveTextPointPage(false);
  UpdateTextPointPage();
}

wxPanel *QuickStyleVectorDialog::CreateTextLinePage(wxWindow * parent)
{
//
// creating the Text Symbolizer page (Line Placement)
//
  wxString FillColor = wxT("#808080");
  wxPanel *panel = new wxPanel(parent, ID_PANE_TEXT1);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// enable/disable Labels
  wxBoxSizer *auxBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(auxBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *auxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Labels"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *auxSizer = new wxStaticBoxSizer(auxBox, wxVERTICAL);
  auxBoxSizer->Add(auxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(enableSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_TEXT2_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(Style->IsLabelPrint());
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  enableSizer->AddSpacer(20);
  enableSizer->AddSpacer(20);
  enableSizer->AddSpacer(20);
  enableSizer->AddSpacer(20);
  wxStaticText *columnLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Column:"));
  enableSizer->Add(columnLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxComboBox *columnList =
    new wxComboBox(panel, ID_SYMBOLIZER_TEXT2_LABEL, wxT(""), wxDefaultPosition,
                   wxSize(400, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  InitializeComboColumns(columnList);
  enableSizer->Add(columnList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString noGeom =
    wxT("Don't paint the Line Symbolizer; Text Symbolizer alone");
  wxCheckBox *noGeomSymbolizerCtrl =
    new wxCheckBox(panel, ID_LABEL_NO_GEOM_SYMBOLIZER, noGeom,
                   wxDefaultPosition, wxDefaultSize);
  noGeomSymbolizerCtrl->SetValue(Style->IsDontPaintGeomSymbolizer());
  wxBoxSizer *noGeomSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(noGeomSizer, 0, wxALIGN_LEFT | wxALL, 5);
  noGeomSizer->Add(noGeomSymbolizerCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *fontBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *font9Sizer = new wxStaticBoxSizer(fontBox, wxHORIZONTAL);
  auxSizer->Add(font9Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Second row: Font Name, Size and Fill 
  wxBoxSizer *fontSizer = new wxBoxSizer(wxHORIZONTAL);
  font9Sizer->Add(fontSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *font0Sizer = new wxBoxSizer(wxHORIZONTAL);
  fontSizer->Add(font0Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Font Name
  wxBoxSizer *fontXSizer = new wxBoxSizer(wxVERTICAL);
  font0Sizer->Add(fontXSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxComboBox *fontList =
    new wxComboBox(panel, ID_SYMBOLIZER_FONT2_NAME, wxT(""), wxDefaultPosition,
                   wxSize(250, 21), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  InitializeComboFonts(fontList);
  fontList->Select(0);
  fontXSizer->Add(fontList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Font Size
  wxBoxSizer *fontYSizer = new wxBoxSizer(wxHORIZONTAL);
  fontXSizer->Add(fontYSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticBox *sizeBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sizeSizer = new wxStaticBoxSizer(sizeBox, wxVERTICAL);
  fontYSizer->Add(sizeSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sizeCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FONT2_SIZE, wxT("15.0"),
                   wxDefaultPosition, wxSize(100, 22));
  sizeSizer->Add(sizeCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *boldBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  fontYSizer->Add(boldBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *boldBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font Style"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *boldSizer = new wxStaticBoxSizer(boldBox, wxHORIZONTAL);
  boldBoxSizer->Add(boldSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *italicSizer = new wxBoxSizer(wxHORIZONTAL);
  boldSizer->Add(italicSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxCheckBox *italicCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_TEXT2_ITALIC,
                                          wxT("Italic"),
                                          wxDefaultPosition, wxDefaultSize);
  italicCtrl->SetValue(false);
  italicCtrl->Enable(false);
  italicSizer->Add(italicCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxCheckBox *boldCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_TEXT2_BOLD,
                                        wxT("Bold"),
                                        wxDefaultPosition, wxDefaultSize);
  boldCtrl->SetValue(false);
  boldCtrl->Enable(false);
  italicSizer->Add(boldCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Font Opacity
  wxStaticBox *opacityFontBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Opacity"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *opacityFontSizer =
    new wxStaticBoxSizer(opacityFontBox, wxVERTICAL);
  fontSizer->Add(opacityFontSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityFontCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FONT2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(150, 35),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityFontSizer->Add(opacityFontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Font color
  wxStaticBox *fontColorBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *fontColorSizer = new wxStaticBoxSizer(fontColorBox, wxHORIZONTAL);
  fontSizer->Add(fontColorSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxBoxSizer *font2Sizer = new wxBoxSizer(wxVERTICAL);
  fontColorSizer->Add(font2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *fontCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_FONT2_COLOR,
                                        FillColor,
                                        wxDefaultPosition, wxSize(80, 22));
  font2Sizer->Add(fontCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleFontCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FONT2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  fontColorSizer->Add(sampleFontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *pickFont = new wxButton(panel, ID_SYMBOLIZER_FONT2_PICKER_BTN,
                                    wxT("&Pick a color"));
  font2Sizer->Add(pickFont, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
// second row: LinePlacement
  wxBoxSizer *lineBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(lineBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *lineBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Label Line Placement"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *lineSizer = new wxStaticBoxSizer(lineBox, wxVERTICAL);
  lineBoxSizer->Add(lineSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row A: PerpendicularOffset
  wxBoxSizer *perpendicularBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  lineSizer->Add(perpendicularBoxSizer, 0,
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_PERPENDICULAR, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  perp1Sizer->Add(perpendicularCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  perpendicularCtrl->Enable(false);
  wxStaticText *perp1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Draw lines in parallel to the original geometry."));
  perp1Sizer->Add(perp1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticText *perp2Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Positive to the left-hand side. Negative numbers mean right."));
  perpendicularSizer->Add(perp2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row B: IsRepeated, InitialGap and Gap
  wxBoxSizer *repeatedBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  lineSizer->Add(repeatedBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *repeatedBox = new wxStaticBox(panel, wxID_STATIC,
                                             wxT("Repeated Label"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *repeatedSizer = new wxStaticBoxSizer(repeatedBox, wxHORIZONTAL);
  repeatedBoxSizer->Add(repeatedSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxCheckBox *isRepeatedCtrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_TEXT_IS_REPEATED,
                   wxT("Repeated"),
                   wxDefaultPosition, wxDefaultSize);
  isRepeatedCtrl->SetValue(false);
  isRepeatedCtrl->Enable(false);
  repeatedSizer->Add(isRepeatedCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *inigapBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Initial Gap"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *inigapSizer = new wxStaticBoxSizer(inigapBox, wxVERTICAL);
  repeatedSizer->Add(inigapSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *inigapCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_INITIAL_GAP, wxT("0.0"),
                   wxDefaultPosition, wxSize(60, 22));
  inigapCtrl->Enable(false);
  inigapSizer->Add(inigapCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *gapBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Gap"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *gapSizer = new wxStaticBoxSizer(gapBox, wxVERTICAL);
  repeatedSizer->Add(gapSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *gapCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_TEXT_GAP, wxT("0.0"),
                   wxDefaultPosition, wxSize(60, 22));
  gapCtrl->Enable(false);
  gapSizer->Add(gapCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row C: IsAligned and Generalize
  wxBoxSizer *optBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  repeatedBoxSizer->Add(optBoxSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticBox *optBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Options"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *optSizer = new wxStaticBoxSizer(optBox, wxVERTICAL);
  optBoxSizer->Add(optSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxCheckBox *isAlignedCtrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_TEXT_IS_ALIGNED,
                   wxT("Aligned"),
                   wxDefaultPosition, wxDefaultSize);
  isAlignedCtrl->SetValue(false);
  isAlignedCtrl->Enable(false);
  optSizer->Add(isAlignedCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *generalizeCtrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_TEXT_GENERALIZE,
                   wxT("Generalize"),
                   wxDefaultPosition, wxDefaultSize);
  generalizeCtrl->SetValue(false);
  generalizeCtrl->Enable(false);
  optSizer->Add(generalizeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Halo 
  wxBoxSizer *haloBoxSizer = new wxBoxSizer(wxVERTICAL);
  lineBoxSizer->Add(haloBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *haloBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Font Halo"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *haloSizer = new wxStaticBoxSizer(haloBox, wxVERTICAL);
  haloBoxSizer->Add(haloSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *halo0Sizer = new wxBoxSizer(wxHORIZONTAL);
  haloSizer->Add(halo0Sizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxCheckBox *enableHaloCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_HALO2_ENABLE,
                                              wxT("Enable"),
                                              wxDefaultPosition, wxDefaultSize);
  enableHaloCtrl->SetValue(Style->IsHaloEnabled());
  enableHaloCtrl->Enable(false);
  halo0Sizer->Add(enableHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Halo Radius
  wxStaticBox *radiusBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Radius"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *radiusSizer = new wxStaticBoxSizer(radiusBox, wxVERTICAL);
  halo0Sizer->Add(radiusSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxTextCtrl *radiusCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_HALO2_RADIUS, wxT("1.0"),
                   wxDefaultPosition, wxSize(50, 22));
  radiusCtrl->Enable(false);
  radiusSizer->Add(radiusCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// Halo Opacity
  wxStaticBox *opacityHaloBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Opacity"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *opacityHaloSizer =
    new wxStaticBoxSizer(opacityHaloBox, wxVERTICAL);
  haloSizer->Add(opacityHaloSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxSlider *opacityHaloCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_HALO2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(130, 35),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityHaloSizer->Add(opacityHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  opacityHaloCtrl->Enable(false);
// Halo color
  wxStaticBox *haloColorBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *haloColorSizer = new wxStaticBoxSizer(haloColorBox, wxHORIZONTAL);
  haloSizer->Add(haloColorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxBoxSizer *halo2Sizer = new wxBoxSizer(wxVERTICAL);
  haloColorSizer->Add(halo2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *haloCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_HALO2_COLOR,
                                        FillColor,
                                        wxDefaultPosition, wxSize(80, 22));
  halo2Sizer->Add(haloCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  haloCtrl->Enable(false);
  color = wxColour(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleHaloCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_HALO2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  haloColorSizer->Add(sampleHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *pickHalo = new wxButton(panel, ID_SYMBOLIZER_HALO2_PICKER_BTN,
                                    wxT("&Pick a color"));
  halo2Sizer->Add(pickHalo, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  pickHalo->Enable(false);

  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_TEXT2_ENABLE,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnCmdLabel2Changed);
  Connect(ID_LABEL_NO_GEOM_SYMBOLIZER,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdDontPaintGeom2Changed);
  Connect(ID_SYMBOLIZER_FONT2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdFont2ColorPicker);
  Connect(ID_SYMBOLIZER_FONT2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdFont2ColorChanged);
  Connect(ID_SYMBOLIZER_TEXT_IS_REPEATED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdIsRepeatedChanged);
  Connect(ID_SYMBOLIZER_TEXT_IS_ALIGNED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdIsAlignedChanged);
  Connect(ID_SYMBOLIZER_TEXT_GENERALIZE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdGeneralizeLineChanged);
  Connect(ID_SYMBOLIZER_FONT2_NAME, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & QuickStyleVectorDialog::OnFont2Changed);
  Connect(ID_SYMBOLIZER_HALO2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdHalo2EnableChanged);
  Connect(ID_SYMBOLIZER_HALO2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdColorHalo2Picker);
  Connect(ID_SYMBOLIZER_HALO2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleVectorDialog::OnCmdColorHalo2Changed);
  return panel;
}

void QuickStyleVectorDialog::
OnCmdLabel2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Label enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    {
      Style->SetLabelPrint(true);
      Style->SetLabelLinePlacement(true);
  } else
    Style->SetLabelPrint(false);
  RetrieveTextLinePage(false);
  UpdateTextLinePage();
}

void QuickStyleVectorDialog::
OnCmdDontPaintGeom2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Don't Paint Line Symbolizer: enable/disable 
//
  wxCheckBox *noGeomCtrl =
    (wxCheckBox *) FindWindow(ID_LABEL_NO_GEOM_SYMBOLIZER);
  if (noGeomCtrl->IsChecked() == true)
    Style->SetDontPaintGeomSymbolizer(true);
  else
    Style->SetDontPaintGeomSymbolizer(false);
  RetrieveTextLinePage(false);
  UpdateTextLinePage();
}

void QuickStyleVectorDialog::OnFont2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// the Font is changed
//
  wxCheckBox *boldCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_BOLD);
  wxCheckBox *italicCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_ITALIC);
  wxComboBox *fontList = (wxComboBox *) FindWindow(ID_SYMBOLIZER_FONT2_NAME);
  wxString font = fontList->GetValue();
  char facename[1024];
  strcpy(facename, font.ToUTF8());
  if (strncmp(facename, "ToyFont: ", 9) == 0)
    {
      boldCtrl->Enable(true);
      italicCtrl->Enable(true);
  } else
    {
      bool bold = false;
      bool italic = false;
      MainFrame->CheckTTFont(facename, &bold, &italic);
      boldCtrl->SetValue(bold);
      italicCtrl->SetValue(italic);
      boldCtrl->Enable(false);
      italicCtrl->Enable(false);
    }
}

void QuickStyleVectorDialog::
OnCmdFont2ColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT2_COLOR);
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

void QuickStyleVectorDialog::
OnCmdFont2ColorChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Font Color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT2_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FONT2_PICKER_HEX);
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

void QuickStyleVectorDialog::
OnCmdIsRepeatedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Label IsRepeated enable/disable 
//
  wxCheckBox *repeatedCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT_IS_REPEATED);
  if (repeatedCtrl->IsChecked() == true)
    Style->SetRepeatedLabel(true);
  else
    Style->SetRepeatedLabel(false);
  RetrieveTextLinePage(false);
  UpdateTextLinePage();
}

void QuickStyleVectorDialog::
OnCmdIsAlignedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Label IsAligned enable/disable 
//
  wxCheckBox *alignedCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT_IS_ALIGNED);
  if (alignedCtrl->IsChecked() == true)
    Style->SetLabelIsAligned(true);
  else
    Style->SetLabelIsAligned(false);
}

void QuickStyleVectorDialog::
OnCmdGeneralizeLineChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Label GeneralizeLine enable/disable 
//
  wxCheckBox *generalizeCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT_GENERALIZE);
  if (generalizeCtrl->IsChecked() == true)
    Style->SetLabelGeneralizeLine(true);
  else
    Style->SetLabelGeneralizeLine(false);
}

void QuickStyleVectorDialog::
OnCmdColorHalo2Picker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO2_COLOR);
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

void QuickStyleVectorDialog::
OnCmdColorHalo2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Halo Color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO2_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_HALO2_PICKER_HEX);
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

void QuickStyleVectorDialog::
OnCmdHalo2EnableChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Graphic Halo enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_HALO2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    Style->EnableHalo(true);
  else
    Style->EnableHalo(false);
  RetrieveTextLinePage(false);
  UpdateTextLinePage();
}

bool QuickStyleVectorDialog::RetrieveMainPage()
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

void QuickStyleVectorDialog::UpdateMainPage()
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

bool QuickStyleVectorDialog::RetrievePointPage(bool check)
{
//
// retrieving params from the Point Symbolizer page
//
  double opacity;
  double size;
  double rotation;
  double anchorPointX;
  double anchorPointY;
  double displacementX;
  double displacementY;
  char fillColor[8];
  char strokeColor[8];
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_SIZE);
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
  wxTextCtrl *rotationCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ROTATION);
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
  wxTextCtrl *anchorXCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_X);
  value = anchorXCtrl->GetValue();
  if (value.ToDouble(&anchorPointX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorPointX < 0.0 || anchorPointX > 1.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X must be between 0.0 and 1.0 !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *anchorYCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_Y);
  value = anchorYCtrl->GetValue();
  if (value.ToDouble(&anchorPointY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorPointY < 0.0 || anchorPointY > 1.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_X);
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_Y);
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
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
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
  colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_COLOR);
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

  Style->SetSymbolOpacity(opacity);
  Style->SetSymbolSize(size);
  Style->SetSymbolAnchorX(anchorPointX);
  Style->SetSymbolAnchorY(anchorPointY);
  Style->SetSymbolRotation(rotation);
  Style->SetSymbolDisplacementX(displacementX);
  Style->SetSymbolDisplacementY(displacementY);
  Style->SetSymbolFillColor(fillColor);
  Style->SetSymbolStrokeColor(strokeColor);
  return true;
}

void QuickStyleVectorDialog::UpdatePointPage()
{
//
// updating the Point Symbolizer page
//
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  opacityCtrl->SetValue(Style->GetSymbolOpacity() * 100.0);
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetSymbolSize());
  wxString str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  wxTextCtrl *rotationCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ROTATION);
  sprintf(dummy, "%1.2f", Style->GetSymbolRotation());
  str = wxString::FromUTF8(dummy);
  rotationCtrl->SetValue(str);
  wxTextCtrl *anchorXCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_X);
  sprintf(dummy, "%1.2f", Style->GetSymbolAnchorX());
  str = wxString::FromUTF8(dummy);
  anchorXCtrl->SetValue(str);
  wxTextCtrl *anchorYCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_Y);
  sprintf(dummy, "%1.2f", Style->GetSymbolAnchorY());
  str = wxString::FromUTF8(dummy);
  anchorYCtrl->SetValue(str);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_X);
  sprintf(dummy, "%1.2f", Style->GetSymbolDisplacementX());
  str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetSymbolDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxRadioBox *markCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MARK);
  switch (Style->GetSymbolWellKnownMark())
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
  wxString fillColor = wxString::FromUTF8(Style->GetSymbolFillColor());
  colorFillCtrl->SetValue(fillColor);
  wxTextCtrl *colorStrokeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_COLOR);
  wxString strokeColor = wxString::FromUTF8(Style->GetSymbolStrokeColor());
  colorStrokeCtrl->SetValue(strokeColor);
}

bool QuickStyleVectorDialog::RetrieveLinePage(bool check)
{
//
// retrieving params from the Line Symbolizer page
//
  double opacity;
  double perpendicularOffset;
  char strokeColor[8];
  double strokeWidth;
  char stroke2Color[8];
  double stroke2Width;
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
                       ("STROKE-COLOR #1 isn't a valid HexRGB color !!!"),
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
                       ("STROKE-WIDTH #1 isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (strokeWidth <= 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH #1 must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxRadioBox *dotCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT);
  switch (dotCtrl->GetSelection())
    {
      case 1:
        Style->SetLineDotStyle(QUICK_STYLE_DOT_LINE);
        break;
      case 2:
        Style->SetLineDotStyle(QUICK_STYLE_DASH_LINE);
        break;
      case 3:
        Style->SetLineDotStyle(QUICK_STYLE_DASH_DOT_LINE);
        break;
      default:
        Style->SetLineDotStyle(QUICK_STYLE_SOLID_LINE);
        break;
    };
  Style->SetLineOpacity(opacity);
  Style->SetLinePerpendicularOffset(perpendicularOffset);
  Style->SetLineStrokeWidth(strokeWidth);
  Style->SetLineStrokeColor(strokeColor);

  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_SECOND_STROKE_ENABLE);
  bool line2Enabled = enableCtrl->IsChecked();
  Style->SetLine2Enabled(line2Enabled);
  // Double stroke - second pass
  wxTextCtrl *color2Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  color = color2Ctrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-COLOR #2 isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(stroke2Color, color.ToUTF8());
  wxTextCtrl *width2Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  value = width2Ctrl->GetValue();
  if (value.ToDouble(&stroke2Width) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH #2 isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (stroke2Width <= 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH #2 must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxRadioBox *dot1Ctrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT_2);
  switch (dot1Ctrl->GetSelection())
    {
      case 1:
        Style->SetLine2DotStyle(QUICK_STYLE_DOT_LINE);
        break;
      case 2:
        Style->SetLine2DotStyle(QUICK_STYLE_DASH_LINE);
        break;
      case 3:
        Style->SetLine2DotStyle(QUICK_STYLE_DASH_DOT_LINE);
        break;
      default:
        Style->SetLine2DotStyle(QUICK_STYLE_SOLID_LINE);
        break;
    };
  Style->SetLine2StrokeWidth(stroke2Width);
  Style->SetLine2StrokeColor(stroke2Color);
  return true;
}

void QuickStyleVectorDialog::UpdateLinePage()
{
//
// updating the Line Symbolizer page
//
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  opacityCtrl->SetValue(Style->GetLineOpacity() * 100.0);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_PERPENDICULAR);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetLinePerpendicularOffset());
  wxString str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE1_PICKER_BTN);
  colorCtrl->Enable(true);
  pick->Enable(true);
  wxString strokeColor = wxString::FromUTF8(Style->GetLineStrokeColor());
  colorCtrl->SetValue(strokeColor);
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  sprintf(dummy, "%1.2f", Style->GetLineStrokeWidth());
  str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  wxRadioBox *dotCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT);
  switch (Style->GetLineDotStyle())
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

  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_SECOND_STROKE_ENABLE);
  enableCtrl->SetValue(Style->IsLine2Enabled());
  wxTextCtrl *color1Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxString stroke1Color = wxString::FromUTF8(Style->GetLine2StrokeColor());
  color1Ctrl->SetValue(stroke1Color);
  wxButton *pick1 = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_BTN);
  wxTextCtrl *width1Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  sprintf(dummy, "%1.2f", Style->GetLine2StrokeWidth());
  str = wxString::FromUTF8(dummy);
  width1Ctrl->SetValue(str);
  wxRadioBox *dot1Box = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_DASH_DOT_2);
  switch (Style->GetLine2DotStyle())
    {
      case QUICK_STYLE_DOT_LINE:
        dot1Box->SetSelection(1);
        break;
      case QUICK_STYLE_DASH_LINE:
        dot1Box->SetSelection(2);
        break;
      case QUICK_STYLE_DASH_DOT_LINE:
        dot1Box->SetSelection(3);
        break;
      default:
        dot1Box->SetSelection(0);
        break;
    };
  if (Style->IsLine2Enabled() == true)
    {
      color1Ctrl->Enable(true);
      pick1->Enable(true);
      width1Ctrl->Enable(true);
      dot1Box->Enable(true);
  } else
    {
      color1Ctrl->Enable(false);
      pick1->Enable(false);
      width1Ctrl->Enable(false);
      dot1Box->Enable(false);
    }
}

bool QuickStyleVectorDialog::RetrievePolygonPage(bool check)
{
//
// retrieving params from the Polygon Synbolizer page
//
  double fillOpacity;
  double displacementX;
  double displacementY;
  double perpendicularOffset;
  char fillColor[8];
  double strokeOpacity;
  char strokeColor[8];
  double strokeWidth;
  bool solidFill;
  int brushId;
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL2_OPACITY);
  fillOpacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X);
  wxString value = displXCtrl->GetValue();
  if (Style->IsPolygonFill() != true && Style->IsPolygonStroke() != true)
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
  wxSlider *opacity2Ctrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  strokeOpacity = opacity2Ctrl->GetValue() / 100.0;
  wxTextCtrl *color2Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  color = color2Ctrl->GetValue();
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
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
  wxRadioBox *styleBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL1_TYPE);
  if (styleBox->GetSelection() == 0)
    solidFill = true;
  else
    solidFill = false;
  wxRadioBox *brushBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL2_TYPE);
  brushId = brushBox->GetSelection();

  Style->SetPolygonFillOpacity(fillOpacity);
  Style->SetPolygonDisplacementX(displacementX);
  Style->SetPolygonDisplacementY(displacementY);
  Style->SetPolygonPerpendicularOffset(perpendicularOffset);
  Style->SetPolygonFillColor(fillColor);
  Style->SetPolygonSolidFill(solidFill);
  Style->SetPolygonFillBrushId(brushId);
  Style->SetPolygonStrokeOpacity(strokeOpacity);
  Style->SetPolygonStrokeColor(strokeColor);
  Style->SetPolygonStrokeWidth(strokeWidth);
  return true;
}

void QuickStyleVectorDialog::UpdatePolygonPage()
{
//
// updating the Polygon Symbolizer page
//
  wxCheckBox *enableBox = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE);
  enableBox->SetValue(Style->IsPolygonFill());
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetPolygonDisplacementX());
  wxString str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetPolygonDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_PERPENDICULAR);
  sprintf(dummy, "%1.2f", Style->GetPolygonPerpendicularOffset());
  str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL2_OPACITY);
  opacityCtrl->SetValue(Style->GetPolygonFillOpacity() * 100.0);
  opacityCtrl->Enable(Style->IsPolygonFill());
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_FILL2_PICKER_BTN);
  wxColour color = wxNullColour;
  str = wxString::FromUTF8(Style->GetPolygonFillColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorCtrl->SetValue(str);
    }
  colorCtrl->Enable(Style->IsPolygonFill());
  pick->Enable(Style->IsPolygonFill());
  wxRadioBox *styleBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL1_TYPE);
  styleBox->Enable(true);
  if (Style->IsPolygonSolidFill() == true)
    styleBox->SetSelection(0);
  else
    styleBox->SetSelection(1);
  if (HasStandardBrushes == false)
    styleBox->Enable(false);
  else
    styleBox->Enable(true);
  wxRadioBox *brushBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL2_TYPE);
  brushBox->SetSelection(Style->GetPolygonFillBrushId());
  if (HasStandardBrushes == false || Style->IsPolygonSolidFill() == true)
    brushBox->Enable(false);
  else
    brushBox->Enable(true);
  wxCheckBox *enable2Box =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  enable2Box->SetValue(Style->IsPolygonStroke());
  wxSlider *opacity2Ctrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  opacity2Ctrl->SetValue(Style->GetPolygonStrokeOpacity() * 100.0);
  opacity2Ctrl->Enable(Style->IsPolygonStroke());
  wxTextCtrl *color2Ctrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxButton *pick2 = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_BTN);
  color = wxNullColour;
  str = wxString::FromUTF8(Style->GetPolygonStrokeColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      color2Ctrl->SetValue(str);
    }
  color2Ctrl->Enable(Style->IsPolygonStroke());
  pick2->Enable(Style->IsPolygonStroke());
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  sprintf(dummy, "%1.2f", Style->GetPolygonStrokeWidth());
  str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  widthCtrl->Enable(Style->IsPolygonStroke());
}

bool QuickStyleVectorDialog::RetrieveTextPointPage(bool check)
{
//
// retrieving params from the Text Symbolizer page - Point Placement
//
  wxString column;
  bool noGeom;
  wxString font;
  double fontSize;
  int fontStyle;
  int fontWeight;
  double opacity;
  char fontColor[8];
  double haloRadius;
  double haloOpacity;
  char haloColor[8];
  double rotation;
  double anchorPointX;
  double anchorPointY;
  double displacementX;
  double displacementY;

  if (Style->IsLabelPrint() == false)
    return true;

  wxCheckBox *noGeomCtrl =
    (wxCheckBox *) FindWindow(ID_LABEL_NO_GEOM_SYMBOLIZER);
  noGeom = noGeomCtrl->GetValue();
  wxComboBox *columnCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_TEXT1_LABEL);
  int idSel = columnCtrl->GetSelection();
  if (idSel == wxNOT_FOUND)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You must select some Column !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  column = columnCtrl->GetValue();
  wxComboBox *fontCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_FONT1_NAME);
  idSel = fontCtrl->GetSelection();
  if (idSel == wxNOT_FOUND)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You must select some Font !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  font = fontCtrl->GetValue();
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT1_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&fontSize) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (fontSize <= 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-SIZE should be a positive numberr !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxCheckBox *boldCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_BOLD);
  wxCheckBox *italicCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_ITALIC);
  if (boldCtrl->GetValue() == true)
    fontWeight = RL2_FONTWEIGHT_BOLD;
  else
    fontWeight = RL2_FONTWEIGHT_NORMAL;
  if (italicCtrl->GetValue() == true)
    fontStyle = RL2_FONTSTYLE_ITALIC;
  else
    fontStyle = RL2_FONTSTYLE_NORMAL;
  wxSlider *opacityFontCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FONT1_OPACITY);
  opacity = opacityFontCtrl->GetValue() / 100.0;
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT1_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(fontColor, color.ToUTF8());
// Point Placement
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_ROTATION);
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_ANCHOR_X);
  value = anchorXCtrl->GetValue();
  if (value.ToDouble(&anchorPointX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorPointX < 0.0 || anchorPointX > 1.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_ANCHOR_Y);
  value = anchorYCtrl->GetValue();
  if (value.ToDouble(&anchorPointY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (anchorPointY < 0.0 || anchorPointY > 1.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_DISPLACEMENT_X);
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_DISPLACEMENT_Y);
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
  if (Style->IsHaloEnabled() == true)
    {
      wxSlider *opacityHaloCtrl =
        (wxSlider *) FindWindow(ID_SYMBOLIZER_HALO1_OPACITY);
      haloOpacity = opacityHaloCtrl->GetValue() / 100.0;
      wxTextCtrl *radiusCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO1_RADIUS);
      wxString value = radiusCtrl->GetValue();
      if (value.ToDouble(&haloRadius) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-RADIUS isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (haloRadius <= 0.0)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-RADIUS should be a positive number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      wxTextCtrl *colorHaloCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO1_COLOR);
      wxString color = colorHaloCtrl->GetValue();
      if (ColorMapEntry::IsValidColor(color) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-COLOR isn't a valid HexRGB color !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      strcpy(haloColor, color.ToUTF8());
    }

  Style->SetLabelColumn(column.ToUTF8());
  Style->SetDontPaintGeomSymbolizer(noGeom);
  Style->SetFontFacename(font.ToUTF8());
  Style->SetFontOpacity(opacity);
  Style->SetFontSize(fontSize);
  Style->SetFontStyle(fontStyle);
  Style->SetFontWeight(fontWeight);
  Style->SetFontColor(fontColor);
  Style->SetLabelRotation(rotation);
  Style->SetLabelAnchorX(anchorPointX);
  Style->SetLabelAnchorY(anchorPointY);
  Style->SetLabelDisplacementX(displacementX);
  Style->SetLabelDisplacementY(displacementY);
  if (Style->IsHaloEnabled() == true)
    {
      Style->SetHaloRadius(haloRadius);
      Style->SetHaloOpacity(haloOpacity);
      Style->SetHaloColor(haloColor);
    }
  return true;
}

void QuickStyleVectorDialog::UpdateTextPointPage()
{
//
// updating the Text Symbolizer page - Point Placement
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_ENABLE);
  enableCtrl->SetValue(Style->IsLabelPrint());
  wxCheckBox *noGeomCtrl =
    (wxCheckBox *) FindWindow(ID_LABEL_NO_GEOM_SYMBOLIZER);
  noGeomCtrl->SetValue(Style->IsDontPaintGeomSymbolizer());
  noGeomCtrl->Enable(Style->IsLabelPrint());
  wxComboBox *columnCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_TEXT1_LABEL);
  wxString str = wxString::FromUTF8(Style->GetLabelColumn());
  int idSel = columnCtrl->FindString(str);
  columnCtrl->SetSelection(idSel);
  columnCtrl->Enable(Style->IsLabelPrint());
  wxComboBox *fontCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_FONT1_NAME);
  str = wxString::FromUTF8(Style->GetFontFacename());
  idSel = fontCtrl->FindString(str);
  fontCtrl->SetSelection(idSel);
  fontCtrl->Enable(Style->IsLabelPrint());
  wxSlider *opacityFontCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FONT1_OPACITY);
  opacityFontCtrl->SetValue(Style->GetFontOpacity() * 100.0);
  opacityFontCtrl->Enable(Style->IsLabelPrint());
  wxTextCtrl *colorFontCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT1_COLOR);
  wxButton *pickFont = (wxButton *) FindWindow(ID_SYMBOLIZER_FONT1_PICKER_BTN);
  wxColour color = wxNullColour;
  str = wxString::FromUTF8(Style->GetFontColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorFontCtrl->SetValue(str);
    }
  colorFontCtrl->Enable(Style->IsLabelPrint());
  pickFont->Enable(Style->IsLabelPrint());
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT1_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetFontSize());
  str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  sizeCtrl->Enable(Style->IsLabelPrint());
  wxCheckBox *boldCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_BOLD);
  wxCheckBox *italicCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT1_ITALIC);
  if (Style->GetFontWeight() == RL2_FONTWEIGHT_BOLD)
    boldCtrl->SetValue(true);
  else
    boldCtrl->SetValue(false);
  if (Style->GetFontStyle() == RL2_FONTSTYLE_ITALIC)
    italicCtrl->SetValue(true);
  else
    italicCtrl->SetValue(false);
  if (Style->GetFontFacename() != NULL)
    {
      if (strncmp(Style->GetFontFacename(), "ToyFont: ", 9) == 0)
        {
          boldCtrl->Enable(Style->IsLabelPrint());
          italicCtrl->Enable(Style->IsLabelPrint());
      } else
        {
          bool bold = false;
          bool italic = false;
          MainFrame->CheckTTFont(Style->GetFontFacename(), &bold, &italic);
          boldCtrl->SetValue(bold);
          italicCtrl->SetValue(italic);
          boldCtrl->Enable(false);
          italicCtrl->Enable(false);
        }
    }
// Point Placement
  wxTextCtrl *rotationCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_ROTATION);
  sprintf(dummy, "%1.2f", Style->GetLabelRotation());
  str = wxString::FromUTF8(dummy);
  rotationCtrl->SetValue(str);
  rotationCtrl->Enable(Style->IsLabelPrint());
  wxTextCtrl *anchorXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_ANCHOR_X);
  sprintf(dummy, "%1.2f", Style->GetLabelAnchorX());
  str = wxString::FromUTF8(dummy);
  anchorXCtrl->SetValue(str);
  anchorXCtrl->Enable(Style->IsLabelPrint());
  wxTextCtrl *anchorYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_ANCHOR_Y);
  sprintf(dummy, "%1.2f", Style->GetLabelAnchorY());
  str = wxString::FromUTF8(dummy);
  anchorYCtrl->SetValue(str);
  anchorYCtrl->Enable(Style->IsLabelPrint());
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_DISPLACEMENT_X);
  sprintf(dummy, "%1.2f", Style->GetLabelDisplacementX());
  str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  displXCtrl->Enable(Style->IsLabelPrint());
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", Style->GetLabelDisplacementY());
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  displYCtrl->Enable(Style->IsLabelPrint());
  wxCheckBox *enableHaloBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_HALO1_ENABLE);
  if (Style->IsHaloEnabled() == true)
    enableHaloBox->SetValue(true);
  else
    enableHaloBox->SetValue(false);
  enableHaloBox->Enable(Style->IsLabelPrint());
  bool enable = false;
  if (Style->IsLabelPrint() == true)
    enable = Style->IsHaloEnabled();
  wxTextCtrl *radiusCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO1_RADIUS);
  radiusCtrl->Enable(enable);
  wxTextCtrl *colorHaloCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO1_COLOR);
  colorHaloCtrl->Enable(enable);
  wxButton *pickHalo = (wxButton *) FindWindow(ID_SYMBOLIZER_HALO1_PICKER_BTN);
  pickHalo->Enable(enable);
  wxSlider *opacityHaloCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_HALO1_OPACITY);
  opacityHaloCtrl->Enable(enable);
  opacityHaloCtrl->SetValue(Style->GetHaloOpacity() * 100.0);
  sprintf(dummy, "%1.2f", Style->GetHaloRadius());
  str = wxString::FromUTF8(dummy);
  radiusCtrl->SetValue(str);
  color = wxNullColour;
  str = wxString::FromUTF8(Style->GetHaloColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorHaloCtrl->SetValue(str);
    }
  if (Style->IsHaloEnabled() == false)
    {
      opacityHaloCtrl->Enable(false);
      pickHalo->Enable(false);
      colorHaloCtrl->Enable(false);
      radiusCtrl->Enable(false);
  } else
    {
      opacityHaloCtrl->Enable(true);
      pickHalo->Enable(true);
      colorHaloCtrl->Enable(true);
      radiusCtrl->Enable(true);
    }
}

bool QuickStyleVectorDialog::RetrieveTextLinePage(bool check)
{
//
// retrieving params from the Text Symbolizer page - Line Placement
//
  wxString column;
  bool noGeom;
  wxString font;
  double fontSize;
  int fontStyle;
  int fontWeight;
  double opacity;
  char fontColor[8];
  double haloRadius;
  double haloOpacity;
  char haloColor[8];
  double perpendicularOffset;
  double initialGap;
  double gap;

  if (Style->IsLabelPrint() == false)
    return true;

  wxCheckBox *noGeomCtrl =
    (wxCheckBox *) FindWindow(ID_LABEL_NO_GEOM_SYMBOLIZER);
  noGeom = noGeomCtrl->GetValue();
  wxComboBox *columnCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_TEXT2_LABEL);
  int idSel = columnCtrl->GetSelection();
  if (idSel == wxNOT_FOUND)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You must select some Column !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  column = columnCtrl->GetValue();
  wxComboBox *fontCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_FONT2_NAME);
  idSel = fontCtrl->GetSelection();
  if (idSel == wxNOT_FOUND)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You must select some Font !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  font = fontCtrl->GetValue();
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT2_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&fontSize) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (fontSize <= 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-SIZE should be a positive numberr !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxCheckBox *boldCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_BOLD);
  wxCheckBox *italicCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_ITALIC);
  if (boldCtrl->GetValue() == true)
    fontWeight = RL2_FONTWEIGHT_BOLD;
  else
    fontWeight = RL2_FONTWEIGHT_NORMAL;
  if (italicCtrl->GetValue() == true)
    fontStyle = RL2_FONTSTYLE_ITALIC;
  else
    fontStyle = RL2_FONTSTYLE_NORMAL;
  wxSlider *opacityFontCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FONT2_OPACITY);
  opacity = opacityFontCtrl->GetValue() / 100.0;
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT2_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-COLOR isn't a valid HexRGB color !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  strcpy(fontColor, color.ToUTF8());
// Line Placement
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_PERPENDICULAR);
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
  wxTextCtrl *inigapCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_INITIAL_GAP);
  value = inigapCtrl->GetValue();
  if (value.ToDouble(&initialGap) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("INITIAL-GAP isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (initialGap < 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("INITIAL-GAP should be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxTextCtrl *gapCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_GAP);
  value = gapCtrl->GetValue();
  if (value.ToDouble(&gap) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("GAP isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (gap < 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("GAP should be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Style->IsHaloEnabled() == true)
    {
      wxSlider *opacityHaloCtrl =
        (wxSlider *) FindWindow(ID_SYMBOLIZER_HALO2_OPACITY);
      haloOpacity = opacityHaloCtrl->GetValue() / 100.0;
      wxTextCtrl *radiusCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO2_RADIUS);
      wxString value = radiusCtrl->GetValue();
      if (value.ToDouble(&haloRadius) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-RADIUS isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (haloRadius <= 0.0)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-RADIUS should be a positive number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      wxTextCtrl *colorHaloCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO2_COLOR);
      wxString color = colorHaloCtrl->GetValue();
      if (ColorMapEntry::IsValidColor(color) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-COLOR isn't a valid HexRGB color !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      strcpy(haloColor, color.ToUTF8());
    }
  Style->SetDontPaintGeomSymbolizer(noGeom);
  Style->SetLabelColumn(column.ToUTF8());
  Style->SetFontFacename(font.ToUTF8());
  Style->SetFontOpacity(opacity);
  Style->SetFontSize(fontSize);
  Style->SetFontStyle(fontStyle);
  Style->SetFontWeight(fontWeight);
  Style->SetFontColor(fontColor);
  Style->SetLabelPerpendicularOffset(perpendicularOffset);
  Style->SetLabelInitialGap(initialGap);
  Style->SetLabelGap(gap);
  if (Style->IsHaloEnabled() == true)
    {
      Style->SetHaloRadius(haloRadius);
      Style->SetHaloOpacity(haloOpacity);
      Style->SetHaloColor(haloColor);
    }
  return true;
}

void QuickStyleVectorDialog::UpdateTextLinePage()
{
//
// updating the Text Symbolizer page - Line Placement
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_ENABLE);
  enableCtrl->SetValue(Style->IsLabelPrint());
  wxCheckBox *noGeomCtrl =
    (wxCheckBox *) FindWindow(ID_LABEL_NO_GEOM_SYMBOLIZER);
  noGeomCtrl->SetValue(Style->IsDontPaintGeomSymbolizer());
  noGeomCtrl->Enable(Style->IsLabelPrint());
  wxComboBox *columnCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_TEXT2_LABEL);
  wxString str = wxString::FromUTF8(Style->GetLabelColumn());
  int idSel = columnCtrl->FindString(str);
  columnCtrl->SetSelection(idSel);
  columnCtrl->Enable(Style->IsLabelPrint());
  wxComboBox *fontCtrl = (wxComboBox *) FindWindow(ID_SYMBOLIZER_FONT2_NAME);
  str = wxString::FromUTF8(Style->GetFontFacename());
  idSel = fontCtrl->FindString(str);
  fontCtrl->SetSelection(idSel);
  fontCtrl->Enable(Style->IsLabelPrint());
  wxSlider *opacityFontCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FONT2_OPACITY);
  opacityFontCtrl->SetValue(Style->GetFontOpacity() * 100.0);
  opacityFontCtrl->Enable(Style->IsLabelPrint());
  wxTextCtrl *colorFontCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT2_COLOR);
  wxButton *pickFont = (wxButton *) FindWindow(ID_SYMBOLIZER_FONT2_PICKER_BTN);
  wxColour color = wxNullColour;
  str = wxString::FromUTF8(Style->GetFontColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorFontCtrl->SetValue(str);
    }
  colorFontCtrl->Enable(Style->IsLabelPrint());
  pickFont->Enable(Style->IsLabelPrint());
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FONT2_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Style->GetFontSize());
  str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  sizeCtrl->Enable(Style->IsLabelPrint());
  wxCheckBox *boldCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_BOLD);
  wxCheckBox *italicCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT2_ITALIC);
  if (Style->GetFontWeight() == RL2_FONTWEIGHT_BOLD)
    {
      boldCtrl->SetValue(true);
  } else
    boldCtrl->SetValue(false);
  if (Style->GetFontStyle() == RL2_FONTSTYLE_ITALIC)
    italicCtrl->SetValue(true);
  else
    italicCtrl->SetValue(false);
  if (Style->GetFontFacename() != NULL)
    {
      if (strncmp(Style->GetFontFacename(), "ToyFont: ", 9) == 0)
        {
          boldCtrl->Enable(Style->IsLabelPrint());
          italicCtrl->Enable(Style->IsLabelPrint());
      } else
        {
          bool bold = false;
          bool italic = false;
          MainFrame->CheckTTFont(Style->GetFontFacename(), &bold, &italic);
          boldCtrl->SetValue(bold);
          italicCtrl->SetValue(italic);
          boldCtrl->Enable(false);
          italicCtrl->Enable(false);
        }
    }
// Line Placement
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_PERPENDICULAR);
  sprintf(dummy, "%1.2f", Style->GetLabelPerpendicularOffset());
  str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  perpCtrl->Enable(Style->IsLabelPrint());
  wxCheckBox *repeatBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT_IS_REPEATED);
  repeatBox->SetValue(Style->IsRepeatedLabel());
  repeatBox->Enable(Style->IsLabelPrint());
  wxTextCtrl *inigapCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_INITIAL_GAP);
  sprintf(dummy, "%1.2f", Style->GetLabelInitialGap());
  str = wxString::FromUTF8(dummy);
  inigapCtrl->SetValue(str);
  wxTextCtrl *gapCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TEXT_GAP);
  sprintf(dummy, "%1.2f", Style->GetLabelGap());
  str = wxString::FromUTF8(dummy);
  gapCtrl->SetValue(str);
  if (Style->IsLabelPrint() && Style->IsRepeatedLabel())
    {
      inigapCtrl->Enable(true);
      gapCtrl->Enable(true);
  } else
    {
      inigapCtrl->Enable(false);
      gapCtrl->Enable(false);
    }
  wxCheckBox *alignBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT_IS_ALIGNED);
  alignBox->SetValue(Style->IsLabelAligned());
  alignBox->Enable(Style->IsLabelPrint());
  wxCheckBox *generalizeBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_TEXT_GENERALIZE);
  generalizeBox->SetValue(Style->IsLabelGeneralizeLine());
  generalizeBox->Enable(Style->IsLabelPrint());
  wxCheckBox *enableHaloBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_HALO2_ENABLE);
  if (Style->IsHaloEnabled() == true)
    enableHaloBox->SetValue(true);
  else
    enableHaloBox->SetValue(false);
  enableHaloBox->Enable(Style->IsLabelPrint());
  bool enable = false;
  if (Style->IsLabelPrint() == true)
    enable = Style->IsHaloEnabled();
  wxTextCtrl *radiusCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO2_RADIUS);
  radiusCtrl->Enable(enable);
  wxTextCtrl *colorHaloCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO2_COLOR);
  colorHaloCtrl->Enable(enable);
  wxButton *pickHalo = (wxButton *) FindWindow(ID_SYMBOLIZER_HALO2_PICKER_BTN);
  pickHalo->Enable(enable);
  wxSlider *opacityHaloCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_HALO2_OPACITY);
  opacityHaloCtrl->Enable(enable);
  opacityHaloCtrl->SetValue(Style->GetHaloOpacity() * 100.0);
  sprintf(dummy, "%1.2f", Style->GetHaloRadius());
  str = wxString::FromUTF8(dummy);
  radiusCtrl->SetValue(str);
  color = wxNullColour;
  str = wxString::FromUTF8(Style->GetHaloColor());
  ColorMapEntry::GetWxColor(str, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorHaloCtrl->SetValue(str);
    }
  if (Style->IsHaloEnabled() == false)
    {
      opacityHaloCtrl->Enable(false);
      pickHalo->Enable(false);
      colorHaloCtrl->Enable(false);
      radiusCtrl->Enable(false);
  } else
    {
      opacityHaloCtrl->Enable(true);
      pickHalo->Enable(true);
      colorHaloCtrl->Enable(true);
      radiusCtrl->Enable(true);
    }
}

void QuickStyleVectorDialog::OnPageChanging(wxNotebookEvent & event)
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
      if (idx == PagePointIndex)
        ret = RetrievePointPage();
      if (idx == PageLineIndex)
        ret = RetrieveLinePage();
      if (idx == PagePolygonIndex)
        ret = RetrievePolygonPage();
      if (idx == PageTextPointIndex)
        ret = RetrieveTextPointPage();
      if (idx == PageTextLineIndex)
        ret = RetrieveTextLinePage();
    }
  if (ret != true)
    event.Veto();
}

void QuickStyleVectorDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  int idx = event.GetSelection();
  if (idx == 0)
    UpdateMainPage();
  else
    {
      if (idx == PagePointIndex)
        UpdatePointPage();
      if (idx == PageLineIndex)
        UpdateLinePage();
      if (idx == PagePolygonIndex)
        UpdatePolygonPage();
      if (idx == PageTextPointIndex)
        UpdateTextPointPage();
      if (idx == PageTextLineIndex)
        UpdateTextLinePage();
    }
}

bool QuickStyleVectorDialog::UpdateStyle()
{
//
// updating the QuickStyle
//
  bool ret = false;
  int idx = GetBookCtrl()->GetSelection();
  if (idx == 0)
    ret = RetrieveMainPage();
  else
    {
      if (idx == PagePointIndex)
        ret = RetrievePointPage();
      if (idx == PageLineIndex)
        ret = RetrieveLinePage();
      if (idx == PagePolygonIndex)
        ret = RetrievePolygonPage();
      if (idx == PageTextPointIndex)
        ret = RetrieveTextPointPage();
      if (idx == PageTextLineIndex)
        ret = RetrieveTextLinePage();
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

void QuickStyleVectorDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// permanently saving the QuickStyle and quitting
//
  if (UpdateStyle() == true)
    wxDialog::EndModal(wxID_OK);
}

void QuickStyleVectorDialog::OnApply(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleVectorDialog::OnExport(wxCommandEvent & WXUNUSED(event))
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
  else
    {
      if (idx == PagePointIndex)
        xret = RetrievePointPage();
      if (idx == PageLineIndex)
        xret = RetrieveLinePage();
      if (idx == PagePolygonIndex)
        xret = RetrievePolygonPage();
      if (idx == PageTextPointIndex)
        xret = RetrieveTextPointPage();
      if (idx == PageTextLineIndex)
        xret = RetrieveTextLinePage();
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

void QuickStyleVectorDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the Quick Style into the Clipboard 
//
  bool ret = false;
  int idx = GetBookCtrl()->GetSelection();
  if (idx == 0)
    ret = RetrieveMainPage();
  else
    {
      if (idx == PagePointIndex)
        ret = RetrievePointPage();
      if (idx == PageLineIndex)
        ret = RetrieveLinePage();
      if (idx == PagePolygonIndex)
        ret = RetrievePolygonPage();
      if (idx == PageTextPointIndex)
        ret = RetrieveTextPointPage();
      if (idx == PageTextLineIndex)
        ret = RetrieveTextLinePage();
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

void QuickStyleVectorDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}
