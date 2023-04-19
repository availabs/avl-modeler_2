/*
/ RasterSymbolizers.cpp
/ various dialog classes
/
/ version 1.8, 2015 March 23
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
#include "wx/filename.h"
#include "wx/colordlg.h"
#include "wx/clipbrd.h"

bool RasterSymbolizerContrastDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Normalize = true;
  Histogram = false;
  Gamma = false;
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("RasterSymbolizer: ContrastEnhancement")) == false)
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

void RasterSymbolizerContrastDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fifth row: Contrast Enhancement
  wxBoxSizer *contrastSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(contrastSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString contrast[3];
  contrast[0] = wxT("&Normalize/Stretch");
  contrast[1] = wxT("&Histogram");
  contrast[2] = wxT("&GammaValue");
  wxRadioBox *contrastBox = new wxRadioBox(this, ID_SYMBOLIZER_CONTRAST,
                                           wxT("&Contrast Enhancement Method"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 3,
                                           contrast, 1,
                                           wxRA_SPECIFY_ROWS);
  contrastSizer->Add(contrastBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  contrastBox->SetSelection(0);
  wxBoxSizer *gammaSizer = new wxBoxSizer(wxHORIZONTAL);
  contrastSizer->Add(gammaSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticText *gammaLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&GammaValue [x100]:"));
  gammaSizer->Add(gammaLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *gammaCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_GAMMA, wxT("100"),
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxSP_ARROW_KEYS,
                                         100, 500, 100);
  gammaCtrl->Enable(false);
  gammaSizer->Add(gammaCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// sixth row: optional Visibility Range
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerContrastDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerContrastDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerContrastDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerContrastDialog::OnCopy);
  Connect(ID_SYMBOLIZER_CONTRAST, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerContrastDialog::OnCmdModeChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerContrastDialog::OnCmdScaleChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerContrastDialog::OnCmdScaleChanged);
}

void RasterSymbolizerContrastDialog::
OnCmdModeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Contrast Enhancement Method selection changed
//
  wxRadioBox *contrastCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CONTRAST);
  wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
  switch (contrastCtrl->GetSelection())
    {
      case 0:
        Normalize = true;
        Histogram = false;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 1:
        Normalize = false;
        Histogram = true;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 2:
        Normalize = false;
        Histogram = false;
        Gamma = true;
        gammaCtrl->Enable(true);
        break;
    };
}

void RasterSymbolizerContrastDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

bool RasterSymbolizerContrastDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  if (Gamma == true)
    {
      wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
      GammaValue = gammaCtrl->GetValue() / 100.0;
  } else
    GammaValue = 1.0;
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

char *RasterSymbolizerContrastDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
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
        sqlite3_mprintf("%s\t\t\t\t<GammaValue>%1.2f</GammaValue>\r\n", prev,
                        GammaValue);
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
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerContrastDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<ContrastEnhancement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (Histogram == true)
    {
      xml = sqlite3_mprintf("%s\t\t<Histogram/>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
  } else if (Gamma == true)
    {
      xml =
        sqlite3_mprintf("%s\t\t<GammaValue>%1.2f</GammaValue>\r\n", prev,
                        GammaValue);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      xml = sqlite3_mprintf("%s\t\t<Normalize/>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t</ContrastEnhancement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerContrastDialog::OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerContrastDialog::OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerContrastDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerContrastDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool MyFrame::ValidateRasterStyle(void **blob, int *blob_size, const char *xml)
{
//
// attempting to parse and validate a Raster Style
//
  int ret;
  sqlite3_stmt *stmt;
  void *xblob = NULL;
  int xblob_size;
  int valid = 0;

// Schema validation
  const char *sql = "SELECT XB_Create(?, 1, 1)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, xml, strlen(xml), SQLITE_STATIC);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              const void *xxblob = sqlite3_column_blob(stmt, 0);
              xblob_size = sqlite3_column_bytes(stmt, 0);
              xblob = malloc(xblob_size);
              memcpy(xblob, xxblob, xblob_size);
            }
      } else
        {
          sqlite3_finalize(stmt);
          return false;
        }
    }
  sqlite3_finalize(stmt);
  if (xblob == NULL)
    return false;

// Checking if really is a Raster Style
  stmt = NULL;
  sql = "SELECT XB_IsSldSERasterStyle(?)";
  ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    goto invalid;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, xblob, xblob_size, SQLITE_STATIC);
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
        goto invalid;
    }
  sqlite3_finalize(stmt);
  stmt = NULL;
  if (!valid)
    goto invalid;
  *blob = xblob;
  *blob_size = xblob_size;
  return true;

invalid:
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  free(xblob);
  *blob = NULL;
  *blob_size = 0;
  return false;
}

bool MyFrame::DoInsertRasterSymbolizer(char *xml)
{
//
// attempting to register a RasterSymbolizer
//
  void *blob = NULL;
  int blob_size;
  ::wxBeginBusyCursor();
  if (ValidateRasterStyle(&blob, &blob_size, xml) != true)
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT("Error: not a valid SLD/SE RasterSymbolizer"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(xml);
      return false;
    }
  ::wxEndBusyCursor();

  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterRasterStyle(?)";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql),
                               &stmt, NULL);
  if (ret != SQLITE_OK)
    return false;

  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  sqlite3_bind_blob(stmt, 1, blob, blob_size, free);
  ret = sqlite3_step(stmt);
  if (ret == SQLITE_DONE || ret == SQLITE_ROW)
    ;
  else
    {
      sqlite3_finalize(stmt);
      return false;
    }
  sqlite3_finalize(stmt);
  return true;
}

bool RasterSymbolizerChannelRgbDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Normalize = false;
  Histogram = false;
  Gamma = false;
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("RasterSymbolizer: ChannelSelection (RGB: false-colors)"))
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

void RasterSymbolizerChannelRgbDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fifth row: ChannelSelection
  wxBoxSizer *channelBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(channelBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *channelBox = new wxStaticBox(this, wxID_STATIC,
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
    new wxStaticText(this, wxID_STATIC, wxT("&Red Channel is Band #"));
  redSizer->Add(redLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *redCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_RED, wxT("1"),
                                       wxDefaultPosition, wxSize(80, 22),
                                       wxSP_ARROW_KEYS,
                                       1, 256, 1);
  redSizer->Add(redCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  wxBoxSizer *greenSizer = new wxBoxSizer(wxHORIZONTAL);
  bandSizer->Add(greenSizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *greenLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Green Channel is Band #"));
  greenSizer->Add(greenLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *greenCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_GREEN, wxT("2"),
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxSP_ARROW_KEYS,
                                         1, 256, 2);
  greenSizer->Add(greenCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
  wxBoxSizer *blueSizer = new wxBoxSizer(wxHORIZONTAL);
  bandSizer->Add(blueSizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *blueLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Blue Channel is Band #"));
  blueSizer->Add(blueLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *blueCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_BLUE, wxT("3"),
                                        wxDefaultPosition, wxSize(80, 22),
                                        wxSP_ARROW_KEYS,
                                        1, 256, 3);
  blueSizer->Add(blueCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// sixth row: Contrast Enhancement
  wxBoxSizer *contrastSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(contrastSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString contrast[4];
  contrast[0] = wxT("&None");
  contrast[1] = wxT("Normalize/&Stretch");
  contrast[2] = wxT("&Histogram");
  contrast[3] = wxT("&GammaValue");
  wxRadioBox *contrastBox = new wxRadioBox(this, ID_SYMBOLIZER_CONTRAST,
                                           wxT("&Contrast Enhancement Method"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 4,
                                           contrast, 2,
                                           wxRA_SPECIFY_COLS);
  contrastSizer->Add(contrastBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  contrastBox->SetSelection(0);
  wxBoxSizer *gammaSizer = new wxBoxSizer(wxHORIZONTAL);
  contrastSizer->Add(gammaSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticText *gammaLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&GammaValue [x100]:"));
  gammaSizer->Add(gammaLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *gammaCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_GAMMA, wxT("100"),
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxSP_ARROW_KEYS,
                                         100, 500, 100);
  gammaCtrl->Enable(false);
  gammaSizer->Add(gammaCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: optional Visibility Range
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerChannelRgbDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerChannelRgbDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerChannelRgbDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerChannelRgbDialog::OnCopy);
  Connect(ID_SYMBOLIZER_CONTRAST, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelRgbDialog::OnCmdModeChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelRgbDialog::OnCmdScaleChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelRgbDialog::OnCmdScaleChanged);
}

void RasterSymbolizerChannelRgbDialog::
OnCmdModeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Contrast Enhancement Method selection changed
//
  wxRadioBox *contrastCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CONTRAST);
  wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
  switch (contrastCtrl->GetSelection())
    {
      case 0:
        Normalize = false;
        Histogram = false;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 1:
        Normalize = true;
        Histogram = false;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 2:
        Normalize = false;
        Histogram = true;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 3:
        Normalize = false;
        Histogram = false;
        Gamma = true;
        gammaCtrl->Enable(true);
        break;
    };
}

void RasterSymbolizerChannelRgbDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

bool RasterSymbolizerChannelRgbDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  wxSpinCtrl *redCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RED);
  RedBand = redCtrl->GetValue();
  wxSpinCtrl *greenCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GREEN);
  GreenBand = greenCtrl->GetValue();
  wxSpinCtrl *blueCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_BLUE);
  BlueBand = blueCtrl->GetValue();
  if (Gamma == true)
    {
      wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
      GammaValue = gammaCtrl->GetValue() / 100.0;
  } else
    GammaValue = 1.0;
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

char *RasterSymbolizerChannelRgbDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t<RedChannel>\r\n\t\t\t\t\t<SourceChannelName>%d",
                    prev, RedBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t\t\t</RedChannel>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<GreenChannel>\r\n\t\t\t\t\t<SourceChannelName>%d", prev,
     GreenBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t\t\t</GreenChannel>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<BlueChannel>\r\n\t\t\t\t\t<SourceChannelName>%d", prev,
     BlueBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t\t\t</BlueChannel>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
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
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerChannelRgbDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t<RedChannel>\r\n\t\t\t<SourceChannelName>%d", prev,
                    RedBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t</RedChannel>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t<GreenChannel>\r\n\t\t\t<SourceChannelName>%d", prev,
                    GreenBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t</GreenChannel>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t<BlueChannel>\r\n\t\t\t<SourceChannelName>%d", prev,
                    BlueBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t</BlueChannel>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (Normalize == true || Histogram == true || Gamma == true)
    {
      xml = sqlite3_mprintf("%s\t<ContrastEnhancement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Histogram == true)
        {
          xml = sqlite3_mprintf("%s\t\t<Histogram/>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
      } else if (Gamma == true)
        {
          xml =
            sqlite3_mprintf("%s\t\t<GammaValue>%1.2f</GammaValue>\r\n", prev,
                            GammaValue);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          xml = sqlite3_mprintf("%s\t\t<Normalize/>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</ContrastEnhancement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerChannelRgbDialog::
OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerChannelRgbDialog::
OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerChannelRgbDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerChannelRgbDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool RasterSymbolizerChannelGrayDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Normalize = false;
  Histogram = false;
  Gamma = false;
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("RasterSymbolizer: ChannelSelection (single band, Grayscale)"))
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

void RasterSymbolizerChannelGrayDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fifth row: ChannelSelection
  wxBoxSizer *channelBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(channelBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *channelBox = new wxStaticBox(this, wxID_STATIC,
                                            wxT
                                            ("Channel Selection (first Band is #1)"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *channelSizer = new wxStaticBoxSizer(channelBox, wxVERTICAL);
  channelBoxSizer->Add(channelSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *bandSizer = new wxBoxSizer(wxVERTICAL);
  channelSizer->Add(bandSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *graySizer = new wxBoxSizer(wxHORIZONTAL);
  bandSizer->Add(graySizer, 0, wxALIGN_RIGHT | wxALL, 3);
  wxStaticText *grayLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Gray Channel is Band #"));
  graySizer->Add(grayLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *grayCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_GRAY, wxT("1"),
                                        wxDefaultPosition, wxSize(80, 22),
                                        wxSP_ARROW_KEYS,
                                        1, 256, 1);
  graySizer->Add(grayCtrl, 0, wxALIGN_RIGHT | wxALL, 0);
// sixth row: Contrast Enhancement
  wxBoxSizer *contrastSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(contrastSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString contrast[4];
  contrast[0] = wxT("&None");
  contrast[1] = wxT("Normalize/&Stretch");
  contrast[2] = wxT("&Histogram");
  contrast[3] = wxT("&GammaValue");
  wxRadioBox *contrastBox = new wxRadioBox(this, ID_SYMBOLIZER_CONTRAST,
                                           wxT("&Contrast Enhancement Method"),
                                           wxDefaultPosition,
                                           wxDefaultSize, 4,
                                           contrast, 2,
                                           wxRA_SPECIFY_COLS);
  contrastSizer->Add(contrastBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  contrastBox->SetSelection(0);
  wxBoxSizer *gammaSizer = new wxBoxSizer(wxHORIZONTAL);
  contrastSizer->Add(gammaSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticText *gammaLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&GammaValue [x100]:"));
  gammaSizer->Add(gammaLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *gammaCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_GAMMA, wxT("100"),
                                         wxDefaultPosition, wxSize(80, 22),
                                         wxSP_ARROW_KEYS,
                                         100, 500, 100);
  gammaCtrl->Enable(false);
  gammaSizer->Add(gammaCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// seventh row: optional Visibility Range
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerChannelGrayDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelGrayDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelGrayDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerChannelGrayDialog::OnCopy);
  Connect(ID_SYMBOLIZER_CONTRAST, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelGrayDialog::OnCmdModeChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerChannelGrayDialog::OnCmdScaleChanged);
}

void RasterSymbolizerChannelGrayDialog::
OnCmdModeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Contrast Enhancement Method selection changed
//
  wxRadioBox *contrastCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CONTRAST);
  wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
  switch (contrastCtrl->GetSelection())
    {
      case 0:
        Normalize = false;
        Histogram = false;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 1:
        Normalize = true;
        Histogram = false;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 2:
        Normalize = false;
        Histogram = true;
        Gamma = false;
        gammaCtrl->Enable(false);
        break;
      case 3:
        Normalize = false;
        Histogram = false;
        Gamma = true;
        gammaCtrl->Enable(true);
        break;
    };
}

void RasterSymbolizerChannelGrayDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

bool RasterSymbolizerChannelGrayDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  wxSpinCtrl *grayCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GRAY);
  GrayBand = grayCtrl->GetValue();
  if (Gamma == true)
    {
      wxSpinCtrl *gammaCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_GAMMA);
      GammaValue = gammaCtrl->GetValue() / 100.0;
  } else
    GammaValue = 1.0;
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

char *RasterSymbolizerChannelGrayDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<GrayChannel>\r\n\t\t\t\t\t<SourceChannelName>%d", prev,
     GrayBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t\t\t</GrayChannel>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
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
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerChannelGrayDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t<GrayChannel>\r\n\t\t\t<SourceChannelName>%d", prev,
                    GrayBand);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s</SourceChannelName>\r\n\t\t</GrayChannel>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</ChannelSelection>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (Normalize == true || Histogram == true || Gamma == true)
    {
      xml = sqlite3_mprintf("%s\t<ContrastEnhancement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Histogram == true)
        {
          xml = sqlite3_mprintf("%s\t\t<Histogram/>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
      } else if (Gamma == true)
        {
          xml =
            sqlite3_mprintf("%s\t\t<GammaValue>%1.2f</GammaValue>\r\n", prev,
                            GammaValue);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          xml = sqlite3_mprintf("%s\t\t<Normalize/>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</ContrastEnhancement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerChannelGrayDialog::
OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerChannelGrayDialog::
OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerChannelGrayDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerChannelGrayDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool RasterSymbolizerShadedReliefDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("RasterSymbolizer: Shaded Relief (brightness only)"))
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

void RasterSymbolizerShadedReliefDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fifth row: the Relief Factor
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *shadedBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(shadedBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *shadedBox = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Shaded Relief"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *shadedSizer = new wxStaticBoxSizer(shadedBox, wxHORIZONTAL);
  shadedBoxSizer->Add(shadedSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *reliefSizer = new wxBoxSizer(wxVERTICAL);
  shadedSizer->Add(reliefSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *reliefLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Relief Factor:"));
  reliefSizer->Add(reliefLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *reliefCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_RELIEF, wxT(""),
                                          wxDefaultPosition, wxSize(80, 22),
                                          wxSP_ARROW_KEYS,
                                          1, 200, 25);
  reliefSizer->Add(reliefCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// sixth row: optional Visibility Range
  miscSizer->AddSpacer(25);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerShadedReliefDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerShadedReliefDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerShadedReliefDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerShadedReliefDialog::OnCopy);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerShadedReliefDialog::OnCmdScaleChanged);
}

bool RasterSymbolizerShadedReliefDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  wxSpinCtrl *reliefCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RELIEF);
  ReliefFactor = reliefCtrl->GetValue();
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

void RasterSymbolizerShadedReliefDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

char *RasterSymbolizerShadedReliefDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<ShadedRelief>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t<BrightnessOnly>1</BrightnessOnly>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n", prev,
                    ReliefFactor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</ShadedRelief>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerShadedReliefDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<ShadedRelief>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t<BrightnessOnly>1</BrightnessOnly>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n", prev,
                    ReliefFactor);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</ShadedRelief>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerShadedReliefDialog::
OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerShadedReliefDialog::
OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerShadedReliefDialog::
OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerShadedReliefDialog::
OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool ColorMapEntry::IsValidColor(wxString & color)
{
// testing an HexRGB color for validity
  if (color.Len() != 7)
    return false;
  char hex[64];
  strcpy(hex, color.ToUTF8());
  if (*(hex + 0) != '#')
    return false;
  for (int i = 1; i <= 6; i++)
    {
      char x = *(hex + i);
      if (x >= '0' && x <= '9')
        continue;
      if (x >= 'a' && x <= 'f')
        continue;
      if (x >= 'A' && x <= 'F')
        continue;
      return false;
    }
  return true;
}

unsigned char ColorMapEntry::ParseHex(unsigned char hi, unsigned char lo)
{
// parsing an HEX color
  unsigned char byte;
  switch (hi)
    {
      case '0':
        byte = 16 * 0;
        break;
      case '1':
        byte = 16 * 1;
        break;
      case '2':
        byte = 16 * 2;
        break;
      case '3':
        byte = 16 * 3;
        break;
      case '4':
        byte = 16 * 4;
        break;
      case '5':
        byte = 16 * 5;
        break;
      case '6':
        byte = 16 * 6;
        break;
      case '7':
        byte = 16 * 7;
        break;
      case '8':
        byte = 16 * 8;
        break;
      case '9':
        byte = 16 * 9;
        break;
      case 'a':
      case 'A':
        byte = 16 * 10;
        break;
      case 'b':
      case 'B':
        byte = 16 * 11;
        break;
      case 'c':
      case 'C':
        byte = 16 * 12;
        break;
      case 'd':
      case 'D':
        byte = 16 * 13;
        break;
      case 'e':
      case 'E':
        byte = 16 * 14;
        break;
      case 'f':
      case 'F':
        byte = 16 * 15;
        break;
    };
  switch (lo)
    {
      case '0':
        byte += 0;
        break;
      case '1':
        byte += 1;
        break;
      case '2':
        byte += 2;
        break;
      case '3':
        byte += 3;
        break;
      case '4':
        byte += 4;
        break;
      case '5':
        byte += 5;
        break;
      case '6':
        byte += 6;
        break;
      case '7':
        byte += 7;
        break;
      case '8':
        byte += 8;
        break;
      case '9':
        byte += 9;
        break;
      case 'a':
      case 'A':
        byte += 10;
        break;
      case 'b':
      case 'B':
        byte += 11;
        break;
      case 'c':
      case 'C':
        byte += 12;
        break;
      case 'd':
      case 'D':
        byte += 13;
        break;
      case 'e':
      case 'E':
        byte += 14;
        break;
      case 'f':
      case 'F':
        byte += 15;
        break;
    };
  return byte;
}

void ColorMapEntry::GetWxColor(wxString & color, wxColour & clr)
{
// return a wxWidger color from an HexRGB color
  if (IsValidColor(color) == false)
    {
      clr = wxColour(0, 0, 0);
      return;
    }
  char hex[64];
  strcpy(hex, color.ToUTF8());
  unsigned char red = ParseHex(*(hex + 1), *(hex + 2));
  unsigned char green = ParseHex(*(hex + 3), *(hex + 4));
  unsigned char blue = ParseHex(*(hex + 5), *(hex + 6));
  clr = wxColour(red, green, blue);
}

void ColorMapEntry::DoPaintColorSample(int width, int height, wxColour & color,
                                       wxBitmap & bitmap)
{
// painting a Color Bitmap
  bitmap = wxBitmap(width, height);
  wxMemoryDC *dc = new wxMemoryDC(bitmap);
  if (dc->IsOk() == true)
    {
      dc->SetBrush(wxBrush(color));
      dc->DrawRectangle(-1, -1, width + 2, height + 2);
      dc->SetBrush(wxNullBrush);
      delete dc;
    }
}

ColorMapCategorize::~ColorMapCategorize()
{
// dtor
  ColorMapEntry *pE;
  ColorMapEntry *pEn;
  pE = First;
  while (pE != NULL)
    {
      pEn = pE->GetNext();
      delete pE;
      pE = pEn;
    }
}

void ColorMapCategorize::Add(double value, wxString & color)
{
// adding a new Entry into the Map by searching the appropriate position
// or updating an already existing Entry
  ColorMapEntry *pE;
  ColorMapEntry *pEnew;
  pE = First;
  while (pE != NULL)
    {
      if (pE->GetValue() == value)
        {
          // updating an already defined Entry
          pE->SetColor(color);
          return;
        }
      pE = pE->GetNext();
    }

// inserting a new Entry
  bool is_first = false;
  if (First == NULL)
    is_first = true;
  else if (value < First->GetValue())
    is_first = true;
  if (is_first == true)
    {
      // inserting at the beginning of the Map
      pEnew = new ColorMapEntry(value, color);
      if (First == NULL)
        {
          First = pEnew;
          Last = pEnew;
          return;
        }
      First->SetPrev(pEnew);
      pEnew->SetNext(First);
      First = pEnew;
      return;
    }

  pE = First;
  while (pE != NULL)
    {
      // searching the appropriate position into the Map
      if (value < pE->GetValue())
        {
          pEnew = new ColorMapEntry(value, color);
          pE->GetPrev()->SetNext(pEnew);
          pEnew->SetPrev(pE->GetPrev());
          pEnew->SetNext(pE);
          pE->SetPrev(pEnew);
          return;
        }
      pE = pE->GetNext();
    }

// last item
  pEnew = new ColorMapEntry(value, color);
  pEnew->SetPrev(Last);
  Last->SetNext(pEnew);
  Last = pEnew;
}

void ColorMapCategorize::Remove(double value)
{
// removing an Entry from the Map
  ColorMapEntry *pE;
  pE = First;
  while (pE != NULL)
    {
      if (pE->GetValue() == value)
        {
          if (pE == First && pE == Last)
            {
              // removing the unique Entry
              First = NULL;
              Last = NULL;
              delete pE;
          } else if (pE == First)
            {
              // removing the first Entry
              pE->GetNext()->SetPrev(NULL);
              First = pE->GetNext();
              delete pE;
          } else if (pE == Last)
            {
              // removing the last Entry
              pE->GetPrev()->SetNext(NULL);
              Last = pE->GetPrev();
              delete pE;
          } else
            {
              // removing any other Entry
              pE->GetPrev()->SetNext(pE->GetNext());
              pE->GetNext()->SetPrev(pE->GetPrev());
              delete pE;
            }
          break;
        }
      pE = pE->GetNext();
    }
}

bool RasterSymbolizerCategorizeDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  ShadedRelief = false;
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("RasterSymbolizer: Color Map - Categorize")) == false)
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

void RasterSymbolizerCategorizeDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row: GRID to show the Color Map
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Color Map"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SYMBOLIZER_MAP, wxDefaultPosition, wxSize(380, 250));
  GridCtrl->CreateGrid(1, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Min Value"));
  GridCtrl->SetColLabelValue(1, wxT("Max Value"));
  GridCtrl->SetColLabelValue(2, wxT("Color"));
  GridCtrl->SetColLabelValue(3, wxT("Sample"));
  GridCtrl->SetRowLabelValue(0, wxT("1"));
  wxString cell = wxT("-Infinite");
  GridCtrl->SetCellValue(0, 0, cell);
  GridCtrl->SetCellAlignment(0, 0, wxALIGN_RIGHT, wxALIGN_TOP);
  cell = wxT("+Infinite");
  GridCtrl->SetCellValue(0, 1, cell);
  GridCtrl->SetCellAlignment(0, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  GridCtrl->SetCellValue(0, 2, Map.GetFirstColor());
  wxColour color;
  ColorMapEntry::GetWxColor(Map.GetFirstColor(), color);
  GridCtrl->SetCellBackgroundColour(0, 3, color);
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// extra Map items
  wxBoxSizer *extraBoxSizer = new wxBoxSizer(wxVERTICAL);
  gridSizer->Add(extraBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *entryBox = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Map Entry"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *entrySizer = new wxStaticBoxSizer(entryBox, wxHORIZONTAL);
  extraBoxSizer->Add(entrySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *entry1Sizer = new wxBoxSizer(wxVERTICAL);
  entrySizer->Add(entry1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxBoxSizer *entry2Sizer = new wxBoxSizer(wxVERTICAL);
  entrySizer->Add(entry2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *valueSizer = new wxBoxSizer(wxHORIZONTAL);
  entry1Sizer->Add(valueSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *valueLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Threshold:"));
  valueSizer->Add(valueLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *valueCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_VALUE, wxT("0.00"),
                                         wxDefaultPosition, wxSize(100, 22));
  valueSizer->Add(valueCtrl, 0, wxALIGN_RIGHT | wxALL, 2);
  wxBoxSizer *colorSizer = new wxBoxSizer(wxHORIZONTAL);
  entry1Sizer->Add(colorSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *colorLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Color:"));
  colorSizer->Add(colorLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_COLOR, wxT("#000000"),
                   wxDefaultPosition, wxSize(100, 22));
  colorSizer->Add(colorCtrl, 0, wxALIGN_RIGHT | wxALL, 2);
  wxButton *add = new wxButton(this, ID_SYMBOLIZER_ADD, wxT("&Update Map"));
  entry2Sizer->Add(add, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *pickerBox = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Color Picker"),
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxTE_READONLY);
  wxBoxSizer *pickerSizer = new wxStaticBoxSizer(pickerBox, wxHORIZONTAL);
  extraBoxSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *pick =
    new wxButton(this, ID_SYMBOLIZER_PICKER_BTN, wxT("&Pick a color"));
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *pickerCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_PICKER_HEX, wxT(""),
                   wxDefaultPosition, wxSize(100, 22));
  pickerSizer->Add(pickerCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// sixth row: optional Shaded Relief
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *shadedBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(shadedBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *shadedBox = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Shaded Relief"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *shadedSizer = new wxStaticBoxSizer(shadedBox, wxHORIZONTAL);
  shadedBoxSizer->Add(shadedSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *reliefSizer = new wxBoxSizer(wxVERTICAL);
  shadedSizer->Add(reliefSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxCheckBox *shadedCtrl = new wxCheckBox(this, ID_SYMBOLIZER_SHADED,
                                          wxT("Shaded Relief"),
                                          wxDefaultPosition, wxDefaultSize);
  reliefSizer->Add(shadedCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *relief2Sizer = new wxBoxSizer(wxHORIZONTAL);
  reliefSizer->Add(relief2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *reliefLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Relief Factor:"));
  relief2Sizer->Add(reliefLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *reliefCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_RELIEF, wxT(""),
                                          wxDefaultPosition, wxSize(80, 22),
                                          wxSP_ARROW_KEYS,
                                          0, 0, 0);
  reliefCtrl->Enable(false);
  relief2Sizer->Add(reliefCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// seventh row: optional Visibility Range
  miscSizer->AddSpacer(75);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerCategorizeDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerCategorizeDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerCategorizeDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerCategorizeDialog::OnCopy);
  Connect(ID_SYMBOLIZER_SHADED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerCategorizeDialog::OnShadedChanged);
  Connect(ID_SYMBOLIZER_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerCategorizeDialog::OnCmdAdd);
  Connect(ID_SYMBOLIZER_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerCategorizeDialog::OnCmdRemove);
  Connect(ID_SYMBOLIZER_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerCategorizeDialog::OnCmdColorPicker);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) &
          RasterSymbolizerCategorizeDialog::OnRightClick);
  Connect(wxID_ANY, wxEVT_GRID_SELECT_CELL,
          (wxObjectEventFunction) &
          RasterSymbolizerCategorizeDialog::OnCellSelected);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerCategorizeDialog::OnCmdScaleChanged);
}

void RasterSymbolizerCategorizeDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

void RasterSymbolizerCategorizeDialog::RefreshGrid()
{
//
// refreshing the Grid
//
  int tot_rows = GridCtrl->GetNumberRows();
  if (tot_rows > 0)
    GridCtrl->DeleteRows(0, tot_rows);
  int count = 1;
  ColorMapEntry *pE = Map.GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->AppendRows(count);
  count = 1;
  char dummy[1024];
  wxString cell;
  wxString last = wxT("-Infinite");
  GridCtrl->SetCellValue(0, 0, last);
  GridCtrl->SetCellAlignment(0, 0, wxALIGN_RIGHT, wxALIGN_TOP);
  pE = Map.GetFirst();
  if (pE != NULL)
    {
      sprintf(dummy, "%1.6f", pE->GetValue());
      cell = wxString::FromUTF8(dummy);
  } else
    cell = wxT("+Infinite");
  GridCtrl->SetCellValue(0, 1, cell);
  last = cell;
  GridCtrl->SetCellAlignment(0, 1, wxALIGN_RIGHT, wxALIGN_TOP);
  GridCtrl->SetCellValue(0, 2, Map.GetFirstColor());
  wxColour color;
  ColorMapEntry::GetWxColor(Map.GetFirstColor(), color);
  GridCtrl->SetCellBackgroundColour(0, 3, color);
  pE = Map.GetFirst();
  while (pE)
    {
      // feeding grid rows
      GridCtrl->SetCellValue(count, 0, last);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      ColorMapEntry *pEn = pE->GetNext();
      if (pEn == NULL)
        cell = wxT("+Infinite");
      else
        {
          sprintf(dummy, "%1.6f", pEn->GetValue());
          cell = wxString::FromUTF8(dummy);
        }
      GridCtrl->SetCellValue(count, 1, cell);
      last = cell;
      GridCtrl->SetCellAlignment(count, 1, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 2, pE->GetColor());
      ColorMapEntry::GetWxColor(pE->GetColor(), color);
      GridCtrl->SetCellBackgroundColour(count, 3, color);
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->AutoSizeColumns();
}

void RasterSymbolizerCategorizeDialog::OnCellSelected(wxGridEvent & event)
{
//
// cell selection changed
//
  CurrentRow = event.GetRow();
  wxString value = GridCtrl->GetCellValue(CurrentRow, 0);
  wxString color = GridCtrl->GetCellValue(CurrentRow, 2);
  wxTextCtrl *valueCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_VALUE);
  valueCtrl->SetValue(value);
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
  colorCtrl->SetValue(color);
  if (CurrentRow == 0)
    valueCtrl->Enable(false);
}

void RasterSymbolizerCategorizeDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  if (CurrentRow == 0)
    return;
  GridCtrl->SelectRow(CurrentRow);
  wxString value = GridCtrl->GetCellValue(CurrentRow, 0);
  double val;
  value.ToDouble(&val);
  CurrentValue = val;
  menuItem = new wxMenuItem(&menu, ID_SYMBOLIZER_REMOVE, wxT("&Remove Entry"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void RasterSymbolizerCategorizeDialog::
OnCmdAdd(wxCommandEvent & WXUNUSED(event))
{
//
// adding a new Map Entry
//
  wxTextCtrl *valueCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_VALUE);
  wxString value = valueCtrl->GetValue();
  double val;
  if (valueCtrl->IsEnabled() == true)
    {
      if (value.ToDouble(&val) != true)
        {
          wxMessageBox(wxT
                       ("VALUE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return;
        }
    }
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      wxMessageBox(wxT
                   ("COLOR isn't a valid HexRGB color !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  if (valueCtrl->IsEnabled() == true)
    Map.Add(val, color);
  else
    Map.SetFirstColor(color);
  RefreshGrid();
  valueCtrl->SetValue(wxT(""));
  valueCtrl->Enable(true);
  colorCtrl->SetValue(wxT(""));
}

void RasterSymbolizerCategorizeDialog::
OnCmdRemove(wxCommandEvent & WXUNUSED(event))
{
//
// removing a Map Entry
//
  Map.Remove(CurrentValue);
  RefreshGrid();
}

void RasterSymbolizerCategorizeDialog::
OnShadedChanged(wxCommandEvent & WXUNUSED(event))
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
      ShadedRelief = true;
  } else
    {
      reliefCtrl->SetRange(0, 0);
      reliefCtrl->SetValue(0);
      reliefCtrl->Enable(false);
      ShadedRelief = false;
    }
}

void RasterSymbolizerCategorizeDialog::
OnCmdColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *pickerCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_PICKER_HEX);
  wxColour clr = wxNullColour;
  wxString str = pickerCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      pickerCtrl->SetValue(str);
    }
}

bool RasterSymbolizerCategorizeDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  if (ShadedRelief == true)
    {
      wxSpinCtrl *reliefCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RELIEF);
      ReliefFactor = reliefCtrl->GetValue();
    }
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

char *RasterSymbolizerCategorizeDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t<ColorMap>\r\n\t\t\t\t<Categorize fallbackValue=\"#ffffff\">\r\n",
     prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Map.GetFirstColor().ToUTF8()) + 1];
  strcpy(str, Map.GetFirstColor().ToUTF8());
  xml = sqlite3_mprintf("%s\t\t\t\t\t<Value>%s</Value>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  ColorMapEntry *pE = Map.GetFirst();
  while (pE != NULL)
    {
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<Threshold>%1.6f</Threshold>\r\n", prev,
                        pE->GetValue());
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(pE->GetColor().ToUTF8()) + 1];
      strcpy(str, pE->GetColor().ToUTF8());
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Value>%s</Value>\r\n", prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      pE = pE->GetNext();
    }
  xml =
    sqlite3_mprintf("%s\t\t\t\t</Categorize>\r\n\t\t\t</ColorMap>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (ShadedRelief == true)
    {
      xml = sqlite3_mprintf("%s\t\t\t<ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n",
                        prev, ReliefFactor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerCategorizeDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t<ColorMap>\r\n\t\t<Categorize fallbackValue=\"#ffffff\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Map.GetFirstColor().ToUTF8()) + 1];
  strcpy(str, Map.GetFirstColor().ToUTF8());
  xml = sqlite3_mprintf("%s\t\t\t<Value>%s</Value>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  ColorMapEntry *pE = Map.GetFirst();
  while (pE != NULL)
    {
      xml =
        sqlite3_mprintf("%s\t\t\t<Threshold>%1.6f</Threshold>\r\n", prev,
                        pE->GetValue());
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(pE->GetColor().ToUTF8()) + 1];
      strcpy(str, pE->GetColor().ToUTF8());
      xml = sqlite3_mprintf("%s\t\t\t<Value>%s</Value>\r\n", prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      pE = pE->GetNext();
    }
  xml = sqlite3_mprintf("%s\t\t</Categorize>\r\n\t</ColorMap>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (ShadedRelief == true)
    {
      xml = sqlite3_mprintf("%s\t<ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n", prev,
                        ReliefFactor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t</ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerCategorizeDialog::
OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerCategorizeDialog::
OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerCategorizeDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerCategorizeDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

ColorMapInterpolate::~ColorMapInterpolate()
{
// dtor
  ColorMapEntry *pE;
  ColorMapEntry *pEn;
  pE = First;
  while (pE != NULL)
    {
      pEn = pE->GetNext();
      delete pE;
      pE = pEn;
    }
}

void ColorMapInterpolate::Add(double value, wxString & color)
{
// adding a new Entry into the Map by searching the appropriate position
// or updating an already existing Entry
  ColorMapEntry *pE;
  ColorMapEntry *pEnew;
  pE = First;
  while (pE != NULL)
    {
      if (pE->GetValue() == value)
        {
          // updating an already defined Entry
          pE->SetColor(color);
          return;
        }
      pE = pE->GetNext();
    }

// inserting a new Entry
  bool is_first = false;
  if (First == NULL)
    is_first = true;
  else if (value < First->GetValue())
    is_first = true;
  if (is_first == true)
    {
      // inserting at the beginning of the Map
      pEnew = new ColorMapEntry(value, color);
      if (First == NULL)
        {
          First = pEnew;
          Last = pEnew;
          return;
        }
      First->SetPrev(pEnew);
      pEnew->SetNext(First);
      First = pEnew;
      return;
    }

  pE = First;
  while (pE != NULL)
    {
      // searching the appropriate position into the Map
      if (value < pE->GetValue())
        {
          pEnew = new ColorMapEntry(value, color);
          pE->GetPrev()->SetNext(pEnew);
          pEnew->SetPrev(pE->GetPrev());
          pEnew->SetNext(pE);
          pE->SetPrev(pEnew);
          return;
        }
      pE = pE->GetNext();
    }

// last item
  pEnew = new ColorMapEntry(value, color);
  pEnew->SetPrev(Last);
  Last->SetNext(pEnew);
  Last = pEnew;
}

void ColorMapInterpolate::Remove(double value)
{
// removing an Entry from the Map
  ColorMapEntry *pE;
  pE = First;
  while (pE != NULL)
    {
      if (pE->GetValue() == value)
        {
          if (pE == First && pE == Last)
            {
              // removing the unique Entry
              First = NULL;
              Last = NULL;
              delete pE;
          } else if (pE == First)
            {
              // removing the first Entry
              pE->GetNext()->SetPrev(NULL);
              First = pE->GetNext();
              delete pE;
          } else if (pE == Last)
            {
              // removing the last Entry
              pE->GetPrev()->SetNext(NULL);
              Last = pE->GetPrev();
              delete pE;
          } else
            {
              // removing any other Entry
              pE->GetPrev()->SetNext(pE->GetNext());
              pE->GetNext()->SetPrev(pE->GetPrev());
              delete pE;
            }
          break;
        }
      pE = pE->GetNext();
    }
}

bool RasterSymbolizerInterpolateDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Fallback = wxT("#ffffff");
  ShadedRelief = false;
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT
                       ("RasterSymbolizer: Color Map - Interpolate")) == false)
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

void RasterSymbolizerInterpolateDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fourth row: GRID to show the Color Map
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *gridBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Color Map"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxVERTICAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(this, ID_SYMBOLIZER_MAP, wxDefaultPosition, wxSize(280, 250));
  GridCtrl->CreateGrid(1, 3, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Value"));
  GridCtrl->SetColLabelValue(1, wxT("Color"));
  GridCtrl->SetColLabelValue(2, wxT("Sample"));
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// extra Map items
  wxBoxSizer *extraBoxSizer = new wxBoxSizer(wxVERTICAL);
  gridSizer->Add(extraBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *baseBox = new wxStaticBox(this, wxID_STATIC,
                                         wxT("Fallback Color"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *baseSizer = new wxStaticBoxSizer(baseBox, wxHORIZONTAL);
  extraBoxSizer->Add(baseSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *fallbackSizer = new wxBoxSizer(wxHORIZONTAL);
  baseSizer->Add(fallbackSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *fallbackCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_FALLBACK, Fallback,
                   wxDefaultPosition, wxSize(100, 22));
  fallbackSizer->Add(fallbackCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxStaticBox *entryBox = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Color Map Entry"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *entrySizer = new wxStaticBoxSizer(entryBox, wxHORIZONTAL);
  extraBoxSizer->Add(entrySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *entry1Sizer = new wxBoxSizer(wxVERTICAL);
  entrySizer->Add(entry1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxBoxSizer *entry2Sizer = new wxBoxSizer(wxVERTICAL);
  entrySizer->Add(entry2Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *valueSizer = new wxBoxSizer(wxHORIZONTAL);
  entry1Sizer->Add(valueSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *valueLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Value:"));
  valueSizer->Add(valueLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *valueCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_VALUE, wxT("0.00"),
                                         wxDefaultPosition, wxSize(100, 22));
  valueSizer->Add(valueCtrl, 0, wxALIGN_RIGHT | wxALL, 2);
  wxBoxSizer *colorSizer = new wxBoxSizer(wxHORIZONTAL);
  entry1Sizer->Add(colorSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *colorLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Color:"));
  colorSizer->Add(colorLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_COLOR, wxT("#000000"),
                   wxDefaultPosition, wxSize(100, 22));
  colorSizer->Add(colorCtrl, 0, wxALIGN_RIGHT | wxALL, 2);
  wxButton *add = new wxButton(this, ID_SYMBOLIZER_ADD, wxT("&Update Map"));
  entry2Sizer->Add(add, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticBox *pickerBox = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Color Picker"),
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxTE_READONLY);
  wxBoxSizer *pickerSizer = new wxStaticBoxSizer(pickerBox, wxHORIZONTAL);
  extraBoxSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxButton *pick =
    new wxButton(this, ID_SYMBOLIZER_PICKER_BTN, wxT("&Pick a color"));
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *pickerCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_PICKER_HEX, wxT(""),
                   wxDefaultPosition, wxSize(100, 22));
  pickerSizer->Add(pickerCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// sixth row: optional Shaded Relief
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *shadedBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(shadedBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *shadedBox = new wxStaticBox(this, wxID_STATIC,
                                           wxT("Shaded Relief"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *shadedSizer = new wxStaticBoxSizer(shadedBox, wxHORIZONTAL);
  shadedBoxSizer->Add(shadedSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *reliefSizer = new wxBoxSizer(wxVERTICAL);
  shadedSizer->Add(reliefSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxCheckBox *shadedCtrl = new wxCheckBox(this, ID_SYMBOLIZER_SHADED,
                                          wxT("Shaded Relief"),
                                          wxDefaultPosition, wxDefaultSize);
  reliefSizer->Add(shadedCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *relief2Sizer = new wxBoxSizer(wxHORIZONTAL);
  reliefSizer->Add(relief2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *reliefLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Relief Factor:"));
  relief2Sizer->Add(reliefLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSpinCtrl *reliefCtrl = new wxSpinCtrl(this, ID_SYMBOLIZER_RELIEF, wxT(""),
                                          wxDefaultPosition, wxSize(80, 22),
                                          wxSP_ARROW_KEYS,
                                          0, 0, 0);
  reliefCtrl->Enable(false);
  relief2Sizer->Add(reliefCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// seventh row: optional Visibility Range
  miscSizer->AddSpacer(50);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerInterpolateDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerInterpolateDialog::OnCopy);
  Connect(ID_SYMBOLIZER_SHADED, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnShadedChanged);
  Connect(ID_SYMBOLIZER_ADD, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnCmdAdd);
  Connect(ID_SYMBOLIZER_REMOVE, wxEVT_COMMAND_MENU_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnCmdRemove);
  Connect(ID_SYMBOLIZER_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnCmdColorPicker);
  Connect(wxID_ANY, wxEVT_GRID_CELL_RIGHT_CLICK,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnRightClick);
  Connect(wxID_ANY, wxEVT_GRID_SELECT_CELL,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnCellSelected);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerInterpolateDialog::OnCmdScaleChanged);
}

void RasterSymbolizerInterpolateDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

void RasterSymbolizerInterpolateDialog::RefreshGrid()
{
//
// refreshing the Grid
//
  int tot_rows = GridCtrl->GetNumberRows();
  if (tot_rows > 0)
    GridCtrl->DeleteRows(0, tot_rows);
  int count = 0;
  ColorMapEntry *pE = Map.GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->AppendRows(count);
  count = 0;
  char dummy[1024];
  wxString cell;
  pE = Map.GetFirst();
  while (pE)
    {
      // feeding grid rows
      sprintf(dummy, "%1.6f", pE->GetValue());
      cell = wxString::FromUTF8(dummy);
      GridCtrl->SetCellValue(count, 0, cell);
      GridCtrl->SetCellAlignment(count, 0, wxALIGN_RIGHT, wxALIGN_TOP);
      GridCtrl->SetCellValue(count, 1, pE->GetColor());
      wxColour color;
      ColorMapEntry::GetWxColor(pE->GetColor(), color);
      GridCtrl->SetCellBackgroundColour(count, 2, color);
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->AutoSizeColumns();
}

void RasterSymbolizerInterpolateDialog::OnCellSelected(wxGridEvent & event)
{
//
// cell selection changed
//
  CurrentRow = event.GetRow();
  if (CurrentRow == 0)
    return;
  wxString value = GridCtrl->GetCellValue(CurrentRow, 0);
  wxString color = GridCtrl->GetCellValue(CurrentRow, 1);
  wxTextCtrl *valueCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_VALUE);
  valueCtrl->SetValue(value);
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
  colorCtrl->SetValue(color);
}

void RasterSymbolizerInterpolateDialog::OnRightClick(wxGridEvent & event)
{
//
// right click on some cell [mouse action]
//
  wxMenu menu;
  wxMenuItem *menuItem;
  wxPoint pt = event.GetPosition();
  CurrentRow = event.GetRow();
  if (CurrentRow == 0)
    return;
  GridCtrl->SelectRow(CurrentRow);
  wxString value = GridCtrl->GetCellValue(CurrentRow, 0);
  double val;
  value.ToDouble(&val);
  CurrentValue = val;
  menuItem = new wxMenuItem(&menu, ID_SYMBOLIZER_REMOVE, wxT("&Remove Entry"));
  menu.Append(menuItem);
  GridCtrl->PopupMenu(&menu, pt);
}

void RasterSymbolizerInterpolateDialog::
OnCmdAdd(wxCommandEvent & WXUNUSED(event))
{
//
// adding a new Map Entry
//
  wxTextCtrl *valueCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_VALUE);
  wxString value = valueCtrl->GetValue();
  double val;
  if (value.ToDouble(&val) != true)
    {
      wxMessageBox(wxT
                   ("VALUE isn't a valid decimal number !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
  wxString color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(color) != true)
    {
      wxMessageBox(wxT
                   ("COLOR isn't a valid HexRGB color !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return;
    }
  Map.Add(val, color);
  RefreshGrid();
}

void RasterSymbolizerInterpolateDialog::
OnCmdRemove(wxCommandEvent & WXUNUSED(event))
{
//
// removing a Map Entry
//
  Map.Remove(CurrentValue);
  RefreshGrid();
}

void RasterSymbolizerInterpolateDialog::
OnShadedChanged(wxCommandEvent & WXUNUSED(event))
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
      ShadedRelief = true;
  } else
    {
      reliefCtrl->SetRange(0, 0);
      reliefCtrl->SetValue(0);
      reliefCtrl->Enable(false);
      ShadedRelief = false;
    }
}

void RasterSymbolizerInterpolateDialog::
OnCmdColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *pickerCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_PICKER_HEX);
  wxColour clr = wxNullColour;
  wxString str = pickerCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(str) == true)
    ColorMapEntry::GetWxColor(str, clr);
  wxColour color = wxGetColourFromUser(this, clr);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      pickerCtrl->SetValue(str);
    }
}

bool RasterSymbolizerInterpolateDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *fallbackCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FALLBACK);
  Fallback = fallbackCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(Fallback) != true)
    {
      wxMessageBox(wxT
                   ("FALLBACK isn't a valid HexRGB color !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (ShadedRelief == true)
    {
      wxSpinCtrl *reliefCtrl = (wxSpinCtrl *) FindWindow(ID_SYMBOLIZER_RELIEF);
      ReliefFactor = reliefCtrl->GetValue();
    }
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

char *RasterSymbolizerInterpolateDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Fallback.ToUTF8()) + 1];
  strcpy(str, Fallback.ToUTF8());
  xml =
    sqlite3_mprintf
    ("%s\t<ColorMap>\r\n\t\t\t\t<Interpolate fallbackValue=\"%s\">\r\n", prev,
     str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  ColorMapEntry *pE = Map.GetFirst();
  while (pE != NULL)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t<InterpolationPoint>\r\n\t\t\t\t\t\t<Data>%1.6f</Data>\r\n",
         prev, pE->GetValue());
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(pE->GetColor().ToUTF8()) + 1];
      strcpy(str, pE->GetColor().ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t<Value>%s</Value>\r\n\t\t\t\t\t</InterpolationPoint>\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      pE = pE->GetNext();
    }
  xml =
    sqlite3_mprintf("%s\t\t\t\t</Interpolate>\r\n\t\t\t</ColorMap>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (ShadedRelief == true)
    {
      xml = sqlite3_mprintf("%s\t\t\t<ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n",
                        prev, ReliefFactor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerInterpolateDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Fallback.ToUTF8()) + 1];
  strcpy(str, Fallback.ToUTF8());
  xml =
    sqlite3_mprintf
    ("%s\t<ColorMap>\r\n\t\t<Interpolate fallbackValue=\"%s\">\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  ColorMapEntry *pE = Map.GetFirst();
  while (pE != NULL)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t<InterpolationPoint>\r\n\t\t\t\t<Data>%1.6f</Data>\r\n", prev,
         pE->GetValue());
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(pE->GetColor().ToUTF8()) + 1];
      strcpy(str, pE->GetColor().ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<Value>%s</Value>\r\n\t\t\t</InterpolationPoint>\r\n", prev,
         str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      pE = pE->GetNext();
    }
  xml = sqlite3_mprintf("%s\t\t</Interpolate>\r\n\t</ColorMap>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (ShadedRelief == true)
    {
      xml = sqlite3_mprintf("%s\t<ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<ReliefFactor>%1.2f</ReliefFactor>\r\n", prev,
                        ReliefFactor);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t</ShadedRelief>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerInterpolateDialog::
OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerInterpolateDialog::
OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerInterpolateDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerInterpolateDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool RasterSymbolizerMonochromeDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Color = wxT("#000000");
  MinScale = false;
  MaxScale = false;
  if (wxDialog::Create(parent, wxID_ANY,
                       wxT("RasterSymbolizer: Recolored Monochrome")) == false)
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

void RasterSymbolizerMonochromeDialog::CreateControls()
{
//
// creating individual control and setting initial values
//
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
// first row: the RasterSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(this, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the RasterSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the RasterSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(this, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: the RasterSymbolizer Opacity
  wxBoxSizer *opacitySizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacitySizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *opacityLabel =
    new wxStaticText(this, wxID_STATIC, wxT("&Opacity:"));
  opacitySizer->Add(opacityLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxSlider *opacityCtrl = new wxSlider(this, ID_SYMBOLIZER_OPACITY, 100, 0, 100,
                                       wxDefaultPosition, wxSize(600, 45),
                                       wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// fifth row: the Remapped Color
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *remapBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(remapBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *remapBox = new wxStaticBox(this, wxID_STATIC,
                                          wxT("Black remapped Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *remapSizer = new wxStaticBoxSizer(remapBox, wxVERTICAL);
  remapBoxSizer->Add(remapSizer, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *colorSizer = new wxBoxSizer(wxHORIZONTAL);
  remapSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *colorCtrl = new wxTextCtrl(this, ID_SYMBOLIZER_COLOR, Color,
                                         wxDefaultPosition, wxSize(100, 22));
  colorSizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(this, ID_SYMBOLIZER_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  colorSizer->Add(sampleCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *pick =
    new wxButton(this, ID_SYMBOLIZER_PICKER_BTN, wxT("&Pick a color"));
  colorSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
// sixth row: optional Visibility Range
  miscSizer->AddSpacer(75);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxVERTICAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// buttons
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *insert =
    new wxButton(this, ID_SYMBOLIZER_INSERT, wxT("&Insert into DBMS"));
  btnBox->Add(insert, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *exp =
    new wxButton(this, ID_SYMBOLIZER_EXPORT, wxT("&Export to file"));
  btnBox->Add(exp, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxButton *copy = new wxButton(this, ID_SYMBOLIZER_COPY, wxT("&Copy"));
  btnBox->Add(copy, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  btnBox->AddSpacer(100);
  wxButton *ok = new wxButton(this, wxID_OK, wxT("&Quit"));
  btnBox->Add(ok, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// appends event handler for OK button;
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerMonochromeDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerMonochromeDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerMonochromeDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & RasterSymbolizerMonochromeDialog::OnCopy);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          RasterSymbolizerMonochromeDialog::OnCmdScaleChanged);
  Connect(ID_SYMBOLIZER_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          RasterSymbolizerMonochromeDialog::OnCmdColorChanged);
  Connect(ID_SYMBOLIZER_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          RasterSymbolizerMonochromeDialog::OnCmdColorPicker);
}

void RasterSymbolizerMonochromeDialog::
OnCmdColorChanged(wxCommandEvent & WXUNUSED(event))
{
//
// remapped color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_PICKER_HEX);
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

void RasterSymbolizerMonochromeDialog::
OnCmdColorPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
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

bool RasterSymbolizerMonochromeDialog::RetrieveParams()
{
//
// retrieving the RasterSymbolizer params 
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the RasterSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some RasterSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR);
  Color = colorCtrl->GetValue();
  if (ColorMapEntry::IsValidColor(Color) != true)
    {
      wxMessageBox(wxT
                   ("REMAPPED_COLOR isn't a valid HexRGB color !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (MinScale == true)
    {
      wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
      wxString value = minCtrl->GetValue();
      if (value.ToDouble(&MinScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MinScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MIN_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MaxScale == true)
    {
      wxTextCtrl *maxCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MAX_SCALE);
      wxString value = maxCtrl->GetValue();
      if (value.ToDouble(&MaxScaleDenominator) != true)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
      if (MaxScaleDenominator < 0.0)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE must be a positive number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (MinScale == true && MaxScale == true)
    {
      if (MinScaleDenominator >= MaxScaleDenominator)
        {
          wxMessageBox(wxT
                       ("MAX_SCALE is always expected to be greater than MIN_SCALE !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

void RasterSymbolizerMonochromeDialog::
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
        MinScale = false;
        MaxScale = false;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 1:
        MinScale = true;
        MaxScale = false;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT("+Infinite"));
        maxCtrl->Enable(false);
        break;
      case 2:
        MinScale = false;
        MaxScale = true;
        minCtrl->SetValue(wxT("0.0"));
        minCtrl->Enable(false);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
      case 3:
        MinScale = true;
        MaxScale = true;
        minCtrl->SetValue(wxT(""));
        minCtrl->Enable(true);
        maxCtrl->SetValue(wxT(""));
        maxCtrl->Enable(true);
        break;
    };
}

char *RasterSymbolizerMonochromeDialog::DoCreateCoverageXML()
{
//
// creating the SLD/SE (XML) code - CoverageStyle
//
  char *str;
  char *prev;
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
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Rule>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (MinScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MinScaleDenominator>%1.2f</MinScaleDenominator>\r\n", prev,
         MinScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MaxScale == true)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t<MaxScaleDenominator>%1.2f</MaxScaleDenominator>\r\n", prev,
         MaxScaleDenominator);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t<RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t<ColorMap>\r\n\t\t\t\t<Categorize fallbackValue=\"#ffffff\">\r\n",
     prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n",
                    prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t\t\t<Value>#000000</Value>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Color.ToUTF8()) + 1];
  strcpy(str, Color.ToUTF8());
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t\t<Threshold>1</Threshold>\r\n\t\t\t\t\t<Value>%s</Value>\r\n",
     prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t\t</Categorize>\r\n\t\t\t</ColorMap>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</CoverageStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *RasterSymbolizerMonochromeDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - RasterSymbolizer
//
  char *str;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<RasterSymbolizer version=\"1.1.0\" ", prev);
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Name.ToUTF8()) + 1];
  strcpy(str, Name.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Name>%s</Name>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (Title.Len() > 0 || Abstract.Len() > 0)
    {
      xml = sqlite3_mprintf("%s\t<Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (Title.Len() > 0)
        {
          str = new char[strlen(Title.ToUTF8()) + 1];
          strcpy(str, Title.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Title>%s</Title>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      if (Abstract.Len() > 0)
        {
          str = new char[strlen(Abstract.ToUTF8()) + 1];
          strcpy(str, Abstract.ToUTF8());
          xml = sqlite3_mprintf("%s\t\t<Abstract>%s</Abstract>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t</Description>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t<ColorMap>\r\n\t\t<Categorize fallbackValue=\"#ffffff\">\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf("%s\t\t\t<LookupValue>Rasterdata</LookupValue>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Value>#000000</Value>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Color.ToUTF8()) + 1];
  strcpy(str, Color.ToUTF8());
  xml =
    sqlite3_mprintf
    ("%s\t\t\t<Threshold>1</Threshold>\r\n\t\t\t<Value>%s</Value>\r\n", prev,
     str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</Categorize>\r\n\t</ColorMap>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s</RasterSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void RasterSymbolizerMonochromeDialog::
OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the RasterSymbolizer into the DBMS
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertRasterSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE RasterSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerMonochromeDialog::
OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the RasterSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  if (RetrieveParams() == false)
    return;
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE RasterSymbolizer to a file"),
                          wxT(""), Name + wxT(".xml"),
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
          char *xml;
          if (MinScale == true || MaxScale == true)
            xml = DoCreateCoverageXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE RasterSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void RasterSymbolizerMonochromeDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the RasterSymbolizer into the Clipboard 
//
  if (RetrieveParams() == false)
    return;
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateCoverageXML();
  else
    xml = DoCreateSymbolizerXML();
  wxString XMLstring = wxString::FromUTF8(xml);
  sqlite3_free(xml);
  if (wxTheClipboard->Open())
    {
      wxTheClipboard->SetData(new wxTextDataObject(XMLstring));
      wxTheClipboard->Close();
    }
}

void RasterSymbolizerMonochromeDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}
