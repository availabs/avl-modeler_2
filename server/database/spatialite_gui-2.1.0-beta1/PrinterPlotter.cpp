/*
/ PrinterPlotter.cpp
/ various classes supporting Map Printing and Map Images
/
/ version 2.0, 2019 June 23
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2019  Alessandro Furieri
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

#include <wx/spinctrl.h>

bool PrinterDialog::Create(MyMapPanel * parent)
{
//
// creating the dialog
//
  MapPanel = parent;
  HorzPixels = 2400;
  VertPixels = 3508;
  MarginWidth = 100;
  PaperFormat = MY_PAPER_A4;
  Dpi = 300;
  Portrait = true;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Printing a PDF Map")) == false)
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

void PrinterDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: Format Selection
  wxBoxSizer *paperSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(paperSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *paperLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Page Format:"));
  paperSizer->Add(paperLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *paperCtrl =
    new wxComboBox(this, ID_PRINTER_FORMAT, wxT(""), wxDefaultPosition,
                   wxSize(200, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  paperSizer->Add(paperCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  paperCtrl->Append(wxT("A0 - 841 x 1189 mm"));
  paperCtrl->Append(wxT("A1 - 594 x 841 mm"));
  paperCtrl->Append(wxT("A2 - 420 x 594 mm"));
  paperCtrl->Append(wxT("A3 - 297 x 420 mm"));
  paperCtrl->Append(wxT("A4 - 210 x 297 mm"));
  paperCtrl->Append(wxT("A5 - 148 x 210 mm"));
  paperCtrl->SetSelection(4);
// second row: Dots Per Inch
  wxBoxSizer *dpiSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(dpiSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *dpiLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Dots per Inch (DPI):"));
  dpiSizer->Add(dpiLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *dpiCtrl =
    new wxComboBox(this, ID_PRINTER_DPI, wxT(""), wxDefaultPosition,
                   wxSize(200, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  dpiSizer->Add(dpiCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  dpiCtrl->Append(wxT("72 DPI"));
  dpiCtrl->Append(wxT("150 DPI"));
  dpiCtrl->Append(wxT("300 DPI"));
  dpiCtrl->Append(wxT("600 DPI"));
  dpiCtrl->SetSelection(2);
// third row: White Marging Width
  wxBoxSizer *marginSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(marginSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticText *marginLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&White Margin Width (mm):"));
  marginSizer->Add(marginLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *marginCtrl =
    new wxSpinCtrl(this, ID_PRINTER_MARGIN, wxEmptyString,
                   wxDefaultPosition, wxSize(60, 36),
                   wxSP_ARROW_KEYS,
                   0, 30, 20);
  marginSizer->Add(marginCtrl, 0, wxALIGN_LEFT | wxALL, 5);
// fourth row: Portrait - Landscape
  wxBoxSizer *mixSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(mixSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *portraitSizer = new wxBoxSizer(wxVERTICAL);
  mixSizer->Add(portraitSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString orient[2];
  orient[0] = wxT("Portrait");
  orient[1] = wxT("Landscape");
  wxRadioBox *portraitBox = new wxRadioBox(this, ID_PRINTER_PORTRAIT,
                                           wxT("Orientation"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 2,
                                           orient, 2,
                                           wxRA_SPECIFY_ROWS);
  portraitBox->SetSelection(0);
  portraitSizer->Add(portraitBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// fifth row: size in pixels
  wxStaticBox *pixelBox = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Size in Pixels"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *pixelBoxSizer = new wxStaticBoxSizer(pixelBox, wxVERTICAL);
  mixSizer->Add(pixelBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *wSizer = new wxBoxSizer(wxHORIZONTAL);
  pixelBoxSizer->Add(wSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *wLabel = new wxStaticText(this, wxID_STATIC, wxT("&Width:"));
  wSizer->Add(wLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *wCtrl = new wxTextCtrl(this, ID_PRINTER_WIDTH, wxT("2480"),
                                     wxDefaultPosition, wxSize(70, 22),
                                     wxTE_READONLY | wxTE_RIGHT);
  wSizer->Add(wCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);
  pixelBoxSizer->Add(hSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *hLabel = new wxStaticText(this, wxID_STATIC, wxT("&Heigth:"));
  hSizer->Add(hLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *hCtrl = new wxTextCtrl(this, ID_PRINTER_HEIGTH, wxT("3508"),
                                     wxDefaultPosition, wxSize(70, 22),
                                     wxTE_READONLY | wxTE_RIGHT);
  hSizer->Add(hCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// Ok Cancel buttons
  wxBoxSizer *okSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  okSizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  okSizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handlers
  Connect(ID_PRINTER_FORMAT, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & PrinterDialog::OnPaperFormatChanged);
  Connect(ID_PRINTER_DPI, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & PrinterDialog::OnDpiChanged);
  Connect(ID_PRINTER_PORTRAIT, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) & PrinterDialog::OnPortraitChanged);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & PrinterDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & PrinterDialog::OnCancel);
}

void PrinterDialog::OnPaperFormatChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Paper Format changed: updating sizes in pixels
//
  wxComboBox *formatCtrl = (wxComboBox *) FindWindow(ID_PRINTER_FORMAT);
  wxString value = formatCtrl->GetValue().Left(2);
  if (value.Cmp(wxT("A0")) == 0)
    PaperFormat = MY_PAPER_A0;
  else if (value.Cmp(wxT("A1")) == 0)
    PaperFormat = MY_PAPER_A1;
  else if (value.Cmp(wxT("A2")) == 0)
    PaperFormat = MY_PAPER_A2;
  else if (value.Cmp(wxT("A3")) == 0)
    PaperFormat = MY_PAPER_A3;
  else if (value.Cmp(wxT("A5")) == 0)
    PaperFormat = MY_PAPER_A5;
  else
    PaperFormat = MY_PAPER_A4;
  DoUpdateSize();
}

void PrinterDialog::OnDpiChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Paper Format changed: updating sizes in pixels
//
  wxComboBox *dpiCtrl = (wxComboBox *) FindWindow(ID_PRINTER_DPI);
  wxString value = dpiCtrl->GetValue();
  if (value.Cmp(wxT("72 DPI")) == 0)
    Dpi = 72;
  else if (value.Cmp(wxT("150 DPI")) == 0)
    Dpi = 150;
  else if (value.Cmp(wxT("600 DPI")) == 0)
    Dpi = 600;
  else
    Dpi = 300;
  DoUpdateSize();
}

void PrinterDialog::OnPortraitChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Sheet orientation changed: Portrait or Landscape
//
  wxRadioBox *orient = (wxRadioBox *) FindWindow(ID_PRINTER_PORTRAIT);
  if (orient->GetSelection() == 0)
    Portrait = true;
  else
    Portrait = false;
  DoUpdateSize();
}

void PrinterDialog::DoUpdateSize()
{
//
// updating the Size measured in pixels
//
  wxTextCtrl *wCtrl = (wxTextCtrl *) FindWindow(ID_PRINTER_WIDTH);
  wxTextCtrl *hCtrl = (wxTextCtrl *) FindWindow(ID_PRINTER_HEIGTH);
  int w = 210;
  int h = 297;
  double mm2in = 0.039370078740157;
  double w_inch;
  double h_inch;
  double w_pix;
  double h_pix;
  switch (PaperFormat)
    {
      case MY_PAPER_A0:
        w = 841;
        h = 1189;
        break;
      case MY_PAPER_A1:
        w = 594;
        h = 841;
        break;
      case MY_PAPER_A2:
        w = 420;
        h = 594;
        break;
      case MY_PAPER_A3:
        w = 297;
        h = 420;
        break;
      case MY_PAPER_A5:
        w = 148;
        h = 210;
        break;
      case MY_PAPER_A4:
      default:
        w = 210;
        h = 297;
        break;
    };
  if (Portrait == true)
    {
      // orientation Portrait
      w_inch = (double) w *mm2in;
      h_inch = (double) h *mm2in;
  } else
    {
      // orientation Landscape
      w_inch = (double) h *mm2in;
      h_inch = (double) w *mm2in;
    }
  w_pix = w_inch * (double) Dpi;
  h_pix = h_inch * (double) Dpi;
  HorzPixels = w_pix;
  VertPixels = h_pix;
  wxString intval = wxString::Format(wxT("%i"), HorzPixels);
  wCtrl->SetValue(intval);
  intval = wxString::Format(wxT("%i"), VertPixels);
  hCtrl->SetValue(intval);
}

void PrinterDialog::OnCancel(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void PrinterDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool MapImageDialog::Create(MyMapPanel * parent)
{
//
// creating the dialog
//
  MapPanel = parent;
  HorzPixels = 1024;
  VertPixels = 1024;
  ImageFormat = MY_IMAGE_TIFF;
  CompressionMethod = MY_IMG_COMPRESSION_NONE;
  Quality = 100;
  WorldFile = false;
  GeoTiff = true;
  if (wxDialog::Create(parent, wxID_ANY, wxT("Creating a Map Image")) == false)
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

void MapImageDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// firsf row: size in pixels
  wxStaticBox *pixelBox = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Size in Pixels"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *pixelBoxSizer = new wxStaticBoxSizer(pixelBox, wxHORIZONTAL);
  boxSizer->Add(pixelBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *wSizer = new wxBoxSizer(wxHORIZONTAL);
  pixelBoxSizer->Add(wSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *wLabel = new wxStaticText(this, wxID_STATIC, wxT("&Width:"));
  wSizer->Add(wLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *wCtrl = new wxSpinCtrl(this, ID_MAP_IMAGE_WIDTH, wxEmptyString,
                                     wxDefaultPosition, wxSize(80, 36),
                                     wxSP_ARROW_KEYS,
                                     512, 25000, 1024);
  wSizer->Add(wCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  pixelBoxSizer->AddSpacer(20);
  wxBoxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);
  pixelBoxSizer->Add(hSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *hLabel = new wxStaticText(this, wxID_STATIC, wxT("&Heigth:"));
  hSizer->Add(hLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *hCtrl = new wxSpinCtrl(this, ID_MAP_IMAGE_HEIGTH, wxEmptyString,
                                     wxDefaultPosition, wxSize(80, 36),
                                     wxSP_ARROW_KEYS,
                                     512, 25000, 1024);
  hSizer->Add(hCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: Image Format
  wxBoxSizer *formatSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(formatSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *formatLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Image Format:"));
  formatSizer->Add(formatLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxComboBox *formatCtrl =
    new wxComboBox(this, ID_MAP_IMAGE_FORMAT, wxT(""), wxDefaultPosition,
                   wxSize(200, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  formatSizer->Add(formatCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  formatCtrl->Append(wxT("PNG  image"));
  formatCtrl->Append(wxT("JPEG image"));
  formatCtrl->Append(wxT("TIFF image"));
  formatCtrl->SetSelection(2);
  wxCheckBox *worldFile = new wxCheckBox(this, ID_MAP_IMAGE_WORLD_FILE,
                                         wxT("with WorldFile"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  worldFile->SetValue(false);
  formatSizer->AddSpacer(20);
  formatSizer->AddSpacer(20);
  formatSizer->AddSpacer(20);
  formatSizer->AddSpacer(20);
  formatSizer->AddSpacer(20);
  formatSizer->Add(worldFile, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// third row: JPEG / TIFF options
  wxBoxSizer *mixSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(mixSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *jpegBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("JPEG options"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *jpegBoxSizer = new wxStaticBoxSizer(jpegBox, wxHORIZONTAL);
  mixSizer->Add(jpegBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *qualitySizer = new wxBoxSizer(wxHORIZONTAL);
  jpegBoxSizer->Add(qualitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *qualityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Quality:"));
  qualitySizer->Add(qualityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *qualityCtrl =
    new wxSpinCtrl(this, ID_MAP_IMAGE_QUALITY, wxEmptyString,
                   wxDefaultPosition, wxSize(60, 36),
                   wxSP_ARROW_KEYS,
                   5, 99, 85);
  qualityCtrl->Enable(false);
  qualitySizer->Add(qualityCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticBox *tiffBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("TIFF options"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *tiffBoxSizer = new wxStaticBoxSizer(tiffBox, wxVERTICAL);
  mixSizer->Add(tiffBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *compressionSizer = new wxBoxSizer(wxHORIZONTAL);
  tiffBoxSizer->Add(compressionSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *compressionLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Compression:"));
  compressionSizer->Add(compressionLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                        5);
  wxComboBox *compressionCtrl =
    new wxComboBox(this, ID_MAP_IMAGE_COMPRESSION, wxT(""), wxDefaultPosition,
                   wxSize(300, 22), 0, NULL, wxCB_DROPDOWN | wxCB_READONLY);
  compressionSizer->Add(compressionCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL,
                        5);
  compressionCtrl->Append(wxT("NONE    not compressed"));
  compressionCtrl->Append(wxT("DEFLATE lossless compression (bland)"));
  compressionCtrl->Append(wxT("LZMA    lossless compression (strong)"));
  compressionCtrl->Append(wxT("JPEG    lossy compression"));
  compressionCtrl->SetSelection(0);
  wxBoxSizer *geotiffSizer = new wxBoxSizer(wxHORIZONTAL);
  tiffBoxSizer->Add(geotiffSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxCheckBox *geoTiff = new wxCheckBox(this, ID_MAP_IMAGE_GEO_TIFF,
                                       wxT("GeoTiff"),
                                       wxDefaultPosition,
                                       wxDefaultSize);
  geoTiff->SetValue(true);
  geotiffSizer->Add(geoTiff, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Ok Cancel buttons
  wxBoxSizer *okSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(okSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Ok"));
  okSizer->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("&Cancel"));
  okSizer->Add(cancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handlers
  Connect(ID_MAP_IMAGE_FORMAT, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & MapImageDialog::OnImageFormatChanged);
  Connect(ID_MAP_IMAGE_COMPRESSION, wxEVT_COMMAND_COMBOBOX_SELECTED,
          (wxObjectEventFunction) & MapImageDialog::OnCompressionChanged);
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapImageDialog::OnOk);
  Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & MapImageDialog::OnCancel);
}

void MapImageDialog::OnImageFormatChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Image Format selection changed
//
  wxSpinCtrl *qualityCtrl = (wxSpinCtrl *) FindWindow(ID_MAP_IMAGE_QUALITY);
  wxComboBox *compressionCtrl =
    (wxComboBox *) FindWindow(ID_MAP_IMAGE_COMPRESSION);
  wxCheckBox *geoTiff = (wxCheckBox *) FindWindow(ID_MAP_IMAGE_GEO_TIFF);
  wxComboBox *formatCtrl = (wxComboBox *) FindWindow(ID_MAP_IMAGE_FORMAT);
  wxString value = formatCtrl->GetValue().Left(4);
  if (value.Cmp(wxT("PNG ")) == 0)
    {
      qualityCtrl->Enable(false);
      compressionCtrl->Enable(false);
      geoTiff->Enable(false);
  } else if (value.Cmp(wxT("JPEG")) == 0)
    {
      qualityCtrl->Enable(true);
      compressionCtrl->Enable(false);
      geoTiff->Enable(false);
  } else
    {
      qualityCtrl->Enable(false);
      compressionCtrl->Enable(true);
      geoTiff->Enable(true);
    }
}

void MapImageDialog::OnCompressionChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Compression selection changed
//
  wxComboBox *compressionCtrl =
    (wxComboBox *) FindWindow(ID_MAP_IMAGE_COMPRESSION);
  wxSpinCtrl *qualityCtrl = (wxSpinCtrl *) FindWindow(ID_MAP_IMAGE_QUALITY);
  wxString value = compressionCtrl->GetValue().Left(8);
  if (value.Cmp(wxT("JPEG    ")) == 0)
    qualityCtrl->Enable(true);
  else
    qualityCtrl->Enable(false);
}

void MapImageDialog::OnCancel(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_CANCEL);
}

void MapImageDialog::OnOk(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxSpinCtrl *widthCtrl = (wxSpinCtrl *) FindWindow(ID_MAP_IMAGE_WIDTH);
  wxSpinCtrl *heigthCtrl = (wxSpinCtrl *) FindWindow(ID_MAP_IMAGE_HEIGTH);
  HorzPixels = widthCtrl->GetValue();
  VertPixels = heigthCtrl->GetValue();
  wxComboBox *formatCtrl = (wxComboBox *) FindWindow(ID_MAP_IMAGE_FORMAT);
  wxString value = formatCtrl->GetValue().Left(4);
  if (value.Cmp(wxT("PNG ")) == 0)
    ImageFormat = MY_IMAGE_PNG;
  else if (value.Cmp(wxT("JPEG")) == 0)
    ImageFormat = MY_IMAGE_JPEG;
  else
    ImageFormat = MY_IMAGE_TIFF;
  wxComboBox *compressionCtrl =
    (wxComboBox *) FindWindow(ID_MAP_IMAGE_COMPRESSION);
  value = compressionCtrl->GetValue().Left(8);
  if (value.Cmp(wxT("LZMA    ")) == 0)
    CompressionMethod = MY_IMG_COMPRESSION_LZMA;
  else if (value.Cmp(wxT("DEFLATE ")) == 0)
    CompressionMethod = MY_IMG_COMPRESSION_DEFLATE;
  else if (value.Cmp(wxT("JPEG    ")) == 0)
    CompressionMethod = MY_IMG_COMPRESSION_JPEG;
  else
    CompressionMethod = MY_IMG_COMPRESSION_NONE;
  wxSpinCtrl *qualityCtrl = (wxSpinCtrl *) FindWindow(ID_MAP_IMAGE_QUALITY);
  Quality = qualityCtrl->GetValue();
  wxCheckBox *worldFile = (wxCheckBox *) FindWindow(ID_MAP_IMAGE_WORLD_FILE);
  wxCheckBox *geoTiff = (wxCheckBox *) FindWindow(ID_MAP_IMAGE_GEO_TIFF);
  WorldFile = worldFile->IsChecked();
  GeoTiff = geoTiff->IsChecked();
  wxDialog::EndModal(wxID_OK);
}
