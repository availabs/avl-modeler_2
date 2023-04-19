/*
/ MapView.cpp
/ the Map View of spatialite_gui  - a SQLite /SpatiaLite GUI tool
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
#include "wx/image.h"
#include "wx/tooltip.h"
#include "wx/graphics.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#include <windows.h>
#include <process.h>
#include <io.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif
#ifdef __MINGW32__
#include <unistd.h>
#include <pthread.h>
#endif

#include <float.h>

#include "icons/crosshair.xpm"

MyMapView::MyMapView(MyMapPanel * parent, wxWindowID id):
wxPanel(parent, id, wxDefaultPosition, wxDefaultSize,
        wxBORDER_SUNKEN | wxWANTS_CHARS)
{
//
// constructor: a frame to show the Map
//
  MapPanel = parent;
  TimerMouseWheel = NULL;
  TimerMapPaint = NULL;
  TimerMapBlink = NULL;
  TimerMapTip = NULL;
  FirstLayer = NULL;
  LastLayer = NULL;
  ActiveLayer = NULL;
  MapSRID = -1;
  FrameMinX = DBL_MAX;
  FrameMaxY = -DBL_MAX;
  CurrentCanvas = NULL;
  SelectedFeatures = NULL;
  AttachedList = NULL;
  LoadedMapConfig = NULL;
  CurrentMapConfigXML = NULL;
  CurrentMapConfigName = NULL;
  DynamicBlink = false;
  MultiThreadingEnabled = MapPanel->IsMapMultiThreadingEnabled();
  MaxThreads = MapPanel->GetMapMaxThreads();
  AutoTransformEnabled = MapPanel->IsMapAutoTransformEnabled();
  RasterWmsAutoSwitch = false;
  LabelAntiCollision = false;
  LabelWrapText = false;
  LabelAutoRotate = false;
  LabelShiftPosition = false;
  GeographicCoordsDMS = false;
  CheckeredMapBackground = true;
  SolidMapBackground = wxColour(255, 255, 255);
  Invalidate();

  BitmapWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
  BitmapHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
  MapBitmap = wxBitmap(BitmapWidth, BitmapHeight, -1);
  ScreenBitmap = wxBitmap(BitmapWidth, BitmapHeight, -1);

  wxImage crossImage = wxBitmap(crosshair_xpm).ConvertToImage();
// setting up a  cursor
  crossImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 8);
  crossImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 8);
  CursorCross = wxCursor(crossImage);
  CursorHand = wxCursor(wxCURSOR_HAND);
  SetCursor(CursorCross);

// setting up event handlers
  Connect(wxID_ANY, wxEVT_ERASE_BACKGROUND,
          (wxObjectEventFunction) & MyMapView::OnEraseBackground);
  Connect(wxID_ANY, wxEVT_SIZE, (wxObjectEventFunction) & MyMapView::OnSize);
  Connect(wxID_ANY, wxEVT_PAINT, (wxObjectEventFunction) & MyMapView::OnPaint);
  Connect(ID_PAINT_MAP_STEP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyMapView::OnMapImagePaintStep);
  Connect(ID_PAINT_MAP_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MyMapView::OnThreadFinished);
  Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(MyMapView::OnMouseMove),
          NULL, this);
  Connect(wxID_ANY, wxEVT_LEFT_UP,
          wxMouseEventHandler(MyMapView::OnMouseDragStop), NULL, this);
  Connect(wxID_ANY, wxEVT_LEAVE_WINDOW,
          wxMouseEventHandler(MyMapView::OnMouseDragStop), NULL, this);
  Connect(wxID_ANY, wxEVT_LEFT_DOWN,
          wxMouseEventHandler(MyMapView::OnMouseClick), NULL, this);
  Connect(wxID_ANY, wxEVT_MOUSEWHEEL,
          wxMouseEventHandler(MyMapView::OnMouseWheel), NULL, this);
  Connect(ID_WHEEL_TIMER, wxEVT_TIMER,
          wxTimerEventHandler(MyMapView::OnTimerMouseWheel), NULL, this);
  Connect(ID_MAP_PAINT_TIMER, wxEVT_TIMER,
          wxTimerEventHandler(MyMapView::OnTimerMapPaint), NULL, this);
  Connect(ID_MAP_BLINK_TIMER, wxEVT_TIMER,
          wxTimerEventHandler(MyMapView::OnTimerMapBlink), NULL, this);
  Connect(ID_MAP_TIP_TIMER, wxEVT_TIMER,
          wxTimerEventHandler(MyMapView::OnTimerMapTip), NULL, this);
  Connect(KEY_Center, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyCenter);
  Connect(KEY_Up, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyUp);
  Connect(KEY_Down, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyDown);
  Connect(KEY_Left, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyLeft);
  Connect(KEY_Right, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyRight);
  Connect(KEY_ZoomIn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyZoomIn);
  Connect(KEY_ZoomOut, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyZoomOut);
  Connect(KEY_MicroUp, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyMicroUp);
  Connect(KEY_MicroDown, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyMicroDown);
  Connect(KEY_MicroLeft, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyMicroLeft);
  Connect(KEY_MicroRight, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyMicroRight);
  Connect(KEY_MicroZoomIn, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyMicroZoomIn);
  Connect(KEY_MicroZoomOut, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) & MyMapView::OnKeyMicroZoomOut);

// setting up an ACCELERATOR TABLE for Keyboard shortcuts
  wxAcceleratorEntry accelEntries[13];
  accelEntries[0].Set(wxACCEL_NORMAL, WXK_HOME, KEY_Center);
  accelEntries[1].Set(wxACCEL_NORMAL, WXK_UP, KEY_MicroUp);
  accelEntries[2].Set(wxACCEL_SHIFT, WXK_UP, KEY_Up);
  accelEntries[3].Set(wxACCEL_NORMAL, WXK_DOWN, KEY_MicroDown);
  accelEntries[4].Set(wxACCEL_SHIFT, WXK_DOWN, KEY_Down);
  accelEntries[5].Set(wxACCEL_NORMAL, WXK_LEFT, KEY_MicroLeft);
  accelEntries[6].Set(wxACCEL_SHIFT, WXK_LEFT, KEY_Left);
  accelEntries[7].Set(wxACCEL_NORMAL, WXK_RIGHT, KEY_MicroRight);
  accelEntries[8].Set(wxACCEL_SHIFT, WXK_RIGHT, KEY_Right);
  accelEntries[9].Set(wxACCEL_NORMAL, WXK_PAGEUP, KEY_MicroZoomIn);
  accelEntries[10].Set(wxACCEL_SHIFT, WXK_PAGEUP, KEY_ZoomIn);
  accelEntries[11].Set(wxACCEL_NORMAL, WXK_PAGEDOWN, KEY_MicroZoomOut);
  accelEntries[12].Set(wxACCEL_SHIFT, WXK_PAGEDOWN, KEY_ZoomOut);
  wxAcceleratorTable accelTbl(13, accelEntries);
  SetAcceleratorTable(accelTbl);
}

MyMapView::~MyMapView()
{
// dtor
  if (LoadedMapConfig != NULL)
    rl2_destroy_map_config(LoadedMapConfig);
  if (CurrentMapConfigXML != NULL)
    sqlite3_free(CurrentMapConfigXML);
  if (CurrentMapConfigName != NULL)
    free(CurrentMapConfigName);
  DetachAll();
  if (TimerMouseWheel)
    {
      TimerMouseWheel->Stop();
      delete TimerMouseWheel;
    }
  if (TimerMapPaint)
    {
      TimerMapPaint->Stop();
      delete TimerMapPaint;
    }
  if (TimerMapBlink)
    {
      TimerMapBlink->Stop();
      delete TimerMapBlink;
    }
  if (TimerMapTip)
    {
      TimerMapTip->Stop();
      delete TimerMapTip;
    }
  if (SelectedFeatures != NULL)
    delete SelectedFeatures;
  MapLayer *pLn;
  MapLayer *pL = FirstLayer;
  while (pL != NULL)
    {
      pLn = pL->GetNext();
      delete pL;
      pL = pLn;
    }
}

void MyMapView::MapPanelClosing()
{
//
// the parent is closing
//
  if (MapConfigurationChanged() == true)
    SaveMapConfiguration();
}

void MyMapView::DetachAll()
{
//
// DETACHing all DATABASEs
//
  if (AttachedList != NULL)
    {
      AttachedList->DetachAll();
      delete AttachedList;
      AttachedList = NULL;
    }
}

MapConfigAttached *MyMapView::CreateAttachedList()
{
//
// creating a list of ATTACHED DATABESES
//
  if (AttachedList != NULL)
    {
      AttachedList->DetachAll();
      delete AttachedList;
    }
  AttachedList = new MapConfigAttached(MapPanel->GetParent());
  return AttachedList;
}

void MyMapView::OnSize(wxSizeEvent & WXUNUSED(event))
{
//
// this window has changed its size
//
  PrepareMap();
}

int MyMapView::GetImageWidth()
{
// will return the current image Width
  wxSize sz = GetClientSize();
  return sz.GetWidth();
}

int MyMapView::GetImageHeight()
{
// will return the current image Width
  wxSize sz = GetClientSize();
  return sz.GetHeight();
}

void MyMapView::OnPaint(wxPaintEvent & WXUNUSED(event))
{
//
// this window needs to be repainted
//
  wxPaintDC dc(this);
  if (ValidMap == false)
    {
      // no map: black background
      wxSize sz = GetClientSize();
      dc.SetBrush(wxBrush(wxColour(128, 110, 96), wxSOLID));
      dc.SetPen(wxPen(wxColour(0, 0, 0), 1));
      dc.DrawRectangle(0, 0, sz.GetWidth(), sz.GetHeight());
      dc.SetBrush(wxNullBrush);
      dc.SetPen(wxNullPen);
  } else if (dc.IsOk() && ScreenBitmap.IsOk())
    {
      wxMemoryDC *memDC = new wxMemoryDC(ScreenBitmap);
      dc.Blit(0, 0, BitmapWidth, BitmapHeight, memDC, 0, 0);
      delete memDC;
    }
  //MainFrame->UpdateMapCRS();
}

void MyMapView::OnKeyCenter(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a CENTER MAP command
//
  SetMapToFullExtent();
  PrepareMap();
}

void MyMapView::OnKeyUp(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MOVE UP command
//
  double shift = ((FrameHeight * PixelRatio) / 4.0) * 3.0;
  FrameCY += shift;
  PrepareMap();
}

void MyMapView::OnKeyMicroUp(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MICRO MOVE UP command
//
  double shift = (FrameHeight * PixelRatio) / 4.0;
  FrameCY += shift;
  PrepareMap();
}

void MyMapView::OnKeyDown(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MOVE DOWN command
//
  double shift = ((FrameHeight * PixelRatio) / 4.0) * 3.0;
  FrameCY -= shift;
  PrepareMap();
}

void MyMapView::OnKeyMicroDown(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MICRO MOVE DOWN command
//
  double shift = (FrameHeight * PixelRatio) / 4.0;
  FrameCY -= shift;
  PrepareMap();
}

void MyMapView::OnKeyLeft(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MOVE LEFT command
//
  double shift = ((FrameWidth * PixelRatio) / 4.0) * 3.0;
  FrameCX -= shift;
  PrepareMap();
}

void MyMapView::OnKeyMicroLeft(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MICRO MOVE LEFT command
//
  double shift = (FrameWidth * PixelRatio) / 4.0;
  FrameCX -= shift;
  PrepareMap();
}

void MyMapView::OnKeyRight(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MOVE RIGHT command
//
  double shift = ((FrameWidth * PixelRatio) / 4.0) * 3.0;
  FrameCX += shift;
  PrepareMap();
}

void MyMapView::OnKeyMicroRight(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MICRO MOVE RIGHT command
//
  double shift = (FrameWidth * PixelRatio) / 4.0;
  FrameCX += shift;
  PrepareMap();
}

void MyMapView::OnKeyZoomIn(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a ZOOM IN command
//
  PixelRatio *= 0.5;
  PrepareMap();
}

void MyMapView::OnKeyMicroZoomIn(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MICRO ZOOM IN command
//
  PixelRatio *= 0.75;
  PrepareMap();
}

void MyMapView::OnKeyZoomOut(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a ZOOM OUT command
//
  PixelRatio /= 0.5;
  PrepareMap();
}

void MyMapView::OnKeyMicroZoomOut(wxCommandEvent & WXUNUSED(event))
{
//
// excuting a MICRO ZOOM OUT command
//
  PixelRatio /= 0.75;
  PrepareMap();
}

void MyMapView::OnTimerMapPaint(wxTimerEvent & WXUNUSED(event))
{
//
// Map Paint in progress - Timer event handler
// 
  MapPaintPhase++;
  if ((MapPaintPhase % 2) == 0)
    MapPanel->SetRedLight();
  else
    MapPanel->SetYellowLight();
  TimerMapPaint->Start(250, wxTIMER_ONE_SHOT);
}

void MyMapView::OnTimerMapBlink(wxTimerEvent & WXUNUSED(event))
{
//
// Map Blinking in progress - Timer event handler
// 
  if (MapBlinkCount >= 60 || SelectedFeatures == NULL)
    {
      // expired
      TimerMapBlink->Stop();
      delete TimerMapBlink;
      TimerMapBlink = NULL;
      if (SelectedFeatures != NULL)
        delete SelectedFeatures;
      SelectedFeatures = NULL;
      ResetScreenBitmap();
      return;
    }
  if (SelectedFeatures->GetSRID() != GetMapSRID())
    {
      // the Map Srid has changed - stop
      TimerMapBlink->Stop();
      delete TimerMapBlink;
      TimerMapBlink = NULL;
      delete SelectedFeatures;
      SelectedFeatures = NULL;
      ResetScreenBitmap();
      return;
    }
  MapBlinkCount++;
  DoPaintMapBlink(MapBlinkCount % 2);
  TimerMapBlink->Start(500, wxTIMER_ONE_SHOT);
}

void MyMapView::OnTimerMapTip(wxTimerEvent & WXUNUSED(event))
{
//
// Map Tool Tip - time expired, deleting the Tool Tip
// 
  TimerMapTip->Stop();
  delete TimerMapTip;
  TimerMapTip = NULL;
  SetToolTip((wxToolTip *) NULL);
}

void MyMapView::OnMouseWheel(wxMouseEvent & event)
{
//
// MOUSE wheel rotation - event handler
//
  if (wxIsBusy())
    return;
  int tics = event.GetWheelRotation() / event.GetWheelDelta();
  if (!TimerMouseWheel)
    TimerMouseWheel = new wxTimer(this, ID_WHEEL_TIMER);
  WheelTics += tics;
  TimerMouseWheel->Start(100, wxTIMER_ONE_SHOT);
}

void MyMapView::OnTimerMouseWheel(wxTimerEvent & WXUNUSED(event))
{
//
// MOUSE wheel rotation - Timer event handler
// 
  if (wxIsBusy())
    return;
  if (WheelTics > 0)
    {
      // zoom out
      double factor = 1.0 - ((double) WheelTics * 0.25);
      PixelRatio *= factor;
  } else
    {
      // zoom in
      double factor = 1.0 - ((double) (WheelTics * -1) * 0.25);
      PixelRatio /= factor;
    }
  WheelTics = 0;
  PrepareMap();
}

void MyMapView::OnMouseMove(wxMouseEvent & event)
{
//
// MOUSE motion - event handler
// 
  double x;
  double y;
  char *dummy;
  wxString coords;
  bool isIdentifyRaster = false;
  bool busy = wxIsBusy();
  // if (busy == true && PrintInProgress == true)
  if (busy == true)
    return;
  SetFocus();
  if (event.LeftIsDown() == true && MapPanel->IsModeIdentify() == true
      && ActiveLayer != NULL && !busy)
    {
      if (ActiveLayer->IsQueryable() == true
          && ActiveLayer->GetType() == MAP_LAYER_RASTER)
        isIdentifyRaster = true;
    }
  if (isIdentifyRaster == true)
    {
      // dynamic Raster Identify
      x = FrameMinX + (event.GetX() * PixelRatio);
      y = FrameMaxY - (event.GetY() * PixelRatio);
      DoIdentifyRaster(x, y);
      return;
    }
  if (event.LeftIsDown() == true && MapPanel->IsModePan() == true && !busy)
    {
      SetCursor(CursorHand);
      if (abs(LastDragX - event.GetX()) > 25
          || abs(LastDragY - event.GetY()) > 25)
        DragMap(event.GetX(), event.GetY());
      return;
    }
  if (event.LeftIsDown() == true && MapPanel->IsModeZoomIn() == true && !busy)
    {
      // marking the Zoom-in rectangle
      ResetScreenBitmap();
      wxMemoryDC *memDC = new wxMemoryDC(ScreenBitmap);
      memDC->SetPen(wxPen(wxColour(255, 0, 0), 2));
      memDC->SetBrush(wxBrush(wxColour(0, 0, 0), wxTRANSPARENT));
      int xx;
      int yy;
      int width;
      int height;
      if (event.GetX() > DragStartX)
        {
          xx = DragStartX;
          width = event.GetX() - DragStartX;
      } else
        {
          xx = event.GetX();
          width = DragStartX - event.GetX();
        }
      if (event.GetY() > DragStartY)
        {
          yy = DragStartY;
          height = event.GetY() - DragStartY;
      } else
        {
          yy = event.GetY();
          height = DragStartY - event.GetY();
        }
      memDC->DrawRectangle(xx, yy, width + 1, height + 1);
      delete memDC;
      Refresh();
    }
  SetCursor(CursorCross);

  if (FrameMinX == DBL_MAX || FrameMaxY == -DBL_MAX || PixelRatio == DBL_MAX)
    dummy = sqlite3_mprintf("#err#     #err#");
  else
    {
      x = FrameMinX + ((double) (event.GetX()) * PixelRatio);
      y = FrameMaxY - ((double) (event.GetY()) * PixelRatio);
      if (MapPanel->IsGeographicSRID(GetMapSRID()) == true)
        {
          if (GeographicCoordsDMS == true)
            {
              char *dms = gaiaConvertToDMS(x, y);
              dummy = sqlite3_mprintf("%s", dms);
              free(dms);
          } else
            dummy = sqlite3_mprintf("%1.6f     %1.6f", x, y);
      } else
        dummy = sqlite3_mprintf("%1.3f     %1.3f", x, y);
    }
  coords = wxString::FromUTF8(dummy);
  sqlite3_free(dummy);

  MapPanel->UpdateMapScale();
  MapPanel->UpdateMapCoords(coords);
  MapPanel->UpdateMapSRID();
}

void MyMapView::DoPrepareBBox(wxString & bbox)
{
// preparing the current frame BBOX
  char *buf =
    sqlite3_mprintf("BuildMBR(%f, %f, %f, %f, %d), ", FrameMinX, FrameMinY,
                    FrameMaxX, FrameMaxY, MapSRID);
  bbox = wxString::FromUTF8(buf);
  sqlite3_free(buf);
}

void MyMapView::GetBBox(int *srid, double *minx, double *miny, double *maxx,
                        double *maxy)
{
// returning the current fram BBOX
  *srid = MapSRID;
  *minx = FrameMinX;
  *miny = FrameMinY;
  *maxx = FrameMaxX;
  *maxy = FrameMaxY;
}

void MyMapView::Invalidate()
{
// resetting to initial state
  ValidMap = false;
  MapMinX = DBL_MAX;
  MapMinY = DBL_MAX;
  MapMaxX = -DBL_MAX;
  MapMaxY = -DBL_MAX;
  GeoMinX = DBL_MAX;
  GeoMinY = DBL_MAX;
  GeoMaxX = -DBL_MAX;
  GeoMaxY = -DBL_MAX;
  ActiveLayer = NULL;
  DragStartX = -1;
  DragStartY = -1;
  LastDragX = -1;
  LastDragY = -1;
  WheelTics = 0;
  if (TimerMouseWheel)
    {
      TimerMouseWheel->Stop();
      delete TimerMouseWheel;
    }
  TimerMouseWheel = NULL;
  /*
     if (UpdateTimer)
     {
     UpdateTimer->Stop();
     delete UpdateTimer;
     }
     UpdateTimer = NULL;
     if (TimerDynamic)
     {
     TimerDynamic->Stop();
     delete TimerDynamic;
     }
     TimerDynamic = NULL;
   */
}

