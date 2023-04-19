/*
/ VectorSymbolizers2.cpp
/ various dialog classes
/
/ version 1.8, 2015 April 21
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
#include "wx/tokenzr.h"
#include "wx/clipbrd.h"

#include <rasterlite2/rasterlite2.h>
#include <rasterlite2/rl2graphics.h>
#include <rasterlite2/rl2svg.h>

SimplePointSymbolizerDialog::SimplePointSymbolizerDialog()
{
// ctor
  StrokeDashArray = NULL;
  List = NULL;
}

SimplePointSymbolizerDialog::~SimplePointSymbolizerDialog()
{
// dtor
  if (StrokeDashArray != NULL)
    delete[]StrokeDashArray;
  if (List)
    delete List;
}

bool SimplePointSymbolizerDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Uom = GUI_UOM_PIXEL;
  MinScale = false;
  MaxScale = false;
  Opacity = 1.0;
  Size = 16.0;
  Rotation = 0.0;
  AnchorPointX = 0.5;
  AnchorPointY = 0.5;
  DisplacementX = 0.0;
  DisplacementY = 0.0;
  OnlyRescaleSVG = true;
  HasGraphic = false;
  EnableFill = true;
  EnableStroke = true;
  StrokeColor = wxT("#000000");
  FillColor = wxT("#808080");
  EnableColorReplacement = false;
  ColorReplacement = wxT("#000000");
  WellKnownMark = RL2_GRAPHIC_MARK_SQUARE;
  StrokeWidth = 1.0;
  StrokeLineJoin = RL2_PEN_JOIN_ROUND;
  StrokeLineCap = RL2_PEN_CAP_ROUND;
  StrokeDashCount = 0;
  StrokeDashOffset = 0.0;
  PreviewBackground = GUI_PREVIEW_BACKGROUND_CHECKERED;
  Crosshair = true;
  List = MainFrame->FindExternalGraphic(false);

  if (wxPropertySheetDialog::Create
      (parent, wxID_ANY, wxT("Simple Point Symbolizer")) == false)
    return false;
  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  wxPanel *positionPage = CreatePositionPage(book);
  book->AddPage(positionPage, wxT("Position"), false);
  wxPanel *graphicPage = CreateGraphicPage(book);
  book->AddPage(graphicPage, wxT("Graphic"), false);
  wxPanel *markPage = CreateMarkPage(book);
  book->AddPage(markPage, wxT("Mark"), false);
  wxPanel *previewPage = CreatePreviewPage(book);
  book->AddPage(previewPage, wxT("Preview"), false);

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & SimplePointSymbolizerDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePointSymbolizerDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePointSymbolizerDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePointSymbolizerDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePointSymbolizerDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

wxPanel *SimplePointSymbolizerDialog::CreateMainPage(wxWindow * parent)
{
//
// creating the MAIN page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MAIN);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the PointSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(panel, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the PointSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the PointSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: UOM and Visibility Range
  boxSizer->AddSpacer(50);
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// fourth row A: Unit Of Measure - UOM
  wxString uom[3];
  uom[0] = wxT("&Pixel");
  uom[1] = wxT("&Metre");
  uom[2] = wxT("&Inch");
  wxRadioBox *uomBox = new wxRadioBox(panel, ID_SYMBOLIZER_UOM,
                                      wxT("&Unit Of Measure"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      uom, 1,
                                      wxRA_SPECIFY_ROWS);
  miscSizer->Add(uomBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  uomBox->SetSelection(0);
// fourth row B: optional Visibility Range
  miscSizer->AddSpacer(50);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// fifth row: Symbol Type
  boxSizer->AddSpacer(25);
  wxBoxSizer *typeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(typeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Graphic");
  type[1] = wxT("&Mark");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_TYPE,
                                       wxT("&Symbol Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  typeSizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  typeBox->SetSelection(1);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_UOM, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdUomChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdScaleChanged);
  Connect(ID_SYMBOLIZER_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdTypeChanged);
  return panel;
}

void SimplePointSymbolizerDialog::
OnCmdUomChanged(wxCommandEvent & WXUNUSED(event))
{
//
// UOM selection changed
//
  wxRadioBox *uomCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_UOM);
  switch (uomCtrl->GetSelection())
    {
      case 1:
        Uom = GUI_UOM_METRE;
        break;
      case 2:
        Uom = GUI_UOM_INCH;
        break;
      default:
        Uom = GUI_UOM_PIXEL;
        break;
    };
}

void SimplePointSymbolizerDialog::
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

void SimplePointSymbolizerDialog::
OnCmdTypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Symbol Type selection changed
//
  wxRadioBox *typeCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_TYPE);
  switch (typeCtrl->GetSelection())
    {
      case 1:
        HasGraphic = false;
        break;
      default:
        HasGraphic = true;
        break;
    };
}

wxPanel *SimplePointSymbolizerDialog::CreatePositionPage(wxWindow * parent)
{
//
// creating the Position page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_POSITION);
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
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row: Size and Rotation
  wxBoxSizer *sizeBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(sizeBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  boxSizer->Add(anchorBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
  return panel;
}

wxPanel *SimplePointSymbolizerDialog::CreateGraphicPage(wxWindow * parent)
{
//
// creating the Graphic page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_GRAPHIC);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);

// GRID to select an External Graphic
  wxBoxSizer *graphicSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(graphicSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  graphicSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(panel, ID_SYMBOLIZER_GRAPHIC, wxDefaultPosition,
               wxSize(500, 250));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl->SetColLabelValue(1, wxT("Title"));
  GridCtrl->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl->SetCellRenderer(count, 0, renderer);
      GridCtrl->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl->Enable(false);
// second row C: Color Replacement
  wxBoxSizer *replacementBoxSizer = new wxBoxSizer(wxVERTICAL);
  GridSizer->Add(replacementBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *replacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Color Replacement"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *replacementSizer =
    new wxStaticBoxSizer(replacementBox, wxVERTICAL);
  replacementBoxSizer->Add(replacementSizer, 0,
                           wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxCheckBox *enableReplacementCtrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_ENABLE_COLOR_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_COLOR_REPLACEMENT,
                   ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_COLOR_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxButton *pick = new wxButton(panel, ID_SYMBOLIZER_REPLACEMENT_PICKER_BTN,
                                wxT("&Pick a color"));
  replacementSizer->Add(pick, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  pick->Enable(false);
// third row: rescaling only SVG symbols
  wxCheckBox *rescaleCtrl = new wxCheckBox(panel, ID_ONLY_RESCALE_SVG_SYMBOLS,
                                           wxT
                                           ("Rescaling applies only to SVG symbols"),
                                           wxDefaultPosition, wxDefaultSize);
  rescaleCtrl->SetValue(true);
  boxSizer->Add(rescaleCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  rescaleCtrl->Enable(false);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_REPLACEMENT_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdColorReplacementPicker);
  Connect(ID_SYMBOLIZER_ENABLE_COLOR_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdEnableReplacementChanged);
  Connect(ID_SYMBOLIZER_COLOR_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdColorReplacementChanged);
  Connect(ID_ONLY_RESCALE_SVG_SYMBOLS, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdOnlyRescaleSVG);
  return panel;
}

void SimplePointSymbolizerDialog::
OnCmdColorReplacementPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR_REPLACEMENT);
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

void SimplePointSymbolizerDialog::
OnCmdEnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Graphic ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_ENABLE_COLOR_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableColorReplacement = true;
  else
    EnableColorReplacement = false;
  RetrieveGraphicPage(false);
  UpdateGraphicPage();
}

void SimplePointSymbolizerDialog::
OnCmdColorReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Graphic Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_COLOR_REPLACEMENT_HEX);
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

void SimplePointSymbolizerDialog::
OnCmdOnlyRescaleSVG(wxCommandEvent & WXUNUSED(event))
{
//
// Rescaling only SVG symbols enable/disable
//
  wxCheckBox *rescaleCtrl =
    (wxCheckBox *) FindWindow(ID_ONLY_RESCALE_SVG_SYMBOLS);
  if (rescaleCtrl->IsChecked() == true)
    OnlyRescaleSVG = true;
  else
    OnlyRescaleSVG = false;
}

wxPanel *SimplePointSymbolizerDialog::CreateMarkPage(wxWindow * parent)
{
//
// creating the Mark page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MARK);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *box1Sizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(box1Sizer, 0, wxALIGN_CENTER | wxALL, 5);
// Well Known Mark Name
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
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  box1Sizer->Add(boxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// first row: Fill
  wxBoxSizer *fillBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(fillBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *fillBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Fill"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *fillSizer = new wxStaticBoxSizer(fillBox, wxHORIZONTAL);
  fillBoxSizer->Add(fillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// first row A: Fill Enable
  wxCheckBox *enableFillCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_FILL_ENABLE,
                                              wxT("Enable"),
                                              wxDefaultPosition, wxDefaultSize);
  enableFillCtrl->SetValue(true);
  fillSizer->Add(enableFillCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// first row B: Fill Color
  wxStaticBox *colorFillBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Fill Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *colorFillSizer = new wxStaticBoxSizer(colorFillBox, wxVERTICAL);
  fillSizer->Add(colorFillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
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
// second row: Stroke
  wxBoxSizer *strokeBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(strokeBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *strokeBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *strokeXSizer = new wxStaticBoxSizer(strokeBox, wxVERTICAL);
  strokeBoxSizer->Add(strokeXSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *strokeSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeXSizer->Add(strokeSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row A: Stroke Enable
  wxCheckBox *enableStrokeCtrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_STROKE_ENABLE,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableStrokeCtrl->SetValue(true);
  strokeSizer->Add(enableStrokeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row B: Stroke Color
  wxStaticBox *colorStrokeBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Stroke Color"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *colorStrokeSizer =
    new wxStaticBoxSizer(colorStrokeBox, wxVERTICAL);
  strokeSizer->Add(colorStrokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
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
// second row C: StrokeWidth
  wxStaticBox *widthBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Width"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *widthSizer = new wxStaticBoxSizer(widthBox, wxVERTICAL);
  strokeSizer->Add(widthSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *widthCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: Stroke-LineJoin, Stroke-LineCap and Stroke-Dasharray 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeXSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row B: LineJoin
  wxString join[3];
  join[0] = wxT("&Mitre");
  join[1] = wxT("&Round");
  join[2] = wxT("&Bevel");
  wxRadioBox *joinBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE_LINEJOIN,
                                       wxT("&Line Join"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       join, 1,
                                       wxRA_SPECIFY_COLS);
  miscSizer->Add(joinBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  joinBox->SetSelection(1);
// third row C: LineCap
  wxString cap[3];
  cap[0] = wxT("&Butt");
  cap[1] = wxT("&Round");
  cap[2] = wxT("&Square");
  wxRadioBox *capBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE_LINECAP,
                                      wxT("&Line Cap"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      cap, 1,
                                      wxRA_SPECIFY_COLS);
  miscSizer->Add(capBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  capBox->SetSelection(1);
// third row D: DashArray and DashOffset
  wxBoxSizer *dashBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(dashBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *dashBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Dashed/Dotted Stroke"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *dashSizer = new wxStaticBoxSizer(dashBox, wxVERTICAL);
  dashBoxSizer->Add(dashSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *dash1Sizer = new wxBoxSizer(wxHORIZONTAL);
  dashSizer->Add(dash1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *dash1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT("Dash Array:"));
  dash1Sizer->Add(dash1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *dashArrayCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE_DASHARRAY, wxT(""),
                   wxDefaultPosition, wxSize(200, 22));
  dash1Sizer->Add(dashArrayCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticText *dash2Label =
    new wxStaticText(panel, wxID_STATIC, wxT("e.g. 10,3,2,3"));
  dash2Label->SetForegroundColour(wxColour(255, 0, 0));
  dash1Sizer->Add(dash2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *dash2Sizer = new wxBoxSizer(wxHORIZONTAL);
  dashSizer->Add(dash2Sizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticText *dashOffsetLabel = new wxStaticText(panel, wxID_STATIC,
                                                   wxT("Dash Offset:"));
  dash2Sizer->Add(dashOffsetLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *dashOffsetCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE_DASHOFFSET, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  dash2Sizer->Add(dashOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_MARK, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdMarkChanged);
  Connect(ID_SYMBOLIZER_FILL_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdFillChanged);
  Connect(ID_SYMBOLIZER_FILL_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdColorFillChanged);
  Connect(ID_SYMBOLIZER_FILL_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdColorFillPicker);
  Connect(ID_SYMBOLIZER_STROKE_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdStrokeChanged);
  Connect(ID_SYMBOLIZER_STROKE_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdColorStrokeChanged);
  Connect(ID_SYMBOLIZER_STROKE_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdColorStrokePicker);
  Connect(ID_SYMBOLIZER_STROKE_LINEJOIN, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdLineJoinChanged);
  Connect(ID_SYMBOLIZER_STROKE_LINECAP, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdLineCapChanged);
  return panel;
}

void SimplePointSymbolizerDialog::
OnCmdMarkChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Mark selection changed
//
  wxRadioBox *markCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MARK);
  switch (markCtrl->GetSelection())
    {
      case 1:
        WellKnownMark = RL2_GRAPHIC_MARK_CIRCLE;
        break;
      case 2:
        WellKnownMark = RL2_GRAPHIC_MARK_TRIANGLE;
        break;
      case 3:
        WellKnownMark = RL2_GRAPHIC_MARK_STAR;
        break;
      case 4:
        WellKnownMark = RL2_GRAPHIC_MARK_CROSS;
        break;
      case 5:
        WellKnownMark = RL2_GRAPHIC_MARK_X;
        break;
      default:
        WellKnownMark = RL2_GRAPHIC_MARK_SQUARE;
        break;
    };
}

void SimplePointSymbolizerDialog::
OnCmdFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill enable/disable 
//
  wxCheckBox *enableCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableFill = true;
  else
    EnableFill = false;
  RetrieveMarkPage(false);
  UpdateMarkPage();
}

void SimplePointSymbolizerDialog::
OnCmdColorFillChanged(wxCommandEvent & WXUNUSED(event))
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

void SimplePointSymbolizerDialog::
OnCmdColorFillPicker(wxCommandEvent & WXUNUSED(event))
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

void SimplePointSymbolizerDialog::
OnCmdStrokeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableStroke = true;
  else
    EnableStroke = false;
  RetrieveMarkPage(false);
  UpdateMarkPage();
}

void SimplePointSymbolizerDialog::
OnCmdColorStrokeChanged(wxCommandEvent & WXUNUSED(event))
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

void SimplePointSymbolizerDialog::
OnCmdColorStrokePicker(wxCommandEvent & WXUNUSED(event))
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

void SimplePointSymbolizerDialog::
OnCmdLineJoinChanged(wxCommandEvent & WXUNUSED(event))
{
//
// LineJoin selection changed
//
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE_LINEJOIN);
  switch (lineJoinCtrl->GetSelection())
    {
      case 0:
        StrokeLineJoin = RL2_PEN_JOIN_MITER;
        break;
      case 2:
        StrokeLineJoin = RL2_PEN_JOIN_BEVEL;
        break;
      default:
        StrokeLineJoin = RL2_PEN_JOIN_ROUND;
        break;
    };
}

void SimplePointSymbolizerDialog::
OnCmdLineCapChanged(wxCommandEvent & WXUNUSED(event))
{
//
// LineCap selection changed
//
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE_LINECAP);
  switch (lineCapCtrl->GetSelection())
    {
      case 0:
        StrokeLineCap = RL2_PEN_CAP_BUTT;
        break;
      case 2:
        StrokeLineCap = RL2_PEN_CAP_SQUARE;
        break;
      default:
        StrokeLineCap = RL2_PEN_CAP_ROUND;
        break;
    };
}

wxPanel *SimplePointSymbolizerDialog::CreatePreviewPage(wxWindow * parent)
{
//
// creating the Preview page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_PREVIEW);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *previewBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(previewBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// creating a control to show the PoinrSymbolizer Preview
  wxStaticBox *previewBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("PointSymbolizer Preview"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *previewSizer = new wxStaticBoxSizer(previewBox, wxVERTICAL);
  previewBoxSizer->Add(previewSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  DrawPreview(500, 300);
  SymbolizerPreview *previewCtrl =
    new SymbolizerPreview(this, panel, ID_SYMBOLIZER_PREVIEW,
                          PreviewBackBitmap, wxSize(500, 300));
  previewSizer->Add(previewCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// Background selector
  wxBoxSizer *extraSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(extraSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString back[3];
  back[0] = wxT("&Checked");
  back[1] = wxT("&White");
  back[2] = wxT("&Black");
  wxRadioBox *backBox = new wxRadioBox(panel, ID_SYMBOLIZER_BACKGROUND,
                                       wxT("&Background"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       back, 1,
                                       wxRA_SPECIFY_COLS);
  extraSizer->Add(backBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  backBox->SetSelection(0);
// Crosshair selector
  wxString crosshair[2];
  crosshair[0] = wxT("&Show");
  crosshair[1] = wxT("&Hide");
  wxRadioBox *crossBox = new wxRadioBox(panel, ID_SYMBOLIZER_CROSSHAIR,
                                        wxT("&Crosshair"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 2,
                                        crosshair, 1,
                                        wxRA_SPECIFY_COLS);
  extraSizer->Add(crossBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  crossBox->SetSelection(0);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_BACKGROUND, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdBackgroundChanged);
  Connect(ID_SYMBOLIZER_CROSSHAIR, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePointSymbolizerDialog::OnCmdCrosshairChanged);
  return panel;
}

void SimplePointSymbolizerDialog::
OnCmdBackgroundChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Preview Background selection changed
//
  wxRadioBox *backCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_BACKGROUND);
  switch (backCtrl->GetSelection())
    {
      case 1:
        PreviewBackground = GUI_PREVIEW_BACKGROUND_WHITE;
        break;
      case 2:
        PreviewBackground = GUI_PREVIEW_BACKGROUND_BLACK;
        break;
      default:
        PreviewBackground = GUI_PREVIEW_BACKGROUND_CHECKERED;
        break;
    };
  UpdatePreviewPage();
}

void SimplePointSymbolizerDialog::
OnCmdCrosshairChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Crosshair selection changed
//
  wxRadioBox *crossCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CROSSHAIR);
  switch (crossCtrl->GetSelection())
    {
      case 1:
        Crosshair = false;
        break;
      default:
        Crosshair = true;
        break;
    };
  UpdatePreviewPage();
}

void SimplePointSymbolizerDialog::DrawPreview(int horz, int vert)
{
//
// drawing a Symbolizer Preview
//
  PreviewBackBitmap.Create(horz, vert);
  wxMemoryDC dc(PreviewBackBitmap);
//
// background filling
//
  wxImage img(24, 24);
  for (int y = 0; y < 24; y++)
    {
      // creating a checked background
      for (int x = 0; x < 24; x++)
        {
          if (y < 12)
            {
              if (x < 12)
                img.SetRGB(x, y, 176, 176, 176);
              else
                img.SetRGB(x, y, 208, 208, 208);
          } else
            {
              if (x < 12)
                img.SetRGB(x, y, 208, 208, 208);
              else
                img.SetRGB(x, y, 176, 176, 176);
            }
        }
    }
  wxBrush stipple(img);
  dc.SetBrush(stipple);
  dc.DrawRectangle(0, 0, horz, vert);
}

void SimplePointSymbolizerDialog::CreateButtons()
{
// 
// adding the common Buttons
//
  wxBoxSizer *topSizer = (wxBoxSizer *) (this->GetSizer());
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
}

bool SimplePointSymbolizerDialog::FinalValidityCheck()
{
//
// last check before generating the SLD/SE Style
//
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the PointSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some PointSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some PointSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  return true;
}

bool SimplePointSymbolizerDialog::RetrieveMainPage()
{
//
// retrieving params from the MAIN page
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
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

void SimplePointSymbolizerDialog::UpdateMainPage()
{
//
// updating the MAIN page
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  nameCtrl->SetValue(Name);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  titleCtrl->SetValue(Title);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  absCtrl->SetValue(Abstract);
  wxRadioBox *uomBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_UOM);
  switch (Uom)
    {
      case GUI_UOM_METRE:
        uomBox->SetSelection(1);
        break;
      case GUI_UOM_INCH:
        uomBox->SetSelection(2);
        break;
      default:
        uomBox->SetSelection(0);
        break;
    };
  wxRadioBox *rangeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MINMAX_SCALE);
  if (MinScale != true && MaxScale != true)
    rangeBox->SetSelection(0);
  else if (MinScale == true && MaxScale != true)
    rangeBox->SetSelection(1);
  else if (MinScale != true && MaxScale == true)
    rangeBox->SetSelection(2);
  else
    rangeBox->SetSelection(3);
  wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
  char dummy[64];
  wxString str;
  if (MinScale == true)
    {
      sprintf(dummy, "%1.2f", MinScaleDenominator);
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
  if (MaxScale == true)
    {
      sprintf(dummy, "%1.2f", MaxScaleDenominator);
      str = wxString::FromUTF8(dummy);
      maxCtrl->SetValue(str);
      maxCtrl->Enable(true);
  } else
    {
      str = wxT("+Infinite");
      maxCtrl->SetValue(str);
      maxCtrl->Enable(false);
    }
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_TYPE);
  if (HasGraphic == false)
    typeBox->SetSelection(1);
  else
    typeBox->SetSelection(0);
}

bool SimplePointSymbolizerDialog::RetrievePositionPage(bool check)
{
//
// retrieving params from the Position page
//
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&Size) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Size < 0.0)
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
  if (value.ToDouble(&Rotation) != true)
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
  if (value.ToDouble(&AnchorPointX) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (AnchorPointX < 0.0 || AnchorPointX > 1.0)
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
  if (value.ToDouble(&AnchorPointY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (AnchorPointY < 0.0 || AnchorPointY > 1.0)
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
  if (value.ToDouble(&DisplacementX) != true)
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
  if (value.ToDouble(&DisplacementY) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("DISPLACEMENT-Y isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  return true;
}

void SimplePointSymbolizerDialog::UpdatePositionPage()
{
//
// updating the Position page
//
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_OPACITY);
  opacityCtrl->SetValue(Opacity * 100.0);
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", Size);
  wxString str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  wxTextCtrl *rotationCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ROTATION);
  sprintf(dummy, "%1.2f", Rotation);
  str = wxString::FromUTF8(dummy);
  rotationCtrl->SetValue(str);
  wxTextCtrl *anchorXCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_X);
  sprintf(dummy, "%1.2f", AnchorPointX);
  str = wxString::FromUTF8(dummy);
  anchorXCtrl->SetValue(str);
  wxTextCtrl *anchorYCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_Y);
  sprintf(dummy, "%1.2f", AnchorPointY);
  str = wxString::FromUTF8(dummy);
  anchorYCtrl->SetValue(str);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_X);
  sprintf(dummy, "%1.2f", DisplacementX);
  str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", DisplacementY);
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
}

bool SimplePointSymbolizerDialog::RetrieveGraphicPage(bool check)
{
//
// retrieving params from the Graphic page
//
  if (HasGraphic == false)
    return true;
  int selCount = 0;
  int selected = -1;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          selected = i;
          selCount++;
        }
    }
  if (selCount < 1)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You must select an External Graphic resource !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (selCount > 1)
    {
      if (check == true)
        {
          wxString msg =
            wxT
            ("You must select just a single External Graphic resource !!!\n");
          msg += wxT("Multiple selection is not supported");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  List->FindByIndex(selected, XLinkHref, MimeType);
  if (EnableColorReplacement == true)
    {
      wxTextCtrl *replacementCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR_REPLACEMENT);
      wxString color = replacementCtrl->GetValue();
      if (ColorMapEntry::IsValidColor(color) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      ColorReplacement = color;
    }
  return true;
}

void SimplePointSymbolizerDialog::UpdateGraphicPage()
{
//
// updating the Graphic page
//
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_ENABLE_COLOR_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_COLOR_REPLACEMENT);
  wxButton *pick =
    (wxButton *) FindWindow(ID_SYMBOLIZER_REPLACEMENT_PICKER_BTN);
  int sel = List->FindByXLinkHref(XLinkHref);
  if (sel >= 0)
    GridCtrl->SelectRow(sel);
  if (EnableColorReplacement == true)
    {
      wxColour color = wxNullColour;
      ColorMapEntry::GetWxColor(ColorReplacement, color);
      if (color.IsOk() == true)
        {
          char hex[16];
          sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                  color.Blue());
          wxString str = wxString::FromUTF8(hex);
          replacementCtrl->SetValue(str);
        }
    }
  wxCheckBox *rescaleCtrl =
    (wxCheckBox *) FindWindow(ID_ONLY_RESCALE_SVG_SYMBOLS);
  if (OnlyRescaleSVG == true)
    rescaleCtrl->SetValue(true);
  else
    rescaleCtrl->SetValue(false);
  if (HasGraphic == true)
    {
      GridCtrl->Enable(true);
      enableReplacement->Enable(true);
      if (EnableColorReplacement == true)
        {
          replacementCtrl->Enable(true);
          pick->Enable(true);
      } else
        {
          replacementCtrl->Enable(false);
          pick->Enable(false);
        }
      rescaleCtrl->Enable(true);
  } else
    {
      GridCtrl->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
      pick->Enable(false);
      rescaleCtrl->Enable(false);
    }
}

bool SimplePointSymbolizerDialog::DoParseDashArray(wxString & str)
{
//
// attempting to parse a Stroke DashArray string
//
  StrokeDashCount = 0;
  if (StrokeDashArray != NULL)
    delete[]StrokeDashArray;
  StrokeDashArray = NULL;
  if (str.Len() == 0)
    return true;
  int count = 0;
  double interval;
  wxStringTokenizer tkz(str, wxT(","));
  while (tkz.HasMoreTokens())
    {
      wxString token = tkz.GetNextToken();
      if (token.ToDouble(&interval) != true)
        return false;
      if (interval <= 0.0)
        return false;
      count++;
    }
  if (count == 0)
    return true;
  double *array;
  StrokeDashCount = count;
  StrokeDashArray = new double[StrokeDashCount];
  array = StrokeDashArray;
  count = 0;
  wxStringTokenizer tkz2(str, wxT(","));
  while (tkz2.HasMoreTokens())
    {
      wxString token = tkz2.GetNextToken();
      token.ToDouble(&interval);
      *(array + count++) = interval;
    }
  return true;
}

void SimplePointSymbolizerDialog::NormalizedDashArray(wxString & str,
                                                      char delimiter)
{
//
// creating a normalized DashArray string
//
  int count;
  double *array;
  count = StrokeDashCount;
  array = StrokeDashArray;
  str = wxT("");
  if (count == 0)
    return;
  for (int i = 0; i < count; i++)
    {
      char dummy[64];
      if (i == 0)
        sprintf(dummy, "%1.2f", *(array + i));
      else if (delimiter == ' ')
        sprintf(dummy, " %1.2f", *(array + i));
      else
        sprintf(dummy, "%c %1.2f", delimiter, *(array + i));
      str += wxString::FromUTF8(dummy);
    }
}

bool SimplePointSymbolizerDialog::RetrieveMarkPage(bool check)
{
//
// retrieving params from the Mark page
//
  if (HasGraphic == true)
    return true;
  if (EnableFill == false && EnableStroke == false)
    {
      wxMessageBox(wxT
                   ("Effectless Mark: both Fill and Stroke are disabled !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (EnableFill == true)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
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
      FillColor = color;
    }
  if (EnableStroke == true)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_COLOR);
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
      StrokeColor = color;
      wxTextCtrl *widthCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_WIDTH);
      wxString value = widthCtrl->GetValue();
      if (value.ToDouble(&StrokeWidth) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-WIDTH isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (StrokeWidth <= 0.0)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-WIDTH must be a positive number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      wxTextCtrl *dashArrayCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_DASHARRAY);
      value = dashArrayCtrl->GetValue();
      if (DoParseDashArray(value) == false)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-DASH-ARRAY: invalid expression !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
      } else
        {
          NormalizedDashArray(value, 0);
          dashArrayCtrl->SetValue(value);
        }
      if (StrokeDashCount == 0)
        StrokeDashOffset = 0.0;
      else
        {
          wxTextCtrl *offsetCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_DASHOFFSET);
          wxString value = offsetCtrl->GetValue();
          if (value.ToDouble(&StrokeDashOffset) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("STROKE-DASH-OFFSET isn't a valid decimal number !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
        }
    }
  return true;
}

void SimplePointSymbolizerDialog::UpdateMarkPage()
{
//
// updating the Mark page
//
  wxRadioBox *markCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MARK);
  switch (WellKnownMark)
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
  wxCheckBox *enableFillBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL_ENABLE);
  if (EnableFill == true)
    enableFillBox->SetValue(true);
  else
    enableFillBox->SetValue(false);
  wxTextCtrl *colorFillCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
  colorFillCtrl->SetValue(FillColor);
  wxButton *pickFill = (wxButton *) FindWindow(ID_SYMBOLIZER_FILL_PICKER_BTN);
  wxCheckBox *enableStrokeBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE_ENABLE);
  if (EnableStroke == true)
    enableStrokeBox->SetValue(true);
  else
    enableStrokeBox->SetValue(false);
  wxTextCtrl *colorStrokeCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_COLOR);
  colorStrokeCtrl->SetValue(StrokeColor);
  wxButton *pickStroke =
    (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE_PICKER_BTN);
  wxTextCtrl *widthCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_WIDTH);
  char dummy[64];
  sprintf(dummy, "%1.2f", StrokeWidth);
  wxString str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE_LINEJOIN);
  switch (StrokeLineJoin)
    {
      case RL2_PEN_JOIN_MITER:
        lineJoinCtrl->SetSelection(0);
        break;
      case RL2_PEN_JOIN_BEVEL:
        lineJoinCtrl->SetSelection(2);
        break;
      default:
        lineJoinCtrl->SetSelection(1);
        break;
    };
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE_LINECAP);
  switch (StrokeLineCap)
    {
      case RL2_PEN_CAP_BUTT:
        lineCapCtrl->SetSelection(0);
        break;
      case RL2_PEN_CAP_SQUARE:
        lineCapCtrl->SetSelection(2);
        break;
      default:
        lineCapCtrl->SetSelection(1);
        break;
    };
  wxTextCtrl *dashArrayCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_DASHARRAY);
  wxString value;
  NormalizedDashArray(value, 0);
  dashArrayCtrl->SetValue(value);
  wxTextCtrl *offsetCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE_DASHOFFSET);
  if (StrokeDashCount == 0)
    offsetCtrl->SetValue(wxT("0.0"));
  else
    {
      char dummy[64];
      sprintf(dummy, "%1.2f", StrokeDashOffset);
      wxString str = wxString::FromUTF8(dummy);
      offsetCtrl->SetValue(str);
    }
  if (HasGraphic == false)
    {
      markCtrl->Enable(true);
      enableFillBox->Enable(true);
      if (EnableFill == false)
        {
          colorFillCtrl->Enable(false);
          pickFill->Enable(false);
      } else
        {
          colorFillCtrl->Enable(true);
          pickFill->Enable(true);
        }
      enableStrokeBox->Enable(true);
      if (EnableStroke == false)
        {
          colorStrokeCtrl->Enable(false);
          pickStroke->Enable(false);
          widthCtrl->Enable(false);
          lineJoinCtrl->Enable(false);
          lineCapCtrl->Enable(false);
          dashArrayCtrl->Enable(false);
          offsetCtrl->Enable(false);
      } else
        {
          colorStrokeCtrl->Enable(true);
          pickStroke->Enable(true);
          widthCtrl->Enable(true);
          lineJoinCtrl->Enable(true);
          lineCapCtrl->Enable(true);
          dashArrayCtrl->Enable(true);
          offsetCtrl->Enable(true);
        }
  } else
    {
      markCtrl->Enable(false);
      enableFillBox->Enable(false);
      colorFillCtrl->Enable(false);
      pickFill->Enable(false);
      enableStrokeBox->Enable(false);
      colorStrokeCtrl->Enable(false);
      pickStroke->Enable(false);
      widthCtrl->Enable(false);
      lineJoinCtrl->Enable(false);
      lineCapCtrl->Enable(false);
      dashArrayCtrl->Enable(false);
      offsetCtrl->Enable(false);
    }
}

bool SimplePointSymbolizerDialog::RetrievePreviewPage()
{
//
// retrieving params from the PREVIEW page
//
  return true;
}

void SimplePointSymbolizerDialog::UpdatePreviewPage()
{
//
// updating the PREVIEW page
//
  wxRadioBox *backCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_BACKGROUND);
  switch (PreviewBackground)
    {
      case GUI_PREVIEW_BACKGROUND_WHITE:
        backCtrl->SetSelection(1);
        break;
      case GUI_PREVIEW_BACKGROUND_BLACK:
        backCtrl->SetSelection(2);
        break;
      default:
        backCtrl->SetSelection(0);
        break;
    };
  wxRadioBox *crossCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CROSSHAIR);
  if (Crosshair == false)
    crossCtrl->SetSelection(1);
  else
    crossCtrl->SetSelection(0);
  rl2GraphicsContextPtr ctx = NULL;
  ctx = rl2_graph_create_context(MainFrame->GetRL2PrivateData(), 500, 300);
  if (ctx == NULL)
    return;
// transparent background initialization
  rl2_graph_set_brush(ctx, 251, 5, 249, 255);
  rl2_graph_draw_rectangle(ctx, -1, -1, 501, 301);

  rl2GraphicsPatternPtr pattern = NULL;
  if (HasGraphic == true)
    {
      // Graphic Symbol
      if (MimeType.Cmp(wxT("image/svg+xml")) == 0)
        {
          // SVG symbol
          pattern =
            rl2_create_pattern_from_external_svg(MainFrame->GetSqlite(),
                                                 XLinkHref.ToUTF8(), Size);
      } else
        {
          // bitmap symbol
          pattern =
            rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                     XLinkHref.ToUTF8(), 0);
        }
      if (pattern != NULL)
        {
          if (EnableColorReplacement)
            {
              // attempting to recolor the External Graphic resource
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(ColorReplacement, color);
              rl2_graph_pattern_recolor(pattern, color.Red(), color.Green(),
                                        color.Blue());
            }
          if (Opacity < 1.0)
            {
              // setting up the required transparency
              double aleph = 255.0 * Opacity;
              if (aleph < 0.0)
                aleph = 0.0;
              if (aleph > 255.0)
                aleph = 255.0;
              unsigned char alpha = aleph;
              rl2_graph_pattern_transparency(pattern, alpha);
            }
          unsigned int width;
          unsigned int height;
          rl2_graph_get_pattern_size(pattern, &width, &height);
          double out_width = width;
          double out_height = height;
          if (OnlyRescaleSVG == false)
            {
              double scale;
              if (width > height)
                scale = Size / (double) width;
              else
                scale = Size / (double) height;
              out_width = (double) width *scale;
              out_height = (double) height *scale;
            }
          rl2_graph_draw_graphic_symbol(ctx, pattern, out_width, out_height,
                                        250.0 + DisplacementX,
                                        150.0 - DisplacementY, Rotation,
                                        AnchorPointX, AnchorPointY);
          rl2_graph_destroy_pattern(pattern);
        }
  } else
    {
      // Mark Symbol
      if (EnableFill == true)
        {
          // preparing a Color-based Brush
          double aleph = 255.0 * Opacity;
          if (aleph < 0.0)
            aleph = 0.0;
          if (aleph > 255.0)
            aleph = 255.0;
          unsigned char alpha = aleph;
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(FillColor, color);
          rl2_graph_set_brush(ctx, color.Red(), color.Green(), color.Blue(),
                              alpha);
        }
      if (EnableStroke == true)
        {
          // preparing a Color-based Pen
          double aleph = 255.0 * Opacity;
          if (aleph < 0.0)
            aleph = 0.0;
          if (aleph > 255.0)
            aleph = 255.0;
          unsigned char alpha = aleph;
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(StrokeColor, color);
          if (StrokeDashCount == 0)
            rl2_graph_set_solid_pen(ctx, color.Red(), color.Green(),
                                    color.Blue(), alpha, StrokeWidth,
                                    StrokeLineCap, StrokeLineJoin);
          else
            rl2_graph_set_dashed_pen(ctx, color.Red(), color.Green(),
                                     color.Blue(), alpha, StrokeWidth,
                                     StrokeLineCap, StrokeLineJoin,
                                     StrokeDashCount, StrokeDashArray,
                                     StrokeDashOffset);
        }
      // drawing the Mark Symbol
      int fill = 1;
      int stroke = 1;
      if (EnableFill == false)
        fill = 0;
      if (EnableStroke == false)
        stroke = 0;
      rl2_graph_draw_mark_symbol(ctx, WellKnownMark, Size,
                                 250.0 + DisplacementX, 150.0 - DisplacementY,
                                 Rotation, AnchorPointX, AnchorPointY, fill,
                                 stroke);
    }

// creating the RGB array
  unsigned char *rgb_array = rl2_graph_get_context_rgb_array(ctx);
  rl2_graph_destroy_context(ctx);
  if (rgb_array == NULL)
    return;
// creating the Preview from RGB array
  wxImage img(500, 300);
  img.SetData(rgb_array);
  wxBitmap bmp(img);
  wxMask *mask = new wxMask(bmp, wxColour(251, 5, 249));
  bmp.SetMask(mask);
  wxBitmap bmp2;
  wxBrush brush;
  wxMemoryDC dc;
  wxBitmap white = wxBitmap(500, 300);
  wxBitmap black = wxBitmap(500, 300);
  switch (PreviewBackground)
    {
      case GUI_PREVIEW_BACKGROUND_WHITE:
        dc.SelectObject(white);
        brush = wxBrush(wxColour(255, 255, 255));
        dc.SetBrush(brush);
        dc.DrawRectangle(0, 0, 500, 300);
        dc.SelectObject(wxNullBitmap);
        bmp2 =
          white.GetSubBitmap(wxRect(0, 0, white.GetWidth(), white.GetHeight()));
        break;
      case GUI_PREVIEW_BACKGROUND_BLACK:
        dc.SelectObject(black);
        brush = wxBrush(wxColour(0, 0, 0));
        dc.SetBrush(brush);
        dc.DrawRectangle(0, 0, 500, 300);
        dc.SelectObject(wxNullBitmap);
        bmp2 =
          black.GetSubBitmap(wxRect(0, 0, black.GetWidth(), black.GetHeight()));
        break;
      default:
        bmp2 =
          PreviewBackBitmap.GetSubBitmap(wxRect
                                         (0, 0, PreviewBackBitmap.GetWidth(),
                                          PreviewBackBitmap.GetHeight()));
        break;
    };
// printing the Preview over the currently selected background
  dc.SelectObject(bmp2);
  dc.DrawBitmap(bmp, 0, 0, true);
  if (Crosshair == true)
    {
      // printing the Crosshair over the preview
      wxColour color = wxColour(255, 255, 255);
      if (PreviewBackground == GUI_PREVIEW_BACKGROUND_BLACK)
        color = wxColour(0, 0, 0);
      dc.SetPen(wxPen(color, 3));
      dc.DrawLine(250 + DisplacementX, 0, 250 + DisplacementX, 300);
      dc.DrawLine(0, 150 - DisplacementY, 500, 150 - DisplacementY);
      color = wxColour(0, 0, 0);
      if (PreviewBackground == GUI_PREVIEW_BACKGROUND_BLACK)
        color = wxColour(255, 255, 255);
      dc.SetPen(wxPen(color, 1));
      dc.DrawLine(250 + DisplacementX, 0, 250 + DisplacementX, 300);
      dc.DrawLine(0, 150 - DisplacementY, 500, 150 - DisplacementY);
      color = wxColour(0, 0, 0);
      if (PreviewBackground == GUI_PREVIEW_BACKGROUND_BLACK)
        color = wxColour(255, 255, 255);
      dc.SetPen(wxPen(color, 5));
      dc.DrawLine(248, 150, 252, 150);
      dc.DrawLine(250, 148, 250, 152);
      color = wxColour(255, 0, 0);
      dc.SetPen(wxPen(color, 3));
      dc.DrawLine(249, 150, 251, 150);
      dc.DrawLine(250, 149, 250, 151);
    }
  dc.SelectObject(wxNullBitmap);
// updating the GUI Preview
  SymbolizerPreview *previewCtrl =
    (SymbolizerPreview *) FindWindow(ID_SYMBOLIZER_PREVIEW);
  previewCtrl->SetBitmap(bmp2);
}

void SimplePointSymbolizerDialog::OnPageChanging(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changing
//
  bool ret;
  switch (event.GetOldSelection())
    {
      case 0:
        ret = RetrieveMainPage();
        break;
      case 1:
        ret = RetrievePositionPage();
        break;
      case 2:
        ret = RetrieveGraphicPage();
        break;
      case 3:
        ret = RetrieveMarkPage();
        break;
      case 4:
        ret = RetrievePreviewPage();
        break;
    };
  if (ret != true)
    event.Veto();
}

void SimplePointSymbolizerDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  switch (event.GetSelection())
    {
      case 0:
        UpdateMainPage();
        break;
      case 1:
        UpdatePositionPage();
        break;
      case 2:
        UpdateGraphicPage();
        break;
      case 3:
        UpdateMarkPage();
        break;
      case 4:
        UpdatePreviewPage();
        break;
    };
}

char *SimplePointSymbolizerDialog::DoCreateFeatureTypeXML()
{
//
// creating the SLD/SE (XML) code - Feature Type
//
  char *str;
  const char *cstr;
  char *prev;
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
  switch (Uom)
    {
      case GUI_UOM_METRE:
        cstr = "http://www.opengeospatial.org/se/units/metre";
        break;
      case GUI_UOM_INCH:
        cstr = "http://www.opengeospatial.org/se/units/inch";
        break;
      default:
        cstr = "http://www.opengeospatial.org/se/units/pixel";
        break;
    };
  xml = sqlite3_mprintf("%s\t\t<PointSymbolizer uom=\"%s\">\r\n", prev, cstr);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Graphic>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasGraphic == true)
    {
      // graphic symbol
      xml = sqlite3_mprintf("%s\t\t\t\t<ExternalGraphic>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(XLinkHref.ToUTF8()) + 1];
      strcpy(str, XLinkHref.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(MimeType.ToUTF8()) + 1];
      strcpy(str, MimeType.ToUTF8());
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Format>%s</Format>\r\n", prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      if (EnableColorReplacement == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t<MapItem>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(ColorReplacement.ToUTF8()) + 1];
          strcpy(str, ColorReplacement.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t</ColorReplacement>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t\t\t</ExternalGraphic>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      // mark symbol
      xml = sqlite3_mprintf("%s\t\t\t\t<Mark>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      switch (WellKnownMark)
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
        sqlite3_mprintf("%s\t\t\t\t\t<WellKnownName>%s</WellKnownName>\r\n",
                        prev, cstr);
      sqlite3_free(prev);
      prev = xml;
      if (EnableFill == true)
        {
          // Mark Fill
          xml = sqlite3_mprintf("%s\t\t\t\t\t<Fill>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(FillColor.ToUTF8()) + 1];
          strcpy(str, FillColor.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t\t\t</Fill>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (EnableStroke == true)
        {
          // Mark Stroke
          xml = sqlite3_mprintf("%s\t\t\t\t\t<Stroke>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(StrokeColor.ToUTF8()) + 1];
          strcpy(str, StrokeColor.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
             prev, StrokeWidth);
          sqlite3_free(prev);
          prev = xml;
          switch (StrokeLineJoin)
            {
              case RL2_PEN_JOIN_MITER:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">mitre</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              case RL2_PEN_JOIN_BEVEL:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">bevel</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              default:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
            };
          switch (StrokeLineCap)
            {
              case RL2_PEN_CAP_BUTT:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">butt</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              case RL2_PEN_CAP_SQUARE:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">square</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              default:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
            };
          if (StrokeDashCount > 0 && StrokeDashArray != NULL)
            {
              wxString dash;
              NormalizedDashArray(dash, ' ');
              str = new char[strlen(dash.ToUTF8()) + 1];
              strcpy(str, dash.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              if (StrokeDashOffset != 0.0)
                {
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                     prev, StrokeDashOffset);
                  sqlite3_free(prev);
                  prev = xml;
                }
            }
          xml = sqlite3_mprintf("%s\t\t\t\t\t</Stroke>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t\t\t</Mark>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Opacity != 1.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MimeType.Cmp(wxT("image/svg+xml")) == 0 || HasGraphic == false)
    {
      xml = sqlite3_mprintf("%s\t\t\t\t<Size>%1.2f</Size>\r\n", prev, Size);
      sqlite3_free(prev);
      prev = xml;
  } else if (OnlyRescaleSVG == false)
    {
      xml = sqlite3_mprintf("%s\t\t\t\t<Size>%1.2f</Size>\r\n", prev, Size);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Rotation != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev, Rotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (AnchorPointX != 0.5 || AnchorPointY != 0.5)
    {
      xml = sqlite3_mprintf("%s\t\t\t\t<AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                        prev, AnchorPointX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                        prev, AnchorPointY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (DisplacementX != 0.0 || DisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s\t\t\t\t<Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, DisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, DisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t\t</Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t\t</Graphic>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</PointSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</FeatureTypeStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *SimplePointSymbolizerDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - PointSymbolizer
//
  char *str;
  const char *cstr;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
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
    sqlite3_mprintf("%sxmlns:xlink=\"http://www.w3.org/1999/xlink\" ", prev);
  sqlite3_free(prev);
  prev = xml;
  switch (Uom)
    {
      case GUI_UOM_METRE:
        cstr = "http://www.opengeospatial.org/se/units/metre";
        break;
      case GUI_UOM_INCH:
        cstr = "http://www.opengeospatial.org/se/units/inch";
        break;
      default:
        cstr = "http://www.opengeospatial.org/se/units/pixel";
        break;
    };
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" uom=\"%s\">\r\n",
     prev, cstr);
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
  xml = sqlite3_mprintf("%s\t<Graphic>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasGraphic == true)
    {
      // graphic symbol
      xml = sqlite3_mprintf("%s\t\t<ExternalGraphic>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(XLinkHref.ToUTF8()) + 1];
      strcpy(str, XLinkHref.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(MimeType.ToUTF8()) + 1];
      strcpy(str, MimeType.ToUTF8());
      xml = sqlite3_mprintf("%s\t\t\t<Format>%s</Format>\r\n", prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      if (EnableColorReplacement == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<ColorReplacement>\r\n\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t<MapItem>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(ColorReplacement.ToUTF8()) + 1];
          strcpy(str, ColorReplacement.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t<Value>%s</Value>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t</MapItem>\r\n\t\t\t\t</Recode>\r\n\t\t\t</ColorReplacement>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</ExternalGraphic>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      // mark symbol
      xml = sqlite3_mprintf("%s\t\t<Mark>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      switch (WellKnownMark)
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
        sqlite3_mprintf("%s\t\t\t<WellKnownName>%s</WellKnownName>\r\n", prev,
                        cstr);
      sqlite3_free(prev);
      prev = xml;
      if (EnableFill == true)
        {
          // Mark Fill
          xml = sqlite3_mprintf("%s\t\t\t<Fill>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(FillColor.ToUTF8()) + 1];
          strcpy(str, FillColor.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t</Fill>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (EnableStroke == true)
        {
          // Mark Stroke
          xml = sqlite3_mprintf("%s\t\t\t<Stroke>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(StrokeColor.ToUTF8()) + 1];
          strcpy(str, StrokeColor.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
             prev, StrokeWidth);
          sqlite3_free(prev);
          prev = xml;
          switch (StrokeLineJoin)
            {
              case RL2_PEN_JOIN_MITER:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t<SvgParameter name=\"stroke-linejoin\">mitre</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              case RL2_PEN_JOIN_BEVEL:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t<SvgParameter name=\"stroke-linejoin\">bevel</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              default:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
            };
          switch (StrokeLineCap)
            {
              case RL2_PEN_CAP_BUTT:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t<SvgParameter name=\"stroke-linecap\">butt</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              case RL2_PEN_CAP_SQUARE:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t<SvgParameter name=\"stroke-linecap\">square</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
              default:
                xml =
                  sqlite3_mprintf
                  ("%s\t\t\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
                   prev);
                sqlite3_free(prev);
                prev = xml;
                break;
            };
          if (StrokeDashCount > 0 && StrokeDashArray != NULL)
            {
              wxString dash;
              NormalizedDashArray(dash, ' ');
              str = new char[strlen(dash.ToUTF8()) + 1];
              strcpy(str, dash.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              if (StrokeDashOffset != 0.0)
                {
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                     prev, StrokeDashOffset);
                  sqlite3_free(prev);
                  prev = xml;
                }
            }
          xml = sqlite3_mprintf("%s\t\t\t</Stroke>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</Mark>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Opacity != 1.0)
    {
      xml =
        sqlite3_mprintf("%s\t\t<Opacity>%1.2f</Opacity>\r\n", prev, Opacity);
      sqlite3_free(prev);
      prev = xml;
    }
  if (MimeType.Cmp(wxT("image/svg+xml")) == 0 || HasGraphic == false)
    {
      xml = sqlite3_mprintf("%s\t\t<Size>%1.2f</Size>\r\n", prev, Size);
      sqlite3_free(prev);
      prev = xml;
  } else if (OnlyRescaleSVG == false)
    {
      xml = sqlite3_mprintf("%s\t\t<Size>%1.2f</Size>\r\n", prev, Size);
      sqlite3_free(prev);
      prev = xml;
    }
  if (Rotation != 0.0)
    {
      xml =
        sqlite3_mprintf("%s\t\t<Rotation>%1.2f</Rotation>\r\n", prev, Rotation);
      sqlite3_free(prev);
      prev = xml;
    }
  if (AnchorPointX != 0.5 || AnchorPointY != 0.5)
    {
      xml = sqlite3_mprintf("%s\t\t<AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n", prev,
                        AnchorPointX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n", prev,
                        AnchorPointY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t</AnchorPoint>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (DisplacementX != 0.0 || DisplacementY != 0.0)
    {
      xml = sqlite3_mprintf("%s\t\t<Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, DisplacementX);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, DisplacementY);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t</Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t</Graphic>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s</PointSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void SimplePointSymbolizerDialog::OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the VectorSymbolizer into the DBMS
//
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrievePositionPage();
        break;
      case 2:
        RetrieveGraphicPage();
        break;
      case 3:
        RetrieveMarkPage();
        break;
      case 4:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateFeatureTypeXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertVectorSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE VectorSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void SimplePointSymbolizerDialog::OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the VectorSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrievePositionPage();
        break;
      case 2:
        RetrieveGraphicPage();
        break;
      case 3:
        RetrieveMarkPage();
        break;
      case 4:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE PointSymbolizer to a file"),
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
            xml = DoCreateFeatureTypeXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE PointSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void SimplePointSymbolizerDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the VectorSymbolizer into the Clipboard 
//
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrievePositionPage();
        break;
      case 2:
        RetrieveGraphicPage();
        break;
      case 3:
        RetrieveMarkPage();
        break;
      case 4:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateFeatureTypeXML();
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

void SimplePointSymbolizerDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

SimpleTextSymbolizerDialog::SimpleTextSymbolizerDialog()
{
// ctor
  List = NULL;
}

SimpleTextSymbolizerDialog::~SimpleTextSymbolizerDialog()
{
// dtor
  if (List)
    delete List;
}

bool SimpleTextSymbolizerDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Uom = GUI_UOM_PIXEL;
  MinScale = false;
  MaxScale = false;
  FontStyle = RL2_FONTSTYLE_NORMAL;
  FontWeight = RL2_FONTWEIGHT_NORMAL;
  FontSize = 10.0;
  PointPlacement = true;
  Rotation = 0.0;
  AnchorPointX = 0.5;
  AnchorPointY = 0.5;
  DisplacementX = 0.0;
  DisplacementY = 0.0;
  PerpendicularOffset = 0.0;
  IsRepeated = false;
  InitialGap = 0.0;
  Gap = 0.0;
  IsAligned = true;
  GeneralizeLine = false;
  HasHalo = false;
  HaloRadius = 1.0;
  HaloColor = wxT("#ffffff");
  HaloOpacity = 1.0;
  FillColor = wxT("#000000");
  FillOpacity = 1.0;
  PreviewBackground = GUI_PREVIEW_BACKGROUND_CHECKERED;
  Crosshair = true;
  ReferenceLine = true;
  List = MainFrame->FindTextFont();

  if (wxPropertySheetDialog::Create
      (parent, wxID_ANY, wxT("Simple Text Symbolizer")) == false)
    return false;
  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  wxPanel *fontPage = CreateFontPage(book);
  book->AddPage(fontPage, wxT("Font"), false);
  wxPanel *placementPage = CreatePlacementPage(book);
  book->AddPage(placementPage, wxT("Placement"), false);
  wxPanel *previewPage = CreatePreviewPage(book);
  book->AddPage(previewPage, wxT("Preview"), false);

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) & SimpleTextSymbolizerDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & SimpleTextSymbolizerDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleTextSymbolizerDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleTextSymbolizerDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleTextSymbolizerDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleTextSymbolizerDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

wxPanel *SimpleTextSymbolizerDialog::CreateMainPage(wxWindow * parent)
{
//
// creating the MAIN page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MAIN);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the TextSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(panel, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the TextSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the TextSymbolizer Abstract
  wxBoxSizer *absSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(absSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *absLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Abstract:"));
  absSizer->Add(absLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *abstractCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_ABSTRACT, wxT(""),
                   wxDefaultPosition, wxSize(600, 60),
                   wxTE_MULTILINE);
  absSizer->Add(abstractCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// fourth row: UOM and Visibility Range
  boxSizer->AddSpacer(50);
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// fourth row A: Unit Of Measure - UOM
  wxString uom[3];
  uom[0] = wxT("&Pixel");
  uom[1] = wxT("&Metre");
  uom[2] = wxT("&Inch");
  wxRadioBox *uomBox = new wxRadioBox(panel, ID_SYMBOLIZER_UOM,
                                      wxT("&Unit Of Measure"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      uom, 1,
                                      wxRA_SPECIFY_ROWS);
  miscSizer->Add(uomBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  uomBox->SetSelection(0);
// fourth row B: optional Visibility Range
  miscSizer->AddSpacer(50);
  wxBoxSizer *visibilityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  miscSizer->Add(visibilityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  visibilitySizer->Add(rangeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  rangeBox->SetSelection(0);
  visibilitySizer->AddSpacer(20);
  wxBoxSizer *scaleBoxSizer = new wxBoxSizer(wxVERTICAL);
  visibilitySizer->Add(scaleBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxBoxSizer *scaleMinSizer = new wxBoxSizer(wxHORIZONTAL);
  scaleBoxSizer->Add(scaleMinSizer, 0, wxALIGN_RIGHT | wxALL, 0);
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
// fifth row: the Label
  boxSizer->AddSpacer(30);
  wxBoxSizer *labelSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(labelSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *labelLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Label:"));
  labelSizer->Add(labelLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *labelCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_LABEL, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  labelSizer->Add(labelCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_UOM, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdUomChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdScaleChanged);
  return panel;
}

void SimpleTextSymbolizerDialog::
OnCmdUomChanged(wxCommandEvent & WXUNUSED(event))
{
//
// UOM selection changed
//
  wxRadioBox *uomCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_UOM);
  switch (uomCtrl->GetSelection())
    {
      case 1:
        Uom = GUI_UOM_METRE;
        break;
      case 2:
        Uom = GUI_UOM_INCH;
        break;
      default:
        Uom = GUI_UOM_PIXEL;
        break;
    };
}

void SimpleTextSymbolizerDialog::
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

wxPanel *SimpleTextSymbolizerDialog::CreateFontPage(wxWindow * parent)
{
//
// creating the Font page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_FONT);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);

// GRID to select a Text Font
  wxBoxSizer *graphicSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(graphicSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  graphicSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Registered Text Fonts"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl =
    new wxGrid(panel, ID_SYMBOLIZER_FONT, wxDefaultPosition, wxSize(500, 250));
  int count = 3;
  TextFont *pF = List->GetFirst();
  while (pF)
    {
      // counting how many lines are there
      count++;
      pF = pF->GetNext();
    }
  GridCtrl->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl->SetColLabelValue(0, wxT("FaceName"));
  GridCtrl->SetColLabelValue(1, wxT("Bold"));
  GridCtrl->SetColLabelValue(2, wxT("Italic"));
  GridCtrl->SetColLabelValue(3, wxT("Sample"));
// insertint the ToyFonts into the list
  count = 0;
  GridCtrl->SetCellValue(count, 0, wxT("ToyFont: serif"));
  MyFontCellRenderer *renderer = new MyFontCellRenderer;
  renderer->SetFontExample(MainFrame->GetRL2PrivateData(), "serif");
  GridCtrl->SetCellRenderer(count, 3, renderer);
  GridCtrl->SetCellBackgroundColour(count, 3, wxColour(255, 255, 255));
  count++;
  GridCtrl->SetCellValue(count, 0, wxT("ToyFont: sans-serif"));
  renderer = new MyFontCellRenderer;
  renderer->SetFontExample(MainFrame->GetRL2PrivateData(), "sans-serif");
  GridCtrl->SetCellRenderer(count, 3, renderer);
  GridCtrl->SetCellBackgroundColour(count, 3, wxColour(255, 255, 255));
  count++;
  GridCtrl->SetCellValue(count, 0, wxT("ToyFont: monospace"));
  renderer = new MyFontCellRenderer;
  renderer->SetFontExample(MainFrame->GetRL2PrivateData(), "monospace");
  GridCtrl->SetCellRenderer(count, 3, renderer);
  GridCtrl->SetCellBackgroundColour(count, 3, wxColour(255, 255, 255));
  count++;
  pF = List->GetFirst();
  while (pF)
    {
      // feeding grid rows - TrueType Fonts
      GridCtrl->SetCellValue(count, 0, pF->GetFacename());
      renderer = new MyFontCellRenderer;
      if (pF->IsBold() == true)
        GridCtrl->SetCellValue(count, 1, wxT("YES"));
      if (pF->IsItalic() == true)
        GridCtrl->SetCellValue(count, 2, wxT("YES"));
      renderer->SetFontExample(pF->GetFontExample());
      GridCtrl->SetCellRenderer(count, 3, renderer);
      GridCtrl->SetCellBackgroundColour(count, 3, wxColour(255, 255, 255));
      count++;
      pF = pF->GetNext();
    }
  GridCtrl->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl->AutoSize();
  GridCtrl->EnableEditing(false);
  gridSizer->Add(GridCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: Halo 
  wxBoxSizer *haloBoxSizer = new wxBoxSizer(wxVERTICAL);
  GridSizer->Add(haloBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *haloBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Halo"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *haloSizer = new wxStaticBoxSizer(haloBox, wxVERTICAL);
  haloBoxSizer->Add(haloSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *halo0Sizer = new wxBoxSizer(wxHORIZONTAL);
  haloSizer->Add(halo0Sizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxCheckBox *enableHaloCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_HALO_ENABLE,
                                              wxT("Enable"),
                                              wxDefaultPosition, wxDefaultSize);
  enableHaloCtrl->SetValue(false);
  halo0Sizer->Add(enableHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Halo Radius
  wxStaticBox *radiusBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Radius"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *radiusSizer = new wxStaticBoxSizer(radiusBox, wxVERTICAL);
  halo0Sizer->Add(radiusSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxTextCtrl *radiusCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_HALO_RADIUS, wxT("1.0"),
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
    new wxSlider(panel, ID_SYMBOLIZER_HALO_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(130, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityHaloSizer->Add(opacityHaloCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  opacityHaloCtrl->Enable(false);
// Halo color
  wxStaticBox *haloColorBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *haloColorSizer = new wxStaticBoxSizer(haloColorBox, wxVERTICAL);
  haloSizer->Add(haloColorSizer, 0, wxALIGN_RIGHT | wxALL, 2);
  wxBoxSizer *halo2Sizer = new wxBoxSizer(wxHORIZONTAL);
  haloColorSizer->Add(halo2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *haloCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_HALO_COLOR,
                                        HaloColor,
                                        wxDefaultPosition, wxSize(80, 22));
  halo2Sizer->Add(haloCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  haloCtrl->Enable(false);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleHaloCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_HALO_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  halo2Sizer->Add(sampleHaloCtrl, 0, wxALIGN_RIGHT | wxALL, 2);
  wxButton *pickHalo = new wxButton(panel, ID_SYMBOLIZER_HALO_PICKER_BTN,
                                    wxT("&Pick a color"));
  haloColorSizer->Add(pickHalo, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  pickHalo->Enable(false);
// third row: Size and Fill 
  wxBoxSizer *fontSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(fontSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *font0Sizer = new wxBoxSizer(wxHORIZONTAL);
  fontSizer->Add(font0Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Font Size
  wxStaticBox *sizeBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Size"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *sizeSizer = new wxStaticBoxSizer(sizeBox, wxVERTICAL);
  font0Sizer->Add(sizeSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *sizeCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_SIZE, wxT("15.0"),
                                        wxDefaultPosition, wxSize(100, 22));
  sizeSizer->Add(sizeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// Font Opacity
  wxStaticBox *opacityFontBox = new wxStaticBox(panel, wxID_STATIC,
                                                wxT("Opacity"),
                                                wxDefaultPosition,
                                                wxDefaultSize);
  wxBoxSizer *opacityFontSizer =
    new wxStaticBoxSizer(opacityFontBox, wxVERTICAL);
  fontSizer->Add(opacityFontSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxSlider *opacityFontCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FONT_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(250, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityFontSizer->Add(opacityFontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// Font color
  wxStaticBox *fontColorBox = new wxStaticBox(panel, wxID_STATIC,
                                              wxT("Color"),
                                              wxDefaultPosition,
                                              wxDefaultSize);
  wxBoxSizer *fontColorSizer = new wxStaticBoxSizer(fontColorBox, wxHORIZONTAL);
  fontSizer->Add(fontColorSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *font2Sizer = new wxBoxSizer(wxHORIZONTAL);
  fontColorSizer->Add(font2Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *fontCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_FILL_COLOR,
                                        FillColor,
                                        wxDefaultPosition, wxSize(80, 22));
  font2Sizer->Add(fontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBitmap *sampleFontCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  font2Sizer->Add(sampleFontCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxButton *pickFont = new wxButton(panel, ID_SYMBOLIZER_FILL_PICKER_BTN,
                                    wxT("&Pick a color"));
  fontColorSizer->Add(pickFont, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_FILL_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdColorFillPicker);
  Connect(ID_SYMBOLIZER_FILL_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdColorFillChanged);
  Connect(ID_SYMBOLIZER_HALO_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdColorHaloPicker);
  Connect(ID_SYMBOLIZER_HALO_ENABLE,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdHaloEnableChanged);
  Connect(ID_SYMBOLIZER_HALO_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdColorHaloChanged);
  return panel;
}

void SimpleTextSymbolizerDialog::
OnCmdColorFillPicker(wxCommandEvent & WXUNUSED(event))
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

void SimpleTextSymbolizerDialog::
OnCmdColorHaloPicker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO_COLOR);
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

void SimpleTextSymbolizerDialog::
OnCmdHaloEnableChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Graphic Halo enable/disable 
//
  wxCheckBox *enableCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_HALO_ENABLE);
  if (enableCtrl->IsChecked() == true)
    HasHalo = true;
  else
    HasHalo = false;
  RetrieveFontPage(false);
  UpdateFontPage();
}

void SimpleTextSymbolizerDialog::
OnCmdColorFillChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill Color changed: updating the visual sample
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

void SimpleTextSymbolizerDialog::
OnCmdColorHaloChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Halo Color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_HALO_PICKER_HEX);
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

wxPanel *SimpleTextSymbolizerDialog::CreatePlacementPage(wxWindow * parent)
{
//
// creating the Placement page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_PLACEMENT);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(mainSizer, 0, wxALIGN_CENTER | wxALL, 0);
  wxBoxSizer *auxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(auxSizer, 0, wxALIGN_CENTER | wxALL, 0);
// block 0: Placement Type
  wxBoxSizer *typeSizer = new wxBoxSizer(wxHORIZONTAL);
  mainSizer->Add(typeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Point");
  type[1] = wxT("&Line");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_TYPE,
                                       wxT("&Placement"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  typeSizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
// first row: PointPlacement
  wxBoxSizer *pointBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(pointBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *pointBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Point Placement"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *pointSizer = new wxStaticBoxSizer(pointBox, wxHORIZONTAL);
  pointBoxSizer->Add(pointSizer, 0, wxALIGN_LEFT | wxALL, 5);
// first row A: Anchor Point
  wxStaticBox *anchorBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Anchor Point"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *anchorSizer = new wxStaticBoxSizer(anchorBox, wxVERTICAL);
  pointSizer->Add(anchorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
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
// first row B: Displacement
  wxStaticBox *displacementBox = new wxStaticBox(panel, wxID_STATIC,
                                                 wxT("Displacement"),
                                                 wxDefaultPosition,
                                                 wxDefaultSize);
  wxBoxSizer *displacementSizer =
    new wxStaticBoxSizer(displacementBox, wxVERTICAL);
  pointSizer->Add(displacementSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
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
// first row C: Rotation
  wxStaticBox *rotBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Rotation"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *rotSizer = new wxStaticBoxSizer(rotBox, wxVERTICAL);
  pointSizer->Add(rotSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 20);
  wxBoxSizer *rot1Sizer = new wxBoxSizer(wxHORIZONTAL);
  rotSizer->Add(rot1Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxTextCtrl *rotCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_ROTATION, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  rot1Sizer->Add(rotCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row: LinePlacement
  wxBoxSizer *lineBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(lineBoxSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticBox *lineBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("Line Placement"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *lineSizer = new wxStaticBoxSizer(lineBox, wxVERTICAL);
  lineBoxSizer->Add(lineSizer, 0, wxALIGN_LEFT | wxALL, 5);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_PERPENDICULAR, wxT("0.0"),
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
  lineSizer->Add(repeatedBoxSizer, 0, wxALIGN_LEFT | wxALL, 0);
  wxStaticBox *repeatedBox = new wxStaticBox(panel, wxID_STATIC,
                                             wxT("Repeated Label"),
                                             wxDefaultPosition,
                                             wxDefaultSize);
  wxBoxSizer *repeatedSizer = new wxStaticBoxSizer(repeatedBox, wxHORIZONTAL);
  repeatedBoxSizer->Add(repeatedSizer, 0, wxALIGN_LEFT | wxALL, 5);
  wxCheckBox *isRepeatedCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_IS_REPEATED,
                                              wxT("is Repeated"),
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_INITIAL_GAP, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  inigapCtrl->Enable(false);
  inigapSizer->Add(inigapCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxStaticBox *gapBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Gap"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *gapSizer = new wxStaticBoxSizer(gapBox, wxVERTICAL);
  repeatedSizer->Add(gapSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxTextCtrl *gapCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_GAP, wxT("0.0"),
                                       wxDefaultPosition, wxSize(100, 22));
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
  wxCheckBox *isAlignedCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_IS_ALIGNED,
                                             wxT("is Aligned"),
                                             wxDefaultPosition, wxDefaultSize);
  isAlignedCtrl->SetValue(false);
  isAlignedCtrl->Enable(false);
  optSizer->Add(isAlignedCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *generalizeCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_GENERALIZE,
                                              wxT("Generalize Line"),
                                              wxDefaultPosition, wxDefaultSize);
  generalizeCtrl->SetValue(false);
  generalizeCtrl->Enable(false);
  optSizer->Add(generalizeCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdTypeChanged);
  Connect(ID_SYMBOLIZER_IS_REPEATED,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdIsRepeatedChanged);
  Connect(ID_SYMBOLIZER_IS_ALIGNED,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdIsAlignedChanged);
  Connect(ID_SYMBOLIZER_GENERALIZE,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdGeneralizeLineChanged);
  return panel;
}

void SimpleTextSymbolizerDialog::
OnCmdTypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Symbol Type selection changed
//
  wxRadioBox *typeCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_TYPE);
  switch (typeCtrl->GetSelection())
    {
      case 1:
        PointPlacement = false;
        break;
      default:
        PointPlacement = true;
        break;
    };
  RetrievePlacementPage(false);
  UpdatePlacementPage();
}

void SimpleTextSymbolizerDialog::
OnCmdIsRepeatedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Label IsRepeated enable/disable 
//
  wxCheckBox *repeatedCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_IS_REPEATED);
  if (repeatedCtrl->IsChecked() == true)
    IsRepeated = true;
  else
    IsRepeated = false;
  RetrievePlacementPage(false);
  UpdatePlacementPage();
}

void SimpleTextSymbolizerDialog::
OnCmdIsAlignedChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Label IsAligned enable/disable 
//
  wxCheckBox *alignedCtrl = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_IS_ALIGNED);
  if (alignedCtrl->IsChecked() == true)
    IsAligned = true;
  else
    IsAligned = false;
}

void SimpleTextSymbolizerDialog::
OnCmdGeneralizeLineChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Label GeneralizeLine enable/disable 
//
  wxCheckBox *generalizeCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_GENERALIZE);
  if (generalizeCtrl->IsChecked() == true)
    GeneralizeLine = true;
  else
    GeneralizeLine = false;
}

wxPanel *SimpleTextSymbolizerDialog::CreatePreviewPage(wxWindow * parent)
{
//
// creating the Preview page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_PREVIEW);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
  wxBoxSizer *previewBoxSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(previewBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// creating a control to show the TextSymbolizer Preview
  wxStaticBox *previewBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("TextSymbolizer Preview"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *previewSizer = new wxStaticBoxSizer(previewBox, wxVERTICAL);
  previewBoxSizer->Add(previewSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  DrawPreview(500, 300);
  SymbolizerPreview *previewCtrl =
    new SymbolizerPreview(this, panel, ID_SYMBOLIZER_PREVIEW,
                          PreviewBackBitmap, wxSize(500, 300));
  previewSizer->Add(previewCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// Background selector
  wxBoxSizer *extraSizer = new wxBoxSizer(wxVERTICAL);
  boxSizer->Add(extraSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxString back[3];
  back[0] = wxT("&Checked");
  back[1] = wxT("&White");
  back[2] = wxT("&Black");
  wxRadioBox *backBox = new wxRadioBox(panel, ID_SYMBOLIZER_BACKGROUND,
                                       wxT("&Background"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       back, 1,
                                       wxRA_SPECIFY_COLS);
  extraSizer->Add(backBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  backBox->SetSelection(0);
// Crosshair selector
  wxString crosshair[2];
  crosshair[0] = wxT("&Show");
  crosshair[1] = wxT("&Hide");
  wxRadioBox *crossBox = new wxRadioBox(panel, ID_SYMBOLIZER_CROSSHAIR,
                                        wxT("&Crosshair"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 2,
                                        crosshair, 1,
                                        wxRA_SPECIFY_COLS);
  extraSizer->Add(crossBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  crossBox->SetSelection(0);
// Reference Line selector
  wxString refline[2];
  refline[0] = wxT("&Show");
  refline[1] = wxT("&Hide");
  wxRadioBox *reflineBox = new wxRadioBox(panel, ID_SYMBOLIZER_REFLINE,
                                          wxT("&Test Line"),
                                          wxDefaultPosition,
                                          wxDefaultSize, 2,
                                          refline, 1,
                                          wxRA_SPECIFY_COLS);
  extraSizer->Add(reflineBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  reflineBox->SetSelection(0);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_BACKGROUND, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdBackgroundChanged);
  Connect(ID_SYMBOLIZER_CROSSHAIR, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdCrosshairChanged);
  Connect(ID_SYMBOLIZER_REFLINE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleTextSymbolizerDialog::OnCmdReferenceLineChanged);
  return panel;
}

void SimpleTextSymbolizerDialog::
OnCmdBackgroundChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Preview Background selection changed
//
  wxRadioBox *backCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_BACKGROUND);
  switch (backCtrl->GetSelection())
    {
      case 1:
        PreviewBackground = GUI_PREVIEW_BACKGROUND_WHITE;
        break;
      case 2:
        PreviewBackground = GUI_PREVIEW_BACKGROUND_BLACK;
        break;
      default:
        PreviewBackground = GUI_PREVIEW_BACKGROUND_CHECKERED;
        break;
    };
  UpdatePreviewPage();
}

void SimpleTextSymbolizerDialog::
OnCmdCrosshairChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Preview Background selection changed
//
  wxRadioBox *crossCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CROSSHAIR);
  switch (crossCtrl->GetSelection())
    {
      case 1:
        Crosshair = false;
        break;
      default:
        Crosshair = true;
        break;
    };
  UpdatePreviewPage();
}

void SimpleTextSymbolizerDialog::
OnCmdReferenceLineChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Test Line selection changed
//
  wxRadioBox *reflineCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_REFLINE);
  switch (reflineCtrl->GetSelection())
    {
      case 1:
        ReferenceLine = false;
        break;
      default:
        ReferenceLine = true;
        break;
    };
  UpdatePreviewPage();
}

void SimpleTextSymbolizerDialog::DrawPreview(int horz, int vert)
{
//
// drawing a Symbolizer Preview
//
  PreviewBackBitmap.Create(horz, vert);
  wxMemoryDC dc(PreviewBackBitmap);
//
// background filling
//
  wxImage img(24, 24);
  for (int y = 0; y < 24; y++)
    {
      // creating a checked background
      for (int x = 0; x < 24; x++)
        {
          if (y < 12)
            {
              if (x < 12)
                img.SetRGB(x, y, 176, 176, 176);
              else
                img.SetRGB(x, y, 208, 208, 208);
          } else
            {
              if (x < 12)
                img.SetRGB(x, y, 208, 208, 208);
              else
                img.SetRGB(x, y, 176, 176, 176);
            }
        }
    }
  wxBrush stipple(img);
  dc.SetBrush(stipple);
  dc.DrawRectangle(0, 0, horz, vert);
}

void SimpleTextSymbolizerDialog::CreateButtons()
{
// 
// adding the common Buttons
//
  wxBoxSizer *topSizer = (wxBoxSizer *) (this->GetSizer());
  wxBoxSizer *btnBox = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
}

bool SimpleTextSymbolizerDialog::FinalValidityCheck()
{
//
// last check before generating the SLD/SE Style
//
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the TextSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (Label.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the TextSymbolizer LABEL !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some TextSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some TextSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  return true;
}

bool SimpleTextSymbolizerDialog::RetrieveMainPage()
{
//
// retrieving params from the MAIN page
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  Name = nameCtrl->GetValue();
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  Title = titleCtrl->GetValue();
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  Abstract = absCtrl->GetValue();
  wxTextCtrl *labelCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_LABEL);
  Label = labelCtrl->GetValue();
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

void SimpleTextSymbolizerDialog::UpdateMainPage()
{
//
// updating the MAIN page
//
  wxTextCtrl *nameCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_NAME);
  nameCtrl->SetValue(Name);
  wxTextCtrl *titleCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_TITLE);
  titleCtrl->SetValue(Title);
  wxTextCtrl *absCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ABSTRACT);
  absCtrl->SetValue(Abstract);
  wxTextCtrl *labelCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_LABEL);
  labelCtrl->SetValue(Label);
  wxRadioBox *uomBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_UOM);
  switch (Uom)
    {
      case GUI_UOM_METRE:
        uomBox->SetSelection(1);
        break;
      case GUI_UOM_INCH:
        uomBox->SetSelection(2);
        break;
      default:
        uomBox->SetSelection(0);
        break;
    };
  wxRadioBox *rangeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_MINMAX_SCALE);
  if (MinScale != true && MaxScale != true)
    rangeBox->SetSelection(0);
  else if (MinScale == true && MaxScale != true)
    rangeBox->SetSelection(1);
  else if (MinScale != true && MaxScale == true)
    rangeBox->SetSelection(2);
  else
    rangeBox->SetSelection(3);
  wxTextCtrl *minCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_MIN_SCALE);
  char dummy[64];
  wxString str;
  if (MinScale == true)
    {
      sprintf(dummy, "%1.2f", MinScaleDenominator);
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
  if (MaxScale == true)
    {
      sprintf(dummy, "%1.2f", MaxScaleDenominator);
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

bool SimpleTextSymbolizerDialog::RetrieveFontPage(bool check)
{
//
// retrieving params from the FONT page
//
  int selCount = 0;
  int selected = -1;
  for (int i = 0; i < GridCtrl->GetNumberRows(); i++)
    {
      if (GridCtrl->IsInSelection(i, 0) == true)
        {
          selected = i;
          selCount++;
        }
    }
  if (selCount < 1)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("You must select some Text Font !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (selCount > 1)
    {
      if (check == true)
        {
          wxString msg = wxT("You must select just a single TextFont !!!\n");
          msg += wxT("Multiple selection is not supported");
          wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (selected < 3)
    {
      // Toy Fonts
      switch (selected)
        {
          case 0:
            FontFamily = wxT("serif");
            break;
          case 1:
            FontFamily = wxT("sans-serif");
            break;
          case 2:
            FontFamily = wxT("monospace");
            break;
        };
      FontStyle = RL2_FONTSTYLE_NORMAL;
      FontWeight = RL2_FONTWEIGHT_NORMAL;
  } else
    List->FindByIndex(selected, FontFamily, &FontStyle, &FontWeight);
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_SIZE);
  wxString value = sizeCtrl->GetValue();
  if (value.ToDouble(&FontSize) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-SIZE isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (FontSize <= 0.0)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("FONT-SIZE should be a positive numberr !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  wxSlider *opacityFontCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FONT_OPACITY);
  FillOpacity = opacityFontCtrl->GetValue() / 100.0;
  wxTextCtrl *colorFillCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
  wxString color = colorFillCtrl->GetValue();
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
  FillColor = color;

  if (HasHalo == true)
    {
      wxSlider *opacityHaloCtrl =
        (wxSlider *) FindWindow(ID_SYMBOLIZER_HALO_OPACITY);
      HaloOpacity = opacityHaloCtrl->GetValue() / 100.0;
      wxTextCtrl *radiusCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO_RADIUS);
      wxString value = radiusCtrl->GetValue();
      if (value.ToDouble(&HaloRadius) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("HALO-RADIUS isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (HaloRadius <= 0.0)
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
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO_COLOR);
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
      HaloColor = color;
    }
  return true;
}

void SimpleTextSymbolizerDialog::UpdateFontPage()
{
//
// updating the FONT page
//
  wxSlider *opacityFontCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_FONT_OPACITY);
  opacityFontCtrl->SetValue(FillOpacity * 100.0);
  wxTextCtrl *colorFontCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL_COLOR);
  wxColour color = wxNullColour;
  ColorMapEntry::GetWxColor(FillColor, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorFontCtrl->SetValue(str);
    }
  wxTextCtrl *sizeCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_SIZE);
  char dummy[64];
  sprintf(dummy, "%1.2f", FontSize);
  wxString str = wxString::FromUTF8(dummy);
  sizeCtrl->SetValue(str);
  wxCheckBox *enableHaloBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_HALO_ENABLE);
  if (HasHalo == true)
    enableHaloBox->SetValue(true);
  else
    enableHaloBox->SetValue(false);
  wxTextCtrl *radiusCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO_RADIUS);
  wxTextCtrl *colorHaloCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_HALO_COLOR);
  wxButton *pickHalo = (wxButton *) FindWindow(ID_SYMBOLIZER_HALO_PICKER_BTN);
  wxSlider *opacityHaloCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_HALO_OPACITY);
  opacityHaloCtrl->SetValue(HaloOpacity * 100.0);
  sprintf(dummy, "%1.2f", HaloRadius);
  str = wxString::FromUTF8(dummy);
  radiusCtrl->SetValue(str);
  color = wxNullColour;
  ColorMapEntry::GetWxColor(HaloColor, color);
  if (color.IsOk() == true)
    {
      char hex[16];
      sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(), color.Blue());
      wxString str = wxString::FromUTF8(hex);
      colorHaloCtrl->SetValue(str);
    }
  if (HasHalo == false)
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

bool SimpleTextSymbolizerDialog::RetrievePlacementPage(bool check)
{
//
// retrieving params from the Placement page
//
  if (PointPlacement == true)
    {
      // Point Placement
      wxTextCtrl *rotationCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ROTATION);
      wxString value = rotationCtrl->GetValue();
      if (value.ToDouble(&Rotation) != true)
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
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_X);
      value = anchorXCtrl->GetValue();
      if (value.ToDouble(&AnchorPointX) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("ANCHOR-POINT-X isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (AnchorPointX < 0.0 || AnchorPointX > 1.0)
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
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_Y);
      value = anchorYCtrl->GetValue();
      if (value.ToDouble(&AnchorPointY) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("ANCHOR-POINT-Y isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (AnchorPointY < 0.0 || AnchorPointY > 1.0)
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
      if (value.ToDouble(&DisplacementX) != true)
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
      if (value.ToDouble(&DisplacementY) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("DISPLACEMENT-Y isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
  } else
    {
      // Line Placement
      wxTextCtrl *perpCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_PERPENDICULAR);
      wxString value = perpCtrl->GetValue();
      if (value.ToDouble(&PerpendicularOffset) != true)
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
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_INITIAL_GAP);
      value = inigapCtrl->GetValue();
      if (value.ToDouble(&InitialGap) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("INITIAL-GAP isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (InitialGap < 0.0)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("INITIAL-GAP should be a positive number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      wxTextCtrl *gapCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_GAP);
      value = gapCtrl->GetValue();
      if (value.ToDouble(&Gap) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("GAP isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
      if (Gap < 0.0)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("GAP should be a positive number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
    }
  return true;
}

void SimpleTextSymbolizerDialog::UpdatePlacementPage()
{
//
// updating the Placement page
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_TYPE);
  if (PointPlacement == false)
    typeBox->SetSelection(1);
  else
    typeBox->SetSelection(0);
  if (PointPlacement == true)
    {
      // Point Placement
      wxTextCtrl *rotationCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ROTATION);
      char dummy[64];
      sprintf(dummy, "%1.2f", Rotation);
      wxString str = wxString::FromUTF8(dummy);
      rotationCtrl->SetValue(str);
      rotationCtrl->Enable(true);
      wxTextCtrl *anchorXCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_X);
      sprintf(dummy, "%1.2f", AnchorPointX);
      str = wxString::FromUTF8(dummy);
      anchorXCtrl->SetValue(str);
      anchorXCtrl->Enable(true);
      wxTextCtrl *anchorYCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_Y);
      sprintf(dummy, "%1.2f", AnchorPointY);
      str = wxString::FromUTF8(dummy);
      anchorYCtrl->SetValue(str);
      anchorYCtrl->Enable(true);
      wxTextCtrl *displXCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_X);
      sprintf(dummy, "%1.2f", DisplacementX);
      str = wxString::FromUTF8(dummy);
      displXCtrl->SetValue(str);
      displXCtrl->Enable(true);
      wxTextCtrl *displYCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_Y);
      sprintf(dummy, "%1.2f", DisplacementY);
      str = wxString::FromUTF8(dummy);
      displYCtrl->SetValue(str);
      displYCtrl->Enable(true);
      wxTextCtrl *perpCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_PERPENDICULAR);
      perpCtrl->Enable(false);
      wxCheckBox *repeatBox =
        (wxCheckBox *) FindWindow(ID_SYMBOLIZER_IS_REPEATED);
      repeatBox->Enable(false);
      wxTextCtrl *inigapCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_INITIAL_GAP);
      inigapCtrl->Enable(false);
      wxTextCtrl *gapCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_GAP);
      gapCtrl->Enable(false);
      wxCheckBox *alignBox =
        (wxCheckBox *) FindWindow(ID_SYMBOLIZER_IS_ALIGNED);
      alignBox->Enable(false);
      wxCheckBox *generalizeBox =
        (wxCheckBox *) FindWindow(ID_SYMBOLIZER_GENERALIZE);
      generalizeBox->Enable(false);
  } else
    {
      // Line Placement
      wxTextCtrl *perpCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_PERPENDICULAR);
      char dummy[64];
      sprintf(dummy, "%1.2f", PerpendicularOffset);
      wxString str = wxString::FromUTF8(dummy);
      perpCtrl->SetValue(str);
      perpCtrl->Enable(true);
      wxCheckBox *repeatBox =
        (wxCheckBox *) FindWindow(ID_SYMBOLIZER_IS_REPEATED);
      if (IsRepeated == true)
        repeatBox->SetValue(true);
      else
        repeatBox->SetValue(false);
      repeatBox->Enable(true);
      wxTextCtrl *inigapCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_INITIAL_GAP);
      sprintf(dummy, "%1.2f", InitialGap);
      str = wxString::FromUTF8(dummy);
      inigapCtrl->SetValue(str);
      wxTextCtrl *gapCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_GAP);
      sprintf(dummy, "%1.2f", Gap);
      str = wxString::FromUTF8(dummy);
      gapCtrl->SetValue(str);
      if (IsRepeated == true)
        {
          inigapCtrl->Enable(true);
          gapCtrl->Enable(true);
      } else
        {
          inigapCtrl->Enable(false);
          gapCtrl->Enable(false);
        }
      wxCheckBox *alignBox =
        (wxCheckBox *) FindWindow(ID_SYMBOLIZER_IS_ALIGNED);
      if (IsAligned == true)
        alignBox->SetValue(true);
      else
        alignBox->SetValue(false);
      alignBox->Enable(true);
      wxCheckBox *generalizeBox =
        (wxCheckBox *) FindWindow(ID_SYMBOLIZER_GENERALIZE);
      if (GeneralizeLine == true)
        generalizeBox->SetValue(true);
      else
        generalizeBox->SetValue(false);
      generalizeBox->Enable(true);
      wxTextCtrl *rotationCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ROTATION);
      rotationCtrl->Enable(false);
      wxTextCtrl *anchorXCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_X);
      anchorXCtrl->Enable(false);
      wxTextCtrl *anchorYCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_ANCHOR_Y);
      anchorYCtrl->Enable(false);
      wxTextCtrl *displXCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_X);
      displXCtrl->Enable(false);
      wxTextCtrl *displYCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_DISPLACEMENT_Y);
      displYCtrl->Enable(false);
    }
}

bool SimpleTextSymbolizerDialog::RetrievePreviewPage()
{
//
// retrieving params from the PREVIEW page
//
  return true;
}

gaiaGeomCollPtr SimpleTextSymbolizerDialog::PrepareLinestring(double
                                                              perpendicular_offset)
{
// preparing the reference Line
  double pi = 3.14159265359;
  gaiaDynamicLinePtr dyn = gaiaAllocDynamicLine();
  gaiaAppendPointToDynamicLine(dyn, 200.0, 75.0);
  gaiaAppendPointToDynamicLine(dyn, 150.0, 60.0);
  gaiaAppendPointToDynamicLine(dyn, 50.0, 60.0);
  for (double rads = pi; rads > 0.0; rads -= 0.666666)
    {
      double x = 150.0 + (100.0 * cos(rads));
      double y = 150.0 + (100.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  for (double rads = pi; rads <= (pi * 2.0); rads += 0.1)
    {
      double x = 350.0 + (100.0 * cos(rads));
      double y = 150.0 + (100.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  gaiaAppendPointToDynamicLine(dyn, 450.0, 240.0);
  gaiaAppendPointToDynamicLine(dyn, 350.0, 240.0);
  gaiaAppendPointToDynamicLine(dyn, 300.0, 225.0);

  int points = 0;
  gaiaPointPtr pt = dyn->First;
  while (pt != NULL)
    {
      // counting how many Points are there
      points++;
      pt = pt->Next;
    }
  gaiaGeomCollPtr geom = gaiaAllocGeomColl();
  gaiaLinestringPtr ln = gaiaAddLinestringToGeomColl(geom, points);
  int iv = 0;
  pt = dyn->First;
  while (pt != NULL)
    {
      // preparing the Linestring
      gaiaSetPoint(ln->Coords, iv, pt->X, pt->Y);
      iv++;
      pt = pt->Next;
    }
  gaiaFreeDynamicLine(dyn);

  gaiaGeomCollPtr geom2;
  if (perpendicular_offset != 0.0)
    {
      // Offset Curve
      geom2 =
        gaiaOffsetCurve_r(MainFrame->GetSpliteInternalCache(), geom,
                          perpendicular_offset, 16, 0);
      gaiaFreeGeomColl(geom);
  } else
    {
      // unchanged
      geom2 = geom;
    }
  return geom2;
}

void SimpleTextSymbolizerDialog::PrepareLinestringPath(void *xctx,
                                                       double
                                                       perpendicular_offset)
{
// preparing the Line Path
  gaiaGeomCollPtr geom = PrepareLinestring(perpendicular_offset);
  if (geom == NULL)
    return;

  gaiaLinestringPtr ln = geom->FirstLinestring;
  rl2GraphicsContextPtr ctx = (rl2GraphicsContextPtr) xctx;
  for (int iv = 0; iv < ln->Points; iv++)
    {
      double x;
      double y;
      gaiaGetPoint(ln->Coords, iv, &x, &y);
      if (iv == 0)
        rl2_graph_move_to_point(ctx, x, y);
      else
        rl2_graph_add_line_to_path(ctx, x, y);
    }
  gaiaFreeGeomColl(geom);
}

void SimpleTextSymbolizerDialog::GetLineCenterPoint(double perpendicular_offset,
                                                    double *x, double *y)
{
// computing the Reference Line Centre Point
  bool ok = 0;
  unsigned char *blob;
  int size;
  sqlite3_stmt *stmt = NULL;
  int ret;
  const char *sql = "SELECT ST_Line_Interpolate_Point(?, 0.5)";
  gaiaGeomCollPtr geom = PrepareLinestring(perpendicular_offset);
  if (geom == NULL)
    goto error;

  ret =
    sqlite3_prepare_v2(MainFrame->GetSqlite(), sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    goto error;
  sqlite3_reset(stmt);
  sqlite3_clear_bindings(stmt);
  gaiaToSpatiaLiteBlobWkb(geom, &blob, &size);
  sqlite3_bind_blob(stmt, 1, blob, size, free);
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          if (sqlite3_column_type(stmt, 0) == SQLITE_BLOB)
            {
              blob = (unsigned char *) sqlite3_column_blob(stmt, 0);
              size = sqlite3_column_bytes(stmt, 0);
              gaiaGeomCollPtr g =
                gaiaFromSpatiaLiteBlobWkb((const unsigned char *) blob, size);
              if (g == NULL)
                goto error;
              if (g->FirstPoint == NULL)
                goto error;
              *x = g->FirstPoint->X;
              *y = g->FirstPoint->Y;
              ok = 1;
            }
      } else
        goto error;
    }
  if (!ok)
    goto error;
  sqlite3_finalize(stmt);
  gaiaFreeGeomColl(geom);
  return;

error:
  if (stmt != NULL)
    sqlite3_finalize(stmt);
  if (geom != NULL)
    gaiaFreeGeomColl(geom);
  *x = 250.0;
  *y = 150.0;
}

void SimpleTextSymbolizerDialog::CreateLineArray(double perpendicular_offset,
                                                 int *points, double **x,
                                                 double **y, int generalize)
{
// creating the X and Y arrays required by rl2_graph_draw_warped_text()
  double *xx;
  double *yy;
  gaiaLinestringPtr ln;
  gaiaGeomCollPtr geom = PrepareLinestring(perpendicular_offset);
  if (geom == NULL)
    goto error;

  if (generalize)
    generalize = 1;             // silencing stupid compiler warning about unused args

  ln = geom->FirstLinestring;
  if (ln == NULL)
    goto error;

  xx = new double[ln->Points];
  yy = new double[ln->Points];
  if (xx == NULL || yy == NULL)
    {
      if (xx != NULL)
        delete[]xx;
      if (yy != NULL)
        delete[]yy;
      goto error;
    }
  for (int iv = 0; iv < ln->Points; iv++)
    {
      double cx;
      double cy;
      gaiaGetPoint(ln->Coords, iv, &cx, &cy);
      *(xx + iv) = cx;
      *(yy + iv) = cy;

    }
  *points = ln->Points;
  *x = xx;
  *y = yy;
  gaiaFreeGeomColl(geom);
  return;

error:
  *points = 0;
  *x = NULL;
  *y = NULL;
  if (geom != NULL)
    gaiaFreeGeomColl(geom);
}

void SimpleTextSymbolizerDialog::UpdatePreviewPage()
{
//
// updating the PREVIEW page
//
  const char *sample = "This little piggy went to market";
  wxRadioBox *backCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_BACKGROUND);
  switch (PreviewBackground)
    {
      case GUI_PREVIEW_BACKGROUND_WHITE:
        backCtrl->SetSelection(1);
        break;
      case GUI_PREVIEW_BACKGROUND_BLACK:
        backCtrl->SetSelection(2);
        break;
      default:
        backCtrl->SetSelection(0);
        break;
    };
  wxRadioBox *crossCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_CROSSHAIR);
  if (Crosshair == false)
    crossCtrl->SetSelection(1);
  else
    crossCtrl->SetSelection(0);
  if (PointPlacement == true)
    crossCtrl->Enable(true);
  else
    crossCtrl->Enable(false);
  wxRadioBox *reflineCtrl = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_REFLINE);
  if (ReferenceLine == false)
    reflineCtrl->SetSelection(1);
  else
    reflineCtrl->SetSelection(0);
  if (PointPlacement == true)
    {
      crossCtrl->Enable(true);
      reflineCtrl->Enable(false);
  } else
    {
      crossCtrl->Enable(false);
      reflineCtrl->Enable(true);
    }

  rl2GraphicsContextPtr ctx = NULL;
  ctx = rl2_graph_create_context(MainFrame->GetRL2PrivateData(), 500, 300);
  if (ctx == NULL)
    return;
// transparent background initialization
  rl2_graph_set_brush(ctx, 251, 5, 249, 255);
  rl2_graph_draw_rectangle(ctx, -1, -1, 501, 301);

// setting up the Font
  char facename[1024];
  strcpy(facename, FontFamily.ToUTF8());
  rl2GraphicsFontPtr font = rl2_search_TrueType_font(MainFrame->GetSqlite(),
                                                     MainFrame->GetRL2PrivateData
                                                     (),
                                                     facename,
                                                     FontSize);
  if (font == NULL)
    {
      // defaulting to some toy font 
      font =
        rl2_graph_create_toy_font(facename, FontSize, RL2_FONTSTYLE_NORMAL,
                                  RL2_FONTWEIGHT_NORMAL);
    }
  double aleph = 255.0 * FillOpacity;
  if (aleph < 0.0)
    aleph = 0.0;
  if (aleph > 255.0)
    aleph = 255.0;
  unsigned char alpha = aleph;
  wxColour color = wxNullColour;
  ColorMapEntry::GetWxColor(FillColor, color);
  rl2_graph_font_set_color(font, color.Red(), color.Green(), color.Blue(),
                           alpha);
  if (HasHalo == true)
    {
      // setting optional Halo
      aleph = 255.0 * HaloOpacity;
      if (aleph < 0.0)
        aleph = 0.0;
      if (aleph > 255.0)
        aleph = 255.0;
      alpha = aleph;
      color = wxNullColour;
      ColorMapEntry::GetWxColor(HaloColor, color);
      rl2_graph_font_set_halo(font, HaloRadius, color.Red(), color.Green(),
                              color.Blue(), alpha);
    }
  rl2_graph_set_font(ctx, font);

  if (PointPlacement == true)
    {
      // Point Placement
      rl2_graph_draw_text(ctx, sample, 250.0 + DisplacementX,
                          150.0 - DisplacementY, Rotation,
                          AnchorPointX, AnchorPointY);
  } else
    {
      // Line Placement
      if (ReferenceLine == true)
        {
          // drawing the reference Line
          double array34[] = { 3.0, 4.0 };
          PrepareLinestringPath(ctx, 0.0);
          if (PreviewBackground == GUI_PREVIEW_BACKGROUND_CHECKERED)
            rl2_graph_set_dashed_pen(ctx, 107, 142, 35, 255, 3.0,
                                     RL2_PEN_CAP_BUTT, RL2_PEN_JOIN_BEVEL, 2,
                                     array34, 0.0);
          else
            rl2_graph_set_dashed_pen(ctx, 0x90, 0x90, 0x90, 255, 3.0,
                                     RL2_PEN_CAP_BUTT, RL2_PEN_JOIN_BEVEL, 2,
                                     array34, 0.0);
          PrepareLinestringPath(ctx, 0.0);
          rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
          if (PerpendicularOffset != 0.0)
            {
              // drawing the offset line
              double array23[] = { 2.0, 3.0 };
              if (PreviewBackground == GUI_PREVIEW_BACKGROUND_CHECKERED)
                rl2_graph_set_dashed_pen(ctx, 107, 142, 35, 255, 2.0,
                                         RL2_PEN_CAP_BUTT, RL2_PEN_JOIN_BEVEL,
                                         2, array23, 0.0);
              else
                rl2_graph_set_dashed_pen(ctx, 0x90, 0x90, 0x90, 255, 2.0,
                                         RL2_PEN_CAP_BUTT, RL2_PEN_JOIN_BEVEL,
                                         2, array23, 0.0);
              PrepareLinestringPath(ctx, 0.0);
              PrepareLinestringPath(ctx, PerpendicularOffset);
              rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
            }
        }
      if (IsAligned == false)
        {
          // text is always horizontal (not aligned to the line)
          double cx;
          double cy;
          GetLineCenterPoint(PerpendicularOffset, &cx, &cy);
          rl2_graph_draw_text(ctx, sample, cx, cy, 0.0, 0.5, 0.5);
      } else
        {
          // text aligned to the line
          int repeated = 1;
          int generalize = 0;
          if (IsRepeated == false)
            repeated = 0;
          if (GeneralizeLine == false)
            generalize = 0;
          int points;
          double *x_array;
          double *y_array;
          CreateLineArray(PerpendicularOffset, &points, &x_array, &y_array,
                          generalize);
          if (x_array != NULL && y_array != NULL)
            {
              rl2_graph_draw_warped_text(MainFrame->GetSqlite(), ctx, sample,
                                         points, x_array, y_array, InitialGap,
                                         Gap, repeated);
              delete[]x_array;
              delete[]y_array;
            }
        }
    }
  // destroying the Font
  rl2_graph_release_font(ctx);
  rl2_graph_destroy_font(font);

// creating the RGB array
  unsigned char *rgb_array = rl2_graph_get_context_rgb_array(ctx);
  rl2_graph_destroy_context(ctx);
  if (rgb_array == NULL)
    return;
// creating the Preview from RGB array
  wxImage img(500, 300);
  img.SetData(rgb_array);
  wxBitmap bmp(img);
  wxMask *mask = new wxMask(bmp, wxColour(251, 5, 249));
  bmp.SetMask(mask);
  wxBitmap bmp2;
  wxBrush brush;
  wxMemoryDC dc;
  wxBitmap white = wxBitmap(500, 300);
  wxBitmap black = wxBitmap(500, 300);
  switch (PreviewBackground)
    {
      case GUI_PREVIEW_BACKGROUND_WHITE:
        dc.SelectObject(white);
        brush = wxBrush(wxColour(255, 255, 255));
        dc.SetBrush(brush);
        dc.DrawRectangle(0, 0, 500, 300);
        dc.SelectObject(wxNullBitmap);
        bmp2 =
          white.GetSubBitmap(wxRect(0, 0, white.GetWidth(), white.GetHeight()));
        break;
      case GUI_PREVIEW_BACKGROUND_BLACK:
        dc.SelectObject(black);
        brush = wxBrush(wxColour(0, 0, 0));
        dc.SetBrush(brush);
        dc.DrawRectangle(0, 0, 500, 300);
        dc.SelectObject(wxNullBitmap);
        bmp2 =
          black.GetSubBitmap(wxRect(0, 0, black.GetWidth(), black.GetHeight()));
        break;
      default:
        bmp2 =
          PreviewBackBitmap.GetSubBitmap(wxRect
                                         (0, 0, PreviewBackBitmap.GetWidth(),
                                          PreviewBackBitmap.GetHeight()));
        break;
    };
// printing the Preview over the currently selected background
  dc.SelectObject(bmp2);
  dc.DrawBitmap(bmp, 0, 0, true);
  if (PointPlacement == true && Crosshair == true)
    {
      // printing the Crosshair over the preview
      wxColour color = wxColour(255, 255, 255);
      if (PreviewBackground == GUI_PREVIEW_BACKGROUND_BLACK)
        color = wxColour(0, 0, 0);
      dc.SetPen(wxPen(color, 3));
      dc.DrawLine(250 + DisplacementX, 0, 250 + DisplacementX, 300);
      dc.DrawLine(0, 150 - DisplacementY, 500, 150 - DisplacementY);
      color = wxColour(0, 0, 0);
      if (PreviewBackground == GUI_PREVIEW_BACKGROUND_BLACK)
        color = wxColour(255, 255, 255);
      dc.SetPen(wxPen(color, 1));
      dc.DrawLine(250 + DisplacementX, 0, 250 + DisplacementX, 300);
      dc.DrawLine(0, 150 - DisplacementY, 500, 150 - DisplacementY);
      color = wxColour(0, 0, 0);
      if (PreviewBackground == GUI_PREVIEW_BACKGROUND_BLACK)
        color = wxColour(255, 255, 255);
      dc.SetPen(wxPen(color, 5));
      dc.DrawLine(248, 150, 252, 150);
      dc.DrawLine(250, 148, 250, 152);
      color = wxColour(255, 0, 0);
      dc.SetPen(wxPen(color, 3));
      dc.DrawLine(249, 150, 251, 150);
      dc.DrawLine(250, 149, 250, 151);
    }
  dc.SelectObject(wxNullBitmap);
// updating the GUI Preview
  SymbolizerPreview *previewCtrl =
    (SymbolizerPreview *) FindWindow(ID_SYMBOLIZER_PREVIEW);
  previewCtrl->SetBitmap(bmp2);
}

void SimpleTextSymbolizerDialog::OnPageChanging(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changing
//
  bool ret;
  switch (event.GetOldSelection())
    {
      case 0:
        ret = RetrieveMainPage();
        break;
      case 1:
        ret = RetrieveFontPage();
        break;
      case 2:
        ret = RetrievePlacementPage();
        break;
      case 3:
        ret = RetrievePreviewPage();
        break;
    };
  if (ret != true)
    event.Veto();
}

void SimpleTextSymbolizerDialog::OnPageChanged(wxNotebookEvent & event)
{
//
// TAB/PAGE selection changed
//
  switch (event.GetSelection())
    {
      case 0:
        UpdateMainPage();
        break;
      case 1:
        UpdateFontPage();
        break;
      case 2:
        UpdatePlacementPage();
        break;
      case 3:
        UpdatePreviewPage();
        break;
    };
}

char *SimpleTextSymbolizerDialog::DoCreateFeatureTypeXML()
{
//
// creating the SLD/SE (XML) code - Feature Type
//
  char *str;
  const char *cstr;
  char *prev;
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
  switch (Uom)
    {
      case GUI_UOM_METRE:
        cstr = "http://www.opengeospatial.org/se/units/metre";
        break;
      case GUI_UOM_INCH:
        cstr = "http://www.opengeospatial.org/se/units/inch";
        break;
      default:
        cstr = "http://www.opengeospatial.org/se/units/pixel";
        break;
    };
  xml = sqlite3_mprintf("%s\t\t<TextSymbolizer uom=\"%s\">\r\n", prev, cstr);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(Label.ToUTF8()) + 1];
  strcpy(str, Label.ToUTF8());
  xml = sqlite3_mprintf("%s\t\t\t<Label>%s</Label>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(FontFamily.ToUTF8()) + 1];
  strcpy(str, FontFamily.ToUTF8());
  const char *font;
  if (strcmp(str, "ToyFont: serif") == 0)
    font = "serif";
  else if (strcmp(str, "ToyFont: sans-serif") == 0)
    font = "sans serif";
  else if (strcmp(str, "ToyFont: monospace") == 0)
    font = "monospace";
  else
    font = str;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"font-family\">%s</SvgParameter>\r\n", prev,
     font);
  delete[]str;
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
  xml = sqlite3_mprintf("%s\t</Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;

  xml = sqlite3_mprintf("%s\t\t\t<LabelPlacement>\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (PointPlacement == true)
    {
      // PointPlacement
      xml = sqlite3_mprintf("%s\t\t\t\t<PointPlacement>\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (AnchorPointX != 0.5 || AnchorPointY != 0.5)
        {
          xml = sqlite3_mprintf("%s\t\t\t\t\t<AnchorPoint>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n", prev,
             AnchorPointX);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n", prev,
             AnchorPointY);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t\t\t</AnchorPoint>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (DisplacementX != 0.0 || DisplacementY != 0.0)
        {
          xml = sqlite3_mprintf("%s\t\t\t\t\t<Displacement>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
             DisplacementX);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
             DisplacementY);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t\t\t</Displacement>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (Rotation != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev, Rotation);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t\t\t</PointPlacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      // LinePlacement
      xml = sqlite3_mprintf("%s\t\t\t\t<LinePlacement>\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (PerpendicularOffset != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n",
             prev, PerpendicularOffset);
          sqlite3_free(prev);
          prev = xml;
        }
      if (IsRepeated == true)
        {
          // Repeated: InitialGap and Gap
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t<IsRepeated>true</IsRepeated>\r\n",
                            prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<InitialGap>%1.4f</InitialGap>\r\n", prev,
             InitialGap);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t\t\t<Gap>%1.4f</Gap>\r\n", prev, Gap);
          sqlite3_free(prev);
          prev = xml;
        }
      if (IsAligned == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<IsAligned>true</IsAligned>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (GeneralizeLine == true)
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
    }
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
      str = new char[strlen(HaloColor.ToUTF8()) + 1];
      strcpy(str, FillColor.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
         prev, str);
      delete[]str;
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
  str = new char[strlen(FillColor.ToUTF8()) + 1];
  strcpy(str, FillColor.ToUTF8());
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
     prev, FillOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t</Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t</TextSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</FeatureTypeStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *SimpleTextSymbolizerDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - TextSymbolizer
//
  char *str;
  const char *cstr;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
  xml = sqlite3_mprintf("%s<TextSymbolizer version=\"1.1.0\" ", prev);
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
  switch (Uom)
    {
      case GUI_UOM_METRE:
        cstr = "http://www.opengeospatial.org/se/units/metre";
        break;
      case GUI_UOM_INCH:
        cstr = "http://www.opengeospatial.org/se/units/inch";
        break;
      default:
        cstr = "http://www.opengeospatial.org/se/units/pixel";
        break;
    };
  xml =
    sqlite3_mprintf
    ("%sxmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" uom=\"%s\">\r\n",
     prev, cstr);
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
  str = new char[strlen(Label.ToUTF8()) + 1];
  strcpy(str, Label.ToUTF8());
  xml = sqlite3_mprintf("%s\t<Label>%s</Label>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t<Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(FontFamily.ToUTF8()) + 1];
  strcpy(str, FontFamily.ToUTF8());
  const char *font;
  if (strcmp(str, "ToyFont: serif") == 0)
    font = "serif";
  else if (strcmp(str, "ToyFont: sans-serif") == 0)
    font = "sans serif";
  else if (strcmp(str, "ToyFont: monospace") == 0)
    font = "monospace";
  else
    font = str;
  xml =
    sqlite3_mprintf
    ("%s\t\t<SvgParameter name=\"font-family\">%s</SvgParameter>\r\n", prev,
     font);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  if (FontStyle == RL2_FONTSTYLE_ITALIC)
    xml =
      sqlite3_mprintf
      ("%s\t\t<SvgParameter name=\"font-style\">italic</SvgParameter>\r\n",
       prev);
  else if (FontStyle == RL2_FONTSTYLE_OBLIQUE)
    xml =
      sqlite3_mprintf
      ("%s\t\t<SvgParameter name=\"font-style\">oblique</SvgParameter>\r\n",
       prev);
  else
    xml =
      sqlite3_mprintf
      ("%s\t\t<SvgParameter name=\"font-style\">normal</SvgParameter>\r\n",
       prev);
  sqlite3_free(prev);
  prev = xml;
  if (FontWeight == RL2_FONTWEIGHT_BOLD)
    xml =
      sqlite3_mprintf
      ("%s\t\t<SvgParameter name=\"font-weight\">bold</SvgParameter>\r\n",
       prev);
  else
    xml =
      sqlite3_mprintf
      ("%s\t\t<SvgParameter name=\"font-weight\">normal</SvgParameter>\r\n",
       prev);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t<SvgParameter name=\"font-size\">%1.2f</SvgParameter>\r\n", prev,
     FontSize);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Font>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;

  xml = sqlite3_mprintf("%s\t<LabelPlacement>\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (PointPlacement == true)
    {
      // PointPlacement
      xml = sqlite3_mprintf("%s\t\t<PointPlacement>\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (AnchorPointX != 0.5 || AnchorPointY != 0.5)
        {
          xml = sqlite3_mprintf("%s\t\t\t<AnchorPoint>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s\t\t\t\t<AnchorPointX>%1.4f</AnchorPointX>\r\n",
                            prev, AnchorPointX);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf("%s\t\t\t\t<AnchorPointY>%1.4f</AnchorPointY>\r\n",
                            prev, AnchorPointY);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t</AnchorPoint>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (DisplacementX != 0.0 || DisplacementY != 0.0)
        {
          xml = sqlite3_mprintf("%s\t\t\t<Displacement>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
             DisplacementX);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
             DisplacementY);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t</Displacement>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (Rotation != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<Rotation>%1.2f</Rotation>\r\n", prev, Rotation);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</PointPlacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      // LinePlacement
      xml = sqlite3_mprintf("%s\t\t<LinePlacement>\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (PerpendicularOffset != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n",
             prev, PerpendicularOffset);
          sqlite3_free(prev);
          prev = xml;
        }
      if (IsRepeated == true)
        {
          // Repeated: InitialGap and Gap
          xml =
            sqlite3_mprintf("%s\t\t\t<IsRepeated>true</IsRepeated>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<InitialGap>%1.4f</InitialGap>\r\n", prev, InitialGap);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t<Gap>%1.4f</Gap>\r\n", prev, Gap);
          sqlite3_free(prev);
          prev = xml;
        }
      if (IsAligned == true)
        {
          xml =
            sqlite3_mprintf("%s\t\t\t<IsAligned>true</IsAligned>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (GeneralizeLine == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<GeneralizeLine>true</GeneralizeLine>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</LinePlacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t</LabelPlacement>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasHalo == true)
    {
      // Halo
      xml = sqlite3_mprintf("%s\t<Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<Radius>%1.2f</Radius>\r\n", prev, HaloRadius);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(HaloColor.ToUTF8()) + 1];
      strcpy(str, FillColor.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t</Halo>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t<Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  str = new char[strlen(HaloColor.ToUTF8()) + 1];
  strcpy(str, FillColor.ToUTF8());
  xml =
    sqlite3_mprintf
    ("%s\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev, str);
  delete[]str;
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
     prev, FillOpacity);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t</Fill>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s</TextSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void SimpleTextSymbolizerDialog::OnInsert(wxCommandEvent & WXUNUSED(event))
{
//
// inserting the VectorSymbolizer into the DBMS
//
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrieveFontPage();
        break;
      case 2:
        RetrievePlacementPage();
        break;
      case 3:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateFeatureTypeXML();
  else
    xml = DoCreateSymbolizerXML();
  bool ret = MainFrame->DoInsertVectorSymbolizer(xml);
  sqlite3_free(xml);
  if (ret == true)
    wxMessageBox(wxT
                 ("SLD/SE VectorSymbolizer successfully registered into the DBMS"),
                 wxT("spatialite_gui"), wxOK | wxICON_INFORMATION, this);
  wxDialog::EndModal(wxID_OK);
}

void SimpleTextSymbolizerDialog::OnExport(wxCommandEvent & WXUNUSED(event))
{
//
// exporting the VectorSymbolizer as an external file
//
  int ret;
  wxString path;
  wxString lastDir;
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrieveFontPage();
        break;
      case 2:
        RetrievePlacementPage();
        break;
      case 3:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  wxFileDialog fileDialog(this,
                          wxT("Exporting an SLD/SE TextSymbolizer to a file"),
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
            xml = DoCreateFeatureTypeXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE TextSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void SimpleTextSymbolizerDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
{
//
// Copying the VectorSymbolizer into the Clipboard 
//
  switch (GetBookCtrl()->GetSelection())
    {
      case 0:
        RetrieveMainPage();
        break;
      case 1:
        RetrieveFontPage();
        break;
      case 2:
        RetrievePlacementPage();
        break;
      case 3:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml;
  if (MinScale == true || MaxScale == true)
    xml = DoCreateFeatureTypeXML();
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

void SimpleTextSymbolizerDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}
