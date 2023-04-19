/*
/ Raster.cpp
/ various dialog classes supporting Raster datasources
/
/ version 1.8, 2015 March 10
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2015  Alessandro Furieri
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
#include "wx/tokenzr.h"

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

bool CreateRasterCoverageDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  SampleType = RL2_SAMPLE_UINT8;
  PixelType = RL2_PIXEL_RGB;
  NumBands = 3;
  RedBand = -1;
  GreenBand = -1;
  BlueBand = -1;
  NIRband = -1;
  AutoNDVI = false;
  Compression = RL2_COMPRESSION_NONE;
  Quality = 100;
  TileWidth = 512;
  TileHeight = 512;
  NotGeoreferenced = false;
  SRID = -1;
  HorzResolution = 1.0;
  VertResolution = 1.0;
  StrictResolution = false;
  MixedResolutions = false;
  InputPaths = true;
  MD5 = true;
  Summary = true;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Creating a new Raster Coverage"))
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

void CreateRasterCoverageDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *headSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(headSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  headSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, ID_CVG_NAME, wxT(""),
                                       wxDefaultPosition, wxSize(600, 22));
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  headSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_CVG_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  headSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_CVG_ABSTRACT, wxT(""),
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: main attributes
  wxBoxSizer *typeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(typeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// fourth row, column #1: Pixel Type
  wxString pixel[6];
  pixel[0] = wxT("&Monochrome");
  pixel[1] = wxT("&Palette");
  pixel[2] = wxT("&Grayscale");
  pixel[3] = wxT("&RGB");
  pixel[4] = wxT("&MultiBand");
  pixel[5] = wxT("&DataGrid");
  wxRadioBox *pixelBox = new wxRadioBox(this, ID_CVG_PIXEL,
                                        wxT("&Pixel Type"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 6,
                                        pixel, 1,
                                        wxRA_SPECIFY_COLS);
  typeSizer->Add(pixelBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  pixelBox->SetSelection(3);
// fourth row, column #2: Sample Type
  wxString sample[11];
  sample[0] = wxT("&1-BIT");
  sample[1] = wxT("&2-BIT");
  sample[2] = wxT("&4-BIT");
  sample[3] = wxT("INT8");
  sample[4] = wxT("UINT8");
  sample[5] = wxT("INT16");
  sample[6] = wxT("UINT16");
  sample[7] = wxT("INT32");
  sample[8] = wxT("UINT32");
  sample[9] = wxT("FLOAT");
  sample[10] = wxT("DOUBLE");
  wxRadioBox *sampleBox = new wxRadioBox(this, ID_CVG_SAMPLE,
                                         wxT("&Sample Type"),
                                         wxDefaultPosition,
                                         wxDefaultSize, 11,
                                         sample, 2,
                                         wxRA_SPECIFY_COLS);
  typeSizer->Add(sampleBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  sampleBox->SetSelection(4);
  sampleBox->Enable(0, false);
  sampleBox->Enable(1, false);
  sampleBox->Enable(2, false);
  sampleBox->Enable(3, false);
  sampleBox->Enable(5, false);
  sampleBox->Enable(7, false);
  sampleBox->Enable(8, false);
  sampleBox->Enable(9, false);
  sampleBox->Enable(10, false);
// fourth row, column #3a: NumBands / Compression Quality
  wxBoxSizer *bndqtySizer = new wxBoxSizer(wxVERTICAL);
  typeSizer->Add(bndqtySizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *bndBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  bndqtySizer->Add(bndBoxSizer, 0, wxALIGN_TOP | wxALL, 0);
  wxStaticBox *bndBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Bands"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *bndSizer = new wxStaticBoxSizer(bndBox, wxVERTICAL);
  bndBoxSizer->Add(bndSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxSpinCtrl *bandCtrl = new wxSpinCtrl(this, ID_CVG_BANDS, wxEmptyString,
                                        wxDefaultPosition, wxSize(60, 20),
                                        wxSP_ARROW_KEYS,
                                        1, 255, 3);
  bandCtrl->Enable(false);
  bndSizer->Add(bandCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row, column #3b: Compression Quality
  bndqtySizer->AddSpacer(30);
  wxBoxSizer *qtyBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  bndqtySizer->Add(qtyBoxSizer, 0, wxALIGN_BOTTOM | wxALL, 0);
  wxStaticBox *qtyBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Quality"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *qtySizer = new wxStaticBoxSizer(qtyBox, wxVERTICAL);
  qtyBoxSizer->Add(qtySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxSpinCtrl *qualityCtrl = new wxSpinCtrl(this, ID_CVG_QUALITY, wxEmptyString,
                                           wxDefaultPosition, wxSize(60, 20),
                                           wxSP_ARROW_KEYS,
                                           1, 100, 100);
  qualityCtrl->Enable(false);
  qtySizer->Add(qualityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row, column #4: Compression
  wxString compression[16];
  compression[0] = wxT("&Deflate [Zip]");
  compression[1] = wxT("&LZ4 [fast lossless]");
  compression[2] = wxT("&ZSTD [Zstandard]");
  compression[3] = wxT("&LZMA [7zip]");
  compression[4] = wxT("&PNG");
  compression[5] = wxT("&JPEG");
  compression[6] = wxT("&WebP (lossy)");
  compression[7] = wxT("&WebP (lossless)");
  compression[8] = wxT("&Jpeg2000 (lossy)");
  compression[9] = wxT("&Jpeg2000 (lossless)");
  compression[10] = wxT("&CCITT FAX4");
  compression[11] = wxT("&Deflate-NoDelta");
  compression[12] = wxT("&LZ4-NoDelta");
  compression[13] = wxT("&ZSTD-NoDelta");
  compression[14] = wxT("LZMA-NoDelta");
  compression[15] = wxT("&None");
  wxRadioBox *compressionBox = new wxRadioBox(this, ID_CVG_COMPRESSION,
                                              wxT("&Compression Type"),
                                              wxDefaultPosition,
                                              wxDefaultSize, 16,
                                              compression, 3,
                                              wxRA_SPECIFY_COLS);
  typeSizer->Add(compressionBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  compressionBox->SetSelection(15);
  compressionBox->Enable(10, false);
// fifth row: default bands and AutoNDVI
  int spacer_sz = 30;
  wxBoxSizer *defBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(defBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *defBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Default bands selection"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *defSizer = new wxStaticBoxSizer(defBox, wxVERTICAL);
  defBoxSizer->Add(defSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *defbdsSizer = new wxBoxSizer(wxHORIZONTAL);
  defSizer->Add(defbdsSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *redLabel = new wxStaticText(this, wxID_STATIC,
                                            wxT("Red:"));
  defbdsSizer->Add(redLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxSpinCtrl *redCtrl = new wxSpinCtrl(this, ID_CVG_RED, wxEmptyString,
                                       wxDefaultPosition, wxSize(60, 20),
                                       wxSP_ARROW_KEYS, -1, -1);
  RedBand = -1;
  redCtrl->SetValue(RedBand);
  redCtrl->Enable(false);
  defbdsSizer->Add(redCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  defbdsSizer->AddSpacer(spacer_sz);
  wxStaticText *greenLabel = new wxStaticText(this, wxID_STATIC,
                                              wxT("Green:"));
  defbdsSizer->Add(greenLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxSpinCtrl *greenCtrl = new wxSpinCtrl(this, ID_CVG_GREEN, wxEmptyString,
                                         wxDefaultPosition, wxSize(60, 20),
                                         wxSP_ARROW_KEYS, -1, -1);
  GreenBand = -1;
  greenCtrl->SetValue(GreenBand);
  greenCtrl->Enable(false);
  defbdsSizer->Add(greenCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  defbdsSizer->AddSpacer(spacer_sz);
  wxStaticText *blueLabel = new wxStaticText(this, wxID_STATIC,
                                             wxT("Blue:"));
  defbdsSizer->Add(blueLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxSpinCtrl *blueCtrl = new wxSpinCtrl(this, ID_CVG_BLUE, wxEmptyString,
                                        wxDefaultPosition, wxSize(60, 20),
                                        wxSP_ARROW_KEYS, -1, -1);
  BlueBand = -1;
  blueCtrl->SetValue(BlueBand);
  blueCtrl->Enable(false);
  defbdsSizer->Add(blueCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  defbdsSizer->AddSpacer(spacer_sz);
  wxStaticText *nirLabel = new wxStaticText(this, wxID_STATIC,
                                            wxT("NIR:"));
  defbdsSizer->Add(nirLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxSpinCtrl *nirCtrl = new wxSpinCtrl(this, ID_CVG_NIR, wxEmptyString,
                                       wxDefaultPosition, wxSize(60, 20),
                                       wxSP_ARROW_KEYS, -1, -1);
  NIRband = -1;
  nirCtrl->SetValue(NIRband);
  nirCtrl->Enable(false);
  defbdsSizer->Add(nirCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  defbdsSizer->AddSpacer(spacer_sz);
  wxCheckBox *autoNDVIctrl = new wxCheckBox(this, ID_CVG_AUTO_NDVI,
                                            wxT("Auto NDVI"),
                                            wxDefaultPosition, wxDefaultSize);
  autoNDVIctrl->SetValue(false);
  autoNDVIctrl->Enable(false);
  defbdsSizer->Add(autoNDVIctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
// fifth row: NO-DATA value Name
  wxBoxSizer *ndBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(ndBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *ndBox = new wxStaticBox(this, wxID_STATIC,
                                       wxT("NO-DATA Pixel"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  wxBoxSizer *ndSizer = new wxStaticBoxSizer(ndBox, wxVERTICAL);
  ndBoxSizer->Add(ndSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *nodataSizer = new wxBoxSizer(wxHORIZONTAL);
  ndSizer->Add(nodataSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *nodataCtrl = new wxTextCtrl(this, ID_CVG_NODATA, wxT("NONE"),
                                          wxDefaultPosition, wxSize(200, 22));
  nodataSizer->Add(nodataCtrl, 0, wxALIGN_RIGHT | wxALL, 1);
  wxStaticText *nodataLabel = new wxStaticText(this, wxID_STATIC,
                                               wxT
                                               ("  (list of comma-separated values) e.g.: "));
  nodataSizer->Add(nodataLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxStaticText *nodata2Label =
    new wxStaticText(this, wxID_STATIC, wxT("255,255,255"));
  nodata2Label->SetForegroundColour(wxColour(255, 0, 0));
  nodataSizer->Add(nodata2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);


  wxStaticText *nodata3Label =
    new wxStaticText(this, wxID_STATIC, wxT("  or  "));
  nodataSizer->Add(nodata3Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxStaticText *nodata4Label = new wxStaticText(this, wxID_STATIC, wxT("NONE"));
  nodata4Label->SetForegroundColour(wxColour(255, 0, 0));
  nodataSizer->Add(nodata4Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
// sixth row: other attributes
  wxBoxSizer *mixSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(mixSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// sixth row, column #1: Tile Size
  wxBoxSizer *tileBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  mixSizer->Add(tileBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *tileBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Tile Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *tileSizer = new wxStaticBoxSizer(tileBox, wxVERTICAL);
  tileBoxSizer->Add(tileSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *tileWSizer = new wxBoxSizer(wxHORIZONTAL);
  tileSizer->Add(tileWSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticText *tileWLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Width:"));
  tileWSizer->Add(tileWLabel, 0, wxALIGN_RIGHT | wxALL, 5);
  wxSpinCtrl *widthCtrl = new wxSpinCtrl(this, ID_CVG_WIDTH, wxEmptyString,
                                         wxDefaultPosition, wxSize(60, 20),
                                         wxSP_ARROW_KEYS,
                                         256, 1024, 512);
  tileWSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *tileXSizer = new wxBoxSizer(wxHORIZONTAL);
  tileSizer->Add(tileXSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxCheckBox *squareTileCtrl = new wxCheckBox(this, ID_CVG_SQTILE,
                                              wxT("square tile"),
                                              wxDefaultPosition, wxDefaultSize);
  squareTileCtrl->SetValue(true);
  tileXSizer->Add(squareTileCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *tileHSizer = new wxBoxSizer(wxHORIZONTAL);
  tileSizer->Add(tileHSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxStaticText *tileHLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Height:"));
  tileHSizer->Add(tileHLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *heightCtrl = new wxSpinCtrl(this, ID_CVG_HEIGHT, wxEmptyString,
                                          wxDefaultPosition, wxSize(60, 20),
                                          wxSP_ARROW_KEYS,
                                          256, 1024, 512);
  heightCtrl->Enable(false);
  tileHSizer->Add(heightCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// sixth row, column #2: SRID
  wxBoxSizer *sridQueryableSizer = new wxBoxSizer(wxVERTICAL);
  mixSizer->Add(sridQueryableSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *sridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  sridQueryableSizer->Add(sridBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *sridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Georeferencing"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *geoSizer = new wxStaticBoxSizer(sridBox, wxVERTICAL);
  sridBoxSizer->Add(geoSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *norefSizer = new wxBoxSizer(wxHORIZONTAL);
  geoSizer->Add(norefSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxCheckBox *norefCtrl = new wxCheckBox(this, ID_CVG_NOREF,
                                         wxT("none"),
                                         wxDefaultPosition, wxDefaultSize);
  norefCtrl->SetValue(false);
  norefSizer->Add(norefCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *sridSizer = new wxBoxSizer(wxHORIZONTAL);
  geoSizer->Add(sridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticText *sridLabel = new wxStaticText(this, wxID_STATIC, wxT("&SRID:"));
  sridSizer->Add(sridLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  int srid = 0;
  wxSpinCtrl *sridCtrl = new wxSpinCtrl(this, ID_CVG_SRID, wxEmptyString,
                                        wxDefaultPosition, wxSize(80, 20),
                                        wxSP_ARROW_KEYS,
                                        -1, 1000000, srid);
  sridSizer->Add(sridCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// sixth row, column #2/bis: queryable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  sridQueryableSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_CVG_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  queryableBox->SetSelection(1);
// sixth row, column #3: Pixel Resolution
  wxBoxSizer *resBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  mixSizer->Add(resBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *resBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Pixel Resolution"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *resSizer = new wxStaticBoxSizer(resBox, wxVERTICAL);
  resBoxSizer->Add(resSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *resHSizer = new wxBoxSizer(wxHORIZONTAL);
  resSizer->Add(resHSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticText *resHLabel = new wxStaticText(this, wxID_STATIC, wxT("&Horz:"));
  resHSizer->Add(resHLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *horzCtrl = new wxTextCtrl(this, ID_CVG_HORZ_RES, wxT("1.0"),
                                        wxDefaultPosition, wxSize(80, 22));
  resHSizer->Add(horzCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *resXSizer = new wxBoxSizer(wxHORIZONTAL);
  resSizer->Add(resXSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxCheckBox *sameResCtrl = new wxCheckBox(this, ID_CVG_SAME_RES,
                                           wxT("same resolution"),
                                           wxDefaultPosition, wxDefaultSize);
  sameResCtrl->SetValue(true);
  resXSizer->Add(sameResCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *resVSizer = new wxBoxSizer(wxHORIZONTAL);
  resSizer->Add(resVSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxStaticText *resVLabel = new wxStaticText(this, wxID_STATIC, wxT("&Vert:"));
  resVSizer->Add(resVLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *vertCtrl = new wxTextCtrl(this, ID_CVG_VERT_RES, wxT("1.0"),
                                        wxDefaultPosition, wxSize(80, 22));
  vertCtrl->Enable(false);
  resVSizer->Add(vertCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// sixth row, column #4: Policies
  mixSizer->AddSpacer(50);
  wxBoxSizer *polBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  mixSizer->Add(polBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *polBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Policies"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *polSizer = new wxStaticBoxSizer(polBox, wxVERTICAL);
  polBoxSizer->Add(polSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxCheckBox *strictCtrl = new wxCheckBox(this, ID_CVG_STRICT_RES,
                                          wxT("Strict Resolution"),
                                          wxDefaultPosition, wxDefaultSize);
  strictCtrl->SetValue(false);
  polSizer->Add(strictCtrl, 0, wxALIGN_LEFT | wxALL, 2);
  wxCheckBox *mixedCtrl = new wxCheckBox(this, ID_CVG_MIXED_RES,
                                         wxT("Mixed Resolutions"),
                                         wxDefaultPosition, wxDefaultSize);
  mixedCtrl->SetValue(false);
  polSizer->Add(mixedCtrl, 0, wxALIGN_LEFT | wxALL, 2);
  wxCheckBox *pathsCtrl = new wxCheckBox(this, ID_CVG_PATHS,
                                         wxT("Input Paths"),
                                         wxDefaultPosition, wxDefaultSize);
  pathsCtrl->SetValue(true);
  polSizer->Add(pathsCtrl, 0, wxALIGN_LEFT | wxALL, 2);
  wxCheckBox *md5Ctrl = new wxCheckBox(this, ID_CVG_MD5,
                                       wxT("MD5 Checksum"),
                                       wxDefaultPosition, wxDefaultSize);
  md5Ctrl->SetValue(true);
  polSizer->Add(md5Ctrl, 0, wxALIGN_LEFT | wxALL, 2);
  wxCheckBox *summaryCtrl = new wxCheckBox(this, ID_CVG_SUMMARY,
                                           wxT("XML Summary"),
                                           wxDefaultPosition, wxDefaultSize);
  summaryCtrl->SetValue(true);
  polSizer->Add(summaryCtrl, 0, wxALIGN_LEFT | wxALL, 2);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&OK"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & CreateRasterCoverageDialog::OnOk);
  Connect(ID_CVG_PIXEL, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & CreateRasterCoverageDialog::OnPixelChanged);
  Connect(ID_CVG_SAMPLE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnSampleChanged);
  Connect(ID_CVG_COMPRESSION, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnCompressionChanged);
  Connect(ID_CVG_BANDS, wxEVT_COMMAND_SPINCTRL_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnNumBandsChanged);
  Connect(ID_CVG_BANDS, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnNumBandsChanged);
  Connect(ID_CVG_RED, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnRedBandChanged);
  Connect(ID_CVG_GREEN, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnGreenBandChanged);
  Connect(ID_CVG_BLUE, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnBlueBandChanged);
  Connect(ID_CVG_NIR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnNIRbandChanged);
  Connect(ID_CVG_AUTO_NDVI, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnAutoNDVIchanged);
  Connect(ID_CVG_SQTILE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnSquareTileChanged);
  Connect(ID_CVG_WIDTH, wxEVT_COMMAND_SPINCTRL_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnTileWidthChanged);
  Connect(ID_CVG_WIDTH, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnTileWidthChanged);
  Connect(ID_CVG_HEIGHT, wxEVT_COMMAND_SPINCTRL_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnTileHeightChanged);
  Connect(ID_CVG_HEIGHT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnTileHeightChanged);
  Connect(ID_CVG_NOREF, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnNoGeorefChanged);
  Connect(ID_CVG_SAME_RES, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnSameResChanged);
  Connect(ID_CVG_HORZ_RES, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnHorzResChanged);
  Connect(ID_CVG_STRICT_RES, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnStrictChanged);
  Connect(ID_CVG_MIXED_RES, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & CreateRasterCoverageDialog::OnMixedChanged);
  Connect(ID_CVG_PATHS, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & CreateRasterCoverageDialog::OnPathsChanged);
  Connect(ID_CVG_MD5, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & CreateRasterCoverageDialog::OnMD5Changed);
  Connect(ID_CVG_SUMMARY, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          CreateRasterCoverageDialog::OnSummaryChanged);
}

void CreateRasterCoverageDialog::
OnPixelChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Pixel Type selection changed
//
  bool is_8_bit = false;
  bool is_16_bit = false;
  unsigned char num_bands;
  wxRadioBox *pixelCtrl = (wxRadioBox *) FindWindow(ID_CVG_PIXEL);
  wxRadioBox *sampleCtrl = (wxRadioBox *) FindWindow(ID_CVG_SAMPLE);
  wxRadioBox *compressionCtrl = (wxRadioBox *) FindWindow(ID_CVG_COMPRESSION);
  wxSpinCtrl *bandCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_BANDS);
  wxSpinCtrl *redBand = (wxSpinCtrl *) FindWindow(ID_CVG_RED);
  wxSpinCtrl *greenBand = (wxSpinCtrl *) FindWindow(ID_CVG_GREEN);
  wxSpinCtrl *blueBand = (wxSpinCtrl *) FindWindow(ID_CVG_BLUE);
  wxSpinCtrl *nirBand = (wxSpinCtrl *) FindWindow(ID_CVG_NIR);
  wxCheckBox *autoNDVIctrl = (wxCheckBox *) FindWindow(ID_CVG_AUTO_NDVI);
  switch (pixelCtrl->GetSelection())
    {
      case 0:
        PixelType = RL2_PIXEL_MONOCHROME;
        break;
      case 1:
        PixelType = RL2_PIXEL_PALETTE;
        break;
      case 2:
        PixelType = RL2_PIXEL_GRAYSCALE;
        break;
      case 3:
        PixelType = RL2_PIXEL_RGB;
        break;
      case 4:
        PixelType = RL2_PIXEL_MULTIBAND;
        break;
      case 5:
        PixelType = RL2_PIXEL_DATAGRID;
        break;
    };
  if (sampleCtrl->GetSelection() == 4)
    is_8_bit = true;
  if (sampleCtrl->GetSelection() == 6)
    is_16_bit = true;
  num_bands = bandCtrl->GetValue();
  switch (PixelType)
    {
      case RL2_PIXEL_MONOCHROME:
        sampleCtrl->Enable(0, true);
        sampleCtrl->Enable(1, false);
        sampleCtrl->Enable(2, false);
        sampleCtrl->Enable(3, false);
        sampleCtrl->Enable(4, false);
        sampleCtrl->Enable(5, false);
        sampleCtrl->Enable(6, false);
        sampleCtrl->Enable(7, false);
        sampleCtrl->Enable(8, false);
        sampleCtrl->Enable(9, false);
        sampleCtrl->Enable(10, false);
        SampleType = RL2_SAMPLE_1_BIT;
        sampleCtrl->SetSelection(0);
        bandCtrl->SetValue(1);
        bandCtrl->Enable(false);
        RedBand = -1;
        GreenBand = -1;
        BlueBand = -1;
        NIRband = -1;
        AutoNDVI = false;
        redBand->SetValue(-1);
        redBand->Enable(false);
        greenBand->SetValue(-1);
        greenBand->Enable(false);
        blueBand->SetValue(-1);
        blueBand->Enable(false);
        nirBand->SetValue(-1);
        nirBand->Enable(false);
        autoNDVIctrl->SetValue(false);
        autoNDVIctrl->Enable(false);
        compressionCtrl->Enable(0, false);
        compressionCtrl->Enable(1, false);
        compressionCtrl->Enable(2, false);
        compressionCtrl->Enable(3, false);
        compressionCtrl->Enable(4, true);
        compressionCtrl->Enable(5, false);
        compressionCtrl->Enable(6, false);
        compressionCtrl->Enable(7, false);
        compressionCtrl->Enable(8, false);
        compressionCtrl->Enable(9, false);
        compressionCtrl->Enable(10, true);
        compressionCtrl->Enable(11, false);
        compressionCtrl->Enable(12, false);
        compressionCtrl->Enable(13, false);
        compressionCtrl->Enable(14, false);
        compressionCtrl->Enable(15, true);
        if (compressionCtrl->GetSelection() == 15
            || compressionCtrl->GetSelection() == 4
            || compressionCtrl->GetSelection() == 10)
          ;
        else
          {
            Compression = RL2_COMPRESSION_CCITTFAX4;
            compressionCtrl->SetSelection(10);
          }
        break;
      case RL2_PIXEL_PALETTE:
        sampleCtrl->Enable(0, true);
        sampleCtrl->Enable(1, true);
        sampleCtrl->Enable(2, true);
        sampleCtrl->Enable(3, false);
        sampleCtrl->Enable(4, true);
        sampleCtrl->Enable(5, false);
        sampleCtrl->Enable(6, false);
        sampleCtrl->Enable(7, false);
        sampleCtrl->Enable(8, false);
        sampleCtrl->Enable(9, false);
        sampleCtrl->Enable(10, false);
        if (sampleCtrl->GetSelection() == 0 || sampleCtrl->GetSelection() == 1
            || sampleCtrl->GetSelection() == 2
            || sampleCtrl->GetSelection() == 4)
          ;
        else
          {
            SampleType = RL2_SAMPLE_UINT8;
            sampleCtrl->SetSelection(4);
          }
        bandCtrl->SetValue(1);
        bandCtrl->Enable(false);
        RedBand = -1;
        GreenBand = -1;
        BlueBand = -1;
        NIRband = -1;
        AutoNDVI = false;
        redBand->SetValue(-1);
        redBand->Enable(false);
        greenBand->SetValue(-1);
        greenBand->Enable(false);
        blueBand->SetValue(-1);
        blueBand->Enable(false);
        nirBand->SetValue(-1);
        nirBand->Enable(false);
        autoNDVIctrl->SetValue(false);
        autoNDVIctrl->Enable(false);
        compressionCtrl->Enable(0, false);
        compressionCtrl->Enable(1, false);
        compressionCtrl->Enable(2, false);
        compressionCtrl->Enable(3, false);
        compressionCtrl->Enable(4, true);
        compressionCtrl->Enable(5, false);
        compressionCtrl->Enable(6, false);
        compressionCtrl->Enable(7, false);
        compressionCtrl->Enable(8, false);
        compressionCtrl->Enable(9, false);
        compressionCtrl->Enable(10, false);
        compressionCtrl->Enable(11, false);
        compressionCtrl->Enable(12, false);
        compressionCtrl->Enable(13, false);
        compressionCtrl->Enable(14, false);
        compressionCtrl->Enable(15, true);
        if (compressionCtrl->GetSelection() == 15
            || compressionCtrl->GetSelection() == 4)
          ;
        else
          {
            Compression = RL2_COMPRESSION_PNG;
            compressionCtrl->SetSelection(4);
          }
        break;
      case RL2_PIXEL_GRAYSCALE:
        sampleCtrl->Enable(0, false);
        sampleCtrl->Enable(1, true);
        sampleCtrl->Enable(2, true);
        sampleCtrl->Enable(3, false);
        sampleCtrl->Enable(4, true);
        sampleCtrl->Enable(5, false);
        sampleCtrl->Enable(6, false);
        sampleCtrl->Enable(7, false);
        sampleCtrl->Enable(8, false);
        sampleCtrl->Enable(9, false);
        sampleCtrl->Enable(10, false);
        if (sampleCtrl->GetSelection() == 1 || sampleCtrl->GetSelection() == 2
            || sampleCtrl->GetSelection() == 4)
          ;
        else
          {
            SampleType = RL2_SAMPLE_UINT8;
            sampleCtrl->SetSelection(4);
          }
        bandCtrl->SetValue(1);
        bandCtrl->Enable(false);
        RedBand = -1;
        GreenBand = -1;
        BlueBand = -1;
        NIRband = -1;
        AutoNDVI = false;
        redBand->SetValue(-1);
        redBand->Enable(false);
        greenBand->SetValue(-1);
        greenBand->Enable(false);
        blueBand->SetValue(-1);
        blueBand->Enable(false);
        nirBand->SetValue(-1);
        nirBand->Enable(false);
        autoNDVIctrl->SetValue(false);
        autoNDVIctrl->Enable(false);
        compressionCtrl->Enable(0, true);
        compressionCtrl->Enable(1, true);
        compressionCtrl->Enable(2, true);
        compressionCtrl->Enable(3, true);
        compressionCtrl->Enable(4, true);
        compressionCtrl->Enable(5, true);
        compressionCtrl->Enable(6, true);
        compressionCtrl->Enable(7, true);
        compressionCtrl->Enable(8, true);
        compressionCtrl->Enable(9, true);
        compressionCtrl->Enable(10, false);
        compressionCtrl->Enable(11, true);
        compressionCtrl->Enable(12, true);
        compressionCtrl->Enable(13, true);
        compressionCtrl->Enable(14, true);
        compressionCtrl->Enable(15, true);
        if (compressionCtrl->GetSelection() == 10)
          {
            Compression = RL2_COMPRESSION_PNG;
            compressionCtrl->SetSelection(4);
          }
        break;
      case RL2_PIXEL_RGB:
        sampleCtrl->Enable(0, false);
        sampleCtrl->Enable(1, false);
        sampleCtrl->Enable(2, false);
        sampleCtrl->Enable(3, false);
        sampleCtrl->Enable(4, true);
        sampleCtrl->Enable(5, false);
        sampleCtrl->Enable(6, true);
        sampleCtrl->Enable(7, false);
        sampleCtrl->Enable(8, false);
        sampleCtrl->Enable(9, false);
        sampleCtrl->Enable(10, false);
        if (sampleCtrl->GetSelection() == 4 || sampleCtrl->GetSelection() == 6)
          ;
        else
          {
            SampleType = RL2_SAMPLE_UINT8;
            sampleCtrl->SetSelection(4);
          }
        bandCtrl->SetValue(3);
        bandCtrl->Enable(false);
        RedBand = -1;
        GreenBand = -1;
        BlueBand = -1;
        NIRband = -1;
        AutoNDVI = false;
        redBand->SetValue(-1);
        redBand->Enable(false);
        greenBand->SetValue(-1);
        greenBand->Enable(false);
        blueBand->SetValue(-1);
        blueBand->Enable(false);
        nirBand->SetValue(-1);
        nirBand->Enable(false);
        autoNDVIctrl->SetValue(false);
        autoNDVIctrl->Enable(false);
        compressionCtrl->Enable(0, true);
        compressionCtrl->Enable(1, true);
        compressionCtrl->Enable(2, true);
        compressionCtrl->Enable(3, true);
        compressionCtrl->Enable(4, true);
        if (is_8_bit)
          {
            compressionCtrl->Enable(5, true);
            compressionCtrl->Enable(6, true);
            compressionCtrl->Enable(7, true);
        } else
          {
            compressionCtrl->Enable(5, false);
            compressionCtrl->Enable(6, false);
            compressionCtrl->Enable(7, false);
          }
        compressionCtrl->Enable(8, true);
        compressionCtrl->Enable(9, true);
        compressionCtrl->Enable(10, false);
        compressionCtrl->Enable(11, true);
        compressionCtrl->Enable(12, true);
        compressionCtrl->Enable(13, true);
        compressionCtrl->Enable(14, true);
        compressionCtrl->Enable(15, true);
        if (is_8_bit)
          {
            if (compressionCtrl->GetSelection() == 11)
              {
                Compression = RL2_COMPRESSION_PNG;
                compressionCtrl->SetSelection(4);
              }
        } else
          {
            if (compressionCtrl->GetSelection() == 5
                || compressionCtrl->GetSelection() == 6
                || compressionCtrl->GetSelection() == 7
                || compressionCtrl->GetSelection() == 10)
              {
                Compression = RL2_COMPRESSION_PNG;
                compressionCtrl->SetSelection(4);
              }
          }
        break;
      case RL2_PIXEL_MULTIBAND:
        sampleCtrl->Enable(0, false);
        sampleCtrl->Enable(1, false);
        sampleCtrl->Enable(2, false);
        sampleCtrl->Enable(3, false);
        sampleCtrl->Enable(4, true);
        sampleCtrl->Enable(5, false);
        sampleCtrl->Enable(6, true);
        sampleCtrl->Enable(7, false);
        sampleCtrl->Enable(8, false);
        sampleCtrl->Enable(9, false);
        sampleCtrl->Enable(10, false);
        if (sampleCtrl->GetSelection() == 4 || sampleCtrl->GetSelection() == 6)
          ;
        else
          {
            SampleType = RL2_SAMPLE_UINT8;
            sampleCtrl->SetSelection(4);
          }
        bandCtrl->Enable(true);
        if (num_bands == 3 || num_bands == 4)
          {
            compressionCtrl->Enable(0, true);
            compressionCtrl->Enable(1, true);
            compressionCtrl->Enable(2, true);
            compressionCtrl->Enable(3, true);
            compressionCtrl->Enable(4, true);
            compressionCtrl->Enable(5, false);
            if (is_8_bit)
              {
                compressionCtrl->Enable(6, true);
                compressionCtrl->Enable(7, true);
            } else
              {
                compressionCtrl->Enable(6, false);
                compressionCtrl->Enable(7, false);
              }
            compressionCtrl->Enable(8, true);
            compressionCtrl->Enable(9, true);
            compressionCtrl->Enable(10, false);
            compressionCtrl->Enable(11, true);
            compressionCtrl->Enable(12, true);
            compressionCtrl->Enable(13, true);
            compressionCtrl->Enable(14, true);
            compressionCtrl->Enable(15, true);
            if (is_8_bit)
              {
                if (compressionCtrl->GetSelection() == 5
                    || compressionCtrl->GetSelection() == 10)
                  {
                    Compression = RL2_COMPRESSION_PNG;
                    compressionCtrl->SetSelection(4);
                  }
            } else
              {
                if (compressionCtrl->GetSelection() == 5
                    || compressionCtrl->GetSelection() == 6
                    || compressionCtrl->GetSelection() == 7
                    || compressionCtrl->GetSelection() == 10)
                  {
                    Compression = RL2_COMPRESSION_PNG;
                    compressionCtrl->SetSelection(4);
                  }
              }
        } else
          {
            compressionCtrl->Enable(0, true);
            compressionCtrl->Enable(1, true);
            compressionCtrl->Enable(2, true);
            compressionCtrl->Enable(3, true);
            compressionCtrl->Enable(4, false);
            compressionCtrl->Enable(5, false);
            compressionCtrl->Enable(6, false);
            compressionCtrl->Enable(7, false);
            compressionCtrl->Enable(8, false);
            compressionCtrl->Enable(9, false);
            compressionCtrl->Enable(10, false);
            compressionCtrl->Enable(11, true);
            compressionCtrl->Enable(12, true);
            compressionCtrl->Enable(13, true);
            compressionCtrl->Enable(14, true);
            compressionCtrl->Enable(15, true);
            if (compressionCtrl->GetSelection() == 11
                || compressionCtrl->GetSelection() == 12
                || compressionCtrl->GetSelection() == 13
                || compressionCtrl->GetSelection() == 14
                || compressionCtrl->GetSelection() == 15
                || compressionCtrl->GetSelection() == 0
                || compressionCtrl->GetSelection() == 1
                || compressionCtrl->GetSelection() == 2
                || compressionCtrl->GetSelection() == 3)
              ;
            else
              {
                Compression = RL2_COMPRESSION_DEFLATE;
                compressionCtrl->SetSelection(0);
              }
          }
        redBand->SetRange(-1, num_bands - 1);
        redBand->SetValue(-1);
        RedBand = -1;
        redBand->Enable(true);
        greenBand->SetRange(-1, num_bands - 1);
        greenBand->SetValue(-1);
        GreenBand = -1;
        greenBand->Enable(true);
        blueBand->SetRange(-1, num_bands - 1);
        blueBand->SetValue(-1);
        BlueBand = -1;
        blueBand->Enable(true);
        nirBand->SetRange(-1, num_bands - 1);
        nirBand->SetValue(-1);
        NIRband = -1;
        nirBand->Enable(true);
        autoNDVIctrl->SetValue(false);
        autoNDVIctrl->Enable(true);
        break;
      case RL2_PIXEL_DATAGRID:
        sampleCtrl->Enable(0, false);
        sampleCtrl->Enable(1, false);
        sampleCtrl->Enable(2, false);
        sampleCtrl->Enable(3, true);
        sampleCtrl->Enable(4, true);
        sampleCtrl->Enable(5, true);
        sampleCtrl->Enable(6, true);
        sampleCtrl->Enable(7, true);
        sampleCtrl->Enable(8, true);
        sampleCtrl->Enable(9, true);
        sampleCtrl->Enable(10, true);
        if (sampleCtrl->GetSelection() == 0 || sampleCtrl->GetSelection() == 1
            || sampleCtrl->GetSelection() == 2)
          {
            SampleType = RL2_SAMPLE_DOUBLE;
            sampleCtrl->SetSelection(10);
          }
        bandCtrl->SetValue(1);
        bandCtrl->Enable(false);
        RedBand = -1;
        GreenBand = -1;
        BlueBand = -1;
        NIRband = -1;
        AutoNDVI = false;
        redBand->SetValue(-1);
        redBand->Enable(false);
        greenBand->SetValue(-1);
        greenBand->Enable(false);
        blueBand->SetValue(-1);
        blueBand->Enable(false);
        nirBand->SetValue(-1);
        nirBand->Enable(false);
        autoNDVIctrl->SetValue(false);
        autoNDVIctrl->Enable(false);
        if (is_8_bit || is_16_bit)
          {
            compressionCtrl->Enable(0, true);
            compressionCtrl->Enable(1, true);
            compressionCtrl->Enable(2, true);
            compressionCtrl->Enable(3, true);
            compressionCtrl->Enable(4, true);
            compressionCtrl->Enable(5, false);
            compressionCtrl->Enable(6, false);
            compressionCtrl->Enable(7, false);
            compressionCtrl->Enable(8, true);
            compressionCtrl->Enable(9, true);
            compressionCtrl->Enable(10, false);
            compressionCtrl->Enable(11, true);
            compressionCtrl->Enable(12, true);
            compressionCtrl->Enable(13, true);
            compressionCtrl->Enable(14, true);
            compressionCtrl->Enable(15, true);
            if (compressionCtrl->GetSelection() == 5
                || compressionCtrl->GetSelection() == 6
                || compressionCtrl->GetSelection() == 7
                || compressionCtrl->GetSelection() == 10)
              {
                Compression = RL2_COMPRESSION_DEFLATE;
                compressionCtrl->SetSelection(0);
              }
        } else
          {
            compressionCtrl->Enable(0, true);
            compressionCtrl->Enable(1, true);
            compressionCtrl->Enable(2, true);
            compressionCtrl->Enable(3, true);
            compressionCtrl->Enable(4, false);
            compressionCtrl->Enable(5, false);
            compressionCtrl->Enable(6, false);
            compressionCtrl->Enable(7, false);
            compressionCtrl->Enable(8, false);
            compressionCtrl->Enable(9, false);
            compressionCtrl->Enable(10, false);
            compressionCtrl->Enable(11, true);
            compressionCtrl->Enable(12, true);
            compressionCtrl->Enable(13, true);
            compressionCtrl->Enable(14, true);
            compressionCtrl->Enable(15, true);
            if (compressionCtrl->GetSelection() == 11
                || compressionCtrl->GetSelection() == 12
                || compressionCtrl->GetSelection() == 13
                || compressionCtrl->GetSelection() == 14
                || compressionCtrl->GetSelection() == 0
                || compressionCtrl->GetSelection() == 1
                || compressionCtrl->GetSelection() == 2
                || compressionCtrl->GetSelection() == 3
                || compressionCtrl->GetSelection() == 4)
              ;
            else
              {
                Compression = RL2_COMPRESSION_DEFLATE;
                compressionCtrl->SetSelection(0);
              }
          }
        break;
    };
}

void CreateRasterCoverageDialog::
OnSampleChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Sample Type selection changed
//
  bool is_8_bit = false;
  bool is_16_bit = false;
  unsigned char num_bands;
  wxRadioBox *pixelCtrl = (wxRadioBox *) FindWindow(ID_CVG_PIXEL);
  wxRadioBox *sampleCtrl = (wxRadioBox *) FindWindow(ID_CVG_SAMPLE);
  wxRadioBox *compressionCtrl = (wxRadioBox *) FindWindow(ID_CVG_COMPRESSION);
  wxSpinCtrl *bandCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_BANDS);
  switch (sampleCtrl->GetSelection())
    {
      case 0:
        SampleType = RL2_SAMPLE_1_BIT;
        break;
      case 1:
        SampleType = RL2_SAMPLE_2_BIT;
        break;
      case 2:
        SampleType = RL2_SAMPLE_4_BIT;
        break;
      case 3:
        SampleType = RL2_SAMPLE_INT8;
        break;
      case 4:
        SampleType = RL2_SAMPLE_UINT8;
        is_8_bit = true;
        break;
      case 5:
        SampleType = RL2_SAMPLE_INT16;
        break;
      case 6:
        SampleType = RL2_SAMPLE_UINT16;
        is_16_bit = true;
        break;
      case 7:
        SampleType = RL2_SAMPLE_INT32;
        break;
      case 8:
        SampleType = RL2_SAMPLE_UINT32;
        break;
      case 9:
        SampleType = RL2_SAMPLE_FLOAT;
        break;
      case 10:
        SampleType = RL2_SAMPLE_DOUBLE;
        break;
    };
  num_bands = bandCtrl->GetValue();
  if (pixelCtrl->GetSelection() == 3)
    {
      // RGB pixel
      if (is_8_bit)
        {
          compressionCtrl->Enable(5, true);
          compressionCtrl->Enable(6, true);
          compressionCtrl->Enable(7, true);
      } else
        {
          compressionCtrl->Enable(5, false);
          compressionCtrl->Enable(6, false);
          compressionCtrl->Enable(7, false);
        }
      if (is_8_bit)
        {
          if (compressionCtrl->GetSelection() == 10)
            {
              Compression = RL2_COMPRESSION_PNG;
              compressionCtrl->SetSelection(4);
            }
      } else
        {
          if (compressionCtrl->GetSelection() == 5
              || compressionCtrl->GetSelection() == 6
              || compressionCtrl->GetSelection() == 7
              || compressionCtrl->GetSelection() == 10)
            {
              Compression = RL2_COMPRESSION_PNG;
              compressionCtrl->SetSelection(4);
            }
        }
    }
  if (pixelCtrl->GetSelection() == 4)
    {
      // MULTIBAND pixel
      if (num_bands == 3 || num_bands == 4)
        {
          compressionCtrl->Enable(4, true);
          if (is_8_bit)
            {
              compressionCtrl->Enable(6, true);
              compressionCtrl->Enable(7, true);
          } else
            {
              compressionCtrl->Enable(6, false);
              compressionCtrl->Enable(7, false);
            }
          compressionCtrl->Enable(8, true);
          compressionCtrl->Enable(9, true);
          if (is_8_bit)
            {
              if (compressionCtrl->GetSelection() == 5
                  || compressionCtrl->GetSelection() == 11)
                {
                  Compression = RL2_COMPRESSION_PNG;
                  compressionCtrl->SetSelection(4);
                }
          } else
            {
              if (compressionCtrl->GetSelection() == 5
                  || compressionCtrl->GetSelection() == 6
                  || compressionCtrl->GetSelection() == 7
                  || compressionCtrl->GetSelection() == 10)
                {
                  Compression = RL2_COMPRESSION_PNG;
                  compressionCtrl->SetSelection(4);
                }
            }
      } else
        {
          compressionCtrl->Enable(4, false);
          compressionCtrl->Enable(5, false);
          compressionCtrl->Enable(6, false);
          compressionCtrl->Enable(7, false);
          compressionCtrl->Enable(8, false);
          compressionCtrl->Enable(9, false);
          compressionCtrl->Enable(10, false);
          if (compressionCtrl->GetSelection() == 11
              || compressionCtrl->GetSelection() == 12
              || compressionCtrl->GetSelection() == 13
              || compressionCtrl->GetSelection() == 14
              || compressionCtrl->GetSelection() == 15
              || compressionCtrl->GetSelection() == 0
              || compressionCtrl->GetSelection() == 1
              || compressionCtrl->GetSelection() == 2
              || compressionCtrl->GetSelection() == 3)
            ;
          else
            {
              Compression = RL2_COMPRESSION_DEFLATE;
              compressionCtrl->SetSelection(0);
            }
        }
    }
  if (pixelCtrl->GetSelection() == 5)
    {
      // DATAGRID pixel
      if (is_8_bit || is_16_bit)
        {
          compressionCtrl->Enable(0, true);
          compressionCtrl->Enable(1, true);
          compressionCtrl->Enable(2, true);
          compressionCtrl->Enable(3, true);
          compressionCtrl->Enable(4, true);
          compressionCtrl->Enable(5, false);
          compressionCtrl->Enable(6, false);
          compressionCtrl->Enable(7, false);
          compressionCtrl->Enable(8, true);
          compressionCtrl->Enable(9, true);
          compressionCtrl->Enable(10, false);
          compressionCtrl->Enable(11, true);
          compressionCtrl->Enable(12, true);
          compressionCtrl->Enable(13, true);
          compressionCtrl->Enable(14, true);
          compressionCtrl->Enable(15, true);
          if (compressionCtrl->GetSelection() == 5
              || compressionCtrl->GetSelection() == 6
              || compressionCtrl->GetSelection() == 7
              || compressionCtrl->GetSelection() == 10)
            {
              Compression = RL2_COMPRESSION_PNG;
              compressionCtrl->SetSelection(4);
            }
      } else
        {
          compressionCtrl->Enable(0, true);
          compressionCtrl->Enable(1, true);
          compressionCtrl->Enable(2, true);
          compressionCtrl->Enable(3, true);
          compressionCtrl->Enable(4, false);
          compressionCtrl->Enable(5, false);
          compressionCtrl->Enable(6, false);
          compressionCtrl->Enable(7, false);
          compressionCtrl->Enable(8, false);
          compressionCtrl->Enable(9, false);
          compressionCtrl->Enable(10, false);
          compressionCtrl->Enable(11, true);
          compressionCtrl->Enable(12, true);
          compressionCtrl->Enable(13, true);
          compressionCtrl->Enable(14, true);
          compressionCtrl->Enable(15, true);
          if (compressionCtrl->GetSelection() == 11
              || compressionCtrl->GetSelection() == 12
              || compressionCtrl->GetSelection() == 13
              || compressionCtrl->GetSelection() == 14
              || compressionCtrl->GetSelection() == 15
              || compressionCtrl->GetSelection() == 0
              || compressionCtrl->GetSelection() == 1
              || compressionCtrl->GetSelection() == 2
              || compressionCtrl->GetSelection() == 3)
            ;
          else
            {
              Compression = RL2_COMPRESSION_DEFLATE;
              compressionCtrl->SetSelection(0);
            }
        }
    }
}

void CreateRasterCoverageDialog::
OnCompressionChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Compression selection changed
//
  bool lossy = false;
  wxRadioBox *compressionCtrl = (wxRadioBox *) FindWindow(ID_CVG_COMPRESSION);
  wxSpinCtrl *qualityCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_QUALITY);
  switch (compressionCtrl->GetSelection())
    {
      case 0:
        Compression = RL2_COMPRESSION_DEFLATE;
        break;
      case 1:
        Compression = RL2_COMPRESSION_LZ4;
        break;
      case 2:
        Compression = RL2_COMPRESSION_ZSTD;
        break;
      case 3:
        Compression = RL2_COMPRESSION_LZMA;
        break;
      case 4:
        Compression = RL2_COMPRESSION_PNG;
        break;
      case 5:
        Compression = RL2_COMPRESSION_JPEG;
        lossy = true;
        break;
      case 6:
        Compression = RL2_COMPRESSION_LOSSY_WEBP;
        lossy = true;
        break;
      case 7:
        Compression = RL2_COMPRESSION_LOSSLESS_WEBP;
        break;
      case 8:
        Compression = RL2_COMPRESSION_LOSSY_JP2;
        lossy = true;
        break;
      case 9:
        Compression = RL2_COMPRESSION_LOSSLESS_JP2;
        break;
      case 10:
        Compression = RL2_COMPRESSION_CCITTFAX4;
        break;
      case 11:
        Compression = RL2_COMPRESSION_DEFLATE_NO;
        break;
      case 12:
        Compression = RL2_COMPRESSION_LZ4_NO;
        break;
      case 13:
        Compression = RL2_COMPRESSION_ZSTD_NO;
        break;
      case 14:
        Compression = RL2_COMPRESSION_NONE;
        break;
    };
  if (lossy)
    {
      if (qualityCtrl->GetValue() == 100)
        {
          if (Compression == RL2_COMPRESSION_LOSSY_JP2)
            qualityCtrl->SetValue(25);
          else
            qualityCtrl->SetValue(80);
        }
      qualityCtrl->Enable(true);
  } else
    {
      qualityCtrl->SetValue(100);
      qualityCtrl->Enable(false);
    }
}

void CreateRasterCoverageDialog::
OnNumBandsChanged(wxCommandEvent & WXUNUSED(event))
{
//
// NumBands changed
//
  bool is_8_bit = false;
  wxRadioBox *compressionCtrl = (wxRadioBox *) FindWindow(ID_CVG_COMPRESSION);
  wxRadioBox *sampleCtrl = (wxRadioBox *) FindWindow(ID_CVG_SAMPLE);
  wxSpinCtrl *bandsCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_BANDS);
  wxSpinCtrl *redBand = (wxSpinCtrl *) FindWindow(ID_CVG_RED);
  wxSpinCtrl *greenBand = (wxSpinCtrl *) FindWindow(ID_CVG_GREEN);
  wxSpinCtrl *blueBand = (wxSpinCtrl *) FindWindow(ID_CVG_BLUE);
  wxSpinCtrl *nirBand = (wxSpinCtrl *) FindWindow(ID_CVG_NIR);
  unsigned char num_bands = bandsCtrl->GetValue();
  if (sampleCtrl->GetSelection() == 4)
    is_8_bit = true;
  redBand->SetRange(-1, num_bands - 1);
  if (RedBand >= 0 && RedBand < num_bands)
    redBand->SetValue(RedBand);
  else
    {
      RedBand = -1;
      redBand->SetValue(-1);
    }
  greenBand->SetRange(-1, num_bands - 1);
  if (GreenBand >= 0 && GreenBand < num_bands)
    greenBand->SetValue(GreenBand);
  else
    {
      GreenBand = -1;
      greenBand->SetValue(-1);
    }
  blueBand->SetRange(-1, num_bands - 1);
  if (BlueBand >= 0 && BlueBand < num_bands)
    blueBand->SetValue(BlueBand);
  else
    {
      BlueBand = -1;
      blueBand->SetValue(-1);
    }
  nirBand->SetRange(-1, num_bands - 1);
  if (NIRband >= 0 && NIRband < num_bands)
    nirBand->SetValue(NIRband);
  else
    {
      NIRband = -1;
      nirBand->SetValue(-1);
    }
  if (num_bands == 3 || num_bands == 4)
    {
      compressionCtrl->Enable(0, true);
      compressionCtrl->Enable(1, true);
      compressionCtrl->Enable(2, true);
      compressionCtrl->Enable(3, true);
      compressionCtrl->Enable(4, true);
      compressionCtrl->Enable(5, false);
      if (is_8_bit)
        {
          compressionCtrl->Enable(6, true);
          compressionCtrl->Enable(7, true);
      } else
        {
          compressionCtrl->Enable(6, false);
          compressionCtrl->Enable(7, false);
        }
      compressionCtrl->Enable(8, true);
      compressionCtrl->Enable(9, true);
      compressionCtrl->Enable(10, false);
      compressionCtrl->Enable(11, true);
      compressionCtrl->Enable(12, true);
      compressionCtrl->Enable(13, true);
      compressionCtrl->Enable(14, true);
      compressionCtrl->Enable(15, true);
      if (is_8_bit)
        {
          if (compressionCtrl->GetSelection() == 5
              || compressionCtrl->GetSelection() == 10)
            {
              Compression = RL2_COMPRESSION_PNG;
              compressionCtrl->SetSelection(4);
            }
      } else
        {
          if (compressionCtrl->GetSelection() == 5
              || compressionCtrl->GetSelection() == 6
              || compressionCtrl->GetSelection() == 7
              || compressionCtrl->GetSelection() == 10)
            {
              Compression = RL2_COMPRESSION_PNG;
              compressionCtrl->SetSelection(4);
            }
        }
  } else
    {
      compressionCtrl->Enable(0, true);
      compressionCtrl->Enable(1, true);
      compressionCtrl->Enable(2, true);
      compressionCtrl->Enable(3, true);
      compressionCtrl->Enable(4, false);
      compressionCtrl->Enable(5, false);
      compressionCtrl->Enable(6, false);
      compressionCtrl->Enable(7, false);
      compressionCtrl->Enable(8, false);
      compressionCtrl->Enable(9, false);
      compressionCtrl->Enable(10, false);
      compressionCtrl->Enable(11, true);
      compressionCtrl->Enable(12, true);
      compressionCtrl->Enable(13, true);
      compressionCtrl->Enable(14, true);
      compressionCtrl->Enable(15, true);
      if (compressionCtrl->GetSelection() == 11
          || compressionCtrl->GetSelection() == 12
          || compressionCtrl->GetSelection() == 13
          || compressionCtrl->GetSelection() == 14
          || compressionCtrl->GetSelection() == 15
          || compressionCtrl->GetSelection() == 0
          || compressionCtrl->GetSelection() == 1
          || compressionCtrl->GetSelection() == 2
          || compressionCtrl->GetSelection() == 3)
        ;
      else
        {
          Compression = RL2_COMPRESSION_DEFLATE;
          compressionCtrl->SetSelection(0);
        }
    }
}

void CreateRasterCoverageDialog::
OnRedBandChanged(wxCommandEvent & WXUNUSED(event))
{
//
// default Red Band changed
//
  wxSpinCtrl *redCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_RED);
  RedBand = redCtrl->GetValue();
}

void CreateRasterCoverageDialog::
OnGreenBandChanged(wxCommandEvent & WXUNUSED(event))
{
//
// default Green Band changed
//
  wxSpinCtrl *greenCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_GREEN);
  GreenBand = greenCtrl->GetValue();
}

void CreateRasterCoverageDialog::
OnBlueBandChanged(wxCommandEvent & WXUNUSED(event))
{
//
// default Blue Band changed
//
  wxSpinCtrl *blueCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_BLUE);
  BlueBand = blueCtrl->GetValue();
}

void CreateRasterCoverageDialog::
OnNIRbandChanged(wxCommandEvent & WXUNUSED(event))
{
//
// default NIR Band changed
//
  wxSpinCtrl *nirCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_NIR);
  NIRband = nirCtrl->GetValue();
}

void CreateRasterCoverageDialog::
OnAutoNDVIchanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Auto NDVI (on/off): 
//
  wxCheckBox *autoNDVIctrl = (wxCheckBox *) FindWindow(ID_CVG_AUTO_NDVI);
  if (autoNDVIctrl->IsChecked() == true)
    AutoNDVI = true;
  else
    AutoNDVI = false;
}

void CreateRasterCoverageDialog::
OnSquareTileChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Square Tile (on/off): 
//
  wxCheckBox *squareCtrl = (wxCheckBox *) FindWindow(ID_CVG_SQTILE);
  wxSpinCtrl *widthCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_WIDTH);
  wxSpinCtrl *heightCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_HEIGHT);
  if (squareCtrl->IsChecked() == true)
    {
      heightCtrl->SetValue(widthCtrl->GetValue());
      heightCtrl->Enable(false);
  } else
    heightCtrl->Enable(true);
}

void CreateRasterCoverageDialog::
OnTileWidthChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Tile Width changed
//
  wxCheckBox *squareCtrl = (wxCheckBox *) FindWindow(ID_CVG_SQTILE);
  wxSpinCtrl *widthCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_WIDTH);
  wxSpinCtrl *heightCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_HEIGHT);
  int width = widthCtrl->GetValue();
  if ((width % 8) != 0)
    {
      // ensuring to be am exact multiple of 8
      if (width > TileWidth)
        TileWidth = ((width / 8) + 1) * 8;
      else
        TileWidth = (width / 8) * 8;
  } else
    TileWidth = width;
  widthCtrl->SetValue(TileWidth);
  if (squareCtrl->IsChecked() == true)
    {
      TileHeight = TileWidth;
      heightCtrl->SetValue(TileHeight);
    }
}

void CreateRasterCoverageDialog::
OnTileHeightChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Tile Height changed
//
  wxSpinCtrl *heightCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_HEIGHT);
  int height = heightCtrl->GetValue();
  if ((height % 8) != 0)
    {
      // ensuring to be an exact multiple of 8
      if (height > TileHeight)
        TileHeight = ((height / 8) + 1) * 8;
      else
        TileHeight = (height / 8) * 8;
  } else
    TileHeight = height;
  heightCtrl->SetValue(TileHeight);
}

void CreateRasterCoverageDialog::
OnNoGeorefChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Not Georeferenced (on/off): 
//
  wxCheckBox *norefCtrl = (wxCheckBox *) FindWindow(ID_CVG_NOREF);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_SRID);
  if (norefCtrl->IsChecked() == true)
    {
      sridCtrl->SetValue(-1);
      sridCtrl->Enable(false);
  } else
    sridCtrl->Enable(true);
}

void CreateRasterCoverageDialog::
OnSameResChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Square Tile (on/off): 
//
  wxCheckBox *sameresCtrl = (wxCheckBox *) FindWindow(ID_CVG_SAME_RES);
  wxTextCtrl *horzCtrl = (wxTextCtrl *) FindWindow(ID_CVG_HORZ_RES);
  wxTextCtrl *vertCtrl = (wxTextCtrl *) FindWindow(ID_CVG_VERT_RES);
  if (sameresCtrl->IsChecked() == true)
    {
      vertCtrl->SetValue(horzCtrl->GetValue());
      vertCtrl->Enable(false);
  } else
    vertCtrl->Enable(true);
}

void CreateRasterCoverageDialog::
OnHorzResChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Horz Resolution  
//
  wxCheckBox *sameresCtrl = (wxCheckBox *) FindWindow(ID_CVG_SAME_RES);
  wxTextCtrl *horzCtrl = (wxTextCtrl *) FindWindow(ID_CVG_HORZ_RES);
  wxTextCtrl *vertCtrl = (wxTextCtrl *) FindWindow(ID_CVG_VERT_RES);
  if (sameresCtrl->IsChecked() == true)
    vertCtrl->SetValue(horzCtrl->GetValue());
}

void CreateRasterCoverageDialog::
OnStrictChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Mixed Resolutions (on/off): 
//
  wxCheckBox *sameresCtrl = (wxCheckBox *) FindWindow(ID_CVG_SAME_RES);
  wxTextCtrl *horzCtrl = (wxTextCtrl *) FindWindow(ID_CVG_HORZ_RES);
  wxTextCtrl *vertCtrl = (wxTextCtrl *) FindWindow(ID_CVG_VERT_RES);
  wxCheckBox *strictCtrl = (wxCheckBox *) FindWindow(ID_CVG_STRICT_RES);
  wxCheckBox *mixedCtrl = (wxCheckBox *) FindWindow(ID_CVG_MIXED_RES);
  if (strictCtrl->IsChecked() == true)
    StrictResolution = true;
  else
    StrictResolution = false;
  if (StrictResolution)
    {
      MixedResolutions = false;
      mixedCtrl->SetValue(false);
    }
  if (MixedResolutions == true)
    {
      sameresCtrl->Enable(false);
      horzCtrl->Enable(false);
      vertCtrl->Enable(false);
  } else
    {
      horzCtrl->Enable(true);
      sameresCtrl->Enable(true);
      if (sameresCtrl->IsChecked() == true)
        vertCtrl->Enable(false);
      else
        vertCtrl->Enable(true);
    }
}

void CreateRasterCoverageDialog::
OnMixedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Mixed Resolutions (on/off): 
//
  wxCheckBox *sameresCtrl = (wxCheckBox *) FindWindow(ID_CVG_SAME_RES);
  wxTextCtrl *horzCtrl = (wxTextCtrl *) FindWindow(ID_CVG_HORZ_RES);
  wxTextCtrl *vertCtrl = (wxTextCtrl *) FindWindow(ID_CVG_VERT_RES);
  wxCheckBox *strictCtrl = (wxCheckBox *) FindWindow(ID_CVG_STRICT_RES);
  wxCheckBox *mixedCtrl = (wxCheckBox *) FindWindow(ID_CVG_MIXED_RES);
  if (mixedCtrl->IsChecked() == true)
    MixedResolutions = true;
  else
    MixedResolutions = false;
  if (MixedResolutions)
    {
      StrictResolution = false;
      strictCtrl->SetValue(false);
    }
  if (MixedResolutions == true)
    {
      sameresCtrl->Enable(false);
      horzCtrl->Enable(false);
      vertCtrl->Enable(false);
  } else
    {
      horzCtrl->Enable(true);
      sameresCtrl->Enable(true);
      if (sameresCtrl->IsChecked() == true)
        vertCtrl->Enable(false);
      else
        vertCtrl->Enable(true);
    }
}

void CreateRasterCoverageDialog::
OnPathsChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed Input Paths (on/off): 
//
  wxCheckBox *pathsCtrl = (wxCheckBox *) FindWindow(ID_CVG_PATHS);
  if (pathsCtrl->IsChecked() == true)
    InputPaths = true;
  else
    InputPaths = false;
}

void CreateRasterCoverageDialog::OnMD5Changed(wxCommandEvent & WXUNUSED(event))
{
//
// changed MD5 (on/off): 
//
  wxCheckBox *md5Ctrl = (wxCheckBox *) FindWindow(ID_CVG_MD5);
  if (md5Ctrl->IsChecked() == true)
    MD5 = true;
  else
    MD5 = false;
}

void CreateRasterCoverageDialog::
OnSummaryChanged(wxCommandEvent & WXUNUSED(event))
{
//
// changed XML Summary (on/off): 
//
  wxCheckBox *summaryCtrl = (wxCheckBox *) FindWindow(ID_CVG_SUMMARY);
  if (summaryCtrl->IsChecked() == true)
    Summary = true;
  else
    Summary = false;
}

bool CreateRasterCoverageDialog::IsValidNoData(wxString & no_data, int sample,
                                               int num_bands)
{
//
// checking a NO-DATA value
//
  int count = 0;
  int error = 0;
  char dummy[128];
  if (no_data.CmpNoCase(wxT("NONE")) == 0)
    return true;
  if (no_data.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some NO-DATA value"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxStringTokenizer tokenizer(no_data, wxT(","));
  while (tokenizer.HasMoreTokens())
    {
      count++;
      wxString token = tokenizer.GetNextToken();
      if (sample == RL2_SAMPLE_FLOAT || sample == RL2_SAMPLE_DOUBLE)
        {
          double v;
          if (token.ToDouble(&v) == false)
            {
              sprintf(dummy, "#%d) ", count);
              wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                           wxString::FromUTF8(dummy) + token,
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              error++;
            }
      } else
        {
          long v;
          if (token.ToLong(&v) == false)
            {
              sprintf(dummy, "#%d) ", count);
              wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                           wxString::FromUTF8(dummy) + token,
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              error++;
          } else
            {
              int err = 0;
              switch (sample)
                {
                  case RL2_SAMPLE_1_BIT:
                    if (v < 0 || v > 1)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_2_BIT:
                    if (v < 0 || v > 3)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_4_BIT:
                    if (v < 0 || v > 15)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_INT8:
                    if (v < CHAR_MIN || v > CHAR_MAX)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_UINT8:
                    if (v < 0 || v > UCHAR_MAX)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_INT16:
                    if (v < SHRT_MIN || v > SHRT_MAX)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_UINT16:
                    if (v < 0 || v > USHRT_MAX)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_INT32:
                    if (v < INT_MIN || v > INT_MAX)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                  case RL2_SAMPLE_UINT32:
                    if (v < 0 || v > UINT_MAX)
                      {
                        sprintf(dummy, "#%d) ", count);
                        wxMessageBox(wxT("NO-DATA - invalid sample value: ") +
                                     wxString::FromUTF8(dummy) + token,
                                     wxT("spatialite_gui"),
                                     wxOK | wxICON_WARNING, this);
                        err = 1;
                      }
                    break;
                };
              if (err)
                error++;
            }
        }
    }
  if (error)
    return false;
  if (count != num_bands)
    {
      sprintf(dummy, "expected %d, found #%d", num_bands, count);
      wxMessageBox(wxT("NO-DATA - invalid samples count: ") +
                   wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_WARNING, this);
      return false;
    }
  return true;
}

void CreateRasterCoverageDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_CVG_NAME);
  CoverageName = nameCtrl->GetValue();
  if (CoverageName.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the COVERAGE NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (MainFrame->CoverageAlreadyExists(CoverageName) == true)
    {
      wxMessageBox(wxT("a Coverage name '") + CoverageName +
                   wxT("' already exists"), wxT("spatialite_gui"),
                   wxOK | wxICON_WARNING, this);
      return;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_CVG_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxMessageBox(wxT("You must specify some TITLE !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_CVG_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxMessageBox(wxT("You must specify some ABSTRACT !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  wxSpinCtrl *bandsCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_BANDS);
  NumBands = bandsCtrl->GetValue();
  if (PixelType == RL2_PIXEL_MULTIBAND && NumBands < 2)
    {
      wxMessageBox(wxT("Invalid number of Bands"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (PixelType == RL2_PIXEL_MULTIBAND)
    {
      if (RedBand >= 0)
        {
          if (RedBand >= NumBands)
            {
              wxMessageBox(wxT("Invalid default Red Band"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return;
            }
        }
      if (GreenBand >= 0)
        {
          if (GreenBand >= NumBands)
            {
              wxMessageBox(wxT("Invalid default Green Band"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return;
            }
        }
      if (BlueBand >= 0)
        {
          if (BlueBand >= NumBands)
            {
              wxMessageBox(wxT("Invalid default Blue Band"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return;
            }
        }
      if (NIRband >= 0)
        {
          if (NIRband >= NumBands)
            {
              wxMessageBox(wxT("Invalid default NIR Band"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return;
            }
        }
      if (RedBand == GreenBand)
        {
          wxMessageBox(wxT("default Red and Green are the same Band"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
      if (RedBand == BlueBand)
        {
          wxMessageBox(wxT("default Red and Blue are the same Band"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
      if (RedBand == NIRband)
        {
          wxMessageBox(wxT("default Red and NIR are the same Band"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
      if (GreenBand == BlueBand)
        {
          wxMessageBox(wxT("default Green and Blue are the same Band"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
      if (GreenBand == NIRband)
        {
          wxMessageBox(wxT("default Green and NIR are the same Band"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
      if (BlueBand == NIRband)
        {
          wxMessageBox(wxT("default Blue and NIR are the same Band"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
  } else
    {
      RedBand = -1;
      GreenBand = -1;
      BlueBand = -1;
      NIRband = -1;
      AutoNDVI = false;
    }
  wxCheckBox *norefCtrl = (wxCheckBox *) FindWindow(ID_CVG_NOREF);
  if (norefCtrl->IsChecked() == true)
    {
      NotGeoreferenced = true;
      SRID = -1;
  } else
    {
      wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_SRID);
      SRID = sridCtrl->GetValue();
      if (SRID <= 0)
        {
          wxMessageBox(wxT("You must specify some SRID value !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
      } else if (MainFrame->SRIDnotExists(SRID) == true)
        {
          wxMessageBox(wxT("invalid SRID value"), wxT("spatialite_gui"),
                       wxOK | wxICON_WARNING, this);
          return;
        }
    }
  wxSpinCtrl *qtyCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_QUALITY);
  Quality = qtyCtrl->GetValue();
  wxSpinCtrl *widthCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_WIDTH);
  TileWidth = widthCtrl->GetValue();
  wxSpinCtrl *heightCtrl = (wxSpinCtrl *) FindWindow(ID_CVG_HEIGHT);
  TileHeight = heightCtrl->GetValue();
  wxTextCtrl *horzCtrl = (wxTextCtrl *) FindWindow(ID_CVG_HORZ_RES);
  wxString v = horzCtrl->GetValue();
  if (v.ToDouble(&HorzResolution) == false)
    {
      wxMessageBox(wxT("invalid HORIZONTAL resolution value"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (HorzResolution <= 0.0)
    {
      wxMessageBox(wxT("HORIZONTAL resolution should be a positive value"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  wxTextCtrl *vertCtrl = (wxTextCtrl *) FindWindow(ID_CVG_VERT_RES);
  v = vertCtrl->GetValue();
  if (v.ToDouble(&VertResolution) == false)
    {
      wxMessageBox(wxT("invalid VERTICAL resolution value"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (VertResolution <= 0.0)
    {
      wxMessageBox(wxT("VERTICAL resolution should be a positive value"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  wxTextCtrl *ndCtrl = (wxTextCtrl *) FindWindow(ID_CVG_NODATA);
  NoData = ndCtrl->GetValue();
  if (IsValidNoData(NoData, SampleType, NumBands) == false)
    return;
  wxCheckBox *strictCtrl = (wxCheckBox *) FindWindow(ID_CVG_STRICT_RES);
  if (strictCtrl->IsChecked() == true)
    StrictResolution = true;
  else
    StrictResolution = false;
  wxCheckBox *mixedCtrl = (wxCheckBox *) FindWindow(ID_CVG_MIXED_RES);
  if (mixedCtrl->IsChecked() == true)
    MixedResolutions = true;
  else
    MixedResolutions = false;
  wxCheckBox *pathsCtrl = (wxCheckBox *) FindWindow(ID_CVG_PATHS);
  if (pathsCtrl->IsChecked() == true)
    InputPaths = true;
  else
    InputPaths = false;
  wxCheckBox *md5Ctrl = (wxCheckBox *) FindWindow(ID_CVG_MD5);
  if (md5Ctrl->IsChecked() == true)
    MD5 = true;
  else
    MD5 = false;
  wxCheckBox *summaryCtrl = (wxCheckBox *) FindWindow(ID_CVG_SUMMARY);
  if (summaryCtrl->IsChecked() == true)
    Summary = true;
  else
    Summary = false;
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_CVG_QUERYABLE);
  if (queryableBox->GetSelection() == 1)
    Queryable = true;
  else
    Queryable = false;
  wxDialog::EndModal(wxID_OK);
  return;
}

bool ImportRasterDialog::Create(MyFrame * parent, wxString & coverage,
                                wxArrayString & paths, wxString & path,
                                wxString & title, wxString & abstract,
                                wxString & sample, wxString & pixel,
                                wxString & compression, int srid)
{
//
// creating the dialog
//
  MainFrame = parent;
  CoverageName = coverage;
  Paths = paths;
  Path = path;
  Title = title;
  Abstract = abstract;
  Sample = sample;
  Pixel = pixel;
  Compression = compression;
  SRID = srid;
  ForceSRID = false;
  WithWorldFile = false;
  Pyramidize = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Loading external files into a Raster Coverage")) ==
      false)
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

void ImportRasterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Coverage &Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Sample Type
  wxBoxSizer *sampleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sampleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *sampleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Sample Type:"));
  sampleSizer->Add(sampleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sampleCtrl = new wxTextCtrl(this, wxID_ANY, Sample,
                                          wxDefaultPosition, wxSize(600, 22),
                                          wxTE_READONLY);
  sampleSizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: the Pixel Type
  wxBoxSizer *pixelSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pixelSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *pixelLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Pixel Type:"));
  pixelSizer->Add(pixelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pixelCtrl = new wxTextCtrl(this, wxID_ANY, Pixel,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  pixelSizer->Add(pixelCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: the Compression Type
  wxBoxSizer *comprSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(comprSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *comprLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Compression:"));
  comprSizer->Add(comprLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *comprCtrl = new wxTextCtrl(this, wxID_ANY, Compression,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  comprSizer->Add(comprCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: files to be imported
  wxBoxSizer *fileSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(fileSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *fileLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Import &File(s):"));
  fileSizer->Add(fileLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *filesCtrl = new wxTextCtrl(this, wxID_ANY, Path,
                                         wxDefaultPosition, wxSize(600, 60),
                                         wxTE_MULTILINE | wxTE_READONLY);
  fileSizer->Add(filesCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// eighth  row: Import Options
  int space_sz = 25;
  wxBoxSizer *optBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(optBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *optBox = new wxStaticBox(this, wxID_STATIC,
                                        wxT("Import Options"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *optSizer = new wxStaticBoxSizer(optBox, wxHORIZONTAL);
  optBoxSizer->Add(optSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *sridSizer = new wxBoxSizer(wxHORIZONTAL);
  optSizer->Add(sridSizer, 0, wxALIGN_LEFT | wxALL, 0);
  optSizer->AddSpacer(space_sz);
  wxCheckBox *forceSridCtrl = new wxCheckBox(this, ID_LOAD_FORCE_SRID,
                                             wxT("Forced SRID value"),
                                             wxDefaultPosition, wxDefaultSize);
  forceSridCtrl->SetValue(false);
  sridSizer->Add(forceSridCtrl, 0, wxALIGN_LEFT | wxALL, 5);
  wxSpinCtrl *sridCtrl = new wxSpinCtrl(this, ID_LOAD_SRID, wxEmptyString,
                                        wxDefaultPosition, wxSize(80, 20),
                                        wxSP_ARROW_KEYS, -1, 1000000, SRID);
  sridCtrl->Enable(false);
  sridSizer->Add(sridCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  wxCheckBox *worldCtrl = new wxCheckBox(this, ID_LOAD_WITH_WORLDFILE,
                                         wxT("With WorldFile support"),
                                         wxDefaultPosition, wxDefaultSize);
  worldCtrl->SetValue(false);
  optSizer->Add(worldCtrl, 0, wxALIGN_LEFT | wxALL, 5);
  optSizer->AddSpacer(space_sz);
  wxCheckBox *pyrCtrl = new wxCheckBox(this, ID_LOAD_PYRAMIDIZE,
                                       wxT
                                       ("Immediately build Section Pyramids"),
                                       wxDefaultPosition, wxDefaultSize);
  pyrCtrl->SetValue(false);
  optSizer->Add(pyrCtrl, 0, wxALIGN_LEFT | wxALL, 5);
  wxTextCtrl *doneCtrl = new wxTextCtrl(this, ID_LOAD_LIST_DONE, ListDone,
                                        wxDefaultPosition, wxSize(650, 100),
                                        wxTE_MULTILINE | wxTE_READONLY |
                                        wxTE_RICH2);
  optBoxSizer->Add(doneCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Import"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *abort = new wxButton(this, ID_LOAD_ABORT, wxT("&Abort"));
  abort->Enable(false);
  okCancelBox->Add(abort, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ImportRasterDialog::OnOk);
  Connect(ID_LOAD_ABORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ImportRasterDialog::OnCmdAbort);
  Connect(ID_LOAD_FORCE_SRID, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) & ImportRasterDialog::OnCmdForceSridChanged);
  Connect(ID_LOAD_RASTER_START, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ImportRasterDialog::OnRequestStart);
  Connect(ID_LOAD_RASTER_STOP, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ImportRasterDialog::OnRequestStop);
  Connect(ID_LOAD_RASTER_THREAD_FINISHED, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & ImportRasterDialog::OnThreadFinished);
}

void ImportRasterDialog::OnCmdForceSridChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Force SRID selection changed 
//
  wxCheckBox *forceSridCtrl = (wxCheckBox *) FindWindow(ID_LOAD_FORCE_SRID);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_LOAD_SRID);
  if (forceSridCtrl->IsChecked() == true)
    {
      sridCtrl->SetValue(SRID);
      sridCtrl->Enable(true);
  } else
    sridCtrl->Enable(false);
}

void ImportRasterDialog::OnCmdAbort(wxCommandEvent & WXUNUSED(event))
{
//
// aborting the Raster Import process
//
  if (Params.IsAbortPending() == true)
    return;
  Params.RequestAbort();
  wxString report =
    wxT("\nan ABORT request is now pending and will be accepted ASAP");
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_LIST_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(192, 0, 0);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(report);
}

void ImportRasterDialog::OnRequestStart(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_LIST_DONE);
  wxColour fore = wxColour(255, 255, 255);
  wxColour back = wxColour(0, 0, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(msg);
  doneCtrl->MarkDirty();
}

void ImportRasterDialog::OnRequestStop(wxCommandEvent & event)
{
//
// updating the Progress Report
//
  wxString msg = event.GetString();
  wxTextCtrl *doneCtrl = (wxTextCtrl *) FindWindow(ID_LOAD_LIST_DONE);
  ListDone += msg;
  doneCtrl->Clear();
  wxColour fore = wxColour(0, 0, 0);
  wxColour back = wxColour(255, 255, 255);
  wxTextAttr style = wxTextAttr(fore, back);
  doneCtrl->SetDefaultStyle(style);
  doneCtrl->AppendText(ListDone);
  doneCtrl->MarkDirty();
}

#if defined(_WIN32) && !defined(__MINGW32__)
DWORD WINAPI DoExecuteRasterLoad(void *arg)
#else
void *DoExecuteRasterLoad(void *arg)
#endif
{
//
// threaded function: processing a Raster Import operation
//
  RasterLoadParams *params = (RasterLoadParams *) arg;
  const char *sql;
  int ret;
  sqlite3_stmt *stmt = NULL;
  int count = params->GetPathsCount();
  int i;
  clock_t clock_start;
  clock_t clock_end;
  double seconds;
  char elapsed[64];
  char ordinal[64];
  wxString report;
  wxString path;
  wxCommandEvent evt_start(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_RASTER_START);
  wxCommandEvent evt_stop(wxEVT_COMMAND_BUTTON_CLICKED, ID_LOAD_RASTER_STOP);

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

  sql = "SELECT RL2_LoadRaster(?, ?, ?, ?, ?, 1)";
  ret =
    sqlite3_prepare_v2(params->GetMainFrame()->GetSqlite(), sql, strlen(sql),
                       &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      params->SetError();
      goto error;
    }

  for (i = 0; i < count; i++)
    {
      // loading each Raster File
      if (params->IsAbortPending() == true)
        {
          report = wxT("STOP .... aborted by the user !!!!");
          evt_start.SetString(report);
          params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
          break;
        }
      path = params->GetPathByIndex(i);
      params->SetCurrentPath(path);
      report = wxT("Loading: ") + path;
      evt_start.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_start);
      clock_start = clock();
      if (params->GetMainFrame()->DoImportRaster
          (stmt, params->GetCoverageName(), path, params->GetSRID(),
           params->IsWithWorldFile(), params->IsPyramidize()) != true)
        {
          params->SetError();
          goto error;
        }
      clock_end = clock();
      seconds = (double) (clock_end - clock_start) / (double) CLOCKS_PER_SEC;
      MyResultSetView::FormatElapsedTime(seconds, elapsed);
      sprintf(ordinal, "done %d/%d: ", i + 1, count);
      report =
        wxString::FromUTF8(ordinal) + path + wxT("    [") +
        wxString::FromUTF8(elapsed) + wxT("]\n");
      evt_stop.SetString(report);
      params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
      params->Done();
    }
  sqlite3_finalize(stmt);
  goto end;

error:
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  report = wxT("FAILED: ") + path;
  evt_stop.SetString(report);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(evt_stop);
  params->SetError();
end:
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,
                       ID_LOAD_RASTER_THREAD_FINISHED);
  params->GetDlg()->GetEventHandler()->AddPendingEvent(event);
#if defined(_WIN32) && !defined(__MINGW32__)
  return 0;
#else
  pthread_exit(NULL);
  return NULL;
#endif
}

void ImportRasterDialog::OnThreadFinished(wxCommandEvent & WXUNUSED(event))
{
// resuming execution when the Import Raster thread quits
  ::wxEndBusyCursor();
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  quitBtn->Enable(true);
  abortBtn->Enable(false);
  if (Params.GetError() == true)
    {
      char dummy[80];
      sprintf(dummy, "%d Raster files have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nA fatal error occurred while loading:\n") +
                   Params.GetCurrentPath(), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
  } else if (Params.IsAbortPending() == true)
    {
      char dummy[80];
      sprintf(dummy, "%d Raster files have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy) +
                   wxT("\n\nStopped by an Abort user request"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
  } else
    {
      char dummy[80];
      sprintf(dummy, "%d Raster files have been successfully imported",
              Params.GetCount());
      wxMessageBox(wxString::FromUTF8(dummy), wxT("spatialite_gui"),
                   wxOK | wxICON_INFORMATION, this);
    }
}

void ImportRasterDialog::DoRunLoad()
{
//
// executing the Load Raster(s) process in a separate Thread
//
#if defined(_WIN32) && !defined(__MINGW32__)
  HANDLE thread_handle;
  DWORD dwThreadId;
#else
  pthread_t thread_id;
#endif
  Params.Initialize(MainFrame, this, CoverageName, Paths, SRID, WithWorldFile,
                    Pyramidize);
#if defined(_WIN32) && !defined(__MINGW32__)
  thread_handle =
    CreateThread(NULL, 0, DoExecuteRasterLoad, &Params, 0, &dwThreadId);
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
              pthread_create(&thread_id, &attr, DoExecuteRasterLoad, &Params);
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
                  pthread_create(&thread_id, &attr, DoExecuteRasterLoad,
                                 &Params);
                  pthread_detach(thread_id);
                }
            }
        }
    }
#endif
  if (!ok_prior)
    {
      // failure: using standard priority
      pthread_create(&thread_id, NULL, DoExecuteRasterLoad, &Params);
      pthread_detach(thread_id);
    }
#endif
}

void ImportRasterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxCheckBox *forceSridCtrl = (wxCheckBox *) FindWindow(ID_LOAD_FORCE_SRID);
  wxSpinCtrl *sridCtrl = (wxSpinCtrl *) FindWindow(ID_LOAD_SRID);
  wxCheckBox *worldCtrl = (wxCheckBox *) FindWindow(ID_LOAD_WITH_WORLDFILE);
  wxCheckBox *pyramidCtrl = (wxCheckBox *) FindWindow(ID_LOAD_PYRAMIDIZE);
  wxButton *loadBtn = (wxButton *) FindWindow(wxID_OK);
  wxButton *quitBtn = (wxButton *) FindWindow(wxID_CANCEL);
  wxButton *abortBtn = (wxButton *) FindWindow(ID_LOAD_ABORT);
  if (forceSridCtrl->IsChecked() == true)
    {
      ForceSRID = true;
      SRID = sridCtrl->GetValue();
  } else
    {
      ForceSRID = false;
      SRID = -1;
    }
  if (worldCtrl->IsChecked() == true)
    WithWorldFile = true;
  else
    WithWorldFile = false;
  if (pyramidCtrl->IsChecked() == true)
    Pyramidize = true;
  else
    Pyramidize = false;
  forceSridCtrl->Enable(false);
  sridCtrl->Enable(false);
  worldCtrl->Enable(false);
  pyramidCtrl->Enable(false);
  loadBtn->Enable(false);
  quitBtn->Enable(false);
  abortBtn->Enable(true);
  ::wxBeginBusyCursor();
  DoRunLoad();
}

bool PyramidizeDialog::Create(MyFrame * parent, wxString & coverage,
                              wxString & title, wxString & abstract,
                              wxString & sample, wxString & pixel,
                              wxString & compression)
{
//
// creating the dialog
//
  MainFrame = parent;
  CoverageName = coverage;
  Title = title;
  Abstract = abstract;
  Sample = sample;
  Pixel = pixel;
  Compression = compression;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("Building MultiResolution Pyramid Levels (by Section)"))
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

void PyramidizeDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Coverage &Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Sample Type
  wxBoxSizer *sampleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sampleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *sampleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Sample Type:"));
  sampleSizer->Add(sampleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sampleCtrl = new wxTextCtrl(this, wxID_ANY, Sample,
                                          wxDefaultPosition, wxSize(600, 22),
                                          wxTE_READONLY);
  sampleSizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: the Pixel Type
  wxBoxSizer *pixelSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pixelSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *pixelLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Pixel Type:"));
  pixelSizer->Add(pixelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pixelCtrl = new wxTextCtrl(this, wxID_ANY, Pixel,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  pixelSizer->Add(pixelCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: the Compression Type
  wxBoxSizer *comprSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(comprSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *comprLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Compression:"));
  comprSizer->Add(comprLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *comprCtrl = new wxTextCtrl(this, wxID_ANY, Compression,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  comprSizer->Add(comprCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row, mode force all
  wxString mode[2];
  mode[0] = wxT("&Only build missing Section Pyramids");
  mode[1] = wxT("&Unconditionally re-build all Section Pyramids");
  wxRadioBox *modeBox = new wxRadioBox(this, ID_PYRAMID_MODE,
                                       wxT("&Mode selection"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       mode, 1,
                                       wxRA_SPECIFY_COLS);
  boxSizer->Add(modeBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  modeBox->SetSelection(0);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Pyramidize"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & PyramidizeDialog::OnOk);
}

bool PyramidizeDialog::DoPyramidize()
{
//
// Building Section Pyramid levels */
//
  int ret;
  bool force = false;
  wxRadioBox *mode = (wxRadioBox *) FindWindow(ID_PYRAMID_MODE);
  if (mode->GetSelection() == 1)
    force = true;
  ::wxBeginBusyCursor();
  ret =
    rl2_build_all_section_pyramids(MainFrame->GetSqlite(),
                                   MainFrame->GetRL2PrivateData(),
                                   CoverageName.ToUTF8(), force, 1);
  ::wxEndBusyCursor();
  if (ret == RL2_OK)
    return true;
  else
    return false;
}

void PyramidizeDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  if (DoPyramidize() == true)
    wxMessageBox(wxT
                 ("Section Pyramids successfully built on Raster Coverage \"") +
                 CoverageName + wxT("\""), wxT("spatialite_gui"),
                 wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("Failure: some unexpected error occurred"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  wxDialog::EndModal(wxID_CANCEL);
}

bool PyramidizeMonolithicDialog::Create(MyFrame * parent, wxString & coverage,
                                        wxString & title, wxString & abstract,
                                        wxString & sample, wxString & pixel,
                                        wxString & compression)
{
//
// creating the dialog
//
  MainFrame = parent;
  CoverageName = coverage;
  Title = title;
  Abstract = abstract;
  Sample = sample;
  Pixel = pixel;
  Compression = compression;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("Building MultiResolution Pyramid Levels (Monolithic)"))
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

void PyramidizeMonolithicDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Coverage &Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Sample Type
  wxBoxSizer *sampleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sampleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *sampleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Sample Type:"));
  sampleSizer->Add(sampleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sampleCtrl = new wxTextCtrl(this, wxID_ANY, Sample,
                                          wxDefaultPosition, wxSize(600, 22),
                                          wxTE_READONLY);
  sampleSizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: the Pixel Type
  wxBoxSizer *pixelSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pixelSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *pixelLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Pixel Type:"));
  pixelSizer->Add(pixelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pixelCtrl = new wxTextCtrl(this, wxID_ANY, Pixel,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  pixelSizer->Add(pixelCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: the Compression Type
  wxBoxSizer *comprSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(comprSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *comprLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Compression:"));
  comprSizer->Add(comprLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *comprCtrl = new wxTextCtrl(this, wxID_ANY, Compression,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  comprSizer->Add(comprCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row, mode force all
  wxString mode[3];
  mode[0] =
    wxT("All Pyramid Levels are &Physical, there are no Virtual Levels");
  mode[1] = wxT("Interleave-1: every Physical Level supports a &Virtual Level");
  mode[2] =
    wxT("Interleave-2: every Physical Level supports &two Virtual Levels");
  wxRadioBox *modeBox = new wxRadioBox(this, ID_PYRAMID_MODE,
                                       wxT("&Mode selection"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       mode, 1,
                                       wxRA_SPECIFY_COLS);
  boxSizer->Add(modeBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  if (Sample == wxT("1-BIT") || Sample == wxT("2-BIT")
      || Sample == wxT("4-BIT"))
    modeBox->SetSelection(0);
  else
    modeBox->SetSelection(2);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Pyramidize"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & PyramidizeMonolithicDialog::OnOk);
}

bool PyramidizeMonolithicDialog::DoPyramidizeMonolithic()
{
//
// Building Monolithic Pyramid levels */
//
  int ret;
  int num_levels = 0;
  wxRadioBox *mode = (wxRadioBox *) FindWindow(ID_PYRAMID_MODE);
  if (mode->GetSelection() == 0)
    num_levels = 1;
  if (mode->GetSelection() == 1)
    num_levels = 2;
  if (mode->GetSelection() == 2)
    num_levels = 3;
  ::wxBeginBusyCursor();
  ret =
    rl2_build_monolithic_pyramid(MainFrame->GetSqlite(),
                                 MainFrame->GetRL2PrivateData(),
                                 CoverageName.ToUTF8(), num_levels, 1);
  ::wxEndBusyCursor();
  if (ret == RL2_OK)
    return true;
  else
    return false;
}

void PyramidizeMonolithicDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
//
  if (DoPyramidizeMonolithic() == true)
    wxMessageBox(wxT
                 ("Monolithic Pyramid successfully built on Raster Coverage \"")
                 + CoverageName + wxT("\""), wxT("spatialite_gui"),
                 wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("Failure: some unexpected error occurred"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  wxDialog::EndModal(wxID_CANCEL);
}

bool DePyramidizeDialog::Create(MyFrame * parent, wxString & coverage,
                                wxString & title, wxString & abstract,
                                wxString & sample, wxString & pixel,
                                wxString & compression)
{
//
// creating the dialog
//
  MainFrame = parent;
  CoverageName = coverage;
  Title = title;
  Abstract = abstract;
  Sample = sample;
  Pixel = pixel;
  Compression = compression;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("Removing MultiResolution Pyramid Levels")) == false)
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

void DePyramidizeDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Coverage &Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the Sample Type
  wxBoxSizer *sampleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sampleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *sampleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Sample Type:"));
  sampleSizer->Add(sampleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sampleCtrl = new wxTextCtrl(this, wxID_ANY, Sample,
                                          wxDefaultPosition, wxSize(600, 22),
                                          wxTE_READONLY);
  sampleSizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: the Pixel Type
  wxBoxSizer *pixelSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pixelSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *pixelLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Pixel Type:"));
  pixelSizer->Add(pixelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pixelCtrl = new wxTextCtrl(this, wxID_ANY, Pixel,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  pixelSizer->Add(pixelCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: the Compression Type
  wxBoxSizer *comprSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(comprSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *comprLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Compression:"));
  comprSizer->Add(comprLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *comprCtrl = new wxTextCtrl(this, wxID_ANY, Compression,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  comprSizer->Add(comprCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: confirm message
  wxBoxSizer *msgSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(msgSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
  wxString confirm =
    wxT
    ("All multi-resolution Pyramid Levels supporting this Raster Coverage will be irreversibly destroyed");
  wxStaticText *msg1Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg1Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  confirm = wxT("Do you really confirm De-Pyramidizing the Raster Coverage ?");
  wxStaticText *msg2Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&DePyramidize"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & DePyramidizeDialog::OnOk);
}

bool DePyramidizeDialog::DoDePyramidize()
{
//
// Deleting all Pyramid levels */
//
  int ret;
  ::wxBeginBusyCursor();
  ret = rl2_delete_all_pyramids(MainFrame->GetSqlite(), CoverageName.ToUTF8());
  ::wxEndBusyCursor();
  if (ret == RL2_OK)
    return true;
  else
    return false;
}

void DePyramidizeDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  if (DoDePyramidize() == true)
    wxMessageBox(wxT
                 ("Pyramid Levels successfully removed from Raster Coverage \"")
                 + CoverageName + wxT("\""), wxT("spatialite_gui"),
                 wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("Failure: some unexpected error occurred"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  wxDialog::EndModal(wxID_CANCEL);
}

bool RasterDropDialog::Create(MyFrame * parent, wxString & coverage,
                              wxString & title, wxString & abstract,
                              wxString & copyright, wxString & data_license,
                              wxString & sample, wxString & pixel,
                              wxString & compression)
{
//
// creating the dialog
//
  MainFrame = parent;
  CoverageName = coverage;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = data_license;
  Sample = sample;
  Pixel = pixel;
  Compression = compression;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Drop Raster Coverage")) == false)
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

void RasterDropDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Coverage &Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyCtrl = new wxTextCtrl(this, wxID_ANY, Copyright,
                                        wxDefaultPosition, wxSize(600, 60),
                                        wxTE_MULTILINE | wxTE_READONLY);
  copySizer->Add(copyCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Data License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *licCtrl = new wxTextCtrl(this, wxID_ANY, DataLicense,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  licSizer->Add(licCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: the Sample Type
  wxBoxSizer *sampleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sampleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *sampleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Sample Type:"));
  sampleSizer->Add(sampleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sampleCtrl = new wxTextCtrl(this, wxID_ANY, Sample,
                                          wxDefaultPosition, wxSize(600, 22),
                                          wxTE_READONLY);
  sampleSizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: the Pixel Type
  wxBoxSizer *pixelSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(pixelSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *pixelLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Pixel Type:"));
  pixelSizer->Add(pixelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *pixelCtrl = new wxTextCtrl(this, wxID_ANY, Pixel,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  pixelSizer->Add(pixelCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// eigth row: the Compression Type
  wxBoxSizer *comprSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(comprSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *comprLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Compression:"));
  comprSizer->Add(comprLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *comprCtrl = new wxTextCtrl(this, wxID_ANY, Compression,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  comprSizer->Add(comprCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// nineth row: confirm message
  wxBoxSizer *msgSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(msgSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
  wxString confirm =
    wxT
    ("This Raster Coverage will be completely and irreversibly removed from the DBMS");
  wxStaticText *msg1Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg1Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  confirm = wxT("Do you really confirm dropping the Raster Coverage ?");
  wxStaticText *msg2Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Drop"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterDropDialog::OnOk);
}

bool RasterDropDialog::DoDropCoverage()
{
//
// Dropping a Raster Coverage
//
  rl2CoveragePtr cvg = NULL;
  ::wxBeginBusyCursor();
  cvg =
    rl2_create_coverage_from_dbms(MainFrame->GetSqlite(), NULL,
                                  CoverageName.ToUTF8());
  if (cvg == NULL)
    goto error;
  if (rl2_drop_dbms_coverage(MainFrame->GetSqlite(), CoverageName.ToUTF8()) !=
      RL2_OK)
    goto error;
  rl2_destroy_coverage(cvg);
  ::wxEndBusyCursor();
  return true;

error:
  if (cvg != NULL)
    rl2_destroy_coverage(cvg);
  ::wxEndBusyCursor();
  return false;
}

void RasterDropDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  if (DoDropCoverage() == true)
    wxMessageBox(wxT("Raster Coverage \"") + CoverageName +
                 wxT("\" successfully removed from the DBMS\n\n") +
                 wxT
                 ("You could now eventually consider to execute VACUUM in order to reclaim the unused disk space."),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("Failure: some unexpected error occurred"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  wxDialog::EndModal(wxID_CANCEL);
}

bool RasterInfosDialog::Create(MyFrame * parent, wxString & coverage)
{
//
// creating the dialog
//
  MainFrame = parent;
  int pos = coverage.Find(wxT(" [SRID="));
  if (pos != wxNOT_FOUND)
    CoverageName = coverage.Left(pos);
  else
    CoverageName = coverage;
  if (DoReadCoverage() == false)
    return false;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Edit Raster Coverage Infos"))
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

void RasterInfosDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Coverage Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_CVG_NAME, CoverageName,
                                        wxDefaultPosition, wxSize(550, 22),
                                        wxTE_READONLY);
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_CVG_TITLE, Title,
                                         wxDefaultPosition, wxSize(550, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, ID_CVG_ABSTRACT, Abstract,
                                            wxDefaultPosition, wxSize(550, 60),
                                            wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyrightCtrl = new wxTextCtrl(this, ID_CVG_COPYRIGHT, Copyright,
                                             wxDefaultPosition, wxSize(550, 60),
                                             wxTE_MULTILINE);
  copySizer->Add(copyrightCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Data &License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *licenseCtrl =
    new wxComboBox(this, ID_CVG_LICENSE, wxT(""), wxDefaultPosition,
                   wxSize(550, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  licSizer->Add(licenseCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  PopulateDataLicenses(licenseCtrl);
// sixth row: queryable
  wxBoxSizer *radioSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(radioSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString yesno[2];
  yesno[0] = wxT("&No");
  yesno[1] = wxT("&Yes");
  wxRadioBox *queryableBox = new wxRadioBox(this, ID_CVG_QUERYABLE,
                                            wxT("Is &Queryable"),
                                            wxDefaultPosition,
                                            wxDefaultSize, 2,
                                            yesno, 1,
                                            wxRA_SPECIFY_ROWS);
  radioSizer->Add(queryableBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  if (IsQueryable == false)
    queryableBox->SetSelection(0);
  else
    queryableBox->SetSelection(1);
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&OK"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  btnBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterInfosDialog::OnOk);
}

void RasterInfosDialog::PopulateDataLicenses(wxComboBox * licCtrl)
{
// populating the Data Licenses List
  const char *sql;
  int ret;
  char **results;
  int rows;
  int columns;
  int sel = -1;

  sql =
    sqlite3_mprintf("SELECT id, name FROM MAIN.data_licenses ORDER BY name");
  ret =
    sqlite3_get_table(MainFrame->GetSqlite(), sql, &results, &rows, &columns,
                      NULL);
  if (ret != SQLITE_OK)
    return;

  for (int i = 1; i <= rows; i++)
    {
      // populating the list
      int id = atoi(results[(i * columns) + 0]);
      const char *name = results[(i * columns) + 1];
      wxString str = wxString::FromUTF8(name);
      licCtrl->Append(str);
      if (id == LicenseID)
        sel = i - 1;
    }
  sqlite3_free_table(results);
  licCtrl->SetSelection(sel);
}

bool RasterInfosDialog::DoReadCoverage()
{
// will retrieve to read the given Coverage definition
  int i;
  char **results;
  int rows;
  int columns;
  char *errMsg = NULL;
  char *value;
  char *sql;
  char cvg[1024];
  int count = 0;

  wxCharBuffer buffer = CoverageName.ToUTF8();
  sprintf(cvg, "%s", buffer.data());
  sql =
    sqlite3_mprintf
    ("SELECT title, abstract, copyright, license, is_queryable "
     "FROM MAIN.raster_coverages WHERE Lower(coverage_name) = Lower(%Q)", cvg);
  int ret = sqlite3_get_table(MainFrame->GetSqlite(), sql, &results,
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
          value = results[(i * columns) + 0];
          Title = wxString::FromUTF8(value);
          value = results[(i * columns) + 1];
          Abstract = wxString::FromUTF8(value);
          value = results[(i * columns) + 2];
          if (value == NULL)
            Copyright = wxT("");
          else
            Copyright = wxString::FromUTF8(value);
          value = results[(i * columns) + 3];
          LicenseID = atoi(value);
          value = results[(i * columns) + 4];
          if (atoi(value) == 0)
            IsQueryable = false;
          else
            IsQueryable = true;
          count++;
        }
    }
  sqlite3_free_table(results);
  if (count == 1)
    return true;
  return false;
}

void RasterInfosDialog::DoUpdateCoverage()
{
// will retrieve to update the given Coverage definition
  int ret;
  sqlite3_stmt *stmt = NULL;
  char *dummy;
  bool error = false;

// updating the Title and Abstract
  const char *sql = "SELECT RL2_SetRasterCoverageInfos(?, ?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetRasterCoverageInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
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
  if (IsQueryable == false)
    sqlite3_bind_int(stmt, 4, 0);
  else
    sqlite3_bind_int(stmt, 4, 1);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetRasterCoverageInfos: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
  if (error)
    return;

// updating the Copyright and License
  sql = "SELECT RL2_SetRasterCoverageCopyright(?, ?, ?)";
  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetRasterCoverageCopyright: ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      return;
    }
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  if (Copyright.Len() == 0)
    sqlite3_bind_null(stmt, 2);
  else
    {
      dummy = (char *) malloc((Copyright.Len() * 4) + 1);
      strcpy(dummy, Copyright.ToUTF8());
      sqlite3_bind_text(stmt, 2, dummy, -1, SQLITE_TRANSIENT);
      free(dummy);
    }
  dummy = (char *) malloc((DataLicense.Len() * 4) + 1);
  strcpy(dummy, DataLicense.ToUTF8());
  sqlite3_bind_text(stmt, 3, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      const char *msg = sqlite3_errmsg(MainFrame->GetSqlite());
      wxMessageBox(wxT("ERROR - SetRasterCoverageCopyright ") +
                   wxString::FromUTF8(msg), wxT("spatialite_gui"),
                   wxOK | wxICON_ERROR, this);
      error = true;
    }
  sqlite3_finalize(stmt);
}

void RasterInfosDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_CVG_TITLE);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_CVG_ABSTRACT);
  wxTextCtrl *copyCtrl = (wxTextCtrl *) FindWindow(ID_CVG_COPYRIGHT);
  wxComboBox *licCtrl = (wxComboBox *) FindWindow(ID_CVG_LICENSE);
  wxRadioBox *queryableBox = (wxRadioBox *) FindWindow(ID_CVG_QUERYABLE);
  wxString str = titleCtrl->GetValue().Trim();
  Title = str.Trim(false);
  if (Title.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Title !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = absCtrl->GetValue().Trim();
  Abstract = str.Trim(false);
  if (Abstract.Len() == 0)
    {
      wxMessageBox(wxT("You must specify some Abstract !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  str = copyCtrl->GetValue().Trim();
  Copyright = str.Trim(false);
  DataLicense = licCtrl->GetValue();
  if (queryableBox->GetSelection() == 1)
    IsQueryable = true;
  else
    IsQueryable = false;
  DoUpdateCoverage();
  wxDialog::EndModal(wxID_OK);
}

bool VectorUnregisterDialog::Create(MyFrame * parent, wxString & coverage,
                                    wxString & title, wxString & abstract,
                                    wxString & copyright, wxString & license,
                                    int origin, wxString & type)
{
//
// creating the dialog
//
  MainFrame = parent;
  CoverageName = coverage;
  Title = title;
  Abstract = abstract;
  Copyright = copyright;
  DataLicense = license;
  Origin = origin;
  Type = type;
  wxString xtitle = wxT("Unregister Vector Coverage (based on ");
  switch (origin)
    {
      case MyFrame::VECTOR_GEOTABLE:
        xtitle += wxT("GeoTable)");
        break;
      case MyFrame::VECTOR_SPATIALVIEW:
        xtitle += wxT("SpatialView)");
        break;
      case MyFrame::VECTOR_VIRTUALTABLE:
        xtitle += wxT("VirtualTable)");
        break;
      case MyFrame::VECTOR_TOPOGEO:
        xtitle += wxT("TopoGeo)");
        break;
      case MyFrame::VECTOR_TOPONET:
        xtitle += wxT("TopoNet)");
        break;
      case MyFrame::VECTOR_UNKNOWN:
      default:
        xtitle += wxT("??unknown??");
        xtitle += wxT(")");
        break;
    };
  if (wxDialog::Create(parent, wxID_ANY, xtitle) == false)
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

void VectorUnregisterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the Coverage Name
  wxBoxSizer *cvgSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(cvgSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *cvgLabel =
    new wxStaticText(this, wxID_STATIC, wxT("Coverage &Name:"));
  cvgSizer->Add(cvgLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *cvgCtrl = new wxTextCtrl(this, wxID_ANY, CoverageName,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  cvgSizer->Add(cvgCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the Coverage Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, wxID_ANY, Title,
                                         wxDefaultPosition, wxSize(600, 22),
                                         wxTE_READONLY);
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the Coverage Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl = new wxTextCtrl(this, wxID_ANY, Abstract,
                                            wxDefaultPosition, wxSize(600, 60),
                                            wxTE_MULTILINE | wxTE_READONLY);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: Copyright
  wxBoxSizer *copySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(copySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *copyLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Copyright:"));
  copySizer->Add(copyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *copyCtrl = new wxTextCtrl(this, wxID_ANY, Copyright,
                                        wxDefaultPosition, wxSize(600, 60),
                                        wxTE_MULTILINE | wxTE_READONLY);
  copySizer->Add(copyCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fifth row: Data License
  wxBoxSizer *licSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(licSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *licLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Data License:"));
  licSizer->Add(licLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *licCtrl = new wxTextCtrl(this, wxID_ANY, DataLicense,
                                       wxDefaultPosition, wxSize(600, 22),
                                       wxTE_READONLY);
  licSizer->Add(licCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: the Geometry Type
  wxBoxSizer *geomSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(geomSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *geomLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Geometry Type:"));
  geomSizer->Add(geomLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *geomCtrl = new wxTextCtrl(this, wxID_ANY, Type,
                                        wxDefaultPosition, wxSize(600, 22),
                                        wxTE_READONLY);
  geomSizer->Add(geomCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: confirm message
  wxBoxSizer *msgSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(msgSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
  wxString confirm =
    wxT
    ("This Vector Coverage will be UnRegistered preserving the underlying datasource");
  wxStaticText *msg1Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg1Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  confirm = wxT("Do you really confirm unregistering the Vector Coverage ?");
  wxStaticText *msg2Label = new wxStaticText(this, wxID_STATIC, confirm);
  msgSizer->Add(msg2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

// OK - CANCEL buttons
  wxBoxSizer *okCancelBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okCancelBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&UnRegister"));
  okCancelBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Quit"));
  okCancelBox->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & VectorUnregisterDialog::OnOk);
}

bool VectorUnregisterDialog::DoVectorUnregister()
{
//
// UnRegistering a Vector Coverage
//
  char *dummy;
  sqlite3_stmt *stmt = NULL;
  int valid = 0;
  ::wxBeginBusyCursor();
  const char *sql = "SELECT SE_UnRegisterVectorCoverage(?)";
  int ret = sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    goto error;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  dummy = (char *) malloc((CoverageName.Len() * 4) + 1);
  strcpy(dummy, CoverageName.ToUTF8());
  sqlite3_bind_text(stmt, 1, dummy, -1, SQLITE_TRANSIENT);
  free(dummy);
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
        goto error;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;
  if (!valid)
    goto error;
  ::wxEndBusyCursor();
  return true;

error:
  ::wxEndBusyCursor();
  return false;
}

void VectorUnregisterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  if (DoVectorUnregister() == true)
    wxMessageBox(wxT("Vector Coverage \"") + CoverageName +
                 wxT("\" successfully UnRegistered."),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  else
    wxMessageBox(wxT("Failure: some unexpected error occurred"),
                 wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
  wxDialog::EndModal(wxID_CANCEL);
}