void MyMapView::OnMouseDragStop(wxMouseEvent & event)
{
//
// MOUSE left button up - event handler
// 
  if (wxIsBusy())
    return;
  if (DragStartX < 0 || DragStartY < 0)
    return;
  LastDragX = event.GetX();
  LastDragY = event.GetY();
  if (MapPanel->IsModePan() == true)
    {
      // computing the new Map position
      if (DragStartX == LastDragX && DragStartY == LastDragY)
        goto none;
      double shiftX = (double) (LastDragX - DragStartX) * PixelRatio;
      double shiftY = (double) (LastDragY - DragStartY) * PixelRatio;
      FrameCX -= shiftX;
      FrameCY += shiftY;
      PrepareMap();
    }
  if (MapPanel->IsModeZoomIn() == true)
    {
      // computing the new Map position 
      if (DragStartX < 0 || DragStartY < 0)
        goto none;
      if (DragStartX == LastDragX && DragStartY == LastDragY)
        {
          FrameCX = (FrameMinX + (event.GetX() * PixelRatio));
          FrameCY = (FrameMaxY - (event.GetY() * PixelRatio));
          PixelRatio /= 2.0;
          PrepareMap();
          DragStartX = -1;
          DragStartY = -1;
          LastDragX = -1;
          LastDragY = -1;
          return;
        }
      // computing the new Zoom
      wxSize sz = GetClientSize();
      FrameWidth = sz.GetWidth();
      FrameHeight = sz.GetHeight();
      // computing the current PixelRatio
      double x0 = FrameMinX + ((double) DragStartX * PixelRatio);
      double y0 = FrameMaxY - ((double) DragStartY * PixelRatio);
      double x1 = FrameMinX + ((double) LastDragX * PixelRatio);
      double y1 = FrameMaxY - ((double) LastDragY * PixelRatio);
      double extentX;
      double extentY;
      if (x0 > x1)
        {
          extentX = x0 - x1;
          FrameCX = x1 + (extentX / 2.0);
      } else
        {
          extentX = x1 - x0;
          FrameCX = x0 + (extentX / 2.0);
        }
      if (y0 > y1)
        {
          extentY = y0 - y1;
          FrameCY = y1 + (extentY / 2.0);
      } else
        {
          extentY = y1 - y0;
          FrameCY = y0 + (extentY / 2.0);
        }
      double x_ratio = extentX / (double) (FrameWidth);
      double y_ratio = extentY / (double) (FrameHeight);
      if (extentX / x_ratio <= FrameWidth && extentY / x_ratio <= FrameHeight)
        PixelRatio = x_ratio;
      else if (extentX / y_ratio <= FrameWidth
               && extentY / y_ratio <= FrameHeight)
        PixelRatio = y_ratio;
      else
        {
          if (x_ratio > y_ratio)
            PixelRatio = x_ratio;
          else
            PixelRatio = y_ratio;
        }
      PrepareMap();
    }
none:
  SetCursor(CursorCross);
  DragStartX = -1;
  DragStartY = -1;
  LastDragX = -1;
  LastDragY = -1;
}

void MyMapView::DragMap(int end_x, int end_y)
{
//
// Map dragging
//
  if (wxIsBusy())
    return;
  int width = FrameWidth;
  int height = FrameHeight;
  int baseX = 0;
  int baseY = 0;
  baseX = end_x - DragStartX;
  baseY = end_y - DragStartY;

  wxClientDC dc(this);
  dc.SetBrush(wxBrush(wxColour(192, 192, 192)));
  dc.DrawRectangle(-1, -1, width + 1, height + 1);
// copying the ScreenImage - dragged
  wxMemoryDC bmpDc(ScreenBitmap);
  int eff_width = width;
  int eff_height = height;
  int sx = 0;
  int sy = 0;
  int ix = 0;
  int iy = 0;
  if (baseX < 0)
    {
      ix = 0;
      sx = 0 - baseX;
      eff_width = width + baseX;
  } else
    {
      ix = baseX;
      eff_width = width - baseX;
    }
  if (baseY < 0)
    {
      iy = 0;
      sy = 0 - baseY;
      eff_height = height + baseY;
  } else
    {
      iy = baseY;
      eff_height = height - baseY;
    }
  dc.Blit(ix, iy, eff_width, eff_height, &bmpDc, sx, sy);
  LastDragX = end_x;
  LastDragY = end_y;
}

void MyMapView::OnMouseClick(wxMouseEvent & event)
{
//
// MOUSE Click - event handler
// 
  SetFocus();
  if (wxIsBusy())
    {
      /*
         if (PrintInProgress == true)
         return;
         PendingWmsAbort = true;
         PendingRefresh = true;
       */
    }

  if (MapPanel->IsModeIdentify() == true)
    {
      // identify
      if (ActiveLayer != NULL)
        {
          if (ActiveLayer->IsQueryable() == true)
            DoIdentify(event.GetX(), event.GetY());
        }
      return;
    }
  if (MapPanel->IsModeZoomIn() == true)
    {
      // zoom in - Rect
      DragStartX = event.GetX();
      DragStartY = event.GetY();
      LastDragX = event.GetX();
      LastDragY = event.GetY();
      return;
    }
  if (MapPanel->IsModeZoomOut() == true)
    {
      // zoom out
      FrameCX = (FrameMinX + (event.GetX() * PixelRatio));
      FrameCY = (FrameMaxY - (event.GetY() * PixelRatio));
      PixelRatio *= 2.0;
      PrepareMap();
      return;
    }

  if (MapPanel->IsModePan() == true)
    {
      DragStartX = event.GetX();
      DragStartY = event.GetY();
      LastDragX = event.GetX();
      LastDragY = event.GetY();
    }
}

void MyMapView::UpdateMapFullExtent()
{
//
// updating the Map Full extent
//
  ValidMap = false;
  MapMinX = DBL_MAX;
  MapMinY = DBL_MAX;
  MapMaxX = -DBL_MAX;
  MapMaxY = -DBL_MAX;
  GeoMinX = DBL_MAX;
  GeoMinY = DBL_MAX;
  GeoMaxX = -DBL_MAX;
  GeoMaxY = -DBL_MAX;
  MapLayer *lyr = FirstLayer;

  if (lyr == NULL)
    return;
  else
    {
      lyr->UpdateMapExtent(MapPanel->GetSqlite());
      if (lyr->IsValidMapExtent())
        {
          ValidMap = true;
          if (lyr->IsSwapXY() == true)
            {
              MapMinY = lyr->GetMapMinX();
              MapMaxY = lyr->GetMapMaxX();
              MapMinX = lyr->GetMapMinY();
              MapMaxX = lyr->GetMapMaxY();
          } else
            {
              MapMinX = lyr->GetMapMinX();
              MapMaxX = lyr->GetMapMaxX();
              MapMinY = lyr->GetMapMinY();
              MapMaxY = lyr->GetMapMaxY();
            }
        }
    }
  while (lyr != NULL)
    {
      lyr->UpdateMapExtent(MapPanel->GetSqlite());
      if (lyr->IsValidMapExtent())
        {
          ValidMap = true;
          if (lyr->IsSwapXY() == true)
            {
              if (lyr->GetMapMinX() < MapMinY)
                MapMinY = lyr->GetMapMinX();
              if (lyr->GetMapMaxX() > MapMaxY)
                MapMaxY = lyr->GetMapMaxX();
              if (lyr->GetMapMinY() < MapMinX)
                MapMinX = lyr->GetMapMinY();
              if (lyr->GetMapMaxY() > MapMaxX)
                MapMaxX = lyr->GetMapMaxY();
          } else
            {
              if (lyr->GetMapMinX() < MapMinX)
                MapMinX = lyr->GetMapMinX();
              if (lyr->GetMapMaxX() > MapMaxX)
                MapMaxX = lyr->GetMapMaxX();
              if (lyr->GetMapMinY() < MapMinY)
                MapMinY = lyr->GetMapMinY();
              if (lyr->GetMapMaxY() > MapMaxY)
                MapMaxY = lyr->GetMapMaxY();
            }
        }
      lyr = lyr->GetNext();
    }

  if (ValidMap == true)
    {
      // computing the geographic BBOX 
      sqlite3_stmt *stmt = NULL;
      int ret;
      const char *sql =
        "SELECT MbrMinX(x.bbox), MbrMinY(x.bbox), MbrMaxX(x.bbox), MbrMaxY(x.bbox) "
        "FROM (SELECT ST_Transform(BuildMbr(?, ?, ?, ?, ?), 4326) AS bbox) AS x";
      ret =
        sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt,
                           NULL);
      if (ret != SQLITE_OK)
        return;
      sqlite3_reset(stmt);
      sqlite3_clear_bindings(stmt);
      sqlite3_bind_double(stmt, 1, MapMinX);
      sqlite3_bind_double(stmt, 2, MapMinY);
      sqlite3_bind_double(stmt, 3, MapMaxX);
      sqlite3_bind_double(stmt, 4, MapMaxY);
      sqlite3_bind_int(stmt, 5, MapSRID);
      while (1)
        {
          // scrolling the result set rows
          ret = sqlite3_step(stmt);
          if (ret == SQLITE_DONE)
            break;              // end of result set
          if (ret == SQLITE_ROW)
            {
              GeoMinX = sqlite3_column_double(stmt, 0);
              GeoMinY = sqlite3_column_double(stmt, 1);
              GeoMaxX = sqlite3_column_double(stmt, 2);
              GeoMaxY = sqlite3_column_double(stmt, 3);
            }
        }
      sqlite3_finalize(stmt);
    }
}

void MyMapView::SetMapToFullExtent()
{
//
// setting the Frame to full extent
//
  MapExtentX = MapMaxX - MapMinX;
  MapExtentY = MapMaxY - MapMinY;
  MapCX = MapMinX + (MapExtentX / 2.0);
  MapCY = MapMinY + (MapExtentY / 2.0);
// computig the Frame Extent
  FrameCX = MapCX;
  FrameCY = MapCY;
  wxSize sz = GetClientSize();
  FrameWidth = sz.GetWidth();
  FrameHeight = sz.GetHeight();
// computing the current PixelRatio
  double x_ratio = MapExtentX / (double) (FrameWidth);
  double y_ratio = MapExtentY / (double) (FrameHeight);
  if (MapExtentX / x_ratio <= FrameWidth && MapExtentY / x_ratio <= FrameHeight)
    PixelRatio = x_ratio;
  else if (MapExtentX / y_ratio <= FrameWidth
           && MapExtentY / y_ratio <= FrameHeight)
    PixelRatio = y_ratio;
  else
    {
      if (x_ratio > y_ratio)
        PixelRatio = x_ratio;
      else
        PixelRatio = y_ratio;
    }
}

void MyMapView::SetMapLayerToFullExtent(MapLayer * lyr)
{
//
// setting the Frame to Layer's full extent
//
  if (lyr == NULL)
    return;
  MapExtentX = lyr->GetMapMaxX() - lyr->GetMapMinX();
  MapExtentY = lyr->GetMapMaxY() - lyr->GetMapMinY();
  MapCX = lyr->GetMapMinX() + (MapExtentX / 2.0);
  MapCY = lyr->GetMapMinY() + (MapExtentY / 2.0);
// computig the Frame Extent
  FrameCX = MapCX;
  FrameCY = MapCY;
  wxSize sz = GetClientSize();
  FrameWidth = sz.GetWidth();
  FrameHeight = sz.GetHeight();
// computing the current PixelRatio
  double x_ratio = MapExtentX / (double) (FrameWidth);
  double y_ratio = MapExtentY / (double) (FrameHeight);
  if (MapExtentX / x_ratio <= FrameWidth && MapExtentY / x_ratio <= FrameHeight)
    PixelRatio = x_ratio;
  else if (MapExtentX / y_ratio <= FrameWidth
           && MapExtentY / y_ratio <= FrameHeight)
    PixelRatio = y_ratio;
  else
    {
      if (x_ratio > y_ratio)
        PixelRatio = x_ratio;
      else
        PixelRatio = y_ratio;
    }
}

void MyMapView::UpdateMapViewPoint(int fromSrid, int toSrid)
{
//
// computing a new Map ViewPoint after changing the Map SRID
//
  sqlite3_stmt *stmt = NULL;
  int ret;

  if (!ValidMap)
    return;

  const char *sql =
    "SELECT MbrMinY(x.bbox), MbrMaxY(x.bbox) "
    "FROM (SELECT ST_Transform(BuildMbr(?, ?, ?, ?, ?), ?) AS bbox) AS x";
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_double(stmt, 1, FrameCX);
  sqlite3_bind_double(stmt, 2, FrameMinY);
  sqlite3_bind_double(stmt, 3, FrameCX);
  sqlite3_bind_double(stmt, 4, FrameMaxY);
  sqlite3_bind_int(stmt, 5, fromSrid);
  sqlite3_bind_int(stmt, 6, toSrid);
  while (1)
    {
      // scrolling the result set rows
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          FrameMinY = sqlite3_column_double(stmt, 0);
          FrameMaxY = sqlite3_column_double(stmt, 1);
        }
    }
  sqlite3_finalize(stmt);

  sql = "SELECT ST_X(x.pt), ST_Y(x.pt) "
    "FROM (SELECT ST_Transform(MakePoint(?, ?, ?), ?) AS pt) AS x";
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_double(stmt, 1, FrameCX);
  sqlite3_bind_double(stmt, 2, FrameCY);
  sqlite3_bind_int(stmt, 3, fromSrid);
  sqlite3_bind_int(stmt, 4, toSrid);
  while (1)
    {
      // scrolling the result set rows
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;                  // end of result set
      if (ret == SQLITE_ROW)
        {
          FrameCX = sqlite3_column_double(stmt, 0);
          FrameCY = sqlite3_column_double(stmt, 1);
        }
    }
  sqlite3_finalize(stmt);

  double extentY = FrameMaxY - FrameMinY;
  PixelRatio = extentY / (double) (FrameHeight);
}

