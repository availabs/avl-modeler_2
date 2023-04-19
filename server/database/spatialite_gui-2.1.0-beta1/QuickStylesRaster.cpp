/*
/ QuickStylesRaster.cpp
/ Quick Styles wizards (Raster layers)
/
/ version 2.0, 2018 June 7
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

QuickStyleWmsObj::QuickStyleWmsObj()
{
// ctor
  QuickStyleObj::DoGetUUID(UUID);
  MinScaleEnabled = false;
  MaxScaleEnabled = false;
  ScaleMin = 0.0;
  ScaleMax = 0.0;
}

QuickStyleWmsObj *QuickStyleWmsObj::Clone()
{
//
// cloning a Quick Style - Raster
//
  QuickStyleWmsObj *Style = new QuickStyleWmsObj();
  strcpy(Style->UUID, this->UUID);
  Style->MinScaleEnabled = this->MinScaleEnabled;
  Style->MaxScaleEnabled = this->MaxScaleEnabled;
  Style->ScaleMin = this->ScaleMin;
  Style->ScaleMax = this->ScaleMax;
  return Style;
}

bool QuickStyleWmsObj::Compare(QuickStyleWmsObj * Style)
{
//
// comparing two Quick Style objects - WMS
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
  return true;
}

QuickStyleRasterObj::QuickStyleRasterObj()
{
// ctor
  QuickStyleObj::DoGetUUID(UUID);
  MinScaleEnabled = false;
  MaxScaleEnabled = false;
  ScaleMin = 0.0;
  ScaleMax = 0.0;
  Opacity = 1.0;
  Normalize = false;
  Histogram = false;
  Gamma = false;
  GammaValue = 1.0;
  TripleBand = false;
  SingleBand = false;
  RedBand = 0;
  GreenBand = 0;
  BlueBand = 0;
  GrayBand = 0;
  ColorMap = COLOR_MAP_NONE;
  NdviColorMap = false;
  NdwiColorMap = false;
  ColorRamp = false;
  MinValue = 0.0;
  ShadedRelief = false;
  ShadedReliefFactor = 0.0;
  QuickStyleObj::RandomColor(MinValueColor);
  MaxValue = 0.0;
  QuickStyleObj::RandomColor(MaxValueColor);
  XmlStyle = NULL;
}

QuickStyleRasterObj *QuickStyleRasterObj::Clone()
{
//
// cloning a Quick Style - Raster
//
  QuickStyleRasterObj *Style = new QuickStyleRasterObj();
  strcpy(Style->UUID, this->UUID);
  Style->MinScaleEnabled = this->MinScaleEnabled;
  Style->MaxScaleEnabled = this->MaxScaleEnabled;
  Style->ScaleMin = this->ScaleMin;
  Style->ScaleMax = this->ScaleMax;
  Style->Opacity = this->Opacity;
  Style->Normalize = this->Normalize;
  Style->Histogram = this->Histogram;
  Style->Gamma = this->Gamma;
  Style->GammaValue = this->GammaValue;
  Style->TripleBand = this->TripleBand;
  Style->SingleBand = this->SingleBand;
  Style->RedBand = this->RedBand;
  Style->GreenBand = this->GreenBand;
  Style->BlueBand = this->BlueBand;
  Style->GrayBand = this->GrayBand;
  Style->ColorMap = this->ColorMap;
  Style->NdviColorMap = this->NdviColorMap;
  Style->NdwiColorMap = this->NdwiColorMap;
  Style->ColorRamp = this->ColorRamp;
  Style->MinValue = this->MinValue;
  strcpy(Style->MinValueColor, this->MinValueColor);
  Style->MaxValue = this->MaxValue;
  strcpy(Style->MaxValueColor, this->MaxValueColor);
  Style->ShadedRelief = this->ShadedRelief;
  Style->ShadedReliefFactor = this->ShadedReliefFactor;
  Style->XmlStyle = NULL;
  return Style;
}

bool QuickStyleRasterObj::Compare(QuickStyleRasterObj * Style)
{
//
// comparing two Quick Style objects - Raster
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
  if (Style->Opacity != this->Opacity)
    return false;
  if (Style->Normalize != this->Normalize)
    return false;
  if (Style->Histogram != this->Histogram)
    return false;
  if (Style->Gamma != this->Gamma)
    return false;
  if (Style->GammaValue != this->GammaValue)
    return false;
  if (Style->TripleBand != this->TripleBand)
    return false;
  if (Style->SingleBand != this->SingleBand)
    return false;
  if (Style->RedBand != this->RedBand)
    return false;
  if (Style->GreenBand != this->GreenBand)
    return false;
  if (Style->BlueBand != this->BlueBand)
    return false;
  if (Style->GrayBand != this->GrayBand)
    return false;
  if (Style->ColorMap != this->ColorMap)
    return false;
  if (Style->NdviColorMap != this->NdviColorMap)
    return false;
  if (Style->NdwiColorMap != this->NdwiColorMap)
    return false;
  if (Style->ColorRamp != this->ColorRamp)
    return false;
  if (Style->MinValue != this->MinValue)
    return false;
  if (strcmp(Style->MinValueColor, this->MinValueColor) != 0)
    return false;
  if (Style->MaxValue != this->MaxValue)
    return false;
  if (strcmp(Style->MaxValueColor, this->MaxValueColor) != 0)
    return false;
  if (Style->ShadedRelief != this->ShadedRelief)
    return false;
  if (Style->ShadedReliefFactor != this->ShadedReliefFactor)
    return false;
  return true;
}

char *QuickStyleRasterObj::DoCreatePredefinedEtopo2Style(const char *indent)
{
//
// creating the predefined Etopo2 Style
//
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-11000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-5000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000064</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-1000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#3232c8</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-1.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#9696ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#009600</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>270.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#5aa55a</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>300.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#5aaf5a</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>500.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#32b432</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>1000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#46914b</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>2000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#969c64</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>3000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#dcdcdc</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>4000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#f5f5f5</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>8850.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedSrtmStyle(const char *indent)
{
//
// creating the predefined SRTM Style
//
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-11000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-8000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000032</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-5000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#0a0a64</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-3000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#1e1e96</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-1000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#4646c8</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-100.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#6464e1</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#aaaaff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.10000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#399769</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>100.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#75c25d</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>500.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#e6e680</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>1000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ca9e4b</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>2000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#b99a64</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>3000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#dcdcdc</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>5000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>8850.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#6464c8</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedTerrainStyle(const char *indent)
{
//
// creating the predefined Terrain Style
//
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-11000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-500.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00001e</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>-100.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#0000c8</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-1.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#9696ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#007800</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>100.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#009600</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>270.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#5aa55a</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>300.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#5aaf5a</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>500.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#32b432</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>500.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#46aa46</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>1000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#46914b</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>1000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#469b4b</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>2000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#969c64</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>2800.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#dcdcdc</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>3000.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>8850.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedElevationStyle(const char *indent,
                                                            double min_value,
                                                            double max_value)
{
//
// creating the predefined Elevation Style
//
  double interval = (max_value - min_value) / 5.0;
  double value = min_value;
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00bfbf</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00ff00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffff00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ff7f00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#bf7f3f</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent,
                    max_value);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#c8c8c8</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::
DoCreatePredefinedAspectColorStyle(const char *indent, double min_value,
                                   double max_value)
{
//
// creating the predefined AspectColor Style
//
  double interval = (max_value - min_value) / 4.0;
  double value = min_value;
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffff00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00ffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ff0000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent,
                    max_value);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffff00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedRainbowStyle(const char *indent,
                                                          double min_value,
                                                          double max_value)
{
//
// creating the predefined Rainbow Style
//
  double interval = (max_value - min_value) / 5.0;
  double value = min_value;
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffff00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00ff00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00ffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#0000ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ff00ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent,
                    max_value);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ff0000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedWaveStyle(const char *indent,
                                                       double min_value,
                                                       double max_value)
{
//
// creating the predefined Wave Style
//
  double interval = (max_value - min_value) / 6.0;
  double value = min_value;
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ff8585</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#aaaa00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#55ff55</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#00aaaa</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#5555ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#aa00aa</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent,
                    max_value);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ff5555</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedSepiaStyle(const char *indent,
                                                        double min_value,
                                                        double max_value)
{
//
// creating the predefined Sepia Style
//
  double interval = (max_value - min_value) / 10.0;
  double value = min_value;
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#221709</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#442f13</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#654821</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#826031</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#9e7a47</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#b79760</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ceae7e</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffc8a2</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent, value);
  value += interval;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#f2e3cc</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t\t<Data>%1.6f</Data>\r\n", prev, indent,
                    max_value);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#fffefb</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedNdviStyle(const char *indent)
{
//
// creating the predefined NDVI Style
//
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-1.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#051852</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-0.30000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#051852</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-0.18000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.02500000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#cec5b4</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.07500000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#bfa37c</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.12500000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#b3ae60</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.15000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#a3b550</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.17500000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#90aa3c</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.23300000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#a6c31d</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.26600000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#87b703</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.33300000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#79af01</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.36600000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#65a300</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.43300000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#4e9700</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.46600000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#2b8404</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.55000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#007200</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.65000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#005a01</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.75000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#004900</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.85000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#003800</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.95000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#001f00</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>1.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#000000</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::DoCreatePredefinedNdwiStyle(const char *indent)
{
//
// creating the predefined NDWI Style
//
  char *prev;
  char *xml = sqlite3_mprintf("%s<ColorMap>\r\n", indent);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t<Interpolate fallbackValue=\"#ffffff\">\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s%s\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-1.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#ffffff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>-0.20000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#cdc1ad</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#969696</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>0.50000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#9696ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t<InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Data>1.00000000</Data>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t\t<Value>#0000ff</Value>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t\t</InterpolationPoint>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s\t</Interpolate>\r\n", prev, indent);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s%s</ColorMap>\r\n", prev, indent);
  sqlite3_free(prev);
  return xml;
}

char *QuickStyleRasterObj::CreateXmlStyle()
{
//
// creating the XML Style
//
  if (MinScaleEnabled == false && MaxScaleEnabled == false && Opacity == 1.0
      && Normalize == false && Histogram == false && Gamma == false
      && TripleBand == false && SingleBand == false
      && ColorMap == COLOR_MAP_NONE && NdviColorMap == false
      && NdwiColorMap == false && ColorRamp == false && ShadedRelief == false)
    return NULL;                // NULL Raster Style - no option is set

  char *prev;
  char *xml2;
  const char *indent = "\t\t\t";
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<CoverageStyle version=\"1.1.0\" ", prev);
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
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  if (ColorMap == COLOR_MAP_ETOPO2)
    {
      // special case: predefined Etopo2 style
      xml2 = DoCreatePredefinedEtopo2Style(indent);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_SRTM)
    {
      // special case: predefined SRTM style
      xml2 = DoCreatePredefinedSrtmStyle(indent);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_TERRAIN)
    {
      // special case: predefined Terrain style
      xml2 = DoCreatePredefinedTerrainStyle(indent);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_ELEVATION)
    {
      // special case: predefined Elevation style
      xml2 = DoCreatePredefinedElevationStyle(indent, MinValue, MaxValue);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_ASPECT_COLOR)
    {
      // special case: predefined AspectColor style
      xml2 = DoCreatePredefinedAspectColorStyle(indent, MinValue, MaxValue);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_RAINBOW)
    {
      // special case: predefined Rainbow style
      xml2 = DoCreatePredefinedRainbowStyle(indent, MinValue, MaxValue);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_WAVE)
    {
      // special case: predefined Wave style
      xml2 = DoCreatePredefinedWaveStyle(indent, MinValue, MaxValue);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (ColorMap == COLOR_MAP_SEPIA)
    {
      // special case: predefined Sepia style
      xml2 = DoCreatePredefinedSepiaStyle(indent, MinValue, MaxValue);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto shaded_relief;
    }
  if (NdviColorMap == true)
    {
      // special case: predefined NDVI style
      xml2 = DoCreatePredefinedNdviStyle(indent);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto foot;
    }
  if (NdwiColorMap == true)
    {
      // special case: predefined NDWI style
      xml2 = DoCreatePredefinedNdwiStyle(indent);
      if (xml2 != NULL)
        {
          xml = sqlite3_mprintf("%s%s", prev, xml2);
          sqlite3_free(prev);
          sqlite3_free(xml2);
        }
      prev = xml;
      goto foot;
    }
  if (TripleBand == true || SingleBand == true)
    {
      xml = sqlite3_mprintf("%s\t\t\t<ChannelSelection>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (TripleBand == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<RedChannel>\r\n\t\t\t\t\t<SourceChannelName>%d", prev,
             RedBand + 1);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s</SourceChannelName>\r\n\t\t\t\t</RedChannel>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<GreenChannel>\r\n\t\t\t\t\t<SourceChannelName>%d",
             prev, GreenBand + 1);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s</SourceChannelName>\r\n\t\t\t\t</GreenChannel>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<BlueChannel>\r\n\t\t\t\t\t<SourceChannelName>%d", prev,
             BlueBand + 1);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s</SourceChannelName>\r\n\t\t\t\t</BlueChannel>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (SingleBand == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<GrayChannel>\r\n\t\t\t\t\t<SourceChannelName>%d", prev,
             GrayBand + 1);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s</SourceChannelName>\r\n\t\t\t\t</GrayChannel>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t\t</ChannelSelection>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Normalize == true || Histogram == true || Gamma == true)
    {
      xml = sqlite3_mprintf("%s\t\t\t<ContrastEnhancement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Histogram == true)
        {
          xml = sqlite3_mprintf("%s\t\t\t\t<Histogram/>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
      } else if (Gamma == true)
        {
          xml =
            sqlite3_mprintf("%s\t\t\t\t<GammaValue>%1.2f</GammaValue>\r\n",
                            prev, GammaValue);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          xml = sqlite3_mprintf("%s\t\t\t\t<Normalize/>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t\t</ContrastEnhancement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (ColorRamp == true)
    {
      xml = sqlite3_mprintf("%s\t\t\t<ColorMap>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<Interpolate fallbackValue=\"%s\">\r\n",
                        prev, MinValueColor);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                        prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t<InterpolationPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<Data>%1.8f</Data>\r\n", prev, MinValue);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<Value>%s</Value>\r\n", prev,
                        MinValueColor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t</InterpolationPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t<InterpolationPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<Data>%1.8f</Data>\r\n", prev, MaxValue);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t<Value>%s</Value>\r\n", prev,
                        MaxValueColor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t\t</InterpolationPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</Interpolate>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</ColorMap>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
shaded_relief:
  if (ShadedRelief == true)
    {
      xml = sqlite3_mprintf("%s\t\t\t<ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n",
                        prev, ShadedReliefFactor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
foot:
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

unsigned char *QuickStyleRasterObj::CloneXmlStyle()
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

void QuickStyleRasterObj::UpdateXmlStyle()
{
//
// updating the XML Style
//
  if (XmlStyle != NULL)
    sqlite3_free(XmlStyle);
  XmlStyle = CreateXmlStyle();
}

bool QuickStyleRasterDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  CanApplyContrastEnhancement = true;
  IsMultiband = false;
  CanApplyColorMap = false;
  CanApplyShadedRelief = false;
  RedBand = 0;
  GreenBand = 0;
  BlueBand = 0;
  GrayBand = 0;
  MinPixelValue = 0.0;
  MaxPixelValue = 0.0;
  GetCoverageInfos();
  IsConfigChanged = false;

  if (wxPropertySheetDialog::Create
      (parent, wxID_ANY, wxT("QuickStyle (Raster) Edit")) == false)
    return false;
  if (Layer->GetQuickStyleRaster() != NULL)
    Style = Layer->CloneQuickStyleRaster();
  else
    {
      Style = new QuickStyleRasterObj();
      Style->SetRedBand(RedBand);
      Style->SetGreenBand(GreenBand);
      Style->SetBlueBand(BlueBand);
      Style->SetGrayBand(GrayBand);
      Style->SetMinValue(MinPixelValue);
      Style->SetMaxValue(MaxPixelValue);
    }

  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  int next = 1;
  PageContrastEnhancementIndex = 0;
  PageChannelSelectionIndex = 0;
  PageColorMapIndex = 0;
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  wxPanel *contrastEnhancementPage = CreateContrastEnhancementPage(book);
  book->AddPage(contrastEnhancementPage, wxT("ContrastEnhancement"), false);
  PageContrastEnhancementIndex = next++;
  wxPanel *channelSelectionPage = CreateChannelSelectionPage(book);
  book->AddPage(channelSelectionPage, wxT("ChannelSelection"), false);
  PageChannelSelectionIndex = next++;
  wxPanel *colorMapPage = CreateColorMapPage(book);
  book->AddPage(colorMapPage, wxT("ColorMap"), false);
  PageColorMapIndex = next++;

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnQuit);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnOk);
  Connect(ID_QUICK_STYLE_APPLY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnApply);
  Connect(ID_QUICK_STYLE_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnExport);
  Connect(ID_QUICK_STYLE_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

void QuickStyleRasterDialog::CreateButtons()
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

wxPanel *QuickStyleRasterDialog::CreateMainPage(wxWindow * parent)
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
// third row: Opacity 
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
                 wxDefaultPosition, wxSize(400, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnCmdScaleChanged);
  return panel;
}

void QuickStyleRasterDialog::
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

wxPanel *QuickStyleRasterDialog::CreateContrastEnhancementPage(wxWindow *
                                                               parent)
{
//
// creating the Contrast Enhancement page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_CONTRAST_ENHANCEMENT);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: Contrast Enhancement
  boxSizer->AddSpacer(100);
  wxBoxSizer *contrastSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(contrastSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString contrast[4];
  contrast[0] = wxT("&None");
  contrast[1] = wxT("Normalize/&Stretch");
  contrast[2] = wxT("&Histogram");
  contrast[3] = wxT("&GammaValue");
  wxRadioBox *contrastBox = new wxRadioBox(panel, ID_SYMBOLIZER_CONTRAST,
                                           wxT("&Contrast Enhancement Method"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 4,
                                           contrast, 1,
                                           wxRA_SPECIFY_COLS);
  contrastSizer->Add(contrastBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  contrastBox->SetSelection(0);
  if (CanApplyContrastEnhancement == false)
    contrastBox->Enable(false);
  wxBoxSizer *gammaSizer = new wxBoxSizer(wxHORIZONTAL);
  contrastSizer->Add(gammaSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticText *gammaLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&GammaValue [x100]:"));
  gammaSizer->Add(gammaLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *gammaCtrl = new wxSpinCtrl(panel, ID_SYMBOLIZER_GAMMA, wxT("100"),
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxSP_ARROW_KEYS,
                                         100, 500, 100);
  gammaCtrl->Enable(false);
  gammaSizer->Add(gammaCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handler for OK button
  Connect(ID_SYMBOLIZER_CONTRAST, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdContrastChanged);
  return panel;
}

void QuickStyleRasterDialog::
OnCmdContrastChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Contrast Enhancement Method selection changed
//
  wxRadioBox *contrastCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CONTRAST);
  wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
  switch (contrastCtrl->GetSelection())
    {
      case 1:
        Style->SetNormalize(true);
        Style->SetHistogram(false);
        Style->SetGamma(false);
        gammaCtrl->Enable(false);
        break;
      case 2:
        Style->SetNormalize(false);
        Style->SetHistogram(true);
        Style->SetGamma(false);
        gammaCtrl->Enable(false);
        break;
      case 3:
        Style->SetNormalize(false);
        Style->SetHistogram(false);
        Style->SetGamma(true);
        gammaCtrl->Enable(true);
        break;
      default:
        Style->SetNormalize(false);
        Style->SetHistogram(false);
        Style->SetGamma(false);
        gammaCtrl->Enable(false);
        break;
    };
}

wxPanel *QuickStyleRasterDialog::CreateChannelSelectionPage(wxWindow * parent)
{
//
// creating the Channel Selection page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_CHANNEL_SELECTION);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
  panel->SetSizer(topSizer);
// Left panel: mode selector
  boxSizer->AddSpacer(50);
  wxBoxSizer *pageSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pageSizer, 0, wxALIGN_CENTER_VERTICAL);
  wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
  pageSizer->Add(leftSizer, 0, wxALIGN_CENTER_VERTICAL);
  wxString mode[3];
  mode[0] = wxT("&None");
  mode[1] = wxT("&Triple Band (RGB Image)");
  mode[2] = wxT("&Single Band (Grayscale Image)");
  wxRadioBox *bandModeBox = new wxRadioBox(panel, ID_SYMBOLIZER_BAND_MODE,
                                           wxT("&Channel Selection Method"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 3,
                                           mode, 1,
                                           wxRA_SPECIFY_COLS);
  leftSizer->Add(bandModeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  bandModeBox->SetSelection(0);
  if (IsMultiband == false)
    bandModeBox->Enable(false);
// First row: ChannelSelection RGB
  wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
  pageSizer->Add(rightSizer, 0, wxALIGN_CENTER_HORIZONTAL);
  wxBoxSizer *channelBoxSizer = new wxBoxSizer(wxVERTICAL);
  rightSizer->Add(channelBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *channelBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT
                                            ("Channel Selection (first Band is #1)"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *channelSizer = new wxStaticBoxSizer(channelBox, wxVERTICAL);
  channelBoxSizer->Add(channelSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *bandSizer = new wxBoxSizer(wxVERTICAL);
  channelSizer->Add(bandSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *redSizer = new wxBoxSizer(wxHORIZONTAL);
  bandSizer->Add(redSizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *redLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Red Channel is Band #"));
  redSizer->Add(redLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *redCtrl = new wxSpinCtrl(panel, ID_SYMBOLIZER_RED, wxT("1"),
                                       wxDefaultPosition, wxSize(80, 22),
                                       wxSP_ARROW_KEYS,
                                       1, 256, 1);
  redSizer->Add(redCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  redCtrl->Enable(false);
  wxBoxSizer *greenSizer = new wxBoxSizer(wxHORIZONTAL);
  bandSizer->Add(greenSizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *greenLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Green Channel is Band #"));
  greenSizer->Add(greenLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *greenCtrl = new wxSpinCtrl(panel, ID_SYMBOLIZER_GREEN, wxT("2"),
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxSP_ARROW_KEYS,
                                         1, 256, 2);
  greenSizer->Add(greenCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  greenCtrl->Enable(false);
  wxBoxSizer *blueSizer = new wxBoxSizer(wxHORIZONTAL);
  bandSizer->Add(blueSizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *blueLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Blue Channel is Band #"));
  blueSizer->Add(blueLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *blueCtrl = new wxSpinCtrl(panel, ID_SYMBOLIZER_BLUE, wxT("3"),
                                        wxDefaultPosition, wxSize(80, 22),
                                        wxSP_ARROW_KEYS,
                                        1, 256, 3);
  blueSizer->Add(blueCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  blueCtrl->Enable(false);
// Second row: ChannelSelection Grayscale
  rightSizer->AddSpacer(25);
  wxBoxSizer *channel2BoxSizer = new wxBoxSizer(wxVERTICAL);
  rightSizer->Add(channel2BoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *channel2Box = new wxStaticBox(panel, wxID_STATIC,
                                             wxT
                                             ("Channel Selection (first Band is #1)"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *channel2Sizer = new wxStaticBoxSizer(channel2Box, wxVERTICAL);
  channel2BoxSizer->Add(channel2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *band2Sizer = new wxBoxSizer(wxVERTICAL);
  channel2Sizer->Add(band2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *graySizer = new wxBoxSizer(wxHORIZONTAL);
  band2Sizer->Add(graySizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *grayLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Gray Channel is Band #"));
  graySizer->Add(grayLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *grayCtrl = new wxSpinCtrl(panel, ID_SYMBOLIZER_GRAY, wxT("1"),
                                        wxDefaultPosition, wxSize(80, 22),
                                        wxSP_ARROW_KEYS,
                                        1, 256, 1);
  graySizer->Add(grayCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  grayCtrl->Enable(false);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_BAND_MODE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdBandModeChanged);
  return panel;
}

void QuickStyleRasterDialog::
OnCmdBandModeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Channel Selection Method selection changed
//
  wxRadioBox *bandModeCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_BAND_MODE);
  wxSpinCtrl *redCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RED);
  wxSpinCtrl *greenCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GREEN);
  wxSpinCtrl *blueCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_BLUE);
  wxSpinCtrl *grayCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GRAY);
  switch (bandModeCtrl->GetSelection())
    {
      case 1:
        Style->SetTripleBand(true);
        Style->SetSingleBand(false);
        redCtrl->Enable(true);
        greenCtrl->Enable(true);
        blueCtrl->Enable(true);
        grayCtrl->Enable(false);
        break;
      case 2:
        Style->SetTripleBand(false);
        Style->SetSingleBand(true);
        redCtrl->Enable(false);
        greenCtrl->Enable(false);
        blueCtrl->Enable(false);
        grayCtrl->Enable(true);
        break;
      default:
        Style->SetTripleBand(false);
        Style->SetSingleBand(false);
        redCtrl->Enable(false);
        greenCtrl->Enable(false);
        blueCtrl->Enable(false);
        grayCtrl->Enable(false);
        break;
    };
}

wxPanel *QuickStyleRasterDialog::CreateColorMapPage(wxWindow * parent)
{
//
// creating the ColorMap page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_COLOR_MAP);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
  boxSizer->AddSpacer(50);
  wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(mainSizer, 0, wxALIGN_CENTER | wxALL, 5);
// left box: mode selector
  wxBoxSizer *modeSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(modeSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  int max = 10;
  int cols = 2;
  wxString mode[10];
  mode[0] = wxT("&None");
  if (CanApplyNDVI == true)
    {
      mode[1] = wxT("&NDVI (Vegetation Index)");
      mode[2] = wxT("&NDWI (Water Index)");
      max = 3;
      cols = 1;
  } else
    {
      mode[1] = wxT("&Etopo2");
      mode[2] = wxT("&SRTM");
      mode[3] = wxT("&Terrain");
      mode[4] = wxT("&Elevation");
      mode[5] = wxT("&AspectColor");
      mode[6] = wxT("&Rainbow");
      mode[7] = wxT("&Wave");
      mode[8] = wxT("&Sepia");
      mode[9] = wxT("&Color Ramp");
    }
  wxRadioBox *modeBox = new wxRadioBox(panel, ID_SYMBOLIZER_COLOR_MAP_MODE,
                                       wxT("&Color Map Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, max,
                                       mode, cols,
                                       wxRA_SPECIFY_COLS);
  modeSizer->Add(modeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  modeBox->SetSelection(0);
  if (CanApplyColorMap == false && CanApplyNDVI == false)
    modeBox->Enable(false);
// right box: Color Ramp
  wxBoxSizer *rampSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(rampSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *rampBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Color Ramp"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *ramp2Sizer = new wxStaticBoxSizer(rampBox, wxVERTICAL);
  rampSizer->Add(ramp2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *minSizer = new wxBoxSizer(wxHORIZONTAL);
  ramp2Sizer->Add(minSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
// Min Color
  wxStaticBox *minBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Min Value"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *min2Sizer = new wxStaticBoxSizer(minBox, wxHORIZONTAL);
  minSizer->Add(min2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString minColor = wxT("#000000");
  wxTextCtrl *colorMinCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_MIN_COLOR, minColor,
                   wxDefaultPosition, wxSize(80, 22));
  min2Sizer->Add(colorMinCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  colorMinCtrl->Enable(false);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleMinCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_MIN_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  min2Sizer->Add(sampleMinCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxButton *minPick =
    new wxButton(panel, ID_SYMBOLIZER_MIN_PICKER_BTN, wxT("&Pick a color"));
  min2Sizer->Add(minPick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  minPick->Enable(false);
// Max Color
  wxBoxSizer *maxSizer = new wxBoxSizer(wxHORIZONTAL);
  ramp2Sizer->Add(maxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  wxStaticBox *maxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Max Value"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *max2Sizer = new wxStaticBoxSizer(maxBox, wxHORIZONTAL);
  maxSizer->Add(max2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString maxColor = wxT("#ffffff");
  wxTextCtrl *colorMaxCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_MAX_COLOR, maxColor,
                   wxDefaultPosition, wxSize(80, 22));
  max2Sizer->Add(colorMaxCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  colorMaxCtrl->Enable(false);
  wxBitmap bmp2;
  wxColour color2(255, 255, 255);
  ColorMapEntry::DoPaintColorSample(32, 32, color2, bmp2);
  wxStaticBitmap *sampleMaxCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_MAX_PICKER_HEX, bmp2,
                       wxDefaultPosition, wxSize(32, 32));
  max2Sizer->Add(sampleMaxCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxButton *maxPick =
    new wxButton(panel, ID_SYMBOLIZER_MAX_PICKER_BTN, wxT("&Pick a color"));
  max2Sizer->Add(maxPick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  maxPick->Enable(false);
// bottom box: shaded relief
  wxBoxSizer *shadedSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(shadedSizer, 0, wxALIGN_CENTER_HORIZONTAL);
  wxStaticBox *shadedBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Shaded Relief"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *shaded2Sizer = new wxStaticBoxSizer(shadedBox, wxHORIZONTAL);
  shadedSizer->Add(shaded2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *reliefSizer = new wxBoxSizer(wxHORIZONTAL);
  shaded2Sizer->Add(reliefSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxCheckBox *shadedCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_SHADED,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  reliefSizer->Add(shadedCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  if (CanApplyShadedRelief == false)
    shadedCtrl->Enable(false);
  reliefSizer->AddSpacer(25);
  wxBoxSizer *relief2Sizer = new wxBoxSizer(wxHORIZONTAL);
  reliefSizer->Add(relief2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *reliefLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Relief Factor:"));
  relief2Sizer->Add(reliefLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *reliefCtrl = new wxSpinCtrl(panel, ID_SYMBOLIZER_RELIEF, wxT(""),
                                          wxDefaultPosition, wxSize(80, 22),
                                          wxSP_ARROW_KEYS,
                                          0, 0, 0);
  reliefCtrl->Enable(false);
  relief2Sizer->Add(reliefCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_COLOR_MAP_MODE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdColorMapModeChanged);
  Connect(ID_SYMBOLIZER_MIN_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdColorMinChanged);
  Connect(ID_SYMBOLIZER_MIN_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdMinColorPicker);
  Connect(ID_SYMBOLIZER_MAX_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdColorMaxChanged);
  Connect(ID_SYMBOLIZER_MAX_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          QuickStyleRasterDialog::OnCmdMaxColorPicker);
  Connect(ID_SYMBOLIZER_SHADED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & QuickStyleRasterDialog::OnShadedChanged);
  return panel;
}

void QuickStyleRasterDialog::
OnCmdColorMapModeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Color Map Mode selection changed
//
  wxRadioBox *colorMapModeCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_COLOR_MAP_MODE);
  wxTextCtrl *colorMinCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_COLOR);
  wxButton *minPick = (wxButton *) FindWindow(ID_SYMBOLIZER_MIN_PICKER_BTN);
  wxTextCtrl *colorMaxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_COLOR);
  wxButton *maxPick = (wxButton *) FindWindow(ID_SYMBOLIZER_MAX_PICKER_BTN);
  if (CanApplyNDVI == true)
    {
      // special case: NDVI or NDWI
      Style->SetColorMap(COLOR_MAP_NONE);
      Style->SetColorRamp(false);
      colorMinCtrl->Enable(false);
      minPick->Enable(false);
      colorMaxCtrl->Enable(false);
      maxPick->Enable(false);
      colorMinCtrl->SetValue(wxT("#000000"));
      colorMaxCtrl->SetValue(wxT("#ffffff"));
      switch (colorMapModeCtrl->GetSelection())
        {
          case 1:
            Style->SetNdviColorMap(true);
            Style->SetNdwiColorMap(false);
            break;
          case 2:
            Style->SetNdwiColorMap(true);
            Style->SetNdviColorMap(false);
            break;
          default:
            Style->SetNdviColorMap(false);
            Style->SetNdwiColorMap(false);
            break;
        };
  } else
    {
      // ordinary Color Map
      Style->SetNdviColorMap(false);
      Style->SetNdwiColorMap(false);
      Style->SetColorRamp(false);
      colorMinCtrl->Enable(false);
      minPick->Enable(false);
      colorMaxCtrl->Enable(false);
      maxPick->Enable(false);
      colorMinCtrl->SetValue(wxT("#000000"));
      colorMaxCtrl->SetValue(wxT("#ffffff"));
      switch (colorMapModeCtrl->GetSelection())
        {
          case 1:
            Style->SetColorMap(COLOR_MAP_ETOPO2);
            break;
          case 2:
            Style->SetColorMap(COLOR_MAP_SRTM);
            break;
          case 3:
            Style->SetColorMap(COLOR_MAP_TERRAIN);
            break;
          case 4:
            Style->SetColorMap(COLOR_MAP_ELEVATION);
            break;
          case 5:
            Style->SetColorMap(COLOR_MAP_ASPECT_COLOR);
            break;
          case 6:
            Style->SetColorMap(COLOR_MAP_RAINBOW);
            break;
          case 7:
            Style->SetColorMap(COLOR_MAP_WAVE);
            break;
          case 8:
            Style->SetColorMap(COLOR_MAP_SEPIA);
            break;
          case 9:
            Style->SetColorMap(COLOR_MAP_NONE);
            Style->SetColorRamp(true);
            colorMinCtrl->Enable(true);
            minPick->Enable(true);
            colorMaxCtrl->Enable(true);
            maxPick->Enable(true);
            colorMinCtrl->SetValue(wxString::
                                   FromUTF8(Style->GetMinValueColor()));
            colorMaxCtrl->SetValue(wxString::
                                   FromUTF8(Style->GetMaxValueColor()));
            break;
          default:
            Style->SetColorMap(COLOR_MAP_NONE);
            break;
        };
    }
}

void QuickStyleRasterDialog::
OnCmdColorMinChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Min Value color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_MIN_PICKER_HEX);
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

void QuickStyleRasterDialog::
OnCmdMinColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// Min Value color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_COLOR);
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

void QuickStyleRasterDialog::
OnCmdColorMaxChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Max Value color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_MAX_PICKER_HEX);
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

void QuickStyleRasterDialog::
OnCmdMaxColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// Max Value color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_COLOR);
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

void QuickStyleRasterDialog::OnShadedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Shaded Relief (on/off): 
//
  wxCheckBox *shadedCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_SHADED);
  wxSpinCtrl *reliefCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RELIEF);
  if (shadedCtrl->IsChecked() == true)
    {
      reliefCtrl->SetRange(1, 200);
      reliefCtrl->SetValue(25);
      reliefCtrl->Enable(true);
      Style->SetShadedRelief(true);
  } else
    {
      reliefCtrl->SetRange(0, 0);
      reliefCtrl->SetValue(0);
      reliefCtrl->Enable(false);
      Style->SetShadedRelief(false);
    }
}

bool QuickStyleRasterDialog::UpdateStyle()
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
      if (idx == PageContrastEnhancementIndex)
        ret = RetrieveContrastEnhancementPage();
      if (idx == PageChannelSelectionIndex)
        ret = RetrieveChannelSelectionPage();
      if (idx == PageColorMapIndex)
        ret = RetrieveColorMapPage();
    }
  if (ret == false)
    return false;

  RasterLayerConfig *config = Layer->GetRasterConfig();
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

bool QuickStyleRasterDialog::RetrieveMainPage()
{
//
// retrieving params from the MAIN page
//
  double min = Style->GetScaleMin();
  double max = Style->GetScaleMax();
  double opacity = Style->GetOpacity();
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
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  opacity = opacityCtrl->GetValue() / 100.0;
  Style->SetScaleMin(min);
  Style->SetScaleMax(max);
  Style->SetOpacity(opacity);
  return true;
}

void QuickStyleRasterDialog::UpdateMainPage()
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
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  opacityCtrl->SetValue(Style->GetOpacity() * 100.0);
}

bool QuickStyleRasterDialog::RetrieveContrastEnhancementPage()
{
//
// retrieving params from the CONTRAST ENHANCEMENT page
//
  if (Style->IsGamma() == true)
    {
      wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
      Style->SetGammaValue(gammaCtrl->GetValue() / 100.0);
  } else
    Style->SetGammaValue(1.0);
  return true;
}

void QuickStyleRasterDialog::UpdateContrastEnhancementPage()
{
//
// updating the CONTRAST ENHANCEMENT page
//
  wxRadioBox *contrastCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CONTRAST);
  wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
  if (Style->IsNormalize() == true)
    {
      contrastCtrl->SetSelection(1);
      gammaCtrl->Enable(false);
  } else if (Style->IsHistogram() == true)
    {
      contrastCtrl->SetSelection(2);
      gammaCtrl->Enable(false);
  } else if (Style->IsGamma() == true)
    {
      contrastCtrl->SetSelection(3);
      gammaCtrl->Enable(true);
  } else
    {
      contrastCtrl->SetSelection(0);
      gammaCtrl->Enable(false);
    }
  gammaCtrl->SetValue(Style->GetGammaValue() * 100.0);
  if (CanApplyContrastEnhancement == true)
    contrastCtrl->Enable(true);
  else
    contrastCtrl->Enable(false);
}

bool QuickStyleRasterDialog::RetrieveChannelSelectionPage()
{
//
// retrieving params from the CHANNEL SELECTION page
//
  if (Style->IsTripleBand() == true)
    {
      wxSpinCtrl *redCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RED);
      Style->SetRedBand(redCtrl->GetValue() - 1);
      wxSpinCtrl *greenCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GREEN);
      Style->SetGreenBand(greenCtrl->GetValue() - 1);
      wxSpinCtrl *blueCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_BLUE);
      Style->SetBlueBand(blueCtrl->GetValue() - 1);
      Style->SetGrayBand(0);
  } else if (Style->IsSingleBand() == true)
    {
      wxSpinCtrl *grayCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GRAY);
      Style->SetGrayBand(grayCtrl->GetValue() - 1);
      Style->SetRedBand(0);
      Style->SetGreenBand(0);
      Style->SetBlueBand(0);
  } else
    {
      Style->SetRedBand(0);
      Style->SetGreenBand(0);
      Style->SetBlueBand(0);
      Style->SetGrayBand(0);
    }
  return true;
}

void QuickStyleRasterDialog::UpdateChannelSelectionPage()
{
//
// updating the CHANNEL SELECTION page
//
  wxRadioBox *bandModeCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_BAND_MODE);
  wxSpinCtrl *redCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RED);
  wxSpinCtrl *greenCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GREEN);
  wxSpinCtrl *blueCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_BLUE);
  wxSpinCtrl *grayCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GRAY);
  if (IsMultiband == false)
    {
      bandModeCtrl->SetSelection(0);
      redCtrl->Enable(false);
      greenCtrl->Enable(false);
      blueCtrl->Enable(false);
      grayCtrl->Enable(false);
      redCtrl->SetValue(1);
      greenCtrl->SetValue(1);
      blueCtrl->SetValue(1);
      grayCtrl->SetValue(1);
      bandModeCtrl->Enable(false);
      return;
    }
  if (Style->IsTripleBand() == true)
    {
      bandModeCtrl->SetSelection(1);
      redCtrl->Enable(true);
      greenCtrl->Enable(true);
      blueCtrl->Enable(true);
      grayCtrl->Enable(false);
  } else if (Style->IsSingleBand() == true)
    {
      bandModeCtrl->SetSelection(2);
      redCtrl->Enable(false);
      greenCtrl->Enable(false);
      blueCtrl->Enable(false);
      grayCtrl->Enable(true);
  } else
    {
      bandModeCtrl->SetSelection(0);
      redCtrl->Enable(false);
      greenCtrl->Enable(false);
      blueCtrl->Enable(false);
      grayCtrl->Enable(false);
    }
  redCtrl->SetRange(1, NumBands);
  greenCtrl->SetRange(1, NumBands);
  blueCtrl->SetRange(1, NumBands);
  grayCtrl->SetRange(1, NumBands);
  redCtrl->SetValue(Style->GetRedBand() + 1);
  greenCtrl->SetValue(Style->GetGreenBand() + 1);
  blueCtrl->SetValue(Style->GetBlueBand() + 1);
  grayCtrl->SetValue(Style->GetGrayBand() + 1);
  if (IsMultiband == true)
    bandModeCtrl->Enable(true);
  else
    bandModeCtrl->Enable(false);
}

bool QuickStyleRasterDialog::RetrieveColorMapPage()
{
//
// retrieving params from the COLOR MAP page
//
  if (Style->IsShadedRelief() == true)
    {
      wxSpinCtrl *shadedCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RELIEF);
      Style->SetShadedReliefFactor(shadedCtrl->GetValue());
  } else
    Style->SetShadedReliefFactor(0.0);
  wxTextCtrl *colorMinCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_COLOR);
  wxTextCtrl *colorMaxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_COLOR);
  char dummy[64];
  strcpy(dummy, colorMinCtrl->GetValue().ToUTF8());
  Style->SetMinValueColor(dummy);
  strcpy(dummy, colorMaxCtrl->GetValue().ToUTF8());
  Style->SetMaxValueColor(dummy);
  return true;
}

void QuickStyleRasterDialog::UpdateColorMapPage()
{
//
// updating the COLOR MAP page
//
  wxRadioBox *modeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_COLOR_MAP_MODE);
  wxTextCtrl *colorMinCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_COLOR);
  wxButton *minPick = (wxButton *) FindWindow(ID_SYMBOLIZER_MIN_PICKER_BTN);
  wxTextCtrl *colorMaxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_COLOR);
  wxButton *maxPick = (wxButton *) FindWindow(ID_SYMBOLIZER_MAX_PICKER_BTN);
  wxCheckBox *shadedCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_SHADED);
  wxSpinCtrl *reliefCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RELIEF);
  if (CanApplyNDVI == true)
    {
      // special case: NDVI
      if (Style->IsNdviColorMap() == true)
        modeBox->SetSelection(1);
      else if (Style->IsNdwiColorMap() == true)
        modeBox->SetSelection(2);
      else
        modeBox->SetSelection(0);
  } else
    {
      // ordinary Color Map
      if (Style->GetColorMap() == COLOR_MAP_ETOPO2)
        modeBox->SetSelection(1);
      else if (Style->GetColorMap() == COLOR_MAP_SRTM)
        modeBox->SetSelection(2);
      else if (Style->GetColorMap() == COLOR_MAP_TERRAIN)
        modeBox->SetSelection(3);
      else if (Style->GetColorMap() == COLOR_MAP_ELEVATION)
        modeBox->SetSelection(4);
      else if (Style->GetColorMap() == COLOR_MAP_ASPECT_COLOR)
        modeBox->SetSelection(5);
      else if (Style->GetColorMap() == COLOR_MAP_RAINBOW)
        modeBox->SetSelection(6);
      else if (Style->GetColorMap() == COLOR_MAP_WAVE)
        modeBox->SetSelection(7);
      else if (Style->GetColorMap() == COLOR_MAP_SEPIA)
        modeBox->SetSelection(8);
      else if (Style->IsColorRamp() == true)
        modeBox->SetSelection(9);
      else
        modeBox->SetSelection(0);
    }
  if (Style->IsColorRamp() == true)
    {
      colorMinCtrl->Enable(true);
      minPick->Enable(true);
      colorMaxCtrl->Enable(true);
      maxPick->Enable(true);
      colorMinCtrl->SetValue(wxString::FromUTF8(Style->GetMinValueColor()));
      colorMaxCtrl->SetValue(wxString::FromUTF8(Style->GetMaxValueColor()));
  } else
    {
      colorMinCtrl->Enable(false);
      minPick->Enable(false);
      colorMaxCtrl->Enable(false);
      maxPick->Enable(false);
      colorMinCtrl->SetValue(wxT("#000000"));
      colorMaxCtrl->SetValue(wxT("#ffffff"));
    }
  if (Style->IsShadedRelief() == true)
    {
      reliefCtrl->SetRange(1, 200);
      reliefCtrl->SetValue(Style->GetShadedReliefFactor());
      reliefCtrl->Enable(true);
      shadedCtrl->SetValue(true);
  } else
    {
      reliefCtrl->SetRange(0, 0);
      reliefCtrl->SetValue(0);
      reliefCtrl->Enable(false);
      shadedCtrl->SetValue(false);
    }
  if (CanApplyColorMap == true || CanApplyNDVI == true)
    modeBox->Enable(true);
  else
    modeBox->Enable(false);
  if (CanApplyShadedRelief == true)
    shadedCtrl->Enable(true);
  else
    shadedCtrl->Enable(false);
}


void QuickStyleRasterDialog::OnPageChanging(wxNotebookEvent & event)
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
      if (idx == PageContrastEnhancementIndex)
        ret = RetrieveContrastEnhancementPage();
      if (idx == PageChannelSelectionIndex)
        ret = RetrieveChannelSelectionPage();
      if (idx == PageColorMapIndex)
        ret = RetrieveColorMapPage();
    }
  if (ret != true)
    event.Veto();
}

void QuickStyleRasterDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  int idx = event.GetSelection();
  if (idx == 0)
    UpdateMainPage();
  else
    {
      if (idx == PageContrastEnhancementIndex)
        UpdateContrastEnhancementPage();
      if (idx == PageChannelSelectionIndex)
        UpdateChannelSelectionPage();
      if (idx == PageColorMapIndex)
        UpdateColorMapPage();
    }
}

void QuickStyleRasterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// permanently saving the QuickStyle and quitting
//
  if (UpdateStyle() == true)
    wxDialog::EndModal(wxID_OK);
}

void QuickStyleRasterDialog::OnApply(wxCommandEvent & WXUNUSED(event))
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

void QuickStyleRasterDialog::OnExport(wxCommandEvent & WXUNUSED(event))
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
      if (idx == PageContrastEnhancementIndex)
        xret = RetrieveContrastEnhancementPage();
      if (idx == PageChannelSelectionIndex)
        xret = RetrieveChannelSelectionPage();
      if (idx == PageColorMapIndex)
        xret = RetrieveColorMapPage();
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

void QuickStyleRasterDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
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
      if (idx == PageContrastEnhancementIndex)
        ret = RetrieveContrastEnhancementPage();
      if (idx == PageChannelSelectionIndex)
        ret = RetrieveChannelSelectionPage();
      if (idx == PageColorMapIndex)
        ret = RetrieveColorMapPage();
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

void QuickStyleRasterDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void QuickStyleRasterDialog::GetCoverageInfos()
{
//
// attempting to get the basic Coverage Infos
//
  char *prefix;
  char *xprefix;
  char *name;
  char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  wxString pixelType;
  wxString sampleType;
  int numBands;
  bool ok_min = false;
  bool ok_max = false;
  double min = 0.0;
  double max = 0.0;

  if (DbPrefix.Len() == 0)
    {
      prefix = (char *) malloc(5);
      strcpy(prefix, "MAIN");
  } else
    {
      prefix = (char *) malloc(DbPrefix.Len() * 4);
      strcpy(prefix, DbPrefix.ToUTF8());
    }
  xprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  name = (char *) malloc(LayerName.Len() * 4);
  strcpy(name, LayerName.ToUTF8());
  sql =
    sqlite3_mprintf
    ("SELECT pixel_type, sample_type, num_bands, enable_auto_ndvi, "
     "RL2_GetBandStatistics_Min(statistics, 0), RL2_GetBandStatistics_Max(statistics, 0) "
     "FROM \"%s\".raster_coverages WHERE Upper(coverage_name) = Upper(%Q)",
     xprefix, name);
  free(xprefix);
  free(name);
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // retrieving the basic Coverage Infos
      pixelType = wxString::FromUTF8(results[(i * columns) + 0]);
      sampleType = wxString::FromUTF8(results[(i * columns) + 1]);
      numBands = atoi(results[(i * columns) + 2]);
      if (results[(i * columns) + 3] == NULL)
        CanApplyNDVI = false;
      else
        {
          if (atoi(results[(i * columns) + 3]) == 0)
            CanApplyNDVI = false;
          else
            CanApplyNDVI = true;
        }
      if (results[(i * columns) + 4] == NULL)
        ok_min = false;
      else
        {
          ok_min = true;
          min = atof(results[(i * columns) + 4]);
        }
      if (results[(i * columns) + 5] == NULL)
        ok_max = false;
      else
        {
          ok_max = true;
          max = atof(results[(i * columns) + 5]);
        }

    }
  sqlite3_free_table(results);

  if (pixelType.Cmp(wxT("RGB")) == 0 || pixelType.Cmp(wxT("GRAYSCALE")) == 0
      || pixelType.Cmp(wxT("MULTIBAND")) == 0
      || (pixelType.Cmp(wxT("DATAGRID")) == 0
          && sampleType.Cmp(wxT("UINT16")) == 0))
    CanApplyContrastEnhancement = true;
  else
    CanApplyContrastEnhancement = false;
  if (pixelType.Cmp(wxT("MULTIBAND")) == 0)
    {
      IsMultiband = true;
      NumBands = numBands;
      RedBand = 0;
      GreenBand = 1;
      BlueBand = 2;
  } else
    IsMultiband = false;
  if (ok_min == true && ok_max == true)
    {
      CanApplyColorMap = true;
      MinPixelValue = min;
      MaxPixelValue = max;
  } else
    CanApplyColorMap = false;
  if (pixelType.Cmp(wxT("DATAGRID")) == 0
      || pixelType.Cmp(wxT("MONOCHROME")) == 0)
    ;
  else
    CanApplyColorMap = false;
  if (pixelType.Cmp(wxT("DATAGRID")) == 0)
    CanApplyShadedRelief = true;
  else
    CanApplyShadedRelief = false;
}

bool QuickStyleWmsDialog::Create(MyMapPanel * parent, MapLayer * layer)
{
//
// creating the dialog
//
  MainFrame = parent->GetParent();
  MapPanel = parent;
  Layer = layer;
  DbPrefix = layer->GetDbPrefix();
  LayerName = layer->GetName();
  if (wxDialog::Create(parent, wxID_ANY, wxT("QuickStyle (WMS) Edit")) == false)
    return false;
  if (Layer->GetQuickStyleWms() != NULL)
    Style = Layer->CloneQuickStyleWms();
  else
    Style = new QuickStyleWmsObj();
// populates individual controls
  CreateControls();
  UpdateDialog();
// sets dialog sizer
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
// centers the dialog window
  Centre();
  return true;
}

void QuickStyleWmsDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// First row: Layer name
  wxBoxSizer *lyrBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(lyrBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *nameSizer = new wxBoxSizer(wxVERTICAL);
  lyrBoxSizer->Add(nameSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *nameBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("Layer FullName"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *nameBoxSizer = new wxStaticBoxSizer(nameBox, wxHORIZONTAL);
  nameSizer->Add(nameBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString fullName = DbPrefix + wxT(".") + LayerName;
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_WMS_LAYER, fullName,
                                        wxDefaultPosition, wxSize(370, 22),
                                        wxTE_READONLY);
  nameBoxSizer->Add(nameCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *uuidBox = new wxStaticBox(this, wxID_ANY,
                                         wxT("QuickStyle Name"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *uuidBoxSizer = new wxStaticBoxSizer(uuidBox, wxHORIZONTAL);
  nameSizer->Add(uuidBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString uuid = wxString::FromUTF8(Style->GetUUID());
  wxTextCtrl *uuidCtrl = new wxTextCtrl(this, ID_VECTOR_UUID, uuid,
                                        wxDefaultPosition, wxSize(370, 22),
                                        wxTE_READONLY);
  uuidBoxSizer->Add(uuidCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row: Visibility Range
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *visibilityBox = new wxStaticBox(this, wxID_STATIC,
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
  wxRadioBox *rangeBox = new wxRadioBox(this, ID_SYMBOLIZER_MINMAX_SCALE,
                                        wxT("&Range Type"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 4,
                                        range, 2,
                                        wxRA_SPECIFY_COLS);
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  rangeBox->SetSelection(0);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticText *minScaleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Min Scale:"));
  scaleMinSizer->Add(minScaleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *minScaleCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_MIN_SCALE, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  minScaleCtrl->Enable(false);
  scaleMinSizer->Add(minScaleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *scaleMaxSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMaxSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *maxScaleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Max Scale:"));
  scaleMaxSizer->Add(maxScaleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *maxScaleCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_MAX_SCALE, wxT("+Infinite"),
                   wxDefaultPosition, wxSize(100, 22));
  maxScaleCtrl->Enable(false);
  scaleMaxSizer->Add(maxScaleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// adding buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handlers
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleWmsDialog::OnQuit);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & QuickStyleWmsDialog::OnOk);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & QuickStyleWmsDialog::OnCmdScaleChanged);
}

void QuickStyleWmsDialog::OnCmdScaleChanged(wxCommandEvent & WXUNUSED(event))
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
  UpdateDialog();
}

void QuickStyleWmsDialog::UpdateDialog()
{
//
// updating the Dialog
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

bool QuickStyleWmsDialog::ValidateDialog()
{
//
// validating the Dialog
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

void QuickStyleWmsDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// permanently saving the QuickStyle and quitting
//
  if (ValidateDialog() == false)
    return;
  Layer->UpdateQuickStyle(Style);
  wxDialog::EndModal(wxID_OK);
}

void QuickStyleWmsDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}
