/*
/ VectorSymbolizers1.cpp
/ various dialog classes
/
/ version 1.8, 2015 March 28
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

SimpleLineSymbolizerDialog::SimpleLineSymbolizerDialog()
{
// ctor
  Stroke1DashArray = NULL;
  Stroke2DashArray = NULL;
  Stroke3DashArray = NULL;
  List = NULL;
}

SimpleLineSymbolizerDialog::~SimpleLineSymbolizerDialog()
{
// dtor
  if (Stroke1DashArray != NULL)
    delete[]Stroke1DashArray;
  if (Stroke2DashArray != NULL)
    delete[]Stroke2DashArray;
  if (Stroke3DashArray != NULL)
    delete[]Stroke3DashArray;
  if (List)
    delete List;
}

bool SimpleLineSymbolizerDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Uom = GUI_UOM_PIXEL;
  MinScale = false;
  MaxScale = false;
  PerpendicularOffset1 = 0.0;
  Stroke1Opacity = 1.0;
  HasGraphic1 = false;
  Stroke1Color = wxT("#000000");
  EnableStroke1Replacement = false;
  Stroke1ColorReplacement = wxT("#000000");
  Stroke1Width = 1.0;
  Stroke1LineJoin = RL2_PEN_JOIN_ROUND;
  Stroke1LineCap = RL2_PEN_CAP_ROUND;
  Stroke1DashCount = 0;
  Stroke1DashOffset = 0.0;
  EnableStroke2 = false;
  PerpendicularOffset2 = 0.0;
  Stroke2Opacity = 1.0;
  HasGraphic2 = false;
  Stroke2Color = wxT("#000000");
  EnableStroke2Replacement = false;
  Stroke2ColorReplacement = wxT("#000000");
  Stroke2Width = 1.0;
  Stroke2LineJoin = RL2_PEN_JOIN_ROUND;
  Stroke2LineCap = RL2_PEN_CAP_ROUND;
  Stroke2DashCount = 0;
  Stroke2DashOffset = 0.0;
  EnableStroke3 = false;
  PerpendicularOffset3 = 0.0;
  Stroke3Opacity = 1.0;
  HasGraphic3 = false;
  Stroke3Color = wxT("#000000");
  EnableStroke3Replacement = false;
  Stroke3ColorReplacement = wxT("#000000");
  Stroke3Width = 1.0;
  Stroke3LineJoin = RL2_PEN_JOIN_ROUND;
  Stroke3LineCap = RL2_PEN_CAP_ROUND;
  Stroke3DashCount = 0;
  Stroke3DashOffset = 0.0;
  PreviewBackground = GUI_PREVIEW_BACKGROUND_CHECKERED;
  List = MainFrame->FindExternalGraphic(true);

  if (wxPropertySheetDialog::Create
      (parent, wxID_ANY, wxT("Simple Line Symbolizer")) == false)
    return false;
  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  wxPanel *stroke1Page = CreateStroke1Page(book);
  book->AddPage(stroke1Page, wxT("Stroke #1"), false);
  wxPanel *stroke2Page = CreateStroke2Page(book);
  book->AddPage(stroke2Page, wxT("Stroke #2"), false);
  wxPanel *stroke3Page = CreateStroke3Page(book);
  book->AddPage(stroke3Page, wxT("Stroke #3"), false);
  wxPanel *previewPage = CreatePreviewPage(book);
  book->AddPage(previewPage, wxT("Preview"), false);

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) & SimpleLineSymbolizerDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) & SimpleLineSymbolizerDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleLineSymbolizerDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleLineSymbolizerDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleLineSymbolizerDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimpleLineSymbolizerDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

wxPanel *SimpleLineSymbolizerDialog::CreateMainPage(wxWindow * parent)
{
//
// creating the MAIN page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MAIN);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the LineSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(panel, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the LineSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the LineSymbolizer Abstract
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
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_UOM, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdUomChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdScaleChanged);
  return panel;
}

void SimpleLineSymbolizerDialog::
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

void SimpleLineSymbolizerDialog::
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

wxPanel *SimpleLineSymbolizerDialog::CreateStroke1Page(wxWindow * parent)
{
//
// creating the STROKE #1 page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_STROKE1);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #1 Opacity and Perpendicular Offset
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE1_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(300, 45),
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
// second row A: Stroke Type
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxVERTICAL);
  strokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE1_TYPE,
                                       wxT("&Stroke Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  stroke1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
// second row B: Stroke Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  stroke1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_COLOR, Stroke1Color,
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
// second row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl1 =
    new wxGrid(panel, ID_SYMBOLIZER_STROKE1_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl1->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl1->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl1->SetColLabelValue(1, wxT("Title"));
  GridCtrl1->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl1->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl1->SetCellRenderer(count, 0, renderer);
      GridCtrl1->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl1->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl1->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl1->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl1->AutoSize();
  GridCtrl1->EnableEditing(false);
  gridSizer->Add(GridCtrl1, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl1->Enable(false);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_REPLACEMENT,
                   Stroke1ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE1_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: Stroke-Width, Stroke-LineJoin, Stroke-LineCap and Stroke-Dasharray 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
// third row B: LineJoin
  wxString join[3];
  join[0] = wxT("&Mitre");
  join[1] = wxT("&Round");
  join[2] = wxT("&Bevel");
  wxRadioBox *joinBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE1_LINEJOIN,
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
  wxRadioBox *capBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE1_LINECAP,
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_DASHARRAY, wxT(""),
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_DASHOFFSET, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  dash2Sizer->Add(dashOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE1_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke1TypeChanged);
  Connect(ID_SYMBOLIZER_STROKE1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdColor1Changed);
  Connect(ID_SYMBOLIZER_STROKE1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdColor1Picker);
  Connect(ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke1EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE1_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdReplacement1Changed);
  Connect(ID_SYMBOLIZER_STROKE1_LINEJOIN, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdLineJoin1Changed);
  Connect(ID_SYMBOLIZER_STROKE1_LINECAP, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdLineCap1Changed);
  return panel;
}

void SimpleLineSymbolizerDialog::
OnCmdStroke1TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// stroke type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphic1 = false;
  else
    HasGraphic1 = true;
  RetrieveStroke1Page(false);
  UpdateStroke1Page();
}

void SimpleLineSymbolizerDialog::
OnCmdColor1Changed(wxCommandEvent & WXUNUSED(event))
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

void SimpleLineSymbolizerDialog::
OnCmdColor1Picker(wxCommandEvent & WXUNUSED(event))
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

void SimpleLineSymbolizerDialog::
OnCmdStroke1EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #1 ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableStroke1Replacement = true;
  else
    EnableStroke1Replacement = false;
  RetrieveStroke1Page(false);
  UpdateStroke1Page();
}

void SimpleLineSymbolizerDialog::
OnCmdReplacement1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT_HEX);
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

void SimpleLineSymbolizerDialog::
OnCmdLineJoin1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineJoin selection changed
//
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINEJOIN);
  switch (lineJoinCtrl->GetSelection())
    {
      case 0:
        Stroke1LineJoin = RL2_PEN_JOIN_MITER;
        break;
      case 2:
        Stroke1LineJoin = RL2_PEN_JOIN_BEVEL;
        break;
      default:
        Stroke1LineJoin = RL2_PEN_JOIN_ROUND;
        break;
    };
}

void SimpleLineSymbolizerDialog::
OnCmdLineCap1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineCap selection changed
//
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINECAP);
  switch (lineCapCtrl->GetSelection())
    {
      case 0:
        Stroke1LineCap = RL2_PEN_CAP_BUTT;
        break;
      case 2:
        Stroke1LineCap = RL2_PEN_CAP_SQUARE;
        break;
      default:
        Stroke1LineCap = RL2_PEN_CAP_ROUND;
        break;
    };
}

wxPanel *SimpleLineSymbolizerDialog::CreateStroke2Page(wxWindow * parent)
{
//
// creating the STROKE #2 page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_STROKE2);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #2 Opacity and Perpendicular Offset
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *enableBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke #2"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *enableSizer = new wxStaticBoxSizer(enableBox, wxVERTICAL);
  opacityBoxSizer->Add(enableSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_STROKE2_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(false);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(300, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityCtrl->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_PERPENDICULAR, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  perpendicularCtrl->Enable(false);
  perp1Sizer->Add(perpendicularCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticText *perp1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Draw lines in parallel to the original geometry."));
  perpendicularSizer->Add(perp1Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row: Stroke color or Graphic
  wxBoxSizer *strokeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(strokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Stroke Type
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxVERTICAL);
  strokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE2_TYPE,
                                       wxT("&Stroke Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  stroke1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
  typeBox->Enable(false);
// second row B: Stroke Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  stroke1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_COLOR, Stroke2Color,
                   wxDefaultPosition, wxSize(80, 22));
  colorCtrl->Enable(false);
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxT("&Pick a color"));
  pick->Enable(false);
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// second row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl2 =
    new wxGrid(panel, ID_SYMBOLIZER_STROKE2_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl2->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl2->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl2->SetColLabelValue(1, wxT("Title"));
  GridCtrl2->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl2->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl2->SetCellRenderer(count, 0, renderer);
      GridCtrl2->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl2->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl2->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl2->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl2->AutoSize();
  GridCtrl2->EnableEditing(false);
  gridSizer->Add(GridCtrl2, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl2->Enable(false);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_REPLACEMENT,
                   Stroke2ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);;
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: Stroke-Width, Stroke-LineJoin, Stroke-LineCap and Stroke-Dasharray 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  widthCtrl->Enable(false);
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: LineJoin
  wxString join[3];
  join[0] = wxT("&Mitre");
  join[1] = wxT("&Round");
  join[2] = wxT("&Bevel");
  wxRadioBox *joinBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE2_LINEJOIN,
                                       wxT("&Line Join"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       join, 1,
                                       wxRA_SPECIFY_COLS);
  miscSizer->Add(joinBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  joinBox->SetSelection(1);
  joinBox->Enable(false);
// third row C: LineCap
  wxString cap[3];
  cap[0] = wxT("&Butt");
  cap[1] = wxT("&Round");
  cap[2] = wxT("&Square");
  wxRadioBox *capBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE2_LINECAP,
                                      wxT("&Line Cap"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      cap, 1,
                                      wxRA_SPECIFY_COLS);
  miscSizer->Add(capBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  capBox->SetSelection(1);
  capBox->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_DASHARRAY, wxT(""),
                   wxDefaultPosition, wxSize(200, 22));
  dashArrayCtrl->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_DASHOFFSET, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  dashOffsetCtrl->Enable(false);
  dash2Sizer->Add(dashOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke2Changed);
  Connect(ID_SYMBOLIZER_STROKE2_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke2TypeChanged);
  Connect(ID_SYMBOLIZER_STROKE2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdColor2Changed);
  Connect(ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdColor2Picker);
  Connect(ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke2EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE2_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdReplacement2Changed);
  Connect(ID_SYMBOLIZER_STROKE2_LINEJOIN, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdLineJoin2Changed);
  Connect(ID_SYMBOLIZER_STROKE2_LINECAP, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdLineCap2Changed);
  return panel;
}

void SimpleLineSymbolizerDialog::
OnCmdStroke2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #2 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableStroke2 = true;
  else
    EnableStroke2 = false;
  RetrieveStroke2Page(false);
  UpdateStroke2Page();
}

void SimpleLineSymbolizerDialog::
OnCmdStroke2TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// stroke type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphic2 = false;
  else
    HasGraphic2 = true;
  RetrieveStroke2Page(false);
  UpdateStroke2Page();
}

void SimpleLineSymbolizerDialog::
OnCmdColor2Changed(wxCommandEvent & WXUNUSED(event))
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

void SimpleLineSymbolizerDialog::
OnCmdColor2Picker(wxCommandEvent & WXUNUSED(event))
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

void SimpleLineSymbolizerDialog::
OnCmdStroke2EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #2 ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableStroke2Replacement = true;
  else
    EnableStroke2Replacement = false;
  RetrieveStroke2Page(false);
  UpdateStroke2Page();
}

void SimpleLineSymbolizerDialog::
OnCmdReplacement2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT_HEX);
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

void SimpleLineSymbolizerDialog::
OnCmdLineJoin2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineJoin selection changed
//
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINEJOIN);
  switch (lineJoinCtrl->GetSelection())
    {
      case 0:
        Stroke2LineJoin = RL2_PEN_JOIN_MITER;
        break;
      case 2:
        Stroke2LineJoin = RL2_PEN_JOIN_BEVEL;
        break;
      default:
        Stroke2LineJoin = RL2_PEN_JOIN_ROUND;
        break;
    };
}

void SimpleLineSymbolizerDialog::
OnCmdLineCap2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineCap selection changed
//
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINECAP);
  switch (lineCapCtrl->GetSelection())
    {
      case 0:
        Stroke2LineCap = RL2_PEN_CAP_BUTT;
        break;
      case 2:
        Stroke2LineCap = RL2_PEN_CAP_SQUARE;
        break;
      default:
        Stroke2LineCap = RL2_PEN_CAP_ROUND;
        break;
    };
}

wxPanel *SimpleLineSymbolizerDialog::CreateStroke3Page(wxWindow * parent)
{
//
// creating the STROKE #3 page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_STROKE3);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #3 Opacity and Perpendicular Offset
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *enableBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke #3"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *enableSizer = new wxStaticBoxSizer(enableBox, wxVERTICAL);
  opacityBoxSizer->Add(enableSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_STROKE3_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(false);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE3_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(300, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityCtrl->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE3_PERPENDICULAR, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  perpendicularCtrl->Enable(false);
  perp1Sizer->Add(perpendicularCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 2);
  wxStaticText *perp1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Draw lines in parallel to the original geometry."));
  perpendicularSizer->Add(perp1Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row: Stroke color or Graphic
  wxBoxSizer *strokeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(strokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Stroke Type
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxVERTICAL);
  strokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE3_TYPE,
                                       wxT("&Stroke Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  stroke1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
  typeBox->Enable(false);
// second row B: Stroke Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  stroke1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE3_COLOR, Stroke3Color,
                   wxDefaultPosition, wxSize(80, 22));
  colorCtrl->Enable(false);
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE3_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_STROKE3_PICKER_BTN, wxT("&Pick a color"));
  pick->Enable(false);
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// second row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl3 =
    new wxGrid(panel, ID_SYMBOLIZER_STROKE3_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl3->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl3->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl3->SetColLabelValue(1, wxT("Title"));
  GridCtrl3->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl3->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl3->SetCellRenderer(count, 0, renderer);
      GridCtrl3->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl3->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl3->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl3->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl3->AutoSize();
  GridCtrl3->EnableEditing(false);
  gridSizer->Add(GridCtrl3, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl3->Enable(false);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_STROKE3_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE3_REPLACEMENT,
                   Stroke3ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE3_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: Stroke-Width, Stroke-LineJoin, Stroke-LineCap and Stroke-Dasharray 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE3_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  widthCtrl->Enable(false);
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: LineJoin
  wxString join[3];
  join[0] = wxT("&Mitre");
  join[1] = wxT("&Round");
  join[2] = wxT("&Bevel");
  wxRadioBox *joinBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE3_LINEJOIN,
                                       wxT("&Line Join"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       join, 1,
                                       wxRA_SPECIFY_COLS);
  miscSizer->Add(joinBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  joinBox->SetSelection(1);
  joinBox->Enable(false);
// third row C: LineCap
  wxString cap[3];
  cap[0] = wxT("&Butt");
  cap[1] = wxT("&Round");
  cap[2] = wxT("&Square");
  wxRadioBox *capBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE3_LINECAP,
                                      wxT("&Line Cap"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      cap, 1,
                                      wxRA_SPECIFY_COLS);
  miscSizer->Add(capBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  capBox->SetSelection(1);
  capBox->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE3_DASHARRAY, wxT(""),
                   wxDefaultPosition, wxSize(200, 22));
  dashArrayCtrl->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE3_DASHOFFSET, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  dashOffsetCtrl->Enable(false);
  dash2Sizer->Add(dashOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE3_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke3Changed);
  Connect(ID_SYMBOLIZER_STROKE3_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke3TypeChanged);
  Connect(ID_SYMBOLIZER_STROKE3_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdColor3Changed);
  Connect(ID_SYMBOLIZER_STROKE3_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdColor3Picker);
  Connect(ID_SYMBOLIZER_STROKE3_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdStroke3EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE3_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdReplacement3Changed);
  Connect(ID_SYMBOLIZER_STROKE3_LINEJOIN, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdLineJoin3Changed);
  Connect(ID_SYMBOLIZER_STROKE3_LINECAP, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdLineCap3Changed);
  return panel;
}

void SimpleLineSymbolizerDialog::
OnCmdStroke3Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #3 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE3_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableStroke3 = true;
  else
    EnableStroke3 = false;
  RetrieveStroke3Page(false);
  UpdateStroke3Page();
}

void SimpleLineSymbolizerDialog::
OnCmdStroke3TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// stroke type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE3_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphic3 = false;
  else
    HasGraphic3 = true;
  RetrieveStroke3Page(false);
  UpdateStroke3Page();
}

void SimpleLineSymbolizerDialog::
OnCmdColor3Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE3_PICKER_HEX);
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

void SimpleLineSymbolizerDialog::
OnCmdColor3Picker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_COLOR);
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

void SimpleLineSymbolizerDialog::
OnCmdStroke3EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #3 ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE3_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableStroke3Replacement = true;
  else
    EnableStroke3Replacement = false;
  RetrieveStroke3Page(false);
  UpdateStroke3Page();
}

void SimpleLineSymbolizerDialog::
OnCmdReplacement3Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE3_REPLACEMENT_HEX);
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

void SimpleLineSymbolizerDialog::
OnCmdLineJoin3Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineJoin selection changed
//
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE3_LINEJOIN);
  switch (lineJoinCtrl->GetSelection())
    {
      case 0:
        Stroke3LineJoin = RL2_PEN_JOIN_MITER;
        break;
      case 2:
        Stroke3LineJoin = RL2_PEN_JOIN_BEVEL;
        break;
      default:
        Stroke3LineJoin = RL2_PEN_JOIN_ROUND;
        break;
    };
}

void SimpleLineSymbolizerDialog::
OnCmdLineCap3Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineCap selection changed
//
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE3_LINECAP);
  switch (lineCapCtrl->GetSelection())
    {
      case 0:
        Stroke3LineCap = RL2_PEN_CAP_BUTT;
        break;
      case 2:
        Stroke3LineCap = RL2_PEN_CAP_SQUARE;
        break;
      default:
        Stroke3LineCap = RL2_PEN_CAP_ROUND;
        break;
    };
}

wxPanel *SimpleLineSymbolizerDialog::CreatePreviewPage(wxWindow * parent)
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
// creating a control to show the LineSymbolizer Preview
  wxStaticBox *previewBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("LineSymbolizer Preview"),
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
  boxSizer->Add(backBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  backBox->SetSelection(0);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_BACKGROUND, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimpleLineSymbolizerDialog::OnCmdBackgroundChanged);
  return panel;
}

void SimpleLineSymbolizerDialog::
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

void SimpleLineSymbolizerDialog::DrawPreview(int horz, int vert)
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
      // creating a checkered background
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

void SimpleLineSymbolizerDialog::CreateButtons()
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

bool SimpleLineSymbolizerDialog::FinalValidityCheck()
{
//
// last check before generating the SLD/SE Style
//
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the LineSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some LineSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some LineSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  return true;
}

bool SimpleLineSymbolizerDialog::RetrieveMainPage()
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

void SimpleLineSymbolizerDialog::UpdateMainPage()
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
}

bool SimpleLineSymbolizerDialog::DoParseDashArray(wxString & str, int which)
{
//
// attempting to parse a Stroke DashArray string
//
  if (which == 0)
    {
      Stroke1DashCount = 0;
      if (Stroke1DashArray != NULL)
        delete[]Stroke1DashArray;
      Stroke1DashArray = NULL;
    }
  if (which == 1)
    {
      Stroke2DashCount = 0;
      if (Stroke2DashArray != NULL)
        delete[]Stroke2DashArray;
      Stroke2DashArray = NULL;
    }
  if (which == 2)
    {
      Stroke3DashCount = 0;
      if (Stroke3DashArray != NULL)
        delete[]Stroke3DashArray;
      Stroke3DashArray = NULL;
    }
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
  if (which == 0)
    {
      Stroke1DashCount = count;
      Stroke1DashArray = new double[Stroke1DashCount];
      array = Stroke1DashArray;
    }
  if (which == 1)
    {
      Stroke2DashCount = count;
      Stroke2DashArray = new double[Stroke2DashCount];
      array = Stroke2DashArray;
    }
  if (which == 2)
    {
      Stroke3DashCount = count;
      Stroke3DashArray = new double[Stroke3DashCount];
      array = Stroke3DashArray;
    }
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

void SimpleLineSymbolizerDialog::NormalizedDashArray(wxString & str, int which,
                                                     char delimiter)
{
//
// creating a normalized DashArray string
//
  int count;
  double *array;
  if (which == 0)
    {
      count = Stroke1DashCount;
      array = Stroke1DashArray;
    }
  if (which == 1)
    {
      count = Stroke2DashCount;
      array = Stroke2DashArray;
    }
  if (which == 2)
    {
      count = Stroke3DashCount;
      array = Stroke3DashArray;
    }
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

bool SimpleLineSymbolizerDialog::RetrieveStroke1Page(bool check)
{
//
// retrieving params from the STROKE #1 page
//
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  Stroke1Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_PERPENDICULAR);
  wxString value = perpCtrl->GetValue();
  if (value.ToDouble(&PerpendicularOffset1) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (HasGraphic1 == false)
    {
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
      Stroke1Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl1->GetNumberRows(); i++)
        {
          if (GridCtrl1->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Stroke1XLinkHref, Stroke1MimeType);
      if (EnableStroke1Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Stroke1ColorReplacement = color;
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  value = widthCtrl->GetValue();
  if (value.ToDouble(&Stroke1Width) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Stroke1Width <= 0.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHARRAY);
  value = dashArrayCtrl->GetValue();
  if (DoParseDashArray(value, 0) == false)
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
  if (Stroke1DashCount == 0)
    Stroke1DashOffset = 0.0;
  else
    {
      wxTextCtrl *offsetCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHOFFSET);
      wxString value = offsetCtrl->GetValue();
      if (value.ToDouble(&Stroke1DashOffset) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-DASH-OFFSET isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
    }
  return true;
}

bool SimpleLineSymbolizerDialog::RetrieveStroke2Page(bool check)
{
//
// retrieving params from the STROKE #2 page
//
  if (EnableStroke2 == false)
    return true;
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  Stroke2Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_PERPENDICULAR);
  wxString value = perpCtrl->GetValue();
  if (value.ToDouble(&PerpendicularOffset2) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (HasGraphic2 == false)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
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
      Stroke2Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl2->GetNumberRows(); i++)
        {
          if (GridCtrl2->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Stroke2XLinkHref, Stroke2MimeType);
      if (EnableStroke2Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Stroke2ColorReplacement = color;
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  value = widthCtrl->GetValue();
  if (value.ToDouble(&Stroke2Width) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Stroke2Width <= 0.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHARRAY);
  value = dashArrayCtrl->GetValue();
  if (DoParseDashArray(value, 1) == false)
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
      NormalizedDashArray(value, 1);
      dashArrayCtrl->SetValue(value);
    }
  if (Stroke2DashCount == 0)
    Stroke2DashOffset = 0.0;
  else
    {
      wxTextCtrl *offsetCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHOFFSET);
      wxString value = offsetCtrl->GetValue();
      if (value.ToDouble(&Stroke2DashOffset) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-DASH-OFFSET isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
    }
  return true;
}

bool SimpleLineSymbolizerDialog::RetrieveStroke3Page(bool check)
{
//
// retrieving params from the STROKE #3 page
//
  if (EnableStroke2 == false)
    return true;
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE3_OPACITY);
  Stroke3Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_PERPENDICULAR);
  wxString value = perpCtrl->GetValue();
  if (value.ToDouble(&PerpendicularOffset3) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (HasGraphic3 == false)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_COLOR);
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
      Stroke3Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl3->GetNumberRows(); i++)
        {
          if (GridCtrl3->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Stroke3XLinkHref, Stroke3MimeType);
      if (EnableStroke3Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Stroke3ColorReplacement = color;
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_WIDTH);
  value = widthCtrl->GetValue();
  if (value.ToDouble(&Stroke3Width) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Stroke3Width <= 0.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_DASHARRAY);
  value = dashArrayCtrl->GetValue();
  if (DoParseDashArray(value, 2) == false)
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
      NormalizedDashArray(value, 2);
      dashArrayCtrl->SetValue(value);
    }
  if (Stroke3DashCount == 0)
    Stroke3DashOffset = 0.0;
  else
    {
      wxTextCtrl *offsetCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_DASHOFFSET);
      wxString value = offsetCtrl->GetValue();
      if (value.ToDouble(&Stroke3DashOffset) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-DASH-OFFSET isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
    }
  return true;
}

void SimpleLineSymbolizerDialog::UpdateStroke1Page()
{
//
// updating the STROKE #1 page
//
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  opacityCtrl->SetValue(Stroke1Opacity * 100.0);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_PERPENDICULAR);
  char dummy[64];
  sprintf(dummy, "%1.2f", PerpendicularOffset1);
  wxString str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_TYPE);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE1_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT);
  if (HasGraphic1 == false)
    {
      typeBox->SetSelection(0);
      colorCtrl->Enable(true);
      pick->Enable(true);
      GridCtrl1->Enable(false);
      GridCtrl1->ClearSelection();
      wxColour color = wxNullColour;
      ColorMapEntry::GetWxColor(Stroke1Color, color);
      if (color.IsOk() == true)
        {
          char hex[16];
          sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                  color.Blue());
          wxString str = wxString::FromUTF8(hex);
          colorCtrl->SetValue(str);
        }
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else
    {
      typeBox->SetSelection(1);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl1->Enable(true);
      int sel = List->FindByXLinkHref(Stroke1XLinkHref);
      if (sel >= 0)
        GridCtrl1->SelectRow(sel);
      enableReplacement->Enable(true);
      if (EnableStroke1Replacement == true)
        {
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke1ColorReplacement, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              replacementCtrl->SetValue(str);
              replacementCtrl->Enable(true);
          } else
            replacementCtrl->Enable(false);
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  sprintf(dummy, "%1.2f", Stroke1Width);
  str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINEJOIN);
  switch (Stroke1LineJoin)
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
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINECAP);
  switch (Stroke1LineCap)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHARRAY);
  wxString value;
  NormalizedDashArray(value, 0);
  dashArrayCtrl->SetValue(value);
  wxTextCtrl *offsetCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHOFFSET);
  if (Stroke1DashCount == 0)
    offsetCtrl->SetValue(wxT("0.0"));
  else
    {
      sprintf(dummy, "%1.2f", Stroke1DashOffset);
      str = wxString::FromUTF8(dummy);
      offsetCtrl->SetValue(str);
    }
}

void SimpleLineSymbolizerDialog::UpdateStroke2Page()
{
//
// updating the STROKE #2 page
//
  wxCheckBox *enableBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  if (EnableStroke2 == true)
    enableBox->SetValue(true);
  else
    enableBox->SetValue(false);
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  opacityCtrl->SetValue(Stroke2Opacity * 100.0);
  if (EnableStroke2 == false)
    opacityCtrl->Enable(false);
  else
    opacityCtrl->Enable(true);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_PERPENDICULAR);
  char dummy[64];
  sprintf(dummy, "%1.2f", PerpendicularOffset2);
  wxString str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  if (EnableStroke2 == false)
    perpCtrl->Enable(false);
  else
    perpCtrl->Enable(true);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_TYPE);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT);
  if (EnableStroke2 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else
    {
      typeBox->Enable(true);
      if (HasGraphic2 == false)
        {
          typeBox->SetSelection(0);
          colorCtrl->Enable(true);
          pick->Enable(true);
          GridCtrl2->Enable(false);
          GridCtrl2->ClearSelection();
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke2Color, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              colorCtrl->SetValue(str);
            }
          enableReplacement->Enable(false);
          replacementCtrl->Enable(false);
      } else
        {
          typeBox->SetSelection(1);
          colorCtrl->Enable(false);
          pick->Enable(false);
          GridCtrl2->Enable(true);
          int sel = List->FindByXLinkHref(Stroke2XLinkHref);
          if (sel >= 0)
            GridCtrl2->SelectRow(sel);
          enableReplacement->Enable(true);
          if (EnableStroke2Replacement == true)
            {
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke2ColorReplacement, color);
              if (color.IsOk() == true)
                {
                  char hex[16];
                  sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                          color.Blue());
                  wxString str = wxString::FromUTF8(hex);
                  replacementCtrl->SetValue(str);
                  replacementCtrl->Enable(true);
              } else
                replacementCtrl->Enable(false);
            }
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  sprintf(dummy, "%1.2f", Stroke2Width);
  str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  if (EnableStroke2 == false)
    widthCtrl->Enable(false);
  else
    widthCtrl->Enable(true);
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINEJOIN);
  switch (Stroke2LineJoin)
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
  if (EnableStroke2 == false)
    lineJoinCtrl->Enable(false);
  else
    lineJoinCtrl->Enable(true);
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINECAP);
  switch (Stroke2LineCap)
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
  if (EnableStroke2 == false)
    lineCapCtrl->Enable(false);
  else
    lineCapCtrl->Enable(true);
  wxTextCtrl *dashArrayCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHARRAY);
  wxString value;
  NormalizedDashArray(value, 1);
  dashArrayCtrl->SetValue(value);
  if (EnableStroke2 == false)
    dashArrayCtrl->Enable(false);
  else
    dashArrayCtrl->Enable(true);
  wxTextCtrl *offsetCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHOFFSET);
  if (Stroke2DashCount == 0)
    offsetCtrl->SetValue(wxT("0.0"));
  else
    {
      char dummy[64];
      sprintf(dummy, "%1.2f", Stroke2DashOffset);
      wxString str = wxString::FromUTF8(dummy);
      offsetCtrl->SetValue(str);
    }
  if (EnableStroke2 == false)
    offsetCtrl->Enable(false);
  else
    offsetCtrl->Enable(true);
}

void SimpleLineSymbolizerDialog::UpdateStroke3Page()
{
//
// updating the STROKE #3 page
//
  wxCheckBox *enableBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE3_ENABLE);
  if (EnableStroke3 == true)
    enableBox->SetValue(true);
  else
    enableBox->SetValue(false);
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE3_OPACITY);
  opacityCtrl->SetValue(Stroke3Opacity * 100.0);
  if (EnableStroke3 == false)
    opacityCtrl->Enable(false);
  else
    opacityCtrl->Enable(true);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_PERPENDICULAR);
  char dummy[64];
  sprintf(dummy, "%1.2f", PerpendicularOffset3);
  wxString str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  if (EnableStroke3 == false)
    perpCtrl->Enable(false);
  else
    perpCtrl->Enable(true);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE3_TYPE);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE3_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE3_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_REPLACEMENT);
  if (EnableStroke3 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else
    {
      typeBox->Enable(true);
      if (HasGraphic3 == false)
        {
          typeBox->SetSelection(0);
          colorCtrl->Enable(true);
          pick->Enable(true);
          GridCtrl3->Enable(false);
          GridCtrl3->ClearSelection();
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke3Color, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              colorCtrl->SetValue(str);
            }
          enableReplacement->Enable(false);
          replacementCtrl->Enable(false);
      } else
        {
          typeBox->SetSelection(1);
          colorCtrl->Enable(false);
          pick->Enable(false);
          GridCtrl3->Enable(true);
          int sel = List->FindByXLinkHref(Stroke3XLinkHref);
          if (sel >= 0)
            GridCtrl3->SelectRow(sel);
          enableReplacement->Enable(true);
          if (EnableStroke3Replacement == true)
            {
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke3ColorReplacement, color);
              if (color.IsOk() == true)
                {
                  char hex[16];
                  sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                          color.Blue());
                  wxString str = wxString::FromUTF8(hex);
                  replacementCtrl->SetValue(str);
                  replacementCtrl->Enable(true);
              } else
                replacementCtrl->Enable(false);
            }
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_WIDTH);
  sprintf(dummy, "%1.2f", Stroke3Width);
  str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  if (EnableStroke3 == false)
    widthCtrl->Enable(false);
  else
    widthCtrl->Enable(true);
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE3_LINEJOIN);
  switch (Stroke3LineJoin)
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
  if (EnableStroke3 == false)
    lineJoinCtrl->Enable(false);
  else
    lineJoinCtrl->Enable(true);
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE3_LINECAP);
  switch (Stroke3LineCap)
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
  if (EnableStroke3 == false)
    lineCapCtrl->Enable(false);
  else
    lineCapCtrl->Enable(true);
  wxTextCtrl *dashArrayCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_DASHARRAY);
  wxString value;
  NormalizedDashArray(value, 2);
  dashArrayCtrl->SetValue(value);
  if (EnableStroke3 == false)
    dashArrayCtrl->Enable(false);
  else
    dashArrayCtrl->Enable(true);
  wxTextCtrl *offsetCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE3_DASHOFFSET);
  if (Stroke3DashCount == 0)
    offsetCtrl->SetValue(wxT("0.0"));
  else
    {
      char dummy[64];
      sprintf(dummy, "%1.2f", Stroke3DashOffset);
      wxString str = wxString::FromUTF8(dummy);
      offsetCtrl->SetValue(str);
    }
  if (EnableStroke3 == false)
    offsetCtrl->Enable(false);
  else
    offsetCtrl->Enable(true);
}

bool SimpleLineSymbolizerDialog::RetrievePreviewPage()
{
//
// retrieving params from the PREVIEW page
//
  return true;
}

void SimpleLineSymbolizerDialog::PrepareLinestringPath(void *xctx,
                                                       double
                                                       perpendicular_offset)
{
// Visual Preview
  gaiaDynamicLinePtr dyn = gaiaAllocDynamicLine();
  gaiaAppendPointToDynamicLine(dyn, 210.0, 80.0);
  gaiaAppendPointToDynamicLine(dyn, 210.0, 150.0);
  gaiaAppendPointToDynamicLine(dyn, 175.0, 150.0);
  double pi = 3.14159265359;
  for (double rads = (2.0 * pi) - (pi / 2.0); rads >= 0.0; rads -= 0.666666)
    {
      double x = 135.0 + (115.0 * cos(rads));
      double y = 150.0 + (115.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  for (double rads = pi; rads <= (pi * 2.0) + (pi / 2.0); rads += 0.1)
    {
      double x = 365.0 + (115.0 * cos(rads));
      double y = 150.0 + (115.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  gaiaAppendPointToDynamicLine(dyn, 365.0, 150.0);
  gaiaAppendPointToDynamicLine(dyn, 340.0, 200.0);
  gaiaAppendPointToDynamicLine(dyn, 310.0, 200.0);
  gaiaAppendPointToDynamicLine(dyn, 310.0, 100.0);
  gaiaAppendPointToDynamicLine(dyn, 410.0, 100.0);
  gaiaAppendPointToDynamicLine(dyn, 410.0, 200.0);

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

// preparing the Line Path
  ln = geom2->FirstLinestring;
  rl2GraphicsContextPtr ctx = (rl2GraphicsContextPtr) xctx;
  for (iv = 0; iv < ln->Points; iv++)
    {
      double x;
      double y;
      gaiaGetPoint(ln->Coords, iv, &x, &y);
      if (iv == 0)
        rl2_graph_move_to_point(ctx, x, y);
      else
        rl2_graph_add_line_to_path(ctx, x, y);
    }
  gaiaFreeGeomColl(geom2);
}

void SimpleLineSymbolizerDialog::UpdatePreviewPage()
{
//
// updating the PREVIEW page
//
  rl2GraphicsPatternPtr pattern1 = NULL;
  rl2GraphicsPatternPtr pattern2 = NULL;
  rl2GraphicsPatternPtr pattern3 = NULL;
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
  rl2GraphicsContextPtr ctx = NULL;
  ctx = rl2_graph_create_context(MainFrame->GetRL2PrivateData(), 500, 300);
  if (ctx == NULL)
    return;
// transparent background initialization
  rl2_graph_set_brush(ctx, 251, 5, 249, 255);
  rl2_graph_draw_rectangle(ctx, -1, -1, 501, 301);

  if (HasGraphic1 == false)
    {
      // preparing a Color-based Pen
      double aleph = 255.0 * Stroke1Opacity;
      if (aleph < 0.0)
        aleph = 0.0;
      if (aleph > 255.0)
        aleph = 255.0;
      unsigned char alpha = aleph;
      wxColour color = wxNullColour;
      ColorMapEntry::GetWxColor(Stroke1Color, color);
      if (Stroke1DashCount == 0)
        rl2_graph_set_solid_pen(ctx, color.Red(), color.Green(), color.Blue(),
                                alpha, Stroke1Width, Stroke1LineCap,
                                Stroke1LineJoin);
      else
        rl2_graph_set_dashed_pen(ctx, color.Red(), color.Green(), color.Blue(),
                                 alpha, Stroke1Width, Stroke1LineCap,
                                 Stroke1LineJoin, Stroke1DashCount,
                                 Stroke1DashArray, Stroke1DashOffset);
  } else
    {
      // preparing a Pattern-based Pen
      pattern1 =
        rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                 Stroke1XLinkHref.ToUTF8(), 1);
      if (pattern1 != NULL)
        {
          if (EnableStroke1Replacement)
            {
              // attempting to recolor the External Graphic resource
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke1ColorReplacement, color);
              rl2_graph_pattern_recolor(pattern1, color.Red(), color.Green(),
                                        color.Blue());
            }
          if (Stroke1Opacity < 1.0)
            {
              // setting up the required transparency
              double aleph = 255.0 * Stroke1Opacity;
              if (aleph < 0.0)
                aleph = 0.0;
              if (aleph > 255.0)
                aleph = 255.0;
              unsigned char alpha = aleph;
              rl2_graph_pattern_transparency(pattern1, alpha);
            }
          if (Stroke1DashCount == 0)
            rl2_graph_set_pattern_solid_pen(ctx, pattern1, Stroke1Width,
                                            Stroke1LineCap, Stroke1LineJoin);
          else
            rl2_graph_set_pattern_dashed_pen(ctx, pattern1, Stroke1Width,
                                             Stroke1LineCap, Stroke1LineJoin,
                                             Stroke1DashCount, Stroke1DashArray,
                                             Stroke1DashOffset);
      } else
        {
          // invalid Pattern: defaulting to a black Pen
          if (Stroke1DashCount == 0)
            rl2_graph_set_solid_pen(ctx, 0, 0, 0, 255, Stroke1Width,
                                    Stroke1LineCap, Stroke1LineJoin);
          else
            rl2_graph_set_dashed_pen(ctx, 0, 0, 0, 255, Stroke1Width,
                                     Stroke1LineCap, Stroke1LineJoin,
                                     Stroke1DashCount, Stroke1DashArray,
                                     Stroke1DashOffset);
        }
    }

// applying the Stroke #1
  PrepareLinestringPath(ctx, PerpendicularOffset1);
  rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);

  if (EnableStroke2 == true)
    {
      // applying the Stroke #2
      if (HasGraphic2 == false)
        {
          // preparing a Color-based Pen
          double aleph = 255.0 * Stroke2Opacity;
          if (aleph < 0.0)
            aleph = 0.0;
          if (aleph > 255.0)
            aleph = 255.0;
          unsigned char alpha = aleph;
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke2Color, color);
          if (Stroke2DashCount == 0)
            rl2_graph_set_solid_pen(ctx, color.Red(), color.Green(),
                                    color.Blue(), alpha, Stroke2Width,
                                    Stroke2LineCap, Stroke2LineJoin);
          else
            rl2_graph_set_dashed_pen(ctx, color.Red(), color.Green(),
                                     color.Blue(), alpha, Stroke2Width,
                                     Stroke2LineCap, Stroke2LineJoin,
                                     Stroke2DashCount, Stroke2DashArray,
                                     Stroke2DashOffset);
      } else
        {
          // preparing a Pattern-based Pen
          pattern2 =
            rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                     Stroke2XLinkHref.ToUTF8(),
                                                     1);
          if (pattern2 != NULL)
            {
              if (EnableStroke2Replacement)
                {
                  // attempting to recolor the External Graphic resource
                  wxColour color = wxNullColour;
                  ColorMapEntry::GetWxColor(Stroke2ColorReplacement, color);
                  rl2_graph_pattern_recolor(pattern2, color.Red(),
                                            color.Green(), color.Blue());
                }
              if (Stroke2Opacity < 1.0)
                {
                  // setting up the required transparency
                  double aleph = 255.0 * Stroke2Opacity;
                  if (aleph < 0.0)
                    aleph = 0.0;
                  if (aleph > 255.0)
                    aleph = 255.0;
                  unsigned char alpha = aleph;
                  rl2_graph_pattern_transparency(pattern2, alpha);
                }
              if (Stroke2DashCount == 0)
                rl2_graph_set_pattern_solid_pen(ctx, pattern2, Stroke2Width,
                                                Stroke2LineCap,
                                                Stroke2LineJoin);
              else
                rl2_graph_set_pattern_dashed_pen(ctx, pattern2, Stroke2Width,
                                                 Stroke2LineCap,
                                                 Stroke2LineJoin,
                                                 Stroke2DashCount,
                                                 Stroke2DashArray,
                                                 Stroke2DashOffset);
          } else
            {
              // invalid Pattern: defaulting to a black Pen
              if (Stroke2DashCount == 0)
                rl2_graph_set_solid_pen(ctx, 0, 0, 0, 255, Stroke2Width,
                                        Stroke2LineCap, Stroke2LineJoin);
              else
                rl2_graph_set_dashed_pen(ctx, 0, 0, 0, 255, Stroke2Width,
                                         Stroke2LineCap, Stroke2LineJoin,
                                         Stroke2DashCount, Stroke2DashArray,
                                         Stroke2DashOffset);
            }
        }
      PrepareLinestringPath(ctx, PerpendicularOffset2);
      rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
    }

  if (EnableStroke3 == true)
    {
      // applying the Stroke #3
      if (HasGraphic3 == false)
        {
          // preparing a Color-based Pen
          double aleph = 255.0 * Stroke3Opacity;
          if (aleph < 0.0)
            aleph = 0.0;
          if (aleph > 255.0)
            aleph = 255.0;
          unsigned char alpha = aleph;
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke3Color, color);
          if (Stroke3DashCount == 0)
            rl2_graph_set_solid_pen(ctx, color.Red(), color.Green(),
                                    color.Blue(), alpha, Stroke3Width,
                                    Stroke3LineCap, Stroke3LineJoin);
          else
            rl2_graph_set_dashed_pen(ctx, color.Red(), color.Green(),
                                     color.Blue(), alpha, Stroke3Width,
                                     Stroke3LineCap, Stroke3LineJoin,
                                     Stroke3DashCount, Stroke3DashArray,
                                     Stroke3DashOffset);
      } else
        {
          // preparing a Pattern-based Pen
          pattern3 =
            rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                     Stroke3XLinkHref.ToUTF8(),
                                                     1);
          if (pattern3 != NULL)
            {
              if (EnableStroke3Replacement)
                {
                  // attempting to recolor the External Graphic resource
                  wxColour color = wxNullColour;
                  ColorMapEntry::GetWxColor(Stroke3ColorReplacement, color);
                  rl2_graph_pattern_recolor(pattern3, color.Red(),
                                            color.Green(), color.Blue());
                }
              if (Stroke3Opacity < 1.0)
                {
                  // setting up the required transparency
                  double aleph = 255.0 * Stroke3Opacity;
                  if (aleph < 0.0)
                    aleph = 0.0;
                  if (aleph > 255.0)
                    aleph = 255.0;
                  unsigned char alpha = aleph;
                  rl2_graph_pattern_transparency(pattern3, alpha);
                }
              if (Stroke3DashCount == 0)
                rl2_graph_set_pattern_solid_pen(ctx, pattern3, Stroke3Width,
                                                Stroke3LineCap,
                                                Stroke3LineJoin);
              else
                rl2_graph_set_pattern_dashed_pen(ctx, pattern3, Stroke3Width,
                                                 Stroke3LineCap,
                                                 Stroke3LineJoin,
                                                 Stroke3DashCount,
                                                 Stroke3DashArray,
                                                 Stroke3DashOffset);
          } else
            {
              // invalid Pattern: defaulting to a black Pen
              if (Stroke3DashCount == 0)
                rl2_graph_set_solid_pen(ctx, 0, 0, 0, 255, Stroke3Width,
                                        Stroke3LineCap, Stroke3LineJoin);
              else
                rl2_graph_set_dashed_pen(ctx, 0, 0, 0, 255, Stroke3Width,
                                         Stroke3LineCap, Stroke3LineJoin,
                                         Stroke3DashCount, Stroke3DashArray,
                                         Stroke3DashOffset);
            }
        }
      PrepareLinestringPath(ctx, PerpendicularOffset3);
      rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
    }
// creating the RGB array
  unsigned char *rgb_array = rl2_graph_get_context_rgb_array(ctx);
  rl2_graph_destroy_context(ctx);
  if (pattern1 != NULL)
    rl2_graph_destroy_pattern(pattern1);
  if (pattern2 != NULL)
    rl2_graph_destroy_pattern(pattern2);
  if (pattern3 != NULL)
    rl2_graph_destroy_pattern(pattern3);
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
  dc.SelectObject(wxNullBitmap);
// updating the GUI Preview
  SymbolizerPreview *previewCtrl =
    (SymbolizerPreview *) FindWindow(ID_SYMBOLIZER_PREVIEW);
  previewCtrl->SetBitmap(bmp2);
}

void SimpleLineSymbolizerDialog::OnPageChanging(wxNotebookEvent & event)
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
        ret = RetrieveStroke1Page();
        break;
      case 2:
        ret = RetrieveStroke2Page();
        break;
      case 3:
        ret = RetrieveStroke3Page();
        break;
      case 4:
        ret = RetrievePreviewPage();
        break;
    };
  if (ret != true)
    event.Veto();
}

void SimpleLineSymbolizerDialog::OnPageChanged(wxNotebookEvent & event)
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
        UpdateStroke1Page();
        break;
      case 2:
        UpdateStroke2Page();
        break;
      case 3:
        UpdateStroke3Page();
        break;
      case 4:
        UpdatePreviewPage();
        break;
    };
}

char *SimpleLineSymbolizerDialog::DoCreateFeatureTypeXML()
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
  xml = sqlite3_mprintf("%s\t\t<LineSymbolizer uom=\"%s\">\r\n", prev, cstr);
  sqlite3_free(prev);
  prev = xml;
  xml = sqlite3_mprintf("%s\t\t\t<Stroke>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasGraphic1 == true)
    {
      // using an External Graphic
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<GraphicStroke>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
         prev);
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(Stroke1XLinkHref.ToUTF8()) + 1];
      strcpy(str, Stroke1XLinkHref.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(Stroke1MimeType.ToUTF8()) + 1];
      strcpy(str, Stroke1MimeType.ToUTF8());
      xml =
        sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      if (EnableStroke1Replacement == true)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke1ColorReplacement.ToUTF8()) + 1];
          strcpy(str, Stroke1ColorReplacement.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicStroke>\r\n",
         prev);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      // using a Solid Color
      str = new char[strlen(Stroke1Color.ToUTF8()) + 1];
      strcpy(str, Stroke1Color.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
         str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
     prev, Stroke1Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
     prev, Stroke1Width);
  sqlite3_free(prev);
  prev = xml;
  switch (Stroke1LineJoin)
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
  switch (Stroke1LineCap)
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
  if (Stroke1DashCount > 0 && Stroke1DashArray != NULL)
    {
      wxString dash;
      NormalizedDashArray(dash, 0, ' ');
      str = new char[strlen(dash.ToUTF8()) + 1];
      strcpy(str, dash.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      if (Stroke1DashOffset != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
             prev, Stroke1DashOffset);
          sqlite3_free(prev);
          prev = xml;
        }
    }
  xml = sqlite3_mprintf("%s\t\t\t</Stroke>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (PerpendicularOffset1 != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n", prev,
         PerpendicularOffset1);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t</LineSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (EnableStroke2 == true)
    {
      // Stroke #2
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
        sqlite3_mprintf("%s\t\t<LineSymbolizer uom=\"%s\">\r\n", prev, cstr);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t<Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (HasGraphic2 == true)
        {
          // using an External Graphic
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<GraphicStroke>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke2XLinkHref.ToUTF8()) + 1];
          strcpy(str, Stroke2XLinkHref.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke2MimeType.ToUTF8()) + 1];
          strcpy(str, Stroke2MimeType.ToUTF8());
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev,
                            str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (EnableStroke2Replacement == true)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Stroke2ColorReplacement.ToUTF8()) + 1];
              strcpy(str, Stroke2ColorReplacement.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
            }
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicStroke>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a Solid Color
          str = new char[strlen(Stroke2Color.ToUTF8()) + 1];
          strcpy(str, Stroke2Color.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, Stroke2Opacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, Stroke2Width);
      sqlite3_free(prev);
      prev = xml;
      switch (Stroke2LineJoin)
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
      switch (Stroke2LineCap)
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
      if (Stroke2DashCount > 0 && Stroke2DashArray != NULL)
        {
          wxString dash;
          NormalizedDashArray(dash, 1, ' ');
          str = new char[strlen(dash.ToUTF8()) + 1];
          strcpy(str, dash.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (Stroke2DashOffset != 0.0)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                 prev, Stroke2DashOffset);
              sqlite3_free(prev);
              prev = xml;
            }
        }
      xml = sqlite3_mprintf("%s\t\t\t</Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (PerpendicularOffset2 != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n",
             prev, PerpendicularOffset2);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</LineSymbolizer>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EnableStroke3 == true)
    {
      // Stroke #3
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
        sqlite3_mprintf("%s\t\t<LineSymbolizer uom=\"%s\">\r\n", prev, cstr);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t<Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (HasGraphic3 == true)
        {
          // using an External Graphic
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<GraphicStroke>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke3XLinkHref.ToUTF8()) + 1];
          strcpy(str, Stroke3XLinkHref.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke3MimeType.ToUTF8()) + 1];
          strcpy(str, Stroke3MimeType.ToUTF8());
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev,
                            str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (EnableStroke3Replacement == true)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Stroke3ColorReplacement.ToUTF8()) + 1];
              strcpy(str, Stroke3ColorReplacement.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
            }
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicStroke>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a Solid Color
          str = new char[strlen(Stroke3Color.ToUTF8()) + 1];
          strcpy(str, Stroke3Color.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, Stroke3Opacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, Stroke3Width);
      sqlite3_free(prev);
      prev = xml;
      switch (Stroke3LineJoin)
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
      switch (Stroke3LineCap)
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
      if (Stroke3DashCount > 0 && Stroke3DashArray != NULL)
        {
          wxString dash;
          NormalizedDashArray(dash, 0, ' ');
          str = new char[strlen(dash.ToUTF8()) + 1];
          strcpy(str, dash.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (Stroke3DashOffset != 0.0)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                 prev, Stroke1DashOffset);
              sqlite3_free(prev);
              prev = xml;
            }
        }
      xml = sqlite3_mprintf("%s\t\t\t</Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (PerpendicularOffset3 != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n",
             prev, PerpendicularOffset3);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</LineSymbolizer>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</FeatureTypeStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *SimpleLineSymbolizerDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - LineSymbolizer
//
  char *str;
  const char *cstr;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
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
  xml = sqlite3_mprintf("%s\t<Stroke>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (HasGraphic1 == true)
    {
      // using an External Graphic
      xml =
        sqlite3_mprintf
        ("%s\t\t<GraphicStroke>\r\n\t\t\t<Graphic>\r\n\t\t\t\t<ExternalGraphic>\r\n",
         prev);
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(Stroke1XLinkHref.ToUTF8()) + 1];
      strcpy(str, Stroke1XLinkHref.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      str = new char[strlen(Stroke1MimeType.ToUTF8()) + 1];
      strcpy(str, Stroke1MimeType.ToUTF8());
      xml = sqlite3_mprintf("%s\t\t\t\t\t<Format>%s</Format>\r\n", prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      if (EnableStroke1Replacement == true)
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
          str = new char[strlen(Stroke1ColorReplacement.ToUTF8()) + 1];
          strcpy(str, Stroke1ColorReplacement.ToUTF8());
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
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t</ExternalGraphic>\r\n\t\t\t</Graphic>\r\n\t\t</GraphicStroke>\r\n",
         prev);
      sqlite3_free(prev);
      prev = xml;
  } else
    {
      // using a Solid Color
      str = new char[strlen(Stroke1Color.ToUTF8()) + 1];
      strcpy(str, Stroke1Color.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
         str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
    }
  xml =
    sqlite3_mprintf
    ("%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
     prev, Stroke1Opacity);
  sqlite3_free(prev);
  prev = xml;
  xml =
    sqlite3_mprintf
    ("%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n", prev,
     Stroke1Width);
  sqlite3_free(prev);
  prev = xml;
  switch (Stroke1LineJoin)
    {
      case RL2_PEN_JOIN_MITER:
        xml =
          sqlite3_mprintf
          ("%s\t\t<SvgParameter name=\"stroke-linejoin\">mitre</SvgParameter>\r\n",
           prev);
        sqlite3_free(prev);
        prev = xml;
        break;
      case RL2_PEN_JOIN_BEVEL:
        xml =
          sqlite3_mprintf
          ("%s\t\t<SvgParameter name=\"stroke-linejoin\">bevel</SvgParameter>\r\n",
           prev);
        sqlite3_free(prev);
        prev = xml;
        break;
      default:
        xml =
          sqlite3_mprintf
          ("%s\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
           prev);
        sqlite3_free(prev);
        prev = xml;
        break;
    };
  switch (Stroke1LineCap)
    {
      case RL2_PEN_CAP_BUTT:
        xml =
          sqlite3_mprintf
          ("%s\t\t<SvgParameter name=\"stroke-linecap\">butt</SvgParameter>\r\n",
           prev);
        sqlite3_free(prev);
        prev = xml;
        break;
      case RL2_PEN_CAP_SQUARE:
        xml =
          sqlite3_mprintf
          ("%s\t\t<SvgParameter name=\"stroke-linecap\">square</SvgParameter>\r\n",
           prev);
        sqlite3_free(prev);
        prev = xml;
        break;
      default:
        xml =
          sqlite3_mprintf
          ("%s\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
           prev);
        sqlite3_free(prev);
        prev = xml;
        break;
    };
  if (Stroke1DashCount > 0 && Stroke1DashArray != NULL)
    {
      wxString dash;
      NormalizedDashArray(dash, 0, ' ');
      str = new char[strlen(dash.ToUTF8()) + 1];
      strcpy(str, dash.ToUTF8());
      xml =
        sqlite3_mprintf
        ("%s\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
         prev, str);
      delete[]str;
      sqlite3_free(prev);
      prev = xml;
      if (Stroke1DashOffset != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
             prev, Stroke1DashOffset);
          sqlite3_free(prev);
          prev = xml;
        }
    }
  xml = sqlite3_mprintf("%s\t</Stroke>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (PerpendicularOffset1 != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t<PerpendicularOffset>%1.2f</PerpendicularOffset>\r\n", prev,
         PerpendicularOffset1);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s</LineSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void SimpleLineSymbolizerDialog::OnInsert(wxCommandEvent & WXUNUSED(event))
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
        RetrieveStroke1Page();
        break;
      case 2:
        RetrieveStroke2Page();
        break;
      case 3:
        RetrieveStroke3Page();
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
  if (MinScale == true || MaxScale == true || EnableStroke2 == true
      || EnableStroke3 == true)
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

void SimpleLineSymbolizerDialog::OnExport(wxCommandEvent & WXUNUSED(event))
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
        RetrieveStroke1Page();
        break;
      case 2:
        RetrieveStroke2Page();
        break;
      case 3:
        RetrieveStroke3Page();
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
                          wxT("Exporting an SLD/SE LineSymbolizer to a file"),
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
          if (MinScale == true || MaxScale == true || EnableStroke2 == true
              || EnableStroke3 == true)
            xml = DoCreateFeatureTypeXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE LineSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void SimpleLineSymbolizerDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
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
        RetrieveStroke1Page();
        break;
      case 2:
        RetrieveStroke2Page();
        break;
      case 3:
        RetrieveStroke3Page();
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
  if (MinScale == true || MaxScale == true || EnableStroke2 == true
      || EnableStroke3 == true)
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

void SimpleLineSymbolizerDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}

bool MyFrame::ValidateVectorStyle(void **blob, int *blob_size, const char *xml)
{
//
// attempting to parse and validate a Vector Style
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

// Checking if really is a Vector Style
  stmt = NULL;
  sql = "SELECT XB_IsSldSEVectorStyle(?)";
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

bool MyFrame::DoInsertVectorSymbolizer(char *xml)
{
//
// attempting to register a VectorSymbolizer
//
  void *blob = NULL;
  int blob_size;
  ::wxBeginBusyCursor();
  if (ValidateVectorStyle(&blob, &blob_size, xml) != true)
    {
      ::wxEndBusyCursor();
      wxMessageBox(wxT("Error: not a valid SLD/SE VectorSymbolizer"),
                   wxT("spatialite_gui"), wxOK | wxICON_ERROR, this);
      sqlite3_free(xml);
      return false;
    }
  ::wxEndBusyCursor();

  sqlite3_stmt *stmt = NULL;
  const char *sql = "SELECT SE_RegisterVectorStyle(?)";
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


ExternalGraphic::ExternalGraphic(wxString & xlink_href, wxString & title,
                                 wxString & abstract, wxString & mime_type,
                                 const void *blob, int blob_sz)
{
// ctor
  XLinkHref = xlink_href;
  Title = title;
  Abstract = abstract;
  MimeType = mime_type;
  if (DoBuildGraphic(mime_type, blob, blob_sz) != true)
    DoBuildDefaultGraphic();
  Next = NULL;
}

bool ExternalGraphic::DoBuildGraphic(wxString & mime_type, const void *blob,
                                     int blob_sz)
{
// attempting to build an Image from BLOB
  rl2RasterPtr raster = NULL;
  if (mime_type.CmpNoCase(wxT("image/png")) == 0)
    raster = rl2_raster_from_png((const unsigned char *) blob, blob_sz, 1);
  if (mime_type.CmpNoCase(wxT("image/gif")) == 0)
    raster = rl2_raster_from_gif((const unsigned char *) blob, blob_sz);
  if (mime_type.CmpNoCase(wxT("image/jpeg")) == 0)
    raster = rl2_raster_from_jpeg((const unsigned char *) blob, blob_sz);
  if (mime_type.CmpNoCase(wxT("image/svg+xml")) == 0)
    {
      // preparing an SVG icon
      char *svg = gaiaXmlTextFromBlob((const unsigned char *) blob, blob_sz, 0);
      int svg_sz = strlen(svg);
      rl2SvgPtr svg_handle =
        rl2_create_svg((const unsigned char *) svg, svg_sz);
      if (svg_handle != NULL)
        {
          double SvgWidth;
          double SvgHeight;
          if (rl2_get_svg_size(svg_handle, &SvgWidth, &SvgHeight) == RL2_OK)
            {
              double w = SvgWidth;
              double h = SvgHeight;
              if (SvgWidth > 48.0 || SvgHeight > 24.0)
                {
                  while (w > 48.0 || h > 24.0)
                    {
                      // rescaling
                      w *= 0.999;
                      h *= 0.999;
                    }
              } else
                {
                  while (w < 47.0 && h < 23.0)
                    {
                      // rescaling
                      w *= 1.001;
                      h *= 1.001;
                    }
                }
              double sz = w;
              if (h > sz)
                sz = h;
              raster = rl2_raster_from_svg(svg_handle, sz);
            }
          rl2_destroy_svg(svg_handle);
        }
      free(svg);
    }
  if (raster == NULL)
    return false;

// ok, building a wxImage
  unsigned int width;
  unsigned int height;
  unsigned char *rgbaArray = NULL;
  int rgbaSize;
  if (rl2_get_raster_size(raster, &width, &height) == RL2_OK)
    {
      if (rl2_raster_data_to_RGBA(raster, &rgbaArray, &rgbaSize) != RL2_OK)
        rgbaArray = NULL;
    }
  rl2_destroy_raster(raster);
  if (rgbaArray == NULL)
    return false;
  // creating the Image from RGB array
  unsigned int x;
  unsigned int y;
  wxImage img = wxImage(width, height);
  unsigned char *p = rgbaArray;
  img.SetAlpha();
  for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
        {
          unsigned char r = *p++;
          unsigned char g = *p++;
          unsigned char b = *p++;
          unsigned char alpha = *p++;
          img.SetRGB(x, y, r, g, b);
          img.SetAlpha(x, y, alpha);
        }
    }
  free(rgbaArray);

// normalized rescaling
  int norm_width = width;
  int norm_height = height;
  double incr;
  double ww = width;
  double hh = height;
  if (width > 48 || height > 24)
    {
      incr = 0.999;
      while (1)
        {
          ww *= incr;
          hh *= incr;
          if (ww > 47.0 || hh > 23.0)
            {
              norm_width = ww;
              norm_height = hh;
          } else
            break;
        }
  } else
    {
      incr = 1.001;
      while (1)
        {
          ww *= incr;
          hh *= incr;
          if (ww <= 48.0 && hh <= 24.0)
            {
              norm_width = ww;
              norm_height = hh;
          } else
            break;
        }
    }
  Graphic = img.Rescale(norm_width, norm_height);


  return true;
}

void ExternalGraphic::DoBuildDefaultGraphic()
{
// building a default gray image
  Graphic = wxImage(48, 24);
  for (int y = 0; y < 24; y++)
    {
      for (int x = 0; x < 48; x++)
        Graphic.SetRGB(x, y, 128, 128, 128);
    }
}

ExternalGraphicList::~ExternalGraphicList()
{
// dtor
  ExternalGraphic *pE;
  ExternalGraphic *pEn;
  pE = First;
  while (pE != NULL)
    {
      pEn = pE->GetNext();
      delete pE;
      pE = pEn;
    }
}

void ExternalGraphicList::Add(wxString & xlink_href, wxString & title,
                              wxString & abstract, wxString & mime_type,
                              const void *blob, int blob_sz)
{
// inserting a new External Graphic into the list
  ExternalGraphic *pE =
    new ExternalGraphic(xlink_href, title, abstract, mime_type, blob, blob_sz);
  if (First == NULL)
    First = pE;
  if (Last != NULL)
    Last->SetNext(pE);
  Last = pE;
}

void ExternalGraphicList::FindByIndex(int idx, wxString & xlink_href,
                                      wxString & mime_type)
{
// searching an entry by its position
  ExternalGraphic *pE;
  int count = 0;
  pE = First;
  while (pE != NULL)
    {
      if (idx == count)
        {
          xlink_href = pE->GetXLinkHref();
          mime_type = pE->GetMimeType();
          return;
        }
      count++;
      pE = pE->GetNext();
    }
  xlink_href = wxT("");
}

int ExternalGraphicList::FindByXLinkHref(wxString & xlink_href)
{
// searching an entry by XLinkHref value 
  ExternalGraphic *pE;
  int count = 0;
  pE = First;
  while (pE != NULL)
    {
      if (xlink_href.CmpNoCase(pE->GetXLinkHref()) == 0)
        return count;
      count++;
      pE = pE->GetNext();
    }
  return -1;
}

ExternalGraphicList *MyFrame::FindExternalGraphic(bool no_svg)
{
// will retrieve all registered External Graphics
  ExternalGraphicList *list = new ExternalGraphicList();
  sqlite3_stmt *stmt = NULL;
  const char *sql;

  sql =
    "SELECT xlink_href, title, abstract, GetMimeType(resource), resource "
    "FROM SE_external_graphics";
  int ret = sqlite3_prepare_v2(SqliteHandle, sql, strlen(sql), &stmt, NULL);
  if (ret != SQLITE_OK)
    {
      delete list;
      return NULL;
    }
  while (1)
    {
      ret = sqlite3_step(stmt);
      if (ret == SQLITE_DONE)
        break;
      if (ret == SQLITE_ROW)
        {
          wxString xlink_href;
          wxString title;
          wxString abstract;
          wxString mime_type;
          const void *blob = NULL;
          int blob_sz = 0;
          const char *str = (const char *) sqlite3_column_text(stmt, 0);
          xlink_href = wxString::FromUTF8(str);
          str = (const char *) sqlite3_column_text(stmt, 1);
          title = wxString::FromUTF8(str);
          str = (const char *) sqlite3_column_text(stmt, 2);
          abstract = wxString::FromUTF8(str);
          str = (const char *) sqlite3_column_text(stmt, 3);
          if (no_svg == true && strcasecmp(str, "image/svg+xml") == 0)
            continue;
          mime_type = wxString::FromUTF8(str);
          if (sqlite3_column_type(stmt, 4) == SQLITE_BLOB)
            {
              blob = sqlite3_column_blob(stmt, 4);
              blob_sz = sqlite3_column_bytes(stmt, 4);
            }
          list->Add(xlink_href, title, abstract, mime_type, blob, blob_sz);
      } else
        {
          sqlite3_finalize(stmt);
          delete list;
          return NULL;
        }
    }
  sqlite3_finalize(stmt);
  return list;
}

void MyGraphicCellRenderer::Draw(wxGrid & grid, wxGridCellAttr & attr,
                                 wxDC & dc, const wxRect & rect, int row,
                                 int col, bool isSelected)
{
// drawing a Graphic cell
  if (col == row && isSelected)
    row = col;                  // silencing stupid compiler warnings about unused args

  wxBitmap bmp = wxBitmap(*Graphic);
  wxColour color = attr.GetBackgroundColour();
  if (grid.IsEnabled() == false)
    {
      color = wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
      wxImage img = Graphic->ConvertToGreyscale();
      bmp = wxBitmap(img);
    }
  dc.SetBrush(wxBrush(color));
  dc.SetPen(*wxTRANSPARENT_PEN);
  dc.DrawRectangle(rect);
  int x = (rect.GetWidth() - bmp.GetWidth()) / 2;
  int y = (rect.GetHeight() - bmp.GetHeight()) / 2;
  dc.DrawBitmap(bmp, rect.x + x, rect.y + y, true);
}

SymbolizerPreview::SymbolizerPreview(wxPropertySheetDialog * parent,
                                     wxWindow * panel, wxWindowID id,
                                     const wxBitmap & bmp,
                                     const wxSize & size):wxStaticBitmap(panel,
                                                                         id,
                                                                         bmp,
                                                                         wxDefaultPosition,
                                                                         size)
{
  Parent = parent;
}

SimplePolygonSymbolizerDialog::SimplePolygonSymbolizerDialog()
{
// ctor
  Stroke1DashArray = NULL;
  Stroke2DashArray = NULL;
  List = NULL;
}

SimplePolygonSymbolizerDialog::~SimplePolygonSymbolizerDialog()
{
// dtor
  if (Stroke1DashArray != NULL)
    delete[]Stroke1DashArray;
  if (Stroke2DashArray != NULL)
    delete[]Stroke2DashArray;
  if (List)
    delete List;
}

bool SimplePolygonSymbolizerDialog::Create(MyFrame * parent)
{
//
// creating the dialog
//
  MainFrame = parent;
  Uom = GUI_UOM_PIXEL;
  MinScale = false;
  MaxScale = false;
  DisplacementX1 = 0.0;
  DisplacementY1 = 0.0;
  PerpendicularOffset1 = 0.0;
  EnableStroke1 = true;
  HasGraphicStroke1 = false;
  Stroke1Opacity = 1.0;
  Stroke1Color = wxT("#000000");
  EnableStroke1Replacement = false;
  Stroke1ColorReplacement = wxT("#000000");
  Stroke1Width = 1.0;
  Stroke1LineJoin = RL2_PEN_JOIN_ROUND;
  Stroke1LineCap = RL2_PEN_CAP_ROUND;
  Stroke1DashCount = 0;
  Stroke1DashOffset = 0.0;
  EnableFill1 = true;
  HasGraphicFill1 = false;
  Fill1Opacity = 1.0;
  Fill1Color = wxT("#808080");
  EnableFill1Replacement = false;
  Fill1ColorReplacement = wxT("#000000");
  EnablePolygon2 = false;
  EnableStroke2 = false;
  DisplacementX2 = 0.0;
  DisplacementY2 = 0.0;
  PerpendicularOffset2 = 0.0;
  Stroke2Opacity = 1.0;
  HasGraphicStroke2 = false;
  Stroke2Color = wxT("#000000");
  EnableStroke2Replacement = false;
  Stroke2ColorReplacement = wxT("#000000");
  Stroke2Width = 1.0;
  Stroke2LineJoin = RL2_PEN_JOIN_ROUND;
  Stroke2LineCap = RL2_PEN_CAP_ROUND;
  Stroke2DashCount = 0;
  Stroke2DashOffset = 0.0;
  EnableFill2 = false;
  HasGraphicFill2 = false;
  Fill2Opacity = 1.0;
  Fill2Color = wxT("#808080");
  EnableFill2Replacement = false;
  Fill2ColorReplacement = wxT("#000000");
  PreviewBackground = GUI_PREVIEW_BACKGROUND_CHECKERED;
  List = MainFrame->FindExternalGraphic(true);

  if (wxPropertySheetDialog::Create
      (parent, wxID_ANY, wxT("Simple Polygon Symbolizer")) == false)
    return false;
  wxBookCtrlBase *book = GetBookCtrl();
// creates individual panels
  wxPanel *mainPage = CreateMainPage(book);
  book->AddPage(mainPage, wxT("General"), true);
  wxPanel *fill1Page = CreateFill1Page(book);
  book->AddPage(fill1Page, wxT("Polygon #1 Fill"), false);
  wxPanel *stroke1Page = CreateStroke1Page(book);
  book->AddPage(stroke1Page, wxT("Polygon #1 Stroke"), false);
  wxPanel *fill2Page = CreateFill2Page(book);
  book->AddPage(fill2Page, wxT("Polygon #2 Fill"), false);
  wxPanel *stroke2Page = CreateStroke2Page(book);
  book->AddPage(stroke2Page, wxT("Polygon #2 Stroke"), false);
  wxPanel *previewPage = CreatePreviewPage(book);
  book->AddPage(previewPage, wxT("Preview"), false);

  CreateButtons();
  LayoutDialog();
// appends event handler for TAB/PAGE changing
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnPageChanging);
  Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnPageChanged);
// appends event handler for buttons
  Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePolygonSymbolizerDialog::OnQuit);
  Connect(ID_SYMBOLIZER_INSERT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePolygonSymbolizerDialog::OnInsert);
  Connect(ID_SYMBOLIZER_EXPORT, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePolygonSymbolizerDialog::OnExport);
  Connect(ID_SYMBOLIZER_COPY, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) & SimplePolygonSymbolizerDialog::OnCopy);
// centers the dialog window
  Centre();
  UpdateMainPage();
  return true;
}

wxPanel *SimplePolygonSymbolizerDialog::CreateMainPage(wxWindow * parent)
{
//
// creating the MAIN page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_MAIN);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the PolygonSymbolizer Name
  wxBoxSizer *nameSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(nameSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *nameLabel = new wxStaticText(panel, wxID_STATIC, wxT("&Name:"));
  nameSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *nameCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_NAME, wxT(""),
                                        wxDefaultPosition, wxSize(600, 22));
  nameSizer->Add(nameCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// second row: the PolygonSymbolizer Title
  wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(titleSizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxStaticText *titleLabel =
    new wxStaticText(panel, wxID_STATIC, wxT("&Title:"));
  titleSizer->Add(titleLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxTextCtrl *titleCtrl = new wxTextCtrl(panel, ID_SYMBOLIZER_TITLE, wxT(""),
                                         wxDefaultPosition, wxSize(600, 22));
  titleSizer->Add(titleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: the PolygonSymbolizer Abstract
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
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_UOM, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdUomChanged);
  Connect(ID_SYMBOLIZER_MINMAX_SCALE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdScaleChanged);
  return panel;
}

void SimplePolygonSymbolizerDialog::
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

void SimplePolygonSymbolizerDialog::
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

wxPanel *SimplePolygonSymbolizerDialog::CreateFill1Page(wxWindow * parent)
{
//
// creating the Polygon #1 Fill page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_FILL1);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the Polygon #1 Displacement and Perpendicular Offset
  wxBoxSizer *polygonBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(polygonBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *polygonBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Polygon #1"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *polygonSizer = new wxStaticBoxSizer(polygonBox, wxHORIZONTAL);
  polygonBoxSizer->Add(polygonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// first row A: Displacement
  wxBoxSizer *displacementBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  polygonSizer->Add(displacementBoxSizer, 0,
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
  polygonSizer->Add(perpendicularBoxSizer, 0,
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
// second row: Fill#1 Opacity
  wxBoxSizer *auxBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(auxBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *auxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Polygon #1 Fill"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *auxSizer = new wxStaticBoxSizer(auxBox, wxVERTICAL);
  auxBoxSizer->Add(auxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(enableSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_FILL1_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(true);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  enableSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FILL1_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: Fill color or Graphic
  wxBoxSizer *fillSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(fillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row A: Fill Type
  wxBoxSizer *fill1Sizer = new wxBoxSizer(wxVERTICAL);
  fillSizer->Add(fill1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_FILL1_TYPE,
                                       wxT("&Fill Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  fill1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
// third row B: Fill Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  fill1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Fill Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FILL1_COLOR, Fill1Color,
                   wxDefaultPosition, wxSize(80, 22));
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL1_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_FILL1_PICKER_BTN, wxT("&Pick a color"));
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  fillSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl1 =
    new wxGrid(panel, ID_SYMBOLIZER_FILL1_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl1->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl1->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl1->SetColLabelValue(1, wxT("Title"));
  GridCtrl1->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl1->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl1->SetCellRenderer(count, 0, renderer);
      GridCtrl1->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl1->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl1->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl1->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl1->AutoSize();
  GridCtrl1->EnableEditing(false);
  gridSizer->Add(GridCtrl1, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl1->Enable(false);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_FILL1_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FILL1_REPLACEMENT,
                   Fill1ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL1_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// appends event handlers
  Connect(ID_SYMBOLIZER_FILL1_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill1Changed);
  Connect(ID_SYMBOLIZER_FILL1_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill1TypeChanged);
  Connect(ID_SYMBOLIZER_FILL1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor1Changed);
  Connect(ID_SYMBOLIZER_FILL1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor1Picker);
  Connect(ID_SYMBOLIZER_FILL1_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill1EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_FILL1_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill1ReplacementChanged);
  return panel;
}

void SimplePolygonSymbolizerDialog::
OnCmdFill1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Fill #1 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL1_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableFill1 = true;
  else
    EnableFill1 = false;
  RetrieveFill1Page(false);
  UpdateFill1Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdFill1TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill Type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL1_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphicFill1 = false;
  else
    HasGraphicFill1 = true;
  RetrieveFill1Page(false);
  UpdateFill1Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdColor1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Fill color changed: updating the visual sample
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_COLOR);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FILL1_PICKER_HEX);
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

void SimplePolygonSymbolizerDialog::
OnCmdColor1Picker(wxCommandEvent & WXUNUSED(event))
{
//
// color picker
//
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_COLOR);
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

void SimplePolygonSymbolizerDialog::
OnCmdFill1EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Polygon #1 Fill ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL1_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableFill1Replacement = true;
  else
    EnableFill1Replacement = false;
  RetrieveFill1Page(false);
  UpdateFill1Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdFill1ReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FILL1_REPLACEMENT_HEX);
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

wxPanel *SimplePolygonSymbolizerDialog::CreateStroke1Page(wxWindow * parent)
{
//
// creating the STROKE #1 page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_STROKE1);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #1 Opacity 
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *enableBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *enableSizer = new wxStaticBoxSizer(enableBox, wxVERTICAL);
  opacityBoxSizer->Add(enableSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_STROKE1_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(true);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE1_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityCtrl->Enable(false);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row: Stroke color or Graphic
  wxBoxSizer *strokeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(strokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Stroke Type
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxVERTICAL);
  strokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE1_TYPE,
                                       wxT("&Stroke Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  stroke1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
  typeBox->Enable(false);
// second row B: Stroke Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  stroke1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_COLOR, Stroke1Color,
                   wxDefaultPosition, wxSize(80, 22));
  colorCtrl->Enable(false);
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
  pick->Enable(false);
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// second row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl2 =
    new wxGrid(panel, ID_SYMBOLIZER_STROKE1_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl2->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl2->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl2->SetColLabelValue(1, wxT("Title"));
  GridCtrl2->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl2->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl2->SetCellRenderer(count, 0, renderer);
      GridCtrl2->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl2->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl2->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl2->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl2->AutoSize();
  GridCtrl2->EnableEditing(false);
  gridSizer->Add(GridCtrl2, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl2->Enable(false);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_REPLACEMENT,
                   Stroke1ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE1_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: Stroke-Width, Stroke-LineJoin, Stroke-LineCap and Stroke-Dasharray 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
  widthCtrl->Enable(false);
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: LineJoin
  wxString join[3];
  join[0] = wxT("&Mitre");
  join[1] = wxT("&Round");
  join[2] = wxT("&Bevel");
  wxRadioBox *joinBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE1_LINEJOIN,
                                       wxT("&Line Join"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       join, 1,
                                       wxRA_SPECIFY_COLS);
  miscSizer->Add(joinBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  joinBox->SetSelection(1);
  joinBox->Enable(false);
// third row C: LineCap
  wxString cap[3];
  cap[0] = wxT("&Butt");
  cap[1] = wxT("&Round");
  cap[2] = wxT("&Square");
  wxRadioBox *capBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE1_LINECAP,
                                      wxT("&Line Cap"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      cap, 1,
                                      wxRA_SPECIFY_COLS);
  miscSizer->Add(capBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  capBox->SetSelection(1);
  capBox->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_DASHARRAY, wxT(""),
                   wxDefaultPosition, wxSize(200, 22));
  dashArrayCtrl->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE1_DASHOFFSET, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  dashOffsetCtrl->Enable(false);
  dash2Sizer->Add(dashOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE1_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke1Changed);
  Connect(ID_SYMBOLIZER_STROKE1_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke1TypeChanged);
  Connect(ID_SYMBOLIZER_STROKE1_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor2Changed);
  Connect(ID_SYMBOLIZER_STROKE1_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor2Picker);
  Connect(ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke1EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE1_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke1ReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE1_LINEJOIN, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdLineJoin1Changed);
  Connect(ID_SYMBOLIZER_STROKE1_LINECAP, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdLineCap1Changed);
  return panel;
}

void SimplePolygonSymbolizerDialog::
OnCmdStroke1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #1 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE1_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableStroke1 = true;
  else
    EnableStroke1 = false;
  RetrieveStroke1Page(false);
  UpdateStroke1Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdStroke1TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// stroke type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphicStroke1 = false;
  else
    HasGraphicStroke1 = true;
  RetrieveStroke1Page(false);
  UpdateStroke1Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdColor2Changed(wxCommandEvent & WXUNUSED(event))
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

void SimplePolygonSymbolizerDialog::
OnCmdColor2Picker(wxCommandEvent & WXUNUSED(event))
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

void SimplePolygonSymbolizerDialog::
OnCmdStroke1EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Polygon #1 Stroke ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableStroke1Replacement = true;
  else
    EnableStroke1Replacement = false;
  RetrieveStroke1Page(false);
  UpdateStroke1Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdStroke1ReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT_HEX);
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

void SimplePolygonSymbolizerDialog::
OnCmdLineJoin1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineJoin selection changed
//
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINEJOIN);
  switch (lineJoinCtrl->GetSelection())
    {
      case 0:
        Stroke1LineJoin = RL2_PEN_JOIN_MITER;
        break;
      case 2:
        Stroke1LineJoin = RL2_PEN_JOIN_BEVEL;
        break;
      default:
        Stroke1LineJoin = RL2_PEN_JOIN_ROUND;
        break;
    };
}

void SimplePolygonSymbolizerDialog::
OnCmdLineCap1Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineCap selection changed
//
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINECAP);
  switch (lineCapCtrl->GetSelection())
    {
      case 0:
        Stroke1LineCap = RL2_PEN_CAP_BUTT;
        break;
      case 2:
        Stroke1LineCap = RL2_PEN_CAP_SQUARE;
        break;
      default:
        Stroke1LineCap = RL2_PEN_CAP_ROUND;
        break;
    };
}

wxPanel *SimplePolygonSymbolizerDialog::CreateFill2Page(wxWindow * parent)
{
//
// creating the Polygon #2 Fill page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_FILL2);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row: the Polygon #2 Displacement and Perpendicular Offset
  wxBoxSizer *polygonBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(polygonBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *polygonBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Polygon #2"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *polygonSizer = new wxStaticBoxSizer(polygonBox, wxHORIZONTAL);
  polygonBoxSizer->Add(polygonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxCheckBox *polygon2Ctrl =
    new wxCheckBox(panel, ID_SYMBOLIZER_POLYGON2_ENABLE,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  polygon2Ctrl->SetValue(false);
  polygonSizer->Add(polygon2Ctrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// first row A: Displacement
  wxBoxSizer *displacementBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  polygonSizer->Add(displacementBoxSizer, 0,
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_X, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ1Sizer->Add(displacementXCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  displacementXCtrl->Enable(false);
  wxBoxSizer *displ2Sizer = new wxBoxSizer(wxHORIZONTAL);
  displacementSizer->Add(displ2Sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticText *displ2Label = new wxStaticText(panel, wxID_STATIC, wxT("Y"));
  displ2Sizer->Add(displ2Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxTextCtrl *displacementYCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_Y, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  displ2Sizer->Add(displacementYCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  displacementYCtrl->Enable(false);
// first row B: PerpendicularOffset
  wxBoxSizer *perpendicularBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  polygonSizer->Add(perpendicularBoxSizer, 0,
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_POLYGON2_PERPENDICULAR, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  perp1Sizer->Add(perpendicularCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  perpendicularCtrl->Enable(false);
  wxStaticText *perp1Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Positive: larger. / Negative: smaller."));
  perp1Sizer->Add(perp1Label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
  wxStaticText *perp2Label = new wxStaticText(panel, wxID_STATIC,
                                              wxT
                                              ("Drawing polygons smaller or larger than their actual geometry (Buffer)."));
  perpendicularSizer->Add(perp2Label, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row: Fill#1 Opacity
  wxBoxSizer *auxBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(auxBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *auxBox = new wxStaticBox(panel, wxID_STATIC,
                                        wxT("Polygon #2 Fill"),
                                        wxDefaultPosition,
                                        wxDefaultSize);
  wxBoxSizer *auxSizer = new wxStaticBoxSizer(auxBox, wxVERTICAL);
  auxBoxSizer->Add(auxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *enableSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(enableSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_FILL2_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(false);
  enableCtrl->Enable(false);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  enableSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_FILL2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row: Fill color or Graphic
  wxBoxSizer *fillSizer = new wxBoxSizer(wxHORIZONTAL);
  auxSizer->Add(fillSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// third row A: Fill Type
  wxBoxSizer *fill1Sizer = new wxBoxSizer(wxVERTICAL);
  fillSizer->Add(fill1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_FILL2_TYPE,
                                       wxT("&Fill Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  fill1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
// third row B: Fill Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  fill1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Fill Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FILL2_COLOR, Fill2Color,
                   wxDefaultPosition, wxSize(80, 22));
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_FILL2_PICKER_BTN, wxT("&Pick a color"));
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// third row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  fillSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl3 =
    new wxGrid(panel, ID_SYMBOLIZER_FILL2_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl3->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl3->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl3->SetColLabelValue(1, wxT("Title"));
  GridCtrl3->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl3->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl3->SetCellRenderer(count, 0, renderer);
      GridCtrl3->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl3->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl3->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl3->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl3->AutoSize();
  GridCtrl3->EnableEditing(false);
  gridSizer->Add(GridCtrl3, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl3->Enable(false);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_FILL2_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_FILL2_REPLACEMENT,
                   Fill2ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_FILL2_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// appends event handlers
  Connect(ID_SYMBOLIZER_POLYGON2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdPolygon2Changed);
  Connect(ID_SYMBOLIZER_FILL2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill2Changed);
  Connect(ID_SYMBOLIZER_FILL2_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill2TypeChanged);
  Connect(ID_SYMBOLIZER_FILL2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor3Changed);
  Connect(ID_SYMBOLIZER_FILL2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor3Picker);
  Connect(ID_SYMBOLIZER_FILL2_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill2EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_FILL2_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdFill2ReplacementChanged);
  return panel;
}

void SimplePolygonSymbolizerDialog::
OnCmdPolygon2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Polygon #2 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_POLYGON2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnablePolygon2 = true;
  else
    EnablePolygon2 = false;
  RetrieveFill2Page(false);
  UpdateFill2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdFill2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Fill #2 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableFill2 = true;
  else
    EnableFill2 = false;
  RetrieveFill2Page(false);
  UpdateFill2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdFill2TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Fill Type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL2_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphicFill2 = false;
  else
    HasGraphicFill2 = true;
  RetrieveFill2Page(false);
  UpdateFill2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdColor3Changed(wxCommandEvent & WXUNUSED(event))
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

void SimplePolygonSymbolizerDialog::
OnCmdColor3Picker(wxCommandEvent & WXUNUSED(event))
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

void SimplePolygonSymbolizerDialog::
OnCmdFill2EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Polygon #2 Stroke ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableFill2Replacement = true;
  else
    EnableFill2Replacement = false;
  RetrieveFill2Page(false);
  UpdateFill2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdFill2ReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_FILL2_REPLACEMENT_HEX);
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

wxPanel *SimplePolygonSymbolizerDialog::CreateStroke2Page(wxWindow * parent)
{
//
// creating the STROKE #2 page
//
  wxPanel *panel = new wxPanel(parent, ID_PANE_STROKE2);
  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(topSizer);
  wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
  topSizer->Add(boxSizer, 0, wxALIGN_CENTER | wxALL, 5);
// first row A: the Stroke #1 Opacity 
  wxBoxSizer *opacityBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(opacityBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxStaticBox *enableBox = new wxStaticBox(panel, wxID_STATIC,
                                           wxT("Stroke"),
                                           wxDefaultPosition,
                                           wxDefaultSize);
  wxBoxSizer *enableSizer = new wxStaticBoxSizer(enableBox, wxVERTICAL);
  opacityBoxSizer->Add(enableSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxCheckBox *enableCtrl = new wxCheckBox(panel, ID_SYMBOLIZER_STROKE2_ENABLE,
                                          wxT("Enable"),
                                          wxDefaultPosition, wxDefaultSize);
  enableCtrl->SetValue(false);
  enableCtrl->Enable(false);
  enableSizer->Add(enableCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *opacityBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("Opacity"),
                                            wxDefaultPosition,
                                            wxDefaultSize);
  wxBoxSizer *opacitySizer = new wxStaticBoxSizer(opacityBox, wxVERTICAL);
  opacityBoxSizer->Add(opacitySizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxSlider *opacityCtrl =
    new wxSlider(panel, ID_SYMBOLIZER_STROKE2_OPACITY, 100, 0, 100,
                 wxDefaultPosition, wxSize(600, 45),
                 wxSL_HORIZONTAL | wxSL_LABELS);
  opacityCtrl->Enable(false);
  opacitySizer->Add(opacityCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// second row: Stroke color or Graphic
  wxBoxSizer *strokeSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(strokeSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
// second row A: Stroke Type
  wxBoxSizer *stroke1Sizer = new wxBoxSizer(wxVERTICAL);
  strokeSizer->Add(stroke1Sizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxString type[2];
  type[0] = wxT("&Color");
  type[1] = wxT("&Graphic");
  wxRadioBox *typeBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE2_TYPE,
                                       wxT("&Stroke Type"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 2,
                                       type, 1,
                                       wxRA_SPECIFY_ROWS);
  stroke1Sizer->Add(typeBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  typeBox->SetSelection(0);
  typeBox->Enable(false);
// second row B: Stroke Color
  wxBoxSizer *colorBoxSizer = new wxBoxSizer(wxVERTICAL);
  stroke1Sizer->Add(colorBoxSizer, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
  wxStaticBox *colorBox = new wxStaticBox(panel, wxID_STATIC,
                                          wxT("Stroke Color"),
                                          wxDefaultPosition,
                                          wxDefaultSize);
  wxBoxSizer *colorSizer = new wxStaticBoxSizer(colorBox, wxVERTICAL);
  colorBoxSizer->Add(colorSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *color1Sizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(color1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *colorCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_COLOR, Stroke2Color,
                   wxDefaultPosition, wxSize(80, 22));
  colorCtrl->Enable(false);
  color1Sizer->Add(colorCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  wxBitmap bmp;
  wxColour color(0, 0, 0);
  ColorMapEntry::DoPaintColorSample(32, 32, color, bmp);
  wxStaticBitmap *sampleCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_PICKER_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  color1Sizer->Add(sampleCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
  wxBoxSizer *pickerSizer = new wxBoxSizer(wxHORIZONTAL);
  colorSizer->Add(pickerSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxButton *pick =
    new wxButton(panel, ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxT("&Pick a color"));
  pick->Enable(false);
  pickerSizer->Add(pick, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
// second row C: GRID to select an External Graphic
  wxBoxSizer *gridBoxSizer = new wxBoxSizer(wxHORIZONTAL);
  strokeSizer->Add(gridBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxStaticBox *gridBox = new wxStaticBox(panel, wxID_STATIC,
                                         wxT("External Graphic resources"),
                                         wxDefaultPosition,
                                         wxDefaultSize);
  wxBoxSizer *GridSizer = new wxStaticBoxSizer(gridBox, wxHORIZONTAL);
  gridBoxSizer->Add(GridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  wxBoxSizer *gridSizer = new wxBoxSizer(wxHORIZONTAL);
  GridSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
  GridCtrl4 =
    new wxGrid(panel, ID_SYMBOLIZER_STROKE2_GRAPHIC, wxDefaultPosition,
               wxSize(420, 150));
  int count = 0;
  ExternalGraphic *pE = List->GetFirst();
  while (pE)
    {
      // counting how many lines are there
      count++;
      pE = pE->GetNext();
    }
  GridCtrl4->CreateGrid(count, 4, wxGrid::wxGridSelectRows);
  GridCtrl4->SetColLabelValue(0, wxT("Graphic"));
  GridCtrl4->SetColLabelValue(1, wxT("Title"));
  GridCtrl4->SetColLabelValue(2, wxT("Abstract"));
  GridCtrl4->SetColLabelValue(3, wxT("MimeType"));
  count = 0;
  pE = List->GetFirst();
  while (pE)
    {
      // feeding grid rows
      MyGraphicCellRenderer *renderer = new MyGraphicCellRenderer;
      renderer->SetGraphic(pE->GetGraphic());
      GridCtrl4->SetCellRenderer(count, 0, renderer);
      GridCtrl4->SetCellValue(count, 1, pE->GetTitle());
      GridCtrl4->SetCellValue(count, 2, pE->GetAbstract());
      GridCtrl4->SetCellValue(count, 3, pE->GetMimeType());
      count++;
      pE = pE->GetNext();
    }
  GridCtrl4->SetRowLabelSize(wxGRID_AUTOSIZE);
  GridCtrl4->AutoSize();
  GridCtrl4->EnableEditing(false);
  gridSizer->Add(GridCtrl4, 0, wxALIGN_RIGHT | wxALL, 5);
  GridCtrl4->Enable(false);
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
    new wxCheckBox(panel, ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT,
                   wxT("Enable"),
                   wxDefaultPosition, wxDefaultSize);
  enableReplacementCtrl->SetValue(false);
  enableReplacementCtrl->Enable(false);
  replacementSizer->Add(enableReplacementCtrl, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
  wxBoxSizer *replacement1Sizer = new wxBoxSizer(wxHORIZONTAL);
  replacementSizer->Add(replacement1Sizer, 0, wxALIGN_RIGHT | wxALL, 0);
  wxTextCtrl *replacementCtrl =
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_REPLACEMENT,
                   Stroke2ColorReplacement,
                   wxDefaultPosition, wxSize(80, 22));
  replacement1Sizer->Add(replacementCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL,
                         5);
  replacementCtrl->Enable(false);
  wxStaticBitmap *sampleReplacementCtrl =
    new wxStaticBitmap(panel, ID_SYMBOLIZER_STROKE2_REPLACEMENT_HEX, bmp,
                       wxDefaultPosition, wxSize(32, 32));
  replacement1Sizer->Add(sampleReplacementCtrl, 0, wxALIGN_RIGHT | wxALL, 5);
// third row: Stroke-Width, Stroke-LineJoin, Stroke-LineCap and Stroke-Dasharray 
  wxBoxSizer *miscSizer = new wxBoxSizer(wxHORIZONTAL);
  boxSizer->Add(miscSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_WIDTH, wxT("1.0"),
                   wxDefaultPosition, wxSize(100, 22));
  widthCtrl->Enable(false);
  widthSizer->Add(widthCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
// third row B: LineJoin
  wxString join[3];
  join[0] = wxT("&Mitre");
  join[1] = wxT("&Round");
  join[2] = wxT("&Bevel");
  wxRadioBox *joinBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE2_LINEJOIN,
                                       wxT("&Line Join"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 3,
                                       join, 1,
                                       wxRA_SPECIFY_COLS);
  miscSizer->Add(joinBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  joinBox->SetSelection(1);
  joinBox->Enable(false);
// third row C: LineCap
  wxString cap[3];
  cap[0] = wxT("&Butt");
  cap[1] = wxT("&Round");
  cap[2] = wxT("&Square");
  wxRadioBox *capBox = new wxRadioBox(panel, ID_SYMBOLIZER_STROKE2_LINECAP,
                                      wxT("&Line Cap"),
                                      wxDefaultPosition,
                                      wxDefaultSize, 3,
                                      cap, 1,
                                      wxRA_SPECIFY_COLS);
  miscSizer->Add(capBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  capBox->SetSelection(1);
  capBox->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_DASHARRAY, wxT(""),
                   wxDefaultPosition, wxSize(200, 22));
  dashArrayCtrl->Enable(false);
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
    new wxTextCtrl(panel, ID_SYMBOLIZER_STROKE2_DASHOFFSET, wxT("0.0"),
                   wxDefaultPosition, wxSize(100, 22));
  dashOffsetCtrl->Enable(false);
  dash2Sizer->Add(dashOffsetCtrl, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_STROKE2_ENABLE, wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke2Changed);
  Connect(ID_SYMBOLIZER_STROKE2_TYPE, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke2TypeChanged);
  Connect(ID_SYMBOLIZER_STROKE2_COLOR, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor4Changed);
  Connect(ID_SYMBOLIZER_STROKE2_PICKER_BTN, wxEVT_COMMAND_BUTTON_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdColor4Picker);
  Connect(ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT,
          wxEVT_COMMAND_CHECKBOX_CLICKED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke2EnableReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE2_REPLACEMENT, wxEVT_COMMAND_TEXT_UPDATED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdStroke2ReplacementChanged);
  Connect(ID_SYMBOLIZER_STROKE2_LINEJOIN, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdLineJoin2Changed);
  Connect(ID_SYMBOLIZER_STROKE2_LINECAP, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdLineCap2Changed);
  return panel;
}

void SimplePolygonSymbolizerDialog::
OnCmdStroke2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// Stroke #2 enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  if (enableCtrl->IsChecked() == true)
    EnableStroke2 = true;
  else
    EnableStroke2 = false;
  RetrieveStroke2Page(false);
  UpdateStroke2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdStroke2TypeChanged(wxCommandEvent & WXUNUSED(event))
{
//
// stroke type changed: updating the visual sample
//
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_TYPE);
  if (typeBox->GetSelection() == 0)
    HasGraphicStroke2 = false;
  else
    HasGraphicStroke2 = true;
  RetrieveStroke2Page(false);
  UpdateStroke2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdColor4Changed(wxCommandEvent & WXUNUSED(event))
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

void SimplePolygonSymbolizerDialog::
OnCmdColor4Picker(wxCommandEvent & WXUNUSED(event))
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

void SimplePolygonSymbolizerDialog::
OnCmdStroke2EnableReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Polygon #2 Stroke ColorReplacement enable/disable 
//
  wxCheckBox *enableCtrl =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT);
  if (enableCtrl->IsChecked() == true)
    EnableStroke2Replacement = true;
  else
    EnableStroke2Replacement = false;
  RetrieveStroke2Page(false);
  UpdateStroke2Page();
}

void SimplePolygonSymbolizerDialog::
OnCmdStroke2ReplacementChanged(wxCommandEvent & WXUNUSED(event))
{
//
// Color Replacement changed: updating the visual sample
//
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT);
  wxStaticBitmap *sampleCtrl =
    (wxStaticBitmap *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT_HEX);
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

void SimplePolygonSymbolizerDialog::
OnCmdLineJoin2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineJoin selection changed
//
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINEJOIN);
  switch (lineJoinCtrl->GetSelection())
    {
      case 0:
        Stroke2LineJoin = RL2_PEN_JOIN_MITER;
        break;
      case 2:
        Stroke2LineJoin = RL2_PEN_JOIN_BEVEL;
        break;
      default:
        Stroke2LineJoin = RL2_PEN_JOIN_ROUND;
        break;
    };
}

void SimplePolygonSymbolizerDialog::
OnCmdLineCap2Changed(wxCommandEvent & WXUNUSED(event))
{
//
// LineCap selection changed
//
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINECAP);
  switch (lineCapCtrl->GetSelection())
    {
      case 0:
        Stroke2LineCap = RL2_PEN_CAP_BUTT;
        break;
      case 2:
        Stroke2LineCap = RL2_PEN_CAP_SQUARE;
        break;
      default:
        Stroke2LineCap = RL2_PEN_CAP_ROUND;
        break;
    };
}

wxPanel *SimplePolygonSymbolizerDialog::CreatePreviewPage(wxWindow * parent)
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
// creating a control to show the LineSymbolizer Preview
  wxStaticBox *previewBox = new wxStaticBox(panel, wxID_STATIC,
                                            wxT("PolygonSymbolizer Preview"),
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
  boxSizer->Add(backBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
  backBox->SetSelection(0);
  panel->SetSizer(topSizer);
  topSizer->Fit(panel);
// appends event handlers
  Connect(ID_SYMBOLIZER_BACKGROUND, wxEVT_COMMAND_RADIOBOX_SELECTED,
          (wxObjectEventFunction) &
          SimplePolygonSymbolizerDialog::OnCmdBackgroundChanged);
  return panel;
}

void SimplePolygonSymbolizerDialog::
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

void SimplePolygonSymbolizerDialog::DrawPreview(int horz, int vert)
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

void SimplePolygonSymbolizerDialog::CreateButtons()
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

bool SimplePolygonSymbolizerDialog::FinalValidityCheck()
{
//
// last check before generating the SLD/SE Style
//
  if (Name.Len() < 1)
    {
      wxMessageBox(wxT("You must specify the PolygonSymbolizer NAME !!!"),
                   wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
      return false;
    }
  if (Title.Len() < 1)
    {
      wxString msg =
        wxT("Setting some PolygonSymbolizer TITLE is warmly suggested\n\n");
      msg += wxT("Do you really confirm leaving an empty (undefined) Title ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  if (Abstract.Len() < 1)
    {
      wxString msg =
        wxT("Setting some PolygonSymbolizer ABSTRACT is warmly suggested\n\n");
      msg +=
        wxT("Do you really confirm leaving an empty (undefined) Abstract ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  if (EnableFill1 == false && EnableStroke1 == false)
    {
      wxString msg =
        wxT
        ("Effectless Polygon #1 Style: both Fill and Stroke are disabled\n\n");
      msg +=
        wxT("Do you really confirm leaving an effectless Polygon #1 Style ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  if (EnablePolygon2 == true && EnableFill2 == false && EnableStroke2 == false)
    {
      wxString msg =
        wxT
        ("Effectless Polygon #2 Style: both Fill and Stroke are disabled\n\n");
      msg +=
        wxT("Do you really confirm leaving an effectless Polygon #2 Style ?");
      if (wxMessageBox
          (msg, wxT("spatialite_gui"), wxYES_NO | wxICON_WARNING,
           this) != wxYES)
        return false;
    }
  return true;
}

bool SimplePolygonSymbolizerDialog::RetrieveMainPage()
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

void SimplePolygonSymbolizerDialog::UpdateMainPage()
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
}

bool SimplePolygonSymbolizerDialog::RetrieveFill1Page(bool check)
{
//
// retrieving params from the Polygon #1 Fill page
//
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL1_OPACITY);
  Fill1Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X);
  wxString value = displXCtrl->GetValue();
  if (value.ToDouble(&DisplacementX1) != true)
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
  if (value.ToDouble(&DisplacementY1) != true)
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
  if (value.ToDouble(&PerpendicularOffset1) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (HasGraphicFill1 == false)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_COLOR);
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
      Fill1Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl1->GetNumberRows(); i++)
        {
          if (GridCtrl1->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Fill1XLinkHref, Fill1MimeType);
      if (EnableFill1Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Fill1ColorReplacement = color;
        }
    }
  return true;
}

void SimplePolygonSymbolizerDialog::UpdateFill1Page()
{
//
// updating the Polygon #1 Fill page
//
  wxCheckBox *enableBox = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL1_ENABLE);
  if (EnableFill1 == true)
    enableBox->SetValue(true);
  else
    enableBox->SetValue(false);
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL1_OPACITY);
  opacityCtrl->SetValue(Fill1Opacity * 100.0);
  if (EnableFill1 == false)
    opacityCtrl->Enable(false);
  else
    opacityCtrl->Enable(true);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_X);
  char dummy[64];
  sprintf(dummy, "%1.2f", DisplacementX1);
  wxString str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", DisplacementY1);
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON1_PERPENDICULAR);
  sprintf(dummy, "%1.2f", PerpendicularOffset1);
  str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL1_TYPE);
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_FILL1_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL1_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL1_REPLACEMENT);
  if (EnableFill1 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl1->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else if (HasGraphicFill1 == false)
    {
      typeBox->SetSelection(0);
      typeBox->Enable(true);
      colorCtrl->Enable(true);
      pick->Enable(true);
      GridCtrl1->Enable(false);
      GridCtrl1->ClearSelection();
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
      wxColour color = wxNullColour;
      ColorMapEntry::GetWxColor(Fill1Color, color);
      if (color.IsOk() == true)
        {
          char hex[16];
          sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                  color.Blue());
          wxString str = wxString::FromUTF8(hex);
          colorCtrl->SetValue(str);
        }
  } else
    {
      typeBox->SetSelection(1);
      typeBox->Enable(true);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl1->Enable(true);
      int sel = List->FindByXLinkHref(Fill1XLinkHref);
      if (sel >= 0)
        GridCtrl1->SelectRow(sel);
      enableReplacement->Enable(true);
      if (EnableFill1Replacement == true)
        {
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Fill1ColorReplacement, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              replacementCtrl->SetValue(str);
              replacementCtrl->Enable(true);
          } else
            replacementCtrl->Enable(false);
        }
    }
}

bool SimplePolygonSymbolizerDialog::DoParseDashArray(wxString & str, int which)
{
//
// attempting to parse a Stroke DashArray string
//
  if (which == 0)
    {
      Stroke1DashCount = 0;
      if (Stroke1DashArray != NULL)
        delete[]Stroke1DashArray;
      Stroke1DashArray = NULL;
    }
  if (which == 1)
    {
      Stroke2DashCount = 0;
      if (Stroke2DashArray != NULL)
        delete[]Stroke2DashArray;
      Stroke2DashArray = NULL;
    }
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
  if (which == 0)
    {
      Stroke1DashCount = count;
      Stroke1DashArray = new double[Stroke1DashCount];
      array = Stroke1DashArray;
    }
  if (which == 1)
    {
      Stroke2DashCount = count;
      Stroke2DashArray = new double[Stroke2DashCount];
      array = Stroke2DashArray;
    }
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

void SimplePolygonSymbolizerDialog::NormalizedDashArray(wxString & str,
                                                        int which,
                                                        char delimiter)
{
//
// creating a normalized DashArray string
//
  int count;
  double *array;
  if (which == 0)
    {
      count = Stroke1DashCount;
      array = Stroke1DashArray;
    }
  if (which == 1)
    {
      count = Stroke2DashCount;
      array = Stroke2DashArray;
    }
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

bool SimplePolygonSymbolizerDialog::RetrieveStroke1Page(bool check)
{
//
// retrieving params from the STROKE #1 page
//
  if (EnableStroke1 == false)
    return true;
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  Stroke1Opacity = opacityCtrl->GetValue() / 100.0;
  if (HasGraphicStroke1 == false)
    {
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
      Stroke1Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl2->GetNumberRows(); i++)
        {
          if (GridCtrl2->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Stroke1XLinkHref, Stroke1MimeType);
      if (EnableStroke1Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Stroke1ColorReplacement = color;
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  wxString value = widthCtrl->GetValue();
  if (value.ToDouble(&Stroke1Width) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Stroke1Width <= 0.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHARRAY);
  value = dashArrayCtrl->GetValue();
  if (DoParseDashArray(value, 0) == false)
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
  if (Stroke1DashCount == 0)
    Stroke1DashOffset = 0.0;
  else
    {
      wxTextCtrl *offsetCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHOFFSET);
      wxString value = offsetCtrl->GetValue();
      if (value.ToDouble(&Stroke1DashOffset) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-DASH-OFFSET isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
    }
  return true;
}

void SimplePolygonSymbolizerDialog::UpdateStroke1Page()
{
//
// updating the STROKE #1 page
//
  wxCheckBox *enableBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE1_ENABLE);
  if (EnableStroke1 == true)
    enableBox->SetValue(true);
  else
    enableBox->SetValue(false);
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE1_OPACITY);
  opacityCtrl->SetValue(Stroke1Opacity * 100.0);
  if (EnableStroke1 == false)
    opacityCtrl->Enable(false);
  else
    opacityCtrl->Enable(true);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_TYPE);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE1_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE1_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_REPLACEMENT);
  if (EnableStroke1 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl2->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else
    {
      typeBox->Enable(true);
      if (HasGraphicStroke1 == false)
        {
          typeBox->SetSelection(0);
          typeBox->Enable(true);
          colorCtrl->Enable(true);
          pick->Enable(true);
          GridCtrl2->Enable(false);
          GridCtrl2->ClearSelection();
          enableReplacement->Enable(false);
          replacementCtrl->Enable(false);
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke1Color, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              colorCtrl->SetValue(str);
            }
      } else
        {
          typeBox->SetSelection(1);
          typeBox->Enable(true);
          colorCtrl->Enable(false);
          pick->Enable(false);
          GridCtrl2->Enable(true);
          int sel = List->FindByXLinkHref(Stroke1XLinkHref);
          if (sel >= 0)
            GridCtrl2->SelectRow(sel);
          enableReplacement->Enable(true);
          if (EnableStroke1Replacement == true)
            {
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke1ColorReplacement, color);
              if (color.IsOk() == true)
                {
                  char hex[16];
                  sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                          color.Blue());
                  wxString str = wxString::FromUTF8(hex);
                  replacementCtrl->SetValue(str);
                  replacementCtrl->Enable(true);
              } else
                replacementCtrl->Enable(false);
            }
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_WIDTH);
  char dummy[64];
  sprintf(dummy, "%1.2f", Stroke1Width);
  wxString str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  if (EnableStroke1 == false)
    widthCtrl->Enable(false);
  else
    widthCtrl->Enable(true);
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINEJOIN);
  switch (Stroke1LineJoin)
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
  if (EnableStroke1 == false)
    lineJoinCtrl->Enable(false);
  else
    lineJoinCtrl->Enable(true);
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE1_LINECAP);
  switch (Stroke1LineCap)
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
  if (EnableStroke1 == false)
    lineCapCtrl->Enable(false);
  else
    lineCapCtrl->Enable(true);
  wxTextCtrl *dashArrayCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHARRAY);
  wxString value;
  NormalizedDashArray(value, 0);
  dashArrayCtrl->SetValue(value);
  if (EnableStroke1 == false)
    dashArrayCtrl->Enable(false);
  else
    dashArrayCtrl->Enable(true);
  wxTextCtrl *offsetCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE1_DASHOFFSET);
  if (Stroke1DashCount == 0)
    offsetCtrl->SetValue(wxT("0.0"));
  else
    {
      char dummy[64];
      sprintf(dummy, "%1.2f", Stroke1DashOffset);
      wxString str = wxString::FromUTF8(dummy);
      offsetCtrl->SetValue(str);
    }
  if (EnableStroke1 == false)
    offsetCtrl->Enable(false);
  else
    offsetCtrl->Enable(true);
}

bool SimplePolygonSymbolizerDialog::RetrieveFill2Page(bool check)
{
//
// retrieving params from the Polygon #2 Fill page
//
  if (EnablePolygon2 == false)
    return true;
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL2_OPACITY);
  Fill2Opacity = opacityCtrl->GetValue() / 100.0;
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_X);
  wxString value = displXCtrl->GetValue();
  if (value.ToDouble(&DisplacementX2) != true)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_Y);
  value = displYCtrl->GetValue();
  if (value.ToDouble(&DisplacementY2) != true)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON2_PERPENDICULAR);
  value = perpCtrl->GetValue();
  if (value.ToDouble(&PerpendicularOffset2) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("PERPENDICULAR-OFFSET isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (HasGraphicFill2 == false)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
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
      Fill2Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl3->GetNumberRows(); i++)
        {
          if (GridCtrl3->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Fill2XLinkHref, Fill2MimeType);
      if (EnableFill2Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Fill2ColorReplacement = color;
        }
    }
  return true;
}

void SimplePolygonSymbolizerDialog::UpdateFill2Page()
{
//
// updating the Polygon #2 Fill page
//
  wxCheckBox *polygonBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_POLYGON2_ENABLE);
  if (EnablePolygon2 == true)
    polygonBox->SetValue(true);
  else
    polygonBox->SetValue(false);
  if (EnablePolygon2 == false)
    EnableFill2 = false;
  wxCheckBox *enableBox = (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE);
  if (EnableFill2 == true)
    enableBox->SetValue(true);
  else
    enableBox->SetValue(false);
  if (EnablePolygon2 == true)
    enableBox->Enable(true);
  else
    enableBox->Enable(false);
  wxSlider *opacityCtrl = (wxSlider *) FindWindow(ID_SYMBOLIZER_FILL2_OPACITY);
  opacityCtrl->SetValue(Fill2Opacity * 100.0);
  if (EnablePolygon2 == true)
    {
      if (EnableFill2 == false)
        opacityCtrl->Enable(false);
      else
        opacityCtrl->Enable(true);
  } else
    opacityCtrl->Enable(false);
  wxTextCtrl *displXCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_X);
  char dummy[64];
  sprintf(dummy, "%1.2f", DisplacementX2);
  wxString str = wxString::FromUTF8(dummy);
  displXCtrl->SetValue(str);
  if (EnablePolygon2 == true)
    displXCtrl->Enable(true);
  else
    displXCtrl->Enable(false);
  wxTextCtrl *displYCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON2_DISPLACEMENT_Y);
  sprintf(dummy, "%1.2f", DisplacementY2);
  str = wxString::FromUTF8(dummy);
  displYCtrl->SetValue(str);
  if (EnablePolygon2 == true)
    displYCtrl->Enable(true);
  else
    displYCtrl->Enable(false);
  wxTextCtrl *perpCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_POLYGON2_PERPENDICULAR);
  sprintf(dummy, "%1.2f", PerpendicularOffset2);
  str = wxString::FromUTF8(dummy);
  perpCtrl->SetValue(str);
  if (EnablePolygon2 == true)
    perpCtrl->Enable(true);
  else
    perpCtrl->Enable(false);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_FILL2_TYPE);
  wxTextCtrl *colorCtrl = (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_FILL2_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_FILL2_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_FILL2_REPLACEMENT);
  if (EnablePolygon2 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl3->Enable(false);
  } else if (EnableFill2 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl3->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else if (HasGraphicFill2 == false)
    {
      typeBox->SetSelection(0);
      typeBox->Enable(true);
      colorCtrl->Enable(true);
      pick->Enable(true);
      GridCtrl1->Enable(false);
      GridCtrl1->ClearSelection();
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
      wxColour color = wxNullColour;
      ColorMapEntry::GetWxColor(Fill2Color, color);
      if (color.IsOk() == true)
        {
          char hex[16];
          sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                  color.Blue());
          wxString str = wxString::FromUTF8(hex);
          colorCtrl->SetValue(str);
        }
  } else
    {
      typeBox->SetSelection(1);
      typeBox->Enable(true);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl3->Enable(true);
      int sel = List->FindByXLinkHref(Fill2XLinkHref);
      if (sel >= 0)
        GridCtrl3->SelectRow(sel);
      enableReplacement->Enable(true);
      if (EnableFill2Replacement == true)
        {
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Fill2ColorReplacement, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              replacementCtrl->SetValue(str);
              replacementCtrl->Enable(true);
          } else
            replacementCtrl->Enable(false);
        }
    }
}

bool SimplePolygonSymbolizerDialog::RetrieveStroke2Page(bool check)
{
//
// retrieving params from the STROKE #2 page
//
  if (EnableStroke2 == false || EnablePolygon2 == false)
    return true;
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  Stroke2Opacity = opacityCtrl->GetValue() / 100.0;
  if (HasGraphicStroke2 == false)
    {
      wxTextCtrl *colorCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
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
      Stroke2Color = color;
  } else
    {
      int selCount = 0;
      int selected = -1;
      for (int i = 0; i < GridCtrl4->GetNumberRows(); i++)
        {
          if (GridCtrl4->IsInSelection(i, 0) == true)
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
              wxMessageBox(msg, wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                           this);
              return false;
            }
        }
      List->FindByIndex(selected, Stroke2XLinkHref, Stroke2MimeType);
      if (EnableStroke2Replacement == true)
        {
          wxTextCtrl *replacementCtrl =
            (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT);
          wxString color = replacementCtrl->GetValue();
          if (ColorMapEntry::IsValidColor(color) != true)
            {
              if (check == true)
                {
                  wxMessageBox(wxT
                               ("COLOR-REPACEMENT isn't a valid HexRGB color !!!"),
                               wxT("spatialite_gui"), wxOK | wxICON_WARNING,
                               this);
                  return false;
                }
            }
          Stroke2ColorReplacement = color;
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  wxString value = widthCtrl->GetValue();
  if (value.ToDouble(&Stroke2Width) != true)
    {
      if (check == true)
        {
          wxMessageBox(wxT
                       ("STROKE-WIDTH isn't a valid decimal number !!!"),
                       wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
          return false;
        }
    }
  if (Stroke2Width <= 0.0)
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
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHARRAY);
  value = dashArrayCtrl->GetValue();
  if (DoParseDashArray(value, 1) == false)
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
      NormalizedDashArray(value, 1);
      dashArrayCtrl->SetValue(value);
    }
  if (Stroke2DashCount == 0)
    Stroke2DashOffset = 0.0;
  else
    {
      wxTextCtrl *offsetCtrl =
        (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHOFFSET);
      wxString value = offsetCtrl->GetValue();
      if (value.ToDouble(&Stroke2DashOffset) != true)
        {
          if (check == true)
            {
              wxMessageBox(wxT
                           ("STROKE-DASH-OFFSET isn't a valid decimal number !!!"),
                           wxT("spatialite_gui"), wxOK | wxICON_WARNING, this);
              return false;
            }
        }
    }
  return true;
}

void SimplePolygonSymbolizerDialog::UpdateStroke2Page()
{
//
// updating the STROKE #1 page
//
  if (EnablePolygon2 == false)
    EnableStroke2 = false;
  wxCheckBox *enableBox =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE);
  if (EnableStroke2 == true)
    enableBox->SetValue(true);
  else
    enableBox->SetValue(false);
  if (EnablePolygon2 == true)
    enableBox->Enable(true);
  else
    enableBox->Enable(false);
  wxSlider *opacityCtrl =
    (wxSlider *) FindWindow(ID_SYMBOLIZER_STROKE2_OPACITY);
  opacityCtrl->SetValue(Stroke2Opacity * 100.0);
  if (EnableStroke2 == false)
    opacityCtrl->Enable(false);
  else
    opacityCtrl->Enable(true);
  wxRadioBox *typeBox = (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_TYPE);
  wxTextCtrl *colorCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_COLOR);
  wxButton *pick = (wxButton *) FindWindow(ID_SYMBOLIZER_STROKE2_PICKER_BTN);
  wxCheckBox *enableReplacement =
    (wxCheckBox *) FindWindow(ID_SYMBOLIZER_STROKE2_ENABLE_REPLACEMENT);
  wxTextCtrl *replacementCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_REPLACEMENT);
  if (EnableStroke2 == false)
    {
      typeBox->Enable(false);
      colorCtrl->Enable(false);
      pick->Enable(false);
      GridCtrl4->Enable(false);
      enableReplacement->Enable(false);
      replacementCtrl->Enable(false);
  } else
    {
      typeBox->Enable(true);
      if (HasGraphicStroke2 == false)
        {
          typeBox->SetSelection(0);
          typeBox->Enable(true);
          colorCtrl->Enable(true);
          pick->Enable(true);
          GridCtrl4->Enable(false);
          GridCtrl4->ClearSelection();
          enableReplacement->Enable(false);
          replacementCtrl->Enable(false);
          wxColour color = wxNullColour;
          ColorMapEntry::GetWxColor(Stroke2Color, color);
          if (color.IsOk() == true)
            {
              char hex[16];
              sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                      color.Blue());
              wxString str = wxString::FromUTF8(hex);
              colorCtrl->SetValue(str);
            }
      } else
        {
          typeBox->SetSelection(1);
          typeBox->Enable(true);
          colorCtrl->Enable(false);
          pick->Enable(false);
          GridCtrl4->Enable(true);
          int sel = List->FindByXLinkHref(Stroke2XLinkHref);
          if (sel >= 0)
            GridCtrl4->SelectRow(sel);
          enableReplacement->Enable(true);
          if (EnableStroke2Replacement == true)
            {
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke2ColorReplacement, color);
              if (color.IsOk() == true)
                {
                  char hex[16];
                  sprintf(hex, "#%02x%02x%02x", color.Red(), color.Green(),
                          color.Blue());
                  wxString str = wxString::FromUTF8(hex);
                  replacementCtrl->SetValue(str);
                  replacementCtrl->Enable(true);
              } else
                replacementCtrl->Enable(false);
            }
        }
    }
  wxTextCtrl *widthCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_WIDTH);
  char dummy[64];
  sprintf(dummy, "%1.2f", Stroke2Width);
  wxString str = wxString::FromUTF8(dummy);
  widthCtrl->SetValue(str);
  if (EnableStroke2 == false)
    widthCtrl->Enable(false);
  else
    widthCtrl->Enable(true);
  wxRadioBox *lineJoinCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINEJOIN);
  switch (Stroke2LineJoin)
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
  if (EnableStroke2 == false)
    lineJoinCtrl->Enable(false);
  else
    lineJoinCtrl->Enable(true);
  wxRadioBox *lineCapCtrl =
    (wxRadioBox *) FindWindow(ID_SYMBOLIZER_STROKE2_LINECAP);
  switch (Stroke2LineCap)
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
  if (EnableStroke2 == false)
    lineCapCtrl->Enable(false);
  else
    lineCapCtrl->Enable(true);
  wxTextCtrl *dashArrayCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHARRAY);
  wxString value;
  NormalizedDashArray(value, 1);
  dashArrayCtrl->SetValue(value);
  if (EnableStroke2 == false)
    dashArrayCtrl->Enable(false);
  else
    dashArrayCtrl->Enable(true);
  wxTextCtrl *offsetCtrl =
    (wxTextCtrl *) FindWindow(ID_SYMBOLIZER_STROKE2_DASHOFFSET);
  if (Stroke2DashCount == 0)
    offsetCtrl->SetValue(wxT("0.0"));
  else
    {
      char dummy[64];
      sprintf(dummy, "%1.2f", Stroke2DashOffset);
      wxString str = wxString::FromUTF8(dummy);
      offsetCtrl->SetValue(str);
    }
  if (EnableStroke2 == false)
    offsetCtrl->Enable(false);
  else
    offsetCtrl->Enable(true);
}

bool SimplePolygonSymbolizerDialog::RetrievePreviewPage()
{
//
// retrieving params from the PREVIEW page
//
  return true;
}

void SimplePolygonSymbolizerDialog::PreparePolygonPath(void *xctx,
                                                       double
                                                       perpendicular_offset,
                                                       double displacement_x,
                                                       double displacement_y)
{
// Visual Preview

// exterior ring
  gaiaDynamicLinePtr dyn = gaiaAllocDynamicLine();
  gaiaAppendPointToDynamicLine(dyn, 190.0, 40.0);
  double pi = 3.14159265359;
  for (double rads = (2.0 * pi) - (pi / 2.0); rads >= 0.1; rads -= 0.666666)
    {
      double x = 135.0 + (115.0 * cos(rads));
      double y = 150.0 + (115.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  gaiaAppendPointToDynamicLine(dyn, 230.0, 200.0);
  gaiaAppendPointToDynamicLine(dyn, 250.0, 200.0);
  gaiaAppendPointToDynamicLine(dyn, 270.0, 220.0);
  gaiaAppendPointToDynamicLine(dyn, 290.0, 200.0);
  gaiaAppendPointToDynamicLine(dyn, 310.0, 220.0);
  gaiaAppendPointToDynamicLine(dyn, 330.0, 200.0);
  gaiaAppendPointToDynamicLine(dyn, 350.0, 220.0);
  gaiaAppendPointToDynamicLine(dyn, 350.0, 260.0);
  for (double rads = (pi * 2.0) + (pi / 2.0); rads >= pi + (pi / 4.0);
       rads -= 0.1)
    {
      double x = 365.0 + (115.0 * cos(rads));
      double y = 150.0 + (115.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  gaiaAppendPointToDynamicLine(dyn, 250.0, 40.0);
  gaiaAppendPointToDynamicLine(dyn, 250.0, 80.0);
  gaiaAppendPointToDynamicLine(dyn, 200.0, 80.0);

  int points = 0;
  gaiaPointPtr pt = dyn->First;
  while (pt != NULL)
    {
      // counting how many Points are there
      points++;
      pt = pt->Next;
    }
  gaiaGeomCollPtr geom = gaiaAllocGeomColl();
  gaiaPolygonPtr pg = gaiaAddPolygonToGeomColl(geom, points + 1, 1);
  gaiaRingPtr rng = pg->Exterior;
  int iv = 0;
  pt = dyn->First;
  while (pt != NULL)
    {
      // preparing the Exterior Ring
      gaiaSetPoint(rng->Coords, iv, pt->X, pt->Y);
      iv++;
      pt = pt->Next;
    }
  gaiaSetPoint(rng->Coords, points, dyn->First->X, dyn->Last->Y);
  gaiaFreeDynamicLine(dyn);

// interior ring
  dyn = gaiaAllocDynamicLine();
  gaiaAppendPointToDynamicLine(dyn, 350.0, 160.0);
  gaiaAppendPointToDynamicLine(dyn, 380.0, 160.0);
  gaiaAppendPointToDynamicLine(dyn, 380.0, 130.0);
  for (double rads = (pi * 2.0) + (pi / 4.0); rads >= pi + (pi / 4.0);
       rads -= 0.1)
    {
      double x = 370.0 + (55.0 * cos(rads));
      double y = 140.0 + (55.0 * sin(rads));
      gaiaAppendPointToDynamicLine(dyn, x, y);
    }
  points = 0;
  pt = dyn->First;
  while (pt != NULL)
    {
      // counting how many Points are there
      points++;
      pt = pt->Next;
    }
  rng = gaiaAddInteriorRing(pg, 0, points + 1);
  iv = 0;
  pt = dyn->First;
  while (pt != NULL)
    {
      // preparing the Interior Ring
      gaiaSetPoint(rng->Coords, iv, pt->X, pt->Y);
      iv++;
      pt = pt->Next;
    }
  gaiaSetPoint(rng->Coords, points, dyn->First->X, dyn->Last->Y);
  gaiaFreeDynamicLine(dyn);

  gaiaGeomCollPtr geom2;
  if (perpendicular_offset != 0.0)
    {
      // buffering
      geom2 =
        gaiaGeomCollBuffer_r(MainFrame->GetSpliteInternalCache(), geom,
                             perpendicular_offset, 16);
      gaiaFreeGeomColl(geom);
  } else
    {
      // unchanged
      geom2 = geom;
    }
  if (displacement_x != 0.0 || displacement_y != 0.0)
    {
      // displacing
      gaiaShiftCoords(geom2, displacement_x, displacement_y);
    }
// preparing the Stroke Path
  pg = geom2->FirstPolygon;
  rng = pg->Exterior;
  rl2GraphicsContextPtr ctx = (rl2GraphicsContextPtr) xctx;
  for (iv = 0; iv < rng->Points; iv++)
    {
      double x;
      double y;
      gaiaGetPoint(rng->Coords, iv, &x, &y);
      if (iv == 0)
        rl2_graph_move_to_point(ctx, x, y);
      else
        rl2_graph_add_line_to_path(ctx, x, y);
    }
  rl2_graph_close_subpath(ctx);
  rng = pg->Interiors;
  if (rng != NULL)
    {
      for (iv = 0; iv < rng->Points; iv++)
        {
          double x;
          double y;
          gaiaGetPoint(rng->Coords, iv, &x, &y);
          if (iv == 0)
            rl2_graph_move_to_point(ctx, x, y);
          else
            rl2_graph_add_line_to_path(ctx, x, y);
        }
      rl2_graph_close_subpath(ctx);
    }
  gaiaFreeGeomColl(geom2);
}

void SimplePolygonSymbolizerDialog::UpdatePreviewPage()
{
//
// updating the PREVIEW page
//
  rl2GraphicsPatternPtr pattern1 = NULL;
  rl2GraphicsPatternPtr pattern2 = NULL;
  rl2GraphicsPatternPtr pattern3 = NULL;
  rl2GraphicsPatternPtr pattern4 = NULL;
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
  rl2GraphicsContextPtr ctx = NULL;
  ctx = rl2_graph_create_context(MainFrame->GetRL2PrivateData(), 500, 300);
  if (ctx == NULL)
    return;
// transparent background initialization
  rl2_graph_set_brush(ctx, 251, 5, 249, 255);
  rl2_graph_draw_rectangle(ctx, -1, -1, 501, 301);

  if (EnableFill1 == true || EnableStroke1 == true)
    {
      if (EnableFill1 == true)
        {
          if (HasGraphicFill1 == false)
            {
              // preparing a Color-based Brush
              double aleph = 255.0 * Fill1Opacity;
              if (aleph < 0.0)
                aleph = 0.0;
              if (aleph > 255.0)
                aleph = 255.0;
              unsigned char alpha = aleph;
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Fill1Color, color);
              rl2_graph_set_brush(ctx, color.Red(), color.Green(), color.Blue(),
                                  alpha);
          } else
            {
              // preparing a Pattern-based Brush
              pattern1 =
                rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                         Fill1XLinkHref.ToUTF8
                                                         (), 1);
              if (pattern1 != NULL)
                {
                  if (EnableFill1Replacement)
                    {
                      // attempting to recolor the External Graphic resource
                      wxColour color = wxNullColour;
                      ColorMapEntry::GetWxColor(Fill1ColorReplacement, color);
                      rl2_graph_pattern_recolor(pattern1, color.Red(),
                                                color.Green(), color.Blue());
                    }
                  if (Fill1Opacity < 1.0)
                    {
                      // setting up the required transparency
                      double aleph = 255.0 * Fill1Opacity;
                      if (aleph < 0.0)
                        aleph = 0.0;
                      if (aleph > 255.0)
                        aleph = 255.0;
                      unsigned char alpha = aleph;
                      rl2_graph_pattern_transparency(pattern1, alpha);
                    }
                  rl2_graph_set_pattern_brush(ctx, pattern1);
              } else
                {
                  // invalid Pattern: defaulting to a Gray brush
                  rl2_graph_set_brush(ctx, 128, 128, 128, 255);
                }
            }
        }
      if (EnableStroke1 == true)
        {
          if (HasGraphicStroke1 == false)
            {
              // preparing a Color-based Pen
              double aleph = 255.0 * Stroke1Opacity;
              if (aleph < 0.0)
                aleph = 0.0;
              if (aleph > 255.0)
                aleph = 255.0;
              unsigned char alpha = aleph;
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke1Color, color);
              if (Stroke1DashCount == 0)
                rl2_graph_set_solid_pen(ctx, color.Red(), color.Green(),
                                        color.Blue(), alpha, Stroke1Width,
                                        Stroke1LineCap, Stroke1LineJoin);
              else
                rl2_graph_set_dashed_pen(ctx, color.Red(), color.Green(),
                                         color.Blue(), alpha, Stroke1Width,
                                         Stroke1LineCap, Stroke1LineJoin,
                                         Stroke1DashCount, Stroke1DashArray,
                                         Stroke1DashOffset);
          } else
            {
              // preparing a Pattern-based Pen
              pattern2 =
                rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                         Stroke1XLinkHref.ToUTF8
                                                         (), 1);
              if (pattern2 != NULL)
                {
                  if (EnableStroke1Replacement)
                    {
                      // attempting to recolor the External Graphic resource
                      wxColour color = wxNullColour;
                      ColorMapEntry::GetWxColor(Stroke1ColorReplacement, color);
                      rl2_graph_pattern_recolor(pattern2, color.Red(),
                                                color.Green(), color.Blue());
                    }
                  if (Stroke1Opacity < 1.0)
                    {
                      // setting up the required transparency
                      double aleph = 255.0 * Stroke1Opacity;
                      if (aleph < 0.0)
                        aleph = 0.0;
                      if (aleph > 255.0)
                        aleph = 255.0;
                      unsigned char alpha = aleph;
                      rl2_graph_pattern_transparency(pattern2, alpha);
                    }
                  if (Stroke1DashCount == 0)
                    rl2_graph_set_pattern_solid_pen(ctx, pattern2, Stroke1Width,
                                                    Stroke1LineCap,
                                                    Stroke1LineJoin);
                  else
                    rl2_graph_set_pattern_dashed_pen(ctx, pattern2,
                                                     Stroke1Width,
                                                     Stroke1LineCap,
                                                     Stroke1LineJoin,
                                                     Stroke1DashCount,
                                                     Stroke1DashArray,
                                                     Stroke1DashOffset);
              } else
                {
                  // invalid Pattern: defaulting to a black Pen
                  if (Stroke1DashCount == 0)
                    rl2_graph_set_solid_pen(ctx, 0, 0, 0, 255, Stroke1Width,
                                            Stroke1LineCap, Stroke1LineJoin);
                  else
                    rl2_graph_set_dashed_pen(ctx, 0, 0, 0, 255, Stroke1Width,
                                             Stroke1LineCap, Stroke1LineJoin,
                                             Stroke1DashCount, Stroke1DashArray,
                                             Stroke1DashOffset);
                }
            }
        }
      PreparePolygonPath(ctx, PerpendicularOffset1, DisplacementX1,
                         DisplacementY1);
      if (EnableFill1 == true && EnableStroke1)
        {
          // applying both Fill and Stroke
          rl2_graph_fill_path(ctx, RL2_PRESERVE_PATH);
          rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
      } else if (EnableFill1 == true)
        {
          // applying Fill only
          rl2_graph_fill_path(ctx, RL2_CLEAR_PATH);
      } else if (EnableStroke1 == true)
        {
          // applying Stroke only
          rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
        }
    }

  if (EnablePolygon2 == true && (EnableFill2 == true || EnableStroke2 == true))
    {
      if (EnableFill2 == true)
        {
          if (HasGraphicFill2 == false)
            {
              // preparing a Color-based Brush
              double aleph = 255.0 * Fill2Opacity;
              if (aleph < 0.0)
                aleph = 0.0;
              if (aleph > 255.0)
                aleph = 255.0;
              unsigned char alpha = aleph;
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Fill2Color, color);
              rl2_graph_set_brush(ctx, color.Red(), color.Green(), color.Blue(),
                                  alpha);
          } else
            {
              // preparing a Pattern-based Brush
              pattern3 =
                rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                         Fill2XLinkHref.ToUTF8
                                                         (), 1);
              if (pattern3 != NULL)
                {
                  if (EnableFill2Replacement)
                    {
                      // attempting to recolor the External Graphic resource
                      wxColour color = wxNullColour;
                      ColorMapEntry::GetWxColor(Fill2ColorReplacement, color);
                      rl2_graph_pattern_recolor(pattern3, color.Red(),
                                                color.Green(), color.Blue());
                    }
                  if (Fill2Opacity < 1.0)
                    {
                      // setting up the required transparency
                      double aleph = 255.0 * Fill2Opacity;
                      if (aleph < 0.0)
                        aleph = 0.0;
                      if (aleph > 255.0)
                        aleph = 255.0;
                      unsigned char alpha = aleph;
                      rl2_graph_pattern_transparency(pattern3, alpha);
                    }
                  rl2_graph_set_pattern_brush(ctx, pattern3);
              } else
                {
                  // invalid Pattern: defaulting to a Gray brush
                  rl2_graph_set_brush(ctx, 128, 128, 128, 255);
                }
            }
        }
      if (EnableStroke2 == true)
        {
          if (HasGraphicStroke2 == false)
            {
              // preparing a Color-based Pen
              double aleph = 255.0 * Stroke2Opacity;
              if (aleph < 0.0)
                aleph = 0.0;
              if (aleph > 255.0)
                aleph = 255.0;
              unsigned char alpha = aleph;
              wxColour color = wxNullColour;
              ColorMapEntry::GetWxColor(Stroke2Color, color);
              if (Stroke2DashCount == 0)
                rl2_graph_set_solid_pen(ctx, color.Red(), color.Green(),
                                        color.Blue(), alpha, Stroke2Width,
                                        Stroke2LineCap, Stroke2LineJoin);
              else
                rl2_graph_set_dashed_pen(ctx, color.Red(), color.Green(),
                                         color.Blue(), alpha, Stroke2Width,
                                         Stroke2LineCap, Stroke2LineJoin,
                                         Stroke2DashCount, Stroke2DashArray,
                                         Stroke2DashOffset);
          } else
            {
              // preparing a Pattern-based Pen
              pattern4 =
                rl2_create_pattern_from_external_graphic(MainFrame->GetSqlite(),
                                                         Stroke2XLinkHref.ToUTF8
                                                         (), 1);
              if (pattern4 != NULL)
                {
                  if (EnableStroke2Replacement)
                    {
                      // attempting to recolor the External Graphic resource
                      wxColour color = wxNullColour;
                      ColorMapEntry::GetWxColor(Stroke2ColorReplacement, color);
                      rl2_graph_pattern_recolor(pattern4, color.Red(),
                                                color.Green(), color.Blue());
                    }
                  if (Stroke2Opacity < 1.0)
                    {
                      // setting up the required transparency
                      double aleph = 255.0 * Stroke2Opacity;
                      if (aleph < 0.0)
                        aleph = 0.0;
                      if (aleph > 255.0)
                        aleph = 255.0;
                      unsigned char alpha = aleph;
                      rl2_graph_pattern_transparency(pattern4, alpha);
                    }
                  if (Stroke2DashCount == 0)
                    rl2_graph_set_pattern_solid_pen(ctx, pattern4, Stroke2Width,
                                                    Stroke2LineCap,
                                                    Stroke2LineJoin);
                  else
                    rl2_graph_set_pattern_dashed_pen(ctx, pattern4,
                                                     Stroke2Width,
                                                     Stroke2LineCap,
                                                     Stroke2LineJoin,
                                                     Stroke2DashCount,
                                                     Stroke2DashArray,
                                                     Stroke2DashOffset);
              } else
                {
                  // invalid Pattern: defaulting to a black Pen
                  if (Stroke2DashCount == 0)
                    rl2_graph_set_solid_pen(ctx, 0, 0, 0, 255, Stroke2Width,
                                            Stroke2LineCap, Stroke2LineJoin);
                  else
                    rl2_graph_set_dashed_pen(ctx, 0, 0, 0, 255, Stroke2Width,
                                             Stroke2LineCap, Stroke2LineJoin,
                                             Stroke2DashCount, Stroke2DashArray,
                                             Stroke2DashOffset);
                }
            }
        }
      PreparePolygonPath(ctx, PerpendicularOffset2, DisplacementX2,
                         DisplacementY2);
      if (EnableFill2 == true && EnableStroke2)
        {
          // applying both Fill and Stroke
          rl2_graph_fill_path(ctx, RL2_PRESERVE_PATH);
          rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
      } else if (EnableFill2 == true)
        {
          // applying Fill only
          rl2_graph_fill_path(ctx, RL2_CLEAR_PATH);
      } else if (EnableStroke2 == true)
        {
          // applying Stroke only
          rl2_graph_stroke_path(ctx, RL2_CLEAR_PATH);
        }
    }
// creating the RGB array
  unsigned char *rgb_array = rl2_graph_get_context_rgb_array(ctx);
  rl2_graph_destroy_context(ctx);
  if (pattern1 != NULL)
    rl2_graph_destroy_pattern(pattern1);
  if (pattern2 != NULL)
    rl2_graph_destroy_pattern(pattern2);
  if (pattern3 != NULL)
    rl2_graph_destroy_pattern(pattern3);
  if (pattern4 != NULL)
    rl2_graph_destroy_pattern(pattern4);
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
  dc.SelectObject(wxNullBitmap);
// updating the GUI Preview
  SymbolizerPreview *previewCtrl =
    (SymbolizerPreview *) FindWindow(ID_SYMBOLIZER_PREVIEW);
  previewCtrl->SetBitmap(bmp2);
}

void SimplePolygonSymbolizerDialog::OnPageChanging(wxNotebookEvent & event)
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
        ret = RetrieveFill1Page();
        break;
      case 2:
        ret = RetrieveStroke1Page();
        break;
      case 3:
        ret = RetrieveFill2Page();
        break;
      case 4:
        ret = RetrieveStroke2Page();
        break;
      case 5:
        ret = RetrievePreviewPage();
        break;
    };
  if (ret != true)
    event.Veto();
}

void SimplePolygonSymbolizerDialog::OnPageChanged(wxNotebookEvent & event)
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
        UpdateFill1Page();
        break;
      case 2:
        UpdateStroke1Page();
        break;
      case 3:
        UpdateFill2Page();
        break;
      case 4:
        UpdateStroke2Page();
        break;
      case 5:
        UpdatePreviewPage();
        break;
    };
}

char *SimplePolygonSymbolizerDialog::DoCreateFeatureTypeXML()
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
  xml = sqlite3_mprintf("%s\t\t<PolygonSymbolizer uom=\"%s\">\r\n", prev, cstr);
  sqlite3_free(prev);
  prev = xml;
  if (EnableFill1 == true)
    {
      // Polygon Fill
      xml = sqlite3_mprintf("%s\t\t\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (HasGraphicFill1 == true)
        {
          // using an External Graphic
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<GraphicFill>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Fill1XLinkHref.ToUTF8()) + 1];
          strcpy(str, Fill1XLinkHref.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Fill1MimeType.ToUTF8()) + 1];
          strcpy(str, Fill1MimeType.ToUTF8());
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev,
                            str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (EnableFill1Replacement == true)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Fill1ColorReplacement.ToUTF8()) + 1];
              strcpy(str, Fill1ColorReplacement.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
            }
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicFill>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a Solid Color
          str = new char[strlen(Fill1Color.ToUTF8()) + 1];
          strcpy(str, Fill1Color.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, Fill1Opacity);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EnableStroke1 == true)
    {
      // Polygon Stroke
      xml = sqlite3_mprintf("%s\t\t\t<Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (HasGraphicStroke1 == true)
        {
          // using an External Graphic
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<GraphicStroke>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke1XLinkHref.ToUTF8()) + 1];
          strcpy(str, Stroke1XLinkHref.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke1MimeType.ToUTF8()) + 1];
          strcpy(str, Stroke1MimeType.ToUTF8());
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev,
                            str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (EnableStroke1Replacement == true)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Stroke1ColorReplacement.ToUTF8()) + 1];
              strcpy(str, Stroke1ColorReplacement.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
            }
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicStroke>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a Solid Color
          str = new char[strlen(Stroke1Color.ToUTF8()) + 1];
          strcpy(str, Stroke1Color.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, Stroke1Opacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, Stroke1Width);
      sqlite3_free(prev);
      prev = xml;
      switch (Stroke1LineJoin)
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
      switch (Stroke1LineCap)
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
      if (Stroke1DashCount > 0 && Stroke1DashArray != NULL)
        {
          wxString dash;
          NormalizedDashArray(dash, 0, ' ');
          str = new char[strlen(dash.ToUTF8()) + 1];
          strcpy(str, dash.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (Stroke1DashOffset != 0.0)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                 prev, Stroke1DashOffset);
              sqlite3_free(prev);
              prev = xml;
            }
        }
      xml = sqlite3_mprintf("%s\t\t\t</Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (DisplacementX1 != 0.0 || DisplacementY1 != 0.0)
    {
      xml = sqlite3_mprintf("%s\t\t\t<Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n",
                        prev, DisplacementX1);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n",
                        prev, DisplacementY1);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t\t\t</Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (PerpendicularOffset1 != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n", prev,
         PerpendicularOffset1);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t\t</PolygonSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  prev = xml;
  if (EnablePolygon2 == true)
    {
      // Polygon #2
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
        sqlite3_mprintf("%s\t\t<PolygonSymbolizer uom=\"%s\">\r\n", prev, cstr);
      sqlite3_free(prev);
      prev = xml;
      if (EnableFill2 == true)
        {
          // Polygon Fill
          xml = sqlite3_mprintf("%s\t\t\t<Fill>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          if (HasGraphicFill2 == true)
            {
              // using an External Graphic
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<GraphicFill>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Fill2XLinkHref.ToUTF8()) + 1];
              strcpy(str, Fill2XLinkHref.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Fill2MimeType.ToUTF8()) + 1];
              strcpy(str, Fill2MimeType.ToUTF8());
              xml =
                sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev,
                                str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              if (EnableFill2Replacement == true)
                {
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
                     prev);
                  sqlite3_free(prev);
                  prev = xml;
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
                     prev);
                  sqlite3_free(prev);
                  prev = xml;
                  str = new char[strlen(Fill2ColorReplacement.ToUTF8()) + 1];
                  strcpy(str, Fill2ColorReplacement.ToUTF8());
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
                     prev, str);
                  delete[]str;
                  sqlite3_free(prev);
                  prev = xml;
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                     prev);
                  sqlite3_free(prev);
                  prev = xml;
                }
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicFill>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
          } else
            {
              // using a Solid Color
              str = new char[strlen(Fill2Color.ToUTF8()) + 1];
              strcpy(str, Fill2Color.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
            }
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
             prev, Fill2Opacity);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t</Fill>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (EnableStroke2 == true)
        {
          // Polygon Stroke
          xml = sqlite3_mprintf("%s\t\t\t<Stroke>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          if (HasGraphicStroke2 == true)
            {
              // using an External Graphic
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<GraphicStroke>\r\n\t\t\t\t\t<Graphic>\r\n\t\t\t\t\t\t<ExternalGraphic>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Stroke2XLinkHref.ToUTF8()) + 1];
              strcpy(str, Stroke2XLinkHref.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              str = new char[strlen(Stroke2MimeType.ToUTF8()) + 1];
              strcpy(str, Stroke2MimeType.ToUTF8());
              xml =
                sqlite3_mprintf("%s\t\t\t\t\t\t\t<Format>%s</Format>\r\n", prev,
                                str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              if (EnableStroke2Replacement == true)
                {
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t<ColorReplacement>\r\n\t\t\t\t\t\t\t\t<Recode fallbackValue=\"#000000\">\r\n",
                     prev);
                  sqlite3_free(prev);
                  prev = xml;
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t\t\t<LookupValue>ExternalGraphic</LookupValue>\r\n\t\t\t\t\t\t\t\t\t<MapItem>\r\n",
                     prev);
                  sqlite3_free(prev);
                  prev = xml;
                  str = new char[strlen(Stroke2ColorReplacement.ToUTF8()) + 1];
                  strcpy(str, Stroke2ColorReplacement.ToUTF8());
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t\t\t\t<Data>1</Data>\r\n\t\t\t\t\t\t\t\t\t\t<Value>%s</Value>\r\n",
                     prev, str);
                  delete[]str;
                  sqlite3_free(prev);
                  prev = xml;
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t\t\t\t\t\t</MapItem>\r\n\t\t\t\t\t\t\t\t</Recode>\r\n\t\t\t\t\t\t\t</ColorReplacement>\r\n",
                     prev);
                  sqlite3_free(prev);
                  prev = xml;
                }
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t\t\t</ExternalGraphic>\r\n\t\t\t\t\t</Graphic>\r\n\t\t\t\t</GraphicStroke>\r\n",
                 prev);
              sqlite3_free(prev);
              prev = xml;
          } else
            {
              // using a Solid Color
              str = new char[strlen(Stroke2Color.ToUTF8()) + 1];
              strcpy(str, Stroke2Color.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
            }
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
             prev, Stroke2Opacity);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
             prev, Stroke2Width);
          sqlite3_free(prev);
          prev = xml;
          switch (Stroke2LineJoin)
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
          switch (Stroke2LineCap)
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
          if (Stroke2DashCount > 0 && Stroke2DashArray != NULL)
            {
              wxString dash;
              NormalizedDashArray(dash, 1, ' ');
              str = new char[strlen(dash.ToUTF8()) + 1];
              strcpy(str, dash.ToUTF8());
              xml =
                sqlite3_mprintf
                ("%s\t\t\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
                 prev, str);
              delete[]str;
              sqlite3_free(prev);
              prev = xml;
              if (Stroke2DashOffset != 0.0)
                {
                  xml =
                    sqlite3_mprintf
                    ("%s\t\t\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                     prev, Stroke2DashOffset);
                  sqlite3_free(prev);
                  prev = xml;
                }
            }
          xml = sqlite3_mprintf("%s\t\t\t</Stroke>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (DisplacementX2 != 0.0 || DisplacementY2 != 0.0)
        {
          xml = sqlite3_mprintf("%s\t\t\t<Displacement>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
             DisplacementX2);
          sqlite3_free(prev);
          prev = xml;
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
             DisplacementY2);
          sqlite3_free(prev);
          prev = xml;
          xml = sqlite3_mprintf("%s\t\t\t</Displacement>\r\n", prev);
          sqlite3_free(prev);
          prev = xml;
        }
      if (PerpendicularOffset2 != 0.0)
        {
          xml =
            sqlite3_mprintf
            ("%s\t\t\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n",
             prev, PerpendicularOffset2);
          sqlite3_free(prev);
          prev = xml;
        }
      xml = sqlite3_mprintf("%s\t\t</PolygonSymbolizer>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s\t</Rule>\r\n</FeatureTypeStyle>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

char *SimplePolygonSymbolizerDialog::DoCreateSymbolizerXML()
{
//
// creating the SLD/SE (XML) code - LineSymbolizer
//
  char *str;
  const char *cstr;
  char *prev;
  char *xml = sqlite3_mprintf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
  prev = xml;
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
  if (EnableFill1 == true)
    {
      // Polygon Fill
      xml = sqlite3_mprintf("%s\t<Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (HasGraphicFill1 == true)
        {
          // using an External Graphic
          xml =
            sqlite3_mprintf
            ("%s\t\t<GraphicFill>\r\n\t\t\t<Graphic>\r\n\t\t\t\t<ExternalGraphic>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Fill1XLinkHref.ToUTF8()) + 1];
          strcpy(str, Fill1XLinkHref.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Fill1MimeType.ToUTF8()) + 1];
          strcpy(str, Fill1MimeType.ToUTF8());
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t<Format>%s</Format>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (EnableFill1Replacement == true)
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
              str = new char[strlen(Fill1ColorReplacement.ToUTF8()) + 1];
              strcpy(str, Fill1ColorReplacement.ToUTF8());
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
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t</ExternalGraphic>\r\n\t\t\t</Graphic>\r\n\t\t</GraphicFill>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a Solid Color
          str = new char[strlen(Fill1Color.ToUTF8()) + 1];
          strcpy(str, Fill1Color.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t<SvgParameter name=\"fill\">%s</SvgParameter>\r\n", prev,
             str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t<SvgParameter name=\"fill-opacity\">%1.2f</SvgParameter>\r\n",
         prev, Fill1Opacity);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t</Fill>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (EnableStroke1 == true)
    {
      // Polygon Stroke
      xml = sqlite3_mprintf("%s\t<Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      if (HasGraphicStroke1 == true)
        {
          // using an External Graphic
          xml =
            sqlite3_mprintf
            ("%s\t\t<GraphicStroke>\r\n\t\t\t<Graphic>\r\n\t\t\t\t<ExternalGraphic>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke1XLinkHref.ToUTF8()) + 1];
          strcpy(str, Stroke1XLinkHref.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t\t<OnlineResource xlink:type=\"simple\" xlink:href=\"%s\" />\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          str = new char[strlen(Stroke1MimeType.ToUTF8()) + 1];
          strcpy(str, Stroke1MimeType.ToUTF8());
          xml =
            sqlite3_mprintf("%s\t\t\t\t\t<Format>%s</Format>\r\n", prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (EnableStroke1Replacement == true)
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
              str = new char[strlen(Stroke1ColorReplacement.ToUTF8()) + 1];
              strcpy(str, Stroke1ColorReplacement.ToUTF8());
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
          xml =
            sqlite3_mprintf
            ("%s\t\t\t\t</ExternalGraphic>\r\n\t\t\t</Graphic>\r\n\t\t</GraphicStroke>\r\n",
             prev);
          sqlite3_free(prev);
          prev = xml;
      } else
        {
          // using a Solid Color
          str = new char[strlen(Stroke1Color.ToUTF8()) + 1];
          strcpy(str, Stroke1Color.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t<SvgParameter name=\"stroke\">%s</SvgParameter>\r\n", prev,
             str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
        }
      xml =
        sqlite3_mprintf
        ("%s\t\t<SvgParameter name=\"stroke-opacity\">%1.2f</SvgParameter>\r\n",
         prev, Stroke1Opacity);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf
        ("%s\t\t<SvgParameter name=\"stroke-width\">%1.2f</SvgParameter>\r\n",
         prev, Stroke1Width);
      sqlite3_free(prev);
      prev = xml;
      switch (Stroke1LineJoin)
        {
          case RL2_PEN_JOIN_MITER:
            xml =
              sqlite3_mprintf
              ("%s\t\t<SvgParameter name=\"stroke-linejoin\">mitre</SvgParameter>\r\n",
               prev);
            sqlite3_free(prev);
            prev = xml;
            break;
          case RL2_PEN_JOIN_BEVEL:
            xml =
              sqlite3_mprintf
              ("%s\t\t<SvgParameter name=\"stroke-linejoin\">bevel</SvgParameter>\r\n",
               prev);
            sqlite3_free(prev);
            prev = xml;
            break;
          default:
            xml =
              sqlite3_mprintf
              ("%s\t\t<SvgParameter name=\"stroke-linejoin\">round</SvgParameter>\r\n",
               prev);
            sqlite3_free(prev);
            prev = xml;
            break;
        };
      switch (Stroke1LineCap)
        {
          case RL2_PEN_CAP_BUTT:
            xml =
              sqlite3_mprintf
              ("%s\t\t<SvgParameter name=\"stroke-linecap\">butt</SvgParameter>\r\n",
               prev);
            sqlite3_free(prev);
            prev = xml;
            break;
          case RL2_PEN_CAP_SQUARE:
            xml =
              sqlite3_mprintf
              ("%s\t\t<SvgParameter name=\"stroke-linecap\">square</SvgParameter>\r\n",
               prev);
            sqlite3_free(prev);
            prev = xml;
            break;
          default:
            xml =
              sqlite3_mprintf
              ("%s\t\t<SvgParameter name=\"stroke-linecap\">round</SvgParameter>\r\n",
               prev);
            sqlite3_free(prev);
            prev = xml;
            break;
        };
      if (Stroke1DashCount > 0 && Stroke1DashArray != NULL)
        {
          wxString dash;
          NormalizedDashArray(dash, 0, ' ');
          str = new char[strlen(dash.ToUTF8()) + 1];
          strcpy(str, dash.ToUTF8());
          xml =
            sqlite3_mprintf
            ("%s\t\t<SvgParameter name=\"stroke-dasharray\">%s</SvgParameter>\r\n",
             prev, str);
          delete[]str;
          sqlite3_free(prev);
          prev = xml;
          if (Stroke1DashOffset != 0.0)
            {
              xml =
                sqlite3_mprintf
                ("%s\t\t<SvgParameter name=\"stroke-dashoffset\">%1.2f</SvgParameter>\r\n",
                 prev, Stroke1DashOffset);
              sqlite3_free(prev);
              prev = xml;
            }
        }
      xml = sqlite3_mprintf("%s\t</Stroke>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (DisplacementX1 != 0.0 || DisplacementY1 != 0.0)
    {
      xml = sqlite3_mprintf("%s\t<Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<DisplacementX>%1.4f</DisplacementX>\r\n", prev,
                        DisplacementX1);
      sqlite3_free(prev);
      prev = xml;
      xml =
        sqlite3_mprintf("%s\t\t<DisplacementY>%1.4f</DisplacementY>\r\n", prev,
                        DisplacementY1);
      sqlite3_free(prev);
      prev = xml;
      xml = sqlite3_mprintf("%s\t</Displacement>\r\n", prev);
      sqlite3_free(prev);
      prev = xml;
    }
  if (PerpendicularOffset1 != 0.0)
    {
      xml =
        sqlite3_mprintf
        ("%s\t<PerpendicularOffset>%1.4f</PerpendicularOffset>\r\n", prev,
         PerpendicularOffset1);
      sqlite3_free(prev);
      prev = xml;
    }
  xml = sqlite3_mprintf("%s</PolygonSymbolizer>\r\n", prev);
  sqlite3_free(prev);
  return xml;
}

void SimplePolygonSymbolizerDialog::OnInsert(wxCommandEvent & WXUNUSED(event))
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
        RetrieveFill1Page();
        break;
      case 2:
        RetrieveStroke1Page();
        break;
      case 3:
        RetrieveFill2Page();
        break;
      case 4:
        RetrieveStroke2Page();
        break;
      case 5:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml;
  if (MinScale == true || MaxScale == true || EnablePolygon2 == true)
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

void SimplePolygonSymbolizerDialog::OnExport(wxCommandEvent & WXUNUSED(event))
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
        RetrieveFill1Page();
        break;
      case 2:
        RetrieveStroke1Page();
        break;
      case 3:
        RetrieveFill2Page();
        break;
      case 4:
        RetrieveStroke2Page();
        break;
      case 5:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  wxFileDialog fileDialog(this,
                          wxT
                          ("Exporting an SLD/SE PolygonSymbolizer to a file"),
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
          if (MinScale == true || MaxScale == true || EnablePolygon2 == true)
            xml = DoCreateFeatureTypeXML();
          else
            xml = DoCreateSymbolizerXML();
          fwrite(xml, 1, strlen(xml), out);
          sqlite3_free(xml);
          fclose(out);
          wxMessageBox(wxT
                       ("SLD/SE PolygonSymbolizer successfully saved into:\n\n\"")
                       + path + wxT("\""), wxT("spatialite_gui"),
                       wxOK | wxICON_INFORMATION, this);
        }
    }
  wxDialog::EndModal(wxID_OK);
}

void SimplePolygonSymbolizerDialog::OnCopy(wxCommandEvent & WXUNUSED(event))
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
        RetrieveFill1Page();
        break;
      case 2:
        RetrieveStroke1Page();
        break;
      case 3:
        RetrieveFill2Page();
        break;
      case 4:
        RetrieveStroke2Page();
        break;
      case 5:
        RetrievePreviewPage();
        break;
    };
  if (FinalValidityCheck() == false)
    {
      GetBookCtrl()->ChangeSelection(0);
      return;
    }
  char *xml;
  if (MinScale == true || MaxScale == true || EnablePolygon2 == true)
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

void SimplePolygonSymbolizerDialog::OnQuit(wxCommandEvent & WXUNUSED(event))
{
//
// all done: 
//
  wxDialog::EndModal(wxID_OK);
}