void MyMapView::DoPaintCheckeredBackground(wxMemoryDC * dc)
{
//
// drawing a Checkered Bakcground
//
  wxImage img(24, 24);
  for (int y = 0; y < 24; y++)
    {
      // creating a checkered background
      for (int x = 0; x < 24; x++)
        {
          if (y < 12)
            {
              if (x < 12)
                img.SetRGB(x, y, 224, 224, 224);
              else
                img.SetRGB(x, y, 208, 208, 208);
          } else
            {
              if (x < 12)
                img.SetRGB(x, y, 208, 208, 208);
              else
                img.SetRGB(x, y, 224, 224, 224);
            }
        }
    }
  wxBrush stipple(img);
  dc->SetBrush(stipple);
  dc->DrawRectangle(-1, -1, FrameWidth + 2, FrameHeight + 2);
}

void MyMapView::DoIdentify(int x, int y)
{
// Identify
  if (ActiveLayer == NULL)
    return;

  double dx = FrameMinX + (x * PixelRatio);
  double dy = FrameMaxY - (y * PixelRatio);

  if (ActiveLayer->IsQueryable() == true
      && ActiveLayer->GetType() == MAP_LAYER_RASTER)
    DoIdentifyRaster(dx, dy);
  if (ActiveLayer->IsQueryable() == true
      && (ActiveLayer->GetType() == MAP_LAYER_VECTOR
          || ActiveLayer->GetType() == MAP_LAYER_VECTOR_VIEW
          || ActiveLayer->GetType() == MAP_LAYER_VECTOR_VIRTUAL))
    DoIdentifyVector(dx, dy);
  if (ActiveLayer->IsQueryable() == true
      && ActiveLayer->GetType() == MAP_LAYER_TOPOLOGY)
    DoIdentifyTopology(dx, dy);
  if (ActiveLayer->IsQueryable() == true
      && ActiveLayer->GetType() == MAP_LAYER_NETWORK)
    DoIdentifyNetwork(dx, dy);
}

void MyMapView::DoIdentifyRaster(double x, double y)
{
//
// Identify Pixel - Raster Coverage
//
  int ret;
  sqlite3_stmt *stmt = NULL;
  char *sql;
  char *info;
  char *prev;
  char *db_prefix;
  char *coverage;
  rl2PixelPtr pixel = NULL;
  int err = 0;
  unsigned char sample_type;
  unsigned char pixel_type;
  unsigned char num_bands;
  int b;
  const char *xsample;
  const char *xpixel;

  db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
  strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
  coverage = (char *) malloc(ActiveLayer->GetName().Len() * 4);
  strcpy(coverage, ActiveLayer->GetName().ToUTF8());
  sql = sqlite3_mprintf("SELECT RL2_GetPixelFromRasterByPoint(%Q, %Q, "
                        "MakePoint(?, ?, ?), ?, ?)", db_prefix, coverage);
  free(db_prefix);
  free(coverage);
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  sqlite3_free(sql);
  if (ret != SQLITE_OK)
    return;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_double(stmt, 1, x);
  sqlite3_bind_double(stmt, 2, y);
  sqlite3_bind_int(stmt, 3, GetMapSRID());
  sqlite3_bind_double(stmt, 4, PixelRatio);
  sqlite3_bind_double(stmt, 5, PixelRatio);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const unsigned char *blob =
                (const unsigned char *) sqlite3_column_blob(stmt, 0);
              int blob_sz = sqlite3_column_bytes(stmt, 0);
              if (pixel != NULL)
                rl2_destroy_pixel(pixel);
              pixel = rl2_deserialize_dbms_pixel(blob, blob_sz);
            }
      } else
        {
          err = 1;
          break;
        }
    }
  sqlite3_finalize(stmt);

  if (pixel == NULL || err)
    return;

  info = sqlite3_mprintf("Pixel at: %f    %f", x, y);
  prev = info;
  rl2_get_pixel_type(pixel, &sample_type, &pixel_type, &num_bands);
  switch (sample_type)
    {
      case RL2_SAMPLE_1_BIT:
        xsample = "1-BIT";
        break;
      case RL2_SAMPLE_2_BIT:
        xsample = "2-BIT";
        break;
      case RL2_SAMPLE_4_BIT:
        xsample = "4-BIT";
        break;
      case RL2_SAMPLE_INT8:
        xsample = "INT8";
        break;
      case RL2_SAMPLE_UINT8:
        xsample = "UINT8";
        break;
      case RL2_SAMPLE_INT16:
        xsample = "INT16";
        break;
      case RL2_SAMPLE_UINT16:
        xsample = "UINT16";
        break;
      case RL2_SAMPLE_INT32:
        xsample = "INT32";
        break;
      case RL2_SAMPLE_UINT32:
        xsample = "UINT32";
        break;
      case RL2_SAMPLE_FLOAT:
        xsample = "FLOAT";
        break;
      case RL2_SAMPLE_DOUBLE:
        xsample = "DOUBLE";
        break;
      default:
        xsample = "UNKNOWN";
        break;
    };
  switch (pixel_type)
    {
      case RL2_PIXEL_MONOCHROME:
        xpixel = "MONOCHRMONE";
        break;
      case RL2_PIXEL_PALETTE:
        xpixel = "PALETTE";
        break;
      case RL2_PIXEL_GRAYSCALE:
        xpixel = "GRAYSCALE";
        break;
      case RL2_PIXEL_RGB:
        xpixel = "RGB";
        break;
      case RL2_PIXEL_MULTIBAND:
        xpixel = "MULTIBAND";
        break;
      case RL2_PIXEL_DATAGRID:
        xpixel = "DATAGRID";
        break;
      default:
        xpixel = "UNKNOWN";
        break;
    };
  info = sqlite3_mprintf("%s\n\nSampleType: %s", prev, xsample);
  sqlite3_free(prev);
  prev = info;
  info = sqlite3_mprintf("%s\nPixelType: %s", prev, xpixel);
  sqlite3_free(prev);
  prev = info;
  info = sqlite3_mprintf("%s\nNumBands: %u\n", prev, num_bands);
  sqlite3_free(prev);
  prev = info;
  for (b = 0; b < num_bands; b++)
    {
      char int8;
      unsigned char uint8;
      short int16;
      unsigned short uint16;
      int int32;
      unsigned int uint32;
      float flt32;
      double flt64;
      switch (sample_type)
        {
          case RL2_SAMPLE_1_BIT:
            rl2_get_pixel_sample_1bit(pixel, &uint8);
            info = sqlite3_mprintf("%s\nBand #%d: %u", prev, b, uint8);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_2_BIT:
            rl2_get_pixel_sample_2bit(pixel, &uint8);
            info = sqlite3_mprintf("%s\nBand #%d: %u", prev, b, uint8);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_4_BIT:
            rl2_get_pixel_sample_4bit(pixel, &uint8);
            info = sqlite3_mprintf("%s\nBand #%d: %u", prev, b, uint8);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_INT8:
            rl2_get_pixel_sample_int8(pixel, &int8);
            info = sqlite3_mprintf("%s\nBand #%d: %d\n", prev, b, int8);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_UINT8:
            rl2_get_pixel_sample_uint8(pixel, b, &uint8);
            info = sqlite3_mprintf("%s\nBand #%d: %u", prev, b, uint8);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_INT16:
            rl2_get_pixel_sample_int16(pixel, &int16);
            info = sqlite3_mprintf("%s\nBand #%d: %d", prev, b, int16);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_UINT16:
            rl2_get_pixel_sample_uint16(pixel, b, &uint16);
            info = sqlite3_mprintf("%s\nBand #%d: %u", prev, b, uint16);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_INT32:
            rl2_get_pixel_sample_int32(pixel, &int32);
            info = sqlite3_mprintf("%s\nBand #%d: %d", b, int32);
            sqlite3_mprintf(prev);
            prev = info;
            break;
          case RL2_SAMPLE_UINT32:
            rl2_get_pixel_sample_uint32(pixel, &uint32);
            info = sqlite3_mprintf("%s\nBand #%d: %u", prev, b, uint32);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_FLOAT:
            rl2_get_pixel_sample_float(pixel, &flt32);
            info = sqlite3_mprintf("%s\nBand #%d: %f", prev, b, flt32);
            sqlite3_free(prev);
            prev = info;
            break;
          case RL2_SAMPLE_DOUBLE:
            rl2_get_pixel_sample_double(pixel, &flt64);
            info = sqlite3_mprintf("%s\nBand #%d: %f", prev, b, flt64);
            sqlite3_free(prev);
            prev = info;
            break;
        };
    }
  rl2_destroy_pixel(pixel);

  wxString msg = wxString::FromUTF8(info);
  sqlite3_free(info);
  wxToolTip *tip = new wxToolTip(msg);
  tip->SetDelay(1);
// installing a timer for automatically destroying the Tool Tip 
  if (TimerMapTip)
    {
      TimerMapTip->Stop();
      delete TimerMapTip;
    }
  TimerMapTip = new wxTimer(this, ID_MAP_TIP_TIMER);
  TimerMapTip->Start(15000, wxTIMER_ONE_SHOT);
  SetToolTip(tip);
}

void MyMapView::DoIdentifyVector(double x, double y)
{
//
// Identify Feature(s) - Vector Coverage
//
  char *sql;
  char *db_prefix;
  char *xdb_prefix;
  char *xtable;
  char *xgeometry;
  char *filter;
  double radius = 10.0 * PixelRatio;

  db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
  strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
  xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
  free(db_prefix);
  xtable = gaiaDoubleQuotedSql(ActiveLayer->GetTableName());
  xgeometry = gaiaDoubleQuotedSql(ActiveLayer->GetGeometryColumn());

  bool isView = false;
  if (ActiveLayer->GetType() == MAP_LAYER_VECTOR_VIEW)
    isView = true;

  if (ActiveLayer->HasSpatialIndex() == true)
    {
      // queryng the R*Tree Spatial Index
      char *xrowid = NULL;
      const char *table = ActiveLayer->GetTableName();
      const char *geom = ActiveLayer->GetGeometryColumn();
      if (isView == true)
        {
          char *rowid = gaiaDoubleQuotedSql(ActiveLayer->GetViewRowidColumn());
          xrowid = sqlite3_mprintf("\"%s\"", rowid);
          free(rowid);
          table = ActiveLayer->GetViewMotherName();
          geom = ActiveLayer->GetViewMotherGeometry();
        }
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      char *rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          filter =
            sqlite3_mprintf
            ("AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))",
             (xrowid == NULL) ? "ROWID" : xrowid, rtree_name, geom, x, y,
             radius + (radius / 100.0));
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          filter =
            sqlite3_mprintf
            ("AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             (xrowid == NULL) ? "ROWID" : xrowid, rtree_name, geom, x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
        }
      sqlite3_free(rtree_name);
      if (xrowid != NULL)
        sqlite3_free(xrowid);
  } else
    {
      // applying MBR filtering
      char *geom = gaiaDoubleQuotedSql(ActiveLayer->GetGeometryColumn());
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          filter =
            sqlite3_mprintf
            ("\n\tAND MbrIntersects(\"%s\", BuildCircleMBR(%f, %f, %f))", geom,
             x, y, radius + (radius / 100.0));
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          filter =
            sqlite3_mprintf
            ("\n\tAND MbrIntersects(\"%s\", ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             geom, GetMapSRID(), ActiveLayer->GetNativeSRID(), x, y,
             radius + (radius / 100.0));
        }
    }

  if (ActiveLayer->GetNativeSRID() == GetMapSRID())
    {
      // the Vector Layer and the Map share the same SRID
      sql = sqlite3_mprintf("SELECT * FROM \"%s\".\"%s\"\n"
                            "WHERE ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f %s",
                            xdb_prefix, xtable, xgeometry, x, y, radius,
                            filter);
  } else
    {
      // the Vector Layer and the Map have different SRIDs
      sql = sqlite3_mprintf("SELECT * FROM \"%s\".\"%s\"\n"
                            "WHERE ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f %s",
                            xdb_prefix, xtable, xgeometry, GetMapSRID(),
                            x, y, GetMapSRID(), radius, filter);
    }
  free(xdb_prefix);
  free(xtable);
  free(xgeometry);
  sqlite3_free(filter);

  wxString table = wxString::FromUTF8(ActiveLayer->GetTableName());
  wxString geom = wxString::FromUTF8(ActiveLayer->GetGeometryColumn());
  MapPanel->InitializeSqlFilters(ActiveLayer->GetDbPrefix(), table, true, geom);
  wxString sqlstr = wxString::FromUTF8(sql);
  sqlite3_free(sql);
  MapPanel->SetSql(sqlstr, true);
}

void MyMapView::DoIdentifyTopology(double x, double y)
{
//
// Identify Primitives - Topology Coverage
//
  char *sql;
  char *sql2;
  char *prev = NULL;
  char *db_prefix;
  char *xdb_prefix;
  char *table;
  char *xtable;
  char *xgeometry;
  char *rtree_name;
  double radius = 10.0 * PixelRatio;
  VectorLayerConfig *config = ActiveLayer->GetVectorConfig();

  if (config == NULL)
    return;

  if (config->AreNodesVisible() == true)
    {
      // Identifying Node Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_node", ActiveLayer->GetTopologyName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geom");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql =
            sqlite3_mprintf
            ("SELECT 'Node' AS primitive_class, node_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0));
          prev = sql;
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql =
            sqlite3_mprintf
            ("SELECT 'Node' AS primitive_class, node_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          prev = sql;
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }
  if (config->AreEdgesVisible() == true)
    {
      // Identifying Edge Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_edge", ActiveLayer->GetTopologyName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geom");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql2 =
            sqlite3_mprintf
            ("SELECT 'Edge' AS primitive_class, edge_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0));
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql2 =
            sqlite3_mprintf
            ("SELECT 'Edge' AS primitive_class, edge_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }
  if (config->AreFacesVisible() == true)
    {
      // Identifying Face Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_face", ActiveLayer->GetTopologyName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("mbr");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql2 =
            sqlite3_mprintf
            ("SELECT 'Face' AS primitive_class, face_id AS primitive_id, "
             "ST_GetFaceGeometry(%Q, face_id) AS geom\n" "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(geom, MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))",
             ActiveLayer->GetTopologyName(), xdb_prefix, xtable, x, y, radius,
             "ROWID", rtree_name, "mbr", x, y, radius + (radius / 100.0));
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql2 =
            sqlite3_mprintf
            ("SELECT 'Face' AS primitive_class, face_id AS primitive_id, "
             "ST_GetFaceGeometry(%Q, face_id) AS geom\n" "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(ST_Transform(geom, %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             ActiveLayer->GetTopologyName(), xdb_prefix, xtable, GetMapSRID(),
             x, y, GetMapSRID(), radius, "ROWID", rtree_name, "mbr", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }
  if (config->AreEdgeSeedsVisible() == true)
    {
      // Identifying EdgeSeeds Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_seeds", ActiveLayer->GetTopologyName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geom");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql2 =
            sqlite3_mprintf
            ("SELECT 'EdgeSeed' AS primitive_class, seed_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE edge_id IS NOT NULL AND ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0));
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql2 =
            sqlite3_mprintf
            ("SELECT 'EdgeSeed' AS primitive_class, seed_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE edge_id IS NOT NULL AND ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }
  if (config->AreFaceSeedsVisible() == true)
    {
      // Identifying FaceSeeds Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_seeds", ActiveLayer->GetTopologyName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geom");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql2 =
            sqlite3_mprintf
            ("SELECT 'FaceSeed' AS primitive_class, seed_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE face_id IS NOT NULL AND ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0));
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql2 =
            sqlite3_mprintf
            ("SELECT 'FaceSeed' AS primitive_class, seed_id AS primitive_id, geom AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE face_id IS NOT NULL AND ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geom", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }

  sql = prev;
  wxString tbl = wxString::FromUTF8(ActiveLayer->GetTableName());
  wxString geom = wxT("geom");
  MapPanel->InitializeSqlFilters(ActiveLayer->GetDbPrefix(), tbl, true, geom);
  wxString sqlstr = wxString::FromUTF8(sql);
  sqlite3_free(sql);
  MapPanel->SetSql(sqlstr, true);
}

void MyMapView::DoIdentifyNetwork(double x, double y)
{
//
// Identify Primitives - Network Coverage
//
  char *sql;
  char *sql2;
  char *prev = NULL;
  char *db_prefix;
  char *xdb_prefix;
  char *table;
  char *xtable;
  char *xgeometry;
  char *rtree_name;
  double radius = 10.0 * PixelRatio;
  VectorLayerConfig *config = ActiveLayer->GetVectorConfig();

  if (config == NULL)
    return;

  if (config->AreNodesVisible() == true)
    {
      // Identifying Node Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_node", ActiveLayer->GetNetworkName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geometry");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql =
            sqlite3_mprintf
            ("SELECT 'Node' AS primitive_class, node_id AS primitive_id, geometry AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geometry",
             x, y, radius + (radius / 100.0));
          prev = sql;
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql =
            sqlite3_mprintf
            ("SELECT 'Node' AS primitive_class, node_id AS primitive_id, geometry AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geometry", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          prev = sql;
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }
  if (config->AreLinksVisible() == true)
    {
      // Identifying Link Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_link", ActiveLayer->GetNetworkName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geometry");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql2 =
            sqlite3_mprintf
            ("SELECT 'Link' AS primitive_class, link_id AS primitive_id, geometry AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geometry",
             x, y, radius + (radius / 100.0));
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql2 =
            sqlite3_mprintf
            ("SELECT 'Link' AS primitive_class, link_id AS primitive_id, geometry AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geometry", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }
  if (config->AreLinkSeedsVisible() == true)
    {
      // Identifying FaceSeeds Primitives
      db_prefix = (char *) malloc(ActiveLayer->GetDbPrefix().Len() * 4);
      strcpy(db_prefix, ActiveLayer->GetDbPrefix().ToUTF8());
      xdb_prefix = gaiaDoubleQuotedSql(db_prefix);
      table = sqlite3_mprintf("%s_seeds", ActiveLayer->GetNetworkName());
      xtable = gaiaDoubleQuotedSql(table);
      xgeometry = gaiaDoubleQuotedSql("geometry");
      rtree_name = sqlite3_mprintf("DB=%s.%s", db_prefix, table);
      free(db_prefix);
      sqlite3_free(table);
      if (ActiveLayer->GetNativeSRID() == GetMapSRID())
        {
          // the Vector Layer and the Map share the same SRID
          sql2 =
            sqlite3_mprintf
            ("SELECT 'LinkSeed' AS primitive_class, seed_id AS primitive_id, geometry AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE ST_Distance(\"%s\", MakePoint(%f, %f)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_table_name = %Q AND f_geometry_column = %Q\n"
             "\tAND search_frame = BuildCircleMBR(%f, %f, %f))", xdb_prefix,
             xtable, xgeometry, x, y, radius, "ROWID", rtree_name, "geometry",
             x, y, radius + (radius / 100.0));
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
      } else
        {
          // the Vector Layer and the Map have different SRIDs
          sql2 =
            sqlite3_mprintf
            ("SELECT 'LinkSeed' AS primitive_class, seed_id AS primitive_id, geometru AS geom\n"
             "FROM \"%s\".\"%s\"\n"
             "WHERE face_id IS NOT NULL AND ST_Distance(ST_Transform(\"%s\", %d), MakePoint(%f, %f, %d)) <= %f "
             "AND %s IN (\n\tSELECT ROWID FROM SpatialIndex\n\t"
             "WHERE f_geometry_column = %Q\n"
             "\tAND search_frame = ST_Transform(BuildCircleMBR(%f, %f, %f, %d), %d))",
             xdb_prefix, xtable, xgeometry, GetMapSRID(), x, y, GetMapSRID(),
             radius, "ROWID", rtree_name, "geometry", x, y,
             radius + (radius / 100.0), GetMapSRID(),
             ActiveLayer->GetNativeSRID());
          if (prev == NULL)
            prev = sql2;
          else
            {
              sql = sqlite3_mprintf("%s\nUNION\n%s", prev, sql2);
              sqlite3_free(sql2);
              sqlite3_free(prev);
              prev = sql;
            }
        }
      free(xdb_prefix);
      free(xtable);
      free(xgeometry);
      sqlite3_free(rtree_name);
    }

  sql = prev;
  wxString tbl = wxString::FromUTF8(ActiveLayer->GetTableName());
  wxString geom = wxT("geom");
  MapPanel->InitializeSqlFilters(ActiveLayer->GetDbPrefix(), tbl, true, geom);
  wxString sqlstr = wxString::FromUTF8(sql);
  sqlite3_free(sql);
  MapPanel->SetSql(sqlstr, true);
}

void MyMapView::PrepareMap()
{
//
// draws the MapBitmap
//
  double linear_res;
  double factor;
  if (FirstLayer == NULL)
    ValidMap = false;
  if (!ValidMap)
    {
      // displaying an empty map
      Refresh();
      return;
    }
  if (CurrentCanvas != NULL)
    return;

  MapPanel->UpdateTools();
  wxMemoryDC *dc = NULL;
  dc = new wxMemoryDC(MapBitmap);
  if (dc->IsOk() == false)
    {
      delete dc;
      return;
    }

  wxSize sz = GetClientSize();
  FrameWidth = sz.GetWidth();
  FrameHeight = sz.GetHeight();

// initializing the Map Background
  dc->SetBrush(wxBrush(wxColour(0, 0, 0), wxSOLID));
  if (CheckeredMapBackground == true)
    DoPaintCheckeredBackground(dc);
  else
    dc->SetBrush(wxBrush
                 (wxColour
                  (SolidMapBackground.Red(), SolidMapBackground.Green(),
                   SolidMapBackground.Blue()), wxSOLID));
  dc->SetPen(wxPen(wxColour(255, 255, 255), 1, wxSOLID));
  dc->DrawRectangle(-1, -1, FrameWidth + 2, FrameHeight + 2);

// invalidating the current Screen
  wxMemoryDC *dc2 = NULL;
  dc2 = new wxMemoryDC(ScreenBitmap);
  if (dc2->IsOk() == true)
    {
      dc2->SetBrush(wxBrush(wxColour(0, 0, 0), wxSOLID));
      if (CheckeredMapBackground == true)
        DoPaintCheckeredBackground(dc2);
      else
        dc2->SetBrush(wxBrush
                      (wxColour
                       (SolidMapBackground.Red(), SolidMapBackground.Green(),
                        SolidMapBackground.Blue()), wxSOLID));
      dc2->SetPen(wxPen(wxColour(255, 255, 255), 1, wxSOLID));
      dc2->DrawRectangle(-1, -1, FrameWidth + 2, FrameHeight + 2);
      dc2->SetBrush(wxNullBrush);
      dc2->SetPen(wxNullPen);
      delete dc2;
    }
// computing the current Frame
  FrameMinX = FrameCX - ((FrameWidth / 2.0) * PixelRatio);
  FrameMaxX = FrameCX + ((FrameWidth / 2.0) * PixelRatio);
  FrameMinY = FrameCY - ((FrameHeight / 2.0) * PixelRatio);
  FrameMaxY = FrameCY + ((FrameHeight / 2.0) * PixelRatio);
  FrameExtentX = FrameMaxX - FrameMinX;
  FrameExtentY = FrameMaxY - FrameMinY;

  if (MapPanel->IsGeographicSRID(GetMapSRID()) == true)
    {
      // LongLat: computing the Scale 
      double metres =
        FrameExtentX * (6378137.0 * 2.0 * 3.141592653589793) / 360.0;
      linear_res = metres / (double) FrameWidth;
  } else
    {
      // planar: computing the Scale
      double x_res = FrameExtentX / (double) FrameWidth;
      double y_res = FrameExtentY / (double) FrameHeight;
      linear_res = sqrt(x_res * y_res);
    }
  factor = linear_res / 0.000254;
  CurrentScale = factor * (0.28 / 0.254);
  MapPanel->UpdateMapScale();

  dc->SetBrush(wxNullBrush);
  dc->SetPen(wxNullPen);
  delete dc;
  ResetScreenBitmap();

// preparing the multi-threaded canvas renderer
  CurrentCanvas = new MultiLayerPainter(this, FrameWidth, FrameHeight);
  MapLayer *lyr = FirstLayer;
  while (lyr != NULL)
    {
      CurrentCanvas->InsertLayer(lyr);
      lyr = lyr->GetNext();
    }
  DoPaintMap();
}

void MyMapView::DoPaintMap()
{
// Painting a multi-layer Map
  SingleLayerPainter *lyr;
  ::wxBeginBusyCursor();

  lyr = CurrentCanvas->GetFirst();
  while (lyr != NULL)
    {
      // resetting all cached layers
      const char *style = NULL;
      MapLayer *layer = lyr->GetLayer();
      switch (layer->GetType())
        {
          case MAP_LAYER_WMS:
            {
              WmsLayerSettings *config = layer->GetWmsConfig();
              style = config->GetStyle();
            }
            break;
          case MAP_LAYER_RASTER:
            {
              RasterLayerConfig *config = layer->GetRasterConfig();
              style = config->GetStyle();
            }
            break;
          case MAP_LAYER_VECTOR:
          case MAP_LAYER_VECTOR_VIEW:
          case MAP_LAYER_VECTOR_VIRTUAL:
          case MAP_LAYER_TOPOLOGY:
          case MAP_LAYER_NETWORK:
            {
              VectorLayerConfig *config = layer->GetVectorConfig();
              style = config->GetStyle();
            }
            break;
        };
      layer->PrepareGraphicsContext(FrameWidth, FrameHeight, FrameMinX,
                                    FrameMinY, FrameMaxX, FrameMaxY, style);
      lyr = lyr->GetNext();
    }

// refreshing the Map Image
  wxString mode = wxT("WAIT ... screen refresh in progress");
  MapPanel->UpdateMapMode(mode);
  MapPaintPhase = 0;
  MapPanel->SetRedLight();
  if (!TimerMapPaint)
    TimerMapPaint = new wxTimer(this, ID_MAP_PAINT_TIMER);
  TimerMapPaint->Start(250, wxTIMER_ONE_SHOT);
  if (IsMultiThreadingEnabled() == true && GetMaxThreads() > 1)
    {
      // using a parallel MultiThread approach
      RunMultiThreadPaintMap();
  } else
    {
      // using the most classic serial approach
      RunMonoThreadPaintMap();
    }
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoMonoThreadPaintMap(void *arg)
#else
void *DoMonoThreadPaintMap(void *arg)
#endif
{
// Painting the Map in a serial way using a single thread
  MapViewPaintParams *params = (MapViewPaintParams *) arg;
  MultiLayerPainter *canvas = params->GetCanvas();
  MyMapView *view = params->GetMapView();
  SingleLayerPainter *lyr;

#ifndef _WIN32
  pthread_t th_id = pthread_self();
  int policy;
  struct sched_param sched;
  if (pthread_getschedparam(th_id, &policy, &sched) == 0)
    {
      if (policy == SCHED_OTHER && sched.sched_priority == 0)
        {
          // setting a lower priority
          nice(10);
        }
    }
#endif

  int retry = 5;
  while (retry >= 0)
    {
      lyr = canvas->GetFirst();
      while (lyr != NULL)
        {
          if (lyr->GetLayer()->IsVisible() == false)
            {
              // skipping all hidden Layers
              lyr = lyr->GetNext();
              continue;
            }
          if (lyr->IsReady() == false)
            {
              view->DoPaintLayer(lyr);
              canvas->SetChanged(true);
            }
          if (lyr->IsReady() == true)
            {
              // updating the Map Image on the Screen
              canvas->SetChanged(true);
              wxCommandEvent evt_step(wxEVT_COMMAND_BUTTON_CLICKED,
                                      ID_PAINT_MAP_STEP);
              view->GetEventHandler()->AddPendingEvent(evt_step);
            }
          lyr = lyr->GetNext();
        }
      if (canvas->IsReady() == true)
        break;
      retry--;
    }

  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_PAINT_MAP_THREAD_FINISHED);
  view->GetEventHandler()->AddPendingEvent(event);

#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void MyMapView::RunMonoThreadPaintMap()
{
// Painting the Map in a serial way using a single thread
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  MapThreadParams.Initialize(MapPanel, this, CurrentCanvas);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoMonoThreadPaintMap, &Params, 0, &dwThreadId);
  SetThreadPriority(thread_handle, THREAD_PRIORITY_IDLE);
#else
  int ok_prior = 0;
  int policy;
  int min_prio;
  pthread_attr_t attr;
  struct sched_param sp;
  pthread_attr_init(&attr);
  if (pthread_attr_setschedpolicy(&attr, SCHED_RR) == 0)
    {
      // attempting to set the lowest priority  
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          min_prio = sched_get_priority_min(policy);
          sp.sched_priority = min_prio;
          if (pthread_attr_setschedparam(&attr, &sp) == 0)
            {
              // ok, setting the lowest priority  
              ok_prior = 1;
              pthread_create(&thread_id, &attr, DoMonoThreadPaintMap,
                             &MapThreadParams);
              pthread_detach(thread_id);
            }
        }
    }
#ifdef __MINGW32__
  if (!ok_prior)
    {
      // attempting to set the lowest priority on Windows
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          if (policy == SCHED_OTHER)
            {
              min_prio = sched_get_priority_min(policy);
              sp.sched_priority = min_prio;
              if (pthread_attr_setschedparam(&attr, &sp) == 0)
                {
                  // ok, setting the lowest priority  
                  ok_prior = 1;
                  pthread_create(&thread_id, &attr, DoMonoThreadPaintMap,
                                 &MapThreadParams);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoMonoThreadPaintMap, &MapThreadParams);
      pthread_detach(thread_id);
    }
#endif
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoThreadPaintLayer(void *arg)
#else
void *DoThreadPaintLayer(void *arg)
#endif
{
// Painting a single Map Layer using a parallel thread
  LayerThreadParams *params = (LayerThreadParams *) arg;
  MultiLayerPainter *canvas = params->GetCanvas();
  SingleLayerPainter *lyr = params->GetLayer();
  MapViewPaintParams *mother = params->GetMother();
  MyMapView *view = params->GetMapView();
  int index = params->GetThreadIndex();

  delete params;

#ifndef _WIN32
  pthread_t th_id = pthread_self();
  int policy;
  struct sched_param sched;
  if (pthread_getschedparam(th_id, &policy, &sched) == 0)
    {
      if (policy == SCHED_OTHER && sched.sched_priority == 0)
        {
          // setting a lower priority
          nice(10);
        }
    }
#endif

  lyr->IncrementRetryCount();
  view->DoPaintLayer(lyr);
  mother->ReleaseThread(index);
  lyr->SetLocked(false);
  if (lyr->IsReady() == true)
    {
      canvas->SetChanged(true);
      wxCommandEvent evt_step(wxEVT_COMMAND_BUTTON_CLICKED, ID_PAINT_MAP_STEP);
      view->GetEventHandler()->AddPendingEvent(evt_step);
    }
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

static void RunPaintLayerThread(int index, SingleLayerPainter * lyr,
                                MyMapPanel * panel, MyMapView * view,
                                MultiLayerPainter * canvas,
                                MapViewPaintParams * mother)
{
// Painting a Map Layer by starting a parallel Thread
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  LayerThreadParams *params =
    new LayerThreadParams(index, lyr, panel, view, canvas, mother);

#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoThreadPaintLayer, params, 0, &dwThreadId);
  SetThreadPriority(thread_handle, THREAD_PRIORITY_IDLE);
#else
  int ok_prior = 0;
  int policy;
  int min_prio;
  pthread_attr_t attr;
  struct sched_param sp;
  pthread_attr_init(&attr);
  if (pthread_attr_setschedpolicy(&attr, SCHED_RR) == 0)
    {
      // attempting to set the lowest priority  
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          min_prio = sched_get_priority_min(policy);
          sp.sched_priority = min_prio;
          if (pthread_attr_setschedparam(&attr, &sp) == 0)
            {
              // ok, setting the lowest priority  
              ok_prior = 1;
              pthread_create(&thread_id, &attr, DoThreadPaintLayer, params);
              pthread_detach(thread_id);
            }
        }
    }
#ifdef __MINGW32__
  if (!ok_prior)
    {
      // attempting to set the lowest priority on Windows
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          if (policy == SCHED_OTHER)
            {
              min_prio = sched_get_priority_min(policy);
              sp.sched_priority = min_prio;
              if (pthread_attr_setschedparam(&attr, &sp) == 0)
                {
                  // ok, setting the lowest priority  
                  ok_prior = 1;
                  pthread_create(&thread_id, &attr, DoThreadPaintLayer, params);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoThreadPaintLayer, params);
      pthread_detach(thread_id);
    }
#endif
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoMultiThreadPaintMap(void *arg)
#else
void *DoMultiThreadPaintMap(void *arg)
#endif
{
// Painting the Map in a paralle way using multiple threads
  MapViewPaintParams *params = (MapViewPaintParams *) arg;
  MyMapPanel *panel = params->GetMapPanel();
  MyMapView *view = params->GetMapView();
  MultiLayerPainter *canvas = params->GetCanvas();
  SingleLayerPainter *lyr;

#ifndef _WIN32
  pthread_t th_id = pthread_self();
  int policy;
  struct sched_param sched;
  if (pthread_getschedparam(th_id, &policy, &sched) == 0)
    {
      if (policy == SCHED_OTHER && sched.sched_priority == 0)
        {
          // setting a lower priority
          nice(10);
        }
    }
#endif

  bool repaint = false;
  lyr = canvas->GetFirst();
  while (lyr != NULL)
    {
      if (lyr->GetLayer()->IsVisible() == false)
        {
          // skipping all hidden Layers
          lyr = lyr->GetNext();
          continue;
        }
      if (lyr->IsReady() == true)
        repaint = true;
      lyr = lyr->GetNext();
    }
  if (repaint == true)
    {
      canvas->SetChanged(true);
      wxCommandEvent evt_step(wxEVT_COMMAND_BUTTON_CLICKED, ID_PAINT_MAP_STEP);
      params->GetMapView()->GetEventHandler()->AddPendingEvent(evt_step);
    }

  int refused = 0;
  while (1)
    {
      lyr = canvas->GetFirst();
      while (lyr != NULL)
        {
          if (lyr->GetLayer()->IsVisible() == false)
            {
              // skipping all hidden Layers
              lyr = lyr->GetNext();
              continue;
            }
          if (lyr->IsReady() == false && lyr->IsLocked() == false
              && lyr->GetRetryCount() < 5)
            {
              int index = params->FindUnusedThread();
              if (index >= 0)
                {
                  lyr->SetLocked(true);
                  ::RunPaintLayerThread(index, lyr, panel, view, canvas,
                                        params);
              } else
                refused = 1;
            }
          lyr = lyr->GetNext();
        }
      if (canvas->IsReady() == true)
        break;
      if (canvas->IsExhausted() == true)
        break;
      if (refused)
        ::wxMilliSleep(10);
      else
        ::wxMilliSleep(100);
    }

  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_PAINT_MAP_THREAD_FINISHED);
  params->GetMapView()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void MyMapView::RunMultiThreadPaintMap()
{
// Painting the Map in a parallel way using a pool of multiple threads
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  MapThreadParams.Initialize(MapPanel, this, CurrentCanvas);
  MapThreadParams.PrepareThreadsPool(GetMaxThreads());
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoMultiThreadPaintMap, &Params, 0, &dwThreadId);
  SetThreadPriority(thread_handle, THREAD_PRIORITY_IDLE);
#else
  int ok_prior = 0;
  int policy;
  int min_prio;
  pthread_attr_t attr;
  struct sched_param sp;
  pthread_attr_init(&attr);
  if (pthread_attr_setschedpolicy(&attr, SCHED_RR) == 0)
    {
      // attempting to set the lowest priority  
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          min_prio = sched_get_priority_min(policy);
          sp.sched_priority = min_prio;
          if (pthread_attr_setschedparam(&attr, &sp) == 0)
            {
              // ok, setting the lowest priority  
              ok_prior = 1;
              pthread_create(&thread_id, &attr, DoMultiThreadPaintMap,
                             &MapThreadParams);
              pthread_detach(thread_id);
            }
        }
    }
#ifdef __MINGW32__
  if (!ok_prior)
    {
      // attempting to set the lowest priority on Windows
      if (pthread_attr_getschedpolicy(&attr, &policy) == 0)
        {
          if (policy == SCHED_OTHER)
            {
              min_prio = sched_get_priority_min(policy);
              sp.sched_priority = min_prio;
              if (pthread_attr_setschedparam(&attr, &sp) == 0)
                {
                  // ok, setting the lowest priority  
                  ok_prior = 1;
                  pthread_create(&thread_id, &attr, DoMultiThreadPaintMap,
                                 &MapThreadParams);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoMultiThreadPaintMap, &MapThreadParams);
      pthread_detach(thread_id);
    }
#endif
}

void MyMapView::DoFinalMapImage()
{
// showing the final Map Image on the Screen
  MultiLayerPainter *canvas = CurrentCanvas;
  SingleLayerPainter *lyr = canvas->GetFirst();
  while (lyr != NULL)
    {
      // testing for Layers now yet shown on the screen
      if (lyr->GetLayer()->IsVisible() == false)
        {
          // skipping all hidden Layers
          lyr = lyr->GetNext();
          continue;
        }
      if (lyr->IsOnScreen() == false)
        canvas->SetChanged(true);
      lyr = lyr->GetNext();
    }

  if (canvas->IsChanged() == false)
    {
      delete canvas;
      CurrentCanvas = NULL;
      return;
    }

  wxMemoryDC *dc = NULL;
  dc = new wxMemoryDC(MapBitmap);
  if (dc->IsOk() == false)
    {
      delete dc;
      return;
    }

  wxSize sz = GetClientSize();
  FrameWidth = sz.GetWidth();
  FrameHeight = sz.GetHeight();

// initializing the Map Background
  dc->SetBrush(wxBrush(wxColour(0, 0, 0), wxSOLID));
  if (CheckeredMapBackground == true)
    DoPaintCheckeredBackground(dc);
  else
    dc->SetBrush(wxBrush
                 (wxColour
                  (SolidMapBackground.Red(), SolidMapBackground.Green(),
                   SolidMapBackground.Blue()), wxSOLID));
  dc->SetPen(wxPen(wxColour(255, 255, 255), 1, wxSOLID));
  dc->DrawRectangle(-1, -1, FrameWidth + 2, FrameHeight + 2);
  delete dc;

  lyr = canvas->GetFirst();
  while (lyr != NULL)
    {
      // painting all visible Layers
      if (lyr->GetLayer()->IsVisible() == false)
        {
          // skipping all hidden Layers
          lyr = lyr->GetNext();
          continue;
        }
      MapLayer *layer = lyr->GetLayer();
      rl2GraphicsContextPtr ctx_out = canvas->GetGraphicsContext();
      rl2GraphicsContextPtr ctx_in = layer->GetGraphicsContext();
      if (ctx_out != NULL && ctx_in != NULL)
        rl2_graph_merge(ctx_out, ctx_in);
      lyr = lyr->GetNext();
    }
// overimposing all Labels (if any)
  lyr = canvas->GetFirst();
  while (lyr != NULL)
    {
      if (lyr->GetLayer()->IsVisible() == false)
        {
          // skipping all hidden Layers
          lyr = lyr->GetNext();
          continue;
        }
      MapLayer *layer = lyr->GetLayer();
      rl2GraphicsContextPtr ctx_out = canvas->GetGraphicsContext();
      rl2GraphicsContextPtr ctx_in = layer->GetLabelsGraphicsContext();
      if (ctx_out != NULL && ctx_in != NULL)
        rl2_graph_merge(ctx_out, ctx_in);
      lyr = lyr->GetNext();
    }

// updating the Screen
  unsigned int width = canvas->GetWidth();
  unsigned int height = canvas->GetHeight();
  wxImage img = wxImage(width, height);
  img.SetData(canvas->GetRGB(), false);
  img.SetAlpha(canvas->GetAlpha(), false);
  delete canvas;
  CurrentCanvas = NULL;
  ResetMapBitmap(img, width, height);
}

void MyMapView::OnMapImagePaintStep(wxCommandEvent & WXUNUSED(event))
{
// showing on the Screen a step of the Map Image paint process
  MultiLayerPainter *canvas = CurrentCanvas;
  if (canvas->IsChanged() == false)
    return;

  wxMemoryDC *dc = NULL;
  dc = new wxMemoryDC(MapBitmap);
  if (dc->IsOk() == false)
    {
      delete dc;
      return;
    }

  wxSize sz = GetClientSize();
  FrameWidth = sz.GetWidth();
  FrameHeight = sz.GetHeight();

// initializing the Map Background
  dc->SetBrush(wxBrush(wxColour(0, 0, 0), wxSOLID));
  if (CheckeredMapBackground == true)
    DoPaintCheckeredBackground(dc);
  else
    dc->SetBrush(wxBrush
                 (wxColour
                  (SolidMapBackground.Red(), SolidMapBackground.Green(),
                   SolidMapBackground.Blue()), wxSOLID));
  dc->SetPen(wxPen(wxColour(255, 255, 255), 1, wxSOLID));
  dc->DrawRectangle(-1, -1, FrameWidth + 2, FrameHeight + 2);
  delete dc;

  SingleLayerPainter *lyr = canvas->GetFirst();
  while (lyr != NULL)
    {
      // painting all visible Layers
      if (lyr->GetLayer()->IsVisible() == false)
        {
          // skipping all hidden Layers
          lyr = lyr->GetNext();
          continue;
        }
      if (lyr->IsReady() == false)
        {
          // skipping all incomplete Layers
          lyr = lyr->GetNext();
          continue;
        }
      MapLayer *layer = lyr->GetLayer();
      rl2GraphicsContextPtr ctx_out = canvas->GetGraphicsContext();
      rl2GraphicsContextPtr ctx_in = layer->GetGraphicsContext();
      if (ctx_out != NULL && ctx_in != NULL)
        rl2_graph_merge(ctx_out, ctx_in);
      lyr = lyr->GetNext();
    }
// overimposing all Labels (if any)
  lyr = canvas->GetFirst();
  while (lyr != NULL)
    {
      if (lyr->GetLayer()->IsVisible() == false)
        {
          // skipping all hidden Layers
          lyr = lyr->GetNext();
          continue;
        }
      if (lyr->IsReady() == false)
        {
          // skipping all incomplete Layers
          lyr = lyr->GetNext();
          continue;
        }
      MapLayer *layer = lyr->GetLayer();
      rl2GraphicsContextPtr ctx_out = canvas->GetGraphicsContext();
      rl2GraphicsContextPtr ctx_in = layer->GetLabelsGraphicsContext();
      if (ctx_out != NULL && ctx_in != NULL)
        rl2_graph_merge(ctx_out, ctx_in);
      lyr->SetOnScreen(true);
      lyr = lyr->GetNext();
    }

// updating the Screen
  unsigned int width = canvas->GetWidth();
  unsigned int height = canvas->GetHeight();
  wxImage img = wxImage(width, height);
  img.SetData(canvas->GetRGB(), false);
  img.SetAlpha(canvas->GetAlpha(), false);
  ResetMapBitmap(img, width, height);
  canvas->SetChanged(false);
}

void MyMapView::OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
// resuming execution when the Paint Map thread quits
  DoFinalMapImage();
  if (TimerMapPaint)
    {
      TimerMapPaint->Stop();
      delete TimerMapPaint;
      TimerMapPaint = NULL;
    }
  MapPanel->SetGreenLight();
  MapPanel->UpdateCurrentMapMode();
  ::wxEndBusyCursor();
}

void MyMapView::DoPaintLayer(SingleLayerPainter * lyr)
{
// painting some Map Layer
  if (lyr->IsReady() == true)
    return;
  switch (lyr->GetLayer()->GetType())
    {
      case MAP_LAYER_WMS:
        DoPaintLayerWms(lyr);
        break;
      case MAP_LAYER_RASTER:
        DoPaintLayerRaster(lyr);
        break;
      case MAP_LAYER_VECTOR:
      case MAP_LAYER_VECTOR_VIEW:
      case MAP_LAYER_VECTOR_VIRTUAL:
      case MAP_LAYER_TOPOLOGY:
      case MAP_LAYER_NETWORK:
        DoPaintLayerVector(lyr);
        break;
      default:
        break;
    };
}

void MyMapView::DoPaintLayerWms(SingleLayerPainter * lyr)
{
// painting a WMS Layer
  MapLayer *layer;
  WmsLayerSettings *config;
  char *proxy = NULL;
  char *layer_name = NULL;
  int len;
  unsigned char *rgba;
  QuickStyleWmsObj *qStyle;

  if (lyr == NULL)
    return;
  layer = lyr->GetLayer();
  if (layer == NULL)
    return;
  config = layer->GetWmsConfig();
  if (config == NULL)
    return;

  qStyle = layer->GetQuickStyleWms();
  if (qStyle != NULL)
    {
      // testing for conditional visibility depending on current Scale
      bool visible = true;
      if (qStyle->IsMinScaleEnabled() == true)
        {
          if (CurrentScale < qStyle->GetScaleMin())
            visible = false;
        }
      if (qStyle->IsMaxScaleEnabled() == true)
        {
          if (CurrentScale > qStyle->GetScaleMax())
            visible = false;
        }
      if (visible == false)     // layer is not currently visible
        return;
    }

  wxString http_proxy = MapPanel->GetHttpProxy();
  len = http_proxy.Len();
  if (len == 0)
    proxy = NULL;
  else
    {
      proxy = (char *) malloc((len * 4) + 1);
      strcpy(proxy, http_proxy.ToUTF8());
    }
  wxString name = layer->GetName();
  len = name.Len();
  if (len == 0)
    layer_name = NULL;
  else
    {
      layer_name = (char *) malloc((len * 4) + 1);
      strcpy(layer_name, name.ToUTF8());
    }

  rgba =
    do_wms_GetMap_get(MapPanel->GetWmsCache(), layer->GetWmsGetMapURL(), proxy,
                      config->GetVersion(), layer_name, config->GetRefSys(),
                      config->IsSwapXY(), FrameMinX, FrameMinY, FrameMaxX,
                      FrameMaxY, lyr->GetWidth(), lyr->GetHeight(),
                      config->GetStyle(), config->GetImageFormat(),
                      config->IsOpaque(), config->IsCached());
  if (rgba != NULL)
    {
      rl2GraphicsContextPtr ctx_out = layer->GetGraphicsContext();
      rl2GraphicsContextPtr ctx_in =
        rl2_graph_create_context_rgba(MapPanel->
                                      GetParent()->GetRL2PrivateData(),
                                      lyr->GetWidth(),
                                      lyr->GetHeight(), rgba);
      if (ctx_out != NULL && ctx_in != NULL)
        {
          rl2_graph_merge(ctx_out, ctx_in);
          layer->Validate();
        }
      if (ctx_in)
        rl2_graph_destroy_context(ctx_in);
      free(rgba);
    }

  if (proxy != NULL)
    free(proxy);
  if (layer_name != NULL)
    free(layer_name);
}

void MyMapView::DoPaintLayerRaster(SingleLayerPainter * lyr)
{
// painting a Raster Layer
  MapLayer *layer;
  RasterLayerConfig *config;
  char *db_prefix;
  char *layer_name = NULL;
  int len;
  rl2CanvasPtr canvas;
  sqlite3_stmt *stmt = NULL;
  const char *sql;
  int ret;
  unsigned char *quickStyle = NULL;
  unsigned char syntetic_band = RL2_SYNTETIC_NONE;

  if (lyr == NULL)
    return;
  layer = lyr->GetLayer();
  if (layer == NULL)
    return;
  config = layer->GetRasterConfig();
  if (config == NULL)
    return;

// attempting to apply a QuickStyle
  if (layer->GetQuickStyleRaster() != NULL)
    {
      if (strcasecmp
          (layer->GetQuickStyleRaster()->GetUUID(), config->GetStyle()) == 0)
        {
          // applying the Quick Style
          quickStyle = layer->GetQuickStyleRaster()->CloneXmlStyle();
          if (layer->GetQuickStyleRaster()->IsNdviColorMap())
            syntetic_band = RL2_SYNTETIC_NDVI;
          if (layer->GetQuickStyleRaster()->IsNdwiColorMap())
            syntetic_band = RL2_SYNTETIC_NDWI;
        }
    }

  wxString prefix = layer->GetDbPrefix();
  len = prefix.Len();
  if (len == 0)
    db_prefix = NULL;
  else
    {
      db_prefix = (char *) malloc((len * 4) + 1);
      strcpy(db_prefix, prefix.ToUTF8());
    }
  wxString name = layer->GetName();
  len = name.Len();
  if (len == 0)
    layer_name = NULL;
  else
    {
      layer_name = (char *) malloc((len * 4) + 1);
      strcpy(layer_name, name.ToUTF8());
    }

// building the Map's Bounding Box
  sql = "SELECT BuildMbr(?, ?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_double(stmt, 1, FrameMinX);
  sqlite3_bind_double(stmt, 2, FrameMinY);
  sqlite3_bind_double(stmt, 3, FrameMaxX);
  sqlite3_bind_double(stmt, 4, FrameMaxY);
  sqlite3_bind_int(stmt, 5, layer->GetMapSRID());

  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              unsigned char *blob =
                (unsigned char *) sqlite3_column_blob(stmt, 0);
              int blob_sz = sqlite3_column_bytes(stmt, 0);
              canvas = layer->CreateCanvas();
              rl2_map_image_paint_from_raster(MapPanel->GetSqlite(),
                                              MapPanel->GetRL2PrivateData(),
                                              canvas, db_prefix, layer_name,
                                              blob, blob_sz,
                                              config->GetStyle(), quickStyle,
                                              syntetic_band);
              layer->Validate(canvas);
              rl2_destroy_canvas(canvas);
            }
        }
    }
  sqlite3_finalize(stmt);
  if (db_prefix != NULL)
    free(db_prefix);
  if (layer_name != NULL)
    free(layer_name);
}

void MyMapView::DoPaintLayerVector(SingleLayerPainter * lyr)
{
// painting a Vector Layer
  MapLayer *layer;
  VectorLayerConfig *config;
  char *db_prefix;
  char *layer_name = NULL;
  int len;
  rl2CanvasPtr canvas;
  sqlite3_stmt *stmt = NULL;
  const char *sql;
  int ret;
  char *styleName = NULL;
  int freeStyleName = 0;
  unsigned char *quickStyle = NULL;

  if (lyr == NULL)
    return;
  layer = lyr->GetLayer();
  if (layer == NULL)
    return;
  config = layer->GetVectorConfig();
  if (config == NULL)
    return;

  wxString prefix = layer->GetDbPrefix();
  len = prefix.Len();
  if (len == 0)
    db_prefix = NULL;
  else
    {
      db_prefix = (char *) malloc((len * 4) + 1);
      strcpy(db_prefix, prefix.ToUTF8());
    }
  wxString name = layer->GetName();
  len = name.Len();
  if (len == 0)
    layer_name = NULL;
  else
    {
      layer_name = (char *) malloc((len * 4) + 1);
      strcpy(layer_name, name.ToUTF8());
    }

// attempting to apply a QuickStyle
  styleName = (char *) config->GetStyle();
  if (layer->GetQuickStyle() != NULL)
    {
      if (strcasecmp(layer->GetQuickStyle()->GetUUID(), config->GetStyle()) ==
          0)
        {
          // applying the Quick Style
          int len = strlen(config->GetStyle());
          styleName = (char *) malloc(len + 1);
          strcpy(styleName, config->GetStyle());
          freeStyleName = 1;
          quickStyle = layer->GetQuickStyle()->CloneXmlStyle();
        }
    }
  if (layer->GetQuickStyleTopology() != NULL)
    {
      if (strcasecmp
          (layer->GetQuickStyleTopology()->GetUUID(), config->GetStyle()) == 0)
        {
          // applying the Quick Style (Topology)
          int len = strlen(config->GetStyle());
          styleName = (char *) malloc(len + 1);
          strcpy(styleName, config->GetStyle());
          freeStyleName = 1;
          quickStyle = layer->GetQuickStyleTopology()->CloneXmlStyle();
        }
    }
  if (styleName == NULL)
    {
      // applying an ordinary Style
      int len = strlen(config->GetStyle());
      styleName = (char *) malloc(len + 1);
      strcpy(styleName, config->GetStyle());
fprintf(stderr, "Style=<%s>\n", styleName);
      freeStyleName = 1;
    }
// building the Map's Bounding Box 
  sql = "SELECT BuildMbr(?, ?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_double(stmt, 1, FrameMinX);
  sqlite3_bind_double(stmt, 2, FrameMinY);
  sqlite3_bind_double(stmt, 3, FrameMaxX);
  sqlite3_bind_double(stmt, 4, FrameMaxY);
  sqlite3_bind_int(stmt, 5, layer->GetMapSRID());

  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              unsigned char *blob =
                (unsigned char *) sqlite3_column_blob(stmt, 0);
              int blob_sz = sqlite3_column_bytes(stmt, 0);
              canvas = layer->CreateCanvas();
              const void *data = MapPanel->GetRL2PrivateData();
              if (layer->GetType() == MAP_LAYER_TOPOLOGY)
                rl2_map_image_paint_from_vector_ex(MapPanel->GetSqlite(), data,
                                                   canvas, db_prefix,
                                                   layer_name, blob, blob_sz, 0,
                                                   styleName, quickStyle,
                                                   config->AreNodesVisible(),
                                                   config->AreEdgesVisible(),
                                                   config->AreFacesVisible(),
                                                   config->AreEdgeSeedsVisible
                                                   (),
                                                   config->AreFaceSeedsVisible
                                                   ());
              else if (layer->GetType() == MAP_LAYER_NETWORK)
                rl2_map_image_paint_from_vector_ex(MapPanel->GetSqlite(), data,
                                                   canvas, db_prefix,
                                                   layer_name, blob, blob_sz, 0,
                                                   styleName, quickStyle,
                                                   config->AreNodesVisible(),
                                                   config->AreLinksVisible(), 0,
                                                   config->AreLinkSeedsVisible
                                                   (), 0);
              else
                rl2_map_image_paint_from_vector(MapPanel->GetSqlite(), data,
                                                canvas, db_prefix, layer_name,
                                                blob, blob_sz, 0,
                                                styleName, quickStyle);
              if (quickStyle != NULL)
                free(quickStyle);
              layer->Validate(canvas);
              rl2_destroy_canvas(canvas);
            }
        }
    }
  if (freeStyleName)
    free(styleName);
  sqlite3_finalize(stmt);
  if (db_prefix != NULL)
    free(db_prefix);
  if (layer_name != NULL)
    free(layer_name);
}

void MyMapView::DoPaintBlinkingPoint(rl2GraphicsContextPtr ctx, bool oddEven,
                                     gaiaPointPtr point)
{
// painting a blinking Point

// setting an half-transparent solid brush and pen
  if (oddEven == true)
    {
      rl2_graph_set_brush(ctx, 255, 255, 0, 128);
      rl2_graph_set_solid_pen(ctx, 0, 255, 255, 128, 8, RL2_PEN_CAP_BUTT,
                              RL2_PEN_JOIN_MITER);
  } else
    {
      rl2_graph_set_brush(ctx, 0, 255, 255, 128);
      rl2_graph_set_solid_pen(ctx, 255, 255, 0, 128, 8, RL2_PEN_CAP_BUTT,
                              RL2_PEN_JOIN_MITER);
    }

  double x = (point->X - FrameMinX) / PixelRatio;
  double y = (double) FrameHeight - ((point->Y - FrameMinY) / PixelRatio);
// drawing a Circle
  rl2_graph_draw_mark_symbol(ctx, RL2_GRAPHIC_MARK_CIRCLE, 24, x, y, 0.0, 0.5,
                             0.5, 1, 1);
}

void MyMapView::DoPaintBlinkingLinestring(rl2GraphicsContextPtr ctx,
                                          bool oddEven, gaiaLinestringPtr line)
{
// painting a blinking Linestring
  int iv;
  double dx;
  double dy;
  int x;
  int y;
  int lastX = 0;
  int lastY = 0;

// setting an half-transparent solid pen
  if (oddEven == true)
    rl2_graph_set_solid_pen(ctx, 255, 255, 0, 128, 8, RL2_PEN_CAP_BUTT,
                            RL2_PEN_JOIN_MITER);
  else
    rl2_graph_set_solid_pen(ctx, 0, 255, 255, 128, 8, RL2_PEN_CAP_BUTT,
                            RL2_PEN_JOIN_MITER);

  for (iv = 0; iv < line->Points; iv++)
    {
      gaiaGetPoint(line->Coords, iv, &dx, &dy);
      x = (int) ((dx - FrameMinX) / PixelRatio);
      y = FrameHeight - (int) ((dy - FrameMinY) / PixelRatio);
      if (iv == 0)
        {
          rl2_graph_move_to_point(ctx, x, y);
          lastX = x;
          lastY = y;
      } else
        {
          if (x == lastX && y == lastY)
            ;
          else
            {
              rl2_graph_add_line_to_path(ctx, x, y);
              lastX = x;
              lastY = y;
            }
        }
    }
  rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
}

void MyMapView::DoPaintBlinkingPolygon(rl2GraphicsContextPtr ctx, bool oddEven,
                                       gaiaPolygonPtr polyg)
{
// painting a blinking Polygon
  int iv;
  double dx;
  double dy;
  int x;
  int y;
  int lastX = 0;
  int lastY = 0;
  int ib;

// setting an half-transparent solid brush
  if (oddEven == true)
    rl2_graph_set_brush(ctx, 255, 255, 0, 128);
  else
    rl2_graph_set_brush(ctx, 0, 255, 255, 128);

  gaiaRingPtr ring = polyg->Exterior;
  // exterior border
  for (iv = 0; iv < ring->Points; iv++)
    {
      gaiaGetPoint(ring->Coords, iv, &dx, &dy);
      x = (int) ((dx - FrameMinX) / PixelRatio);
      y = FrameHeight - (int) ((dy - FrameMinY) / PixelRatio);
      if (iv == 0)
        {
          rl2_graph_move_to_point(ctx, x, y);
          lastX = x;
          lastY = y;
      } else
        {
          if (x == lastX && y == lastY)
            ;
          else
            {
              rl2_graph_add_line_to_path(ctx, x, y);
              lastX = x;
              lastY = y;
            }
        }
    }
  rl2_graph_close_subpath(ctx);
  for (ib = 0; ib < polyg->NumInteriors; ib++)
    {
      // interior borders
      ring = polyg->Interiors + ib;
      for (iv = 0; iv < ring->Points; iv++)
        {
          gaiaGetPoint(ring->Coords, iv, &dx, &dy);
          x = (int) ((dx - FrameMinX) / PixelRatio);
          y = FrameHeight - (int) ((dy - FrameMinY) / PixelRatio);
          if (iv == 0)
            {
              rl2_graph_move_to_point(ctx, x, y);
              lastX = x;
              lastY = y;
          } else
            {
              if (x == lastX && y == lastY)
                ;
              else
                {
                  rl2_graph_add_line_to_path(ctx, x, y);
                  lastX = x;
                  lastY = y;
                }
            }
        }
      rl2_graph_close_subpath(ctx);
    }
  rl2_graph_fill_path(ctx, RL2_CLEAR_PATH);
}

void MyMapView::DoPaintMapBlink(bool oddEven)
{
// making the Map to Blink - Higlighted features
  rl2GraphicsContextPtr ctx =
    rl2_graph_create_context(MapPanel->GetParent()->GetRL2PrivateData(),
                             FrameWidth, FrameHeight);
  if (ctx == NULL)
    return;

// preparing a transparent Graphics Context
  rl2_graph_set_brush(ctx, 255, 255, 255, 0);
  rl2_graph_draw_rectangle(ctx, -1, -1, FrameWidth + 2, FrameHeight + 2);

  MapFeatureGeom *mf = SelectedFeatures->GetFirst();
  while (mf != NULL)
    {
      // painting a Map Feature
      gaiaPointPtr pt;
      gaiaLinestringPtr ln;
      gaiaPolygonPtr pg;
      gaiaGeomCollPtr geom = mf->GetGeometry();
      if (geom == NULL)
        {
          mf = mf->GetNext();
          continue;
        }
      pt = geom->FirstPoint;
      while (pt != NULL)
        {
          // painting a Point
          DoPaintBlinkingPoint(ctx, oddEven, pt);
          pt = pt->Next;
        }
      ln = geom->FirstLinestring;
      while (ln != NULL)
        {
          // painting a Linestring
          DoPaintBlinkingLinestring(ctx, oddEven, ln);
          ln = ln->Next;
        }
      pg = geom->FirstPolygon;
      while (pg != NULL)
        {
          // painting a Polygon
          DoPaintBlinkingPolygon(ctx, oddEven, pg);
          pg = pg->Next;
        }
      mf = mf->GetNext();
    }

// transforming the Graphics Context into an Image
  unsigned char *rgb = rl2_graph_get_context_rgb_array(ctx);
  int half_transparent;
  unsigned char *alpha =
    rl2_graph_get_context_alpha_array(ctx, &half_transparent);
  rl2_graph_destroy_context(ctx);

// updating the Screen
  wxImage img = wxImage(FrameWidth, FrameHeight);
  img.SetData(rgb, false);
  img.SetAlpha(alpha, false);
  BlinkBitmap = wxBitmap(img);
  DynamicBlink = true;
  ResetScreenBitmap();
  DynamicBlink = false;
}

void MyMapView::ResetMapBitmap(wxImage & img, unsigned int width,
                               unsigned int height)
{
// showing the Map into the screen
  wxMemoryDC *mapDc = new wxMemoryDC(MapBitmap);
  wxGraphicsContext *gr = wxGraphicsContext::Create(*mapDc);
  gr->DrawBitmap(img, 0, 0, width, height);
  delete gr;
  delete mapDc;
  ResetScreenBitmap();
}

void MyMapView::ResetScreenBitmap()
{
// copying the MapBitmap into the ScreenBitmap
  wxMemoryDC *screenDc = new wxMemoryDC(ScreenBitmap);
  wxGraphicsContext *gr = wxGraphicsContext::Create(*screenDc);
  gr->DrawBitmap(MapBitmap, 0, 0, BitmapWidth, BitmapHeight);
  if (BlinkBitmap.IsOk() == true && DynamicBlink == true)
    {
      gr->DrawBitmap(BlinkBitmap, 0, 0, FrameWidth, FrameHeight);
      BlinkBitmap = wxBitmap();
    }
  /*
     if (ImageMarkerOdd.IsOk() == true && DynamicOddEven == true)
     gr->DrawBitmap(ImageMarkerOdd, 0, 0, ImageMarkerOdd.GetWidth(),
     ImageMarkerOdd.GetHeight());
     if (ImageMarkerEven.IsOk() == true && DynamicOddEven == false)
     gr->DrawBitmap(ImageMarkerEven, 0, 0, ImageMarkerEven.GetWidth(),
     ImageMarkerEven.GetHeight());
   */
  delete gr;
  delete screenDc;
  Refresh();
  Update();
}

void MyMapView::ResetMapLayers()
{
//
// resetting the Map Layer list
//
  FirstLayer = NULL;
  LastLayer = NULL;
}

void MyMapView::RebuildLayerTree(MyLayerTree * LayerTree)
{
//
// rebuilding from scratch the Map Layers Tree
//
  wxTreeItemId layer;
  ::wxBeginBusyCursor();
  LayerTree->Hide();
  LayerTree->DeleteChildren(LayerTree->GetRoot());

  int count_rasters = 0;
  MapLayer *pL = FirstLayer;
  while (pL)
    {
      if (pL->GetType() == MAP_LAYER_RASTER || pL->GetType() == MAP_LAYER_WMS)
        count_rasters++;
      LayerTree->AddLayer(pL);
      pL = pL->GetNext();
    }
  LayerTree->Expand(LayerTree->GetRoot());
  LayerTree->Show();
  ::wxEndBusyCursor();

  if (MapPanel->IsRasterWmsAutoSwitch() == true && count_rasters > 1)
    {
      // AutoSwitching Raster/WMS Coverages
      wxTreeItemId layer;
      wxTreeItemIdValue cookie;
      layer = LayerTree->GetFirstChild(LayerTree->GetRoot(), cookie);
      count_rasters = 0;
      while (layer.IsOk() == true)
        {
          MapLayerObject *obj =
            (MapLayerObject *) LayerTree->GetItemData(layer);
          MapLayer *lyr = obj->GetLayer();
          if (lyr->GetType() == MAP_LAYER_RASTER
              || lyr->GetType() == MAP_LAYER_WMS)
            {
              if (lyr->IsVisible() == true)
                {
                  count_rasters++;
                  if (count_rasters > 1)
                    {
                      // making all Raster/WMS Layers to be invisible except the first one
                      lyr->Hide();
                      int idx = LayerTree->GetIconIndex(lyr);
                      LayerTree->SetItemImage(layer, idx);
                      LayerTree->SetItemTextColour(layer,
                                                   wxColour(192, 192, 192));
                    }
                }
            }
          layer = LayerTree->GetNextChild(LayerTree->GetRoot(), cookie);
        }
    }
  UpdateMapFullExtent();
}

bool MyMapView::IsAlreadyDefined(LayerListItem * layer)
{
//
// checking for an already defined MapLayer
//
  MapLayer *pL = FirstLayer;
  while (pL != NULL)
    {
      bool equals = true;
      if (pL->GetType() != layer->GetLayerType())
        equals = false;
      if (pL->GetDbPrefix() != layer->GetDbPrefix())
        equals = false;
      if (pL->GetVectorPrefix() != layer->GetLayerPrefix())
        equals = false;
      if (pL->GetName() != layer->GetLayerName())
        equals = false;
      if (equals == true)
        return true;
      pL = pL->GetNext();
    }
  return false;
}

bool MyMapView::InsertMapLayer(LayerListItem * layer, MyFrame * MainFrame)
{
//
//  adding a new Map Layer (if not already defined)
//
  if (IsAlreadyDefined(layer) == true)
    return false;

  MapLayer *lyr = new MapLayer(MainFrame->GetRL2PrivateData(), layer);
  if (layer->GetLayerType() == MAP_LAYER_RASTER)
    {
      DoFetchRasterInfos(lyr);
      DoFetchRasterExtent(lyr);
  } else if (layer->GetLayerType() == MAP_LAYER_VECTOR
             || layer->GetLayerType() == MAP_LAYER_VECTOR_VIEW
             || layer->GetLayerType() == MAP_LAYER_VECTOR_VIRTUAL
             || layer->GetLayerType() == MAP_LAYER_TOPOLOGY
             || layer->GetLayerType() == MAP_LAYER_NETWORK)
    DoFetchVectorExtent(lyr);
  else if (layer->GetLayerType() == MAP_LAYER_WMS)
    {
      DoFetchGetMapURL(lyr);
      DoFetchWMSextent(lyr);
      DoFetchWMSconfig(lyr);
    }
  lyr->AdjustMapSRID(MapPanel->GetSqlite(), MainFrame, AutoTransformEnabled,
                     MapSRID);
  lyr->AdjustDefaultStyle(MapPanel->GetSqlite());
  if (FirstLayer == NULL)
    FirstLayer = lyr;
  lyr->SetPrev(LastLayer);
  if (LastLayer != NULL)
    LastLayer->SetNext(lyr);
  LastLayer = lyr;
  return true;
}

bool MyMapView::InsertMapLayer(MapLayer * lyr)
{
//
//  adding a new Map Layer
//
  if (FirstLayer == NULL)
    FirstLayer = lyr;
  lyr->SetPrev(LastLayer);
  if (LastLayer != NULL)
    LastLayer->SetNext(lyr);
  LastLayer = lyr;
  return true;
}

void MyMapView::ReinsertMapLayer(MapLayer * lyr)
{
//
// reinserting yet again a Map Layer (Tree new order)
//
  lyr->SetPrev(NULL);
  lyr->SetNext(NULL);
  if (LastLayer == NULL)
    LastLayer = lyr;
  lyr->SetNext(FirstLayer);
  if (FirstLayer != NULL)
    FirstLayer->SetPrev(lyr);
  FirstLayer = lyr;
}

void MyMapView::RemoveMapLayer(MapLayer * layer)
{
//
// removing a Map layer
//
  if (ActiveLayer == layer)
    ActiveLayer = NULL;
  if (layer->GetPrev() != NULL)
    layer->GetPrev()->SetNext(layer->GetNext());
  if (layer->GetNext() != NULL)
    layer->GetNext()->SetPrev(layer->GetPrev());
  if (FirstLayer == layer)
    FirstLayer = layer->GetNext();
  if (LastLayer == layer)
    LastLayer = layer->GetPrev();
  delete layer;
}

void MyMapView::AdjustLayersMapSRID()
{
//
// adjusting Layers' MapSRID
//
  if (AutoTransformEnabled == false)
    return;

  MapLayer *pL = FirstLayer;
  while (pL)
    {
      pL->AdjustMapSRID(MapPanel->GetSqlite(), MapPanel->GetParent(),
                        AutoTransformEnabled, MapSRID);
      pL = pL->GetNext();
    }
}

bool MyMapView::CanQueryTable()
{
//
// test if Quary Map Table could be currently be enabled
//
  if (ActiveLayer == NULL)
    return false;
  if (ActiveLayer->IsVisible() == 0)
    return false;
  if (ActiveLayer->IsQueryable() == false)
    return false;
  int type = ActiveLayer->GetType();
  if (type == MAP_LAYER_VECTOR || type == MAP_LAYER_VECTOR_VIEW
      || type == MAP_LAYER_VECTOR_VIRTUAL)
    return true;
  return false;
}

bool MyMapView::CanIdentify()
{
//
// test if Identify could be currently be enabled
//
  if (ActiveLayer == NULL)
    return false;
  if (ActiveLayer->IsVisible() == 0)
    return false;
  if (ActiveLayer->IsQueryable() == false)
    return false;
  return true;
}

void MyMapView::DoFetchGetMapURL(MapLayer * layer)
{
//
// retrieving the WMS GetMap URL
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  sqlite3 *sqlite = MapPanel->GetSqlite();

  char *prefix = (char *) malloc((layer->GetDbPrefix().Len() * 4) + 1);
  strcpy(prefix, layer->GetDbPrefix().ToUTF8());
  char *qprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  char *name = (char *) malloc((layer->GetName().Len() * 4) + 1);
  strcpy(name, layer->GetName().ToUTF8());
  char *sql =
    sqlite3_mprintf
    ("SELECT url, getfeatureinfo_url FROM \"%s\".wms_getmap WHERE Lower(layer_name) = Lower(%Q)",
     qprefix, name);
  free(qprefix);
  free(name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
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
          layer->SetWmsGetMapURL(value);
          value = results[(i * columns) + 1];
          layer->SetWmsGetFeatureInfoURL(value);
        }
    }
  sqlite3_free_table(results);
}

void MyMapView::DoFetchRasterInfos(MapLayer * layer)
{
//
// retrieving the Raster Coverage infos
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  sqlite3 *sqlite = MapPanel->GetSqlite();

  char *prefix = (char *) malloc((layer->GetDbPrefix().Len() * 4) + 1);
  strcpy(prefix, layer->GetDbPrefix().ToUTF8());
  char *qprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  char *name = (char *) malloc((layer->GetName().Len() * 4) + 1);
  strcpy(name, layer->GetName().ToUTF8());
  char *sql =
    sqlite3_mprintf
    ("SELECT sample_type, pixel_type, num_bands, compression, quality, "
     "tile_width, tile_height, horz_resolution, vert_resolution "
     "FROM \"%s\".raster_coverages WHERE Lower(coverage_name) = Lower(%Q)",
     qprefix, name);
  free(qprefix);
  free(name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
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
          wxString sample_type = wxT("Unknown");
          if (strcmp(value, "1-BIT") == 0)
            sample_type = wxT("1-BIT");
          if (strcmp(value, "2-BIT") == 0)
            sample_type = wxT("2-BIT");
          if (strcmp(value, "4-BIT") == 0)
            sample_type = wxT("4-BIT");
          if (strcmp(value, "INT8") == 0)
            sample_type = wxT("INT8");
          if (strcmp(value, "UINT8") == 0)
            sample_type = wxT("UINT8");
          if (strcmp(value, "INT16") == 0)
            sample_type = wxT("INT16");
          if (strcmp(value, "UINT16") == 0)
            sample_type = wxT("UINT16");
          if (strcmp(value, "INT32") == 0)
            sample_type = wxT("INT32");
          if (strcmp(value, "UINT32") == 0)
            sample_type = wxT("UINT32");
          if (strcmp(value, "FLOAT") == 0)
            sample_type = wxT("FLOAT");
          if (strcmp(value, "DOUBLE") == 0)
            sample_type = wxT("DOUBLE");
          value = results[(i * columns) + 1];
          wxString pixel_type = wxT("Unknown");
          if (strcmp(value, "MONOCHROME") == 0)
            pixel_type = wxT("Monochrome");
          if (strcmp(value, "PALETTE") == 0)
            pixel_type = wxT("Palette");
          if (strcmp(value, "GRAYSCALE") == 0)
            pixel_type = wxT("Grayscale");
          if (strcmp(value, "RGB") == 0)
            pixel_type = wxT("RGB");
          if (strcmp(value, "MULTIBAND") == 0)
            pixel_type = wxT("MultiBand");
          if (strcmp(value, "DATAGRID") == 0)
            pixel_type = wxT("DataGrid");
          value = results[(i * columns) + 2];
          int bands = atoi(value);
          value = results[(i * columns) + 3];
          wxString compression = wxT("Unknown");
          if (strcmp(value, "NONE") == 0)
            compression = wxT("None");
          if (strcmp(value, "DEFLATE") == 0)
            compression = wxT("Deflate [Zip]");
          if (strcmp(value, "DEFLATE_NO") == 0)
            compression = wxT("Deflate-NoDelta");
          if (strcmp(value, "LZMA") == 0)
            compression = wxT("LZMA [7zip]");
          if (strcmp(value, "LZMA_NO") == 0)
            compression = wxT("LZMA-NoDelta");
          if (strcmp(value, "LZ4") == 0)
            compression = wxT("LZ4 [fast lossless]");
          if (strcmp(value, "LZ4_NO") == 0)
            compression = wxT("LZ4-NoDelta");
          if (strcmp(value, "ZSTD") == 0)
            compression = wxT("ZSTD [Zstandard]");
          if (strcmp(value, "ZSTD_NO") == 0)
            compression = wxT("ZSTD-NoDelta");
          if (strcmp(value, "PNG") == 0)
            compression = wxT("PNG");
          if (strcmp(value, "JPEG") == 0)
            compression = wxT("JPEG");
          if (strcmp(value, "LOSSY_WEBP") == 0)
            compression = wxT("WebP (lossy)");
          if (strcmp(value, "LOSSLESS_WEBP") == 0)
            compression = wxT("WebP (lossless)");
          if (strcmp(value, "CCITTFAX4") == 0)
            compression = wxT("CCITT FAX4");
          if (strcmp(value, "LOSSY_JP2") == 0)
            compression = wxT("Jpeg2000 (lossy)");
          if (strcmp(value, "LOSSLESS_JP2") == 0)
            compression = wxT("Jpeg2000 (lossless)");
          value = results[(i * columns) + 4];
          int quality = atoi(value);
          value = results[(i * columns) + 5];
          int tile_width = atoi(value);
          value = results[(i * columns) + 6];
          int tile_height = atoi(value);
          value = results[(i * columns) + 7];
          double horz_res = atof(value);
          value = results[(i * columns) + 8];
          double vert_res = atof(value);
          layer->SetRasterInfos(sample_type, pixel_type, bands, compression,
                                quality, tile_width, tile_height, horz_res,
                                vert_res);
        }
    }
  sqlite3_free_table(results);
}

void MyMapView::DoFetchRasterExtent(MapLayer * layer)
{
//
// retrieving the Raster full extent
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  sqlite3 *sqlite = MapPanel->GetSqlite();

  char *prefix = (char *) malloc((layer->GetDbPrefix().Len() * 4) + 1);
  strcpy(prefix, layer->GetDbPrefix().ToUTF8());
  char *qprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  char *name = (char *) malloc((layer->GetName().Len() * 4) + 1);
  strcpy(name, layer->GetName().ToUTF8());
  char *sql = sqlite3_mprintf("SELECT geo_minx, geo_miny, geo_maxx, geo_maxy, "
                              "extent_minx, extent_miny, extent_maxx, extent_maxy "
                              "FROM \"%s\".raster_coverages WHERE Lower(coverage_name) = Lower(%Q)",
                              qprefix, name);
  free(qprefix);
  free(name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
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
          double geo_minx = DBL_MAX;
          double geo_miny = DBL_MAX;
          double geo_maxx = -DBL_MAX;
          double geo_maxy = -DBL_MAX;
          double minx = DBL_MAX;
          double miny = DBL_MAX;
          double maxx = -DBL_MAX;
          double maxy = -DBL_MAX;
          value = results[(i * columns) + 0];
          if (value != NULL)
            geo_minx = atof(value);
          value = results[(i * columns) + 1];
          if (value != NULL)
            geo_miny = atof(value);
          value = results[(i * columns) + 2];
          if (value != NULL)
            geo_maxx = atof(value);
          value = results[(i * columns) + 3];
          if (value != NULL)
            geo_maxy = atof(value);
          value = results[(i * columns) + 4];
          if (value != NULL)
            minx = atof(value);
          value = results[(i * columns) + 5];
          if (value != NULL)
            miny = atof(value);
          value = results[(i * columns) + 6];
          if (value != NULL)
            maxx = atof(value);
          value = results[(i * columns) + 7];
          if (value != NULL)
            maxy = atof(value);
          if (geo_minx == DBL_MAX || geo_miny == DBL_MAX || geo_maxx == -DBL_MAX
              || geo_maxy == -DBL_MAX)
            {
              geo_minx = DBL_MAX;
              geo_miny = DBL_MAX;
              geo_maxx = -DBL_MAX;
              geo_maxy = -DBL_MAX;
            }
          if (minx == DBL_MAX || miny == DBL_MAX || maxx == DBL_MAX
              || maxy == DBL_MAX)
            {
              minx = DBL_MAX;
              miny = DBL_MAX;
              maxx = -DBL_MAX;
              maxy = -DBL_MAX;
            }
          layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
          layer->SetExtent(minx, miny, maxx, maxy);
        }
    }
  sqlite3_free_table(results);
}

void MyMapView::DoFetchVectorExtent(MapLayer * layer)
{
//
// retrieving the Vector full extent
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  sqlite3 *sqlite = MapPanel->GetSqlite();

  char *prefix = (char *) malloc((layer->GetDbPrefix().Len() * 4) + 1);
  strcpy(prefix, layer->GetDbPrefix().ToUTF8());
  char *qprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  char *name = (char *) malloc((layer->GetName().Len() * 4) + 1);
  strcpy(name, layer->GetName().ToUTF8());
  char *sql = sqlite3_mprintf("SELECT geo_minx, geo_miny, geo_maxx, geo_maxy, "
                              "extent_minx, extent_miny, extent_maxx, extent_maxy "
                              "FROM \"%s\".vector_coverages WHERE Lower(coverage_name) = Lower(%Q)",
                              qprefix, name);
  free(qprefix);
  free(name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
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
          double geo_minx = DBL_MAX;
          double geo_miny = DBL_MAX;
          double geo_maxx = -DBL_MAX;
          double geo_maxy = -DBL_MAX;
          double minx = DBL_MAX;
          double miny = DBL_MAX;
          double maxx = -DBL_MAX;
          double maxy = -DBL_MAX;
          value = results[(i * columns) + 0];
          if (value != NULL)
            geo_minx = atof(value);
          value = results[(i * columns) + 1];
          if (value != NULL)
            geo_miny = atof(value);
          value = results[(i * columns) + 2];
          if (value != NULL)
            geo_maxx = atof(value);
          value = results[(i * columns) + 3];
          if (value != NULL)
            geo_maxy = atof(value);
          value = results[(i * columns) + 4];
          if (value != NULL)
            minx = atof(value);
          value = results[(i * columns) + 5];
          if (value != NULL)
            miny = atof(value);
          value = results[(i * columns) + 6];
          if (value != NULL)
            maxx = atof(value);
          value = results[(i * columns) + 7];
          if (value != NULL)
            maxy = atof(value);
          if (geo_minx == DBL_MAX || geo_miny == DBL_MAX || geo_maxx == -DBL_MAX
              || geo_maxy == -DBL_MAX)
            {
              geo_minx = DBL_MAX;
              geo_miny = DBL_MAX;
              geo_maxx = -DBL_MAX;
              geo_maxy = -DBL_MAX;
            }
          if (minx == DBL_MAX || miny == DBL_MAX || maxx == -DBL_MAX
              || maxy == -DBL_MAX)
            {
              minx = DBL_MAX;
              miny = DBL_MAX;
              maxx = -DBL_MAX;
              maxy = -DBL_MAX;
            }
          layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
          layer->SetExtent(minx, miny, maxx, maxy);
        }
    }
  sqlite3_free_table(results);
}

void MyMapView::DoFetchWMSextent(MapLayer * layer)
{
//
// retrieving the WMS Layer full extent
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  sqlite3 *sqlite = MapPanel->GetSqlite();

  char *prefix = (char *) malloc((layer->GetDbPrefix().Len() * 4) + 1);
  strcpy(prefix, layer->GetDbPrefix().ToUTF8());
  char *qprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  char *name = (char *) malloc((layer->GetName().Len() * 4) + 1);
  strcpy(name, layer->GetName().ToUTF8());
  char *sql = sqlite3_mprintf("SELECT g.minx, g.miny, g.maxx, g.maxy, d.minx, "
                              "d.miny, d.maxx, d.maxy, d.srs FROM \"%s\".wms_getmap AS w "
                              "LEFT JOIN \"%s\".wms_ref_sys AS g ON (w.id = g.parent_id AND g.srs = 'EPSG:4326') "
                              "LEFT JOIN \"%s\".wms_ref_sys AS d ON (w.id = d.parent_id AND d.is_default = 1) "
                              "WHERE Lower(w.layer_name) = Lower(%Q)",
                              qprefix, qprefix, qprefix, name);
  free(qprefix);
  free(name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
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
          double geo_minx = DBL_MAX;
          double geo_miny = DBL_MAX;
          double geo_maxx = -DBL_MAX;
          double geo_maxy = -DBL_MAX;
          double minx = DBL_MAX;
          double miny = DBL_MAX;
          double maxx = -DBL_MAX;
          double maxy = -DBL_MAX;
          const char *srs = NULL;
          value = results[(i * columns) + 0];
          if (value != NULL)
            geo_minx = atof(value);
          value = results[(i * columns) + 1];
          if (value != NULL)
            geo_miny = atof(value);
          value = results[(i * columns) + 2];
          if (value != NULL)
            geo_maxx = atof(value);
          value = results[(i * columns) + 3];
          if (value != NULL)
            geo_maxy = atof(value);
          value = results[(i * columns) + 4];
          if (value != NULL)
            minx = atof(value);
          value = results[(i * columns) + 5];
          if (value != NULL)
            miny = atof(value);
          value = results[(i * columns) + 6];
          if (value != NULL)
            maxx = atof(value);
          value = results[(i * columns) + 7];
          if (value != NULL)
            maxy = atof(value);
          srs = results[(i * columns) + 8];
          if (geo_minx == DBL_MAX || geo_miny == DBL_MAX || geo_maxx == -DBL_MAX
              || geo_maxy == -DBL_MAX)
            {
              geo_minx = DBL_MAX;
              geo_miny = DBL_MAX;
              geo_maxx = -DBL_MAX;
              geo_maxy = -DBL_MAX;
            }
          if (minx == DBL_MAX || miny == DBL_MAX || maxx == -DBL_MAX
              || maxy == -DBL_MAX)
            {
              minx = DBL_MAX;
              miny = DBL_MAX;
              maxx = -DBL_MAX;
              maxy = -DBL_MAX;
            }
          layer->SetGeoExtent(geo_minx, geo_miny, geo_maxx, geo_maxy);
          layer->SetNativeSRID(srs);
          layer->SetExtent(minx, miny, maxx, maxy);
        }
    }
  sqlite3_free_table(results);
}

void MyMapView::DoFetchWMSconfig(MapLayer * layer)
{
//
// retrieving the WMS Layer standard configuration
//
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  sqlite3 *sqlite = MapPanel->GetSqlite();
  WmsLayerSettings *config = layer->GetWmsConfig();

  if (config == NULL)
    return;

  char *prefix = (char *) malloc((layer->GetDbPrefix().Len() * 4) + 1);
  strcpy(prefix, layer->GetDbPrefix().ToUTF8());
  char *qprefix = gaiaDoubleQuotedSql(prefix);
  free(prefix);
  char *name = (char *) malloc((layer->GetName().Len() * 4) + 1);
  strcpy(name, layer->GetName().ToUTF8());
  char *sql =
    sqlite3_mprintf("SELECT version, srs, format, style, transparent, "
                    "flip_axes, bgcolor, is_cached, tiled, tile_width, tile_height "
                    "FROM \"%s\".wms_getmap WHERE Lower(layer_name) = Lower(%Q)",
                    qprefix, name);
  free(qprefix);
  free(name);
  int ret = sqlite3_get_table(sqlite, sql, &results,
                              &rows, &columns, &errMsg);
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
          if (value != NULL)
            config->SetVersion(value);
          value = results[(i * columns) + 1];
          if (value != NULL)
            config->SetRefSys(value);
          value = results[(i * columns) + 2];
          if (value != NULL)
            config->SetImageFormat(value);
          value = results[(i * columns) + 3];
          if (value != NULL)
            config->SetStyle(value);
          value = results[(i * columns) + 4];
          int opaque = 0;
          if (value != NULL)
            {
              if (atoi(value) == 0)
                opaque = 1;
              else
                opaque = 0;
            }
          config->SetOpaque(opaque);
          value = results[(i * columns) + 5];
          int flip = 0;
          if (value != NULL)
            {
              if (atoi(value) == 0)
                flip = 0;
              else
                flip = 1;
            }
          config->SetSwapXY(flip);
          value = results[(i * columns) + 6];
          if (value != NULL)
            {
              config->SetBgColor(value);
              if (opaque)
                config->SetBgColorEnabled(true);
            }
          value = results[(i * columns) + 7];
          int cached = 0;
          if (value != NULL)
            {
              if (atoi(value) == 0)
                cached = 0;
              else
                cached = 1;
            }
          config->SetCached(cached);
          value = results[(i * columns) + 8];
          int tiled = 0;
          if (value != NULL)
            {
              if (atoi(value) == 0)
                tiled = 0;
              else
                tiled = 1;
            }
          config->SetTiled(tiled);
          value = results[(i * columns) + 9];
          if (value != NULL)
            config->SetTileWidth(atoi(value));
          value = results[(i * columns) + 10];
          if (value != NULL)
            config->SetTileHeight(atoi(value));
        }
    }
  sqlite3_free_table(results);
}

void MyMapView::SetMapExtent(double minX, double minY, double maxX, double maxY)
{
//
// setting an externally defined Map Extent
//
  wxSize sz = GetClientSize();
  FrameWidth = sz.GetWidth();
  FrameHeight = sz.GetHeight();
  FrameCX = minX + ((maxX - minX) / 2.0);
  FrameCY = minY + ((maxY - minY) / 2.0);
  double extentX = maxX - minX;
  double extentY = maxY - minY;
  double x_ratio = extentX / (double) (FrameWidth);
  double y_ratio = extentY / (double) (FrameHeight);
  if (extentX / x_ratio <= FrameWidth && extentY / x_ratio <= FrameHeight)
    PixelRatio = x_ratio;
  else if (extentX / y_ratio <= FrameWidth && extentY / y_ratio <= FrameHeight)
    PixelRatio = y_ratio;
  else
    {
      if (x_ratio > y_ratio)
        PixelRatio = x_ratio;
      else
        PixelRatio = y_ratio;
    }
}

void MyMapView::ShowSelectedFeatures(MapFeaturesList * list, bool zoom_mode)
{
//
// Showing a list of selected Features on the Map Panel
//
  if (TimerMapBlink != NULL)
    {
      TimerMapBlink->Stop();
      delete TimerMapBlink;
    }
  if (SelectedFeatures != NULL)
    delete SelectedFeatures;
  SelectedFeatures = list;
  if (SelectedFeatures != NULL)
    {
      list->UpdateBBox();
      if (zoom_mode == true)
        {
          // unconditionally zooming the map at optimal scale
          double tic = 1000.0;
          if (MapPanel->IsGeographicSRID(GetMapSRID()) == true)
            tic = 0.005;
          double minx = list->GetMinX() - tic;
          double miny = list->GetMinY() - tic;
          double maxx = list->GetMaxX() + tic;
          double maxy = list->GetMaxY() + tic;
          FrameCX = minx + ((maxx - minx) / 2.0);
          FrameCY = miny + ((maxy - miny) / 2.0);
          double extentX = maxx - minx;
          double extentY = maxy - miny;
          double x_ratio = extentX / (double) (FrameWidth);
          double y_ratio = extentY / (double) (FrameHeight);
          if (extentX / x_ratio <= FrameWidth
              && extentY / x_ratio <= FrameHeight)
            PixelRatio = x_ratio;
          else if (extentX / y_ratio <= FrameWidth
                   && extentY / y_ratio <= FrameHeight)
            PixelRatio = y_ratio;
          else
            {
              if (x_ratio > y_ratio)
                PixelRatio = x_ratio;
              else
                PixelRatio = y_ratio;
            }
          PrepareMap();
      } else
        {
          if (TestSelectedFeaturesBBox() != true)
            {
              // 
              // we require moving the Map to a new position
              // because the Selected Features' BBOX isn't
              // fully covered by the current Map Frame
              //
              double tic = 1000.0;
              if (MapPanel->IsGeographicSRID(GetMapSRID()) == true)
                tic = 0.005;
              double minx = list->GetMinX() - tic;
              double miny = list->GetMinY() - tic;
              double maxx = list->GetMaxX() + tic;
              double maxy = list->GetMaxY() + tic;
              FrameCX = minx + ((maxx - minx) / 2.0);
              FrameCY = miny + ((maxy - miny) / 2.0);
              double extentX = (FrameMaxX - FrameMinX) / 2.0;
              double extentY = (FrameMaxY - FrameMinY) / 2.0;
              FrameMinX = FrameCX - (extentX / 2.0);
              FrameMaxX = FrameMinX + extentX;
              FrameMinY = FrameCY - (extentY / 2.0);
              FrameMaxY = FrameMinY + extentY;
              if (TestSelectedFeaturesBBox() != true)
                {
                  //
                  // the BBOX won't fit: we require to ZoomOut 
                  // so to let the Map Frame to cover all
                  // Selected Features
                  //
                  extentX = maxx - minx;
                  extentY = maxy - miny;
                  double x_ratio = extentX / (double) (FrameWidth);
                  double y_ratio = extentY / (double) (FrameHeight);
                  if (extentX / x_ratio <= FrameWidth
                      && extentY / x_ratio <= FrameHeight)
                    PixelRatio = x_ratio;
                  else if (extentX / y_ratio <= FrameWidth
                           && extentY / y_ratio <= FrameHeight)
                    PixelRatio = y_ratio;
                  else
                    {
                      if (x_ratio > y_ratio)
                        PixelRatio = x_ratio;
                      else
                        PixelRatio = y_ratio;
                    }
                }
            }
          PrepareMap();
        }
      MapBlinkCount = 0;
      TimerMapBlink = new wxTimer(this, ID_MAP_BLINK_TIMER);
      TimerMapBlink->Start(500, wxTIMER_ONE_SHOT);
    }

}

bool MyMapView::TestSelectedFeaturesBBox()
{
//
// testing if the Selected Features' BBOX fits into
// the current Map Frame
//
  if (SelectedFeatures->GetMinX() < FrameMinX)
    return false;
  if (SelectedFeatures->GetMinY() < FrameMinY)
    return false;
  if (SelectedFeatures->GetMaxX() > FrameMaxX)
    return false;
  if (SelectedFeatures->GetMaxY() > FrameMaxY)
    return false;
  return true;
}

void MyMapView::SetLoadedMapConfiguration(rl2MapConfigPtr config)
{
//
// replacing the current Loaded Map Configuration
//
  if (LoadedMapConfig != NULL)
    rl2_destroy_map_config(LoadedMapConfig);
  LoadedMapConfig = config;
}

bool MyMapView::MapConfigurationChanged()
{
//
// checking if the current Map Configuration changed
//
  bool ret = false;
  char *xml;
// printing the current  XML Map Config
  XmlMapConfigPrinter *xmlPrinter = new XmlMapConfigPrinter(MapPanel);
  xmlPrinter->InitializeFromParent();
  xml = xmlPrinter->GetXML();
// parsing the current XML Map Config
  rl2MapConfigPtr map_config = rl2_parse_map_config_xml((unsigned char *) xml);
  if (rl2_compare_map_configs(LoadedMapConfig, map_config) == 0)
    {
      ret = true;
      if (CurrentMapConfigName != NULL)
        free(CurrentMapConfigName);
      CurrentMapConfigName = NULL;
      if (map_config != NULL)
        {
          if (map_config->name != NULL)
            {
              int len = strlen(map_config->name);
              CurrentMapConfigName = (char *) malloc(len + 1);
              strcpy(CurrentMapConfigName, map_config->name);
            }
        }
    }
// memory clean-up
  rl2_destroy_map_config(map_config);
  delete xmlPrinter;
  if (CurrentMapConfigXML != NULL)
    sqlite3_free(CurrentMapConfigXML);
  CurrentMapConfigXML = xml;
  return ret;
}

void MyMapView::SaveMapConfiguration()
{
//
// saving the current Map Config on user request
//
  int valid;
  const char *sql;
  sqlite3_stmt *stmt;

  if (CurrentMapConfigName == NULL)
    return;

  wxString msg =
    wxT
    ("The Map Configuration contains unsaved changes\n\nDo you want to permanently update it ?");
  int ret = wxMessageBox(msg, wxT("spatialite_gui"),
                         wxYES_NO | wxICON_QUESTION, this);
  if (ret != wxYES)
    return;

  int id;
  bool reload = false;
  if (MapConfigDialog::CheckExistingMapConfiguration(MapPanel->GetSqlite(),
                                                     MapPanel->GetParent(),
                                                     CurrentMapConfigName, &id))
    {
      sql = "SELECT RL2_ReloadMapConfiguration(?, XB_Create(?, 1, 1))";
      reload = true;
  } else
    sql = "SELECT RL2_RegisterMapConfiguration(XB_Create(?, 1, 1))";
  ret =
    sqlite3_prepare_v2(MapPanel->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      wxMessageBox(wxT("RL2_RegisterMapConfiguration error"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  if (reload)
    {
      sqlite3_bind_int(stmt, 1, id);
      sqlite3_bind_blob(stmt, 2, CurrentMapConfigXML,
                        strlen(CurrentMapConfigXML), SQLITE_STATIC);
  } else
    sqlite3_bind_blob(stmt, 1, CurrentMapConfigXML, strlen(CurrentMapConfigXML),
                      SQLITE_STATIC);
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
    wxMessageBox(wxT("Current Map Configuration succesfully registered"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("RL2_RegisterMapConfiguration error"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
}

SingleLayerPainter::SingleLayerPainter(unsigned int width, unsigned int height,
                                       MapLayer * layer)
{
// ctor
  Locked = false;
  OnScreen = false;
  Retry = 0;
  Width = width;
  Height = height;
  Layer = layer;
  Next = NULL;
}

bool SingleLayerPainter::IsReady()
{
// testing if the Layer has been rendered
  if (Locked == true)
    return false;
  return Layer->IsReady();
}

MultiLayerPainter::MultiLayerPainter(MyMapView * map, unsigned int width,
                                     unsigned int height)
{
// ctor
  Map = map;
  Width = width;
  Height = height;
  Changed = false;
  Graphics =
    rl2_graph_create_context(Map->GetMapPanel()->
                             GetParent()->GetRL2PrivateData(), Width, Height);
  ResetGraphics();
  First = NULL;
  Last = NULL;
}

MultiLayerPainter::~MultiLayerPainter()
{
// dtor
  SingleLayerPainter *lyr = First;
  SingleLayerPainter *n_lyr;
  while (lyr != NULL)
    {
      n_lyr = lyr->GetNext();
      delete lyr;
      lyr = n_lyr;
    }
  if (Graphics != NULL)
    rl2_graph_destroy_context(Graphics);
}

void MultiLayerPainter::InsertLayer(MapLayer * layer)
{
// inserting a Layer into the Map Painter
  SingleLayerPainter *lyr = new SingleLayerPainter(Width, Height, layer);
  if (First == NULL)
    First = lyr;
  if (Last != NULL)
    Last->SetNext(lyr);
  Last = lyr;
}

void MultiLayerPainter::ResetGraphics()
{
// initializing a full transparent Map background
  rl2_graph_set_brush(Graphics, 255, 255, 255, 0);
  rl2_graph_draw_rectangle(Graphics, -1, -1, Width + 2, Height + 2);
}

bool MultiLayerPainter::IsReady()
{
// checking for completely painted Map
  SingleLayerPainter *lyr = First;
  while (lyr != NULL)
    {
      if (lyr->IsReady() == false && lyr->GetLayer()->IsVisible() == true)
        return false;
      lyr = lyr->GetNext();
    }
  return true;
}

bool MultiLayerPainter::IsExhausted()
{
// checking for an exhausted Map
  SingleLayerPainter *lyr = First;
  while (lyr != NULL)
    {
      if (lyr->IsReady() == false && lyr->GetLayer()->IsVisible() == true
          && lyr->GetRetryCount() < 5)
        return false;
      lyr = lyr->GetNext();
    }
  return true;
}

unsigned char *MultiLayerPainter::GetRGB()
{
// returning the composite RGB buffer
  unsigned char *rgb = NULL;
  if (Graphics == NULL)
    return NULL;
  rgb = rl2_graph_get_context_rgb_array(Graphics);
  return rgb;
}

unsigned char *MultiLayerPainter::GetAlpha()
{
// returning the composite Alpha buffer
  unsigned char *alpha = NULL;
  int half_transparent;
  if (Graphics == NULL)
    return NULL;
  alpha = rl2_graph_get_context_alpha_array(Graphics, &half_transparent);
  return alpha;
}

CachedFrame::CachedFrame(const void *priv_data)
{
// ctor
  PrivData = priv_data;
  Width = 0;
  Height = 0;
  MinX = DBL_MAX;
  MinY = DBL_MAX;
  MaxX = -DBL_MAX;
  MaxY = -DBL_MAX;
  Style = NULL;
  Graphics = NULL;
  Ok = false;
}

CachedFrame::~CachedFrame()
{
// dtor
  if (Style != NULL)
    free(Style);
  if (Graphics != NULL)
    rl2_graph_destroy_context(Graphics);
}

bool CachedFrame::IsValid(int width, int height, double minx, double miny,
                          double maxx, double maxy, const char *style)
{
//
// testing if the cached frame is still valid
//
  if (Width != width)
    return false;
  if (Height != height)
    return false;
  if (MinX != minx)
    return false;
  if (MinY != miny)
    return false;
  if (MaxX != maxx)
    return false;
  if (MaxY != maxy)
    return false;
  if (Graphics == NULL)
    return false;
  if (Style == NULL && style != NULL)
    return false;
  if (Style != NULL && style == NULL)
    return false;
  if (Style != NULL && style != NULL)
    {
      if (strcasecmp(Style, style) != 0)
        return false;
    }
  return true;
}

void CachedFrame::Reset(int width, int height, double minx, double miny,
                        double maxx, double maxy, const char *style)
{
//
// resetting the cached frame 
//
  Width = width;
  Height = height;
  MinX = minx;
  MinY = miny;
  MaxX = maxx;
  MaxY = maxy;
  if (Style != NULL)
    free(Style);
  if (style == NULL)
    Style = NULL;
  else
    {
      int len = strlen(style);
      Style = (char *) malloc(len + 1);
      strcpy(Style, style);
    }
  if (Graphics != NULL)
    rl2_graph_destroy_context(Graphics);
  Graphics = rl2_graph_create_context(PrivData, Width, Height);
  Ok = false;
}

void CachedFrame::Invalidate()
{
//
// Invalidating the cached frame 
//
  if (Graphics != NULL)
    {
      rl2_graph_destroy_context(Graphics);
      Graphics = rl2_graph_create_context(PrivData, Width, Height);
    }
  Ok = false;
}

void MapViewPaintParams::PrepareThreadsPool(int count)
{
// setting up a pool of threads
  ThreadsPoolCleanUp();
  if (count > 1)
    {
      UnusedThreads = new bool[count];
      for (int i = 0; i < count; i++)
        *(UnusedThreads + i) = true;
      NumThreads = count;
    }
}

int MapViewPaintParams::FindUnusedThread()
{
// searching an unused Thread (if any)
  if (UnusedThreads == NULL || NumThreads <= 1)
    return -1;
  for (int i = 0; i < NumThreads; i++)
    {
      if (*(UnusedThreads + i) == true)
        {
          // locking the Thread slot
          *(UnusedThreads + i) = false;
          return i;
        }
    }
  return -1;
}

void MapViewPaintParams::ReleaseThread(int index)
{
// releasing a Thread slot
  if (UnusedThreads == NULL)
    return;
  if (index >= 0 && index < NumThreads)
    {
      // unlocking the Thread slot
      *(UnusedThreads + index) = true;
    }
}

MapFeaturesList::~MapFeaturesList()
{
// dtor
  MapFeatureGeom *p;
  MapFeatureGeom *pN;
  p = First;
  while (p != NULL)
    {
      pN = p->GetNext();
      delete p;
      p = pN;
    }
}

void MapFeaturesList::Add(gaiaGeomCollPtr geom)
{
//
// inserting a Map Feature's Geometry into the list
//
  MapFeatureGeom *ptr = new MapFeatureGeom(geom);
  if (First == NULL)
    First = ptr;
  if (Last != NULL)
    Last->SetNext(ptr);
  Last = ptr;
}

void MapFeaturesList::UpdateBBox()
{
//
// computing the overall Bounding Box
//
  MinX = DBL_MAX;
  MinY = DBL_MAX;
  MaxX = -DBL_MAX;
  MaxY = -DBL_MAX;
  MapFeatureGeom *p;
  p = First;
  while (p != NULL)
    {
      gaiaGeomCollPtr geom = p->GetGeometry();
      if (geom == NULL)
        {
          p = p->GetNext();
          continue;
        }
      if (geom->MinX < MinX)
        MinX = geom->MinX;
      if (geom->MinY < MinY)
        MinY = geom->MinY;
      if (geom->MaxX > MaxX)
        MaxX = geom->MaxX;
      if (geom->MaxY > MaxY)
        MaxY = geom->MaxY;
      p = p->GetNext();
    }
}
